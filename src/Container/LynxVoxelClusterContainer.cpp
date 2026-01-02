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
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Container/LynxVoxelClusterContainer.h>
#include <Container/LynxCameraContainer.h>
#include <SceneSystem/Light/LynxLight.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <Shader/LynxVoxelClusterOneLightingVS.h>
#include <Shader/LynxVoxelSpriteVS.h>

namespace LynxEngine 
{	
	const float CVoxelClusterContainer::VOXEL_UNIT = 1.0f; 
    const float CVoxelClusterContainer::VOXEL_HALFSIZE = 0.5f;//0.475f;
	#ifdef __MOBILE__
        int CVoxelClusterContainer::m_BatchSize = 36;
		int CVoxelClusterContainer::m_SpriteBatchSize = 54;
	#elif defined __DESKTOP__
		int CVoxelClusterContainer::m_BatchSize = 80; 
		int CVoxelClusterContainer::m_SpriteBatchSize = 96;
	#endif

	GraphicsSystem::CVertexArrayPtr		CVoxelClusterContainer::m_SharedVertexArray;
	GraphicsSystem::CIndexArrayPtr		CVoxelClusterContainer::m_SharedIndexArray;
	GraphicsSystem::CVertexLayoutPtr	CVoxelClusterContainer::m_SpriteVertexLayout;
	GraphicsSystem::CVertexArrayPtr		CVoxelClusterContainer::m_SharedSpriteVertexArray;
	GraphicsSystem::CIndexArrayPtr		CVoxelClusterContainer::m_SharedSpriteIndexArray;
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CVoxelClusterContainer::CVoxelClusterContainer(CEngine *lpengine)	
	: CContainer(lpengine)
	{
		m_ContainerType = VOXEL_CLUSTER;		
		m_RenderedSize.clear();
		m_ClusterSize.clear();
		m_lpV = NULL;		
		m_bInstancingVoxelArray = LYNX_FALSE;
		m_CurrentVoxelArray = 0;
		m_VoxelAnimationArray.clear();
		m_GlowVoxelData.clear();
		
		m_VoxelScale.clear();
		m_VoxelScale.push_back(1.0f);

		m_PositionBuffer = NULL;
		m_ColorBuffer = NULL;		

		m_bSpriteRender = LYNX_FALSE;
		m_bRenderOccludedVoxels = LYNX_FALSE;

		m_bGlowDataDirty = LYNX_FALSE;
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
	CVoxelClusterContainer::~CVoxelClusterContainer(void)
	{			
		for (int i=0; i<m_VoxelAnimationArray.size(); i++)
		{
			LYNXDEL m_VoxelAnimationArray[i];
		}
		m_VoxelAnimationArray.clear();

		for (int i=0; i<m_GlowVoxelData.size(); i++)
		{
			if (m_GlowVoxelData[i].VoxelList)
			{
				LYNXGLOBAL_DEL_ARRAY(m_GlowVoxelData[i].VoxelList);
				m_GlowVoxelData[i].VoxelList = NULL;
			}
			if (m_GlowVoxelData[i].VoxelPosArray)
			{
				LYNXGLOBAL_DEL_ARRAY(m_GlowVoxelData[i].VoxelPosArray);
				m_GlowVoxelData[i].VoxelPosArray = NULL;
			}
			if (m_GlowVoxelData[i].VoxelColorArray)
			{
				LYNXGLOBAL_DEL_ARRAY(m_GlowVoxelData[i].VoxelColorArray);
				m_GlowVoxelData[i].VoxelColorArray = NULL;
			}
		}
		m_GlowVoxelData.clear();

		m_RenderedSize.clear();
		m_ClusterSize.clear();
		m_VoxelScale.clear();
   	}			
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	//CVoxelClusterContainer& CVoxelClusterContainer::operator =(const CVoxelClusterContainer& rhs)
	//{	
	//	return (*this);
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CVoxelClusterContainer::vInstance(const CContainer& rhs)
	{
		CContainer::vInstance(rhs);

		const CVoxelClusterContainer* RHS = Container_Cast<const CVoxelClusterContainer*>(&rhs);	

		m_VoxelScale = RHS->m_VoxelScale;
		m_Name = RHS->GetName();		
		m_Dimension = RHS->m_Dimension;
		m_ClusterSize = RHS->m_ClusterSize;
		m_RenderedSize = RHS->m_RenderedSize;				
		m_OccludedSize = RHS->m_OccludedSize;
		m_LocalAABB = RHS->m_LocalAABB;

		if (m_bInstancingVoxelArray)
		{
			m_VoxelArray = RHS->m_VoxelArray;
		}
		else
		{
			m_VoxelArray = CCountedPtr<VOXELARRAY>(LYNXNEW VOXELARRAY);
			m_VoxelArray->PosArray.resize(RHS->m_VoxelArray->PosArray.size());
			for (int i=0; i<RHS->m_VoxelArray->PosArray.size(); i++)
			{
				m_VoxelArray->PosArray[i] = LYNXGLOBAL_NEW LYNXVECTOR3D[RHS->m_ClusterSize[i]];
				lynxMemCpy(m_VoxelArray->PosArray[i], RHS->m_VoxelArray->PosArray[i], sizeof(LYNXVECTOR3D)*RHS->m_ClusterSize[i]);			
			}		

			m_VoxelArray->ColorArray.resize(RHS->m_VoxelArray->ColorArray.size());
			for (int i=0; i<RHS->m_VoxelArray->ColorArray.size(); i++)
			{
				m_VoxelArray->ColorArray[i] = LYNXGLOBAL_NEW float[RHS->m_ClusterSize[i]*4];
				lynxMemCpy(m_VoxelArray->ColorArray[i], RHS->m_VoxelArray->ColorArray[i], sizeof(float)*RHS->m_ClusterSize[i]*4);			
			}

			m_VoxelArray->ColorIndexArray.resize(RHS->m_VoxelArray->ColorIndexArray.size());
			for (int i=0; i<RHS->m_VoxelArray->ColorIndexArray.size(); i++)
			{
				m_VoxelArray->ColorIndexArray[i] = LYNXGLOBAL_NEW int[RHS->m_ClusterSize[i]];
				lynxMemCpy(m_VoxelArray->ColorIndexArray[i], RHS->m_VoxelArray->ColorIndexArray[i], sizeof(int)*RHS->m_ClusterSize[i]);			
			}
			
            #if __FAKE_VOXEL_AO__
				#ifdef __DESKTOP__
					m_VoxelArray->AOCodeArray.resize(RHS->m_VoxelArray->AOCodeArray.size());
					for (int i=0; i<RHS->m_VoxelArray->AOCodeArray.size(); i++)
					{
						m_VoxelArray->AOCodeArray[i] = LYNXGLOBAL_NEW DWORD[RHS->m_ClusterSize[i]*2];
						lynxMemCpy(m_VoxelArray->AOCodeArray[i], RHS->m_VoxelArray->AOCodeArray[i], sizeof(DWORD)*RHS->m_ClusterSize[i]*2);
					}
				#else
					m_VoxelArray->AOCodeArray.resize(RHS->m_VoxelArray->AOCodeArray.size());
					for (int i=0; i<RHS->m_VoxelArray->AOCodeArray.size(); i++)
					{
						m_VoxelArray->AOCodeArray[i] = LYNXNEW DWORD[RHS->m_ClusterSize[i]*4];
						lynxMemCpy(m_VoxelArray->AOCodeArray[i], RHS->m_VoxelArray->AOCodeArray[i], sizeof(DWORD)*RHS->m_ClusterSize[i]*4);
					}
				#endif
            #endif

			m_GlowVoxelData.resize(RHS->m_GlowVoxelData.size()); 
			for (int i=0; i<RHS->m_GlowVoxelData.size(); i++)
			{
				m_GlowVoxelData[i].NumVoxels = RHS->m_GlowVoxelData[i].NumVoxels;
				m_GlowVoxelData[i].Scale = RHS->m_GlowVoxelData[i].Scale;
				m_GlowVoxelData[i].VoxelList = LYNXGLOBAL_NEW LYNXDWORD[m_GlowVoxelData[i].NumVoxels];				
				lynxMemCpy(m_GlowVoxelData[i].VoxelList, RHS->m_GlowVoxelData[i].VoxelList, sizeof(LYNXDWORD)*m_GlowVoxelData[i].NumVoxels);
				m_GlowVoxelData[i].VoxelPosArray = LYNXGLOBAL_NEW LYNXVECTOR3D[m_GlowVoxelData[i].NumVoxels];				
				lynxMemCpy(m_GlowVoxelData[i].VoxelPosArray, RHS->m_GlowVoxelData[i].VoxelPosArray, sizeof(LYNXVECTOR3D)*m_GlowVoxelData[i].NumVoxels);
				m_GlowVoxelData[i].VoxelColorArray = LYNXGLOBAL_NEW float[m_GlowVoxelData[i].NumVoxels*4];				
				lynxMemCpy(m_GlowVoxelData[i].VoxelColorArray, RHS->m_GlowVoxelData[i].VoxelColorArray, sizeof(float)*4*m_GlowVoxelData[i].NumVoxels);
			}

		}

		m_VoxelAnimationArray.resize(RHS->m_VoxelAnimationArray.size());
		for (int i=0; i<m_VoxelAnimationArray.size(); i++)
		{
			m_VoxelAnimationArray[i] = LYNXNEW Animation::CVoxelAnimation(m_lpEngine);
			m_VoxelAnimationArray[i]->vInstance(*(RHS->m_VoxelAnimationArray[i]));
		}
		m_VertexArray = RHS->m_VertexArray;
		m_OptimizedVoxelIndexArray = RHS->m_OptimizedVoxelIndexArray;
		m_DisplayListSize = RHS->m_DisplayListSize;
		m_DisplayIndexArray = RHS->m_DisplayIndexArray;
		m_NumBatchs = RHS->m_NumBatchs;
		m_bSpriteRender = RHS->m_bSpriteRender;
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
	LYNXBOOL CVoxelClusterContainer::vCreate(const CCreationDesc* desc)
	{
		MaterialSystem::CTechnique* Technique;
		CVoxelClusterCreationDesc* Desc = (CVoxelClusterCreationDesc*)desc;			
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

		DDesc.Default();
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		if (!desc->m_Name.empty())
			SetName(desc->m_Name);
		else
			SetName(desc->m_FileName);

		//m_BatchSize = (m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCaps().MaxVertexShaderConstants - 16) / 3;

		m_NumLODs = 1;			
		
		if (Desc->m_lpStream)
			Desc->m_lpStream->vClose();

		m_VoxelScale.clear();
		m_VoxelScale = Desc->m_VoxelScale;			

		if (Desc->m_FileName.size() ||  Desc->m_SeqFilename.size())
		{
			if (!LoadVXL(Desc->m_lpStream, Desc->m_FileName, Desc))
				return LYNX_FALSE;
		}
		else if (Desc->m_lpData)
		{
			m_ClusterSize.resize(1);
			m_RenderedSize.resize(1);
			m_OccludedSize.resize(1);

			if (Desc->m_bOverwriteClusterSize)
			{
				m_ClusterSize[0] = Desc->m_DataSize;
			}
			else
			{
				m_ClusterSize[0] = Desc->m_ClusterSize;
			}
			m_VoxelArray = CCountedPtr<VOXELARRAY>(LYNXNEW VOXELARRAY);
			m_VoxelArray->PosArray.resize(1);
			m_VoxelArray->ColorArray.resize(1);
			m_VoxelArray->ColorIndexArray.resize(1);
			m_VoxelArray->PosArray[0] = LYNXGLOBAL_NEW LYNXVECTOR3D[m_ClusterSize[0]];				
			m_VoxelArray->ColorArray[0] = LYNXGLOBAL_NEW float[m_ClusterSize[0]*4];
			m_VoxelArray->ColorIndexArray[0] = LYNXGLOBAL_NEW int[m_ClusterSize[0]];

			m_GlowVoxelData.resize(1);

			LYNX_ASSERT(Desc->m_DataSize < m_ClusterSize[0]);
			for (int i=0; i<Desc->m_DataSize; i++)
			{
				vSetPosition(i, Desc->m_lpData[i].Pos*VOXEL_UNIT*m_VoxelScale[0]);
				SetColor(i, &Desc->m_lpData[i].Color);
				m_VoxelArray->ColorIndexArray[0][i] = Desc->m_lpData[i].AttributeColorIndex;
			}

			#if __FAKE_VOXEL_AO__					
				/*
				m_AOOccluder.resize(m_ClusterSize);
				for (int c=0; c<m_ClusterSize; c++)
				{
					m_AOOccluder[c].resize(12);
					for (int o=0; o<12; o++)
						m_AOOccluder[c][o] = LYNX_FALSE;
				}
				*/
				m_VoxelArray->AOCodeArray.resize(1);
				#if defined (__DESKTOP__) && !defined(__OGLES2__)
					m_VoxelArray->AOCodeArray[0] = LYNXGLOBAL_NEW DWORD[m_ClusterSize[0]*2];
					for (int c=0; c<m_ClusterSize[0]; c++)
					{
						m_VoxelArray->AOCodeArray[0][c*2+0] = 0;
						m_VoxelArray->AOCodeArray[0][c*2+1] = 0;						
					}
				#else
					m_VoxelArray->AOCodeArray[0] = LYNXNEW DWORD[m_ClusterSize[0]*4];
					for (int c=0; c<m_ClusterSize[0]; c++)
					{
						m_VoxelArray->AOCodeArray[0][c*4+0] = 0;
						m_VoxelArray->AOCodeArray[0][c*4+1] = 0;
						m_VoxelArray->AOCodeArray[0][c*4+2] = 0;
						m_VoxelArray->AOCodeArray[0][c*4+3] = 0;
					}
				#endif
			#endif

			m_RenderedSize[0] = Desc->m_DataSize;
		}		
		else
		{
			m_ClusterSize.resize(1);
			m_RenderedSize.resize(1);
			m_OccludedSize.resize(1);

			m_ClusterSize[0] = Desc->m_ClusterSize;
			m_VoxelArray = CCountedPtr<VOXELARRAY>(LYNXNEW VOXELARRAY);
			m_VoxelArray->PosArray.resize(1);
			m_VoxelArray->ColorArray.resize(1);
			m_VoxelArray->ColorIndexArray.resize(1);
			m_VoxelArray->PosArray[0] = LYNXGLOBAL_NEW LYNXVECTOR3D[m_ClusterSize[0]];				
			m_VoxelArray->ColorArray[0] = LYNXGLOBAL_NEW float[m_ClusterSize[0]*4];
			m_VoxelArray->ColorIndexArray[0] = LYNXGLOBAL_NEW int[m_ClusterSize[0]];

			m_GlowVoxelData.resize(1);

			for (int i=0; i<m_ClusterSize[0]; i++)
			{
				m_VoxelArray->ColorArray[0][i*4] = m_VoxelArray->ColorArray[0][i*4+1] = m_VoxelArray->ColorArray[0][i*4+2] = m_VoxelArray->ColorArray[0][i*4+3] = 1.0f;
				m_VoxelArray->PosArray[0][i].x = m_VoxelArray->PosArray[0][i].y = m_VoxelArray->PosArray[0][i].z = 0.0f;			
				m_VoxelArray->ColorIndexArray[0][i] = -1;
			}

			#if __FAKE_VOXEL_AO__					
				/*
				m_AOOccluder.resize(m_ClusterSize);
				for (int c=0; c<m_ClusterSize; c++)
				{
					m_AOOccluder[c].resize(12);
					for (int o=0; o<12; o++)
						m_AOOccluder[c][o] = LYNX_FALSE;
				}
				*/
				m_VoxelArray->AOCodeArray.resize(1);
				#if defined (__DESKTOP__) && !defined(__OGLES2__)
					m_VoxelArray->AOCodeArray[0] = LYNXGLOBAL_NEW DWORD[m_ClusterSize[0]*2];
					for (int c=0; c<m_ClusterSize[0]; c++)
					{
						m_VoxelArray->AOCodeArray[0][c*2+0] = 0;
						m_VoxelArray->AOCodeArray[0][c*2+1] = 0; 						
					}
				#else
					m_VoxelArray->AOCodeArray[0] = LYNXNEW DWORD[m_ClusterSize[0]*4];
					for (int c=0; c<m_ClusterSize[0]; c++)
					{
						m_VoxelArray->AOCodeArray[0][c*4+0] = 0;
						m_VoxelArray->AOCodeArray[0][c*4+1] = 0;
 						m_VoxelArray->AOCodeArray[0][c*4+2] = 0;
						m_VoxelArray->AOCodeArray[0][c*4+3] = 0;
					}
				#endif
			#endif

			m_RenderedSize[0] = 0;
		}

		if (desc->m_bCreateMaterial)
		{
			CString MatName;

			if (Desc->m_MaterialName.empty())
			{
				MatName = m_Name + CString(_T(".Material"));
			}
			else
			{
				MatName = Desc->m_MaterialName;
			}

			MaterialSystem::CMaterialPtr lpMat = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MatName);
			if (!lpMat)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpMat, MaterialSystem::CMaterial::STANDARD);
				lpMat->SetName(MatName);
				lpMat->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_VOXEL_CLUSTER));
				if (!m_SpriteVertexLayout)
				{
					m_lpEngine->CreateVertexLayout(m_SpriteVertexLayout, GetEnumStr(GraphicsSystem::VT_SPRITE3D_CLUSTER));
				}
				CStream* File = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
				lpMat->vSetNumTextureAnimation(4);
#if __FAKE_VOXEL_AO__
				LYNXCHAR OldTexturePath[128];
				lynxStrCpy(OldTexturePath, m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath());
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
				lpMat->GetTextureAnimation(0)->Create(1);
				lpMat->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::AMBIENTMAP));
				lpMat->GetTextureAnimation(0)->LoadTexture(0, *File, _T("voxel_ao.png"));
				SamDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
				SamDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
				SamDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
				lpMat->GetTextureAnimation(0)->GetTexture(0)->CreateSamplerState(SamDesc);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(OldTexturePath);
#endif

				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
				lpMat->GetTextureAnimation(3)->Create(1);
				lpMat->GetTextureAnimation(3)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SHADOWMAP));
				lpMat->GetTextureAnimation(3)->LoadTexture(0, *File, _T("voxel_ao.png"));
				SamDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
				SamDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
				SamDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
				lpMat->GetTextureAnimation(3)->GetTexture(0)->CreateSamplerState(SamDesc);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(OldTexturePath);

				lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
				Technique->GetPass(0).CreateDepthStencilState(DDesc);
				BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).vCreateShaderClasses("VoxelClusterVS", "ColorPS");
				//Technique->GetPass(0).vCreateShaderClasses("VoxelClusterOneLightingFakeAOVS", "VoxelColorFakeAOPS");
				//Technique->GetPass(0).vCreateShaderClasses("VoxelClusterOneLightingVS", "VoxelColorPS");

				//if (Desc->m_bSpriteRender || m_lpEngine->GetlpGraphicsSystem()->m_Config.bCreateVoxelSpriteSource)
				{
					Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE);
					Technique->vAddPass(1);
					Technique->GetPass(1).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
					Technique->GetPass(1).CreateDepthStencilState(DDesc);
					BDesc.RTBlendParam[0].SrcBlend = LynxEngine::GraphicsSystem::CRenderer::BF_SRC_ALPHA;
					BDesc.RTBlendParam[0].DestBlend = LynxEngine::GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
					BDesc.RTBlendParam[0].BlendOp = LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(1).CreateBlendState(BDesc);
					RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
					Technique->GetPass(1).CreateRasterizerState(RDesc);
					Technique->GetPass(1).vCreateShaderClasses("VoxelSpriteVS", "ColorPS");
					if (Technique->GetPass(1).GetShaderGroup())
						Technique->GetPass(1).GetShaderGroup()->AddVertexLayout(m_SpriteVertexLayout);
				}

#if defined(__DESKTOP__) && !defined (__MOBILE_SIMULATION__)					
				lpMat->vAddTechnique(MaterialSystem::CMaterial::SHADOW_TEXTURE);
				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SHADOW_TEXTURE);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;
				Technique->GetPass(0).CreateDepthStencilState(DDesc);
				BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).vCreateShaderClasses("VoxelClusterVS", "VoxelColorPS");

				lpMat->vAddTechnique(MaterialSystem::CMaterial::SHADOWMAP);
				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SHADOWMAP);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;
				Technique->GetPass(0).CreateDepthStencilState(DDesc);
				BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				//Technique->GetPass(0).vCreateShaderClasses("VoxelClusterShadowMapVS", "ShadowMapPS");
				Technique->GetPass(0).vCreateShaderClasses("VoxelClusterVS", "ConstantColorPS");

				lpMat->vAddTechnique(MaterialSystem::CMaterial::ONE_LIGHTING);
				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ONE_LIGHTING);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
				Technique->GetPass(0).CreateDepthStencilState(DDesc);
				BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).vCreateShaderClasses("VoxelClusterOneLightingFakeAOLevel2SpecularVS", "VoxelColorFakeAOLevel2PS");
				//Technique->GetPass(0).vCreateShaderClasses("VoxelClusterOneLightingVS", "VoxelColorPS");

				//if (Desc->m_bSpriteRender || m_lpEngine->GetlpGraphicsSystem()->m_Config.bCreateVoxelSpriteSource)
				{
					Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ONE_LIGHTING);
					Technique->vAddPass(1);
					Technique->GetPass(1).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
					Technique->GetPass(1).CreateDepthStencilState(DDesc);
					BDesc.RTBlendParam[0].SrcBlend = LynxEngine::GraphicsSystem::CRenderer::BF_SRC_ALPHA;
					BDesc.RTBlendParam[0].DestBlend = LynxEngine::GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
					BDesc.RTBlendParam[0].BlendOp = LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(1).CreateBlendState(BDesc);
					RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
					Technique->GetPass(1).CreateRasterizerState(RDesc);
					Technique->GetPass(1).vCreateShaderClasses("VoxelSpriteOneLightingVS", "ColorPS");
					//Technique->GetPass(1).vCreateShaderClasses("VoxelSpriteVS", "ColorPS");
					if (Technique->GetPass(1).GetShaderGroup())
						Technique->GetPass(1).GetShaderGroup()->AddVertexLayout(m_SpriteVertexLayout);
				}
