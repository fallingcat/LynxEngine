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

#ifndef __LYNXNULLINDEXARRAY_H__
#define __LYNXNULLINDEXARRAY_H__

#include <GraphicsSystem/LynxIndexArray.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullIndexArray : public CIndexArray
		{
			LYNX_DECLARE_CLASS(CNullIndexArray);
		protected:
			void*							m_lpBuffer;
		public:						
			CNullIndexArray(CGraphicsSystem* lpsys):CIndexArray(lpsys) {m_lpBuffer = NULL; };
			virtual ~CNullIndexArray() {};	
			
			LYNXINLINE CNullIndexArray&		operator =(const CNullIndexArray& t) {return (*this); };		

			LYNXBOOL						vCreate(int stride, int num, int flag, LYNXBOOL bbo = LYNX_TRUE, void* data = NULL) {m_lpBuffer = (BYTE*)LYNXALLOC(num*stride); return LYNX_TRUE; };
			LYNXINLINE void					vSet(int first) {};			
			LYNXINLINE void*				vMap(int start, int size, CRenderer::LOCKMODE flag) { return m_lpBuffer; };			
			LYNXINLINE void					vUnmap() {};			
		};
	}		
}

#endif