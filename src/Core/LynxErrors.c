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

#include <LynxErrors.h>

//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxErrors 
*  @brief .
*
*	@param .
*	@param .  
*	@return . 
*/		            
//-------------------------------------------------------------------------------------------------------
void	lynxGetErrorString(char *str, LYNXERRORNO error)
{
	switch(error)
	{
		case ERROR_LYNX_SET_DISPLAYMODE:
			strcpy(str, "Unable to set correct display mode!");
			break;

		case ERROR_LYNX_NO_SELECTED_DISPLAYMODE:
			strcpy(str, "No such display mode you selected!");
			break;

		case ERROR_LYNX_INIT_3DSYSTEM:
			strcpy(str, "Unable to initialize 3D graphic system!");
			break;

		default:
			strcpy(str, "Unknow error!!");
			break;
	}
}