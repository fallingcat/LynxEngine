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

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#import <UIKit/UIKit.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Platform/iOS/LynxiOSTrueTypeFont.h>

namespace LynxEngine
{
    namespace iOS
    {
        //-------------------------------------------------------------------------------------------------------
        /**
         *	@brief °C
         *
         *	@param °C
         *	@param °C
         *	@return °C
         */
        //-------------------------------------------------------------------------------------------------------
        CTrueTypeFont::CTrueTypeFont(LynxEngine::CEngine* const lpengine)
        : LynxEngine::CTrueTypeFont(lpengine)
        {
            m_Font = nil;
        }
        //-------------------------------------------------------------------------------------------------------
        /**
         *	@brief °C
         *
         *	@param °C
         *	@param °C
         *	@return °C
         */
        //-------------------------------------------------------------------------------------------------------
        CTrueTypeFont::~CTrueTypeFont(void)
        {
            if (m_Font)
            {
                //[m_Font release];
            }
        }
        //-------------------------------------------------------------------------------------------------------
        /**
         *	@brief °C
         *
         *	@param °C
         *	@param °C
         *	@return °C
         */
        //-------------------------------------------------------------------------------------------------------
        LYNXBOOL CTrueTypeFont::vCreate(const FontCreationDesc* desc)
        {
            if (!desc)
                return LYNX_FALSE;
            
            NSString* FontName = nil;
            if (!desc->FontName.empty())
            {
                lynxcString2NSString(&FontName, desc->FontName.c_str());
                m_Font = [UIFont fontWithName:FontName size:desc->Size];
            }
            else
            {
                m_Font = [UIFont boldSystemFontOfSize:desc->Size];
            }
            SetName(desc->FontName);
            m_Size = desc->Size;
            
            return LYNX_TRUE;
        }
        //-------------------------------------------------------------------------------------------------------
        /**
         *	@brief °C
         *
         *	@param °C
         *	@param °C
         *	@return °C
         */
        //-------------------------------------------------------------------------------------------------------
        CReal CTrueTypeFont::vGetCharacterHeight() const
        {
            return m_Font.lineHeight;
        }
    }
}