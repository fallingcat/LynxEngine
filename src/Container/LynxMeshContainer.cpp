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
#include <SceneSystem/LynxSceneSystem.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <Container/LynxCameraContainer.h>
#include <Container/LynxMeshContainer.h>
#include <Container/LynxModelContainer.h>
#include <LynxBoxProxy.h>
#include <CollisionSystem/LynxCollisionHierarchy.h>
#include <CollisionSystem/LynxAABB.h>
#include <CollisionSystem/LynxSphere.h>
#include <Shader/LynxSkinningModelVS.h>

namespace LynxEngine 
{		
	LYNXMATRIX CMeshContainer::m_BoneMatrixArray[CMeshContainer::MAX_BONE_PER_MESH];			
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CMeshContainer::CMeshContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_ContainerType = MESH;		
		m_AnimationFlag = RIGID_ANIMATION;
		m_DynamicAttr = 0;	
		m_lpPhyObj = NULL;

		m_MeshData = CCountedPtr<MeshData>(LYNXNEW MeshData);	
		
		m_VertexArray.clear();
		m_IndexArray.clear();					
		m_BoneShaderData.clear();
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
	CMeshContainer::~CMeshContainer()	
	{
		m_VertexArray.clear();
		m_IndexArray.clear();		
		m_BoneShaderData.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CMeshContainer& CMeshContainer::operator =(const CMeshContainer& rhs)
	{	
		vInstance(rhs);

		return (*this);
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CMeshContainer::vInstance(const CContainer& rhs)
	{
		CContainer::vInstance(rhs);

		const CMeshContainer* RHS = Container_Cast<const CMeshContainer*>(&rhs);	
		
		m_Name = RHS->GetName();
		m_MaterialIndex = RHS->m_MaterialIndex;				
		m_NumLODs = RHS->m_NumLODs;
		m_AnimationFlag = RHS->m_AnimationFlag;			
		m_DynamicAttr = RHS->m_DynamicAttr;								

		m_MeshData = RHS->m_MeshData;
		
		m_VertexArray = RHS->m_VertexArray;
		m_IndexArray = RHS->m_IndexArray;

		m_BoneShaderData = RHS->m_BoneShaderData;
		m_VertexConstantIndex = RHS->m_VertexConstantIndex;
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
	void CMeshContainer::vSetNumLODs(int lod)	
	{
		m_NumLODs = lod;

		m_MeshData->VertexList.resize(lod);
		m_MeshData->VertexTangentList.resize(lod);
		m_MeshData->VertexBinormalList.resize(lod);
		m_MeshData->VertexNormalList.resize(lod);
		m_MeshData->UVVertexList.resize(lod);
		m_MeshData->VertexColorList.resize(lod);
		m_MeshData->WeightList.resize(lod);
		m_MeshData->TriangleList.resize(lod);
		m_MeshData->Primitives.resize(lod);		

		m_VertexArray.resize(lod);
		m_IndexArray.resize(lod);

		for (int i=0; i<m_NumLODs; i++)
		{
			LYNXCHAR LOD[32];
			CString LODStr;
			lynxItoA(i, LOD, 10);
			LODStr = CString(LOD);

			m_lpEngine->CreateResource(m_VertexArray[i], VERTEX_ARRAY);
			m_VertexArray[i]->SetName(m_Name+CString(_T(".VertexArray.LOD."))+LODStr);
			m_lpEngine->CreateResource(m_IndexArray[i], INDEX_ARRAY);	
			m_IndexArray[i]->SetName(m_Name+CString(_T(".IndexArray.LOD."))+LODStr);
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
	void CMeshContainer::vAddLOD()
	{
		int LOD = m_NumLODs + 1;

		m_MeshData->VertexList.resize(LOD);
		m_MeshData->VertexTangentList.resize(LOD);
		m_MeshData->VertexBinormalList.resize(LOD);
		m_MeshData->VertexNormalList.resize(LOD);
		m_MeshData->UVVertexList.resize(LOD);
		m_MeshData->VertexColorList.resize(LOD);
		m_MeshData->WeightList.resize(LOD);
		m_MeshData->TriangleList.resize(LOD);
		m_MeshData->Primitives.resize(LOD);

		m_VertexArray.resize(LOD);
		m_IndexArray.resize(LOD);
		
		LYNXCHAR Buffer[32];
		CString LODStr;
		lynxItoA(m_NumLODs, Buffer, 10);
		LODStr = CString(Buffer);

		m_lpEngine->CreateResource(m_VertexArray[m_NumLODs], VERTEX_ARRAY);
		m_VertexArray[m_NumLODs]->SetName(m_Name+CString(_T(".VertexArray.LOD."))+LODStr);
		m_lpEngine->CreateResource(m_IndexArray[m_NumLODs], INDEX_ARRAY);	
		m_IndexArray[m_NumLODs]->SetName(m_Name+CString(_T(".IndexArray.LOD."))+LODStr);
		
		m_NumLODs = LOD;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	int	CMeshContainer::GetVertexRelatedBoneConstantIndex(int bi)
	{
		int							i;

		for (i=0; i<m_BoneShaderData.size(); i++)
		{
			if (bi == m_BoneShaderData[i].BoneIndex)
			{
				return (m_BoneShaderData[i].ConstantIndex)/3;
			}
		}
		return -1;
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
	void CMeshContainer::SetupBlendVertex(int LOD)
	{
		int			BI;			

		BI = BONE_START_CONSTANT;
		for (int i=0; i<m_BoneShaderData.size(); i++)
		{
			/*
			//skip constant -38 & -37
			if (BI >= -41 && BI <= -37)
			{
				BI = -36;				
			}
			*/
			m_BoneShaderData[i].ConstantIndex = BI;							
			BI+=3;	//we only need the first 3 rows of bone matrix 					
		}

		for (int i=0; i<m_MeshData->VertexList[LOD].size(); i++)
		{					
			m_lpBV[i].Color.r = (float)m_MeshData->VertexColorList[LOD][i].Red/255.0f;
			m_lpBV[i].Color.g = (float)m_MeshData->VertexColorList[LOD][i].Green/255.0f;
			m_lpBV[i].Color.b = (float)m_MeshData->VertexColorList[LOD][i].Blue/255.0f;
			m_lpBV[i].Color.a = (float)m_MeshData->VertexColorList[LOD][i].Alpha/255.0f;
			
			m_lpBV[i].TexCoord[0].u = m_MeshData->UVVertexList[LOD][i].u;
			m_lpBV[i].TexCoord[0].v = m_MeshData->UVVertexList[LOD][i].v;

			m_lpBV[i].Tangent.x = m_MeshData->VertexTangentList[LOD][i].x;
			m_lpBV[i].Tangent.y = m_MeshData->VertexTangentList[LOD][i].y;
			m_lpBV[i].Tangent.z = m_MeshData->VertexTangentList[LOD][i].z;															

			m_lpBV[i].Normal.x = m_MeshData->VertexNormalList[LOD][i].x;
			m_lpBV[i].Normal.y = m_MeshData->VertexNormalList[LOD][i].y;
			m_lpBV[i].Normal.z = m_MeshData->VertexNormalList[LOD][i].z;															

			m_lpBV[i].Offset[0].x = 0.0f;
			m_lpBV[i].Offset[0].y = 0.0f;
			m_lpBV[i].Offset[0].z = 0.0f;		
			m_lpBV[i].Weight[0] = 0.0f;		
			m_lpBV[i].BoneIndex[0] = BONE_START_CONSTANT;

			m_lpBV[i].Offset[1].x = 0.0f;
			m_lpBV[i].Offset[1].y = 0.0f;
			m_lpBV[i].Offset[1].z = 0.0f;		
			m_lpBV[i].Weight[1] = 0.0f;		
			m_lpBV[i].BoneIndex[1] = BONE_START_CONSTANT;

			m_lpBV[i].Offset[2].x = 0.0f;
			m_lpBV[i].Offset[2].y = 0.0f;
			m_lpBV[i].Offset[2].z = 0.0f;		
			m_lpBV[i].Weight[2] = 0.0f;		
			m_lpBV[i].BoneIndex[2] = BONE_START_CONSTANT;

			m_lpBV[i].Offset[3].x = 0.0f;
			m_lpBV[i].Offset[3].y = 0.0f;
			m_lpBV[i].Offset[3].z = 0.0f;		
			m_lpBV[i].Weight[3] = 0.0f;		
			m_lpBV[i].BoneIndex[3] = BONE_START_CONSTANT;

			for (int j=0; j<m_MeshData->WeightList[LOD][i].NumBones; j++)
			{
				if (j < 4)
				{
					m_lpBV[i].Weight[j] = m_MeshData->WeightList[LOD][i].lpWeight[j];
					m_lpBV[i].BoneIndex[j] = GetVertexRelatedBoneConstantIndex(m_MeshData->WeightList[LOD][i].lpBoneIndex[j]);				

					m_lpBV[i].Offset[j].x = m_MeshData->WeightList[LOD][i].lpOffset[j].x;
					m_lpBV[i].Offset[j].y = m_MeshData->WeightList[LOD][i].lpOffset[j].y;
					m_lpBV[i].Offset[j].z = m_MeshData->WeightList[LOD][i].lpOffset[j].z;		
				}							
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
	void CMeshContainer::SetupRigidVertex(int LOD)
	{
		for (int i=0; i<m_MeshData->VertexList[LOD].size(); i++)
		{	
			m_lpMV[i].Color.r = (float)m_MeshData->VertexColorList[LOD][i].Red/255.0f;
			m_lpMV[i].Color.g = (float)m_MeshData->VertexColorList[LOD][i].Green/255.0f;
			m_lpMV[i].Color.b = (float)m_MeshData->VertexColorList[LOD][i].Blue/255.0f;
			m_lpMV[i].Color.a = (float)m_MeshData->VertexColorList[LOD][i].Alpha/255.0f;
			m_lpMV[i].TexCoord[0].u = m_MeshData->UVVertexList[LOD][i].u;
			m_lpMV[i].TexCoord[0].v = m_MeshData->UVVertexList[LOD][i].v;					
					
			m_lpMV[i].Pos.x = m_MeshData->VertexList[LOD][i].x;
			m_lpMV[i].Pos.y = m_MeshData->VertexList[LOD][i].y;
			m_lpMV[i].Pos.z = m_MeshData->VertexList[LOD][i].z;									
			m_lpMV[i].Normal.x = m_MeshData->VertexNormalList[LOD][i].x;
			m_lpMV[i].Normal.y = m_MeshData->VertexNormalList[LOD][i].y;
			m_lpMV[i].Normal.z = m_MeshData->VertexNormalList[LOD][i].z;	
			m_lpMV[i].Tangent.x = m_MeshData->VertexTangentList[LOD][i].x;
			m_lpMV[i].Tangent.y = m_MeshData->VertexTangentList[LOD][i].y;
			m_lpMV[i].Tangent.z = m_MeshData->VertexTangentList[LOD][i].z;														
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
	void CMeshContainer::SetupMaterial(MaterialSystem::CMaterial* lpm)
	{
		char MName[128], TextureName[128];	
		CString TName;
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

		DDesc.Default();		
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		MaterialSystem::CTechnique* Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);														
		LynxEngine::MaterialSystem::CPass& Pass = Technique->GetPass(0);
		
		switch (m_AnimationFlag)
		{
			case RIGID_ANIMATION:				
				lpm->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_RIGID));						
				Technique->GetPass(0).vCreateShaderClasses("RigidModelVS", "DiffuseMapXColorPS");		

				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);					
				
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);				

				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
				Technique->GetPass(0).CreateRasterizerState(RDesc);

				Technique->GetPass(0).m_bUseFog = LYNX_FALSE;
				
				#if defined(__DESKTOP__) && !defined (__MOBILE_SIMULATION__)
					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE_LIGHTING);				
					Technique->vCreatePass(1);				
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelVS", "DiffuseMapXColorPS");

					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);					
				
					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

				
					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::WIREFRAME);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelVS", "ConstantColorPS");
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;



					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::AMBIENT_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelLightingVS", "AmbientLightingPS");				
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);									

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_DEST_COLOR;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;



					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelLightingVS", "PointLightingPS");
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);									

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;


					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SPOT_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelLightingVS", "SpotLightingPS");	
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

				
					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ZPASS);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelZPassVS", "ZPass_Velocity_AlphaTestPS");						
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;


					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::GPASS);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelGPassVS", "GPassPS");			
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;


					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SHADOWMAP);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelShadowMapVS", "ShadowMap_AlphaTestPS");			
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SHADOW_TEXTURE);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelGPassVS", "ConstantColorPS");			
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ONE_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("RigidModelOneLightingVS", "OneLightingPS");					
                    DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;
				#elif defined (__MOBILE__) || defined (__MOBILE_SIMULATION__)
					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ONE_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					//Technique->GetPass(0).vCreateShaderClasses("RigidModelOneLightingVS", "OneLightingPS");
					Technique->GetPass(0).vCreateShaderClasses("RigidModelVS", "DiffuseMapXColorPS");
                    DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;
				#endif
				break;

