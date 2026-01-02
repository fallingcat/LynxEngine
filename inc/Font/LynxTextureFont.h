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

#ifndef __LYNXTEXTUREFONT_H__
#define __LYNXTEXTUREFONT_H__

#include <LynxCore.h>
#include <LynxResource.h>
#include <LynxMathWrap.h>
#include <LynxEngineClasses.h>
#include <Font/LynxFont.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{	
	struct FONTDATA {
		CReal							Width, Height;
		LYNXUVPOINT						UV[4];
		Math::CVector2					Offset;
	};

	class LYNXENGCLASS CTextureFont : public CFont
	{	
		LYNX_DECLARE_CLASS(CFont);
	public:
        Math::CVector2					m_CharacterOffset;
	protected:	
		MaterialSystem::CMaterialPtr	m_lpMaterial;
		CReal							m_CharHeight;
		CMap<DWORD, FONTDATA>			m_CharData;
			
	public:			
		CTextureFont(CEngine* const lpengine);
		virtual ~CTextureFont();			

		LYNXFORCEINLINE MaterialSystem::CMaterialPtr& GetlpMaterial() {return m_lpMaterial; };

		LYNXFORCEINLINE CReal			vGetCharacterHeight() const {return m_CharHeight; };
		void							SetCharacterOffset(const Math::CVector2& offset);
		void							SetCharacterOffset(const DWORD i, const Math::CVector2& offset);
		LYNXBOOL						Create();
		LYNXBOOL						vCreate(const FontCreationDesc* desc) {return LYNX_TRUE; }
		void							LoadFixed(const LYNXCHAR *filename, CReal cw, CReal ch, int si, int num, int x, int y, int w, int h);
		void							LoadVar(const LYNXCHAR *filename, int row, CReal scale, int* si, int* num, int* x, int* y, int w[][128], int h, int tw = -1, int th = -1);		
		void							LoadVarByCharacterOffset(const LYNXCHAR *filename, int row, CReal scale, int* si, int* num, int* x, int* y, int off[][128], int h, int tw = -1, int th = -1);
		void							LoadFnt(const LYNXCHAR *filename, float scale);
		int								GetCharacterWidth(int c);

		friend class CTextureFontString;
	};					
}

#endif