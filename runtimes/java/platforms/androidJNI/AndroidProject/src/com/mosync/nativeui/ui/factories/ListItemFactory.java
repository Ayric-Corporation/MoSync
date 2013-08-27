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

package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.mosync.nativeui.ui.widgets.ListItemWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a list item.
 * 
 * @author fmattias
 */
public class ListItemFactory implements AbstractViewFactory
{	
	@Override
	public Widget create(Activity activity, int handle)
	{
		RelativeLayout listItemLayout = new RelativeLayout( activity );
		listItemLayout.setPadding( 5, 5, 5, 5 );
		
		TextView label = new TextView( activity );
		ImageView icon = new ImageView( activity );
		icon.setId( 100 );
		
		RelativeLayout.LayoutParams labelParams = new RelativeLayout.LayoutParams( RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT );
		labelParams.addRule( RelativeLayout.ALIGN_PARENT_RIGHT );
		labelParams.addRule( RelativeLayout.RIGHT_OF, icon.getId( ) );
		labelParams.addRule( RelativeLayout.CENTER_VERTICAL );
		labelParams.setMargins( 5, 0, 0, 5 );
		label.setLayoutParams( labelParams );
		
		label.setTextSize( 15.0f );
		
		RelativeLayout.LayoutParams iconParams = new RelativeLayout.LayoutParams( RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT );
		iconParams.addRule( RelativeLayout.ALIGN_PARENT_LEFT );
		iconParams.addRule( RelativeLayout.CENTER_VERTICAL );
		icon.setLayoutParams( iconParams );
		
		listItemLayout.addView( icon );
		listItemLayout.addView( label );
		
		return new ListItemWidget( handle, listItemLayout, label, icon );
	}
}
