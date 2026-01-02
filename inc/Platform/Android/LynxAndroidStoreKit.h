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
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXANDROIDSTOREKIT_H__
#define __LYNXANDROIDSTOREKIT_H__

#include <LynxEngine.h>

namespace LynxEngine 
{
	namespace Android
	{
		// Billing response codes
		typedef enum
		{
			BILLING_RESPONSE_RESULT_OK = 0,
			BILLING_RESPONSE_RESULT_USER_CANCELED = 1,
			BILLING_RESPONSE_RESULT_SERVICE_UNAVAILABLE = 2,
			BILLING_RESPONSE_RESULT_BILLING_UNAVAILABLE = 3,
			BILLING_RESPONSE_RESULT_ITEM_UNAVAILABLE = 4,
			BILLING_RESPONSE_RESULT_DEVELOPER_ERROR = 5,
			BILLING_RESPONSE_RESULT_ERROR = 6,
			BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED = 7,
			BILLING_RESPONSE_RESULT_ITEM_NOT_OWNED = 8,
		} BILLINGRESPONSECODES;

		// IAB Helper error codes
		typedef enum
		{
			IABHELPER_ERROR_BASE = -1000,
			IABHELPER_REMOTE_EXCEPTION = -1001,
			IABHELPER_BAD_RESPONSE = -1002,
			IABHELPER_VERIFICATION_FAILED = -1003,
			IABHELPER_SEND_INTENT_FAILED = -1004,
			IABHELPER_USER_CANCELLED = -1005,
			IABHELPER_UNKNOWN_PURCHASE_RESPONSE = -1006,
			IABHELPER_MISSING_TOKEN = -1007,
			IABHELPER_UNKNOWN_ERROR = -1008,
			IABHELPER_SUBSCRIPTIONS_NOT_AVAILABLE = -1009,
			IABHELPER_INVALID_CONSUMPTION = -1010,
			IABHELPER_SUBSCRIPTION_UPDATE_NOT_AVAILABLE = -1011,
		} IABHELPERERRORCODE;
	}

	STOREKITRESULTCODE GetEnum(int errorCode);
}



#endif
