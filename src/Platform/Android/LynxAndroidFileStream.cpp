//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.com
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <Platform/Android/LynxAndroidFileStream.h>
#include <Platform/Android/LynxAndroidGameFrameworkKernel.h>

//extern AAssetManager*				glpAssetManager;
//AAssetManager*						glpAssetManager = NULL;

namespace LynxEngine 
{	
	namespace Android
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CFileStream::CFileStream(void)	
		{
			m_AssetManager = NULL;
			m_lpAsset = NULL;
			m_lpFile = NULL;
			
			m_InternalPath = CString(static_cast<LynxEngine::GameFramework::Android::CKernel*>(glpKernel)->GetApp()->activity->internalDataPath);
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CFileStream::~CFileStream(void)
		{
			vClose();
			m_AssetManager = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CFileStream::vOpen(const CString& name, int flag) 
		{
			m_bValid = LYNX_FALSE;
			
			m_AssetManager = static_cast<LynxEngine::GameFramework::Android::CKernel*>(glpKernel)->GetAssetManager();

			if (!m_AssetManager)
				return LYNX_FALSE;
			
			CString Filename = name;
			CEngine::FilenameBackslashToSlash(Filename);

			if (Filename.c_str()[0] == _T('.') && Filename.c_str()[1] == _T('.') && Filename.c_str()[2] == _T('/'))
				Filename = &(Filename.c_str()[3]);
			else if (Filename.c_str()[0] == _T('.') && Filename.c_str()[1] == _T('/'))
				Filename = &(Filename.c_str()[2]);
			
			//LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Open "))+Filename+CString(_T("...\r\n")));
			//lynxDebugOutputPrintf("Filename: %s\n", Filename.c_str());
			if (Filename == CString(""))
				return LYNX_FALSE;
			
			#ifdef _UNICODE
				m_lpAsset = AAssetManager_open(m_AssetManager, CAnsiString(Filename.c_str()).c_str(), AASSET_MODE_UNKNOWN);
			#else
				m_lpAsset = AAssetManager_open(m_AssetManager, Filename.c_str(), AASSET_MODE_UNKNOWN);
			#endif
			
			if (flag & Write)
			{
				LYNXCHAR Flag[32] = _T("");
				lynxStrCat(Flag, _T("w"));

				if (flag & Binary)
					lynxStrCat(Flag, _T("b"));
				else if (flag & Text)
				{
					lynxStrCat(Flag, _T("t"));
					if (flag & ANSI)
					{
					}
					else if (flag & UTF8)
					{
						lynxStrCat(Flag, _T(",ccs=UTF-8"));				
					}
					else if (flag & UTF16)
					{
						lynxStrCat(Flag, _T(",ccs=UTF-16LE"));				
					}
					else
					{					
						#if defined(_UNICODE) || defined (UNICODE)
							lynxStrCat(Flag, _T(",ccs=UTF-8"));				
						#endif
					}
				}
				else
					lynxStrCat(Flag, _T("b"));

				//m_lpFile = [NSFileHandle fileHandleForWritingAtPath:[[NSBundle mainBundle] pathForResource:Name ofType:@""]];
				//CString Path = CString(static_cast<LynxEngine::GameFramework::Android::CKernel*>(glpKernel)->GetApp()->activity->internalDataPath);

				LYNXCHAR P[256], N[256], E[256];
				lynxSeparatePathFilenameExtByFullPath(P, N, E, (m_InternalPath + CString(_T("/")) + Filename).c_str());

				struct stat info;
				if( stat(P, &info ) != 0 )
				{
					JNI_ATTACH_CURRENT_THREAD

					android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
					jclass cls = env->GetObjectClass(app->activity->clazz);
					jmethodID makeDirectories = env->GetMethodID(cls, "MakeDirectories", "(Ljava/lang/String;)V");
					jstring n = NewModifiedUTF8String(env, Filename.c_str());
					env->CallVoidMethod(app->activity->clazz, makeDirectories, n);
					env->DeleteLocalRef(n);

					JNI_DETACH_CURRENT_THREAD
				}

				m_lpFile = _tfopen((m_InternalPath + CString(_T("/")) + Filename).c_str(), Flag);

			}
			//else if (flag & Read)
			else
			{
				LYNXCHAR Flag[32] = _T("");
				lynxStrCat(Flag, _T("r"));
				//CString Path = CString(static_cast<LynxEngine::GameFramework::Android::CKernel*>(glpKernel)->GetApp()->activity->internalDataPath);
				//lynxDebugOutputPrintf("Filepath: %s\n", (m_InternalPath + CString(_T("/")) + Filename).c_str());
				m_lpFile = _tfopen((m_InternalPath + CString(_T("/")) + Filename).c_str(), "r");
			}
			/*else
			{
				m_lpFile = [NSFileHandle fileHandleForReadingAtPath:[[NSBundle mainBundle] pathForResource:Name ofType:@""]];
			}
			*/

			

			if (m_lpFile == NULL && m_lpAsset == NULL)
			{
				//LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Open "))+Filename+CString(_T(" failed.\r\n")));
				return LYNX_FALSE;		
			}
			
			m_bValid = LYNX_TRUE;
			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Open "))+Filename+CString(_T(" OK\r\n")));

			return LYNX_TRUE;		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CFileStream::vClose() 
		{
			if (m_lpFile)
			{
				fclose(m_lpFile);
				m_lpFile = NULL;
			}
			if (m_lpAsset)
			{
				AAsset_close(m_lpAsset);
				m_lpAsset = NULL;
			}			
			m_bValid = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vRead(void *lpdst, int size, int len) 
		{	
			if (m_lpFile!=NULL)
				return (int)fread(lpdst, size, len, m_lpFile);
			else
				return AAsset_read(m_lpAsset, lpdst, size*len);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vRead(CString& str) 
		{
			int Len = 0;

			if (m_lpFile!=NULL)
			{
				LYNXCHAR Str[1024];

				int Last;
				str = _T("");
				do {
					if (!_fgetts(Str, 1024, m_lpFile))
						return 0;
					str += CString(Str);
					Last = lynxStrLen(Str) - 1;
				} while (!feof(m_lpFile) && Str[Last] != _T('\n'));
		
				Len = str.length();
			}
			else
			{
				LYNXCHAR C[4];

				str.clear();
				while (1)				
				{
					if (AAsset_read(m_lpAsset, (void*)C, sizeof(LYNXCHAR)) <= 0)
						break;

					if (C[0] == '\n')
						break;

					C[1] = 0x00;
					str += CString(C);
					Len++;
				}
			}
			/*
			#ifdef _UNICODE
				NSString* Input = [[NSString alloc] initWithData: [m_lpFile availableData] encoding:NSUTF8StringEncoding]; 
				str = [Input cStringUsingEncoding:NSUTF8StringEncoding];
				[Input release];
			#else
				NSString* Input = [[NSString alloc] initWithData: [m_lpFile availableData] encoding:NSASCIIStringEncoding]; 
				str = [Input cStringUsingEncoding:NSASCIIStringEncoding];
				[Input release];
			#endif		
			*/
					
			return Len;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vWrite(const void *lpsrc, int size, int len)
		{
			return (int)fwrite(lpsrc, size, len, m_lpFile);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vWrite(const CString& str)
		{
			return (int)_fputts(str.c_str(), m_lpFile);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CFileStream::vSeek(LYNXDWORD len, int flag)
		{
			if (m_lpFile!=NULL)
				return fseek(m_lpFile, len, flag);
			else
				return AAsset_seek(m_lpAsset, len, flag);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CFileStream::vTell()
		{
			if (m_lpFile!=NULL)
			{
				return ftell(m_lpFile);
			}
			else
			{
				LYNXDWORD Tell = vSize() - AAsset_getRemainingLength(m_lpAsset);
				return Tell;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXDWORD CFileStream::vSize()
		{
			
			if (m_lpFile!=NULL)
			{
				LYNXDWORD	Len, Offset;
				Offset = ftell(m_lpFile); 
				vSeek(0, SeekEnd);
				Len = ftell(m_lpFile); 
				vSeek(Offset, SeekSet);
				return Len;
			}
			else
			{
				LYNXDWORD Tell = AAsset_getLength(m_lpAsset);
				return Tell;
			}
		}
	}
}