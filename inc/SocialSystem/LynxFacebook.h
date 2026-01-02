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

#ifndef __LYNXSFACEBOOK_H__
#define __LYNXSFACEBOOK_H__

#include <LynxEngineClasses.h>
#include <LynxSystem.h>

namespace LynxEngine 
{		
	namespace SocialSystem 
	{		
		class LYNXENGCLASS CFacebook : public LynxEngine::CObj
		{	
			LYNX_DECLARE_CLASS(CFacebook);
		public:									
			typedef struct {
				unsigned long long				ID;
				CString							Name;
				CString							FirstName;
				CString							LastName;
				CString							Email;
				CString							ProfileImagePath;
			} USERDATA;
		protected:		
		public:						
			CFacebook(CEngine* const lpengine):LynxEngine::CObj(lpengine) {};				
			virtual ~CFacebook() {};		

			virtual LYNXBOOL					vLogIn(USERDATA& user) = 0;
			virtual void						vLogOut() = 0;
			virtual LYNXBOOL					vGetPersonalData(USERDATA& user) = 0;
			virtual LYNXBOOL					vGetFriendData(CList<USERDATA>& friend_list) = 0;
			virtual void						vPostNewsFeed() = 0;
			virtual void						vInviteFriends(CList<USERDATA>* friend_list = NULL) = 0;
			virtual void						vPostHiScore(unsigned int score) = 0;
			virtual void						vPostAchievement() = 0;
		};		
	}
}
#endif