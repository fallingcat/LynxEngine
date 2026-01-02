//
//  LynxGameAnalytics.cpp
//  LynxEngine
//
//  Created by Qubit on 2016/3/15.
//
//

#include <LynxEngine_PCH.h>
#include <Platform/iOS/LynxiOSFirebaseAnalytics.h>

#import "Firebase.h"

namespace LynxEngine
{
    namespace iOS
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		CFirebaseAnalytics::CFirebaseAnalytics()
		{
		}
		//--------------------------------------------------------------------------------------------------- ----
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		CFirebaseAnalytics::~CFirebaseAnalytics(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFirebaseAnalytics::Config(const CString& build)
		{

		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFirebaseAnalytics::Init()
		{
            //firebase.
            //[FIROptions defaultOptions].deepLinkURLScheme = @"qubot";
            //[FIRApp configure];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CFirebaseAnalytics::ConfigureAvailableEvent(const GameAnalyticsSystem::AVAILABLEEVENTTYPE type, const CArray<CString>& values)
		{
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFirebaseAnalytics::AddProgressionEvent(const GameAnalyticsSystem::PROGRESSSTATUS progressionStatus, const CString& progression01, const CString& progression02, const CString& progression03)
        {
            NSMutableDictionary* dic = [NSMutableDictionary dictionary];
            int statusint = (int)progressionStatus;
            [dic setObject:[NSNumber numberWithInt:statusint] forKey:@"progressionStatus"];
            
            NSString *Progression01, *Progression02 = nil, *Progression03 = nil;
            lynxcString2NSString(&Progression01, progression01.c_str());
            [dic setObject:Progression01 forKey:@"progression01"];
            
            if (progression02 != "")
            {
                lynxcString2NSString(&Progression02, progression02.c_str());
                [dic setObject:Progression02 forKey:@"progression02"];
            }
            
            if (progression03 != "")
            {
                lynxcString2NSString(&Progression03, progression03.c_str());
                [dic setObject:Progression03 forKey:@"progression03"];
            }
            
            [FIRAnalytics logEventWithName:@"ProgressionEvent" parameters:dic];
            
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// 
		//-------------------------------------------------------------------------------------------------------
		void CFirebaseAnalytics::AddDesignEvent(const CString& eventId, const CString& stack)
        {
            NSString *EventId;
            lynxcString2NSString(&EventId, eventId.c_str());
            
            NSMutableDictionary* dic = [NSMutableDictionary dictionary];
            [dic setObject:EventId forKey:@"eventId"];
            if (stack != "")
            {
                NSString *Stackstr;
                lynxcString2NSString(&Stackstr, stack.c_str());
                [dic setObject:Stackstr forKey:@"stack"];
            }
            
            [FIRAnalytics logEventWithName:@"DesignEvent" parameters:dic];
        }
		//-------------------------------------------------------------------------------------------------------
		//
		// 
		//-------------------------------------------------------------------------------------------------------
		void CFirebaseAnalytics::AddResourceEvent(const GameAnalyticsSystem::RESOURCEFLOWTYPE flow, const CString& type, const CString& id, const float amount, const CString& currency)		
        {
            NSString *Type;
            lynxcString2NSString(&Type, type.c_str());
            
            NSString *ID;
            lynxcString2NSString(&ID, id.c_str());
            
            NSString *Currency;
            lynxcString2NSString(&Currency, currency.c_str());
            
            NSNumber *Value = [NSNumber numberWithFloat:(const float)amount];
            
            NSMutableDictionary* dic = [NSMutableDictionary dictionary];
            [dic setObject:[NSNumber numberWithInt:(int)flow] forKey:@"flow"];
            [dic setObject:Type forKey:@"type"];
            [dic setObject:ID forKey:@"id"];
            [dic setObject:Currency forKey:@"currency"];
            [dic setObject:Value forKey:@"amount"];
            [FIRAnalytics logEventWithName:@"ResourceEvent" parameters:dic];
            
        }
        
		//-------------------------------------------------------------------------------------------------------
		//				
		//  example for Java implementation.
		// 
		//	
		//-------------------------------------------------------------------------------------------------------
		void CFirebaseAnalytics::AddBusinessEvent(const CString& currency,
											  const int amount,
											  const CString& itemType,
											  const CString& itemId,
											  const CString& cartType,
											  const CString& receipt,
											  const CString& store,
											  const CString& signature,
                                              const CString& stack)
        {
            NSString* Currency;
            lynxcString2NSString(&Currency, currency.c_str());
            
            NSString* ItemType;
            lynxcString2NSString(&ItemType, itemType.c_str());
            
            NSString* ItemId;
            lynxcString2NSString(&ItemId, itemId.c_str());
            
            NSString* CartType;
            lynxcString2NSString(&CartType, cartType.c_str());
            
            NSString* Receipt;
            lynxcString2NSString(&Receipt, receipt.c_str());
            
            NSNumber *Value = [NSNumber numberWithFloat:(const float)amount];
            
            
            NSMutableDictionary* dic = [NSMutableDictionary dictionary];
            [dic setObject:Currency forKey:@"currency"];
            [dic setObject:Value forKey:@"amount"];
            [dic setObject:ItemType forKey:@"itemType"];
            [dic setObject:ItemId forKey:@"itemId"];
            [dic setObject:CartType forKey:@"cartType"];
            [dic setObject:Receipt forKey:@"receipt"];
            
            if (stack != "")
            {
                NSString *Stackstr;
                lynxcString2NSString(&Stackstr, stack.c_str());
                [dic setObject:Stackstr forKey:@"stack"];
            }
            
            [FIRAnalytics logEventWithName:@"BusinessEvent" parameters:dic];
            
        }
        
        //-------------------------------------------------------------------------------------------------------
        //
        //  ª°©˙:
        //-------------------------------------------------------------------------------------------------------
        void CFirebaseAnalytics::UpdateUserProperty(const CString& key, const CString& value)
        {
            NSString* Nskey;
            lynxcString2NSString(&Nskey, key.c_str());
            
            NSString* Nsvalue;
            lynxcString2NSString(&Nsvalue, value.c_str());
            NSLog(@"updateUserProperties %@  %@", Nskey, Nsvalue);
            
            [FIRAnalytics setUserPropertyString:Nsvalue forName:Nskey];
        }
    }
}
