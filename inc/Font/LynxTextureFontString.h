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

#ifndef __LYNXTEXTUREFONTSTRING_H__
#define __LYNXTEXTUREFONTSTRING_H__

#include <LynxEngineClasses.h>
#include <Container/LynxSpriteContainer.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CTextureFontString : public CSpriteContainer
	{	
		LYNX_DECLARE_CLASS(CTextureFontString);
	public:			
		static int						m_BatchSize;
	protected:				
		CFontPtr						m_lpFont;
		LYNXBOOL                        m_bSpaceChecking;
        LYNXBOOL                        m_bAutoReturn;
        
		CVector2						m_Scale;	
		GraphicsSystem::CIndexArrayPtr	m_IndexArray;
		DWORD							m_MaxLen;	
		LYNXVECTOR4D*					m_PosArray;
		LYNXVECTOR4D*					m_TexCoordArray;		
	public:			
		CTextureFontString(CEngine *lpengine);
		virtual ~CTextureFontString();			

		virtual void					vRelease();
		LYNXBOOL						Create(DWORD maxlen, LYNXBOOL bbo = LYNX_TRUE);		
		void							SetFont(CFontPtr& lpfont);
        LYNXFORCEINLINE LYNXBOOL        AutoReturn(LYNXBOOL b) {LYNXBOOL Old = m_bAutoReturn; m_bAutoReturn = b; return Old; };
		LYNXFORCEINLINE LYNXBOOL        SpaceChecking(LYNXBOOL b) {LYNXBOOL Old = m_bSpaceChecking; m_bSpaceChecking = b; return Old; };
		
		LYNXFORCEINLINE const CVector2	SetScale(const float s) {CVector2 Old = m_Scale; m_Scale.x = m_Scale.y = s; return Old; };
		LYNXFORCEINLINE const CVector2	SetScale(const float x, const float y) {CVector2 Old = m_Scale; m_Scale.x = x; m_Scale.y = y; return Old; };
		LYNXFORCEINLINE const CVector2	SetScale(const CVector2& s) {CVector2 Old = m_Scale; m_Scale = s; return Old; };
		LYNXFORCEINLINE const CVector2&	GetScale() {return m_Scale; };
		LYNXFORCEINLINE const float		GetCurrentFontHeight() {return m_lpFont ? m_Scale.y * m_lpFont->vGetCharacterHeight() : 0.0f; };

		int								Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);	
		Math::CVector2					Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...);	
		int								PrintfLength(const LYNXCHAR *fmt, ...);
		Math::CVector2					PrintfSize(const LYNXCHAR *fmt, ...);
	};					
}

#endif