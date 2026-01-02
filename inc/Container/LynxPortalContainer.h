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

#ifndef __LYNXPORTALCONTAINER_H__
#define __LYNXPORTALCONTAINER_H__

#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CPortalContainer : public CContainer
	{	
		LYNX_DECLARE_CLASS(CPortalContainer);
	public:		
	protected:	
		CArray<Math::CVector3>							m_VertexList;				
		GraphicsSystem::CVertexArrayPtr					m_VertexArray;				
	public:		
		CPortalContainer(CEngine *lpengine);
		virtual ~CPortalContainer();		

		virtual void									vInstance(CContainer& rhs);		
		LYNXBOOL										Setup(void);			
		LYNXINLINE CArray<Math::CVector3>&				GetVertexList() {return m_VertexList; };
		LYNXINLINE const Math::CVector3&				GetVertex(int i) {return m_VertexList[i]; };
		//LYNXINLINE LYNXVERTEXARRAY&						GetVertexArray() {return m_VertexArray; };		
		virtual void									vRender(CCameraContainer* cam = NULL);				

	friend class SceneSystem::CScene;		
	};
}

#endif