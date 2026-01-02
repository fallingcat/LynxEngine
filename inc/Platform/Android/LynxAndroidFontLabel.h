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

#ifndef __LYNXANDROIDFONTLABEL_H__
#define __LYNXANDROIDFONTLABEL_H__

#include <LynxEngineClasses.h>
#include <Font/LynxFontLabel.h>

namespace LynxEngine
{
	namespace Android
    {
        class LYNXENGCLASS CFontLabel : public LynxEngine::CFontLabel
        {
            LYNX_DECLARE_CLASS(CFontLabel);
        public:
        protected:            
        public:
			CFontLabel(LynxEngine::CEngine *lpengine):LynxEngine::CFontLabel(lpengine) {};
			virtual ~CFontLabel() {};
            
			void                                vRelease() {};
			LYNXBOOL                            vCreate() {return LYNX_TRUE; };
			void                                vSetFont(LynxEngine::CTrueTypeFont* lpfont) {};
			void                                vShow(LYNXBOOL b) {};
			void                                vAutoReturn(LYNXBOOL b) {};
			void                                vPrintf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...) {};
			void                                vPrintf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...) {};
			int                                 vPrintfLength(const LYNXCHAR *fmt, ...) {return 0;};
        };
    }
}

#endif