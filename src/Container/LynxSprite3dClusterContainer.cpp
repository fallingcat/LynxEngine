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
#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxCameraContainer.h>
#include <SceneSystem/Light/LynxLight.h>
#include <SceneSystem/LynxSceneSystem.h>

namespace LynxEngine 
{	
	#ifdef __MOBILE__
		int CSprite3DClusterContainer::m_BatchSize = 32; 
	#elif defined __DESKTOP__
		int CSprite3DClusterContainer::m_BatchSize = 48; 
	#endif
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CSprite3DClusterContainer::CSprite3DClusterContainer(CEngine *lpengine)	
	: CSprite3DContainer(lpengine)
	{
		m_ContainerType = SPRITE3D_CLUSTER;		
		m_RenderedSize = m_ClusterSize = 1;
		m_LockedAxis = LYNX_X_AXIS | LYNX_Y_AXIS | LYNX_Z_AXIS;
		m_lpV = NULL;
		m_PosArray = NULL;
		m_SizeArray = NULL;
		m_UVArray = NULL;
		m_ColorArray  = NULL;
		m_DegArray = NULL;
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
	CSprite3DClusterContainer::~CSprite3DClusterContainer(void)
	{			
		if (m_PosArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_PosArray);		
			m_PosArray = NULL;
		}
		if (m_SizeArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_SizeArray);
			m_SizeArray = NULL;
		}
		if (m_ColorArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_ColorArray);		
			m_ColorArray = NULL;
		}
		if (m_DegArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_DegArray);
			m_DegArray = NULL;
		}
		if (m_UVArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_UVArray);
			m_UVArray = NULL;
		}
	}			
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CSprite3DClusterContainer& CSprite3DClusterContainer::operator =(const CSprite3DClusterContainer& rhs)
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
	LYNXBOOL CSprite3DClusterContainer::vCreate(const CCreationDesc* desc)
	{
		CPlatformFileStream FileStream;
		CStream* lpStream;										
		MaterialSystem::CTechnique* Technique;
		CSprite3DClusterCreationDesc* Desc = (CSprite3DClusterCreationDesc*)desc;			
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;
		
		DDesc.Default();
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		CContainer::vCreate(desc);

		m_NumLODs = 1;		
		m_lpEngine->CreateResource(m_VertexArray, VERTEX_ARRAY);		
		m_lpEngine->CreateResource(m_IndexArray, INDEX_ARRAY);				

		m_Width = 50.0f;
		m_Height = 50.0f;
		m_NumSegX = Desc->m_NumSegX;
		m_NumSegY = Desc->m_NumSegY;

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
				lpMat->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE3D_CLUSTER));									

				lpMat->vSetNumTextureAnimation(1);
				if (Desc->m_Number > 1)
				{
					lpMat->GetTextureAnimation(0)->LoadTextureSeuence(Desc->m_Number, Desc->m_Length, *(Desc->m_lpStream), Desc->m_FileName.c_str(), Desc->m_FileExt.c_str(), &Desc->m_TextureDesc);										
				}
				else
				{
					lpMat->GetTextureAnimation(0)->Create(1);
					lpMat->GetTextureAnimation(0)->LoadTexture(0, *(Desc->m_lpStream), Desc->m_FileName.c_str(), &Desc->m_TextureDesc);						
				}				
				lpMat->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());		

				lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
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
                Technique->GetPass(0).vCreateShaderClasses("Sprite3DClusterVS", "DiffuseMapXDiffuseColorPS");
                
                #if defined(__DESKTOP__) && !defined (__MOBILE_SIMULATION__)
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::WIREFRAME);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::ZPASS);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::SHADOWMAP);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::GPASS);
                
                    Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ZPASS);
                    Technique->vCreatePass(1);
                    Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
                    Technique->GetPass(0).vCreateShaderClasses("Sprite3DClusterZPassVS", "ZPass_Velocity_AlphaTestPS");
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
                    Technique->GetPass(0).vCreateShaderClasses("Sprite3DClusterShadowMapVS", "ShadowMap_AlphaTestPS");
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
                    Technique->GetPass(0).vCreateShaderClasses("Sprite3DClusterGPassVS", "Sprite3DClusterGPassPS");
                    DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
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
                    
                    Technique = &lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE_LIGHTING);
                    Technique->vCreatePass(1);
                    Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
                    Technique->GetPass(0).vCreateShaderClasses("Sprite3DClusterGPassVS", "SimpleLightingPS");
                    DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
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
                    
                    lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::AMBIENT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
                    lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
                    lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
                    lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SPOT_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
                #elif defined (__MOBILE__) || defined (__MOBILE_SIMULATION__)
                #endif
			}
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
		}
		m_ClusterSize = Desc->m_ClusterSize;

		if (m_PosArray)
			LYNXGLOBAL_DEL_ARRAY(m_PosArray);		
		if (m_SizeArray)
			LYNXGLOBAL_DEL_ARRAY(m_SizeArray);
		if (m_ColorArray)
			LYNXGLOBAL_DEL_ARRAY(m_ColorArray);		
		if (m_DegArray)
			LYNXGLOBAL_DEL_ARRAY(m_DegArray);
		if (m_UVArray)
			LYNXGLOBAL_DEL_ARRAY(m_UVArray);

		m_PosArray = LYNXGLOBAL_NEW LYNXVECTOR3D[m_ClusterSize];		
		m_SizeArray = LYNXGLOBAL_NEW LYNXVECTOR2D[m_ClusterSize];
		m_ColorArray = LYNXGLOBAL_NEW float[m_ClusterSize*4];
		m_UVArray = LYNXGLOBAL_NEW LYNXVECTOR2D[m_ClusterSize];
		m_DegArray = LYNXGLOBAL_NEW float[m_ClusterSize];

		for (int i=0; i<m_ClusterSize; i++)
		{
			m_ColorArray[i*4] = m_ColorArray[i*4+1] = m_ColorArray[i*4+2] = m_ColorArray[i*4+3] = 1.0f;
			m_PosArray[i].x = m_PosArray[i].y = m_PosArray[i].z = 0.0f;
			m_SizeArray[i].x = m_SizeArray[i].y = 1.0f;
			m_UVArray[i].x = m_UVArray[i].y = 0.0f;
		}

		CVector2 LeftTop, RightBottom;
		if (Desc->m_UVOffset[0].x >= 0.0f && Desc->m_UVOffset[0].y >= 0.0f)
		{
			LeftTop.x = Desc->m_UVOffset[0].x;
			LeftTop.y = Desc->m_UVOffset[0].y;
			RightBottom.x = Desc->m_UVOffset[1].x;
			RightBottom.y = Desc->m_UVOffset[1].y;
		}
		else
		{
			LeftTop.x = LeftTop.y = 0.0f;
			RightBottom.x = RightBottom.y = 1.0f;
		}

		int NumV = (m_NumSegX+1)*(m_NumSegY+1);
		int CurrentV = 0;
		int NumSprites = LYNX_MIN(m_BatchSize, m_ClusterSize);
		GraphicsSystem::SPRITE3DCLUSTERVERTEX* lpV = LYNXGLOBAL_NEW GraphicsSystem::SPRITE3DCLUSTERVERTEX[NumSprites*NumV];
		for (int i=0; i<NumSprites; i++)
		{
			for (int y=0; y<=m_NumSegY; y++)
			{
				for (int x=0; x<=m_NumSegX; x++)
				{
					lpV[CurrentV].TexCoord.u = LeftTop.x + ((float)x/(float)m_NumSegX) * (RightBottom.x - LeftTop.x);		
					lpV[CurrentV].TexCoord.v = LeftTop.y + ((float)y/(float)m_NumSegY) * (RightBottom.y - LeftTop.y); 					
					lpV[CurrentV].Instance = (float)i;
					CurrentV++;
				}
			}
		}
		m_VertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE3D_CLUSTER)), NumSprites*NumV, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, Desc->m_bUseBufferObject, lpV);		
		LYNXGLOBAL_DEL_ARRAY(lpV);		

		int NumI = m_NumSegX*m_NumSegY*6;
		int CurrentIndex = 0;
		WORD* lpI = LYNXGLOBAL_NEW WORD[NumSprites*NumI];
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
	void CSprite3DClusterContainer::vSetPosition(const int index, const CVector3& pos)
	{	
		m_PosArray[index].x = pos.x;
		m_PosArray[index].y = pos.y;
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
	void CSprite3DClusterContainer::vGetPosition(const int index, CVector3& pos) const
	{	
		pos.x = m_PosArray[index].x;
		pos.y = m_PosArray[index].y;
		pos.z = m_PosArray[index].z;		
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
	void CSprite3DClusterContainer::vSetSize(const int index, const Math::CReal& w, const Math::CReal& h)
	{	
		m_SizeArray[index].x = w;
		m_SizeArray[index].y = -h;
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
	void CSprite3DClusterContainer::vGetSize(const int index, Math::CReal& w, Math::CReal& h) const
	{
		w = m_SizeArray[index].x;
		h = -m_SizeArray[index].y;
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
	void CSprite3DClusterContainer::SetColor(int index, LPLYNXCOLORRGBA lpc)
	{	
		m_ColorArray[index*4+0] = (float)lpc->Red/255.0f;
		m_ColorArray[index*4+1] = (float)lpc->Green/255.0f;
		m_ColorArray[index*4+2] = (float)lpc->Blue/255.0f;
		m_ColorArray[index*4+3] = (float)lpc->Alpha/255.0f;		
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
	void CSprite3DClusterContainer::vSetUV(const int index, LPLYNXUVPOINT lpuv)
	{		
		m_UVArray[index].x = lpuv->u;
		m_UVArray[index].y = lpuv->v;		
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
	LYNXBOOL CSprite3DClusterContainer::vPlay(CReal step)
	{		
		SetRenderedSize(m_SubContainerList.size());

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
	void CSprite3DClusterContainer::vRender(CCameraContainer* cam)
	{		
		CVector3 Dir, Pos;
		CMatrix4 VM, WVM, WVPM;

		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		
		lpR->SetVertexArray(m_VertexArray, 0, 0);				
		lpR->SetIndexArray(m_IndexArray, 0);				
		
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)				
		{
			Technique.GetPass(p).Set(this, cam);
			if (m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene())
				m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();
			int NumSprites = m_RenderedSize;			
			int NumRenderedSprites, BaseSprites = 0;
			while (NumSprites > 0)
			{
				NumRenderedSprites = LYNX_MIN(m_BatchSize, NumSprites);
				lpR->GetCachedVertexShader()->vSetConstantV3("gPositionArray", (float*)&m_PosArray[BaseSprites], NumRenderedSprites);									
				lpR->GetCachedVertexShader()->vSetConstantV2("gSizeArray", (float*)&m_SizeArray[BaseSprites], NumRenderedSprites);	
				lpR->GetCachedVertexShader()->vSetConstantV2("gUVArray", (float*)&m_UVArray[BaseSprites], NumRenderedSprites);	
				//lpR->GetCachedVertexShader()->vSetConstantF("gSizeData", (float*)&m_SizeArray[BaseSprites], 2);	
				lpR->GetCachedVertexShader()->vSetConstantV4("gColorArray", (float*)&m_ColorArray[BaseSprites*4], NumRenderedSprites);									
				lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedSprites*(m_NumSegX+1)*(m_NumSegY+1), 0, NumRenderedSprites*m_NumSegX*m_NumSegY*2);

				BaseSprites += NumRenderedSprites;
				NumSprites -= m_BatchSize;
			}
		}
	}
}