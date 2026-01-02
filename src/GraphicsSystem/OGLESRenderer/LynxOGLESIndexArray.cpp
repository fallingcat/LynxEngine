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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLESRenderer/LynxOGLESIndexArray.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESIndexArray::COGLESIndexArray(CGraphicsSystem *pgs)
		: CIndexArray(pgs)
		{
		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESIndexArray::~COGLESIndexArray(void)
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
		LYNXBOOL COGLESIndexArray::vCreate(int stride, int num, int flag, LYNXBOOL bbo, void* data)
		{
			GLenum Usage = GL_STATIC_DRAW;
			
			if (bbo)
			{
				if (flag & CRenderer::DYNAMIC)
					Usage = GL_DYNAMIC_DRAW;
				
				if ((flag & CRenderer::WRITEONLY) || (flag & CRenderer::STATIC))
					Usage = GL_STATIC_DRAW;
				
				glGenBuffers(1, &m_GLHandle);		
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLHandle);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, stride*num, data, Usage);		
			}
			
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
		void COGLESIndexArray::vSet(int first) 
		{ 
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLHandle);			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* COGLESIndexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{
			m_lpBuffer = LYNXNEW LYNXBYTE[size];
			m_MappedStartOffset = start;
			m_MappedSize = size;
			
			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESIndexArray::vUnmap()
		{ 
			glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);
			glBufferSubData(GL_ARRAY_BUFFER, m_MappedStartOffset, m_MappedSize, m_lpBuffer);
			
			LYNXDEL_ARRAY(m_lpBuffer);
		}
	}	
}

#endif