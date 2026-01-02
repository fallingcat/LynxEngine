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

#ifndef __LYNXNETWORKSYSTEM_H__
#define __LYNXNETWORKSYSTEM_H__

#include <LynxSystem.h>

namespace LynxEngine 
{
	namespace NetworkSystem 
	{
		class LYNXENGCLASS CNetworkSystem : public CSystem
		{
			LYNX_DECLARE_CLASS(CNetworkSystem);
		private:			
		protected:
			CFactory<CNetClient, CAnsiString>		m_NetClientFactory;

			LYNXBOOL								m_bOffline;
			CString									m_HttpErrorKey;
			CNetGame*								m_Game;
			CNetClient*								m_NetClient;
		public:			
			CNetworkSystem(CEngine* const lpengine);			
			~CNetworkSystem();			

			LYNXBOOL								vCreate(void);
			LYNXBOOL								vInit();		

			void									RegisterNetClient(const CAnsiString& t, CNetClient* (*func)(CEngine*));
			LYNXBOOL								CreateNetClient(CAnsiString& name);
			LYNXFORCEINLINE CNetClient*				GetNetClient() { return m_NetClient; }

			LYNXFORCEINLINE LYNXBOOL				IsOffline() {return m_bOffline; }		
			void									SetOffline(LYNXBOOL b);

			LYNXFORCEINLINE void					SetHttpErrorKey(const CString& s) {m_HttpErrorKey = s; }		
			LYNXFORCEINLINE const CString&			GetHttpErrorKey() const {return m_HttpErrorKey; }			
				
		};	
	}
}

#endif