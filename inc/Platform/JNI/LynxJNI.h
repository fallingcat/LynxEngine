#ifndef __LYNXJNI_H__
#define __LYNXJNI_H__

#if (defined(__WEB__) || defined(__ANDROID__))
	#include <jni.h>
	#ifdef __WIN32__
		#include <jawt.h>
	#endif

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

	
	JNIEXPORT void JNICALL 
	Java_LynxEngine_Kernel_NativeInit(JNIEnv* env, jobject kernel, jobject context, jint w, jint h, jint rendertype);

	#if defined(__ANDROID__)
		JNIEXPORT void JNICALL
		Java_LynxEngine_Android_LynxActivity_NativeInit(JNIEnv* env, jobject activity, jobject assetmanager);
		
		JNIEXPORT void JNICALL 
		Java_LynxEngine_Android_Renderer_Clear(JNIEnv * env, jobject obj, jfloat r, jfloat g, jfloat b);
	#endif

	JNIEXPORT void JNICALL
	Java_LynxEngine_LynxApplet_NativeInit(JNIEnv* env, jobject applet);

	JNIEXPORT void JNICALL 
	Java_LynxEngine_Kernel_Loop(JNIEnv* env, jobject kernel);

	JNIEXPORT void JNICALL 
	Java_LynxEngine_Kernel_SetMousePollData(JNIEnv *, jobject, jint, jint, jint);	

	JNIEXPORT void JNICALL
	Java_LynxEngine_Kernel_SetKeyboardPollData(JNIEnv* env, jobject kernel, jint key, jboolean bpressed);

	JNIEXPORT void JNICALL
	Java_LynxEngine_Kernel_SetCharPressed(JNIEnv* env, jobject kernel, jint c);

	#ifdef __cplusplus
	}
	#endif

#endif

#endif