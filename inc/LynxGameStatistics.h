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

#ifndef __LYNXGAMESTATISTICS_H__
#define __LYNXGAMESTATISTICS_H__

#include <LynxType.h>
#include <LynxObj.h>
#include <LynxString.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CGameStatistics : public CObj
	{	
		LYNX_DECLARE_CLASS(CGameStatistics);
	public:		
	private:		
	protected:					
	public:	
		CGameStatistics(CEngine* lpeng);
		virtual ~CGameStatistics();		

		virtual LYNXBOOL					vInit() {};
		virtual void						vDownload() = 0;
		virtual void						vUpload() = 0;
	};		
}

#endif