//#######################################################################
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
#include <LynxRenderableObj.h>
#include <LynxEngine.h>
#include <LynxObjProxy.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <Container/LynxTerrainContainer.h>
#include <Container/LynxCameraContainer.h>
#include <StateMachine/LynxStateMachine.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>
#include <StateMachine/LynxState.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>
#ifdef __LUA__
	#include <LynxLuaScript.h>
#endif

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(CRenderableObj::DEPTHLAYERTYPE e)
	{
		CString				DepthLayer;

		switch (e)
		{
			case CRenderableObj::DEPTH_LAYER_0:
				DepthLayer = _T("Depth Layer 0");
				break;
			case CRenderableObj::DEPTH_LAYER_1:
				DepthLayer = _T("Depth Layer 1");
				break;
			case CRenderableObj::DEPTH_LAYER_2:
				DepthLayer = _T("Depth Layer 2");
				break;
			case CRenderableObj::DEPTH_LAYER_3:
				DepthLayer = _T("Depth Layer 3");
				break;
			case CRenderableObj::DEPTH_LAYER_4:
				DepthLayer = _T("Depth Layer 4");
				break;
			case CRenderableObj::DEPTH_LAYER_5:
				DepthLayer = _T("Depth Layer 5");
				break;
		}
		return DepthLayer;
	}
	IMPLEMENT_CLASSSCRIPT(CRenderableObj, CGameObj)
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CRenderableObj::CRenderableObj(void)
	{
		m_SourceType = ORIGIN;
		m_DepthLayer = DEPTH_LAYER_0;
		m_lpContainer = NULL;
		m_lpPhyObj = NULL;		
		m_VisStaticLightList.clear();
		m_VisDynamicLightList.clear();		

		m_bAttachedToTerrain = LYNX_TRUE;
		m_lpAttachedTerrain = NULL;

		m_bShow = LYNX_TRUE;

		m_ClassScriptClassName = ClassScriptClassName();

		IMPLEMENT_PARAM_ENUM(m_DepthLayer, 0);
		IMPLEMENT_PARAM_BOOL(m_bAttachedToTerrain, 0);
		IMPLEMENT_PARAM_BOOL(m_bShow, 0);		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CRenderableObj::CRenderableObj(CEngine* lpe)
	: CGameObj(lpe)
	{
		m_SourceType = ORIGIN;
		m_DepthLayer = DEPTH_LAYER_0;
		m_lpContainer = NULL;
		m_lpPhyObj = NULL;
		m_VisStaticLightList.clear();
		m_VisDynamicLightList.clear();		

		m_bAttachedToTerrain = LYNX_TRUE;
		m_lpAttachedTerrain = NULL;

		m_bShow = LYNX_TRUE;

		INIT_CLASSSCRIPT

		IMPLEMENT_PARAM_ENUM(m_DepthLayer, 0);
		IMPLEMENT_PARAM_BOOL(m_bAttachedToTerrain, 0);
		IMPLEMENT_PARAM_BOOL(m_bShow, 0);	
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CRenderableObj::~CRenderableObj(void)
	{
		if (m_lpContainer)
		{
			m_lpEngine->DestroyContainer(m_lpContainer);
			m_lpContainer = NULL;
		}
	}			
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CRenderableObj::Create(CContainer::TYPE type)
	{
		LYNX_ASSERT(m_lpEngine);			

		m_lpContainer = m_lpEngine->CreateContainer(type);		
		m_lpContainer->SetlpRenObj(this);
		m_lpContainer->SetName(GetName() + CString(_T(".Container")));

		m_VisStaticLightList.clear();
		m_VisDynamicLightList.clear();

		return LYNX_TRUE;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CRenderableObj::SetAttachedTerrain(CTerrainContainer* t) 
	{
		m_lpAttachedTerrain = t;
		t->AttachObject(this);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CRenderableObj::ComputeVisLightList()
	{			
		m_VisDynamicLightList.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CRenderableObj::vSetCurrentState(int s) 
	{
		if (CGameObj::vSetCurrentState(s))
		{
			m_lpContainer->SetCurrentAnimation(m_StateMachine->GetlpCurrentState()->GetlpAnimation());
			return LYNX_TRUE;
		}
		return LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CRenderableObj::vSetCurrentState(const CString& s) 
	{
		if (CGameObj::vSetCurrentState(s))
		{
			m_lpContainer->SetCurrentAnimation(m_StateMachine->GetlpCurrentState()->GetlpAnimation());
			return LYNX_TRUE;
		}
		return LYNX_FALSE;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CRenderableObj::vUpdateStateMachine(const CEvent& event)
	{
		if (m_StateMachine->Update(event))		
		{
			m_lpContainer->SetCurrentAnimation(m_StateMachine->GetlpCurrentState()->GetlpAnimation());
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CRenderableObj::vOnEvent(const CEvent& event)
	{
		if (m_StateMachine->Update(event))		
		{
			m_lpContainer->SetCurrentAnimation(m_StateMachine->GetlpCurrentState()->GetlpAnimation());
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	StateMachine::CState* CRenderableObj::CreateState(const CString& name, const CString&filename)
	{
		StateMachine::CState* State = m_StateMachine->CreateState();	
		State->SetName(name);
		Animation::CAnimation* Ani = LoadAnimation(name, filename);		
		State->SetlpAnimation(Ani);    
		m_StateMachine->AddState(State);

		return State;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CRenderableObj::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
	{			
		RegisterSuperClassMethod(lps);

		REGISTER_SCRIPT_METHOD(lps, "GetContainer",					LynxEngine::CRenderableObj,	void*,			GetlpContainer,				())
		REGISTER_SCRIPT_METHOD(lps, "SetDepthLayer",				LynxEngine::CRenderableObj,	void,			SetDepthLayer,				(int))			
		REGISTER_SCRIPT_METHOD(lps, "Forward",						LynxEngine::CRenderableObj,	void,			Forward,					(LYNXREAL))
		REGISTER_SCRIPT_METHOD(lps, "Yaw",							LynxEngine::CRenderableObj,	void,			Yaw,						(const Math::CReal, int))
		REGISTER_SCRIPT_METHOD(lps, "Pitch",						LynxEngine::CRenderableObj,	void,			Pitch,						(const Math::CReal, int))
		REGISTER_SCRIPT_METHOD(lps, "SetPosition",					LynxEngine::CRenderableObj,	void,			SetPosition,				(const Math::CVector3&))
		REGISTER_SCRIPT_METHOD(lps, "GetPosition",					LynxEngine::CRenderableObj,	void,			GetPosition,				(Math::CVector3&))
		REGISTER_SCRIPT_METHOD(lps, "GetDirection",					LynxEngine::CRenderableObj,	void,			GetDirection,				(Math::CVector3&))
		REGISTER_SCRIPT_METHOD(lps, "UpdateTransform",				LynxEngine::CRenderableObj,	void,			UpdateTransform,			(void))
		REGISTER_SCRIPT_METHOD(lps, "ResetTransform",				LynxEngine::CRenderableObj,	void,			ResetTransform,				(void))
		REGISTER_SCRIPT_METHOD(lps, "Play",							LynxEngine::CRenderableObj,	LYNXBOOL,		Play,						(const Math::CReal))		
		REGISTER_SCRIPT_METHOD(lps, "Render",						LynxEngine::CRenderableObj,	void,			_Render,					(void*))		
		REGISTER_SCRIPT_METHOD(lps, "LoadAnimation",				LynxEngine::CRenderableObj,	void*,			_LoadAnimation,				(const LYNXCHAR*))
		REGISTER_SCRIPT_METHOD(lps, "SetCurrentAnimation",			LynxEngine::CRenderableObj,	void,			SetCurrentAnimationByPtr,	(void*))
		REGISTER_SCRIPT_METHOD(lps, "SetCurrentAnimationByName",	LynxEngine::CRenderableObj,	void*,			SetCurrentAnimationByName,	(const LYNXCHAR*))
		REGISTER_SCRIPT_METHOD(lps, "IsCurrentAnimationStopped",	LynxEngine::CRenderableObj,	LYNXBOOL,		IsCurrentAnimationStopped,	(void))		
		REGISTER_SCRIPT_METHOD(lps, "GetPhysicsObj",				LynxEngine::CRenderableObj,	void*,			GetlpPhyObj,				(void))						
			
		return LYNX_TRUE;
	}		
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CRenderableObj::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
	{
		RegisterSuperClassVar(lps);

		return LYNX_TRUE;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CRenderableObj::DestoryPhysics()
	{
		if (m_lpPhyObj)
		{
			m_lpPhyObj->GetlpWorld()->vDeleteObj(m_lpPhyObj);
			m_lpPhyObj = NULL;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CRenderableObj::vRenderCollisionHie(CCameraContainer* cam)
	{
		if (m_lpContainer && m_lpContainer->m_lpCollisionHierarchy)
		{
			for (int i = 0; i < m_lpContainer->m_lpCollisionHierarchy->GetNumLevels(); i++)
			{
				for (CList<CollisionSystem::CCollisionObj*>::iterator Obj = m_lpContainer->m_lpCollisionHierarchy->GetCollisionObjList(i).begin(); Obj != m_lpContainer->m_lpCollisionHierarchy->GetCollisionObjList(i).end(); Obj++)
				{
					if ((*Obj))
						(*Obj)->vRender(*cam);
				}
			}
			/*
			for (CList<CContainer*>::iterator Con = m_lpContainer->GetSubContainerList().begin(); Con != m_lpContainer->GetSubContainerList().end(); Con++)
			{
				for (int i = 0; i < (*Con)->m_lpCollisionHierarchy->GetNumLevels(); i++)
				{
					for (CList<CollisionSystem::CCollisionObj*>::iterator Obj = (*Con)->m_lpCollisionHierarchy->GetCollisionObjList(i).begin(); Obj != (*Con)->m_lpCollisionHierarchy->GetCollisionObjList(i).end(); Obj++)
					{
						if ((*Obj))
							(*Obj)->vRender(*cam);
					}
				}
			}
			*/
		}
	}
}