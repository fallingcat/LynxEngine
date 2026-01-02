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

#ifndef __LYNXDOMAINSHADER_H__
#define __LYNXDOMAINSHADER_H__

#include <GraphicsSystem/LynxShader.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CDomainShader : public CShader
		{
			LYNX_DECLARE_CLASS(CDomainShader);
		protected:
		public:						
			CDomainShader(CGraphicsSystem* lpsys);
			virtual ~CDomainShader();										
		};
	}		
}

#endif