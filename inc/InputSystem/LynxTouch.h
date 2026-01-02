#ifndef __LYNXTOUCH_H__
#define __LYNXTOUCH_H__

#include <InputSystem/LynxInputDevice.h>

namespace LynxEngine 
{
	namespace InputSystem 
	{
		class LYNXENGCLASS CTouch : public CInputDevice
		{
		LYNX_DECLARE_CLASS(CTouch);
			
			typedef enum {
				TG_NULL = -1,
				TG_SCALE,
				TG_ROTATE,
				TG_PAN,
				TG_TAP,
				TG_TAP2,
				TG_TAP3,
				TG_LONGPRESS,
                TG_SWIPE,
				NUM_TOUCHGESTURE,
			}TOUCHGESTURE;
			
			static const int				TGF_NULL		= 0x0;
			static const int				TGF_SCALE		= 0x1;
			static const int				TGF_ROTATE		= 0x2;
			static const int				TGF_PAN			= 0x4;
			static const int				TGF_TAP			= 0x8;
			static const int				TGF_TAP2		= 0x10;
			static const int				TGF_TAP3		= 0x20;
			static const int				TGF_LONGPRESS	= 0x40;
			static const int				TGF_SWIPE		= 0x80;
			static const int				TGF_ALL			= (TGF_SCALE | TGF_ROTATE | TGF_PAN | TGF_TAP | TGF_TAP2 | TGF_TAP3 | TGF_LONGPRESS | TGF_SWIPE);

			typedef struct {
				unsigned long				TimeStamp;
				int							TapCount;
				Math::CVector2				Pos;
			}TOUCHDATA;
			
		protected:			
			TOUCHGESTURE					m_Gesture;
			Math::CVector2					m_Value;
			Math::CVector2					m_Value2;
			CArray<TOUCHDATA>				m_TouchArray;	
			TOUCHDATA						m_TouchData;
		public:						
			CTouch(CInputSystem* lpsys);
			~CTouch();			
            
            DECLARE_CLASSSCRIPT
			LYNXBOOL						vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			void							PollTouchData(int i); 
		
			LYNXFORCEINLINE TOUCHGESTURE	GetTouchGesture() {return m_Gesture; };
			LYNXFORCEINLINE void			SetTouchGesture(TOUCHGESTURE tg) {m_Gesture = tg; };
			LYNXFORCEINLINE Math::CVector2	GetGestureValue() {return m_Value; };
            LYNXFORCEINLINE Math::CVector2	GetGestureValue2() {return m_Value2; };
			LYNXFORCEINLINE void			SetGestureValue(const Math::CVector2& v) {m_Value = v; };
			LYNXFORCEINLINE void			SetGestureValue2(const Math::CVector2& v) {m_Value2 = v; };
			
			LYNXFORCEINLINE void			ClearTouchGesture() {SetTouchGesture(TG_NULL); };
			LYNXFORCEINLINE void			SetScaleGesture(Math::CReal& s, Math::CReal& v) {m_Gesture = TG_SCALE; m_Value.x = s; m_Value.y = v; };	
			LYNXFORCEINLINE void			SetRotateGesture(Math::CReal& r, Math::CReal& v) {m_Gesture = TG_ROTATE; m_Value.x = r; m_Value.y = v; };	
			LYNXFORCEINLINE void			SetLongPressGesture() {m_Gesture = TG_LONGPRESS; };
			LYNXFORCEINLINE void			SetPanGesture(Math::CVector2& trans, Math::CVector2& vel) {m_Gesture = TG_PAN; m_Value = trans; m_Value2 = vel; };
            LYNXFORCEINLINE void			SetTapGesture() {m_Gesture = TG_TAP; };
			LYNXFORCEINLINE void			SetTap2Gesture() {m_Gesture = TG_TAP2; };
			LYNXFORCEINLINE void			SetTap3Gesture() {m_Gesture = TG_TAP3; };
			LYNXFORCEINLINE void			SetSwipeGesture(Math::CVector2& dir) {m_Gesture = TG_SWIPE; m_Value = dir; };
			
			LYNXFORCEINLINE int				GetNumTouches() {return m_TouchArray.size(); };
			LYNXFORCEINLINE void			ClearTouches() {m_TouchArray.clear(); };
			LYNXFORCEINLINE void			AddTouch(const TOUCHDATA& data) {m_TouchArray.push_back(data); };
			LYNXFORCEINLINE const TOUCHDATA& GetTouch(int i) const {return m_TouchArray[i]; };
			
			virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
			virtual void					vPoll();
		};
	}
}

#endif