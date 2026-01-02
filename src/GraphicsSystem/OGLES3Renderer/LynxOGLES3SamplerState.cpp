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
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3SamplerState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3SamplerState::COGLES3SamplerState(CGraphicsSystem* pgs)
		: CSamplerState(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3SamplerState::~COGLES3SamplerState(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3SamplerState::vCreate(const CRenderer::SAMPLERSTATEDESC& desc)
		{
			CSamplerState::vCreate(desc);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3SamplerState::vSet(int channel)
		{
            //glActiveTexture(GL_TEXTURE0+channel);       				
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, COGLES3Renderer::TranslateAddressMode(m_AddressU));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, COGLES3Renderer::TranslateAddressMode(m_AddressV));		
		
			if (m_MinLOD == m_MaxLOD)
			{
				switch (m_Filter)
				{
					default:
					case CRenderer::FM_MIN_MAG_MIP_POINT:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
					case CRenderer::FM_MIN_MAG_POINT_MIP_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
					case CRenderer::FM_MIN_POINT_MAG_LINEAR_MIP_POINT:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;
					case CRenderer::FM_MIN_POINT_MAG_MIP_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;
					case CRenderer::FM_MIN_LINEAR_MAG_MIP_POINT:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
					case CRenderer::FM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
					case CRenderer::FM_MIN_MAG_LINEAR_MIP_POINT:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;
					case CRenderer::FM_MIN_MAG_MIP_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;			
				}
			}
			else
			{
				switch (m_Filter)
				{
					default:
					case CRenderer::FM_MIN_MAG_MIP_POINT:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
					case CRenderer::FM_MIN_MAG_POINT_MIP_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
					case CRenderer::FM_MIN_POINT_MAG_LINEAR_MIP_POINT:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;
					case CRenderer::FM_MIN_POINT_MAG_MIP_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;
					case CRenderer::FM_MIN_LINEAR_MAG_MIP_POINT:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
					case CRenderer::FM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
					case CRenderer::FM_MIN_MAG_LINEAR_MIP_POINT:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;
					case CRenderer::FM_MIN_MAG_MIP_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;			
				}
			}
		}		
	}
}

#endif