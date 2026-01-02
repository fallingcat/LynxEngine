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

#ifndef __LYNXNULLVERTEXARRAY_H__
#define __LYNXNULLVERTEXARRAY_H__

#include <GraphicsSystem/LynxVertexArray.h>
#include <GraphicsSystem/LynxVertexLayout.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullVertexArray : public CVertexArray
		{
			LYNX_DECLARE_CLASS(CNullVertexArray);
		protected:		
			void*							m_lpBuffer;
		public:						
			CNullVertexArray(CGraphicsSystem* lpsys):CVertexArray(lpsys) {m_lpBuffer = NULL; };
			virtual ~CNullVertexArray() { if (m_lpBuffer) LYNXGLOBAL_DEL_ARRAY(m_lpBuffer); };
			
			LYNXINLINE CNullVertexArray&	operator =(const CNullVertexArray& t) {return (*this); };		

			LYNXBOOL						vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo = LYNX_TRUE, void* data = NULL) {m_lpBuffer = (BYTE*)LYNXALLOC(num*lpl->vGetStride()); return LYNX_TRUE; };
			LYNXINLINE void					vSet(int stream, int first) {};			
			LYNXINLINE void*				vMap(int start, int size, CRenderer::LOCKMODE flag) { return m_lpBuffer; };			
			LYNXINLINE void					vUnmap() {};			
			CString							vGetDesc() {return CString(""); };
		};
	}		
}

#endif