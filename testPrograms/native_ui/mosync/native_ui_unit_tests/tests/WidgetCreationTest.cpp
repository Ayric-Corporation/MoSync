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

/*
 * WidgetCreationTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#include <Testify/testify.hpp>

#include <ma.h>

#include <IX_WIDGET.h>

struct widget_test_t
{
	const char *type;
	const char *description;
};

/**
 * List of the widgets to create.
 *
 * Note: Currently description is here because addTest
 * does not make an internal copy of the description argument,
 * so if we generate the description we have to keep track
 * of the memory.
 */
widget_test_t g_widgetsToCreate[] =
{
	{ MAW_BUTTON, "Creating MAW_BUTTON." },
	{ MAW_CHECK_BOX, "Creating MAW_CHECK_BOX." },
	{ MAW_EDIT_BOX, "Creating MAW_EDIT_BOX." },
	{ MAW_GL_VIEW, "Creating MAW_GL_VIEW." },
	{ MAW_HORIZONTAL_LAYOUT, "Creating MAW_HORIZONTAL_LAYOUT." },
	{ MAW_IMAGE, "Creating MAW_IMAGE." },
	{ MAW_LABEL, "Creating MAW_LABEL." },
	{ MAW_LIST_VIEW, "Creating MAW_LIST_VIEW." },
	{ MAW_LIST_VIEW_ITEM, "Creating MAW_LIST_VIEW_ITEM." },
	{ MAW_STACK_SCREEN, "Creating MAW_STACK_SCREEN." },
	{ MAW_RELATIVE_LAYOUT, "Creating MAW_RELATIVE_LAYOUT." },
	{ MAW_SCREEN, "Creating MAW_SCREEN." },
	{ MAW_SEARCH_BAR, "Creating MAW_SEARCH_BAR." },
	{ MAW_TAB_SCREEN, "Creating MAW_TAB_SCREEN." },
	{ MAW_VERTICAL_LAYOUT, "Creating MAW_VERTICAL_LAYOUT." },
	{ MAW_WEB_VIEW, "Creating MAW_WEB_VIEW." },
	{ NULL, NULL} // Marks the end
};

/**
 * The purpose of this test case is to test the functionality
 * related to a screen.
 */
class WidgetCreation
: public Testify::TestCase
{
public:
	WidgetCreation()
	: Testify::TestCase( "Widget Creation" )
	{
		for(int i = 0; g_widgetsToCreate[ i ].type != NULL; i++)
		{
			addTest( bind( &WidgetCreation::createWidget, this, g_widgetsToCreate[ i ].type ),
					g_widgetsToCreate[ i ].description );
		}
	}

	/**
	 * The purpose of this test is to test the creation and destruction
	 * of a screen.
	 */
	void createWidget(const char *type)
	{
		int widgetHandle = maWidgetCreate( type );
		TESTIFY_ASSERT_NOT_EQUAL( widgetHandle, MAW_RES_INVALID_TYPE_NAME );
		TESTIFY_ASSERT( widgetHandle >= 0 );

		int destroyStatus = maWidgetDestroy( widgetHandle );
		TESTIFY_ASSERT_EQUAL( destroyStatus, MAW_RES_OK );
	}
};

static Testify::TestHook hook( new WidgetCreation( ), "Widget Creation" );
