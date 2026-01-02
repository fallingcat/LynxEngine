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
#include <InputSystem/LynxInputRecord.h>

namespace LynxEngine 
{	
	namespace InputSystem 
	{
		IMPLEMENT_CLASSSCRIPT(CInputRecord, CInputDevice)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CInputRecord::CInputRecord(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CInputRecord::CInputRecord(CInputSystem* lpsys)
		: CInputDevice(lpsys)
		{
			m_InputDeviceType = INPUT_RECORD;
			SetName(_T("System Input Record"));			

			m_InputMap.clear();

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CInputRecord::~CInputRecord(void)
		{
			m_InputMap.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CInputRecord::vCreate()
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::StartRecord()
		{
			m_InputMap.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::EndRecord(const CString& filename)
		{
			CPlatformFileStream File;
			int Size, Frame;
			MOUSEINPUTDATA Data;

			if (File.vOpen(filename, CStream::Binary|CStream::Write))
			{
				Size = m_InputMap.size();
				File.vWrite(&Size, sizeof(int), 1);

				for (CMap<int, MOUSEINPUTDATA>::CIterator Input=m_InputMap.begin(); Input != m_InputMap.end(); Input++)
				{
					Frame = Input->first;
					Data = Input->second;
					File.vWrite(&Frame, sizeof(int), 1);
					File.vWrite(&(Data.ButtonStatus), sizeof(int), 1);
					File.vWrite(&(Data.CursorPos.x), sizeof(float), 1);
					File.vWrite(&(Data.CursorPos.y), sizeof(float), 1);
				}
				File.vClose();
				m_InputMap.clear();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::Record()
		{
			int Frame = glpKernel->GetFrameCount();

			InputSystem::CMouse* lpMouse = (InputSystem::CMouse*)(glpKernel->GetlpEngine()->GetlpSystemMouse());
			if (lpMouse)
			{
				lpMouse->vPoll();					

				//if (lpMouse->GetButtonStatus() != 0)
				{
					m_InputMap[Frame].ButtonStatus = lpMouse->GetButtonStatus();
					m_InputMap[Frame].CursorPos = lpMouse->GetPos();
				}
				
			}						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::StartPlayback(const CString& filename)
		{
			CPlatformFileStream File;
			int Size;
			int Frame;
			MOUSEINPUTDATA Data;

			m_InputMap.clear();
			if (File.vOpen(filename, CStream::Binary|CStream::Read))
			{
				File.vRead(&Size, sizeof(int), 1);
				for (int i = 0; i< Size; i++)
				{
					File.vRead(&Frame, sizeof(int), 1);
					File.vRead(&(Data.ButtonStatus), sizeof(int), 1);
					File.vRead(&(Data.CursorPos.x), sizeof(float), 1);
					File.vRead(&(Data.CursorPos.y), sizeof(float), 1);

					Data.CursorPos.x *= m_lpEngine->GetlpGUISystem()->GetScaleFactor().x;
					Data.CursorPos.y *= m_lpEngine->GetlpGUISystem()->GetScaleFactor().y;

					m_InputMap[Frame] = Data;

					if (i == 0)
					{
						m_FrameOffset = Frame - glpKernel->GetFrameCount();
					}
				}
				File.vClose();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::Playback()
		{
			int Frame = glpKernel->GetFrameCount() + m_FrameOffset;

			InputSystem::CMouse* lpMouse = (InputSystem::CMouse*)(glpKernel->GetlpEngine()->GetlpSystemMouse());
			if (lpMouse)
			{
				lpMouse->vPoll();					

				if (m_InputMap.is_exist(Frame))
				{
					lpMouse->SetPollData(m_InputMap[Frame].CursorPos, m_InputMap[Frame].ButtonStatus);	

					glpKernel->GetlpEngine()->GetlpInputSystem()->m_CurrentPlaybackFrame = Frame;
				}
				else
				{
					glpKernel->GetlpEngine()->GetlpInputSystem()->m_CurrentPlaybackFrame = -1;
				}
			}

			InputSystem::CTouch* lpTouch = (InputSystem::CTouch*)(glpKernel->GetlpEngine()->GetlpSystemTouch());
			if (lpTouch)
			{
				InputSystem::CTouch::TOUCHDATA TouchData;
				if (m_InputMap.is_exist(Frame))
				{
					if (m_InputMap[Frame].ButtonStatus & InputSystem::CMouse::LBUTTON)
					{
						lpTouch->ClearTouches();
						lpTouch->SetTapGesture();		
						lpTouch->SetGestureValue(CVector2(m_InputMap[Frame].CursorPos.x, m_InputMap[Frame].CursorPos.y));
						TouchData.TimeStamp = 0;
						TouchData.TapCount = 1;
						TouchData.Pos = CVector2(m_InputMap[Frame].CursorPos.x, m_InputMap[Frame].CursorPos.y);						
						lpTouch->AddTouch(TouchData);

						glpKernel->GetlpEngine()->GetlpInputSystem()->m_CurrentPlaybackFrame = Frame;
					}
					else
					{
						lpTouch->ClearTouches();
						lpTouch->SetTouchGesture(InputSystem::CTouch::TG_NULL);

						glpKernel->GetlpEngine()->GetlpInputSystem()->m_CurrentPlaybackFrame = -1;						
					}
				}		
				else
				{
					lpTouch->ClearTouches();
					lpTouch->SetTouchGesture(InputSystem::CTouch::TG_NULL);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::vPoll()
		{
			//Playback();
		}
		/*
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::SetPollData(LYNXPOINT2D& pos, int mask)
		{
			m_ButtonStatus = mask;
			m_CursorPos = pos;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::vPoll(void)
		{
			#ifdef __WEB__
			#else
				lynxGetClientCursorPos(&m_CursorPos, m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow());								

				m_ButtonStatus = 0;
				
				if (lynxGetKeyState(m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow(), VK_LBUTTON))
					m_ButtonStatus |= LBUTTON;
				if (lynxGetKeyState(m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow(), VK_MBUTTON))
					m_ButtonStatus |= MBUTTON;
				if (lynxGetKeyState(m_lpInputSystem->GetlpEngine()->GetlpKernel()->GetWindow(), VK_RBUTTON))
					m_ButtonStatus |= RBUTTON;
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CInputRecord::Poll(void)
		{
			vPoll();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CInputRecord::ClearPollData(LYNXBOOL bclearqueued) 
		{
			m_ButtonStatus = 0;
			if (bclearqueued)
			{
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_LBUTTON] = LYNX_FALSE;
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_MBUTTON] = LYNX_FALSE;
				m_lpInputSystem->GetlpEngine()->GetlpKernel()->m_KBD[VK_RBUTTON] = LYNX_FALSE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CInputRecord::ButtonStatus(int mask)
		{
			return (m_ButtonStatus & mask) ? LYNX_TRUE : LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CInputRecord::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			REGISTER_SCRIPT_VAR(lps, "m_CursorPos",		LynxEngine::InputSystem::CInputRecord, m_CursorPos)
			REGISTER_SCRIPT_VAR(lps, "m_ButtonStatus",	LynxEngine::InputSystem::CInputRecord, m_ButtonStatus)

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CInputRecord::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "Poll",			LynxEngine::InputSystem::CInputRecord, void,		Poll,			(void))
			REGISTER_SCRIPT_METHOD(lps, "ButtonStatus", LynxEngine::InputSystem::CInputRecord, LYNXBOOL,	ButtonStatus,	(int))

			return LYNX_TRUE;
		}
		*/
	}
}
