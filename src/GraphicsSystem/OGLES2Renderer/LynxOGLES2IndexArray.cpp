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
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2IndexArray.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2IndexArray::COGLES2IndexArray(CGraphicsSystem *pgs)
		: CIndexArray(pgs)
		{
			m_GLHandle = 0;		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2IndexArray::~COGLES2IndexArray(void)
		{
			LOCK_MUTEX

			if (m_GLHandle)
			{
				glDeleteBuffers(1, &m_GLHandle);
				m_GLHandle = 0;
			}

			UNLOCK_MUTEX
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2IndexArray::vCreate(int stride, int num, int flag, LYNXBOOL bbo, void* data)
		{
			LOCK_MUTEX

			GLenum Usage = GL_STATIC_DRAW;
			
			if (flag & CRenderer::DYNAMIC)
				Usage = GL_DYNAMIC_DRAW;				
			else if (flag & CRenderer::STATIC)
				Usage = GL_STATIC_DRAW;
			else if (flag & CRenderer::STAGING)
				Usage = GL_STREAM_DRAW;
			
			GL_TRACE(glGenBuffers(1, &m_GLHandle);)		
			GL_TRACE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLHandle);)
			GL_TRACE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, stride*num, data, Usage);)		
			
			m_Stride = stride;
			m_Size = stride * num;
			
			switch (m_Stride)
			{
				case (sizeof(GLubyte)):
					m_IndexType = GL_UNSIGNED_BYTE;
					break;
				case (sizeof(GLushort)):
					m_IndexType = GL_UNSIGNED_SHORT;
					break;
                default:
					LYNX_ASSERT(0);
					break;
			}	

			UNLOCK_MUTEX
			
			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2IndexArray::vSet(int first) 
		{ 
			LOCK_MUTEX

			GL_TRACE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLHandle);)

			UNLOCK_MUTEX
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* COGLES2IndexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{
			m_lpBuffer = LYNXALLOC(size);
			m_MappedStartOffset = start;
			m_MappedSize = size;
			
			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2IndexArray::vUnmap()
		{ 
			LOCK_MUTEX

			GL_TRACE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLHandle);)
			GL_TRACE(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_MappedStartOffset, m_MappedSize, m_lpBuffer);)
			
			LYNXFREE(m_lpBuffer);

			UNLOCK_MUTEX
		}
	}	
}

#endif