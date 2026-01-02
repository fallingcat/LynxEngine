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

#ifndef __LYNXINPUTSYSTEM_H__
#define __LYNXINPUTSYSTEM_H__

#include <LynxSystem.h>
#include <LynxFactory.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CInputSystem : public CSystem
		{
			LYNX_DECLARE_CLASS(CInputSystem);
		public:			
			int								m_CurrentPlaybackFrame;
		protected:
			CFactory<CInputDevice, INPUTDEVICETYPE, CInputSystem*> m_InputDeviceFactory;
			CList<CInputDevice*>			m_InputDeviceList;
			CUserInputTranslator*			m_lpCurrentUserInputtranslator;	
			LYNXBOOL						m_bRecordingInput;
			LYNXBOOL						m_bPlayingBack;			
			LYNXBOOL						m_bBlockAllInput;
		public:			
			CInputSystem(CEngine* const lpengine);
			~CInputSystem();				

			DECLARE_CLASSSCRIPT
			virtual LYNXBOOL				vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);

			LYNXFORCEINLINE LYNXBOOL		IsRecording() {return m_bRecordingInput; }
			LYNXFORCEINLINE LYNXBOOL		IsPlayingBack() {return m_bPlayingBack; }
			LYNXFORCEINLINE void			BlockAllInput(LYNXBOOL b) { m_bBlockAllInput = b; }
			LYNXFORCEINLINE LYNXBOOL		IsAllInputBlocked() { return m_bBlockAllInput; }
			
			virtual LYNXBOOL				vCreate();
			virtual LYNXBOOL				vInit();
			virtual void					vLoop(float step);
			void							RegisterDevice(INPUTDEVICETYPE t, CInputDevice* (*func)(CInputSystem*));
			CInputDevice*					CreateDevice(INPUTDEVICETYPE t);
			void							AddDevice(CInputDevice* dev);
			CInputDevice*					FindDevice(const CString& name);
			CInputDevice*					FindDevice(const LYNXCHAR* name);
			void							Poll();
			void							SetRecordingInput(LYNXBOOL b, const CString& filename);
			void							SetPlayingBack(LYNXBOOL b, const CString& filename);
		};	
	}
}

#endif