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
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/StaticObj/LynxStaticObj.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>
#include <StateMachine/LynxState.h>
#include <StateMachine/LynxStateMachine.h>
#include <StateMachine/LynxStateTranslation.h>
#ifdef __LUA__
	#include <LynxLuaScript.h>
#endif

namespace LynxEngine 
{
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(SceneSystem::CStaticObj::TYPE e)
	{
		CString				StaticObjType;

		switch (e)
		{
			case SceneSystem::CStaticObj::STATIC_MESH:
				StaticObjType = _T("Static Mesh");
				break;
			case SceneSystem::CStaticObj::STATIC_MODEL:
				StaticObjType = _T("Static Model");
				break;
			case SceneSystem::CStaticObj::SKY:
				StaticObjType = _T("Sky");
				break;
			case SceneSystem::CStaticObj::CLOUD:
				StaticObjType = _T("Cloud");
				break;
			case SceneSystem::CStaticObj::TERRAIN:
				StaticObjType = _T("Terrain");
				break;			
			case SceneSystem::CStaticObj::TREE:
				StaticObjType = _T("Tree");
				break;			
			case SceneSystem::CStaticObj::GRASS:
				StaticObjType = _T("Grass");
				break;			
		}
		return StaticObjType;
	}
	namespace SceneSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CStaticObj, CRenderableObj)		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObj::CStaticObj(CScene* const lpscene)
		: CRenderableObj(lpscene->GetlpSceneSystem()->GetlpEngine())
		{
			m_lpScene = lpscene;
			m_SourceType = ORIGIN;
			m_VisStaticLightList.clear();
			m_VisDynamicLightList.clear();	

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObj::~CStaticObj(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStaticObj::vCreate(const LYNXCHAR *name, CContainer::TYPE mtype, CCreationDesc* desc, LYNXBOOL bphy, PhysicsSystem::CPhysicsWorld* lpw)
		{
			LYNX_GUARD(CStaticObj::vCreate)

			SetName(name);
			CRenderableObj::Create(mtype);				
			m_lpContainer->SetName(CString(name)+CString(_T(".Container")));
			if (desc)			
				m_lpContainer->vCreate(desc);			

			if (bphy && lpw)
			{
				if (!vCreatePhysics(lpw))
					return LYNX_FALSE;
			}
			
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
		LYNXBOOL CStaticObj::vInstance(const LYNXCHAR *name, CStaticObj& rhs)
		{
			m_SourceType = INSTANCE;
			SetName(name);
			CRenderableObj::Create(rhs.GetlpContainer()->GetContainerType());				
			m_lpContainer->vInstance(*(rhs.GetlpContainer()));			
			m_lpContainer->SetName(CString(name)+CString(_T(".Container")));

			m_StateMachine = rhs.m_StateMachine;
			
			if (rhs.IsPhysics())
			{
				if (!vCreatePhysics(rhs.GetlpPhyObj()->GetlpWorld()))
					return LYNX_FALSE;
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStaticObj::CreateStaticMesh(const LYNXCHAR *name, const LYNXCHAR *filename)
		{
			CGeometryCreationDesc	GeometryDesc;

			GeometryDesc.m_bUseBufferObject = LYNX_TRUE;
			GeometryDesc.m_FileName = filename;
			GeometryDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			GeometryDesc.m_Offset = 0;
			GeometryDesc.m_bCreateMaterial = LYNX_TRUE;			
			
			return vCreate(name, CContainer::GEOMETRY, &GeometryDesc);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStaticObj::CreateStaticModel(const LYNXCHAR *name, const LYNXCHAR *filename)
		{
			CModelCreationDesc	ModelDesc;

			ModelDesc.m_bUseBufferObject = LYNX_TRUE;
			ModelDesc.m_FileName = filename;
			ModelDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
			ModelDesc.m_Offset = 0;
			ModelDesc.m_bCreateMaterial = LYNX_TRUE;			
			
			return vCreate(name, CContainer::MODEL, &ModelDesc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStaticObj::vLoop(float step)
		{
			//UpdateTransform(UPDATE_RENDERABLEOBJ);//UPDATE_PHYSICSOBJ);										

			CGameObj::vLoop(step);

			UpdateTransform();
			if (m_lpContainer->vPlay(step))		
			{
				if (m_StateMachine->GetlpCurrentState())
				{						
					if (m_StateMachine->GetlpCurrentState()->GetEventForAnimationStop())
						vUpdateStateMachine(CEvent(CEvent::KERNEL_EVENT, m_StateMachine->GetlpCurrentState()->GetEventForAnimationStop()->GetName()));								
					else
						vUpdateStateMachine(StateMachine::CStateMachine::GetAnimationStopEvent());					
				}
			}
 
			for (int i=0; i<m_HierarchyData.ChildArray.size(); ++i)
				m_HierarchyData.ChildArray[i]->vLoop(step);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStaticObj::vCreatePhysics(PhysicsSystem::CPhysicsWorld* lpw, PhysicsSystem::CPhysicsShape* lps)
		{
			LYNX_GUARD(CStaticObj::vCreatePhysics)

			if (m_lpPhyObj)
			{
				lpw->vDeleteObj(m_lpPhyObj);
			}				

			LYNX_ASSERT(lpw != NULL);
			
			m_lpPhyObj = lpw->vCreateObj(PhysicsSystem::CPhysicsObj::STATIC, lps);
			if (!m_lpPhyObj)
				return LYNX_FALSE;
				//throw CException(_T("This static object without physical object"));

			CString Name;

			Name = GetName() + CString(".PhyObj");
			m_lpPhyObj->SetName(Name);
			m_lpPhyObj->SetlpRenObj(this);

			return LYNX_TRUE;

			#ifndef __NO_GUARD__		
				LYNX_UNGUARD
				catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
				LYNX_DEFAULTCATCH
			#endif
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CStaticObj::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "CreateStaticMesh",		LynxEngine::SceneSystem::CStaticObj, LYNXBOOL,	CreateStaticMesh, (const LYNXCHAR*, const LYNXCHAR*))
			REGISTER_SCRIPT_METHOD(lps, "CreateStaticModel",	LynxEngine::SceneSystem::CStaticObj, LYNXBOOL,	CreateStaticModel, (const LYNXCHAR*, const LYNXCHAR*))				
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CStaticObj::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			//REGISTER_SCRIPT_VAR(lps, "m_ID", LynxEngine::CObj, m_ID)
		
			return LYNX_TRUE;
		}
	}
}