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
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/StaticObj/LynxSky.h>
#include <Container/LynxSkyLightMap.h>
#include <Container/LynxSkyDomeContainer.h>
#include <Shader/LynxSkyDomeVS.h>
#include <Shader/LynxSkyDomePS.h>
#include <Shader/LynxScreenQuadVS.h>
#include <Shader/LynxSkyLightPS.h>

namespace LynxEngine 
{		
	GraphicsSystem::CVertexLayoutPtr CSkyDomeContainer::m_VertexLayout;
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------	
	void CSkyDomeContainer::UpdateParam()
	{		
		LYNXBOOL	bMoon;
		float		g2;

		m_SunDir.x = 0.0f;
	    m_SunDir.y = (float)lynxCos(m_Time);
	    m_SunDir.z = (float)lynxSin(m_Time);

		m_Time = fabs(m_Time);
		/*while (m_Time >= 360.0f)
		{
			m_Time -= 360.0f;
		}*/
		
		bMoon = LYNX_FALSE;
		/*if (m_Time <= 60.0f)
			m_ESun = (60.0f-m_Time)*20.0f/60.0f + 10.0f;
		else if (m_Time >= 300.0f)
			m_ESun = (m_Time-300.0f)*20.0f/60.0f + 10.0f;
		else if (m_Time >= 120.0f && m_Time <= 260.0f)
		{
			m_ESun = 5.0f;
			bMoon = LYNX_TRUE;
		}
		else */
			//m_ESun = 10.0f;

		m_ESun = m_SunIntensity*lynxCos(m_Time);
		if (m_ESun < 1.0f)
			m_ESun = 1.0f;		
		//m_ESun += 200.0f;

		//m_ESun = 500.0f;

		if (!bMoon)
		{
			m_Kr4PI = m_Kr * 4.0f * LYNXPI;		
			m_Km4PI = m_Km * 4.0f * LYNXPI;		
			m_KrESun = m_ESun * m_Kr;
			m_KmESun = m_ESun * m_Km;		
			g2 = m_g * m_g;			
			m_OuterRadius = m_InnerRadius * 1.025f;
			m_Scale = 1.0f / (m_OuterRadius - m_InnerRadius);
			
			m_InvWavelength.x = 1.0f / powf(m_Color.Red/255.0f, m_ColorPower);
			m_InvWavelength.y = 1.0f / powf(m_Color.Green/255.0f, m_ColorPower);
			m_InvWavelength.z = 1.0f / powf(m_Color.Blue/255.0f, m_ColorPower);
			m_HG.x = 1.5f * ( (1.0f - g2) / (2.0f + g2) );
			m_HG.y = 1.0f + g2;
			m_HG.z = 2.0f * m_g;
		}
		else
		{
			m_SunDir.x = 0.0f;
			m_SunDir.y = (float)lynxCos(m_Time+180.0f);
			m_SunDir.z = (float)lynxSin(m_Time+180.0f);

			float MoonKr = m_Kr/3.0f;//0.001;
			float MoonKm = m_Km/3.3f;//0.0003
			m_ESun = 2.0f;
			m_Kr4PI = MoonKr * 4.0f * LYNXPI;		
			m_Km4PI = MoonKm * 4.0f * LYNXPI;		
			m_KrESun = m_ESun * MoonKr;
			m_KmESun = m_ESun * MoonKm;		
			g2 = m_g * m_g;			
			m_OuterRadius = m_InnerRadius * 1.025f;
			m_Scale = 1.0f / (m_OuterRadius - m_InnerRadius);
			
			m_InvWavelength.x = 1.0f / powf(m_Color.Red/255.0f, m_ColorPower);
			m_InvWavelength.y = 1.0f / powf(m_Color.Green/255.0f, m_ColorPower);
			m_InvWavelength.z = 1.0f / powf(m_Color.Blue/255.0f, m_ColorPower);
			m_HG.x = 1.5f * ( (1.0f - g2) / (2.0f + g2) );
			m_HG.y = 1.0f + g2;
			m_HG.z = 2.0f * m_g;
		}		
		m_EyePos.x = 0.0f;
		m_EyePos.y = m_InnerRadius;
		m_EyePos.z = 0.0f;		


		/*
		m_Kr4PI = m_Kr * 4.0f * LYNXPI;		
		m_Km4PI = m_Km * 4.0f * LYNXPI;		
		m_KrESun = m_ESun * m_Kr;
		m_KmESun = m_ESun * m_Km;		
		m_g2 = m_g * m_g;		
		m_InnerRadius2 = m_InnerRadius * m_InnerRadius;
		m_OuterRadius = m_InnerRadius * 1.025f;
		m_OuterRadius2 = m_OuterRadius * m_OuterRadius;
		m_Scale = 1.0f / (m_OuterRadius - m_InnerRadius);

		m_ScaleOverScaleDepth = m_Scale / m_RayleighScaleDepth;		
		m_InvWavelength4.x = 1.0f / powf(0.650f, 4.0f);
		m_InvWavelength4.y = 1.0f / powf(0.570f, 4.0f);
		m_InvWavelength4.z = 1.0f / powf(0.475f, 4.0f);
		m_HG.x = 1.5f * ( (1.0f - m_g2) / (2.0f + m_g2) );
		m_HG.y = 1.0f + m_g2;
		m_HG.z = 2.0f * m_g;
		*/
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
	void CSkyDomeContainer::InitMembers()
	{
		float		g2;

		m_ResourceType = CONTAINER;
		m_ContainerType = SKYDOME;	
		m_lpSunLight = NULL;
		m_lpSkyLightMap = NULL;

		m_SunIntensity = 50.0f;
		m_SunLightIntensityScale = 0.1f;
		m_NumSamples = 16;
		m_Time = 0.0f;
		m_SkyLightMapSize = 1024;
		m_InnerRadius = 100;
		m_Kr = 0.0025f;
		m_Kr4PI = m_Kr * 4.0f * LYNXPI;
		m_Km = 0.0010f;
		m_Km4PI = m_Km * 4.0f * LYNXPI;
		m_ESun = 20.0f*30.0f;
		m_KrESun = m_ESun * m_Kr;
		m_KmESun = m_ESun * m_Km;
		m_g = -0.990f;
		g2 = m_g * m_g;
		//m_Exposure = -2.0f;
		m_OuterRadius = m_InnerRadius * 1.025f;
		m_Scale = 1.0f / (m_OuterRadius - m_InnerRadius);

		m_RayleighScaleDepth = 0.25f;
		//m_MieScaleDepth = 0.1f;

		m_Color.Red = 0.650f*255.0f;
		m_Color.Green = 0.570f*255.0f;
		m_Color.Blue = 0.475f*255.0f;
		m_ColorPower = 4.0f;

		m_HG.x = 1.5f * ( (1.0f - g2) / (2.0f + g2) );
		m_HG.y = 1.0f + g2;
		m_HG.z = 2.0f * m_g;
		
		//m_nSize = 256; //Higher, Better, More CPU/GPU
		//m_nSamples = 16;//Higher, Better, More CPU/GPU
		/*m_Kr = 0.0025f;		
		m_Km = 0.0010f;		
		m_ESun = 20.0f;		
		m_g = -0.990f;		
		m_Exposure = -2.0f;
		m_RayleighScaleDepth = 0.25f;
		m_MieScaleDepth = 0.1f;
		
		UpdateParam();*/

		IMPLEMENT_PARAM_FLOAT(m_SunIntensity, 0)
		IMPLEMENT_PARAM_FLOAT(m_SunLightIntensityScale, 0)			
		IMPLEMENT_PARAM_INT(m_NumSamples, 0)
		IMPLEMENT_PARAM_RANGE_FLOAT(m_Time, 0.0f, 360.0f, 3600.0f, 0)
		IMPLEMENT_PARAM_RANGE_FLOAT(m_Kr, 0.0f, 0.1f, 1000.0f, 0)			
		IMPLEMENT_PARAM_RANGE_FLOAT(m_Km, -0.05f, 0.05f, 1000.0f, 0)
		IMPLEMENT_PARAM_FLOAT(m_g, 0)				
		//IMPLEMENT_PARAM_FLOAT(m_Exposure, 0)
		IMPLEMENT_PARAM_RANGE_FLOAT(m_RayleighScaleDepth, 0.0f, 2.0f, 500.0f, 0)
		//IMPLEMENT_PARAM_FLOAT(m_MieScaleDepth, 0)
		IMPLEMENT_PARAM_URGB(m_Color, 0)
		IMPLEMENT_PARAM_FLOAT(m_ColorPower, 0)
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
	CSkyDomeContainer::CSkyDomeContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		InitMembers();
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
	CSkyDomeContainer::~CSkyDomeContainer(void)
	{			
		LYNXDEL(m_lpSkyLightMap);
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
	void CSkyDomeContainer::vSetNumLODs(int lod)
	{
		m_NumLODs = lod;		
		m_PrimitiveList = LYNXGLOBAL_NEW CArray<GraphicsSystem::CRenderer::PRIMITIVE>[lod];
		m_VertexArray = LYNXGLOBAL_NEW GraphicsSystem::CVertexArrayPtr[lod];
		m_IndexArray = LYNXGLOBAL_NEW GraphicsSystem::CIndexArrayPtr[lod];
		for (int i=0; i<m_NumLODs; i++)
		{
			m_lpEngine->CreateResource(m_VertexArray[i], VERTEX_ARRAY);
			m_lpEngine->CreateResource(m_IndexArray[i], INDEX_ARRAY);			
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
	LYNXBOOL CSkyDomeContainer::Setup(CReal radius, int slices, int sides, LYNXBOOL exponential)
	{		
		m_InnerRadius = radius;
		UpdateParam();

		m_NumVertices = (slices+1)*(sides+1);
		int NumSides = sides;
		int NumSlices = slices;
		int NumIndices = slices * (sides + 1) * 2;
		int SkyRadius = radius;
		float polyAng = (2.0f * LYNXPI / sides)*LYNXR2D, ang;
		float vx, vy, vz;

		vSetNumLODs(1);

		SKYVERTEX* lpV = LYNXGLOBAL_NEW SKYVERTEX[m_NumVertices];			
		int DomeIndex = 0;
		for( int j = 0; j <= slices; j++ )		
		{
			const float MoveY = (LYNXPI * 2.0f) * j / ((float)slices) ;			
			for(int i = 0; i<=sides; i++ )
			{	
				const float MoveXZ = cos( 1.0f ) * i / ((float)sides) ;//0.15
				lpV[DomeIndex].Pos.x = sin( MoveXZ ) * cos( MoveY  );
				lpV[DomeIndex].Pos.y = cos( MoveXZ );
				lpV[DomeIndex].Pos.z = sin( MoveXZ ) * sin( MoveY  );

				//lpV[DomeIndex].Pos.x = j;
				//lpV[DomeIndex].Pos.y = i;
				//lpV[DomeIndex].Pos.z = 0;

				lpV[DomeIndex].Pos.x *= m_OuterRadius;
				lpV[DomeIndex].Pos.y *= m_OuterRadius;
				lpV[DomeIndex].Pos.z *= m_OuterRadius;

				lpV[DomeIndex].TexCoord.u = i / ((float)sides);
				lpV[DomeIndex].TexCoord.v = j / ((float)slices);
				DomeIndex++;
			}
		}
		m_VertexArray[0]->vCreate(m_VertexLayout, m_NumVertices, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpV);
		LYNXGLOBAL_DEL_ARRAY(lpV);

		WORD *lpI = LYNXGLOBAL_NEW WORD[NumIndices];
		int ind = 0;
		for (int j = 1; j <= slices; j++)
		{
			for (int i = 0; i <= sides; i++)
			{			
				lpI[ind++] =  j      * (sides + 1) + i;
				lpI[ind++] = (j - 1) * (sides + 1) + i;
			}
		}	
		m_IndexArray[0]->vCreate(sizeof(WORD), NumIndices, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpI);	
		LYNXGLOBAL_DEL_ARRAY(lpI);

		m_PrimitiveList[0].resize(NumSlices);
		for (int i = 0; i < NumSlices; i++)
		{
			m_PrimitiveList[0][i].Type = GraphicsSystem::CRenderer::TRIANGLE_STRIP;
			m_PrimitiveList[0][i].BaseVertex = 0;
			m_PrimitiveList[0][i].BaseIndex = i * (NumSides + 1) * 2;
			m_PrimitiveList[0][i].NumIndex = (NumSides + 1) * 2;
			m_PrimitiveList[0][i].NumPrimitives = m_PrimitiveList[0][i].NumIndex - 2;
		}


		/*
		m_PrimitiveList[0].resize(1);
		m_PrimitiveList[0][0].Type = LYNX_PRIMITIVE_TRIANGLE_STRIP;
		m_PrimitiveList[0][0].BaseVertex = 0;
		m_PrimitiveList[0][0].BaseIndex = 0;
		m_PrimitiveList[0][0].NumIndex = NumIndices;
		m_PrimitiveList[0][0].NumPrimitives = NumIndices-2;
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
	GraphicsSystem::CVertexLayout* CSkyDomeContainer::CreateVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[2];

		Slots[0].Stream		= 0;	Slots[0].Type	= GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex	= 0;	Slots[0].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number		= 3;  // Position in object space 
		Slots[1].Stream		= 0;	Slots[1].Type	= GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[1].TypeIndex	= 0;	Slots[1].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number		= 2;  // Texture coordinate 0			 
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 2);
		v->SetName(CString(_T("SkyDome Vertex Layout")));
		
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
	void CSkyDomeContainer::CreateVertexLayout(void)
	{
		if (!m_VertexLayout && m_lpEngine->GetlpGraphicsSystem() && m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer())
		{
			m_lpEngine->CreateVertexLayout(m_VertexLayout, CString(_T("SkyDome Vertex Layout")));			
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
	LYNXBOOL CSkyDomeContainer::vCreate(const CCreationDesc* desc)
	{
		LynxEngine::GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		LynxEngine::GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
		LynxEngine::GraphicsSystem::CRenderer::RASTERIZERSTATEDESC RDesc;
		LynxEngine::GraphicsSystem::CRenderer::SAMPLERSTATEDESC SamDesc;

		DDesc.Default();
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();		

		CSkyDomeCreationDesc* lpDesc = (CSkyDomeCreationDesc*)desc;
		if (!lpDesc)
			return LYNX_FALSE;

		CreateVertexLayout();

		m_SkyLightMapSize = lpDesc->m_SkyLightMapSize;
		m_lpSkyLightMap = LYNXNEW CSkyLightMap(m_lpEngine->GetlpGraphicsSystem());
		m_lpSkyLightMap->Create(1, _T("SkyLightMap"), m_SkyLightMapSize, m_SkyLightMapSize, lpDesc->m_SkyLightMapFormat);

		if (lpDesc->m_bCreateMaterial)
		{
			m_lpSkyLightMaterial = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(_T("SkyLight Material")); 
			if (!m_lpSkyLightMaterial)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_lpSkyLightMaterial, MaterialSystem::CMaterial::POSTPROCESSING);			
				m_lpSkyLightMaterial->SetLightingType(MaterialSystem::CMaterial::UNLIGHTING);
				m_lpSkyLightMaterial->SetName(_T("SkyLight Material"));
			}		
			m_lpSkyLightMaterial->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SCREEN_QUAD));		
			m_lpSkyLightMaterial->m_SpPower = 0;				
			//m_lpSkyLightMaterial->vCreateTechnique(1);
			m_lpSkyLightMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			MaterialSystem::CTechnique* Tech = &m_lpSkyLightMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);			
			Tech->vCreatePass(1);
			Tech->SetName(m_lpSkyLightMaterial->GetName()+LynxEngine::CString(_T(".Tech0")));
			Tech->GetPass(0).SetName(Tech->GetName()+LynxEngine::CString(_T(".P0")));	
			Tech->GetPass(0).vCreateShaderClasses("ScreenQuadVS", "SkyLightPS");
			DDesc.StencilEnable = LYNX_FALSE;
			DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
			DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
			Tech->GetPass(0).CreateDepthStencilState(DDesc);					
			BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
			BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
			Tech->GetPass(0).CreateBlendState(BDesc);
			RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
			RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
			Tech->GetPass(0).CreateRasterizerState(RDesc);
			Tech->GetPass(0).m_bUseFog = LYNX_FALSE;					
			Tech->GetPass(0).vSetNumTextureAnimation(0);

			MaterialSystem::CMaterialPtr lpMat;									
			lpMat = ((CEngine *)m_lpEngine)->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(_T("SkyDome Material")); 
			if (!lpMat)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpMat, MaterialSystem::CMaterial::STANDARD);
				lpMat->SetLightingType(MaterialSystem::CMaterial::UNLIGHTING);
				lpMat->SetName(_T("SkyDome Material"));
			}
			lpMat->m_Diffuse.Red = 255;
			lpMat->m_Diffuse.Green = 255;
			lpMat->m_Diffuse.Blue = 255;
			lpMat->m_Diffuse.Alpha = 255;		
			lpMat->SetVertexLayout(m_VertexLayout);
			lpMat->vSetNumTextureAnimation(1);
			lpMat->GetTextureAnimation(0)->Create(1);
			lpMat->GetTextureAnimation(0)->SetTexture(0, m_lpSkyLightMap->GetTexture());

			lpMat->m_TransparentType = TT_OPAQUE;
			lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			lpMat->vAddTechnique(MaterialSystem::CMaterial::WIREFRAME);
			LynxEngine::MaterialSystem::CTechnique* Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE);
			Technique->vCreatePass(1);

			LynxEngine::MaterialSystem::CPass* Pass = &Technique->GetPass(0);
			Technique->SetName(lpMat->GetName()+LynxEngine::CString(".Tech0"));
			Pass->SetName(Technique->GetName()+LynxEngine::CString(".P0"));										
			Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();			
			Technique->GetPass(0).vCreateShaderClasses("SkyDomeVS", "SkyDomePS");
			DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
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

			Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::WIREFRAME);
			Technique->vCreatePass(1);					
			Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
			Technique->GetPass(0).vCreateShaderClasses("SkyDomeVS", "DiffuseMapPS");
			DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
			DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
			Technique->GetPass(0).CreateDepthStencilState(DDesc);		
			
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);						
		}		
		else
		{
			m_lpCurrentMaterial->GetTextureAnimation(0)->SetTexture(0, m_lpSkyLightMap->GetTexture());
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);						
		}
		
		Setup(lpDesc->m_Radius, lpDesc->m_Slices, lpDesc->m_Sides);

		//UpdateSkyLightMap();

		return CContainer::vCreate(desc);		
	}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void CSkyDomeContainer::vInstance(CContainer& rhs)
	//{		
	//	CContainer::vInstance(rhs);
	//	CSkyDomeContainer& RHS = CAST(CSkyDomeContainer&, rhs);
	//	/*
	//	m_NumLODs = RHS.m_NumLODs;
	//	m_VertexList = RHS.m_VertexList;
	//	m_TriangleList = RHS.m_TriangleList;
	//	m_PrimitiveList = RHS.m_PrimitiveList;		
	//	m_VertexArray = RHS.m_VertexArray;
	//	m_IndexArray = RHS.m_IndexArray;
	//	m_SourceType = CResource::INSTANCE;
	//	*/
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
	void CSkyDomeContainer::UpdateSkyLightMap()
	{
		UpdateParam();
		if (m_lpSunLight)
			m_lpSunLight->m_Intensity = m_ESun*m_SunLightIntensityScale;

		LYNXRECT ViewportRect;
		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetViewportRect(&ViewportRect);

		GraphicsSystem::CBuffer* LastBuffers[2];
		//LPLYNXBUFFER lpRenderBuffer = ((CEngine *)m_lpEngine)->GetlpGraphicsSystem()->GetRenderBuffer(0);
		//GraphicsSystem::CBuffer* lpDepthBuffer = m_lpEngine->GetlpGraphicsSystem()->GetDepthBuffer();
		GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();

		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_SkyLightMapSize, m_SkyLightMapSize);
		m_lpEngine->GetlpGraphicsSystem()->SetMapAsRenderBuffer(LastBuffers, 0, m_lpSkyLightMap);		
		//m_lpEngine->GetlpGraphicsSystem()->SetBackBufferAsRenderBuffer(0);		
		m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);			
		m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(2);			
		m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(3);			

		m_lpSkyLightMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(this, NULL);				
		m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();	

		//m_lpSkyLightMap->FindMaxPass();

		if (LastBuffers[0])
			m_lpEngine->GetlpGraphicsSystem()->SetRenderBuffer(0, LastBuffers[0]);
		if (LastBuffers[1])
			m_lpEngine->GetlpGraphicsSystem()->SetDepthBuffer(LastBuffers[1]);
		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(ViewportRect.x1, ViewportRect.y1, ViewportRect.x2, ViewportRect.y2);
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
	LYNXBOOL CSkyDomeContainer::vPlay(CReal step) 
	{
		//m_Time = 40.0f;
		m_Time += 0.1f*step;

		if (m_lpSunLight)
		{
			CVector3 Pos, LookAt;


			Pos.x = 0.0f;
			Pos.y = 0.0f;
			Pos.z = -m_lpSunLight->m_Radius*0.5f;

			LookAt.x = 0.0f;
			LookAt.y = 0.0f;
			LookAt.z = 0.0f;

			m_lpSunLight->ResetTransform();
			m_lpSunLight->SetPosition(Pos);
			//((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->GetlpContainer()->UpdateTransform();
			//((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->GetlpContainer()->LookAt(&LookAt);
			//((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->GetlpContainer()->UpdateTransform();
			m_lpSunLight->GetlpContainer()->Pitch(m_Time+90.0f, LYNX_MATH_POST);
			m_lpSunLight->GetlpContainer()->UpdateTransform();






			/*Pos.x = 0.0f;
			Pos.y = m_SunDir.y*((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->m_Radius*0.5f;
			Pos.z = m_SunDir.z*((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->m_Radius*0.5f;

			LookAt.x = 0.0f;
			LookAt.y = 0.0f;
			LookAt.z = 0.0f;

			((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->SetPosition(&Pos);
			((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->GetlpContainer()->UpdateTransform();
			((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->GetlpContainer()->LookAt(&LookAt);
			((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->GetlpContainer()->UpdateTransform();*/				
		}		

		//m_lpEngine->GetlpSceneSystem()->GetlpScene(0)->GetlpStaticObj(1)->SetTransformMatrix(((SceneSystem::CSky*)m_lpRenObj)->GetSunLight()->GetlpContainer()->GetTransformMatrix());
		//m_lpEngine->GetlpSceneSystem()->GetlpScene(0)->GetlpStaticObj(1)->UpdateTransform();

		UpdateSkyLightMap();

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
	void CSkyDomeContainer::vRender(CCameraContainer* cam)
	{			
		LYNXMATRIX MM, WVPM, WVM;
		float ZScale;
		float ZBias = 0.0005f;
		float TexelGrad[2];
		float Color[3];
		LYNXVECTOR3D Pos;
		float Rad;
		int	 FilterType;
		float WireframeColor[4];		

		LYNX_ASSERT(cam);	

		GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();
		
		m_VertexArray[m_CurrentLOD]->vSet(0, 0);		
		m_IndexArray[m_CurrentLOD]->vSet(0);

		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique(0);
		for (int p=0; p<Technique.GetNumPass(); ++p)
		{
			Technique.GetPass(p).Set(this, cam);			

			/*lpR->GetCachedPixelShader()->vSetConstantF("gSunDir", &m_SunDir.x, 3);																	
			lpR->GetCachedPixelShader()->vSetConstantF("gInvWavelength", &m_InvWavelength.x, 3);
			lpR->GetCachedPixelShader()->vSetConstantF("gHG", &m_HG.x, 3);
			lpR->GetCachedPixelShader()->vSetConstantF("gKrESun", m_KrESun);
			lpR->GetCachedPixelShader()->vSetConstantF("gKmESun", m_KmESun);
			float Temp[4] = { (float)m_SkyLightMapSize, 1.0f / (float)m_SkyLightMapSize, m_KrESun, m_KmESun};
			lpR->GetCachedPixelShader()->vSetConstantF("gTextureSizeAndKrKmData", &Temp[0], 4);*/

			//lpR->SetTexture(0, m_lpSkyLightMap->GetTexture());
			//lpR->SetTexture(0, m_lpEngine->GetlpGraphicsSystem()->GetSystemTexture(LynxEngine::GraphicsSystem::WHITE_TEXTURE));
			lpR->SetTexture(1, NULL);
			lpR->SetTexture(2, NULL);
			lpR->SetTexture(3, NULL);
			
			/*lpR->GetCachedVertexShader()->vSetConstantM("gWorldViewProj", &WVPM);
			lpR->GetCachedVertexShader()->vSetConstantM("gWorldView", &WVM);	
			lpR->GetCachedVertexShader()->vSetConstantM("gWorld", &m_M);
			lpR->GetCachedVertexShader()->vSetConstantF("gCameraPos", &Pos.x, 3);	
			lpR->GetCachedVertexShader()->vSetConstantF("gEyePos", &m_EyePos.x, 3);
			*/

			
			
			/*switch (m_CurrentMaterialTechnique)
			{
				case MaterialSystem::CMaterial::WIREFRAME:				
					WireframeColor[0] = m_lpCurrentMaterial->m_Diffuse.Red/255.0f;
					WireframeColor[1] = m_lpCurrentMaterial->m_Diffuse.Green/255.0f;
					WireframeColor[2] = m_lpCurrentMaterial->m_Diffuse.Blue/255.0f;
					WireframeColor[3] = m_lpCurrentMaterial->m_Diffuse.Alpha/255.0f;
					lpR->GetCachedPixelShader()->vSetConstantF("gOutputColor", WireframeColor, 4);	
					break;

				default:
					break;
			}*/

			for (int i=0; i<(int)m_PrimitiveList[m_CurrentLOD].size(); ++i)
			{		
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawPrimitiveBO(&m_PrimitiveList[m_CurrentLOD][i], (DWORD)m_NumVertices);				
			}
		}		
	}
}