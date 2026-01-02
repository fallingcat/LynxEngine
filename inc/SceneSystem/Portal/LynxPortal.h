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

#ifndef __LYNXPORTAL_H__
#define __LYNXPORTAL_H__

#include <LynxRenderableObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class CScene;		
		class LYNXENGCLASS CPortal : public CRenderableObj
		{
			LYNX_DECLARE_CLASS(CPortal);
		public:			
		protected:
			CScene*								m_lpScene;											
		public:						
		public:			
			CPortal();
			CPortal(CScene* const lpscene);				
			virtual ~CPortal();
			
			LYNXINLINE void						SetlpScene(CScene* const lps) {m_lpScene = lps; };									
			void								InitMembers();
		private:	
			
			friend class CScene;			
		};
	}
}

#endif
