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

#ifndef __LYNXCLUSTECONTAINER_H__
#define __LYNXCLUSTECONTAINER_H__

#include <LynxEngineClasses.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	/**	
	*  @brief Reference container class。
	* 
	*  This class is used as a spatial reference of a 3D world. It would be rendered since is doen't present any physical object. 
	*	
	*/
	class LYNXENGCLASS CClusterContainer : public CContainer
	{		
		LYNX_DECLARE_CLASS(CClusterContainer);
	protected:								
	public:		
		CClusterContainer(CEngine *lpengine):CContainer(lpengine) {};
		virtual ~CClusterContainer() {};		

		virtual void						vSetSize(int i, const Math::CReal w, const Math::CReal h) = 0;
		virtual void						vGetSize(int i, Math::CReal& w, Math::CReal& h) = 0;
	};
}

#endif