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

#ifndef __LYNXIOSVIDEO_H__
#define __LYNXIOSVIDEO_H__

#include <LynxEngineClasses.h>
#include <video/LynxVideo.h>

LYNX_FORWARD_DECLARATION(MPMoviePlayerController)
LYNX_FORWARD_DECLARATION(VideoNotificationHandler)

namespace LynxEngine 
{
	namespace iOS
	{
		class LYNXENGCLASS CVideo : public LynxEngine::CVideo
		{
			LYNX_DECLARE_CLASS(CVideo);
		private:	
		protected:
			MPMoviePlayerController*		m_MoviePlayer;
			VideoNotificationHandler*		m_NotificationHandler;	
		public:			
			CVideo(LynxEngine::CEngine* lpeng);
			~CVideo();			

			LYNXFORCEINLINE bool			operator ==(const CString& name) const {return (m_Name == name); };		
			
			virtual LYNXBOOL				vIsPlaying();
			virtual LYNXBOOL				vCreate(const CCreationDesc* desc);	
			virtual LYNXBOOL				vPlay(void);	
			virtual void					vPause(void);	
			virtual void					vStop(void);		
		};		
	}
	LYNXFORCEINLINE bool operator == (const iOS::CVideo* s, const CString& name) {return ((*s) == name); };
}

#endif