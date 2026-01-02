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

#ifndef __LYNXSOUNDSYSTEM_H__
#define __LYNXSOUNDSYSTEM_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <LynxSystem.h>
#include <vorbis/vorbisfile.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CMusicCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CMusicCreationDesc);	
	public:							
		int							m_NumLoops;
		float						m_Volume;
		LYNXBOOL					m_bStreaming;

		CMusicCreationDesc() {Default(); };
		void Default() {m_NumLoops = 1; m_Volume = 1.0f; m_bStreaming = LYNX_FALSE; };
	};

	class LYNXENGCLASS CSoundCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CSoundCreationDesc);	
	public:							
		LYNXBOOL					m_b3DSound;
		LYNXBOOL					m_bRecording;
		int							m_NumLoops;
		float						m_Volume;		

		CSoundCreationDesc() {Default(); };
		void Default() {m_b3DSound = LYNX_FALSE; m_bRecording = LYNX_FALSE; m_NumLoops = 0; m_Volume = 1.0f; };
	};		

	namespace SoundSystem 
	{
		typedef struct{
			CString						Filename;			
			int							Loop;
			float						Volume;
			LYNXBOOL					bShared;
		}SOUNDDESC;

		typedef struct{
			CString						Filename;			
			int							Loop;
			float						Volume;
			LYNXBOOL					bStreaming;
		}MUSICDESC;

		typedef enum {
			SP_LOW,
			SP_MEDIUM,
			SP_HIGH,
		}SOUNDPRIORITY;

		typedef struct{
			CSound*						Sound;
			SOUNDPRIORITY				Priority;
			LYNXBOOL					bPostpone;
		}PLAYINGSOUNDDATA;

		class LYNXENGCLASS CSoundSystem : public CSystem
		{
			LYNX_DECLARE_CLASS(CSoundSystem);
		public:		
			CList<CMusicPtr>				m_MusicList;
			CList<CSoundPtr>				m_SoundList;
			CList<PLAYINGSOUNDDATA>			m_SoundGoingToPlayList;
			CList<PLAYINGSOUNDDATA>			m_SoundPlayingList;
		private:			
			static CMusicPtr				m_NULLMusic;
			static CSoundPtr				m_NULLSound;
		protected:
			LYNXBOOL						m_bPaused;
			
			CListener*						m_lpListener;

			LYNXBOOL						m_bSound;
			LYNXBOOL						m_bMusic;

			int								m_MaxPlayingSoundsSimultaneously;

			float							m_MusicVolumeMultiplier;
			float							m_SoundVolumeMultiplier;

			LYNXBOOL						m_bStreamingStopped;
			LYNXBOOL						m_bStreamingMusic;
			LYNXTHREADHANDLE				m_StreamingThread;
			LYNXMUTEX						m_StreamingMutex;
		public:			
			CSoundSystem(CEngine* const lpengine);
			~CSoundSystem();			

			DECLARE_CLASSSCRIPT
			LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
			CSound*							_LoadSound(const LYNXCHAR* name, const LYNXCHAR* filename);			
			LYNXFORCEINLINE	void			_DeleteSound(CSound* sound) {DeleteSound(sound); }

			virtual const LYNXCHAR*			vGetAPIString() = 0;
			LYNXBOOL						vCreate(void);			
			LYNXBOOL						vInit();
			virtual void					vLoop(float step);
			virtual void					vPause() = 0;
			virtual void					vResume() = 0;
			virtual void					vLoadParam(const LYNXCHAR* filename);

			void							AddToPlayingList(CSound* sound, SOUNDPRIORITY priority);

			void							StartStreamingThread();

			LYNXFORCEINLINE void			SetMaxPlayingSoundsSimultaneously(int s) {m_MaxPlayingSoundsSimultaneously = s;	}

			LYNXBOOL						LoadMusic(CMusicPtr& sound, const LYNXCHAR* name, const LYNXCHAR* filename, LYNXBOOL bstreaming = LYNX_FALSE);									
			virtual CMusic*					vCreateMusic() = 0;
			virtual LYNXBOOL				vCreateMusic(CMusicPtr& s) = 0;						
			virtual void					DeleteMusic(const CMusic* music);
			virtual void					DeleteMusic(const CMusicPtr& music);
			virtual void					DeleteMusic(const CString& name);
			CMusicPtr&						FindMusic(const LYNXCHAR* name);
			CMusicPtr&						FindMusic(const CString& name);
			CMusicPtr&						FindMusic(const CMusic* sound);
			void                            StopStreamingMusic();

			LYNXBOOL						LoadSound(CSoundPtr& sound, const LYNXCHAR* name, const LYNXCHAR* filename, LYNXBOOL bshared = LYNX_TRUE);
			virtual CSound*					vCreateSound() = 0;
			virtual LYNXBOOL				vCreateSound(CSoundPtr& s) = 0;
			virtual void					DeleteSound(const CSound* sound);
			virtual void					DeleteSound(const CSoundPtr& sound);
			virtual void					DeleteSound(const CString& name);
			CSoundPtr&						FindSound(const LYNXCHAR* name);
			CSoundPtr&						FindSound(const CString& name);
			CSoundPtr&						FindSound(const CSound* sound);
            void                            StopAllPlayingSound();

			LYNXFORCEINLINE float			GetMusicVolumeMultiplier() {return m_MusicVolumeMultiplier; };
			LYNXFORCEINLINE void			SetMusicVolumeMultiplier(float v) {m_MusicVolumeMultiplier = v; };
			void							EnableMusic(LYNXBOOL b);
			LYNXFORCEINLINE LYNXBOOL		IsMusicEnabled() {return m_bMusic; };

			LYNXFORCEINLINE float			GetSoundVolumeMultiplier() {return m_SoundVolumeMultiplier; };
			LYNXFORCEINLINE void			SetSoundVolumeMultiplier(float v) {m_SoundVolumeMultiplier = v; };
			void							EnableSound(LYNXBOOL b);
			LYNXFORCEINLINE LYNXBOOL		IsSoundEnabled() {return m_bSound; };			

			LYNXFORCEINLINE LYNXBOOL		IsPaused() {return m_bPaused; };	

			static int						OVOpen(CStream* f, OggVorbis_File *vf, const char *initial, long ibytes);
		private:
			static void						StreamingThread(void* data);
		};	

		LYNXFORCEINLINE bool operator == (const PLAYINGSOUNDDATA& lhs, const CSound* rhs) { return (lhs.Sound == rhs); }
		LYNXFORCEINLINE bool operator == (const PLAYINGSOUNDDATA& lhs, const PLAYINGSOUNDDATA& rhs) { return (lhs.Sound == rhs.Sound); }
		
		template <LYNXBOOL bDecending>
		struct SortBySoundPriority
		{
			bool operator()(const PLAYINGSOUNDDATA& lhs, const PLAYINGSOUNDDATA& rhs)
			{
				if (bDecending)
				{
					return (lhs.Priority > rhs.Priority);
				}
				else
				{
					return (lhs.Priority < rhs.Priority);
				}
			}
		};
	}	
}

#endif