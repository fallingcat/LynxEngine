//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Kasaki Tzeng : 2015/04/14
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <Platform/Android/LynxAndroidUtility.h>
#include <Platform/Android/LynxAndroidJNI.h>

#if defined(__ANDROID__)

#include <android/log.h>

#define LOG_TAG    "LynxEngine"
#define ANDROID_LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define ANDROID_LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define ANDROID_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define ANDROID_LOG_FOOT   ANDROID_LOGD("%s %s %d", __FILE__, __FUNCTION__, __LINE__)

#define ANDROID_DEBUG_IAP_JSONVALUE

using namespace LynxEngine;

/**
 * Implement of JNI interface
 *
 * @description
 *    DON'T call Java method in JNI implement, you should keep data and call Java method after callback finish. 
 */

enum {
	AMOTION_EVENT_GESTURE_START = AMOTION_EVENT_ACTION_HOVER_EXIT, // 10
	AMOTION_EVENT_GESTURE_DOWN,
	AMOTION_EVENT_GESTURE_SCROLL,
    AMOTION_EVENT_GESTURE_SINGLETAPUP,
    AMOTION_EVENT_GESTURE_SHOWPRESS,
    AMOTION_EVENT_GESTURE_LONGPRESS,
	AMOTION_EVENT_GESTURE_FLING,
	AMOTION_EVENT_GESTURE_DOUBLETAP,
};

static jobject gClassLoader;
static jmethodID gFindClassMethod;

JNIEnv* GetJVMEnv()
{
	JNIEnv *env;
	int status = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (status < 0)
	{
		status = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetJavaVM()->AttachCurrentThread(&env, NULL);
		if (status < 0)
		{
			return NULL;
		}
	}
	return env;
}

jclass FindClass(JNIEnv *env, const char* name)
{
	jstring JName = env->NewStringUTF(name);
	jclass Ret = static_cast<jclass>(env->CallObjectMethod(gClassLoader, gFindClassMethod, JName));
	env->DeleteLocalRef(JName);

	return Ret;
}

jstring NewModifiedUTF8String(JNIEnv *env, const LYNXCHAR* str)
{
	//return env->NewStringUTF(str);

	LYNXCHAR* NewStr = LYNXNEW LYNXCHAR[lynxStrLen(str) + 10];
	lynxModifiedUTF8String(NewStr, str);
	//lynxStrCpy(NewStr, str);
	//return env->NewStringUTF(NewStr);

	int Len = lynxStrLen(NewStr);
	jbyteArray array = env->NewByteArray(Len);
	env->SetByteArrayRegion(array, 0, Len, (const jbyte*)NewStr);
	jstring strEncode = env->NewStringUTF("UTF-8");
	jclass cls = env->FindClass("java/lang/String");
	jmethodID ctor = env->GetMethodID(cls, "<init>", "([BLjava/lang/String;)V");
	jstring object = (jstring)env->NewObject(cls, ctor, array, strEncode);

	LYNXDEL_ARRAY(NewStr);

	return object;
}

jobjectArray NewUTF8StringArray(JNIEnv *env, const LynxEngine::CArray<LynxEngine::CString>& values)
{
	jsize len = values.size();
	jclass clz = env->FindClass("java/lang/String");
	jobjectArray jarray = env->NewObjectArray(len, clz, NULL);
	jstring value;

	for (jsize i = 0; i < len; ++i)
	{
		value = NewModifiedUTF8String(env, values[i]);
		env->SetObjectArrayElement(jarray, i, value);
	}

	return jarray;
}

void DelUTF8StringArray(JNIEnv *env, jobjectArray& jarray)
{
	int len = env->GetArrayLength(jarray);

	for (int i = 0; i < len; ++i)
	{
		jobject obj = env->GetObjectArrayElement(jarray, i);
		env->DeleteLocalRef(obj);
	}
	env->DeleteLocalRef(jarray);
}

//-------------------------------------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------------------------------------

JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_OnLoad(JNIEnv *env, jclass clz, jobject activity)
{    	
    jclass activityClass = env->GetObjectClass(activity);	    
    jmethodID getClassLoaderMethod = env->GetMethodID(activityClass, "getClassLoader",
                                             "()Ljava/lang/ClassLoader;");
    jobject Temp = env->CallObjectMethod(activity, getClassLoaderMethod);
	gClassLoader = env->NewGlobalRef(Temp);    

	jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
	gFindClassMethod = env->GetMethodID(classLoaderClass, "findClass",
                                    "(Ljava/lang/String;)Ljava/lang/Class;");

	//env->DeleteGlobalRef(Temp);
}

JNIEXPORT jboolean JNICALL
Java_LynxEngine_Android_JNI_IsApplicationInForeground(JNIEnv *env, jclass clz)
{
	return (glpKernel) ? glpKernel->IsRunning() : LYNX_TRUE ;
}

JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_OnBackPressed(JNIEnv *env, jclass clz)
{
	LYNX_LOG(glpLogger, CString("JNI OnBackPressed"));

	glpKernel->vOnSysBackPressed();
}

void OnGestureOff (JNIEnv *env, jclass clz, jobject eventObj, int pointerCount, long timeStamp)
{
	InputSystem::CTouch* lpTouch = static_cast<InputSystem::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
	if (NULL == lpTouch)	
		return;

	jclass clazz = env->GetObjectClass(eventObj);
	jmethodID mid;
	int pointerId = -1, pointerIndex = -1;
	float x = 0, y = 0;	

	InputSystem::CTouch::TOUCHDATA TouchData;
	//lpTouch->SetTouchGesture(InputSystem::CTouch::TG_NULL);
	TouchData.TimeStamp = timeStamp;
	TouchData.TapCount = pointerCount;

	for (int i=0 ; i<pointerCount ; ++i)
	{
		mid = env->GetMethodID(clazz, "getPointerId", "(I)I");
		pointerId = env->CallIntMethod(eventObj, mid, i);
		mid = env->GetMethodID(clazz, "findPointerIndex", "(I)I");
		pointerIndex = env->CallIntMethod(eventObj, mid, pointerId);
		mid = env->GetMethodID(clazz, "getX", "(I)F");
		x = env->CallFloatMethod(eventObj, mid, pointerIndex) * glpKernel->m_InputScaleFactor;
		mid = env->GetMethodID(clazz, "getY", "(I)F");
		y = env->CallFloatMethod(eventObj, mid, pointerIndex) * glpKernel->m_InputScaleFactor;

		TouchData.Pos = Math::CVector2(x, y);
		if (i==0) lpTouch->ClearTouches();
		lpTouch->AddTouch(TouchData);
		//LYNX_LOG(glpLogger, CString("JNI OnGestureOff"));
	}
}

JNIEXPORT void JNICALL Java_LynxEngine_Android_JNI_OnTouchEvent (JNIEnv *env, jclass clz, jint action, jobject eventObj)
{
	InputSystem::CTouch* lpTouch = static_cast<InputSystem::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
	if (NULL == lpTouch)	
		return;

	jclass clazz = env->GetObjectClass(eventObj);
	jmethodID mid;
	mid = env->GetMethodID(clazz, "getPointerCount", "()I");
	int pointerCount = env->CallIntMethod(eventObj, mid);
	mid = env->GetMethodID(clazz, "getEventTime", "()J");
	long timeStamp = env->CallLongMethod(eventObj, mid);
	int pointerId = -1, pointerIndex = -1;
	float x = 0, y = 0;	
	int eventAction = (int)action;

	InputSystem::CTouch::TOUCHDATA TouchData;
	//lpTouch->SetTouchGesture(InputSystem::CTouch::TG_NULL);
	TouchData.TimeStamp = timeStamp;
	TouchData.TapCount = pointerCount;

	if (LYNX_FALSE == glpKernel->IsGestureRecognization())
	{
		if (eventAction == AMOTION_EVENT_ACTION_UP)
			lpTouch->ClearTouches();
		else {
			if (eventAction == AMOTION_EVENT_ACTION_DOWN)
				lpTouch->SetTapGesture();
			else if (eventAction == AMOTION_EVENT_ACTION_MOVE)
				lpTouch->SetTouchGesture(LynxEngine::InputSystem::CTouch::TG_PAN);
			OnGestureOff (env, clz, eventObj, pointerCount, timeStamp);
		}
		return;
	}
}
	
JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_OnTouch
(JNIEnv *env, jclass clz, jint action, jobject eventObj)
{
	InputSystem::CTouch* lpTouch = static_cast<InputSystem::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
	if (NULL == lpTouch)	
		return;
	
	// find java class "MotionEvent"
	jclass clazz = env->GetObjectClass(eventObj);

	jmethodID mid_getClass = env->GetMethodID(clazz, "getClass", "()Ljava/lang/Class;");
	jobject clazzObj = env->CallObjectMethod(eventObj, mid_getClass);
	// Now get the class object's class descriptor
	jclass cls = env->GetObjectClass(clazzObj);
	// Find the getName() method on the class object
	jmethodID mid_getName = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
	jstring name = (jstring)env->CallObjectMethod(clazz, mid_getName);
	const char *str= env->GetStringUTFChars(name, 0);
	LYNX_LOG(glpLogger, CString("JNI") + CString(str));
	env->ReleaseStringUTFChars(name, str);

	jmethodID mid;
	mid = env->GetMethodID(clazz, "getPointerCount", "()I");
	int pointerCount = env->CallIntMethod(eventObj, mid);
	mid = env->GetMethodID(clazz, "getEventTime", "()J");
	long timeStamp = env->CallLongMethod(eventObj, mid);
	int pointerId = -1, pointerIndex = -1;
	float x = 0, y = 0;	
	
	int eventAction = (int)action;

	if (LYNX_FALSE == glpKernel->IsGestureRecognization())
	{
		return;
	}

	int GestureState = glpKernel->GetGestureState();

	switch(eventAction)
	{
		// TAP2
		case AMOTION_EVENT_GESTURE_DOUBLETAP:
		if (GestureState & LynxEngine::GameFramework::CKernel::TAP_GESTURE)
		{
			lpTouch->SetTap2Gesture();
			lpTouch->ClearTouches();
            
			mid = env->GetMethodID(clazz, "getX", "()F");
			x = env->CallFloatMethod(eventObj, mid) * glpKernel->m_InputScaleFactor;
			mid = env->GetMethodID(clazz, "getY", "()F");
			y = env->CallFloatMethod(eventObj, mid) * glpKernel->m_InputScaleFactor;

			InputSystem::CTouch::TOUCHDATA TouchData;
			TouchData.TimeStamp = timeStamp;
			TouchData.TapCount = 2;
			TouchData.Pos = Math::CVector2(x, y);
			lpTouch->AddTouch(TouchData);
		}
		else 
		{
			OnGestureOff (env, clz, eventObj, pointerCount, timeStamp);
		}
		break;

		// TAP
		case AMOTION_EVENT_GESTURE_SINGLETAPUP:
		if (GestureState & LynxEngine::GameFramework::CKernel::TAP_GESTURE)
		{
			lpTouch->SetTapGesture();
			lpTouch->ClearTouches();
    
			mid = env->GetMethodID(clazz, "getX", "()F");
			x = env->CallFloatMethod(eventObj, mid) * glpKernel->m_InputScaleFactor;
			mid = env->GetMethodID(clazz, "getY", "()F");
			y = env->CallFloatMethod(eventObj, mid) * glpKernel->m_InputScaleFactor;

			InputSystem::CTouch::TOUCHDATA TouchData;
			TouchData.TimeStamp = timeStamp;
			TouchData.TapCount = 1;
			TouchData.Pos = Math::CVector2(x, y);
			lpTouch->AddTouch(TouchData);         
		}
		else 
		{
			OnGestureOff (env, clz, eventObj, pointerCount, timeStamp);
		}
		break;

		case AMOTION_EVENT_GESTURE_LONGPRESS: 
		if (GestureState & LynxEngine::GameFramework::CKernel::LONGPRESS_GESTURE)
		{
			lpTouch->SetLongPressGesture();
			lpTouch->ClearTouches();
    
			mid = env->GetMethodID(clazz, "getX", "()F");
			x = env->CallFloatMethod(eventObj, mid) * glpKernel->m_InputScaleFactor;
			mid = env->GetMethodID(clazz, "getY", "()F");
			y = env->CallFloatMethod(eventObj, mid) * glpKernel->m_InputScaleFactor;

			InputSystem::CTouch::TOUCHDATA TouchData;
			TouchData.TimeStamp = timeStamp;
			TouchData.TapCount = 1;
			TouchData.Pos = Math::CVector2(x, y);
			lpTouch->AddTouch(TouchData); 
		}
		else 
		{
			OnGestureOff (env, clz, eventObj, pointerCount, timeStamp);
		}
		break;
	} // End switch

	ANDROID_LOGD("JNIObject_OnTouch::pointerCount=%d, action=%d", 
		pointerCount, eventAction);
} // End of OnTouch

