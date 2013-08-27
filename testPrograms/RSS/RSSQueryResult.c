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

#include "RSSQueryResult.h"
#include "MAFeed.h"

#define NEW(o) ((o*)malloc(sizeof(o)))

typedef enum {
	RSSQ_TITLE,
	RSSQ_ITEMS
} RSSQueryResultType;

INHERIT_MAUIQUERYRESULT_BEGIN(RSSQueryResult)
	MAFeedChannel* feed;
	int queryType; // must be 
	int pos;
INHERIT_MAUIQUERYRESULT_END(RSSQueryResult)

// virtual functions

BOOL RSSQueryResult_first(MAUIQueryResult* self) {
	((RSSQueryResult*)self)->pos = 0;
	return TRUE;
}

BOOL RSSQueryResult_absolute(MAUIQueryResult* self, int n) {
	RSSQueryResult *r = (RSSQueryResult*)self; 
	r->pos = n;
	return n >= 0 && n < MAPtrVector_getSize(r->feed->items);
}

BOOL RSSQueryResult_next(MAUIQueryResult* self) {
	RSSQueryResult *r = (RSSQueryResult*)self; 
	r->pos++;
	return r->pos >= 0 && r->pos < MAPtrVector_getSize(r->feed->items);
}

BOOL RSSQueryResult_previous(MAUIQueryResult* self) {
	RSSQueryResult *r = (RSSQueryResult*)self; 
	r->pos--;
	return r->pos >= 0 && r->pos < MAPtrVector_getSize(r->feed->items);
}

char* RSSQueryResult_getString(MAUIQueryResult* self, char* columnName) {
	RSSQueryResult *r = (RSSQueryResult*)self; 
	if(r->queryType == RSSQ_TITLE) {
		if(r->pos == 0 && STREQ(columnName, "title"))
			return r->feed->title;
		else if(r->pos == 0 && STREQ(columnName, "link"))
			return r->feed->link;
		else if(r->pos == 0 && STREQ(columnName, "description"))
			return r->feed->description;
		else 
			return NULL;
	} 
	else if(r->queryType == RSSQ_ITEMS) {
		if(r->pos < MAPtrVector_getSize(r->feed->items)) {
			MAFeedItem* item = MAPtrVector_get(r->feed->items, r->pos);
			if(STREQ(columnName, "title"))
				return item->title;
			if(STREQ(columnName, "description"))
				return item->description;
			if(STREQ(columnName, "link"))
				return item->link;
		} else {
			return NULL;
		}
	}
	return NULL;
}

int RSSQueryResult_getInt(MAUIQueryResult* self, char* columnName) {
	return 0;
}

Handle RSSQueryResult_getImage(MAUIQueryResult* self, char* columnName) {
	return 0;
}

int RSSQueryResult_getColumnCount(MAUIQueryResult* self) {
	RSSQueryResult *r = (RSSQueryResult*)self; 
	if(r->queryType == RSSQ_TITLE) {
		return 1;
	} else {
		return 3;
	}
}

MAUIType RSSQueryResult_getType(MAUIQueryResult* self, int index) {
	return MT_STRING;
}

char* RSSQueryResult_getColumnName(MAUIQueryResult* self, int index) {
	RSSQueryResult *r = (RSSQueryResult*)self; 
	if(r->queryType == RSSQ_TITLE) {
		return index == 0 ? "title" : NULL;
	} else {
		switch(index) {
			case 0 : return "title";
			case 1 : return "description";
			case 2 : return "link";
			default: return NULL;
		}
	}
}

int RSSQueryResult_getColumnIndex(MAUIQueryResult* self, char* columnName) {
	RSSQueryResult *r = (RSSQueryResult*)self; 
	if(r->queryType == RSSQ_TITLE) {
		if(STREQ(columnName, "title")) {
			return 0;
		}
		else {
			return -1;
		}

	} else {
		if(STREQ(columnName, "title")) {
			return 0;
		}
		else if(STREQ(columnName, "description")) {
			return 1;
		}
		else if(STREQ(columnName, "link")) {
			return 2;
		}
		else
		{
			return -1;
		}
	}
}


MAUIQueryResultVtable vtableRSSQueryResult = {
	RSSQueryResult_first,
	RSSQueryResult_absolute,
	RSSQueryResult_next,
	RSSQueryResult_previous,
	RSSQueryResult_getString,
	RSSQueryResult_getInt,
	RSSQueryResult_getImage,
	RSSQueryResult_getColumnCount,
	RSSQueryResult_getType,
	RSSQueryResult_getColumnName,
	RSSQueryResult_getColumnIndex
};


// constructor

MAUIQueryResult* RSSQueryResult_create(char* query, MAFeedChannel* feed) {
	RSSQueryResult* ret = NEW(RSSQueryResult);
	ret->feed = feed;
	if(STREQ(query, "title"))
		ret->queryType = RSSQ_TITLE;
	else if (STREQ(query, "items"))
		ret->queryType = RSSQ_ITEMS;
	else
		ret->queryType = RSSQ_TITLE;
	ret->pos = 0;
	ret->vtable = &vtableRSSQueryResult;
	return (MAUIQueryResult*)ret;
}

