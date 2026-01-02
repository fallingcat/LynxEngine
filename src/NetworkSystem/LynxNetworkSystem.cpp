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
#include <NetworkSystem/LynxNetworkSystem.h>
#ifdef __CURL__
	#include <curl/curl.h>	
#endif
#ifdef __PHOTON__
	
#endif

namespace LynxEngine 
{	
	namespace NetworkSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CNetworkSystem::CNetworkSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{		
			m_bOffline = LYNX_FALSE;
			m_Game = NULL;
			m_NetClient = NULL;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CNetworkSystem::~CNetworkSystem(void)
		{			
			if (m_Game)
			{
				LYNXDEL m_Game;
				m_Game = NULL;
			}

			if (m_NetClient)
			{
				LYNXDEL m_NetClient;
				m_NetClient = NULL;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CNetworkSystem::vCreate(void)
		{			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CNetworkSystem::vInit()
		{
			#ifdef __CURL__				
				curl_global_init(CURL_GLOBAL_ALL);
			#endif

			return LYNX_TRUE;
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CNetworkSystem::CreateNetClient(CAnsiString& name)
		{
			if (m_NetClient)
			{
				LYNXDEL m_NetClient;
				m_NetClient = NULL;
			}
			m_NetClient = m_NetClientFactory.CreateObject(name);

			if (!m_NetClient)
				return LYNX_FALSE;

			m_NetClient->vCreate();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CNetworkSystem::RegisterNetClient(const CAnsiString& t, CNetClient* (*func)(CEngine*))
		{
			m_NetClientFactory.Register(t, func);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CNetworkSystem::SetOffline(LYNXBOOL b)
		{
			m_bOffline = b;
		}
	}
}