#elif defined(__MOBILE__) || defined (__MOBILE_SIMULATION__)
				lpMat->vAddTechnique(MaterialSystem::CMaterial::ONE_LIGHTING);
				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ONE_LIGHTING);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
				Technique->GetPass(0).CreateDepthStencilState(DDesc);
				BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).vCreateShaderClasses("VoxelClusterOneLightingFakeAOLevel2SpecularVS", "VoxelColorFakeAOLevel2PS");
				//Technique->GetPass(0).vCreateShaderClasses("VoxelClusterOneLightingVS", "VoxelColorPS");

				//if (Desc->m_bSpriteRender || m_lpEngine->GetlpGraphicsSystem()->m_Config.bCreateVoxelSpriteSource)
				{
					Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ONE_LIGHTING);
					Technique->vAddPass(1);
					Technique->GetPass(1).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
					Technique->GetPass(1).CreateDepthStencilState(DDesc);
					BDesc.RTBlendParam[0].SrcBlend = LynxEngine::GraphicsSystem::CRenderer::BF_SRC_ALPHA;
					BDesc.RTBlendParam[0].DestBlend = LynxEngine::GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
					BDesc.RTBlendParam[0].BlendOp = LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(1).CreateBlendState(BDesc);
					RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
					Technique->GetPass(1).CreateRasterizerState(RDesc);
					Technique->GetPass(1).vCreateShaderClasses("VoxelSpriteOneLightingVS", "ColorPS");
					//Technique->GetPass(1).vCreateShaderClasses("VoxelSpriteVS", "ColorPS");
					if (Technique->GetPass(1).GetShaderGroup())
						Technique->GetPass(1).GetShaderGroup()->AddVertexLayout(m_SpriteVertexLayout);
				}
