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
#include <LynxEngine.h>
#include <LynxRenderableObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <Container/LynxCameraContainer.h>
#include <Shader/LynxConstantColorPS.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CConstantColorPS::CConstantColorPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_ConstantColor[0] = 1.0f;
			m_ConstantColor[1] = 1.0f;
			m_ConstantColor[2] = 1.0f;
			m_ConstantColor[3] = 1.0f;
			m_ConstantColor_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CConstantColorPS::~CConstantColorPS(void)
		{
			if (m_ConstantColor_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ConstantColor_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CConstantColorPS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("common/lynxConstantColor.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CConstantColorPS::vCreateConstantAndResource() 
		{ 
			if (!m_ConstantColor_Const)
				m_ConstantColor_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gOutputColor");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CConstantColorPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 			
			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();	

			m_ConstantColor_Const->vMap();
				if (lpc)
				{
					float Color[4];
					Color[0] = (lpc->GetlpCurrentMaterial()->m_Diffuse.Red/255.0f) * (lpc->m_Diffuse.Red/255.0f);
					Color[1] = (lpc->GetlpCurrentMaterial()->m_Diffuse.Green/255.0f) * (lpc->m_Diffuse.Green/255.0f);
					Color[2] = (lpc->GetlpCurrentMaterial()->m_Diffuse.Blue/255.0f) * (lpc->m_Diffuse.Blue/255.0f);
					Color[3] = (lpc->GetlpCurrentMaterial()->m_Diffuse.Alpha/255.0f) * (lpc->m_Diffuse.Alpha/255.0f);
					m_ConstantColor_Const->vSetVector4(Color);
				}
				else
				{
					m_ConstantColor_Const->vSetVector4(m_ConstantColor);				
				}
			m_ConstantColor_Const->vUnmap();

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));
		}		
	}
}
