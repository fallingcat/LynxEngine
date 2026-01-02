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
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <Container/LynxModelContainer.h>
#include <SceneSystem/DynObj/LynxDynObj.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>
#include <PhysicsSystem/LynxPhysicsRagdoll.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>
#include <StateMachine/LynxState.h>
#include <StateMachine/LynxStateMachine.h>
#include <StateMachine/LynxStateTranslation.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{		
		IMPLEMENT_CLASSSCRIPT(CDynamicObj, CRenderableObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDynamicObj::CDynamicObj(CScene* const lpscene)
		: CRenderableObj(lpscene->GetlpSceneSystem()->GetlpEngine())
		{
			InitMembers();
			m_lpScene = lpscene;					

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDynamicObj::~CDynamicObj()
		{
			if (m_lpScene)
			{
				m_lpScene->RemoveDynamicObj(this);
			}

			if (m_lpRagdoll)
			{
				LYNXDEL m_lpRagdoll;
				m_lpRagdoll = NULL;
			}

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDynamicObj::InitMembers()			
		{
			LYNX_GUARD(CDynamicObj::InitMembers)			
			
			m_lpScene = NULL;						
			m_VisStaticLightList.clear();
			m_VisDynamicLightList.clear();			
			m_VisType = VT_NONE;

			m_lpRagdoll = NULL;

			#ifndef __NO_GUARD__
				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); throw e; }
				LYNX_DEFAULTCATCH
			#endif
		}											
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDynamicObj::vCreate(const LYNXCHAR *name, LynxEngine::CContainer::TYPE mtype, CCreationDesc* desc, LYNXBOOL bphy, PhysicsSystem::CPhysicsWorld* lpw)
		{
			LYNX_GUARD(CDynamicObj::vCreate)			

			SetName(name);
			CRenderableObj::Create(mtype);		
			desc->m_Name = CString(name)+CString(_T(".Container"));			

			if (!m_lpContainer->vCreate(desc))
				return LYNX_FALSE;

			if (bphy && lpw)
			{
				if (!vCreatePhysics(lpw))
					return LYNX_FALSE;
			}					

			CRenderableObj::vCreate();

			return LYNX_TRUE;

			#ifndef __NO_GUARD__
				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); throw e; }
				LYNX_DEFAULTCATCH
			#endif
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDynamicObj::vCreate(const LYNXCHAR *name, LynxEngine::CContainer::TYPE mtype, CUserProfile& file, CXMLElement* node, LYNXBOOL bphy, PhysicsSystem::CPhysicsWorld* lpw)
		{
			LYNX_GUARD(CDynamicObj::vCreate)

			SetName(name);
			CRenderableObj::Create(mtype);
			
			if (!m_lpContainer->vLoad(&file, node))
				return LYNX_FALSE;

			if (bphy && lpw)
			{
				if (!vCreatePhysics(lpw))
					return LYNX_FALSE;
			}

			CRenderableObj::vCreate();

			return LYNX_TRUE;

#ifndef __NO_GUARD__
			LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); throw e; }
			LYNX_DEFAULTCATCH
