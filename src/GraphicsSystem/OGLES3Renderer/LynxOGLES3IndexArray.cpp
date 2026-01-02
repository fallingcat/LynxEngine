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
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3IndexArray.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3IndexArray::COGLES3IndexArray(CGraphicsSystem *pgs)
		: CIndexArray(pgs)
		{
			m_GLHandle = 0;		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3IndexArray::~COGLES3IndexArray(void)
		{
			if (m_GLHandle)
			{
				glDeleteBuffers(1, &m_GLHandle);
				m_GLHandle = 0;
			}
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3IndexArray::vCreate(int stride, int num, int flag, LYNXBOOL bbo, void* data)
		{
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
			
			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3IndexArray::vSet(int first) 
		{ 
			GL_TRACE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLHandle);)
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* COGLES3IndexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{
			m_lpBuffer = LYNXCORE_NEW LYNXBYTE[size];
			m_MappedStartOffset = start;
			m_MappedSize = size;
			
			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3IndexArray::vUnmap()
		{ 
			GL_TRACE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLHandle);)
			GL_TRACE(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_MappedStartOffset, m_MappedSize, m_lpBuffer);)
			
			LYNXCORE_DEL_ARRAY(m_lpBuffer);
		}
	}	
}

#endif