JNIEXPORT void 
JNICALL Java_LynxEngine_Android_JNI_OnScroll
(JNIEnv *env, jclass clz, jint action, jobject eventObj1, jobject eventObj2, jfloat diffx, jfloat diffy)
{
	InputSystem::CTouch* lpTouch = static_cast<InputSystem::CTouch*>(glpKernel->GetlpEngine()->GetlpSystemTouch());
	if (NULL == lpTouch)	
		return;

	// find java class "MotionEvent"
	jclass clazz1 = env->GetObjectClass(eventObj1);
	jclass clazz2 = env->GetObjectClass(eventObj2);
	jmethodID mid;
	mid = env->GetMethodID(clazz1, "getPointerCount", "()I");
	int pointerCount = env->CallIntMethod(eventObj1, mid);
	mid = env->GetMethodID(clazz2, "getEventTime", "()J");
	long timeStamp = env->CallLongMethod(eventObj2, mid);
	int pointerId = -1, pointerIndex = -1;
	float x1=0,y1=0,x2=0,y2=0;
	float deltaTime = 0.02f * glpKernel->m_Step;
	float panscale = 0.5f;
	if (LYNX_FALSE == glpKernel->IsGestureRecognization())
	{
		return;
	}
	int GestureState = glpKernel->GetGestureState();

	int eventAction = (int)action;
	switch(eventAction)
	{
		// PAN
		case AMOTION_EVENT_GESTURE_SCROLL:
			if (GestureState & LynxEngine::GameFramework::CKernel::PAN_GESTURE)
			{
				mid = env->GetMethodID(clazz1, "getX", "()F");
				x1 = env->CallFloatMethod(eventObj1, mid) * glpKernel->m_InputScaleFactor;
				mid = env->GetMethodID(clazz1, "getY", "()F");
				y1 = env->CallFloatMethod(eventObj1, mid) * glpKernel->m_InputScaleFactor;

				mid = env->GetMethodID(clazz2, "getX", "()F");
				x2 = env->CallFloatMethod(eventObj2, mid) * glpKernel->m_InputScaleFactor;
				mid = env->GetMethodID(clazz2, "getY", "()F");
				y2 = env->CallFloatMethod(eventObj2, mid) * glpKernel->m_InputScaleFactor;

				LynxEngine::Math::CVector2 Trans, Vel;
				Trans.x = x2-x1;
				Trans.y = y2-y1;
				Vel.x = -diffx / deltaTime * glpKernel->m_InputScaleFactor * panscale;
				Vel.y = -diffy / deltaTime * glpKernel->m_InputScaleFactor * panscale;
				lpTouch->SetPanGesture(Trans, Vel);

				InputSystem::CTouch::TOUCHDATA TouchData;
				//TouchData.TimeStamp = timeStamp;
				TouchData.TapCount = 1;
				TouchData.Pos = Math::CVector2(x1, y1);
				lpTouch->AddTouch(TouchData);
			}
			else
			{
				OnGestureOff (env, clz, eventObj2, pointerCount, timeStamp);
			}
			break;

		// Swipe
		case AMOTION_EVENT_GESTURE_FLING:
			if (GestureState & LynxEngine::GameFramework::CKernel::SWIPE_GESTURE)
			{
				LynxEngine::Math::CVector2 dir;
				dir.x = diffx;
				dir.y = diffy;
				lpTouch->SetSwipeGesture(dir);

				mid = env->GetMethodID(clazz1, "getX", "()F");
				x1 = env->CallFloatMethod(eventObj1, mid) * glpKernel->m_InputScaleFactor;
				mid = env->GetMethodID(clazz1, "getY", "()F");
				y1 = env->CallFloatMethod(eventObj1, mid) * glpKernel->m_InputScaleFactor;

				InputSystem::CTouch::TOUCHDATA TouchData;
				//TouchData.TimeStamp = timeStamp;
				TouchData.TapCount = 1;
				TouchData.Pos = Math::CVector2(x1, y1);
				lpTouch->AddTouch(TouchData);        
			}
			else
			{
				OnGestureOff (env, clz, eventObj2, pointerCount, timeStamp);
			}
			break;
	}

	ANDROID_LOGD("JNIObject_OnScroll::pointerCount=%d, avtion=%d", 
		pointerCount, eventAction);
} // End of OnScroll

