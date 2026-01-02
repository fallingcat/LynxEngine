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

#include <GraphicsSystem/OGLESRenderer/LynxOGLESVertexArray.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESVertexLayout.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESVertexArray::COGLESVertexArray(CGraphicsSystem *pgs)
		: CVertexArray(pgs)
		{	
			m_GLHandle = 0;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESVertexArray::~COGLESVertexArray(void)
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
		LYNXBOOL COGLESVertexArray::vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo, void* data)
		{ 
			GLenum Usage = GL_STATIC_DRAW;		
			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create()...\r\n")));

			if (bbo)
			{
				if (flag & CRenderer::DYNAMIC)
					Usage = GL_DYNAMIC_DRAW;
				
				if ((flag & CRenderer::WRITEONLY) || (flag & CRenderer::STATIC))
					Usage = GL_STATIC_DRAW;
					
				glGenBuffers(1, &m_GLHandle);		
				glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);
				glBufferData(GL_ARRAY_BUFFER, lpl->vGetStride()*num, data, Usage);			
			}
			else
			{
							
			}
			m_Stride = lpl->vGetStride();
			m_Size = m_Stride * num;
			m_lpLayout = lpl;

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create() OK\r\n")));
							
			return LYNX_TRUE; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESVertexArray::vSet(int stream, int first) 
		{ 
			int Offset = (first*m_Stride);
			glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);	
			
            COGLESVertexLayout* VLayout = (COGLESVertexLayout*)(&(*m_lpLayout));
            
			// Position
			if (VLayout->GetPositionAttribute())
			{
				glEnableClientState(VLayout->GetPositionAttribute()->Attribute);			
				glVertexPointer(VLayout->GetPositionAttribute()->Size, VLayout->GetPositionAttribute()->Type, m_Stride, (const GLvoid *)(Offset + VLayout->GetPositionAttribute()->Offset));
			}
			
			if (VLayout->GetTexCoordAttribute())
			{
				//Texture coord
				glEnableClientState(VLayout->GetTexCoordAttribute()->Attribute);
				glTexCoordPointer(VLayout->GetTexCoordAttribute()->Size, VLayout->GetTexCoordAttribute()->Type, m_Stride, (const GLvoid *)(Offset + VLayout->GetTexCoordAttribute()->Offset));				
			}	
			
			if (VLayout->GetNormalAttribute())
			{
				//Norml
				glEnableClientState(VLayout->GetNormalAttribute()->Attribute);
				glNormalPointer(VLayout->GetNormalAttribute()->Type, m_Stride, (const GLvoid *)(Offset + VLayout->GetNormalAttribute()->Offset));				
			}
			
			if (VLayout->GetColorAttribute())
			{
				//Color
				glEnableClientState(VLayout->GetColorAttribute()->Attribute);
				glColorPointer(VLayout->GetColorAttribute()->Size, VLayout->GetColorAttribute()->Type, m_Stride, (const GLvoid *)(Offset + VLayout->GetColorAttribute()->Offset));	
			}				
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void* COGLESVertexArray::vMap(int start, int size, CRenderer::LOCKMODE flag)
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
		void COGLESVertexArray::vUnmap()
		{ 			
			glBindBuffer(GL_ARRAY_BUFFER, m_GLHandle);
			glBufferSubData(GL_ARRAY_BUFFER, m_MappedStartOffset, m_MappedSize, m_lpBuffer);
			
			LYNXDEL_ARRAY(m_lpBuffer);
		}
	}	
}

#endif