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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9VertexShader.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9PixelShader.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9ShaderConstant.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9ShaderConstant::CD3D9ShaderConstant(CShader *lps, const char* name)	
		: CShaderConstant(lps, name)
		{		
			m_lpConstantTable = NULL;
			m_D3DHandle = NULL;

			switch (lps->GetResourceType())
			{
				case VERTEX_SHADER:
					m_lpConstantTable = ((CD3D9VertexShader*)lps)->GetlpConstantTable();
					break;
				case PIXEL_SHADER:
					m_lpConstantTable = ((CD3D9PixelShader*)lps)->GetlpConstantTable();
					break;
			}
			if (m_lpConstantTable)
				m_D3DHandle = m_lpConstantTable->GetConstantByName(NULL, name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9ShaderConstant::~CD3D9ShaderConstant(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetFloat(const float f) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloat(lpR->GetlpD3DDevice(), m_D3DHandle, f);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetFloat(const float *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetInt(const int i) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetInt(lpR->GetlpD3DDevice(), m_D3DHandle, i);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetInt(const int *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetInt2(const int* i) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), m_D3DHandle, i, 2);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetInt2(const int *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num*2);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetInt3(const int* i) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), m_D3DHandle, i, 3);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetInt3(const int *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num*3);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetInt4(const int* i) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), m_D3DHandle, i, 4);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetInt4(const int *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num*4);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetBool(const BOOL b) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetBool(lpR->GetlpD3DDevice(), m_D3DHandle, b);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetBool(const BOOL *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetBoolArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetMatrix(LPCLYNXMATRIX lpm) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, (float*)lpm, 16);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetVector2(const float* lpdata) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, 2);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetVector2(const float *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num*2);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetVector3(const float* lpdata) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, 3);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetVector3(const float *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num*3);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetVector4(const float* lpdata) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, 4);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetVector4(const float *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, lpdata, num*4);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9ShaderConstant::vSetMatrix(LPCLYNXMATRIX lpm, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer());
			if (m_D3DHandle)
				m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), m_D3DHandle, (float*)lpm, 16*num);

			INC_COUNTER(m_lpShader->GetlpGraphicsSystem()->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
	}
}
#endif