JNIEXPORT void 
JNICALL Java_LynxEngine_Android_JNI_OnQueryInventoryFinished
(JNIEnv *env, jclass jclazz, jobjectArray jobjArray)
{
	ANDROID_LOGD("JNIObject_OnQueryInventoryFinished");

	const CString StripTokenPrefix = "SkuDetails:";
	CArray<CJsonValue> ProductList;

	int arrayLength = env->GetArrayLength(jobjArray);
    for (int i=0; i<arrayLength; i++) 
	{
        jstring jstr = (jstring)env->GetObjectArrayElement(jobjArray, i);
        const char *cStr = env->GetStringUTFChars(jstr, 0);
		if (cStr)
		{
			CJsonValue SkuDetails;
			CString OrgStr = CString(cStr);
			CEngine::GetJSONValues(SkuDetails, OrgStr.erase(0, StripTokenPrefix.size()));

#if defined(ANDROID_DEBUG_IAP_JSONVALUE)
			/**
			JSON string format =

			SkuDetails:
			{
				"title":"gas (QubotTest)",
				"price":"NT$30.00",
				"type":"inapp",
				"description":"test product",
				"price_amount_micros":30000000,
				"price_currency_code":"TWD",
				"productId":"gas"
			}
			*/

			LYNXCHAR Buffer[256];
			lynxSprintf(Buffer, "productId=%s, type=%s, price=%s, title=%s, description=%s\n", 
				SkuDetails["productId"].JSON_VALUE_AS_CSTRING(), 
				SkuDetails["type"].JSON_VALUE_AS_CSTRING(), 
				SkuDetails["price"].JSON_VALUE_AS_CSTRING(),
				SkuDetails["title"].JSON_VALUE_AS_CSTRING(), 
				SkuDetails["description"].JSON_VALUE_AS_CSTRING());
			LYNX_LOG(glpLogger, Buffer);
#endif
			ProductList.push_back(SkuDetails);
		}
		env->ReleaseStringUTFChars(jstr, cStr);
	} // End for

	LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;		
	if (lpKernel)
	{
		lpKernel->vOnFinishingQueryIAPList(ProductList);
	}
} // End of OnQueryInventoryFinished

