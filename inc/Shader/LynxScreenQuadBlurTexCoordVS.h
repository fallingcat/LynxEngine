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

#ifndef __LYNXSCREENQUADBLURTEXCOORDVS_H__
#define __LYNXSCREENQUADBLURTEXCOORDVS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CScreenQuadBlurTexCoordVS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CScreenQuadBlurTexCoordVS);
		public:			
			LYNXUVPOINT							m_BlurUVOffset[4];
			LYNXUVPOINT							m_UVOffset[4];
		protected:					
			GraphicsSystem::CShaderConstant*	m_BlurUVOffset_Const;
			GraphicsSystem::CShaderConstant*	m_UVOffset_Const;
		public:			
			CScreenQuadBlurTexCoordVS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CScreenQuadBlurTexCoordVS();

			LYNXFORCEINLINE	void				SetUVOffset(float* c) {lynxMemCpy(m_UVOffset, c, sizeof(float)*8); }; 
			LYNXFORCEINLINE	void				SetBlurUVOffset(float* c) {lynxMemCpy(m_BlurUVOffset, c, sizeof(float)*8); }; 
			virtual LYNXBOOL					vCreate();
			virtual void						vCreateConstantAndResource();
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		};		
	}		
}

#endif