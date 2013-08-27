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
#include <MAUtil/Moblet.h>
#include <maassert.h>
#include <MAUtil/Connection.h>
#include <conprint.h>
#include <MAUtil/String.h>
#include <wchar.h>
#include <mawvsprintf.h>

using namespace MAUtil;

//******************************************************************************
// Config
//******************************************************************************

#define MTX_UNICODE 1

//#define STANDARD
#define SVD_LOCAL
//#define SVD_NET

#define BUFLEN 1024*2
#define DO_MULTIPLE_ROUNDS 0
#define TEST_STACK 0

#define PROCESSING_START 1
#define TORTURE_TEST_START 0
#define TORTURE2_START 0

static const char* urls[] = {
#ifdef STANDARD
	"http://rshotels.com/demo/GEORSS/PlacesToGoAMS.xml",
	//"http://localhost:5002/rss1.xml",
	//"http://localhost:5002/menuText.xml",
	/*"http://localhost:5002/torture.xml",
	"http://localhost:5002/guido_menu.xml",
	"http://localhost:5002/devices.xml",
	"http://localhost:5002/override_devices.xml",
	"http://localhost:5002/cnn_topstories.xml",*/
#endif
#ifdef SVD_LOCAL
	"http://localhost:5002/rss1.xml",
	"http://localhost:5002/rss2.xml",
	"http://localhost:5002/rss3.xml",
	"http://localhost:5002/rss4.xml",
#endif
#ifdef SVD_NET
	"http://www.svd.se/?service=rss&type=latest",
	"http://www.svd.se/nyheter/inrikes/?service=rss",
	"http://www.svd.se/naringsliv/nyheter/?service=rss&type=latest",
	"http://www.svd.se/nyheter/inrikes/?service=rss",
#endif
	""
};
static const int nUrls = (sizeof(urls) / sizeof(char*)) - 1;

#if MTX_UNICODE
typedef wchar_t tchar;
#define TLPRINTFLN wlprintfln
#define TPRINTF wprintf
//#define US "%s"	//unicode string
//#define CS "%S"	//C string
#else
typedef char tchar;
#define TLPRINTFLN lprintfln
#define TPRINTF printf
//unicode string is not supported in this mode.
//#define CS "%s"	//C string
#endif

typedef BasicString<tchar> TString;

// if log is on, error is not needed.
#if 0	//no log
#define LOG(...)
#define ERROR(fmt, ...) TPRINTF(L##fmt, ##__VA_ARGS__)
#elif 0	//screen log
#define LOG(fmt, ...) TPRINTF(L##fmt, ##__VA_ARGS__)
#define ERROR(...)
#else	//file log
#define LOG(fmt, ...) TLPRINTFLN(L##fmt, ##__VA_ARGS__)
#define ERROR(...)
#endif

//******************************************************************************
// Moblet
//******************************************************************************

class MyMoblet : public Moblet, private ConnectionListener, Mtx::MtxListener,
#if MTX_UNICODE
	Mtx::XmlListenerW
#else
	Mtx::XmlListener
