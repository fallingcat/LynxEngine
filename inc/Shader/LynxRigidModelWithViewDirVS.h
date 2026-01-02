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

#ifndef __LYNXRIGIDMODELWITHVIEWDIRVS_H__
#define __LYNXRIGIDMODELWITHVIEWDIRVS_H__

#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxShaderClass.h>
#include <Shader/LynxRigidModelVS.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CRigidModelWithViewDirVS : public CRigidModelVS
		{	
			LYNX_DECLARE_CLASS(CRigidModelWithViewDirVS);
		public:						
		protected:	
			GraphicsSystem::CShaderConstant*	m_CameraPos_Const;
		public:			
			CRigidModelWithViewDirVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CRigidModelWithViewDirVS();

			virtual LYNXBOOL					vCreate();
			void								vCreateConstantAndResource(); 
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif