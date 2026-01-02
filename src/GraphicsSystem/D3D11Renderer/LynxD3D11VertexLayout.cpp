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
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexLayout.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11VertexLayout::CD3D11VertexLayout(CGraphicsSystem *pgs)
		: CVertexLayout(pgs)
		{
			m_Layout = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11VertexLayout::~CD3D11VertexLayout(void)
		{
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11VertexLayout::vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname, const LYNXCHAR* vsname) 
		{
			for (int i=0; i<num; i++)
				AddSlot(vs[i].Stream, vs[i].Type, vs[i].TypeIndex, vs[i].Format, vs[i].Number);
			return vCreate(layoutname, vsname);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LPCSTR CD3D11VertexLayout::TranslateSlotName(CRenderer::VERTEXSLOT& slot)
		{
			switch (slot.Type)
			{
				case CRenderer::VST_NULL:
					return slot.TypeName.c_str();
					break;
				default:	
					static LYNXANSICHAR Name[256];
					lynxAnsiStr(Name, GetEnumStr(slot.Type).c_str());
					return Name;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		DXGI_FORMAT CD3D11VertexLayout::TranslateSlotFormat(CRenderer::VERTEXSLOT& slot)
		{
			switch (slot.Format)
			{
				case CRenderer::VSF_NULL:
					return DXGI_FORMAT_UNKNOWN;
					break;
				case CRenderer::VSF_UBYTE:
					switch (slot.Number)
					{
						case 1:							
							return DXGI_FORMAT_R8_UINT;
							break;
						case 2:
							return DXGI_FORMAT_R8G8_UINT;
							break;
						case 3:
							return DXGI_FORMAT_R8G8B8A8_UINT;
							break;
						case 4:
							return DXGI_FORMAT_R8G8B8A8_UINT;
							break;						
					}
					break;
				case CRenderer::VSF_SHORT:
					switch (slot.Number)
					{
						case 1:
							return DXGI_FORMAT_R16_UINT;
							break;
						case 2:
							return DXGI_FORMAT_R16G16_UINT;
							break;
						case 3:
							return DXGI_FORMAT_R16G16B16A16_UINT;
							break;
						case 4:
							return DXGI_FORMAT_R16G16B16A16_UINT;
							break;						
					}
					break;				
				case CRenderer::VSF_FLOAT:
					switch (slot.Number)
					{
						case 1:
							return DXGI_FORMAT_R32_FLOAT;
							break;
						case 2:
							return DXGI_FORMAT_R32G32_FLOAT;
							break;
						case 3:
							return DXGI_FORMAT_R32G32B32_FLOAT;
							break;						
						case 4:
							return DXGI_FORMAT_R32G32B32A32_FLOAT;
							break;						
					}
					break;
				default:
					LYNX_ASSERT(0);
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CD3D11VertexLayout::GetSlotStride(CRenderer::VERTEXSLOT& slot)
		{
			switch (slot.Format)
			{
				default:
				case CRenderer::VSF_NULL:
					return 0;
					break;
				case CRenderer::VSF_UBYTE:
					return slot.Number;
					break;
				case CRenderer::VSF_SHORT:
					return (2*slot.Number);					
					break;
				case CRenderer::VSF_FLOAT:
					return (4*slot.Number);					
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11VertexLayout::vCreate(const LYNXCHAR* layoutname, const LYNXCHAR* vsname)
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			m_Stride = ComputeStride(); 

			int Offset = 0;
			D3D11_INPUT_ELEMENT_DESC* lpSlot = LYNXNEW D3D11_INPUT_ELEMENT_DESC[m_VertexSlots.size()];
			for (int i=0; i<m_VertexSlots.size(); i++)
			{
				lpSlot[i].SemanticName = LYNXNEW LYNXANSICHAR[256];
				strcpy((LYNXANSICHAR*)lpSlot[i].SemanticName, TranslateSlotName(m_VertexSlots[i]));
				lpSlot[i].SemanticIndex			= m_VertexSlots[i].TypeIndex;
				lpSlot[i].InputSlot				= m_VertexSlots[i].Stream;
				lpSlot[i].Format				= TranslateSlotFormat(m_VertexSlots[i]);
				lpSlot[i].AlignedByteOffset		= Offset;
				lpSlot[i].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
				lpSlot[i].InstanceDataStepRate	= 0;

				Offset += GetSlotStride(m_VertexSlots[i]);
			}

			CAnsiString ShaderSrc;

			if (vsname)
				ShaderSrc += CAnsiString("#include \"") + CAnsiString(vsname) + CAnsiString("\" \n");				
			else
				ShaderSrc += CAnsiString("#include \"") + CAnsiString("ShaderType.h") + CAnsiString("\" \n");

			if (layoutname)
				ShaderSrc += CAnsiString("float4 main(") + CAnsiString(layoutname) + CAnsiString(" input) : SV_POSITION  { return float4(0, 0, 0, 1); }");
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::[Error] "))+CString(_T("Can't create vertex layout because lack of <layoutname>!!\n")));
				return LYNX_FALSE;
			}

			lpR->m_ShaderIncludeHandler.ComputeCurrentShaderPath(m_lpGraphicsSystem->GetlpRenderer()->GetShaderSrcPath().c_str());

			HRESULT hResult = S_OK;
			ID3D10Blob* pBlob = NULL;			
			char Version[16];
			LYNXCHAR Target[16];
			LYNXCHAR Path[512];

			lynxStrCpy(Target, LynxEngine::GetEnumStr(m_lpGraphicsSystem->GetlpRenderer()->GetShaderTarget()).c_str());
			Version[0] = 0x00;	
			if (!lynxStriCmp(Target, _T("1_1")))
			{
				strcpy(Version, "vs_1_1");		
			}
			else if (!lynxStriCmp(Target, _T("2_0")))
			{
				strcpy(Version, "vs_2_0");		
			}
			else if (!lynxStriCmp(Target, _T("2_X")))
			{
				strcpy(Version, "vs_2_a");		
			}		
			else if (!lynxStriCmp(Target, _T("3_0")))
			{
				strcpy(Version, "vs_3_0");		
			}
			else if (!lynxStriCmp(Target, _T("4_0")))
			{
				strcpy(Version, "vs_4_0");		
			}
			else if (!lynxStriCmp(Target, _T("4_1")))
			{
				strcpy(Version, "vs_4_1");		
			}
			else if (!lynxStriCmp(Target, _T("5_0")))
			{
				strcpy(Version, "vs_5_0");		
			}			

			DWORD Flag = 0;
			Flag |= D3DCOMPILE_IEEE_STRICTNESS;
			Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
			
			int NumDefs = 1;
			NumDefs += m_lpGraphicsSystem->GetlpRenderer()->m_ShaderMacros.size();
			
			D3D_SHADER_MACRO* lpD3DXMacro = LYNXNEW D3D_SHADER_MACRO[NumDefs];
			NumDefs = 0;
			//Global shader macros
			for (int i=0; i<m_lpGraphicsSystem->GetlpRenderer()->m_ShaderMacros.size(); i++)
			{
				lpD3DXMacro[NumDefs].Name = m_lpGraphicsSystem->GetlpRenderer()->m_ShaderMacros[i].Name.c_str();
				lpD3DXMacro[NumDefs].Definition = m_lpGraphicsSystem->GetlpRenderer()->m_ShaderMacros[i].Definition.c_str();
				NumDefs++;
			}						
			lpD3DXMacro[NumDefs].Name = NULL;
			lpD3DXMacro[NumDefs].Definition = NULL;

			ID3D10Blob*	lpErrorMessage = NULL;
			HRESULT hRes = D3DCompile(ShaderSrc, ShaderSrc.size(), NULL, lpD3DXMacro, &(lpR->m_ShaderIncludeHandler), "main", Version, Flag, 0, &pBlob, &lpErrorMessage);
			if (hResult != S_OK || !pBlob)
			{
				if (lpErrorMessage)
				{
					char* ErrorMsg = (char *)lpErrorMessage->GetBufferPointer();					
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::[Error] "))+CString(ErrorMsg)+CString(_T("!!\n")));
				}
				return LYNX_FALSE;
			}
			LYNXDEL [] lpD3DXMacro;
			lpD3DXMacro = NULL;
			
			hResult = ((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateInputLayout(lpSlot, m_VertexSlots.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_Layout);			
			if (hResult!= S_OK)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::[Error] "))+CString(_T("Can't create vertex layout because validation failed!!\n")));
				return LYNX_FALSE;
			}

			pBlob->Release();
			for (int i=0; i<m_VertexSlots.size(); i++)
			{
				LYNXDEL [] lpSlot[i].SemanticName;
			}
			LYNXDEL lpSlot;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexLayout::vSet()
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			lpR->GetlpDeviceContext()->IASetInputLayout(m_Layout);
		}
	}
}

#endif