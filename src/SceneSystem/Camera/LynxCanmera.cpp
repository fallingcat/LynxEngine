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
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/Camera/LynxCamera.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CCamera, CDynamicObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCamera::CCamera(CScene* const lpscene)
		: CDynamicObj(lpscene)
		{				
			INIT_CLASSSCRIPT
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCamera::~CCamera(void)
		{
			vRelease();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CCamera::vRelease(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCamera::vCreate()
		{		
			LYNX_ASSERT(m_lpScene);

			if (!CRenderableObj::Create(CContainer::CAMERA))
				return LYNX_FALSE;

			m_lpContainer->SetlpRenObj(this);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CCamera::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "Create",					LynxEngine::SceneSystem::CCamera,	LYNXBOOL, vCreate, ())

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CCamera::vLoop(float step)
		{
			CVector3 Pos, Look, PosDir, LookDir;

			m_lpContainer->vGetPosition(Pos);
			((CCameraContainer*)m_lpContainer)->GetLookAtPoint(Look);

			PosDir = m_TransitionPos - Pos;
			LookDir = m_TransitionLook - Look;

			if (PosDir.Length(3) > 1.0f || LookDir.Length(3) > 1.0)
			{
				Pos += PosDir * m_TrasitionStep * step;
				Look += LookDir * m_TrasitionStep * step;
				((CCameraContainer*)m_lpContainer)->vSetPosition(Pos);
				((CCameraContainer*)m_lpContainer)->LookAt(Look);
				((CCameraContainer*)m_lpContainer)->UpdateViewMatrix();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CCamera::TransitTo(const CVector3& pos, const CVector3& look, const float& step)
		{
			m_TransitionPos = pos;
			m_TransitionLook = look;
			m_TrasitionStep = step;
		}
	}
}