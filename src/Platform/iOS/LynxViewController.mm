//
//  Stage2ViewController.m
//  Stage2
//
//  Created by owen on 2010/10/1.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
//#import <libkern/OSMemoryNotification.h>

#include <LynxEngine_PCH.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <LynxEngine.h>
#include <LynxDebugMemAllocator.h>
#include <LynxReleaseMemAllocator.h>
#include <LynxSmallObjAllocator.h>

#include <Platform/iOS/LynxiOSGameFrameworkKernel.h>
#include <Platform/iOS/LynxViewController.h>
#include <Platform/iOS/LynxiOSConsoleLogger.h>
#include <Platform/iOS/LynxiOSConsoleOutputDevice.h>
#include <Platform/iOS/LynxBrowserViewController.h>

@implementation LynxViewController

@synthesize TextInput;

- (id)init 
{
    _session = nil;
    _videoTextureCache = nil;
	lynxInit(1);//LYNX_FALSE);
    ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetlpViewController(self);
    return [super init];
}

- (id)initWithCoder:(NSCoder*)coder
{
    _session = nil;
    _videoTextureCache = nil;
    lynxInit(1);//LYNX_FALSE);
    ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetlpViewController(self);
    return [super initWithCoder:coder];
}

- (void)awakeFromNib 
{
    ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetlpViewController(self);
	
	return [super awakeFromNib];
}

- (void)viewDidLoad 
{
    #ifdef __GLKVIEW__
        self.preferredFramesPerSecond = 60;
    #endif
    ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetlpViewController(self);
    
    [super viewDidLoad];
}

- (void)dealloc
{
	lynxRelease();
	
	//[super dealloc];
}


- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];

	//if (OSMemoryNotificationCurrentLevel() == OSMemoryNotificationLevelCritical)
		((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->vOnLowMemoryWarning();
}

-(void)OpenBrowserViewController:(const LYNXCHAR*)url withTitle:(const LYNXCHAR*)title
{
    NSString* Title = nil;
    lynxcString2NSString(&Title, title);
    NSString* URL = nil;
    lynxcString2NSString(&URL, url);
        
    BrowserViewController *Browser = [[BrowserViewController alloc] init:Title withURL:URL];
    //Browser.delegate = self;
    UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:Browser];
    [Browser loadView];    
    [self presentModalViewController:navigationController animated:YES];
}

-(void)OpenTextInputViewController:(const LYNXCHAR*)name withText:(LynxEngine::GUISystem::CText*)text
{
    NSString* Name = nil;
    lynxcString2NSString(&Name, name);
    
    TextInput = [[TextInputViewController alloc] init:Name withText:text];
    [TextInput loadView];
    [((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpView() addSubview:TextInput.view];
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    CGSize contentSize = webView.scrollView.contentSize;
    CGSize viewSize = webView.bounds.size;
    
    float rw = viewSize.width / contentSize.width;
    //webView.scrollView.minimumZoomScale = rw;
    //webView.scrollView.maximumZoomScale = rw;
    webView.scrollView.zoomScale = rw;
}
/*
- (void)setupAVCapture
{
    if (_session == nil)
    {
        //-- Create CVOpenGLESTextureCacheRef for optimal CVImageBufferRef to GLES texture conversion.
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, (((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpView().m_Context), NULL, &_videoTextureCache);
        if (err) 
        {
            LYNX_LOG(glpLogger, _T("[Error] Error at CVOpenGLESTextureCacheCreate!\n"));
            return;
        }
        
        _sessionPreset = AVCaptureSessionPreset352x288;
        
        ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetCameraCaptureWidth(352);
        ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetCameraCaptureHeight(288);
        
        //-- Setup Capture Session.
        _session = [[AVCaptureSession alloc] init];
        [_session beginConfiguration];
        
        //-- Set preset session size.
        [_session setSessionPreset:_sessionPreset];
        bCameraCptureSize = false;
        
        //-- Creata a video device and input from that Device.  Add the input to the capture session.
        AVCaptureDevice * videoDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        if(videoDevice == nil)
            assert(0);
        
        //-- Add the device to the session.
        NSError *error;        
        AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
        if(error)
            assert(0);
        
        [_session addInput:input];
        
        //-- Create the output for the capture session.
        AVCaptureVideoDataOutput * dataOutput = [[AVCaptureVideoDataOutput alloc] init];
        [dataOutput setAlwaysDiscardsLateVideoFrames:YES]; // Probably want to set this to NO when recording
        
        //-- Set to YUV420.
        [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] 
                                                                 forKey:(id)kCVPixelBufferPixelFormatTypeKey]]; // Necessary for manual preview
        
        // Set dispatch to be on the main thread so OpenGL can do things with the data
        [dataOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];        
        
        [_session addOutput:dataOutput];
        [_session commitConfiguration];
    }
    
    [self startAVCapture];
}

- (void)startAVCapture
{
    if (_session)
        [_session startRunning];
}

- (void)stopAVCapture
{
    if (_session)
        [_session stopRunning];
}

- (void)tearDownAVCapture
{
    if (_session)
    {
        [self cleanUpTextures];
        
        CFRelease(_videoTextureCache);
        
        [self stopAVCapture];
        [_session release];
        
        bCameraCptureSize = false;
        _session = nil;
    }
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput 
        didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
        fromConnection:(AVCaptureConnection *)connection 
{   
    if (_session)
    {
        CVImageBufferRef cameraFrame = CMSampleBufferGetImageBuffer(sampleBuffer);
        
        if (!bCameraCptureSize)
        {
            int bufferHeight = CVPixelBufferGetHeight(cameraFrame);
            int bufferWidth = CVPixelBufferGetWidth(cameraFrame);
            
            ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetCameraCaptureWidth(bufferWidth);
            ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetCameraCaptureHeight(bufferHeight);
            
            bCameraCptureSize = true;
            
            [self stopAVCapture];
        }
        else
        {
            CVPixelBufferLockBaseAddress(cameraFrame, 0);
                
                if (glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetSystemTexture(LynxEngine::GraphicsSystem::CAMERA_CAPTURE_TEXTURE))
                    glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetSystemTexture(LynxEngine::GraphicsSystem::CAMERA_CAPTURE_TEXTURE)->vUpdate(CVPixelBufferGetBaseAddress(cameraFrame));
                
                ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetCameraCaptureRawData(CVPixelBufferGetBaseAddress(cameraFrame));
        
            CVPixelBufferUnlockBaseAddress(cameraFrame, 0);
        }
    }
}
*/

@end
