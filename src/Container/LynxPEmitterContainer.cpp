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
//  
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <Container/LynxParticle.h>
#include <Container/LynxPEmitterContainer.h>
#include <Container/LynxModelContainer.h>
#include <Container/LynxCameraContainer.h>
#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxSpriteClusterContainer.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <Animation/LynxAnimationKeys.h>
#include <Shader/LynxDiffuseMapXColorPS.h>

#define ALLOCATE_PARTICLE_DESC			0

namespace LynxEngine 
{	
	IMPLEMENT_CLASSSCRIPT(CPEmitterContainer, CContainer)
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ??
	*
	*	@param ??
	*	@param ?? 
	*	@return ??
	*/
	//-------------------------------------------------------------------------------------------------------	
	void CPEmitterContainer::InitMembers()
	{
		m_ContainerType = PEMITTER;
		m_PEType = PE_POINT;
		m_ParticleDesc = NULL;
		m_lpClusterContainer = NULL;
		m_Gravity = CVector3(0.0f, 0.0f, 0.0f);
		m_Density = 2;
		m_Range = 20.0f;
		m_MaxParticle = 512;
		m_LifeTimeLimit = 128;
		m_bSortParticle = LYNX_FALSE;
		m_NumStoppingParticles = -1;
		m_ParticleCounter = 0;
		m_MaxTrailStep = 1;
		m_TrailLengthScale = 5.0f;		
		m_bShrunkenTrail = LYNX_FALSE;
		m_ShrunkenSizeRatio = 0.6f;
		m_ParticleRatio = 1.0f;				
		m_EmitInterval = 0;
		m_EmitTime = 0;
		m_ScaleFactor = CVector2(1.0f, 1.0f);
		m_Speed = 1.0f;
		m_RotSpeed = 0.0f;

		m_bUseShiftTextureAnimation = LYNX_FALSE;
		m_ShiftAnimationData.NumFrames = -1;
		m_ShiftAnimationData.NumColumns = 0;
		m_ShiftAnimationData.NumRows = 0;
		
		m_bDirty = LYNX_TRUE;
		m_bEmit = LYNX_FALSE;

		m_GroundHeight = 0.0f;
		m_BounceAtGround = LYNX_FALSE;

		IMPLEMENT_PARAM_VECTOR3D_COMMENT(m_Gravity, _T("Gravity vector"), 0)
		IMPLEMENT_PARAM_INT_COMMENT(m_Density, _T("Particle density"), 0)
		IMPLEMENT_PARAM_FLOAT_COMMENT(m_Range, _T("Particle spread range"), 0)
		IMPLEMENT_PARAM_FLOAT_COMMENT(m_LifeTimeLimit, _T("Particle life time"), 0)
		IMPLEMENT_PARAM_INT_COMMENT(m_MaxParticle, _T("Max number of particles"), 0)
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ??
	*
	*	@param ??
	*	@param ?? 
	*	@return ??
	*/
	//-------------------------------------------------------------------------------------------------------
	CPEmitterContainer::CPEmitterContainer(CEngine* lpengine)	
	: CContainer(lpengine)
	{
		InitMembers();
		m_ParticleAnimationSet = Animation::CAnimationSetPtr(LYNXNEW Animation::CAnimationSet(lpengine));
		INIT_CLASSSCRIPT
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ??
	*
	*	@param ??
	*	@param ?? 
	*	@return ??
	*/
	//-------------------------------------------------------------------------------------------------------
	CPEmitterContainer::~CPEmitterContainer()	
	{
		CList<CParticle*>::CIterator P;

		for (P = m_DeadParticleList.begin(); P != m_DeadParticleList.end(); ++P)
		{
			if (*P)
				LYNXDEL (*P);
		}
		for (P = m_LiveParticleList.begin(); P != m_LiveParticleList.end(); ++P)
		{
			if (*P)
				LYNXDEL (*P);
		}
		m_DeadParticleList.clear();
		m_LiveParticleList.clear();

	#if ALLOCATE_PARTICLE_DESC
		if (m_ParticleDesc)
		{
			LYNXDEL(m_ParticleDesc);
			m_ParticleDesc = NULL;
		}
	#endif
		
		if (m_lpClusterContainer)
		{
			m_lpEngine->DestroyContainer(m_lpClusterContainer);		
			m_lpClusterContainer = NULL;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object??
	*/
	//-------------------------------------------------------------------------------------------------------
	CPEmitterContainer& CPEmitterContainer::operator =(const CPEmitterContainer& rhs)
	{	
		return (*this);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ??
	*
	*	@param ??
	*	@param ?? 
	*	@return ??
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CPEmitterContainer::vCreate(const CCreationDesc* desc)
	{		
		CSprite3DClusterCreationDesc*	S3DCDesc;
		MaterialSystem::CTechnique*		Technique;
		CPEmitterCreationDesc*			Desc = (CPEmitterCreationDesc*)desc;			
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;

		DDesc.Default();
		BDesc.Default();
		RDesc.Default();

	#if ALLOCATE_PARTICLE_DESC
		if (m_ParticleDesc)
		{
			LYNXDEL(m_ParticleDesc);
			m_ParticleDesc = NULL;
		}
	#endif

		if (m_lpClusterContainer)
		{
			m_lpEngine->DestroyContainer(m_lpClusterContainer);
			m_lpClusterContainer = NULL;
		}

		CContainer::vCreate(desc);
					
		m_PEType = Desc->m_Type;
		m_ParticleType = Desc->m_ParticleType;			
		m_Density = Desc->m_Density;
		m_Range = Desc->m_Range;
		m_MaxParticle = ((Desc->m_MaxParticle / Desc->m_Density) + 1) * Desc->m_Density;
		m_LifeTimeLimit = Desc->m_LifeTimeLimit;
		m_NumStoppingParticles = Desc->m_NumStoppingParticles;
		m_MaxTrailStep  = Desc->m_MaxTrailStep;
		m_bShrunkenTrail = Desc->m_bShrunkenTrail;
		m_TrailLengthScale = Desc->m_TrailLengthScale;
		m_ParticleRatio = Desc->m_ParticleRatio;
		m_ShrunkenSizeRatio = Desc->m_ShrunkenSizeRatio;
		m_ScaleFactor = Desc->m_ScaleFactor;		
		m_Gravity = Desc->m_Gravity;
		m_EmitInterval = Desc->m_EmitInterval;
		m_Speed = Desc->m_Speed;
		m_RotSpeed = Desc->m_RotSpeed;
		m_Radius = Desc->m_Radius;

		CVector3 Position = Desc->m_Position;
		ResetTransform();
		Scale(Math::CVector3(m_Speed*m_ScaleFactor.x, m_Speed*m_ScaleFactor.x, m_Speed*m_ScaleFactor.x), LYNX_MATH_POST);
		Rotate(Desc->m_Rotation, LYNX_MATH_POST);
		UpdateTransform();
		vSetPosition(CVector3(Position.x * m_ScaleFactor.x, Position.y * m_ScaleFactor.y, Position.z));		

		if (!Desc->m_ParticleDesc)
		{
			Desc->m_ParticleDesc = CreateParticleDesc(m_ParticleType, Desc->m_FileName, m_MaxParticle * 2);
			LYNX_ASSERT(Desc->m_ParticleDesc && _T("Force unsupport particle type."));
		}

		if (m_ParticleType == CContainer::SPRITE3D_CLUSTER)
		{
		#if ALLOCATE_PARTICLE_DESC
			m_ParticleDesc =  LYNXNEW CSprite3DClusterCreationDesc();
		#else
			m_ParticleDesc = &m_Sprite3DClusterDesc;
		#endif
			*((CSprite3DClusterCreationDesc*)m_ParticleDesc) = *((CSprite3DClusterCreationDesc*)Desc->m_ParticleDesc);
			m_lpClusterContainer = m_lpEngine->CreateContainer(m_ParticleType);				
			if (((CSprite3DClusterCreationDesc*)m_ParticleDesc)->m_ClusterSize < 0)
				((CSprite3DClusterCreationDesc*)m_ParticleDesc)->m_ClusterSize = m_MaxParticle * m_MaxTrailStep;
			else
			{
				if (((CSprite3DClusterCreationDesc*)m_ParticleDesc)->m_ClusterSize < m_MaxParticle * m_MaxTrailStep)
					((CSprite3DClusterCreationDesc*)m_ParticleDesc)->m_ClusterSize = m_MaxParticle * m_MaxTrailStep;
			}

			//m_ParticleDesc->m_Name = m_Name + CString(_T(".ParticleConatiner"));
			//m_ParticleDesc->m_MaterialName = Desc->m_MaterialName;
			m_lpClusterContainer->vCreate(m_ParticleDesc);
			SetlpCurrentMaterial(m_lpClusterContainer->GetlpCurrentMaterial());					
			m_lpClusterContainer->SetlpRenObj(m_lpRenObj);

			Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
			BDesc.Default();
			BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
			Technique->GetPass(0).CreateBlendState(BDesc);											
			DDesc.Default();
			DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
			DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
			Technique->GetPass(0).CreateDepthStencilState(DDesc);		
			RDesc.Default();
			RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
			RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
			Technique->GetPass(0).CreateRasterizerState(RDesc);
			
			//Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING);
			//DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
			//DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
			//Technique->GetPass(0).CreateDepthStencilState(DDesc);
		}
		else if (m_ParticleType == CContainer::SPRITE_CLUSTER)
		{
		#if ALLOCATE_PARTICLE_DESC
			m_ParticleDesc = LYNXNEW CSpriteClusterCreationDesc();
		#else
			m_ParticleDesc = &m_SpriteClusterDesc;
		#endif
			*((CSpriteClusterCreationDesc*)m_ParticleDesc) = *((CSpriteClusterCreationDesc*)Desc->m_ParticleDesc);
			m_lpClusterContainer = m_lpEngine->CreateContainer(m_ParticleType);				
			//m_ParticleDesc->m_Name = m_Name + CString(_T(".ParticleConatiner"));
			//m_ParticleDesc->m_MaterialName = Desc->m_MaterialName;
			m_lpClusterContainer->vCreate(m_ParticleDesc);
			SetlpCurrentMaterial(m_lpClusterContainer->GetlpCurrentMaterial());					
			m_lpClusterContainer->SetlpRenObj(m_lpRenObj);

			/*
			Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
			BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
			BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
			Technique->GetPass(0).CreateBlendState(BDesc);											
			DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
			DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
			Technique->GetPass(0).CreateDepthStencilState(DDesc);		
			RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
			RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
			Technique->GetPass(0).CreateRasterizerState(RDesc);			
			*/
		}
		else if (m_ParticleType == CContainer::VOXEL_CLUSTER)
		{
		#if ALLOCATE_PARTICLE_DESC
			m_ParticleDesc = LYNXNEW CVoxelClusterCreationDesc();
		#else
			m_ParticleDesc = &m_VoxelClusterDesc;
		#endif
			*((CVoxelClusterCreationDesc*)m_ParticleDesc) = *((CVoxelClusterCreationDesc*)Desc->m_ParticleDesc);
			m_lpClusterContainer = m_lpEngine->CreateContainer(m_ParticleType);				
			//m_ParticleDesc->m_Name = m_Name + CString(_T(".ParticleConatiner"));
			//m_ParticleDesc->m_MaterialName = Desc->m_MaterialName;
			m_lpClusterContainer->vCreate(m_ParticleDesc);
			SetlpCurrentMaterial(m_lpClusterContainer->GetlpCurrentMaterial());					
			m_lpClusterContainer->SetlpRenObj(m_lpRenObj);			
		}
		else if (m_ParticleType == CContainer::SPRITE3D)
		{
		#if ALLOCATE_PARTICLE_DESC
			m_ParticleDesc = LYNXNEW CSprite3DCreationDesc();
		#else
			m_ParticleDesc = &m_Sprite3DDesc;
		#endif
			*((CSprite3DCreationDesc*)m_ParticleDesc) = *((CSprite3DCreationDesc*)Desc->m_ParticleDesc);
		}
		else if (m_ParticleType == CContainer::MODEL)
		{
		#if ALLOCATE_PARTICLE_DESC
			m_ParticleDesc = LYNXNEW CModelCreationDesc();
		#else
			m_ParticleDesc = &m_ModelDesc;
		#endif
			*((CModelCreationDesc*)m_ParticleDesc) = *((CModelCreationDesc*)Desc->m_ParticleDesc);
		}
		else if (m_ParticleType == CContainer::PEMITTER)
		{
		#if ALLOCATE_PARTICLE_DESC
			m_ParticleDesc = LYNXNEW CPEmitterCreationDesc();
		#else
			m_ParticleDesc = &m_PEmitterDesc;
		#endif
			*((CPEmitterCreationDesc*)m_ParticleDesc) = *((CPEmitterCreationDesc*)Desc->m_ParticleDesc);
		}
		else if (m_ParticleType == CContainer::SPRITE)
		{
		#if ALLOCATE_PARTICLE_DESC
			m_ParticleDesc = LYNXNEW CSpriteCreationDesc();
		#else
			m_ParticleDesc = &m_SpriteDesc;
		#endif
			*((CSpriteCreationDesc*)m_ParticleDesc) = *((CSpriteCreationDesc*)Desc->m_ParticleDesc);
			CreateDefaultMaterial(m_ParticleDesc);
			m_ParticleDesc->m_MaterialName = m_lpCurrentMaterial->GetName();
		}	
		
		if (m_lpCurrentMaterial)
		{		
			BDesc = TranslateBlendType(Desc->m_BlendType);
			m_lpCurrentMaterial->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateBlendState(BDesc);
			
			DDesc.Default();
			if (m_ParticleType == CContainer::SPRITE || m_ParticleType == CContainer::SPRITE_CLUSTER)
			{
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
			}
			else
			{
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
			}
			DDesc.StencilEnable = LYNX_FALSE;
			m_lpCurrentMaterial->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateDepthStencilState(DDesc);						
		}
		
		if (Desc->m_Animation)
		{
			Animation::CAnimation* lpAni = vCreateAnimation();
			lpAni->vInstance(*(Desc->m_Animation));
			AddParticleAnimation(lpAni);
			SetCurrentParticleAnimation(lpAni);
		}		
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ??
	*
	*	@param ??
	*	@param ?? 
	*	@return ??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::vCreateDefaultResource()
	{		
		Animation::CAnimation* lpAni = vCreateAnimation();
		
		AddAnimation(lpAni);		

		Animation::ANIMATIONKEYDATA ScaleKeyData[4];

		ScaleKeyData[0].StartTime = 0.0f;
		ScaleKeyData[0].EndTime = m_LifeTimeLimit*0.4f;
		ScaleKeyData[0].Values.resize(1);
		ScaleKeyData[0].Values[0] = 16.0f;

		ScaleKeyData[1].StartTime = m_LifeTimeLimit*0.4f;
		ScaleKeyData[1].EndTime = m_LifeTimeLimit*0.8f;
		ScaleKeyData[1].Values.resize(1);
		ScaleKeyData[1].Values[0] = 16.0f;

		ScaleKeyData[2].StartTime = m_LifeTimeLimit*0.8f;
		ScaleKeyData[2].EndTime = m_LifeTimeLimit+1;
		ScaleKeyData[2].Values.resize(1);
		ScaleKeyData[2].Values[0] = 16.0f;

		ScaleKeyData[3].StartTime = m_LifeTimeLimit+1;
		ScaleKeyData[3].EndTime = m_LifeTimeLimit+2;
		ScaleKeyData[3].Values.resize(1);
		ScaleKeyData[3].Values[0] = 16.0f;

		AddAnimationKey(0, Animation::CAnimation::KT_SCALE, 4, ScaleKeyData);

		Animation::ANIMATIONKEYDATA ColorKeyData[2];

		ColorKeyData[0].StartTime = 0.0f;
		ColorKeyData[0].EndTime = m_LifeTimeLimit+1;
		ColorKeyData[0].Values.resize(4);
		ColorKeyData[0].Values[0] = 1.0f;
		ColorKeyData[0].Values[1] = 1.0f;
		ColorKeyData[0].Values[2] = 0.0f;
		ColorKeyData[0].Values[3] = 1.0f;

		ColorKeyData[1].StartTime = m_LifeTimeLimit+1;
		ColorKeyData[1].EndTime = m_LifeTimeLimit+2;
		ColorKeyData[1].Values.resize(4);
		ColorKeyData[1].Values[0] = 1.0f;
		ColorKeyData[1].Values[1] = 1.0f;
		ColorKeyData[1].Values[2] = 0.0f;
		ColorKeyData[1].Values[3] = 0.0f;

		AddAnimationKey(0, Animation::CAnimation::KT_COLOR, 2, ColorKeyData);		
		
		m_AnimationSet->GetAnimation(0)->SetRange(0.0f, m_LifeTimeLimit);
		m_AnimationSet->GetAnimation(0)->SetStep(1.0f);
		m_AnimationSet->GetAnimation(0)->SetPlayMode(PM_ONCE);		

		m_AnimationSet->GetAnimation(0)->SetName(_T("Default"));
		m_AnimationSet->SetCurrentAnimation(m_AnimationSet->GetAnimation(0));
		m_AnimationSet->GetAnimation(0)->m_PlayMode = PM_ONCE;		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::CreateDefaultMaterial(const CCreationDesc* desc)
	{	
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::TEXTUREDESC			 TDesc;
		DDesc.Default();
		BDesc.Default();
		RDesc.Default();
		TDesc.Default();
		TDesc.MipMapLevel = 1;
		TDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
		TDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
		TDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
		TDesc.SamplerDesc.Filter = GraphicsSystem::CRenderer::FM_MIN_MAG_MIP_LINEAR;
		TDesc.SamplerDesc.MinLOD = 0;
		TDesc.SamplerDesc.MaxLOD = 0;
			
		CString MatName(desc->m_MaterialName);
		if (MatName.empty())
		{
			MatName = m_Name + CString(_T(".Material"));
		}
		
		MaterialSystem::CMaterialPtr lpMat = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MatName);
		if (!lpMat)
		{
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpMat, MaterialSystem::CMaterial::STANDARD);
			lpMat->SetName(MatName);
			lpMat->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE));
			lpMat->m_Diffuse.PackedColor = 0xffffffff;

			lpMat->vSetNumTextureAnimation(1);

			CStream* Stream = desc->m_lpStream;
			if (!Stream)
				Stream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			
			lpMat->GetTextureAnimation(0)->Create(1);
			lpMat->GetTextureAnimation(0)->LoadTexture(0, *(Stream), desc->m_FileName.c_str(), &TDesc);

			lpMat->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
			lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).vCreatePass(1);
			RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
			lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateRasterizerState(RDesc);

			lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();

			DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
			DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;
			lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateDepthStencilState(DDesc);

			BDesc.RTBlendParam[0].bBlend = LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend = GraphicsSystem::CRenderer::BF_SRC_ALPHA;
			BDesc.RTBlendParam[0].DestBlend = GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
			BDesc.RTBlendParam[0].BlendOp = GraphicsSystem::CRenderer::BO_ADD;
			BDesc.RTBlendParam[0].RTWriteMask = GraphicsSystem::CRenderer::ALL_CHANNELS;
			lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateBlendState(BDesc);

			lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).vCreateShaderClasses("SpriteVS", "DiffuseMapXColorPS");
		}
		SetlpCurrentMaterial(lpMat);
		SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);
	}
	////-------------------------------------------------------------------------------------------------------
	///**	
	//*	@brief ??
	//*
	//*	@param ??
	//*	@param ?? 
	//*	@return ??
	//*/
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CPEmitterContainer::vCreate(const LYNXCHAR *filename, int cluster_size, LPLYNXFILE lpf, long offset, LYNXBOOL bbo)
	//{
	//	m_ParticleType = CContainer::SPRITE3D_CLUSTER;
	//	m_lpClusterContainer = m_lpEngine->vCreateContainer(CContainer::SPRITE3D_CLUSTER);
	//	m_lpClusterContainer->vCreate(filename, 10240, NULL, 0, LYNX_TRUE);

