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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2RasterizerState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2RasterizerState::COGLES2RasterizerState(CGraphicsSystem* pgs)
		: CRasterizerState(pgs)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2RasterizerState::~COGLES2RasterizerState(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2RasterizerState::vCreate(const CRenderer::RASTERIZERSTATEDESC& desc)
		{
			LOCK_MUTEX

			CRasterizerState::vCreate(desc);

			UNLOCK_MUTEX

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2RasterizerState::vSet()
		{
			LOCK_MUTEX

			if (m_Desc.CullMode == CRenderer::CULL_NONE)
				glDisable(GL_CULL_FACE);
			else 
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glFrontFace(COGLES2Renderer::TranslateCullMode(m_Desc.CullMode));				
			}

			UNLOCK_MUTEX
		}
	}
}

#endif