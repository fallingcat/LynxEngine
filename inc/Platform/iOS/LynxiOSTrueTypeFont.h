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
//  ª°©˙:
//
//###########################################################################

#ifndef __LYNXIOSTRUETYPEFONT_H__
#define __LYNXIOSTRUETYPEFONT_H__

#include <LynxEngineClasses.h>
#include <Font/LynxTrueTypeFont.h>

using namespace LynxEngine::Math;

LYNX_FORWARD_DECLARATION(UIFont)

namespace LynxEngine
{
    namespace iOS
    {
        class LYNXENGCLASS CTrueTypeFont : public LynxEngine::CTrueTypeFont
        {
            LYNX_DECLARE_CLASS(CTrueTypeFont);
        public:
        protected:
            UIFont*                         m_Font;
            float                           m_Size;
        public:
            CTrueTypeFont(LynxEngine::CEngine* const lpengine);
            virtual ~CTrueTypeFont();
            
            LYNXBOOL						vCreate(const FontCreationDesc* desc);
            CReal                           vGetCharacterHeight() const;
            
            LYNXFORCEINLINE UIFont*         GetFont() {return m_Font; }
            LYNXFORCEINLINE float           GetSize() {return m_Size; }

        friend class CFontLabel;
        };
    }
}

#endif