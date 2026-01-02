#ifndef __LYNXACCELEROMETER_H__
#define __LYNXACCELEROMETER_H__

#include <InputSystem/LynxInputDevice.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CAccelerometer : public CInputDevice
		{
			LYNX_DECLARE_CLASS(CAccelerometer);			
		protected:			
			Math::CReal						m_HighPassFactor;	
			Math::CVector3					m_Value;
			Math::CVector3					m_HighPassValue;
		public:						
			CAccelerometer(CInputSystem* lpsys);
			~CAccelerometer();			
            
            DECLARE_CLASSSCRIPT
			LYNXBOOL						vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
		
			virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
			virtual void					vPoll();
			LYNXFORCEINLINE void			SetHighPassFactor(const Math::CReal& f) {m_HighPassFactor = f; };
			void							SetValue(const Math::CVector3& v);
			LYNXFORCEINLINE Math::CVector3	GetValue() {return m_Value; };
			LYNXFORCEINLINE Math::CVector3	GetHighPassValue() {return m_HighPassValue; };
			LYNXFORCEINLINE void			Clear() {m_Value.x = m_Value.y = m_Value.z = 0.0f; m_HighPassValue.x = m_HighPassValue.y = m_HighPassValue.z = 0.0f; };
		};
	}
}

#endif