JNIEXPORT jboolean 
JNICALL Java_LynxEngine_Android_JNI_OnVerifyDeveloperPayload
(JNIEnv* env, jclass clazz, jstring jtype, jstring jstr, jstring jmetaData1, jstring jmetaData2)
{
	ANDROID_LOGD("JNIObject_OnVerifyDeveloperPayload");

	const char *cTypeStr = env->GetStringUTFChars(jtype, 0);
	const char *cStr = env->GetStringUTFChars(jstr, 0);
	const char *cOriginialJson = env->GetStringUTFChars(jmetaData1, 0);
	const char *cSignature = env->GetStringUTFChars(jmetaData2, 0);
	LYNXBOOL bResult = LYNX_FALSE;

	if (cTypeStr && cStr)
	{		

#if defined(ANDROID_DEBUG_IAP_JSONVALUE)
		/**
		JSONStr format

			PurchaseInfo(type:inapp):
			{
			"packageName":"com.qubitent.qubot.test",
			"productId":"qubottest_item_001",
			"purchaseTime":1468384260456,
			"purchaseState":0,
			"developerPayload":"AAAAA",
			"purchaseToken":"pdegploilogcbfplhjgbppgm.AO-J1OyveVvzfQdTj1g2T3UE89bgEB8mRWv3StRy0AYeDryhHjij5lG9l3Q_2SNLMu7nmFlNDH5ivo3IKzEjVgwkKk075UUMSZUyy1I5b6eYNzEZNVFHrpI9HhP6CVK2TUpc55hLq9IU"
		}
		*/
#endif

		const CString StripTokenPrefix = "PurchaseInfo(type:" + CString(cTypeStr) + "):";
		CJsonValue Purchase;
		CString OrgStr = CString(cStr);
		CEngine::GetJSONValues(Purchase, OrgStr.erase(0, StripTokenPrefix.size()));
	
		Purchase["itemType"] = CJsonValue(cTypeStr);
		if (cOriginialJson) { Purchase["originalJson"] = CJsonValue(cOriginialJson); };
		if (cSignature) { Purchase["signature"] = CJsonValue(cSignature); }

		LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;		
		if (lpKernel)
		{
			bResult = lpKernel->vOnVerifyPurchase(Purchase);
		}
	}

	env->ReleaseStringUTFChars(jtype, cTypeStr);
	env->ReleaseStringUTFChars(jstr, cStr);
	env->ReleaseStringUTFChars(jmetaData1, cOriginialJson);
	env->ReleaseStringUTFChars(jmetaData2, cSignature);

	return bResult;
} // End of OnVerifyDeveloperPayload


JNIEXPORT jboolean
JNICALL Java_LynxEngine_Android_JNI_OnVerifyPurchasePromoCode
(JNIEnv* env, jclass clazz, jstring jtype, jstring jstr, jstring jmetaData1, jstring jmetaData2)
{
	ANDROID_LOGD("JNIObject_OnVerifyPurchasePromoCode");

	const char *cTypeStr = env->GetStringUTFChars(jtype, 0);
	const char *cStr = env->GetStringUTFChars(jstr, 0);
	const char *cOriginialJson = env->GetStringUTFChars(jmetaData1, 0);
	const char *cSignature = env->GetStringUTFChars(jmetaData2, 0);
	LYNXBOOL bResult = LYNX_FALSE;

	if (cTypeStr && cStr)
	{
		const CString StripTokenPrefix = "PurchaseInfo(type:" + CString(cTypeStr) + "):";
		CJsonValue Purchase;
		CString OrgStr = CString(cStr);
		CEngine::GetJSONValues(Purchase, OrgStr.erase(0, StripTokenPrefix.size()));

		Purchase["itemType"] = CJsonValue(cTypeStr);
		if (cOriginialJson) { Purchase["originalJson"] = CJsonValue(cOriginialJson); };
		if (cSignature) { Purchase["signature"] = CJsonValue(cSignature); }

		LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;
		if (lpKernel)
		{
			bResult = lpKernel->vOnVerifyPromoCodePurchase(Purchase);
		}
	}

	env->ReleaseStringUTFChars(jtype, cTypeStr);
	env->ReleaseStringUTFChars(jstr, cStr);
	env->ReleaseStringUTFChars(jmetaData1, cOriginialJson);
	env->ReleaseStringUTFChars(jmetaData2, cSignature);

	return bResult;

} // End of OnVerifyPurchasePromoCode


