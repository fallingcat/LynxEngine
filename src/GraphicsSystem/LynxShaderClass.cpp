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

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShaderClass::CShaderClass(CGraphicsSystem *pgs)
		: CObj(pgs->GetlpEngine()) 
		{	
			m_lpGraphicsSystem = pgs; 
			m_ShaderMacros.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShaderClass::~CShaderClass(void)
		{
			m_ShaderMacros.clear();
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CShaderClass::Recompile()
		{			
			if (m_lpShader)
			{
				CString Name = m_lpShader->GetName();
				switch  (m_lpShader->GetResourceType())
				{
					case VERTEX_SHADER:
					{
						m_lpShader.Reset();
						//m_lpGraphicsSystem->GetlpEngine()->DestroyResource(Name);
						LYNXBOOL bOld = m_lpGraphicsSystem->m_bNeedToRecompileShader;
						m_lpGraphicsSystem->m_bNeedToRecompileShader = LYNX_TRUE;
						if (!m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, Name, &(m_ShaderMacros)))
							return LYNX_FALSE;
						m_lpGraphicsSystem->m_bNeedToRecompileShader = bOld;
						break;
					}
					case PIXEL_SHADER:
					{
						m_lpShader.Reset();
						//m_lpGraphicsSystem->GetlpEngine()->DestroyResource(Name);
						LYNXBOOL bOld = m_lpGraphicsSystem->m_bNeedToRecompileShader;
						m_lpGraphicsSystem->m_bNeedToRecompileShader = LYNX_TRUE;
						if (!m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, Name, &(m_ShaderMacros)))
							return LYNX_FALSE;
						m_lpGraphicsSystem->m_bNeedToRecompileShader = bOld;
						break;
					}
				}
			}
			return LYNX_TRUE;
		}		
	}
}