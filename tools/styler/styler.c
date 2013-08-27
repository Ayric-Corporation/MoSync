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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "helpers/attribute.h"

static const char* sUsage =
"usage: styler <input-filename>\n"
"\n"
"Checks that the input follows a set of style rules.\n"
"Returns 0 if so, non-zero if any rule is broken.\n"
"Outputs diagnostic information to stdout.\n"
"\n"
"The set of rules is as follows:\n"
"* No whitespace except tabs at the beginning of a line.\n"
"* No whitespace just before the end of line.\n"
"* LF(0x0A) line-endings. No CR(0x0D) allowed.\n"
"* Maximum line length: 120 bytes.\n"
"* The file must end with a newline(LF).\n"
;

#define MAX_LINE_LENGTH 120

static const char* sFilename;
static int sLineNum = 1;

static int checkFile(FILE* file);
static int checkLine(const char* start, const char* eol);

static void printerr(const char* fmt, ...) PRINTF_ATTRIB(1, 2);

// currently, this program will quit on the first error.
// however, it would be nice if it would print all errors.

int main(int argc, const char** argv) {
	FILE* file;
	int result;

	if(argc != 2) {
		puts(sUsage);
		return 1;
	}

	sFilename = argv[1];
	file = fopen(sFilename, "rb");
	if(!file) {
		printf("Couldn't open file '%s'\n", sFilename);
		return 1;
	}

	result = checkFile(file);
	fclose(file);
	return result;
}

static void printerr(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);

	printf("%s:%i: ", sFilename, sLineNum);
	vprintf(fmt, argptr);
	printf("\n");
}

static int checkFile(FILE* file) {
	char buf[MAX_LINE_LENGTH + 1];
	int fill = 0;
	int errorState = 0;
	int lastLongLine = 0;

	// read a line and check it, until we reach eof.
	while(1) {
		int res;
		const char* eol;
		const char* remains;
		int remainsLen;

		// fill the buffer
		while(fill < MAX_LINE_LENGTH) {
			res = fread(buf + fill, 1, MAX_LINE_LENGTH - fill, file);
			if(ferror(file)) {
				printf("Error reading file\n");
				return 1;
			}
			fill += res;
			if(feof(file))
				break;
		}
		buf[fill] = 0;

		// find a line ending
		eol = strchr(buf, '\n');
		if(eol == NULL) {
			if(feof(file)) {
				if(fill == 0)
					return errorState;
				printerr("no newline at end of file");
				return 1;
			} else {
				// todo: check the beginning of this line,
				// then read until end-of-line and check it, too.
				// then we can continue.

				// for now, we'll just skip to the end of the line;
				// not check for further errors on this line.
				if(lastLongLine != sLineNum) {
					printerr("line too long (max: %i bytes)", MAX_LINE_LENGTH);
					errorState |= 1;
					lastLongLine = sLineNum;
					fill = 0;
					continue;
				}
			}
		}

		if(lastLongLine != sLineNum) {
			errorState |= checkLine(buf, eol);
		}

		// move the next line to the beginning of the buffer
		remains = eol + 1;
		remainsLen = fill - (remains - buf);
		memmove(buf, remains, remainsLen);
		fill = remainsLen;

		sLineNum++;
	}
}

enum LineState {
	eBegin, eNormal, eWhitespace
};

static int checkLine(const char* start, const char* eol) {
	const char* ptr = start;
	enum LineState state = eBegin;

	// empty lines are ok
	if(start == eol)
		return 0;

	while(ptr != eol) {
		if(*ptr == '\n') {
			printerr("eol in the middle of a line?!? Report to author, please.");
			return 1;
		}
		if(*ptr == '\r') {
			printerr("CR detected");
			exit(1);
		}
		switch(state) {
		case eBegin:
			if(isspace(*ptr)) {
				if(*ptr != '\t') {
					printerr("non-tab whitespace at beginning of line");
					return 1;
				}
			} else {	// not whitespace
				state = eNormal;
			}
			break;
		case eNormal:
			if(isspace(*ptr)) {
				state = eWhitespace;
			}
			break;
		case eWhitespace:
			if(!isspace(*ptr)) {
				state = eNormal;
			}
			break;
		}
		ptr++;
	}
	if(state != eNormal) {
		printerr("whitespace just before end of line");
		return 1;
	}
	return 0;
}
