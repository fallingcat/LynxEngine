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

#ifndef __LYNXPACKAGE_H__
#define __LYNXPACKAGE_H__

#include <LynxSystem.h>

namespace LynxEngine 
{
	namespace FileSystem 
	{
		class LYNXENGCLASS CPackage : public CObj
		{
            LYNX_DECLARE_CLASS(CPackage);
		public:
			typedef struct {
				LYNXULONG				Offset;
				LYNXULONG				Size;
			}PACKDATA;
		private:		
		protected:
			CFileSystem*						m_lpFileSystem;			
			CArray<CStream*>					m_ZipStreamList;
			//CArray<CStream*>					m_StreamList;
			LYNXBOOL							m_bVaild;

			CMap<CString, PACKDATA>				m_PackMeta;
			CStream*							m_PackFile;
		public:			
			CPackage(CFileSystem* const lpsys);
			~CPackage();			

			LYNXFORCEINLINE LYNXBOOL			IsValid() {return m_bVaild; }
			LYNXBOOL							Open(const CString& name);
			void								Close();
			CStream*							GetStream();
			void								DiscardStream(CStream* fs) ;
			void								ClearInvalidStream();
			void								ClearClosedStream();




			LYNXBOOL							OpenFile(CStream* stream, const CString& name, const int flag);
		};	
	}
}

#endif