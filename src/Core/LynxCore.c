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
//  Created by Owen Wu : 2003/06/22
//  Last Update : 
//
//###########################################################################

#include <LynxCore.h>
#include <LynxMath.h>

#ifdef __NON_COMMERCIAL__
	static LYNXLICENSEVERSIONCODE							gLicenseVersion = LYNX_VER_NON_COMMERCIAL;
#else
	static LYNXLICENSEVERSIONCODE							gLicenseVersion = LYNX_VER_COMMERCIAL;
#endif

//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 LYNXLICENSEVERSIONCODE	lynxGetLicenseVersion(void)
{
	return gLicenseVersion;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxInitCore(void)
{	  	
	lynxInitMathSystem();
	lynxInitOS();
	lynxInitIOSystem();
	lynxInitFileIOSystem();	
	lynxInitMemSystem();
	lynxMungeFPCW();	
	lynxInitCRC();

	return LYNX_TRUE;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxReleaseCore(void)
{		
	lynxRestoreFPCW();		

	return LYNX_TRUE;
}
