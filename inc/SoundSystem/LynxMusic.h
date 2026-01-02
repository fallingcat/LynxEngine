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

#ifndef __LYNXMUSIC_H__
#define __LYNXMUSIC_H__

#include <LynxEngineClasses.h>
#include <LynxResource.h>

namespace LynxEngine 
{
	namespace SoundSystem 
	{
		class LYNXENGCLASS CMusic : public CResource
		{
			LYNX_DECLARE_CLASS(CMusic);
		private:		
		protected:
			float							m_Volume;
			LYNXBOOL						m_bStreaming;
			LYNXBOOL						m_bLoop;
			int								m_LoopTimes;
			LYNXBOOL						m_ReadyForStreming;
		public:			
			CMusic(CEngine* lpeng);
			~CMusic();			

			//DECLARE_CLASSSCRIPT
			//LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);

			LYNXFORCEINLINE LYNXBOOL		Play() { return vPlay();};
			LYNXFORCEINLINE void			Stop() { return vStop();};
			LYNXFORCEINLINE void			SetLoops(int n) { vSetLoops(n);};
			LYNXFORCEINLINE void			SetVolume(float v) { vSetVolume(v);};
			LYNXFORCEINLINE float			GetVolume() {return m_Volume; }
			LYNXFORCEINLINE void			Pause() { return vPause();};
			LYNXFORCEINLINE LYNXBOOL		IsPlaying() {return vIsPlaying(); };
			LYNXFORCEINLINE LYNXBOOL		IsStreaming() {return m_bStreaming; };
			LYNXFORCEINLINE LYNXBOOL		IsReadyForStreaming() {return m_ReadyForStreming; };			

			LYNXFORCEINLINE bool			operator ==(const CString& name) const {return (m_Name == name); };		
			
			virtual LYNXBOOL				vIsPlaying() = 0;
			virtual LYNXBOOL				vCreate(const CCreationDesc* desc) = 0;	
			virtual LYNXBOOL				vPlay(void) = 0;	
			virtual void					vPause(void) = 0;	
			virtual void					vStop(void) = 0;
			virtual float					vGetVolume() = 0;
			virtual void					vSetVolume(float v) = 0;
			virtual void					vSetLoops(int n) = 0;		
			virtual LYNXBOOL				vUpdateOGG() = 0;
		};	
	}
	LYNXFORCEINLINE bool operator == (const SoundSystem::CMusic* s, const CString& name) {return ((*s) == name); };	
}

#endif