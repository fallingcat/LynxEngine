//
//  LynxGameAnalytics.cpp
//  LynxEngine
//
//  Created by Qubit on 2016/3/15.
//
//

#include <Platform/iOS/LynxiOSFacebookAnalytics.h>

#import <objc/runtime.h>
#import <objc/message.h>


namespace LynxEngine
{
    namespace iOS
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		CFacebookAnalytics::CFacebookAnalytics()
		{
		}
		//--------------------------------------------------------------------------------------------------- ----
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		CFacebookAnalytics::~CFacebookAnalytics(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFacebookAnalytics::Config(const CString& build)
		{
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFacebookAnalytics::Init()
		{
            [FBSDKAppEvents activateApp];
            
            NSString *UserID;
            lynxcString2NSString(&UserID, glpKernel->GetUDIDString());
            [FBSDKAppEvents setUserID:UserID];
            NSLog(@"FB setUserID %@", UserID);

		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  
		//-------------------------------------------------------------------------------------------------------
		void CFacebookAnalytics::ConfigureAvailableEvent(const GameAnalyticsSystem::AVAILABLEEVENTTYPE type, const CArray<CString>& values)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFacebookAnalytics::AddProgressionEvent(const GameAnalyticsSystem::PROGRESSSTATUS progressionStatus, const CString& progression01, const CString& progression02, const CString& progression03)
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
            
            
            [FBSDKAppEvents logEvent:@"ProgressionEvent" parameters:dic];
 
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFacebookAnalytics::AddDesignEvent(const CString& eventId, const CString& stack)
		{
			NSString *EventId;
			lynxcString2NSString(&EventId, eventId.c_str());
            
            NSMutableDictionary* dic = [NSMutableDictionary dictionary];            
			if (stack != "")
			{			
				NSString *Stackstr;
				lynxcString2NSString(&Stackstr, stack.c_str());
				[dic setObject:Stackstr forKey:@"stack"];
			}
            CString EvFB = eventId;
            std::replace(EvFB.begin(), EvFB.end(), ':', '_');
            NSString *FBEventId;
            lynxcString2NSString(&FBEventId, EvFB.c_str());
			[FBSDKAppEvents logEvent:FBEventId parameters:dic];

			[dic setObject:EventId forKey:@"eventId"];
            [FBSDKAppEvents logEvent:@"DesignEvent" parameters:dic];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFacebookAnalytics::AddResourceEvent(const GameAnalyticsSystem::RESOURCEFLOWTYPE flow, const CString& type, const CString& id, const float amount, const CString& currency)
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
            [FBSDKAppEvents logEvent:@"ResourceEvent" parameters:dic];
			
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //
        //-------------------------------------------------------------------------------------------------------
        void CFacebookAnalytics::AddBusinessEvent(const CString& currency, int amount, const CString& itemType, const CString& itemId
		, const CString& cartType, const CString& receipt, const CString& store, const CString& signature, const CString& stack)
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

            [FBSDKAppEvents logEvent:@"BusinessEvent" parameters:dic];
			[FBSDKAppEvents logPurchase:amount currency:Currency];
        }
		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CFacebookAnalytics::UpdateUserProperty(const CString& key, const CString& value)
        {
            NSString* Key;
            lynxcString2NSString(&Key, key.c_str());
            
            NSString* Value;
            lynxcString2NSString(&Value, value.c_str());
            
            NSMutableDictionary* Prop = [NSMutableDictionary dictionary];
            [Prop setObject:Value forKey:Key];
            
            [FBSDKAppEvents updateUserProperties:Prop handler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error)
             {
                 NSLog(@"updateUserProperties %@  %@", result, error);
             }];
		}
	}
}
