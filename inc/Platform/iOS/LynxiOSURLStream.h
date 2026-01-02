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

#ifndef __LYNXIOSURLSTREAM_H__
#define __LYNXIOSURLSTREAM_H__

#include <LynxEngineClasses.h>
#include <LynxMemoryStream.h>
//#import <CFStream.h>

namespace LynxEngine 
{
	namespace iOS
	{		
		/**	
		*  @brief Base stream object class of the entire engine。
		* 
		*  The basic stream object class of the entire engine. 
		*	
		*/
		class LYNXENGCLASS CURLStream : public CStream
		{	
			LYNX_DECLARE_CLASS(CURLStream);
		public:		
            CString                     m_UploadFilename;
            unsigned long               m_TotalSize;
            unsigned long               m_CurrentProgressSize;
		protected:		
            CPlatformFileStream         m_FileStream;
			CFReadStreamRef				m_RStream;
            CFWriteStreamRef            m_WStream;
		public:	
			CURLStream();
			virtual ~CURLStream();

			virtual LYNXBOOL			vOpen(const CString& name, int flag = Binary|Read);
			virtual void				vClose();
			virtual int					vRead(void *lpdst, int size, int len);
			virtual int					vRead(CString& str);
			virtual int					vWrite(const void *lpsrc, int size, int len);
            virtual int					vWrite(const CString& str);
            #ifdef _UNICODE
                virtual int				vRead(CAnsiString& str) {return 0; };
                virtual int				vWrite(const CAnsiString& str) {return 0; };
            #endif            
			virtual int					vSeek(LYNXDWORD len, int flag);
			virtual LYNXDWORD			vTell();
			virtual LYNXDWORD			vSize();
            
            static LYNXBOOL             IsExisting(const CString& name);
		};	

		/**
		*  @brief The CURLStream derived class of the entire engine support resumable download
		*
		*  The CURLStream derived class of the entire engine support resumable download.
		*
		*/
		class LYNXENGCLASS CURLResumableStream : public CURLStream
		{
			LYNX_DECLARE_CLASS(CURLResumableStream);
		public:
			CURLResumableStream();
			virtual ~CURLResumableStream();

			virtual LYNXBOOL			vOpen(const CString& name, int flag = Binary | Read);		
		};
	}
}

#endif