			case BLEND_ANIMATION:
				lpm->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_BLEND));										
				Technique->GetPass(0).vCreateShaderClasses("SkinningModelVS", "DiffuseMapXColorPS");			

				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);					
				
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);				

				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

				#if defined(__DESKTOP__) && !defined (__MOBILE_SIMULATION__)
					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ZPASS);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelZPassVS", "ZPass_Velocity_AlphaTestPS");			
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);									

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);
					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::GPASS);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					//Technique->GetPass(0).vLoadShaders( _T("scn/model_mbs_gpass.vso"), _T("scn/gpass.pso"));		
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelGPassVS", "GPassPS");			
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);									

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);
					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SHADOWMAP);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					//Technique->GetPass(0).vLoadShaders( _T("scn/model_mbs_shadowmap.vso"), _T("scn/shadowmap_at.pso"));
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelShadowMapVS", "ShadowMap_AlphaTestPS");			
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SHADOW_TEXTURE);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelVS", "ConstantColorPS");			
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

				
					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelSimpleLightingVS", "SimpleLightingPS");							
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);					

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;


					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::WIREFRAME);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelVS", "ConstantColorPS");			
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);				

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_WIREFRAME;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;


					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::AMBIENT_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelVS", "AmbientLightingPS");
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;


				
					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelLightingVS", "PointLightingPS");
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;



					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SPOT_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelLightingVS", "SpotLightingPS");				
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);														

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;


					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelLightingVS", "DirectionalLightingPS");				
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);														

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;

					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ONE_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelOneLightingVS", "OneLightingNoShadowPS");
					//Technique->GetPass(0).vCreateShaderClasses("SkinningModelOneLightingVS", "OneLightingPS");
					//Technique->GetPass(0).vCreateShaderClasses("SkinningModelVS", "DiffuseMapXColorPS");
					//Technique->GetPass(0).vCreateShaderClasses("SkinningModelSimpleLightingVS", "SimpleLightingPS");							
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;
				#elif defined (__MOBILE__) || defined (__MOBILE_SIMULATION__)
					Technique = &lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ONE_LIGHTING);
					Technique->vCreatePass(1);
					Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
					Technique->GetPass(0).vCreateShaderClasses("SkinningModelOneLightingVS", "OneLightingNoShadowPS");
					//Technique->GetPass(0).vCreateShaderClasses("SkinningModelOneLightingVS", "OneLightingPS");
					//Technique->GetPass(0).vCreateShaderClasses("SkinningModelVS", "DiffuseMapXColorPS");
					//Technique->GetPass(0).vCreateShaderClasses("SkinningModelSimpleLightingVS", "SimpleLightingPS");							
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);								

					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;
				#endif
				break;
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
	LYNXBOOL CMeshContainer::Setup(int LOD)
	{
		int					i, j, k;
		WORD				*lpI;
		WORD				T1, T2, T3, C1, C2, C3;		
		DWORD				NumIndices;
			
		switch (m_AnimationFlag)
		{
			case RIGID_ANIMATION:
				m_lpMV = LYNXGLOBAL_NEW GraphicsSystem::RIGIDVERTEX[m_MeshData->VertexList[LOD].size()];
				SetupRigidVertex(LOD);
				m_VertexArray[LOD]->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_RIGID)), (int)m_MeshData->VertexList[LOD].size(), GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, m_lpMV);
				LYNXGLOBAL_DEL_ARRAY(m_lpMV);								
				break;

			case BLEND_ANIMATION:
				m_lpBV = LYNXGLOBAL_NEW GraphicsSystem::BLENDVERTEX[m_MeshData->VertexList[LOD].size()];
				SetupBlendVertex(LOD);
				m_VertexArray[LOD]->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_BLEND)), (int)m_MeshData->VertexList[LOD].size(), GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, m_lpBV);				
				LYNXGLOBAL_DEL_ARRAY(m_lpBV);
				break;
		}
		NumIndices = 0;
		for (j=0; j<m_MeshData->Primitives[LOD].size(); j++)
		{
			NumIndices += m_MeshData->Primitives[LOD][j].NumIndex;
		}											
		lpI = LYNXGLOBAL_NEW WORD[NumIndices];		
		for (j=0; j<m_MeshData->Primitives[LOD].size(); j++)
		{
			m_MeshData->Primitives[LOD][j].BaseVertex = 0;
			m_MeshData->Primitives[LOD][j].BaseIndex = 0;				
			for (k=0; k<m_MeshData->Primitives[LOD][j].NumIndex; k++)
			{
				lpI[k] = m_MeshData->Primitives[LOD][j].lpIndex[k];					
			}	
			#ifndef __TOOL__
				// free original primitive index data
				if (m_MeshData->Primitives[LOD][j].lpIndex)
					LYNXGLOBAL_DEL_ARRAY(m_MeshData->Primitives[LOD][j].lpIndex);
				m_MeshData->Primitives[LOD][j].lpIndex = NULL;
			#endif
		}				
		m_IndexArray[LOD]->vCreate(sizeof(WORD), NumIndices, GraphicsSystem::CRenderer::WRITEONLY|GraphicsSystem::CRenderer::STATIC, LYNX_TRUE, lpI);
		LYNXGLOBAL_DEL_ARRAY(lpI);

		m_MeshData->VertexNormalList[LOD].clear();
		m_MeshData->UVVertexList[LOD].clear();
		m_MeshData->VertexColorList[LOD].clear();
		//LYNX_FREE(m_lpColorVertexList[LOD]);		

		// Temporary code
