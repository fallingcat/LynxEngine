//
//  LynxGameAnalytics.cpp
//  LynxEngine
//
//  Created by Qubit on 2016/3/15.
//
//

#include <Platform/iOS/LynxiOSGameAnalytics.h>

#import <objc/runtime.h>
#import <objc/message.h>

static NSURL* _appStoreReceiptURL_iOS6_support()
{
#define VERIFY_URL @"https://buy.itunes.apple.com/verifyReceipt"
    
    NSLog(@"ext method for iOS_6 support.  -[NSBundle appStoreReceiptURL]");
    return [NSURL URLWithString:VERIFY_URL];
};

@implementation NSBundle_EXT
+ (void) _check_appStoreReceiptURL
{
    if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_6_1) // Less than IOS_7_0
    {
        Method method_appStoreReceiptURL = class_getInstanceMethod([NSBundle class], @selector(appStoreReceiptURL));
        method_setImplementation(method_appStoreReceiptURL, (IMP)_appStoreReceiptURL_iOS6_support);
    } else {
        NSLog(@"-[NSBundle appStoreReceiptURL] method support.(OK.)");
    }
}
@end

namespace LynxEngine
{
    namespace iOS
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
			NSString *Build;
			lynxcString2NSString(&Build, build.c_str());
			[GameAnalytics configureBuild:Build];
            
			/*[GameAnalytics configureAvailableResourceCurrencies:@[@"gems", @"gold"]];
			[GameAnalytics configureAvailableResourceItemTypes:@[@"boost", @"lives"]];
			[GameAnalytics configureAvailableCustomDimensions01:@[@"ninja", @"samurai"]];
			[GameAnalytics configureAvailableCustomDimensions02:@[@"whale", @"dolphin"]];
			[GameAnalytics configureAvailableCustomDimensions03:@[@"horde", @"alliance"]];*/			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::Init(const CString& gameKey, const CString& secretKey)
		{
            [NSBundle_EXT _check_appStoreReceiptURL];
            
            // Enable log
            // [GameAnalytics setEnabledInfoLog:YES];
            // [GameAnalytics setEnabledVerboseLog:YES];
            
			NSString *GameKey, *SecretKey;
			lynxcString2NSString(&GameKey, gameKey.c_str());
			lynxcString2NSString(&SecretKey, secretKey.c_str());
			[GameAnalytics initializeWithGameKey:GameKey gameSecret:SecretKey];			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::ConfigureAvailableEvent(const GameAnalyticsSystem::AVAILABLEEVENTTYPE type, const CArray<CString>& values)
		{
			NSMutableArray* stringArray = [[NSMutableArray alloc] init];
			for (int i=0; i<values.size(); ++i)
			{
				NSString *temp;
				lynxcString2NSString(&temp, values[i].c_str());
				[stringArray addObject:temp];
			}
			
			switch(type)
			{
			case GameAnalyticsSystem::AE_RES_CURRENCIES:
				[GameAnalytics configureAvailableResourceCurrencies:stringArray];
				break;
			case GameAnalyticsSystem::AE_RES_ITEMTYPES:
				[GameAnalytics configureAvailableResourceItemTypes:stringArray];
				break;
			case GameAnalyticsSystem::AE_CUSTOM_DIM01:
				[GameAnalytics configureAvailableCustomDimensions01:stringArray];
				break;
			case GameAnalyticsSystem::AE_CUSTOM_DIM02:
				[GameAnalytics configureAvailableCustomDimensions02:stringArray];
				break;
			case GameAnalyticsSystem::AE_CUSTOM_DIM03:
				[GameAnalytics configureAvailableCustomDimensions03:stringArray];
				break;
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::AddProgressionEvent(const GameAnalyticsSystem::PROGRESSSTATUS progressionStatus, const CString& progression01, const CString& progression02, const CString& progression03)
		{
			NSString *Progression01, *Progression02 = nil, *Progression03 = nil;
			lynxcString2NSString(&Progression01, progression01.c_str());
			if (progression02 != "")
				lynxcString2NSString(&Progression02, progression02.c_str());
			if (progression03 != "")
				lynxcString2NSString(&Progression03, progression03.c_str());
            
			GAProgressionStatus ProgressionStatus = (GAProgressionStatus)progressionStatus;

			[GameAnalytics addProgressionEventWithProgressionStatus:(GAProgressionStatus)ProgressionStatus progression01:Progression01 progression02:Progression02 progression03:Progression03];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::AddDesignEvent(const CString& eventId, const CString& stack)
		{
			NSString *EventId;
			lynxcString2NSString(&EventId, eventId.c_str());
            
			[GameAnalytics addDesignEventWithEventId:EventId];			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ª°©˙:
		//-------------------------------------------------------------------------------------------------------
		void CGameAnalytics::AddResourceEvent(const GameAnalyticsSystem::RESOURCEFLOWTYPE flow, const CString& type, const CString& id, const float amount, const CString& currency)
		{
			NSString *Type;
			lynxcString2NSString(&Type, type.c_str());

			NSString *ID;
			lynxcString2NSString(&ID, id.c_str());

			NSString *Currency;
			lynxcString2NSString(&Currency, currency.c_str());
            
            NSNumber *Value = [NSNumber numberWithFloat:(const float)amount];

			[GameAnalytics addResourceEventWithFlowType:(GAResourceFlowType)flow currency:Currency amount:Value itemType:Type itemId:ID];
		}
        //-------------------------------------------------------------------------------------------------------
        //
        //
        //-------------------------------------------------------------------------------------------------------
        void CGameAnalytics::AddBusinessEvent(const CString& currency, LYNXDOUBLE amount, const CString& itemType, const CString& itemId, const CString& cartType, const CString& receipt, const CString& store, const CString& signature)
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
            
            amount = amount * 100;
            if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_6_1) { // Less than IOS_7_0
                [GameAnalytics addBusinessEventWithCurrency:Currency amount:(NSInteger)amount itemType:ItemType itemId:ItemId cartType:CartType receipt:Receipt];
            }
            else {
                [GameAnalytics addBusinessEventWithCurrency:Currency amount:(NSInteger)amount itemType:ItemType itemId:ItemId cartType:CartType autoFetchReceipt:YES];
            }
        }
	}
}
