/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.ListLayout;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a list capable of displaying a set of widgets.
 *
 * @author fmattias
 */
public class ListFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		ListView listView = new CustomListView( activity );

		// Use the cache color hint optimization.
		// This makes the background not to turn black while scrolling the list.
		listView.setCacheColorHint(0x00000000);

		listView.setOnItemClickListener( new ListOnItemClickListener( handle ) );
		return new ListLayout( handle, listView );
	}

	/**
	 * Class responsible for sending a mosync event when an item
	 * in the list has been clicked.
	 *
	 * @author fmattias
	 */
	public class ListOnItemClickListener implements OnItemClickListener
	{
		private int m_handle = -1;

		public ListOnItemClickListener(int handle)
		{
			m_handle = handle;
		}

		/**
		 * @see OnItemClickListener.onItemClick.
		 */
		@Override
		public void onItemClick(AdapterView<?> parent, View view, int position, long id)
		{
			// Assume that the view's id is always the same as the handle.
			EventQueue.getDefault( ).postWidgetItemClickedEvent( m_handle, position );
		}
	}

	/**
	 * Nullify the addView and removeView methods in ListView
	 * so that it can be used in a convenient way as a Layout
	 * subclass.
	 *
	 * @author fmattias
	 */
	public static class CustomListView extends ListView
	{
		public CustomListView(Context context)
		{
			super( context );
		}

		@Override
		public void addView(View child, int index)
		{
		}

		@Override
		public void addView(View child)
		{
		}

		@Override
		public void removeView(View child)
		{
		}

		@Override
		public void removeViewAt(int index)
		{
		}
	}
}
