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

#ifndef __LYNXVIDEO_H__
#define __LYNXVIDEO_H__

#include <LynxEngineClasses.h>
#include <LynxResource.h>

namespace LynxEngine 
{
	typedef enum {
		VUM_NONE = 0,
		VUM_DEFAULT,
		VUM_EMBEDDED,
		NUM_VIDEOUIMODE
	}VIDEOUIMODE;
	
	typedef enum {
		VSM_NONE = 0,
		VSM_ASPECT_FIT,
		VSM_ASPECT_FILL,
		VSM_FILL,
		NUM_VIDEOSCALINGMODE
	}VIDEOSCALINGMODE;
	
	class LYNXENGCLASS CVideoCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CVideoCreationDesc);	
	public:							
		VIDEOUIMODE					m_UIMode;
		VIDEOSCALINGMODE			m_ScalingMode;
		
		CVideoCreationDesc() {Default(); };
		void Default() {CCreationDesc::Default(); m_UIMode =  VUM_NONE; };
	};
	
	class LYNXENGCLASS CVideo : public CObj
	{
		LYNX_DECLARE_CLASS(CVideo);
	private:	
	protected:
		LYNXBOOL						m_bFinished;
	public:			
		CVideo(CEngine* lpeng):CObj(lpeng) {m_bFinished = LYNX_FALSE; };
		~CVideo() {};			

		LYNXFORCEINLINE bool			operator ==(const CString& name) const {return (m_Name == name); };		
		
		virtual LYNXBOOL				vIsPlaying() = 0;
		virtual LYNXBOOL				vCreate(const CCreationDesc* desc) = 0;	
		virtual LYNXBOOL				vPlay(void) = 0;	
		virtual void					vPause(void) = 0;	
		virtual void					vStop(void) = 0;	
		LYNXFORCEINLINE void			SetFinished(LYNXBOOL b) {m_bFinished = b; };
		LYNXFORCEINLINE LYNXBOOL		IsFinished() {return m_bFinished; };
	};		
	LYNXFORCEINLINE bool operator == (const CVideo* s, const CString& name) {return ((*s) == name); };
}

#endif