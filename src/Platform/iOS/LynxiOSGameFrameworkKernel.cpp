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
#include <GameFramework/LynxUIPage.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <InputSystem/LynxInputSystem.h>
#include <Platform/iOS/LynxiOSGameFrameworkKernel.h>
#include <Platform/iOS/LynxiOSEngine.h>

namespace LynxEngine 
{
	namespace GameFramework 
	{	
		namespace iOS
		{
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CKernel::CKernel(void)
			{
				m_bGestureRecognization = LYNX_TRUE;
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CKernel::~CKernel(void)
			{						
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CEngine* CKernel::vCreateEngine() 
			{
				CEngine* lpEngine = LYNXNEW LynxEngine::iOS::CEngine(this);
				
				return  lpEngine; 
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CKernel::vInit(INIT_DESC* desc)
			{
				m_lpEngine = vCreateEngine();
				m_lpEngine->vCreate(desc);

				if (desc)
				{
					if (!((CEngine*)m_lpEngine)->vInit(m_Window, &(desc->GraphicParam)))
						return LYNX_FALSE;
				}
				else 
				{
					if (!((CEngine*)m_lpEngine)->vInit(m_Window))
						return LYNX_FALSE;
				}			
				SetRunning(LYNX_TRUE);
				
				return LYNX_TRUE;
			}			
		}
	}	
}