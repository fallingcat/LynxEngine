//
//  LynxGameAnalytics.cpp
//  LynxEngine
//
//  Created by Qubit on 2016/3/15.
//
//

#include <GameAnalyticsSystem/LynxGameAnalytics.h>

namespace LynxEngine
{
    namespace GameAnalyticsSystem
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
        void CGameAnalytics::Config(CString build)
        {
			return;

        #ifdef __iOS__
			NSString *Build;
            lynxcString2NSString(&Build, build.c_str());
            [GameAnalytics configureBuild:Build];
            
            /*[GameAnalytics configureAvailableResourceCurrencies:@[@"gems", @"gold"]];
            [GameAnalytics configureAvailableResourceItemTypes:@[@"boost", @"lives"]];
            [GameAnalytics configureAvailableCustomDimensions01:@[@"ninja", @"samurai"]];
            [GameAnalytics configureAvailableCustomDimensions02:@[@"whale", @"dolphin"]];
            [GameAnalytics configureAvailableCustomDimensions03:@[@"horde", @"alliance"]];*/
		#elif defined __ANDROID__
			android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
			JNIEnv* env;
			app->activity->vm->AttachCurrentThread(&env, NULL);
			jclass cls = FindClass("LynxEngine/Android/GameAnalytics");
			jmethodID config = env->GetStaticMethodID(cls, "Config", "(Ljava/lang/String;)V");
			jstring b = env->NewStringUTF(build.c_str());
			env->CallStaticVoidMethod(cls, config, b);

			env->DeleteLocalRef(b);
			env->DeleteLocalRef(cls);
			app->activity->vm->DetachCurrentThread();
        #endif
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  ª°©˙:
        //-------------------------------------------------------------------------------------------------------
        void CGameAnalytics::Init(CString gameKey, CString secretKey)
        {
			return;

        #ifdef __iOS__
            NSString *GameKey, *SecretKey;
            lynxcString2NSString(&GameKey, gameKey.c_str());
            lynxcString2NSString(&SecretKey, secretKey.c_str());
            [GameAnalytics initializeWithGameKey:GameKey gameSecret:SecretKey];
		#elif defined __ANDROID__
			android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
			JNIEnv* env;
			app->activity->vm->AttachCurrentThread(&env, NULL);
			jclass cls = FindClass("LynxEngine/Android/GameAnalytics");
			jmethodID init = env->GetStaticMethodID(cls, "Init", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;)V");
			jstring g = env->NewStringUTF(gameKey.c_str());
			jstring s = env->NewStringUTF(secretKey.c_str());
			env->CallStaticVoidMethod(cls, init, app->activity->clazz, g, s);

			env->DeleteLocalRef(g);
			env->DeleteLocalRef(s);
			app->activity->vm->DetachCurrentThread();
        #endif
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  ª°©˙:
        //-------------------------------------------------------------------------------------------------------
        void CGameAnalytics::AddProgressionEvent(ProgressionStatus progressionStatus, CString progression01, CString progression02, CString progression03)
        {
			return;

        #ifdef __iOS__
            NSString *Progression01, *Progression02 = nil, *Progression03 = nil;
            lynxcString2NSString(&Progression01, progression01.c_str());
            if (progression02 != "")
                lynxcString2NSString(&Progression02, progression02.c_str());
            if (progression03 != "")
                lynxcString2NSString(&Progression03, progression03.c_str());
            
			GAProgressionStatus ProgressionStatus = (GAProgressionStatus)progressionStatus;

            [GameAnalytics addProgressionEventWithProgressionStatus:ProgressionStatus progression01:Progression01 progression02:Progression02 progression03:Progression03];
		#elif defined __ANDROID__
			android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
			JNIEnv* env;
			app->activity->vm->AttachCurrentThread(&env, NULL);
			jclass cls = FindClass("LynxEngine/Android/GameAnalytics");
			jmethodID addProgressionEvent = env->GetStaticMethodID(cls, "AddProgressionEvent", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			jint ps = progressionStatus;
			jstring p1 = env->NewStringUTF(progression01.c_str());
			jstring p2 = env->NewStringUTF(progression02.c_str());
			jstring p3 = env->NewStringUTF(progression03.c_str());
			env->CallStaticVoidMethod(cls, addProgressionEvent, ps, p1, p2, p3);

			env->DeleteLocalRef(p1);
			env->DeleteLocalRef(p2);
			env->DeleteLocalRef(p3);
			app->activity->vm->DetachCurrentThread();
        #endif
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  ª°©˙:
        //-------------------------------------------------------------------------------------------------------
        void CGameAnalytics::AddDesignEvent(CString eventId)
        {
			return;

        #ifdef __iOS__
            NSString *EventId;
            lynxcString2NSString(&EventId, eventId.c_str());
            
            [GameAnalytics addDesignEventWithEventId:EventId];
		#elif defined __ANDROID__
			android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
			JNIEnv* env;
			app->activity->vm->AttachCurrentThread(&env, NULL);
			jclass cls = FindClass("LynxEngine/Android/GameAnalytics");
			jmethodID addDesignEvent = env->GetStaticMethodID(cls, "AddDesignEvent", "(Ljava/lang/String;)V");
			jstring e = env->NewStringUTF(eventId.c_str());
			env->CallStaticVoidMethod(cls, addDesignEvent, e);

			env->DeleteLocalRef(e);
			app->activity->vm->DetachCurrentThread();
        #endif
        }
        /*
        //-------------------------------------------------------------------------------------------------------
        //
        //  ª°©˙:
        //-------------------------------------------------------------------------------------------------------
        void CGameAnalytics::AddDesignEvent(CString eventId, float value)
        {
			return;

        #ifdef __iOS__
            NSString *EventId;
            lynxcString2NSString(&EventId, eventId.c_str());
            NSNumber *Value = [NSNumber numberWithFloat:value];;
            
            [GameAnalytics addDesignEventWithEventId:EventId value:Value];
		#elif defined __ANDROID__
			android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
			JNIEnv* env;
			app->activity->vm->AttachCurrentThread(&env, NULL);
			jclass cls = FindClass("LynxEngine/Android/GameAnalytics");
			jmethodID addDesignEvent = env->GetStaticMethodID(cls, "AddDesignEvent", "(Ljava/lang/String;F)V");
			jstring e = env->NewStringUTF(eventId.c_str());
			env->CallStaticVoidMethod(cls, addDesignEvent, e, value);

			env->DeleteLocalRef(e);
			app->activity->vm->DetachCurrentThread();
        #endif
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  ª°©˙:
        //-------------------------------------------------------------------------------------------------------
        void CGameAnalytics::AddDesignEvent(CString eventId, const LYNXULONGLONG& value)
        {
			return;

        #ifdef __iOS__
            NSString *EventId;
            lynxcString2NSString(&EventId, eventId.c_str());
            NSNumber *Value = [NSNumber numberWithUnsignedLongLong:value];
            
            [GameAnalytics addDesignEventWithEventId:EventId value:Value];
		#elif defined __ANDROID__
			android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
			JNIEnv* env;
			app->activity->vm->AttachCurrentThread(&env, NULL);
			jclass cls = FindClass("LynxEngine/Android/GameAnalytics");
			jmethodID addDesignEvent = env->GetStaticMethodID(cls, "AddDesignEvent", "(Ljava/lang/String;F)V");
			jstring e = env->NewStringUTF(eventId.c_str());
			env->CallStaticVoidMethod(cls, addDesignEvent, e, (float)value);

			env->DeleteLocalRef(e);
			app->activity->vm->DetachCurrentThread();
        #endif
        }
        */
    }
}
