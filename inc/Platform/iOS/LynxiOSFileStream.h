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

#ifndef __LYNXIOSFILESTREAM_H__
#define __LYNXIOSFILESTREAM_H__

#include <LynxEngineClasses.h>
#include <LynxStream.h>

LYNX_FORWARD_DECLARATION(NSString)
LYNX_FORWARD_DECLARATION(NSArray)
LYNX_FORWARD_DECLARATION(NSFileHandle)

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
		class LYNXENGCLASS CFileStream : public CStream
		{	
			LYNX_DECLARE_CLASS(CFileStream);
		public:		
		protected:		
			NSFileHandle*               m_NSFile;
            unsigned int                m_Encoding;
            unsigned int                m_CurrentReadLine;
            NSString*                   m_Input;
            NSArray*                    m_Lines;
            
            FILE*                       m_lpFile;
            
            //unsigned long               m_TotalProcessedLen;
		public:	
			CFileStream();
			virtual ~CFileStream();

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
		};	
	}
}

#endif