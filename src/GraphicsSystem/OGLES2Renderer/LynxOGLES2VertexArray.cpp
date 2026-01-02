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
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2VertexArray.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2VertexLayout.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2VertexArray::COGLES2VertexArray(CGraphicsSystem *pgs)
		: CVertexArray(pgs)
		{	
			m_GLHandle = 0;
			m_VAOHandle = 0;
            m_lpBuffer = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2VertexArray::~COGLES2VertexArray(void)
		{
			LOCK_MUTEX

			if (m_GLHandle)
			{
				glDeleteBuffers(1, &m_GLHandle);
				m_GLHandle = 0;
			}
			if (m_lpBuffer)
			{
				LYNXDEL_ARRAY((LYNXBYTE*)m_lpBuffer);
				m_lpBuffer = NULL;
			}

			if (m_VAOHandle)
			{
				#ifdef __iOS__
					glDeleteVertexArraysOES(1, &m_VAOHandle);					
				#else			
					if (glBindVertexArrayOES)
						glDeleteVertexArraysOES(1, &m_VAOHandle);					
				#endif				
			}

			UNLOCK_MUTEX
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2VertexArray::vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo, void* data)
		{ 			
			LOCK_MUTEX

    		GLenum Usage = GL_STATIC_DRAW;		
			
			if (flag & CRenderer::DYNAMIC)
				Usage = GL_DYNAMIC_DRAW;				
			else if (flag & CRenderer::STATIC)
				Usage = GL_STATIC_DRAW;
			else if (flag & CRenderer::STAGING)
				Usage = GL_STREAM_DRAW;
			
            m_Stride = lpl->vGetStride();
			m_Size = m_Stride * num;
			m_lpLayout = lpl;
			
			#ifdef __iOS__
				// Create and bind the vertex array object.
				glGenVertexArraysOES(1, &m_VAOHandle);
				glBindVertexArrayOES(m_VAOHandle);    
			#else			
				if (glBindVertexArrayOES)
				{
					glGenVertexArraysOES(1, &m_VAOHandle);
					glBindVertexArrayOES(m_VAOHandle);    
				}			
			#endif
            
            glGenBuffers(1, &m_GLHandle);		
            glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);
                        
            int Offset = 0;//(first*m_Stride);            
            COGLES2VertexLayout* VLayout = (COGLES2VertexLayout*)(&(*m_lpLayout));
            for (int i=0; i<VLayout->GetNumSlots(); i++)
            {			
                glVertexAttribPointer(VLayout->GetAttribute(i).Attribute, VLayout->GetAttribute(i).Size, VLayout->GetAttribute(i).Type, GL_FALSE, m_Stride, (const GLvoid *)(Offset + VLayout->GetAttribute(i).Offset));
                glEnableVertexAttribArray(VLayout->GetAttribute(i).Attribute);
            }
              
            glBufferData(GL_ARRAY_BUFFER, lpl->vGetStride()*num, data, Usage);
            
            // Bind back to the default state.
            glBindBuffer(GL_ARRAY_BUFFER, 0);
			
            #ifdef __iOS__
				glBindVertexArrayOES(0);			
			#else			
				if (glBindVertexArrayOES)
					glBindVertexArrayOES(0);					
			#endif
            
			UNLOCK_MUTEX

			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2VertexArray::vSet(int stream, int first) 
		{
			LOCK_MUTEX

            #ifdef __iOS__
				glBindVertexArrayOES(m_VAOHandle);            
			#else
				if (glBindVertexArrayOES)
				{
					glBindVertexArrayOES(m_VAOHandle);            
				}
				else
				{				
                    glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);
					int Offset = 0;//(first*m_Stride);            
					COGLES2VertexLayout* VLayout = (COGLES2VertexLayout*)(&(*m_lpLayout));
					for (int i=0; i<VLayout->GetNumSlots(); i++)
					{			
						glVertexAttribPointer(VLayout->GetAttribute(i).Attribute, VLayout->GetAttribute(i).Size, VLayout->GetAttribute(i).Type, GL_FALSE, m_Stride, (const GLvoid *)(Offset + VLayout->GetAttribute(i).Offset));
						glEnableVertexAttribArray(VLayout->GetAttribute(i).Attribute);
					}
				}			
			#endif

			UNLOCK_MUTEX
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* COGLES2VertexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{			
			if (m_lpBuffer)
            {
				LYNXDEL_ARRAY((LYNXBYTE*)m_lpBuffer);
                m_lpBuffer = NULL;
            }

			m_lpBuffer = LYNXNEW LYNXBYTE[size];
			m_MappedStartOffset = start;
			m_MappedSize = size;
			
			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2VertexArray::vUnmap()
		{ 			
			LOCK_MUTEX

			glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);
			glBufferSubData(GL_ARRAY_BUFFER, m_MappedStartOffset, m_MappedSize, m_lpBuffer);
			
			if (m_lpBuffer)
            {
				LYNXDEL_ARRAY((LYNXBYTE*)m_lpBuffer);
                m_lpBuffer = NULL;
            }

			UNLOCK_MUTEX
		}
	}	
}

#endif