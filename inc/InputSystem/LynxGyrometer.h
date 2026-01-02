#ifndef __LYNXGYROMETER_H__
#define __LYNXGYROMETER_H__

#include <InputSystem/LynxInputDevice.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CGyrometer : public CInputDevice
		{
			LYNX_DECLARE_CLASS(CGyrometer);			
		protected:				
            Math::CVector3                  m_Value;
		public:						
			CGyrometer(CInputSystem* lpsys);
			~CGyrometer();			
            
            DECLARE_CLASSSCRIPT
			LYNXBOOL						vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
		
			virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
			virtual void					vPoll();
            LYNXFORCEINLINE Math::CVector3	GetValue() {return m_Value; };
		};
	}
}

#endif