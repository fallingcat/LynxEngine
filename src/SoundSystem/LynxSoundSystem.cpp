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

#include <LynxEngine_PCH.h>
#include <SoundSystem/LynxSoundSystem.h>
#include <SoundSystem/LynxSound.h>

namespace LynxEngine 
{	
	namespace SoundSystem 
	{
		CMusicPtr CSoundSystem::m_NULLMusic;
		CSoundPtr CSoundSystem::m_NULLSound;

		IMPLEMENT_CLASSSCRIPT(CSoundSystem, CSystem)	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundSystem::CSoundSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{				
			m_bSound = LYNX_TRUE;
			m_bMusic = LYNX_TRUE;
			m_bPaused = LYNX_FALSE;
			m_lpListener = NULL;
			m_MusicList.clear();
			m_SoundList.clear();

			m_MusicVolumeMultiplier = 1.0f;
			m_SoundVolumeMultiplier = 1.0f;

			m_StreamingThread = 0;
			m_bStreamingMusic = LYNX_FALSE;
			m_bStreamingStopped = LYNX_TRUE;

			m_MaxPlayingSoundsSimultaneously = 32;

            #ifdef __WIN32__
                m_StreamingMutex = 0x00;
            #endif
            
			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundSystem::~CSoundSystem(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));

			StopStreamingMusic();

			lynxOSCloseThread(m_StreamingThread);

			//while (!m_bStreamingStopped);

            #ifdef __WIN32__
                if (m_StreamingMutex)
                    lynxOSDestoryMutex(&m_StreamingMutex);
            #endif
            
			if (m_lpListener)
			{
				LYNXDEL m_lpListener;
				m_lpListener = NULL;
			}			
			for (CList<CMusicPtr>::CIterator Music = m_MusicList.begin();  Music != m_MusicList.end(); ++Music)
			{
				(*Music)->vStop();
			}
			m_MusicList.clear();
			
			for (CList<CSoundPtr>::CIterator Sound = m_SoundList.begin();  Sound != m_SoundList.end(); ++Sound)
			{
				(*Sound)->vStop();
			}
			m_SoundList.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vCreate()
		{			
			if (m_bStreamingMusic)
			{
				StartStreamingThread();
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::StartStreamingThread()
		{
			if (!m_StreamingThread)
			{
				m_bStreamingMusic = LYNX_TRUE;
				m_StreamingThread = lynxOSCreateThread(StreamingThread, (void*)(this));
				m_bStreamingStopped = LYNX_FALSE;
				lynxOSCreateMutex(&m_StreamingMutex);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::vLoadParam(const LYNXCHAR* filename)
		{
			CIniReader IniReader(filename);
			
			m_MusicVolumeMultiplier = IniReader.ReadFloat(_T("Sound"), _T("MusicVolumeMultiplier"), 1.0f);
			m_SoundVolumeMultiplier = IniReader.ReadFloat(_T("Sound"), _T("SoundVolumeMultiplier"), 1.0f);
			m_MaxPlayingSoundsSimultaneously = IniReader.ReadInt(_T("Sound"), _T("MaxPlayingSoundsSimultaneously"), 8);
			SetMaxPlayingSoundsSimultaneously(m_MaxPlayingSoundsSimultaneously);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vInit()
		{
			vLoadParam(m_lpEngine->GetConfigFilename());				

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CSoundSystem::EnableMusic(LYNXBOOL b) 
		{
			m_bMusic = b; 			

			for (CList<CMusicPtr>::CIterator Music = m_MusicList.begin();  Music != m_MusicList.end(); ++Music)
			{
				(*Music)->SetVolume((*Music)->vGetVolume());
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteMusic(const CString& name)
		{
			if (m_bStreamingMusic)
			{
				lynxOSLockMutex(&m_StreamingMutex);
			}

			CMusicPtr Music = FindMusic(name);
			Music->vStop();
			m_MusicList.remove(Music);

			if (m_bStreamingMusic)
			{
				lynxOSUnlockMutex(&m_StreamingMutex);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteMusic(const CMusic* music)
		{
			if (m_bStreamingMusic)
			{
				lynxOSLockMutex(&m_StreamingMutex);
			}
			
			if 	(music)
			{
				CMusicPtr Music = FindMusic(music);
				if (Music)
				{
					Music->vStop();
					m_MusicList.remove(Music);
				}
			}
			if (m_bStreamingMusic)
			{
				lynxOSUnlockMutex(&m_StreamingMutex);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteMusic(const CMusicPtr& music)
		{
			if (m_bStreamingMusic)
			{
				lynxOSLockMutex(&m_StreamingMutex);
			}

			m_MusicList.remove(music);

			if (m_bStreamingMusic)
			{
				lynxOSUnlockMutex(&m_StreamingMutex);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMusicPtr& CSoundSystem::FindMusic(const LYNXCHAR* name)
		{			
			return FindMusic(CString(name));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMusicPtr& CSoundSystem::FindMusic(const CString& name)
		{
			CList<CMusicPtr>::CIterator Music = LynxEngine::find(m_MusicList.begin(), m_MusicList.end(), name);
			if (Music == m_MusicList.end())
				return m_NULLMusic;
			
			return (*Music);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMusicPtr& CSoundSystem::FindMusic(const CMusic* sound)
		{
			CList<CMusicPtr>::CIterator Music = LynxEngine::find(m_MusicList.begin(), m_MusicList.end(), sound);
			if (Music == m_MusicList.end())
				return m_NULLMusic;
			
			return (*Music);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::LoadMusic(CMusicPtr& music, const LYNXCHAR* name, const LYNXCHAR* filename, LYNXBOOL bstreaming)
		{
			CMusicCreationDesc MusicDesc;

			MusicDesc.Default();
			MusicDesc.m_FileName = filename;
			MusicDesc.m_NumLoops = 1;
			MusicDesc.m_Volume = 1.0f;
			MusicDesc.m_Name = name;
			MusicDesc.m_bStreaming = bstreaming;
			MusicDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();

			if (!vCreateMusic(music))
			{
				return LYNX_FALSE;
			}

			if (!(music)->vCreate(&MusicDesc))
			{
				DeleteMusic(music);
				music.Reset();
				return LYNX_FALSE;
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::StopStreamingMusic()
		{
			m_bStreamingMusic = LYNX_FALSE; 			
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::StopAllPlayingSound()
        {
            for (CList<CSoundPtr>::CIterator Sound = m_SoundList.begin();  Sound != m_SoundList.end(); ++Sound)
            {
                if ((*Sound)->vIsPlaying())
                    (*Sound)->Stop();
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CSoundSystem::EnableSound(LYNXBOOL b) 
		{
			m_bSound = b; 

			if (!b)
			{
				StopAllPlayingSound();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteSound(const CString& name)
		{
			CSoundPtr Sound = FindSound(name);
			if (Sound)
			{
				Sound->vStop();
				m_SoundList.remove(Sound);
				CList<PLAYINGSOUNDDATA>::CIterator Pos = LynxEngine::find(m_SoundPlayingList.begin(), m_SoundPlayingList.end(), &(*Sound));
				if (Pos != m_SoundPlayingList.end())
					m_SoundPlayingList.erase(Pos);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteSound(const CSoundPtr& sound)
		{
			m_SoundList.remove(sound);
			CList<PLAYINGSOUNDDATA>::CIterator Pos = LynxEngine::find(m_SoundPlayingList.begin(), m_SoundPlayingList.end(), &(*sound));
			if (Pos != m_SoundPlayingList.end())
				m_SoundPlayingList.erase(Pos);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteSound(const CSound* sound)
		{	
			if 	(sound)
			{
				CSoundPtr Sound = FindSound(sound);
				if (Sound)
				{
					Sound->vStop();
					m_SoundList.remove(Sound);
					CList<PLAYINGSOUNDDATA>::CIterator Pos = LynxEngine::find(m_SoundPlayingList.begin(), m_SoundPlayingList.end(), sound);
					if (Pos != m_SoundPlayingList.end())
						m_SoundPlayingList.erase(Pos);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundPtr& CSoundSystem::FindSound(const LYNXCHAR* name)
		{			
			return FindSound(CString(name));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundPtr& CSoundSystem::FindSound(const CString& name)
		{
			CList<CSoundPtr>::CIterator Sound = LynxEngine::find(m_SoundList.begin(), m_SoundList.end(), name);
			if (Sound == m_SoundList.end())
				return m_NULLSound;
			
			return (*Sound);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundPtr& CSoundSystem::FindSound(const CSound* sound)
		{
			CList<CSoundPtr>::CIterator Sound = LynxEngine::find(m_SoundList.begin(), m_SoundList.end(), sound);
			if (Sound == m_SoundList.end())
				return m_NULLSound;
			
			return (*Sound);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::LoadSound(CSoundPtr& sound, const LYNXCHAR* name, const LYNXCHAR* filename, LYNXBOOL bshared)
		{
			CSoundCreationDesc SoundDesc;			

            if (bshared)
            {
                sound = FindSound(name);
                if (sound)
                    return LYNX_TRUE;
            }            
            
			if (!vCreateSound(sound))
				return LYNX_FALSE;

			CSoundPtr& Source = FindSound(name);
			if (Source)
			{
				sound->vInstance(*(dynamic_cast<CResource*>(&(*Source))));				
			}
			else
			{
				SoundDesc.Default();
				SoundDesc.m_FileName = filename;
				SoundDesc.m_NumLoops = 1;
				SoundDesc.m_Volume = 1.0f;
				SoundDesc.m_Name = name;
				SoundDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();			
				if (!(sound)->vCreate(&SoundDesc))
				{
					DeleteSound(sound);
					sound.Reset();
					return LYNX_FALSE;
				}
			}


			/*
			CSoundCreationDesc SoundDesc;

            if (bshared)
            {
                sound = FindSound(name);
                if (sound)
                    return LYNX_TRUE;
            }            
            
			if (!vCreateSound(sound))
				return LYNX_FALSE;

			SoundDesc.Default();
			SoundDesc.m_FileName = filename;
			SoundDesc.m_NumLoops = 1;
			SoundDesc.m_Volume = 1.0f;
			SoundDesc.m_Name = name;
			SoundDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();			

			(sound)->vCreate(&SoundDesc);			
			*/
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSound* CSoundSystem::_LoadSound(const LYNXCHAR* name, const LYNXCHAR* filename)
		{
			CSoundCreationDesc SoundDesc;

			SoundDesc.Default();
			SoundDesc.m_FileName = filename;
			SoundDesc.m_NumLoops = 1;
			SoundDesc.m_Volume = 1.0f;
			SoundDesc.m_Name = name;
			SoundDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();

			CSoundPtr pSound;
			vCreateSound(pSound);
			(pSound)->vCreate(&SoundDesc);			
			return &(*pSound);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CSoundSystem::StreamingThread(void* data)
		{
			CSoundSystem* System = (CSoundSystem*)data;			

			CList<CMusicPtr>::CIterator Music;
			
			while (System->m_bStreamingMusic)
			{
				lynxOSLockMutex(&(System->m_StreamingMutex));
				for (Music = System->m_MusicList.begin(); Music != System->m_MusicList.end(); Music++)
				{
					if ((*Music)->IsStreaming() && (*Music)->IsReadyForStreaming())
						(*Music)->vUpdateOGG();
				}
				lynxOSUnlockMutex(&(System->m_StreamingMutex));
			}
			System->m_bStreamingStopped = LYNX_TRUE; 
			System->m_StreamingThread = 0;
			lynxOSExitThread(0);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CSoundSystem::AddToPlayingList(CSound* sound, SOUNDPRIORITY priority)
		{			
			//sound->vPlay();
			//sound->m_Priority = priority;
			//return;

			PLAYINGSOUNDDATA Data;			
			if (LynxEngine::find(m_SoundGoingToPlayList.begin(), m_SoundGoingToPlayList.end(), sound) == m_SoundGoingToPlayList.end())
			{
				if (LynxEngine::find(m_SoundPlayingList.begin(), m_SoundPlayingList.end(), sound) == m_SoundPlayingList.end())
				{
					sound->m_Priority = priority;
					Data.Sound = sound;
					Data.Priority = priority;
					Data.bPostpone = LYNX_FALSE;
					m_SoundGoingToPlayList.push_back(Data);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CSoundSystem::vLoop(float step)
		{
			CList<PLAYINGSOUNDDATA>::CIterator Sound;
			int NumHighPrioritySounds;
			
			//LynxEngine::sort(m_SoundGoingToPlayList.begin(), m_SoundGoingToPlayList.end(), SortBySoundPriority<LYNX_SORT_DEC>());

			NumHighPrioritySounds = 0;
			// If there are too many sounds are going to play
			if (m_SoundPlayingList.size() >= m_MaxPlayingSoundsSimultaneously)
			{
				// postpone low and medium priority sounds first				
				Sound = m_SoundGoingToPlayList.begin();
				while (Sound != m_SoundGoingToPlayList.end())
				{
					CList<PLAYINGSOUNDDATA>::CIterator Temp = Sound;
					Sound++;
					if ((*Temp).Priority == SP_LOW || (*Temp).Priority == SP_MEDIUM)
					{
						(*Temp).bPostpone = LYNX_TRUE;
					}					
					else if ((*Temp).Priority == SP_HIGH) // Keep high priority sound in an array
					{
						(*Temp).bPostpone = LYNX_FALSE;
						NumHighPrioritySounds++;
					}
				}

				// Stop low and medium priority playing sound to play high priority sounds 
				if (NumHighPrioritySounds > 0)
				{
					int NumStoppedSound = 0;
					CList<PLAYINGSOUNDDATA>::CIterator PSound = m_SoundPlayingList.begin();
					while (PSound != m_SoundPlayingList.end())
					{
						PLAYINGSOUNDDATA CurrentSound = (*PSound);
						PSound++;
						if (CurrentSound.Priority == SP_LOW)
						{
							CurrentSound.Sound->vStop();
							m_SoundPlayingList.remove(CurrentSound);
							NumStoppedSound++;
							if (NumStoppedSound >= NumHighPrioritySounds)
								break;
						}
					}

					if (NumStoppedSound == 0)
					{
						CList<PLAYINGSOUNDDATA>::CIterator PSound = m_SoundPlayingList.begin();
						while (PSound != m_SoundPlayingList.end())
						{
							PLAYINGSOUNDDATA CurrentSound = (*PSound);
							PSound++;
							if (CurrentSound.Priority == SP_MEDIUM)
							{
								CurrentSound.Sound->vStop();
								m_SoundPlayingList.remove(CurrentSound);
								NumStoppedSound++;
								if (NumStoppedSound >= NumHighPrioritySounds)
									break;
							}
						}
					}
				}
			}

			Sound = m_SoundGoingToPlayList.begin();
			while (Sound != m_SoundGoingToPlayList.end())
			{
				CList<PLAYINGSOUNDDATA>::CIterator Temp = Sound;
				Sound++;
				if (!(*Temp).bPostpone)
				{
					(*Temp).Sound->vPlay();
					m_SoundPlayingList.push_back((*Temp));
					m_SoundGoingToPlayList.erase(Temp);					
				}				
			}			

			// remove the sounds have stpped
			CList<PLAYINGSOUNDDATA>::CIterator PSound = m_SoundPlayingList.begin();
			while (PSound != m_SoundPlayingList.end())
			{
				PLAYINGSOUNDDATA CurrentSound = (*PSound);
				PSound++;
				if (!CurrentSound.Sound->IsPlaying())
				{
					m_SoundPlayingList.remove(CurrentSound);
				}	
			}

			/*
			CList<CMusicPtr>::CIterator Music;
			
			for (Music = m_MusicList.begin(); Music != m_MusicList.end(); Music++)
			{
				if ((*Music)->IsStreaming())
					(*Music)->vUpdateOGG();
			}
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSoundSystem::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "LoadSound",		LynxEngine::SoundSystem::CSoundSystem,	void*,	_LoadSound, 	(const LYNXCHAR *, const LYNXCHAR *))
			REGISTER_SCRIPT_METHOD(lps, "DeleteSound",		LynxEngine::SoundSystem::CSoundSystem,	void,	_DeleteSound, 	(void*))
			
			return LYNX_TRUE;
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		size_t OGGFileRead(void *data, size_t size, size_t nmemb, void* file)
		{
            LynxEngine::CStream* lpStream = static_cast<LynxEngine::CStream*>(file);
			return lpStream->vRead(data, size, nmemb);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int OGGFileSeek(void *file, ogg_int64_t offset, int whence)
		{
            LynxEngine::CStream* lpStream = static_cast<LynxEngine::CStream*>(file);
			return lpStream->vSeek(offset, whence);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int OGGFileClose(void *file)
		{
            LynxEngine::CStream* lpStream = static_cast<LynxEngine::CStream*>(file);
			lpStream->vClose();			
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		long OGGFileTell(void *file)
		{
            LynxEngine::CStream* lpStream = static_cast<LynxEngine::CStream*>(file);
			return lpStream->vTell();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CSoundSystem::OVOpen(CStream* f, OggVorbis_File *vf, const char *initial, long ibytes)
		{
			ov_callbacks callbacks = {
				OGGFileRead,
				OGGFileSeek,
				OGGFileClose,
				OGGFileTell,
			};
			return ov_open_callbacks((void *)f, vf, initial, ibytes, callbacks);
		}  
	}
}