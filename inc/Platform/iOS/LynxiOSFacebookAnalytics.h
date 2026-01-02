//
//  LynxGameAnalytics.h
//  LynxEngine
//
//  Created by Qubit on 2016/3/15.
//
//

#ifndef __LYNXIOSFACEBOOKALYTICS_H__
#define __LYNXIOSFACEBOOKALYTICS_H__

#include <LynxEngineClasses.h>
#include <LynxCore.h>
#import <GameAnalyticsSystem/LynxFacebookAnalytics.h>

LYNX_FORWARD_DECLARATION(NSString)

extern "C" void     lynxcString2NSString(NSString** out_str, const LYNXCHAR* str);
extern "C" void     lynxcString2NSStringUTF16(NSString** out_str, const LYNXCHAR* str);
extern "C" void     lynxNSString2cString(LYNXCHAR* out_str, const NSString* str);
extern "C" void     lynxNSString2cStringUTF16(LYNXCHAR* out_str, const NSString* str);
extern "C" void     lynxcString2NSStringUTF16BigEndian(NSString** out_str, const LYNXCHAR* str);
extern "C" void     lynxcString2NSStringUTF8(NSString** out_str, const LYNXCHAR* str);
extern "C" void     lynxcStringUTF162UTF32(LYNXCHAR* out_str, const LYNXCHAR* str);
extern "C" void     lynxcStringUTF322UTF16(LYNXCHAR* out_str, const LYNXCHAR* str);
extern "C" int      lynxiOSAtoi(const LYNXCHAR* str);
extern "C" float    lynxiOSAtof(const LYNXCHAR* str);
extern "C" void     lynxiOSLog(const LYNXCHAR *str);


/**
 *
 *
 */
#import <Foundation/Foundation.h>



namespace LynxEngine
{
    namespace iOS
	{
        class LYNXENGCLASS CFacebookAnalytics
		{
			LYNX_DECLARE_CLASS(CFacebookAnalytics);
		public:		
		protected:
		public:
			CFacebookAnalytics();
			~CFacebookAnalytics();
            
			static void					Config(const CString& build);
			static void                 Init();
			static void					ConfigureAvailableEvent(const GameAnalyticsSystem::AVAILABLEEVENTTYPE type, const CArray<CString>& values);
			static void                 AddProgressionEvent(const GameAnalyticsSystem::PROGRESSSTATUS progressionStatus, const CString& progression01, const CString& progression02 = "", const CString& progression03 = "");
			static void					AddDesignEvent(const CString& eventId, const CString& stack = "");

			static void					AddResourceEvent(const GameAnalyticsSystem::RESOURCEFLOWTYPE flow, const CString& type, const CString& id, const float ammount, const CString& currency);
			static void                 AddBusinessEvent(const CString& currency, int amount, const CString& itemType, const CString& itemId, const CString& cartType, const CString& receipt, const CString& store, const CString& signature, const CString& stack = "");
			static void					UpdateUserProperty(const CString& key, const CString& value);
		};
	}
}

#endif /* LynxGameAnalytics_h */
