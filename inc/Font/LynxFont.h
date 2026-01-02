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

#ifndef __LYNXFONT_H__
#define __LYNXFONT_H__

#include <LynxCore.h>
#include <LynxResource.h>
#include <LynxMathWrap.h>
#include <LynxEngineClasses.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{	
	class LYNXENGCLASS CFont : public CResource
	{	
		LYNX_DECLARE_CLASS(CFont);
	public:			
		typedef enum {
			FT_TEXTURE_FONT,
			FT_TRUETYPE_FONT,
		}FONTTYPE;

		typedef struct {
            CString                     FontName;
            float                       Size;
        }FontCreationDesc;
	protected:	
		FONTTYPE						m_FontType;
	public:
		CFont(CEngine* const lpengine);
		virtual ~CFont();
        
		LYNXFORCEINLINE FONTTYPE		GetFontType() {return m_FontType; }
        virtual CReal					vGetCharacterHeight() const = 0;
		virtual LYNXBOOL				vCreate(const FontCreationDesc* desc) = 0;
	};					
}

#endif