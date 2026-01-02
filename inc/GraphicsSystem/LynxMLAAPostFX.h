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

#ifndef __LYNXMLAAPOSTFX_H__
#define __LYNXMLAAPOSTFX_H__

#include <GraphicsSystem/LynxPostProcessing.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		/**	
		*  @brief CMLAAPostFx is a post-processing object which performance MLAA technique to a buffer.
		* 
		*  CMLAAPostFx is a post-processing object which performance MLAA technique to a buffer.
		*	
		*/
		class LYNXENGCLASS CMLAAPostFX : public CPostProcessing
		{	
			LYNX_DECLARE_CLASS(CMLAAPostFX);
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
			CMLAAPostFX(CGraphicsSystem* lpgs);
			virtual ~CMLAAPostFX();			

			LYNXBOOL							vCreate(CMap& output);
			void								vAddSourceMap(CMap& src);
			void								vRender(CCameraContainer* cam = NULL);					
		};	
	}
}

#endif