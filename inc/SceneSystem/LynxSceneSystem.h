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

#ifndef __LYNXSCENESYSTEM_H__
#define __LYNXSCENESYSTEM_H__

#include <LynxList.h>
#include <LynxSystem.h>

#include <SceneSystem/LynxScn.h>

namespace LynxEngine 
{
	class CEngine;
	namespace SceneSystem 
	{
		class LYNXENGCLASS CSceneSystem : public CSystem
		{
		protected:					
			CList<CScene*>						m_SceneList;
			CScene*								m_lpCurrentScene;
		public:
			LYNX_DECLARE_CLASS(CSceneSystem);
			CSceneSystem(CEngine* const lpengine);
			~CSceneSystem();		

			DECLARE_CLASSSCRIPT
			LYNXBOOL							vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			LYNXFORCEINLINE CScene* const		_AddScene(const CCreationDesc* desc = NULL) { return AddScene(desc); }

			LYNXBOOL							vCreate();
			LYNXBOOL							vInit();
			CScene* const						AddScene(const CCreationDesc* desc = NULL);
			void								AddScene(CScene* scene);
			CScene* const						GetlpScene(int i) {return m_SceneList[i]; };
			LYNXINLINE void						SetCurrentScene(int i) {SetCurrentScene(m_SceneList[i]); };
			void								SetCurrentScene(CString& name);
			void								SetCurrentScene(CScene* scene);
			LYNXINLINE CScene* const			GetlpCurrentScene() {return m_lpCurrentScene;}
			void								RemoveScene(CScene *lpscene);
			void								RemoveAllScenes();
		};	
	}
}

#endif