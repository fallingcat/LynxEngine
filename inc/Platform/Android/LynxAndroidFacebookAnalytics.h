//
//  LynxGameAnalytics.h
//  LynxEngine
//
//  Created by Qubit on 2016/3/15.
//
//

#ifndef __LYNXANDROIDFACEBOOKANALYTICS_H__
#define __LYNXANDROIDFACEBOOKANALYTICS_H__

#include <LynxEngineClasses.h>
#include <LynxCore.h>

namespace LynxEngine
{
    namespace Android
	{
		class LYNXENGCLASS CFacebookAnalytics
		{
			LYNX_DECLARE_CLASS(CFacebookAnalytics);
		public:	
		protected:
		public:
			CFacebookAnalytics();
			~CFacebookAnalytics();
            
			static void						Config(const CString& build);
			static void						Init();			
			static void						ConfigureAvailableEvent(const GameAnalyticsSystem::AVAILABLEEVENTTYPE type, const CArray<CString>& values);
			static void						AddProgressionEvent(const GameAnalyticsSystem::PROGRESSSTATUS progressionStatus, const CString& progression01, const CString& progression02 = "", const CString& progression03 = "");
			static void						AddDesignEvent(const CString& eventId, const CString& stack = "");
			static void						AddResourceEvent(const GameAnalyticsSystem::RESOURCEFLOWTYPE flow, const CString& type, const CString& id, const float amount, const CString& currency);
			static void						AddBusinessEvent(const CString& currency, const LYNXDOUBLE amount, const CString& itemType, const CString& itemId, const CString& cartType, const CString& receipt, const CString& store, const CString& signature, const CString& stack = "");
			static void						UpdateUserProperty(const CString& key, const CString& value);
		};
	}
}

#endif /* LynxGameAnalytics_h */
