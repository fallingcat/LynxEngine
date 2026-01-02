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

#ifndef __LYNXANIMATIONFRAME_H__
#define __LYNXANIMATIONFRAME_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{	
	namespace Animation
	{
		/**	
		*  @brief Animation frame data。
		* 
		*  Frame data of the animation, including name, start frame, end frame, play mode data。
		*	
		*/
		class LYNXENGCLASS CAnimationFrame// : public CObj
		{
			LYNX_DECLARE_CLASS(CAnimationFrame);
		public:
			int									m_ID;
			LYNXCHAR							m_Name[64];		
			LYNXREAL							m_StartTime, m_EndTime;
			LYNXPLAYMODE						m_PlayMode;
			LYNXREAL							m_Interval;
			LYNXBOOL							m_bAlignToBoundaryPlane;
			LYNXBOOL							m_bHasOffset;
			LYNXBOOL							m_bResetWhenStop;			
		protected:			
			
		public:			
			CAnimationFrame();			
			~CAnimationFrame();					
		};	
	}
}
#endif