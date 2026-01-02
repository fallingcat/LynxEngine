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

#ifndef __LYNXANDROIDBITMAPCONTEXT_H__
#define __LYNXANDROIDBITMAPCONTEXT_H__

#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxBitmapContext.h>
#include <jni.h>
#include <LynxEngine_PCH.h>

//LYNX_FORWARD_DECLARATION(CGContextRef)

namespace LynxEngine 
{	
	namespace Android
	{
		class LYNXENGCLASS CBitmapContext : public GraphicsSystem::CBitmapContext
		{
			LYNX_DECLARE_CLASS(CBitmapContext);
		private:
			void							Print(const LYNXCHAR* text, const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth);
		protected:
			LYNXDC							m_Context;
			android_app*					m_App;
			jobject							m_BCobject;
			jmethodID						m_GetBitmap;
			LYNXBOOL						m_bDirty;
			int								m_Alignment;
		public:						
			CBitmapContext(GraphicsSystem::CGraphicsSystem* lpsys);
			virtual ~CBitmapContext();		

			LYNXBOOL						vCreate(const int w, const int h,  const int alinment = CSpriteContainer::H_ALIGN_CENTER);

			LYNXFORCEINLINE LYNXDC			GetContext() {return m_Context; };

			void							vPrint(const LYNXCHAR* text, const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth);
			void							vPrint(const LYNXCHAR* text, const int x, const int y, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth);

			void							UpdateTextureIfNecessary();
		};
	}
}

#endif