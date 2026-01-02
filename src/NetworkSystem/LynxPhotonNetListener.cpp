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

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		namespace Photon
		{
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CNetListener::CNetListener(CNetClient* netClient, ExitGames::Common::JString& appId, ExitGames::Common::JString& appVersion, ExitGames::Common::JString& username
										, const ExitGames::LoadBalancing::AuthenticationValues& authenticationValues)
			{
				m_NetClient = netClient;
				m_LoadBalancingClient = new ExitGames::LoadBalancing::Client(*this, appId, appVersion, username, ExitGames::Photon::ConnectionProtocol::UDP, authenticationValues);
				m_IsHost = false;
				m_bConnected = LYNX_FALSE;
				m_bJoined = LYNX_FALSE;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetListener::connect()
			{
				m_LoadBalancingClient->connect(ExitGames::Common::JString(L"app-asia.exitgamescloud.com"), ExitGames::LoadBalancing::ServerType::MASTER_SERVER);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetListener::disconnect()
			{
				m_LoadBalancingClient->disconnect();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetListener::run()
			{
				m_LoadBalancingClient->service();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetListener::opCreateRoom(ExitGames::Common::JString& name, bool isVisible, bool isOpen, nByte maxPlayers)
			{
				m_LoadBalancingClient->opCreateRoom(name, isVisible, isOpen, maxPlayers);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetListener::opJoinRoom(ExitGames::Common::JString& name)
			{
				m_LoadBalancingClient->opJoinRoom(name, true);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CNetListener::sendEvent(nByte eventCode, nByte* parameters, int size, bool reliable, int *targetPlayers, int targetPlayersNum)
			{
				m_LoadBalancingClient->opRaiseEvent(reliable, parameters, size, eventCode, 0, ExitGames::Lite::EventCache::DO_NOT_CACHE, targetPlayers, targetPlayersNum);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CString CNetListener::GetStringFromJString(ExitGames::Common::JString& jstr)
			{
				LYNXCHAR* str = new LYNXCHAR[wcslen(jstr.cstr())+1];
				lynxFromUniStr(str, jstr.cstr());
				CString cstr = str;
				delete [] str;
				return cstr;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明: Callbacks  
			//-------------------------------------------------------------------------------------------------------
			void CNetListener::debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string){}

			void CNetListener::connectionErrorReturn(int errorCode){}
			void CNetListener::clientErrorReturn(int errorCode){}
			void CNetListener::warningReturn(int warningCode){}
			void CNetListener::serverErrorReturn(int errorCode){}

			void CNetListener::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
			{
				lynxDebugOutputPrintf("player %d has joined room\n", playerNr);
				m_NetClient->vOnPlayerJoinGame(playerNr);
			}
			void CNetListener::leaveRoomEventAction(int playerNr, bool isInactive)
			{
				lynxDebugOutputPrintf("player %d has left room\n", playerNr);
				m_NetClient->vOnPlayerLeaveGame(playerNr);
			}
			void CNetListener::disconnectEventAction(int playerNr){lynxDebugOutputPrintf("player %d has disconnected\n", playerNr);}
			void CNetListener::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
			{
				lynxDebugOutputPrintf("commad received with eventCode = %d\n", eventCode);
				m_NetClient->vOnCmd(playerNr, eventCode, ExitGames::Common::ValueObject<nByte*>(eventContent).getDataCopy());
			}
    
			void CNetListener::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats){}
			void CNetListener::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats){}
			void CNetListener::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers){}

			void CNetListener::connectReturn(int errorCode, const ExitGames::Common::JString& errorString)
			{
				if(errorCode)
				{
					LYNXCHAR* str = new LYNXCHAR[wcslen(errorString.cstr())+1];
					lynxFromUniStr(str, errorString.cstr());
					lynxDebugOutputPrintf("error: %s\n", str);
					delete [] str;
					return;
				}
				lynxDebugOutputPrintf("connected\n");
				m_bConnected = LYNX_TRUE;
				m_NetClient->vOnConnect();
				if(m_IsHost) {
					//opCreateRoom(ExitGames::Common::JString("test_room"));
					NETGAMEDESC gameDesc;
					gameDesc.Default();
					m_NetClient->vCreateGame(CString("test_room"), gameDesc);
				}
				else{
					ExitGames::Common::JString name = ExitGames::Common::JString("test_room");
					opJoinRoom(name);
				}
			}
			void CNetListener::disconnectReturn(void){
				m_bConnected = LYNX_FALSE;
				lynxDebugOutputPrintf("disconnected\n");
			}
			void CNetListener::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
			{
				if(errorCode)
				{
					LYNXCHAR* str = new LYNXCHAR[wcslen(errorString.cstr())+1];
					lynxFromUniStr(str, errorString.cstr());
					lynxDebugOutputPrintf("error: %s\n", str);
					delete [] str;
					return;
				}
				LYNXCHAR* str = new LYNXCHAR[wcslen(m_LoadBalancingClient->getCurrentlyJoinedRoom().getName().cstr())+1];
				lynxFromUniStr(str, m_LoadBalancingClient->getCurrentlyJoinedRoom().getName().cstr());
				lynxDebugOutputPrintf("room %s has been created\n", str);
				delete [] str;
				m_bJoined = LYNX_TRUE;
				m_NetClient->vOnCreateGame();
			}
			void CNetListener::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
			{
				if(errorCode)
				{
					LYNXCHAR* str = new LYNXCHAR[wcslen(errorString.cstr())+1];
					lynxFromUniStr(str, errorString.cstr());
					lynxDebugOutputPrintf("error: %s\n", str);
					delete [] str;
					return;
				}
				LYNXCHAR* str = new LYNXCHAR[wcslen(m_LoadBalancingClient->getCurrentlyJoinedRoom().getName().cstr())+1];
				lynxFromUniStr(str, m_LoadBalancingClient->getCurrentlyJoinedRoom().getName().cstr());
				lynxDebugOutputPrintf("joined room %s\n", str);
				delete [] str;
				m_bJoined = LYNX_TRUE;
				m_NetClient->vOnJoinGame(localPlayerNr);
			}
			void CNetListener::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString){m_bJoined = LYNX_TRUE;}
			void CNetListener::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
			{
				m_bJoined = LYNX_FALSE;
				lynxDebugOutputPrintf("left room\n");
			}
			void CNetListener::joinLobbyReturn(void){lynxDebugOutputPrintf("joined lobby\n");}
			void CNetListener::leaveLobbyReturn(void){lynxDebugOutputPrintf("left lobby\n");}
		}
	}
}

#endif