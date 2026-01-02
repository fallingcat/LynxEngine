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

#ifndef __LYNXSTREAM_H__
#define __LYNXSTREAM_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	/**	
	*  @brief Base stream object class。
	* 
	*  The basic stream object class. 
	*	
	*/
	class LYNXENGCLASS CStream : public CUnknown
	{	
		LYNX_DECLARE_CLASS(CStream);
	public:		
		static const int SeekSet = SEEK_SET;
		static const int SeekEnd = SEEK_END;
		static const int SeekCur = SEEK_CUR;		

		static const int Binary     = 0x01;		
		static const int Text       = 0x02;
		static const int Write      = 0x04;
		static const int Read       = 0x08;
		static const int ANSI       = 0x10;
        static const int UTF8       = 0x20;
        static const int UTF16      = 0x40;
		static const int Append		= 0x80;

		typedef enum {
			FILE_STREAM = 0,
			MEMORY_STREAM,
			ZIP_STREAM,
			URL_STREAM,
			NUM_STREAMTYPE,
		}STREAMTYPE;

	public:		
	protected:					
		LYNXBOOL					m_bValid;
		STREAMTYPE					m_StreamType;
	public:	
		CStream() : CUnknown() { m_bValid = LYNX_FALSE; };
		virtual ~CStream() {};

		LYNXFORCEINLINE STREAMTYPE	GetStreamType() const {return m_StreamType; };
		virtual LYNXBOOL			vOpen(const CString& name, int flag = Binary|Read) = 0;
		virtual void				vClose() = 0;
		virtual int					vRead(void *lpdst, int size, int len) = 0;
		virtual int					vRead(CString& str) = 0;
        virtual int					vWrite(const void *lpsrc, int size, int len) = 0;
		virtual int					vWrite(const CString& str) = 0;
		virtual void				vAppend(const void *lpsrc, int size, int len) {};
		virtual void				vAppend(const CString& str) {};
		#ifdef _UNICODE
			virtual int				vRead(CAnsiString& str) = 0;
			virtual int				vWrite(const CAnsiString& str) = 0;
		#endif
		virtual int					vSeek(LYNXDWORD len, int flag) = 0;
		virtual LYNXDWORD			vTell() = 0;
		virtual LYNXDWORD			vSize() = 0;

		LYNXFORCEINLINE  void		SetValid(LYNXBOOL b) {m_bValid = LYNX_TRUE; };
		LYNXFORCEINLINE  const LYNXBOOL	IsValid() const {return m_bValid; };		
	};		
}

#endif