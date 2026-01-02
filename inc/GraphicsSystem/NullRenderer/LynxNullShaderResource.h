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

#ifndef __LYNXNULLSHADERRESOURCE_H__
#define __LYNXNULLSHADERRESOURCE_H__

#include <GraphicsSystem/LynxShaderResource.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullShaderResource : public CShaderResource
		{	
			LYNX_DECLARE_CLASS(CNullShaderResource);
		public:						
		protected:	
			int									m_Index;					
		public:			
			CNullShaderResource(CShader* lps, const char* name):CShaderResource(lps, name) {};
			CNullShaderResource(CShader* lps, const char* name, CResource* r):CShaderResource(lps, name, r) {};
			virtual ~CNullShaderResource() {};
		};		
	}
}

#endif