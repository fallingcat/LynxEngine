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
#include <Platform/Android/LynxAndroidURLStream.h>
#include <Platform/Android/LynxAndroidUtility.h>

namespace LynxEngine 
{	
	namespace Android
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CURLStream::CURLStream(void)			
		{
			m_bValid = LYNX_FALSE;
			m_StreamType = URL_STREAM;
			m_TotalSize = 0;
            m_CurrentProgressSize = 0;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CURLStream::~CURLStream(void)
		{
			vClose();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CURLStream::vOpen(const CString& name, int flag) 
		{
			if (flag & Write)
			{
				return LYNX_FALSE;
			}
			else // read
			{
				//if (URLDownloadToFile(NULL, name, _T("../temp/temp_read_stream.bin"), 0, NULL) != S_OK)
					//return LYNX_FALSE;

				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID download = env->GetMethodID(cls, "DownloadURLtoFile", "(Ljava/lang/String;Ljava/lang/String;Z)Z");
				jstring u = NewModifiedUTF8String(env, name.c_str());
				jstring n = NewModifiedUTF8String(env, _T("../temp/temp_read_stream.bin"));
				jboolean b = env->CallBooleanMethod(app->activity->clazz, download, u, n, LYNX_FALSE);

				env->DeleteLocalRef(u);
				env->DeleteLocalRef(n);

				JNI_DETACH_CURRENT_THREAD

				if (!b)
					return LYNX_FALSE;

				if (!m_FileStream.vOpen(_T("../temp/temp_read_stream.bin"), CStream::Binary|CStream::Read))
					return LYNX_FALSE;

				m_bValid = LYNX_TRUE;
			}

			return TRUE;				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CURLStream::vClose() 
		{
			if (m_bValid)
            {
                if (m_FileStream.IsValid())
					m_FileStream.vClose();                                      	
            }
			m_bValid = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vRead(void *lpdst, int size, int len) 
		{
			return m_FileStream.vRead(lpdst, size, len);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vRead(CString& str) 
		{
			return m_FileStream.vRead(str);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vWrite(const void *lpsrc, int size, int len)
		{
			return m_FileStream.vWrite(lpsrc, size, len);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vWrite(const CString& str)
		{
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CURLStream::vSeek(LYNXDWORD len, int flag)
		{
			return m_FileStream.vSeek(len, flag);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CURLStream::vTell()
		{
			return m_FileStream.vTell();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CURLStream::vSize()
		{
			return m_FileStream.vSize();
		}


		//-------------------------------------------------------------------------------------------------------
		//  		
		//  Implementation of CURLResumableStream object.
		//-------------------------------------------------------------------------------------------------------
		CURLResumableStream::CURLResumableStream(void)
			:CURLStream()
		{
			m_bValid = LYNX_FALSE;
			m_StreamType = URL_STREAM;
			m_TotalSize = 0;
			m_CurrentProgressSize = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CURLResumableStream::~CURLResumableStream(void)
		{
			vClose();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CURLResumableStream::vOpen(const CString& name, int flag)
		{
			if (flag & Write)
			{
				return LYNX_FALSE;
			}
			else // read
			{
				//if (URLDownloadToFile(NULL, name, _T("../temp/temp_read_stream.bin"), 0, NULL) != S_OK)
				//return LYNX_FALSE;
				
				LYNXCHAR FileNameWithoutExt[256], TempFile[256];
				lynxGetFilenameOnlyByPath(FileNameWithoutExt, name.c_str());
				lynxSprintf(TempFile, _T("../temp/%s.bin"), FileNameWithoutExt);

				JNI_ATTACH_CURRENT_THREAD

				android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
				jclass cls = env->GetObjectClass(app->activity->clazz);
				jmethodID download = env->GetMethodID(cls, "DownloadURLtoFile", "(Ljava/lang/String;Ljava/lang/String;Z)Z");
				jstring u = NewModifiedUTF8String(env, name.c_str());
				// jstring n = NewModifiedUTF8String(env, _T("../temp/temp_read_stream.bin"));
				jstring n = NewModifiedUTF8String(env, TempFile);
				jboolean b = env->CallBooleanMethod(app->activity->clazz, download, u, n, LYNX_TRUE);
				
				env->DeleteLocalRef(u);
				env->DeleteLocalRef(n);

				JNI_DETACH_CURRENT_THREAD

					if (!b)
						return LYNX_FALSE;

				// if (!m_FileStream.vOpen(_T("../temp/temp_read_stream.bin"), CStream::Binary | CStream::Read))
				if (!m_FileStream.vOpen(TempFile, CStream::Binary | CStream::Read))
					return LYNX_FALSE;

				m_bValid = LYNX_TRUE;
			}

			return TRUE;
		}		
	}
}