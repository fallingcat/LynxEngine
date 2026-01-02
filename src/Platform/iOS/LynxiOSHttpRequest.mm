//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <Platform/iOS/LynxiOSHttpRequest.h>
#import <UIKit/UIKit.h>
#import <ASIHTTPRequest.h>

namespace LynxEngine 
{	
	namespace NetworkSystem
	{
		namespace iOS
		{			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CHttpRequest::CHttpRequest(CEngine* const lpengine)
			: LynxEngine::NetworkSystem::CHttpRequest(lpengine)
			{
                m_Request = nil;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CHttpRequest::~CHttpRequest(void)
			{
			}
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CHttpRequest::vInit(const CString& url)
			{
                NSString* URLStr = nil;
                lynxcString2NSString(&URLStr, url.c_str());
                NSURL* URL = [NSURL URLWithString:URLStr];
                m_Request = [ASIHTTPRequest requestWithURL:URL];
                //[URLStr release];
            }
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CHttpRequest::vStartSynchronous()
			{
                [m_Request startSynchronous];
                NSError *Error = [m_Request error];
                if (Error)
                    return LYNX_FALSE;
                
                return LYNX_TRUE;
			}
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CHttpRequest::vStartAsynchronous()
			{
                [m_Request startAsynchronous];
                NSError *Error = [m_Request error];
                if (Error)
                    return LYNX_FALSE;
                
                return LYNX_TRUE;
			}
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			CString CHttpRequest::vGetResponse()
			{
                LYNXCHAR* cResponse = NULL;
                
                NSString* Response = [m_Request responseString];
                cResponse = LYNXNEW LYNXCHAR[[Response length] + 1];
                lynxNSString2cString(cResponse, Response);
                CString ResponseStr = cResponse;
                LYNXDEL_ARRAY(cResponse);
                
                return ResponseStr;
            }
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CHttpRequest::vGetJSONResponse(CJsonValue& response)
            {
                CString Resp = vGetResponse();
                CEngine::GetJSONValues(response, Resp);
            }
		}
	}
}