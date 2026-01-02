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

#ifndef __LYNXSHADERCONSTANTGROUP_H__
#define __LYNXSHADERCONSTANTGROUP_H__

#include <LynxUnknown.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CShaderConstantGroup : public CUnknown
		{	
			LYNX_DECLARE_CLASS(CShaderConstantGroup);
		public:							
		protected:		
			CRenderer::CONSTANTUPDATEFREQENCE	m_UpdateFrequence;
			CShader*							m_lpShader;	
			CAnsiString							m_Name;		
			LYNXBOOL							m_bDirty;
		public:			
			CShaderConstantGroup(CShader* lps, const char* name);
			virtual ~CShaderConstantGroup();			

			virtual void*						vMap() = 0;
			virtual void						vUnmap() = 0;	
			
			virtual void						vSetUpdateFrequence(const CRenderer::CONSTANTUPDATEFREQENCE f) = 0;	
			CRenderer::CONSTANTUPDATEFREQENCE	GetUpdateFrequence() {return m_UpdateFrequence; };	
			LYNXFORCEINLINE void				SetDirty(LYNXBOOL b) {m_bDirty = b; };
			LYNXFORCEINLINE LYNXBOOL			IsDirty() const {return m_bDirty; };
		};		
	}
}

#endif