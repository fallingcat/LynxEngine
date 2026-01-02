//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXANDROIDUTILITY_H__
#define __LYNXANDROIDUTILITY_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT jclass					FindClass(JNIEnv *env, const char* name);

	JNIEXPORT JNIEnv*					GetJVMEnv();

	void								OnGestureOff(JNIEnv *, jclass, jobject, int, long);

	jstring								NewModifiedUTF8String(JNIEnv *env, const LYNXCHAR* str);
	jobjectArray						NewUTF8StringArray(JNIEnv *env, const LynxEngine::CArray<LynxEngine::CString>& values);
	void								DelUTF8StringArray(JNIEnv *env, jobjectArray& jarray);

#ifdef __cplusplus
}
#endif

#define JNI_ATTACH_CURRENT_THREAD \					
JNIEnv* env = NULL; \
LYNXBOOL bAttached = LYNX_FALSE; \
LYNXBOOL bAttachRet = LYNX_FALSE; \
switch (static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp()->activity->vm->GetEnv((void**)&env, JNI_VERSION_1_6))\
{\
case JNI_OK:\
			bAttachRet = LYNX_TRUE; \
			break; \
case JNI_EDETACHED:\
				   if (static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp()->activity->vm->AttachCurrentThread(&env, NULL) != 0)\
					   bAttachRet = LYNX_FALSE; \
				   else\
				   {\
				   bAttachRet = LYNX_TRUE; \
				   bAttached = LYNX_TRUE; \
				   }\
				   break; \
case JNI_EVERSION:\
				  LYNX_LOG(glpLogger, "Fatal error. Invalid java version."); \
				  bAttachRet = LYNX_FALSE; \
				  break; \
}
#define JNI_DETACH_CURRENT_THREAD \
	if (bAttached && bAttachRet)\
		static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp()->activity->vm->DetachCurrentThread();


#endif
