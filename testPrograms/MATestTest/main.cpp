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

extern "C" {
#include <maapi.h>
}

#include "MAHeaders.h"
#include "MATest/Test.h"
#include <conprint.h>
#include <mastdlib.h>

using namespace MATest;

class GraphicsSyscallsTestCase : public TestCase {

public:
	GraphicsSyscallsTestCase() : TestCase("Graphics Syscalls Test") {
		Extent e = maGetScrSize();
		w = EXTENT_X(e);
		h = EXTENT_Y(e);
	}

	typedef void (GraphicsSyscallsTestCase::*InteractiveTestFunction)();

	int interactiveTest(InteractiveTestFunction testFunction) {
		while(1) {
			int retCode;
			while(retCode=maGetEvent()) {
				if(	retCode==MAK_LEFT ||
					retCode==MAK_RIGHT ||
					retCode==MAK_UP ||
					retCode==MAK_DOWN ||
					retCode==MAK_FIRE ||
					retCode==MAK_0 ||
					retCode==MAK_1 ||
					retCode==MAK_2 ||
					retCode==MAK_3 ||
					retCode==MAK_4 ||
					retCode==MAK_5 ||
					retCode==MAK_6 ||
					retCode==MAK_7 ||
					retCode==MAK_8 ||
					retCode==MAK_9) return retCode;
			}
			
			((*this).*testFunction)();

			maUpdateScreen();
			maWait(WAIT_EVENT);
		}
	}

	void testRedSquareBottomRightQuadrant() {
			maSetColor(0x000000);
			maFillRect(0, 0, w, h);
			maSetColor(0xff0000);
			maFillRect(w>>1, h>>1, w>>1, h>>1);
	}

	void testGreenLineBottomLeftQuadrant() {
			maSetColor(0x000000);
			maFillRect(0, 0, w, h);
			maSetColor(0x00ff00);
			maLine(0, h>>1, w>>1, h);		
	}
	
	void testBlueTextTopRightQuadrant() {
			maSetColor(0x000000);
			maFillRect(0, 0, w, h);
			maSetColor(0x0000ff);
			maDrawText((w>>1)+(w>>2), 2, "mosync");		
	}

	void run() {
		printf("\ndo you see a square?");
		printf("if that is the case press right");
		assert("maFillRect draws a rect", interactiveTest(&GraphicsSyscallsTestCase::testRedSquareBottomRightQuadrant)==MAK_RIGHT);

		printf("\nis the square in the bottom right quadrant?");
		printf("if that is the case press right");
		assert("rect is in bottom right quadrant", interactiveTest(&GraphicsSyscallsTestCase::testRedSquareBottomRightQuadrant)==MAK_RIGHT);

		printf("\nis the square red?");
		printf("if that is the case press right");
		assert("rect is colored red", interactiveTest(&GraphicsSyscallsTestCase::testRedSquareBottomRightQuadrant)==MAK_RIGHT);

		printf("\ndo you see a line?");
		printf("if that is the case press right");
		assert("maLine draws a line", interactiveTest(&GraphicsSyscallsTestCase::testGreenLineBottomLeftQuadrant)==MAK_RIGHT);

		printf("\nis the line in the bottom left quadrant?");
		printf("if that is the case press right");
		assert("line is in bottom left quadrant", interactiveTest(&GraphicsSyscallsTestCase::testGreenLineBottomLeftQuadrant)==MAK_RIGHT);

		printf("\nis the line green?");
		printf("if that is the case press right");
		assert("line is colored green", interactiveTest(&GraphicsSyscallsTestCase::testGreenLineBottomLeftQuadrant)==MAK_RIGHT);

		printf("\ndo you see text?");
		printf("if that is the case press right");
		assert("maDrawText draws a text", interactiveTest(&GraphicsSyscallsTestCase::testBlueTextTopRightQuadrant)==MAK_RIGHT);

		printf("\nis the text in the top right quadrant?");
		printf("if that is the case press right");
		assert("text is in top right quadrant", interactiveTest(&GraphicsSyscallsTestCase::testBlueTextTopRightQuadrant)==MAK_RIGHT);

		printf("\nis the text blue?");
		printf("if that is the case press right");
		assert("text is colored blue", interactiveTest(&GraphicsSyscallsTestCase::testBlueTextTopRightQuadrant)==MAK_RIGHT);
	}

private:
	int w, h;
};
	

class BasicSyscallsTestCase : public TestCase {

public:
	BasicSyscallsTestCase() : TestCase("Basic Syscalls Test") {
	}
	
	void trigTest() {
		double sinTable[32] = {0.0, 0.19509032179607727, 0.38268343195052273, 0.5555702324599515, 0.7071067805519557, 0.8314696116793008, 0.9238795319961289, 0.9807852800968344, 1.0, 0.9807852807971682, 0.9238795333698833, 0.8314696136736829, 0.7071067830903227, 0.5555702354447553, 0.382683435267059, 0.19509032531689338, 3.5897930298416118E-9, -0.19509031827526108, -0.38268342863398636, -0.5555702294751477, -0.7071067780135887, -0.8314696096849186, -0.9238795306223747, -0.9807852793965004, -1.0, -0.9807852814975022, -0.9238795347436375, -0.8314696156680653, -0.7071067856286897, -0.5555702384295594, -0.38268343858359527, -0.19509032883771};
		double tanTable[32] = {0.0, 0.098491403243895, 0.1989123671464188, 0.3033466832398309, 0.414213561847382, 0.5345111352296347, 0.6681786369457049, 0.8206787895145049, 0.9999999982051034, 1.218503523079299, 1.4966057590310156, 1.8708684062362453, 2.4142135531808555, 3.2965581916316093, 5.027339450861404, 10.15317021246028, 5.571351839435277E8, -10.153170586110596, -5.027339545180137, -3.2965582342327457, -2.4142135776934954, -1.8708684223908474, -1.49660577066133, -1.2185035319990407, -1.0000000053846894, -0.8206787955220729, -0.6681786421382064, -0.5345111398450395, -0.4142135660530861, -0.303346687159954, -0.19891237087824604, -0.09849140686851124};

		double pi = 3.14159265;
		double EPS = 0.0001;
		double delta = 2*pi/32;
		int numFailed = 0;
		double ang = 0;

		for(int i = 0; i < 32; i++) {
			double correct = sinTable[i];
			double test = sin(ang);
			if((test>=correct-EPS)&&(test<=correct+EPS)) {
			}
			else {
				numFailed++;
			}
			ang+=delta;
		}
		assert("testing sin()", numFailed==0);

		numFailed = 0;
		ang = 0;
		for(int i = 0; i < 32; i++) {
			double correct = sinTable[(i+8)&31];
			double test = cos(ang);
			if((test>=correct-EPS)&&(test<=correct+EPS)) {
			}
			else {
				numFailed++;
			}
			ang+=delta;
		}
		assert("testing cos()", numFailed==0);

		numFailed = 0;
		ang = 0;
		delta = pi/32;
		for(int i = 0; i < 32; i++) {
			double correct = tanTable[i];
			double test = tan(ang);
			if((test>=correct-EPS)&&(test<=correct+EPS)) {
			}
			else {
				numFailed++;
			}
			ang+=delta;
		}
		assert("testing tan()", numFailed==0);
	}

	void sqrtTest() {
		double sqrtTable[32] = {0.0, 1.0, 1.4142135623730951, 1.7320508075688772, 2.0, 2.23606797749979, 2.449489742783178, 2.6457513110645907, 2.8284271247461903, 3.0, 3.1622776601683795, 3.3166247903554, 3.4641016151377544, 3.605551275463989, 3.7416573867739413, 3.872983346207417, 4.0, 4.123105625617661, 4.242640687119285, 4.358898943540674, 4.47213595499958, 4.58257569495584, 4.69041575982343, 4.795831523312719, 4.898979485566356, 5.0, 5.0990195135927845, 5.196152422706632, 5.291502622129181, 5.385164807134504, 5.477225575051661, 5.5677643628300215};

		int numFailed = 0;
		double EPS = 0.0001;

		for(int i = 0; i < 32; i++) {
			double correct = sqrtTable[i];
			double test = sqrt(i);
			if((test>=correct-EPS)&&(test<=correct+EPS)) {
			}
			else {
				numFailed++;
			}
		}
		assert("testing sqrt()", numFailed==0);
	}

