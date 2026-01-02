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

#ifndef __LYNXFONTSTRING_H__
#define __LYNXFONTSTRING_H__

#include <LynxEngineClasses.h>
#include <Container/LynxSpriteContainer.h>

namespace LynxEngine 
{	
	class CFont;
	class LYNXENGCLASS CFontString : public CSpriteContainer
	{	
		LYNX_DECLARE_CLASS(CFontString);
	public:			
		static int						m_BatchSize;
        LYNXBOOL                        m_bAutoReturn;
	protected:				
		CFont*							m_lpFont;
		GraphicsSystem::CIndexArrayPtr	m_IndexArray;
		DWORD							m_MaxLen;	
		LYNXVECTOR4D*					m_PosArray;
		LYNXVECTOR4D*					m_TexCoordArray;		
	
	public:			
		CFontString(CEngine *lpengine);
		virtual ~CFontString();			

		virtual void					vRelease();
		LYNXBOOL						Create(DWORD maxlen, LYNXBOOL bbo = LYNX_TRUE);		
		void							SetFont(CFont* lpfont);
		
		void							Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);	
		void							Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);	
		int								PrintfLength(const LYNXCHAR *fmt, ...);
	};					
}

#endif