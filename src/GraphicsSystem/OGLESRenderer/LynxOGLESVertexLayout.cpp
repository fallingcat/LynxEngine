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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLESRenderer/LynxOGLESRenderer.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESVertexLayout.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESVertexLayout::COGLESVertexLayout(CGraphicsSystem* pgs)
		: CVertexLayout(pgs)
		{
			m_Stride = 0;		
			m_PositionAttribute = NULL;
			m_TexCoordAttribute = NULL;
			m_ColorAttribute = NULL;
			m_NormalAttribute = NULL;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESVertexLayout::~COGLESVertexLayout()
		{			
			m_Stride = 0;
			LYNXDEL(m_PositionAttribute);
			LYNXDEL(m_TexCoordAttribute);
			LYNXDEL(m_ColorAttribute);
			LYNXDEL(m_NormalAttribute);
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESVertexLayout::vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname, const LYNXCHAR* vsname)
		{
			for (int i=0; i<num; i++)
				AddSlot(vs[i].Stream, vs[i].Type, vs[i].TypeIndex, vs[i].Format, vs[i].Number);

			return (vCreate());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESVertexLayout::GetVertexSlotOffset(int i, int* offset)
		{			
			switch (m_VertexSlots[i].Format)
			{
				case CRenderer::VSF_UBYTE:
					switch (m_VertexSlots[i].Number)
					{
						case 4:
							*offset += sizeof(LYNXBYTE)*4; 							
							break;
						default:
							//D3D doesn't support
							break;
					}	
					break;

				case CRenderer::VSF_SHORT:
					switch (m_VertexSlots[i].Number)
					{
						case 2:
							*offset += sizeof(LYNXSHORT)*2;							
							break;
						case 4:
							*offset += sizeof(LYNXSHORT)*4;							
							break;
						default:
							//D3D doesn't support
							break;
					}		
					break;

				case CRenderer::VSF_FLOAT:
					switch (m_VertexSlots[i].Number)
					{
						case 1:
							*offset += sizeof(LYNXFLOAT)*1;
							break;
						case 2:
							*offset += sizeof(LYNXFLOAT)*2;
							break;
						case 3:
							*offset += sizeof(LYNXFLOAT)*3;
							break;
						case 4:
							*offset += sizeof(LYNXFLOAT)*4;
							break;
					}				
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESVertexLayout::vCreate(const LYNXCHAR* layoutname, const LYNXCHAR* vsname)
		{	
			int Offset = 0;
			for (int i=0; i<m_VertexSlots.size(); i++)
			{	
				switch (m_VertexSlots[i].Type)
				{
					case CRenderer::VST_POSITION:
						if (!m_PositionAttribute)
						{
							m_PositionAttribute = LYNXNEW OGLESVertexAttribute;
							m_PositionAttribute->Attribute = COGLESRenderer::TranslateVertexSlotType(m_VertexSlots[i].Type);
							m_PositionAttribute->Type = COGLESRenderer::TranslateVertexSlotFormat(m_VertexSlots[i].Format);
							m_PositionAttribute->Size = m_VertexSlots[i].Number;						
							m_PositionAttribute->Offset = Offset;
						}
						break;
						
					case CRenderer::VST_TEX_COORD:
						if (!m_TexCoordAttribute)
						{
							m_TexCoordAttribute = LYNXNEW OGLESVertexAttribute;
							m_TexCoordAttribute->Attribute = COGLESRenderer::TranslateVertexSlotType(m_VertexSlots[i].Type);
							m_TexCoordAttribute->Type = COGLESRenderer::TranslateVertexSlotFormat(m_VertexSlots[i].Format);
							m_TexCoordAttribute->Size = m_VertexSlots[i].Number;
							m_TexCoordAttribute->Offset = Offset;
						}
						break;
						
					case CRenderer::VST_COLOR:	
						if (!m_ColorAttribute)
						{
							m_ColorAttribute = LYNXNEW OGLESVertexAttribute;
							m_ColorAttribute->Attribute = COGLESRenderer::TranslateVertexSlotType(m_VertexSlots[i].Type);
							m_ColorAttribute->Type = COGLESRenderer::TranslateVertexSlotFormat(m_VertexSlots[i].Format);
							m_ColorAttribute->Size = m_VertexSlots[i].Number;
							m_ColorAttribute->Offset = Offset;
						}
						break;
					
					case CRenderer::VST_NORMAL:	
						if (!m_NormalAttribute)
						{
							m_NormalAttribute = LYNXNEW OGLESVertexAttribute;
							m_NormalAttribute->Attribute = COGLESRenderer::TranslateVertexSlotType(m_VertexSlots[i].Type);
							m_NormalAttribute->Type = COGLESRenderer::TranslateVertexSlotFormat(m_VertexSlots[i].Format);
							m_NormalAttribute->Size = m_VertexSlots[i].Number;
							m_NormalAttribute->Offset = Offset;
						}
						break;				
						
				}				
				GetVertexSlotOffset(i, &Offset);
			}			
			m_Stride = Offset;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESVertexLayout::vSet()
		{			
		}
	}
}

#endif