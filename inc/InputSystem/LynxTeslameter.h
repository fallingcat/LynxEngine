#ifndef __LYNXTESLAMETER_H__
#define __LYNXTESLAMETER_H__

#include <InputSystem/LynxInputDevice.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CTeslameter : public CInputDevice
		{
			LYNX_DECLARE_CLASS(CTeslameter);			
		protected:				
            Math::CVector3                  m_Value;
		public:						
			CTeslameter(CInputSystem* lpsys);
			~CTeslameter();			
            
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