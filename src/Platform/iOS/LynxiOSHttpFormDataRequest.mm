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
#include <Platform/iOS/LynxiOSHttpFormDataRequest.h>
#import <UIKit/UIKit.h>
#import <ASIFormDataRequest.h>

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
			CHttpFormDataRequest::CHttpFormDataRequest(CEngine* const lpengine)
			: CHttpRequest(lpengine)
			{
            }
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CHttpFormDataRequest::~CHttpFormDataRequest(void)
			{
			}
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CHttpFormDataRequest::vInit(const CString& url)
			{
                NSString* URLStr = nil;
                lynxcString2NSString(&URLStr, url.c_str());
                NSURL* URL = [NSURL URLWithString:URLStr];
                m_Request = [ASIFormDataRequest requestWithURL:URL];
                //[URLStr release];
            }
            //-------------------------------------------------------------------------------------------------------
			//
			//  說明:
			//-------------------------------------------------------------------------------------------------------
			void CHttpFormDataRequest::vAddFile(const CString& key, const CString& filename)
			{
                NSString* Key = nil;
                lynxcString2NSString(&Key, key.c_str());
                
                //m_lpEngine->GetlpFileSystem()()->
                //[m_Request addFile:Path path] forKey:Key];
            }
            
		}
	}
}