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

#ifndef __LYNXAD_H__
#define __LYNXAD_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace AdSystem 
	{		
		class LYNXENGCLASS CAd : public CObj
		{
			LYNX_DECLARE_CLASS(CAd);
		private:			
		protected:		
			CAdSystem*						m_lpAdSystem;
			ADTYPE							m_AdType;
			LYNXRECT						m_Rect;
		public:			
			CAd(CAdSystem* lpsys);
			~CAd();			

			virtual LYNXBOOL				vCreate(const CCreationDesc& desc);			
			LYNXBOOL						vInit();				
			void							vLoop(float step);
			void							vRender();
			LYNXBOOL						vDestroy();

			LYNXFORCEINLINE LYNXRECT		GetRect() const {return m_Rect; };		
		};	
	}
}	

#endif