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
#include <LynxEngine.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexLayout.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9VertexLayout::CD3D9VertexLayout(CGraphicsSystem* pgs)
		: CVertexLayout(pgs)
		{
			m_Stride = 0;
			m_Layout = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9VertexLayout::~CD3D9VertexLayout()
		{			
			m_Layout->Release();
			m_Layout = NULL;
			m_Stride = 0;
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9VertexLayout::vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname, const LYNXCHAR* vsname)
		{
			for (int i=0; i<num; i++)
				AddSlot(vs[i].Stream, vs[i].Type, vs[i].TypeIndex, vs[i].Format, vs[i].Number);

			return (vCreate(vsname));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		D3DDECLTYPE CD3D9VertexLayout::TranslateVertexSlotFormat(int i, int* offset)
		{
			D3DDECLTYPE Type;

			switch (m_VertexSlots[i].Format)
			{
				case CRenderer::VSF_UBYTE:
					switch (m_VertexSlots[i].Number)
					{
						case 4:
							*offset += sizeof(LYNXBYTE)*4; 
							return D3DDECLTYPE_D3DCOLOR;
							break;
						default:
							//D3D doesn't support
							return D3DDECLTYPE_UNUSED;
							break;
					}	
					break;

				case CRenderer::VSF_SHORT:
					switch (m_VertexSlots[i].Number)
					{
						case 2:
							*offset += sizeof(LYNXSHORT)*2;
							return D3DDECLTYPE_SHORT2;
							break;
						case 4:
							*offset += sizeof(LYNXSHORT)*4;
							return D3DDECLTYPE_SHORT4;
							break;
						default:
							//D3D doesn't support
							return D3DDECLTYPE_UNUSED;
							break;
					}		
					break;

				case CRenderer::VSF_FLOAT:
					switch (m_VertexSlots[i].Number)
					{
						case 1:
							*offset += sizeof(LYNXFLOAT)*1;
							return D3DDECLTYPE_FLOAT1;
							break;
						case 2:
							*offset += sizeof(LYNXFLOAT)*2;
							return D3DDECLTYPE_FLOAT2;
							break;
						case 3:
							*offset += sizeof(LYNXFLOAT)*3;
							return D3DDECLTYPE_FLOAT3;
							break;						
						case 4:
							*offset += sizeof(LYNXFLOAT)*4;
							return D3DDECLTYPE_FLOAT4;
							break;
						default:
							//D3D doesn't support
							return D3DDECLTYPE_UNUSED;
							break;
					}				
					break;

				default:
					//D3D doesn't support
					return D3DDECLTYPE_UNUSED;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9VertexLayout::vCreate(const LYNXCHAR* layoutname, const LYNXCHAR* vsname)
		{		
			D3DVERTEXELEMENT9* lpDesc;
			lpDesc = LYNXGLOBAL_NEW D3DVERTEXELEMENT9[m_VertexSlots.size()+1];
			int Offset = 0;
			for (int i=0; i<m_VertexSlots.size(); i++)
			{
				lpDesc[i].Stream		= m_VertexSlots[i].Stream;
				lpDesc[i].Usage			= CD3D9Renderer::TranslateVertexSlotType(m_VertexSlots[i].Type);
				lpDesc[i].UsageIndex	= m_VertexSlots[i].TypeIndex;
				lpDesc[i].Offset		= Offset;
				lpDesc[i].Type			= TranslateVertexSlotFormat(i, &Offset);
				lpDesc[i].Method		= D3DDECLMETHOD_DEFAULT;
			}			 
			lpDesc[m_VertexSlots.size()].Stream = 0xff;
			lpDesc[m_VertexSlots.size()].Offset = 0;
			lpDesc[m_VertexSlots.size()].Type = D3DDECLTYPE_UNUSED;
			lpDesc[m_VertexSlots.size()].Method = 0;
			lpDesc[m_VertexSlots.size()].Usage = 0;
			lpDesc[m_VertexSlots.size()].UsageIndex = 0;
			m_Stride = Offset;

			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			if (FAILED(lpR->GetlpD3DDevice()->CreateVertexDeclaration(lpDesc, &m_Layout)))	
			{
				LYNXGLOBAL_DEL_ARRAY(lpDesc);
				return LYNX_FALSE;
			}

			LYNXGLOBAL_DEL_ARRAY(lpDesc);
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9VertexLayout::vSet()
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			lpR->GetlpD3DDevice()->SetVertexDeclaration(m_Layout);
		}
	}
}

#endif