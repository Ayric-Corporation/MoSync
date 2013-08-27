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

package com.mosync.nativeui.util.properties;

import java.util.HashMap;

import android.view.Gravity;

import com.mosync.nativeui.core.Types;

public class VerticalAlignment
{
	/**
	 * Map from mosync string property to android int gravity.
	 */
	public static HashMap<String, Integer> m_mosyncVertical = new HashMap<String, Integer>( );
	
	static
	{	
		m_mosyncVertical.put( Types.WIDGET_VERTICAL_TOP, Gravity.TOP );
		m_mosyncVertical.put( Types.WIDGET_VERTICAL_CENTER, Gravity.CENTER_VERTICAL );
		m_mosyncVertical.put( Types.WIDGET_VERTICAL_BOTTOM, Gravity.BOTTOM );
	}
	
	/**
	 * Convert the given vertical alignment property to a
	 * native android gravity.
	 * 
	 * @param value A mosync vertical alignment property.
	 * @return the converted android gravity.
	 * @throws PropertyConversionException if the given property could not be converted.
	 */
	public static int convert(String value) throws PropertyConversionException
	{
		Integer androidGravity = m_mosyncVertical.get( value );
		
		if( androidGravity != null)
		{
			return androidGravity;
		}
		else
		{
			throw new PropertyConversionException( value );
		}
	}
}
