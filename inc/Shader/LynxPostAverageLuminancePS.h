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

#ifndef __LYNXPOSTAVERAGELUMINANCEPS_H__
#define __LYNXPOSTAVERAGELUMINANCEPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CPostAverageLuminancePS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CPostAverageLuminancePS);
		public:						
			LYNXUVPOINT							m_SampleOffset[16];
		protected:					
			GraphicsSystem::CShaderConstantGroup* m_Param;
			GraphicsSystem::CShaderConstant*	m_Offset_Const;
		public:			
			CPostAverageLuminancePS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CPostAverageLuminancePS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);

			void								SetParam();
		};		
	}		
}

#endif