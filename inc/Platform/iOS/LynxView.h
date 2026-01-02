//
//  EAGLView.h
//  Stage2
//
//  Created by owen on 2010/10/1.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKView.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface TextLayer : UIView 
{
	NSString *currentDisplayString;	
}

@end

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
#ifdef __GLKVIEW__
    @interface LynxView : GLKView<GLKViewDelegate> //<UIAccelerometerDelegate>
#else
    @interface LynxView : UIView
#endif
{
//@private
	LynxEngine::GameFramework::CKernel::INIT_DESC m_Desc;
    EAGLContext*					m_Context; 
	EAGLContext*					m_LoadingContext; 
	TextLayer*						m_TextLayer;
	GLuint							m_FrameBuffer;
    LYNXBOOL                        m_bDisplayLinkSupported;
    id                              m_DisplayLink;
	NSTimer*						m_LoopTimer;
		
	UIPinchGestureRecognizer*		m_PinchGesture;
	UIRotationGestureRecognizer*	m_RotationGesture;
	UILongPressGestureRecognizer*	m_LongPressGesture;
    UIPanGestureRecognizer*         m_PanGesture;
	UITapGestureRecognizer*			m_TapGesture;
	UITapGestureRecognizer*			m_Tap2Gesture;
	UITapGestureRecognizer*			m_Tap3Gesture;
	UISwipeGestureRecognizer*		m_SwipeLeftGesture;
	UISwipeGestureRecognizer*		m_SwipeRightGesture;
	UISwipeGestureRecognizer*		m_SwipeUpGesture;
	UISwipeGestureRecognizer*		m_SwipeDownGesture;
}

@property (nonatomic, retain) EAGLContext *m_Context;
@property (nonatomic, retain) EAGLContext *m_LoadingContext;

- (void)setupDesc;
- (LYNXBOOL)initEngine;
- (void)Activate:(LYNXBOOL)b;
- (void)SetCurrentContext;
- (void)SetLoadingContext;
- (void)ReleaseCurrentContext;
- (void)Loop;
- (void)SetGestureRecognization:(bool)b;
- (void)SetTapGestureRecognization:(bool)b;
- (void)SetPinchGestureRecognization:(bool)b;
- (void)SetRotationGestureRecognization:(bool)b;
- (void)SetLongPressGestureRecognization:(bool)b;
- (void)SetPanGestureRecognization:(bool)b;
- (void)SetSwipeGestureRecognization:(bool)b;

@end
