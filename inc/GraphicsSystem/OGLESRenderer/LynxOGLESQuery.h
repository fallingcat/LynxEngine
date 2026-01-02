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

#ifndef __LYNXOGLESQUERY_H__
#define __LYNXOGLESQUERY_H__

#include <GraphicsSystem/LynxQuery.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLESQuery : public CQuery
		{
			LYNX_DECLARE_CLASS(COGLESQuery);
		protected:
		public:						
			COGLESQuery(CGraphicsSystem* lpgs):CQuery(lpgs) {};
			virtual ~COGLESQuery() {};				
			
			virtual LYNXBOOL				vIsValid() {return LYNX_TRUE; };						
			virtual void					vCreate(CRenderer::QUERYTYPE type) {};
			virtual void					vStart() {};			
			virtual void					vEnd() {};		
			virtual LYNXBOOL				vGetData(void* data, DWORD size, LYNXLONG wait_time) {return LYNX_TRUE; };		
		};
	}		
}

#endif
