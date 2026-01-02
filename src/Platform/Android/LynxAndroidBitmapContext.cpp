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

#include <LynxEngine_PCH.h>
#include <Platform/Android/LynxAndroidBitmapContext.h>
#include <Platform/Android/LynxAndroidUtility.h>

namespace LynxEngine 
{
	namespace Android
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBitmapContext::CBitmapContext(GraphicsSystem::CGraphicsSystem *pgs)
		: GraphicsSystem::CBitmapContext(pgs)
		{	
			m_Context = 0x000;
			m_Data = NULL;
			m_bDirty = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBitmapContext::~CBitmapContext(void)
		{	
			JNI_ATTACH_CURRENT_THREAD
			env->DeleteGlobalRef(m_BCobject);
			JNI_DETACH_CURRENT_THREAD

			if (m_Data)
            {
                LYNXFREE(m_Data);
                m_Data = NULL;
            }
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBitmapContext::vCreate(const int w, const int h,  const int alignment)
		{
			m_Alignment = alignment;
			const int BitsPerElement = 8;
			int SizeInBytes = w * h * 4;
			int TexturePitch = w * 4;
			m_Data = (LYNXBYTE*)LYNXALLOC(SizeInBytes);
			lynxMemSet(m_Data, 0xff, SizeInBytes);

			LYNXGUID Guid;
            lynxOSMakeGUID(&Guid);
			CString Name = CString(lynxOSGUIDToString(&Guid))+CString(_T(".BitmapContext"));
			m_lpEngine->CreateResource(m_Texture, TEXTURE);
			if (!m_Texture->vCreate(Name.c_str(), w, h, 1, GraphicsSystem::CRenderer::TT_2D_TEXTURE, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8))
				return LYNX_FALSE;

            m_Size.x = w;
            m_Size.y = h;
			
			JNI_ATTACH_CURRENT_THREAD
			
			jclass cls = FindClass(env, "LynxEngine/Android/BitmapContext");
			jmethodID constr = env->GetMethodID(cls, "<init>", "(II)V");
			m_BCobject = env->NewGlobalRef(env->NewObject(cls, constr, w, h));
			m_GetBitmap = env->GetMethodID(cls, "getBitmap", "(Ljava/lang/String;Landroid/graphics/Typeface;IIIIIIIZZII)[B");
			
			//env->DeleteLocalRef(cls);

			JNI_DETACH_CURRENT_THREAD

            return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBitmapContext::vPrint(const LYNXCHAR* text, const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth)
		{	
			Print(text, rect, lpc, bshadow, boutline, lpstrokeColor, strokeWidth);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBitmapContext::vPrint(const LYNXCHAR* text, const int x, const int y, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth)
		{		
			LYNXRECT Rect = {x, y, (int)m_Size.x + x, (int)m_Size.y + y};
			Print (text, Rect, lpc, bshadow, boutline, lpstrokeColor, strokeWidth);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBitmapContext::Print(const LYNXCHAR* text, const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth)
		{
			Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);

			int offsetX = 1;
			int offsetY = -int(0 * PositionFactor.y);
			if ((m_Alignment & 0x10) == 0)
				offsetY = 0;

			int SizeInBytes = m_Size.x * m_Size.y * 4;
			lynxMemSet(m_Data, 0x00, SizeInBytes);

			JNI_ATTACH_CURRENT_THREAD
			
			jstring str = NewModifiedUTF8String(env, text);
			
			jint color = lpc==NULL? 0xffffffff : lpc->PackedColor;
			jint strokeColor = lpstrokeColor == NULL ? 0xff0000ff : lpstrokeColor->PackedColor;

			jbyteArray buffer = (jbyteArray)env->CallObjectMethod(m_BCobject, m_GetBitmap, str, 
				static_cast<LynxEngine::Android::CTrueTypeFont*>(m_Font)->GetTypeface(), static_cast<LynxEngine::Android::CTrueTypeFont*>(m_Font)->m_Size, color,
				m_Alignment, rect.x1 + offsetX, rect.y1 + offsetY, rect.x2, rect.y2 + offsetY, bshadow, boutline, strokeColor, strokeWidth);
			lynxMemCpy(m_Data, env->GetByteArrayElements(buffer, NULL), m_Size.x*m_Size.y * 4);

			m_bDirty = LYNX_TRUE;

			//env->DeleteLocalRef(byteArray);
			env->DeleteLocalRef(str);
			env->DeleteLocalRef(buffer);
			
			JNI_DETACH_CURRENT_THREAD
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBitmapContext::UpdateTextureIfNecessary()
		{
			if (m_bDirty) {
				m_Texture->vUpdate(0, m_Data);
				m_bDirty = LYNX_FALSE;
			}
		}
	}						
}
