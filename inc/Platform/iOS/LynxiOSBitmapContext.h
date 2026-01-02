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

#ifndef __LYNXIOSBITMAPCONTEXT_H__
#define __LYNXIOSBITMAPCONTEXT_H__

#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxBitmapContext.h>
#include <CoreGraphics/CoreGraphics.h>

LYNX_FORWARD_DECLARATION(NSShadow)
LYNX_FORWARD_DECLARATION(NSMutableParagraphStyle)
LYNX_FORWARD_DECLARATION(NSMutableDictionary)

namespace LynxEngine 
{	
	namespace iOS
	{
		class LYNXENGCLASS CBitmapContext : public GraphicsSystem::CBitmapContext
		{
			LYNX_DECLARE_CLASS(CBitmapContext);
		protected:
			CGContextRef					m_Context;
            NSShadow*                       m_Shadow;
            NSMutableParagraphStyle*        m_ParagraphStyle;
            NSMutableDictionary*            m_Attributes;
		public:						
			CBitmapContext(GraphicsSystem::CGraphicsSystem* lpsys);
			virtual ~CBitmapContext();		

            LYNXBOOL						vCreate(const int w, const int h, const int alinment = CSpriteContainer::H_ALIGN_CENTER);

			LYNXFORCEINLINE CGContextRef	GetContext() {return m_Context; };

			void							vSetTextAlignment(const DWORD alignment);
            void                            vReleaseContextData(void);
			void							vPrint(const LYNXCHAR* text, const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth);
			void							vPrint(const LYNXCHAR* text, const int x, const int y, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth);			
		};
	}
}

#endif