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

#include "MAFeed.h"
#include <maheap.h>
#include <MAUtil/XML.h>
#include "MAString.h"

#define STREQ(a, b) strcmp(a, b)

char* strippedString(char *src) {
	char *ret = 0;

	while(*src == ' ' || *src == '\t' || *src == 13 || *src == 10) {
		src++;
	}

	//printf("strip: '%s'\n", src);

	ret = (char*)malloc(strlen(src)+1);
	strcpy(ret, src);
	return ret;
}

char* newString(char *src) {
	char* ret = (char*)malloc(strlen(src)+1);
	strcpy(ret, src);
	return ret;
}

MAFeedItem *MAFeedItem_create() {
	MAFeedItem* item = (MAFeedItem *)malloc(sizeof(MAFeedItem));
	item->title = 0;
	item->link = 0;
	item->description = 0;
	return item;
}

void MAFeedItem_destroy(MAFeedItem *item) {
	free(item->title);
	free(item->link);
	free(item->description);
	free(item);
}

void MAFeedCHannel_add(MAFeedChannel* c, MAFeedItem* item) {
	
	c->items.add(item);
	printf("added item to feed (%x), new size: %d\n", c, c->items.size());
}

MAFeedChannel *MAFeedChannel_create() {
	MAFeedChannel* channel = new MAFeedChannel(); //(MAFeedChannel *)malloc(sizeof(MAFeedChannel));
	channel->title = 0;
	channel->link = 0;
	channel->description = 0;
	channel->language = 0;
	channel->copyright = 0;
	return channel;
}

void MAFeedChannel_destroy(MAFeedChannel *c) {
	free(c->title);
	free(c->link);
	free(c->description);
	free(c->language);
	free(c->copyright);
	Vector_each(MAFeedItem*, it, c->items) {
		delete (*it);
	}
	free(c);
}

/*

typedef BOOL (*StartCallback)(const char* tag, const AttrMap* argv);
typedef BOOL (*EndCallback)(const char* tag);
typedef BOOL (*CharDataCallback)(const char* data);


*/

//#define STREQ(a,b) (strcmp((a),(b))==0)

MAFeedChannel* channel = 0;
MAFeedItem* currentItem = 0;

char cDataBuffer[2048];

BOOL start(const char* tag, const AttrMap* /*argv*/) {
	cDataBuffer[0] = '\0';
	//printf("found tag: %s\n", tag);

	if(STREQ(tag, "channel")) {
		printf("found a channel element!\n");
		channel = MAFeedChannel_create();
	}

	if(STREQ(tag, "item")) {
		currentItem = MAFeedItem_create();
		//printf("found an item element!\n");
		MAFeedCHannel_add(channel, currentItem);
	}
	
	return TRUE;
}

BOOL end(const char* tag) {

	if(STREQ(tag, "item")) {
		currentItem = 0;
	}

	else if(STREQ(tag, "title")) {
		if(currentItem)
			currentItem->title = strippedString(cDataBuffer);
		else
			channel->title = strippedString(cDataBuffer);
	}

	else if(STREQ(tag, "link")) {
		if(currentItem)
			currentItem->link = strippedString(cDataBuffer);
		else
			channel->link = strippedString(cDataBuffer);
	}

	else if(STREQ(tag, "description")) {
		if(currentItem) {
			char *desc = strippedString(cDataBuffer);
			currentItem->description = desc;
		}
		else
			channel->description = strippedString(cDataBuffer);
	}

	else if(STREQ(tag, "language")) {
		channel->language = strippedString(cDataBuffer);
	}

	else if(STREQ(tag, "copyright")) {
		channel->copyright = strippedString(cDataBuffer);
	}


	cDataBuffer[0] = '\0';
	return TRUE;
}

BOOL cData(const char* data) {
	strcat(cDataBuffer, data);
	return TRUE;
}

MAFeedChannel* MAFeedChannel_parse(Handle data) {
	Stream* stream = Stream_create_conn(data);
	XML_parse(stream, start, end, cData);
	delete stream;
	return channel;
}

MAFeedChannel* MAFeedChannel_parseURL(const char* url) {
	int size;
	int res;
	Handle conn;
	Stream* stream;
	maConnect(url, &size);
	maWait(WAIT_CONN, 0);
	conn = maConnState();
	if(conn < 0) {
		return NULL;
	}

	stream = Stream_create_conn(conn);

	res = XML_parse(stream, start, end, cData);

	delete stream;
	maConnClose(conn);

	return res == MA_SUCCESS ? channel : NULL;
}
