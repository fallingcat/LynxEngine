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

#ifndef __LYNXOGLES3VERTEXLAYOUT_H__
#define __LYNXOGLES3VERTEXLAYOUT_H__

#include <GraphicsSystem/LynxVertexLayout.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
	#include <EGL/egl.h>
#elif __iOS__
	#import <OpenGLES/ES3/gl.h>
	#import <OpenGLES/ES3/glext.h>
#elif __ANDROID__
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		struct OGLES3VertexAttribute
		{
			GLuint						Attribute;
			GLenum						Type;
			int							Size;
			int							Offset;		
		};
		class LYNXENGCLASS COGLES3VertexLayout : public CVertexLayout
		{	
			LYNX_DECLARE_CLASS(COGLES3VertexLayout);
		public:			
		protected:					
			int										m_Stride;
			CArray<OGLES3VertexAttribute>			m_AttributeArray;
		public:			
			COGLES3VertexLayout(CGraphicsSystem* lpgs);
			virtual ~COGLES3VertexLayout();

			LYNXBOOL								vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL);
			LYNXBOOL								vCreate(const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL);
			LYNXFORCEINLINE DWORD					vGetStride() {return m_Stride; };
			void									vSet();
			
			LYNXFORCEINLINE OGLES3VertexAttribute&	GetAttribute(int i) {return m_AttributeArray[i]; };	
			void									Bind(GLuint program);
		private:
			void									GetVertexSlotOffset(int i, int* offset);
			static const LYNXANSICHAR*				TranslateSlotName(CRenderer::VERTEXSLOT& slot);
			
		};		
	}		
}

#endif

#endif