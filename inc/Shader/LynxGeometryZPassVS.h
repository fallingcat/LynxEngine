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

#ifndef __LYNXGEOMETRYZPASSVS_H__
#define __LYNXGEOMETRYZPASSVS_H__

#include <Shader/LynxGeometryVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CGeometryZPassVS : public CGeometryVS
		{	
			LYNX_DECLARE_CLASS(CGeometryZPassVS);
		public:						
		protected:		
			GraphicsSystem::CShaderConstant*	m_WorldViewProjM;
			GraphicsSystem::CShaderConstant*	m_WorldViewM;
			GraphicsSystem::CShaderConstant*	m_WorldM;
			GraphicsSystem::CShaderConstant*	m_LastWorldViewProjM;
		public:			
			CGeometryZPassVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CGeometryZPassVS();

			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif