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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2VertexShader.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2PixelShader.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2ShaderConstant.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2ShaderConstant::COGLES2ShaderConstant(CShader *lps, const char* name)	
		: CShaderConstant(lps, name)
		{		
			LOCK_MUTEX

			COGLES2ShaderGroup* lpSG;
			
			switch (lps->GetResourceType())
			{
				case VERTEX_SHADER:
					lpSG = (COGLES2ShaderGroup*)(((COGLES2VertexShader*)lps)->GetlpShaderGroup());
					break;
					
				case PIXEL_SHADER:
					lpSG = (COGLES2ShaderGroup*)(((COGLES2PixelShader*)lps)->GetlpShaderGroup());
					break;
			}			
			m_Location = glGetUniformLocation(lpSG->GetProgram(), name);	
			#if defined(_DEBUG) || defined(DEBUG)
				if( m_Location < 0)
				{
					LYNX_LOG(glpLogger, CString("Shader Warning(")+lps->GetName()+CString("): ")+CString("Can't find constant : ")+CString(name));
				}				
			#endif

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2ShaderConstant::~COGLES2ShaderConstant(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetFloat(const float f) 
		{
			LOCK_MUTEX

			glUniform1f(m_Location, f);	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetFloat(const float *lpdata, int num) 
		{
			LOCK_MUTEX

			glUniform1fv(m_Location, num, lpdata);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetInt(const int i) 
		{
			LOCK_MUTEX

			glUniform1i(m_Location, i);	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetInt(const int *lpdata, int num) 
		{
			LOCK_MUTEX

			glUniform1iv(m_Location, num, lpdata);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetInt2(const int* i) 
		{
			LOCK_MUTEX

			glUniform2i(m_Location, (*i), *(i+1));	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetInt2(const int *lpdata, int num) 
		{
			LOCK_MUTEX

			glUniform2iv(m_Location, num, lpdata);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetInt3(const int* i) 
		{
			LOCK_MUTEX

			glUniform3i(m_Location, (*i), *(i+1), *(i+2));	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetInt3(const int *lpdata, int num) 
		{
			LOCK_MUTEX

			glUniform3iv(m_Location, num, lpdata);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetInt4(const int* i) 
		{
			LOCK_MUTEX

			glUniform4i(m_Location, (*i), *(i+1), *(i+2), *(i+3));	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetInt4(const int *lpdata, int num) 
		{
			LOCK_MUTEX

			glUniform4iv(m_Location, num, lpdata);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetBool(const BOOL b) 
		{
			LOCK_MUTEX

			glUniform1i(m_Location, b);	

			UNLOCK_MUTEX
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetBool(const BOOL *lpdata, int num) 
		{
			LOCK_MUTEX

			int* Bool = LYNXNEW int[num];
			
			for (int i=0; i<num; i++)
				Bool[i] = lpdata[i];
			
			glUniform1iv(m_Location, num, Bool);	

			LYNXDEL_ARRAY(Bool);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetVector2(const float* lpdata) 
		{
			LOCK_MUTEX

			glUniform2f(m_Location, lpdata[0], lpdata[1]);	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetVector2(const float *lpdata, int num) 
		{
			LOCK_MUTEX

			glUniform2fv(m_Location, num, lpdata);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetVector3(const float* lpdata) 
		{
			LOCK_MUTEX

			glUniform3f(m_Location, lpdata[0], lpdata[1], lpdata[2]);	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetVector3(const float *lpdata, int num) 
		{
			LOCK_MUTEX

			glUniform3fv(m_Location, num, lpdata);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetVector4(const float* lpdata) 
		{
			LOCK_MUTEX

			glUniform4f(m_Location, lpdata[0], lpdata[1], lpdata[2], lpdata[3]);	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetVector4(const float *lpdata, int num) 
		{
			LOCK_MUTEX

			glUniform4fv(m_Location, num, lpdata);

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetMatrix(LPCLYNXMATRIX lpm) 
		{		
			LOCK_MUTEX

			glUniformMatrix4fv(m_Location, 1, GL_FALSE, (const GLfloat*)lpm);			

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2ShaderConstant::vSetMatrix(LPCLYNXMATRIX lpm, int num) 
		{		
			LOCK_MUTEX

			glUniformMatrix4fv(m_Location, num, GL_FALSE, (const GLfloat*)lpm);			

			UNLOCK_MUTEX
		}
	}
}
#endif