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
#ifndef __LYNXNULLNETCLIENT_H__
#define __LYNXNULLNETCLIENT_H__
	
#include <LynxEngineClasses.h>
#include <NetworkSystem/LynxNetClient.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		class LYNXENGCLASS CNullNetClient : public CNetClient
		{
			LYNX_DECLARE_CLASS(CNullNetClient);
		public:						
		protected:			
		public:
			CNullNetClient(CEngine* const lpengine) : LynxEngine::NetworkSystem::CNetClient(lpengine) {};
			~CNullNetClient() {};

			virtual LYNXBOOL								vCreate(void) { return LYNX_TRUE; };
			virtual LYNXBOOL								vInit() { return LYNX_TRUE; };
			virtual void									vLoop(float step) {};
			virtual LYNXBOOL								vConnect() { return LYNX_TRUE; };
			virtual LYNXBOOL								vDisconnect() { return LYNX_TRUE; };
			virtual LYNXBOOL								vCreateGame(const CString& name, const NETGAMEDESC& gameDesc = NETGAMEDESC()) { return LYNX_TRUE; };
			virtual LYNXBOOL								vJoinGame(const CString& name) { return LYNX_TRUE; };
			virtual void									vOnConnect() {};
			virtual void									vOnCreateGame() {};
			virtual void									vOnJoinGame(int localPlayerNr) {};
			virtual void									vOnPlayerJoinGame(int playerNr) {};
			virtual void									vOnPlayerLeaveGame(int playerNr) {};
			virtual void									vSendCmd(LYNXBYTE eventCode, LYNXBYTE* parameters, int size, LYNXBOOL reliable, int *targetPlayers = NULL, int targetPlayersNum = 0) {};
			virtual void									vOnCmd(int player, LYNXBYTE eventCode, LYNXBYTE* parameters) {};
			
			virtual LYNXBOOL								vIsConnected() { return LYNX_TRUE; };
			virtual LYNXBOOL								vIsJoined() { return LYNX_TRUE; };
			virtual LYNXBOOL								vIsMasterPlayer() { return LYNX_TRUE; };
			virtual CNetClient::PEERSTATETYPE				vGetPeerState() { return NetworkSystem::CNetClient::PST_LEFT; };			
		};		
	}
}
#endif