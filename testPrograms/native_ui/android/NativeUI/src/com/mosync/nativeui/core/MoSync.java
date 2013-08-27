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

package com.mosync.nativeui.core;

import com.mosync.nativeui.ui.factories.ViewFactory;
import com.mosync.nativeui.ui.widgets.Layout;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.content.Context;
import android.util.Log;
import android.view.ViewGroup;

public class MoSync
{
	/**
	 * Context of the main activity.
	 */
	private Context m_context;
	
	/**
	 * A table that contains a mapping between a handle and a widget, in a
	 * mosync program a handle is the only reference to a widget.
	 */
	private HandleTable<Widget> m_widgetTable = new HandleTable<Widget>();
	
	/**
	 * 
	 * 
	 * @param context The context that the widgets should be created in.
	 * @param rootWidget The root widget in this context, usually a FrameLayout.
	 */
	public MoSync(Context context, ViewGroup rootWidget)
	{
		m_context = context;
		m_widgetTable.add( Types.WIDGET_ROOT, new Layout( Types.WIDGET_ROOT, rootWidget ) );
	}
	
	public int maWidgetCreate(String type)
	{
		Widget widget = ViewFactory.createView( type, m_context );
		
		if( widget != null )
		{	
			int handle = m_widgetTable.add( widget );
			return handle;
		}
		else
		{
			return Types.WIDGET_ERROR;
		}
	}
	
	public int maWidgetDestroy(int widgetHandle)
	{
		Widget widget = m_widgetTable.get( widgetHandle );
		if( widget != null )
		{
			/**
			 * XXX: Destroy all children?
			 */
			Layout parent = (Layout)  widget.getParent( );
			if( parent != null )
			{
				Widget child = m_widgetTable.get( widgetHandle );
				parent.removeChild( child );
			}
			
			m_widgetTable.remove( widgetHandle );
			return Types.WIDGET_OK;
		}
		else
		{
			return Types.WIDGET_ERROR;
		}
	}
	
	public int maWidgetAdd(int parentHandle, int childHandle)
	{
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null || parent == null )
		{
			return Types.WIDGET_ERROR;
		}

		if( parent.isLayout( ) )
		{
			Layout parentAsLayout = (Layout) parent;
			parentAsLayout.addChild( child );
			return Types.WIDGET_OK;
		}
		else
		{
			return Types.WIDGET_ERROR;
		}
	}
	
	public int maWidgetRemove(int parentHandle, int childHandle)
	{
		if( childHandle == Types.WIDGET_ROOT )
		{
			return Types.WIDGET_ERROR;
		}
		
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null || parent == null )
		{
			return Types.WIDGET_ERROR;
		}
		
		if( parent.isLayout( ) )
		{
			Layout parentAsLayout = (Layout) parent;
			parentAsLayout.removeChild( child );
			return Types.WIDGET_OK;
		}
		else
		{
			return Types.WIDGET_ERROR;
		}
	}
	
	public int maWidgetSetProperty(int widgetHandle, String key, String value)
	{
		Widget widget = m_widgetTable.get( widgetHandle );
		if( widget == null )
		{
			return Types.WIDGET_ERROR;
		}
		
		try
		{
			widget.setProperty( key, value );
			return Types.WIDGET_OK;
		}
		catch(PropertyConversionException pce)
		{
			Log.e( this.getClass( ).getCanonicalName( ) , "Error while converting property: " + pce.getMessage( ), pce );
			return Types.WIDGET_ERROR;
		}
	}
}
