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
#include <GraphicsSystem/OGLESRenderer/LynxOGLESVertexShader.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESPixelShader.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESShaderConstant.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESShaderConstant::COGLESShaderConstant(CShader *lps, const char* name)	
		: CShaderConstant(lps, name)
		{		
			//m_Location = glGetUniformLocation(program, name);
			switch (lps->GetResourceType())
			{
				case VERTEX_SHADER:
					break;
				case PIXEL_SHADER:
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESShaderConstant::~COGLESShaderConstant(void)
		{			
		}			
	}
}
#endif