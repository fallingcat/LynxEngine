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

#ifndef __LYNXNULLVERTEXLAYOUT_H__
#define __LYNXNULLVERTEXLAYOUT_H__

#include <GraphicsSystem/LynxVertexLayout.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullVertexLayout : public CVertexLayout
		{	
			LYNX_DECLARE_CLASS(CNullVertexLayout);
		public:			
		protected:					
			int										m_Stride;
		public:			
			CNullVertexLayout(CGraphicsSystem* lpgs):CVertexLayout(lpgs) {};
			virtual ~CNullVertexLayout() {};

			LYNXBOOL								vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL)
			{
				for (int i=0; i<num; i++)
					AddSlot(vs[i].Stream, vs[i].Type, vs[i].TypeIndex, vs[i].Format, vs[i].Number);
				m_Stride = ComputeStride();

				return LYNX_TRUE;
			}
			LYNXBOOL								vCreate(const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL) {m_Stride = ComputeStride(); return LYNX_TRUE; };
			LYNXINLINE DWORD						vGetStride() {return m_Stride; };
			void									vSet() {};
		};		
	}		
}

#endif