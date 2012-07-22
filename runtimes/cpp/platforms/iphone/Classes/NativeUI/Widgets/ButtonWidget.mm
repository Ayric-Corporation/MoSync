/* Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#import "ButtonWidget.h"
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>
#include "UIColorExpanded.h"

@implementation ButtonWidget

- (id)init {
	id ret = [super init];
    self.view = [[UIButton buttonWithType:UIButtonTypeRoundedRect] retain];
	UIButton* button = (UIButton*) self.view;
    button.contentEdgeInsets = UIEdgeInsetsMake(1.0, 1.0, 1.0, 1.0);
	[button addTarget:self action:@selector(touchDownEvent) forControlEvents:UIControlEventTouchDown];
    [button addTarget:self action:@selector(touchUpInsideEvent) forControlEvents:UIControlEventTouchUpInside];
    [button addTarget:self action:@selector(touchUpOutsideEvent) forControlEvents:UIControlEventTouchUpOutside];
	image = nil;
	leftCapWidth = 0;
	topCapHeight = 0;
	button.titleLabel.numberOfLines = 0;
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];

    self.autoSizeWidth = WidgetAutoSizeWrapContent;
    self.autoSizeHeight = WidgetAutoSizeWrapContent;
    button.imageView.contentMode = UIViewContentModeCenter;

    return ret;
}

-(void) touchDownEvent
{
    [super sendEvent:MAW_EVENT_POINTER_PRESSED];
}

-(void)touchUpInsideEvent
{
    [super sendEvent:MAW_EVENT_POINTER_RELEASED];
    [super sendEvent:MAW_EVENT_CLICKED];
}

-(void) touchUpOutsideEvent
{
    [super sendEvent:MAW_EVENT_POINTER_RELEASED];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@MAW_BUTTON_TEXT]) {
		UIButton* button = (UIButton*) self.view;
		[button setTitle:value forState:UIControlStateNormal];
		[self layout];
	} else
	if([key isEqualToString:@MAW_BUTTON_FONT_SIZE]) {
        TEST_FOR_NEGATIVE_VALUE([value floatValue]);
		UIButton* button = (UIButton*) self.view;
        UILabel* label = button.titleLabel;
		float fontSize = [value floatValue];
        UIFont* currentFont = label.font;
        NSString* currentFontName = [currentFont fontName];
        UIFont* newFont = [UIFont fontWithName:currentFontName size:fontSize];
        [label setFont:newFont];
		[self layout];
	} else
    if([key isEqualToString:@MAW_BUTTON_FONT_HANDLE]) {
        UIButton* button = (UIButton*) self.view;
        UILabel* label = button.titleLabel;
        UIFont* font = Base::getUIFontObject([value intValue]);
        if (!font)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
        [label setFont:font];
        [self layout];
    } else
	if([key isEqualToString:@MAW_BUTTON_FONT_COLOR]) {
		UIButton* button = (UIButton*) self.view;
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		[button setTitleColor:color forState:UIControlStateNormal];
	} else
	if([key isEqualToString:@MAW_IMAGE_BUTTON_BACKGROUND_IMAGE]) {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_PROPERTY_VALUE;
		UIButton* button = (UIButton*) self.view;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
		image = [UIImage imageWithCGImage:imageResource->image];
		[button setBackgroundImage:image forState:UIControlStateNormal];
	}
    else
    if([key isEqualToString:@MAW_IMAGE_BUTTON_IMAGE]) {
        int imageHandle = [value intValue];
        if(imageHandle<=0) return MAW_RES_INVALID_PROPERTY_VALUE;
        UIButton* button = (UIButton*) self.view;
        Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
        image = [UIImage imageWithCGImage:imageResource->image];
        [button setImage:image forState:UIControlStateNormal];
    }
    /* // doesn't work on iphone....
    else if ([key isEqualToString:@"scaleMode"]) {
        UIButton* button = (UIButton*)view;

        UIViewContentMode contentMode;

        if([value isEqualToString:@"none"])
            contentMode = UIViewContentModeCenter;
        else if([value isEqualToString:@"scaleXY"])
            contentMode = UIViewContentModeScaleToFill;
        else if([value isEqualToString:@"scalePreserveAspect"])
            contentMode = UIViewContentModeScaleAspectFit;
        else
            return MAW_RES_INVALID_PROPERTY_NAME;

        [button setContentMode:contentMode];
        button.imageView.contentMode = contentMode;
    }
    */
	else if([key isEqualToString:@"leftCapWidth"]) {
		int newLeftCapWidth = [value intValue];
		if(image != nil) {
			UIButton* button = (UIButton*) self.view;
			UIImage* newImage = [image stretchableImageWithLeftCapWidth:newLeftCapWidth topCapHeight:topCapHeight];
			[button setBackgroundImage:newImage forState:UIControlStateNormal];
			image = newImage;
		}
		leftCapWidth = newLeftCapWidth;
	}
	else if ([key isEqualToString:@"topCapHeight"]) {
		int newTopCapHeight = [value intValue];
		if(image != nil) {
			UIButton* button = (UIButton*) self.view;
			UIImage* newImage = [image stretchableImageWithLeftCapWidth:leftCapWidth topCapHeight:newTopCapHeight];
			[button setBackgroundImage:newImage forState:UIControlStateNormal];
			image = newImage;
		}
		topCapHeight = newTopCapHeight;
	}
	else if([key isEqualToString:@MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT]) {
		UIButton* button = (UIButton*) self.view;
        if([value isEqualToString:@"left"]) {
			[button setContentHorizontalAlignment:UIControlContentHorizontalAlignmentLeft];
		}
		else if([value isEqualToString:@"center"]) {
			[button setContentHorizontalAlignment:UIControlContentHorizontalAlignmentCenter];
		}
		else if([value isEqualToString:@"right"]) {
			[button setContentHorizontalAlignment:UIControlContentHorizontalAlignmentRight];
		}
        else {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
	}
    else if([key isEqualToString:@MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT]) {
		UIButton* button = (UIButton*) self.view;
		if([value isEqualToString:@"top"]) {
			[button setContentVerticalAlignment:UIControlContentVerticalAlignmentTop];
		}
		else if([value isEqualToString:@"center"]) {
			[button setContentVerticalAlignment:UIControlContentVerticalAlignmentCenter];
		}
		else if([value isEqualToString:@"bottom"]) {
			[button setContentVerticalAlignment:UIControlContentVerticalAlignmentBottom];
		} else {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@MAW_BUTTON_TEXT]) {
		UIButton* button = (UIButton*) self.view;
		return [button.titleLabel.text retain];
	} else {
		return [super getPropertyWithKey:key];
	}
}

@end
