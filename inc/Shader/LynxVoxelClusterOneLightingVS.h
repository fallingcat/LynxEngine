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

#ifndef __LYNXVOXELCLUSTERONELIGHTINGVS_H__
#define __LYNXVOXELCLUSTERONELIGHTINGVS_H__

#include <Shader/LynxVoxelClusterVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		template <LYNXBOOL bFakeAO, LYNXBOOL bFakeAOLevel2, LYNXBOOL bSpecular, LYNXBOOL bUseShadowMap = LYNX_FALSE>
		class TVoxelClusterOneLightingVS : public CVoxelClusterVS
		{	
			LYNX_DECLARE_CLASS(TVoxelClusterOneLightingVS);
		public:						
		protected:								
			LYNXBOOL							m_bUseWorldViewProjection;
			Math::CVector3						m_LightDir;
			float								m_LightColor[4];	
			float								m_AmbientLightColor[4];

			GraphicsSystem::CShaderConstant*	m_AmbientColorConst;
			GraphicsSystem::CShaderConstant*	m_CameraPos;
			GraphicsSystem::CShaderConstant*	m_SpecularColor;    
		public:			
			TVoxelClusterOneLightingVS(GraphicsSystem::CGraphicsSystem* lpg)
			: CVoxelClusterVS(lpg)
			{
				m_bAOLevel2 = bFakeAOLevel2;

				m_LightDir = Math::CVector3(0.407f, 0.339f, -0.848f);
				m_LightColor[0] = 0.7f;
				m_LightColor[1] = 0.7f;
				m_LightColor[2] = 0.7f;
				m_LightColor[3] = 1.0f;
				m_CameraPos = NULL;
				m_SpecularColor = NULL;

				m_AmbientColorConst = NULL;
				m_AmbientLightColor[0] = 0.4f;
				m_AmbientLightColor[1] = 0.4f;
				m_AmbientLightColor[2] = 0.4f;
				m_AmbientLightColor[3] = 1.0f;

				//#if defined (__iOS__) || defined (__ANDROID__)
				#if defined (__MOBILE__)
					m_bUseWorldViewProjection = LYNX_FALSE;
				#else
					m_bUseWorldViewProjection = LYNX_TRUE;
				#endif
			}

			virtual ~TVoxelClusterOneLightingVS()
			{				
				if (m_CameraPos)
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_CameraPos);
				if (m_SpecularColor)
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_SpecularColor);            
				if (m_AmbientColorConst)
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant(m_AmbientColorConst);            
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

				ShaderMacro.Name = CAnsiString("USE_SHADOWMAP");
				ShaderMacro.Definition = bUseShadowMap ? CAnsiString("1") : CAnsiString("0");
				m_ShaderMacros.push_back(ShaderMacro);				

				ShaderMacro.Name = CAnsiString("USE_WORLDVIEWPROJECTION");
				ShaderMacro.Definition = m_bUseWorldViewProjection ? CAnsiString("1") : CAnsiString("0");
				m_ShaderMacros.push_back(ShaderMacro);				
				
				return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/voxelcluster_onelighting.vso"), &m_ShaderMacros);
			}

            virtual void vCreateConstantAndResource()
			{
				if (!m_WorldM)
					m_WorldM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorld");

				if (m_bUseWorldViewProjection)
				{
					if (!m_WorldViewProjM)
                        m_WorldViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gWorldViewProj");
				}
				else
				{
                    if (!m_ViewProjM)
                        m_ViewProjM = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gViewProj");
				}
                
                if (!m_LightDirConst)
					m_LightDirConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLightDir");

				if (!m_LightColorConst)
					m_LightColorConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gLightColor");

				if (!m_AmbientColorConst)
					m_AmbientColorConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gAmbientColor");
	            			
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

				if (!m_VoxelScaleConst)
					m_VoxelScaleConst = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstant(m_lpShader, "gVoxelScale");				
			}

			virtual void vSet(CContainer* lpc, CCameraContainer* cam)
			{
				Math::CMatrix4 MM, WVPM, WVM, WSM, VPM;
			
				GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();						
				lpR->SetVertexShader((GraphicsSystem::CVertexShader*)((GraphicsSystem::CShader*)m_lpShader));
	        
                m_VoxelScale = static_cast<CVoxelClusterContainer*>(lpc)->GetCurrentVoxelScale();
                
                if (m_bUseWorldViewProjection)
				{
					lynxMatrixXMatrix(&WVM, &lpc->GetTransformMatrix(), &cam->GetViewMatrix());
                    lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());
				}
				else
				{
                    lynxMatrixXMatrix(&VPM, &cam->GetViewMatrix(), &cam->GetProjectionMatrix());
				}

				float Color[4];		

				m_WorldM->vMap();
					m_WorldM->vSetMatrix(&(lpc->GetTransformMatrix()));
					// TODO : find out the bug of ES2 shader
					if (m_bUseWorldViewProjection)
					{
						m_WorldViewProjM->vSetMatrix(&WVPM);
					}
					else
					{
						m_ViewProjM->vSetMatrix(&VPM);
					}
                    
					SceneSystem::CLight* lpL = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentLight();
					if (lpL)
					{
						LYNXMATRIX WLPM, WLM;
						lynxMatrixXMatrix(&WLPM, &lpc->GetTransformMatrix(), &lpL->m_LightViewProj);								
						m_lpShader->vSetConstantM("gWorldLightViewProj", &WLPM);				

						if (lpL->GetLightType() == SceneSystem::CLight::DIRECTIONAL)
						{
							lpL->GetDirection(m_LightDir);							
							m_LightDir = -1.0f * m_LightDir;
						}
						m_LightColor[0] = (lpL->m_Color.Red * lpL->m_Intensity) / 255.0f;
						m_LightColor[1] = (lpL->m_Color.Green * lpL->m_Intensity) / 255.0f;
						m_LightColor[2] = (lpL->m_Color.Blue * lpL->m_Intensity)/ 255.0f;
						m_LightColor[3] = 1.0f;
					}
					m_LightDirConst->vSetVector3((float*)(&(m_LightDir.x)));					
					m_LightColorConst->vSetVector4(m_LightColor);

					lpL = m_lpGraphicsSystem->GetlpMaterialSystem()->GetlpCurrentAmbientLight();
					if (lpL)
					{
						m_AmbientLightColor[0] = (lpL->m_Color.Red * lpL->m_Intensity)/ 255.0f;
						m_AmbientLightColor[1] = (lpL->m_Color.Green * lpL->m_Intensity) / 255.0f;
						m_AmbientLightColor[2] = (lpL->m_Color.Blue * lpL->m_Intensity) / 255.0f;
						m_AmbientLightColor[3] = 1.0f;
					}
					m_AmbientColorConst->vSetVector4(m_AmbientLightColor);
					m_VoxelScaleConst->vSetFloat(m_VoxelScale);				

				m_WorldM->vUnmap();					

				if (bSpecular)
				{				
					m_SpecularColor->vMap();
						Math::CVector3 Pos;
						cam->vGetPosition(Pos);
						m_CameraPos->vSetVector3(&(Pos.x));			
						Color[0] = lpc->GetlpCurrentMaterial()->m_Specular.Red / 255.0f;
						Color[1] = lpc->GetlpCurrentMaterial()->m_Specular.Green / 255.0f;
						Color[2] = lpc->GetlpCurrentMaterial()->m_Specular.Blue / 255.0f;
						Color[3] = 1.0f;
						m_SpecularColor->vSetVector4(Color);							
					m_SpecularColor->vUnmap();
				}				
			}   
        };								
	}			
}

