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
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2VertexLayout.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2VertexShader.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2PixelShader.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2ShaderGroup.h>

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
		COGLES2ShaderGroup::COGLES2ShaderGroup(CGraphicsSystem *pgs)
		: CShaderGroup(pgs)
		{	
			m_Program = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2ShaderGroup::~COGLES2ShaderGroup(void)
		{
			LOCK_MUTEX

			if (m_Program)
			{
				GL_TRACE(glDeleteProgram(m_Program);)
			}
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));

			UNLOCK_MUTEX
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2ShaderGroup::vCreate(void)
		{
			LYNX_ASSERT(m_lpVertexShader && m_lpPixelShader);
			
            LYNX_LOG(glpLogger, CString("Create program (") + m_lpVertexShader->GetName() + CString(" + ") +  m_lpPixelShader->GetName() + CString(")\n"));
            
            LOCK_MUTEX
			GL_TRACE(m_Program = glCreateProgram();)
			GL_TRACE(glAttachShader(m_Program, ((COGLES2VertexShader*)(m_lpVertexShader->GetlpShader()))->GetGLHandle());)
			GL_TRACE(glAttachShader(m_Program, ((COGLES2PixelShader*)(m_lpPixelShader->GetlpShader()))->GetGLHandle());	)
            UNLOCK_MUTEX

            static_cast<COGLES2VertexLayout*>(&(*m_lpVertexLayout))->Bind(m_Program);
			
            LOCK_MUTEX
            GL_TRACE(glLinkProgram(m_Program);)
						
			#if defined(_DEBUG)
				GLint logLength;
				GL_TRACE(glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &logLength);)
				if (logLength > 0)
				{
					GLchar *log = (GLchar *)malloc(logLength);
					GL_TRACE(glGetProgramInfoLog(m_Program, logLength, &logLength, log);)
					LYNX_LOG(glpLogger, CString("Program link log:\n")+CString(log));
					free(log);
				}
			#endif
			
			GLint Status;
			GL_TRACE(glGetProgramiv(m_Program, GL_LINK_STATUS, &Status);)
			if (Status == 0)
			{
				LYNX_LOG(glpLogger, CString("Program link failed\n"));
                UNLOCK_MUTEX
				return LYNX_FALSE;
			}
            UNLOCK_MUTEX

			((COGLES2VertexShader*)(m_lpVertexShader->GetlpShader()))->SetlpShaderGroup(this);
			((COGLES2VertexShader*)(m_lpVertexShader->GetlpShader()))->SetProgram(m_Program);
			((COGLES2PixelShader*)(m_lpPixelShader->GetlpShader()))->SetlpShaderGroup(this);
			((COGLES2PixelShader*)(m_lpPixelShader->GetlpShader()))->SetProgram(m_Program);

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
		void COGLES2ShaderGroup::vSet(CContainer* lpo, CCameraContainer* cam)
		{			
			if (m_Program)
			{
				LOCK_MUTEX

				GL_TRACE(glUseProgram(m_Program);)         

				m_TextureChannel = 0;
				m_lpGraphicsSystem->GetlpRenderer()->SetVertexLayout(m_lpVertexLayout);
				m_lpVertexShader->vSet(lpo, cam);
				m_lpPixelShader->vSet(lpo, cam);

				UNLOCK_MUTEX
			}			
		}
	}
}

#endif