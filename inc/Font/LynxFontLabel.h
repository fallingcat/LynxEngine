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

#ifndef __LYNXFONTLABEL_H__
#define __LYNXFONTLABEL_H__

#include <LynxEngineClasses.h>

namespace LynxEngine
{
	class LYNXENGCLASS CFontLabel : public CObj
    {
        LYNX_DECLARE_CLASS(CFontLabel);
    public:
    protected:
        LYNXBOOL                                    m_bShow;
        WORD										m_AlignmentType;
		LYNXRECT									m_AlignmentRect;
    public:
        CFontLabel(CEngine *lpengine):CObj(lpengine) {m_bShow = LYNX_TRUE; };
        virtual ~CFontLabel() {};
        
        virtual void                                vRelease() = 0;
        virtual LYNXBOOL                            vCreate() = 0;
        virtual void                                vSetFont(CTrueTypeFont* lpfont) = 0;
        virtual void                                vShow(LYNXBOOL b) {m_bShow = b;};
        
        LYNXFORCEINLINE void                        SetAlignmentType(const WORD at) {m_AlignmentType = at; };
		LYNXFORCEINLINE const WORD                  GetAlignmentType() const {return m_AlignmentType;};
		LYNXFORCEINLINE void                        SetAlignmentRect(LYNXRECT* lpr) {m_AlignmentRect = (*lpr); };
		LYNXFORCEINLINE void                        GetAlignmentRect(LYNXRECT* lpr) {(*lpr) = m_AlignmentRect; };
        
        virtual void                                vAutoReturn(LYNXBOOL b) = 0;
        
        virtual void                                vPrintf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...) = 0;
        virtual void                                vPrintf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...) = 0;
        virtual int                                 vPrintfLength(const LYNXCHAR *fmt, ...) = 0;
    };
}

#endif
