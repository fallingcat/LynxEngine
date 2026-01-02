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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLESRenderer/LynxOGLESRenderer.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESDepthStencilState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESDepthStencilState::COGLESDepthStencilState(CGraphicsSystem* pgs)
		: CDepthStencilState(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESDepthStencilState::~COGLESDepthStencilState(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESDepthStencilState::vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc)
		{
			return CDepthStencilState::vCreate(desc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESDepthStencilState::vSet()
		{
			switch (m_Desc.DepthMode)
			{
				case CRenderer::DB_NONE:
					glDisable(GL_DEPTH_TEST);
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
			glDepthFunc(COGLESRenderer::TranslateTestFunction(m_Desc.DepthFunc));
			
			if (m_Desc.StencilEnable)
			{
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(COGLESRenderer::TranslateTestFunction(m_Desc.StencilFunc), m_Desc.StencilRef, m_Desc.StencilReadMask);
				glStencilOp(COGLESRenderer::TranslateStencilOP(m_Desc.StencilFailOp),
							COGLESRenderer::TranslateStencilOP(m_Desc.StencilDepthFailOp),
							COGLESRenderer::TranslateStencilOP(m_Desc.StencilPassOp));				
			}
			else 
			{
				glDisable(GL_STENCIL_TEST);
			}
		}
	}
}

#endif