#if 1
		if (LOD == 0)
		{
			Math::CVector3 Min(FLT_MAX, FLT_MAX, FLT_MAX);
			Math::CVector3 Max(-FLT_MAX,-FLT_MAX,-FLT_MAX);						
			Math::CVector3 Center, Ext;
			//CObjProxy* lpProxy = LYNXNEW CBoxProxy(m_lpEngine);
			//lpProxy->SetlpParent(m_lpRenObj, this);
			//lpProxy->SetName(m_Name + CString(_T(".Proxy00")));
			for (int j=0; j<m_MeshData->VertexList[0].size(); j++)
			{
				if (m_MeshData->VertexList[0][j].x > Max.x)
					Max.x = m_MeshData->VertexList[0][j].x;
				else if (m_MeshData->VertexList[0][j].x < Min.x)
					Min.x = m_MeshData->VertexList[0][j].x;

				if (m_MeshData->VertexList[0][j].y > Max.y)
					Max.y = m_MeshData->VertexList[0][j].y;
				else if (m_MeshData->VertexList[0][j].y < Min.y)
					Min.y = m_MeshData->VertexList[0][j].y;

				if (m_MeshData->VertexList[0][j].z > Max.z)
					Max.z = m_MeshData->VertexList[0][j].z;
				else if (m_MeshData->VertexList[0][j].z < Min.z)
					Min.z = m_MeshData->VertexList[0][j].z;
			}
			Center = (Max+Min)*0.5f;
			Ext = (Max-Min)*0.5f;
			CReal Radius = LYNX_MAX(LYNX_MAX(Ext.x, Ext.y), Ext.z);
			//((CBoxProxy*)lpProxy)->Create(Center, Ext);
			//m_ProxyList.push_back(lpProxy);
			//GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
			//DDesc.Default();
			//DDesc.StencilEnable = LYNX_FALSE;
			//DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
			//DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
			//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->CreateDepthStencilState(m_OcclusionDepthStencilState, DDesc);			
			
			m_lpCollisionHierarchy->Create(2);
			CollisionSystem::CBtOBB* Box = LYNXNEW CollisionSystem::CBtOBB();
			//Min.x *= 0.7f;
			//Max.x *= 0.7f;
			/*
			if (lynxStriStr(m_Name.c_str(), _T("obb")))
			{
				Min.y = -300.0f;
				Max.y = 300.0f;			
			}
			*/
			Box->SetByBoundary(Min, Max);
			m_lpCollisionHierarchy->Add(COT_CAPSULE, Box);
			Box->SetParentContainer(this);

			Box = LYNXNEW CollisionSystem::CBtOBB();
			Box->SetByBoundary(Min, Max);
			m_lpCollisionHierarchy->Add(COT_OBB, Box);
			Box->SetParentContainer(this);

			/*
			if (lynxStriStr(m_Name.c_str(), _T("obb")))
			{
				m_Visibility = 0.0f;
			}
			*/

			// Level 1
			/*
			CollisionSystem::CSphere* pSphere = LYNXNEW CollisionSystem::CSphere(Center, Radius);		
			m_lpCollisionHierarchy->Add(0, pSphere);	
			// Level 2
			CollisionSystem::CAABB* pAABB = LYNXNEW CollisionSystem::CAABB(Center, Ext);					
			m_lpCollisionHierarchy->Add(1, pAABB);	
			*/
		}
