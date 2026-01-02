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

#ifndef __LYNXTOUCH_H__
#define __LYNXTOUCH_H__

#include <InputSystem/LynxInputDevice.h>

class NSSet;

namespace LynxEngine 
{
	namespace InputSystem 
	{
		namespace iOS
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
				
				typedef struct {
					LYNXLONG					TimeStamp;
					int							TapCount;
					Math::CVector2				Pos;
				}TOUCHDATA;
				
			protected:
				LYNXPOINT2D						m_CursorPos;	
				TOUCHGESTURE					m_Gesture;
				Math::CVector2					m_Value;
				NSSet*							m_Touches;
				CArray<TOUCHDATA>				m_TouchArray;	
			public:						
				CTouch(CInputSystem* lpsys);
				~CTouch();			
			
				LYNXFORCEINLINE TOUCHGESTURE	GetTouchGesture() {return m_Gesture; };
				LYNXFORCEINLINE void			SetTouchGesture(TOUCHGESTURE tg) {m_Gesture = tg; };
				LYNXFORCEINLINE Math::CVector2	GetGestureValue() {return m_Value; };
				
				LYNXFORCEINLINE void			ClearTouchGesture() {SetTouchGesture(TG_NULL); };
				LYNXFORCEINLINE void			SetScaleGesture(Math::CReal& s, Math::CReal& v) {m_Gesture = TG_SCALE; m_Value.x = s; m_Value.y = v; };	
				LYNXFORCEINLINE void			SetRotateGesture(Math::CReal& r, Math::CReal& v) {m_Gesture = TG_ROTATE; m_Value.x = r; m_Value.y = v; };	
				LYNXFORCEINLINE void			SetLongPressGesture() {m_Gesture = TG_LONGPRESS; };
				LYNXFORCEINLINE void			SetTapGesture() {m_Gesture = TG_TAP; };
				LYNXFORCEINLINE void			SetTap2Gesture() {m_Gesture = TG_TAP2; };
				LYNXFORCEINLINE void			SetTap3Gesture() {m_Gesture = TG_TAP3; };
				LYNXFORCEINLINE void			SetSwipeGesture(Math::CVector2& dir) {m_Gesture = TG_SWIPE; m_Value = dir; };
				
				LYNXFORCEINLINE int				GetNumTouches() {return m_TouchArray.size(); };
				LYNXFORCEINLINE void			ClearTouches() {m_TouchArray.clear(); };
				LYNXFORCEINLINE void			AddTouch(const TOUCHDATA& data) {m_TouchArray.push_back(data); };
				LYNXFORCEINLINE const TOUCHDATA& GetTouch(int i) const {return m_TouchArray[i]; };
				
				LYNXFORCEINLINE void			SetTouches(NSSet* t) {m_Touches = t; };
				virtual LYNXBOOL				vCreate() {return LYNX_TRUE; };
				virtual void					vPoll();
			};
		}
	}
}

#endif