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
#include <Container/LynxParticle.h>
#include <Container/LynxPEmitterContainer.h>
#include <Animation/LynxAnimationKeys.h>

namespace LynxEngine 
{
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
	CParticle::CParticle()
	{
		m_lpEmitter = NULL;
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
	CParticle::CParticle(CPEmitterContainer* ppe)
	: CRenderableObj(ppe->GetlpEngine())
	{			
		m_lpEmitter = ppe;
		m_TextureTime = 0.0f;
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
	CParticle::~CParticle()
	{
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
	LYNXBOOL CParticle::vCreate(CContainer::TYPE type, const CCreationDesc* desc)
	{			
		if (!m_lpEmitter)
			return LYNX_FALSE;

		m_lpContainer = m_lpEmitter->GetlpEngine()->CreateContainer(type);
		//m_lpContainer->SetName(m_lpEmitter->GetName()+CString(_T(".Particle"));
		m_lpContainer->vCreate(desc);		

		m_lpContainer->AddAnimation(m_lpContainer->vCreateAnimation());
		m_lpContainer->m_AnimationSet->SetCurrentAnimation(m_lpContainer->m_AnimationSet->GetAnimationList()[0]);
		m_lpContainer->m_AnimationSet->GetCurrentAnimation()->m_StartTime = m_lpEmitter->m_ParticleAnimationSet->GetCurrentAnimation()->m_StartTime;
		m_lpContainer->m_AnimationSet->GetCurrentAnimation()->m_EndTime = m_lpEmitter->m_ParticleAnimationSet->GetCurrentAnimation()->m_EndTime;
		m_lpContainer->m_AnimationSet->GetCurrentAnimation()->m_Step = m_lpEmitter->m_ParticleAnimationSet->GetCurrentAnimation()->m_Step;
		m_lpContainer->m_AnimationSet->GetCurrentAnimation()->m_PlayMode = m_lpEmitter->m_ParticleAnimationSet->GetCurrentAnimation()->m_PlayMode;		
		
		return LYNX_TRUE;
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
	void CParticle::Init(const CVector3& pos, const CVector3& vel, const CVector3& rot, const CVector3& rvel, CReal r, LYNXCOLORRGBA& c)
	{
		CMatrix4 M;

		m_LifeTime = 0.0f;
		m_LifeTimeLimit = 0.0f;		
		m_Vel0 = CVector3(0.0f, 0.0f, 0.0f);
		m_Vel = vel;
		m_VelR = rvel;
		m_Radius = r;
		m_Color.PackedColor = c.PackedColor;
		m_RandTextureOffset = lynxRand();
		m_Rot = CVector3(0.0f);
		LYNX_ASSERT(m_lpContainer->m_AnimationSet->GetCurrentAnimation());
		m_lpContainer->m_AnimationSet->GetCurrentAnimation()->m_Time = 0.0f;
	
		// Init orientation
		m_Rot += rot;
		lynxEulerAnglesXYZ2Matrix(&M, m_Rot);
		m_lpContainer->SetLocalMatrix(M);
		m_lpContainer->ResetGlobalMatrix();
		m_lpContainer->UpdateTransform();
		m_Pos = pos;
		SetPosition(pos);
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
	void CParticle::vLoop(float step)
	{		
		CMatrix4 M;
		GetPosition(m_Pos);

		LYNX_ASSERT(m_lpContainer->m_AnimationSet->GetCurrentAnimation());
		m_lpContainer->m_AnimationSet->GetCurrentAnimation()->Play(step);
		float AnimationTime = m_lpContainer->m_AnimationSet->GetCurrentAnimation()->m_Time;
		Math::CReal Value[4];

		Animation::CAnimation* lpParticleAni = m_lpEmitter->m_ParticleAnimationSet->GetCurrentAnimation();
		LYNX_ASSERT(lpParticleAni);		
		if (lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION] && lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys.size()>0)
		{			
			lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->Lerp(Value, AnimationTime);
			lynxQuaternion2Matrix(&M, (LPLYNXQUATERNION)Value);
			m_lpContainer->SetLocalMatrix(M);			
			m_lpContainer->ResetGlobalMatrix();
			m_lpContainer->UpdateTransform();
		}
		else
		{
			m_Rot += m_VelR * step;
			lynxEulerAnglesXYZ2Matrix(&M, m_Rot);
			m_lpContainer->SetLocalMatrix(M);
			m_lpContainer->ResetGlobalMatrix();
			m_lpContainer->UpdateTransform();			
		}

		if (lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE] && lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys.size()>0)
		{
			lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->Lerp(Value, AnimationTime);
			m_Radius = Value[0];
		}

		if (lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR] && lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->m_Keys.size()>0)
		{
			lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->Lerp(Value, AnimationTime);
			m_Color.Red = Value[0] * 255.0f;
			m_Color.Green = Value[1] * 255.0f;
			m_Color.Blue = Value[2] * 255.0f;
			m_Color.Alpha = Value[3] * 255.0f;
		}

		if (lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE])
		{
			lpParticleAni->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->Lerp(Value, AnimationTime);
            m_TextureTime = Value[0];// + m_RandTextureOffset;
		}

		m_Vel += m_lpEmitter->m_Gravity * step;		
		m_Pos += step * m_Vel;
		if (m_lpEmitter->m_BounceAtGround)
		{
			if (m_Pos.y <= m_lpEmitter->m_GroundHeight)
			{
				m_Pos.y = m_lpEmitter->m_GroundHeight;
				m_Vel.x = m_Vel.z = 0.0f;
				m_Vel.y *= -0.9f;
				if (m_Vel.Length(3) < 0.3f)
				{
					m_Vel.x = 0.0f;
					m_Vel.y = 0.0f;
					m_Vel.z = 0.0f;
				}
			}
		}
		SetPosition(m_Pos);	

		m_LifeTime += step;		
	}
}