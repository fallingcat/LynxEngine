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

#ifndef __LYNXSOUND_H__
#define __LYNXSOUND_H__

#include <LynxEngineClasses.h>
#include <LynxResource.h>
#ifdef __WIN32__
	#include <mmsystem.h>
#endif

enum WAVEFILETYPE {
	WF_EX  = 1,
	WF_EXT = 2
};

typedef struct {
	char			szRIFF[4];
	unsigned long	ulRIFFSize;
	char			szWAVE[4];
} WAVEFILEHEADER;

typedef struct {
	char			szChunkName[4];
	unsigned long	ulChunkSize;
} RIFFCHUNK;	

#ifdef __WIN32__
	typedef struct
	{
		unsigned short	usFormatTag;
		unsigned short	usChannels;
		unsigned long	ulSamplesPerSec;
		unsigned long	ulAvgBytesPerSec;
		unsigned short	usBlockAlign;
		unsigned short	usBitsPerSample;
		unsigned short	usSize;
		unsigned short  usReserved;
		unsigned long	ulChannelMask;
		GUID            guidSubFormat;
	}WAVEFMT;

	#ifndef __WINAPP__
		typedef struct {
			WAVEFORMATEX    Format;
			union {
				WORD		wValidBitsPerSample;       /* bits of precision  */
				WORD		wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
				WORD		wReserved;                 /* If neither applies, set to zero. */
			} Samples;
			DWORD           dwChannelMask;      /* which channels are present in stream  */
			GUID            SubFormat;
		}WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
	#endif
#else
	typedef struct waveformat_tag {
		WORD    wFormatTag;        /* format type */
		WORD    nChannels;         /* number of channels (i.e. mono, stereo, etc.) */
		DWORD   nSamplesPerSec;    /* sample rate */
		DWORD   nAvgBytesPerSec;   /* for buffer estimation */
		WORD    nBlockAlign;       /* block size of data */
	} WAVEFORMAT, *PWAVEFORMAT, *NPWAVEFORMAT, *LPWAVEFORMAT;

	typedef struct tWAVEFORMATEX {
		WORD        wFormatTag;         /* format type */
		WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
		DWORD       nSamplesPerSec;     /* sample rate */
		DWORD       nAvgBytesPerSec;    /* for buffer estimation */
		WORD        nBlockAlign;        /* block size of data */
		WORD        wBitsPerSample;     /* number of bits per sample of mono data */
		WORD        cbSize;             /* the count in bytes of the size of */
										/* extra information (after cbSize) */
	}WAVEFORMATEX, *PWAVEFORMATEX, *NPWAVEFORMATEX, *LPWAVEFORMATEX;

	typedef struct {
		unsigned short	usFormatTag;
		unsigned short	usChannels;
		unsigned long	ulSamplesPerSec;
		unsigned long	ulAvgBytesPerSec;
		unsigned short	usBlockAlign;
		unsigned short	usBitsPerSample;
		unsigned short	usSize;
		unsigned short  usReserved;
		unsigned long	ulChannelMask;
		LYNXGUID        guidSubFormat;
	}WAVEFMT;

	typedef struct {
		WAVEFORMATEX    Format;
		union {
			WORD		wValidBitsPerSample;       /* bits of precision  */
			WORD		wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
			WORD		wReserved;                 /* If neither applies, set to zero. */
		} Samples;
		DWORD           dwChannelMask;      /* which channels are present in stream  */
		LYNXGUID        SubFormat;
	}WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;		

	typedef struct pcmwaveformat_tag {
		WAVEFORMAT  wf;
		WORD        wBitsPerSample;
	} PCMWAVEFORMAT, *PPCMWAVEFORMAT, *NPPCMWAVEFORMAT, *LPPCMWAVEFORMAT;

	#define WAVE_FORMAT_PCM     1
#endif

namespace LynxEngine 
{	
	namespace SoundSystem 
	{
		class LYNXENGCLASS CSound : public CResource
		{
			LYNX_DECLARE_CLASS(CSound);
		public:
			SOUNDPRIORITY					m_Priority;
		private:	
		protected:
			int								m_NumLoops;
			float							m_Volume;
			LYNXBOOL						m_bPlayed;
		public:			
			CSound(CEngine* lpeng);
			~CSound();			

			DECLARE_CLASSSCRIPT

			LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			LYNXFORCEINLINE LYNXBOOL		Play() { return vPlay();}			
			LYNXFORCEINLINE void			Stop() { return vStop();}
			LYNXFORCEINLINE void			SetLoops(int n) { vSetLoops(n);};
			LYNXFORCEINLINE void			SetVolume(float v) { vSetVolume(v);};
			LYNXFORCEINLINE void			Pause() { return vPause();};
			LYNXFORCEINLINE LYNXBOOL		IsPlaying() {return vIsPlaying(); };
			LYNXFORCEINLINE LYNXBOOL		IsPlayed() {return m_bPlayed; };

			LYNXFORCEINLINE bool			operator ==(const CString& name) const {return (m_Name == name); };		
			
			virtual LYNXBOOL				vIsPlaying() = 0;
			virtual LYNXBOOL				vCreate(const CCreationDesc* desc) = 0;	
			virtual LYNXBOOL				vPlay(void) = 0;
			virtual	LYNXBOOL				vPlay(float pitch) = 0;				
			virtual void					vPause(void) = 0;	
			virtual void					vStop(void) = 0;
			virtual float					vGetVolume() = 0;
			virtual void					vSetVolume(float v) = 0;
			virtual void					vSetLoops(int n) = 0;
			
			virtual LYNXBOOL				vIsRecording() = 0;
			virtual LYNXBOOL				vOpenRecorded() = 0;
			virtual LYNXBOOL				vRecord(void) = 0;
			virtual void					vPauseRecording(void) = 0;
			virtual void					vStopRecording(void) = 0;

			void							AddToPlayingList(SOUNDPRIORITY priority = SP_MEDIUM);
		};	
	}
	LYNXFORCEINLINE bool operator == (const SoundSystem::CSound* s, const CString& name) {return ((*s) == name); };
	LYNXFORCEINLINE bool operator == (const SoundSystem::CSoundPtr& lhs, const SoundSystem::CSound* rhs) {return ((&(*lhs)) == rhs); };
}

#endif