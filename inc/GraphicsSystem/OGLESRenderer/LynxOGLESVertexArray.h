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

#ifndef __LYNXOGLESVERTEXARRAY_H__
#define __LYNXOGLESVERTEXARRAY_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/LynxVertexArray.h>
#include <GraphicsSystem/LynxVertexLayout.h>

#ifdef __WIN32__
	#include <GLES/gl.h>
	#include <GLES/egl.h>
#elif defined(__iOS__)
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>	
#elif defined(__ANDROID__)
	#include <GLES/gl.h>
	#include <GLES/glext.h>	
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		class COGLESVertexLayout;
		class LYNXENGCLASS COGLESVertexArray : public CVertexArray
		{
			LYNX_DECLARE_CLASS(COGLESVertexArray);
		protected:
			GLuint							m_GLHandle;
			void*							m_lpBuffer;		
			GraphicsSystem::CVertexLayoutPtr m_lpLayout;
			int								m_MappedStartOffset;
			int								m_MappedSize;
		public:						
			COGLESVertexArray(CGraphicsSystem* lpsys);
			virtual ~COGLESVertexArray();
			
			LYNXINLINE COGLESVertexArray&	operator =(const COGLESVertexArray& t) {return (*this); };		

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