#endif
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDynamicObj::vInstance(const LYNXCHAR *name, CDynamicObj& rhs)
		{
			m_SourceType = INSTANCE;
			SetName(name);
			if (rhs.GetlpContainer())
			{
				CRenderableObj::Create(rhs.GetlpContainer()->GetContainerType());				
				m_lpContainer->vInstance(*(rhs.GetlpContainer()));			
				m_lpContainer->SetName(CString(name)+CString(_T(".Container")));
			}			
			m_DepthLayer = rhs.m_DepthLayer;

			m_StateMachine = rhs.m_StateMachine;
			
			if (rhs.IsPhysics())
			{
				if (!vCreatePhysics(rhs.GetlpPhyObj()->GetlpWorld()))
					return LYNX_FALSE;
			}
			return LYNX_TRUE;
		}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------	
		//ScriptSystem::CScriptClass* CDynamicObj::vGetBaseClassScript(const CString& s)
		//{
		//	LYNX_ASSERT(m_lpEngine->GetlpScriptSystem());

		//	ScriptSystem::CScriptClass* lpBaseClass = m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->FindClass(m_ClassScriptClassName);
		//	if (!lpBaseClass)
		//	{
		//		ScriptSystem::CScript* lpS = m_lpEngine->GetlpScriptSystem()->AddScript();			
		//		lpS->vCompile(s, NULL, 0);		
		//		CDynamicObj::vRegisterScriptClassTable(lpS, m_ClassScriptClassName);		
		//		CDynamicObj::vRegisterScriptClassMethod(lpS);
		//		CDynamicObj::vRegisterScriptClassVar(lpS);
		//		lpBaseClass = lpS->GetlpScriptClass();
		//	}
		//	return lpBaseClass;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------	
		//LYNXBOOL CDynamicObj::vLoadClassScript(const CString& s)
		//{
		//	ScriptSystem::CScriptClass* lpBaseClass = CGameObj::vGetBaseClassScript("../script/class/CGameObj.lua");
		//	CGameObj::vLoadClassScript(s);
		//	m_lpScript->GetlpScriptClass()->SetlpBaseClass(lpBaseClass);

		//	return LYNX_TRUE;
		//}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDynamicObj::CreateReferenceContainer(const LYNXCHAR* name)
		{
			CCreationDesc	Desc;
			
			Desc.Default();			
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
			vCreate(CString(name), CContainer::REFERENCE, &Desc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDynamicObj::CreateModelContainer(const LYNXCHAR* name, const LYNXCHAR* filename)
		{
			CPlatformFileStream FileStream;
			CModelCreationDesc	ModelDesc;

			ModelDesc.m_bUseBufferObject = LYNX_TRUE;
			ModelDesc.m_FileName = filename;
			ModelDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			ModelDesc.m_Offset = 0;
			ModelDesc.m_bCreateMaterial = LYNX_TRUE;			
			vCreate(CString(name), CContainer::MODEL, &ModelDesc);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDynamicObj::CreateVoxelClusterContainer(const LYNXCHAR* name, const LYNXCHAR* filename)
		{
			CPlatformFileStream FileStream;
			CVoxelClusterCreationDesc Desc;

			Desc.m_ClusterSize = 16*16*16;
			Desc.m_LightDir = Math::CVector3(1.0f, 1.0f, -1.0f);
			Desc.m_DataSize = 0;
			Desc.m_lpData = NULL;
			Desc.m_FileName = filename;	
			Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			vCreate(CString(name), CContainer::VOXEL_CLUSTER, &Desc);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDynamicObj::CreateSpriteContainer(const LYNXCHAR* name, const LYNXCHAR* filename, float w, float h)
		{
			CSpriteCreationDesc	SpriteDesc;
			
			SpriteDesc.Default();			
			SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			SpriteDesc.m_TextureDesc.MipMapLevel = 1;
			SpriteDesc.m_TextureDesc.AlphaReference = 0.3f;
			SpriteDesc.m_Width = w;
			SpriteDesc.m_Height = h;
			SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			SpriteDesc.m_Offset = 0;
			SpriteDesc.m_FileName = filename;
			SpriteDesc.m_bCreateMaterial = LYNX_TRUE;			
			SpriteDesc.m_AlignmentType = 0;
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
			vCreate(CString(name), CContainer::SPRITE, &SpriteDesc);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDynamicObj::CreateSprite3DContainer(const LYNXCHAR* name, const LYNXCHAR* filename, float w, float h)
		{
			CSprite3DCreationDesc	SpriteDesc;
			
			SpriteDesc.Default();			
			SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			SpriteDesc.m_TextureDesc.MipMapLevel = 0;
			SpriteDesc.m_TextureDesc.AlphaReference = 0.3f;
			SpriteDesc.m_Width = w;
			SpriteDesc.m_Height = h;
			SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			SpriteDesc.m_Offset = 0;
			SpriteDesc.m_FileName = filename;
			SpriteDesc.m_bCreateMaterial = LYNX_TRUE;						
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
			vCreate(CString(name), CContainer::SPRITE3D, &SpriteDesc);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDynamicObj::CreateClusterSubContainer(const LYNXCHAR* name, CContainer* lpc, int index)
		{
			CClusterSubCreationDesc	SubDesc;
			
			SubDesc.Default();			
			SubDesc.m_lpClusterContainer = lpc;
			SubDesc.m_SubContainerIndex = index;

			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
			vCreate(CString(name), CContainer::CLUSTER_SUB, &SubDesc);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDynamicObj::CreateSprite3DClusterContainer(const LYNXCHAR* name, const LYNXCHAR* filename, int cluster_size)
		{
			CSprite3DClusterCreationDesc	SpriteDesc;
			
			SpriteDesc.Default();			
			SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
			SpriteDesc.m_TextureDesc.MipMapLevel = 0;
			SpriteDesc.m_TextureDesc.AlphaReference = 0.3f;
			SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			SpriteDesc.m_Offset = 0;
			SpriteDesc.m_FileName = filename;
			SpriteDesc.m_bCreateMaterial = LYNX_TRUE;
			SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
			SpriteDesc.m_ClusterSize = cluster_size;	

			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
			vCreate(CString(name), CContainer::SPRITE3D_CLUSTER, &SpriteDesc);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CDynamicObj::CreatePEmitterContainer(const LYNXCHAR* name, const LYNXCHAR* filename, int cluster_size, int max_particle, int density, float life, float range)
		{
			CPEmitterCreationDesc PEDesc;
			CSprite3DClusterCreationDesc PDesc;
			CSpriteCreationDesc SDesc;	
			
			PDesc.m_bUseBufferObject = LYNX_TRUE;
			PDesc.m_ClusterSize = cluster_size;	
			PDesc.m_Number = 1;
			PDesc.m_FileName = filename; 

			PEDesc.m_ParticleType = LynxEngine::CContainer::SPRITE_CLUSTER;			
			PEDesc.m_ParticleDesc = &PDesc;				
			PEDesc.m_MaxParticle = max_particle;
			PEDesc.m_Density = density;
			PEDesc.m_LifeTimeLimit = life;
			PEDesc.m_Range = range;	
			PEDesc.m_Type = PE_POINT;	
			
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));
			vCreate(name, CContainer::PEMITTER, &PEDesc);
			((CPEmitterContainer*)(GetlpContainer()))->vCreateDefaultResource();	
			((CPEmitterContainer*)(GetlpContainer()))->m_Gravity = Math::CVector3(0, -0.1, 0);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CDynamicObj::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "CreateModel",		LynxEngine::SceneSystem::CDynamicObj, void,	CreateModelContainer, (const LYNXCHAR*, const LYNXCHAR*))
			REGISTER_SCRIPT_METHOD(lps, "CreateSprite",		LynxEngine::SceneSystem::CDynamicObj, void,	CreateSpriteContainer, (const LYNXCHAR*, const LYNXCHAR*, float, float))				
			REGISTER_SCRIPT_METHOD(lps, "CreatePEmitter",	LynxEngine::SceneSystem::CDynamicObj, void,	CreatePEmitterContainer, (const LYNXCHAR*, const LYNXCHAR*, int, int, int, float, float))				

			REGISTER_SCRIPT_METHOD(lps, "CreatePhysics",	LynxEngine::SceneSystem::CDynamicObj, LYNXBOOL,	CreatePhysics, (int))						

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CDynamicObj::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			//REGISTER_SCRIPT_VAR(lps, "m_ID", LynxEngine::CObj, m_ID)
		
			return LYNX_TRUE;
		}
		////-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDynamicObj::UpdateTransform(OBJUPDATETYPE type) 
		{
			if (type == OBJUPDATE_RENDERABLEOBJ)
			{
				CRenderableObj::UpdateTransform(); 
				if (m_lpPhyObj) 
				{
					m_lpPhyObj->vSetTransformMatrix(GetTransformMatrix());
				}
			}
			else //Update by physics system
			{
				if (m_lpPhyObj) 
				{					
					SetTransformMatrix(m_lpPhyObj->vGetTransformMatrix());						
					//m_lpContainer->SetGlobalMatrix((m_lpPhyObj->vGetTransformMatrix()));					
				}				
			}

			if (m_lpRagdoll)
			{
				m_lpRagdoll->UpdateTransform(type);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDynamicObj::vAddToSceneVisList()
		{
			m_lpScene->GetlpRenderer()->AddObjToVisList(this);												
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDynamicObj::vLoop(float step)
		{	
			if (m_lpPhyObj || m_lpRagdoll)
			{
				if (m_lpPhyObj->vIsActive())
					UpdateTransform(OBJUPDATE_PHYSICSOBJ);					
			}
			else
				UpdateTransform(OBJUPDATE_RENDERABLEOBJ);//UPDATE_PHYSICSOBJ);										
			
			CGameObj::vLoop(step);

			if (m_bActive && m_lpContainer->vPlay(step))		
			{
				if (m_StateMachine->GetlpCurrentState())
				{						
					if (m_StateMachine->GetlpCurrentState()->GetEventForAnimationStop())
						vUpdateStateMachine(CEvent(CEvent::KERNEL_EVENT, m_StateMachine->GetlpCurrentState()->GetEventForAnimationStop()->GetName()));								
					else
						vUpdateStateMachine(StateMachine::CStateMachine::GetAnimationStopEvent());					
				}
			}

			if (m_HierarchyData.Parent.lpObj && m_HierarchyData.Parent.lpObj != m_lpScene->GetlpRoot())
			{
				if (m_bInheritParentTransform)
				{
					if (m_HierarchyData.Parent.lpSubObj)
					{
						MulMatrix(Container_Cast<CContainer*>(m_HierarchyData.Parent.lpSubObj)->GetTransformMatrix(), LYNX_MATH_PRE);
					}
					else
					{
						MulMatrix(((CRenderableObj*)(m_HierarchyData.Parent.lpObj))->GetTransformMatrix(), LYNX_MATH_PRE);
					}
				}
			}

			for (int i=0; i<m_HierarchyData.ChildArray.size(); ++i)
				m_HierarchyData.ChildArray[i]->vLoop(step);
		}						
	}
}