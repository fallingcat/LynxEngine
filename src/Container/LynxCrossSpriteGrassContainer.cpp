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
#include <Container/LynxCrossSpriteGrassContainer.h>
#include <Container/LynxCameraContainer.h>
#include <SceneSystem/Light/LynxLight.h>
#include <SceneSystem/LynxSceneSystem.h>

namespace LynxEngine 
{	
	int									CCrossSpriteGrassContainer::m_BatchSize = 64; 
	GraphicsSystem::CVertexLayoutPtr	CCrossSpriteGrassContainer::m_VertexLayout;
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	GraphicsSystem::CVertexLayout* CCrossSpriteGrassContainer::CreateVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[3];

		Slots[0].Stream	= 0;	Slots[0].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[0].TypeIndex = 0;	Slots[0].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number = 2;  // Texture coordinate 0 			
		Slots[1].Stream	= 0;	Slots[1].Type = GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[1].TypeIndex = 1;	Slots[1].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number = 2;  // Direction 			
		Slots[2].Stream	= 0;	Slots[2].Type = GraphicsSystem::CRenderer::VST_POSITION;		Slots[2].TypeIndex = 0;	Slots[2].Format = GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number = 1;  // Instance
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 3);
		v->SetName(CString(_T("CrossSpriteGrass Vertex Layout")));

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
	void CCrossSpriteGrassContainer::CreateVertexLayout(void)
	{
		if (!m_VertexLayout && m_lpEngine->GetlpGraphicsSystem() && m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer())
		{
			m_lpEngine->CreateVertexLayout(m_VertexLayout, CString(_T("CrossSpriteGrass Vertex Layout")));
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
	CCrossSpriteGrassContainer::CCrossSpriteGrassContainer(CEngine *lpengine)	
	: CContainer(lpengine)
	{
		m_ContainerType = CROSS_GRASS;
		m_NumSegX = m_NumSegY = 1;
		m_RenderedSize = 1;
		m_ClusterSize = 1024;		
		m_lpV = NULL;
		m_InstanceArray.clear();
		m_VisInstanceArray.clear();
		m_VisPosArray = NULL;
		m_VisNormalArray = NULL;
		m_VisRotationArray = NULL;
		m_bFadeOut = LYNX_TRUE;
		m_bSort = LYNX_FALSE;
		m_VisDistance = 200.0f;
		m_FadeOutDistance = 400.0f;
		m_ColliderData[0] = 0.0f;
		m_ColliderData[1] = 0.0f;
		m_ColliderData[2] = 0.0f;
		m_ColliderData[3] = 0.0f;

		CreateVertexLayout();

		IMPLEMENT_PARAM_FLOAT(m_Width, 0)
		IMPLEMENT_PARAM_FLOAT(m_Height, 0)
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bFadeOut, "Fade out grass or not.", 0)
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bSort, "Sort fade out grass or not.", 0)
		IMPLEMENT_PARAM_FLOAT_COMMENT(m_VisDistance, "The visible distance of each grass.", 0)
		IMPLEMENT_PARAM_FLOAT_COMMENT(m_FadeOutDistance, "The fade out distance of each grass.", 0)
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
	CCrossSpriteGrassContainer::~CCrossSpriteGrassContainer(void)
	{			
		m_InstanceArray.clear();
		m_VisInstanceArray.clear();
		
		if (m_VisPosArray)
			LYNXGLOBAL_DEL_ARRAY(m_VisPosArray);
		if (m_VisNormalArray)
			LYNXGLOBAL_DEL_ARRAY(m_VisNormalArray);				
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
	CCrossSpriteGrassContainer& CCrossSpriteGrassContainer::operator =(const CCrossSpriteGrassContainer& rhs)
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
	LYNXBOOL CCrossSpriteGrassContainer::vCreate(const CCreationDesc* desc)
	{
		MaterialSystem::CTechnique* Technique;
		CCrossSpriteGrassCreationDesc* Desc = (CCrossSpriteGrassCreationDesc*)desc;			
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		
		DDesc.Default();
		BDesc.Default();
		RDesc.Default();		
		
		CreateVertexLayout();
		
		if (desc->m_bCreateMaterial)
		{
			CString MatName = desc->m_FileName + CString(_T(".CrossSpriteGrass Material"));
			MaterialSystem::CMaterialPtr lpMat = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MatName);
			if (!lpMat)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpMat, MaterialSystem::CMaterial::STANDARD);			
				lpMat->SetName(MatName);
				lpMat->SetVertexLayout(m_VertexLayout);
				lpMat->m_TransparentType = TT_OPAQUE_TRANSLUCENT;

				lpMat->vSetNumTextureAnimation(1);
				lpMat->GetTextureAnimation(0)->Create(1);				
				lpMat->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
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
				BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
				BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);			
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);							
				Technique->GetPass(0).vCreateShaderClasses("CrossSpriteGrassVS", "DiffuseMapXColorPS");				

				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ZPASS);
				Technique->vCreatePass(1);					
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				Technique->GetPass(0).vCreateShaderClasses("CrossSpriteGrassZPassVS", "ZPass_Velocity_AlphaTestPS");								
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);		
				BDesc.RTBlendParam[0].bBlend	= LYNX_FALSE;
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
				Technique->GetPass(0).vCreateShaderClasses("CrossSpriteGrassShadowMapVS", "ShadowMap_AlphaTestPS");				
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
				Technique->GetPass(0).vCreateShaderClasses("CrossSpriteGrassGPassVS", "SpriteGrassGPassPS");				
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);									
				BDesc.RTBlendParam[0].bBlend	= LYNX_FALSE;
				BDesc.RTBlendParam[1].bBlend	= LYNX_FALSE;
				BDesc.RTBlendParam[2].bBlend	= LYNX_FALSE;
				BDesc.RTBlendParam[3].bBlend	= LYNX_FALSE;
				Technique->GetPass(0).CreateBlendState(BDesc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).m_bUseFog = LYNX_FALSE;			

				Technique = &lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE_LIGHTING);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				Technique->GetPass(0).vCreateShaderClasses("CrossSpriteGrassVS", "SimpleLightingPS");				
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

				Technique = &lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::AMBIENT_LIGHTING);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				Technique->GetPass(0).vCreateShaderClasses("CrossSpriteGrassFadeOutVS", "SpriteGrassFadeOutAmbLightingPS");				
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);									
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).m_bUseFog = LYNX_FALSE;			

				Technique = &lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);
				Technique->vCreatePass(1);
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				Technique->GetPass(0).vCreateShaderClasses("CrossSpriteGrassFadeOutVS", "SpriteGrassFadeOutDirLightingPS");				
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);									
				BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE;
				BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);				
				RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);
				Technique->GetPass(0).m_bUseFog = LYNX_FALSE;	

				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SPOT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);

			}
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
		}

		m_NumLODs = 1;		
		m_lpEngine->CreateResource(m_VertexArray, VERTEX_ARRAY);		
		m_lpEngine->CreateResource(m_IndexArray, INDEX_ARRAY);		

		m_Width = Desc->m_BaseWidth;
		m_Height = Desc->m_BaseHeight;
		m_NumSegX = Desc->m_NumSegX;
		m_NumSegY = Desc->m_NumSegY;
		m_ClusterSize = Desc->m_ClusterSize;
		m_NumBlades = LYNX_MAX(Desc->m_NumBlades, 1);

		m_InstanceArray.resize(m_ClusterSize);		
		m_VisInstanceArray.resize(m_ClusterSize);		
		m_VisPosArray = LYNXGLOBAL_NEW LYNXVECTOR4D[m_ClusterSize];				
		m_VisNormalArray = LYNXGLOBAL_NEW LYNXVECTOR4D[m_ClusterSize];			
		m_VisRotationArray = LYNXGLOBAL_NEW LYNXVECTOR4D[m_ClusterSize];				
		
		int NumV = m_NumBlades*(m_NumSegX+1)*(m_NumSegY+1);
		int CurrentV = 0;
		int NumSprites = LYNX_MIN(m_BatchSize, m_ClusterSize);
		m_VertexArray->vCreate(m_VertexLayout, NumSprites*NumV, GraphicsSystem::CRenderer::DYNAMIC|GraphicsSystem::CRenderer::WRITEONLY, Desc->m_bUseBufferObject);		
		GRASSVERTEX* lpV = reinterpret_cast<GRASSVERTEX*>(m_VertexArray->vMap(0, 0, GraphicsSystem::CRenderer::LOCK_TO_WRITE));						
			for (int i=0; i<NumSprites; i++)
			{
				for (int s=0; s<m_NumBlades; s++)
				{
					for (int y=0; y<=m_NumSegY; y++)
					{
						for (int x=0; x<=m_NumSegX; x++)
						{
							lpV[CurrentV].TexCoord.u = (float)x/(float)m_NumSegX;		
							lpV[CurrentV].TexCoord.v = (float)y/(float)m_NumSegY; 										
							lpV[CurrentV].Dir[0] = lynxCos(s*(180.0f/m_NumBlades));
							lpV[CurrentV].Dir[1] = lynxSin(s*(180.0f/m_NumBlades));							
							lpV[CurrentV].Instance = i;
							CurrentV++;
						}
					}
				}
			}
		m_VertexArray->vUnmap();		

		int NumVPerInstance = (m_NumSegX+1)*(m_NumSegY+1);
		int NumI = m_NumSegX*m_NumSegY*6*m_NumBlades;
		int CurrentIndex = 0;
		WORD* lpI = LYNXGLOBAL_NEW WORD [NumSprites*NumI];
		for (int i=0; i<NumSprites; i++)
		{
			for (int s=0; s<m_NumBlades; s++)
			{
				for (int y=0; y<m_NumSegY; y++)
				{
					for (int x=0; x<m_NumSegX; x++)
					{					
						lpI[CurrentIndex++] = i*NumV+(s*NumVPerInstance)+y*(m_NumSegX+1)+x;		lpI[CurrentIndex++] = i*NumV+(s*NumVPerInstance)+y*(m_NumSegX+1)+x+1; lpI[CurrentIndex++] = i*NumV+(s*NumVPerInstance)+(y+1)*(m_NumSegX+1)+x; 
						lpI[CurrentIndex++] = i*NumV+(s*NumVPerInstance)+(y+1)*(m_NumSegX+1)+x;	lpI[CurrentIndex++] = i*NumV+(s*NumVPerInstance)+y*(m_NumSegX+1)+x+1; lpI[CurrentIndex++] = i*NumV+(s*NumVPerInstance)+(y+1)*(m_NumSegX+1)+x+1; 					
					}
				}
			}
		}
		m_IndexArray->vCreate(sizeof(WORD), NumSprites*NumI, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, Desc->m_bUseBufferObject, lpI);
		LYNXGLOBAL_DEL_ARRAY(lpI);
		
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
	void CCrossSpriteGrassContainer::IncreaseClusterSize() 
	{
		int Size = m_ClusterSize + 256;

		m_InstanceArray.resize(Size);
		m_VisInstanceArray.resize(Size);
		
		for (int i=m_ClusterSize; i<Size; i++)
		{
			vSetSize(i, 3.0f, 3.0f);
		}

		LYNXGLOBAL_DEL_ARRAY(m_VisPosArray);
		LYNXGLOBAL_DEL_ARRAY(m_VisNormalArray);
		LYNXGLOBAL_DEL_ARRAY(m_VisRotationArray);		
		m_VisPosArray = LYNXGLOBAL_NEW LYNXVECTOR4D[Size];		
		m_VisNormalArray = LYNXGLOBAL_NEW LYNXVECTOR4D[Size];	
		m_VisRotationArray = LYNXGLOBAL_NEW LYNXVECTOR4D[Size];		

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
	void CCrossSpriteGrassContainer::SetRenderedSize(int size) 
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
	void CCrossSpriteGrassContainer::Remove(int index) 
	{
		m_InstanceArray.erase((m_InstanceArray.begin()+index));		
		m_RenderedSize--;
		m_ClusterSize--;
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
	void CCrossSpriteGrassContainer::vSetPosition(const int index, const CVector3& pos)
	{	
		m_InstanceArray[index].Pos.x = pos.x;					
		m_InstanceArray[index].Pos.y = pos.y;
		m_InstanceArray[index].Pos.z = pos.z;
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
	void CCrossSpriteGrassContainer::SetNormal(int index, CVector3& n)
	{		
		m_InstanceArray[index].Normal.x = n.x;
		m_InstanceArray[index].Normal.y = n.y;
		m_InstanceArray[index].Normal.z = n.z;		
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
	void CCrossSpriteGrassContainer::vSetSize(const int index, const CReal& w, const CReal& h)
	{	
		//m_SizeArray[index].x = w;
		//m_SizeArray[index].y = -h;

		m_InstanceArray[index].Pos.w = w;
		m_InstanceArray[index].Normal.w = -h;
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
	void CCrossSpriteGrassContainer::SetRotDeg(int index, CVector3& r)
	{	
		m_InstanceArray[index].Rotation.x = r.x*LYNXD2R;
		m_InstanceArray[index].Rotation.y = r.y*LYNXD2R;
		m_InstanceArray[index].Rotation.z = r.z*LYNXD2R;
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
	void CCrossSpriteGrassContainer::SetAlpha(int index, Math::CReal& a)
	{	
		m_InstanceArray[index].Rotation.w = a;
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
	void CCrossSpriteGrassContainer::SetUV(int index, LPLYNXUVPOINT lpuv)
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
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CCrossSpriteGrassContainer::vAttachToTerrain(CTerrainContainer& t, LYNXBOOL balign)
	{
		LynxEngine::Math::CMatrix3 M, RM3, OM;
		LynxEngine::Math::CMatrix4 RM;
		LynxEngine::Math::CVector3 Euler, Normal, InstancePos;
		LynxEngine::Math::CVector3 OTangent, TTangent;
		float Deg;

		for (int i=0; i<m_RenderedSize; i++)
		{
			GetRotDeg(i, Euler);
			lynxEulerAnglesXYZ2Matrix(&RM, &Euler);
			RM.GetRight(OTangent);
			vGetPosition(i, InstancePos);
			InstancePos.y = t.GetHeight(InstancePos); 	
			t.GetNormal(Normal, InstancePos);
			vSetPosition(i, InstancePos);								
			SetNormal(i, Normal);

			if (balign)
			{				
				t.GetOrientation(M, InstancePos);									
				M.GetRight(TTangent);
				Deg = acosf(LynxEngine::Math::Dot(TTangent, OTangent))*LYNXR2D;
				lynxMakeRotateMatrixRespectVector(RM, Normal, Deg);
				RM3.m[0][0] = RM.m[0][0];	RM3.m[0][1] = RM.m[0][1];	RM3.m[0][2] = RM.m[0][2];
				RM3.m[1][0] = RM.m[1][0];	RM3.m[1][1] = RM.m[1][1];	RM3.m[1][2] = RM.m[1][2];
				RM3.m[2][0] = RM.m[2][0];	RM3.m[2][1] = RM.m[2][1];	RM3.m[2][2] = RM.m[2][2];									
				OM = M*RM3;														
				OM.GetRotation(Euler);
				SetRotDeg(i, Euler);								
			}
			else
			{				
				SetRotDeg(i, Euler);																	
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
	LYNXBOOL CCrossSpriteGrassContainer::vPlay(CReal step)
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
	void CCrossSpriteGrassContainer::ComputeTranslucentVis(CCameraContainer* cam)
	{
		CVector3 CPos;		
		CList<VISINSTANCE*> TempList;
		CList<VISINSTANCE*>::CIterator Ins;
		float FadeOutDist = m_FadeOutDistance-m_VisDistance;

		TempList.clear();
		for (int i=0; i<m_RenderedSize; i++)
		{
			if (!m_InstanceArray[i].bVisible)
			{
				m_VisInstanceArray[i].Distance2Camera = lynxSqrt(m_InstanceArray[i].SquareDist2Camera);
				if (m_VisInstanceArray[i].Distance2Camera > m_VisDistance && m_VisInstanceArray[i].Distance2Camera <= (m_FadeOutDistance))
				{
					m_VisInstanceArray[i].Index = i;
					Math::CReal Alpha(LYNX_MIN(LYNX_MAX(1.0f - (m_VisInstanceArray[i].Distance2Camera-m_VisDistance)/(FadeOutDist), 0.0f), 1.0f));
					SetAlpha(i, Alpha);										
					TempList.push_back(&m_VisInstanceArray[i]);
				}
			}
		}	
		if (m_bSort)
			TempList.sort(compare_back2front);

		m_VisRenderedSize = 0;
		for (Ins = TempList.begin(); Ins != TempList.end(); ++Ins)
		{			
			m_VisPosArray[m_VisRenderedSize] = m_InstanceArray[(*Ins)->Index].Pos;
			m_VisNormalArray[m_VisRenderedSize] = m_InstanceArray[(*Ins)->Index].Normal;
			m_VisRotationArray[m_VisRenderedSize] = m_InstanceArray[(*Ins)->Index].Rotation;
			m_VisRenderedSize++;
		}	
		TempList.clear();
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
	void CCrossSpriteGrassContainer::ComputeVis(CCameraContainer* cam)
	{
		CVector3 CPos;
		float SquareVisDist = m_VisDistance*m_VisDistance;

		#ifdef __TOOL__
			if (m_CurrentMaterialTechnique == MaterialSystem::CMaterial::SIMPLE ||
				m_CurrentMaterialTechnique == MaterialSystem::CMaterial::SIMPLE_LIGHTING)
			{
				m_VisRenderedSize = 0;
				for (int i=0; i<m_RenderedSize; i++)
				{
					SetAlpha(i, Math::CReal(1.0f));
					m_VisPosArray[m_VisRenderedSize] = m_InstanceArray[i].Pos;
					m_VisNormalArray[m_VisRenderedSize] = m_InstanceArray[i].Normal;
					m_VisRotationArray[m_VisRenderedSize] = m_InstanceArray[i].Rotation;

					m_VisRenderedSize++;
				}
			}			 
			else if (m_CurrentMaterialTechnique == MaterialSystem::CMaterial::ZPASS) // Store instance visibility on Z pass then retreive the instance visibility for following passes
			{
				m_VisRenderedSize = 0;
				cam->vGetPosition(CPos);
				for (int i=0; i<m_RenderedSize; i++)
				{
					m_InstanceArray[i].bVisible = LYNX_FALSE;
					CVector3 Pos;
					vGetPosition(i, Pos);
					CVector3 NewPos;
					VectorXMatrix(NewPos, Pos, m_M);			
					CVector3 Dist = NewPos - CPos;
					m_InstanceArray[i].SquareDist2Camera = Math::Dot(Dist, Dist);
					if (m_InstanceArray[i].SquareDist2Camera <= SquareVisDist)
					{
						SetAlpha(i, Math::CReal(1.0f));
						m_VisPosArray[m_VisRenderedSize] = m_InstanceArray[i].Pos;
						m_VisNormalArray[m_VisRenderedSize] = m_InstanceArray[i].Normal;
						m_VisRotationArray[m_VisRenderedSize] = m_InstanceArray[i].Rotation;
						m_InstanceArray[i].bVisible = LYNX_TRUE;

						m_VisRenderedSize++;
					}
				}
			}
		#else
			// Store instance visibility on Z pass then retreive the instance visibility for following passes
			if (m_CurrentMaterialTechnique == MaterialSystem::CMaterial::ZPASS)
			{
				m_VisRenderedSize = 0;
				cam->vGetPosition(CPos);
				for (int i=0; i<m_RenderedSize; i++)
				{
					m_InstanceArray[i].bVisible = LYNX_FALSE;
					CVector3 Pos;
					vGetPosition(i, Pos);
					CVector3 NewPos;
					VectorXMatrix(NewPos, Pos, m_M);			
					CVector3 Dist = NewPos - CPos;
					m_InstanceArray[i].SquareDist2Camera = Math::Dot(Dist, Dist);
					if (m_InstanceArray[i].SquareDist2Camera <= SquareVisDist)
					{
						Math::CReal Alpha(1.0f);
						SetAlpha(i, Alpha);
						m_VisPosArray[m_VisRenderedSize] = m_InstanceArray[i].Pos;
						m_VisNormalArray[m_VisRenderedSize] = m_InstanceArray[i].Normal;
						m_VisRotationArray[m_VisRenderedSize] = m_InstanceArray[i].Rotation;
						m_InstanceArray[i].bVisible = LYNX_TRUE;

						m_VisRenderedSize++;
					}
				}
			}
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
	void CCrossSpriteGrassContainer::vRender(CCameraContainer* cam)
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

				lpR->GetCachedVertexShader()->vSetConstantF("gColliderData", m_ColliderData, 4);			
				lpR->GetCachedVertexShader()->vSetConstantF("gPositionArray", (float*)&m_VisPosArray[BaseSprites], NumRenderedSprites*4);									
				lpR->GetCachedVertexShader()->vSetConstantF("gNormalArray", (float*)&m_VisNormalArray[BaseSprites], NumRenderedSprites*4);									
				lpR->GetCachedVertexShader()->vSetConstantF("gRotationArray", (float*)&m_VisRotationArray[BaseSprites], NumRenderedSprites*4);									
				lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedSprites*(m_NumSegX+1)*(m_NumSegY+1)*m_NumBlades, 0, NumRenderedSprites*m_NumSegX*m_NumSegY*2*m_NumBlades);

				BaseSprites += NumRenderedSprites;
				NumSprites -= m_BatchSize;
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
	void CCrossSpriteGrassContainer::vTranslucentRender(CCameraContainer* cam)
	{	
		if (m_bFadeOut)
		{
			GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();		

			ComputeTranslucentVis(cam);
			
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

					lpR->GetCachedVertexShader()->vSetConstantF("gColliderData", m_ColliderData, 4);			
					lpR->GetCachedVertexShader()->vSetConstantF("gPositionArray", (float*)&m_VisPosArray[BaseSprites], NumRenderedSprites*4);									
					lpR->GetCachedVertexShader()->vSetConstantF("gNormalArray", (float*)&m_VisNormalArray[BaseSprites], NumRenderedSprites*4);									
					lpR->GetCachedVertexShader()->vSetConstantF("gRotationArray", (float*)&m_VisRotationArray[BaseSprites], NumRenderedSprites*4);									
					lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedSprites*(m_NumSegX+1)*(m_NumSegY+1)*m_NumBlades, 0, NumRenderedSprites*m_NumSegX*m_NumSegY*2*m_NumBlades);

					BaseSprites += NumRenderedSprites;
					NumSprites -= m_BatchSize;
				}
			}
		}
	}
}