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
#include <SoundSystem/LynxSound.h>

namespace LynxEngine 
{	
	namespace SoundSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CSound, CObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSound::CSound(CEngine* lpengine)
		: CResource(lpengine)
		{				
			m_ResourceType = SOUND;
			m_NumLoops = 1;
			m_Volume = 1.0f;
			m_bPlayed = LYNX_FALSE;
			m_Priority = SP_MEDIUM;

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSound::~CSound(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")))
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSound::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "SetLoops",	 LynxEngine::SoundSystem::CSound,	void,		SetLoops, 	(int))
			REGISTER_SCRIPT_METHOD(lps, "SetVolume", LynxEngine::SoundSystem::CSound,	void,		SetVolume, 	(float))
			REGISTER_SCRIPT_METHOD(lps, "Play",		 LynxEngine::SoundSystem::CSound,	LYNXBOOL,	Play, 		(void))
			REGISTER_SCRIPT_METHOD(lps, "Pause",	 LynxEngine::SoundSystem::CSound,	void,		Pause, 		(void))
			REGISTER_SCRIPT_METHOD(lps, "Stop",		 LynxEngine::SoundSystem::CSound,	void,		Stop, 		(void))
			REGISTER_SCRIPT_METHOD(lps, "IsPlaying", LynxEngine::SoundSystem::CSound,	LYNXBOOL,	IsPlaying,	(void))			
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::AddToPlayingList(SOUNDPRIORITY priority)
		{
			m_lpEngine->GetlpSoundSystem()->AddToPlayingList(this, priority);
		}
	}
}