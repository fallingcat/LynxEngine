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

#ifndef __LYNXHALORADIALBLURPS_H__
#define __LYNXHALORADIALBLURPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CHaloRadialBlurPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CHaloRadialBlurPS);
		public:				
			LYNXUVPOINT							m_RadialCenter;
			float								m_Distance;
			float								m_Weight;
			float								m_HaloRadius;
		protected:					
			GraphicsSystem::CShaderConstant*	m_RadialCenter_Const;
			GraphicsSystem::CShaderConstant*	m_HaloRadius_Const;
		public:			
			CHaloRadialBlurPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CHaloRadialBlurPS();

			virtual LYNXBOOL					vCreate();
			virtual	void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif