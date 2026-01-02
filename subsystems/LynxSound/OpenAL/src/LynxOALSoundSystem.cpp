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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxOALMusic.h>
#include <LynxOALSound.h>
#include <LynxOALSoundSystem.h>

namespace LynxSound 
{
	namespace OpenAL
	{
		#ifdef __iOS__
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			OSStatus iOSAudioFileRead(void *inClientData, SInt64 inPosition, UInt32 requestCount, void *buffer, UInt32 *actualCount)
			{
                LynxEngine::CStream* lpStream = static_cast<LynxEngine::CStream*>(inClientData);
				lpStream->vSeek(inPosition, LynxEngine::CStream::SeekSet);
				*actualCount = lpStream->vRead(buffer, 1, requestCount);
                
                return noErr;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			OSStatus iOSAudioFileWrite(void *inClientData, SInt64 inPosition, UInt32 requestCount, const void *buffer, UInt32 *actualCount)
			{
				LynxEngine::CStream* lpStream = static_cast<LynxEngine::CStream*>(inClientData);
				lpStream->vSeek(inPosition, LynxEngine::CStream::SeekSet);
				*actualCount = lpStream->vWrite(buffer, 1, requestCount);
			
                return noErr;
            }
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			SInt64 iOSAudioFileGetSize(void *inClientData)
			{
				LynxEngine::CStream* lpStream = static_cast<LynxEngine::CStream*>(inClientData);
				return lpStream->vSize();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			OSStatus iOSAudioFileSetSize(void  *inClientData, SInt64 inSize)
			{
				return noErr;
			}
		#endif

		#ifdef __iOS__
			#define MAX_SOURCE							64	
		#else
			#define MAX_SOURCE							64
		#endif
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundSystem::CSoundSystem(LynxEngine::CEngine* const lpengine)
		: LynxEngine::SoundSystem::CSoundSystem(lpengine)
		{
			m_Device = NULL;
			m_Context = NULL;
			m_SourceArray.resize(MAX_SOURCE);
			for (int i=0; i<MAX_SOURCE; i++)
			{
				m_SourceArray[i] = 0;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSoundSystem::~CSoundSystem(void)
		{			
			for (int i=0; i<MAX_SOURCE; i++)
			{
				if (m_SourceArray[i])
					alDeleteSources(1, &m_SourceArray[i]);		
			}
			m_SourceArray.clear();

			alcMakeContextCurrent(NULL);
			alcDestroyContext(m_Context);
			m_Context = NULL;

			alcCloseDevice(m_Device);
			m_Device = NULL;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vCreate()
		{			
			LynxEngine::SoundSystem::CSoundSystem::vCreate();

			LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Create sound subsystem...\r\n")));
			LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Create sound subsystem OK.\r\n")));

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSoundSystem::vInit()
		{
			char *DefaultDeviceName = NULL;

			vLoadParam(m_lpEngine->GetConfigFilename());

			#ifdef __iOS__			
				LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Init OpenAL...\r\n")));
				DefaultDeviceName = NULL;
				LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Init OpenAL OK.\r\n")));
			#else
				LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Find all devices...\r\n")));

				char *Devices = (char *)alcGetString(NULL, ALC_DEVICE_SPECIFIER);
				DefaultDeviceName = (char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);		

				LYNXCHAR Buffer[512];
				int DeviceIndex = 0;
				// go through device list (each device terminated with a single NULL, list terminated with double NULL)
				while (*Devices != NULL) 
				{
					ALCdevice *Device = alcOpenDevice(Devices);
					if (Device) 
					{
						ALCcontext *Context = alcCreateContext(Device, NULL);
						if (Context) 
						{
							alcMakeContextCurrent(Context);
							// if new actual device name isn't already in the list, then add it...
							const char *ActualDeviceName = alcGetString(Device, ALC_DEVICE_SPECIFIER);
							int MajorVersion, MinorVersion;
							alcGetIntegerv(Device, ALC_MAJOR_VERSION, sizeof(int), &MajorVersion);
							alcGetIntegerv(Device, ALC_MINOR_VERSION, sizeof(int), &MinorVersion);

							lynxSprintf(Buffer, _T("\tOpenAL device[%d] Name : %s\r\n"), DeviceIndex, LynxEngine::CString(ActualDeviceName).c_str());
							LYNX_LOG(glpLogger, LynxEngine::CString(Buffer));
							lynxSprintf(Buffer, _T("\tOpenAL device[%d] Version : %d.%d\r\n"), DeviceIndex, MajorVersion, MinorVersion);
							LYNX_LOG(glpLogger, LynxEngine::CString(Buffer));

							alcMakeContextCurrent(NULL);
							alcDestroyContext(Context);
						}
						alcCloseDevice(Device);
					}
					Devices += strlen(Devices) + 1;
					DeviceIndex++;
				}			
				LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Find all devices OK.\r\n")));
			#endif			

			m_Device = alcOpenDevice(DefaultDeviceName);
			if (!m_Device) 
				return LYNX_FALSE;

			m_Context = alcCreateContext(m_Device, NULL);
			if (!m_Context) 
				return LYNX_FALSE;

			alcMakeContextCurrent(m_Context);

			for (int i=0; i<MAX_SOURCE; i++)
			{
				if (m_SourceArray[i])
					alDeleteSources(1, &m_SourceArray[i]);		

				alGenSources(1, &m_SourceArray[i]);		

				if (!m_SourceArray[i])				
				{
					LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::[Error] Can't generate OpenAL source!!\n")));
				}
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
				// set the current context to NULL will 'shutdown' openAL
				alcMakeContextCurrent(NULL);
				// now suspend your context to 'pause' your sound world
				alcSuspendContext(m_Context);
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
				// Restore open al context
				alcMakeContextCurrent(m_Context);
				// 'unpause' my context
				alcProcessContext(m_Context);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSoundSystem::ResetSoundSource(ALuint s)
		{
			for (LynxEngine::CList<LynxEngine::SoundSystem::CSoundPtr>::CIterator Sound = m_SoundList.begin(); Sound != m_SoundList.end(); ++Sound)
			{
				OpenAL::CSound* OALSound = static_cast<OpenAL::CSound*>(&(*(*Sound)));
				if (OALSound->GetSource() == s)
				{
					OALSound->SetSource(0);
					break;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		ALuint CSoundSystem::GetFreeSource(OpenAL::CSound* s)
		{
			ALint State;

			for (int i=0; i<MAX_SOURCE; i++)
			{
				if (m_SourceArray[i])
				{
					alGetSourcei(m_SourceArray[i], AL_SOURCE_STATE, &State);
					if (State != AL_PLAYING)
					{						
						ResetSoundSource(m_SourceArray[i]);
						return m_SourceArray[i];
					}
				}
			}

			/*
			for (int i=0; i<MAX_SOURCE; i++)
			{
				if (m_SourceArray[i])
				{
					alGetSourcei(m_SourceArray[i], AL_LOOPING, &State);
					if (!State)
					{
						LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::[Warning] Stop some playing sound to get free OpenAL source!!\n")));
						alSourceStop(m_SourceArray[i]);
						ResetSoundSource(m_SourceArray[i]);
						return m_SourceArray[i];
					}
				}
			}

			alSourceStop(m_SourceArray[0]);
			ResetSoundSource(m_SourceArray[0]);
			return m_SourceArray[0];	
			*/
			

			// if can't find any free source, stop low and medium sound to get their source
			if (s->m_Priority == LynxEngine::SoundSystem::SP_HIGH)
			{
				for (LynxEngine::CList<LynxEngine::SoundSystem::CSoundPtr>::CIterator Sound = m_SoundList.begin(); Sound != m_SoundList.end(); ++Sound)
				{
					OpenAL::CSound* OALSound = static_cast<OpenAL::CSound*>(&(*(*Sound)));
					if (OALSound->GetSource())
					{
						if (OALSound->m_Priority == LynxEngine::SoundSystem::SP_LOW)
						{
							LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName()) + LynxEngine::CString(_T("::[Warning] Stop low priority playing sound to get free OpenAL source!!\n")));
							//OALSound->vStop();
							ALuint TempSource = OALSound->GetSource();
							alSourceStop(TempSource);
							OALSound->SetSource(0);
							return TempSource;
						}
					}
				}

				for (LynxEngine::CList<LynxEngine::SoundSystem::CSoundPtr>::CIterator Sound = m_SoundList.begin(); Sound != m_SoundList.end(); ++Sound)
				{
					OpenAL::CSound* OALSound = static_cast<OpenAL::CSound*>(&(*(*Sound)));
					if (OALSound->GetSource())
					{
						if (OALSound->m_Priority == LynxEngine::SoundSystem::SP_MEDIUM)
						{
							LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName()) + LynxEngine::CString(_T("::[Warning] Stop medium priority playing sound to get free OpenAL source!!\n")));
							//OALSound->vStop();
							ALuint TempSource = OALSound->GetSource();
							alSourceStop(TempSource);
							OALSound->SetSource(0);
							return TempSource;
						}
					}
				}

				//alSourceStop(m_SourceArray[0]);
				//ResetSoundSource(m_SourceArray[0]);
				//return m_SourceArray[0];	
			}			

			LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName()) + LynxEngine::CString(_T("::[Error] Can't find any free OpenAL source!!\n")));
			return 0;
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
	}
}