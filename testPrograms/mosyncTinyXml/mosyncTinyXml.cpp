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

#include <MTXml/MTXml.h>
#include <conprint.h>
#include <maassert.h>

static char sDocument[] = "<devices>"
"<!-- New devices -->"
"	<device>"
"		<identifier>MobileSorcery/Emulator</identifier>"
"		<features>hasPointerEvents</features> "
"		<capability name=\"JavaPackage\" value=\"mmapi, btapi\" />"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"		<capability name=\"JavaPlatform\" value=\"MIDP/2.0\" />"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.1\" />"
" 	</device>"
""
"	<device>"
"		<identifier>Motorola/ROKR Z6</identifier>"
"		<capability name=\"JavaPackage\" value=\"mmapi\" />"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"		<capability name=\"BitsPerPixel\" value=\"16\" />"
"		<capability name=\"JavaPlatform\" value=\"MIDP/2.0\" />"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.1\" />"
"	</device>"
""
""
"<!-- Changed devices -->"
"	<device>"
"		<identifier>HTC/Excalibur, HTC/S620, HTC/S621</identifier>"
"		<features>hasCommandKeyEvents</features>"
"		<capability name=\"ScreenSize\" value=\"320x240\" />"
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
"	<device>"
"		<identifier>HTC/Vox, HTC/S720</identifier>"
"		<features>hasCommandKeyEvents</features> "
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
"	<device>"
"		<identifier>HTC/Titan</identifier>"
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
"	<device>"
"		<identifier>HTC/Tornado</identifier>"
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
"	<device>"
"		<identifier>HTC/Wizard</identifier>"
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
""
"	<device>"
"		<identifier>Motorola/C261</identifier>"
"		<capability name=\"ScreenSize\" value=\"128x160\" />"
"	</device>"
""
"	<device>"
"		<identifier>O2/XDAII</identifier>"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/SLVR</identifier>"
"		<features>doubleBuffering, hasCamera</features>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V1050</identifier>"
"		<features>doubleBuffering, hasCamera</features>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V360</identifier>"
"		<features>doubleBuffering, hasCamera</features>"
"	</device>"
""
"	<device>"
"		<identifier>Sony-Ericsson/P1</identifier> "
"		<features>hasCamera, hasVideo, doubleBuffering, hasCommandKeyEvents, hasPointerEvents</features> "
"	</device>"
""
"	<device>"
"		<identifier>Sony-Ericsson/P990i</identifier>"
"		<capability name=\"MoSyncBugs\" value=\"backlightFlashes\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/5700_XpressMusic</identifier>"
"		<groups>Series60E3FP1</groups>"
"		<capability name=\"IconSize\" value=\"53x53,55x55\"/>"
"	</device>"
""
""
"<!-- Missing attributes -->"
"	<device>"
"		<identifier>Motorola/V220</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V180</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V80</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/T725</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V300</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V400</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V500</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V600</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/A760</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/6660</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"nonexistent phone\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/7610</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/6620</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/6600</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/5140</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.1\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/3220</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.1\"/>"
"	</device>"
""
""
""
"	<!-- Imate devices -->"
"	<device>"
"  		<identifier>Imate/JasJar</identifier>"
"  		<capability name=\"ScreenSize\" value=\"640x480\" />"
"	</device>"
"	 "
"	<device>"
"		<identifier>Imate/K-Jam</identifier>"
"		<capability name=\"ScreenSize\" value=\"240x320\"/>"
"	</device>"
"	"
"	<device>"
"		<identifier>Imate/Jam</identifier>"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"	</device>"
"	"
"	<device>"
"		<identifier>Imate/Jamin</identifier>"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"	</device>"
"	"
"	<device>"
"		<identifier>Imate/SP3, Imate/SP3K</identifier>"
"		<capability name=\"ScreenSize\" value=\"176x220\" />"
"	</device>"
""
"</devices>"
;

static void encoding(MTXContext* context, const char* value) {
	printf("encoding: \"%s\"\n", value);
}
static void tagStart(MTXContext* context, const void* name, int len) {
	printf("s %i: \"%s\"\n", len, (char*)name);
}
static void tagAttr(MTXContext* context, const void* attrName, const void* attrValue) {
	printf("a \"%s\": \"%s\"\n", (char*)attrName, (char*)attrValue);
}
static void tagData(MTXContext* context, const void* data, int len) {
	printf("d %i: \"%s\"\n", len, (char*)data);
}
static void tagStartEnd(MTXContext* context) {
	printf("se\n");
}
static void tagEnd(MTXContext* context, const void* name, int len) {
	printf("e %i: \"%s\"\n", len, (char*)name);
}
static void dataRemains(MTXContext* context, const char* data, int len) {
	printf("r %i: \"%s\"\n", len, data);
}
static void parseError(MTXContext* context, int offset) {
	printf("parseError %i\n", offset);
}
static void emptyTagEnd(MTXContext* context) {
	printf("emptyTagEnd\n");
}

extern "C" int MAMain() GCCATTRIB(noreturn);
extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	printf("Hello World!\n");

	MTXContext c;
	c.encoding = encoding;
	c.tagStart = tagStart;
	c.tagAttr = tagAttr;
	c.tagData = tagData;
	c.tagEnd = tagEnd;
	c.tagStartEnd = tagStartEnd;
	c.dataRemains = dataRemains;
	c.parseError = parseError;
	c.emptyTagEnd = emptyTagEnd;
	c.unicodeCharacter = mtxBasicUnicodeConvert;
	mtxStart(&c);
	mtxFeed(&c, sDocument);

	FREEZE;
}
