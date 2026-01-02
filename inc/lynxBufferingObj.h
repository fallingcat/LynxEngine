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

#ifndef __LYNXBUFFERINGOBJ_H__
#define __LYNXBUFFERINGOBJ_H__

namespace LynxEngine 
{
	// Used for the data which will be accessed by multiple threads.
	// One thread can read/write the data and the other thread can read it at the same time.
	// i.e. the game main thread read/write the data and the rendering thread only read the data for rendering. 
	template <class T> class CBufferingObj
	{				
	protected:
		T						m_Origin;
		T						m_Copy;

	public:
		CBufferingObj() {};
		~CBufferingObj() {};

		operator T&()			{return m_Origin; };
		operator T()			{return m_Copy; };
		//operator const T()		{return m_Copy; };

		const T&				Read()	{return m_Copy; };
		void					Sync() {m_Copy = m_Origin; };						
	};					
}
#endif