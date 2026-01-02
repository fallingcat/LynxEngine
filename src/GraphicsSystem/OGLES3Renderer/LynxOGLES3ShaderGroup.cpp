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
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexLayout.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexShader.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3PixelShader.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3ShaderGroup.h>

#ifdef __WIN32__
	typedef char					GLchar;
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3ShaderGroup::COGLES3ShaderGroup(CGraphicsSystem *pgs)
		: CShaderGroup(pgs)
		{	
			m_Program = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3ShaderGroup::~COGLES3ShaderGroup(void)
		{
			if (m_Program)
			{
				GL_TRACE(glDeleteProgram(m_Program);)
			}
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3ShaderGroup::vCreate(void)
		{
			LYNX_ASSERT(m_lpVertexShader && m_lpPixelShader);
			
            LYNX_LOG(glpLogger, CString("Create program (") + m_lpVertexShader->GetName() + CString(" + ") +  m_lpPixelShader->GetName() + CString(")\n"));
            
			GL_TRACE(m_Program = glCreateProgram();)
			GL_TRACE(glAttachShader(m_Program, ((COGLES3VertexShader*)(m_lpVertexShader->GetlpShader()))->GetGLHandle());)
			GL_TRACE(glAttachShader(m_Program, ((COGLES3PixelShader*)(m_lpPixelShader->GetlpShader()))->GetGLHandle());	)
			((COGLES3VertexLayout*)(m_lpVertexLayout))->Bind(m_Program);
			GL_TRACE(glLinkProgram(m_Program);)	
						
			#if defined(_DEBUG)
				GLint logLength;
				GL_TRACE(glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &logLength);)
				if (logLength > 0)
				{
					GLchar *log = (GLchar *)malloc(logLength);
					GL_TRACE(glGetProgramInfoLog(m_Program, logLength, &logLength, log);)
					LYNX_LOG(glpLogger, CString("Program link log:\n%s")+CString(log));
					free(log);
				}
			#endif
			
			GLint Status;
			GL_TRACE(glGetProgramiv(m_Program, GL_LINK_STATUS, &Status);)
			if (Status == 0)
			{
				LYNX_LOG(glpLogger, CString("Program link failed\n"));
				return LYNX_FALSE;
			}		

			((COGLES3VertexShader*)(m_lpVertexShader->GetlpShader()))->SetlpShaderGroup(this);
			((COGLES3VertexShader*)(m_lpVertexShader->GetlpShader()))->SetProgram(m_Program);
			((COGLES3PixelShader*)(m_lpPixelShader->GetlpShader()))->SetlpShaderGroup(this);
			((COGLES3PixelShader*)(m_lpPixelShader->GetlpShader()))->SetProgram(m_Program);

            // Constant location can be found only after program is created
			m_lpVertexShader->vCreateConstantAndResource();
			m_lpPixelShader->vCreateConstantAndResource();
						
			CShaderGroup::vCreate();

			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3ShaderGroup::vSet(CContainer* lpo, CCameraContainer* cam)
		{
			GL_TRACE(glUseProgram(m_Program);)
			
			m_TextureChannel = 0;
			m_lpGraphicsSystem->GetlpRenderer()->SetVertexLayout(m_lpVertexLayout);
			m_lpVertexShader->vSet(lpo, cam);
			m_lpPixelShader->vSet(lpo, cam);		
		}
	}
}

#endif