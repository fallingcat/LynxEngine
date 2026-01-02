//
//  EAGLView.m
//  Stage2
//
//  Created by owen on 2010/10/1.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import <Platform/iOS/LynxViewController.h>
#import <Platform/iOS/LynxView.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <LynxDebugMemAllocator.h>
#include <Platform/iOS/LynxiOSGameFrameworkKernel.h>
#include <Platform/iOS/LynxiOSFileStream.h>
#include <Platform/iOS/LynxiOSTouch.h>
#include <Platform/iOS/LynxiOSAccelerometer.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Buffer.h>

using namespace LynxEngine;

void lynxiOSSetupGLColorBufferFromContext()
{
	LynxView* lpView = ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->GetlpView();
	EAGLContext* lpContext = lpView.m_Context;
	[lpContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>)lpView.layer];
    //[lpContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>)nil];
}

@implementation TextLayer

- (void)drawRect:(CGRect)rect 
{
	currentDisplayString = @"Text Layer";
	CGPoint point;
	UIFont *font = [UIFont systemFontOfSize:16];
	[[UIColor blackColor] set];
	point = CGPointMake(0, 0);
	[currentDisplayString drawAtPoint:point forWidth:(self.bounds.size.width) withFont:font fontSize:16 lineBreakMode:NSLineBreakByWordWrapping baselineAdjustment:UIBaselineAdjustmentAlignBaselines];
}

- (id)init
{
    if ((self = [super init]))
	{
	}
	return self;
}

- (id)initWithCoder:(NSCoder*)coder
{
    if ((self = [super initWithCoder:coder])) 
	{		
	}	
	return self;
}

- (void)dealloc 
{
	//[currentDisplayString release];
	//[super dealloc];
}

@end

// CONSTANTS
#define kAccelerometerFrequency		60.0 // Hz

@implementation LynxView

@synthesize m_Context;
@synthesize m_LoadingContext;

- (void)dealloc
{
	glpKernel->vRelease();
	if([EAGLContext currentContext] == m_Context) 
	{
		[EAGLContext setCurrentContext:nil];
	}
	
	/*
    [m_PinchGesture release];
	[m_RotationGesture release];
	[m_LongPressGesture release];
    [m_PanGesture release];
	[m_TapGesture release];
	[m_Tap2Gesture release];
	[m_Tap3Gesture release];
	[m_SwipeLeftGesture release];
	[m_SwipeRightGesture release];
	[m_SwipeUpGesture release];
	[m_SwipeDownGesture release];
	
	[m_TextLayer release];
	[m_Context release];
	*/

    m_Context = nil;	
	m_LoadingContext = nil;
	
	//[super dealloc];
}

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (void)setupDesc
{
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        m_Desc.GraphicParam.Width = self.bounds.size.width;
        m_Desc.GraphicParam.Height = self.bounds.size.height;	
    }
    else
    {
        m_Desc.GraphicParam.Width = self.bounds.size.width;
        m_Desc.GraphicParam.Height = self.bounds.size.height;
    }
    
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && ([UIScreen mainScreen].scale == 2.0)) 
    {        
        // Retina display
        m_Desc.GraphicParam.Width *= 2.0f;
        m_Desc.GraphicParam.Height *= 2.0f;
    }
    
    m_Desc.GraphicParam.DepthBufferFormat = LynxEngine::GraphicsSystem::CRenderer::FORMAT_D24S8;
	m_Desc.GraphicParam.NumMultiSamples = 1;
	m_Desc.GraphicParam.RendererType = LynxEngine::GraphicsSystem::CRenderer::OGLES2_RENDERER;
    
    self.contentScaleFactor = float(m_Desc.GraphicParam.Width)/float(self.bounds.size.width);
}

void InterruptionListenerCallback(void *inUserData, UInt32 interruptionState) 
{
    LynxView* View = (LynxView *)CFBridgingRelease(inUserData);

	if (glpKernel->GetlpEngine()->GetlpSoundSystem())
	{
		if (interruptionState == kAudioSessionBeginInterruption)
		{
            if (!glpKernel->GetlpEngine()->GetlpSoundSystem()->IsPaused())
            {
                AudioSessionSetActive(NO);
                glpKernel->GetlpEngine()->GetlpSoundSystem()->vPause();
            }
		}
		else if (interruptionState == kAudioSessionEndInterruption)
		{
            if (glpKernel->GetlpEngine()->GetlpSoundSystem()->IsPaused())
            {
                // Reset audio session
                UInt32 Category = kAudioSessionCategory_AmbientSound;
                AudioSessionSetProperty ( kAudioSessionProperty_AudioCategory, sizeof (Category), &Category );
                
                // Reactivate the current audio session
                AudioSessionSetActive(YES);
                
                glpKernel->GetlpEngine()->GetlpSoundSystem()->vResume();
            }
		}
	}
}

- (LYNXBOOL)initEngine
{
    // Get the layer
    //self.transform = CGAffineTransformMakeScale(1.0, -1.0);
    //CAEAGLLayer *eaglLayer = (CAEAGLLayer*)self.layer;
    //eaglLayer.opaque = YES;
    //eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE],
    //                                kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    
    [self setupDesc];
    switch (m_Desc.GraphicParam.RendererType)
    {
        case LynxEngine::GraphicsSystem::CRenderer::OGLES_RENDERER:
            m_Context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            break;
        case LynxEngine::GraphicsSystem::CRenderer::OGLES2_RENDERER:
            m_Context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            break;
    }
    if (m_Context == nil)
    {
        LYNX_LOG(glpLogger, CString("LynxView")+CString("Failed to create EAGLContext!\r\n"));
        return LYNX_FALSE;
    }
    m_LoadingContext = [[EAGLContext alloc] initWithAPI:[m_Context API] sharegroup: [m_Context sharegroup]];
    
    if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.1"))
    {
        m_Context.multiThreaded = YES;
    }

    #ifdef __GLKVIEW__
        self.context = m_Context;
        self.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
        self.drawableDepthFormat = GLKViewDrawableDepthFormat24;
        self.drawableStencilFormat = GLKViewDrawableStencilFormat8;
        self.delegate = self;
    #endif
    
    ((LynxEngine::GameFramework::iOS::CKernel*)glpKernel)->SetlpView(self);
    
    // Make sure that you are drawing to the current context
    [EAGLContext setCurrentContext:m_Context];
    
    glpKernel->vInit(0, NULL, &m_Desc);
    
    // Add gesture rocognizer for scaling
    m_PinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(ScaleGesture:)];
    
    // Add gesture rocognizer for rotation
    m_RotationGesture = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(RotateGesture:)];
    
    // Add gesture rocognizer for long press
    m_LongPressGesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(LongPressGesture:)];
    
    // Add gesture rocognizer for pan
    m_PanGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(PanGesture:)];
    m_PanGesture.cancelsTouchesInView = true;
    
    // Add gesture rocognizer for tap
    m_TapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(TapGesture:)];
    m_TapGesture.numberOfTapsRequired = 1;
    m_TapGesture.numberOfTouchesRequired = 1;
    
    // Add gesture rocognizer for tap2
    m_Tap2Gesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(Tap2Gesture:)];
    m_Tap2Gesture.numberOfTapsRequired = 2;
    m_Tap2Gesture.numberOfTouchesRequired = 1;
    
    // Add gesture rocognizer for tap3
    m_Tap3Gesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(Tap3Gesture:)];
    m_Tap3Gesture.numberOfTapsRequired = 3;
    m_Tap3Gesture.numberOfTouchesRequired = 1;
    
    // Add gesture rocognizer for swipe
    m_SwipeLeftGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(SwipeGesture:)];
    m_SwipeLeftGesture.direction = UISwipeGestureRecognizerDirectionLeft;
    m_SwipeLeftGesture.cancelsTouchesInView = true;
    
    m_SwipeRightGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(SwipeGesture:)];
    m_SwipeRightGesture.direction = UISwipeGestureRecognizerDirectionRight;
    m_SwipeRightGesture.cancelsTouchesInView = true;

    
    m_SwipeUpGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(SwipeGesture:)];
    m_SwipeUpGesture.direction = UISwipeGestureRecognizerDirectionUp;
    m_SwipeUpGesture.cancelsTouchesInView = true;

    m_SwipeDownGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(SwipeGesture:)];
    m_SwipeDownGesture.direction = UISwipeGestureRecognizerDirectionDown;
    m_SwipeDownGesture.cancelsTouchesInView = true;

    
    //Configure and start accelerometer
    /*
     if (((LynxEngine::iOS::CEngine*)(glpKernel->GetlpEngine()))->GetlpSystemAccelerometer())
     {
     [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
     [[UIAccelerometer sharedAccelerometer] setDelegate:self];
     }
     */
    
    m_LoopTimer = nil;
    m_DisplayLink = nil;
    m_bDisplayLinkSupported = LYNX_FALSE;
    
    /*
     NSString *reqSysVer = @"3.1";
     NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
     if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
     m_bDisplayLinkSupported = LYNX_TRUE;
     */
    
    float Scale  = self.contentScaleFactor;
    CGRect Rect = self.bounds;
    
    //AudioSessionInitialize(NULL, NULL, InterruptionListenerCallback, self);    // In your init code
    
    return LYNX_TRUE;
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    if ((self = [super initWithCoder:coder])) 
	{
        [self initEngine];
	}	
	return self;
}

- (id)initWithFrame:(CGRect)aRect
{
    if ((self = [super initWithFrame:aRect]))
	{
        [self initEngine];
	}
	return self;
}

- (void)Activate:(LYNXBOOL)b
{
    #ifndef __GLKVIEW__
        if (m_bDisplayLinkSupported)
        {
            if (b)
            {
                // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
                // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
                // not be called in system versions earlier than 3.1.
                if (!m_DisplayLink)
                {
                    m_DisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(Loop)];
                    [m_DisplayLink setFrameInterval:(NSTimeInterval)((1.0f / 1000.0f))];
                    [m_DisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
                }
            }
            else
            {
                if (m_DisplayLink)
                {
                    [m_DisplayLink invalidate];
                    m_DisplayLink = nil;
                }
            }
        }
        else
        {
            if (b)
            {
                if (!m_LoopTimer)
                    m_LoopTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0f / 1000.0f)) target:self selector:@selector(Loop) userInfo:nil repeats:TRUE];
            }
            else
            {
                if (m_LoopTimer)
                {
                    [m_LoopTimer invalidate];
                    m_LoopTimer = nil;
                }
            }
        }
    #endif
}

- (void)SetGestureRecognization:(bool)b
{
	if (b)
	{
		[self addGestureRecognizer:m_PinchGesture];
		[self addGestureRecognizer:m_RotationGesture];
		[self addGestureRecognizer:m_LongPressGesture];
        [self addGestureRecognizer:m_PanGesture];
		[self addGestureRecognizer:m_TapGesture];
		[self addGestureRecognizer:m_Tap2Gesture];
		[self addGestureRecognizer:m_Tap3Gesture];
		[self addGestureRecognizer:m_SwipeLeftGesture];
		[self addGestureRecognizer:m_SwipeRightGesture];
		[self addGestureRecognizer:m_SwipeUpGesture];
		[self addGestureRecognizer:m_SwipeDownGesture];
	}
	else 
	{
		[self removeGestureRecognizer:m_PinchGesture];
		[self removeGestureRecognizer:m_RotationGesture];
		[self removeGestureRecognizer:m_LongPressGesture];
        [self removeGestureRecognizer:m_PanGesture];
		[self removeGestureRecognizer:m_TapGesture];
		[self removeGestureRecognizer:m_Tap2Gesture];
		[self removeGestureRecognizer:m_Tap3Gesture];	
		[self removeGestureRecognizer:m_SwipeLeftGesture];
		[self removeGestureRecognizer:m_SwipeRightGesture];
		[self removeGestureRecognizer:m_SwipeUpGesture];
		[self removeGestureRecognizer:m_SwipeDownGesture];
	}
}

- (void)SetTapGestureRecognization:(bool)b 
{
	if (b)
	{
		[self addGestureRecognizer:m_TapGesture];
		[self addGestureRecognizer:m_Tap2Gesture];
		[self addGestureRecognizer:m_Tap3Gesture];
	}
	else 
	{
		[self removeGestureRecognizer:m_TapGesture];
		[self removeGestureRecognizer:m_Tap2Gesture];
		[self removeGestureRecognizer:m_Tap3Gesture];	
	}
}

- (void)SetPinchGestureRecognization:(bool)b 
{
	if (b)
	{
		[self addGestureRecognizer:m_PinchGesture];
	}
	else 
	{
		[self removeGestureRecognizer:m_PinchGesture];
	}
}

- (void)SetRotationGestureRecognization:(bool)b 
{
	if (b)
	{
		[self addGestureRecognizer:m_RotationGesture];
	}
	else 
	{
		[self removeGestureRecognizer:m_RotationGesture];
	}
}

- (void)SetLongPressGestureRecognization:(bool)b 
{
	if (b)
	{
		[self addGestureRecognizer:m_LongPressGesture];
	}
	else 
	{
		[self removeGestureRecognizer:m_LongPressGesture];
	}
}

- (void)SetPanGestureRecognization:(bool)b
{
	if (b)
	{
		[self addGestureRecognizer:m_PanGesture];
	}
	else
	{
		[self removeGestureRecognizer:m_PanGesture];
	}
}

- (void)SetSwipeGestureRecognization:(bool)b 
{
	if (b)
	{
		[self addGestureRecognizer:m_SwipeLeftGesture];
		[self addGestureRecognizer:m_SwipeRightGesture];
		[self addGestureRecognizer:m_SwipeUpGesture];
		[self addGestureRecognizer:m_SwipeDownGesture];
	}
	else 
	{
		[self removeGestureRecognizer:m_SwipeLeftGesture];
		[self removeGestureRecognizer:m_SwipeRightGesture];
		[self removeGestureRecognizer:m_SwipeUpGesture];
		[self removeGestureRecognizer:m_SwipeDownGesture];
	}
}

- (void)adjustAnchorPointForGestureRecognizer:(UIGestureRecognizer *)gestureRecognizer 
{
    if (gestureRecognizer.state == UIGestureRecognizerStateBegan) 
	{
        UIView *piece = gestureRecognizer.view;
        CGPoint locationInView = [gestureRecognizer locationInView:piece];
        CGPoint locationInSuperview = [gestureRecognizer locationInView:piece.superview];
        
        piece.layer.anchorPoint = CGPointMake(locationInView.x / piece.bounds.size.width, locationInView.y / piece.bounds.size.height);
        piece.center = locationInSuperview;
    }
}

- (void)ScaleGesture:(UIPinchGestureRecognizer *)gestureRecognizer
{
    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];
    
    //if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) 
	{
        InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
		if (lpTouch)
		{
			Math::CReal Scale(gestureRecognizer.scale);
			Math::CReal Velocity(gestureRecognizer.velocity/30.0f);
			lpTouch->SetScaleGesture(Scale, Velocity);
			
			LYNXCHAR Buffer[256];
			lynxSprintf(Buffer, _T("Pinch : %.2f, %.2f\r\n"), Scale.r, Velocity.r);
			LYNX_LOG(glpLogger, CString(Buffer));
		}				
        [gestureRecognizer setScale:1];
    }
}

- (void)RotateGesture:(UIRotationGestureRecognizer *)gestureRecognizer
{
    [self adjustAnchorPointForGestureRecognizer:gestureRecognizer];
    
    //if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) 
	{
        InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
		if (lpTouch)
		{
			Math::CReal Rotate(gestureRecognizer.rotation*LYNXR2D);
			Math::CReal Velocity(gestureRecognizer.velocity*LYNXR2D/30.0f);
			lpTouch->SetRotateGesture(Rotate, Velocity);
		}
        [gestureRecognizer setRotation:0];
    }
}

- (void)LongPressGesture:(UILongPressGestureRecognizer *)gestureRecognizer
{
    //if (gestureRecognizer.state == UIGestureRecognizerStateBegan) 
	{
        InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
		if (lpTouch)
		{
			lpTouch->SetLongPressGesture();
		}		
    }
}

- (void)TapGesture:(UITapGestureRecognizer *)gestureRecognizer
{
	//if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) 
	{
        InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
		if (lpTouch)
		{
			lpTouch->SetTapGesture();
            lpTouch->ClearTouches();
            
            UIView *piece = gestureRecognizer.view;
            InputSystem::iOS::CTouch::TOUCHDATA TouchData;
            
            for (int i=0; i<[gestureRecognizer numberOfTouches]; i++)
            {
                CGPoint location = [gestureRecognizer locationOfTouch:i inView:piece];//[gestureRecognizer locationInView:this];
                //TouchData.TimeStamp = touch.timestamp;
                TouchData.TapCount = 1;
                TouchData.Pos = Math::CVector2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor);
                lpTouch->AddTouch(TouchData);
            }
		}
    }
}

- (void)Tap2Gesture:(UITapGestureRecognizer *)gestureRecognizer
{
	//if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) 
	{
        InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
		if (lpTouch)
		{
			lpTouch->SetTap2Gesture();
            lpTouch->ClearTouches();    
            
            UIView *piece = gestureRecognizer.view;
            InputSystem::iOS::CTouch::TOUCHDATA TouchData;
            
            for (int i=0; i<[gestureRecognizer numberOfTouches]; i++)
            {
                CGPoint location = [gestureRecognizer locationOfTouch:0 inView:piece];//[gestureRecognizer locationInView:this];
                //TouchData.TimeStamp = touch.timestamp;
                TouchData.TapCount = 2;
                TouchData.Pos = Math::CVector2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor);
                lpTouch->AddTouch(TouchData);
            }
        }	
    }
}

- (void)Tap3Gesture:(UITapGestureRecognizer *)gestureRecognizer
{
	//if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) 
	{
        InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
		if (lpTouch)
		{
			lpTouch->SetTap3Gesture();
		}
    }
}

- (void)PanGesture:(UIPanGestureRecognizer *)gestureRecognizer
{
	//if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged)
	{
		LynxEngine::Math::CVector2 Trans, Vel;
        InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
		if (lpTouch)
		{
            Trans.x = [gestureRecognizer translationInView:self].x;
            Trans.y = [gestureRecognizer translationInView:self].y;
            Vel.x = [gestureRecognizer velocityInView:self].x;
            Vel.y = [gestureRecognizer velocityInView:self].y;
            lpTouch->SetPanGesture(Trans, Vel);
            
            
            InputSystem::iOS::CTouch::TOUCHDATA TouchData;
            CGPoint location = [gestureRecognizer locationInView:self];
            //TouchData.TimeStamp = touch.timestamp;
            TouchData.TapCount = 1;
            TouchData.Pos = Math::CVector2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor);
            lpTouch->AddTouch(TouchData);

		}
    }
}

