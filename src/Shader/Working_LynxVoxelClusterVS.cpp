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
#include <GraphicsSystem/LynxShaderClass.h>
#include <Container/LynxCameraContainer.h>
#include <Container/LynxVoxelClusterContainer.h>
#include <Shader/LynxVoxelClusterVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		template <LYNXBOOL bFakeAO, LYNXBOOL bFakeAOLevel2, LYNXBOOL bSpecular>
		class CVoxelClusterOneLightingVS : public CVoxelClusterVS
		{	
			LYNX_DECLARE_CLASS(CVoxelClusterOneLightingVS);
		public:						
		protected:								
			GraphicsSystem::CShaderConstant*	m_CameraPos;
			GraphicsSystem::CShaderConstant*	m_SpecularColor;            
		public:			
			CVoxelClusterOneLightingVS(GraphicsSystem::CGraphicsSystem* lpg)
			: CVoxelClusterVS(lpg)
			{
				m_bAOLevel2 = bFakeAOLevel2;

				m_CameraPos = NULL;
				m_SpecularColor = NULL;
			}

			virtual ~CVoxelClusterOneLightingVS()
			{				
				if (m_CameraPos)
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_CameraPos);
				if (m_SpecularColor)
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_SpecularColor);            
			}
            
			virtual LYNXBOOL vCreate()
			{ 
				GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
				LYNXCHAR Buffer[128];
	            
				m_ShaderMacros.clear();
	            
				lynxSprintf(Buffer, _T("%d"), CVoxelClusterContainer::m_BatchSize);
				ShaderMacro.Name = CAnsiString("VOXELCLUSTER_BATCH_SIZE");
				ShaderMacro.Definition = CAnsiString(Buffer);
				m_ShaderMacros.push_back(ShaderMacro);            
				
				ShaderMacro.Name = CAnsiString("FAKE_VOXEL_AO");
				#ifdef __FAKE_VOXEL_AO__
					ShaderMacro.Definition = CAnsiString("1");
				#else
					ShaderMacro.Definition = CAnsiString("0");
				#endif
				m_ShaderMacros.push_back(ShaderMacro);
	            
				ShaderMacro.Name = CAnsiString("USE_FAKE_VOXEL_AO");
				ShaderMacro.Definition = bFakeAO ? CAnsiString("1") : CAnsiString("0");
				m_ShaderMacros.push_back(ShaderMacro);

				ShaderMacro.Name = CAnsiString("FAKE_AO_LEVEL2");
				ShaderMacro.Definition = bFakeAOLevel2 ? CAnsiString("1") : CAnsiString("0");
				m_ShaderMacros.push_back(ShaderMacro);
				
				ShaderMacro.Name = CAnsiString("USE_SPECULAR");
				ShaderMacro.Definition = bSpecular ? CAnsiString("1") : CAnsiString("0");
				m_ShaderMacros.push_back(ShaderMacro);
				
				return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/voxelcluster_onelighting.vso"), &m_ShaderMacros);
			}

            virtual void vCreateConstantAndResource()
			{
				if (!m_WorldViewProjM)
					m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");
				if (!m_WorldM)
					m_WorldM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorld");
				if (!m_ViewProjM)
					m_ViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gViewProj");
	            			
				if (!m_PositionArray)
					m_PositionArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gPositionArray");
				if (!m_ColorArray)
					m_ColorArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gColorArray");			

				if (bSpecular)
				{
					if (!m_CameraPos)
						m_CameraPos = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gCameraPos");			
					if (!m_SpecularColor)
						m_SpecularColor = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gSpecularColor");            
				}

				if (bFakeAO)
				{
					if (!m_AOCodeArray)
						m_AOCodeArray = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gAOCodeArray");          
				}
			}

			virtual void vSet(CContainer* lpc, CCameraContainer* cam)
			{
				Math::CMatrix4 MM, WVPM, WVM, WSM, VPM;
			
				GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();						
				lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
	            
				lynxMatrixXMatrix(&WVM, &lpc->GetTransformMatrix(), &cam->GetViewMatrix());	
				lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());
				lynxMatrixXMatrix(&VPM, &cam->GetViewMatrix(), &cam->GetProjectionMatrix());				    
				
	            
				m_WorldViewProjM->vMap();
					m_WorldViewProjM->vSetMatrix(&WVPM);				
					m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
					m_ViewProjM->vSetMatrix(&VPM);

					if (bSpecular)
					{
						Math::CVector3 Pos;
						cam->vGetPosition(Pos);
						m_CameraPos->vSetVector3(&(Pos.x));			
					
						float Color[4];
						Color[0] = lpc->GetlpCurrentMaterial()->m_Specular.Red / 255.0f;
						Color[1] = lpc->GetlpCurrentMaterial()->m_Specular.Green / 255.0f;
						Color[2] = lpc->GetlpCurrentMaterial()->m_Specular.Blue / 255.0f;
						Color[3] = 1.0f;
						m_SpecularColor->vSetVector4(Color);
					}
				m_WorldViewProjM->vUnmap();						
			}   
        };								
	}			
}

/*
namespace LynxEngine
{
	IMPLEMENT_VOXELCLUSTERVS(FALSE, FALSE, FALSE)	// FakeAO : 0, AOLevel2 : 0, Specular : 0	
	IMPLEMENT_VOXELCLUSTERVS(FALSE, FALSE, TRUE)	// FakeAO : 0, AOLevel2 : 0, Specular : 1	

	IMPLEMENT_VOXELCLUSTERVS(TRUE, FALSE, FALSE)	// FakeAO : 1, AOLevel2 : 0, Specular : 0	
	IMPLEMENT_VOXELCLUSTERVS(TRUE, FALSE, TRUE)		// FakeAO : 1, AOLevel2 : 0, Specular : 1	

	IMPLEMENT_VOXELCLUSTERVS(TRUE, TRUE, FALSE)		// FakeAO : 1, AOLevel2 : 1, Specular : 0	
	IMPLEMENT_VOXELCLUSTERVS(TRUE, TRUE, TRUE)		// FakeAO : 1, AOLevel2 : 1, Specular : 1	
}
*/
