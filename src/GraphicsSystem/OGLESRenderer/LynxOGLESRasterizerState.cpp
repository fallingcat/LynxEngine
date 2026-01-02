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
#include <GraphicsSystem/OGLESRenderer/LynxOGLESRasterizerState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESRasterizerState::COGLESRasterizerState(CGraphicsSystem* pgs)
		: CRasterizerState(pgs)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESRasterizerState::~COGLESRasterizerState(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESRasterizerState::vCreate(const CRenderer::RASTERIZERSTATEDESC& desc)
		{
			CRasterizerState::vCreate(desc);
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESRasterizerState::vSet()
		{
			if (m_Desc.CullMode == CRenderer::CULL_NONE)
				glDisable(GL_CULL_FACE);
			else 
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glFrontFace(COGLESRenderer::TranslateCullMode(m_Desc.CullMode));				
			}
		}
	}
}

#endif