#endif
{
public:
	MyMoblet() : mHttp(this), currentUrl(0) {
		totalStartTime = maGetMilliSecondCount();
		mTortureTest = TORTURE_TEST_START;
		mTorture2 = TORTURE2_START;
		mProcessing = PROCESSING_START;
		connect();
	}
private:
	Connection mHttp;
	int currentUrl;
#if MTX_UNICODE
	Mtx::ContextW mtx;
#else
	Mtx::Context mtx;
#endif
	char mBuffer[BUFLEN];
	char* mPtr;
	bool mError;
#if TEST_STACK
	Vector<TString> mStack;
#endif
	int totalStartTime, connectStartTime, recvStartTime;	//TODO: rename
	char mRecvBuffer[BUFLEN];
	bool mTortureTest, mTorture2, mProcessing;

#define STATS(m) \
	m(tagStart) m(tagAttr) m(tagData) m(tagEnd) m(emptyTagEnd) m(dataRemains) \
	m(recv) \
	m(recvTime) m(parseTime) m(connectTime) m(documentTime)

#define DECLARE_STAT(name) int name;
	struct STATS {
		STATS(DECLARE_STAT);
	} m;

	void dumpStats() {
#define DUMPINT(i) printf("%s: %i\n", #i, (i))
#define DUMPSTAT(name) DUMPINT(m.name);
		DUMPINT(m.tagEnd + m.emptyTagEnd);
		STATS(DUMPSTAT);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
	}

	void connect() {
		if(currentUrl >= nUrls) {
			bool goOn = false;
#if DO_MULTIPLE_ROUNDS
			if(mTortureTest && mTorture2 == TORTURE2_START) {
				mTorture2 = !mTorture2;
				goOn = true;
			} else {
				if(mTortureTest == TORTURE_TEST_START) {
					mTortureTest = !mTortureTest;
					goOn = true;
				}
			}
			if(!goOn) {
				if(mProcessing == PROCESSING_START) {
					mProcessing = !mProcessing;
					mTortureTest = TORTURE_TEST_START;
					mTorture2 = TORTURE2_START;
					goOn = true;
				}
			}
#endif	//DO_MULTIPLE_ROUNDS
			if(goOn) {
				printf("We're done with this round.\n");
				currentUrl = 0;
			} else {
				printf("We're really done.\n");
				return;
			}
		}
		printf("start... p(%i) t(%i) t2(%i)\n", mProcessing, mTortureTest, mTorture2);
		printf("%s\n", urls[currentUrl]);
		memset(&m, 0, sizeof(m));
		connectStartTime = maGetMilliSecondCount();
		mHttp.connect(urls[currentUrl]);
	}

	//ConnectionListener
	void connectFinished(Connection* conn, int result) {
		m.connectTime += maGetMilliSecondCount() - connectStartTime;
		if(result < 200 || result >= 300) {
			printf("HTTP error: %i\n", result);
			return;
		}
		mtx.init(this, this);
		mPtr = mBuffer;
		mError = false;
		recvStartTime = maGetMilliSecondCount();
		if(mTortureTest) {
			mHttp.recv(mRecvBuffer, BUFLEN - 1);	// -1 for the terminator
		} else {
			mHttp.recv(mBuffer, BUFLEN - 1);	// -1 for the terminator
		}
	}
	void connRecvFinished(Connection* conn, int result) {
		m.recv++;
		m.recvTime += maGetMilliSecondCount() - recvStartTime;
		LOG("recv'd %i\n", result);
		if(result <= 0) {
			ERROR("recv error: %i\n", result);
			m.documentTime = maGetMilliSecondCount() - connectStartTime;
			dumpStats();
			if(result == CONNERR_CLOSED) {	//we're done, s'cool
				currentUrl++;
				mHttp.close();
				connect();
			}
			return;
		}
		if(mTortureTest) {
			//call feed with one byte more each time, to stress-test the Remains system.
			for(int i=0; i<result; i++) {
				*(mPtr++) = mRecvBuffer[i];
				if(mTorture2) {
					if(i+1 < result)
						*(mPtr++) = mRecvBuffer[++i];
				}
				*mPtr = 0;
#if 0
				if((mPtr - mBuffer) != 1) {
					LOG("feed %i\n", (mPtr - mBuffer));
				}
#endif
				mPtr = mBuffer;
				int parseStartTime = maGetMilliSecondCount();
#if MTX_UNICODE
				wchar_t wbuf[BUFLEN];
				mtx.feed(mBuffer, wbuf);
#else
				if(mProcessing)
					mtx.feedProcess(mBuffer);	//causes mtx callbacks
				else
					mtx.feed(mBuffer);
#endif
				m.parseTime += maGetMilliSecondCount() - parseStartTime;
				if(mError) {
					printf("error, breaking.\n");
					break;
				}
			}
			if(!mError) {
				recvStartTime = maGetMilliSecondCount();
				mHttp.recv(mRecvBuffer, BUFLEN - 1);
			}
			else
				dumpStats();
		}	else {	//(mTortureTest)
			mPtr[result] = 0;
			LOG("feed %li\n", (mPtr - mBuffer) + result);
			mPtr = mBuffer;
			int parseStartTime = maGetMilliSecondCount();
#if MTX_UNICODE
				wchar_t wbuf[BUFLEN];
				mtx.feed(mBuffer, wbuf);
#else
			if(mProcessing)
				mtx.feedProcess(mBuffer);	//causes mtx callbacks
			else
				mtx.feed(mBuffer);
#endif
			m.parseTime += maGetMilliSecondCount() - parseStartTime;
			if(!mError) {
				recvStartTime = maGetMilliSecondCount();
				mHttp.recv(mPtr, BUFLEN - 1 - (mPtr - mBuffer));
			}
			else
				dumpStats();
		}//(mTortureTest)
	}

	//MtxListener
	void mtxDataRemains(const char* data, int len) {
		m.dataRemains++;
		//LOG("dr %i: \"%s\"\n", len, data);
		if(len > BUFLEN || len <= 0 || *data == 0) {
			ERROR("dr %i: \"%s\"\n", len, data);
			mError = true;
			return;
		}
		if(mBuffer == data) {	//valid value: don't memcpy.
			//printf("dr no-move\n");
			//mError = true;
			//return;
		} else {
			memcpy(mBuffer, data, len);
		}
		mPtr = mBuffer + len;
	}
	virtual void mtxEncoding(const char* name) {
		m.tagStart++;
		printf("Encoding: \"%s\"\n", name);
	}
	void mtxParseError() {
		printf("parseError\n");
		mError = true;
	}
	virtual void mtxTagStart(const tchar* name, int len) {
		m.tagStart++;
		LOG("s %i: \"%s\"\n", len, name);
		if(len > BUFLEN || len <= 0 || *name == '<') {
			ERROR("s %i: \"%s\"\n", len, name);
			mError = true;
			return;
		}
#if TEST_STACK
		mStack.add(name);
#endif
	}
	virtual void mtxTagAttr(const tchar* attrName, const tchar* attrValue) {
		m.tagAttr++;
		LOG("a \"%s\"=\"%s\"\n", attrName, attrValue);
		if(attrName[0] == 0) {
			ERROR("a \"%s\"=\"%s\"\n", attrName, attrValue);
			mError = true;
			return;
		}
	}
	virtual void mtxTagStartEnd() {
		LOG("mtxTagStartEnd\n");
	}
	virtual void mtxTagData(const tchar* data, int len) {
		m.tagData++;
		LOG("d %i: \"%s\"\n", len, data);
		if(len > BUFLEN || len <= 0 || (int)tstrlen(data) != len) {
			ERROR("d %i: \"%s\"\n", len, data);
			mError = true;
			return;
		}
#if 0
		if(strcmp(data, "HTC/Vox, HTC/S720") == 0)
			printf("temp\n");
#endif
	}
	virtual void mtxTagEnd(const tchar* name, int len) {
		m.tagEnd++;
		LOG("e %i: \"%s\"\n", len, name);
		if(len > BUFLEN || len <= 0) {
			ERROR("e %i: \"%s\"\n", len, name);
			mError = true;
			return;
		}
#if TEST_STACK
		if(mStack.size() == 0) {
			printf("Error: stack empty!\n");
			mError = true;
			return;
		}
		TString& s(mStack[mStack.size() - 1]);
		if(tstrcmp(s.c_str(), name) != 0 || s.length() != len) {
			printf("Error: stack mismatch\n"
#if MTX_UNICODE
				"\"%S\" != \"%S\"\n",
#else
				"\"%s\" != \"%s\"\n",
#endif
				s.c_str(), name);
			mError = true;
			return;
		}
		mStack.resize(mStack.size() - 1);
#endif
	}
	virtual void mtxEmptyTagEnd() {
		m.emptyTagEnd++;
#if TEST_STACK
		if(mStack.size() == 0) {
			printf("emptyTagEnd Error: stack empty!\n");
			mError = true;
			return;
		}
		LOG("emptyTagEnd: %s\n", mStack[mStack.size() - 1].c_str());
		mStack.resize(mStack.size() - 1);
#endif
	}
#if !MTX_UNICODE
	virtual unsigned char mtxUnicodeCharacter(int unicode) {
		unsigned char result;
		switch(unicode) {
		case 0x201C:
		case 0x201D:
			result = '\"';
			break;
		case 0x2013:
			result = '-';
			break;
		case 0x2009:
			result = ' ';
			break;
		default:
			result = XmlListener::mtxUnicodeCharacter(unicode);;
		}
		{
			char buf[8];
			if(result)
				sprintf(buf, "'%c'", result);
			else
				strcpy(buf, "0");
			printf("unicode 0x%X => %s\n", unicode, buf);
		}
		return result;
	}
#endif
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
};
