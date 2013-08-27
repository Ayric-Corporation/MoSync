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

/**
 * @file WebViewMultiViews.cpp
 * @author Mikael Kindborg
 *
 * Application for testing multiple WebView widgets.
 *
 * TODO: This program uses hard coded layout sizes to work
 * on iOS. Fix this when iOS layout bugs are fixed.
 */

#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAUtil/String.h>
#include <IX_WIDGET.h>
#include "MAHeaders.h"

#include "WebViewUtil.h"

using namespace MoSync;

class WebViewMultiViewsApp
{
private:
	MAWidgetHandle mScreen;
	MAWidgetHandle mWebViews[4];
	MAWidgetHandle mButtonAllWhite;
	MAWidgetHandle mButtonAllBlack;
	Platform* mPlatform;

public:
	WebViewMultiViewsApp()
	{
		createUI();
	}

	virtual ~WebViewMultiViewsApp()
	{
		destroyUI();
	}

	void createUI()
	{
		mPlatform = Platform::create();

		// Create screen.
		mScreen = maWidgetCreate(MAW_SCREEN);
		widgetShouldBeValid(mScreen, "Could not create screen");

		// Create layouts.
		MAWidgetHandle verticalLayout = createLayout(MAW_VERTICAL_LAYOUT, "-1", "-1");
		MAWidgetHandle horizontalLayoutOne = createLayout(MAW_HORIZONTAL_LAYOUT, "-1", "200");
		MAWidgetHandle horizontalLayoutTwo = createLayout(MAW_HORIZONTAL_LAYOUT, "-1", "200");
		MAWidgetHandle horizontalLayoutThree = createLayout(MAW_HORIZONTAL_LAYOUT, "-1", "80");

		// Shrink wrap last layout, which will be used for buttons.
		maWidgetSetProperty(horizontalLayoutThree, "height", "80");

		// Create web views.
		mWebViews[0] = createWebView();
		mWebViews[1] = createWebView();
		mWebViews[2] = createWebView();
		mWebViews[3] = createWebView();

		mButtonAllWhite = createButton("All White");
		mButtonAllBlack = createButton("All Black");

		// Compose objects.
		maWidgetAddChild(verticalLayout, horizontalLayoutOne);
		maWidgetAddChild(verticalLayout, horizontalLayoutTwo);
		maWidgetAddChild(verticalLayout, horizontalLayoutThree);
		maWidgetAddChild(horizontalLayoutOne, mWebViews[0]);
		maWidgetAddChild(horizontalLayoutOne, mWebViews[1]);
		maWidgetAddChild(horizontalLayoutTwo, mWebViews[2]);
		maWidgetAddChild(horizontalLayoutTwo, mWebViews[3]);
		maWidgetAddChild(horizontalLayoutThree, mButtonAllWhite);
		maWidgetAddChild(horizontalLayoutThree, mButtonAllBlack);
		maWidgetAddChild(mScreen, verticalLayout);

		// Show the screen.
		maWidgetScreenShow(mScreen);
	}

	MAWidgetHandle createWebView()
	{
		// Create web view.
		MAWidgetHandle webView = maWidgetCreate(MAW_WEB_VIEW);
		widgetShouldBeValid(webView, "Could not create web view");

		// Set size of the web view to fill the parent.
		maWidgetSetProperty(webView, "width", "-1");
		maWidgetSetProperty(webView, "height", "-1");

		// Method 1: Directly html property of WebView.

//		MAUtil::String html =
//			mPlatform->createTextFromHandle(ColorPage_html);
//		maWidgetSetProperty(webView, "html", html.c_str());

		// Method 2: Write html file to local file system.

		// Write HTML resource files to the local file system
		// so they can be accessed by the web view.
		// TODO: This can be removed once automatic resource
		// unpacking of file systems is supported.

		mPlatform->writeTextToFile(
			mPlatform->getLocalPath() + "ColorPage.html",
			mPlatform->createTextFromHandle(ColorPage_html));

		// As a test, create a second page (linked to from ColorPage.html).
		mPlatform->writeTextToFile(
			mPlatform->getLocalPath() + "AnotherPage.html",
			"<html><body><p><a href=\"ColorPage.html\">Hello World</a></p></body></html>");

		// Set the URL the web view displays.
		// We support both absolute file url and
		// as url that assumes a url base is set.
		maWidgetSetProperty(webView, "url", "ColorPage.html");

		// Register to receive messages from the WebView.
		WebViewMessage::getMessagesFor(webView);

		return webView;
	}

	MAWidgetHandle createButton(const MAUtil::String& text)
	{
		// Create button.
		MAWidgetHandle button = maWidgetCreate(MAW_BUTTON);
		widgetShouldBeValid(button, "Could not create button");

		// Set button text.
		maWidgetSetProperty(button, "text", text.c_str());

		// Set size of the button to fill the parent horizontally
		// and to wrap content vertically.
		maWidgetSetProperty(button, "width", "-1");
		maWidgetSetProperty(button, "height", "-2");

		return button;
	}

	MAWidgetHandle createLayout(const char* type, const char * width, const char * height)
	{
		// Create web view
		MAWidgetHandle layout = maWidgetCreate(type);
		widgetShouldBeValid(layout, "Could not create layout");

		// Set size of the layout to fill the parent.
		maWidgetSetProperty(layout, "width", width);
		maWidgetSetProperty(layout, "height", height);

		return layout;
	}

	void destroyUI()
	{
		maWidgetDestroy(mScreen);
		delete mPlatform;
	}

	void runEventLoop()
	{
		MAEvent event;

		bool isRunning = true;
		while (isRunning)
		{
			maWait(0);
			maGetEvent(&event);
			switch (event.type)
			{
				case EVENT_TYPE_CLOSE:
					isRunning = false;
					break;

				case EVENT_TYPE_KEY_PRESSED:
					if (event.key == MAK_BACK)
					{
						isRunning = false;
					}
					break;

				case EVENT_TYPE_WIDGET:
					handleWidgetEvent((MAWidgetEventData*) event.data);
					break;
			}
		}
	}

	/**
	 * Change background colors of web views when we get the
	 * OnBackgorundClicked message, and also handle button clicks.
	 */
	void handleWidgetEvent(MAWidgetEventData* widgetEvent)
	{
		// Get handle to the widget.
		MAWidgetHandle widget = widgetEvent->widgetHandle;

		// Handle messages from the WebView widgets.
		if (MAW_EVENT_WEB_VIEW_HOOK_INVOKED == widgetEvent->eventType &&
			MAW_CONSTANT_HARD == widgetEvent->hookType)
		{
			// Get message.
			WebViewMessage message(widgetEvent->urlData);

			// This is the only message we handle.
			if ( ! message.is("OnBackgroundClick") )
			{
				return;
			}

			// This was just annoying! But good as a demo.
			// maVibrate(500);

			if (widget == mWebViews[0])
			{
				setWebViewBackgroundColor(mWebViews[0], "red");
			}
			if (widget == mWebViews[1])
			{
				setWebViewBackgroundColor(mWebViews[0], "blue");
				setWebViewBackgroundColor(mWebViews[1], "green");
			}
			if (widget == mWebViews[2])
			{
				setWebViewBackgroundColor(mWebViews[0], "yellow");
				setWebViewBackgroundColor(mWebViews[1], "orange");
				setWebViewBackgroundColor(mWebViews[2], "pink");
			}
			if (widget == mWebViews[3])
			{
				setWebViewBackgroundColor(mWebViews[0], "black");
				setWebViewBackgroundColor(mWebViews[1], "white");
				setWebViewBackgroundColor(mWebViews[2], "white");
				setWebViewBackgroundColor(mWebViews[3], "black");
			}
		}

		if (MAW_EVENT_CLICKED == widgetEvent->eventType)
		{
			if (widget == mButtonAllWhite)
			{
				setWebViewBackgroundColor(mWebViews[0], "white");
				setWebViewBackgroundColor(mWebViews[1], "white");
				setWebViewBackgroundColor(mWebViews[2], "white");
				setWebViewBackgroundColor(mWebViews[3], "white");
			}
			if (widget == mButtonAllBlack)
			{
				setWebViewBackgroundColor(mWebViews[0], "black");
				setWebViewBackgroundColor(mWebViews[1], "black");
				setWebViewBackgroundColor(mWebViews[2], "black");
				setWebViewBackgroundColor(mWebViews[3], "black");
			}
		}
	}

	void setWebViewBackgroundColor(
		MAWidgetHandle webView,
		const MAUtil::String& color)
	{
		char script[256];
		sprintf(
			script,
			"javascript:document.bgColor='%s';",
			color.c_str());
		maWidgetSetProperty(webView, "url", script);
	}

	void widgetShouldBeValid(MAWidgetHandle widget, const char* panicMessage)
	{
		if (widget <= 0)
		{
			maPanic(0, panicMessage);
		}
	}
};
// End of class WebViewMultiViews

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	WebViewMultiViewsApp app;
	app.runEventLoop();
	return 0;
}
