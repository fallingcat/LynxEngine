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

#ifndef __LYNXGEOMETRYGPASSVS_H__
#define __LYNXGEOMETRYGPASSVS_H__

#include <LynxEngineClasses.h>
#include <Shader/LynxGeometryVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CGeometryGPassVS : public CGeometryVS
		{	
			LYNX_DECLARE_CLASS(CGeometryGPassVS);
		public:						
		protected:					
			GraphicsSystem::CShaderConstant*	m_WorldViewProjM;
			GraphicsSystem::CShaderConstant*	m_WorldViewM;
			GraphicsSystem::CShaderConstant*	m_WorldM;			
		public:			
			CGeometryGPassVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CGeometryGPassVS();

			LYNXBOOL							vCreate();
			void								vCreateConstantAndResource(); 
			void								vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif