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

#ifndef __LYNXOBJPROXY_H__
#define __LYNXOBJPROXY_H__

#include <LynxGameObj.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CObjProxy : public CGameObj
	{
		LYNX_DECLARE_CLASS(CObjProxy);
	public:			
		typedef enum {
			PROXY_BOX = 0,
			PROXY_SPHERE,
			PROXY_PYRAMID,
			PROXY_CONE,
			PROXY_GEOMETRY,
			NUM_PROXYTYPE,
		}PROXYTYPE;

	protected:		
		PROXYTYPE								m_ProxyType;
		CArray<Math::CVector3>					m_V;
		CArray<LYNXSIMPLETRIANGLE>				m_T;		
		GraphicsSystem::CVertexArrayPtr			m_VertexArray;
		GraphicsSystem::CIndexArrayPtr			m_IndexArray;
	public:			
		CObjProxy();
		CObjProxy(CEngine* lpe);
		virtual ~CObjProxy();	

		LYNXFORCEINLINE PROXYTYPE				GetProxyType() const {return m_ProxyType; };
		LYNXINLINE int							GetNumVertices() {return (int)m_V.size(); };
		LYNXINLINE int							GetNumTriangles() {return (int)m_T.size(); };
		LYNXINLINE const Math::CVector3&		GetVertex(int i) const {return m_V[i]; };
		LYNXINLINE const LYNXSIMPLETRIANGLE&	GetTriangle(int i) const {return m_T[i]; };
		virtual void							vCreate(DWORD numv, Math::CVector3* lpv, DWORD numi, int* lpi);
		virtual void							vRender(CCameraContainer* cam) ;
	private:			
	};
}

#endif
