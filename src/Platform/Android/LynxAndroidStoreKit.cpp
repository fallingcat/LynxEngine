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
#include <Platform/Android/LynxAndroidStoreKit.h>

#if defined(__ANDROID__)

namespace LynxEngine
{
	using namespace Android;

	STOREKITRESULTCODE GetEnum(int errorCode)
	{
		switch (errorCode)
		{
		case BILLING_RESPONSE_RESULT_OK:
			return SKRC_SUCCESS;
			break;

		case BILLING_RESPONSE_RESULT_USER_CANCELED:
		case IABHELPER_USER_CANCELLED:
			return SKRC_USER_CANCELLED;
			break;

		case BILLING_RESPONSE_RESULT_ITEM_UNAVAILABLE:
		case BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED:
		case BILLING_RESPONSE_RESULT_ITEM_NOT_OWNED:
			return SKRC_ITEM_UNAVAILABLE;
			break;

		case IABHELPER_VERIFICATION_FAILED:
			return SKRC_GAMESERVER_VERIFY_FAILED;
			break;
			
		case SKRC_USER_CANCELLED:
		case SKRC_SERVICE_NOT_INSTALL:
		case SKRC_INIT_IAP_FAILED:
		case SKRC_IABHELPER_EXCEPITION:
		case SKRC_ITEM_UNAVAILABLE:
		case SKRC_GAMESERVER_REQUEST_FAILED:
		case SKRC_GAMESERVER_VERIFY_FAILED:
			return (STOREKITRESULTCODE)errorCode;
			break;

		case IABHELPER_UNKNOWN_PURCHASE_RESPONSE:
		case IABHELPER_UNKNOWN_ERROR:
		default:
			return SKRC_UNKNOWN_ERROR;
			break;
		}

		return SKRC_UNKNOWN_ERROR;
	}
}
#endif // End of platform define
