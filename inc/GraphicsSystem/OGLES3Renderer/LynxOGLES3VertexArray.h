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

#ifndef __LYNXOGLES3VERTEXARRAY_H__
#define __LYNXOGLES3VERTEXARRAY_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/LynxVertexArray.h>
#include <GraphicsSystem/LynxVertexLayout.h>

#ifdef __WIN32__
	#define GL_GLEXT_PROTOTYPES	1
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
	#include <EGL/egl.h>	
#elif defined (__iOS__)
	#import <OpenGLES/ES3/gl.h>
	#import <OpenGLES/ES3/glext.h>
#elif defined (__ANDROID__)
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		class COGLES3VertexLayout;
		class LYNXENGCLASS COGLES3VertexArray : public CVertexArray
		{
			LYNX_DECLARE_CLASS(COGLES3VertexArray);
		protected:
			GLuint							m_GLHandle;
            GLuint                          m_VAOHandle;			
			GraphicsSystem::CVertexLayoutPtr m_lpLayout;
			int								m_MappedStartOffset;
			int								m_MappedSize;	
		public:						
			COGLES3VertexArray(CGraphicsSystem* lpsys);
			virtual ~COGLES3VertexArray();
			
			LYNXINLINE COGLES3VertexArray&	operator =(const COGLES3VertexArray& t) {return (*this); };		

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