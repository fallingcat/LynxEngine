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

#ifndef __LYNXSHADERRESOURCE_H__
#define __LYNXSHADERRESOURCE_H__

#include <LynxUnknown.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CShaderResource : public CUnknown
		{	
			LYNX_DECLARE_CLASS(CShaderResource);
		public:						
			struct ResourceSlot {
				CAnsiString						Name;
				int								BindPoint;
				CResource*						lpResource;
			};
		protected:					
			CShader*							m_lpShader;			
			CAnsiString							m_Name;	
			int									m_BindPoint;
			CResource*							m_lpResource;
		public:			
			CShaderResource(CShader* lps, const char* name);
			CShaderResource(CShader* lps, const char* name, CResource* r);
			virtual ~CShaderResource();
			
			LYNXFORCEINLINE void				SetResource(CResource* const r) {m_lpResource = r; };
			LYNXFORCEINLINE void				Set() {Set(m_lpResource); };			
			void								Set(const CResource* r);			
		};		
	}
}

#endif