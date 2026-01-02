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

#ifndef __LYNXANDROIDFILESTREAM_H__
#define __LYNXANDROIDFILESTREAM_H__

#include <LynxEngineClasses.h>
#include <LynxStream.h>
#include <android/asset_manager.h>

namespace LynxEngine 
{
	namespace Android
	{		
		/**	
		*  @brief Base stream object class of the entire engine。
		* 
		*  The basic stream object class of the entire engine. 
		*	
		*/
		class LYNXENGCLASS CFileStream : public CStream
		{	
			LYNX_DECLARE_CLASS(CFileStream);
		public:		
		protected:					
			AAssetManager*				m_AssetManager;
			AAsset*						m_lpAsset;
			FILE*						m_lpFile;
			CString						m_InternalPath;
		public:	
			CFileStream();
			virtual ~CFileStream();

			virtual LYNXBOOL			vOpen(const CString& name, int flag = Binary|Read);
			virtual void				vClose();
			virtual int					vRead(void *lpdst, int size, int len);
			virtual int					vRead(CString& str);
			virtual int					vWrite(const void *lpsrc, int size, int len);
			virtual int					vWrite(const CString& str);
			virtual int					vSeek(LYNXDWORD len, int flag);
			virtual LYNXDWORD			vTell();
			virtual LYNXDWORD			vSize();
		};	
	}
}

#endif