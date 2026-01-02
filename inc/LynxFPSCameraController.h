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

#ifndef __LYNXFPSCAMERACONTROLLER_H__
#define __LYNXFPSCAMERACONTROLLER_H__

#include <LynxGameObj.h>
#include <Container/LynxCameraContainer.h>

namespace LynxEngine 
{
	class CEngine;
	class LYNXENGCLASS CFPSCameraController : public CGameObj
	{		
		LYNX_DECLARE_CLASS(CFPSCameraController);
	public:
		float								m_Speed;
		float								m_Sensitivity;
	protected:
		CCameraContainer*					m_CameraContainer;				
		int									m_KeyPressed;		
	public:		
		CFPSCameraController(CEngine* lpe);
		virtual ~CFPSCameraController();	

		virtual void						vBehave(LYNXREAL skip);
		// Event handling functions
		virtual void						vOnSysKeyDown(int key);
		virtual void						vOnSysKeyUp(int key);

	};		
}

#endif