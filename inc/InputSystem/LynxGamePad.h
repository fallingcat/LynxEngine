#ifndef __LYNXGAMEPAD_H__
#define __LYNXGAMEPAD_H__

#include <InputSystem/LynxInputDevice.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CGamePad : public CInputDevice
		{
			LYNX_DECLARE_CLASS(CGamePad);
		public:			
			static const LYNXWORD DPAD_UP		= 0x0001;
			static const LYNXWORD DPAD_DOWN = 0x0002;
			static const LYNXWORD DPAD_LEFT = 0x0004;
			static const LYNXWORD DPAD_RIGHT = 0x0008;
			static const LYNXWORD START = 0x0010;
			static const LYNXWORD BACK = 0x0020;
			static const LYNXWORD LEFT_THUMB = 0x0040;
			static const LYNXWORD RIGHT_THUMB = 0x0080;
			static const LYNXWORD LEFT_SHOULDER = 0x0100;
			static const LYNXWORD RIGHT_SHOULDER = 0x0200;
			static const LYNXWORD BUTTON_A = 0x1000;
			static const LYNXWORD BUTTON_B = 0x2000;
			static const LYNXWORD BUTTON_X = 0x4000;
			static const LYNXWORD BUTTON_Y = 0x8000;
			
			Math::CVector2						m_LeftSticker;
			Math::CVector2						m_RightSticker;
			LYNXSHORT							m_LeftTrigger;
			LYNXSHORT							m_RightTrigger;		
			LYNXWORD							m_ButtonState;
			Math::CVector2						m_VibrationSpeed;
			float								m_DeadZone;
		protected:
			int									m_Index;
			LYNXBOOL							m_bConnected;
			LYNXLONGLONG						m_VibrationT0;
			float								m_VibrationTime;
		public:						
			CGamePad(CInputSystem* lpsys) :CInputDevice(lpsys) 
			{
				m_bConnected = LYNX_FALSE;
				m_LeftSticker = Math::CVector2(0.0f);
				m_RightSticker = Math::CVector2(0.0f);
				m_LeftTrigger = 0;
				m_RightTrigger = 0;
				m_Index = 0;
				m_VibrationSpeed = Math::CVector2(0.0f);
				m_DeadZone = 0.24f;

				m_VibrationT0 = 0;
				m_VibrationTime = 0;
			}
			~CGamePad() {};			
            
            //DECLARE_CLASSSCRIPT
			//LYNXBOOL						vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			//LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
		
			LYNXFORCEINLINE LYNXBOOL		IsConnected() { return m_bConnected; }
			LYNXFORCEINLINE int				GetControllerIndex() { return m_Index; }
			LYNXFORCEINLINE void			SetControllerIndex(const int i) { m_Index = i; }
			LYNXFORCEINLINE LYNXBOOL		GetButtonState(const int mask) { return (m_ButtonState & mask) ? LYNX_TRUE : LYNX_FALSE; }

			virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
			virtual void					vPoll() {};
			virtual void					vSetVibrationSpeed(const CVector2& v, const float& time = 0, LYNXBOOL boverwrite = LYNX_TRUE) {};
		};
	}
}

#endif