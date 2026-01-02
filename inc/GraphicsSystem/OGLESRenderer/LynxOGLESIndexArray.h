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

#ifndef __LYNXOGLESINDEXARRAY_H__
#define __LYNXOGLESINDEXARRAY_H__

#include <GraphicsSystem/LynxIndexArray.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/egl.h>
#elif defined (__iOS__)
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>	
#elif defined (__ANDROID__)
	#include <GLES/gl.h>
	#include <GLES/glext.h>	
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLESIndexArray : public CIndexArray
		{
			LYNX_DECLARE_CLASS(COGLESIndexArray);
		protected:
			GLuint							m_GLHandle;			
			void*							m_lpBuffer;
			GLenum							m_IndexType;
			int								m_MappedStartOffset;
			int								m_MappedSize;
			
		public:						
			COGLESIndexArray(CGraphicsSystem* lpsys);
			virtual ~COGLESIndexArray();
			
			LYNXINLINE COGLESIndexArray&		operator =(const COGLESIndexArray& t) {return (*this); };		

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