JNIEXPORT void 
JNICALL Java_LynxEngine_Android_JNI_OnConsumeFinished
(JNIEnv *env, jclass jclazz, jint jresult, jstring jmessage, jstring jtype, jstring jjaonstr)
{
	ANDROID_LOGD("JNIObject_OnConsumeFinished");

	int Result = (int)jresult;
	const char *szMessage = env->GetStringUTFChars(jmessage, 0);
	const char *szType = env->GetStringUTFChars(jtype, 0);
	const char *szJSONString = env->GetStringUTFChars(jjaonstr, 0);

	CJsonValue Purchase;
	CString Message;
	if (szType && szJSONString)
	{
		const CString StripTokenPrefix = "PurchaseInfo(type:" + CString(szType) + "):";
		
		CString OrgStr = CString(szJSONString);
		CEngine::GetJSONValues(Purchase, OrgStr.erase(0, StripTokenPrefix.size()));
	}

	if (szMessage)
		Message = CString(szMessage);

	LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;		
	if (lpKernel)
	{
		lpKernel->vOnDeliverPurchase(Result, Message, Purchase);		
	}

	env->ReleaseStringUTFChars(jmessage, szMessage);
	env->ReleaseStringUTFChars(jtype, szType);
	env->ReleaseStringUTFChars(jjaonstr, szJSONString);
} // End of OnConsumeFinished

JNIEXPORT void
JNICALL Java_LynxEngine_Android_JNI_OnIapPurchaseFinished
(JNIEnv *env, jclass jobject, jint jresult, jstring jmessage)
{
	ANDROID_LOGD("JNIObject_OnIapPurchaseFinished");

	int Result = (int)jresult;
	const char *szMessage = env->GetStringUTFChars(jmessage, 0);
	CString Message(szMessage);

	LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;
	if (lpKernel)
	{
		lpKernel->vOnFinishingPurchase(Result, Message);
	}
	
	env->ReleaseStringUTFChars(jmessage, szMessage);
} // End of OnIapPurchaseFailed

JNIEXPORT void 
JNICALL Java_LynxEngine_Android_JNI_OnIapPurchaseFailed
(JNIEnv *env, jclass jobject, jint jresult, jstring jmessage)
{
	ANDROID_LOGD("JNIObject_OnIapPurchaseFailed");

	int Result = (int)jresult;
	const char *szMessage = env->GetStringUTFChars(jmessage, 0);
	CString Message(szMessage);

	LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;
	if (lpKernel)
	{
		lpKernel->vOnErrorPurchase(Result, Message);
	}

	env->ReleaseStringUTFChars(jmessage, szMessage);	
} // End of OnIapPurchaseFailed

JNIEXPORT void 
JNICALL Java_LynxEngine_Android_JNI_OnAlertDialogClick
(JNIEnv *env, jclass jobject, jint jindex)
{
	ANDROID_LOGD("JNIObject_OnAlertDialogClick");
}

JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_UpdateText(JNIEnv *env, jclass clz, jlong ptr, jstring str)
{
	GUISystem::CText* Text = (GUISystem::CText*)ptr;
	if (str != NULL)
	{
		CString cstr = _T(env->GetStringUTFChars(str, 0));
		LYNXCHAR* NewStr = LYNXNEW LYNXCHAR[cstr.size() + 10];		
		lynxModifiedUTF8String(NewStr, cstr.c_str());
						
		Text->SetUserData(NewStr);
		Text->SetText(CEngine::ProfanityFilterString(CString(NewStr), glpKernel->GetlpGame()->GetProfanityTable()));

		LYNXDEL_ARRAY(NewStr);
	}
	Text->Activate(LYNX_TRUE);
}

JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_SetDownloadTotalSize(JNIEnv *env, jclass clz, jlong size)
{
	CPlatformURLStream* URLStream = glpKernel->GetlpEngine()->GetlpFileSystem()->GetUpdate()->GetPlatformURLStream();
	URLStream->m_TotalSize = size;
}

JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_SetDownloadProgressSize(JNIEnv *env, jclass clz, jlong size)
{
	CPlatformURLStream* URLStream = glpKernel->GetlpEngine()->GetlpFileSystem()->GetUpdate()->GetPlatformURLStream();
	URLStream->m_CurrentProgressSize = size;
}

JNIEXPORT void JNICALL
Java_LynxEngine_Android_JNI_OnFinishFacebookLogin(JNIEnv *env, jclass clz, jstring str)
{
	CString cstr = "";
	if (str != NULL)
	{
		cstr = _T(env->GetStringUTFChars(str, 0));
	}

	glpKernel->vOnFinishFacebookLogin(cstr);
}

JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_OnFinishQueryFacebookAccountData(JNIEnv *env, jclass clz, jstring str)
{
	if (str != NULL)
	{
		CString cstr = _T(env->GetStringUTFChars(str, 0));
		glpKernel->vOnFinishQueryFacebookAccountData(cstr);
	}
}

JNIEXPORT void JNICALL
Java_LynxEngine_Android_JNI_OnFinishQueryFacebookFriendsData(JNIEnv *env, jclass clz, jstring str)
{
	if (str != NULL)
	{
		CString cstr = _T(env->GetStringUTFChars(str, 0));
		glpKernel->vOnFinishQueryFacebookFriendsData(cstr);
	}
}

JNIEXPORT void JNICALL
Java_LynxEngine_Android_JNI_OnFinishFacebookShareLink(JNIEnv *env, jclass clz, jstring str)
{
	if (str != NULL)
	{
		CString cstr = _T(env->GetStringUTFChars(str, 0));
		glpKernel->vOnFinishFacebookShareLink(cstr);
	}

}

JNIEXPORT void JNICALL
Java_LynxEngine_Android_JNI_OnFinishFacebookUploadPic(JNIEnv *env, jclass clz, jstring str)
{
	if (str != NULL)
	{
		CString cstr = _T(env->GetStringUTFChars(str, 0));
		glpKernel->vOnFinishFacebookUploadPic(cstr);
	}

}

JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_HandleURL(JNIEnv *env, jclass clz, jstring key, jstring value)
{
	if (key!= NULL  && value != NULL)
	{
		CString ckey = _T(env->GetStringUTFChars(key, 0));
		CString cvalue = _T(env->GetStringUTFChars(value, 0));

		glpKernel->vHandleURLQueryParam(ckey, cvalue);
	}
}


JNIEXPORT void JNICALL
Java_LynxEngine_Android_JNI_SetAdvertisingIDString(JNIEnv *env, jclass clz, jstring value)
{
	if (value != NULL)
	{
		CString cvalue = _T(env->GetStringUTFChars(value, 0));
		glpKernel->SetAdvertisingIDString(cvalue);
	}
}

JNIEXPORT void JNICALL 
Java_LynxEngine_Android_JNI_OnClickRatingDialog(JNIEnv *env, jclass clz, jint value)
{	
	LynxEngine::GameFramework::Android::CKernel* lpKernel = (LynxEngine::GameFramework::Android::CKernel*)glpKernel;
	if (lpKernel)
	{
		lpKernel->vOnClickRatingDialog(value);
	}	
}

#endif // End of platform define
