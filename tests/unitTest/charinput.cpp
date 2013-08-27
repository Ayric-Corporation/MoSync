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

#include <conprint.h>
#include "common.h"
#include <MAUtil/CharInput.h>

class CharInputTest : public CharInputBaseCase, public KeyListener
{
public:
	CharInputTest(String testMatch, const String& name) : CharInputBaseCase(name), matchString(testMatch) {}

	void start()
	{
		currentLength = 0;
		MAUtil::Environment::getEnvironment().addKeyListener(this);
		clearScreen();
		renderCurrent();
		maUpdateScreen();
	}

	int renderCurrent()
	{
		int h = 1;
		MAExtent e = maGetScrSize();
		Dimensions screen;
		screen.width = EXTENT_X(e);
		screen.height = EXTENT_Y(e);

		maSetClipRect(0, 0, screen.width, screen.height);
		maSetColor(BLACK);
		maFillRect(0, 0, screen.width, screen.height);

		maSetColor(0xffffff);
		sprintf(buff, "Write the following string");
		mExtent = maGetTextSize(buff);


		maDrawText(1,h,buff);
		h+= EXTENT_Y(mExtent);

		sprintf(buff, "%s", matchString.c_str());
		mExtent = maGetTextSize(buff);

		maDrawText(1,h,buff);
		h+=EXTENT_Y(mExtent);

		sprintf(buff, "end");
		mExtent = maGetTextSize(buff);
		maDrawText(screen.width - EXTENT_X(mExtent),screen.height-EXTENT_Y(mExtent),buff);

		return h;
	}

	void renderCursor(int h, int w)
	{
		maLine(w, h, w+8, h);
		maUpdateScreen();
	}

	void characterChanged(char c)
	{
		int h = renderCurrent();
		currentString[currentLength] = c;
		currentString[currentLength+1] = 0;

		sprintf(buff, "%s", currentString);
		mExtent = maGetTextSize(buff);
		maDrawText(1,h,buff);
		h+=EXTENT_Y(mExtent);
		renderCursor(h, EXTENT_X(mExtent)-8);
	}

	void characterDeployed(char c)
	{
		int h = renderCurrent();
		currentString[currentLength] = c;
		currentString[currentLength+1] = 0;

		if(currentLength<10)currentLength++;

		sprintf(buff, "%s", currentString);
		mExtent = maGetTextSize(buff);
		maDrawText(1,h,buff);
		h+=EXTENT_Y(mExtent);
		renderCursor(h, EXTENT_X(mExtent));
	}

	void keyPressEvent(int keyCode) {}

	void keyReleaseEvent(int keyCode) {
		if(keyCode == MAK_CLEAR)
		{
			if(currentLength!=0)currentLength--;
			currentString[currentLength] = 0;

			int h = renderCurrent();
			sprintf(buff, "%s", currentString);
			mExtent = maGetTextSize(buff);
			maDrawText(1,h,buff);
			h+=EXTENT_Y(mExtent);
			renderCursor(h, EXTENT_X(mExtent));
		}
		if(keyCode == MAK_SOFTRIGHT)
		{
			const char* ms = matchString.c_str();
			if(strcmp(ms, currentString) == 0)
				assert(name, true);
			else
				assert(name, false);

			MAUtil::Environment::getEnvironment().removeKeyListener(this);
			//printf("charInput test finished\n");
			suite->runNextCase();
		}
	}

private:

	char buff[100];
	int currentLength;
	char currentString[12];
	String matchString;
	MAExtent mExtent;
};

void addCharInputTests(TestSuite* suite);
void addCharInputTests(TestSuite* suite) {
	suite->addTestCase(new CharInputTest("mobile", "CharInput"));
}
