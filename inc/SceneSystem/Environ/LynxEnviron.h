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

#ifndef __LYNXENVIRON_H__
#define __LYNXENVIRON_H__

#include <LynxGameObj.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{		
		class LYNXENGCLASS CEnviron : public CGameObj
		{	
			LYNX_DECLARE_CLASS(CEnviron);
		public:			
		protected:			
			CScene*										m_lpScene;
			CStaticObj*									m_lpSky;
			CList<CCloud*>								m_CloudList;			
		public:			
			CEnviron();			
			CEnviron(CScene* lps);
			virtual ~CEnviron();

			LYNXINLINE void								AddSky(CStaticObj* sky) {m_lpSky = sky; };
			LYNXINLINE CStaticObj*						GetlpSky() {return m_lpSky; };
		};
	}
}

#endif