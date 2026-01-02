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

#ifndef __LYNXOGLESVERTEXLAYOUT_H__
#define __LYNXOGLESVERTEXLAYOUT_H__

#include <GraphicsSystem/LynxVertexLayout.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/egl.h>
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
		struct OGLESVertexAttribute
		{
			GLuint						Attribute;
			GLenum						Type;
			int							Size;
			int							Offset;		
		};
		class LYNXENGCLASS COGLESVertexLayout : public CVertexLayout
		{	
			LYNX_DECLARE_CLASS(COGLESVertexLayout);
		public:			
		protected:					
			int										m_Stride;
			OGLESVertexAttribute*					m_PositionAttribute;
			OGLESVertexAttribute*					m_TexCoordAttribute;
			OGLESVertexAttribute*					m_ColorAttribute;
			OGLESVertexAttribute*					m_NormalAttribute;		
		public:			
			COGLESVertexLayout(CGraphicsSystem* lpgs);
			virtual ~COGLESVertexLayout();

			LYNXBOOL								vCreate(CRenderer::VERTEXSLOT* vs, int num, const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL);
			LYNXBOOL								vCreate(const LYNXCHAR* layoutname = NULL, const LYNXCHAR* vsname = NULL);
			LYNXFORCEINLINE DWORD					vGetStride() {return m_Stride; };
			void									vSet();
			
			LYNXFORCEINLINE OGLESVertexAttribute*	GetPositionAttribute() const {return m_PositionAttribute; };
			LYNXFORCEINLINE OGLESVertexAttribute*	GetTexCoordAttribute() const {return m_TexCoordAttribute; };
			LYNXFORCEINLINE OGLESVertexAttribute*	GetColorAttribute() const {return m_ColorAttribute; };
			LYNXFORCEINLINE OGLESVertexAttribute*	GetNormalAttribute() const {return m_NormalAttribute; };
		private:
			void									GetVertexSlotOffset(int i, int* offset);
			
		};		
	}		
}

#endif

#endif