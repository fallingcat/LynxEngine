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

#ifndef __LYNXNETCLIENT_H__
#define __LYNXNETCLIENT_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		class LYNXENGCLASS CNetClient : public CObj
		{
			LYNX_DECLARE_CLASS(CNetClient);
		public:
			typedef enum
			{
				PST_UNINITIALIZED,
				PST_PEERCREATED,
				PST_CONNECTINGTONAMESERVER,
				PST_CONNECTEDTONAMESERVER,
				PST_DISCONNECTINGFROMNAMESERVER,
				PST_CONNECTING,
				PST_CONNECTED,
				PST_WAITINGFORCUSTOMAUTHENTICATIONNEXTSTEPCALL,
				PST_AUTHENTICATED,
				PST_JOINEDLOBBY,
				PST_DISCONNECTINGFROMMASTERSERVER,
				PST_CONNECTINGTOGAMESERVER,
				PST_CONNECTEDTOGAMESERVER,
				PST_AUTHENTICATEDONGAMESERVER,
				PST_JOINING,
				PST_JOINED,
				PST_LEAVING,
				PST_LEFT,
				PST_DISCONNECTINGFROMGAMESERVER,
				PST_CONNECTINGTOMASTERSERVER,
				PST_CONNECTEDCOMINGFROMGAMESERVER,
				PST_AUTHENTICATEDCOMINGFROMGAMESERVER,
				PST_DISCONNECTING,
				PST_DISCONNECTED,
				NUM_PEERSTATETYPE,
			}PEERSTATETYPE;

		protected:			
		public:			
			CNetClient(CEngine* const lpengine);			
			~CNetClient();			

			virtual LYNXBOOL						vCreate(void) = 0;
			virtual LYNXBOOL						vInit() = 0;
			virtual void							vLoop(float step) = 0;
			virtual LYNXBOOL						vConnect() = 0;
			virtual LYNXBOOL						vDisconnect() = 0;
			virtual LYNXBOOL						vCreateGame(const CString& name, const NETGAMEDESC& gameDesc = NETGAMEDESC()) = 0;
			virtual LYNXBOOL						vJoinGame(const CString& name) = 0;	
			virtual void							vOnConnect() = 0;
			virtual void							vOnCreateGame() = 0;
			virtual void							vOnJoinGame(int localPlayerNr) = 0;
			virtual void							vOnPlayerJoinGame(int playerNr) = 0;
			virtual void							vOnPlayerLeaveGame(int playerNr) = 0;
			virtual void							vSendCmd(LYNXBYTE eventCode, LYNXBYTE* parameters, int size, LYNXBOOL reliable, int *targetPlayers = NULL, int targetPlayersNum = 0) = 0;		
			virtual void							vOnCmd(int player, LYNXBYTE eventCode, LYNXBYTE* parameters) = 0;
			virtual LYNXBOOL						vIsConnected() = 0;
			virtual LYNXBOOL						vIsJoined() = 0;
			virtual LYNXBOOL						vIsMasterPlayer() = 0;
			virtual PEERSTATETYPE					vGetPeerState() = 0;
		};	
	}
}

#endif