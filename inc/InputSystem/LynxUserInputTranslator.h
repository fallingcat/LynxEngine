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

#ifndef __LYNXUSERINPUTTRANSLATOR_H__
#define __LYNXUSERINPUTTRANSLATOR_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CUserInputTranslator : public CObj
		{
			LYNX_DECLARE_CLASS(CUserInputTranslator);
		public:			
		protected:		
			CInputDevice*					m_lpDevice;
			CMap<int, CEvent>				m_EventMap;
		public:	
			CUserInputTranslator(CInputDevice* lpd);			
			~CUserInputTranslator();		

			void							RegisterEvent(int k, CEvent& e);
			virtual CEvent					vTranslate() = 0;	
		};	
	}
}

#endif