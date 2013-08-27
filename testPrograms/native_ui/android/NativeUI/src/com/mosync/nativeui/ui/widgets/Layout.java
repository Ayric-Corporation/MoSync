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

package com.mosync.nativeui.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import com.mosync.nativeui.util.LayoutParamsSetter;

import android.view.ViewGroup;

/**
 * This subclass of Widget represents the behavior of a Widget
 * that can layout other widgets.
 * 
 * @author fmattias
 */
public class Layout extends Widget
{
	/**
	 * A list of the children of this widget. This information
	 * is perhaps needed if we destroy the layout before
	 * destroying all of the children.
	 */
	List<Widget> m_children = new ArrayList<Widget>( );

	/**
	 * Constructor.
	 * 
	 * @param handle Handle to this layout.
	 * @param view The layout that is wrapped.
	 */
	public Layout(int handle, ViewGroup view)
	{
		super(handle, view);
	}
	
	/**
	 * Adds a child to this layout, by default the view
	 * of the child is added to the wrapped ViewGroup.
	 * 
	 * The child's parent will be set to this layout.
	 * 
	 * @param child The child of to be added.
	 */
	public void addChild(Widget child)
	{
		child.setParent( this );
		m_children.add( child );

		// Set layout params for the child
		ViewGroup.LayoutParams nativeLayoutParams = createNativeLayoutParams( child.getLayoutParams( ) );
		LayoutParamsSetter.setPossibleParams( child.getLayoutParams( ), nativeLayoutParams );
		child.getView( ).setLayoutParams( nativeLayoutParams );
		
		// Add child to layout
		ViewGroup layout = getView( );
		layout.addView( child.getView( ) );
	}
	
	/**
	 * Removes a child form this layout, by default
	 * the view of the child is removed from the wrapped
	 * ViewGroup.
	 * 
	 * The child's parent will be set to null.
	 * 
	 * @param child
	 */
	public void removeChild(Widget child)
	{
		child.setParent( null );
		m_children.remove( child );
		ViewGroup layout = getView( );
		layout.removeView( child.getView( ) );
	}
	
	/**
	 * Updates the layout params for the given child
	 * to those suitable for this layout.
	 * 
	 * @param mosyncLayoutParams The mosync layout params to create a specific layout params for.
	 */
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new ViewGroup.LayoutParams( mosyncLayoutParams.getWidth( ), mosyncLayoutParams.getHeight( ) );
	}
	
	/**
	 * @see Widget.isLayout.
	 */
	public boolean isLayout()
	{
		return true;
	}
	
	/**
	 * @see Widget.getView.
	 */
	@Override
	public ViewGroup getView()
	{
		// ViewGroup is a covariant return type to View
		return (ViewGroup) super.getView( );
	}
}
