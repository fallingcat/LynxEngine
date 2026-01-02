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

#ifndef __LYNXHTTPFORMDATAREQUEST_H__
#define __LYNXHTTPFORMDATAREQUEST_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		class LYNXENGCLASS CHttpFormDataRequest : public CHttpRequest
		{
			LYNX_DECLARE_CLASS(CHttpFormDataRequest);
		private:			
		public:			
			CHttpFormDataRequest(CEngine* const lpengine):CHttpRequest(lpengine) {};			
			~CHttpFormDataRequest() {};

			virtual void					vAddPostValue(const CString& key, const CString& value) {};
			virtual void					vAddPostValue(const CString& key, const int& value) {};
			virtual void					vAddPostValue(const CString& key, const unsigned int& value) {};
			virtual void					vAddPostValue(const CString& key, const LYNXBOOL& value) {};			
			virtual void					vAddFile(const CString& key, const CString& filename) {};
		};	
	}
}

#endif