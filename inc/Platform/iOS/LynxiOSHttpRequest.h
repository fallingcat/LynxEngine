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

#ifndef __LYNXIOSHTTPREQUEST_H__
#define __LYNXIOSHTTPREQUEST_H__

#include <LynxEngineClasses.h>
#include <NetworkSystem/LynxHttpRequest.h>


LYNX_FORWARD_DECLARATION(ASIHTTPRequest)

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		namespace iOS
		{
			class LYNXENGCLASS CHttpRequest : public LynxEngine::NetworkSystem::CHttpRequest
			{
				LYNX_DECLARE_CLASS(CHttpRequest);
			protected:
                ASIHTTPRequest*                 m_Request;
			public:			
				CHttpRequest(CEngine* const lpengine);			
				~CHttpRequest();

                void                            vInit(const CString& url);
				LYNXBOOL                        vStartSynchronous();
				LYNXBOOL                        vStartAsynchronous();
				CString                         vGetResponse();
				void                            vGetJSONResponse(CJsonValue& response);
                void                            vAddFile(const CString& key, const CString& filename) {};
				
			};	
		}
	}
}

#endif