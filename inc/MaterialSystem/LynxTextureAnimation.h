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

#ifndef __LYNXTEXTUREANIMATION_H__
#define __LYNXTEXTUREANIMATION_H__

#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxString.h>
#include <LynxParam.h>
#include <Animation/LynxAnimation.h>
#include <GraphicsSystem/LynxRenderer.h>

namespace LynxEngine 
{	
	namespace MaterialSystem 
	{			
		class LYNXENGCLASS CTextureAnimation : public CResource
		{	
			LYNX_DECLARE_CLASS(CTextureAnimation);
		public:			
			typedef struct {
				Math::CMatrix3					UVMatrix;
				float							Color[4];
			}TEXTUREANIMATIONPARAM;

			int									m_CurrentTexture;			
		protected:					
			GraphicsSystem::CGraphicsSystem*	m_lpGraphicsSystem;
			DECLAR_PARAM_ANSISTRING(m_ShaderConstantName)
			
			DECLAR_PARAM_ARRAY(LynxEngine::GraphicsSystem::CTexturePtr, m_TextureArray)				
			Animation::CAnimation*				m_lpAnimation;					

			LYNXBOOL							m_bUpdateAnimationParam;
			TEXTUREANIMATIONPARAM				m_TexAnimationParam;	
		public:			
			CTextureAnimation(GraphicsSystem::CGraphicsSystem* pgs);
			virtual ~CTextureAnimation();		

			CTextureAnimation&					operator =(const CTextureAnimation& t);

			void								SetlpGraphicsSystem(GraphicsSystem::CGraphicsSystem* pgs) {m_lpGraphicsSystem = pgs; };
			LYNXINLINE Animation::CAnimation*	GetlpAnimation() {return m_lpAnimation; };	
			void								Create(int num);
			void								AddTexture(CStream& fs, const LYNXCHAR *filename, GraphicsSystem::CRenderer::TEXTUREDESC* desc = NULL);
			virtual Animation::CAnimation*		vCreateAnimation();
			LYNXINLINE void						SetShaderConstantName(const char* s) {m_ShaderConstantName = s; };
			LYNXINLINE const CAnsiString&		GetShaderConstantName() {return m_ShaderConstantName; };
			LYNXINLINE int						GetNumTextures() {return (DWORD)m_TextureArray.size(); };
			LYNXBOOL							Play(LYNXREAL step);
			LYNXINLINE GraphicsSystem::CTexturePtr& GetTexture() {return m_TextureArray[m_CurrentTexture]; };
			LYNXINLINE GraphicsSystem::CTexturePtr& GetTexture(int i) {return m_TextureArray[i]; };
			LYNXINLINE void						SetTexture(int i, GraphicsSystem::CTexturePtr& t) {m_TextureArray[i] = t; };			
			LYNXBOOL							LoadTexture(int i, CStream& fs, const LYNXCHAR *filename, GraphicsSystem::CRenderer::TEXTUREDESC* desc = NULL);
			LYNXBOOL							LoadTextureSeuence(int num, int length, CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ext, GraphicsSystem::CRenderer::TEXTUREDESC* desc = NULL);			
			LYNXBOOL							LoadAnimation(const CCreationDesc* desc);			
			virtual void						vSet(int channel);
			virtual void						vSet(GraphicsSystem::CShader* lps, int channel);
		};		
	}		
}

#endif