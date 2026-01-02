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

#ifndef __LYNXSSAOPOSTFX_H__
#define __LYNXSSAOPOSTFX_H__

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
		class LYNXENGCLASS CSSAOPostFX : public CPostProcessing
		{	
			LYNX_DECLARE_CLASS(CSSAOPostFX);
		public:					
			DECLAR_PARAM_RANGE_INT(m_SSAOQuality)
			DECLAR_PARAM_BOOL(bDepthDownSampling)
			DECLAR_PARAM_BOOL(bSSAOPass)
			DECLAR_PARAM_FLOAT(m_SSAOSamplingKernel)
			DECLAR_PARAM_BOOL(m_bSSAOBlur)
			DECLAR_PARAM_FLOAT(m_SSAOBlurKernel)
			DECLAR_PARAM_FLOAT(m_SSAOBlurScale)				
			DECLAR_PARAM_FLOAT(m_SSAOBlurSharpness)
			DECLAR_PARAM_FLOAT(m_SSAOThreshold)
		protected:									
			
		public:			
			CSSAOPostFX(CGraphicsSystem* lpgs);
			virtual ~CSSAOPostFX();			

			LYNXBOOL							vCreate(CMap& output);
			void								vAddSourceMap(CMap& src);
			void								vRender(CCameraContainer* cam = NULL);					
		};	
	}
}

#endif