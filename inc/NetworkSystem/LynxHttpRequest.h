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

#ifndef __LYNXHTTPREQUEST_H__
#define __LYNXHTTPREQUEST_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		class LYNXENGCLASS CHttpRequest : public CObj
		{
			LYNX_DECLARE_CLASS(CHttpRequest);
		public:
			typedef enum {
				MT_POST = 0,
				MT_GET,
				MT_PUT,
				MT_DELETE
			}METHODTYPE;

			typedef enum {
				CMT_DEFLATE = 0,
				CMT_GZIP,
				CMT_ALL,
			}COMPRESSIONMETHODTYPE;


			void*							m_CallBackUserData;
		protected:
			LYNXBOOL						m_bFinished;
			void*							m_fpFinishCB;
			std::function<void(void)>		m_FinishCallback;
		private:			
		public:			
			CHttpRequest(CEngine* const lpengine) :CObj(lpengine) { m_FinishCallback = nullptr;  m_fpFinishCB = NULL; m_bFinished = LYNX_FALSE; };
			~CHttpRequest() {};

			virtual void                    vInit(const CString& url) {};
			virtual void					vReset() {};
			virtual LYNXBOOL				vStartSynchronous(const METHODTYPE t = MT_POST) { return LYNX_FALSE; };
			virtual LYNXBOOL				vStartAsynchronous(const METHODTYPE t = MT_POST, LYNXBOOL bWithWriteCB = LYNX_TRUE) { return LYNX_FALSE; };
			virtual CString					vGetResponse() { return CString(_T("")); };
			virtual void					vGetJSONResponse(CJsonValue& response, LYNXBOOL bunpack = LYNX_FALSE) {};
			virtual void					vAddPostValue(const CString& key, const CString& value) {LYNX_ASSERT(0); };
			virtual void					vAddPostValue(const CString& key, const int& value) {LYNX_ASSERT(0);};
			virtual void					vAddPostValue(const CString& key, const unsigned int& value) {LYNX_ASSERT(0);};
			virtual void					vAddPostValue(const CString& key, const LYNXBOOL& value) {LYNX_ASSERT(0);};
			virtual void					vAddFile(const CString& key, const CString& filename) {LYNX_ASSERT(0);};
			virtual void					vSetCompressionMethod(const COMPRESSIONMETHODTYPE t) {LYNX_ASSERT(0);};
			virtual void					vWriteResponseToFile(CString& filename) {};
			virtual LYNXBOOL				vIsAsynchronousFinish(){ return LYNX_TRUE; };
			virtual void					vSetFinishCallBack(void* callback, void* userdata){};
			virtual CString					vGetUrlEncode(const CString &fromurl){ return ""; };
		};	
	}
}

#endif