#define DECLARE_VOXELCLUSTERVS(bFakeAO, bFakeAOLevel2, bSpecular) \
	GraphicsSystem::CShaderClass* Create##VoxelClusterOneLightingBaseVS_##bFakeAO##_##bFakeAOLevel2##_##bSpecular(CEngine* e);

#define IMPLEMENT_VOXELCLUSTERVS(bFakeAO, bFakeAOLevel2, bSpecular) \
	namespace Shader{\
		typedef TVoxelClusterOneLightingVS<bFakeAO, bFakeAOLevel2, bSpecular>		 CVoxelClusterOneLightingBaseVS_##bFakeAO##_##bFakeAOLevel2##_##bSpecular;\
	}\
	LYNXFORCEINLINE IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingBaseVS_##bFakeAO##_##bFakeAOLevel2##_##bSpecular)

#define REGISTER_VOXELCLUSTERVS(classname, bFakeAO, bFakeAOLevel2, bSpecular) \
	m_ShaderClassManager.Register(classname, Create##VoxelClusterOneLightingBaseVS_##bFakeAO##_##bFakeAOLevel2##_##bSpecular);

namespace LynxEngine 
{
	IMPLEMENT_VOXELCLUSTERVS(FALSE, FALSE, FALSE)	// FakeAO : 0, AOLevel2 : 0, Specular : 0	
	IMPLEMENT_VOXELCLUSTERVS(FALSE, FALSE, TRUE)	// FakeAO : 0, AOLevel2 : 0, Specular : 1	

	IMPLEMENT_VOXELCLUSTERVS(TRUE, FALSE, FALSE)	// FakeAO : 1, AOLevel2 : 0, Specular : 0	
	IMPLEMENT_VOXELCLUSTERVS(TRUE, FALSE, TRUE)		// FakeAO : 1, AOLevel2 : 0, Specular : 1	

	IMPLEMENT_VOXELCLUSTERVS(TRUE, TRUE, FALSE)		// FakeAO : 1, AOLevel2 : 1, Specular : 0	
	IMPLEMENT_VOXELCLUSTERVS(TRUE, TRUE, TRUE)		// FakeAO : 1, AOLevel2 : 1, Specular : 1	
}

#endif