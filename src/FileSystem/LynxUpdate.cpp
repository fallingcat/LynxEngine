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

#include <LynxEngine_PCH.h>
#include <FileSystem/LynxPackage.h>

#define FAKE_PROGRESS								0
#define SUPPORT_RESUMABLE_DOWNLOAD					1

namespace LynxEngine
{
	namespace FileSystem
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CUpdate::CUpdate(CFileSystem* const lpsys)
			: CObj(lpsys->GetlpEngine())
		{
			m_CurrentVersion = 0;
			m_UpdateURL = _T("");
			m_lpUpdateStream = NULL;
			m_lpZipStream = NULL;
			m_DownloadRatio = 0.0f;
			m_ExtractRatio = 0.0f;
			m_CurrentState = STATE_NONE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CUpdate::~CUpdate(void)
		{
			if (m_lpZipStream)
			{
				m_lpZipStream->vClose();
				m_lpZipStream->CloseZipFile();
				LYNXDEL(m_lpZipStream);
				m_lpZipStream = NULL;
			}

			if (m_lpUpdateStream)
			{
				m_lpUpdateStream->vClose();
				LYNXDEL(m_lpUpdateStream);
				m_lpUpdateStream = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUpdate::DoesNeedUpdate(const CString& url)
		{
			int LatestVersion;
			unsigned int UpdateFileSize;

			CPlatformHttpRequest Request(m_lpEngine);
			Request.vInit(url);
			if (Request.vStartSynchronous(NetworkSystem::CHttpRequest::MT_GET))
			{
				Request.vGetJSONResponse(m_UpdateInfo);
				for (Json::ValueIterator itr = m_UpdateInfo.begin(); itr != m_UpdateInfo.end(); itr++)
				{
					if (!(*itr)["version"].empty())
					{
						LatestVersion = lynxAtoi((*itr)["version"].JSON_VALUE_AS_CSTRING());

						if (LatestVersion > m_CurrentVersion)
						{
							return LYNX_TRUE;
						}
					}
				}
			}
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CUpdate::GetLatestUpdateVersion(int& ver, const CString& url)
		{
			int LatestVersion = 0;
			unsigned int UpdateFileSize;

			CPlatformHttpRequest Request(m_lpEngine);
			Request.vInit(url);
			if (Request.vStartSynchronous(NetworkSystem::CHttpRequest::MT_GET))
			{
				Request.vGetJSONResponse(m_UpdateInfo);
				for (Json::ValueIterator itr = m_UpdateInfo.begin(); itr != m_UpdateInfo.end(); itr++)
				{
					if (!(*itr)["version"].empty())
					{
						int PatchVer = lynxAtoi((*itr)["version"].JSON_VALUE_AS_CSTRING());
						if (PatchVer > LatestVersion)
							LatestVersion = PatchVer;
					}
				}
			}
			ver = LatestVersion;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUpdate::CheckUpdate(const CString& url, int& LatestVersion)
		{
			LatestVersion = 0;
			unsigned int UpdateFileSize(0);

			m_UpdateInfo.clear();
			CPlatformHttpRequest Request(m_lpEngine);
			Request.vInit(url);
			Request.vSetCompressionMethod(NetworkSystem::CHttpRequest::CMT_ALL);
			if (Request.vStartSynchronous(NetworkSystem::CHttpRequest::MT_GET))
			{
				Request.vGetJSONResponse(m_UpdateInfo);

				m_UpdateArray.resize(m_UpdateInfo.size());
				int Index = 0;
				for (Json::ValueIterator itr = m_UpdateInfo.begin(); itr != m_UpdateInfo.end(); itr++)
				{
					//LatestVersion = lynxAtoi((*itr)["version"].JSON_VALUE_AS_CSTRING());
					m_UpdateArray[Index].Version = lynxAtoi((*itr)["version"].JSON_VALUE_AS_CSTRING());;
					m_UpdateArray[Index].URL = (*itr)["url"].JSON_VALUE_AS_CSTRING();
					m_UpdateArray[Index].Size = lynxAtoi((*itr)["size"].JSON_VALUE_AS_CSTRING());
					Index++;
				}

				LynxEngine::sort(m_UpdateArray.begin(), m_UpdateArray.end());

				LatestVersion = m_UpdateArray[m_UpdateArray.size() - 1].Version;
				return LYNX_TRUE;
			}

			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUpdate::StartDownloadUpdate(const CString& dest_path, const CAnsiString& password)
		{
			int LatestVersion(0);
			unsigned int UpdateFileSize(0);

			LYNXCHAR FileNameWithoutExt[256], TempFile[256];
			for (int i = 0; i < m_UpdateArray.size(); i++)
			{
				LatestVersion = m_UpdateArray[i].Version;
				m_UpdateURL = m_UpdateArray[i].URL;
				UpdateFileSize = m_UpdateArray[i].Size;

				if (LatestVersion > m_CurrentVersion)
				{
					lynxCreateFolder(_T("../temp/"));

#if SUPPORT_RESUMABLE_DOWNLOAD
					lynxGetFilenameOnlyByPath(FileNameWithoutExt, m_UpdateURL.c_str());
					lynxSprintf(TempFile, _T("../temp/%s.bin"), FileNameWithoutExt);
#else								
					lynxSprintf(TempFile, _T("../temp/temp_read_stream.bin"));
					lynxDeleteFile(TempFile);
#endif
					lynxDeleteFile(_T("../temp/update.zip"));

					//downlaod update
					m_CurrentState = STATE_DOWNLOADING;
					if (m_lpUpdateStream)
					{
						m_lpUpdateStream->vClose();
						LYNXDEL(m_lpUpdateStream);
						m_lpUpdateStream = NULL;
					}

#if SUPPORT_RESUMABLE_DOWNLOAD					
					m_lpUpdateStream = LYNXNEW CPlatformURLResumableStream();
#else
					m_lpUpdateStream = LYNXNEW CPlatformURLStream();
#endif
					m_lpUpdateStream->m_TotalSize = UpdateFileSize;

					if (m_lpUpdateStream->vOpen(m_UpdateURL, CStream::Read | CStream::Binary))
					{
						m_lpUpdateStream->vClose();

						lynxRenameFile(_T("../temp/update.zip"), TempFile);
						//LYNXCRC UpodateCRC = m_lpEngine->ComputeFileCRC(CString("../temp/update.zip"));

						//Extract the update
						if (!ExtractUpdate(dest_path, password))
							return LYNX_FALSE;

						m_CurrentVersion = LatestVersion;

						lynxDeleteFile(_T("../temp/update.zip"));
						lynxDeleteFile(TempFile);
					}
					else
					{
						m_CurrentState = STATE_ERROR_NO_UPDATE;
						return LYNX_FALSE;
					}
				}
			}
			m_UpdateArray.clear();
			m_CurrentState = STATE_DONE;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//	dest_path : destination path
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CUpdate::ExtractUpdate(const CString& dest_path, const CAnsiString& password)
		{
			if (m_lpZipStream)
			{
				m_lpZipStream->vClose();
				m_lpZipStream->CloseZipFile();
				LYNXDEL(m_lpZipStream);
				m_lpZipStream = NULL;
			}

			m_lpZipStream = LYNXNEW CZipStream();

			if (m_lpZipStream)
			{
				m_CurrentState = STATE_EXTRACTING;
				m_lpZipStream->SetPassword(password.c_str());

				if (m_lpZipStream->OpenZipFile(_T("../temp/update.zip")))
				{
					if (m_lpZipStream->ExtractAll(dest_path))
					{
						m_CurrentState = STATE_DOWNLOADING;
						m_lpZipStream->vClose();
						m_lpZipStream->CloseZipFile();
						LYNXDEL(m_lpZipStream);
						m_lpZipStream = NULL;

						return LYNX_TRUE;
					}
				}
				else
				{
					if (m_lpZipStream->OpenZipFile(_T("../temp/temp_read_stream.bin")))
					{
						if (m_lpZipStream->ExtractAll(dest_path))
						{
							m_CurrentState = STATE_DOWNLOADING;
							m_lpZipStream->vClose();
							m_lpZipStream->CloseZipFile();
							LYNXDEL(m_lpZipStream);
							m_lpZipStream = NULL;

							return LYNX_TRUE;
						}
					}
					else
					{
						m_CurrentState = STATE_ERROR_NO_UPDATE;
						return LYNX_FALSE;
					}
				}
			}

			m_CurrentState = STATE_ERROR_EXTRACTING;
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		unsigned long CUpdate::GetDownloadProgressSize()
		{
			unsigned long Size = 0;
			if (m_CurrentState == STATE_DOWNLOADING)
			{
				if (m_lpUpdateStream)
					Size = m_lpUpdateStream->m_CurrentProgressSize;
			}
			return Size;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		unsigned long CUpdate::GetDownloadTotalSize()
		{
			unsigned long Size = 0;
			if (m_CurrentState == STATE_DOWNLOADING)
			{
				if (m_lpUpdateStream)
					Size = m_lpUpdateStream->m_TotalSize;
			}
			return Size;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		float CUpdate::GetDownloadRatio()
		{
			return ((float)m_lpUpdateStream->m_CurrentProgressSize / m_lpUpdateStream->m_TotalSize);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CUpdate::GetDownloadingProgressPecentage()
		{
			static LYNXLONGLONG T0 = 0;
			static int LastPercentage = 0;
			static int FakePercentage = 0;

			if (m_CurrentState == STATE_DOWNLOADING && m_lpUpdateStream)
			{
				int Percentage = ((float)m_lpUpdateStream->m_CurrentProgressSize / m_lpUpdateStream->m_TotalSize) * 100.0f;
#if FAKE_PROGRESS
				int TimeLimit = (lynxRand() % 10 + 8) * 100;
				if (LastPercentage == Percentage)
				{
					if (m_lpEngine->GetlpKernel()->ElapsedKernelTimeSince(T0) >= TimeLimit)
					{
						FakePercentage++;
						if (FakePercentage > 99)
							FakePercentage = 99;

						Percentage = FakePercentage;

						T0 = m_lpEngine->GetlpKernel()->GetKernelTime();
					}
					else
					{
						if (Percentage < FakePercentage)
						{
							Percentage = FakePercentage;
						}
						else
						{
							FakePercentage = Percentage;
						}
					}
				}
				else
				{
					LastPercentage = Percentage;
					if (Percentage < FakePercentage)
					{
						Percentage = FakePercentage;
					}
					else
					{
						FakePercentage = Percentage;
					}
				}
#endif
				m_DownloadRatio = Percentage / 100.0f;
				return Percentage;
			}
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString CUpdate::GetDownloadingProgressPecentageString()
		{
			LYNXCHAR Buffer[128];

			lynxSprintf(Buffer, _T("%d%%%%"), GetDownloadingProgressPecentage());
			return CString(Buffer);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString CUpdate::GetFileProgressString()
		{
			LYNXCHAR Buffer[128];

			lynxStrCpy(Buffer, _T(""));

			if (m_CurrentState == STATE_EXTRACTING && m_lpZipStream)
			{
				lynxSprintf(Buffer, _T("%d/%d"), m_lpZipStream->m_CurrentExtractedFiles, m_lpZipStream->m_TotalFiles);
			}

			return CString(Buffer);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CUpdate::GetFileProgressPecentage()
		{
			static LYNXLONGLONG T0 = 0;
			static int LastPercentage = 0;
			static int FakePercentage = 0;

			if (m_CurrentState == STATE_EXTRACTING && m_lpZipStream)
			{
				int Percentage = ((float)m_lpZipStream->m_CurrentExtractedFiles / m_lpZipStream->m_TotalFiles) * 100.0f;

#if FAKE_PROGRESS
				int TimeLimit = (lynxRand() % 10 + 8) * 100;
				if (LastPercentage == Percentage)
				{
					if (m_lpEngine->GetlpKernel()->ElapsedKernelTimeSince(T0) >= TimeLimit)
					{
						FakePercentage++;
						if (FakePercentage > 99)
							FakePercentage = 99;

						Percentage = FakePercentage;

						T0 = m_lpEngine->GetlpKernel()->GetKernelTime();
					}
					else
					{
						if (Percentage < FakePercentage)
						{
							Percentage = FakePercentage;
						}
						else
						{
							FakePercentage = Percentage;
						}
					}
				}
				else
				{
					LastPercentage = Percentage;
					if (Percentage < FakePercentage)
					{
						Percentage = FakePercentage;
					}
					else
					{
						FakePercentage = Percentage;
					}
				}
#endif
				m_ExtractRatio = Percentage / 100.0f;
				return Percentage;
			}
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString CUpdate::GetFileProgressPecentageString()
		{
			LYNXCHAR Buffer[128];

			lynxSprintf(Buffer, _T("%d%%%%"), GetFileProgressPecentage());
			return CString(Buffer);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		float CUpdate::GetFileRatio()
		{
			return ((float)m_lpZipStream->m_CurrentExtractedFiles / m_lpZipStream->m_TotalFiles);
		}
	}
}
