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
#include <Container/LynxSpriteGrassContainer.h>
#include <Container/LynxCameraContainer.h>
#include <SceneSystem/Light/LynxLight.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScnRenderer.h>

namespace LynxEngine 
{		
	int									CSpriteGrassContainer::m_BatchSize = 100; 
	GraphicsSystem::CVertexLayoutPtr	CSpriteGrassContainer::m_VertexLayout;
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CVertexLayout* CSpriteGrassContainer::CreateVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[2];

		Slots[0].Stream	= 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 2;  // Texture coordinate 0 			
		Slots[1].Stream	= 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[1].TypeIndex = 0;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 1;  // Position Index
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 2);
		v->SetName(CString(_T("SpriteGrass Vertex Layout")));

		return v;		
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
	void CSpriteGrassContainer::CreateVertexLayout(void)
	{
		if (!m_VertexLayout && m_lpEngine->GetlpGraphicsSystem() && m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer())
		{
			m_lpEngine->CreateVertexLayout(m_VertexLayout, CString(_T("SpriteGrass Vertex Layout")));
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
	CSpriteGrassContainer::CSpriteGrassContainer(CEngine *lpengine)	
	: CSprite3DContainer(lpengine)
	{
		m_ContainerType = GRASS;		
		m_RenderedSize = 1;
		m_ClusterSize = 1024;
		m_AlignmentFlag = ALIGNMENT_H_CENTER | ALIGNMENT_V_BOTTOM;
		m_LockedAxis = LYNX_Y_AXIS;//LYNX_X_AXIS | LYNX_Y_AXIS | LYNX_Z_AXIS;
		m_lpV = NULL;
		m_PosArray = NULL;
		m_NormalArray = NULL;
		m_RotationArray = NULL;
		m_VisPosArray = NULL;
		m_VisNormalArray = NULL;
		m_VisRotationArray = NULL;
		m_VisDistance = 400.0f;

		CreateVertexLayout();
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
	CSpriteGrassContainer::~CSpriteGrassContainer(void)
	{			
		if (m_PosArray)
			LYNXGLOBAL_DEL_ARRAY(m_PosArray);
		if (m_NormalArray)
			LYNXGLOBAL_DEL_ARRAY(m_NormalArray);		
		
		if (m_VisPosArray)
			LYNXGLOBAL_DEL_ARRAY(m_VisPosArray);
		if (m_VisNormalArray)
			LYNXGLOBAL_DEL_ARRAY(m_VisNormalArray);				

		if (m_RotationArray)
			LYNXGLOBAL_DEL_ARRAY(m_RotationArray);
		if (m_VisRotationArray)
			LYNXGLOBAL_DEL_ARRAY(m_VisRotationArray);				
	}			
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CSpriteGrassContainer& CSpriteGrassContainer::operator =(const CSpriteGrassContainer& rhs)
	{	
		return (*this);
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
	LYNXBOOL CSpriteGrassContainer::vCreate(const CCreationDesc* desc)
	{
		MaterialSystem::CTechnique* Technique;
		CSpriteGrassCreationDesc* Desc = (CSpriteGrassCreationDesc*)desc;			
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

		DDesc.Default();
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();
		
		CreateVertexLayout();
		
		if (desc->m_bCreateMaterial)
		{
			CString MatName;

			if (Desc->m_MaterialName.empty())
			{
				MatName = m_Name+CString(_T(".Material"));				
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
				lpMat->SetVertexLayout(m_VertexLayout);

				lpMat->vSetNumTextureAnimation(1);
				lpMat->GetTextureAnimation(0)->Create(1);
				lpMat->GetTextureAnimation(0)->LoadTexture(0, *(Desc->m_lpStream), Desc->m_FileName.c_str(), &Desc->m_TextureDesc);		
				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				lpMat->vAddTechnique(MaterialSystem::CMaterial::WIREFRAME);				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::ZPASS);				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::SHADOWMAP);				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::GPASS);				

				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE);
				Technique->vCreatePass(1);			
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);				
				BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);			
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);			
				Technique->GetPass(0).vCreateShaderClasses("SpriteGrassVS", "DiffuseMapPS");				

				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ZPASS);
				Technique->vCreatePass(1);					
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
				Technique->GetPass(0).vCreateShaderClasses("SpriteGrassZPassVS", "ZPass_Velocity_AlphaTestPS");								
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);		
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				
				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SHADOWMAP);
				Technique->vCreatePass(1);		
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
				Technique->GetPass(0).vCreateShaderClasses("SpriteGrassShadowMapVS", "ShadowMap_AlphaTestPS");				
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);									
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).m_bUseFog = LYNX_FALSE;
				
				Technique = &lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::GPASS);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
				Technique->GetPass(0).vCreateShaderClasses("SpriteGrassGPassVS", "SpriteGrassGPassPS");				
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);									
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[1].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[1].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[1].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[1].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[2].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[2].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[2].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[2].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[3].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[3].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[3].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[3].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).m_bUseFog = LYNX_FALSE;			

				Technique = &lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE_LIGHTING);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
				Technique->GetPass(0).vCreateShaderClasses("SpriteGrassGPassVS", "SimpleLightingPS");				
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);									
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).m_bUseFog = LYNX_FALSE;			


				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::AMBIENT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SPOT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

			}
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
		}

		m_NumLODs = 1;		
		m_lpEngine->CreateResource(m_VertexArray, VERTEX_ARRAY);		
		m_lpEngine->CreateResource(m_IndexArray, INDEX_ARRAY);		

		m_Width = 50.0f;
		m_Height = 50.0f;
		m_NumSegX = Desc->m_NumSegX;
		m_NumSegY = Desc->m_NumSegY;
		m_ClusterSize = Desc->m_ClusterSize;

		m_PosArray = LYNXGLOBAL_NEW LYNXVECTOR4D[m_ClusterSize];
		m_VisPosArray = LYNXGLOBAL_NEW LYNXVECTOR4D[m_ClusterSize];		
		m_NormalArray = LYNXGLOBAL_NEW LYNXVECTOR4D[m_ClusterSize];		
		m_VisNormalArray = LYNXGLOBAL_NEW LYNXVECTOR4D[m_ClusterSize];	
		m_RotationArray = LYNXGLOBAL_NEW float[m_ClusterSize];
		m_VisRotationArray = LYNXGLOBAL_NEW float[m_ClusterSize];		

		int NumV = (m_NumSegX+1)*(m_NumSegY+1);
		int CurrentV = 0;
		int NumSprites = LYNX_MIN(m_BatchSize, m_ClusterSize);
		m_VertexArray->vCreate(m_VertexLayout, NumSprites*NumV, GraphicsSystem::CRenderer::DYNAMIC|GraphicsSystem::CRenderer::WRITEONLY, Desc->m_bUseBufferObject);		
		GRASSVERTEX* lpV = reinterpret_cast<GRASSVERTEX*>(m_VertexArray->vMap(0, 0, GraphicsSystem::CRenderer::LOCK_TO_WRITE));						
			for (int i=0; i<NumSprites; i++)
			{
				for (int y=0; y<=m_NumSegY; y++)
				{
					for (int x=0; x<=m_NumSegX; x++)
					{
						lpV[CurrentV].TexCoord.u = (float)x/(float)m_NumSegX;		
						lpV[CurrentV].TexCoord.v = (float)y/(float)m_NumSegY; 										
						lpV[CurrentV].Instance = (float)i;
						CurrentV++;
					}
				}
			}
		m_VertexArray->vUnmap();		

		int NumI = m_NumSegX*m_NumSegY*6;
		int CurrentIndex = 0;
		WORD* lpI = LYNXGLOBAL_NEW WORD [NumSprites*NumI];
		for (int i=0; i<NumSprites; i++)
		{
			for (int y=0; y<m_NumSegY; y++)
			{
				for (int x=0; x<m_NumSegX; x++)
				{
					lpI[CurrentIndex++] = i*NumV+y*(m_NumSegX+1)+x;		lpI[CurrentIndex++] = i*NumV+y*(m_NumSegX+1)+x+1; lpI[CurrentIndex++] = i*NumV+(y+1)*(m_NumSegX+1)+x; 
					lpI[CurrentIndex++] = i*NumV+(y+1)*(m_NumSegX+1)+x;	lpI[CurrentIndex++] = i*NumV+y*(m_NumSegX+1)+x+1; lpI[CurrentIndex++] = i*NumV+(y+1)*(m_NumSegX+1)+x+1; 
				}
			}
		}
		m_IndexArray->vCreate(sizeof(WORD), NumSprites*NumI, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, Desc->m_bUseBufferObject, lpI);
		LYNXGLOBAL_DEL_ARRAY(lpI);
		
		CContainer::vCreate(desc);

		return LYNX_TRUE;
	}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*	@brief 。
	//*
	//*	@param 。
	//*	@param 。  
	//*	@return 。 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CSpriteGrassContainer::vCreate(const LYNXCHAR *name, int cluster_size, LPLYNXFILE lpf, long offset, LYNXBOOL bbo)
	//{
	//	MaterialSystem::CTechnique* Technique;

	//	m_Width = 10.0f;
	//	m_Height = 20.0f;
	//	MaterialSystem::CMaterial* lpMat = NULL;//((CEngine*)m_lpEngine)->GetlpMaterialSystem()->FindMaterial(name);
	//	if (!lpMat)
	//	{
	//		lpMat = vCreateMaterial();	
	//		lpMat->vCreateTechnique(MaterialSystem::CMaterial::NUM_TECHTYPE);
	//		lpMat->SetTechnique(MaterialSystem::CMaterial::SIMPLE, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).vCreatePass(1);
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).SetPass(0, LYNXNEW MaterialSystem::CPass(&(lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0))));
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).vSetNumTextureAnimation(1);
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->AddressMode = GraphicsSystem::CRenderer::AM_WRAP;
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->CombineFunc = LYNX_CF_MODULATE;
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->CombineArg[0] = LYNX_CA_CURRENT;
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->CombineArg[1] = LYNX_CA_TEXTURE;
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->CombineArg[2] = LYNX_CA_TEXTURE;		
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->Create(1);
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->LoadTexture(0, 0, name, lpf, offset);		
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_DepthBufferMode = LYNX_GraphicsSystem::CRenderer::DB_CW;
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_CullMode = LYNX_GraphicsSystem::CRenderer::CULL_NONE;		
	//		lpMat->SetVertexLayout(GraphicsSystem::gSprite3DClusterVertexLayout);
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).GetPass(0).vLoadShaders(_T("scn/sprite3dcluster.vso"), _T("builtin/common/lynxDiffuseMapXColor.pso"));

	//		lpMat->SetTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::AMBIENT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

	//		lpMat->SetTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

	//		lpMat->SetTechnique(MaterialSystem::CMaterial::POINT_LIGHTING, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

	//		lpMat->SetTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SPOT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

	//		lpMat->SetTechnique(MaterialSystem::CMaterial::ZPASS, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ZPASS) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
	//		Technique = &lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ZPASS);
	//		Technique->GetPass(0).vLoadShaders(_T("scn/sprite3dcluster_zpass.vso"), _T("scn/sprite3d_zpass.pso"));				
	//		Technique->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ZERO);
	//		Technique->GetPass(0).m_DepthBufferMode = LYNX_GraphicsSystem::CRenderer::DB_CW;						

	//		lpMat->SetTechnique(MaterialSystem::CMaterial::SHADOWMAP, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SHADOWMAP) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
	//		Technique = &lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SHADOWMAP);
	//		Technique->GetPass(0).vLoadShaders(_T("scn/sprite3dcluster_shadowmap.vso"), _T("scn/sprite3d_shadowmap.pso"));				
	//		Technique->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ZERO);
	//		Technique->GetPass(0).m_DepthBufferMode = LYNX_GraphicsSystem::CRenderer::DB_CW;						
	//		
	//		if (m_lpEngine && ((CEngine*)m_lpEngine)->GetlpMaterialSystem())
	//			((CEngine*)m_lpEngine)->GetlpMaterialSystem()->AddMaterial(lpMat);
	//	}

	//	SetlpCurrentMaterial(lpMat);
	//	SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);

	//	m_ClusterSize = cluster_size;
	//	GraphicsSystem::SPRITE3DCLUSTERVERTEX* lpV;
	//	lynxCreateVertexArray(&m_VertexArray, &GraphicsSystem::gSprite3DClusterVertexLayout, m_ClusterSize*4, LYNX_BOF_DYNAMIC|LYNX_BOF_WRITEONLY, bbo);
	//	lynxLockVertexArray(&m_VertexArray, 0, 0, LYNX_LOCK_TO_WRITE_DISCARD_OLD);						
	//		lpV = (GraphicsSystem::SPRITE3DCLUSTERVERTEX*)m_VertexArray.lpBuffer;
	//		for (int i=0; i<m_ClusterSize; ++i)
	//		{
	//			lpV[i*4+0].Pos.x = (i-50)*5;	lpV[i*4+0].Pos.y = 0.0f;	lpV[i*4+0].Pos.z = 0.0f;	lpV[i*4+0].Pos.w = 0.0f;
	//			lpV[i*4+1].Pos.x = (i-50)*5;	lpV[i*4+1].Pos.y = 0.0f;	lpV[i*4+1].Pos.z = 0.0f;	lpV[i*4+1].Pos.w = 0.0f;
	//			lpV[i*4+2].Pos.x = (i-50)*5;	lpV[i*4+2].Pos.y = 0.0f;	lpV[i*4+2].Pos.z = 0.0f;	lpV[i*4+2].Pos.w = 0.0f;
	//			lpV[i*4+3].Pos.x = (i-50)*5;	lpV[i*4+3].Pos.y = 0.0f;	lpV[i*4+3].Pos.z = 0.0f;	lpV[i*4+3].Pos.w = 0.0f;

	//			lpV[i*4+0].Offset.x = -m_Width*0.5f;	lpV[i*4+0].Offset.y = m_Height*0.5f;	lpV[i*4+0].TexCoord.u = 0.0f; lpV[i*4+0].TexCoord.v = 0.0f; lpV[i*4+0].Color.PackedColor = 0xffffffff;
	//			lpV[i*4+1].Offset.x =  m_Width*0.5f;	lpV[i*4+1].Offset.y = m_Height*0.5f;	lpV[i*4+1].TexCoord.u = 1.0f; lpV[i*4+1].TexCoord.v = 0.0f; lpV[i*4+1].Color.PackedColor = 0xffffffff;
	//			lpV[i*4+2].Offset.x = -m_Width*0.5f;	lpV[i*4+2].Offset.y = -m_Height*0.5f;	lpV[i*4+2].TexCoord.u = 0.0f; lpV[i*4+2].TexCoord.v = 1.0f; lpV[i*4+2].Color.PackedColor = 0xffffffff;
	//			lpV[i*4+3].Offset.x =  m_Width*0.5f;	lpV[i*4+3].Offset.y = -m_Height*0.5f;	lpV[i*4+3].TexCoord.u = 1.0f; lpV[i*4+3].TexCoord.v = 1.0f; lpV[i*4+3].Color.PackedColor = 0xffffffff;		
	//		}
	//	lynxUnlockVertexArray(&m_VertexArray);		

	//	lynxCreateIndexArray(&m_IndexArray, 2, m_ClusterSize*6, LYNX_BOF_STATIC|LYNX_BOF_WRITEONLY, bbo);
	//	lynxLockIndexArray(&m_IndexArray, 0, 0, LYNX_LOCK_TO_WRITE_DISCARD_OLD);			
	//		WORD* lpI = (WORD*)m_IndexArray.lpBuffer;
	//		for (int i=0; i<m_ClusterSize; ++i)
	//		{
	//			lpI[i*6+0] = i*4+0;	lpI[i*6+1] = i*4+1; lpI[i*6+2] = i*4+2; 
	//			lpI[i*6+3] = i*4+2; lpI[i*6+4] = i*4+1; lpI[i*6+5] = i*4+3; 
	//		}
	//	lynxUnlockIndexArray(&m_IndexArray);		

	//	return LYNX_TRUE;
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------	
	void CSpriteGrassContainer::IncreaseClusterSize() 
	{
		int Size = m_ClusterSize + 256;

		LYNXVECTOR4D* TempPosArray = LYNXGLOBAL_NEW LYNXVECTOR4D[Size];
		lynxMemCpy(TempPosArray, m_PosArray, m_ClusterSize*sizeof(LYNXVECTOR4D));		
		LYNXGLOBAL_DEL_ARRAY(m_PosArray);
		m_PosArray = TempPosArray;		

		LYNXVECTOR4D* TempNormalArray = LYNXGLOBAL_NEW LYNXVECTOR4D[Size];		
		lynxMemCpy(TempNormalArray, m_NormalArray, m_ClusterSize*sizeof(LYNXVECTOR4D));
		LYNXGLOBAL_DEL_ARRAY(m_NormalArray);
		m_NormalArray = TempNormalArray;
		
		float* TempRotationArray = LYNXGLOBAL_NEW float[Size];
		lynxMemCpy(TempRotationArray, m_RotationArray, m_ClusterSize*sizeof(float));
		LYNXGLOBAL_DEL_ARRAY(m_RotationArray);
		m_RotationArray = TempRotationArray;

		for (int i=m_ClusterSize; i<Size; i++)
		{
			vSetSize(i, 3.0f, 3.0f);
		}

		LYNXGLOBAL_DEL_ARRAY(m_VisPosArray);
		LYNXGLOBAL_DEL_ARRAY(m_VisNormalArray);
		LYNXGLOBAL_DEL_ARRAY(m_VisRotationArray);
		m_VisPosArray = LYNXGLOBAL_NEW LYNXVECTOR4D[Size];		
		m_VisNormalArray = LYNXGLOBAL_NEW LYNXVECTOR4D[Size];	
		m_VisRotationArray = LYNXGLOBAL_NEW float[Size];		

		m_ClusterSize = Size; 
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
	void CSpriteGrassContainer::SetRenderedSize(int size) 
	{
		if (size < m_ClusterSize) 
			m_RenderedSize = size; 
		else
		{
			do {
				IncreaseClusterSize();
			}while (size >= m_ClusterSize);
			m_RenderedSize = size; 
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
	void CSpriteGrassContainer::vSetPosition(const int index, const CVector3& pos)
	{	
		if (m_AlignmentFlag & ALIGNMENT_H_CENTER)
		{
			m_PosArray[index].x = pos.x;			
		}
		else if (m_AlignmentFlag & ALIGNMENT_H_LEFT)
		{
			m_PosArray[index].x = pos.x + m_PosArray[index].w*0.5f;			
		}
		else if (m_AlignmentFlag & ALIGNMENT_H_RIGHT)
		{
			m_PosArray[index].x = pos.x - m_PosArray[index].w*0.5f;			
		}
		
		if (m_AlignmentFlag & ALIGNMENT_V_CENTER)
		{
			m_PosArray[index].y = pos.y;
		}
		else if (m_AlignmentFlag & ALIGNMENT_V_TOP)
		{
			m_PosArray[index].y = pos.y + m_NormalArray[index].w*0.5f;
		}
		else if (m_AlignmentFlag & ALIGNMENT_V_BOTTOM)
		{
			m_PosArray[index].y = pos.y - m_NormalArray[index].w*0.5f;
		}

		m_PosArray[index].z = pos.z;
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
	void CSpriteGrassContainer::SetNormal(int index, CVector3& n)
	{		
		m_NormalArray[index].x = n.x;
		m_NormalArray[index].y = n.y;
		m_NormalArray[index].z = n.z;		
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
	void CSpriteGrassContainer::vSetSize(const int index, const CReal& w, const CReal& h)
	{	
		//m_SizeArray[index].x = w;
		//m_SizeArray[index].y = -h;

		m_PosArray[index].w = w;
		m_NormalArray[index].w = -h;
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
	void CSpriteGrassContainer::SetRotDeg(int index, CReal deg)
	{	
		m_RotationArray[index] = deg*LYNXD2R;
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
	void CSpriteGrassContainer::SetUV(int index, LPLYNXUVPOINT lpuv)
	{		
		int NumV = index*(m_NumSegX+1)*(m_NumSegY+1);
		int UVIndex = 0;
		for (int y=0; y<=m_NumSegY; y++)
		{
			for (int x=0; x<=m_NumSegX; x++)
			{
				m_lpV[NumV].TexCoord.u = lpuv[UVIndex].u;
				m_lpV[NumV].TexCoord.v = lpuv[UVIndex].v;
				NumV++;
				UVIndex++;
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
	LYNXBOOL CSpriteGrassContainer::vPlay(CReal step)
	{		
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
	void CSpriteGrassContainer::ComputeVis(CCameraContainer* cam)
	{
		CVector3 CPos;

		m_VisRenderedSize = 0;
		cam->vGetPosition(CPos);
		for (int i=0; i<m_RenderedSize; i++)
		{
			CVector3 Pos = CVector3(m_PosArray[i].x, m_PosArray[i].y, m_PosArray[i].z);
			CVector3 NewPos;
			VectorXMatrix(NewPos, Pos, m_M);			
			CVector3 Dist = NewPos - CPos;
			if (Dist.Length(3) < m_VisDistance)
			{
				m_VisPosArray[m_VisRenderedSize].x = m_PosArray[i].x;
				m_VisPosArray[m_VisRenderedSize].y = m_PosArray[i].y;
				m_VisPosArray[m_VisRenderedSize].z = m_PosArray[i].z;
				m_VisPosArray[m_VisRenderedSize].w = m_PosArray[i].w;

				m_VisNormalArray[m_VisRenderedSize].x = m_NormalArray[i].x;
				m_VisNormalArray[m_VisRenderedSize].y = m_NormalArray[i].y;
				m_VisNormalArray[m_VisRenderedSize].z = m_NormalArray[i].z;
				m_VisNormalArray[m_VisRenderedSize].w = m_NormalArray[i].w;

				m_VisRotationArray[m_VisRenderedSize] = m_RotationArray[i];

				m_VisRenderedSize++;
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
	void CSpriteGrassContainer::vRender(CCameraContainer* cam)
	{		
		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		
		ComputeVis(cam);

		m_VertexArray->vSet(0, 0);				
		m_IndexArray->vSet(0);						

		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)				
		{
			Technique.GetPass(p).Set(this, cam);
			m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();
			int NumSprites = m_VisRenderedSize;			
			int NumRenderedSprites, BaseSprites = 0;
			while (NumSprites > 0)
			{
				NumRenderedSprites = LYNX_MIN(m_BatchSize, NumSprites);

				lpR->GetCachedVertexShader()->vSetConstantF("gPositionArray", (float*)&m_VisPosArray[BaseSprites], NumRenderedSprites*4);									
				lpR->GetCachedVertexShader()->vSetConstantF("gNormalArray", (float*)&m_VisNormalArray[BaseSprites], NumRenderedSprites*4);									
				lpR->GetCachedVertexShader()->vSetConstantF("gRotationArray", (float*)&m_VisRotationArray[BaseSprites], NumRenderedSprites);									
				lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedSprites*(m_NumSegX+1)*(m_NumSegY+1), 0, NumRenderedSprites*m_NumSegX*m_NumSegY*2);

				BaseSprites += NumRenderedSprites;
				NumSprites -= m_BatchSize;
			}
		}
	}		
}