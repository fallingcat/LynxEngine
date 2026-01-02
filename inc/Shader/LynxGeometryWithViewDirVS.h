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

#ifndef __LYNXGEOMETRYWITHVIEWDIRVS_H__
#define __LYNXGEOMETRYWITHVIEWDIRVS_H__

#include <GraphicsSystem/LynxShaderClass.h>
#include <GraphicsSystem/LynxShaderConstant.h>
#include <Shader/LynxGeometryVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CGeometryWithViewDirVS : public CGeometryVS
		{	
			LYNX_DECLARE_CLASS(CGeometryWithViewDirVS);
		public:						
		protected:		
			GraphicsSystem::CShaderConstant*	m_CameraPos_Const;
		public:			
			CGeometryWithViewDirVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CGeometryWithViewDirVS();

			LYNXBOOL							vCreate();	
			void								vCreateConstantAndResource(); 
			void								vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif