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

#ifndef __LYNXVERTEXLAYOUT_H__
#define __LYNXVERTEXLAYOUT_H__

#include <GraphicsSystem/LynxRenderer.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CVertexLayout : public CResource
		{	
			LYNX_DECLARE_CLASS(CVertexLayout);
		public:			
		protected:		
			CGraphicsSystem*					m_lpGraphicsSystem;				
			CArray<CRenderer::VERTEXSLOT>		m_VertexSlots;					
		protected:
			void								Default();
		public:			
			CVertexLayout(CGraphicsSystem* lpgs);
			virtual ~CVertexLayout();

			LYNXFORCEINLINE int					GetNumSlots() const {return m_VertexSlots.size(); };
			void								AddSlot(BYTE stream, CRenderer::VERTEXSLOTTYPE	type, DWORD typeindex, CRenderer::VERTEXSLOTFORMAT format, DWORD number);
			int									ComputeStride();
			virtual LYNXBOOL					vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL) = 0;
			virtual LYNXBOOL					vCreate(const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vs = NULL) = 0;
			virtual DWORD						vGetStride() = 0;
			virtual void						vSet() = 0;
		};		
	}		
}

#endif