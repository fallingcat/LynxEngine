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

#include <GraphicsSystem/OGLESRenderer/LynxOGLESPixelShader.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESPixelShader::COGLESPixelShader(CGraphicsSystem *pgs)
		:CPixelShader(pgs)
		{	
			m_GLHandle = 0;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESPixelShader::~COGLESPixelShader(void)
		{						
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESPixelShader::vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def, CStream* stream)
		{ 
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESPixelShader::vSetTexture(const char *name, CTexture* t)  
		{
			if (CAnsiString("DiffuseMap") == name)
			{
				m_lpGraphicsSystem->GetlpRenderer()->SetTexture(0, t);
				// Temp code, reset samplerstate every time no matter what is in cached samplerstate
				t->GetSamplerState()->vSet(0);
			}
			/*
			 else if (CAnsiString("NormalMap") == name)
			 {
			 lpt->vSet(1); 
			 m_lpGraphicsSystem->GetlpRenderer()->SetSamplerState(0, lpt->GetSamplerState());	
			 }
			 */
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESPixelShader::vSetSampler(const char *name, CSamplerState* lps)
		{ 
			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int COGLESPixelShader::vGetTextureIndex(const char *name)
		{
			return 0;			
		}
	}	
}

#endif