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

#ifndef __LYNXCONSTANTCOLORPS_H__
#define __LYNXCONSTANTCOLORPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CConstantColorPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CConstantColorPS);
		public:						
			float								m_ConstantColor[4];
		protected:								
			GraphicsSystem::CShaderConstant*	m_ConstantColor_Const;
		public:			
			CConstantColorPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CConstantColorPS();

			LYNXFORCEINLINE	void				SetConstantColor(float* c) {lynxMemCpy(m_ConstantColor, c, sizeof(float)*4); }; 
			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif