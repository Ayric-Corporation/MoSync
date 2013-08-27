/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package com.mosync.nativeui.ui.egl;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLSurface;

import android.content.Context;
import android.graphics.PixelFormat;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * This view acts as a placeholder for an OpenGL surface
 * by extending a SurfaceView which is a general placeholder
 * for any kind of surface.
 *
 * The EGLView is fairly similar to the GLSurfaceView with
 * the exception that its rendered content is provided by
 * the MoSync user, rather than from a separate rendering
 * thread. The reason for this design is:
 *
 * Using the GLSurfaceView we cannot bind the EGLContext to
 * the MoSync interpreter thread that issues the OpenGL calls.
 *
 * @author fmattias
 */
public class EGLView extends SurfaceView implements SurfaceHolder.Callback
{
	/**
	 * Reference to the holder and controller of this surface.
	 */
	private SurfaceHolder m_holder = null;

	/**
	 * Holds the EGL information necessary to draw onto an
	 */
	private EGLState m_eglState;


	/**
	 * Listener for egl view ready listener.
	 */
	private EGLViewReadyListener m_readyListener = null;

	/**
	 * Constructor.
	 *
	 * @param context Context in which the view is created.
	 */
	public EGLView(Context context, int glApi)
	{
		super( context );

		m_eglState = new EGLState(glApi);
		m_holder = getHolder( );
		m_holder.addCallback( this );
		m_holder.setType( SurfaceHolder.SURFACE_TYPE_GPU );
		m_holder.setFormat( PixelFormat.RGBA_8888 );
	}

	/**
	 * Binds the context of this view to the thread calling
	 * this function.
	 */
	public void bind()
	{
		m_eglState.bind( );
	}

	/**
	 * Makes the surface ready to be drawn on, by binding
	 * it to the current EGL context.
	 *
	 * Note: You must call the finishRender function to
	 * draw the surface on the screen.
	 */
	public void enterRender()
	{
		m_eglState.prepareForRender( );
	}

	/**
	 * Draws the open GL surface on the screen, and releases
	 * it from rendering mode.
	 */
	public void finishRender()
	{
		m_eglState.finishRender( );
	}

	/**
	 * Sets a listener that notifies when the EGL view has been created
	 * and is ready to be drawn.
	 *
	 * @param readyListener The listener to call.
	 */
	public void setEglViewReadyListener(EGLViewReadyListener readyListener)
	{
		m_readyListener = readyListener;
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height)
	{
		// Create a new EGL surface
		Log.i( "EGLView", "New surface width: " + width + " height: " + height );
		m_eglState.newSurface( holder, width, height );
		if( m_readyListener != null )
		{
			m_readyListener.eglViewReady( );
		}
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder)
	{
		// Once in here you are allowed to start rendering
		Log.i( "EGLView", "Surface created" );
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder)
	{
		// The surface is destroyed, further rendering is prevented
		m_eglState.destroySurface( );
		Log.i( "EGLView", "Surface destroyed" );
	}

	/**
	 * This class is responsible for holding and updating
	 * an EGL context and its surface, as the state of
	 * the surface view changes.
	 *
	 * @author fmattias
	 */
	public class EGLState
	{
		/**
		 * The configuration of the context.
		 */
		private EGLConfig m_config = null;

		/**
		 * The current context.
		 */
		private EGLContext m_context = null;

		/**
		 * The active surface.
		 */
		private EGLSurface m_surface = null;

		/**
		 * Access to EGL functions.
		 */
		private EGL10 m_egl = (EGL10) EGLContext.getEGL( );

		/**
		 * Workaround for eglGetCurrentSurface that is not implemented
		 * correctly on android. It destroys the context or crashes.
		 */
		private boolean m_surfaceIsCurrent = false;

		/**
		 * This lock and condition guards the surface from being destroyed
		 * while changing the EGLContext.
		 */
		private final Lock m_surfaceLock = new ReentrantLock( );

		private int m_glApi;

		public EGLState(int glApi)
		{
			m_glApi = glApi;
			EGLManager.initEgl( );
			m_config = EGLConfigFactory.findConfig( m_egl, EGLManager.getDisplay( ), m_glApi);
		}

		/**
		 * This function attaches the egl context to the current thread,
		 * the first time it is called. Subsequent calls just returns.
		 */
		public void bind()
		{
			if( m_context != null )
			{
				return;
			}

			/*
			 * Create an OpenGL ES context. This must be done only once, an
			 * OpenGL context is a somewhat heavy object.
			 */
			m_context = EGLContextFactory.createContext( m_egl, m_config, EGLManager.getDisplay( ), m_glApi);
			int eglError = m_egl.eglGetError( );
			if( m_context == null || m_context == EGL10.EGL_NO_CONTEXT || eglError != EGL10.EGL_SUCCESS )
			{
				throw new RuntimeException( "Could not create context. EGL Error: " + eglError );
			}
		}

		/**
		 * Makes the surface ready to be drawn on, by binding
		 * it to the current EGL context.
		 *
		 * @return The surface that can be drawn on.
		 */
		public void prepareForRender()
		{
			if( m_surface == null )
			{
				return;
			}

			/**
			 * If our surface is already set up, do nothing.
			 */
			if( surfaceIsCurrent( ) )
			{
				return;
			}

			/*
			 * Before we can issue GL commands, we need to make sure the context
			 * is current and bound to a surface.
			 */
			m_surfaceLock.lock( );
			if( !m_egl.eglMakeCurrent( EGLManager.getDisplay( ), m_surface,
					m_surface, m_context ) )
			{
				m_surfaceLock.unlock( );
				throw new RuntimeException(
						"Failed to bind surface to current context. EGL Error: " + m_egl.eglGetError( ) );
			}
			setSurfaceCurrent( true );
			m_surfaceLock.unlock( );
		}

		/**
		 * Draws the open GL surface on the screen, and makes it possible
		 * to change the underlying SurfaceView again.
		 */
		public void finishRender()
		{
			if( m_surface == null )
			{
				return;
			}

			m_surfaceLock.lock( );
			// Wait for pending GL commands to finish and draw the surface
			// onto the screen.
			m_egl.eglWaitGL();

			m_egl.eglSwapBuffers( EGLManager.getDisplay( ), m_surface );
			int eglError = m_egl.eglGetError( );
			if( eglError != EGL10.EGL_SUCCESS )
			{
				Log.i( "EGLView", "Error while swapping buffers. EGL Error: " + eglError );
			}

			m_surfaceLock.unlock( );
		}

		/**
		 * Creates a new surface associated with this EGL state.
		 *
		 * @param holder Reference to the holder and controller of the SurfaceView
		 *               that this EGLState is bound to.
		 * @param width Width of the surface.
		 * @param height Height of the surface.
		 */
		public void newSurface(SurfaceHolder holder, int width, int height)
		{
			m_surfaceLock.lock( );

			/*
			 * The window size has changed, so we need to create a new surface.
			 */
			if( m_surface != null && m_surface != EGL10.EGL_NO_SURFACE )
			{
				/*
				 * Unbind and destroy the old EGL surface, if there is one.
				 */
				destroySurface( );
			}

			/*
			 * Create an EGL surface we can render into.
			 */
			m_surface = EGLSurfaceFactory.createSurface( m_egl, m_config, EGLManager.getDisplay( ), holder);
			if( m_surface == null || m_surface == EGL10.EGL_NO_SURFACE || m_egl.eglGetError( ) != EGL10.EGL_SUCCESS )
			{
				Log.i( "EGLView", "Failed to create surface. EGL Error: " + m_egl.eglGetError( ) );
				m_surfaceLock.unlock( );
				throw new RuntimeException( "Failed to create surface." );
			}

			setSurfaceCurrent( false );

			m_surfaceLock.unlock( );
		}

		/**
		 * Detaches the current surface from the context,
		 * and destroys the surface.
		 */
		public void destroySurface()
		{
			m_surfaceLock.lock( );

			m_egl.eglMakeCurrent( EGLManager.getDisplay( ), EGL10.EGL_NO_SURFACE,
					EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT );
			EGLSurfaceFactory.destroySurface( m_egl, EGLManager.getDisplay( ),
					m_surface );
			m_surface = null;
			setSurfaceCurrent( false );

			m_surfaceLock.unlock( );
		}

		/**
		 * Returns true if the surface is current (by makeCurrent).
		 *
		 * @return true if the surface is current (by makeCurrent),
		 *         false otherwise.
		 */
		private boolean surfaceIsCurrent()
		{
			return m_surfaceIsCurrent;
		}

		/**
		 * The surface is marked as current.
		 *
		 * @param surfaceIsCurrent
		 */
		private void setSurfaceCurrent(boolean surfaceIsCurrent)
		{
			m_surfaceIsCurrent = surfaceIsCurrent;
		}
	}

	/**
	 * Class responsible for notifying when an EGL surface has
	 * been created.
	 *
	 * @author fmattias
	 */
	public interface EGLViewReadyListener
	{
		/**
		 * Called when the EGL view has created a surface
		 * that can be drawn on.
		 */
		void eglViewReady();
	}
}
