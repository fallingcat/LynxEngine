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

#ifndef __LYNXSCREENDEPTHQUADVS_H__
#define __LYNXSCREENDEPTHQUADVS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CScreenDepthQuadVS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CScreenDepthQuadVS);
		public:						
			LYNXUVPOINT							m_UVOffset[4];
			LYNXVECTOR3D						m_FarCornerDir[4];
		protected:		
			GraphicsSystem::CShaderConstant*	m_UVOffset_Const;
			GraphicsSystem::CShaderConstant*	m_FarCornerDir_Const;
		public:			
			CScreenDepthQuadVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CScreenDepthQuadVS();

			LYNXFORCEINLINE	void				SetUVOffset(float* c) {lynxMemCpy(m_UVOffset, c, sizeof(float)*8); }; 
			LYNXFORCEINLINE	void				SetFarCornerDir(LYNXVECTOR3D* c) {lynxMemCpy(m_FarCornerDir, c, sizeof(LYNXVECTOR3D)*4); }; 
			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif