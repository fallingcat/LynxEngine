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

#ifndef __LYNXMATERIALPASS_H__
#define __LYNXMATERIALPASS_H__

#include <LynxObj.h>
#include <LynxArray.h>
#include <LynxSystem.h>
#include <LynxParam.h>
#include <LynxEngineClasses.h>
#include <MaterialSystem/LynxTextureAnimation.h>
#include <GraphicsSystem/LynxVertexShader.h>
#include <GraphicsSystem/LynxPixelShader.h>

namespace LynxEngine 
{	
	namespace MaterialSystem 
	{		
		class LYNXENGCLASS CPass : public CObj
		{	
			LYNX_DECLARE_CLASS(CPass);
		public:
			CArray<CTextureAnimationPtr>			m_TextureAnimationArray;
			
			LYNXBOOL								m_bDisable;	
			LYNXBOOL								m_bGlow;
			LYNXBOOL								m_bUseFog;			

			//DECLAR_PARAM_SHADERCLASS(m_lpVS)
			//DECLAR_PARAM_SHADERCLASS(m_lpPS)
		protected:			
			CTechnique*								m_lpTechnique;			
			
			GraphicsSystem::CShaderGroupPtr			m_lpShaderGroup;
			GraphicsSystem::CDepthStencilStatePtr	m_DepthStencilState;
			GraphicsSystem::CBlendStatePtr			m_BlendState;
			GraphicsSystem::CRasterizerStatePtr		m_RasterizerState;			
		public:			
			CPass(CTechnique* lpt);
			CPass(const CPass& m);
			virtual ~CPass();				

			CPass&									operator =(const CPass& m);
			
			LYNXFORCEINLINE GraphicsSystem::CShaderGroupPtr& GetShaderGroup() {return m_lpShaderGroup; }
			LYNXFORCEINLINE GraphicsSystem::CBlendStatePtr& GetBlendState() {return m_BlendState; }
			LYNXFORCEINLINE GraphicsSystem::CDepthStencilStatePtr& GetDepthStencilState() {return m_DepthStencilState; }
			LYNXFORCEINLINE GraphicsSystem::CRasterizerStatePtr& GetRasterizerState() {return m_RasterizerState; }

			virtual void							vPlayAll(LYNXREAL skip);
			virtual LYNXBOOL						vPlay(int channel, LYNXREAL skip);
			void									CreateTextureAnimation(int channel, int num);
			void									LoadTexture(int channel, int frame, const char *filename);
			LYNXINLINE void							SetlpTechnique(CTechnique *lpt) {m_lpTechnique = lpt; };
			LYNXINLINE CTechnique* const			GetlpTechnique() {return m_lpTechnique; };

			virtual void							vSetParameter(void);			
			void									Set(CContainer* lpc, CCameraContainer* cam);
			LYNXINLINE virtual void					vSet(CContainer* lpc, CCameraContainer* cam, GraphicsSystem::CVertexLayoutPtr& v, GraphicsSystem::CShaderClassPtr& vs) {};
			virtual void							vSetNumTextureAnimation(int num);			
			void									AddTextureAnimation();
			LYNXINLINE int							GetNumTextureAnimation() {return (DWORD)m_TextureAnimationArray.size(); };
			LYNXINLINE CTextureAnimationPtr&		GetTextureAnimation(int channel) {return m_TextureAnimationArray[channel]; };						
			LYNXINLINE GraphicsSystem::CShaderClassPtr& GetVS() {return m_lpShaderGroup->m_lpVertexShader; };
			LYNXINLINE GraphicsSystem::CShaderClassPtr& GetPS() {return m_lpShaderGroup->m_lpPixelShader; };			
			virtual void							vCreateShaderClasses(const CAnsiString& vsname, const CAnsiString& psname);
			virtual void							vCreateCustomShaderClasses(const CString& vsname, const CString& psname);
			void									CreateDepthStencilState(const GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC& desc);
			void									CreateBlendState(const GraphicsSystem::CRenderer::BLENDSTATEDESC& desc);
			void									CreateRasterizerState(const GraphicsSystem::CRenderer::RASTERIZERSTATEDESC& desc);

			LYNXBOOL								vSave(CStream& stream);
			LYNXBOOL								vLoad(CStream& stream, LYNXBOOL skip = LYNX_FALSE);

			LYNXBOOL								vLoad(CUserProfile* file, CXMLElement* c);
			LYNXBOOL								vSave(CUserProfile* file, CXMLElement* c);

			#ifdef __TOOL__
				LYNXINLINE GraphicsSystem::CDepthStencilStatePtr	GetDepthStencilState() {return m_DepthStencilState; };
				LYNXINLINE GraphicsSystem::CBlendStatePtr			GetBlendState() {return m_BlendState; };
				LYNXINLINE GraphicsSystem::CRasterizerStatePtr		GetRasterizerState() {return m_RasterizerState; };
			#endif
			
			friend class CTechnique;
		};		
	}
}

#endif