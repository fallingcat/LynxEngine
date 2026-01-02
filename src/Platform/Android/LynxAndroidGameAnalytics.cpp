//
//  LynxGameAnalytics.cpp
//  LynxEngine
//
//  Created by Qubit on 2016/3/15.
//
//

#include <LynxEngine_PCH.h>
#include <Platform/Android/LynxAndroidGameAnalytics.h>

namespace LynxEngine
{
    namespace Android
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		CGameAnalytics::CGameAnalytics()
		{
		}
		//--------------------------------------------------------------------------------------------------- ----
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		CGameAnalytics::~CGameAnalytics(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::Config(const CString& build)
		{
			JNI_ATTACH_CURRENT_THREAD

			jclass cls = FindClass(env, "LynxEngine/Android/GameAnalytics");
			jmethodID config = env->GetStaticMethodID(cls, "Config", "(Ljava/lang/String;)V");
			jstring b = NewModifiedUTF8String(env, build.c_str());
			env->CallStaticVoidMethod(cls, config, b);

			env->DeleteLocalRef(b);
			env->DeleteLocalRef(cls);

			JNI_DETACH_CURRENT_THREAD
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::Init(const CString& gameKey, const CString& secretKey)
		{
			JNI_ATTACH_CURRENT_THREAD

			android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
			jclass cls = FindClass(env, "LynxEngine/Android/GameAnalytics");
			jmethodID init = env->GetStaticMethodID(cls, "Init", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;)V");
			jstring g = NewModifiedUTF8String(env, gameKey.c_str());
			jstring s = NewModifiedUTF8String(env, secretKey.c_str());
			env->CallStaticVoidMethod(cls, init, app->activity->clazz, g, s);

			env->DeleteLocalRef(g);
			env->DeleteLocalRef(s);

			JNI_DETACH_CURRENT_THREAD
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::ConfigureAvailableEvent(const GameAnalyticsSystem::AVAILABLEEVENTTYPE type, const CArray<CString>& values)
		{
			JNI_ATTACH_CURRENT_THREAD
							
			jclass cls = FindClass(env, "LynxEngine/Android/GameAnalytics");
			jmethodID configureAvailableEvent = env->GetStaticMethodID(cls, "ConfigureAvailableEvent", "(I[Ljava/lang/String;)V");			
			jint enumtype = (int)type;
			jobjectArray jarray = NewUTF8StringArray(env, values);

			env->CallStaticVoidMethod(cls, configureAvailableEvent, enumtype, jarray);
			
			DelUTF8StringArray(env, jarray);

			JNI_DETACH_CURRENT_THREAD
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::AddProgressionEvent(const GameAnalyticsSystem::PROGRESSSTATUS progressionStatus, const CString& progression01, const CString& progression02, const CString& progression03)
		{
			JNI_ATTACH_CURRENT_THREAD

			jclass cls = FindClass(env, "LynxEngine/Android/GameAnalytics");
			jmethodID addProgressionEvent = env->GetStaticMethodID(cls, "AddProgressionEvent", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			jint ps = progressionStatus;
			jstring p1 = NewModifiedUTF8String(env, progression01.c_str());
			jstring p2 = NewModifiedUTF8String(env, progression02.c_str());
			jstring p3 = NewModifiedUTF8String(env, progression03.c_str());
			env->CallStaticVoidMethod(cls, addProgressionEvent, ps, p1, p2, p3);

			env->DeleteLocalRef(p1);
			env->DeleteLocalRef(p2);
			env->DeleteLocalRef(p3);

			JNI_DETACH_CURRENT_THREAD
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// 
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::AddDesignEvent(const CString& eventId, const CString& stack)
		{
			JNI_ATTACH_CURRENT_THREAD

			jclass cls = FindClass(env, "LynxEngine/Android/GameAnalytics");
			jmethodID addDesignEvent = env->GetStaticMethodID(cls, "AddDesignEvent", "(Ljava/lang/String;)V");
			jstring e = NewModifiedUTF8String(env, eventId.c_str());
			env->CallStaticVoidMethod(cls, addDesignEvent, e);

			env->DeleteLocalRef(e);

			JNI_DETACH_CURRENT_THREAD
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// 
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::AddResourceEvent(const GameAnalyticsSystem::RESOURCEFLOWTYPE flow, const CString& type, const CString& id, const float amount, const CString& currency)
		{
			JNI_ATTACH_CURRENT_THREAD

			jclass cls = FindClass(env, "LynxEngine/Android/GameAnalytics");

			jmethodID addResourceEvent = env->GetStaticMethodID(cls, "AddResourceEvent", "(ILjava/lang/String;FLjava/lang/String;Ljava/lang/String;)V");
			jint jflowtype = flow;
			jstring jtype = NewModifiedUTF8String(env, type.c_str());			
			jstring jid = NewModifiedUTF8String(env, id.c_str());
			jfloat jamount = amount;
			jstring jcurrency = NewModifiedUTF8String(env, currency.c_str());
			
			env->CallStaticVoidMethod(cls, addResourceEvent, jflowtype, jcurrency, jamount, jtype, jid);

			env->DeleteLocalRef(jtype);
			env->DeleteLocalRef(jid);
			env->DeleteLocalRef(jcurrency);

			JNI_DETACH_CURRENT_THREAD
		}
		//-------------------------------------------------------------------------------------------------------
		//				
		//  example for Java implementation.
		// 
		//	GameAnalytics.AddBusinessEvent(skuDetail.getPriceCurrencyCode(),
		//								   skuDetail.getPriceAmountMicros() / 1000000, // Amount in cents
		//								   purchase.getItemType(),
		//								   purchase.getSku(),
		//								   "ruby",
		//								   purchase.getOriginalJson(),
		//								   "google_play",
		//								   purchase.getSignature());
		//
		//  Ref: https://github.com/GameAnalytics/GA-SDK-Android/wiki/Business%20Event
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::AddBusinessEvent(const CString& currency,
											  const LYNXDOUBLE amount,
											  const CString& itemType,
											  const CString& itemId,
											  const CString& cartType,
											  const CString& receipt,
											  const CString& store,
											  const CString& signature)
		{			
			JNI_ATTACH_CURRENT_THREAD

			jclass cls = FindClass(env, "LynxEngine/Android/GameAnalytics");
			jmethodID addBusinessEvent = env->GetStaticMethodID(cls, "AddBusinessEvent", "(Ljava/lang/String;DLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			
			jstring jcurrency = NewModifiedUTF8String(env, currency.c_str());
			jdouble jamount = amount;
			jstring jitemType = NewModifiedUTF8String(env, itemType.c_str());
			jstring jitemId = NewModifiedUTF8String(env, itemId.c_str());
			jstring jcartType = NewModifiedUTF8String(env, cartType.c_str());
			jstring jreceipt = NewModifiedUTF8String(env, receipt.c_str());
			jstring jstore = NewModifiedUTF8String(env, store.c_str());
			jstring jsignature = NewModifiedUTF8String(env, signature.c_str());
			
			env->CallStaticVoidMethod(cls, addBusinessEvent, jcurrency, jamount, jitemType, jitemId, jcartType, jreceipt, jstore, jsignature);

			env->DeleteLocalRef(jcurrency);			
			env->DeleteLocalRef(jitemType);
			env->DeleteLocalRef(jitemId);
			env->DeleteLocalRef(jcartType);
			env->DeleteLocalRef(jreceipt);
			env->DeleteLocalRef(jstore);
			env->DeleteLocalRef(jsignature);
			
			JNI_DETACH_CURRENT_THREAD
		}			
	}
}
