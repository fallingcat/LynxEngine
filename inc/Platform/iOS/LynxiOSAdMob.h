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

#ifndef __LYNXIOSADMOB_H__
#define __LYNXIOSADMOB_H__

#include <LynxEngineClasses.h>
#include <AdSystem/LynxAd.h>

LYNX_FORWARD_DECLARATION(GADBannerView)

namespace LynxEngine 
{
	namespace AdSystem 
	{		
		namespace iOS
		{
			class LYNXENGCLASS CAdMob : public CAd
			{
				LYNX_DECLARE_CLASS(CAdMob);
			private:			
			protected:		
				GADBannerView*					m_ADView;
			public:			
				CAdMob(CAdSystem* lpsys);
				~CAdMob();			

				LYNXBOOL						vCreate(const CCreationDesc& desc);			
				LYNXBOOL						vInit();				
				void							vLoop(float step);
				void							vRender();
				LYNXBOOL						vDestroy();
			};	
		}
	}
}	

#endif