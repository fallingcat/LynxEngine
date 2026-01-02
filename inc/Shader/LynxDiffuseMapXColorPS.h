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

#ifndef __LYNXDIFFUSEMAPXCOLORPS_H__
#define __LYNXDIFFUSEMAPXCOLORPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CDiffuseMapXColorPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CDiffuseMapXColorPS);
		public:						
			float								m_ConstantColor[4];
			LYNXBOOL							m_bUseConstantColor;
		protected:								
			GraphicsSystem::CShaderConstant*	m_ConstantColor_Const;
		public:			
			CDiffuseMapXColorPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CDiffuseMapXColorPS();

			LYNXFORCEINLINE	void				SetConstantColor(float* c) {lynxMemCpy(m_ConstantColor, c, sizeof(float)*4); }; 
			LYNXFORCEINLINE	void				SetConstantColor(const LYNXCOLORRGBA& c) { m_ConstantColor[0] = c.Red / 255.0f; m_ConstantColor[1] = c.Green / 255.0f; m_ConstantColor[2] = c.Blue / 255.0f; m_ConstantColor[3] = c.Alpha / 255.0f; };
			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif