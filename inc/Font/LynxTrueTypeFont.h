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

#ifndef __LYNXTRUETYPEFONT_H__
#define __LYNXTRUETYPEFONT_H__

#include <LynxCore.h>
#include <Font/LynxFont.h>
#include <LynxMathWrap.h>
#include <LynxEngineClasses.h>

using namespace LynxEngine::Math;

namespace LynxEngine
{
	class LYNXENGCLASS CTrueTypeFont : public CFont
	{
		LYNX_DECLARE_CLASS(CTrueTypeFont);
	public:        
	protected:
	public:
		CTrueTypeFont(CEngine* const lpengine):CFont(lpengine) {m_FontType = FT_TRUETYPE_FONT;};
		virtual ~CTrueTypeFont() {};
        
		LYNXBOOL						vCreate(const FontCreationDesc* desc) {return LYNX_TRUE; }
	};					
}

#endif