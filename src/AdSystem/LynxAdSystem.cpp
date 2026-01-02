//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <AdSystem/LynxAdSystem.h>
#include <AdSystem/LynxAd.h>
#ifdef __iOS__
	#include <Platform/iOS/LynxiOSiAd.h>
	#include <Platform/iOS/LynxiOSAdMob.h>    
#endif

namespace LynxEngine 
{	
	namespace AdSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
	#ifdef __WIN32__		
    #elif defined __iOS__
		//CInputDevice* CreateKeyboardDevice(CAdSystem* i)			{ return LYNXNEW CKeyboard(i); }
		//CInputDevice* CreateMouseDevice(CAdSystem* i)			{ return LYNXNEW CMouse(i); }		
		//CInputDevice* CreateTouchDevice(CAdSystem* i)			{ return LYNXNEW iOS::CTouch(i); }	
		//CInputDevice* CreateAccelerometerDevice(CAdSystem* i)	{ return LYNXNEW iOS::CAccelerometer(i); }	
		//CInputDevice* CreateMotionInputDevice(CAdSystem* i)		{ return LYNXNEW iOS::CMotionInput(i); }	
        //CInputDevice* CreateGyrometerDevice(CAdSystem* i)		{ return LYNXNEW iOS::CGyrometer(i); }
	#endif

		//IMPLEMENT_CLASSSCRIPT(CAdSystem, CSystem)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdSystem::CAdSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{				
			m_AdList.clear();

			//INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdSystem::~CAdSystem(void)
		{
			m_AdList.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAdSystem::vCreate() 
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAdSystem::vInit() 
		{	
			CSystem::vInit();

			#if defined(__MOBILE__)
				//RegisterDevice(TOUCH,			CreateTouchDevice);
				//RegisterDevice(ACCELEROMETER,	CreateAccelerometerDevice);
				//RegisterDevice(GYROMETER,		CreateGyrometerDevice);
				//RegisterDevice(MOTION_INPUT,	CreateMotionInputDevice);
			#elif defined(__DESKTOP__)				
			#endif		
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAd* CAdSystem::CreateAd(const CCreationDesc& desc)
		{
			CAdCreationDesc* Desc = (CAdCreationDesc*)(&desc);
			CAd* Ad = NULL;
			switch (Desc->m_AdType)
			{
				case AT_IAD:
					#if defined(__iOS__)
						Ad = LYNXNEW iOS::CiAd(this);
					#endif
					break;

				case AT_ADMOB:
					#if defined(__iOS__)
						//Ad = LYNXNEW iOS::CAdMob(this);
					#endif
					break;
			}
			if (Ad)
			{
				Ad->vCreate(*Desc);
				m_AdList.push_back(Ad);
			}

			return Ad;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdSystem::ReleaseAd(CAd* ad)
		{
			m_AdList.remove(ad);
			LYNXDEL ad;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdSystem::ReleaseAllAd()
		{
			CList<CAd*>::CIterator Ad;

			for (Ad = m_AdList.begin(); Ad != m_AdList.end(); ++Ad)			
				LYNXDEL (*Ad);

			m_AdList.clear();
		}
	}
}