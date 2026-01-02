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
//  Created by Owen Wu : 2006/11/16
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXEVENT_H__
#define __LYNXEVENT_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CEvent : public CObj
	{	
		LYNX_DECLARE_CLASS(CEvent);
	public:		
		typedef enum{
			NONE = -1,
			SYSTEM_EVENT = 0,
			SYS_KEY_DOWN = 0,			
			SYS_KEY_UP,
			SYS_LBUTTON_DOWN,
			SYS_LBUTTON_UP,
			SYS_RBUTTON_DOWN,
			SYS_RBUTTON_UP,
			KERNEL_EVENT = 100,
			ANIMATIONSTOP,
			USER_INPUT,
			ENTER,
			USE,
			DROP,
			PICK,
			DESTROY,
			RESPAWN,
			DIE,
			GAME_OVER,
			MISSION_COMPLETE,
			MISSION_FAILD,
			NUM_TYPE,		
		}TYPE;
		int							m_Param;		
	protected:	
		TYPE						m_Type;
	public:			
		CEvent() {m_Type = NONE; };			
		CEvent(TYPE t) {m_Type = t; };	
		CEvent(TYPE t, const CString& name) {m_Type = t; SetName(name); };	
		virtual ~CEvent() {};	

		LYNXINLINE void				SetType(TYPE t) {m_Type = t; };
		LYNXINLINE TYPE				GetType() const {return m_Type; };
	};
}
#endif

