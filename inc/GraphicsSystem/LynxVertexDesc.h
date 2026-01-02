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

#ifndef __LYNXVERTEXDESC_H__
#define __LYNXVERTEXDESC_H__

#include <GraphicsSystem/LynxRenderer.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class CGraphicsSystem;

		template <class Type> 
		class LYNXENGCLASS CVertexDesc : public CResource
		{	
			LYNX_DECLARE_CLASS(CVertexDesc);
		public:			
		protected:		
			CGraphicsSystem*					m_lpGraphicsSystem;				
			CArray<LYNXVERTEXSLOT>				m_VertexSlot;
			LYNXVERTEXLAYOUT					m_Layout;
		public:			
			CVertexDesc() { m_ResourceType = CResource::VERTEX_DESC; m_lpGraphicsSystem = NULL;	m_VertexSlot.clear(); };				
			CVertexDesc(CGraphicsSystem* lpgs) {m_ResourceType = CResource::VERTEX_DESC; m_lpGraphicsSystem = pgs; m_VertexSlot.clear();};
			virtual ~CVertexDesc() {};

			void								SetlpGraphicsSystem(CGraphicsSystem* pgs) {m_lpGraphicsSystem = pgs; };
			LYNXVERTEXLAYOUT&					GetLayout() { return m_Layout; };
			static Type*						Cast(void* p) {return (Type*)p; };
		};		
	}		
}

#endif