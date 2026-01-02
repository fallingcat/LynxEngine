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
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXOALBUFFER_H__
#define __LYNXOALBUFFER_H__

#include <LynxEngineClasses.h>
#include <SoundSystem/LynxSound.h>
#include <LynxUnknown.h>
#ifdef __iOS__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#elif defined __ANDROID__
	#include <AL/al.h>
	#include <AL/alc.h>
#else
	#include <al.h>
	#include <alc.h>
#endif

typedef int (__cdecl *PFNALGETENUMVALUE)( const char *szEnumName );

namespace LynxSound 
{
	namespace OpenAL 
	{
		class LYNXENGCLASS CBuffer : public LynxEngine::CUnknown
		{
			LYNX_DECLARE_CLASS(CBuffer);
		protected:			
			ALuint						m_ID;			
		public:			
			CBuffer();
			~CBuffer();

			LYNXFORCEINLINE	ALuint		GetID() const {return m_ID; };
			LYNXBOOL					Create();	
		};		
	}
}

#endif