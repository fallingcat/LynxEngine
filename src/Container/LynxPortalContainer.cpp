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
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <SceneSystem/LynxScn.h>
#include <Container/LynxPortalContainer.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CPortalContainer::CPortalContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_ResourceType = CONTAINER;
		m_ContainerType = PORTAL;	
		m_VertexList.resize(4);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CPortalContainer::~CPortalContainer(void)
	{		
		m_VertexList.clear();			
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPortalContainer::vInstance(CContainer& rhs)
	{		
		CContainer::vInstance(rhs);
		CPortalContainer& RHS = Container_Cast<CPortalContainer&>(rhs);
		m_VertexList = RHS.m_VertexList;		
		m_VertexArray = RHS.m_VertexArray;		
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CPortalContainer::Setup(void)
	{
		/*
		for (int i=0; i<m_NumLODs; i++)
		{
			lynxCreateVertexArray(&m_VertexArray[i], &m_lpCurrentMaterial->GetVertexLayout(), m_VertexList[i].size(), LYNX_BOF_STATIC|LYNX_BOF_WRITEONLY, LYNX_TRUE);
			lynxLockVertexArray(&m_VertexArray[i], 0, 0, LYNX_LOCK_TO_WRITE);
			GraphicsSystem::STATICVERTEX* lpV = (GraphicsSystem::STATICVERTEX*)m_VertexArray[i].lpBuffer;
			for (int j=0; j<m_VertexList[i].size(); j++)
			{	
				lpV[j].Pos.s.x = m_VertexList[i][j].Pos.s.x;
				lpV[j].Pos.s.y = m_VertexList[i][j].Pos.s.y;
				lpV[j].Pos.s.z = m_VertexList[i][j].Pos.s.z;

				lpV[j].Tangent.s.x = m_VertexList[i][j].Tangent.s.x;
				lpV[j].Tangent.s.y = m_VertexList[i][j].Tangent.s.y;
				lpV[j].Tangent.s.z = m_VertexList[i][j].Tangent.s.z;

				lpV[j].Binormal.s.x = m_VertexList[i][j].Binormal.s.x;
				lpV[j].Binormal.s.y = m_VertexList[i][j].Binormal.s.y;
				lpV[j].Binormal.s.z = m_VertexList[i][j].Binormal.s.z;

				lpV[j].Normal.s.x = m_VertexList[i][j].Normal.s.x;
				lpV[j].Normal.s.y = m_VertexList[i][j].Normal.s.y;
				lpV[j].Normal.s.z = m_VertexList[i][j].Normal.s.z;

				lpV[j].TexCoord[0].u = m_VertexList[i][j].TexCoord[0].u;
				lpV[j].TexCoord[0].v = m_VertexList[i][j].TexCoord[0].v;			
			}		
			lynxUnlockVertexArray(&m_VertexArray[i]);	

			DWORD NumIndices = 0;

			for (int j= 0; j<m_PrimitiveList[i].size(); j++)
			{
				NumIndices += m_PrimitiveList[i][j].NumIndex;
			}

			lynxCreateIndexArray(&m_IndexArray[i], sizeof(WORD), NumIndices, LYNX_BOF_STATIC|LYNX_BOF_WRITEONLY, LYNX_TRUE);		
			lynxLockIndexArray(&m_IndexArray[i], 0, 0, LYNX_LOCK_TO_WRITE);
			WORD *lpI = (WORD *)m_IndexArray[i].lpBuffer;			
			NumIndices = 0;
			for (int j= 0; j<m_PrimitiveList[i].size(); j++)
			{			
				m_PrimitiveList[i][j].BaseIndex = NumIndices;
				for (int k=0; k<m_PrimitiveList[i][j].NumIndex; k++)
				{
					lpI[NumIndices] = m_PrimitiveList[i][j].lpIndex[k];				
					NumIndices++;
				}		
				// free original primitive index data
				delete [] m_PrimitiveList[i][j].lpIndex;
			}
			lynxUnlockIndexArray(&m_IndexArray[i]);
		}
		*/

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPortalContainer::vRender(CCameraContainer* cam)
	{	
		/*
		LYNXMATRIX MM, WVPM, WVM;
		float ZScale;
		float ZBias = 0.0005f;
		float TexelGrad[2];
		float Color[3];
		LYNXVECTOR3D Pos;
		float Rad;
		int	 FilterType;
		float WireframeColor[4];

		lynxSetVertexArray(&m_VertexArray[m_CurrentLOD], 0, 0);		
		lynxSetIndexArray(&m_IndexArray[m_CurrentLOD], 0);

		LYNX_ASSERT(cam);		
		if (cam->IsMirror())
		{
			lynxMatrixXMatrix(&MM, &m_M, &cam->GetMirrorMatrix());	
			lynxMatrixXMatrix(&WVM, &MM, &cam->GetViewMatrix());	
			lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					
		}
		else
		{
			lynxMatrixXMatrix(&WVM, &m_M, &cam->GetViewMatrix());	
			lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					
		}

		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)
		{
			Technique.GetPass(p).Set();

			if (cam->IsMirror())
			{
				switch (Technique.GetPass(p).m_CullMode)
				{
					case LYNX_CULL_CCW:
						lynxSetHardwareCullMode(LYNX_CULL_CW);
						break;

					case LYNX_CULL_CW:
						lynxSetHardwareCullMode(LYNX_CULL_CCW);
						break;
				}
			}

			lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldViewProj", &WVPM);
			lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldView", &WVM);	
			lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorld", &m_M);	
			
			switch (m_CurrentMaterialTechnique)
			{
				case MaterialSystem::CMaterial::VARIANCE_SHADOWMAP:
				case MaterialSystem::CMaterial::SHADOWMAP:
					if (m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight())
					{
						m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetPosition(&Pos);
						lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gLightPos", &Pos.s.x, 3);																	
						lynxSetVertexShaderConstantTable(lynxGetCurrentVertexShader(), "gDepthScale", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_DepthScale);													
					}
					break;

				case MaterialSystem::CMaterial::ZPASS:
					lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gLastWorldViewProj", &m_LastWVPM);								
					if (cam->m_bMotionBlur)
					{
						if (cam->m_bCameraMovingBlur)
						{							
							SetLastWVPM(WVPM);														
							lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbMotionBlur", LYNX_TRUE);				
						}
						else
						{
							lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbMotionBlur", LYNX_FALSE);				
						}
					}
					else
					{
						lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbMotionBlur", LYNX_FALSE);				
					}					
					break;
				
				case MaterialSystem::CMaterial::SIMPLE:
					break;

				case MaterialSystem::CMaterial::AMBIENT_LIGHTING:
					break;

				case MaterialSystem::CMaterial::POINT_LIGHTING:
					if (m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight())
					{
						lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldLightViewProj", &m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_LightViewProj);			
						lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldLightView", &m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetCamera().GetViewMatrix());									
						cam->GetPosition(&Pos);
						lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gCameraPos", &Pos.s.x, 3);	

						if (m_lpRenObj->m_bReceiveShadow)
						{
							lynxSetTexture(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent());
							lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_U_COORD, LYNX_ADDRESS_CLAMP);
							lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_V_COORD, LYNX_ADDRESS_CLAMP);
							if (m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType >= GraphicsSystem::CShadowMap::VSM && 
								m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType < GraphicsSystem::CShadowMap::NUM_SHADOWFILTER_TYPES)
								lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
							else
								lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_POINT, LYNX_TF_POINT, LYNX_TF_POINT); 					

							lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_CF_MODULATE, LYNX_CA_CURRENT, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);	
							lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL+1, LYNX_CF_NONE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);													

							TexelGrad[0] = 1.0f/(lynxGetTextureWidth(m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent()));
							TexelGrad[1] = 1.0f/(lynxGetTextureHeight(m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent()));
							lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gShadowmapTexelGrad", TexelGrad, 2);
						}
						m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetPosition(&Pos);
						lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gLightPos", &Pos.s.x, 3);							
						lynxSetVertexShaderConstantTable(lynxGetCurrentVertexShader(), "gDepthScale", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_DepthScale);					
					
						Color[0] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Red/255.0f;
						Color[1] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Green/255.0f;
						Color[2] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Blue/255.0f;
						lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gLight.Color", Color, 3);
						lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gLight.Radius", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Radius);								
						lynxSetPixelShaderConstantTableI(lynxGetCurrentPixelShader(), "gShadowFilterType", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType);										
						lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbGlow", m_bGlow);
						lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gBloomingIntensity", m_BloomingIntensity);					
						lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbShadowing", m_lpRenObj->m_bReceiveShadow);			
						lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gZBias", ZBias);							
					}
					break;

				case MaterialSystem::CMaterial::POINT_LIGHTING_RNR:
					lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldLightViewProj", &m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_LightViewProj);			
					lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldLightView", &m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetCamera().GetViewMatrix());									
					cam->GetPosition(&Pos);
					lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gCameraPos", &Pos.s.x, 3);							
                    
					if (m_lpRenObj->m_bReceiveShadow)
					{
						lynxSetTexture(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent());
						lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_U_COORD, LYNX_ADDRESS_CLAMP);
						lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_V_COORD, LYNX_ADDRESS_CLAMP);
						if (m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType >= GraphicsSystem::CShadowMap::VSM && 
							m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType < GraphicsSystem::CShadowMap::NUM_SHADOWFILTER_TYPES)
							lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
						else
							lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_POINT, LYNX_TF_POINT, LYNX_TF_POINT); 					
						lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_CF_MODULATE, LYNX_CA_CURRENT, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);							

						lynxSetTexture(MaterialSystem::CTechnique::REFLECTIONMAP_CHANNEL, m_lpEngine->GetlpGraphicsSystem()->GetlpReflectionMap()->GetCurrent());
						lynxSetTextureAddressMode(MaterialSystem::CTechnique::REFLECTIONMAP_CHANNEL, LYNX_U_COORD, LYNX_ADDRESS_CLAMP);
						lynxSetTextureAddressMode(MaterialSystem::CTechnique::REFLECTIONMAP_CHANNEL, LYNX_V_COORD, LYNX_ADDRESS_CLAMP);
						lynxSetTextureFilterMode(MaterialSystem::CTechnique::REFLECTIONMAP_CHANNEL, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
						lynxSetTexture(MaterialSystem::CTechnique::REFRACTIONMAP_CHANNEL, m_lpEngine->GetlpGraphicsSystem()->GetlpRefractionMap()->GetCurrent());
						lynxSetTextureAddressMode(MaterialSystem::CTechnique::REFRACTIONMAP_CHANNEL, LYNX_U_COORD, LYNX_ADDRESS_CLAMP);
						lynxSetTextureAddressMode(MaterialSystem::CTechnique::REFRACTIONMAP_CHANNEL, LYNX_V_COORD, LYNX_ADDRESS_CLAMP);
						lynxSetTextureFilterMode(MaterialSystem::CTechnique::REFRACTIONMAP_CHANNEL, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
						lynxSetTextureBlend(MaterialSystem::CTechnique::REFRACTIONMAP_CHANNEL+1, LYNX_CF_NONE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);													

						TexelGrad[0] = 1.0f/(lynxGetTextureWidth(m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent()));
						TexelGrad[1] = 1.0f/(lynxGetTextureHeight(m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent()));
						lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gShadowmapTexelGrad", TexelGrad, 2);
					}													

					m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetPosition(&Pos);
					lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gLightPos", &Pos.s.x, 3);							
					lynxSetVertexShaderConstantTable(lynxGetCurrentVertexShader(), "gDepthScale", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_DepthScale);					
					
					Color[0] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Red/255.0f;
					Color[1] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Green/255.0f;
					Color[2] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Blue/255.0f;
					lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gLight.Color", Color, 3);
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gLight.Radius", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Radius);								
					
					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbGlow", m_bGlow);
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gBloomingIntensity", m_BloomingIntensity);					
					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbShadowing", m_lpRenObj->m_bReceiveShadow);			
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gZBias", ZBias);		
					lynxSetPixelShaderConstantTableI(lynxGetCurrentPixelShader(), "gShadowFilterType", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType);										
					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbReflection", m_bReflect);										
					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbRefraction", m_bRefract);															
					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbUseBaseColor", LYNX_FALSE);															
					break;

				case MaterialSystem::CMaterial::SPOT_LIGHTING:
					lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldLightViewProj", &m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_LightViewProj);			
					lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldLightView", &m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetCamera().GetViewMatrix());									
					cam->GetPosition(&Pos);
					lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gCameraPos", &Pos.s.x, 3);	

					if (m_lpRenObj->m_bReceiveShadow)
					{
						lynxSetTexture(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent());
						lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_U_COORD, LYNX_ADDRESS_CLAMP);
						lynxSetTextureAddressMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_V_COORD, LYNX_ADDRESS_CLAMP);
						//if (m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType >= GraphicsSystem::CShadowMap::VSM && 
						//	m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType < GraphicsSystem::CShadowMap::NUM_SHADOWFILTER_TYPES)
							lynxSetTextureFilterMode(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
						//else
						//	lynxSetTextureFilterMode(3, LYNX_TF_POINT, LYNX_TF_POINT, LYNX_TF_POINT); 					
						lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL, LYNX_CF_MODULATE, LYNX_CA_CURRENT, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);	
						lynxSetTextureBlend(MaterialSystem::CTechnique::SHADOWMAP_CHANNEL+1, LYNX_CF_NONE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);													

						TexelGrad[0] = 1.0f/(lynxGetTextureWidth(m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent()));
						TexelGrad[1] = 1.0f/(lynxGetTextureHeight(m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetShadowMap().GetCurrent()));
						lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gShadowmapTexelGrad", TexelGrad, 2);
					}													

					m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetPosition(&Pos);
					lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gLightPos", &Pos.s.x, 3);							
					lynxSetVertexShaderConstantTable(lynxGetCurrentVertexShader(), "gDepthScale", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_DepthScale);					
					
					Color[0] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Red/255.0f;
					Color[1] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Green/255.0f;
					Color[2] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Blue/255.0f;
					lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gLight.Color", Color, 3);
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gLight.Radius", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Radius);								
					Rad = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Deg*LYNXD2R;
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gLight.Rad", Rad);
					Rad = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_FadeOutDeg*LYNXD2R;
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gLight.FadeOutRad", Rad);								
					m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetDirection(&Pos);
					lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gLight.Dir", &Pos.s.x, 3);													

					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbGlow", m_bGlow);
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gBloomingIntensity", m_BloomingIntensity);					
					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbShadowing", m_lpRenObj->m_bReceiveShadow);			
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gZBias", ZBias);										
					lynxSetPixelShaderConstantTableI(lynxGetCurrentPixelShader(), "gShadowFilterType", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_ShadowFilterType);										
					break;

				case MaterialSystem::CMaterial::SPOT_LIGHTING_NO_SHADOW:
					if (m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight())
					{
						lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldLightViewProj", &m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_LightViewProj);			
						lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldLightView", &m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetCamera().GetViewMatrix());									
						cam->GetPosition(&Pos);
						lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gCameraPos", &Pos.s.x, 3);																			

						m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetPosition(&Pos);
						lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gLightPos", &Pos.s.x, 3);							
						lynxSetVertexShaderConstantTable(lynxGetCurrentVertexShader(), "gDepthScale", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_DepthScale);					
						
						Color[0] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Red/255.0f;
						Color[1] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Green/255.0f;
						Color[2] = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Color.Blue/255.0f;
						lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gLight.Color", Color, 3);
						lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gLight.Radius", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Radius);								
						Rad = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_Deg*LYNXD2R;
						lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gLight.Rad", Rad);
						Rad = m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_FadeOutDeg*LYNXD2R;
						lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gLight.FadeOutRad", Rad);								
						m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetDirection(&Pos);
						lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gLight.Dir", &Pos.s.x, 3);													
					}

					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbGlow", m_bGlow);
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gBloomingIntensity", m_BloomingIntensity);					
					lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbShadowing", m_lpRenObj->m_bReceiveShadow);			
					lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gZBias", ZBias);										
					break;

				case MaterialSystem::CMaterial::WIREFRAME:				
					WireframeColor[0] = Technique.GetPass(p).m_Diffuse.s.Red/255.0f;
					WireframeColor[1] = Technique.GetPass(p).m_Diffuse.s.Green/255.0f;
					WireframeColor[2] = Technique.GetPass(p).m_Diffuse.s.Blue/255.0f;
					WireframeColor[3] = Technique.GetPass(p).m_Diffuse.s.Alpha/255.0f;
					lynxSetPixelShaderConstantTableArray(lynxGetCurrentPixelShader(), "gOutputColor", WireframeColor, 4);	
					break;

				default:
					break;
			}

			for (int i=0; i<m_PrimitiveList[m_CurrentLOD].size(); ++i)
			{						
				lynxDrawPrimitiveBO(&m_PrimitiveList[m_CurrentLOD][i], 0, m_VertexList[m_CurrentLOD].size());						
			}
		}
		*/
	}
}