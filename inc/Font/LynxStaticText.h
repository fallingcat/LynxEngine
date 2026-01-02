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
//
//###########################################################################

#ifndef __LYNXSTATICTEXT_H__
#define __LYNXSTATICTEXT_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>
#include <Container/LynxSpriteContainer.h>

namespace LynxEngine
{
	class LYNXENGCLASS CStaticText : public CObj
	{
		LYNX_DECLARE_CLASS(CStaticText);
	public:
        typedef enum {
            STT_TEXTURE_FONT = 0,
            STT_OS_FONT,
        }STATICTEXTTYPE;
	protected:
		STATICTEXTTYPE                  m_FontType;
		CPlatformFontLabel*             m_FontLabel;
        CTextureFontString*             m_FontString;
	public:
		CStaticText(CEngine *lpengine);
		virtual ~CStaticText();
        
		LYNXBOOL						Create(STATICTEXTTYPE type);
		void							SetFont(CResource* lpfont);
		
        void                            Show(LYNXBOOL b);
        
        void							SetOriginType(const CSpriteContainer::ORIGINTYPE ot);
		const CSpriteContainer::ORIGINTYPE GetOriginType() const;
        void                            SetAlignmentType(const WORD at);
		const WORD                      GetAlignmentType();
		void                            SetAlignmentRect(LYNXRECT* lpr);
		void                            GetAlignmentRect(LYNXRECT* lpr);
        
		void							Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);
		void							Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);
		int								PrintfLength(const LYNXCHAR *fmt, ...);
	};
}

#endif