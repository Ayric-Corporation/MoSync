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
 * CheckBoxTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"

static widget_property_test_t g_property_tests[] =
{
	{ MAW_CHECK_BOX_CHECKED, "true", "Testing checked with true.", MAW_RES_OK },
	{ MAW_CHECK_BOX_CHECKED, "false", "Testing checked with false.", MAW_RES_OK },
	{ NULL, NULL, NULL, 0 }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a check box.
 */
class CheckBoxTest
: public WidgetTest
{
public:
	CheckBoxTest()
	: WidgetTest( MAW_CHECK_BOX, "CheckBoxTest" )
	{
		addTest( bind( &CheckBoxTest::testCheckedProperty, this ),
				"Tests get checked property." );

		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

	void testCheckedProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_CHECK_BOX_CHECKED, "false" ), MAW_RES_OK );

		char checkedBuffer[256];
		TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), MAW_CHECK_BOX_CHECKED, checkedBuffer, 256 ), strlen( "false" ) );
		TESTIFY_ASSERT_EQUAL( strcmp( checkedBuffer, "false" ), 0 );

		TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), MAW_CHECK_BOX_CHECKED, checkedBuffer, 1 ), MAW_RES_INVALID_STRING_BUFFER_SIZE );
	}
};

static Testify::TestHook hook( new CheckBoxTest( ), "CheckBoxTest" );
