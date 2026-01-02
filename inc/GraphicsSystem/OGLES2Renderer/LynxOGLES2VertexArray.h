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

#ifndef __LYNXOGLES2VERTEXARRAY_H__
#define __LYNXOGLES2VERTEXARRAY_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/LynxVertexArray.h>
#include <GraphicsSystem/LynxVertexLayout.h>

#ifdef __WIN32__
	#define GL_GLEXT_PROTOTYPES	1
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <EGL/egl.h>	
#elif defined (__iOS__)
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
	#import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
#elif defined (__ANDROID__)
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		class COGLES2VertexLayout;
		class LYNXENGCLASS COGLES2VertexArray : public CVertexArray
		{
			LYNX_DECLARE_CLASS(COGLES2VertexArray);
		protected:
			GLuint							m_GLHandle;
            GLuint                          m_VAOHandle;			
			GraphicsSystem::CVertexLayoutPtr m_lpLayout;
			int								m_MappedStartOffset;
			int								m_MappedSize;	
		public:						
			COGLES2VertexArray(CGraphicsSystem* lpsys);
			virtual ~COGLES2VertexArray();
			
			LYNXINLINE COGLES2VertexArray&	operator =(const COGLES2VertexArray& t) {return (*this); };		

			LYNXBOOL						vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo = LYNX_TRUE, void* data = NULL);
			LYNXINLINE void					vSet(int stream, int first);	
			LYNXINLINE void*				vMap(int start, int size, CRenderer::LOCKMODE flag);		
			LYNXINLINE void					vUnmap();		
			CString							vGetDesc() {return CString(""); };
		};
	}		
}

#endif

#endif