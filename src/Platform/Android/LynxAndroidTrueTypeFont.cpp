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

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Platform/Android/LynxAndroidTrueTypeFont.h>
#include <Platform/Android/LynxAndroidUtility.h>

namespace LynxEngine
{
    namespace Android
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
			JNI_ATTACH_CURRENT_THREAD

			env->DeleteGlobalRef(m_Typeface);

			JNI_DETACH_CURRENT_THREAD
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
			SetName(desc->FontName);
			m_Size = desc->Size;

			JNI_ATTACH_CURRENT_THREAD

			android_app* app = static_cast<GameFramework::Android::CKernel*>(glpKernel)->GetApp();
			jclass cls = env->GetObjectClass(app->activity->clazz);
			jmethodID getTypeface = env->GetMethodID(cls, "getTypeface", "(Ljava/lang/String;)Landroid/graphics/Typeface;");
			jstring name = NewModifiedUTF8String(env, GetName());
			m_Typeface = env->NewGlobalRef(env->CallObjectMethod(app->activity->clazz, getTypeface, name));

			env->DeleteLocalRef(cls);
			env->DeleteLocalRef(name);
			
			JNI_DETACH_CURRENT_THREAD

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
            return 0;
        }
    }
}