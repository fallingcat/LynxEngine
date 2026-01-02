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

#ifndef __LYNXPOSTTINTPS_H__
#define __LYNXPOSTTINTPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CPostTintPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CPostTintPS);
		public:						
			LYNXCOLORRGBA						m_TintColor;
		protected:					
			GraphicsSystem::CShaderConstant*	m_TintColor_Const;
		public:			
			CPostTintPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CPostTintPS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);

			void								SetParam();
		};		
	}		
}

#endif