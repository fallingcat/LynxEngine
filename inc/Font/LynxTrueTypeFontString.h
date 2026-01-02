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
//  ?°?˙:
//
//###########################################################################

#ifndef __LYNXTRUETYPEFONTSTRING_H__
#define __LYNXTRUETYPEFONTSTRING_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

namespace LynxEngine
{
	class LYNXENGCLASS CTrueTypeFontString : public CObj
	{
		LYNX_DECLARE_CLASS(CTrueTypeFontString);
	public:
	protected:
        int                             m_CurrentLabel;
        CTrueTypeFont*                  m_CurrentFont;
        WORD                            m_CurrentAlignmentType;
        LYNXRECT                        m_CurrentAlignmentRect;
        CArray<CFontLabel*>             m_FontLabelArray;
	public:
		CTrueTypeFontString(CEngine *lpengine);
		virtual ~CTrueTypeFontString();
        
		virtual void					vRelease();
		LYNXBOOL						Create(const DWORD init_num_label);
		void							SetFont(CFontPtr& lpfont);
		
        void							SetOriginType(const CSpriteContainer::ORIGINTYPE ot);
		const CSpriteContainer::ORIGINTYPE GetOriginType() const;
        void                            SetAlignmentType(const WORD at);
		const WORD                      GetAlignmentType();
		void                            SetAlignmentRect(LYNXRECT* lpr);
		void                            GetAlignmentRect(LYNXRECT* lpr);
        void                            AutoReturn(LYNXBOOL b);
        
		void							Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);
		void							Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);
		int								PrintfLength(const LYNXCHAR *fmt, ...);
        
        void                            Reset();
    protected:
        void                            CreateLabels(int count);
	};
}

#endif