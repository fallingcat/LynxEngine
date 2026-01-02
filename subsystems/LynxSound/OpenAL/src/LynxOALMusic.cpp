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
#include <LynxOALMusic.h>
#include <vorbis/vorbisfile.h>
#ifdef __iOS__
	#include <AudioToolbox/AudioToolbox.h>
#elif defined __ANDROID__
	#include <AL/alext.h>    
#endif

#define BUFFER_SIZE					(64*1024)

namespace LynxSound 
{
	namespace OpenAL 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CMusic::CMusic(LynxEngine::CEngine* const lpengine)
		: LynxEngine::SoundSystem::CMusic(lpengine)
		{				
			m_Buffer[0] = 0;
			m_Buffer[1] = 0;
			m_Source = 0;			
			m_Stream = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CMusic::~CMusic(void)
		{
			alDeleteSources(1, &m_Source);
			alDeleteBuffers(2, m_Buffer);

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
		//  :   
		//-------------------------------------------------------------------------------------------------------
		unsigned long CMusic::GetWaveALBufferFormat(PFNALGETENUMVALUE pfnGetEnumValue)
		{
			if (pfnGetEnumValue)
			{
				//if (m_WaveIDs[WaveID]->wfType == WF_EX)
				{
					if (m_WavFormat.Format.nChannels == 1)
					{
						switch (m_WavFormat.Format.wBitsPerSample)
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
					else if (m_WavFormat.Format.nChannels == 2)
					{
						switch (m_WavFormat.Format.wBitsPerSample)
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
					else if ((m_WavFormat.Format.nChannels == 4) && (m_WavFormat.Format.wBitsPerSample == 16))
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
			return pfnGetEnumValue("AL_FORMAT_UKNOWN");
		}
    	//-------------------------------------------------------------------------------------------------------
		//
		//  :   
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

			alBufferData(m_Buffer[0], GetWaveALBufferFormat(&alGetEnumValue), lpData, DataSize, m_WavFormat.Format.nSamplesPerSec);

			LYNXGLOBAL_DEL_ARRAY(lpData);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
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
				if (!lpStream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
					return LYNX_FALSE;							
			}
			//if (!lpStream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
				//return LYNX_FALSE;

			CSoundSystem::OVOpen(lpStream, &OGGFile, NULL, 0);

			Info = ov_info(&OGGFile, -1);

			// Check the number of channels... always use 16-bit samples
			if (Info->channels == 1)
				m_Format = AL_FORMAT_MONO16;
			else
				m_Format = AL_FORMAT_STEREO16;

			 // The frequency of the sampling rate
			m_Freq = Info->rate;

			do {
				// Read up to a buffer's worth of decoded sound data
				Bytes = ov_read(&OGGFile, Buffer, BUFFER_SIZE, Endian, 2, 1, &BitStream);
				// Append to end of buffer
				Data.insert(Data.end(), Buffer, Buffer + Bytes);
			} while (Bytes > 0);

			//Clean up
			ov_clear(&OGGFile);

			lpStream->vClose();
			
			alBufferData(m_Buffer[0], m_Format, &Data[0], static_cast<ALsizei> (Data.size()), m_Freq);			
			Data.clear();
			
			return LYNX_TRUE;
		}
		/*
		LYNXBOOL CMusic::LoadSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc)
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

			DataSize = lpStream->vSize();			
			
			char* lpData = LYNXNEW char[DataSize];
			if (!lpData)
			{
				lpStream->vClose();
				return LYNX_FALSE;
			}	
			
			// Read Sample Data
			lpStream->vRead(lpData, 1, DataSize);										

			lpStream->vClose();
			
			// Set XRAM Mode (if application)
			//if (eaxSetBufferMode && eXRAMBufferMode)
			//	eaxSetBufferMode(1, &m_Buffer, eXRAMBufferMode);

			alBufferData(m_Buffer, AL_FORMAT_VORBIS_EXT, lpData, DataSize, 1);

			LYNXDEL_ARRAY(lpData);
			
			return LYNX_TRUE;
		}	  
		*/
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::OpenSoundOGG(const LynxEngine::CString& filename, const LynxEngine::CMusicCreationDesc* desc)
		{
			LynxEngine::CStream* lpStream = desc->m_lpStream;
			vorbis_info* Info;

            OPEN_Z_FILE(lpStream, filename)
            else
            {
                if (!lpStream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
                    return LYNX_FALSE;
            }
            
			//if (!lpStream->vOpen(filename, LynxEngine::CStream::Read|LynxEngine::CStream::Binary))
			//	return LYNX_FALSE;

			CSoundSystem::OVOpen(lpStream, &m_OGGFile, NULL, 0);

			Info = ov_info(&m_OGGFile, -1);

			// Check the number of channels... always use 16-bit samples
			if (Info->channels == 1)
				m_Format = AL_FORMAT_MONO16;
			else
				m_Format = AL_FORMAT_STEREO16;

			 // The frequency of the sampling rate
			m_Freq = Info->rate;
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::StreamOGG(ALuint buffer)
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
 
			alBufferData(buffer, m_Format, Data, Size, m_Freq);			
 
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vUpdateOGG()
		{
            int Processed;
			LYNXBOOL bActive = LYNX_TRUE;
 
			alGetSourcei(m_Source, AL_BUFFERS_PROCESSED, &Processed);
 
			while (Processed--)
			{
				ALuint Buffer;
        
				alSourceUnqueueBuffers(m_Source, 1, &Buffer);
				
				bActive = StreamOGG(Buffer);
 
				alSourceQueueBuffers(m_Source, 1, &Buffer);				
			}
            
            LYNX_LOG(glpLogger, LynxEngine::CString("Streaming Music!\r\n"));
 
			return bActive;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vCreate(const LynxEngine::CCreationDesc* desc)
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

			if (m_Source)
				alDeleteSources(1, &m_Source);
						
			if (m_Buffer[0])
				alDeleteBuffers(1, &(m_Buffer[0]));
			if (m_Buffer[1])
				alDeleteBuffers(1, &(m_Buffer[1]));

			m_bStreaming = lpDesc->m_bStreaming;

            /*
            if (lynxStriCmp(Ext, _T("ogg")))
			{
				//m_bStreaming = LYNX_FALSE;
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
            */

			if (m_bStreaming)
			{
				alGenBuffers(2, m_Buffer);	

				if (!lynxStriCmp(Ext, _T("ogg")))	
				{
					// Load MP3 file into OpenAL Buffer
					if (OpenSoundOGG(Filename, lpDesc))
					{					
						bReturn = LYNX_TRUE;
					}			
				}			
				alGenSources(1, &m_Source);
			}
			else
			{
				alGenBuffers(1, &(m_Buffer[0]));			
			
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
                    
                        char* lpData = LYNXGLOBAL_NEW char[DataSize];
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
                    
                        alBufferData(m_Buffer[0], (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, lpData, DataSize, theOutputFormat.mSampleRate);
                    
						LYNXGLOBAL_DEL_ARRAY(lpData);
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
                            // Load MP3 file into OpenAL Buffer
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
                                alDeleteBuffers(1, &(m_Buffer[0]));
                                return LYNX_FALSE;
                            }
                        }
                    }
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
							alDeleteBuffers(1, &(m_Buffer[0]));
							return LYNX_FALSE;
						}
					}
				#endif

				// Generate a Source to playback the Buffer
				alGenSources(1, &m_Source);

				// Attach Source to Buffer
				alSourcei(m_Source, AL_BUFFER, m_Buffer[0]);
			}

			// Setup attributes
			vSetLoops(lpDesc->m_NumLoops);
			vSetVolume(lpDesc->m_Volume);			

			alGetError();
			if (alGetError() != AL_NO_ERROR)
			{				
				return LYNX_FALSE;
			}

			if (desc->m_Name.empty())
				SetName(Filename);
			else
				SetName(desc->m_Name);

			if (m_bStreaming)
			{
				m_lpEngine->GetlpSoundSystem()->StartStreamingThread();
			}

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vIsPlaying() 
		{
			ALint State;

			alGetSourcei(m_Source, AL_SOURCE_STATE, &State);
			if (State == AL_PLAYING)
				return LYNX_TRUE;

			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMusic::vPlay(void)
		{
			if (m_bStreaming)
			{
				if (vIsPlaying())
					return LYNX_TRUE;
        
				if (!StreamOGG(m_Buffer[0]))
					return LYNX_FALSE;
 
				if (!StreamOGG(m_Buffer[1]))
					return LYNX_FALSE;
    
				alSourceQueueBuffers(m_Source, 2, m_Buffer);
				alSourcePlay(m_Source);

				m_ReadyForStreming = LYNX_TRUE;		
			}
			else
			{
				alSourcePlay(m_Source);
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vPause(void)
		{
			alSourcePause(m_Source);
			if (m_bStreaming)
			{
				m_ReadyForStreming = LYNX_FALSE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vStop(void)
		{
			alSourceStop(m_Source);
			if (m_bStreaming)
			{
				m_ReadyForStreming = LYNX_FALSE;
				alSourceUnqueueBuffers(m_Source, 2, m_Buffer);
				ov_time_seek(&m_OGGFile, 0.0f);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		float CMusic::vGetVolume()
		{
			//float Volume;

			//alGetSourcef(m_Source, AL_MAX_GAIN, &Volume);			

			return m_Volume;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vSetVolume(float v)
		{	
			m_Volume = v;
			float Volume = v * m_lpEngine->GetlpSoundSystem()->GetMusicVolumeMultiplier() * float(m_lpEngine->GetlpSoundSystem()->IsMusicEnabled()); 
			alSourcef(m_Source, AL_GAIN, Volume);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMusic::vSetLoops(int n)
		{
			if (n < 0 || n > 1)
			{
				if (m_bStreaming)
					alSourcei(m_Source, AL_LOOPING, AL_FALSE);
				else
					alSourcei(m_Source, AL_LOOPING, AL_TRUE);
				m_bLoop = LYNX_TRUE;
				m_LoopTimes = n;
			}
			else
			{
				alSourcei(m_Source, AL_LOOPING, AL_FALSE);
				m_bLoop = LYNX_FALSE;
			}
		}		
	}
}