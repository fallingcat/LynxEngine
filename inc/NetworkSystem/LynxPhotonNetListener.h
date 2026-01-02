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

#ifdef __PHOTON__

#ifndef __LYNXPHOTONNETLISTENER_H__
#define __LYNXPHOTONNETLISTENER_H__

	#include <LoadBalancing-cpp/inc/Client.h>
	#include <LynxEngineClasses.h>	

	namespace LynxEngine 
	{
		namespace NetworkSystem 
		{
			namespace Photon
			{
				class CNetListener : private ExitGames::LoadBalancing::Listener
				{
					//LYNX_DECLARE_CLASS(CNetListener);
				protected:
					CNetClient*								m_NetClient;
					ExitGames::LoadBalancing::Client*		m_LoadBalancingClient;
					LYNXBOOL								m_bConnected;
					LYNXBOOL								m_bJoined;
				public:
					CNetListener(CNetClient* netClient, ExitGames::Common::JString& appId, ExitGames::Common::JString& appVersion, ExitGames::Common::JString& username
								, const ExitGames::LoadBalancing::AuthenticationValues& authenticationValues=ExitGames::LoadBalancing::AuthenticationValues());	
					void run(void);
					void connect(void);
					void disconnect(void);
					void opCreateRoom(ExitGames::Common::JString& name, bool isVisible, bool isOpen, nByte maxPlayers);
					void opJoinRoom(ExitGames::Common::JString& name);
					void sendEvent(nByte eventCode, nByte* parameters, int size, bool reliable, int *targetPlayers = NULL, int targetPlayersNum = 0);
					bool m_IsHost;

					CString GetStringFromJString(ExitGames::Common::JString& jstr);
					LYNXFORCEINLINE ExitGames::LoadBalancing::Client* GetLoadBalancingClient(){ return m_LoadBalancingClient; }
					LYNXFORCEINLINE LYNXBOOL GetConnected(){ return m_bConnected; }
					LYNXFORCEINLINE LYNXBOOL GetJoined(){ return m_bJoined; }
				private:
					// receive and print out debug out here
					virtual void debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string);

					// implement your error-handling here
					virtual void connectionErrorReturn(int errorCode);
					virtual void clientErrorReturn(int errorCode);
					virtual void warningReturn(int warningCode);
					virtual void serverErrorReturn(int errorCode);

					// events, triggered by certain operations of all players in the same room
					virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
					virtual void leaveRoomEventAction(int playerNr, bool isInactive);
					virtual void disconnectEventAction(int playerNr);
					virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);
    
					virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats);
					virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats);
					virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers);

					// callbacks for operations on PhotonLoadBalancing server
					virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString);
					virtual void disconnectReturn(void);
					virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
					virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
					virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
					virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
					virtual void joinLobbyReturn(void);
					virtual void leaveLobbyReturn(void);
				};
			}
		}
	}

#endif

#endif