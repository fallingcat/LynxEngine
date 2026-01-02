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

#ifndef __LYNXDYNOBJ_H__
#define __LYNXDYNOBJ_H__

#include <LynxSystem.h>
#include <LynxRenderableObj.h>
#include <PhysicsSystem/LynxPhysicsObj.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{
		class LYNXENGCLASS CDynamicObj : public CRenderableObj
		{	
			LYNX_DECLARE_CLASS(CDynamicObj);
		public:
			enum TYPE{
				LIGHT,
				ENTITY,				
				FX,						
				AMBIENTSOUND,
				AI_WAYPOINT,
				PLAYER,
				CAMERA,
				NULL2,
				NULL3,
				PHYSICS_JOINT,
				NUM_TYPE,
			};

			enum VISTYPE{
				VT_NONE = 0,
				VT_ALWAYS_FAIL,
				VT_ALWAYS_PASS,
				NUM_VISTYPE,
			};
		protected:
			CScene*							m_lpScene;
			TYPE							m_DynamicObjType;					
			PhysicsSystem::CRagdoll*		m_lpRagdoll;
			VISTYPE							m_VisType;			
		public:
			CDynamicObj(CScene* const lpscene);			
			virtual ~CDynamicObj();			

			DECLARE_CLASSSCRIPT
			void							CreateReferenceContainer(const LYNXCHAR* name);
			void							CreateModelContainer(const LYNXCHAR* name, const LYNXCHAR* filename);
			void							CreateVoxelClusterContainer(const LYNXCHAR* name, const LYNXCHAR* filename);
			void							CreateSpriteContainer(const LYNXCHAR* name, const LYNXCHAR* filename, float w, float h);
			void							CreateSprite3DContainer(const LYNXCHAR* name, const LYNXCHAR* filename, float w, float h);
			void							CreateClusterSubContainer(const LYNXCHAR* name, CContainer* lpc, int index);
			void							CreateSprite3DClusterContainer(const LYNXCHAR* name, const LYNXCHAR* filename, int cluster_size);
			void							CreatePEmitterContainer(const LYNXCHAR* name, const LYNXCHAR* filename, int cluster_size, int max_particle, int density, float life, float range);
			LYNXBOOL						CreatePhysics(PhysicsSystem::CPhysicsShape::TYPE shape_type);					

			void							InitMembers();			

			//virtual LYNXBOOL	    		vCreate(LynxEngine::CContainer::TYPE mtype, const char *filename, const char *name);				
			//virtual LYNXBOOL				vCreate(LYNXBOOL bphy = LYNX_FALSE, PhysicsSystem::CPhysicsWorld* lpw = NULL);			

			virtual LYNXBOOL				vCreate(const LYNXCHAR *name, LynxEngine::CContainer::TYPE mtype, CCreationDesc* desc, LYNXBOOL bphy = LYNX_FALSE, PhysicsSystem::CPhysicsWorld* lpw = NULL);									
			virtual LYNXBOOL				vCreate(const LYNXCHAR *name, LynxEngine::CContainer::TYPE mtype, CUserProfile& file, CXMLElement* node = NULL, LYNXBOOL bphy = LYNX_FALSE, PhysicsSystem::CPhysicsWorld* lpw = NULL);
			virtual LYNXBOOL				vInstance(const LYNXCHAR *name, CDynamicObj& rhs);
			//virtual LYNXBOOL				vCreate(LynxEngine::CContainer::TYPE mtype, const LYNXCHAR *filename, const LYNXCHAR *name, int group_size = 1, LYNXBOOL bphy = LYNX_FALSE, PhysicsSystem::CPhysicsWorld* lpw = NULL);			
			virtual void					vLoop(float step);
			LYNXINLINE virtual void			vRender(CCameraContainer* cam = NULL) {m_lpContainer->vRender(cam);};						
			LYNXINLINE virtual void			vGlowRender(CCameraContainer* cam = NULL) { m_lpContainer->vGlowRender(cam); };			
			
			LYNXINLINE const TYPE			GetDynamicObjType() {return m_DynamicObjType; };					
			LYNXINLINE void					SetlpScene(CScene* const lpscene) {m_lpScene = lpscene; };
			LYNXINLINE CScene* const		GetlpScene() {return m_lpScene; };			
			virtual void					vAddToSceneVisList();

			virtual LYNXBOOL				vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			virtual LYNXBOOL				vRegisterScriptClassVar(ScriptSystem::CScript* const lps);

			LYNXFORCEINLINE void			SetVisType(VISTYPE t) { m_VisType = t; };
			LYNXFORCEINLINE VISTYPE			GetVisType() const { return m_VisType; };

			// Physics ------------------------------------------------------------------------------------------------
			LYNXBOOL						vCreatePhysics(PhysicsSystem::CPhysicsWorld* lpw, PhysicsSystem::CPhysicsShape* lps = NULL);					
			LYNXBOOL						CreateRagdoll(PhysicsSystem::CPhysicsWorld* lpw);						
			virtual LYNXBOOL				vGenerateRagdoll(PhysicsSystem::CPhysicsWorld* lpw);						
			void							SetMass(PhysicsSystem::CPhysicsObj::MASSTYPE t, LYNXREAL density, LYNXREAL *lpdata);
			void							SetMass(LYNXREAL mass);
			LYNXINLINE void					SetPosition(const Math::CVector3& pos) { m_lpContainer->vSetPosition(pos); if (m_lpPhyObj) m_lpPhyObj->vSetPosition(pos); };
			//LYNXINLINE void				GetPosition(LPLYNXVECTOR3D lppos) { m_lpContainer->GetPosition(lppos);};
			void                            UpdateTransform(OBJUPDATETYPE type = OBJUPDATE_RENDERABLEOBJ);		
			LYNXFORCEINLINE PhysicsSystem::CRagdoll* GetlpRagdoll() {return m_lpRagdoll; };
			
		friend class CScene;
		};		
	}
}

#endif