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

#ifndef __LYNXUPDATE_H__
#define __LYNXUPDATE_H__

#include <LynxSystem.h>

namespace LynxEngine 
{
	namespace FileSystem 
	{
		class LYNXENGCLASS CUpdate : public CObj
		{
            LYNX_DECLARE_CLASS(CUpdate);
		public:		
			static const int					STATE_NONE              = 0;
			static const int					STATE_DOWNLOADING       = 1;
			static const int					STATE_EXTRACTING        = 2;
			static const int					STATE_DONE              = 3;
            static const int					STATE_ERROR_NO_UPDATE	= 4;
            static const int					STATE_ERROR_EXTRACTING	= 5;

			float								m_DownloadRatio;
			float								m_ExtractRatio;

			typedef struct {
				int								Version;
				CString							URL;
				LYNXLONG						Size;
			}UPDATEDATA;
		protected:
			int									m_CurrentState;
			int									m_CurrentVersion;
			CJsonValue							m_UpdateInfo;
			CString								m_UpdateURL;
            CPlatformURLStream*                 m_lpUpdateStream;
			CZipStream*							m_lpZipStream;
			CArray<UPDATEDATA>					m_UpdateArray;
		public:			
			CUpdate(CFileSystem* const lpsys);
			~CUpdate();			

			LYNXFORCEINLINE int					GetCurrentState() const { return m_CurrentState; }

			LYNXFORCEINLINE void				SetCurrentVersion(int v) { m_CurrentVersion = v; }
			LYNXFORCEINLINE const int			GetCurrentVersion() const { return m_CurrentVersion; }

			LYNXFORCEINLINE const CZipStream*	GetZipFileStream() const {return m_lpZipStream; }
			LYNXFORCEINLINE CPlatformURLStream*	GetPlatformURLStream() {return m_lpUpdateStream; }

			LYNXBOOL							DoesNeedUpdate(const CString& url);
			void								GetLatestUpdateVersion(int& ver, const CString& url);
			LYNXBOOL							CheckUpdate(const CString& url, int& latestVersion);
			LYNXBOOL							StartDownloadUpdate(const CString& dest_path, const CAnsiString& password);
			LYNXBOOL							ExtractUpdate(const CString& dest_path, const CAnsiString& password = "");
            unsigned long 						GetDownloadProgressSize();
            unsigned long 						GetDownloadTotalSize();
			float								GetDownloadRatio();
			CString								GetFileProgressString();
			int									GetDownloadingProgressPecentage();
			CString								GetDownloadingProgressPecentageString();			
			int									GetFileProgressPecentage();
			CString								GetFileProgressPecentageString();
			float								GetFileRatio();			
		};	

		LYNXINLINE bool operator > (const CUpdate::UPDATEDATA& lhs, const CUpdate::UPDATEDATA& rhs) { return (lhs.Version > rhs.Version); };
		LYNXINLINE bool operator < (const CUpdate::UPDATEDATA& lhs, const CUpdate::UPDATEDATA& rhs) { return (lhs.Version < rhs.Version); };
		LYNXINLINE bool operator == (const CUpdate::UPDATEDATA& lhs, const CUpdate::UPDATEDATA& rhs) { return (lhs.Version == rhs.Version); };
	}
}

#endif