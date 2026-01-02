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

#ifdef __PHOTON__

#include <NetworkSystem/LynxPhotonNetListener.h>
#include <NetworkSystem/LynxPhotonNetClient.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		namespace Photon
		{
			ExitGames::Common::JString		CNetClient::m_AppId;
			ExitGames::Common::JString		CNetClient::m_AppVersion;
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CNetClient::PEERSTATETYPE CNetClient::TrnslatePeerState(ExitGames::LoadBalancing::PeerStates::PeerStates s)
			{
				switch (s)
				{
				case ExitGames::LoadBalancing::PeerStates::Uninitialized:
					return PST_UNINITIALIZED;					
					break;
				case ExitGames::LoadBalancing::PeerStates::PeerCreated:
					return PST_PEERCREATED;
					break;
				case ExitGames::LoadBalancing::PeerStates::ConnectingToNameserver:
					return PST_CONNECTINGTONAMESERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::ConnectedToNameserver:
					return PST_CONNECTEDTONAMESERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::DisconnectingFromNameserver:
					return PST_DISCONNECTINGFROMNAMESERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Connecting:
					return PST_CONNECTING;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Connected:
					return PST_CONNECTED;					
					break;
				case ExitGames::LoadBalancing::PeerStates::WaitingForCustomAuthenticationNextStepCall:
					return PST_WAITINGFORCUSTOMAUTHENTICATIONNEXTSTEPCALL;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Authenticated:
					return PST_AUTHENTICATED;					
					break;
				case ExitGames::LoadBalancing::PeerStates::JoinedLobby:
					return PST_JOINEDLOBBY;					
					break;
				case ExitGames::LoadBalancing::PeerStates::DisconnectingFromMasterserver:
					return PST_DISCONNECTINGFROMMASTERSERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::ConnectingToGameserver:
					return PST_CONNECTINGTOGAMESERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::ConnectedToGameserver:
					return PST_CONNECTEDTOGAMESERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::AuthenticatedOnGameServer:
					return PST_AUTHENTICATEDONGAMESERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Joining:
					return PST_JOINING;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Joined:
					return PST_JOINED;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Leaving:
					return PST_LEAVING;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Left:
					return PST_LEFT;					
					break;
				case ExitGames::LoadBalancing::PeerStates::DisconnectingFromGameserver:
					return PST_DISCONNECTINGFROMGAMESERVER;
					break;
				case ExitGames::LoadBalancing::PeerStates::ConnectingToMasterserver:
					return PST_CONNECTINGTOMASTERSERVER;
					break;
				case ExitGames::LoadBalancing::PeerStates::ConnectedComingFromGameserver:
					return PST_CONNECTEDCOMINGFROMGAMESERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::AuthenticatedComingFromGameserver:
					return PST_AUTHENTICATEDCOMINGFROMGAMESERVER;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Disconnecting:
					return PST_DISCONNECTING;					
					break;
				case ExitGames::LoadBalancing::PeerStates::Disconnected:
					return PST_DISCONNECTED;
					break;
				}
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CNetClient::CNetClient(CEngine* const lpengine)
			:LynxEngine::NetworkSystem::CNetClient(lpengine)
			{
				m_AppId = "09d8e256-1d34-4723-8eab-f80285dece55";
				//m_AppId = "c24f4f73-55be-454b-93a1-8f074ec8f95f";
				m_AppVersion = "1.1";
				m_Listener = NULL;
				m_IsHost = false;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CNetClient::~CNetClient(void)
			{
				if (m_Listener)
				{
					delete m_Listener;
					m_Listener = NULL;
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vCreate(void)
			{
				//ExitGames::Common::JString name = m_IsHost? ExitGames::Common::JString("Host"): ExitGames::Common::JString("Guest");
				//m_Listener = new CNetListener(this, m_AppId, m_AppVersion, name);
				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vInit()
			{
				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vConnect()
			{
				lynxOSCreateMutex(&m_Mutex);
				m_Listener->connect();
				m_ThreadHandle = lynxOSCreateThread((LPTHREADPROC)LoopInBackgroundThread, this);
				
                return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vDisconnect()
			{
				m_Listener->disconnect();
				lynxOSCloseThread(m_ThreadHandle);
				lynxOSDestoryMutex(&m_Mutex);

				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vCreateGame(const CString& name, const NETGAMEDESC& gameDesc)
			{
				ExitGames::Common::JString gamename = ExitGames::Common::JString(name.c_str());
				m_Listener->opCreateRoom(gamename, (const bool)gameDesc.IsVisible, (const bool)gameDesc.IsOpen, gameDesc.MaxPlayers);

				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vJoinGame(const CString& name)
			{
				ExitGames::Common::JString gamename = ExitGames::Common::JString(name.c_str());
				m_Listener->opJoinRoom(gamename);

				return LYNX_TRUE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::vOnConnect()
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::vOnCreateGame()
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::vOnJoinGame(int localPlayerNr)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::vOnPlayerJoinGame(int playerNr)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::vOnPlayerLeaveGame(int playerNr)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::vSendCmd(LYNXBYTE eventCode, LYNXBYTE* parameters, int size, LYNXBOOL reliable, int *targetPlayers, int targetPlayersNum)
			{
				lynxOSLockMutex(&m_Mutex);
				if (targetPlayersNum == 0)
					m_Listener->sendEvent(eventCode, parameters, size, (bool)reliable);
				else
					m_Listener->sendEvent(eventCode, parameters, size, (bool)reliable);
				lynxOSUnlockMutex(&m_Mutex);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::vOnCmd(int player, LYNXBYTE eventCode, LYNXBYTE* parameters)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::vLoop(float step)
			{
				m_Listener->run();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetClient::LoopInBackgroundThread(void* data)
			{
				CNetClient* NetClient = (CNetClient*)data;
				while (1)
				{
					lynxOSLockMutex(&(NetClient->GetMutex()));
					NetClient->vLoop(0);
					lynxOSUnlockMutex(&(NetClient->GetMutex()));
					lynxOSSleep(50);
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vIsConnected()
			{
				if (m_Listener)
					return m_Listener->GetConnected();
				else
					return LYNX_FALSE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vIsJoined()
			{
				if (m_Listener)
					return m_Listener->GetJoined();
				else
					return LYNX_FALSE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CNetClient::vIsMasterPlayer()
			{
				if (m_Listener)
					return m_Listener->GetLoadBalancingClient()->getLocalPlayer().getIsMasterClient();
				else
					return LYNX_FALSE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CNetClient::PEERSTATETYPE CNetClient::vGetPeerState()
			{
				return TrnslatePeerState(GetListener()->GetLoadBalancingClient()->getState());
			}			
		}
	}
}

#endif