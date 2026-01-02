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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Renderer.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexShader.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3PixelShader.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3ShaderConstant.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3ShaderConstant::COGLES3ShaderConstant(CShader *lps, const char* name)	
		: CShaderConstant(lps, name)
		{		
			COGLES3ShaderGroup* lpSG;
			
			switch (lps->GetResourceType())
			{
				case VERTEX_SHADER:
					lpSG = (COGLES3ShaderGroup*)(((COGLES3VertexShader*)lps)->GetlpShaderGroup());
					break;
					
				case PIXEL_SHADER:
					lpSG = (COGLES3ShaderGroup*)(((COGLES3PixelShader*)lps)->GetlpShaderGroup());
					break;
			}			
			m_Location = glGetUniformLocation(lpSG->GetProgram(), name);	
			#if defined(_DEBUG) || defined(DEBUG)
				if( m_Location < 0)
				{
					LYNX_LOG(glpLogger, CString("Shader Warning(")+lps->GetName()+CString("): ")+CString("Can't find constant : ")+CString(name));
				}				
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3ShaderConstant::~COGLES3ShaderConstant(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetFloat(const float f) 
		{
			glUniform1f(m_Location, f);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetFloat(const float *lpdata, int num) 
		{
			glUniform1fv(m_Location, num, lpdata);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetInt(const int i) 
		{
			glUniform1i(m_Location, i);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetInt(const int *lpdata, int num) 
		{
			glUniform1iv(m_Location, num, lpdata);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetInt2(const int* i) 
		{
			glUniform2i(m_Location, (*i), *(i+1));	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetInt2(const int *lpdata, int num) 
		{
			glUniform2iv(m_Location, num, lpdata);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetInt3(const int* i) 
		{
			glUniform3i(m_Location, (*i), *(i+1), *(i+2));	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetInt3(const int *lpdata, int num) 
		{
			glUniform3iv(m_Location, num, lpdata);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetInt4(const int* i) 
		{
			glUniform4i(m_Location, (*i), *(i+1), *(i+2), *(i+3));	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetInt4(const int *lpdata, int num) 
		{
			glUniform4iv(m_Location, num, lpdata);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetBool(const BOOL b) 
		{
			glUniform1i(m_Location, b);	
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetBool(const BOOL *lpdata, int num) 
		{
			int* Bool = LYNXCORE_NEW int[num];
			
			for (int i=0; i<num; i++)
				Bool[i] = lpdata[i];
			
			glUniform1iv(m_Location, num, Bool);	

			LYNXCORE_DEL_ARRAY(Bool);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetVector2(const float* lpdata) 
		{
			glUniform2f(m_Location, lpdata[0], lpdata[1]);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetVector2(const float *lpdata, int num) 
		{
			glUniform2fv(m_Location, num, lpdata);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetVector3(const float* lpdata) 
		{
			glUniform3f(m_Location, lpdata[0], lpdata[1], lpdata[2]);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetVector3(const float *lpdata, int num) 
		{
			glUniform3fv(m_Location, num, lpdata);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetVector4(const float* lpdata) 
		{
			glUniform4f(m_Location, lpdata[0], lpdata[1], lpdata[2], lpdata[3]);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetVector4(const float *lpdata, int num) 
		{
			glUniform4fv(m_Location, num, lpdata);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetMatrix(LPCLYNXMATRIX lpm) 
		{			
			glUniformMatrix4fv(m_Location, 1, GL_FALSE, (const GLfloat*)lpm);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderConstant::vSetMatrix(LPCLYNXMATRIX lpm, int num) 
		{			
			glUniformMatrix4fv(m_Location, num, GL_FALSE, (const GLfloat*)lpm);			
		}
	}
}
#endif