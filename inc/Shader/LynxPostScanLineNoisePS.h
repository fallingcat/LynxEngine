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

#ifndef __LYNXSCANLINEMOISEPS_H__
#define __LYNXSCANLINEMOISEPS_H__

#include <GraphicsSystem/LynxShaderClass.h>
#include <Shader/LynxDistortionPS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CPostScanLineNoisePS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CPostScanLineNoisePS);
		public:							
			float								m_Scale;
			float								m_Dir;
		protected:		
			GraphicsSystem::CShaderConstant*	m_Scale_Const;
		public:			
			CPostScanLineNoisePS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CPostScanLineNoisePS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif