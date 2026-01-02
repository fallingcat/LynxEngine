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

#include <LynxOSLSoundSystem.h>
#include <LynxOSLMusic.h>
#include <vorbis/vorbisfile.h>

#define BUFFER_SIZE					(64*1024)

namespace LynxSound 
{
	namespace OpenSL 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMusic::CMusic(LynxEngine::CEngine* const lpengine)
		: LynxEngine::SoundSystem::CMusic(lpengine)
		{					
			m_Stream = NULL;
			m_PlayerObject = NULL;
			m_PlayerPlay = NULL;
			m_PlayerVolume = NULL;
			m_PlayerBufferQueue = NULL;
			m_bDelete = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMusic::~CMusic(void)
		{
			LYNXDEL_ARRAY(m_Buffer);

			m_PlayerObject = NULL;
			m_PlayerPlay = NULL;
			m_PlayerVolume = NULL;
			m_PlayerBufferQueue = NULL;

			if (m_bStreaming)
			{
				//Clean up
				ov_clear(&m_OGGFile);

				if (m_Stream)
					m_Stream->vClose();
			}
		}
    	//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::LoadSoundWAV(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc)
		{
			WAVEFILEHEADER waveFileHeader;
			RIFFCHUNK riffChunk;
			WAVEFMT	waveFmt;
			LynxEngine::CStream* lpStream = desc->m_lpStream;
			LYNXLONG DataSize = 0;
			LYNXLONG DataOffset = 0;		
			
			// Open the wave file for reading
			if (!lpStream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))						
				return LYNX_FALSE;
			
			// Read Wave file header
			lpStream->vRead(&waveFileHeader, 1, sizeof(WAVEFILEHEADER));
			//if (!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4) && !_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
			if (!strncmp(waveFileHeader.szRIFF, "RIFF", 4) && !strncmp(waveFileHeader.szWAVE, "WAVE", 4))
			{
				while (lpStream->vRead(&riffChunk, 1, sizeof(RIFFCHUNK)) == sizeof(RIFFCHUNK))
				{
					//if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4))
					if (!strncmp(riffChunk.szChunkName, "fmt ", 4))
					{
						if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
						{
							lpStream->vRead(&waveFmt, 1, riffChunk.ulChunkSize);
						
							// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
							if (waveFmt.usFormatTag == WAVE_FORMAT_PCM)
							{
								memcpy(&m_WavFormat.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
							}
							/*
							else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
							{
								memcpy(&m_WavFormat, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
							}
							*/
						}
						else
						{
							lpStream->vSeek(riffChunk.ulChunkSize, LynxEngine::CStream::SeekCur);
						}
					}
					//else if (!_strnicmp(riffChunk.szChunkName, "data", 4))
					else if (!strncmp(riffChunk.szChunkName, "data", 4))
					{
						DataSize = riffChunk.ulChunkSize;
						DataOffset = lpStream->vTell();
						lpStream->vSeek(riffChunk.ulChunkSize, LynxEngine::CStream::SeekCur);						
					}
					else
					{
						lpStream->vSeek(riffChunk.ulChunkSize, LynxEngine::CStream::SeekCur);						
					}

					// Ensure that we are correctly aligned for next chunk
					if (riffChunk.ulChunkSize & 1)
						lpStream->vSeek(1, LynxEngine::CStream::SeekCur);												
				}									
			}

			if (!(DataSize && DataOffset && ((waveFmt.usFormatTag == WAVE_FORMAT_PCM))))// || (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE))))
			{
				lpStream->vClose();
				return LYNX_FALSE;
			}

			m_Channels = m_WavFormat.Format.nChannels,
			m_Frequency = m_WavFormat.Format.nSamplesPerSec * 1000,  
			m_BitsPerSample = m_WavFormat.Format.wBitsPerSample,

			m_Buffer = LYNXNEW char[DataSize];
			m_DataSize = DataSize;
			if (!m_Buffer)
			{
				lpStream->vClose();
				return LYNX_FALSE;
			}
			
			// Seek to start of audio data
			lpStream->vSeek(DataOffset, LynxEngine::CStream::SeekSet);																

			// Read Sample Data
			lpStream->vRead(m_Buffer, 1, DataSize);										

			lpStream->vClose();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::LoadSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc)
		{
			LynxEngine::CStream* lpStream = desc->m_lpStream;
			int Endian = 0; // 0 for Little-Endian, 1 for Big-Endian
			int BitStream;
			vorbis_info* Info;
			OggVorbis_File OGGFile;
			char Buffer[BUFFER_SIZE];
			//LynxEngine::CArray<char> Data;
			std::vector<char> Data;
			long Bytes;

			OPEN_Z_FILE(lpStream, filename)
			else
			{
				if (!lpStream->vOpen(filename, LynxEngine::CStream::Read | LynxEngine::CStream::Binary))
					return LYNX_FALSE;
			}

			//if (!lpStream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
			//	return LYNX_FALSE;

			CSoundSystem::OVOpen(lpStream, &OGGFile, NULL, 0);

			Info = ov_info(&OGGFile, -1);

			// Check the number of channels... always use 16-bit samples
			m_BitsPerSample = 16;
			m_Channels = Info->channels;

			 // The frequency of the sampling rate
			m_Frequency = Info->rate * 1000;

			do {
				// Read up to a buffer's worth of decoded sound data
				Bytes = ov_read(&OGGFile, Buffer, BUFFER_SIZE, Endian, 2, 1, &BitStream);
				// Append to end of buffer
				Data.insert(Data.end(), Buffer, Buffer + Bytes);
			} while (Bytes > 0);

			//Clean up
			ov_clear(&OGGFile);

			lpStream->vClose();
			
			m_Buffer = LYNXNEW char[Data.size()];
			std::copy(Data.begin(), Data.end(), m_Buffer);
			m_DataSize = Data.size();
			
			Data.clear();
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::OpenSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc)
		{
			LynxEngine::CStream* m_Stream = desc->m_lpStream;						
			vorbis_info* Info;

			if (!m_Stream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
				return LYNX_FALSE;

			CSoundSystem::OVOpen(m_Stream, &m_OGGFile, NULL, 0);

			Info = ov_info(&m_OGGFile, -1);

			// Check the number of channels... always use 16-bit samples
			m_BitsPerSample = 16;
			m_Channels = Info->channels;

			 // The frequency of the sampling rate
			m_Frequency = Info->rate * 1000;
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::StreamOGG(unsigned int buffer)
		{
			char Data[BUFFER_SIZE];
			int Endian = 0; // 0 for Little-Endian, 1 for Big-Endian
			int BitStream;
			int  Size = 0;
			int  Result;
 
			while (Size < BUFFER_SIZE)
			{
				Result = ov_read(&m_OGGFile, Data + Size, BUFFER_SIZE - Size, Endian, 2, 1, &BitStream);
    
				if (Result < 0)
				{
					return LYNX_FALSE;
				}
				else if (Result == 0)
				{
					if (m_bLoop)
					{
						ov_time_seek(&m_OGGFile, 0.0f);
						if (m_LoopTimes > 0)
							m_LoopTimes--;
					}
					else
					{
						break;
					}
				}
				else if (Result > 0)
				{
					Size += Result;				
				}
			}    
			if(Size == 0)
				return LYNX_FALSE;
				
			m_Buffer = Data;
			m_DataSize = Size;		
 
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vUpdateOGG()
		{
			int Processed;
			LYNXBOOL bActive = LYNX_TRUE;
 
			return bActive;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vCreate(const LynxEngine::CCreationDesc* desc)
		{	
			if (desc)
			{
				LynxEngine::CMusicCreationDesc* lpDesc = (LynxEngine::CMusicCreationDesc*)desc;
				LynxEngine::CString Filename;
				static LYNXCHAR Name[256], Ext[32];
				LYNXBOOL bReturn = LYNX_FALSE;
			
				Filename = desc->m_FileName;				
			
				if (Filename.empty())
				{
					return LYNX_FALSE;
				}

				lynxSeparateFileExtByPath(Name, Ext, Filename.c_str());									    			


				m_bStreaming = lpDesc->m_bStreaming;

				if (lynxStriCmp(Ext, _T("ogg")))
				{
					m_bStreaming = LYNX_FALSE;
				}
				else
				{
					if (!desc->m_lpStream->vOpen(Filename.c_str(), LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
					{
						m_bStreaming = LYNX_FALSE;
					}
					else
					{
						desc->m_lpStream->vClose();
					}
				}

				if (m_bStreaming)
				{
					if (!lynxStriCmp(Ext, _T("ogg")))	
					{
						// Load OGG file into OpenAL Buffer
						if (OpenSoundOGG(Filename, lpDesc))
						{					
							bReturn = LYNX_TRUE;
						}			
					}			
				}
				else
				{
					if (!lynxStriCmp(Ext, _T("wav")))
					{
						// Load Wave file into OpenAL Buffer
						if (LoadSoundWAV(Filename, lpDesc))
						{					
							bReturn = LYNX_TRUE;
						}			
					}
					else if (!lynxStriCmp(Ext, _T("ogg")))	
					{
						// Load OGG file into OpenAL Buffer
						if (LoadSoundOGG(Filename, lpDesc))
						{					
							bReturn = LYNX_TRUE;
						}			
					}
					/*
					else if (!lynxStriCmp(Ext, _T("mp3")))	
					{
						// Load MP3 file into OpenAL Buffer
						//if (!LoadSoundWAV(Filename, lpDesc))
						{					
							bReturn = LYNX_TRUE;
						}			
					}
					*/

					if (!bReturn)
					{
						Filename = Name;
						Filename += LynxEngine::CString(_T(".wav"));
						// Load Wave file into OpenAL Buffer
						if (!LoadSoundWAV(Filename, lpDesc))
						{
							//alDeleteBuffers(1, &(m_Buffer[0]));
							return LYNX_FALSE;
						}
					}
				}	

				if (desc->m_Name.empty())
					SetName(Filename);
				else
					SetName(desc->m_Name);

				// Setup attributes
				vSetLoops(lpDesc->m_NumLoops);
				vSetVolume(lpDesc->m_Volume);
			}

			/*
			SLEngineItf EngineEngine = ((LynxSound::OpenSL::CSoundSystem*)(m_lpEngine->GetlpSoundSystem()))->m_EngineEngine;
			SLObjectItf OutputMixObject = ((LynxSound::OpenSL::CSoundSystem*)(m_lpEngine->GetlpSoundSystem()))->m_OutputMixObject;

			SLresult result;  
			// configure audio source  
			SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1 };  
			SLDataFormat_PCM format_pcm = { SL_DATAFORMAT_PCM,
											m_Channels,
											m_Frequency,  
											m_BitsPerSample,
											m_BitsPerSample,
											m_Channels==1? SL_SPEAKER_FRONT_CENTER : SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
											SL_BYTEORDER_LITTLEENDIAN };  
			SLDataSource audioSrc = { &loc_bufq, &format_pcm };  
  
			// configure audio sink  
			SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, OutputMixObject };  
			SLDataSink audioSnk = { &loc_outmix, NULL };  
  
			// create audio player  
			const SLInterfaceID ids[] = { SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_PLAY };  
			const SLboolean req[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };  
			result = (*EngineEngine)->CreateAudioPlayer(EngineEngine, &m_PlayerObject, &audioSrc, &audioSnk, 3, ids, req);  
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);  
			(void) result;  
  
			// realize the player  
			result = (*m_PlayerObject)->Realize(m_PlayerObject, SL_BOOLEAN_FALSE);  
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);  
			(void) result;  
			*/
			else
			{
				m_PlayerObject = ((LynxSound::OpenSL::CSoundSystem*)(m_lpEngine->GetlpSoundSystem()))->GetFreeAudioPlayer(m_Frequency, m_Channels);
				if (!m_PlayerObject)
					return LYNX_FALSE;

				SLresult result;
				// get the play interface  
				result = (*m_PlayerObject)->GetInterface(m_PlayerObject, SL_IID_PLAY, &m_PlayerPlay);  
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
				(void) result;  

				result = (*m_PlayerObject)->GetInterface(m_PlayerObject, SL_IID_VOLUME, &m_PlayerVolume);  
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
				(void) result;  
  
				// get the buffer queue interface  
				result = (*m_PlayerObject)->GetInterface(m_PlayerObject, SL_IID_BUFFERQUEUE, &m_PlayerBufferQueue);  
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
				(void) result;

				result = (*m_PlayerBufferQueue)->RegisterCallback(m_PlayerBufferQueue, PlayerCallback, this);
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
				(void) result;

				result = (*m_PlayerBufferQueue)->Clear(m_PlayerBufferQueue);
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
				(void) result;

				result = (*m_PlayerBufferQueue)->Enqueue(m_PlayerBufferQueue, m_Buffer, m_DataSize);
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
				(void) result;

				if (m_bStreaming)
				{
					m_lpEngine->GetlpSoundSystem()->StartStreamingThread();
				}
			}

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vIsPlaying() 
		{
			SLuint32 State;

			if (!m_PlayerObject)
				return LYNX_FALSE;

			SLresult result;
			result = (*m_PlayerObject)->GetInterface(m_PlayerObject, SL_IID_PLAY, &m_PlayerPlay);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return LYNX_FALSE;
			}
		#endif
			(void) result;

			result = (*m_PlayerPlay)->GetPlayState(m_PlayerPlay, &State);
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return LYNX_FALSE;
			}
		#endif

			return State == SL_PLAYSTATE_PLAYING;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vPlay(void)
		{
			if (!vCreate(NULL))
				return LYNX_FALSE;

			if (!m_PlayerObject)
				return LYNX_FALSE;

			vSetVolume(m_Volume);

			if (!vIsPlaying())
			{
				SLresult result;

				result = (*m_PlayerBufferQueue)->Clear(m_PlayerBufferQueue);
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
				(void) result; 

				result = (*m_PlayerBufferQueue)->Enqueue(m_PlayerBufferQueue, m_Buffer, m_DataSize);
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
				(void) result;

				result = (*m_PlayerPlay)->SetPlayState(m_PlayerPlay, SL_PLAYSTATE_PLAYING);
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return LYNX_FALSE;
				}
			#endif
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vPause(void)
		{
			SLuint32 State;

			if (!m_PlayerObject)
				return;

			SLresult result;
			result = (*m_PlayerObject)->GetInterface(m_PlayerObject, SL_IID_PLAY, &m_PlayerPlay);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return;
			}
		#endif
			(void) result;

			result = (*m_PlayerPlay)->SetPlayState(m_PlayerPlay, SL_PLAYSTATE_PAUSED);
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return;
			}
		#endif

			if (m_bStreaming)
			{
				m_ReadyForStreming = LYNX_FALSE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vStop(void)
		{
			if (!m_PlayerObject)
				return;

			SLresult result;
			result = (*m_PlayerObject)->GetInterface(m_PlayerObject, SL_IID_PLAY, &m_PlayerPlay);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return;
			}
		#endif
			(void) result;
			result = (*m_PlayerPlay)->SetPlayState(m_PlayerPlay, SL_PLAYSTATE_STOPPED);
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return;
			}
		#endif
			(void) result;
			result = (*m_PlayerBufferQueue)->Clear(m_PlayerBufferQueue);
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return;
			}
		#endif
			(void) result;

			m_PlayerObject = NULL;

			if (m_bStreaming)
			{
				m_ReadyForStreming = LYNX_FALSE;
				//alSourceUnqueueBuffers(m_Source, 2, m_Buffer);
				ov_time_seek(&m_OGGFile, 0.0f);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		float CMusic::vGetVolume()
		{
			return m_Volume;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vSetVolume(float v)
		{	
			m_Volume = v;

			if (!m_PlayerObject)
				return;

			float Volume = v * m_lpEngine->GetlpSoundSystem()->GetMusicVolumeMultiplier() * float(m_lpEngine->GetlpSoundSystem()->IsMusicEnabled());

			int mb = lroundf(2000.f * log10f(Volume));
			if (mb < SL_MILLIBEL_MIN)
				mb = SL_MILLIBEL_MIN;
			else if (mb > 0)
				mb = 0; /* maximum supported level could be higher: GetMaxVolumeLevel */

			SLresult result = (*m_PlayerObject)->GetInterface(m_PlayerObject, SL_IID_VOLUME, &m_PlayerVolume);  
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return;
			}
		#endif
			(void) result;  

			result = (*m_PlayerVolume)->SetVolumeLevel(m_PlayerVolume, mb);
		#ifdef _DEBUG
			LYNX_ASSERT(SL_RESULT_SUCCESS == result);
		#else
			if (SL_RESULT_SUCCESS != result)
			{
				return;
			}
		#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vSetLoops(int n)
		{
			if (n < 0 || n > 1)
			{
				m_bLoop = LYNX_TRUE;
				m_LoopTimes = n;
			}
			else
			{
				//alSourcei(m_Source, AL_LOOPING, AL_FALSE);
				m_bLoop = LYNX_FALSE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::OnEndOfBuffer()
		{
			if (m_bDelete)
			{
				vStop();
				LynxEngine::SoundSystem::CMusicPtr Music = ((LynxSound::OpenSL::CSoundSystem*)(m_lpEngine->GetlpSoundSystem()))->FindMusic(this);
				((LynxSound::OpenSL::CSoundSystem*)(m_lpEngine->GetlpSoundSystem()))->m_MusicList.remove(Music);
				return;
			}

			if (m_LoopTimes != 0)
			{
				SLresult result = (*m_PlayerBufferQueue)->Enqueue(m_PlayerBufferQueue, m_Buffer, m_DataSize);
			#ifdef _DEBUG
				LYNX_ASSERT(SL_RESULT_SUCCESS == result);
			#else
				if (SL_RESULT_SUCCESS != result)
				{
					return;
				}
			#endif

				if (m_LoopTimes > 0)
					m_LoopTimes--;
			}
			else
			{
				vStop();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::PlayerCallback(SLAndroidSimpleBufferQueueItf bq, void* context) {
			LynxSound::OpenSL::CMusic* Music = ((LynxSound::OpenSL::CMusic*)context);

			Music->OnEndOfBuffer();
		}
	}
}