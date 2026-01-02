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
#include <SoundSystem/LynxMusic.h>

namespace LynxEngine 
{	
	namespace SoundSystem 
	{
		//IMPLEMENT_CLASSSCRIPT(CMusic, CObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMusic::CMusic(CEngine* lpengine)
		: CResource(lpengine)
		{				
			m_ResourceType = MUSIC;
			m_Volume = 1.0f;
			m_bStreaming = LYNX_FALSE;
			m_ReadyForStreming = LYNX_FALSE;

			//INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMusic::~CMusic(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")))
		}				
	}
}