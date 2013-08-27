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
//  MoSyncUI.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//

#import "MoSyncUI.h"
#import "LabelWidget.h"
#import "ListViewWidget.h"
#import "ScreenWidget.h"
#import "ReflectionWidget.h"

@implementation MoSyncUI

NSMutableArray* widgetArray;
UIWindow* mainWindow;
UIViewController *mainController;
//UINavigationController *tabBarController;

- (IWidget*)getWidget: (int) handle {
	IWidget *widget = nil;
	widget = [widgetArray objectAtIndex:(NSUInteger)handle];
	return widget;
}

- (id)initWithWindow: (UIWindow*) window andController: (UIViewController*)controller {
	[super init];
	widgetArray = [[NSMutableArray alloc] init];
	
	//mainWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	if(!window) {
		window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];		
		[window makeKeyAndVisible];
	}
	
	mainWindow = window;
	mainController = controller;
	
	mainWindow.backgroundColor = [UIColor whiteColor];
	
	//tabBarController = [[UITabBarController alloc] init];
    //tabBarController = [[UINavigationController alloc] init];
	
	//tabBarController.viewControllers = [NSArray array];										 	
	//tabBarController.view = mainWindow;

	//[mainWindow addSubview:tabBarController.view];	

	
	return self;
}

- (void) close {
}

- (void) createWidget: (NSString*)name {
	IWidget *created = nil;
	NSString* realName = [name stringByAppendingString:@"Widget"];
	Class widgetClass = NSClassFromString(realName);
	if(widgetClass != nil) {
		created = [[widgetClass alloc] init];
		
		/*
		if([widgetClass class] == [ScreenWidget class]) {
			//[mainWindow addSubview: [created getView]];
			ScreenWidget* screen = (ScreenWidget*)created;
			NSMutableArray *newItems = [NSMutableArray arrayWithArray:tabBarController.viewControllers];
			[newItems addObject:[screen getController]];
			tabBarController.viewControllers = newItems;
		}
		*/
		
	} else {
		created = [[ReflectionWidget alloc] initWithName:name];
	}
	
	[created setWidgetHandle:[widgetArray count]]; 
	
	
	// todo handle these things.
	//if(created == nil) return 0;
	
	[widgetArray addObject:created];
	
}

- (void) removeWidget: (IWidget*) handle {
}

- (void) setPropertyOf: (IWidget*) widget withKey: (NSString*)key toValue: (NSString*)value {	
	[widget setPropertyWithKey:key toValue:value];
}

bool nativeUIEnabled = false;

static IWidget* sOldScreen = nil;


- (void)show: (IWidget*) widget {
	if(!nativeUIEnabled) {
		if(mainController)
			[mainController.view removeFromSuperview];
		nativeUIEnabled = true;
	}
	
	if(sOldScreen != nil) {
		UIView* actualView = [sOldScreen getView];
		[actualView removeFromSuperview];
	}
	
	[mainWindow insertSubview:[widget getView] atIndex:0];
	
	[widget layout];
	[mainWindow makeKeyAndVisible];
	sOldScreen = widget;	
}

@end
