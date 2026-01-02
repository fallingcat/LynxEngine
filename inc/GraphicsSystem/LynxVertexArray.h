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

#ifndef __LYNXVERTEXARRAY_H__
#define __LYNXVERTEXARRAY_H__

#include <LynxResource.h>
#include <GraphicsSystem/LynxRenderer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CVertexArray : public CResource
		{
			LYNX_DECLARE_CLASS(CVertexArray);
		public:			
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;	
			DWORD							m_Stride;
			DWORD							m_Size;
			void*							m_lpBuffer;						
		public:						
			CVertexArray(CGraphicsSystem* lpsys);
			virtual ~CVertexArray();	
			
			LYNXINLINE CVertexArray&		operator =(const CVertexArray& t) {m_lpGraphicsSystem = t.m_lpGraphicsSystem; return (*this); };		
			void							SetlpGraphicsSystem(CGraphicsSystem* pgs) {m_lpGraphicsSystem = pgs; };			
			virtual LYNXBOOL				vCreate(GraphicsSystem::CVertexLayoutPtr& lpl, int num, int flag, LYNXBOOL bbo = LYNX_TRUE, void* data = NULL) = 0;
			LYNXFORCEINLINE DWORD			GetSize() const {return m_Size; };
			LYNXFORCEINLINE DWORD			GetStride() const {return m_Stride; };
			virtual void					vSet(int stream, int first) = 0;
			virtual void*					vMap(int start, int size, CRenderer::LOCKMODE flag) = 0;
			virtual void					vUnmap() = 0;			
		};
	}		
}

#endif