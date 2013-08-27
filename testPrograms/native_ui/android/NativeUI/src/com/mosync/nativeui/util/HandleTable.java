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

package com.mosync.nativeui.util;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

/**
 * A table that maps an integer to a Object.
 * 
 * @author fmattias
 */
public class HandleTable<T>
{
	/**
	 * Represents the mapping between an integer handle and
	 * an object.
	 */
	private Map<Integer, T> m_handles = new HashMap<Integer, T>( );
	
	/**
	 * A list of handles that has been used but are now free.
	 */
	private Queue<Integer> m_freeHandles = new LinkedList<Integer>( );
	
	/**
	 * The id of the next handle.
	 */
	private int m_nextHandle = 0;
	
	/**
	 * Adds a view to the widget table and returns
	 * a handle to it.
	 * 
	 * @param view The view that will be added.
	 * @return A positive integer that represents the widget.
	 */
	public int add(T object)
	{
		int newHandle = getNextHandle( );
		add( newHandle, object );
		
		return newHandle;
	}
	
	/**
	 * Adds a handle to an object with a given handle. 
	 * 
	 * @param handle The integer that will be associated
	 *               with the given object.
	 * @param object An object.
	 */
	public boolean add(int handle, T object)
	{
		if( !m_handles.containsKey( handle ) )
		{
			m_handles.put( handle, object );
			m_freeHandles.remove( handle );
			return true;
		}
		else
		{
			return false;
		}
		
	}
	
	/**
	 * Returns the object corresponding to the given handle
	 * but does not remove it.
	 * 
	 * @param handle 
	 * @return The object corresponding to the handle, or null if
	 *         no such handle is in the table.
	 */
	public T get(int handle)
	{
		return m_handles.get( handle );
	}
	
	/**
	 * Removes the given object corresponding to the given
	 * handle from the table.
	 * 
	 * @param handle
	 */
	public void remove(int handle)
	{
		if( m_handles.containsKey( handle ) )
		{
			m_handles.remove( handle );
			m_freeHandles.add( handle );
		}
	}
	
	/**
	 * Returns the next available handle.
	 * 
	 * @return The next available handle.
	 */
	private int getNextHandle()
	{
		if( m_freeHandles.size( ) > 0 )
		{
			return m_freeHandles.poll( );
		}
		else
		{
			return m_nextHandle++;
		}
	}

}
