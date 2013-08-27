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

//
//  NavBarWidget.mm
//  nativeuitest
//
//  Created by Magnus Hult <magnus@magnushult.se> on 01/23/11.
//

#import "NavBarWidget.h"

#ifndef NATIVE_TEST
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>
#endif

@implementation NavBarWidget

- (id)init {	
	UINavigationBar* navbar = [[[UINavigationBar alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];
	navbar.delegate = self;
	prevNavitem = [[UINavigationItem alloc] init];
	currNavitem = [[UINavigationItem alloc] init];
	[navbar pushNavigationItem:prevNavitem animated:false];
	[navbar pushNavigationItem:currNavitem animated:false];
	view = navbar;		
	
	return [super init];	
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"title"]) {
		currNavitem.title = value;
	}
	else if([key isEqualToString:@"backButtonTitle"]) {
		//navitem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:value style:UIBarButtonItemStylePlain target:nil action:nil];
		//navitem.hidesBackButton = YES;
		prevNavitem.title = value;
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	return [super getPropertyWithKey:key];
}

- (void)navigationBar:(UINavigationBar *)navigationBar didPopItem:(UINavigationItem *)item {
}

- (void)navigationBar:(UINavigationBar *)navigationBar didPushItem:(UINavigationItem *)item {
}

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPopItem:(UINavigationItem *)item {
#ifndef NATIVE_TEST
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	event.data = eventData;
	Base::gEventQueue.put(event);
#endif

	return false;
}

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPushItem:(UINavigationItem *)item {
	return true;
}

@end
