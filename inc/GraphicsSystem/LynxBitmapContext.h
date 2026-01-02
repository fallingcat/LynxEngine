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

#ifndef __LYNXBITMAPCONTEXT_H__
#define __LYNXBITMAPCONTEXT_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CBitmapContext : public CObj
		{
			LYNX_DECLARE_CLASS(CBitmapContext);
		protected:
            CVector2                        m_Size;
			LYNXBYTE*						m_Data;
            CTrueTypeFont*                  m_Font;
			CTexturePtr						m_Texture;
            LYNXUINT                        m_TextAlignment;
		public:						
			CBitmapContext(CGraphicsSystem* lpsys);
			virtual ~CBitmapContext();										

			virtual LYNXBOOL				vCreate(const int w, const int h,  const int alinment = CSpriteContainer::H_ALIGN_CENTER) = 0;
			virtual void					vPrint(const LYNXCHAR* text, const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth) = 0;
			virtual void					vPrint(const LYNXCHAR* text, const int x, const int y, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth) = 0;
            virtual void                    vReleaseContextData(void) {};            
			LYNXFORCEINLINE virtual  void	vSetTextAlignment(const DWORD alignment) {m_TextAlignment = alignment; };

			LYNXFORCEINLINE const LYNXBYTE*	GetData() {return m_Data; }
            LYNXFORCEINLINE const CVector2& GetSize() {return m_Size; }
            LYNXFORCEINLINE void            SetFont(CTrueTypeFont* f) {m_Font = f; }
			LYNXFORCEINLINE CTexturePtr&	GetTexture() {return m_Texture; }	
		};
	}		
}

#endif