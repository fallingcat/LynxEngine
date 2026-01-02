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
#include <Container/LynxParticle.h>
#include <Container/LynxVolumetricCloudContainer.h>
#include <Container/LynxModelContainer.h>
#include <Container/LynxSprite3DClusterContainer.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>

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
	CCloudParticle::CCloudParticle()
	{
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
	CCloudParticle::CCloudParticle(CVolumetricCloudContainer* vc)
	: CParticle(vc)
	{
		
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
	CCloudParticle::~CCloudParticle()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief 。
	*
	*	@param 。
	*	@param 。
	*	@param 。
	*	@return。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CCloudParticle::vBehave(LYNXREAL step)
	{		
		CVector3 TPos;

		lynxPointXMatrix(&TPos, &m_Pos, &m_lpEmitter->GetTransformMatrix());
		SetPosition(TPos);
		//UpdateTransform();
		Play(0.0f);
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
	void CVolumetricCloudContainer::InitMembers()
	{
		m_ContainerType = VOLUMETRIC_CLOUD;	
		m_bSortParticle = LYNX_TRUE;
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
	CVolumetricCloudContainer::CVolumetricCloudContainer(CEngine *lpengine)	
	: CPEmitterContainer(lpengine)
	{
		InitMembers();
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
	CVolumetricCloudContainer::~CVolumetricCloudContainer()	
	{		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CVolumetricCloudContainer& CVolumetricCloudContainer::operator =(const CVolumetricCloudContainer& rhs)
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
	LYNXBOOL CVolumetricCloudContainer::vPlay(CReal step)
	{
		static int Count = 0;

		//if (Count == 2)
		//	m_bDirty = LYNX_FALSE;

		UpdateTransform();

		CList<CParticle*>::CIterator P = m_LiveParticleList.begin();
		while (P != m_LiveParticleList.end())
		{				
			(*P)->vLoop(step);				
			++P;			
		}

		Count++;

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
	LYNXBOOL CVolumetricCloudContainer::vCreate(const CCreationDesc* desc)
	{
		LYNXCHAR strname[256];
		LYNXFILE File;
		lynxSprintf(strname, _T("../model/static/cloud/cloud%d.dat"), rand() % 11);	
		if (!LYNX_OPEN_FILE(&File, strname, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_FILE_STR))
			return LYNX_FALSE;
		
		int Num = 0;
		LYNX_READ_FILE(&Num, sizeof(int), 1, &File);

		CPEmitterCreationDesc*	Desc = (CPEmitterCreationDesc*)desc;			
		Desc->m_ParticleType = CContainer::SPRITE3D_CLUSTER;
		((CSprite3DClusterCreationDesc*)(((CVolumetricCloudCreationDesc*)Desc)->m_ParticleDesc))->m_ClusterSize = Num;
		CPEmitterContainer::vCreate(Desc);

		CCloudParticle* P;
		CVector3 TPos, Pos, Vel, TRot, RVel;
		LYNXCOLORRGBA PColor = { 255, 255, 255, 255 };

		for (int i = 0; i < Num; i++)
		{
			P = LYNXNEW CCloudParticle(this);			
			P->vCreate(CContainer::REFERENCE, m_ParticleDesc);			

			LYNX_READ_FILE(&Pos, sizeof(CVector3), 1, &File);			
			P->m_Pos = Pos;			
			P->UpdateTransform();
			CReal R = Pos.Length(3);
			if (R > m_Radius)
				m_Radius = R;

			lynxPointXMatrix(&TPos, &Pos, &m_M);
			P->Init(TPos, Vel, TRot, RVel, 0.5f, PColor);
			
			//compute the bounding box, used for rendering impostors and frustum culling
			/*
			Cloud->BoundingBox1 = Vector3(-1000, -1000, -1000);
			Cloud->BoundingBox2 = Vector3(1000, 1000, 1000);
				
			if (P->Position.x > Cloud->BoundingBox1.x) Cloud->BoundingBox1.x = P->Position.x;				
			if (P->Position.y > Cloud->BoundingBox1.y) Cloud->BoundingBox1.y = P->Position.y;				
			if (P->Position.z > Cloud->BoundingBox1.z) Cloud->BoundingBox1.z = P->Position.z;				
			if (P->Position.x < Cloud->BoundingBox2.x) Cloud->BoundingBox2.x = P->Position.x;				
			if (P->Position.y < Cloud->BoundingBox2.y) Cloud->BoundingBox2.y = P->Position.y;				
			if (P->Position.z < Cloud->BoundingBox2.z) Cloud->BoundingBox2.z = P->Position.z;
			*/
					
			//P->m_Position += Position;
			P->m_Angle = 0.0f;
			P->m_Vel0.x = 0.0f; P->m_Vel0.y = 0.0f; P->m_Vel0.z = 0.0f;
			//P->m_ID = 0;
			P->m_LifeTime = 1.0f;					
			P->m_LifeTimeLimit = -1.0f;	
			
			m_LiveParticleList.push_back(P);			
		}

		float R;
		float Color[4];
		LYNXCOLORRGBA UColor;
		for (int i = 0; i < Num; i++)
		{
			LYNX_READ_FILE(&R, sizeof(float), 1, &File);
			Math::CReal Radius(R*2.0f);
			m_LiveParticleList[i]->SetRadius(Radius);
		}
		for (int i = 0; i < Num; i++)
		{
			LYNX_READ_FILE(Color, sizeof(float), 4, &File);
			UColor.Red = Color[0]*255.0f;
			UColor.Green = Color[1]*255.0f;
			UColor.Blue = Color[2]*255.0f;
			UColor.Alpha = Color[3]*255.0f;
			m_LiveParticleList[i]->SetColor(UColor);
		}
		LYNX_CLOSE_FILE(&File);
		
		//allocate buffers for rendering
		//Cloud->VertexBuffer = new Vector3[Num * 4];
		//Cloud->TexCoordBuffer = new Vector2[Num * 4];
		//Cloud->ColorBuffer = new Color4[Num * 4];

		MaterialSystem::CTechnique* Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		DDesc.Default();
		DDesc.StencilEnable = LYNX_FALSE;
		DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
		DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(DDesc);
		
		Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING);
		DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
		DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(DDesc);

		return LYNX_TRUE;
	}				
}