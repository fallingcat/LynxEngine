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

#ifndef __LYNXPEMITTERCONTAINER_H__
#define __LYNXPEMITTERCONTAINER_H__

#include <Container/LynxContainer.h>

#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxSpriteClusterContainer.h>
#include <Container/LynxSprite3DContainer.h>
#include <Container/LynxSpriteContainer.h>
#include <Container/LynxModelContainer.h>
#include <Container/LynxVoxelClusterContainer.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CPEmitterCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CPEmitterCreationDesc);	
	public:			
		PEMITTERTYPE			m_Type;		
		int						m_Density;
		CReal					m_Range;
		DWORD					m_MaxParticle;
		CReal					m_LifeTimeLimit;
		int						m_NumStoppingParticles;
		int						m_MaxTrailStep;
		float					m_TrailLengthScale;
		LYNXBOOL				m_bShrunkenTrail;
		float					m_ShrunkenSizeRatio;
		float					m_ParticleRatio;
		CVector2				m_ScaleFactor;
		CVector3				m_Gravity;
		float					m_EmitInterval;
		float					m_Speed;
		float					m_RotSpeed;
		float					m_Radius;
		CVector3				m_Position;
		CVector3				m_Rotation;		
		CContainer::TYPE		m_ParticleType;
		CCreationDesc*			m_ParticleDesc;
		GraphicsSystem::CRenderer::BLENDTYPE m_BlendType;
		Animation::CAnimation*  m_Animation;

		CPEmitterCreationDesc() 
		{ 
			m_ParticleDesc = NULL; 
			m_MaxTrailStep = 1; 
			m_TrailLengthScale = 5.0f; 
			m_bShrunkenTrail = LYNX_FALSE; 
			m_ParticleRatio = 1.0f; 
			m_ShrunkenSizeRatio = 0.5f; 
			m_Density = 1; 
			m_Range = 1.0f; 
			m_Radius = 32.0f;
			m_ScaleFactor = CVector2(1.0f, 1.0f); 

			m_Type = PE_POINT;
			m_Density = 1;
			m_Range = 1.0f;
			m_MaxParticle =1;
			m_LifeTimeLimit = 1.0f;
			m_NumStoppingParticles = -1;
			m_MaxTrailStep = 1;
			m_TrailLengthScale = 5.0f;
			m_bShrunkenTrail = LYNX_FALSE;
			m_ShrunkenSizeRatio = 0.5f;
			m_ParticleRatio = 1.0f;
			m_ScaleFactor = CVector2(1.0f, 1.0f);
			// m_Gravity;
			m_EmitInterval = 0.03f;
			m_Speed = 1.0f;
			m_RotSpeed = 0.0f;
			// m_Position;
			// m_Rotation;
			m_BlendType = GraphicsSystem::CRenderer::BLT_ALPHA;
			m_Animation = NULL;
		};

		~CPEmitterCreationDesc()
		{
			if (m_Animation)
			{
				LYNXDEL m_Animation;
				m_Animation = NULL;
			}
		}
	};

	class LYNXENGCLASS CPEmitterContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CPEmitterContainer);			
	public:
		typedef struct {
			int										NumFrames;
			int										NumColumns;
			int										NumRows;
		}ShiftAnimationData;
		
		CVector3				m_EmitVel;
		CReal					m_Speed;
		CReal					m_RotSpeed;

		DECLAR_PARAM_VECTOR3D(m_Gravity)
		DECLAR_PARAM_INT(m_Density)
		DECLAR_PARAM_FLOAT(m_Range)
		DECLAR_PARAM_INT(m_MaxParticle)
		float					m_Radius;
		int						m_NumStoppingParticles;		
		float					m_EmitInterval;

		LYNXBOOL				m_bSortParticle;
		float					m_GroundHeight;
		LYNXBOOL				m_BounceAtGround;
			
		int						m_MaxTrailStep;
		float					m_ShrunkenSizeRatio;
		float					m_TrailLengthScale;
		LYNXBOOL				m_bShrunkenTrail;

		CReal					m_ParticleRatio;

		Animation::CAnimationSetPtr m_ParticleAnimationSet;	
		ShiftAnimationData		m_ShiftAnimationData;
		LYNXBOOL				m_bUseShiftTextureAnimation;
		
	protected:		
		DECLAR_PARAM_FLOAT(m_LifeTimeLimit)		

		LYNXBOOL				m_bDirty;
		PEMITTERTYPE			m_PEType;
		CContainer::TYPE		m_ParticleType;		
		CCreationDesc*			m_ParticleDesc;	
		int						m_ParticleCounter;

		CList<CParticle*>		m_LiveParticleList;	
		CList<CParticle*>		m_DeadParticleList;	

		CContainer*				m_lpClusterContainer;		
		CVector2				m_ScaleFactor;
		
		CVector3				m_Vel0;
		CVector3				m_Vel;
		NOISETYPE				m_NoiseType;
		CVector3				m_Noise;		
		LYNXBOOL				m_bLoop;
		LYNXBOOL				m_bSingleTexture;	
		LYNXBOOL				m_bEmit;				
		
		CReal					m_StartDegX, m_StartDegY, m_StartDegZ;
		CReal					m_RotateSpeedX, m_RotateSpeedY, m_RotateSpeedZ;
		LYNXLONGLONG			m_EmitTime;		
		
		DWORD					m_Flag;		
		DWORD					m_NumParticle, m_NumEmitedParticle;

		CSprite3DClusterCreationDesc	m_Sprite3DClusterDesc;
		CSpriteClusterCreationDesc		m_SpriteClusterDesc;
		CSprite3DCreationDesc			m_Sprite3DDesc;
		CSpriteCreationDesc				m_SpriteDesc;		
		CVoxelClusterCreationDesc		m_VoxelClusterDesc;
		CModelCreationDesc				m_ModelDesc;
		CPEmitterCreationDesc			m_PEmitterDesc;
	private:
		void					InitMembers();
	public:
		CPEmitterContainer(CEngine *lpengine);		
		virtual  ~CPEmitterContainer();		

		DECLARE_CLASSSCRIPT
		LYNXBOOL				vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
		LYNXBOOL				vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);

		CPEmitterContainer&		operator =(const CPEmitterContainer& rhs);				
		
		LYNXINLINE void			SetDirty(LYNXBOOL b) { m_bDirty = b; };
		LYNXINLINE PEMITTERTYPE	GetPEType() { return m_PEType; };
		LYNXINLINE void			SetPEType(PEMITTERTYPE t) {m_PEType = t; };
		LYNXINLINE CContainer::TYPE GetParticleType() const { return m_ParticleType; }
		LYNXINLINE void			SetLifeTime(float t) { m_LifeTimeLimit = t; }
		LYNXFORCEINLINE const float	GetLifeTime() const { return m_LifeTimeLimit; }
		LYNXINLINE CCreationDesc* GetParticleDesc() { return m_ParticleDesc; }
		LYNXFORCEINLINE CContainer* GetClusterContainer() { return m_lpClusterContainer; }

		virtual LYNXBOOL		vCreate(const CCreationDesc* desc);
		virtual void			vCreateDefaultResource();
		void					CreateDefaultMaterial(const CCreationDesc* desc);
		void					UpdateParticles();
		//virtual LYNXBOOL		vCreate(const LYNXCHAR *name, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_TRUE);
		virtual void			vRender(CCameraContainer* cam = NULL);
		

		LYNXFORCEINLINE void	AddParticleAnimation(Animation::CAnimation* a) { m_ParticleAnimationSet->AddAnimation(a); };
		Animation::CAnimation*  SetCurrentParticleAnimation(Animation::CAnimation* lpa); 
		Animation::CAnimation*	SetCurrentParticleAnimation(const CString name);		
		void					AddParticleAnimationKey(int set, Animation::CAnimation::KEYSTYPE type, int num, Animation::ANIMATIONKEYDATA* key);
		
		LYNXBOOL				CreateParticle(const CVector3& vel, const CVector3& rvel, CReal r);
		void					DeleteParticle(CParticle* p);
		void					DeleteAllParticles();
		void					CalculateParticlesDist(CVector3& v);
		void					SortParticles();
		LYNXFORCEINLINE CVector2 GetScaleFactor() {return m_ScaleFactor; }
		void					SetScaleFactor(const CVector2& s);

		LYNXFORCEINLINE int		GetNumActiveParticles() {return m_LiveParticleList.size(); }
		LYNXFORCEINLINE CParticle* GetActiveParticle(int i) {return m_LiveParticleList[i]; }
		LYNXFORCEINLINE CList<CParticle*>& GetLiveParticleList() {return m_LiveParticleList; }

		LYNXFORCEINLINE LYNXBOOL IsEmitting() {return m_bEmit; }	
		LYNXFORCEINLINE void	Emit(LYNXBOOL b) {m_bEmit = b; }	

		//virtual LYNXBOOL		vPlay(LYNXREAL skip, LYNXPLAYMODE mode);
		virtual LYNXBOOL		vPlay(CReal step);	
		void					Reset();

		virtual LYNXBOOL		vSave(CStream& s);
		virtual LYNXBOOL		vLoad(CStream& s, LYNXBOOL skip = LYNX_FALSE);

		virtual LYNXBOOL		vLoad(CUserProfile* file, CXMLElement* c);
		virtual LYNXBOOL		vSave(CUserProfile* file, CXMLElement* c);

	protected:		
		CCreationDesc*			CreateParticleDesc(CContainer::TYPE type, const CString& filename, int cluster_size);
	};			

	LYNXENGCLASS CString				GetEnumStr(PEMITTERTYPE e);
	LYNXENGCLASS PEMITTERTYPE			GetPEmitterType(const CString& s);
}

#endif
