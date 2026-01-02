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

#include <LynxEngine_PCH.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <GameFramework/LynxSimpleUIPage.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <InputSystem/LynxInputSystem.h>

namespace LynxEngine 
{
	namespace GameFramework 
	{	
		IMPLEMENT_CLASSSCRIPT(CSimpleUIPage, CUIPage)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSimpleUIPage::CSimpleUIPage(CKernel* const lpk) 
		: CUIPage(lpk)
		{
			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSimpleUIPage::~CSimpleUIPage(void)
		{			
		}										
	}	
}