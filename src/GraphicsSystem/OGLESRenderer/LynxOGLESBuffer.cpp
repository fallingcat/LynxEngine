//##########################################################################
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
//  ?°?˙:
//
//###########################################################################

#include <LynxEngine_PCH.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLESRenderer/LynxOGLESRenderer.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESBuffer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  ?°?˙:   
		//-------------------------------------------------------------------------------------------------------
		COGLESBuffer::COGLESBuffer(CGraphicsSystem* lpsys)
		: CBuffer(lpsys) 		
		{				
			m_GLHandle = 0;
            m_Width = 0;
            m_Height = 0;
			m_BufferType = BT_RENDERBUFFER;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ?°?˙:   
		//-------------------------------------------------------------------------------------------------------
		COGLESBuffer::~COGLESBuffer()
		{				
			#ifdef __iOS__
				glDeleteRenderbuffersOES(1, &m_GLHandle); 
				m_GLHandle = 0;
			#endif
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ?°?˙:   If wnd == NULL means to create renderable buffer
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESBuffer::vCreate(LYNXWINDOW wnd, DWORD w, DWORD h, GraphicsSystem::CRenderer::FORMAT format, int num_ms, LYNXBOOL blockable) 
		{
            if (format == CRenderer::FORMAT_D16 || 
                format == CRenderer::FORMAT_D24X8 ||
                format == CRenderer::FORMAT_D24S8)
            {
                //glGenRenderbuffers(1, &m_GLHandle);
                //glBindRenderbuffer(GL_RENDERBUFFER, m_GLHandle);
                //glRenderbufferStorage(GL_RENDERBUFFER, COGLESRenderer::TranslateFormat(format), w, h);
            }
            m_Format = format; 
            m_Width = w;
            m_Height = h;
			
			return LYNX_TRUE;
		}		
	}
}

#endif

