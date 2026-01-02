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

#ifndef __LYNXCURLHTTPREQUEST_H__
#define __LYNXCURLHTTPREQUEST_H__

	#include <LynxEngineClasses.h>
	#include <NetworkSystem/LynxHttpRequest.h>
	#include <curl/curl.h>

	namespace LynxEngine 
	{
		namespace NetworkSystem 
		{
			namespace curl
			{
				class LYNXENGCLASS CHttpRequest : public LynxEngine::NetworkSystem::CHttpRequest
				{
					LYNX_DECLARE_CLASS(CHttpRequest);
				protected:
					LYNXLONGLONG					m_T0;

					CString							m_URL;
					CURL*							m_Curl;
					CURLM*							m_CurlMulti;
					CURLcode						m_CurlCode;
					CString							m_CurlPostData;					

					struct curl_httppost*			m_CurlFormFirst;  
					struct curl_httppost*			m_CurlFormLast;
					struct curl_slist*				m_CurlHeader;

					CString							m_CurlWriteData;
					DWORD							m_CurlWriteDataLength;

					COMPRESSIONMETHODTYPE			m_CompressionMethodType;

					char*							m_Buffer;		
					int								m_CurlMultiStillRuning;
				public:
					typedef void(*AsyncFinishCallBack)(void* requeset);

					CHttpRequest(CEngine* const lpengine);			
					~CHttpRequest();

					void                            vInit(const CString& url);
					void                            vReset();
					LYNXBOOL                        vStartSynchronous(const METHODTYPE t = MT_POST);
					LYNXBOOL                        vStartAsynchronous(const METHODTYPE t = MT_POST, LYNXBOOL bWithWriteCB = LYNX_TRUE);
					void							vSetCompressionMethod(const COMPRESSIONMETHODTYPE t);
					CString                         vGetResponse();
					void							vGetJSONResponse(CJsonValue& response, LYNXBOOL bunpack = LYNX_FALSE);
					void							vWriteResponseToFile(CString& filename);
					LYNXBOOL						vIsAsynchronousFinish();
					virtual void					vSetFinishCallBack(void* callback, void* userdata);

					void							SetFinishCallBack(std::function<void(void)> callback);
					
					void							SetHttpRequestTimeout(const int t);
					static size_t					sCurlWriteCallback(void *buffer, size_t size, size_t nmemb, void *userData);
					static size_t					sCurlNopWriteCallback(void *buffer, size_t size, size_t nmemb, void *userData);
					CString							vGetUrlEncode(const CString &fromurl);
                private:
					void							setCurlHttpOption(const METHODTYPE t);		
					void							WriteJSONResponse(CString& json);
					void							ReadJSONResponse(CString& json);
					void							URLToJsonFilePath(CString& path);
				};	
			}
		}
	}

#endif

#endif