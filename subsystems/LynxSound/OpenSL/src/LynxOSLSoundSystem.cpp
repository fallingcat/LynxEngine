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

#include <LynxEngine_PCH.h>
#include <LynxOSLMusic.h>
#include <LynxOSLSound.h>
#include <LynxOSLSoundSystem.h>

#define MAX_PLAYER  24

namespace LynxSound 
{
	namespace OpenSL
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundSystem::CSoundSystem(LynxEngine::CEngine* const lpengine)
		: LynxEngine::SoundSystem::CSoundSystem(lpengine)
		{
			m_EngineObject = NULL;  
			m_EngineEngine = NULL; 
			m_OutputMixObject = NULL;
			m_PlayStateMap = new LYNXBOOL[MAX_PLAYER];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundSystem::~CSoundSystem(void)
		{
			(*m_OutputMixObject)->Destroy(m_OutputMixObject);
			(*m_EngineObject)->Destroy(m_EngineObject);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vCreate()
		{			
			LynxEngine::SoundSystem::CSoundSystem::vCreate();
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vInit()
		{
			SLresult result;  
  
			// create engine  
			result = slCreateEngine(&m_EngineObject, 0, NULL, 0, NULL, NULL);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return LYNX_FALSE;
			}
		#endif
			(void) result;  
  
			// realize the engine  
			result = (*m_EngineObject)->Realize(m_EngineObject, SL_BOOLEAN_FALSE);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return LYNX_FALSE;
			}
		#endif
			(void) result;  
  
			// get the engine interface, which is needed in order to create other objects  
			result = (*m_EngineObject)->GetInterface(m_EngineObject, SL_IID_ENGINE, &m_EngineEngine);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return LYNX_FALSE;
			}
		#endif
			(void) result;  
  
			// create output mix,  
			result = (*m_EngineEngine)->CreateOutputMix(m_EngineEngine, &m_OutputMixObject, 0, 0, 0);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return LYNX_FALSE;
			}
		#endif
			(void) result;  
  
			// realize the output mix  
			result = (*m_OutputMixObject)->Realize(m_OutputMixObject, SL_BOOLEAN_FALSE);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return LYNX_FALSE;
			}
		#endif
			(void) result;

			for (int i=0; i<MAX_PLAYER; i++)
			{
				m_PlayStateMap[i] = LYNX_FALSE;

				SLObjectItf PlayerObject = NULL;

				m_PlayerObjects.push_back(PlayerObject);
			}


			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::vPause()
		{
			if(!m_bPaused)
			{
				m_bPaused = LYNX_TRUE;

				SLuint32 State;
				SLPlayItf PlayerPlay;

				for (int i=0; i<MAX_PLAYER; i++)
				{
					if (m_PlayerObjects[i])
					{
						SLresult result;
						result = (*m_PlayerObjects[i])->GetInterface(m_PlayerObjects[i], SL_IID_PLAY, &PlayerPlay);  
					#ifdef _DEBUG
						LYNX_ASSERT(SL_RESULT_SUCCESS == result);
					#else
						if (SL_RESULT_SUCCESS != result)
						{
							return;
						}
					#endif
						(void) result;
						result = (*PlayerPlay)->GetPlayState(PlayerPlay, &State);
					#ifdef _DEBUG
						LYNX_ASSERT(SL_RESULT_SUCCESS == result);
					#else
						if (SL_RESULT_SUCCESS != result)
						{
							return;
						}
					#endif

						if (State == SL_PLAYSTATE_PLAYING)
						{
							m_PlayStateMap[i] = LYNX_TRUE;
							result = (*PlayerPlay)->SetPlayState(PlayerPlay, SL_PLAYSTATE_PAUSED);
						#ifdef _DEBUG
							LYNX_ASSERT(SL_RESULT_SUCCESS == result);
						#else
							if (SL_RESULT_SUCCESS != result)
							{
								return;
							}
						#endif
						}
					}
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::vResume()
		{	
			if (m_bPaused)
			{
				m_bPaused = LYNX_FALSE;

				SLuint32 State;
				SLPlayItf PlayerPlay;

				for (int i=0; i<MAX_PLAYER; i++)
				{
					if (m_PlayerObjects[i] && m_PlayStateMap[i] == LYNX_TRUE)
					{
						SLresult result;
						result = (*m_PlayerObjects[i])->GetInterface(m_PlayerObjects[i], SL_IID_PLAY, &PlayerPlay);  
					#ifdef _DEBUG
						LYNX_ASSERT(SL_RESULT_SUCCESS == result);
					#else
						if (SL_RESULT_SUCCESS != result)
						{
							return;
						}
					#endif
						(void) result;
						result = (*PlayerPlay)->SetPlayState(PlayerPlay, SL_PLAYSTATE_PLAYING);
					#ifdef _DEBUG
						LYNX_ASSERT(SL_RESULT_SUCCESS == result);
					#else
						if (SL_RESULT_SUCCESS != result)
						{
							return;
						}
					#endif

						m_PlayStateMap[i] = LYNX_FALSE;
					}
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::SoundSystem::CMusic* CSoundSystem::vCreateMusic()
		{
			return LYNXNEW CMusic(m_lpEngine);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vCreateMusic(LynxEngine::SoundSystem::CMusicPtr& s)
		{
			m_lpEngine->CreateResource(s, LynxEngine::MUSIC);
			m_MusicList.push_back(s);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LynxEngine::SoundSystem::CSound* CSoundSystem::vCreateSound()
		{
			return LYNXNEW CSound(m_lpEngine);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vCreateSound(LynxEngine::SoundSystem::CSoundPtr& s)
		{
			m_lpEngine->CreateResource(s, LynxEngine::SOUND);
			m_SoundList.push_back(s);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteMusic(const LynxEngine::CString& name)
		{
			if (m_bStreamingMusic)
			{
				lynxOSLockMutex(&m_StreamingMutex);
			}

			LynxEngine::SoundSystem::CMusicPtr Music = FindMusic(name);
			Music->vStop();
			((LynxSound::OpenSL::CMusic*)(Music.GetPtr()))->m_bDelete = LYNX_TRUE;

			if (m_bStreamingMusic)
			{
				lynxOSUnlockMutex(&m_StreamingMutex);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteMusic(const LynxEngine::SoundSystem::CMusic* music)
		{
			if (m_bStreamingMusic)
			{
				lynxOSLockMutex(&m_StreamingMutex);
			}
			
			if 	(music)
			{
				LynxEngine::SoundSystem::CMusicPtr Music = FindMusic(music);
				if (Music)
				{
					Music->vStop();
					((LynxSound::OpenSL::CMusic*)music)->m_bDelete = LYNX_TRUE;
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
		void CSoundSystem::DeleteMusic(const LynxEngine::SoundSystem::CMusicPtr& music)
		{
			if (m_bStreamingMusic)
			{
				lynxOSLockMutex(&m_StreamingMutex);
			}

			LynxEngine::SoundSystem::CMusicPtr Music = music;
			((LynxSound::OpenSL::CMusic*)(Music.GetPtr()))->m_bDelete = LYNX_TRUE;

			if (m_bStreamingMusic)
			{
				lynxOSUnlockMutex(&m_StreamingMutex);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteSound(const LynxEngine::CString& name)
		{
			LynxEngine::SoundSystem::CSoundPtr Sound = FindSound(name);
			if (Sound)
			{
				Sound->vStop();
				((LynxSound::OpenSL::CSound*)(Sound.GetPtr()))->m_bDelete = LYNX_TRUE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteSound(const LynxEngine::SoundSystem::CSoundPtr& sound)
		{
			LynxEngine::SoundSystem::CSoundPtr Sound = sound;
			((LynxSound::OpenSL::CSound*)(Sound.GetPtr()))->m_bDelete = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::DeleteSound(const LynxEngine::SoundSystem::CSound* sound)
		{	
			if 	(sound)
			{
				LynxEngine::SoundSystem::CSoundPtr Sound = FindSound(sound);
				if (Sound)
				{
					Sound->vStop();
					((LynxSound::OpenSL::CSound*)sound)->m_bDelete = LYNX_TRUE;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		SLObjectItf CSoundSystem::GetFreeAudioPlayer(int frequency, int channels)
		{
			int i;

			for (i=0; i<MAX_PLAYER; i++)
			{
				if (!m_PlayerObjects[i] || !*m_PlayerObjects[i])
					break;

				SLObjectItf PlayerObject = m_PlayerObjects[i];
				SLPlayItf PlayerPlay;
				SLresult result;
				result = (*PlayerObject)->GetInterface(PlayerObject, SL_IID_PLAY, &PlayerPlay);  
				#ifdef _DEBUG
					LYNX_ASSERT(SL_RESULT_SUCCESS == result);  
				#else
					if (SL_RESULT_SUCCESS != result)
					{
						return NULL;
					}				
				#endif
				(void) result; 

				SLuint32 State;
				result = (*PlayerPlay)->GetPlayState(PlayerPlay, &State);
				#ifdef _DEBUG
					LYNX_ASSERT(SL_RESULT_SUCCESS == result);
				#else
					if (SL_RESULT_SUCCESS != result)
					{
						return NULL;
					}
				#endif

				if (State != SL_PLAYSTATE_PLAYING)
				{
					(*PlayerObject)->Destroy(PlayerObject);
					m_PlayerObjects[i] = NULL;
					break;
				}
			}

			if (i == MAX_PLAYER)
				return NULL;

			SLObjectItf PlayerObject;
			SLresult result;  
			// configure audio source  
			SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1 };  
			SLDataFormat_PCM format_pcm = { SL_DATAFORMAT_PCM,
											(SLuint32)channels,
											(SLuint32)frequency,  
											SL_PCMSAMPLEFORMAT_FIXED_16,
											SL_PCMSAMPLEFORMAT_FIXED_16,
											channels==1? SL_SPEAKER_FRONT_CENTER : SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
											SL_BYTEORDER_LITTLEENDIAN };  
			SLDataSource audioSrc = { &loc_bufq, &format_pcm };  
  
			// configure audio sink  
			SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, m_OutputMixObject };  
			SLDataSink audioSnk = { &loc_outmix, NULL };  
  
			// create audio player  
			const SLInterfaceID ids[] = { SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_PLAY };  
			const SLboolean req[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };  
			result = (*m_EngineEngine)->CreateAudioPlayer(m_EngineEngine, &PlayerObject, &audioSrc, &audioSnk, 3, ids, req);  
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return NULL;
				}
			#endif
			(void) result;  
  
			// realize the player  
			result = (*PlayerObject)->Realize(PlayerObject, SL_BOOLEAN_FALSE);  
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return NULL;
				}
			#endif
			(void) result;

			m_PlayerObjects[i] = PlayerObject;

			return PlayerObject;
		}
	}
}