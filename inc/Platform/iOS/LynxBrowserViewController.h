/*
 Copyright 2007 Lee S. Barney
 
 This file is part of QuickConnectiPhoneHybrid.
 
 QuickConnectiPhoneHybrid is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 QuickConnectiPhoneHybrid is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with QuickConnectiPhoneHybrid. If not, see .
 */

#import <UIKit/UIKit.h>

#define kAccelerometerFrequency	 .001 //Hz

@interface BrowserViewController : UIViewController<UIWebViewDelegate> {
    NSString* url;
}

@property (nonatomic, retain) NSString *url;

- (id)init:(NSString*)title withURL:(NSString*)url;

@end