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

#ifndef __LYNXDIRECTIONALBLURPS_H__
#define __LYNXDIRECTIONALBLURPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CDirectionalBlurPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CDirectionalBlurPS);
		public:				
			CVector2							m_BlurDir;
			float								m_Distance;
			float								m_Weight;			
		protected:					
			GraphicsSystem::CShaderConstant*	m_BlurDir_Const;			
		public:			
			CDirectionalBlurPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CDirectionalBlurPS();

			virtual LYNXBOOL					vCreate();
			virtual	void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif