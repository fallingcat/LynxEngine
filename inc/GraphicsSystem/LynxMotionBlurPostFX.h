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

#ifndef __LYNXBLOOMINGPOSTFX_H__
#define __LYNXBLOOMINGPOSTFX_H__

#include <GraphicsSystem/LynxPostProcessing.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		/**	
		*  @brief CMap is an object which can be used as texture and render targe.
		* 
		*  CMap is the object which can be used as texture and render target.
		*	
		*/
		class LYNXENGCLASS CMotionBlurPostFX : public CPostProcessing
		{	
			LYNX_DECLARE_CLASS(CMotionBlurPostFX);
		public:					
		public:			
			CMotionBlurPostFX(CGraphicsSystem* lpgs);
			virtual ~CMotionBlurPostFX();			

			LYNXBOOL							vCreate(CMap& output);
			void								vAddSourceMap(CMap& src);
			void								vRender(CCameraContainer* cam = NULL);					
		};	
	}
}

#endif