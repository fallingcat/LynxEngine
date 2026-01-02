//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.com
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Shayne Wei : 2016/03/28
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXOSLBUFFER_H__
#define __LYNXOSLBUFFER_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxSound.h>
#include <LynxUnknown.h>
#if defined __ANDROID__
	#include <SLES/OpenSLES.h>
	#include <SLES/OpenSLES_Android.h>
#else
#endif

typedef int (__cdecl *PFNALGETENUMVALUE)( const char *szEnumName );

namespace LynxSound 
{
	namespace OpenSL 
	{
		class LYNXENGCLASS CBuffer : public LynxEngine::CUnknown
		{
			LYNX_DECLARE_CLASS(CBuffer);
		protected:			
			char*						m_ID;
		public:			
			CBuffer();
			~CBuffer();

			LYNXFORCEINLINE	char*				GetID() const {return m_ID; };
			LYNXBOOL							Create(char* buffer);	
		};		
	}
}

#endif