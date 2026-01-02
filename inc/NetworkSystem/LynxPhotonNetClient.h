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
#ifndef __LYNXPHOTONNETCLIENT_H__
#define __LYNXPHOTONNETCLIENT_H__
	
#ifdef __PHOTON__

	#include <LynxEngineClasses.h>
	#include <NetworkSystem/LynxNetClient.h>
	#include <LoadBalancing-cpp/inc/Client.h>

	namespace LynxEngine 
	{
		namespace NetworkSystem 
		{
			namespace Photon
			{
				class LYNXENGCLASS CNetClient : public LynxEngine::NetworkSystem::CNetClient
				{
					LYNX_DECLARE_CLASS(CNetClient);
				protected:					
					static ExitGames::Common::JString		m_AppId;
					static ExitGames::Common::JString		m_AppVersion;
					CNetListener*							m_Listener;
					LYNXTHREADHANDLE						m_ThreadHandle;
					LYNXMUTEX								m_Mutex;
				public:
					CNetClient(CEngine* const lpengine);			
					~CNetClient();

					static CNetClient::PEERSTATETYPE		TrnslatePeerState(ExitGames::LoadBalancing::PeerStates::PeerStates s);

					virtual LYNXBOOL						vCreate(void);
					virtual LYNXBOOL						vInit();
					virtual void							vLoop(float step);
					virtual LYNXBOOL						vConnect();
					virtual LYNXBOOL						vDisconnect();
					virtual LYNXBOOL						vCreateGame(const CString& name, const NETGAMEDESC& gameDesc = NETGAMEDESC());
					virtual LYNXBOOL						vJoinGame(const CString& name);	
					virtual void							vOnConnect();
					virtual void							vOnCreateGame();
					virtual void							vOnJoinGame(int localPlayerNr);
					virtual void							vOnPlayerJoinGame(int playerNr);
					virtual void							vOnPlayerLeaveGame(int playerNr);
					virtual void							vSendCmd(LYNXBYTE eventCode, LYNXBYTE* parameters, int size, LYNXBOOL reliable, int *targetPlayers = NULL, int targetPlayersNum = 0);		
					virtual void							vOnCmd(int player, LYNXBYTE eventCode, LYNXBYTE* parameters);

					virtual LYNXBOOL						vIsConnected();
					virtual LYNXBOOL						vIsJoined();
					virtual LYNXBOOL						vIsMasterPlayer();
					virtual CNetClient::PEERSTATETYPE		vGetPeerState();

					bool									m_IsHost;
					LYNXFORCEINLINE CNetListener*			GetListener(){ return m_Listener; }			
					static void								LoopInBackgroundThread(void* data);
					LYNXFORCEINLINE LYNXMUTEX&				GetMutex(){ return m_Mutex; }
				};	
			}
		}
	}

#endif

#endif