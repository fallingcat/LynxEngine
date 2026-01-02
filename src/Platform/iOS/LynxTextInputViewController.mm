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
#import "Platform/iOS/LynxTextInputViewController.h"

@implementation TextInputViewController

@synthesize Textfield;

- (id)init:(NSString*)name withText:(LynxEngine::GUISystem::CText*)text
{
    self = [super initWithNibName:nil bundle:nil];
    
    if (self) {
        self->Name = name;
        self->Text = text;
    }
    
    return self;
}


- (void)loadView
{
    NSLog(@"loading view");
    
    LYNXRECT Rect = glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetViewportRect();
    Textfield = [[UITextField alloc] initWithFrame:CGRectMake(Rect.x1, Rect.y1, Rect.x2-Rect.x1, 30)];
    [Textfield setBackgroundColor:[UIColor whiteColor]];
    NSString* Str = nil;
    lynxcString2NSString(&Str, self->Text->GetUserData().empty()
        ? self->Text->GetText().c_str()
        : self->Text->GetUserData().c_str());
    [Textfield setText:Str];
    [Textfield setFont:[UIFont systemFontOfSize:24]];
    [Textfield setReturnKeyType:UIReturnKeyDone];
    self.view = Textfield;
    [Textfield becomeFirstResponder];
    Textfield.delegate = self;
    [Textfield addTarget:self action:@selector(TextFieldDidChange:)forControlEvents:UIControlEventEditingChanged];
    
    UIButton *transparencyButton = [[UIButton alloc] initWithFrame:((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpView().bounds];
    transparencyButton.backgroundColor = [UIColor clearColor];
    [((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpView() insertSubview:transparencyButton belowSubview:Textfield];
    [transparencyButton addTarget:self action:@selector(dismissHelper:) forControlEvents:UIControlEventTouchUpInside];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    [textField removeFromSuperview];
    
    LYNXCHAR cstr[512];
    lynxNSString2cString(cstr, textField.text);
    Text->SetUserData(cstr);
    Text->SetText(LynxEngine::CEngine::ProfanityFilterString(cstr, glpKernel->GetlpGame()->GetProfanityTable()));
    Text->Activate(LYNX_TRUE);
    return NO;
}

- (void)TextFieldDidChange:(UITextField *)textField {
    LYNXCHAR cstr[512];
    lynxNSString2cString(cstr, textField.text);
    Text->SetUserData(cstr);
    Text->SetText(LynxEngine::CEngine::ProfanityFilterString(cstr, glpKernel->GetlpGame()->GetProfanityTable()));
}

- (void)dismissHelper:(UIButton *)sender
{
    [[((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpView() subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    Text->Activate(LYNX_TRUE);
}
/*
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
        return (UIInterfaceOrientationPortrait);
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
*/
- (void)dealloc
{   
}


@end
