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

#ifdef __CURL__

#ifndef __LYNXCURLHTTPFORMDATAREQUEST_H__
#define __LYNXCURLHTTPFORMDATAREQUEST_H__

	#include <NetworkSystem/LynxcurlHttpRequest.h>

	namespace LynxEngine 
	{
		namespace NetworkSystem 
		{
			namespace curl 
			{
				class LYNXENGCLASS CHttpFormDataRequest : public CHttpRequest
				{
					LYNX_DECLARE_CLASS(CHttpFormDataRequest);
				protected:						
				public:			
					CHttpFormDataRequest(CEngine* const lpengine);			
					~CHttpFormDataRequest();

					//void							vInit(const CString& url);
					void							vAddPostValue(const CString& key, const CString& value);
					void							vAddPostValue(const CString& key, const int& value);
					void							vAddPostValue(const CString& key, const unsigned int& value);
					void							vAddPostValue(const CString& key, const LYNXBOOL& value);
					void							vAddFile(const CString& key, const CString& filename);

					LYNXBOOL						vStartSynchronous(const METHODTYPE t);
					LYNXBOOL						vStartAsynchronous(const METHODTYPE t = MT_POST, LYNXBOOL bWithWriteCB = LYNX_TRUE);
				};
			}
		}
	}

#endif

#endif