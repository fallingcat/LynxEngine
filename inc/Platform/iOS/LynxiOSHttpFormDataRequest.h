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

#ifndef __LYNXIOSHTTPFORMDATAREQUEST_H__
#define __LYNXIOSHTTPFORMDATAREQUEST_H__

#include <Platform/iOS/LynxiOSHttpRequest.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		namespace iOS
		{
			class LYNXENGCLASS CHttpFormDataRequest : public CHttpRequest
			{
				LYNX_DECLARE_CLASS(CHttpFormDataRequest);
			protected:
			public:			
				CHttpFormDataRequest(CEngine* const lpengine);			
				~CHttpFormDataRequest();

                void                            vInit(const CString& url);
				virtual void					vAddFile(const CString& key, const CString& filename);
				
			};	
		}
	}
}

#endif