//
//  EAGLView.h
//  Stage2
//
//  Created by owen on 2010/10/1.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

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
@interface LynxView : UIView <UIAccelerometerDelegate>
{
//@private
	LynxEngine::GameFramework::CKernel::INIT_DESC m_Desc;
    EAGLContext*					m_Context; 
	TextLayer*						m_TextLayer;
	GLuint							m_FrameBuffer;	
	NSTimer*						m_LoopTimer;
		
	UIPinchGestureRecognizer*		m_PinchGesture;
	UIRotationGestureRecognizer*	m_RotationGesture;
	UILongPressGestureRecognizer*	m_LongPressGesture;
	UITapGestureRecognizer*			m_TapGesture;
	UITapGestureRecognizer*			m_Tap2Gesture;
	UITapGestureRecognizer*			m_Tap3Gesture;
	UISwipeGestureRecognizer*		m_SwipeLeftGesture;
	UISwipeGestureRecognizer*		m_SwipeRightGesture;
	UISwipeGestureRecognizer*		m_SwipeUpGesture;
	UISwipeGestureRecognizer*		m_SwipeDownGesture;
}

@property (nonatomic, retain) EAGLContext *m_Context;

- (void)setupDesc;
- (void)Loop;
- (void)SetGestureRecognization:(bool)b;

@end
