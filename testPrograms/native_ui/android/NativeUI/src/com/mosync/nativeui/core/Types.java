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

/**
 * Simple class for accessing the constants related
 * to different types.
 * 
 * @author fmattias
 */
public class Types
{
	public static final int WIDGET_ROOT = -100;
	
	public static final String WIDGET_TYPE_BUTTON = "Button";
	public static final String WIDGET_TYPE_LABEL = "Label";
	public static final String WIDGET_TYPE_LIST = "ListBox";
	public static final String WIDGET_TYPE_LAYOUT_VERTICAL = "VerticalLayout";
	public static final String WIDGET_TYPE_LAYOUT_HORIZONTAL = "HorizontalLayout";
	
	public static final String WIDGET_PROPERTY_WIDTH = "width";
	public static final String WIDGET_PROPERTY_HEIGHT = "height";
	
	public static final String WIDGET_PROPERTY_PADDING_LEFT = "left";
	public static final String WIDGET_PROPERTY_PADDING_TOP = "top";
	public static final String WIDGET_PROPERTY_PADDING_RIGHT = "right";
	public static final String WIDGET_PROPERTY_PADDING_BOTTOM = "bottom";
	
	public static final String WIDGET_PROPERTY_HALIGNMENT = "halignment";
	public static final String WIDGET_PROPERTY_VALIGNMENT = "valignment";
	public static final String WIDGET_PROPERTY_TEXT = "text";
	
	public static final String WIDGET_PROPERTY_BACKGROUND_COLOR = "backgroundColor";
	public static final String WIDGET_PROPERTY_FONT_COLOR = "fontColor";
	
	public static final String WIDGET_HORIZONTAL_LEFT = "left";
	public static final String WIDGET_HORIZONTAL_CENTER = "center";
	public static final String WIDGET_HORIZONTAL_RIGHT = "right";
	
	public static final String WIDGET_VERTICAL_TOP = "top";
	public static final String WIDGET_VERTICAL_CENTER = "center";
	public static final String WIDGET_VERTICAL_BOTTOM = "bottom";
	
	public static final int WIDGET_OK = 0;
	public static final int WIDGET_ERROR = -1;
}
