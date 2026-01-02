#include <LynxEngine_PCH.h>

#ifdef __CURL__

	#include <sstream>
	#include <NetworkSystem/LynxcurlHttpFormDataRequest.h>

	namespace LynxEngine 
	{
		namespace NetworkSystem 
		{
			namespace curl
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
				CHttpFormDataRequest::~CHttpFormDataRequest()
				{					
					if (m_Buffer)
					{
						LYNXGLOBAL_DEL_ARRAY(m_Buffer);
						m_Buffer = NULL;
					}

				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpFormDataRequest::vAddPostValue(const CString& key, const CString& value)
				{
					// application/x-www-form-urlencoded
					if(!m_CurlPostData.empty())
					{
						m_CurlPostData.append("&");
					}
					m_CurlPostData.append(key);
					m_CurlPostData.append("=");
					m_CurlPostData.append(value);

					// multipart/form-data
					curl_formadd(&m_CurlFormFirst, &m_CurlFormLast,
								CURLFORM_COPYNAME, key.c_str(),
								CURLFORM_COPYCONTENTS, value.c_str(),
								CURLFORM_END);

					LYNX_LOG(glpNetworkLogger, CString("PostValue[") + key + CString("] -> ") + value + CString("\n"));
				}

				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpFormDataRequest::vAddPostValue(const CString& key, const int& value)
				{
					LYNXCHAR Buffer[32];

					// application/x-www-form-urlencoded
					lynxSprintf(Buffer, _T("%d"), value);
					if(!m_CurlPostData.empty())
					{
						m_CurlPostData.append("&");
					}
					m_CurlPostData.append(key);
					m_CurlPostData.append("=");
					m_CurlPostData.append(Buffer);

					// multipart/form-data
					curl_formadd(&m_CurlFormFirst, &m_CurlFormLast,
								CURLFORM_COPYNAME, key.c_str(),
								CURLFORM_COPYCONTENTS, Buffer,
								CURLFORM_END);					

					LYNX_LOG(glpNetworkLogger, CString("PostValue[") + key + CString("] -> ") + CString(Buffer) + CString("\n"));
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpFormDataRequest::vAddPostValue(const CString& key, const unsigned int& value)
				{
					LYNXCHAR Buffer[32];

					// application/x-www-form-urlencoded
					lynxSprintf(Buffer, _T("%d"), value);
					if(!m_CurlPostData.empty())
					{
						m_CurlPostData.append("&");
					}
					m_CurlPostData.append(key);
					m_CurlPostData.append("=");
					m_CurlPostData.append(Buffer);

					// multipart/form-data
					curl_formadd(&m_CurlFormFirst, &m_CurlFormLast,
								CURLFORM_COPYNAME, key.c_str(),
								CURLFORM_COPYCONTENTS, Buffer,
								CURLFORM_END);					

					LYNX_LOG(glpNetworkLogger, CString("PostValue[") + key + CString("] -> ") + CString(Buffer) + CString("\n"));
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpFormDataRequest::vAddPostValue(const CString& key, const LYNXBOOL& value)
				{
					vAddPostValue(key, (int)value);					
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpFormDataRequest::vAddFile(const CString& key, const CString& filename)
				{
					int FileSize;
					CPlatformFileStream m_FileStream;

					if (m_Buffer)
					{
						LYNXGLOBAL_DEL_ARRAY(m_Buffer);
						m_Buffer = NULL;
					}

					if (!m_FileStream.vOpen(filename, CStream::Binary|CStream::Read))
						return;

					FileSize = m_FileStream.vSize();
					m_Buffer = LYNXGLOBAL_NEW char[FileSize];
					m_FileStream.vRead(m_Buffer, sizeof(char), FileSize);
					m_FileStream.vClose();

					// multipart/form-data
					curl_formadd(&m_CurlFormFirst, &m_CurlFormLast,
								CURLFORM_COPYNAME, key.c_str(),
								CURLFORM_BUFFER, filename.c_str(),
								CURLFORM_BUFFERPTR, m_Buffer,
								CURLFORM_BUFFERLENGTH, FileSize,
								CURLFORM_END);					

					LYNX_LOG(glpNetworkLogger, CString("PostFile[") + key + CString("] -> ") + filename + CString("\n"));
				}
				//-------------------------------------------------------------------------------------------------------
				//  Todo:
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				LYNXBOOL CHttpFormDataRequest::vStartSynchronous(const METHODTYPE t)
				{
					LYNXBOOL bRet = CHttpRequest::vStartSynchronous(t);
					m_CurlPostData.clear();					

					return bRet;
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				LYNXBOOL CHttpFormDataRequest::vStartAsynchronous(const METHODTYPE t, LYNXBOOL bWithWriteCB)
				{
					LYNXBOOL bRet = CHttpRequest::vStartAsynchronous(t, bWithWriteCB);
					m_CurlPostData.clear();					

					return bRet;					
				}
			}
		}
	}

#endif