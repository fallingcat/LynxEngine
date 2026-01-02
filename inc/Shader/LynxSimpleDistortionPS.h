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

#ifndef __LYNXSIMPLEDISTORTIONPS_H__
#define __LYNXSIMPLEDISTORTIONPS_H__

#include <GraphicsSystem/LynxShaderClass.h>
#include <Shader/LynxDistortionPS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CSimpleDistortionPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CSimpleDistortionPS);
		public:							
			struct DISTORTIONPARAM {
				float		ScreenSize[2];
				float		RefractionIndex;				
				float		RefractionOffset;
			};
			DISTORTIONPARAM						m_DistortionParam;
		protected:		
			GraphicsSystem::CShaderConstant*	m_DistortionParam_Const;
			GraphicsSystem::CShaderConstant*	m_ConstantColor_Const;
		public:			
			CSimpleDistortionPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CSimpleDistortionPS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif