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

#ifndef __LYNXOGLES3INDEXARRAY_H__
#define __LYNXOGLES3INDEXARRAY_H__

#include <GraphicsSystem/LynxIndexArray.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

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
		class LYNXENGCLASS COGLES3IndexArray : public CIndexArray
		{
			LYNX_DECLARE_CLASS(COGLES3IndexArray);
		protected:
			GLuint							m_GLHandle;			
			void*							m_lpBuffer;
			GLenum							m_IndexType;
			int								m_MappedStartOffset;
			int								m_MappedSize;
		public:						
			COGLES3IndexArray(CGraphicsSystem* lpsys);
			virtual ~COGLES3IndexArray();
			
			LYNXINLINE COGLES3IndexArray&		operator =(const COGLES3IndexArray& t) {return (*this); };		

			LYNXBOOL						vCreate(int stride, int num, int flag, LYNXBOOL bbo = LYNX_TRUE, void* data = NULL);
			LYNXINLINE void					vSet(int first);
			LYNXINLINE void*				vMap(int start, int size, CRenderer::LOCKMODE flag);		
			LYNXINLINE void					vUnmap();		
			
			LYNXFORCEINLINE const GLenum	GetIndexType() const {return m_IndexType; };
		};
	}		
}

#endif

#endif