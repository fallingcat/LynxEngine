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
#include <LynxFPSCameraController.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CFPSCameraController::CFPSCameraController(CEngine* lpe)
	: CGameObj(lpe)
	{
		m_KeyPressed = -1;
		m_Speed = 1.0f;				
		m_Sensitivity = 1.0f;
		m_CameraContainer = LYNXNEW CCameraContainer(lpe);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CFPSCameraController::~CFPSCameraController(void)
	{
		LYNXDEL m_CameraContainer;
	}		
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CFPSCameraController::vBehave(LYNXREAL skip)
	{
		switch (m_KeyPressed)
		{
			case VK_UP:
				m_CameraContainer->Forward(m_Speed);		
				m_CameraContainer->UpdateViewMatrix();	
				break;

			case VK_DOWN:
				m_CameraContainer->Forward(-m_Speed);		
				m_CameraContainer->UpdateViewMatrix();	
				break;

			case VK_LEFT:
				m_CameraContainer->SideMove(-m_Speed);		
				m_CameraContainer->UpdateViewMatrix();	
				break;

			case VK_RIGHT:
				m_CameraContainer->SideMove(m_Speed);		
				m_CameraContainer->UpdateViewMatrix();	
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CFPSCameraController::vOnSysKeyDown(int key)
	{
		m_KeyPressed = key;				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CFPSCameraController::vOnSysKeyUp(int key)
	{
		m_KeyPressed = -1;		
	}
}