#endif								                			
			}
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
		}

		if (!m_SharedVertexArray)
		{
			m_lpEngine->CreateResource(m_SharedVertexArray, VERTEX_ARRAY);				
			m_SharedVertexArray->SetName(_T("CVoxelClusterContainer.SharedVertexArray"));
			m_lpEngine->CreateResource(m_SharedIndexArray, INDEX_ARRAY);				
			m_SharedIndexArray->SetName(_T("CVoxelClusterContainer.SharedIndexArray"));

			int NumInstance = m_BatchSize;
			GraphicsSystem::VOXELCLUSTERVERTEX* lpV = LYNXGLOBAL_NEW GraphicsSystem::VOXELCLUSTERVERTEX[NumInstance*24];
			for (int i=0; i<NumInstance; i++)
			{			
				// face +Z	
				lpV[i*24 + 0].Pos.x =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 0].Pos.y =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 0].Pos.z = VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 1].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 1].Pos.y =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 1].Pos.z = VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 2].Pos.x =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 2].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 2].Pos.z = VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 3].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 3].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 3].Pos.z = VOXEL_UNIT *  VOXEL_HALFSIZE;			
				// face -Z
				lpV[i*24 + 4].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 4].Pos.y =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 4].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 5].Pos.x =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 5].Pos.y =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 5].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 6].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 6].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 6].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 7].Pos.x =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 7].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 7].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;			
				// face +X
				lpV[i*24 +  8].Pos.x = VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 +  8].Pos.y =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 +  8].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 +  9].Pos.x = VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 +  9].Pos.y =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 +  9].Pos.z =  VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 10].Pos.x = VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 10].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 10].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 11].Pos.x = VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 11].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 11].Pos.z =  VOXEL_UNIT *  VOXEL_HALFSIZE;			
				// face -X
				lpV[i*24 + 12].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 12].Pos.y =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 12].Pos.z =  VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 13].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 13].Pos.y =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 13].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 14].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 14].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 14].Pos.z =  VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 15].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 15].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 15].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;			
				// face +Y	
				lpV[i*24 + 16].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 16].Pos.y = VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 16].Pos.z =  VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 17].Pos.x =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 17].Pos.y = VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 17].Pos.z =  VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 18].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 18].Pos.y = VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 18].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 19].Pos.x =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 19].Pos.y = VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 19].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;			
				// face -Y
				lpV[i*24 + 20].Pos.x =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 20].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 20].Pos.z =  VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 21].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 21].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 21].Pos.z =  VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 22].Pos.x =  VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 22].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 22].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;
				lpV[i*24 + 23].Pos.x = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 23].Pos.y = -VOXEL_UNIT *  VOXEL_HALFSIZE;	lpV[i*24 + 23].Pos.z = -VOXEL_UNIT *  VOXEL_HALFSIZE;							

				for (int f=0; f<24; f++)
				{
					lpV[i*24 + f].Instance = (float)i;															
				}

				for (int f=0; f<6; f++)
				{
					Math::CVector3 D1, D2, Normal;
	                
					D1 = Math::CVector3(lpV[i*24 + f*4 + 1].Pos) - Math::CVector3(lpV[i*24 + f*4 + 0].Pos);
					D2 = Math::CVector3(lpV[i*24 + f*4 + 2].Pos) - Math::CVector3(lpV[i*24 + f*4 + 0].Pos);
	                
					Normal = Math::Cross(D1, D2);
					Normal.Normalise();
					Desc->m_LightDir.Normalise();
	                
					float Shading = LYNX_MIN(1.0f, (LYNX_MAX(Math::Dot(Normal, Desc->m_LightDir), 0.0f) + 0.3f));
	                
					for (int v=0; v<4; v++)
					{
						lpV[i*24 + f*4 + v].Normal.x = Normal.x;
						lpV[i*24 + f*4 + v].Normal.y = Normal.y;
						lpV[i*24 + f*4 + v].Normal.z = Normal.z;						
					}
					#if __FAKE_VOXEL_AO__
						float SubPixelX = 4.0f / 1024.0f;
						float SubPixelY = 4.0f / 128.0f;
						lpV[i*24 + f*4 + 0].TexCoord.u = 0.0f/16.0f + SubPixelX;	lpV[i*24 + f*4 + 0].TexCoord.v = 0.0f + SubPixelY;
						lpV[i*24 + f*4 + 1].TexCoord.u = 1.0f/16.0f - SubPixelX;	lpV[i*24 + f*4 + 1].TexCoord.v = 0.0f + SubPixelY;
						lpV[i*24 + f*4 + 2].TexCoord.u = 0.0f/16.0f + SubPixelX;	lpV[i*24 + f*4 + 2].TexCoord.v = (1.0f/2.0f) - SubPixelY;
						lpV[i*24 + f*4 + 3].TexCoord.u = 1.0f/16.0f - SubPixelX;	lpV[i*24 + f*4 + 3].TexCoord.v = (1.0f/2.0f) - SubPixelY;

						lpV[i*24 + f*4 + 0].FaceID = f;
						lpV[i*24 + f*4 + 1].FaceID = f;
						lpV[i*24 + f*4 + 2].FaceID = f;
						lpV[i*24 + f*4 + 3].FaceID = f;
					#endif
				}
			}		
			m_SharedVertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_VOXEL_CLUSTER)), NumInstance*24, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpV);		
			LYNXGLOBAL_DEL_ARRAY(lpV);						
			
			int NumI = NumInstance*36;
			int CurrentIndex = 0, BaseIndex = 0;
			WORD* lpI = LYNXGLOBAL_NEW WORD[NumI];
			for (int i=0; i<NumInstance; i++)
			{
				for (int f=0; f<6; f++)
				{
					lpI[CurrentIndex++] = BaseIndex + 0; 
					lpI[CurrentIndex++] = BaseIndex + 1;
					lpI[CurrentIndex++] = BaseIndex + 2;

					lpI[CurrentIndex++] = BaseIndex + 1; 
					lpI[CurrentIndex++] = BaseIndex + 3;
					lpI[CurrentIndex++] = BaseIndex + 2;

					BaseIndex += 4;
				}
			}
			m_SharedIndexArray->vCreate(sizeof(WORD), NumI, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpI);
			LYNXGLOBAL_DEL_ARRAY(lpI);	
		}

		//if (Desc->m_bSpriteRender || m_lpEngine->GetlpGraphicsSystem()->m_Config.bCreateVoxelSpriteSource)
		{
			if (!m_SharedSpriteVertexArray)
			{
				m_lpEngine->CreateResource(m_SharedSpriteVertexArray, VERTEX_ARRAY);				
				m_SharedVertexArray->SetName(_T("CVoxelSpritContainer.SharedVertexArray"));
				m_lpEngine->CreateResource(m_SharedSpriteIndexArray, INDEX_ARRAY);				
				m_SharedIndexArray->SetName(_T("CVoxelSpriteContainer.SharedIndexArray"));

				int NumV = 4;
				int CurrentV = 0;
                int NumSprites = m_SpriteBatchSize;
				GraphicsSystem::SPRITE3DCLUSTERVERTEX* lpV = LYNXGLOBAL_NEW GraphicsSystem::SPRITE3DCLUSTERVERTEX[NumSprites*NumV];
				for (int i=0; i<NumSprites; i++)
				{
					for (int y=0; y<2; y++)
					{
						for (int x=0; x<2; x++)
						{
							lpV[CurrentV].TexCoord.u = x * 1.0f;		
							lpV[CurrentV].TexCoord.v = y * 1.0f; 					
							lpV[CurrentV].Instance = (float)i;
							CurrentV++;
						}
					}
				}
				m_SharedSpriteVertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE3D_CLUSTER)), NumSprites*NumV, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpV);		
				LYNXGLOBAL_DEL_ARRAY(lpV);		
		
				int NumI = 6;
				int CurrentIndex = 0;
				int VertexBase;
				WORD* lpI = LYNXGLOBAL_NEW WORD[NumSprites*NumI];
				for (int i=0; i<NumSprites; i++)
				{
					VertexBase = i * 4;
					lpI[CurrentIndex++] = VertexBase + 0;		lpI[CurrentIndex++] = VertexBase + 1;		lpI[CurrentIndex++] = VertexBase + 2; 
					lpI[CurrentIndex++] = VertexBase + 2;		lpI[CurrentIndex++] = VertexBase + 1;		lpI[CurrentIndex++] = VertexBase + 3; 
				}
				m_SharedSpriteIndexArray->vCreate(sizeof(WORD), NumSprites*NumI, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpI);
				LYNXGLOBAL_DEL_ARRAY(lpI);
			}
		}	

		m_VertexArray = m_SharedVertexArray;
		m_OptimizedVoxelIndexArray = m_SharedIndexArray;	
		
		CContainer::vCreate(desc);

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
	DWORD CVoxelClusterContainer::GetVXLDimension(CStream* lpstream, const CString& filename)
	{
        DWORD Dimension = 0;
        
		if (lpstream)
		{
			if (!lpstream->vOpen(filename, CStream::Binary|CStream::Read))
                return Dimension;
        }
        lpstream->vRead(&Dimension, sizeof(DWORD), 1);
        lpstream->vClose();
        
        return Dimension;
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
	DWORD CVoxelClusterContainer::GetVXLVoxelAmount(CStream* lpstream, const CString& filename, LYNXBOOL boccluded)
	{
		DWORD OptimalSize, OccluedSize, Amount, DisplayListSize, NumBatchs;
        
		Amount = 0;
		if (lpstream)
		{
			if (!lpstream->vOpen(filename, CStream::Binary|CStream::Read))
                return Amount;
        }

		lpstream->vSeek(sizeof(DWORD), CStream::SeekCur);								
		// read in display list 				
		lpstream->vRead(&DisplayListSize, sizeof(DWORD), 1);
		lpstream->vSeek(sizeof(DWORD)*DisplayListSize, CStream::SeekCur);										
		// read in batch size list 			
		lpstream->vRead(&NumBatchs, sizeof(DWORD), 1);
		lpstream->vSeek(sizeof(DWORD)*NumBatchs, CStream::SeekCur);						
							
		// read in optimized voxel
		lpstream->vRead(&OptimalSize, sizeof(DWORD), 1);
		Amount = OptimalSize;					

		if (boccluded)
		{
			lpstream->vSeek(OptimalSize*(sizeof(int)*3+sizeof(LYNXCOLORRGBA)), CStream::SeekCur);
			lpstream->vRead(&OccluedSize, sizeof(DWORD), 1);
			Amount += OccluedSize;			
		}
        
        lpstream->vClose();
        
        return Amount;
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
	LYNXBOOL CVoxelClusterContainer::LoadVXL(CStream* lpstream, const CString& filename, CVoxelClusterCreationDesc* desc)
	{
		int Version = 100;
		LYNXANSICHAR Marker[4];
		int NumVoxelData = desc->m_NumVoxelData;
		if (NumVoxelData <= 1)
			NumVoxelData = 1;

		//CMemoryStream OutStream;

		if (lpstream)
		{
			m_ClusterSize.resize(NumVoxelData);
			m_RenderedSize.resize(NumVoxelData);
			m_OccludedSize.resize(NumVoxelData);

			m_VoxelArray = CCountedPtr<VOXELARRAY>(LYNXNEW VOXELARRAY);
			m_VoxelArray->PosArray.resize(NumVoxelData);
			m_VoxelArray->ColorArray.resize(NumVoxelData);
			m_VoxelArray->ColorIndexArray.resize(NumVoxelData);
			m_VoxelArray->AOCodeArray.resize(NumVoxelData);

			m_GlowVoxelData.resize(NumVoxelData);

			DWORD Size, OptimalSize, OccluedSize;

			for (int v=0; v<NumVoxelData; v++)
			{
				Version = 100;

				m_CurrentVoxelArray = v;

				if (NumVoxelData > 1)
				{			
					/*
					CMemoryStream Stream;
					uLongf Size;					
					if (Stream.vOpen(desc->m_SeqFilename[v]+CString(".z"), CStream::Binary|CStream::Read))
					{
						LYNXDWORD UnzipSize;
						Stream.vRead(&UnzipSize, sizeof(LYNXDWORD), 1);
						Size = UnzipSize;
						BYTE* Dest = LYNXNEW LYNXBYTE[Size];
						BYTE* Src = (BYTE*)Stream.GetCurrentPtr();
						uncompress(Dest, &Size, (const Bytef *)Src, Stream.vSize()-sizeof(LYNXDWORD));												
						OutStream.Set(Dest, Size);
						lpstream->vClose();
						lpstream = &OutStream;
						Stream.vClose();
					}
					else
					{
						if (!lpstream->vOpen(desc->m_SeqFilename[v] , CStream::Binary|CStream::Read))
							return LYNX_FALSE;
					}
					*/					
					
					PACKAGE_OPEN_FILE(lpstream, desc->m_SeqFilename[v], CStream::Read | CStream::Binary)
					if (!bResult)
					{
						CString Message = CString("Can't load ") + desc->m_SeqFilename[v] + CString(" gear object!!");
						glpKernel->vOSN_OpenMessageBox(_T("Error"), Message);
						return LYNX_FALSE;
					}					
				}			
				else
				{
					/*
                    CMemoryStream Stream;
                    uLongf Size;
                    if (Stream.vOpen(filename + CString(".z"), CStream::Binary | CStream::Read))
                    {
                        CPlatformFileStream VXLFile;
                        LYNXDWORD UnzipSize;
                        Stream.vRead(&UnzipSize, sizeof(LYNXDWORD), 1);
                        Size = UnzipSize;
                        BYTE* Dest = LYNXNEW LYNXBYTE[Size];
                        BYTE* Src = (BYTE*)Stream.GetCurrentPtr();
                        uncompress(Dest, &Size, (const Bytef *)Src, Stream.vSize() - sizeof(LYNXDWORD));
                        OutStream.Set(Dest, Size);					
                        
                        lpstream = &OutStream;
                        Stream.vClose();
                    }
                    else
                    {
                        if (!lpstream->vOpen(filename, CStream::Binary | CStream::Read))
                            return LYNX_FALSE;
                    }
					*/
					PACKAGE_OPEN_FILE(lpstream, filename, CStream::Read | CStream::Binary)
					if (!bResult)
					{
						CString Message = CString("Can't load ") + filename + CString(" gear object!!");
						glpKernel->vOSN_OpenMessageBox(_T("Error"), Message);

						return LYNX_FALSE;
					}
				}

				lpstream->vRead(Marker, sizeof(LYNXANSICHAR), 3);
				Marker[3] = NULL;
				if (!lynxStriCmp(Marker, "VXL"))
				{
					lpstream->vRead(&Version, sizeof(int), 1);
				}
				else
				{
					lpstream->vSeek(0, CStream::SeekSet);
				}
				lpstream->vRead(&m_Dimension, sizeof(DWORD), 1);

				// read in display list 				
				lpstream->vRead(&m_DisplayListSize, sizeof(DWORD), 1);
				lpstream->vSeek(sizeof(DWORD)*m_DisplayListSize, CStream::SeekCur);
				//DWORD* lpI = LYNXGLOBAL_NEW DWORD[m_DisplayListSize];
				//lpstream->vRead(lpI, sizeof(DWORD), m_DisplayListSize);			
				//m_lpEngine->CreateResource(m_DisplayIndexArray, INDEX_ARRAY);				
				//m_DisplayIndexArray->vCreate(sizeof(DWORD), m_DisplayListSize, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpI);
				//LYNXGLOBAL_DEL_ARRAY(lpI);

				// read in batch size list 			
				lpstream->vRead(&m_NumBatchs, sizeof(DWORD), 1);
				lpstream->vSeek(sizeof(DWORD)*m_NumBatchs, CStream::SeekCur);						
							
				// read in optimized voxel
				LYNXINT32 iPos[3];
				Math::CVector3 Pos;
				LYNXCOLORRGBA Color;
				lpstream->vRead(&OptimalSize, sizeof(DWORD), 1);
				Size = OptimalSize;
				if (desc->m_bReadOccludedVoxels)
				{
					int SeekOffset = 0;
					if (Version >= 200)
						SeekOffset = (OptimalSize*(sizeof(int)*3+sizeof(LYNXCOLORRGBA)+sizeof(int)));
					else
						SeekOffset = (OptimalSize*(sizeof(int)*3+sizeof(LYNXCOLORRGBA)));

					lpstream->vSeek(SeekOffset, CStream::SeekCur);
					lpstream->vRead(&OccluedSize, sizeof(DWORD), 1);
					Size = OptimalSize + OccluedSize;
					SeekOffset += sizeof(DWORD);
					lpstream->vSeek(-SeekOffset, CStream::SeekCur);
					m_OccludedSize[v] = OccluedSize;
				}
				else
				{
					m_OccludedSize[v] = 0;					
				}

				if (desc->m_bOverwriteClusterSize)  // Use the actual size from file instead of the size in desc
				{
					m_ClusterSize[v] = Size;					
					m_VoxelArray->PosArray[v] = LYNXGLOBAL_NEW LYNXVECTOR3D[m_ClusterSize[v]];				
					m_VoxelArray->ColorArray[v] = LYNXGLOBAL_NEW float[m_ClusterSize[v]*4];
					m_VoxelArray->ColorIndexArray[v] = LYNXGLOBAL_NEW int[m_ClusterSize[v]];
					#if __FAKE_VOXEL_AO__					
						/*
						m_AOOccluder.resize(m_ClusterSize);
						for (int c=0; c<m_ClusterSize; c++)
						{
							m_AOOccluder[c].resize(12);
							for (int o=0; o<12; o++)
								m_AOOccluder[c][o] = LYNX_FALSE;
						}
						*/
						#if defined (__DESKTOP__) && !defined(__OGLES2__)
							m_VoxelArray->AOCodeArray[v] = LYNXGLOBAL_NEW DWORD[m_ClusterSize[v]*2];
							for (int c=0; c<m_ClusterSize[v]; c++)
							{
								m_VoxelArray->AOCodeArray[v][c*2+0] = 0;
								m_VoxelArray->AOCodeArray[v][c*2+1] = 0;								
							}
						#else
							m_VoxelArray->AOCodeArray[v] = LYNXNEW DWORD[m_ClusterSize[v]*4];
							for (int c=0; c<m_ClusterSize[v]; c++)
							{
								m_VoxelArray->AOCodeArray[v][c*4+0] = 0;
								m_VoxelArray->AOCodeArray[v][c*4+1] = 0;
								m_VoxelArray->AOCodeArray[v][c*4+2] = 0;
								m_VoxelArray->AOCodeArray[v][c*4+3] = 0;
							}
						#endif
					#endif
				}
				else
				{
					m_ClusterSize[v] = desc->m_ClusterSize;					
					m_VoxelArray->PosArray[v] = LYNXGLOBAL_NEW LYNXVECTOR3D[m_ClusterSize[v]];				
					m_VoxelArray->ColorArray[v] = LYNXGLOBAL_NEW float[m_ClusterSize[v]*4];
					m_VoxelArray->ColorIndexArray[v] = LYNXGLOBAL_NEW int[m_ClusterSize[v]];
					#if __FAKE_VOXEL_AO__
						/*
						m_AOOccluder.resize(m_ClusterSize);
						for (int c=0; c<m_ClusterSize; c++)
						{
							m_AOOccluder[c].resize(12);
							for (int o=0; o<12; o++)
								m_AOOccluder[c][o] = LYNX_FALSE;
						}
						*/
						#if defined (__DESKTOP__) && !defined(__OGLES2__)
							m_VoxelArray->AOCodeArray[v] = LYNXGLOBAL_NEW DWORD[m_ClusterSize[v]*2];
							for (int c=0; c<m_ClusterSize[v]; c++)
							{
								m_VoxelArray->AOCodeArray[v][c*2+0] = 0;
								m_VoxelArray->AOCodeArray[v][c*2+1] = 0;
							}
						#else
							m_VoxelArray->AOCodeArray[v] = LYNXNEW DWORD[m_ClusterSize[v]*4];
							for (int c=0; c<m_ClusterSize[v]; c++)
							{
								m_VoxelArray->AOCodeArray[v][c*4+0] = 0;
								m_VoxelArray->AOCodeArray[v][c*4+1] = 0;
								m_VoxelArray->AOCodeArray[v][c*4+2] = 0;
								m_VoxelArray->AOCodeArray[v][c*4+3] = 0;
							}
						#endif
					#endif

					for (int i=0; i<m_ClusterSize[v]; i++)
					{
						m_VoxelArray->ColorArray[v][i*4] = m_VoxelArray->ColorArray[v][i*4+1] = m_VoxelArray->ColorArray[v][i*4+2] = m_VoxelArray->ColorArray[v][i*4+3] = 1.0f;
						m_VoxelArray->PosArray[v][i].x = m_VoxelArray->PosArray[v][i].y = m_VoxelArray->PosArray[v][i].z = 0.0f;			
						m_VoxelArray->ColorIndexArray[v][i] = -1;
					}
				}			
				m_RenderedSize[v] = Size;				

				int ColorIndex = -1;
				Math::CVector3 CPos;
				Math::CVector3 Min(FLT_MAX, FLT_MAX, FLT_MAX);
				Math::CVector3 Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

				float Center = ((float)m_Dimension - 1) * 0.5f;
				if (OptimalSize > 0)
				{
					for (int i=0; i<OptimalSize; i++)
					{
						ColorIndex = -1;
						lpstream->vRead(iPos, sizeof(LYNXINT32), 3);
						lpstream->vRead(&Color, sizeof(LYNXCOLORRGBA), 1);						
						if (Version >= 200)
						{
							lpstream->vRead(&ColorIndex, sizeof(int), 1);														
						}

						Pos.x = (iPos[0] - Center);	
						Pos.y = Center - iPos[1];
						Pos.z = (iPos[2] - Center);                    
                    
						// fix the coordinate system error......
						CPos = Math::CVector3(-Pos.x*VOXEL_UNIT*m_VoxelScale[v], Pos.y*VOXEL_UNIT*m_VoxelScale[v], -Pos.z*VOXEL_UNIT*m_VoxelScale[v]);

						if (i == 0)
						{
							Max = CPos;
							Min = CPos;
						}

						if (CPos.x > Max.x)
							Max.x = CPos.x;
						else if (CPos.x < Min.x)
							Min.x = CPos.x;

						if (CPos.y > Max.y)
							Max.y = CPos.y;
						else if (CPos.y < Min.y)
							Min.y = CPos.y;

						if (CPos.z > Max.z)
							Max.z = CPos.z;
						else if (CPos.z < Min.z)
							Min.z = CPos.z;
					
						vSetPosition(i, Pos*VOXEL_UNIT*m_VoxelScale[v]);
						SetColor(i, &Color);
						SetColorIndex(i, ColorIndex);
					}
					m_Center = (Max + Min) * 0.5f;
					Min -= CVector3(VOXEL_HALFSIZE*VOXEL_UNIT*m_VoxelScale[v], VOXEL_HALFSIZE*VOXEL_UNIT*m_VoxelScale[v], VOXEL_HALFSIZE*VOXEL_UNIT*m_VoxelScale[v]);
					Max += CVector3(VOXEL_HALFSIZE*VOXEL_UNIT*m_VoxelScale[v], VOXEL_HALFSIZE*VOXEL_UNIT*m_VoxelScale[v], VOXEL_HALFSIZE*VOXEL_UNIT*m_VoxelScale[v]);
				}
				else
				{
					Min = CVector3(-0.001f);
					Max += CVector3(0.001f);
				}
                
				// Setup collision Hierarchy
				m_lpCollisionHierarchy->Create(3);		

				m_LocalAABB.Min = Min;
				m_LocalAABB.Max = Max;

				CollisionSystem::CBtOBB* Box = LYNXNEW CollisionSystem::CBtOBB();
                Box->SetByBoundary(Min, Max);
				m_lpCollisionHierarchy->Add(COT_OBB, Box);
				Box->SetParentContainer(this);
				Box->m_bSkipTransfomScale = LYNX_FALSE;

				float Height, Radius;
				Math::CReal Deg = 90.0f;
				Math::CMatrix4 OffsetM;

				CVector3 CapsuleCenter = (Max+Min)*0.5f; 		
				CVector3 Dir = Max - Min;
				if (Dir.y >= Dir.z && Dir.y >= Dir.x)
				{
					Height = Dir.y;
					Dir.y = 0.0f;
					Radius = Dir.Length(3) * 0.5f;	
					OffsetM.Identity();					
				}
				else if (Dir.z >= Dir.x && Dir.z >= Dir.y)
				{
					Height = Dir.z;
					Dir.z = 0.0f;
					Radius = Dir.Length(3) * 0.5f;					
					MakeRotateMatrix(OffsetM, Deg, LYNX_X_AXIS);
				}				
				else if (Dir.x >= Dir.y && Dir.x >= Dir.z)
				{
					Height = Dir.x;
					Dir.x = 0.0f;
					Radius = Dir.Length(3) * 0.5f;
					MakeRotateMatrix(OffsetM, Deg, LYNX_Z_AXIS);
				}
				Height -= (Radius*2.0f);
				if (Height < 0.0f)
					Height = 0.0f;
				CollisionSystem::CBtCapsule* Cap = LYNXNEW CollisionSystem::CBtCapsule(CapsuleCenter, Radius, Height);
                m_lpCollisionHierarchy->Add(COT_CAPSULE, Cap);
				Cap->SetParentContainer(this);
				Cap->m_bSkipTransfomScale = LYNX_FALSE;
				Cap->SetOffsetM(OffsetM);

				CollisionSystem::CBtSphere* Sphere = LYNXNEW CollisionSystem::CBtSphere(CapsuleCenter, Radius);
				m_lpCollisionHierarchy->Add(COT_SPHERE, Sphere);
				Sphere->SetParentContainer(this);
				Sphere->m_bSkipTransfomScale = LYNX_FALSE;
								
				if (desc->m_bReadOccludedVoxels)
				{
					lpstream->vRead(&OccluedSize, sizeof(DWORD), 1);
					for (int i=0; i<OccluedSize; i++)
					{
						ColorIndex = -1;
						lpstream->vRead(iPos, sizeof(int), 3);
						lpstream->vRead(&Color, sizeof(LYNXCOLORRGBA), 1);
						if (Version >= 200)
						{
							lpstream->vRead(&ColorIndex, sizeof(int), 1);							
						}

						Pos.x = iPos[0] - Center;
						Pos.y = Center - iPos[1];
						Pos.z = iPos[2] - Center;

						vSetPosition(OptimalSize+i, Pos*VOXEL_UNIT*m_VoxelScale[v]);
						SetColor(OptimalSize+i, &Color);
						SetColorIndex(OptimalSize+i, ColorIndex);
					}
				}
				else
				{
					lpstream->vRead(&OccluedSize, sizeof(DWORD), 1);
					int SeekSize = OccluedSize*(sizeof(int)*3+sizeof(LYNXCOLORRGBA));
					if (Version >= 200)
					{
						SeekSize += OccluedSize*(sizeof(int));
					}
					lpstream->vSeek(SeekSize, CStream::SeekCur);									
				}

				#if __FAKE_VOXEL_AO__
					LYNXBOOL bAOCode = LYNX_FALSE;
					DWORD AOCode = 0;
					if (lpstream->vRead(&bAOCode, sizeof(LYNXBOOL), 1))
					{
						if (bAOCode)
						{
							for (int i=0; i<OptimalSize; i++)
							{			
								lpstream->vRead(&AOCode, sizeof(DWORD), 1);	
								SetAOCode(i, AOCode);
							}

							if (lpstream->vTell() != lpstream->vSize())
							{						
								for (int i=0; i<OptimalSize; i++)
								{			
									lpstream->vRead(&AOCode, sizeof(DWORD), 1);	
									SetAOCode2(i, AOCode);
								}
							}
						}					
					}
				#endif
				lpstream->vClose();

				if (desc)
				{
					if (desc->m_Dimension > 0)
					{
						m_Dimension = desc->m_Dimension;
					}
				}
			}
		}	
		m_CurrentVoxelArray = 0;

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
	Animation::CVoxelAnimation* CVoxelClusterContainer::AddVoxelAnimation()
	{
		Animation::CVoxelAnimation* Ani = LYNXNEW Animation::CVoxelAnimation(m_lpEngine);
		m_VoxelAnimationArray.push_back(Ani);
		return Ani;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param	voxel : the voxel number you want to get the voxel animation
	*	@return : the cooresponding voxel animation pointer or NULL if the voxel doesn't have voxel animation。 
	*/
	//-------------------------------------------------------------------------------------------------------	
	Animation::CVoxelAnimation* CVoxelClusterContainer::GetVoxelAnimationByVoxel(int voxel)
	{
		Animation::CVoxelAnimation Voxel(m_lpEngine);
		Voxel.SetVoxel(voxel);
		CArray<Animation::CVoxelAnimation*>::CIterator Ani;

		Ani = LynxEngine::find(m_VoxelAnimationArray.begin(), m_VoxelAnimationArray.end(), Voxel);
		if (Ani == m_VoxelAnimationArray.end())
			return NULL;

		return (*Ani);
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
	void CVoxelClusterContainer::SetRenderedSize(int size) 
	{
		if (size <= m_ClusterSize[m_CurrentVoxelArray]) 
		{
			m_RenderedSize[m_CurrentVoxelArray] = size; 
		}
		else
		{
			int OldSize = m_RenderedSize[m_CurrentVoxelArray];

			m_RenderedSize[m_CurrentVoxelArray] = size; 
			m_ClusterSize[m_CurrentVoxelArray] = size; 

			LYNXVECTOR3D* OldPosArray = m_VoxelArray->PosArray[m_CurrentVoxelArray];			
			m_VoxelArray->PosArray[m_CurrentVoxelArray] = LYNXGLOBAL_NEW LYNXVECTOR3D[size];
			lynxMemCpy(m_VoxelArray->PosArray[m_CurrentVoxelArray], OldPosArray, sizeof(LYNXVECTOR3D)*OldSize);			
			LYNXGLOBAL_DEL_ARRAY(OldPosArray);

			float* OldColorArray = m_VoxelArray->ColorArray[m_CurrentVoxelArray];			
			m_VoxelArray->ColorArray[m_CurrentVoxelArray] = LYNXGLOBAL_NEW float[size*4];
			lynxMemCpy(m_VoxelArray->ColorArray[m_CurrentVoxelArray], OldColorArray, sizeof(float)*OldSize*4);			
			LYNXGLOBAL_DEL_ARRAY(OldColorArray);

			int* OldColorIndexArray = m_VoxelArray->ColorIndexArray[m_CurrentVoxelArray];			
			m_VoxelArray->ColorIndexArray[m_CurrentVoxelArray] = LYNXGLOBAL_NEW int[size];
			lynxMemCpy(m_VoxelArray->ColorIndexArray[m_CurrentVoxelArray], OldColorIndexArray, sizeof(int)*OldSize);			
			LYNXGLOBAL_DEL_ARRAY(OldColorIndexArray);

			DWORD* OldAOCodeArray = m_VoxelArray->AOCodeArray[m_CurrentVoxelArray];			
			m_VoxelArray->AOCodeArray[m_CurrentVoxelArray] = LYNXGLOBAL_NEW DWORD[size*2];
			lynxMemCpy(m_VoxelArray->AOCodeArray[m_CurrentVoxelArray], OldAOCodeArray, sizeof(DWORD)*OldSize*2);			
			LYNXGLOBAL_DEL_ARRAY(OldAOCodeArray);
		}
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
	int	CVoxelClusterContainer::GetMaxRenderedSize() const
	{
		int Max = 0;
		for (int i=0; i>m_RenderedSize.size(); i++)
		{
			if (m_RenderedSize[i] > Max)
				Max = m_RenderedSize[i];
		}
		return Max;
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
	void CVoxelClusterContainer::vSetPosition(const int index, const CVector3& pos)
	{	
		m_VoxelArray->PosArray[m_CurrentVoxelArray][index].x = pos.x;
		m_VoxelArray->PosArray[m_CurrentVoxelArray][index].y = pos.y;
		m_VoxelArray->PosArray[m_CurrentVoxelArray][index].z = pos.z;
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
	void CVoxelClusterContainer::vGetPosition(const int index, Math::CVector3& pos) const
	{
		pos.x = m_VoxelArray->PosArray[m_CurrentVoxelArray][index].x;
		pos.y = m_VoxelArray->PosArray[m_CurrentVoxelArray][index].y;
		pos.z = m_VoxelArray->PosArray[m_CurrentVoxelArray][index].z;
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
	void CVoxelClusterContainer::SetColorAlpha(LYNXBYTE a)
	{	
		for (int i=0; i>m_RenderedSize[m_CurrentVoxelArray]; i++)
		{
			m_VoxelArray->ColorArray[m_CurrentVoxelArray][i*4+3] = (float)(a/255.0f);		
		}
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
	void CVoxelClusterContainer::SetColor(LPLYNXCOLORRGBA lpc)
	{	
		for (int i=0; i>m_RenderedSize[m_CurrentVoxelArray]; i++)
		{
			m_VoxelArray->ColorArray[m_CurrentVoxelArray][i*4+0] = (float)lpc->Red/255.0f;
			m_VoxelArray->ColorArray[m_CurrentVoxelArray][i*4+1] = (float)lpc->Green/255.0f;
			m_VoxelArray->ColorArray[m_CurrentVoxelArray][i*4+2] = (float)lpc->Blue/255.0f;
			m_VoxelArray->ColorArray[m_CurrentVoxelArray][i*4+3] = (float)lpc->Alpha/255.0f;		
		}
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
	void CVoxelClusterContainer::SetColor(const int index, LPLYNXCOLORRGBA lpc)
	{	
		m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+0] = (float)lpc->Red/255.0f;
		m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+1] = (float)lpc->Green/255.0f;
		m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+2] = (float)lpc->Blue/255.0f;
		m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+3] = (float)lpc->Alpha/255.0f;		
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
	void CVoxelClusterContainer::SetColorAlpha(const int index, LYNXBYTE a)
	{	
		m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+3] = (float)a/255.0f;		
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
	void CVoxelClusterContainer::GetColor(const int index, LPLYNXCOLORRGBA lpc) const
	{	
		lpc->Red	= m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+0] * 255;
		lpc->Green	= m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+1] * 255;
		lpc->Blue	= m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+2] * 255;
		lpc->Alpha	= m_VoxelArray->ColorArray[m_CurrentVoxelArray][index*4+3] * 255;		
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
	void CVoxelClusterContainer::SetColorIndex(const int v, const int index)
	{			
		m_VoxelArray->ColorIndexArray[m_CurrentVoxelArray][v] = index;		
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
	void CVoxelClusterContainer::SetAOCode(int index, DWORD code)
	{	
		#if __FAKE_VOXEL_AO__
			#if defined (__DESKTOP__) && !defined(__OGLES2__)
				m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*2+0] = code;		
				//m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*4+0] = code&0xfff;
				//m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*4+1] = (code>>12);
			#else
				m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*4+0] = code&0xfff;
				m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*4+1] = ((code>>12)&0xfff);
			#endif
        #endif
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
	void CVoxelClusterContainer::SetAOCode2(int index, DWORD code)				
	{	
		#if __FAKE_VOXEL_AO__
			#if defined (__DESKTOP__) && !defined(__OGLES2__)
				m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*2+1] = code;
				//m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*4+2] = code&0xfff;
				//m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*4+3] = code>>12;	
			#else
				m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*4+2] = code&0xfff;
				m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][index*4+3] = ((code>>12)&0xfff);
			#endif
		#endif
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
	LYNXBOOL CVoxelClusterContainer::vPlay(CReal step)
	{		
		LYNXBOOL bStop = LYNX_FALSE;
		static Math::CReal Values[4];

		CContainer::vPlay(step);			
			
		SaveTransformationMatrix();

		//m_CurrentVoxelArray = 0;

		if (!m_lpParentContainer && m_AnimationSet->GetCurrentAnimation())
		{
			bStop = m_AnimationSet->GetCurrentAnimation()->Play(step);		
			m_AnimationSet->GetCurrentAnimation()->UpdateTransform();
			m_M = m_AnimationSet->GetCurrentAnimation()->m_M;
		
			// Use KT_TEXTURE CAnimationKeys for voxel array animation. All voxel array animation data will be stored in 
			// m_VoxelArray then use the m_Time of CAnimation to index to. By this way, the CAnimation can be assigned to 
			// StateMachine::CState as other types of container do.
			if (m_AnimationSet->GetCurrentAnimation()->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE] && m_AnimationSet->GetCurrentAnimation()->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys.size())
			{
				m_AnimationSet->GetCurrentAnimation()->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->Lerp(Values, m_AnimationSet->GetCurrentAnimation()->m_Time);
				m_CurrentVoxelArray = Values[0].Round();
			}
		}

		if (m_VoxelAnimationArray.size())
		{
			m_ColorBuffer = ColorArray();
			m_PositionBuffer = PosArray();

			//if (m_ColorBuffer && m_ColorBuffer != )
			//	GetColorArray(m_ColorBuffer, 0, m_RenderedSize[0]);
			//if (m_PositionBuffer)
			//	GetPosArray(m_PositionBuffer, 0, m_RenderedSize[0]);

			for (int i=0; i<m_VoxelAnimationArray.size(); i++)
			{
				bStop = m_VoxelAnimationArray[i]->Play(step);		
				Animation::CAnimationKeys* Ani = m_VoxelAnimationArray[i]->GetColorAnimationKeys();
				if (Ani && m_ColorBuffer)
				{
					Ani->Lerp((CReal*)(&m_ColorBuffer[m_VoxelAnimationArray[i]->GetVoxel()*4]), m_VoxelAnimationArray[i]->m_Time);
				}			
				Ani = m_VoxelAnimationArray[i]->GetPositionAnimationKeys();
				if (Ani &&  m_PositionBuffer)
				{
					Ani->Lerp((CReal*)(&m_PositionBuffer[m_VoxelAnimationArray[i]->GetVoxel()*3]), m_VoxelAnimationArray[i]->m_Time);
				}
			}

			//if (m_ColorBuffer)
			//	SetColorArray(0, m_RenderedSize[0], m_ColorBuffer);
			//if (m_PositionBuffer)
			//	SetPosArray(0, m_RenderedSize[0], m_PositionBuffer);
		}
			
		return bStop;							
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
	void CVoxelClusterContainer::RenderOccludedVoxels(LYNXBOOL b)
	{
		m_bRenderOccludedVoxels = b;

		for (int i=0; i<m_RenderedSize.size(); i++)
		{
			if (m_bRenderOccludedVoxels)
			{
				m_RenderedSize[i] = m_ClusterSize[i];
			}
			else
			{
				m_RenderedSize[i] = m_ClusterSize[i] - m_OccludedSize[i];
			}
		}
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
	void CVoxelClusterContainer::vRender(CCameraContainer* cam)
	{		
		if (m_bUseVisibilty && m_Visibility <= 0.0f) 
			return;

		if ((m_bSpriteRender || m_lpEngine->GetlpGraphicsSystem()->m_Config.bOverwriteVoxelSpriteRender))
			return SpriteRender(cam);

		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		
		lpR->SetVertexArray(m_VertexArray, 0, 0);	
		lpR->SetIndexArray(m_OptimizedVoxelIndexArray, 0);
		//lpR->SetIndexArray(m_DisplayIndexArray, 0);		
		
		int BatchIndex = 0;
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);        
		
		//for (int p=0; p<Technique.GetNumPass(); ++p)				
		int p = 0;
		{
			Shader::CVoxelClusterVS* lpShader = dynamic_cast<Shader::CVoxelClusterVS*>(&(*(Technique.GetPass(p).GetVS())));
			//lpShader->m_VoxelScale = m_VoxelScale[m_CurrentVoxelArray];

			Technique.GetPass(p).Set(this, cam);
			if (m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene())
				m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();                    
			
			int NumInstance = m_RenderedSize[m_CurrentVoxelArray];			
			int NumRenderedInstance, BaseInstance = 0;
			while (NumInstance > 0)
			{
				NumRenderedInstance = LYNX_MIN(m_BatchSize, NumInstance);
                
				lpShader->GetPositionArrayConstant()->vMap();
					lpShader->GetPositionArrayConstant()->vSetVector3((float*)&(m_VoxelArray->PosArray[m_CurrentVoxelArray][BaseInstance]), NumRenderedInstance);													
					lpShader->GetColorArrayConstant()->vSetVector4((float*)&(m_VoxelArray->ColorArray[m_CurrentVoxelArray][BaseInstance*4]), NumRenderedInstance);													
					#if __FAKE_VOXEL_AO__					
						#if defined (__DESKTOP__) && !defined(__OGLES2__)
							if (lpShader->GetAOCodeArrayConstant())
								lpShader->GetAOCodeArrayConstant()->vSetInt2((int*)&(m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][BaseInstance*2]), NumRenderedInstance);
						#else
							if (lpShader->GetAOCodeArrayConstant())
								lpShader->GetAOCodeArrayConstant()->vSetInt4((int*)&(m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][BaseInstance*4]), NumRenderedInstance);
						#endif
					#endif
				lpShader->GetPositionArrayConstant()->vUnmap();

				lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedInstance*24, 0, NumRenderedInstance*12);				
				
				BaseInstance += NumRenderedInstance;
				NumInstance -= NumRenderedInstance;
			}
		}
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
	void CVoxelClusterContainer::AddGlowVoxels(int v, CArray<int>& list, float scale)
	{
		m_GlowVoxelData[v].NumVoxels = 0;
		if (m_GlowVoxelData[v].VoxelList)
		{
			LYNXGLOBAL_DEL_ARRAY(m_GlowVoxelData[v].VoxelList);
			m_GlowVoxelData[v].VoxelList = NULL;
		}
		if (m_GlowVoxelData[v].VoxelPosArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_GlowVoxelData[v].VoxelPosArray);
			m_GlowVoxelData[v].VoxelPosArray = NULL;
		}
		if (m_GlowVoxelData[v].VoxelColorArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_GlowVoxelData[v].VoxelColorArray);
			m_GlowVoxelData[v].VoxelColorArray = NULL;
		}

		m_GlowVoxelData[v].NumVoxels = list.size();
		m_GlowVoxelData[v].Scale = scale;
		m_GlowVoxelData[v].VoxelList = LYNXGLOBAL_NEW LYNXDWORD[m_GlowVoxelData[v].NumVoxels];
		m_GlowVoxelData[v].VoxelPosArray = LYNXGLOBAL_NEW LYNXVECTOR3D[m_GlowVoxelData[v].NumVoxels];
		m_GlowVoxelData[v].VoxelColorArray = LYNXGLOBAL_NEW float[m_GlowVoxelData[v].NumVoxels*4];
		for (int i=0; i<list.size(); i++)
		{
			m_GlowVoxelData[v].VoxelList[i] = list[i];
			m_GlowVoxelData[v].VoxelPosArray[i] = m_VoxelArray->PosArray[v][list[i]];
			m_GlowVoxelData[v].VoxelColorArray[i*4+0] = m_VoxelArray->ColorArray[v][list[i]*4+0]*scale;
			m_GlowVoxelData[v].VoxelColorArray[i*4+1] = m_VoxelArray->ColorArray[v][list[i]*4+1]*scale;
			m_GlowVoxelData[v].VoxelColorArray[i*4+2] = m_VoxelArray->ColorArray[v][list[i]*4+2]*scale;
			m_GlowVoxelData[v].VoxelColorArray[i*4+3] = m_VoxelArray->ColorArray[v][list[i]*4+3];
		}
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
	void CVoxelClusterContainer::UpdateGlowVoxels(int v)
	{
		for (int i=0; i<m_GlowVoxelData[v].NumVoxels; i++)
		{
			m_GlowVoxelData[v].VoxelPosArray[i] = m_VoxelArray->PosArray[v][m_GlowVoxelData[v].VoxelList[i]];
			m_GlowVoxelData[v].VoxelColorArray[i * 4 + 0] = m_VoxelArray->ColorArray[v][m_GlowVoxelData[v].VoxelList[i] * 4 + 0] * m_GlowVoxelData[v].Scale;
			m_GlowVoxelData[v].VoxelColorArray[i * 4 + 1] = m_VoxelArray->ColorArray[v][m_GlowVoxelData[v].VoxelList[i] * 4 + 1] * m_GlowVoxelData[v].Scale;
			m_GlowVoxelData[v].VoxelColorArray[i * 4 + 2] = m_VoxelArray->ColorArray[v][m_GlowVoxelData[v].VoxelList[i] * 4 + 2] * m_GlowVoxelData[v].Scale;
			m_GlowVoxelData[v].VoxelColorArray[i * 4 + 3] = m_VoxelArray->ColorArray[v][m_GlowVoxelData[v].VoxelList[i] * 4 + 3] * m_GlowVoxelData[v].Scale;
		}
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
	void CVoxelClusterContainer::vGlowRender(CCameraContainer* cam)
	{
		if (m_bUseVisibilty && m_Visibility <= 0.0f)
			return;

		if (m_bGlowDataDirty)
		{
			UpdateGlowVoxels(m_CurrentVoxelArray);
		}

		if (!m_VoxelAnimationArray.empty())
		{
			for (int i = 0; i<m_GlowVoxelData[m_CurrentVoxelArray].NumVoxels; i++)
			{
				int Index = m_GlowVoxelData[m_CurrentVoxelArray].VoxelList[i];
				m_GlowVoxelData[m_CurrentVoxelArray].VoxelPosArray[i] = m_VoxelArray->PosArray[m_CurrentVoxelArray][Index];
				m_GlowVoxelData[m_CurrentVoxelArray].VoxelColorArray[i * 4 + 0] = m_VoxelArray->ColorArray[m_CurrentVoxelArray][Index * 4 + 0] * m_GlowVoxelData[m_CurrentVoxelArray].Scale;
				m_GlowVoxelData[m_CurrentVoxelArray].VoxelColorArray[i * 4 + 1] = m_VoxelArray->ColorArray[m_CurrentVoxelArray][Index * 4 + 1] * m_GlowVoxelData[m_CurrentVoxelArray].Scale;
				m_GlowVoxelData[m_CurrentVoxelArray].VoxelColorArray[i * 4 + 2] = m_VoxelArray->ColorArray[m_CurrentVoxelArray][Index * 4 + 2] * m_GlowVoxelData[m_CurrentVoxelArray].Scale;
				m_GlowVoxelData[m_CurrentVoxelArray].VoxelColorArray[i * 4 + 3] = m_VoxelArray->ColorArray[m_CurrentVoxelArray][Index * 4 + 3];
			}
		}

		if ((m_bSpriteRender || m_lpEngine->GetlpGraphicsSystem()->m_Config.bOverwriteVoxelSpriteRender))
			return SpriteRenderGlowVoxels(cam);

		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();

		lpR->SetVertexArray(m_VertexArray, 0, 0);
		lpR->SetIndexArray(m_OptimizedVoxelIndexArray, 0);

		int BatchIndex = 0;
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE);

		Shader::CVoxelClusterVS* lpShader = dynamic_cast<Shader::CVoxelClusterVS*>(&(*(Technique.GetPass(0).GetVS())));
		//lpShader->m_VoxelScale = m_VoxelScale[m_CurrentVoxelArray];

		Technique.GetPass(0).Set(this, cam);
		int NumInstance = m_GlowVoxelData[m_CurrentVoxelArray].NumVoxels;
		int NumRenderedInstance, BaseInstance = 0;
		while (NumInstance > 0)
		{
			NumRenderedInstance = LYNX_MIN(m_BatchSize, NumInstance);

			lpShader->GetPositionArrayConstant()->vMap();
			lpShader->GetPositionArrayConstant()->vSetVector3((float*)&(m_GlowVoxelData[m_CurrentVoxelArray].VoxelPosArray[BaseInstance]), NumRenderedInstance);
			lpShader->GetColorArrayConstant()->vSetVector4((float*)&(m_GlowVoxelData[m_CurrentVoxelArray].VoxelColorArray[BaseInstance * 4]), NumRenderedInstance);
			lpShader->GetPositionArrayConstant()->vUnmap();

			lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedInstance * 24, 0, NumRenderedInstance * 12);

			BaseInstance += NumRenderedInstance;
			NumInstance -= NumRenderedInstance;
		}
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
	void CVoxelClusterContainer::SpriteRender(CCameraContainer* cam)
	{		
		CVector3 Dir, Pos, Scale;
		CMatrix4 VM, WVM, WVPM;

		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		
		lpR->SetVertexArray(m_SharedSpriteVertexArray, 0, 0);				
		lpR->SetIndexArray(m_SharedSpriteIndexArray, 0);				
		
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);        		
        Shader::CVoxelSpriteVS* lpShader = dynamic_cast<Shader::CVoxelSpriteVS*>(&(*(Technique.GetPass(1).GetVS())));
		lpShader->m_Center = m_Center;

        Technique.GetPass(1).Set(this, cam);
        int NumSprites = m_RenderedSize[m_CurrentVoxelArray];			
        int NumRenderedSprites, BaseSprites = 0;
        while (NumSprites > 0)
        {
            NumRenderedSprites = LYNX_MIN(m_SpriteBatchSize, NumSprites);			
			lpShader->GetPositionArrayConstant()->vSetVector3((float*)&(m_VoxelArray->PosArray[m_CurrentVoxelArray][BaseSprites]), NumRenderedSprites);																
            lpShader->GetColorArrayConstant()->vSetVector4((float*)&(m_VoxelArray->ColorArray[m_CurrentVoxelArray][BaseSprites*4]), NumRenderedSprites);
            lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedSprites*4, 0, NumRenderedSprites*2);
			
            BaseSprites += NumRenderedSprites;
            NumSprites -= NumRenderedSprites;
        }
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
	void CVoxelClusterContainer::SpriteRenderGlowVoxels(CCameraContainer* cam)
	{		
		CVector3 Dir, Pos, Scale;
		CMatrix4 VM, WVM, WVPM;

		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		
		lpR->SetVertexArray(m_SharedSpriteVertexArray, 0, 0);				
		lpR->SetIndexArray(m_SharedSpriteIndexArray, 0);				
		
		//m_M.GetScale(Scale);
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE);
		Shader::CVoxelSpriteVS* lpShader = dynamic_cast<Shader::CVoxelSpriteVS*>(&(*(Technique.GetPass(1).GetVS())));
		lpShader->m_Center = m_Center;

		Technique.GetPass(1).Set(this, cam);
		int NumSprites = m_GlowVoxelData[m_CurrentVoxelArray].NumVoxels;			
		int NumRenderedSprites, BaseSprites = 0;
		while (NumSprites > 0)
		{
			NumRenderedSprites = LYNX_MIN(m_SpriteBatchSize, NumSprites);
			lpShader->GetPositionArrayConstant()->vSetVector3((float*)&(m_GlowVoxelData[m_CurrentVoxelArray].VoxelPosArray[BaseSprites]), NumRenderedSprites);													
			lpShader->GetColorArrayConstant()->vSetVector4((float*)&(m_GlowVoxelData[m_CurrentVoxelArray].VoxelColorArray[BaseSprites*4]), NumRenderedSprites);													
			lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedSprites*4, 0, NumRenderedSprites*2);

			BaseSprites += NumRenderedSprites;
			NumSprites -= NumRenderedSprites;
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CVoxelClusterContainer::WriteOBJCube(CPlatformFileStream& fs, int voxel, int v_base, int nv_base)
	{
		char Buffer[1024];
		float Center = (m_Dimension-1) * 0.5f;	

		// write comment 
		sprintf(Buffer, ("# Voxel-%d\n"), (v_base/8));
		fs.vWrite(CAnsiString(Buffer));
		sprintf(Buffer, ("\n"));
		fs.vWrite(CAnsiString(Buffer));
		
		static float Unit = CVoxelClusterContainer::VOXEL_UNIT * m_VoxelScale[0] * CVoxelClusterContainer::VOXEL_HALFSIZE;

		static LYNXVECTOR3D V[8] = { {-Unit, -Unit,  Unit}, 
									 {-Unit, -Unit, -Unit},
									 { Unit, -Unit, -Unit},
									 { Unit, -Unit,  Unit},
									 {-Unit,  Unit,  Unit}, 
									 { Unit,  Unit,  Unit},
									 { Unit,  Unit, -Unit},
									 {-Unit,  Unit, -Unit},
		};

		static LYNXVECTOR3D NV[6] = { { 0.0f, -1.0f,  0.0f},
									  { 0.0f,  1.0f,  0.0f},
									  { 0.0f,  0.0f,  1.0f}, 
									  { 1.0f,  0.0f,  0.0f},
									  { 0.0f,  0.0f, -1.0f}, 
									  {-1.0f,  0.0f, -0.0f}
		};

		static int Face[6][4] = { { 1, 2, 3, 4},
								  { 5, 6, 7, 8},
								  { 1, 4, 6, 5}, 
								  { 4, 3, 7, 6},
								  { 3, 2, 8, 7}, 
								  { 2, 1, 5, 8}
		};

		static int NFace[6][4] = { { 1, 1, 1, 1},
					 			   { 2, 2, 2, 2},
								   { 3, 3, 3, 3}, 
								   { 4, 4, 4, 4},
								   { 5, 5, 5, 5}, 
								   { 6, 6, 6, 6}
		};
		
		// write vertex
		for (int i=0; i<8; i++)
		{
			sprintf(Buffer, ("v\t%f %f %f\n"), (m_VoxelArray->PosArray[0][voxel].x+V[i].x), (m_VoxelArray->PosArray[0][voxel].z+V[i].z), (m_VoxelArray->PosArray[0][voxel].y+V[i].y));
            //sprintf(Buffer, ("v\t%f %f %f\n"), (m_VoxelArray->PosArray[0][voxel].x+V[i].x), (m_VoxelArray->PosArray[0][voxel].y+V[i].y), (m_VoxelArray->PosArray[0][voxel].z+V[i].z));
            fs.vWrite(CAnsiString(Buffer));	
		}
		sprintf(Buffer, ("\n"));
		fs.vWrite(CAnsiString(Buffer));	

		// write vertex normal
		for (int i=0; i<6; i++)
		{
			sprintf(Buffer, ("vn\t%f %f %f\n"), NV[i].x, NV[i].y, NV[i].z);
			fs.vWrite(CAnsiString(Buffer));	
		}
		sprintf(Buffer, ("\n"));
		fs.vWrite(CAnsiString(Buffer));	

		// write group name	
		sprintf(Buffer, ("g obj_%d_%d_%d\n"), int(m_VoxelArray->ColorArray[0][voxel*4+0]*255), int(m_VoxelArray->ColorArray[0][voxel*4+1]*255), int(m_VoxelArray->ColorArray[0][voxel*4+2]*255));
		fs.vWrite(CAnsiString(Buffer));	

		// write material	
		sprintf(Buffer, ("usemtl mtl_%d_%d_%d\n"), int(m_VoxelArray->ColorArray[0][voxel*4+0]*255), int(m_VoxelArray->ColorArray[0][voxel*4+1]*255), int(m_VoxelArray->ColorArray[0][voxel*4+2]*255));
		fs.vWrite(CAnsiString(Buffer));	

		// write face
		for (int i=0; i<6; i++)
		{
			//sprintf(Buffer, ("f\t%d//%d %d//%d %d//%d %d//%d\n"),
			//			Face[i][0]+v_base, NFace[i][0]+nv_base,
			//			Face[i][1]+v_base, NFace[i][1]+nv_base,
			//			Face[i][2]+v_base, NFace[i][2]+nv_base,
			//			Face[i][3]+v_base, NFace[i][3]+nv_base);
            
            sprintf(Buffer, ("f\t%d//%d %d//%d %d//%d %d//%d\n"),
                    Face[i][0]+v_base, NFace[i][0]+nv_base,
                    Face[i][3]+v_base, NFace[i][3]+nv_base,
                    Face[i][2]+v_base, NFace[i][2]+nv_base,
                   Face[i][1]+v_base, NFace[i][1]+nv_base);
			fs.vWrite(CAnsiString(Buffer));	
		}
		sprintf(Buffer, ("\n"));
		fs.vWrite(CAnsiString(Buffer));				
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CVoxelClusterContainer::ExportOBJFile(const CString& filename)
	{
		CPlatformFileStream File;;
		int VertexBase, NormalVertexBase;
		char Fullname[512], Name[256], Buffer[256], MtlFullname[256];
		
		strcpy(Fullname, CAnsiString(filename).c_str());
		for (int i=strlen(Fullname); i>=0; i--)
		{
			if (Fullname[i] == '.')
			{
				Fullname[i] = 0x00;
				break;
			}
		}

		for (int i=strlen(Fullname); i>=0; i--)
		{
			if (Fullname[i] == '\\' || Fullname[i] == '/')
			{
				strcpy(Name, &(Fullname[i+1]));
				break;
			}
		}

		strcpy(MtlFullname, Fullname);
		strcat(MtlFullname, ".mtl");

		File.vOpen(filename, CStream::Text|CStream::Write|CStream::ANSI); 		

			// write material file
			sprintf(Buffer, ("mtllib %s.mtl\n"), Name);
			File.vWrite(CAnsiString(Buffer));
			sprintf(Buffer, ("\n"));
			File.vWrite(CAnsiString(Buffer));

			VertexBase = 0;
			NormalVertexBase = 0;

			for (int i=0; i<m_ClusterSize[0]; i++)
			{
				WriteOBJCube(File, i, VertexBase, NormalVertexBase);
				VertexBase += 8;
				NormalVertexBase = 6;			
			}

		File.vClose();		

		CList<CAnsiString> MtlList;

		File.vOpen(MtlFullname, CStream::Text|CStream::Write|CStream::ANSI); 		
		
			for (int i=0; i<m_ClusterSize[0]; i++)
			{
				sprintf(Buffer, ("newmtl mtl_%d_%d_%d\n"), int(m_VoxelArray->ColorArray[0][i*4+0]*255), int(m_VoxelArray->ColorArray[0][i*4+1]*255), int(m_VoxelArray->ColorArray[0][i*4+2]*255));
				if (LynxEngine::find(MtlList.begin(), MtlList.end(), Buffer) == MtlList.end())
				{						
					MtlList.push_back(CAnsiString(Buffer));

					File.vWrite(CAnsiString(Buffer));
					sprintf(Buffer, ("Ns 10.0000\nNi 1.5000\nd 1.0000\nTr 0.0000\nTf 1.0000 1.0000 1.0000\nillum 2\n"));
					File.vWrite(CAnsiString(Buffer));

					sprintf(Buffer, ("Ka %f %f %f\nKd %f %f %f\n"), 
						m_VoxelArray->ColorArray[0][i*4+0], m_VoxelArray->ColorArray[0][i*4+1], m_VoxelArray->ColorArray[0][i*4+2],
						m_VoxelArray->ColorArray[0][i*4+0], m_VoxelArray->ColorArray[0][i*4+1], m_VoxelArray->ColorArray[0][i*4+2]);
					File.vWrite(CAnsiString(Buffer));

					sprintf(Buffer, ("Ks 0.0000 0.0000 0.0000\nKe 0.0000 0.0000 0.0000\n\n"));
					File.vWrite(CAnsiString(Buffer));										
				}				
			}
			
		File.vClose();

		MtlList.clear();

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CVoxelClusterContainer::SetDimension(const int d)
	{
		/*
		float Center = (float)(GetDimension()) * 0.5f;	
		float NewCenter = d * 0.5f;

		Math::CVector3 Pos;
		LYNXCOLORRGBA Color;
		for (int i=0; i<m_ClusterSize[0]; i++)
		{
			vGetPosition(i, Pos);		
			Math::CVector3 NewPos = Pos - Math::CVector3(Center, Center, Center);
			NewPos = NewPos + Math::CVector3(NewCenter, NewCenter, NewCenter);
			vSetPosition(i, NewPos);
		}
		*/
		m_Dimension = d;		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明: read .va file to load voxel attribute data.
	//-------------------------------------------------------------------------------------------------------
	void CVoxelClusterContainer::LoadVoxelAttribute(const CString& name, const CString& filename, int voxelarrayindex)
	{
		CString VoxelList, TargetColor, Value;
		CString Token;
		CArray<int>	GlowVoxelList;
		LYNXCOLORRGBA Color[2];
		float Start = 0, End = 100, Step = 1, GlowScale = 1.0f;
		PLAYMODE Mode;
		CString::size_type LastPos, Pos;

		CReadOnlyProfile FrameFile(filename);

		if (FrameFile.Load())
		{
			CString VoxelAttribute;
			VoxelAttribute = FrameFile.Read(_T("VoxelAttribute"), VoxelAttribute);

			CXMLElement* Glows = FrameFile.GetRootElement()->FirstChildElement("Glows");
			if (Glows)
			{
				CXMLElement* Glow = Glows->FirstChildElement("Glow");
				while (Glow)
				{
					GlowVoxelList.clear();
					VoxelList = FrameFile.Read("Voxels", VoxelList, Glow);
					Pos = VoxelList.find_first_of(_T(','));
					int Index = 0, Voxel;
					LastPos = 0;
					while (Pos != CString::npos)
					{
						Token = VoxelList.substr(LastPos, Pos);
						if (!Token.empty())
						{
							Voxel = lynxAtoi(Token.c_str());
							GlowVoxelList.push_back(Voxel);
						}
						Pos++;
						LastPos = Pos;
						Pos = VoxelList.find_first_of(_T(','), Pos);
					}
					Token = VoxelList.substr(LastPos, CString::npos);
					if (!Token.empty())
					{
						Voxel = lynxAtoi(Token.c_str());
						GlowVoxelList.push_back(Voxel);
					}
					GlowScale = FrameFile.Read("Scale", GlowScale, Glow);
					AddGlowVoxels(voxelarrayindex, GlowVoxelList, GlowScale);

					Glow = Glow->NextSiblingElement("Glow");
				}
			}
			GlowVoxelList.clear();

			CXMLElement* ColorAnimationList = FrameFile.GetRootElement()->FirstChildElement("ColorAnimations");
			if (ColorAnimationList)
			{
				CXMLElement* ColorAnimation = ColorAnimationList->FirstChildElement("ColorAnimation");
				while (ColorAnimation)
				{
					Start = FrameFile.Read("Start", Start, ColorAnimation);
					End = FrameFile.Read("End", End, ColorAnimation);
					Step = FrameFile.Read("Step", Step, ColorAnimation);
					Mode = GetEnum(FrameFile.Read("Mode", "Loop", ColorAnimation), NUM_PLAYMODE);

					TargetColor = FrameFile.Read("TargetColor", TargetColor, ColorAnimation);
					Pos = TargetColor.find_first_of(_T(','));
					LastPos = 0;
					Token = TargetColor.substr(LastPos, Pos);
					Color[1].Red = lynxAtoi(Token.c_str());
					Pos++;
					LastPos = Pos;
					Pos = TargetColor.find_first_of(_T(','), Pos);
					Token = TargetColor.substr(LastPos, Pos);
					Color[1].Green = lynxAtoi(Token.c_str());
					Pos++;
					LastPos = Pos;
					Pos = TargetColor.find_first_of(_T(','), Pos);
					Token = TargetColor.substr(LastPos, Pos);
					Color[1].Blue = lynxAtoi(Token.c_str());
					Pos++;
					LastPos = Pos;
					Pos = TargetColor.find_first_of(_T(','), Pos);
					Token = TargetColor.substr(LastPos, Pos);
					Color[1].Alpha = lynxAtoi(Token.c_str());

					VoxelList = FrameFile.Read("Voxels", VoxelList, ColorAnimation);
					Pos = VoxelList.find_first_of(_T(','));
					int Index = 0, Voxel;
					LastPos = 0;
					while (Pos != CString::npos)
					{
						Token = VoxelList.substr(LastPos, Pos);
						if (!Token.empty())
						{
							Voxel = lynxAtoi(Token.c_str());
							vGetColor(Voxel, &Color[0]);
							AddVoxelAnimation()->Create(Voxel, -1, 2);
							GetVoxelAnimation(Index)->SetColorKey(0, Start, End, Color[0]);
							GetVoxelAnimation(Index)->SetColorKey(1, End, End + 1, Color[1]);
							GetVoxelAnimation(Index)->m_StartTime = Start;
							GetVoxelAnimation(Index)->m_EndTime = End;
							GetVoxelAnimation(Index)->m_PlayMode = Mode;
							GetVoxelAnimation(Index)->m_Step = Step;							
						}
						Index++;
						Pos++;
						LastPos = Pos;
						Pos = VoxelList.find_first_of(_T(','), Pos);
					}
					Token = VoxelList.substr(LastPos, CString::npos);
					if (!Token.empty())
					{
						Voxel = lynxAtoi(Token.c_str());
						vGetColor(Voxel, &Color[0]);
						AddVoxelAnimation()->Create(Voxel, -1, 2);
						GetVoxelAnimation(Index)->SetColorKey(0, Start, End, Color[0]);
						GetVoxelAnimation(Index)->SetColorKey(1, End, End + 1, Color[1]);
						GetVoxelAnimation(Index)->m_StartTime = Start;
						GetVoxelAnimation(Index)->m_EndTime = End;
						GetVoxelAnimation(Index)->m_PlayMode = Mode;
						GetVoxelAnimation(Index)->m_Step = Step;
					}
					ColorAnimation = ColorAnimation->NextSiblingElement("ColorAnimation");
				}
			}
		}
	}
}