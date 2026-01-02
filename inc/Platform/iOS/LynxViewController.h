//
//  Stage2ViewController.h
//  Stage2
//
//  Created by owen on 2010/10/1.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <AVFoundation/AVFoundation.h>
#import <GLKit/GLKViewController.h>

#import <LynxEngineClasses.h>

#include <Platform/iOS/LynxTextInputViewController.h>

#ifdef __GLKVIEW__
    @interface LynxViewController : GLKViewController
#else
    @interface LynxViewController : UIViewController<UIWebViewDelegate>
#endif
{
    NSString *_sessionPreset;
    AVCaptureSession *_session;
    CVOpenGLESTextureCacheRef _videoTextureCache;
    
    bool bCameraCptureSize;
}

@property (strong, nonatomic) TextInputViewController *TextInput;

//- (void)setupAVCapture;
//- (void)startAVCapture;
//- (void)stopAVCapture;
//- (void)tearDownAVCapture;
-(void)OpenBrowserViewController:(const LYNXCHAR*)url withTitle:(const LYNXCHAR*)title;
-(void)OpenTextInputViewController:(const LYNXCHAR*)name withText:(LynxEngine::GUISystem::CText*)text;
- (void)webViewDidFinishLoad:(UIWebView *)webView;
@end

extern LynxEngine::GameFramework::CKernel*	glpKernel;
