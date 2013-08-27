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
 * WidgetWithTextTest.h
 *
 *  Created on: Mar 16, 2011
 *      Author: fmattias
 */

#ifndef WIDGET_WITH_TEXT_TEST_H_
#define WIDGET_WITH_TEXT_TEST_H_

#include <ma.h>
#include <IX_WIDGET.h>

#include "WidgetTest.h"

/**
 * This test tests the properties that are common
 * to widgets that contains text.
 */
class WidgetWithTextTest
: public WidgetTest
{
public:
	WidgetWithTextTest(const char *type, const char *title);

	/**
	 * Tests the get text property o
	 *
	 * @param textProperty Name of the text property, different widgets can have different
	 *                     names for it, e.g. MAW_LABEL_TEXT and MAW_BUTTON_TEXT.
	 */
	void testTextProperty(const char *textProperty);
};

#endif /* WIDGET_WITH_TEXT_TEST_H_ */
