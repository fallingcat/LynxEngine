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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexShader.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11PixelShader.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11ShaderConstant.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11ShaderConstant::CD3D11ShaderConstant(CShader *lps, const char* name)	
		: CShaderConstant(lps, name)
		{		
			m_lpReflection = NULL;
			m_lpConstantBuffer = NULL;

			switch (lps->GetResourceType())
			{
				case VERTEX_SHADER:
					m_lpReflection = ((CD3D11VertexShader*)lps)->GetlpShaderReflection();
					break;
				case PIXEL_SHADER:
					m_lpReflection = ((CD3D11PixelShader*)lps)->GetlpShaderReflection();
					break;
				case HULL_SHADER:
					break;
				case DOMAIN_SHADER:
					break;
				case GEOMETRY_SHADER:
					break;
				case COMPUTE_SHADER:
					break;
			}
			
			if (m_lpReflection)
			{
				D3D11_SHADER_DESC ShaderDesc;
				m_lpReflection->GetDesc(&ShaderDesc);
				for (int i=0; i<ShaderDesc.ConstantBuffers; i++)
				{
					ID3D11ShaderReflectionConstantBuffer* lpCBRef = m_lpReflection->GetConstantBufferByIndex(i);
					if (lpCBRef)
					{
						D3D11_SHADER_BUFFER_DESC CBDesc;
						lpCBRef->GetDesc(&CBDesc);
						ID3D11ShaderReflectionVariable* lpVarRef = lpCBRef->GetVariableByName(name);
						if (lpVarRef)
						{
							if (lpVarRef->GetDesc(&m_D3DShaderVarDesc) == S_OK)
							{							
								switch (lps->GetResourceType())
								{
									case VERTEX_SHADER:
										m_lpConstantBuffer = ((CD3D11VertexShader*)lps)->GetConstantBuffer(CBDesc.Name);
										break;
									case PIXEL_SHADER:
										m_lpConstantBuffer = ((CD3D11PixelShader*)lps)->GetConstantBuffer(CBDesc.Name);
										break;
									case HULL_SHADER:
										break;
									case DOMAIN_SHADER:
										break;
									case GEOMETRY_SHADER:
										break;
									case COMPUTE_SHADER:
										break;
								}
								m_lpReflection->GetResourceBindingDescByName(CBDesc.Name, &m_D3DShaderInputDesc);
								break;
							}
						}
					}
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11ShaderConstant::~CD3D11ShaderConstant(void)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* CD3D11ShaderConstant::vMap()
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			return m_lpConstantBuffer->vMap(CRenderer::LOCK_TO_WRITE);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vUnmap()
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			m_lpConstantBuffer->vUnmap();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetFloat(const float f) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);					

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			*lpC = f;

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetFloat(const float *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(float)*num);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetInt(const int i) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			int* lpC = (int*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			*lpC = i;

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetInt(const int *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			int* lpC = (int*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(int)*num);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetInt2(const int* i) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			int* lpC = (int*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, i, sizeof(int)*2);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetInt2(const int *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			int* lpC = (int*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(int)*num*2);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetInt3(const int* i) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			int* lpC = (int*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, i, sizeof(int)*3);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetInt3(const int *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			int* lpC = (int*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(int)*num*3);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetInt4(const int* i) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			int* lpC = (int*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, i, sizeof(int)*4);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetInt4(const int *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			int* lpC = (int*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(int)*num*4);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetBool(const BOOL b) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			BOOL* lpC = (BOOL*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			*lpC = b;

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetBool(const BOOL *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			BOOL* lpC = (BOOL*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(BOOL)*num);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetMatrix(LPCLYNXMATRIX lpm) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpm, sizeof(LYNXMATRIX));
			
			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetMatrix(LPCLYNXMATRIX lpm, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpm, sizeof(LYNXMATRIX)*num);			

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetVector2(const float* lpdata) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(float)*2);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetVector2(const float *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			for (int i=0; i<num; i++)
			{
				lynxMemCpy(lpC, lpdata, sizeof(float)*2);
				lpC += 4;
				lpdata += 2;
			}

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetVector3(const float* lpdata) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(float)*3);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetVector3(const float *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			for (int i=0; i<num; i++)
			{
				lynxMemCpy(lpC, lpdata, sizeof(float)*3);
				lpC += 4;
				lpdata += 3;
			}

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetVector4(const float* lpdata) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(float)*4);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11ShaderConstant::vSetVector4(const float *lpdata, int num) 
		{
			LYNX_ASSERT(m_lpConstantBuffer);

			float* lpC = (float*)((BYTE*)m_lpConstantBuffer->GetlpBuffer() + m_D3DShaderVarDesc.StartOffset);
			lynxMemCpy(lpC, lpdata, sizeof(float)*4*num);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}		
	}
}
#endif