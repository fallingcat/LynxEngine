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

#ifndef __LYNXNETPLAYER_H__
#define __LYNXNETPLAYER_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		class LYNXENGCLASS CNetPlayer : public CObj
		{
			LYNX_DECLARE_CLASS(CNetGame);
		public:	
			typedef enum {
				NPT_LOCAL = 0,
				NPT_REMOTE,
				NUM_NETPLAYERTYPES,
			}NETPLAYERTYPE;			
		protected:			
			NETPLAYERTYPE							m_NetPlayerType;
			CString									m_UDID;						
		public:			
			CNetPlayer(CEngine* const lpengine);			
			~CNetPlayer();			

			LYNXFORCEINLINE NETPLAYERTYPE			GetNetPlayerType() {return m_NetPlayerType; }
			LYNXFORCEINLINE void					SetNetPlayerType(NETPLAYERTYPE type) {m_NetPlayerType = type; }

			LYNXBOOL								vCreate(void);
			LYNXBOOL								vInit();	

			virtual LYNXBOOL						vCreateGame();
			virtual LYNXBOOL						vJoinGame(const CString& name);

			virtual void							vSendCmd();
			virtual void							vOnCmd();
		};	
	}
}

#endif