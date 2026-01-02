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
#include <Container/LynxSpriteClusterContainer.h>
#include <Container/LynxCameraContainer.h>
#include <SceneSystem/Light/LynxLight.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <Shader/LynxSpriteClusterVS.h>

namespace LynxEngine 
{	
	#ifdef __MOBILE__
		int CSpriteClusterContainer::m_BatchSize = 32; 
	#elif defined __DESKTOP__
		int CSpriteClusterContainer::m_BatchSize = 80; 
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
	CSpriteClusterContainer::CSpriteClusterContainer(CEngine *lpengine)	
	: CSpriteContainer(lpengine)
	{
		m_ContainerType = SPRITE_CLUSTER;		
		m_RenderedSize = m_ClusterSize = 1;	
		m_lpV = NULL;
		m_PosArray = NULL;
		m_SizeArray = NULL;
		m_ColorArray  = NULL;		
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
	CSpriteClusterContainer::~CSpriteClusterContainer(void)
	{			
		if (m_PosArray)
			LYNXGLOBAL_DEL_ARRAY(m_PosArray);		
		if (m_SizeArray)
			LYNXGLOBAL_DEL_ARRAY(m_SizeArray);
		if (m_ColorArray)
			LYNXGLOBAL_DEL_ARRAY(m_ColorArray);
	}			
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CSpriteClusterContainer& CSpriteClusterContainer::operator =(const CSpriteClusterContainer& rhs)
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
	LYNXBOOL CSpriteClusterContainer::vCreate(const CCreationDesc* desc)
	{
		CPlatformFileStream FileStream;
		CStream* lpStream;										
		MaterialSystem::CTechnique* Technique;
		CSpriteClusterCreationDesc* Desc = (CSpriteClusterCreationDesc*)desc;			
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

		DDesc.Default();
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		m_NumLODs = 1;		
		m_lpEngine->CreateResource(m_VertexArray, VERTEX_ARRAY);		
		m_lpEngine->CreateResource(m_IndexArray, INDEX_ARRAY);				
		
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
				if (desc->m_lpStream)
					lpStream = desc->m_lpStream;
				else
					lpStream = &FileStream;

				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpMat, MaterialSystem::CMaterial::STANDARD);
				lpMat->SetName(MatName);
				lpMat->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE3D_CLUSTER));									

				lpMat->vSetNumTextureAnimation(1);
				lpMat->GetTextureAnimation(0)->Create(1);
				lpMat->GetTextureAnimation(0)->LoadTexture(0, *lpStream, Desc->m_FileName.c_str(), &Desc->m_TextureDesc);		
				lpMat->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());		
				
				lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
				
				Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE);
				Technique->vCreatePass(1);			
				Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
				Technique->GetPass(0).CreateDepthStencilState(DDesc);				
				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				Technique->GetPass(0).CreateBlendState(BDesc);			
				RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
				RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
				Technique->GetPass(0).CreateRasterizerState(RDesc);			
				Technique->GetPass(0).vCreateShaderClasses("SpriteClusterVS", "DiffuseMapXDiffuseColorPS");								
			}
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);			
		}
		m_Width = Desc->m_Width;
		m_Height = Desc->m_Height;		
		m_ClusterSize = Desc->m_ClusterSize;
		m_PosArray = LYNXGLOBAL_NEW LYNXVECTOR2D[m_ClusterSize];		
		m_SizeArray = LYNXGLOBAL_NEW LYNXVECTOR2D[m_ClusterSize];
		m_ColorArray = LYNXGLOBAL_NEW float[m_ClusterSize*4];

		for (int i=0; i<m_ClusterSize; i++)
		{
			m_ColorArray[i*4] = m_ColorArray[i*4+1] = m_ColorArray[i*4+2] = m_ColorArray[i*4+3] = 1.0f;
			m_PosArray[i].x = m_PosArray[i].y = 0.0f;
			m_SizeArray[i].x = m_SizeArray[i].y = 1.0f;
		}

		int NumV = 4;
		int CurrentV = 0;
		int NumSprites = LYNX_MIN(m_BatchSize, m_ClusterSize);
		GraphicsSystem::SPRITE3DCLUSTERVERTEX* lpV = LYNXGLOBAL_NEW GraphicsSystem::SPRITE3DCLUSTERVERTEX[NumSprites*NumV];
		for (int i=0; i<NumSprites; i++)
		{
			for (int y=0; y<=1; y++)
			{
				for (int x=0; x<=1; x++)
				{
					lpV[CurrentV].TexCoord.u = (float)x/1.0f;		
					lpV[CurrentV].TexCoord.v = (float)y/1.0f; 					
					lpV[CurrentV].Instance = (float)i;
					CurrentV++;
				}
			}
		}
		m_VertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE3D_CLUSTER)), NumSprites*NumV, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, Desc->m_bUseBufferObject, lpV);		
		LYNXGLOBAL_DEL_ARRAY(lpV);		

		int NumI = 6;
		int CurrentIndex = 0;
		WORD* lpI = LYNXGLOBAL_NEW WORD[NumSprites*NumI];
		for (int i=0; i<NumSprites; i++)
		{
			for (int y=0; y<1; y++)
			{
				for (int x=0; x<1; x++)
				{
					lpI[CurrentIndex++] = i*NumV+y*(1+1)+x;		lpI[CurrentIndex++] = i*NumV+y*(1+1)+x+1; lpI[CurrentIndex++] = i*NumV+(y+1)*(1+1)+x; 
					lpI[CurrentIndex++] = i*NumV+(y+1)*(1+1)+x;	lpI[CurrentIndex++] = i*NumV+y*(1+1)+x+1; lpI[CurrentIndex++] = i*NumV+(y+1)*(1+1)+x+1; 
				}
			}
		}
		m_IndexArray->vCreate(sizeof(WORD), NumSprites*NumI, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, Desc->m_bUseBufferObject, lpI);
		LYNXGLOBAL_DEL_ARRAY(lpI);		

		for (int i=0; i<m_ClusterSize; i++)
			vSetSize(i, m_Width, m_Height);

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
	void CSpriteClusterContainer::vSetPosition(const int index, const CVector3& pos)
	{	
		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();

		LYNXRECT Viewport;
		lpR->GetViewportRect(&Viewport);
		int VPWidth = Viewport.x2 - Viewport.x1;
		int VPHeight = Viewport.y2 - Viewport.y1;

		m_PosArray[index].x = pos.x - VPWidth*0.5f;
		m_PosArray[index].y = VPHeight*0.5f - pos.y;		
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
	void CSpriteClusterContainer::vGetPosition(const int index, CVector3& pos) const
	{	
		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();		

		LYNXRECT Viewport;
		lpR->GetViewportRect(&Viewport);
		int VPWidth = Viewport.x2 - Viewport.x1;
		int VPHeight = Viewport.y2 - Viewport.y1;

		pos.x = m_PosArray[index].x + VPWidth*0.5f;
		pos.y = VPHeight*0.5f - m_PosArray[index].y;		
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
	void CSpriteClusterContainer::vSetSize(const int index, const Math::CReal& w, const Math::CReal& h)
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
	void CSpriteClusterContainer::vGetSize(const int index, Math::CReal& w, Math::CReal& h) const
	{	
		w =  m_SizeArray[index].x;
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
	void CSpriteClusterContainer::vSetColor(const int index, LPLYNXCOLORRGBA lpc)
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
	void CSpriteClusterContainer::vSetUV(const int index, LPLYNXUVPOINT lpuv)
	{		
		int NumV = index*(1+1)*(1+1);
		int UVIndex = 0;
		for (int y=0; y<=1; y++)
		{
			for (int x=0; x<=1; x++)
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
	LYNXBOOL CSpriteClusterContainer::vPlay(CReal step)
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
	void CSpriteClusterContainer::vRender(CCameraContainer* cam)
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

			Shader::CSpriteClusterVS* lpShader = dynamic_cast<Shader::CSpriteClusterVS*>(&(*(Technique.GetPass(p).GetVS())));

			if (m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene())
				m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();
			int NumSprites = m_RenderedSize;			
			int NumRenderedSprites, BaseSprites = 0;
			while (NumSprites > 0)
			{
				NumRenderedSprites = LYNX_MIN(m_BatchSize, NumSprites);
				lpShader->GetPositionArrayConstant()->vMap();
					lpShader->GetPositionArrayConstant()->vSetVector2((float*)&m_PosArray[BaseSprites], NumRenderedSprites);									
					lpShader->GetSizeArrayConstant()->vSetVector2((float*)&m_SizeArray[BaseSprites], NumRenderedSprites);					
					lpShader->GetColorArrayConstant()->vSetVector4((float*)&m_ColorArray[BaseSprites*4], NumRenderedSprites);									
				lpShader->GetPositionArrayConstant()->vUnmap();
				lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumRenderedSprites*4, 0, NumRenderedSprites*2);

				BaseSprites += NumRenderedSprites;
				NumSprites -= m_BatchSize;
			}
		}
	}
}