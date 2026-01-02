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
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexArray.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexLayout.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3VertexArray::COGLES3VertexArray(CGraphicsSystem *pgs)
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
		COGLES3VertexArray::~COGLES3VertexArray(void)
		{
			if (m_GLHandle)
			{
				glDeleteBuffers(1, &m_GLHandle);
				m_GLHandle = 0;
			}
			if (m_lpBuffer)
			{
				LYNXCORE_DEL_ARRAY((LYNXBYTE*)m_lpBuffer);
				m_lpBuffer = NULL;
			}

			if (m_VAOHandle)
			{
				#ifdef __iOS__
					glDeleteVertexArraysOES(1, &m_VAOHandle);					
				#else			
					glDeleteVertexArrays(1, &m_VAOHandle);					
				#endif
				
			}
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3VertexArray::vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo, void* data)
		{ 			
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
				glGenVertexArrays(1, &m_VAOHandle);
				glBindVertexArray(m_VAOHandle);    		
			#endif
            
            glGenBuffers(1, &m_GLHandle);		
            glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);
                        
            int Offset = 0;//(first*m_Stride);            
            COGLES3VertexLayout* VLayout = (COGLES3VertexLayout*)(&(*m_lpLayout));
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
				glBindVertexArray(0);					
			#endif
							
			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexArray::vSet(int stream, int first) 
		{ 
			#ifdef __iOS__
				glBindVertexArrayOES(m_VAOHandle);            
			#else
				glBindVertexArray(m_VAOHandle);            
				/*else
				{				
					int Offset = 0;//(first*m_Stride);            
					COGLES3VertexLayout* VLayout = (COGLES3VertexLayout*)(&(*m_lpLayout));
					for (int i=0; i<VLayout->GetNumSlots(); i++)
					{			
						glVertexAttribPointer(VLayout->GetAttribute(i).Attribute, VLayout->GetAttribute(i).Size, VLayout->GetAttribute(i).Type, GL_FALSE, m_Stride, (const GLvoid *)(Offset + VLayout->GetAttribute(i).Offset));
						glEnableVertexAttribArray(VLayout->GetAttribute(i).Attribute);
					}
				}	*/		
			#endif
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* COGLES3VertexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
		{
			if (m_lpBuffer)
            {
				LYNXCORE_DEL_ARRAY((LYNXBYTE*)m_lpBuffer);
                m_lpBuffer = NULL;
            }

			m_lpBuffer = LYNXCORE_NEW LYNXBYTE[size];
			m_MappedStartOffset = start;
			m_MappedSize = size;
			
			return m_lpBuffer;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexArray::vUnmap()
		{ 			
			glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);
			glBufferSubData(GL_ARRAY_BUFFER, m_MappedStartOffset, m_MappedSize, m_lpBuffer);
			
			if (m_lpBuffer)
            {
				LYNXCORE_DEL_ARRAY((LYNXBYTE*)m_lpBuffer);
                m_lpBuffer = NULL;
            }
		}
	}	
}

#endif