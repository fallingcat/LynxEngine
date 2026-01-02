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

#include <LynxOALSoundSystem.h>
#include <LynxOALSound.h>
#include <LynxOALBuffer.h>
#include <vorbis/vorbisfile.h>
#ifdef __iOS__
    #include <AudioToolbox/AudioToolbox.h>
#endif

namespace LynxSound
{
	namespace OpenAL 
	{			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSound::CSound(LynxEngine::CEngine* const lpengine)
		: LynxEngine::SoundSystem::CSound(lpengine)
		{				
			m_Source = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSound::~CSound(void)
		{
			m_Source = 0;
		}
    	//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		unsigned long CSound::GetWaveALBufferFormat(PFNALGETENUMVALUE pfnGetEnumValue)
		{
			if (pfnGetEnumValue)
			{
				//if (m_WaveIDs[WaveID]->wfType == WF_EX)
				{
					if (m_Format.Format.nChannels == 1)
					{
						switch (m_Format.Format.wBitsPerSample)
						{
							case 4:
								return pfnGetEnumValue("AL_FORMAT_MONO_IMA4");
								break;
							case 8:
								return pfnGetEnumValue("AL_FORMAT_MONO8");
								break;
							case 16:
								return pfnGetEnumValue("AL_FORMAT_MONO16");
								break;
						}
					}
					else if (m_Format.Format.nChannels == 2)
					{
						switch (m_Format.Format.wBitsPerSample)
						{
							case 4:
								return pfnGetEnumValue("AL_FORMAT_STEREO_IMA4");
								break;
							case 8:
								return pfnGetEnumValue("AL_FORMAT_STEREO8");
								break;
							case 16:
								return pfnGetEnumValue("AL_FORMAT_STEREO16");
								break;
						}
					}
					else if ((m_Format.Format.nChannels == 4) && (m_Format.Format.wBitsPerSample == 16))
						return pfnGetEnumValue("AL_FORMAT_QUAD16");
				}
				/*
				else if (m_WaveIDs[WaveID]->wfType == WF_EXT)
				{
					if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 1) &&
						((m_WaveIDs[WaveID]->wfEXT.dwChannelMask == SPEAKER_FRONT_CENTER) ||
						 (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)) ||
						 (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == 0)))
					{
						switch (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample)
						{
						case 4:
							return pfnGetEnumValue("AL_FORMAT_MONO_IMA4");
							break;
						case 8:
							return pfnGetEnumValue("AL_FORMAT_MONO8");
							break;
						case 16:
							return pfnGetEnumValue("AL_FORMAT_MONO16");
							break;
						}
					}
					else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 2) && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)))
					{
						switch (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample)
						{
						case 4:
							return pfnGetEnumValue("AL_FORMAT_STEREO_IMA4");
							break;
						case 8:
							return pfnGetEnumValue("AL_FORMAT_STEREO8");
							break;
						case 16:
							return pfnGetEnumValue("AL_FORMAT_STEREO16");
							break;
						}
					}
					else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 2) && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16) && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == (SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
						return  pfnGetEnumValue("AL_FORMAT_REAR16");
					else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 4) && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16) && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
						return pfnGetEnumValue("AL_FORMAT_QUAD16");
					else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 6) && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16) && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
						return pfnGetEnumValue("AL_FORMAT_51CHN16");
					else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 7) && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16) && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_BACK_CENTER)))
						return pfnGetEnumValue("AL_FORMAT_61CHN16");
					else if ((m_WaveIDs[WaveID]->wfEXT.Format.nChannels == 8) && (m_WaveIDs[WaveID]->wfEXT.Format.wBitsPerSample == 16) && (m_WaveIDs[WaveID]->wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_SIDE_LEFT|SPEAKER_SIDE_RIGHT)))
						return pfnGetEnumValue("AL_FORMAT_71CHN16");
				}		
				*/
			}	
			return pfnGetEnumValue("AL_FORMAT_UNKNOWN");
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::LoadSoundWAV(const LynxEngine::CString& filename, const LynxEngine::CSoundCreationDesc* desc)
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
								memcpy(&m_Format.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
							}
							/*
							else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
							{
								memcpy(&m_Format, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
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

			char* lpData = LYNXGLOBAL_NEW char[DataSize];
			if (!lpData)
			{
				lpStream->vClose();
				return LYNX_FALSE;
			}
			
			// Seek to start of audio data
			lpStream->vSeek(DataOffset, LynxEngine::CStream::SeekSet);																

			// Read Sample Data
			lpStream->vRead(lpData, 1, DataSize);										

			lpStream->vClose();
			
			// Set XRAM Mode (if application)
			//if (eaxSetBufferMode && eXRAMBufferMode)
			//	eaxSetBufferMode(1, &m_Buffer, eXRAMBufferMode);

			alBufferData(m_Buffer->GetID(), GetWaveALBufferFormat(&alGetEnumValue), lpData, DataSize, m_Format.Format.nSamplesPerSec);			

			LYNXGLOBAL_DEL_ARRAY(lpData);
			
			return LYNX_TRUE;
		}    
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::LoadSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CSoundCreationDesc* desc)
		{
			LynxEngine::CStream* lpStream = desc->m_lpStream;			
			int Endian = 0; // 0 for Little-Endian, 1 for Big-Endian
			int BitStream;
			vorbis_info* Info;
			OggVorbis_File OGGFile;
			ALenum Format;
			ALsizei Freq;  
			char Buffer[4096];			
			 std::vector<char> Data;
			long Bytes;

			OPEN_Z_FILE(lpStream, filename)
			else
			{
				if (!lpStream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
					return LYNX_FALSE;							
			}

			//if (!lpStream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
				//return LYNX_FALSE;

			CSoundSystem::OVOpen(lpStream, &OGGFile, NULL, 0);

			Info = ov_info(&OGGFile, -1);

			// Check the number of channels... always use 16-bit samples
			if (Info->channels == 1)
				Format = AL_FORMAT_MONO16;
			else
				Format = AL_FORMAT_STEREO16;

			 // The frequency of the sampling rate
			Freq = Info->rate;

			do {
				// Read up to a buffer's worth of decoded sound data
				Bytes = ov_read(&OGGFile, Buffer, 4096, Endian, 2, 1, &BitStream);
				// Append to end of buffer
				Data.insert(Data.end(), Buffer, Buffer + Bytes);
			} while (Bytes > 0);

			//Clean up
			ov_clear(&OGGFile);

			lpStream->vClose();
			
			alBufferData(m_Buffer->GetID(), Format, &Data[0], static_cast<ALsizei> (Data.size()), Freq);			
			Data.clear();
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vInstance(const CResource& rhs) 
		{
			CResource::vInstance(rhs);

			CSound* RHS = (CSound*)(&rhs);

			if (m_Source)
			{
				m_Source = 0;
			}
			
			m_Buffer = RHS->m_Buffer;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vCreate(const LynxEngine::CCreationDesc* desc)
		{	
			LynxEngine::CSoundCreationDesc* lpDesc = (LynxEngine::CSoundCreationDesc*)desc;
			LynxEngine::CString Filename;
			static LYNXCHAR Name[256], Ext[32];
			LYNXBOOL bReturn = LYNX_FALSE;
			
			if (lpDesc->m_bRecording)
			{				
			}
			else 
			{
				Filename = desc->m_FileName;				
			}

			if (Filename.empty())
			{
				return LYNX_FALSE;
			}

			lynxSeparateFileExtByPath(Name, Ext, Filename.c_str());									    			

			if (m_Source)
			{
				m_Source = 0;
			}

			m_Buffer = LynxEngine::CCountedPtr<CBuffer>(LYNXNEW CBuffer);
			m_Buffer->Create();			

			#ifdef __iOS__		
                LYNXBOOL bFileExist = LYNX_FALSE;
                static LYNXCHAR ExtList[][8] = { _T(".aac"),
                                                 _T(".m4a"),
                                                 _T(".caf"),
                                                 _T(".mp3")};
            
                LynxEngine::CStream* lpStream = desc->m_lpStream;
				lynxSeparateFileExtByPath(Name, Ext, Filename.c_str());
            
                bFileExist = LYNX_FALSE;
                LynxEngine::CString DefaultFormatFilename;
                for (int i=0; i<4; i++)
                {
                    DefaultFormatFilename = LynxEngine::CString(Name) + LynxEngine::CString(ExtList[i]);
                    if (lpStream->vOpen(DefaultFormatFilename.c_str(), LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
                    {
                        bFileExist = LYNX_TRUE;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                if (bFileExist)
                {
                    AudioFileID FileID;
                    OSStatus						err = noErr;
                    SInt64							theFileLengthInFrames = 0;
                    AudioStreamBasicDescription		theFileFormat;
                    UInt32							thePropertySize = sizeof(theFileFormat);
                    ExtAudioFileRef					extRef = NULL;
                    void*							theData = NULL;
                    AudioStreamBasicDescription		theOutputFormat;
                    
                    AudioFileOpenWithCallbacks((void*)lpStream, iOSAudioFileRead, iOSAudioFileWrite, iOSAudioFileGetSize, iOSAudioFileSetSize, 0, &FileID);
                    err = ExtAudioFileWrapAudioFileID(FileID, false, &extRef);
                    if (err)
                    {
                        //printf("MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", err); goto Exit;
                    }
                    
                    // Get the audio data format
                    err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
                    if (err)
                    {
                        //printf("MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", err); goto Exit;
                    }
                    if (theFileFormat.mChannelsPerFrame > 2)
                    {
                        //printf("MyGetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n"); goto Exit;
                    }
                    
                    // Set the client format to 16 bit signed integer (native-endian) data
                    // Maintain the channel count and sample rate of the original source format
                    theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
                    theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
                    
                    theOutputFormat.mFormatID = kAudioFormatLinearPCM;
                    theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
                    theOutputFormat.mFramesPerPacket = 1;
                    theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
                    theOutputFormat.mBitsPerChannel = 16;
                    theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
                    
                    // Set the desired client (output) data format
                    err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
                    if (err)
                    {
                        //printf("MyGetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ld\n", err); goto Exit;
                    }
                    
                    // Get the total frame count
                    thePropertySize = sizeof(theFileLengthInFrames);
                    err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
                    if (err)
                    {
                        //printf("MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ld\n", err); goto Exit;
                    }
                    
                    // Read all the data into memory
                    UInt32 DataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;
                    
                    char* lpData = LYNXNEW char[DataSize];
                    if (!lpData)
                    {
                        lpStream->vClose();
                        return LYNX_FALSE;
                    }
                    
                    AudioBufferList		theDataBuffer;
                    theDataBuffer.mNumberBuffers = 1;
                    theDataBuffer.mBuffers[0].mDataByteSize = DataSize;
                    theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
                    theDataBuffer.mBuffers[0].mData = lpData;
                    
                    // Read the data into an AudioBufferList
                    err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
                    if (err)
                    {
                    }
                    AudioFileClose(FileID); //close the file
                    lpStream->vClose();
                    
                    alBufferData(m_Buffer->GetID(), (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, lpData, DataSize, theOutputFormat.mSampleRate); 
                    
                    LYNXDEL_ARRAY(lpData);
                }
                else
                {
                    if (!lynxStriCmp(Ext, _T("ogg")))
                    {
                        // Load OGG file into OpenAL Buffer
                        if (!LoadSoundOGG(Filename, lpDesc))
                        {					
                            return LYNX_FALSE;
                        }
                    }
                    else
                    {
                        return LYNX_FALSE;
                    }
                }
			//#elif defined __DESKTOP__
			#else
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
					// Load MP3 file into OpenAL Buffer
					if (LoadSoundOGG(Filename, lpDesc))
					{					
						bReturn = LYNX_TRUE;;
					}			
				}
				/*
				else if (!lynxStriCmp(Ext, _T("mp3")))	
				{
					// Load MP3 file into OpenAL Buffer
					if (!LoadSoundMP3(Filename, lpDesc))
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
						if (m_Source)
						{
							m_Source = 0;
						}
						m_Buffer.Reset();

						return LYNX_FALSE;
					}
				}				
			#endif

			alGetError();
			if (alGetError() != AL_NO_ERROR)
			{				
				if (m_Source)
				{
					m_Source = 0;
				}
				m_Buffer.Reset();

				return LYNX_FALSE;
			}

			if (desc->m_Name.empty())
				SetName(Filename);
			else
				SetName(desc->m_Name);			
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vOpenRecorded()
		{
			/*
			m_lpAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:m_RecordedFileURL error:nil];	
			if (m_RecordedFileURL)
				[m_RecordedFileURL release];
			
			if (!m_lpAudioPlayer)
				return LYNX_FALSE;
			
			m_lpAudioPlayer.numberOfLoops = 0;
			m_lpAudioPlayer.volume = 1.0f;
			[m_lpAudioPlayer prepareToPlay];
			*/
		
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vIsPlaying() 
		{
			ALint State;
			
			if (m_Source)
			{
				alGetSourcei(m_Source, AL_SOURCE_STATE, &State);
				if (State == AL_PLAYING)
					return LYNX_TRUE;
			}

			return LYNX_FALSE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vPlay(void)
		{
			vSetVolume(m_Volume);

			if (!m_Source)
				m_Source = static_cast<CSoundSystem*>(m_lpEngine->GetlpSoundSystem())->GetFreeSource(this);

			if (m_Source)
			{
				alSourcei(m_Source, AL_BUFFER, m_Buffer->GetID());

				if (m_NumLoops < 0 || m_NumLoops > 1)
					alSourcei(m_Source, AL_LOOPING, AL_TRUE);
				else
					alSourcei(m_Source, AL_LOOPING, AL_FALSE);				

				float Volume = m_Volume * m_lpEngine->GetlpSoundSystem()->GetSoundVolumeMultiplier() * float(m_lpEngine->GetlpSoundSystem()->IsSoundEnabled()); 
				alSourcef(m_Source, AL_GAIN, Volume);			

				alSourcef(m_Source, AL_PITCH, 1.0f);

				alSourcePlay(m_Source);				

				m_bPlayed = LYNX_TRUE;
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vPlay(float pitch)
		{
			vSetVolume(m_Volume);

			if (!m_Source)
				m_Source = static_cast<CSoundSystem*>(m_lpEngine->GetlpSoundSystem())->GetFreeSource(this);

			if (m_Source)
			{
				alSourcei(m_Source, AL_BUFFER, m_Buffer->GetID());

				if (m_NumLoops < 0 || m_NumLoops > 1)
					alSourcei(m_Source, AL_LOOPING, AL_TRUE);
				else
					alSourcei(m_Source, AL_LOOPING, AL_FALSE);

				float Volume = m_Volume * m_lpEngine->GetlpSoundSystem()->GetSoundVolumeMultiplier() * float(m_lpEngine->GetlpSoundSystem()->IsSoundEnabled()); 
				alSourcef(m_Source, AL_GAIN, Volume);			

				alSourcef(m_Source, AL_PITCH, pitch);

				alSourcePlay(m_Source);				

				m_bPlayed = LYNX_TRUE;
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vPause(void)
		{
			if (m_Source)
				alSourcePause(m_Source);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vStop(void)
		{
			if (m_Source)
			{
				alSourceStop(m_Source);
				m_Source = 0;
			}
			m_bPlayed = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		float CSound::vGetVolume()
		{
			//float Volume;
			//alGetSourcef(m_Source, AL_MAX_GAIN, &Volume);			

			return m_Volume;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vSetVolume(float v)
		{	
			m_Volume = v;
			float Volume = v * m_lpEngine->GetlpSoundSystem()->GetSoundVolumeMultiplier() * float(m_lpEngine->GetlpSoundSystem()->IsSoundEnabled()); 
			if (m_Source)
				alSourcef(m_Source, AL_GAIN, Volume);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vSetLoops(int n)
		{
			m_NumLoops = n;

			if (m_Source)
			{
				if (n < 0 || n > 1)
					alSourcei(m_Source, AL_LOOPING, AL_TRUE);
				else
					alSourcei(m_Source, AL_LOOPING, AL_FALSE);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vIsRecording() 
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSound::vRecord(void)
		{
			/*
			[[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryRecord error: nil];
			
			NSDictionary *RecordSettings =
			[[NSDictionary alloc] initWithObjectsAndKeys:
			 [NSNumber numberWithFloat: 44100.0], AVSampleRateKey,
             [NSNumber numberWithInt: kAudioFormatAppleLossless], AVFormatIDKey,
             [NSNumber numberWithInt: 1], AVNumberOfChannelsKey,
             [NSNumber numberWithInt: AVAudioQualityMax],
			 AVEncoderAudioQualityKey,
             nil];
			
			m_lpAudioRecorder = 
            [[AVAudioRecorder alloc] initWithURL: m_RecordedFileURL
                                        settings: RecordSettings
                                           error: nil];
			[RecordSettings release];
			
			if (!m_lpAudioRecorder)
				return LYNX_FALSE;
			
			[m_lpAudioRecorder prepareToRecord];
			[m_lpAudioRecorder record];
			*/
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vPauseRecording(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSound::vStopRecording(void)
		{			
		}
	}
}