	//	SetlpCurrentMaterial(m_lpClusterContainer->GetlpCurrentMaterial());		
	//	MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
	//	Technique.GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ONE);
	//	Technique.GetPass(0).m_DepthBufferMode = LYNX_DB_C;
	//	
	//	Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING);
	//	Technique.GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ONE);
	//	Technique.GetPass(0).m_DepthBufferMode = LYNX_DB_C;

	//	//m_ParticleType = CContainer::SPRITE3D;

	//	return LYNX_TRUE;
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief ??
	*
	*	@param ??
	*	@param ??
	*	@param ??
	*	@return??
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CPEmitterContainer::CreateParticle(const CVector3& vel, const CVector3& rvel, CReal r)
	{
		CVector3 Pos, TPos, TRot, RVel;	

		if (m_LiveParticleList.size() >= m_MaxParticle)
			return LYNX_FALSE;


		switch (m_PEType)
		{
			case PE_CIRCLE_PLANE:
			{
				float Dist = (rand() % int(m_Radius * 10)) / 10.0f;
				float Deg = (rand() % 360);
				Pos = CVector3(lynxQuickCos(Deg)*Dist, lynxQuickSin(Deg)*Dist, 0.0f);
				lynxPointXMatrix(&TPos, &Pos, &m_M);
				break;
			}
			case PE_RING_PLANE:
			{
				float Dist = m_Radius;
				float Deg = (rand() % 360);
				Pos = CVector3(lynxQuickCos(Deg)*Dist, lynxQuickSin(Deg)*Dist, 0.0f);
				lynxPointXMatrix(&TPos, &Pos, &m_M);
				break;
			}
			case PE_CIRCLE:
			case PE_POINT:
				TPos = (*m_lpCenter);
				TRot = CVector3(0.0f, 0.0f, rand() % 360);
				RVel = rvel;
				break;

			case PE_PLANE:				
				CVector3 Pos((5000.0f - (rand()%10000)) * 0.0001f, (5000.0f - (rand()%10000)) * 0.0001f, 0.0f);
				lynxPointXMatrix(&TPos, &Pos, &m_M);												
				break;			
		}			

		CList<CParticle*>::CIterator P;
		if (!m_DeadParticleList.empty())
		{
			P = m_DeadParticleList.begin();
			LYNXCOLORRGBA Color = {255, 255, 255, 255};
			(*P)->Init(TPos, vel, TRot, RVel, r, Color);
			(*P)->m_LifeTimeLimit = m_LifeTimeLimit;
			m_LiveParticleList.push_back((*P));
			m_DeadParticleList.remove((*P));			
		}
		else
		{
			CParticle* pParticle = LYNXNEW CParticle(this);	
			if (m_lpClusterContainer)
			{
				pParticle->vCreate(CContainer::REFERENCE, m_ParticleDesc);
			}
			else
			{
				pParticle->vCreate(m_ParticleType, m_ParticleDesc);
				pParticle->GetlpContainer()->SetlpRenObj(m_lpRenObj);
				if (!m_lpCurrentMaterial)
					SetlpCurrentMaterial(pParticle->GetlpContainer()->GetlpCurrentMaterial());				

				pParticle->GetlpContainer()->SetMotionBlur(LYNX_TRUE);
						
				/*MaterialSystem::CTechnique*		Technique;
				Technique = &pParticle->GetlpContainer()->GetlpCurrentMaterial()->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
				Technique->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ONE);
				Technique->GetPass(0).m_DepthBufferMode = LYNX_DB_C;
				
				Technique = &pParticle->GetlpContainer()->GetlpCurrentMaterial()->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING);
				Technique->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ONE);
				Technique->GetPass(0).m_DepthBufferMode = LYNX_DB_C;*/				

				if (m_ParticleType == CContainer::SPRITE)
				{
					static_cast<CSpriteContainer*>(pParticle->GetlpContainer())->SetAlignmentType(0);
				}
			}
			LYNXCOLORRGBA Color = {255, 255, 255, 255}; 
			pParticle->Init(TPos, vel, TRot, RVel, r, Color);
			pParticle->m_LifeTimeLimit = m_LifeTimeLimit;
			m_LiveParticleList.push_back(pParticle);			
		}	

		m_ParticleCounter++;

		return TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief ??
	*
	*	@param ??
	*	@param ??
	*	@param ??
	*	@return??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::DeleteParticle(CParticle* p)
	{
		if (p->m_Radius != 0.0f)
			int xxx = 0;
		m_DeadParticleList.push_back(p);
		m_LiveParticleList.remove(p);		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief ??
	*
	*	@param ??
	*	@param ??
	*	@param ??
	*	@return??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::DeleteAllParticles()
	{
		CList<CParticle*>::CIterator TempP;
		CList<CParticle*>::CIterator P = m_LiveParticleList.begin();
		while (P != m_LiveParticleList.end())
		{				
			TempP = P;
			++TempP;
			DeleteParticle((*P));				
			P = TempP;			
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief ??
	*
	*	@param ??
	*	@param ??
	*	@param ??
	*	@return??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::CalculateParticlesDist(CVector3& v)
	{
		CList<CParticle*>::CIterator P;
		for (P = m_LiveParticleList.begin(); P != m_LiveParticleList.end(); ++P)
		{
			(*P)->CalculateDist(v);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief ??
	*
	*	@param ??
	*	@param ??
	*	@param ??
	*	@return??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::SortParticles()
	{
		//LynxEngine::sort(m_LiveParticleList.begin(), m_LiveParticleList.end());//, sort_away);				
		m_LiveParticleList.sort(CRenderableObj::compare_back2front);
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
	Animation::CAnimation* CPEmitterContainer::SetCurrentParticleAnimation(Animation::CAnimation* lpa) 
	{
		Animation::CAnimation* A = m_ParticleAnimationSet->GetCurrentAnimation(); 
		m_ParticleAnimationSet->SetCurrentAnimation(lpa);
		return A;	
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
	Animation::CAnimation* CPEmitterContainer::SetCurrentParticleAnimation(const CString name) 
	{
		Animation::CAnimation* A = m_ParticleAnimationSet->GetAnimation(name);
		return SetCurrentParticleAnimation(A);
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ??
	*
	*	@param ??
	*	@param ?? 
	*	@return ??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::AddParticleAnimationKey(int set, Animation::CAnimation::KEYSTYPE type, int num, Animation::ANIMATIONKEYDATA* key)
	{	
		Animation::CAnimation* lpAni = m_ParticleAnimationSet->GetAnimation(set);

		if (lpAni)
		{
			lpAni->CreateKeys(type, num, key[0].Values.size());			

			for (int i=0; i<num; i++)
			{
				lpAni->m_AnimationKeys[type]->m_Keys[i].SetStartTime(key[i].StartTime);
				lpAni->m_AnimationKeys[type]->m_Keys[i].SetEndTime(key[i].EndTime);
				for (int v=0 ;v<key[0].Values.size(); v++)
				{
					lpAni->m_AnimationKeys[type]->m_Keys[i].SetValue(v, key[i].Values[v]);
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief ??
	*
	*	@param ??m_ClusterSize
	*	@param ??
	*	@param ??
	*	@return??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::Reset()
	{
		DeleteAllParticles();
		m_bEmit = LYNX_TRUE;
		m_ParticleCounter = 0;
		m_AnimationSet->ResetTime();
		m_ParticleAnimationSet->ResetTime();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief ??
	*
	*	@param ??
	*	@param ??
	*	@param ??
	*	@return??
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CPEmitterContainer::vPlay(CReal step)
	{
		LYNXBOOL bStop = LYNX_FALSE;	
		if (m_AnimationSet->GetCurrentAnimation())
			bStop = m_AnimationSet->GetCurrentAnimation()->Play(step);		

		m_bDirty = LYNX_TRUE;		

		UpdateTransform();
		
		//if (step > 3.0f)
		//	step = 3.0f;
		
		m_EmitVel = (*m_lpDirection);
		
		if (m_bEmit)
		{
			if (m_LiveParticleList.size() < m_MaxParticle)
			{
				if (m_lpEngine->GetlpKernel()->ElapsedGameTimeSince(m_EmitTime) > m_EmitInterval*1000)
				{
					switch (m_PEType)
					{
						case PE_POINT:
						{
							int MOD = LYNX_ROUND(m_Range*10.0f);
							if (MOD == 0)
								MOD = 1;
							int Base = m_Range;
							CReal Times = lynxLength(&m_EmitVel, 3)/10.0f;	
							//int NumP = LYNX_REAL_ROUND_INT( ((m_Density + ((rand() % m_Density)/2))*step) );	
							int NumP = LYNX_REAL_ROUND_INT( ((m_Density + ((rand() % m_Density)/2))) );	
							for (int i=0; i<NumP; i++)
							{								
								CVector3 V, RV;

								V.x = m_EmitVel.x + ((((rand() % MOD) / 5)) - Base)*Times*m_ScaleFactor.x;
								V.y = m_EmitVel.y + ((((rand() % MOD) / 5)) - Base)*Times*m_ScaleFactor.x;
								V.z = m_EmitVel.z + ((((rand() % MOD) / 5)) - Base)*Times*m_ScaleFactor.x;

								RV.z = m_RotSpeed * (1.0f + (5000 - rand() % 10000) / 100000.0f);

								CReal ParticleSize = 0;//lpm->ParticleSizeKey.lpKeys[0].lpValues[0];
								ParticleSize += ((rand()%10)*ParticleSize)/10.0f;
								ParticleSize = 0;
								CreateParticle(V, RV, ParticleSize);
							}

							/*
							int MOD = LYNX_ROUND(m_Range*10.0f);
							if (MOD == 0)
								MOD = 1;
							int Base = m_Range * 5.0f;
							CReal Times = lynxLength(&m_EmitVel, 3) * 4.0f;							
							int NumP = LYNX_REAL_ROUND_INT(((m_Density + ((rand() % m_Density) / 2))));
							for (int i = 0; i<NumP; i++)
							{
								CVector3 V, Random;

								Random.x = ((rand() % MOD) - Base);
								Random.y = ((rand() % MOD) - Base);
								Random.z = ((rand() % MOD) - Base);
								Random.Normalise(3);
								Random *= Times.r;
								V.x = m_EmitVel.x + Random.x;
								V.y = m_EmitVel.y + Random.y;
								V.z = m_EmitVel.z + Random.z;

								CReal ParticleSize = 0;//lpm->ParticleSizeKey.lpKeys[0].lpValues[0];
								ParticleSize += ((rand() % 10)*ParticleSize) / 10.0f;
								ParticleSize = 0;
								CreateParticle(V, ParticleSize);
							}
							*/
						}
						break;

						case PE_PLANE:				
							break;			

						case PE_CIRCLE:				
						{
							CReal Times = lynxLength(&m_EmitVel, 3);	
							int NumP = m_Density;	
							float PerDeg = 360.0f / NumP;

							for (int i=0; i<NumP; i++)
							{
								CVector3 V, TV, RV;

								V.x = lynxCos(PerDeg*i);  							
								V.y = -lynxSin(PerDeg*i); 
								V.z = 0.0f;

								lynxVectorXMatrix(&TV, &V, &m_M);												

								CReal ParticleSize = 0;//lpm->ParticleSizeKey.lpKeys[0].lpValues[0];
								ParticleSize += ((rand()%10)*ParticleSize)/10.0f;
								ParticleSize = 0;
								CreateParticle(TV, RV, ParticleSize);
							}
						}
						break;

						case PE_RING_PLANE:
						case PE_CIRCLE_PLANE:
						{
							int MOD = LYNX_ROUND(m_Range*10.0f);
							if (MOD == 0)
								MOD = 1;
							int Base = m_Range;
							CReal Times = lynxLength(&m_EmitVel, 3) / 10.0f;
							//int NumP = LYNX_REAL_ROUND_INT( ((m_Density + ((rand() % m_Density)/2))*step) );	
							int NumP = LYNX_REAL_ROUND_INT(((m_Density + ((rand() % m_Density) / 2))));
							for (int i = 0; i<NumP; i++)
							{
								CVector3 V, RV;

								V.x = m_EmitVel.x + ((((rand() % MOD) / 5)) - Base)*Times*m_ScaleFactor.x;
								V.y = m_EmitVel.y + ((((rand() % MOD) / 5)) - Base)*Times*m_ScaleFactor.x;
								V.z = m_EmitVel.z + ((((rand() % MOD) / 5)) - Base)*Times*m_ScaleFactor.x;

								RV.z = m_RotSpeed * (1.0f + (5000 - rand() % 10000) / 100000.0f);

								CReal ParticleSize = 0;//lpm->ParticleSizeKey.lpKeys[0].lpValues[0];
								ParticleSize += ((rand() % 10)*ParticleSize) / 10.0f;
								ParticleSize = 0;
								CreateParticle(V, RV, ParticleSize);
							}
						}
						break;
					}				
					m_EmitTime = m_lpEngine->GetlpKernel()->GetGameTime();
				}				
			}
		}

		CList<CParticle*>::CIterator TempP;
		CList<CParticle*>::CIterator P = m_LiveParticleList.begin();

		while (P != m_LiveParticleList.end())
		{				
			if ((*P)->m_LifeTimeLimit >= 0.0f && (*P)->m_LifeTime >= (*P)->m_LifeTimeLimit )
			{
				TempP = P;
				++TempP;
				DeleteParticle((*P));				
				P = TempP;
			}
			else
			{
				/*
				switch (m_NoiseType)
				{
					case NOISE_NONE:											
						break;

					case NOISE_RANDOM:
						break;

					case NOISE_ZEGMA:						
						if (m_Noise.x >= 0.001f)
							(*P)->m_Vel.x += (m_Noise.x - (rand()%100)/(50.0f/m_Noise.x));
						if (m_Noise.y >= 0.001f)
							(*P)->m_Vel.y += (m_Noise.y - (rand()%100)/(50.0f/m_Noise.y));
						if (m_Noise.z >= 0.001f)
							(*P)->m_Vel.z += (m_Noise.z - (rand()%100)/(50.0f/m_Noise.z));
						break;
				}
				*/																
				(*P)->vLoop(step);							
				++P;
			}
		}		

		if (m_NumStoppingParticles > 0)
		{
			if (m_ParticleCounter >= m_NumStoppingParticles)
			{
				m_bEmit = LYNX_FALSE;
			}
		}

		if (m_LiveParticleList.empty())		
		{
			bStop = LYNX_TRUE;
			m_bEmit = LYNX_FALSE;

			/*
			switch (m_AnimationSet->GetCurrentAnimation()->m_PlayMode)
			{
				case PM_ONCE:
					m_bEmit = LYNX_FALSE;
					break;
			}
			*/
		}
		else
		{
			/*
			switch (m_AnimationSet->GetCurrentAnimation()->m_PlayMode)
			{
				case PM_ONCE:
					if (!m_DeadParticleList.empty())
						m_bEmit = LYNX_FALSE;
					break;
			}
			*/
			bStop = LYNX_FALSE;
		}

		return bStop;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ??
	*
	*	@param ??
	*	@param ?? 
	*	@return ??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::UpdateParticles()
	{		
		int Index;
		CList<CParticle*>::CIterator P;
		CVector3 Pos, TrailPos;
		float TrailLen, ParticleRadius, Speed, RadiusBase;

		RadiusBase = (m_MaxTrailStep/(1.0f-m_ShrunkenSizeRatio));
		if (m_lpClusterContainer)
		{
			if (m_ParticleType == CContainer::SPRITE3D_CLUSTER)
			{
				CSprite3DClusterContainer* lpC = Container_Cast<CSprite3DClusterContainer*>(m_lpClusterContainer);
				for (Index=0, P = m_LiveParticleList.begin(); P != m_LiveParticleList.end(); ++P)
				{
					CVector3 Dir = (*P)->m_Vel;
					Speed = Dir.Length(3);
					Dir.Normalise(3);
					(*P)->GetPosition(Pos);
					
					ParticleRadius = (*P)->m_Radius;
					lpC->vSetSize(Index, ParticleRadius * m_ScaleFactor.x, ParticleRadius * m_ParticleRatio * m_ScaleFactor.y);
					lpC->vSetPosition(Index, Pos);							
					lpC->SetColor(Index, &(*P)->m_Color);								

					if (m_bUseShiftTextureAnimation)
					{
						int Time = int((*P)->m_TextureTime);
                        //int Time = int((*P)->m_LifeTime);
                        if (m_ShiftAnimationData.NumFrames > 0)
							Time = Time % m_ShiftAnimationData.NumFrames;
						CVector2 Shift;
						Shift.x = Time % m_ShiftAnimationData.NumColumns;
						Shift.y = Time / m_ShiftAnimationData.NumColumns;
						LYNXUVPOINT UVOffset;
						UVOffset.u = Shift.x / float(m_ShiftAnimationData.NumColumns);
						UVOffset.v = Shift.y / float(m_ShiftAnimationData.NumRows);
						lpC->vSetUV(Index, &UVOffset);
					}
					Index++;
					
					for (int i=1; i<m_MaxTrailStep; i++)
					{
						if (m_bShrunkenTrail)						
						{
							ParticleRadius = (*P)->m_Radius*((m_MaxTrailStep-i)/(RadiusBase)+m_ShrunkenSizeRatio);
						}
						else
						{
							ParticleRadius = (*P)->m_Radius;
						}
						lpC->vSetSize(Index, ParticleRadius * m_ScaleFactor.x, ParticleRadius * m_ParticleRatio * m_ScaleFactor.y);
						TrailLen = (i*ParticleRadius*0.3f);
						if (TrailLen > Speed*m_TrailLengthScale)
							break;
						TrailPos = Pos - Dir*TrailLen;						
						lpC->vSetPosition(Index, TrailPos);							
						lpC->SetColor(Index, &(*P)->m_Color);								
						Index++;
					}					
				}	
				LYNX_ASSERT(Index <= lpC->GetClusterSize());
				
				lpC->SetRenderedSize(Index);
			}
			else if (m_ParticleType == CContainer::SPRITE_CLUSTER)
			{
				CSpriteClusterContainer* lpC = Container_Cast<CSpriteClusterContainer*>(m_lpClusterContainer);
				for (Index=0, P = m_LiveParticleList.begin(); P != m_LiveParticleList.end(); ++P, Index++)
				{
					lpC->vSetSize(Index, (*P)->m_Radius * m_ScaleFactor.x, (*P)->m_Radius * m_ParticleRatio * m_ScaleFactor.y);
					(*P)->GetPosition(Pos);
					lpC->vSetPosition(Index, Pos);							
					lpC->vSetColor(Index, &(*P)->m_Color);								
				}					
				lpC->SetRenderedSize((int)m_LiveParticleList.size());
			}
			else if (m_ParticleType == CContainer::VOXEL_CLUSTER)
			{
				CVoxelClusterContainer* lpC = Container_Cast<CVoxelClusterContainer*>(m_lpClusterContainer);
				for (Index=0, P = m_LiveParticleList.begin(); P != m_LiveParticleList.end(); ++P, Index++)
				{
					(*P)->GetPosition(Pos);
					lpC->vSetPosition(Index, Pos);							
					lpC->vSetColor(Index, &(*P)->m_Color);								
				}					
				lpC->SetRenderedSize((int)m_LiveParticleList.size());
			}
		}
		else
		{
			for (P = m_LiveParticleList.begin(); P != m_LiveParticleList.end(); ++P)
			{				
				(*P)->SetCurrentMaterialTechnique(m_CurrentMaterialTechnique);
				(*P)->SetSize((*P)->m_Radius * m_ScaleFactor.x, (*P)->m_Radius * m_ParticleRatio * m_ScaleFactor.y);						
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ??
	*
	*	@param ??
	*	@param ?? 
	*	@return ??
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::vRender(CCameraContainer* cam)
	{		
		CList<CParticle*>::CIterator P;		
		CVector3 Pos, TrailPos;
		float TrailLen, ParticleRadius, Speed, RadiusBase;

		if (m_bDirty)
		{
			UpdateParticles();
			if (m_bSortParticle)
			{
				cam->vGetPosition(Pos);
				CalculateParticlesDist(Pos);
				SortParticles();
			}			
		}

		if (m_lpClusterContainer)
		{			
			m_lpClusterContainer->vRender(cam);
		}
		else
		{
			RadiusBase = (m_MaxTrailStep / (1.0f - m_ShrunkenSizeRatio));
			Shader::CDiffuseMapXColorPS* lpPS = ((Shader::CDiffuseMapXColorPS*)&(*(m_lpCurrentMaterial->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetPS())));
			LYNXBOOL bUseConstantColor = lpPS->m_bUseConstantColor;
			float* OldConstantColor = lpPS->m_ConstantColor;
			lpPS->m_bUseConstantColor = LYNX_TRUE;
			for (P = m_LiveParticleList.begin(); P != m_LiveParticleList.end(); ++P)
			{		
				CVector3 Dir = (*P)->m_Vel;
				Speed = Dir.Length(3);
				Dir.Normalise(3);
				(*P)->GetPosition(Pos);
				
				lpPS->SetConstantColor((*P)->GetColor());				
				(*P)->vRender(cam);

				for (int i = 1; i<m_MaxTrailStep; i++)
				{	
					if (m_bShrunkenTrail)
					{
						ParticleRadius = (*P)->m_Radius*((m_MaxTrailStep - i) / (RadiusBase)+m_ShrunkenSizeRatio);
					}
					else
					{
						ParticleRadius = (*P)->m_Radius;
					}
					(*P)->SetSize(ParticleRadius * m_ScaleFactor.x, ParticleRadius * m_ParticleRatio * m_ScaleFactor.y);
					TrailLen = (i*ParticleRadius*0.3f);
					if (TrailLen > Speed*m_TrailLengthScale)
						break;
					TrailPos = Pos - Dir*TrailLen;
					(*P)->SetPosition(TrailPos);					
					(*P)->vRender(cam);
				}				
				(*P)->SetPosition(Pos);
			}

			lpPS->m_bUseConstantColor = bUseConstantColor;
			lpPS->SetConstantColor(OldConstantColor);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CPEmitterContainer::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
	{
		REGISTER_SCRIPT_VAR(lps, "m_Gravity",		LynxEngine::CPEmitterContainer, m_Gravity)
		REGISTER_SCRIPT_VAR(lps, "m_Density",		LynxEngine::CPEmitterContainer, m_Density)
		REGISTER_SCRIPT_VAR(lps, "m_Range",			LynxEngine::CPEmitterContainer, m_Range)
		REGISTER_SCRIPT_VAR(lps, "m_MaxParticle",	LynxEngine::CPEmitterContainer, m_MaxParticle)

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CPEmitterContainer::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
	{	
		RegisterSuperClassMethod(lps);

		REGISTER_SCRIPT_METHOD(lps, "Reset",		LynxEngine::CPEmitterContainer,	void,	Reset, 		(void))
		
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  
	//-------------------------------------------------------------------------------------------------------		
	LYNXBOOL CPEmitterContainer::vSave(CStream& stream)
	{
		return LYNX_TRUE;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  
	//-------------------------------------------------------------------------------------------------------		
	LYNXBOOL CPEmitterContainer::vLoad(CStream& stream, LYNXBOOL bskip)
	{
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------		
	LYNXBOOL CPEmitterContainer::vLoad(CUserProfile* file, CXMLElement* node)
	{
		PEMITTERTYPE Type(LynxEngine::PE_POINT);
		CContainer::TYPE ParticleType(CContainer::SPRITE_CLUSTER);
		CString	Name(_T("CPEmitter")), Texture, EmitAnim, BlendType(_T("AlphaBlend")), MaterialName;
		int Density(1), NumStoppingParticles(-1), MaxTrailStep(1);
		DWORD MaxParticle(32);
		CVector3 Gravity, Position, Rotation;
		LYNXBOOL bBounceAtGround(LYNX_FALSE), bShrunkenTrail(LYNX_FALSE);
		float Range(1.0f), Speed(1.0f), LifeTime(10), TrailLengthScale(5.0f), ParticleRatio(1.0f), ShrunkenSizeRatio(0.5f), EmitInterval(0.03f), RotSpeed(0.0f);
		Math::CVector2 ScaleFactor(1.0f, 1.0f);
		float Radius(32.0f);

		Type = GetPEmitterType(file->Read(_T("Type"), _T("Point"), node));
		ParticleType = TranslateContainerType(file->Read(_T("ParticleType"), GetEnumStr(CContainer::SPRITE_CLUSTER), node));
		Name = file->Read(_T("Name"), Name, node);
		Texture = file->Read(_T("ParticleFile"), Texture, node);
		// EmitAnim = file->Read(_T("ParticleAnimationFile"), EmitAnim, node);
		Position = file->Read(_T("ParticlePosition"), Position, node);
		Rotation = file->Read(_T("ParticleOrientation"), Rotation, node);
		MaxParticle = file->Read(_T("MaxNumParticles"), MaxParticle, node);
		LifeTime = file->Read(_T("ParticleLifeTime"), LifeTime, node);
		EmitInterval = file->Read(_T("ParticleEmitInterval"), EmitInterval, node);
		Range = file->Read(_T("ParticleRange"), Range, node);
		Speed = file->Read(_T("ParticleSpeed"), Speed, node);
		RotSpeed = file->Read(_T("ParticleRotationSpeed"), RotSpeed, node);
		Density = file->Read(_T("ParticleDensity"), Density, node);
		Gravity = file->Read(_T("ParticleGravity"), Gravity, node);
		NumStoppingParticles = file->Read(_T("NumStoppingParticles"), NumStoppingParticles, node);
		BlendType = file->Read(_T("ParticleBlend"), BlendType, node);
		ParticleRatio = file->Read(_T("ParticleRatio"), ParticleRatio, node);
		bShrunkenTrail = file->Read(_T("UseShrunkenTrail"), bShrunkenTrail, node);
		ShrunkenSizeRatio = file->Read(_T("ShrunkenSizeRatio"), ShrunkenSizeRatio, node);
		MaxTrailStep = file->Read(_T("MaxTrailStep"), MaxTrailStep, node);
		TrailLengthScale = file->Read(_T("TrailLengthScale"), TrailLengthScale, node);
		MaxTrailStep = file->Read(_T("m_MaxTrailStep"), MaxTrailStep, node);
		ScaleFactor = file->Read(_T("ScaleFactor"), ScaleFactor, node);
		Radius = file->Read(_T("Radius"), Radius, node);
		//MaterialName = file->Read(_T("MaterialName"), MaterialName, node);
		
		LYNXGUID GUID;
		lynxOSMakeGUID(&GUID);
		Name = lynxOSGUIDToString(&GUID);

		CPEmitterCreationDesc Desc;
		Desc.Default();
		Desc.m_Name = Name;
		Desc.m_FileName = Texture;
		Desc.m_Type = Type;
		Desc.m_ParticleType = ParticleType;	
		Desc.m_ParticleDesc = NULL;
		Desc.m_Density = Density;
		Desc.m_Range = Range;		
		Desc.m_MaxParticle = MaxParticle;
		Desc.m_LifeTimeLimit = LifeTime;
		Desc.m_NumStoppingParticles = NumStoppingParticles;
		Desc.m_MaxTrailStep = MaxTrailStep;		
		Desc.m_TrailLengthScale = TrailLengthScale;
		Desc.m_bShrunkenTrail = bShrunkenTrail;
		Desc.m_ShrunkenSizeRatio = ShrunkenSizeRatio;
		Desc.m_ParticleRatio = ParticleRatio;		
		Desc.m_ScaleFactor = ScaleFactor;
		Desc.m_Gravity = Gravity;
		Desc.m_EmitInterval = EmitInterval;
		Desc.m_Speed = Speed;
		Desc.m_RotSpeed = RotSpeed;
		Desc.m_Rotation = Rotation;
		Desc.m_Position = Position;
		Desc.m_BlendType = GetBlendType(BlendType);
		Desc.m_Radius = Radius;
				
		// Animation
		Desc.m_Animation = vCreateAnimation();
		Desc.m_Animation->vLoad(file, node->FirstChildElement(_T("ParticleAnimationKey")));

		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));

		if (!vCreate(&Desc))
			return LYNX_FALSE;

		LYNX_ASSERT(m_lpCurrentMaterial && _T("Current Material shouldn't be NULL."));
								
		// Init
		Reset();
		Emit(LYNX_FALSE);

		return LYNX_TRUE;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void CPEmitterContainer::SetScaleFactor(const CVector2& s)
	{ 
		CVector3 Scale;
		m_M.GetScale(Scale);
		Scale.x /= m_ScaleFactor.x;
		Scale.y /= m_ScaleFactor.x;
		Scale.z /= m_ScaleFactor.x;
		Scale *= s.x;
		m_M.ReplaceScale(Scale);
		m_ScaleFactor = s; 		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CPEmitterContainer::vSave(CUserProfile* file, CXMLElement* node)	
	{
		CXMLElement* Root = node;
		if (!Root)
			Root = file->GetDoc()->RootElement();

		CString BlendType(_T("Alpha"));
		if (m_lpCurrentMaterial)
		{
			GraphicsSystem::CBlendStatePtr& BlendState = m_lpCurrentMaterial->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).GetBlendState();
			BlendType = GetEnumStr(GetBlendType(*BlendState));
		}
		
		CVector3 Position, Scale, Rotation;		
		LPLYNXQUATERNION Quaternion;
		m_M.GetPos(Position);
		m_M.GetScale(Scale);
		lynxMatrix2EulerAnglesXYZ(Rotation, m_M);

		file->Write(_T("Type"), GetEnumStr(m_PEType));
		file->Write(_T("ParticleType"), GetEnumStr(m_ParticleType));
		file->Write(_T("Name"), m_Name);

		file->Write(_T("ParticleFile"), m_ParticleDesc->m_FileName);
		// EmitAnim = file->Write(_T("ParticleAnimationFile"), EmitAnim);
		file->Write(_T("ParticlePosition"), Position);
		file->Write(_T("ParticleOrientation"), Rotation);
		file->Write(_T("MaxNumParticles"), m_MaxParticle);
		file->Write(_T("ParticleLifeTime"), m_LifeTimeLimit);
		file->Write(_T("ParticleEmitInterval"), m_EmitInterval);
		file->Write(_T("ParticleRange"), m_Range);
		file->Write(_T("ParticleSpeed"), (float)m_Speed);
		file->Write(_T("ParticleRotationSpeed"), (float)m_RotSpeed);
		file->Write(_T("ParticleDensity"), m_Density);
		file->Write(_T("ParticleGravity"), m_Gravity);
		file->Write(_T("NumStoppingParticles"), m_NumStoppingParticles);
		file->Write(_T("ParticleBlend"), BlendType);
		file->Write(_T("ParticleRatio"), (float)m_ParticleRatio);
		file->Write(_T("UseShrunkenTrail"), m_bShrunkenTrail);
		file->Write(_T("ShrunkenSizeRatio"), m_ShrunkenSizeRatio);
		file->Write(_T("MaxTrailStep"), m_MaxTrailStep);
		file->Write(_T("TrailLengthScale"), m_TrailLengthScale);
		file->Write(_T("ScaleFactor"), m_ScaleFactor);
		//file->Write(_T("MaterialName"), m_lpCurrentMaterial->GetName());		
		
		// Animation
		LynxEngine::Animation::CAnimation* lpAni = m_ParticleAnimationSet->GetCurrentAnimation();
		if (lpAni)
		{
			CXMLElement* AnimationElem = file->GetDoc()->NewElement(_T("ParticleAnimationKey"));
			Root->InsertEndChild(AnimationElem);
			
			lpAni->vSave(file, AnimationElem);
		}

		file->Save();

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	CCreationDesc* CPEmitterContainer::CreateParticleDesc(CContainer::TYPE type, const CString& filename, int cluster_size)	
	{		
		if (type == CContainer::SPRITE3D_CLUSTER)
		{
			m_Sprite3DClusterDesc.Default();
			m_Sprite3DClusterDesc.m_bUseBufferObject = LYNX_TRUE;
			m_Sprite3DClusterDesc.m_ClusterSize = cluster_size;
			m_Sprite3DClusterDesc.m_Number = 1;
			m_Sprite3DClusterDesc.m_TextureDesc.bDelayLoading = LYNX_FALSE;
			m_Sprite3DClusterDesc.m_FileName = filename;
			m_Sprite3DClusterDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
		
			return &m_Sprite3DClusterDesc;
		}
		else if (type == CContainer::SPRITE_CLUSTER)
		{
			m_SpriteClusterDesc.Default();
			m_SpriteClusterDesc.m_bUseBufferObject = LYNX_TRUE;
			m_SpriteClusterDesc.m_ClusterSize = cluster_size;
			m_SpriteClusterDesc.m_Number = 1;
			m_SpriteClusterDesc.m_TextureDesc.bDelayLoading = LYNX_FALSE;
			m_SpriteClusterDesc.m_FileName = filename;
			m_SpriteClusterDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
		
			return &m_SpriteClusterDesc;
		}
		else if (type == CContainer::VOXEL_CLUSTER)
		{
			m_VoxelClusterDesc.Default();
			m_VoxelClusterDesc.m_ClusterSize = cluster_size;
			m_VoxelClusterDesc.m_FileName = filename;
			m_VoxelClusterDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			// VDesc.m_MaterialName = filename + CString(_T(".FX.Material"));

			return &m_VoxelClusterDesc;
		}
		else if (type == CContainer::SPRITE)
		{
			m_SpriteDesc.Default();
			m_SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			m_SpriteDesc.m_Number = 1;
			m_SpriteDesc.m_TextureDesc.bDelayLoading = LYNX_FALSE;
			m_SpriteDesc.m_FileName = filename;
			m_SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			// SPDesc.m_MaterialName = filename + CString(_T(".FX.Material"));

			return &m_SpriteDesc;
		}
		return NULL;
	}
}