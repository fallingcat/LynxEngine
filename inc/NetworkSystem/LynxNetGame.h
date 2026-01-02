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

#ifndef __LYNXNETGAME_H__
#define __LYNXNETGAME_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		class LYNXENGCLASS CNetGame : public CObj
		{
			LYNX_DECLARE_CLASS(CNetGame);
		private:	
			int										m_MaxPlayers;			
		protected:			
		public:			
			CNetGame(CEngine* const lpengine);			
			~CNetGame();			

			LYNXBOOL								vCreate(void);
			LYNXBOOL								vInit();					
		};	
	}
}

#endif