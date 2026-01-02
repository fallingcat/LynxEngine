#ifndef __LYNXWEBHELPER_H__
#define __LYNXWEBHELPER_H__

#ifdef __WEB__

	#include <jni.h>
	#include <jawt.h>

	#ifdef __cplusplus
	extern "C" {
	#endif

	#undef LynxEngine_Canvas_FOCUS_TRAVERSABLE_UNKNOWN
	#define LynxEngine_Canvas_FOCUS_TRAVERSABLE_UNKNOWN 0L
	#undef LynxEngine_Canvas_FOCUS_TRAVERSABLE_DEFAULT
	#define LynxEngine_Canvas_FOCUS_TRAVERSABLE_DEFAULT 1L
	#undef LynxEngine_Canvas_FOCUS_TRAVERSABLE_SET
	#define LynxEngine_Canvas_FOCUS_TRAVERSABLE_SET 2L
	#undef LynxEngine_Canvas_TOP_ALIGNMENT
	#define LynxEngine_Canvas_TOP_ALIGNMENT 0.0f
	#undef LynxEngine_Canvas_CENTER_ALIGNMENT
	#define LynxEngine_Canvas_CENTER_ALIGNMENT 0.5f
	#undef LynxEngine_Canvas_BOTTOM_ALIGNMENT
	#define LynxEngine_Canvas_BOTTOM_ALIGNMENT 1.0f
	#undef LynxEngine_Canvas_LEFT_ALIGNMENT
	#define LynxEngine_Canvas_LEFT_ALIGNMENT 0.0f
	#undef LynxEngine_Canvas_RIGHT_ALIGNMENT
	#define LynxEngine_Canvas_RIGHT_ALIGNMENT 1.0f
	#undef LynxEngine_Canvas_serialVersionUID
	#define LynxEngine_Canvas_serialVersionUID -7644114512714619750i64
	#undef LynxEngine_Canvas_serialVersionUID
	#define LynxEngine_Canvas_serialVersionUID -2284879212465893870i64

	/*
	 * Class:     LynxEngine_Canvas
	 * Method:    Setup
	 * Signature: ()V
	 */
	//JNIEXPORT void JNICALL Java_LynxEngine_Canvas_Setup
	//(JNIEnv *, jobject);
	JNIEXPORT void JNICALL Java_LynxEngine_Canvas_paint
	(JNIEnv *, jobject, jobject);

	JNIEXPORT void JNICALL Java_LynxEngine_Kernel_SetMousePollData
	(JNIEnv *, jobject, jint, jint, jint);	

	#ifdef __cplusplus
	}
	#endif

#endif

#endif