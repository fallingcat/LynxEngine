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

#ifndef __LYNXMOTIONBLURPOSTFX_H__
#define __LYNXMOTIONBLURPOSTFX_H__

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
		class LYNXENGCLASS CBloomingPostFX : public CPostProcessing
		{	
			LYNX_DECLARE_CLASS(CBloomingPostFX);
		public:					
			float								m_BloomingBlurKernel;
			float								m_BloomingBlurScale;
			CMap*								m_lpCurrentGlowMap;
		protected:									
			
		public:			
			CBloomingPostFX(CGraphicsSystem* lpgs);
			virtual ~CBloomingPostFX();			

			LYNXBOOL							vCreate(CMap& output);
			void								vAddSourceMap(CMap& src);
			void								vRender(CCameraContainer* cam = NULL);					
		};	
	}
}

#endif