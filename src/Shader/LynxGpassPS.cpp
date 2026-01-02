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
#include <Shader/LynxGPassPS.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialSystem.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGPassPS::CGPassPS(GraphicsSystem::CGraphicsSystem *pgs)
		: CShaderClass(pgs)
		{	
			m_SelfIlluminated_Const = NULL;
			m_SpecularPower_Const = NULL;
			m_IlluminatingIntensity_Const = NULL;
			m_MaterialDiffuse_Const = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGPassPS::~CGPassPS(void)
		{
			if (m_SelfIlluminated_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_SelfIlluminated_Const);
			if (m_SpecularPower_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_SpecularPower_Const);
			if (m_IlluminatingIntensity_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_IlluminatingIntensity_Const);
			if (m_MaterialDiffuse_Const)
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_MaterialDiffuse_Const);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGPassPS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
			LYNXCHAR Buffer[128];

			lynxSprintf(Buffer, _T("%d"), 0);
			ShaderMacro.Name = CAnsiString("DIFFUSEMAP_INDEX");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);

			lynxSprintf(Buffer, _T("%d"), 1);
			ShaderMacro.Name = CAnsiString("NORMALMAP_INDEX");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);

			lynxSprintf(Buffer, _T("%d"), 2);
			ShaderMacro.Name = CAnsiString("SPECULARMAP_INDEX");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);

			lynxSprintf(Buffer, _T("%d"), 3);
			ShaderMacro.Name = CAnsiString("GLOWMAP_INDEX");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);

			lynxSprintf(Buffer, _T("%d"), 4);
			ShaderMacro.Name = CAnsiString("DETAILMAP_INDEX");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);

			return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/gpass.pso"), &m_ShaderMacros);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGPassPS::vCreateConstantAndResource() 
		{ 
			if (!m_SelfIlluminated_Const)
				m_SelfIlluminated_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gbSelfIlluminated");
			if (!m_SpecularPower_Const)
				m_SpecularPower_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gSpecularPower");
			if (!m_IlluminatingIntensity_Const)
				m_IlluminatingIntensity_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gIlluminatingIntensity");
			if (!m_MaterialDiffuse_Const)
				m_MaterialDiffuse_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gMaterialDiffuseColor");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGPassPS::vSet(CContainer* lpc, CCameraContainer* cam) 
		{ 
			LYNXMATRIX MM, WVPM, WVM;

			LYNX_ASSERT(cam);		

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

			m_SelfIlluminated_Const->vMap();
				m_SelfIlluminated_Const->vSetBool(lpc->m_bSelfIlluminated);									
				m_SpecularPower_Const->vSetFloat(lpc->GetlpCurrentMaterial()->m_SpPower);							
				m_IlluminatingIntensity_Const->vSetFloat(lpc->m_IlluminatingIntensity);									
				float Color[4];
				if (lpc)
				{					
					Color[0] = lpc->GetlpCurrentMaterial()->m_Diffuse.Red/255.0f;
					Color[1] = lpc->GetlpCurrentMaterial()->m_Diffuse.Green/255.0f;
					Color[2] = lpc->GetlpCurrentMaterial()->m_Diffuse.Blue/255.0f;
					Color[3] = lpc->GetlpCurrentMaterial()->m_Diffuse.Alpha/255.0f;					
				}
				else
				{
					Color[0] = Color[1] = Color[2] = Color[3] = 1.0f;					
				}
				m_MaterialDiffuse_Const->vSetVector4(Color);
			m_SelfIlluminated_Const->vUnmap();

			lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));				
		}		
	}
}
