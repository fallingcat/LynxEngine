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
#import "Platform/iOS/LynxBrowserViewController.h"

UIWebView *aWebView;

@implementation BrowserViewController

@synthesize url;

- (id)init:(NSString*)title withURL:(NSString*)ourl
{
    self = [super initWithNibName:nil bundle:nil];
    
    if (self) {
        self.navigationItem.title = title;
        self.url = ourl;
    }
    
    return self;
}

- (void)loadView
{
    NSLog(@"loading view");
    
    // the base view for this view controller
    UIView *backgroundView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
    backgroundView.backgroundColor = [UIColor colorWithWhite:0.9f alpha:0.7f];
    
    // important for view orientation rotation
    backgroundView.autoresizesSubviews = YES;
    backgroundView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);
    //self.view = [backgroundView autorelease];
    self.view = backgroundView;
    
    UIBarButtonItem *BackButton = [[UIBarButtonItem alloc] initWithTitle:@"Back" style: UIBarButtonItemStylePlain
                                                                                  target:self
                                                                                  action:@selector(BackButtonPressed)];
    UIBarButtonItem *CancelButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                                                  target:self
                                                                                  action:@selector(CancelButtonPressed)];
    self.navigationItem.leftBarButtonItem = BackButton;
    self.navigationItem.rightBarButtonItem = CancelButton;
    //[CancelButton release];
   
    
    //create a frame that will be used to size and place the web view
    CGRect webFrame = [[UIScreen mainScreen] applicationFrame];
    //webFrame.size.height -= 40;
    //webFrame.origin.y += 40.0;	// shift the display up so that it covers the default open space from the content view
    aWebView = [[UIWebView alloc] initWithFrame:webFrame];
    //aWebView.scalesPageToFit = YES;
    aWebView.autoresizesSubviews = YES;
    aWebView.autoresizingMask=(UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth);
    //set the web view and acceleration delagates for the web view to be itself
    [aWebView setDelegate:self];
    //determine the path the to the index.html file in the Resources directory
    //NSString *filePathString = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"];
    //build the URL and the request for the index.html file
    //NSURL *aURL = [NSURL fileURLWithPath:filePathString];
    
    NSURL *aURL = [NSURL URLWithString:self.url];
    NSURLRequest *aRequest = [NSURLRequest requestWithURL:aURL];
    //load the index.html file into the web view.
    [aWebView loadRequest:aRequest];
    
    //add the web view to the content view
    [backgroundView addSubview:aWebView];
    //[aWebView release];
}

- (void)CancelButtonPressed
{
    [self dismissModalViewControllerAnimated:YES];
}

- (void)BackButtonPressed
{
    if ([aWebView canGoBack]) {
        [aWebView goBack];
    }
}


-(NSInteger)supportedInterfaceOrientations
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
        return (UIInterfaceOrientationMaskLandscapeLeft|UIInterfaceOrientationMaskLandscapeRight|UIInterfaceOrientationMaskPortrait|UIInterfaceOrientationMaskPortraitUpsideDown);
    else
        return (UIInterfaceOrientationMaskPortrait|UIInterfaceOrientationMaskPortraitUpsideDown);
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
        return (UIInterfaceOrientationLandscapeRight);
    else
        return (UIDeviceOrientationPortrait);
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	if (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight)
    {
  		return YES;
    }
	else
		return NO;
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview.
    // Release anything that's not essential, such as cached data.
}
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSLog(@"An error happened during load");
}
- (void)webViewDidStartLoad:(UIWebView *)webView{
    NSLog(@"loading started");
}
- (void)webViewDidFinishLoad:(UIWebView *)webView{
    NSLog(@"finished loading");
}

- (void)dealloc
{
    //[super dealloc];
}


@end