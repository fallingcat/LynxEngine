//##########################################################################
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
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3BlendState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3BlendState::COGLES3BlendState(CGraphicsSystem* pgs)
		: CBlendState(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3BlendState::~COGLES3BlendState(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3BlendState::vCreate(const CRenderer::BLENDSTATEDESC& desc)
		{
			CBlendState::vCreate(desc);
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3BlendState::vSet()
		{
			if (m_RTBlendParam[0].bBlend)
			{
				GL_TRACE(glEnable(GL_BLEND);)
				GL_TRACE(glBlendFunc(COGLES3Renderer::TranslateBlendFunction(m_RTBlendParam[0].SrcBlend),
							COGLES3Renderer::TranslateBlendFunction(m_RTBlendParam[0].DestBlend));)
				GL_TRACE(glBlendEquation(GL_FUNC_ADD);)
			}
			else
			{
				GL_TRACE(glDisable(GL_BLEND);)
			}
		}
	}
}

#endif