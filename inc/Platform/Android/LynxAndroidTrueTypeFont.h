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

#ifndef __LYNXANDROIDTRUETYPEFONT_H__
#define __LYNXANDROIDTRUETYPEFONT_H__

#include <LynxEngineClasses.h>
#include <Font/LynxTrueTypeFont.h>
#include <Platform/Android/LynxAndroidUtility.h>

using namespace LynxEngine::Math;

namespace LynxEngine
{
    namespace Android
    {
        class LYNXENGCLASS CTrueTypeFont : public LynxEngine::CTrueTypeFont
        {
            LYNX_DECLARE_CLASS(CTrueTypeFont);
        public:
			int								m_Size;
        protected:            
			jobject							m_Typeface;
        public:
            CTrueTypeFont(LynxEngine::CEngine* const lpengine);
            virtual ~CTrueTypeFont();
            
            LYNXBOOL						vCreate(const FontCreationDesc* desc);            
			CReal                           vGetCharacterHeight() const;
			LYNXFORCEINLINE jobject			GetTypeface() {return m_Typeface; }

        friend class CFontLabel;
        };
    }
}

#endif