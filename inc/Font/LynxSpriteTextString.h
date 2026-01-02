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

#ifndef __LYNXSPRITETEXTSTRING_H__
#define __LYNXSPRITETEXTSTRING_H__

#include <LynxEngineClasses.h>
#include <Container/LynxSpriteContainer.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CSpriteTextString : public CSpriteContainer
	{	
		LYNX_DECLARE_CLASS(CSpriteTextString);
	public:					
		LYNXBOOL						m_bShadow;
		LYNXBOOL						m_bOutline;
		LYNXCOLORRGBA					m_OutlineColor;
		int								m_OutlineWidth;

	protected:		
		CPlatformBitmapContext*			m_Context;
		LYNXBOOL                        m_bSpaceChecking;
        LYNXBOOL                        m_bAutoReturn;
        CVector2                        m_OriginalSize;
        CVector2                        m_PrintSize;
		float							m_Scale;				
		int								m_TextAlignment;
	public:			
		CSpriteTextString(CEngine *lpengine);
		virtual ~CSpriteTextString();			

		void							vRender(CCameraContainer* cam = NULL);
		virtual void					vRelease();
        void                            ReleaseContext();
        LYNXBOOL						Create(const int sw, const int sh, const int tw, const int th, const int alignment = CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
		void							SetFont(CFontPtr& lpfont);
        LYNXFORCEINLINE LYNXBOOL        AutoReturn(LYNXBOOL b) {LYNXBOOL Old = m_bAutoReturn; m_bAutoReturn = b; return Old; };
		LYNXFORCEINLINE LYNXBOOL        SpaceChecking(LYNXBOOL b) {LYNXBOOL Old = m_bSpaceChecking; m_bSpaceChecking = b; return Old; };
        LYNXFORCEINLINE CPlatformBitmapContext* GetBitmapContext() {return m_Context; }
        void                            SetOriginalSize(const float w, const float h);
		void							SetTextAlignment(const DWORD a);
        
		const float                     SetScale(const float s);
		LYNXFORCEINLINE const float		GetScale() {return m_Scale; };
		//LYNXFORCEINLINE const float		GetCurrentFontHeight() {return m_lpFont ? m_Scale * m_lpFont->vGetCharacterHeight() : 0.0f; };

		int								Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);	
		Math::CVector2					Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);	
		Math::CVector2					Printf(const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);
		int								PrintfLength(const LYNXCHAR *fmt, ...);
		Math::CVector2					PrintfSize(const LYNXCHAR *fmt, ...);

		LYNXFORCEINLINE CPlatformBitmapContext* GetContext() { return m_Context; }
	};					
}

#endif