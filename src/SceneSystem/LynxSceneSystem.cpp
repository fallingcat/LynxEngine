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
#include <LynxAlgorithm.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CSceneSystem, CSystem)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSceneSystem::CSceneSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{
			m_SceneList.clear();
			m_lpCurrentScene = NULL;

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSceneSystem::~CSceneSystem(void)
		{
			RemoveAllScenes();
		}								
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSceneSystem::vCreate()
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSceneSystem::vInit()
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScene* const CSceneSystem::AddScene(const CCreationDesc* desc)
		{
			CScene* lpS = LYNXNEW CScene(this);
			m_SceneList.push_back(lpS); 	
			SetCurrentScene(lpS);			
			lpS->Create(desc);			

			return lpS;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSceneSystem::AddScene(CScene* scene)
		{
			m_SceneList.push_back(scene); 	
			SetCurrentScene(scene);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSceneSystem::RemoveAllScenes()
		{
			for (CList<CScene*>::CIterator i=m_SceneList.begin(); i != m_SceneList.end(); ++i)
                LYNXDEL (*i);
			m_SceneList.clear(); 		
			m_lpCurrentScene = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSceneSystem::RemoveScene(CScene *lpscene)
		{
			LYNXDEL lpscene;
			m_SceneList.remove(lpscene); 			
			if (m_lpCurrentScene == lpscene)
				m_lpCurrentScene = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSceneSystem::SetCurrentScene(CString& name)
		{
			CList<CScene*>::CIterator Scn;

			Scn = LynxEngine::find(m_SceneList.begin(), m_SceneList.end(), name);
			if (Scn != m_SceneList.end())
			{
				m_lpCurrentScene = *Scn;
				m_lpCurrentScene->ScriptBindObject();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSceneSystem::SetCurrentScene(CScene *lpscene)
		{
			CList<CScene*>::CIterator Scn;

			if (lpscene)
			{
				Scn = LynxEngine::find(m_SceneList.begin(), m_SceneList.end(), lpscene);
				if (Scn != m_SceneList.end())
				{
					m_lpCurrentScene = *Scn;
					m_lpCurrentScene->ScriptBindObject();

					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentAmbientLight(static_cast<CAmbientLight*>(m_lpCurrentScene->GetCurrentAmbientLight()));
					if (m_lpCurrentScene->GetNumLights() > 0)
						m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentLight(m_lpCurrentScene->GetlpLight(0));
				}
			}
			else
			{
				m_lpCurrentScene = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSceneSystem::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "AddScene",			LynxEngine::SceneSystem::CSceneSystem,	void*,	_AddScene,			(void))
			REGISTER_SCRIPT_METHOD(lps, "RemoveAllScenes",	LynxEngine::SceneSystem::CSceneSystem,	void,	RemoveAllScenes,	(void))
			REGISTER_SCRIPT_METHOD(lps, "GetCurrentScene",	LynxEngine::SceneSystem::CSceneSystem,	void*,	GetlpCurrentScene,	(void))
			REGISTER_SCRIPT_METHOD(lps, "GetScene",			LynxEngine::SceneSystem::CSceneSystem,	void*,	GetlpScene,			(int))
			
			return LYNX_TRUE;
		}
	}
}