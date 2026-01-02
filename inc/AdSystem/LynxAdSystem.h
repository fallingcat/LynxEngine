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

#ifndef __LYNXADSYSTEM_H__
#define __LYNXADSYSTEM_H__

#include <LynxEngineClasses.h>
#include <LynxSystem.h>

namespace LynxEngine 
{
	namespace AdSystem 
	{
		class LYNXENGCLASS CAdCreationDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CAdCreationDesc);	
		public:					
			ADTYPE							m_AdType;
            LYNXPOINT2D						m_Position;
			LYNXPOINT2D						m_Size;

			CAdCreationDesc() {Default(); };
			void Default() {CCreationDesc::Default(); m_AdType = AT_ADMOB; m_Position.x = 0; m_Position.y = 0; m_Size.x = 320; m_Size.y = 50; };			
		};		

		class LYNXENGCLASS CAdSystem : public CSystem
		{
			LYNX_DECLARE_CLASS(CAdSystem);
		private:			
		protected:					
			CList<CAd*>						m_AdList;
		public:			
			CAdSystem(CEngine* const lpengine);
			~CAdSystem();			

			LYNXBOOL						vCreate(void);			
			LYNXBOOL						vInit();	

			CAd*							CreateAd(const CCreationDesc& desc);			
			void							ReleaseAd(CAd* ad);
			void							ReleaseAllAd();
		};	
	}
}	

#endif