#include <LynxEngine_PCH.h>

#include <msgpack.h>

//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------		
void msgpack_object_sprintf(LynxEngine::CString& out, msgpack_object o)
{
	char Buffer[256];

	switch(o.type) 
	{
	case MSGPACK_OBJECT_NIL:
		out += "nil";
		break;

	case MSGPACK_OBJECT_BOOLEAN:
		out += (o.via.boolean ? "true" : "false");
		break;

	case MSGPACK_OBJECT_POSITIVE_INTEGER:
		lynxSprintf(Buffer, "%I64u", o.via.u64);
		out += Buffer;		
		break;

	case MSGPACK_OBJECT_NEGATIVE_INTEGER:
		lynxSprintf(Buffer, "%I64d", o.via.i64);
		out += Buffer;				
		break;

	case MSGPACK_OBJECT_DOUBLE:
		lynxSprintf(Buffer, "%f", o.via.dec);
		out += Buffer;						
		break;

	case MSGPACK_OBJECT_RAW:
		out += "\"";
		lynxMemCpy(Buffer, o.via.raw.ptr, o.via.raw.size);
		Buffer[o.via.raw.size] = 0x00;
		out += Buffer;
		out += "\"";				
		break;

	case MSGPACK_OBJECT_ARRAY:
		out += "{";
		if(o.via.array.size != 0) {
			msgpack_object* p = o.via.array.ptr;
			msgpack_object_sprintf(out, *p);
			++p;
			msgpack_object* const pend = o.via.array.ptr + o.via.array.size;
			for(; p < pend; ++p) {
				out += ", ";
				msgpack_object_sprintf(out, *p);
			}
		}
		out += "}";
		break;

	case MSGPACK_OBJECT_MAP:
		out += "{";
		if(o.via.map.size != 0) {
			msgpack_object_kv* p = o.via.map.ptr;
			msgpack_object_sprintf(out, p->key);
			out += ":";
			msgpack_object_sprintf(out, p->val);
			++p;
			msgpack_object_kv* const pend = o.via.map.ptr + o.via.map.size;
			for(; p < pend; ++p) {
				out += ", ";
				msgpack_object_sprintf(out, p->key);
				out += ":";
				msgpack_object_sprintf(out, p->val);
			}
		}
		out += "}";
		break;

	default:
		// FIXME
		//fprintf(out, "#<UNKNOWN %i %" PRIu64 ">", o.type, o.via.u64);
		break;
	}			
}

#ifdef __CURL__

	#include <NetworkSystem/LynxcurlHttpRequest.h>

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
				CHttpRequest::CHttpRequest(CEngine* const lpengine)
				: LynxEngine::NetworkSystem::CHttpRequest(lpengine)
				{
					m_CurlHeader = NULL;
					m_CurlFormFirst = NULL;
					m_Buffer = NULL;
					m_CurlMulti = NULL;
					// Curl easy handle init 
					m_Curl = curl_easy_init();
					// Set the parameter to 1 to get the library to display a lot of verbose information about its operations. 
					// Very useful for libcurl and/or protocol debugging and understanding. 
					// The verbose information will be sent to stderr, or the stream set with CURLOPT_STDERR. The default value for this parameter is 0.
					// curl_easy_setopt(m_Curl, CURLOPT_VERBOSE, 1);
				
					// A parameter set to 1 tells the library to include the header in the body output.
					// This is only relevant for protocols that actually have headers preceding the data (like HTTP). The default value for this parameter is 0.
					// curl_easy_setopt(m_Curl, CURLOPT_HEADER, 1);

					// time out is 60 seconds
					SetHttpRequestTimeout(30);
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				CHttpRequest::~CHttpRequest()
				{
					m_CurlWriteDataLength = 0;
					m_CurlWriteData.clear();

					// clear post data
					m_CurlPostData.clear();

					if (m_Buffer)
					{
						LYNXDEL_ARRAY(m_Buffer);
						m_Buffer = NULL;
					}

					// Clean up curl handle
					if (m_CurlMulti)
					{
						curl_multi_cleanup(m_CurlMulti);
					}

					curl_easy_cleanup(m_Curl);

					/* then cleanup the formpost chain */ 
					if (m_CurlFormFirst)
					{
						curl_formfree(m_CurlFormFirst);
						m_CurlFormFirst = NULL;
					}
					
					/* free slist */ 
					if (m_CurlHeader)
					{
						curl_slist_free_all (m_CurlHeader);
						m_CurlHeader = NULL;
					}
				}			
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::vInit(const CString& url)
				{
					m_URL = url;

					m_CurlWriteDataLength = 0;
					m_CurlWriteData.clear();					

					curl_version_info_data *data = curl_version_info(CURLVERSION_NOW);
					if (data->features & CURL_VERSION_IPV6)
					{
						//LYNX_LOG(glpLogger, CString("curl features with IPV6\n"));						
					}
					
					if (data->features & CURL_VERSION_SSL)
					{
						//LYNX_LOG(glpLogger, CString("curl features with SSL\n"));					
					}
					//const char *pPassphrase = NULL;
					//static const char *pCertFile = "cacert.pem";
					//static const char *pCACertFile = "../Data/qubot.self-signed.cacert.pem";
					static const char *pCACertFile = "../data/pem/cacert.pem";
					//const char *pKeyName = "testkey.pem";
					//const char *pKeyType = "PEM";

					// Set url
					curl_easy_setopt(m_Curl, CURLOPT_URL, url.c_str());		
					//curl_easy_setopt(m_Curl, CURLOPT_FRESH_CONNECT, 1);		
#if 0
					if (curl_easy_setopt(m_Curl, CURLOPT_SSLENGINE_DEFAULT, 1L) != CURLE_OK) {
						/* set the crypto engine as default */
						/* only needed for the first time you load
						a engine in a curl object... */
						fprintf(stderr, "can't set crypto engine as default\n");

					}

					///* cert is stored PEM coded in file... */
					///* since PEM is default, we needn't set it for PEM */
					//curl_easy_setopt(m_Curl, CURLOPT_SSLCERTTYPE, "PEM");
					//curl_easy_setopt(m_Curl, CURLOPT_CAPATH, "../Data");
					//curl_easy_setopt(m_Curl, CURLOPT_CAINFO, pCertFile);
					///* set the cert for client authentication */
					//curl_easy_setopt(m_Curl, CURLOPT_SSLCERT, pCertFile);

					/* sorry, for engine we must set the passphrase
					(if the key has one...) */
					//if (pPassphrase)
					//	curl_easy_setopt(m_Curl, CURLOPT_KEYPASSWD, pPassphrase);

					/* if we use a key stored in a crypto engine,
					we must set the key type to "ENG" */
					//curl_easy_setopt(m_Curl, CURLOPT_SSLKEYTYPE, pKeyType);

					/* set the private key (file or ID in engine) */
					//curl_easy_setopt(m_Curl, CURLOPT_SSLKEY, pKeyName);
#endif
					/* set the file with the certs vaildating the server */
					curl_easy_setopt(m_Curl, CURLOPT_CAINFO, pCACertFile);

					/* disconnect if we can't validate server's cert */
					curl_easy_setopt(m_Curl, CURLOPT_SSL_VERIFYPEER, 0L);
					curl_easy_setopt(m_Curl, CURLOPT_SSL_VERIFYHOST, 0L);
					//curl_easy_setopt(m_Curl, CURLOPT_FRESH_CONNECT, true);
					//curl_easy_setopt(m_Curl, CURLOPT_FORBID_REUSE, true);

					// clear post data
					m_CurlPostData.clear();

					// Clean up header
					if (m_CurlHeader)
					{
						curl_slist_free_all(m_CurlHeader);
						m_CurlHeader = NULL;
					}

					// Clean http form data
					if (m_CurlFormFirst)
					{
						curl_formfree(m_CurlFormFirst);
						m_CurlFormFirst = NULL;
					}					

					m_CurlFormLast	= NULL;

					vSetCompressionMethod(CMT_DEFLATE);
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::vReset()
				{
					m_CurlWriteDataLength = 0;
					m_CurlWriteData.clear();
				}
				//-------------------------------------------------------------------------------------------------------
				//  Todo:
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::SetHttpRequestTimeout(const int t)
				{
					curl_easy_setopt(m_Curl, CURLOPT_TIMEOUT, t);
				}
				//-------------------------------------------------------------------------------------------------------
				//  Todo:
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				size_t CHttpRequest::sCurlWriteCallback(void *buffer, size_t size, size_t nmemb, void *userData)
				{
					// Response length
					int Length = size * nmemb;

					// Get http request
					CHttpRequest* HttpRequest = (CHttpRequest*)userData;

					// Append response data
					HttpRequest->m_CurlWriteData.append((char*)buffer, Length);

					// Add response length
					HttpRequest->m_CurlWriteDataLength += Length;

					return Length;
				}

				size_t CHttpRequest::sCurlNopWriteCallback(void *buffer, size_t size, size_t nmemb, void *userData)
				{
					// Response length
					int Length = size * nmemb;

					CHttpRequest* HttpRequest = (CHttpRequest*)userData;


					return Length;
				}
				//-------------------------------------------------------------------------------------------------------
				//  Todo:
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void  CHttpRequest::setCurlHttpOption(const METHODTYPE t)
				{
					// Http method
					switch(t)
					{
					// POST
					case MT_POST:
						// Header : multipart/form-data
						m_CurlHeader = curl_slist_append(m_CurlHeader, "Content-Type:multipart/form-data; charset=UTF-8");

						// nocache.
						//m_CurlHeader = curl_slist_append(m_CurlHeader, "Cache-Control: no-cache");

						// initalize custom header list (stating that Expect: 100-continue is not wanted)
						m_CurlHeader = curl_slist_append(m_CurlHeader, "Expect:");

						// Add form data
						if (m_CurlFormFirst)
						{
							curl_easy_setopt(m_Curl, CURLOPT_HTTPPOST, m_CurlFormFirst);
						}

						LYNX_LOG(glpNetworkLogger, CString("Method : POST\n"));
						break;

					// PUT
					case MT_PUT:
						// Header : application/x-www-form-urlencoded
						m_CurlHeader = curl_slist_append(m_CurlHeader, "Content-Type:application/x-www-form-urlencoded; charset=UTF-8");

						// nocache.
						//m_CurlHeader = curl_slist_append(m_CurlHeader, "Cache-Control: no-cache");

						// Custom request : PUT
						curl_easy_setopt(m_Curl, CURLOPT_CUSTOMREQUEST, "PUT");

						// Add put data
						if (!m_CurlPostData.empty())
						{
							curl_easy_setopt(m_Curl, CURLOPT_POSTFIELDS, m_CurlPostData.c_str());
						}

						LYNX_LOG(glpNetworkLogger, CString("Method : PUT\n"));
						break;

					// DELETE
					case MT_DELETE:

						// nocache.
						//m_CurlHeader = curl_slist_append(m_CurlHeader, "Cache-Control: no-cache");

						// Custom request : DELETE
						curl_easy_setopt(m_Curl, CURLOPT_CUSTOMREQUEST, "DELETE");

						LYNX_LOG(glpNetworkLogger, CString("Method : DELETE\n"));
						break;

					// GET
					case MT_GET:

						// nocache.
						//m_CurlHeader = curl_slist_append(m_CurlHeader, "Cache-Control: no-cache");
						LYNX_LOG(glpNetworkLogger, CString("Method : GET\n"));
						break;

					default:
						break;

					}

					// Set crul http header
					curl_easy_setopt(m_Curl, CURLOPT_HTTPHEADER, m_CurlHeader);
				}
				//-------------------------------------------------------------------------------------------------------
				//  Todo:
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::vSetCompressionMethod(const COMPRESSIONMETHODTYPE t)
				{
					switch (t)
					{
						case CMT_DEFLATE:
							curl_easy_setopt(m_Curl, CURLOPT_ACCEPT_ENCODING, "deflate");					
							break;

						case CMT_GZIP:
							curl_easy_setopt(m_Curl, CURLOPT_HEADER, false);
							curl_easy_setopt(m_Curl, CURLOPT_ACCEPT_ENCODING, "gzip");					
							break;

						case CMT_ALL:							
							curl_easy_setopt(m_Curl, CURLOPT_ACCEPT_ENCODING, "gzip,deflate");					
							break;
					}
					m_CompressionMethodType = t;
				}
				//-------------------------------------------------------------------------------------------------------
				//  Todo:
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				LYNXBOOL CHttpRequest::vStartSynchronous(const METHODTYPE t)
				{


					if (m_lpEngine->GetlpNetworkSystem()->IsOffline())
					{
						CPlatformFileStream File;
						CString Path;

						URLToJsonFilePath(Path);
						if (!File.vOpen(Path, CStream::Read|CStream::Text))
							return LYNX_FALSE;

						File.vClose();

						return LYNX_TRUE;
					}
					else
					{
						LYNX_LOG(glpNetworkLogger, CString("URL : ") + m_URL + CString(_T("\n")));

						// Set curl http options
						setCurlHttpOption(t);

						// Set curl callback function and data
						curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, sCurlWriteCallback);
						curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, this);						
					
						#ifdef __LOG__
							m_T0 = glpKernel->GetKernelTime();
						#endif

						// Perform curl
						m_CurlCode = curl_easy_perform(m_Curl);
				
						m_bFinished = LYNX_TRUE;
						return (m_CurlCode == CURLE_OK);
					}
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:  
				//		bWithWriteCB : 是否要把 收到的 Response 寫下來, 若不在意 Response, 可設成 false 可以減少 block 時間..
				//-------------------------------------------------------------------------------------------------------
				LYNXBOOL CHttpRequest::vStartAsynchronous(const METHODTYPE t, LYNXBOOL bWithWriteCB)
				{
					if (m_lpEngine->GetlpNetworkSystem()->IsOffline())
					{
						CPlatformFileStream File;
						CString Path;


						URLToJsonFilePath(Path);
						if (!File.vOpen(Path, CStream::Read|CStream::Text))
							return LYNX_FALSE;

						File.vClose();

						return LYNX_TRUE;
					}
					else
					{
						LYNX_LOG(glpNetworkLogger, CString("URL : ") + m_URL + CString(_T("\n")));

						// Set curl http options
						setCurlHttpOption(t);
						// Set curl callback function and data
						if (bWithWriteCB)
						{
							curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, sCurlWriteCallback);
						}
						else
						{
							curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, sCurlNopWriteCallback);
						}

						curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, this);
					
						m_T0 = glpKernel->GetKernelTime();
						
						// Perform curl
						// m_CurlCode = curl_easy_perform(m_Curl);

						m_CurlMulti = curl_multi_init();
						curl_multi_add_handle(m_CurlMulti, m_Curl);

#if 0
						CURLMcode mc; /* curl_multi_fdset() return code */
						struct timeval timeout;
						int rc; /* select() return code */
						fd_set fdread;
						fd_set fdwrite;
						fd_set fdexcep;
						int maxfd = -1;

						long curl_timeo = -1;

						FD_ZERO(&fdread);
						FD_ZERO(&fdwrite);
						FD_ZERO(&fdexcep);

						/* get file descriptors from the transfers */
						mc = curl_multi_fdset(m_CurlMulti, &fdread, &fdwrite, &fdexcep, &maxfd);

						if (mc != CURLM_OK)
						{
							fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
						}
						/* On success the value of maxfd is guaranteed to be >= -1. We call
						select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
						no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
						to sleep 100ms, which is the minimum suggested value in the
						curl_multi_fdset() doc. */

						if (maxfd == -1)
						{
#ifdef _WIN32
							//Sleep(100);
							//rc = 0;
#else
							/* Portable sleep for platforms other than Windows. */
							struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
							rc = select(0, NULL, NULL, NULL, &wait);
#endif
						}
						else
						{
							/* Note that on some platforms 'timeout' may be modified by select().
							If you need access to the original value save a copy beforehand. */
							//rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
						}
#endif
						CURLMcode Mmc = curl_multi_perform(m_CurlMulti, &m_CurlMultiStillRuning);

						LYNXCHAR buffer[32];
						lynxItoA(Mmc, buffer, 10);
						LYNX_LOG(glpNetworkLogger, CString("Asynchronous Perform Code : ") + CString(buffer) + +CString(_T("\n")));

						return LYNX_TRUE;
					}					
				}				
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				LYNXBOOL CHttpRequest::vIsAsynchronousFinish()
				{
					if (m_bFinished)
					{
						return m_bFinished;
					}

					CURLMcode Mmc = curl_multi_perform(m_CurlMulti, &m_CurlMultiStillRuning);
					
					if (Mmc == -1)
					{
						return false;
					}

					m_bFinished = m_CurlMultiStillRuning == 0;
					CURLMsg *Msg;

					if (m_bFinished)
					{

						//fd_set fdread;
						//fd_set fdwrite;
						//fd_set fdexcep;
						//int maxfd = -1;

						//long curl_timeo = -1;

						//FD_ZERO(&fdread);
						//FD_ZERO(&fdwrite);
						//FD_ZERO(&fdexcep);

						/* get file descriptors from the transfers */
						//CURLMcode mc = curl_multi_fdset(m_CurlMulti, &fdread, &fdwrite, &fdexcep, &maxfd);
						int MsgLeft = 0;						
						while ((Msg = curl_multi_info_read(m_CurlMulti, &MsgLeft)))
						{
							LYNXCHAR buffer[32];
							lynxItoA(Msg->data.result, buffer, 10);
							LYNX_LOG(glpNetworkLogger, CString("Asynchronous Finish Code : ") + CString(buffer) + +CString(_T("\n")));

							lynxItoA(Msg->msg, buffer, 10);
							LYNX_LOG(glpNetworkLogger, CString("Asynchronous Finish Message Code : ") + CString(buffer) + +CString(_T("\n")));
						}
					

						if (m_fpFinishCB)
						{
							(*(AsyncFinishCallBack)m_fpFinishCB)(this);
						}

						if (m_FinishCallback)
						{
							m_FinishCallback();
						}
					}

					return m_bFinished;

				}


				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   設定 callback.
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::vSetFinishCallBack(void* callback, void* userdata)
				{
					m_CallBackUserData = userdata;
					m_fpFinishCB = callback;
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   設定 callback.
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::SetFinishCallBack(std::function<void(void)> callback)
				{
					m_FinishCallback = callback;
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				CString CHttpRequest::vGetResponse()
				{
					CString Result;

					if (m_lpEngine->GetlpNetworkSystem()->IsOffline())
						ReadJSONResponse(m_CurlWriteData);
					else
					{
						switch (m_CompressionMethodType)
						{
							case CMT_GZIP:
								if (CEngine::GZipInflate(Result, m_CurlWriteData))
								{		
									LYNX_LOG(glpNetworkLogger, CString("Response : ") + Result + CString(_T("\n")));
									WriteJSONResponse(Result);
								}
								break;

							case CMT_DEFLATE:
								LYNX_LOG(glpNetworkLogger, CString("Response : ") + m_CurlWriteData + CString(_T("\n")));
								WriteJSONResponse(m_CurlWriteData);								
								Result = m_CurlWriteData;
								break;

							case CMT_ALL:													
								if (CEngine::GZipInflate(Result, m_CurlWriteData))
								{
									LYNX_LOG(glpNetworkLogger, CString("Response : ") + Result + CString(_T("\n")));
									WriteJSONResponse(Result);
								}
								else
								{
									LYNX_LOG(glpNetworkLogger, CString("Response : ") + m_CurlWriteData + CString(_T("\n")));
									WriteJSONResponse(m_CurlWriteData);
									Result = m_CurlWriteData;
								}
								break;		
						}	
						//WriteJSONResponse(m_CurlWriteData);
					}

					return Result;
				}			
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::vWriteResponseToFile(CString& filename)
				{
					CString Result;
					CPlatformFileStream File;					

					if (m_lpEngine->GetlpNetworkSystem()->IsOffline())
					{
						ReadJSONResponse(m_CurlWriteData);
						if (File.vOpen(filename, CStream::Write|CStream::Text|CStream::UTF8))
						{
							File.vWrite(m_CurlWriteData);
							File.vClose();
						}
					}
					else
					{
						switch (m_CompressionMethodType)
						{
							case CMT_GZIP:
								if (CEngine::GZipInflate(Result, m_CurlWriteData))
								{		
									if (File.vOpen(filename, CStream::Write|CStream::Text|CStream::UTF8))
									{
										File.vWrite(Result);
										File.vClose();
									}
								}
								break;

							case CMT_DEFLATE:
								LYNX_LOG(glpNetworkLogger, CString("Response : ") + m_CurlWriteData + CString(_T("\n")));
								if (File.vOpen(filename, CStream::Write|CStream::Text|CStream::UTF8))
								{
									File.vWrite(m_CurlWriteData);
									File.vClose();
								}
								break;

							case CMT_ALL:													
								if (CEngine::GZipInflate(Result, m_CurlWriteData))
								{
									if (File.vOpen(filename, CStream::Write|CStream::Text|CStream::UTF8))
									{
										File.vWrite(Result);
										File.vClose();
									}
								}
								else
								{
									if (File.vOpen(filename, CStream::Write|CStream::Text|CStream::UTF8))
									{
										File.vWrite(m_CurlWriteData);
										File.vClose();
									}
								}
								break;		
						}	
					}
				}	
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				/*
				void CHttpRequest::vGetJSONResponse(CJsonValue& response)
				{
					CString Resp = vGetResponse();
					CEngine::GetJSONValues(response, Resp);
				}
				*/
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::vGetJSONResponse(CJsonValue& response, LYNXBOOL bunpack)
				{
					CString Result;
					LYNXLONGLONG T2;

					if (m_lpEngine->GetlpNetworkSystem()->IsOffline())
					{
						ReadJSONResponse(Result);
						CEngine::GetJSONValues(response, Result);
						return;
					}
					else
					{
						switch (m_CompressionMethodType)
						{
							case CMT_GZIP:
								if (CEngine::GZipInflate(Result, m_CurlWriteData))
								{				
									LYNX_LOG(glpNetworkLogger, CString("Response : ") + Result + CString(_T("\n")));
									T2 = glpKernel->ElapsedKernelTimeSince(m_T0);

									CEngine::GetJSONValues(response, Result);
									if (response.isObject() && !response.isMember(m_lpEngine->GetlpNetworkSystem()->GetHttpErrorKey()))
										WriteJSONResponse(Result);
								}
								break;

							case CMT_DEFLATE:
								if (bunpack)
								{
									char* Buffer = LYNXGLOBAL_NEW char[m_CurlWriteData.length()];	
									msgpack_unpacked msg;
									msgpack_unpacked_init(&msg);
									int Size = m_CurlWriteData.length();
									lynxMemCpy(Buffer, m_CurlWriteData.data(), Size);
									bool success = msgpack_unpack_next(&msg, Buffer, Size, NULL);	

									msgpack_object obj = msg.data;
									CString Result;
									msgpack_object_sprintf(Result, obj);

									LYNXGLOBAL_DEL_ARRAY(Buffer);	

									T2 = glpKernel->ElapsedKernelTimeSince(m_T0);	
									
									CEngine::GetJSONValues(response, Result);
									if (response.isObject() && !response.isMember(m_lpEngine->GetlpNetworkSystem()->GetHttpErrorKey()))
										WriteJSONResponse(Result);
								}
								else
								{
									LYNX_LOG(glpNetworkLogger, CString("Response : ") + m_CurlWriteData + CString(_T("\n")));
									T2 = glpKernel->ElapsedKernelTimeSince(m_T0);

									CEngine::GetJSONValues(response, m_CurlWriteData);
									if (response.isObject() && !response.isMember(m_lpEngine->GetlpNetworkSystem()->GetHttpErrorKey()))
										WriteJSONResponse(m_CurlWriteData);
								}
								break;

							case CMT_ALL:													
								if (CEngine::GZipInflate(Result, m_CurlWriteData))
								{
									LYNX_LOG(glpNetworkLogger, CString("Response : ") + Result + CString(_T("\n")));
									T2 = glpKernel->ElapsedKernelTimeSince(m_T0);

									CEngine::GetJSONValues(response, Result);
									if (response.isArray() || (response.isObject() && !response.isMember(m_lpEngine->GetlpNetworkSystem()->GetHttpErrorKey())))
										WriteJSONResponse(Result);
								}
								else
								{
									if (bunpack)
									{
										char* Buffer = LYNXGLOBAL_NEW char[m_CurlWriteData.length()];	
										msgpack_unpacked msg;
										msgpack_unpacked_init(&msg);
										int Size = m_CurlWriteData.length();
										lynxMemCpy(Buffer, m_CurlWriteData.data(), Size);
										bool success = msgpack_unpack_next(&msg, Buffer, Size, NULL);	

										msgpack_object obj = msg.data;
										CString Result;
										msgpack_object_sprintf(Result, obj);

										LYNXGLOBAL_DEL_ARRAY(Buffer);	
	
										LYNX_LOG(glpNetworkLogger, CString("Response : ") + Result + CString(_T("\n")));
										T2 = glpKernel->ElapsedKernelTimeSince(m_T0);

										CEngine::GetJSONValues(response, Result);
										if (response.isObject() && !response.isMember(m_lpEngine->GetlpNetworkSystem()->GetHttpErrorKey()))
											WriteJSONResponse(Result);
									}
									else
									{
										LYNX_LOG(glpNetworkLogger, CString("Response : ") + m_CurlWriteData + CString(_T("\n")));
										T2 = glpKernel->ElapsedKernelTimeSince(m_T0);

										CEngine::GetJSONValues(response, m_CurlWriteData);
										if (response.isObject() && !response.isMember(m_lpEngine->GetlpNetworkSystem()->GetHttpErrorKey()))
											WriteJSONResponse(m_CurlWriteData);
									}
								}
								break;		
						}						
						#ifdef __LOG__
							LYNXLONGLONG T = glpKernel->ElapsedKernelTimeSince(m_T0);
							LYNXCHAR Buffer[256];

							LYNX_LOG(glpNetworkLogger, CString("URL : ") + m_URL + CString(_T("\n")));
							lynxSprintf(Buffer, "Time Spent (before json parsing) : %d ms\n", T2);					
							LYNX_LOG(glpNetworkLogger, CString(Buffer));
							lynxSprintf(Buffer, "Time Spent (after json parsing) : %d ms\n", T);					
							LYNX_LOG(glpNetworkLogger, CString(Buffer));
							lynxSprintf(Buffer, "Json Parsing Time : %d ms\n", T-T2);					
							LYNX_LOG(glpNetworkLogger, CString(Buffer));
						#endif
						LYNX_LOG(glpNetworkLogger, CString("-------------------------------------------------------------------------------------------------------------------------------------------------------\n\n"));
					}
				}							
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::URLToJsonFilePath(CString& path)
				{
					LYNXCHAR EncodedURL[1024];
					LYNXCHAR Filename[1024];
					int i;

					lynxStrCpy(EncodedURL, m_URL.c_str());
					for (i=0; i<m_URL.length(); i++)
					{
						if (EncodedURL[i] == _T('?'))
						{
							if (EncodedURL[i+1] == _T('a') && EncodedURL[i+2] == _T('c'))
								EncodedURL[i] = 0x00;
							else
								EncodedURL[i] = _T('#');
						}
						else if (EncodedURL[i] == _T('&'))
						{
							EncodedURL[i] = 0x00;
							break;
							//EncodedURL[i] = _T('#');
						}
						else if (EncodedURL[i] == _T('|'))
						{
							EncodedURL[i] = _T('#');
						}
						else if (EncodedURL[i] == _T('/'))
						{
							EncodedURL[i] = _T('#');
						}
						else if (EncodedURL[i] == _T('\\'))
						{
							EncodedURL[i] = _T('#');
						}						
						else if (EncodedURL[i] == _T('.'))
						{
							EncodedURL[i] = _T('#');
						}
						else if (EncodedURL[i] == _T(':'))
						{
							EncodedURL[i] = _T('#');
						}
					}
					CAnsiString FinalURL = EncodedURL;
					LYNXCRC CRCCode = lynxComputeCRCFast((const unsigned char*)FinalURL.c_str(), FinalURL.size());
					lynxSprintf(Filename, _T("%x"), CRCCode);
					path = Filename;

					path = CString(_T("../data/json/")) + path + CString(_T(".json"));
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::WriteJSONResponse(CString& json)
				{
					#ifdef __DESKTOP__
						CPlatformFileStream File;
						CString Path;

						URLToJsonFilePath(Path);
						if (File.vOpen(Path, CStream::Write|CStream::Text|CStream::UTF8))
						{
							File.vWrite(json);
							File.vClose();
						}
					#endif
				}
				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   
				//-------------------------------------------------------------------------------------------------------
				void CHttpRequest::ReadJSONResponse(CString& json)
				{
					CPlatformFileStream File;
					CString Path, Line;

					URLToJsonFilePath(Path);
                    json.clear();
                    if (File.vOpen(Path, CStream::Read|CStream::Text|CStream::UTF8))
					{
						while (File.vRead(Line))
						{
							json += Line;
						}
						File.vClose();
					}
				}


				//-------------------------------------------------------------------------------------------------------
				//
				//  說明:   把 fromurl 轉成 url encode.
				//-------------------------------------------------------------------------------------------------------
				CString CHttpRequest::vGetUrlEncode(const CString &fromurl)
				{
					CString Ret = fromurl;

					char *output = curl_easy_escape(m_Curl, fromurl.c_str(), fromurl.size());
					if (output)
					{
						Ret = output;
						curl_free(output);
					}

					return Ret;			
				}
			}
		}
	}
#endif