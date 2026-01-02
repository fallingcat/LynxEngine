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

#ifndef __LYNXOGLES2VERTEXLAYOUT_H__
#define __LYNXOGLES2VERTEXLAYOUT_H__

#include <GraphicsSystem/LynxVertexLayout.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <EGL/egl.h>
#elif __iOS__
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
	#import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		struct OGLES2VertexAttribute
		{
			GLuint						Attribute;
			GLenum						Type;
			int							Size;
			int							Offset;		
		};
		class LYNXENGCLASS COGLES2VertexLayout : public CVertexLayout
		{	
			LYNX_DECLARE_CLASS(COGLES2VertexLayout);
		public:			
		protected:					
			int										m_Stride;
			CArray<OGLES2VertexAttribute>			m_AttributeArray;
		public:			
			COGLES2VertexLayout(CGraphicsSystem* lpgs);
			virtual ~COGLES2VertexLayout();

			LYNXBOOL								vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL);
			LYNXBOOL								vCreate(const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL);
			LYNXFORCEINLINE DWORD					vGetStride() {return m_Stride; };
			void									vSet();
			
			LYNXFORCEINLINE OGLES2VertexAttribute&	GetAttribute(int i) {return m_AttributeArray[i]; };	
			void									Bind(GLuint program);
		private:
			void									GetVertexSlotOffset(int i, int* offset);
			static const LYNXANSICHAR*				TranslateSlotName(CRenderer::VERTEXSLOT& slot);
			
		};		
	}		
}

#endif

#endif