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
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxShaderResource.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShaderResource::CShaderResource(CShader *lps, const char* name)		
		{	
			m_lpShader = lps;
			m_Name = name; 
			m_lpResource = NULL;
			m_BindPoint = lps->vGetTextureIndex(name);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShaderResource::CShaderResource(CShader* lps, const char* name, CResource* r)
		{	
			m_lpShader = lps;
			m_Name = name; 
			m_lpResource = r;
			m_BindPoint = lps->vGetTextureIndex(name);			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShaderResource::~CShaderResource(void)
		{			
			m_lpResource = NULL;
			m_BindPoint = -1;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CShaderResource::Set(const CResource* r)
		{
			//LYNX_ASSERT(m_BindPoint >= 0);

			if (m_BindPoint >= 0)
			{
				if (r)
				{				
					switch (r->GetResourceType())
					{
						default:
						case TEXTURE:
							m_lpShader->vSetTexture(m_BindPoint, (CTexture*)r);
							break;

						case BUFFER:
							m_lpShader->vSetTexture(m_BindPoint, (CTexture*)r);
							break;
					}
				}
				else
				{
					m_lpShader->vSetTexture(m_BindPoint, (CTexture*)r);
				}
			}
		}
	}
}