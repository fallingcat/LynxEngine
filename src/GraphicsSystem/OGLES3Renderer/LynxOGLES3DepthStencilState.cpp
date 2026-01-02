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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Renderer.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3DepthStencilState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3DepthStencilState::COGLES3DepthStencilState(CGraphicsSystem* pgs)
		: CDepthStencilState(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3DepthStencilState::~COGLES3DepthStencilState(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3DepthStencilState::vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc)
		{
			return CDepthStencilState::vCreate(desc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3DepthStencilState::vSet()
		{
			switch (m_DepthMode)
			{
				case CRenderer::DB_NONE:
					glDisable(GL_DEPTH_TEST);
					glDepthMask(GL_FALSE);
					break;
				case CRenderer::DB_ALWAYS:
					glEnable(GL_DEPTH_TEST);
					glDepthMask(GL_FALSE);
					break;
				case CRenderer::DB_C:
					glEnable(GL_DEPTH_TEST);
					glDepthMask(GL_FALSE);
					break;
				case CRenderer::DB_CW:
					glEnable(GL_DEPTH_TEST);
					glDepthMask(GL_TRUE);
					break;
				case CRenderer::DB_W:
					glEnable(GL_DEPTH_TEST);
					glDepthMask(GL_TRUE);
					break;
			}
			glDepthFunc(COGLES3Renderer::TranslateTestFunction(m_DepthFunc));
			
			if (m_StencilEnable)
			{
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(COGLES3Renderer::TranslateTestFunction(m_StencilFunc), m_StencilRef, m_StencilReadMask);
				glStencilOp(COGLES3Renderer::TranslateStencilOP(m_StencilFailOp),
							COGLES3Renderer::TranslateStencilOP(m_StencilDepthFailOp),
							COGLES3Renderer::TranslateStencilOP(m_StencilPassOp));				
			}
			else 
			{
				glDisable(GL_STENCIL_TEST);
			}
		}
	}
}

#endif