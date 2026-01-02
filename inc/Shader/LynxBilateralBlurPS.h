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

#ifndef __LYNXBILATERALBLURPS_H__
#define __LYNXBILATERALBLURPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CBilateralBlurPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CBilateralBlurPS);
		public:						
			float								m_BlurParam[4];
			LYNXUVPOINT							m_UVOffsetDir;
		protected:								
			GraphicsSystem::CShaderConstant*	m_BlurParam_Const;
			GraphicsSystem::CShaderConstant*	m_UVOffsetDir_Const;			
		public:			
			CBilateralBlurPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CBilateralBlurPS();

			LYNXFORCEINLINE void				SetBlurRadius(float r) {m_BlurParam[0] = r; };
			LYNXFORCEINLINE void				SetBlurFalloff(float f) {m_BlurParam[1] = f; };
			LYNXFORCEINLINE void				SetBlurSharpness(float s) {m_BlurParam[2] = s; };

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif