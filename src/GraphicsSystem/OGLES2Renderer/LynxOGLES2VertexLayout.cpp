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

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		const LYNXANSICHAR* COGLES2VertexLayout::TranslateSlotName(CRenderer::VERTEXSLOT& slot)
		{
			switch (slot.Type)
			{
				case CRenderer::VST_NULL:
					return slot.TypeName.c_str();
					break;
				default:	
					static LYNXANSICHAR Name[256];
					lynxAnsiStr(Name, GetEnumStr(slot.Type).c_str());
					if (GetEnumStr(CRenderer::VST_NULL) == CString(Name))
					{
						return slot.TypeName.c_str();
					}
					else 
					{
						return Name;
					}
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2VertexLayout::COGLES2VertexLayout(CGraphicsSystem* pgs)
		: CVertexLayout(pgs)
		{
			m_Stride = 0;		
			m_AttributeArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2VertexLayout::~COGLES2VertexLayout()
		{			
			m_Stride = 0;
			m_AttributeArray.clear();
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2VertexLayout::vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname, const LYNXCHAR* vsname)
		{
			for (int i=0; i<num; i++)
				AddSlot(vs[i].Stream, vs[i].Type, vs[i].TypeIndex, vs[i].Format, vs[i].Number);

			return (vCreate());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2VertexLayout::GetVertexSlotOffset(int i, int* offset)
		{			
			switch (m_VertexSlots[i].Format)
			{
				case CRenderer::VSF_UBYTE:
					switch (m_VertexSlots[i].Number)
					{
						case 4:
							*offset += sizeof(LYNXBYTE)*4; 							
							break;
						default:
							//D3D doesn't support
							break;
					}	
					break;

				case CRenderer::VSF_SHORT:
					switch (m_VertexSlots[i].Number)
					{
						case 2:
							*offset += sizeof(LYNXSHORT)*2;							
							break;
						case 4:
							*offset += sizeof(LYNXSHORT)*4;							
							break;
						default:
							//D3D doesn't support
							break;
					}		
					break;

				case CRenderer::VSF_FLOAT:
					switch (m_VertexSlots[i].Number)
					{
						case 1:
							*offset += sizeof(LYNXFLOAT)*1;
							break;
						case 2:
							*offset += sizeof(LYNXFLOAT)*2;
							break;
						case 3:
							*offset += sizeof(LYNXFLOAT)*3;
							break;
						case 4:
							*offset += sizeof(LYNXFLOAT)*4;
							break;
					}				
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2VertexLayout::vCreate(const LYNXCHAR* layoutname, const LYNXCHAR* vsname)
		{	
			int Offset = 0;
			for (int i=0; i<m_VertexSlots.size(); i++)
			{	
				OGLES2VertexAttribute Attrib;			
				Attrib.Attribute = i;
				Attrib.Type = COGLES2Renderer::TranslateVertexSlotFormat(m_VertexSlots[i].Format);
				Attrib.Size = m_VertexSlots[i].Number;
				Attrib.Offset = Offset;
				m_AttributeArray.push_back(Attrib);		
				
				GetVertexSlotOffset(i, &Offset);
			}			
			m_Stride = Offset;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2VertexLayout::Bind(GLuint program)
		{			
			LOCK_MUTEX

			COGLES2Renderer* lpR = (COGLES2Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			
			CAnsiString AttributeName;
			for (int i=0; i<m_VertexSlots.size(); i++)
			{	
				AttributeName = TranslateSlotName(m_VertexSlots[i]);
				if (m_VertexSlots[i].TypeIndex == 0)
					glBindAttribLocation(program, i, AttributeName.c_str());
				else 
				{
					LYNXANSICHAR Name[128];
					sprintf(Name, "%s%d", AttributeName.c_str(), m_VertexSlots[i].TypeIndex);
					glBindAttribLocation(program, i, Name);					
				}				
			}	

			UNLOCK_MUTEX
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2VertexLayout::vSet()
		{		
			/*int Offset = 0;//(first*m_Stride);            
			for (int i=0; i<GetNumSlots(); i++)
			{			
				glVertexAttribPointer(GetAttribute(i).Attribute, GetAttribute(i).Size, GetAttribute(i).Type, GL_FALSE, m_Stride, (const GLvoid *)(Offset + GetAttribute(i).Offset));
				glEnableVertexAttribArray(GetAttribute(i).Attribute);
			}
			*/

			/*
			COGLES2Renderer* lpR = (COGLES2Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			for (int i=0; i<m_VertexSlots.size(); i++)
			{	
				glBindAttribLocation(lpR->GetCachedProgram(), i, TranslateSlotName(m_VertexSlots[i]));
			}	
						
			GLint Status;
			
			glLinkProgram(lpR->GetCachedProgram());
			
			#if defined(_DEBUG)
				GLint logLength;
				glGetProgramiv(lpR->GetCachedProgram(), GL_INFO_LOG_LENGTH, &logLength);
				if (logLength > 0)
				{
					GLchar *log = (GLchar *)malloc(logLength);
					glGetProgramInfoLog(lpR->GetCachedProgram(), logLength, &logLength, log);
					LYNX_LOG(glpLogger, CString("Program link log:\n%s")+CString(log));
					free(log);
				}
			#endif
			
			glGetProgramiv(lpR->GetCachedProgram(), GL_LINK_STATUS, &Status);
			if (Status == 0)
			{
				LYNX_LOG(glpLogger, CString("Program link failed\n"));
			}
			
			glUseProgram(lpR->GetCachedProgram());
			*/
		}
	}
}

#endif