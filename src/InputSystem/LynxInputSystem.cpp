//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>

#include <InputSystem/LynxInputSystem.h>
#include <InputSystem/LynxInputDevice.h>
#include <InputSystem/LynxKeyboard.h>
#include <InputSystem/LynxMouse.h>
#include <InputSystem/LynxInputRecord.h>

#ifdef __WIN32__
	#include <Platform/Win32/LynxWin32Touch.h>
	#include <Platform/Win32/LynxWin32GamePad.h>
#endif

#ifdef __iOS__
	#include <Platform/iOS/LynxiOSTouch.h>
	#include <Platform/iOS/LynxiOSAccelerometer.h>
    #include <Platform/iOS/LynxiOSGyrometer.h>
	#include <Platform/iOS/LynxiOSMotionInput.h>
#endif

namespace LynxEngine 
{	
	namespace InputSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
	#ifdef __WIN32__
		CInputDevice* CreateKeyboardDevice(CInputSystem* i)			{ return LYNXNEW CKeyboard(i); }
		CInputDevice* CreateMouseDevice(CInputSystem* i)			{ return LYNXNEW CMouse(i); }	
		CInputDevice* CreateTouchDevice(CInputSystem* i)			{ return LYNXNEW Win32::CTouch(i); }	
		CInputDevice* CreateAccelerometerDevice(CInputSystem* i)	{ return LYNXNEW CAccelerometer(i); }	
		CInputDevice* CreateGyrometerDevice(CInputSystem* i)		{ return LYNXNEW CGyrometer(i); }	
		CInputDevice* CreateGamePad(CInputSystem* i)				{ return LYNXNEW Win32::CGamePad(i); }		
    #elif defined __iOS__
		CInputDevice* CreateKeyboardDevice(CInputSystem* i)			{ return LYNXNEW CKeyboard(i); }
		CInputDevice* CreateMouseDevice(CInputSystem* i)			{ return LYNXNEW CMouse(i); }		
		CInputDevice* CreateTouchDevice(CInputSystem* i)			{ return LYNXNEW iOS::CTouch(i); }	
		CInputDevice* CreateAccelerometerDevice(CInputSystem* i)	{ return LYNXNEW iOS::CAccelerometer(i); }	
		CInputDevice* CreateMotionInputDevice(CInputSystem* i)		{ return LYNXNEW iOS::CMotionInput(i); }	
        CInputDevice* CreateGyrometerDevice(CInputSystem* i)		{ return LYNXNEW iOS::CGyrometer(i); }
	#elif defined __ANDROID__
		CInputDevice* CreateKeyboardDevice(CInputSystem* i)			{ return LYNXNEW CKeyboard(i); }
		CInputDevice* CreateMouseDevice(CInputSystem* i)			{ return LYNXNEW CMouse(i); }	
		CInputDevice* CreateTouchDevice(CInputSystem* i)			{ return LYNXNEW CTouch(i); }	
		CInputDevice* CreateAccelerometerDevice(CInputSystem* i)	{ return LYNXNEW CAccelerometer(i); }	
		CInputDevice* CreateGyrometerDevice(CInputSystem* i)		{ return LYNXNEW CGyrometer(i); }	
	#endif
		CInputDevice* CreateInputRecordDevice(CInputSystem* i)		{ return LYNXNEW CInputRecord(i); }

		IMPLEMENT_CLASSSCRIPT(CInputSystem, CSystem)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CInputSystem::CInputSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{				
			m_InputDeviceList.clear();
			m_lpCurrentUserInputtranslator = NULL;
			m_InputDeviceFactory.SetParam(this);

			m_bRecordingInput = LYNX_FALSE;
			m_bPlayingBack = LYNX_FALSE;
			m_bBlockAllInput = LYNX_FALSE;

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CInputSystem::~CInputSystem(void)
		{
			CList<CInputDevice*>::CIterator Device;

			for (Device = m_InputDeviceList.begin(); Device != m_InputDeviceList.end(); ++Device)
			{
				LYNXDEL (*Device);
				(*Device) = NULL;
			}
			m_InputDeviceList.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CInputSystem::vCreate() 
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CInputSystem::vInit() 
		{	
			CSystem::vInit();

			#if defined(__MOBILE__)
				RegisterDevice(TOUCH,			CreateTouchDevice);
				RegisterDevice(ACCELEROMETER,	CreateAccelerometerDevice);
				RegisterDevice(GYROMETER,		CreateGyrometerDevice);
				//RegisterDevice(MOTION_INPUT,	CreateMotionInputDevice);
			#elif defined(__DESKTOP__)
				RegisterDevice(KEYBOARD,		CreateKeyboardDevice);
				RegisterDevice(MOUSE,			CreateMouseDevice);				
				RegisterDevice(TOUCH,			CreateTouchDevice);								
				RegisterDevice(GAMEPAD,			CreateGamePad);
			#endif		

			RegisterDevice(INPUT_RECORD,	CreateInputRecordDevice);				

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputSystem::RegisterDevice(INPUTDEVICETYPE t, CInputDevice* (*func)(CInputSystem*))
		{
			m_InputDeviceFactory.Register(t, func);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CInputDevice* CInputSystem::CreateDevice(INPUTDEVICETYPE t)
		{
			CInputDevice* Dev = m_InputDeviceFactory.CreateObject(t);
			Dev->vCreate();
			AddDevice(Dev);

			return Dev;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputSystem::AddDevice(CInputDevice* dev)
		{
			m_InputDeviceList.push_back(dev);
		}
		//-------------------------------------------------	------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CInputDevice* CInputSystem::FindDevice(const LYNXCHAR* name)
		{
			return FindDevice(CString(name));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CInputDevice* CInputSystem::FindDevice(const CString& name)
		{
			CList<CInputDevice*>::CIterator Device;
			
			Device = LynxEngine::find(m_InputDeviceList.begin(), m_InputDeviceList.end(), name);
			if (Device == m_InputDeviceList.end())
				return NULL;
			return (*Device);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputSystem::vLoop(float step)
		{
			CList<CInputDevice*>::CIterator Device;

			for (Device = m_InputDeviceList.begin(); Device != m_InputDeviceList.end(); ++Device)
			{
				if ((*Device)->IsEnabled())
					(*Device)->vLoop(step);
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputSystem::Poll()
		{
			CList<CInputDevice*>::CIterator Device;

			for (Device = m_InputDeviceList.begin(); Device != m_InputDeviceList.end(); ++Device)
			{
				if ((*Device)->IsEnabled())
					(*Device)->vPoll();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CInputSystem::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "FindDevice", LynxEngine::InputSystem::CInputSystem, CInputDevice*,FindDevice, (const LYNXCHAR *))

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CInputSystem::SetRecordingInput(LYNXBOOL b, const CString& filename)
		{
			if (m_lpEngine->GetlpSystemInputRecord())
			{
				if (b)
				{
					m_lpEngine->GetlpSystemInputRecord()->StartRecord();				
					m_bRecordingInput = b;
					m_bPlayingBack = LYNX_FALSE;
				}
				else
				{
					m_lpEngine->GetlpSystemInputRecord()->EndRecord(filename);
					m_bRecordingInput = b;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CInputSystem::SetPlayingBack(LYNXBOOL b, const CString& filename)
		{
			if (m_lpEngine->GetlpSystemInputRecord())
			{
				if (b)
				{
					m_lpEngine->GetlpSystemInputRecord()->StartPlayback(filename);									
					m_bPlayingBack = b;
					m_bRecordingInput = LYNX_FALSE;
				}				
				else
				{
					m_bPlayingBack = b;					
				}
			}
		}
	}
}