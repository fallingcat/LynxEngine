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

#ifndef __LYNXSIOSFACEBOOK_H__
#define __LYNXSIOSFACEBOOK_H__

#include <SocialSystem/LynxFacebook.h>
#import <FacebookSDK/FacebookSDK.h>

namespace LynxEngine 
{		
	namespace SocialSystem 
	{		
		namespace iOS 
		{
			class LYNXENGCLASS CFacebook : public LynxEngine::SocialSystem::CFacebook
			{	
				LYNX_DECLARE_CLASS(CFacebook);
			public:													
			protected:		
				FBSession*					m_FacebookSession;
			public:						
				CFacebook(CEngine* const lpengine);				
				virtual ~CFacebook();		

				LYNXBOOL					vLogIn(USERDATA& user);
				void						vLogOut();
				LYNXBOOL					vGetPersonalData(USERDATA& user);
				LYNXBOOL					vGetFriendData(CList<USERDATA>& friend_list);
				void						vPostNewsFeed();
				void						vInviteFriends(CList<USERDATA>* friend_list = NULL);
				void						vPostHiScore(unsigned int score);
				void						vPostAchievement();
			};
		}
	}
}
#endif