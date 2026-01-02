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

#ifndef __LYNXDIFFUSEMAPXCOLORBLENDPS_H__
#define __LYNXDIFFUSEMAPXCOLORBLENDPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		template <LYNXBOOL bLinearDodge>
		class LYNXENGCLASS TDiffuseMapXColorBlendPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(TDiffuseMapXColorBlendPS);
		public:									
			float								m_ConstantColor[4];
			LYNXBOOL							m_bUseConstantColor;

		protected:
			GraphicsSystem::CShaderConstant*	m_ConstantColor_Const;
			
		public:			
			TDiffuseMapXColorBlendPS(GraphicsSystem::CGraphicsSystem* lpg)
				: CShaderClass(lpg)
			{
				m_ConstantColor[0] = 1.0f;
				m_ConstantColor[1] = 1.0f;
				m_ConstantColor[2] = 1.0f;
				m_ConstantColor[3] = 1.0f;
				m_ConstantColor_Const = NULL;
				m_bUseConstantColor = LYNX_FALSE;
			}

			virtual ~TDiffuseMapXColorBlendPS()
			{
				if (m_ConstantColor_Const)
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_ConstantColor_Const);
			}

			virtual LYNXBOOL vCreate()
			{
				GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;

				m_ShaderMacros.clear();

				ShaderMacro.Name = CAnsiString("BLEND_COLORDODGE");
				ShaderMacro.Definition = bLinearDodge ? CAnsiString("0") : CAnsiString("1");
				m_ShaderMacros.push_back(ShaderMacro);

				ShaderMacro.Name = CAnsiString("BLEND_LINEARDODGE");
				ShaderMacro.Definition = bLinearDodge ? CAnsiString("1") : CAnsiString("0");
				m_ShaderMacros.push_back(ShaderMacro);

				return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("common/lynxDiffuseMapXColorBlend.pso"), &m_ShaderMacros);				
			}

			virtual void vCreateConstantAndResource()
			{				
				if (!m_ConstantColor_Const)
					m_ConstantColor_Const = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gBlendColor");
					
				m_lpShader->CreateResource(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
			}

			virtual void vSet(CContainer* lpc, CCameraContainer* cam)
			{
				GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();

				m_ConstantColor_Const->vMap();
				if (lpc && !m_bUseConstantColor)
				{
					float Color[4];
					Color[0] = (lpc->GetlpCurrentMaterial()->m_Diffuse.Red/255.0f) * (lpc->m_Diffuse.Red/255.0f);
					Color[1] = (lpc->GetlpCurrentMaterial()->m_Diffuse.Green/255.0f) * (lpc->m_Diffuse.Green/255.0f);
					Color[2] = (lpc->GetlpCurrentMaterial()->m_Diffuse.Blue/255.0f) * (lpc->m_Diffuse.Blue/255.0f);
					if (lpc->m_bUseVisibilty)
						Color[3] = lpc->m_Visibility;
					else
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
		};		
	}		
}

#define DECLARE_DIFFUSEMAPXCOLORBLENDPS(bLinearDodge) \
	GraphicsSystem::CShaderClass* Create##DiffuseMapXColorBlendPS_##bLinearDodge(CEngine* e);

#define IMPLEMENT_DIFFUSEMAPXCOLORBLENDPS(bLinearDodge) \
	namespace Shader{\
		typedef TDiffuseMapXColorBlendPS<bLinearDodge>	CDiffuseMapXColorBlendPS_##bLinearDodge;\
	}\
	LYNXFORCEINLINE IMPLEMENT_SHADERCLASS(DiffuseMapXColorBlendPS_##bLinearDodge)

#define REGISTER_DIFFUSEMAPXCOLORBLENDPS(classname, bLinearDodge) \
	m_ShaderClassManager.Register(classname, Create##DiffuseMapXColorBlendPS_##bLinearDodge);

namespace LynxEngine
{
	IMPLEMENT_DIFFUSEMAPXCOLORBLENDPS(TRUE)	 // LinearDodge : 1 (Add)
	IMPLEMENT_DIFFUSEMAPXCOLORBLENDPS(FALSE) // LinearDodge : 0 (ColorDodge)
}
#endif