	void doubleTest() {
		double a, b;

		a = 1.0; 
		b = 2.0;
		assert("testing dcmp()!=0", ((memcmp(&a, &b, sizeof(double)))!=0)&&(dcmp(a, b)!=0));
		
		a = 2.0; 
		b = 2.0;
		assert("testing dcmp()==0", ((memcmp(&a, &b, sizeof(double)))==0)&&(dcmp(a, b)==0));

		a = dadd(1.0, 2.0);
		b = 3.0;
		assert("testing 1.0+2.0==3.0", dcmp(a, b)==0);

		a = dadd(1.0, 2.0);
		b = 2.0;
		assert("testing 1.0+2.0!=2.0", dcmp(a, b)!=0);

		a = dsub(2.0, 1.0);
		b = 1.0;
		assert("testing 2.0-1.0==1.0", dcmp(a, b)==0);

		a = dsub(2.0, 1.0);
		b = 4.0;
		assert("testing 2.0-1.0!=4.0", dcmp(a, b)!=0);

		a = dmul(2.0, 4.0);
		b = 8.0;
		assert("testing 2.0*4.0==8.0", dcmp(a, b)==0);

		a = dmul(2.0, 4.0);
		b = 7.0;
		assert("testing 2.0*4.0!=7.0", dcmp(a, b)!=0);

		a = dneg(2.0);
		b = -2.0;
		assert("testing dneg(2.0)==-2.0", dcmp(a, b)==0);

		a = dneg(2.0);
		b = -3.0;
		assert("testing dneg(2.0)!=-3.0", dcmp(a, b)!=0);

		a = ddiv(4.0, 2.0);
		b = 2.0;
		assert("testing 4.0/2.0==2.0", dcmp(a, b)==0);

		a = ddiv(4.0, 3.0);
		b = 2.0;
		assert("testing 4.0/3.0!=2.0", dcmp(a, b)!=0);
	}

	void stringAndMemTest() {
		char temp[256];
		char *a, *b;

		memset(temp, 3, 256);		
		int numFailed = 255;
		while(numFailed) {
			if(temp[numFailed]!=3)
				break;
			numFailed--;
		}
		assert("testing memset()", numFailed==0);

		unsigned char *tempMemory = new unsigned char[256];
		int numTests = 10;
		while(numTests--) {
			int length = rand()&0xf;
			int src_addr_start = rand()%(256-length);
			int src_addr_end = src_addr_start + length;
			int dst_addr_start = rand()%(256-length);
			int dst_addr_end = dst_addr_start + length;		
			while(dst_addr_start<=src_addr_end && dst_addr_end >= src_addr_start) {
				dst_addr_start = rand()%(256-length);
				dst_addr_end = dst_addr_start + length;	
			}
			unsigned char *src = &tempMemory[src_addr_start];
			unsigned char *dst = &tempMemory[dst_addr_start];

			memcpy(dst, src, length);
			
			numFailed = 0;
			while(length) {
				if(*dst++!=*src++) numFailed++;
				length--;
			}
			assert("testing memcpy()", numFailed==0);
		}

		a = "foo";
		b = "foo";
		assert("testing foo==foo", strcmp(a, b)==0);

		a = "foo";
		b = "bar";
		assert("testing foo!=bar", strcmp(a, b)!=0);
		
		memset(temp, 0, 256);
		strcpy(temp, a);
		assert("testing strcpy()", strcmp(temp, "foo")==0);
	}

	void run() {
		doubleTest();
		stringAndMemTest();
		trigTest();
		sqrtTest();
	}

};


extern "C" 
{
	int MAMain()
	{
		TestListener *xmlListener = new XMLOutputTestListener(XML_OUTPUT, "testreport.xml");
		TestSuite testSuite("Syscall Tests");
		TestCase *testCase;
//		testCase = new GraphicsSyscallsTestCase();
//		testSuite.addTestCase(testCase);
		
		testCase = new BasicSyscallsTestCase();
		testSuite.addTestCase(testCase);

		testSuite.addTestListener(xmlListener);
		testSuite.runTestCases(); 
		
		delete xmlListener;

		while(maGetEvent()!=EVENT_CLOSE) {
			maUpdateScreen();
		}

		return 0;
	}
}
