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

#ifndef __LYNXWIDGET_H__
#define __LYNXWIDGET_H__

#include <LynxList.h>
#include <LynxObj.h>

namespace LynxEngine 
{		
	namespace GUISystem 
	{		
		class CGUISystem;
		class LYNXENGCLASS CWidget : public CObj
		{	
			LYNX_DECLARE_CLASS(CGUISystem);
		public:									
		protected:					
			CList<CWidget*>					m_WidgetList;			
		public:			
			CWidget();	
			CWidget(CGUISystem* const lpgui);				
			virtual ~CWidget();							
		};		
	}
}
#endif