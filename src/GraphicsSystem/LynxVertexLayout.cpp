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

#include <LynxEngine_PCH.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVertexLayout::Default(void)
		{				
			m_ResourceType = VERTEX_LAYOUT; 	
			m_VertexSlots.clear();
			//MakeName();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVertexLayout::CVertexLayout(CGraphicsSystem* pgs)
		: CResource(pgs->GetlpEngine())
		{
			m_lpGraphicsSystem = pgs;
			Default();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVertexLayout::~CVertexLayout()
		{		
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
			m_VertexSlots.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CVertexLayout::AddSlot(BYTE stream, CRenderer::VERTEXSLOTTYPE type, DWORD typeindex, CRenderer::VERTEXSLOTFORMAT format, DWORD number)
		{
			CRenderer::VERTEXSLOT Slot;

			Slot.Stream		= stream;
			Slot.Type		= type;
			Slot.TypeIndex	= typeindex;
			Slot.Format		= format;
			Slot.Number		= number;
			m_VertexSlots.push_back(Slot);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CVertexLayout::ComputeStride()
		{			
			int Offset = 0;
			for (int i=0; i<m_VertexSlots.size(); ++i)
			{
				switch (m_VertexSlots[i].Format)
				{
					case CRenderer::VSF_UBYTE:
						Offset += sizeof(LYNXBYTE)*m_VertexSlots[i].Number;
						break;
					case CRenderer::VSF_SHORT:
						Offset += sizeof(LYNXSHORT)*m_VertexSlots[i].Number;
						break;
					case CRenderer::VSF_FLOAT:
						Offset += sizeof(LYNXFLOAT)*m_VertexSlots[i].Number;
						break;
				}
			}
			return Offset;
		}
	}
}