- (void)SwipeGesture:(UISwipeGestureRecognizer *)gestureRecognizer
{
	//if ([gestureRecognizer state] == UIGestureRecognizerStateBegan || [gestureRecognizer state] == UIGestureRecognizerStateChanged) 
	{
		LynxEngine::Math::CVector2 Dir;
        InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
		if (lpTouch)
		{
			switch (gestureRecognizer.direction)
			{
				case UISwipeGestureRecognizerDirectionRight:
					Dir.x = 1.0f;
					Dir.y = 0.0f;
					lpTouch->SetSwipeGesture(Dir);
					break;
				case UISwipeGestureRecognizerDirectionLeft:
					Dir.x = -1.0f;
					Dir.y = 0.0f;
					lpTouch->SetSwipeGesture(Dir);
					break;
				case UISwipeGestureRecognizerDirectionUp:
					Dir.x = 0.0f;
					Dir.y = -1.0f;
					lpTouch->SetSwipeGesture(Dir);
					break;
				case UISwipeGestureRecognizerDirectionDown:
					Dir.x = 0.0f;
					Dir.y = 1.0f;
					lpTouch->SetSwipeGesture(Dir);
					break;
            }
            
            InputSystem::iOS::CTouch::TOUCHDATA TouchData;
            CGPoint location = [gestureRecognizer locationInView:self];
            //TouchData.TimeStamp = touch.timestamp;
            TouchData.TapCount = 1;
            TouchData.Pos = Math::CVector2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor);
            lpTouch->AddTouch(TouchData);
		}
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
	InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
	if (lpTouch)
	{
		lpTouch->SetTouches(touches);
		lpTouch->ClearTouches();
		
		InputSystem::iOS::CTouch::TOUCHDATA TouchData;
		for (UITouch *touch in touches) 
		{
			CGPoint location = [touch locationInView:self];
			TouchData.TimeStamp = touch.timestamp;
			TouchData.TapCount = touch.tapCount;
			TouchData.Pos = Math::CVector2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor);
			lpTouch->AddTouch(TouchData);			
		}
	}
	
	// We only support single touches, so anyObject retrieves just that touch from touches
	UITouch *touch = [touches anyObject];
	
	// Only move the placard view if the touch was in the placard view
	/*
	 if ([touch view] != placardView) {
	 // In case of a double tap outside the placard view, update the placard's display string
	 if ([touch tapCount] == 2) {
	 [placardView setupNextDisplayString];
	 }
	 return;
	 }
	 */
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{	
	InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
	if (lpTouch)
	{
		lpTouch->SetTouches(touches);
		lpTouch->ClearTouches();
		
		InputSystem::iOS::CTouch::TOUCHDATA TouchData;
		for (UITouch *touch in touches) 
		{
			CGPoint location = [touch locationInView:self];
			TouchData.TimeStamp = touch.timestamp;
			TouchData.TapCount = touch.tapCount;
			TouchData.Pos = Math::CVector2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor);
			lpTouch->AddTouch(TouchData);			
		}
	}
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{
	InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
	if (lpTouch)
	{
		lpTouch->SetTouches(NULL);
        lpTouch->ClearTouches();
	} 
}


- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event 
{
	InputSystem::iOS::CTouch* lpTouch = static_cast<InputSystem::iOS::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
	if (lpTouch)
	{
		lpTouch->SetTouches(NULL);
        lpTouch->ClearTouches();
	} 
}

// UIAccelerometerDelegate method, called when the device accelerates.
/*
 -(void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
	((LynxEngine::iOS::CEngine*)(glpKernel->GetlpEngine()))->GetlpSystemAccelerometer()->SetValue(LynxEngine::Math::CVector3(acceleration.x, acceleration.y, acceleration.z));
}
 */

- (void)SetCurrentContext
{
    [EAGLContext setCurrentContext:m_Context];
}

- (void)SetLoadingContext
{
    [EAGLContext setCurrentContext:m_LoadingContext];
}

- (void)ReleaseCurrentContext
{
    [EAGLContext setCurrentContext:nil];
}

#ifdef __GLKVIEW__
    - (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
    {
        glpKernel->vLoop();
        //[EAGLContext setCurrentContext:m_Context];
    }
#else
    // The main loop of the application
    - (void)Loop
    {
        [EAGLContext setCurrentContext:m_Context];
        glpKernel->vLoop();
        //glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->SwapBackBuffer();
        //[m_Context presentRenderbuffer:GL_RENDERBUFFER_OES];
    }
#endif


- (EAGLContext *)Context
{
    return m_Context;
}

@end