#endif

		//Yahoo!!
		return TRUE;
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
	void CMeshContainer::ComputeOBB()
	{
		/*
		CollisionSystem::CBtOBB* Box = LYNXNEW CollisionSystem::CBtOBB();
		for (int i=0; i<GetVertexList(m_CurrentLOD).size(); i++)
		{
			GetVertex(m_CurrentLOD, i)
		}
		lynxPointXMatrix(&m_TVertexList[NumV], &(GetVertex(lod, j)), &(m_M)); 					

        Box->SetByBoundary(Min, Max);
		m_lpCollisionHierarchy->Add(0, Box);
		Box->SetParentContainer(this);*/
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
	void CMeshContainer::vUpdateVertices(int lod)
	{		
		int NumV = 0;
		switch (m_AnimationFlag)
		{
			case CMeshContainer::BLEND_ANIMATION:
				break;

			case CMeshContainer::RIGID_ANIMATION:
				for (int j=0; j<GetVertexList(lod).size(); j++)
				{
					lynxPointXMatrix(&m_TVertexList[NumV], &(GetVertex(lod, j)), &(m_M)); 					
					NumV++;
				}
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CMeshContainer::vSetupTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc, int lod, LYNXBOOL bapply_world)
	{
		int LOD = lod;
		if (LOD >= m_NumLODs)
			LOD = m_NumLODs - 1;

		m_TVertexList.clear();
		m_TVertexList.resize(m_MeshData->VertexList[LOD].size());

		m_lpParentContainer->PushTransform();
		Math::CMatrix4 M;
		M.Identity();
		m_lpParentContainer->SetTransformMatrix(M);
		m_lpParentContainer->vPlay(0);
		vUpdateVertices(LOD);
		m_lpParentContainer->PopTransform();
		m_lpParentContainer->vPlay(0);

		desc->m_NumVertices		= m_TVertexList.size();
		desc->m_VertexStride	= sizeof(LYNXVECTOR3D);
		desc->m_VertexList		= &m_TVertexList[0].x;
		desc->m_NumTriangles	= m_MeshData->TriangleList[LOD].size();
		desc->m_TriangleStride	= sizeof(LYNXSIMPLETRIANGLE);
		desc->m_TriangleList	= &m_MeshData->TriangleList[LOD][0].v[0];

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CMeshContainer::vReleaseTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CMeshContainer::vSetupHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc, int lod, LYNXBOOL bapply_world)
	{
		int LOD = lod;
		if (LOD >= m_NumLODs)
			LOD = m_NumLODs - 1;
	
		m_TVertexList.clear();
		m_TVertexList.resize(m_MeshData->VertexList[LOD].size());

		m_lpParentContainer->PushTransform();
		Math::CMatrix4 M;
		M.Identity();
		m_lpParentContainer->SetTransformMatrix(M);
		m_lpParentContainer->vPlay(0);
		vUpdateVertices(LOD);
		m_lpParentContainer->PopTransform();
		m_lpParentContainer->vPlay(0);

		desc->m_NumVertices		= m_TVertexList.size();
		desc->m_VertexStride	= sizeof(LYNXVECTOR3D);
		desc->m_VertexList		= &m_TVertexList[0].x;

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Release HullShapeDesc object.
	*
	*	@param desc <in>The HullShapeDesc object which will be released.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CMeshContainer::vReleaseHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc)
	{		
		m_TVertexList.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Create physics object.
	*
	*	@param t <in>Physics object type.	
	*	@param g <in>Physics shape which will be attched to physics object.	
	*	@return Created physics object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	PhysicsSystem::CPhysicsObj*	CMeshContainer::CreatePhyObj(PhysicsSystem::CPhysicsObj::TYPE t, PhysicsSystem::CPhysicsShape* g)
	{
		m_lpPhyObj = m_lpEngine->vCreatePhysicsObj(t, g);
		return m_lpPhyObj;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CMeshContainer::vRender(CCameraContainer* cam)
	{	
		GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();

		if (m_Visibility <= 0.0f)
			return;

		if (m_lpParent)
			m_CurrentLOD = m_lpParent->m_CurrentLOD;
		else
			vCalculateLOD(cam);

		/*
		GraphicsSystem::RIGIDVERTEX* lpTVertex = LYNXNEW GraphicsSystem::RIGIDVERTEX[m_MeshData->VertexList[m_CurrentLOD].size()];
		lynxMemCpy(lpTVertex, m_lpMV, sizeof(GraphicsSystem::RIGIDVERTEX)*m_MeshData->VertexList[m_CurrentLOD].size());
		for (int i=0; i<m_MeshData->VertexList[m_CurrentLOD].size(); i++)
			lynxPointXMatrix(&lpTVertex[i].Pos, &m_lpMV[i].Pos, &m_M);
		
		GraphicsSystem::RIGIDVERTEX* lpMV = (GraphicsSystem::RIGIDVERTEX*)m_VertexArray[m_CurrentLOD]->vMap(0, sizeof(GraphicsSystem::RIGIDVERTEX)*m_MeshData->VertexList[m_CurrentLOD].size(), GraphicsSystem::CRenderer::LOCK_TO_WRITE_DISCARD_OLD);
		lynxMemCpy(lpMV, lpTVertex, sizeof(GraphicsSystem::RIGIDVERTEX)*m_MeshData->VertexList[m_CurrentLOD].size());
		m_VertexArray[m_CurrentLOD]->vUnmap();
		LYNXDEL [] lpTVertex;
		ResetTransform();
		*/
		
		//lpR->SetWorldMatrix(m_M);
		lpR->SetVertexArray(m_VertexArray[m_CurrentLOD], 0, 0);
		lpR->SetIndexArray(m_IndexArray[m_CurrentLOD], 0);	

		LYNX_ASSERT(m_lpCurrentMaterial);

		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)
		{
			Technique.GetPass(p).Set(this, cam);		
			if (m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene() && m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer())
				m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();			
			for (int i=0; i<m_MeshData->Primitives[m_CurrentLOD].size(); i++)
				lpR->DrawPrimitiveBO(&m_MeshData->Primitives[m_CurrentLOD][i], (int)m_MeshData->VertexList[m_CurrentLOD].size());			
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CMeshContainer::UpdateShaderConstant()
	{
		#if 1
			/*
			static LYNXMATRIX4x3 TM[64];
			CModelContainer* lpModel = Container_Cast<CModelContainer*>(m_lpParentContainer);
			for (int i=0; i<m_BoneShaderData.size(); i++)
			{
				lynxCopyMatrix2Matrix4x3(&TM[m_BoneShaderData[i].ConstantIndex/3], &lpModel->GetlpBone(m_BoneShaderData[i].BoneIndex)->GetTransformMatrix());														
			}
			((Shader::CSkinningModelVS*)lps)->GetBoneMatrixConstant()->vMap();
				((Shader::CSkinningModelVS*)lps)->GetBoneMatrixConstant()->vSetFloat((float *)&TM[0], (int)m_BoneShaderData.size()*12);	//we only need 3 rows				
			((Shader::CSkinningModelVS*)lps)->GetBoneMatrixConstant()->vUnmap();
			*/
			
			CModelContainer* lpModel = Container_Cast<CModelContainer*>(m_lpParentContainer);
			for (int i=0; i<m_BoneShaderData.size(); i++)
			{
				lynxMemCpy(&m_BoneMatrixArray[m_BoneShaderData[i].ConstantIndex/3], &lpModel->GetlpBone(m_BoneShaderData[i].BoneIndex)->GetTransformMatrix(), sizeof(LYNXMATRIX));	
			}					
		#elif __iOS__
			CModelContainer* lpModel = Container_Cast<CModelContainer*>(m_lpParentContainer);
			for (int i=0; i<m_BoneShaderData.size(); i++)
			{
				lynxMemCpy(&m_BoneMatrixArray[m_BoneShaderData[i].ConstantIndex/3], &lpModel->GetlpBone(m_BoneShaderData[i].BoneIndex)->GetTransformMatrix(), sizeof(LYNXMATRIX));	
			}		
			lps->vSetConstantM("gBoneMatrix", m_BoneMatrixArray, (int)m_BoneShaderData.size());			
		#endif
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CMeshContainer::vLighting(CCameraContainer* cam, SceneSystem::CLight* lit)
	{
		LYNXMATRIX MM, WVPM, WVM, WLPM, WLM, TM1;
		float ZScale;
		float ZBias = 0.0005f;
		float TexelGrad[2];
		float Color[3];
		Math::CVector3 Pos, Dir;
		float Rad;
		float WireframeColor[4];			

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
		
		GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();

		lpR->SetVertexArray(m_VertexArray[m_CurrentLOD], 0, 0);
		lpR->SetIndexArray(m_IndexArray[m_CurrentLOD], 0);	

		switch (lit->GetLightType())
		{
			case SceneSystem::CLight::AMBIENT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);
				break;
			case SceneSystem::CLight::DIRECTIONAL:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);
				break;
			case SceneSystem::CLight::POINT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);
				break;
			case SceneSystem::CLight::SPOT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);
				break;			
		}
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)				
		{
			Technique.GetPass(p).Set(this, cam);
	
			/*if (cam->IsMirror())
			{
				switch (Technique.GetPass(p).m_CullMode)
				{
					case GraphicsSystem::CRenderer::CULL_CCW:
						lynxSetHardwareCullMode(LYNX_CULL_CW);
						break;

					case GraphicsSystem::CRenderer::CULL_CW:
						lynxSetHardwareCullMode(LYNX_CULL_CCW);
						break;
				}
			}*/

			lpR->GetCachedVertexShader()->vSetConstantM("gWorldViewProj", &WVPM);
			lpR->GetCachedVertexShader()->vSetConstantM("gWorldView", &WVM);
			lpR->GetCachedVertexShader()->vSetConstantM("gWorld", &m_M);
			lit->vSetShaderParam(cam, this);
			
			m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();

			CModelContainer* lpModel = Container_Cast<CModelContainer*>(m_lpParentContainer);
			if (lpModel->UseBufferObject())
			{					
				for (int i=0; i<m_BoneShaderData.size(); i++)
				{
					lynxTransposeMatrix2(&TM1, &lpModel->GetlpBone(m_BoneShaderData[i].BoneIndex)->GetTransformMatrix());										
					lpR->SetVSConstantF(m_BoneShaderData[i].ConstantIndex, (LYNXREAL *)&TM1, 3);	//we only need 3 rows
				}								
				for (int i=0; i<m_MeshData->Primitives[m_CurrentLOD].size(); i++)
					lpR->DrawPrimitiveBO(&m_MeshData->Primitives[m_CurrentLOD][i], (int)m_MeshData->VertexList[m_CurrentLOD].size());
			}
			else
			{	
				lpR->SetWorldMatrix(m_M);
				MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
				for (int p=0; p<Technique.GetNumPass(); ++p)
				{
					Technique.GetPass(p).Set(this, cam);									
					for (int i=0; i<m_MeshData->Primitives[m_CurrentLOD].size(); i++)			
						lpR->DrawElements(m_MeshData->Primitives[m_CurrentLOD][i].Type, 0, (int)m_MeshData->VertexList[m_CurrentLOD].size(), 0, m_MeshData->Primitives[m_CurrentLOD][i].NumPrimitives); 		
				}				
			}						
		}
	}
}