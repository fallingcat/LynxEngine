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

#ifndef __LYNXSPRITE3DGROUPCONTAINER_H__
#define __LYNXSPRITE3DGROUPCONTAINER_H__

#include <GraphicsSystem/LynxRenderer.h>
#include <Container/LynxSprite3DContainer.h>

namespace LynxEngine 
{
	class CEngine;

	class LYNXENGCLASS CSprite3DClusterCreationDesc : public CSprite3DCreationDesc
	{	
		LYNX_DECLARE_CLASS(CSprite3DClusterCreationDesc);	
	public:						
		int								m_ClusterSize;		
		CSprite3DClusterContainer*		m_lpClusterContainer;
		int								m_SubContainerIndex;

		CSprite3DClusterCreationDesc() {m_ClusterSize = 0; m_lpClusterContainer = NULL; m_SubContainerIndex = 0; };

		CSprite3DClusterCreationDesc&		operator =(const CSprite3DClusterCreationDesc& rhs)
		{	
			*((CSprite3DCreationDesc*)this) = (CSprite3DCreationDesc&)rhs;
			m_ClusterSize = rhs.m_ClusterSize;
			m_lpClusterContainer = rhs.m_lpClusterContainer;
			m_SubContainerIndex = rhs.m_SubContainerIndex;
			return (*this);
		}
	};		

	class LYNXENGCLASS CSprite3DClusterContainer : public CSprite3DContainer
	{
		LYNX_DECLARE_CLASS(CSprite3DClusterContainer);
	public:	
		static int									m_BatchSize; 		
	protected:			
		int											m_ClusterSize;
		int											m_RenderedSize;
		GraphicsSystem::CIndexArrayPtr				m_IndexArray;		
		GraphicsSystem::SPRITE3DCLUSTERVERTEX*		m_lpV;
		LYNXVECTOR3D*								m_PosArray;
		LYNXVECTOR2D*								m_SizeArray;
		LYNXVECTOR2D*								m_UVArray;
		float*										m_ColorArray;
		float*										m_DegArray;
	public:		
		CSprite3DClusterContainer(CEngine *lpengine);
		~CSprite3DClusterContainer();				

		CSprite3DClusterContainer&					operator =(const CSprite3DClusterContainer& rhs);

		virtual LYNXBOOL							vCreate(const CCreationDesc* desc);
		//virtual LYNXBOOL							vCreate(const LYNXCHAR *name, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);
		//virtual LYNXBOOL							vCreate(int num, int length, const LYNXCHAR *name, const LYNXCHAR *ext, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);				
		
		virtual LYNXBOOL							vPlay(CReal skip);
		virtual void								vRender(CCameraContainer* cam = NULL);		

		//void										SetAlignType(const WORD at);	
		//LYNXINLINE const WORD						GetAlignType() const {return m_AlignType;};	

		LYNXINLINE int								GetClusterSize() const {return m_ClusterSize; };
		LYNXINLINE void								SetRenderedSize(int size) {if (size <= m_ClusterSize) m_RenderedSize = size; };
		LYNXINLINE int								GetRenderedSize() const {return m_RenderedSize; };

		LYNXINLINE void								Lock(int start = 0, int size = 0, GraphicsSystem::CRenderer::LOCKMODE flag = GraphicsSystem::CRenderer::LOCK_TO_WRITE) { m_lpV = (GraphicsSystem::SPRITE3DCLUSTERVERTEX*)m_VertexArray->vMap(start, size, flag); };
		LYNXINLINE void								Unlock() { m_VertexArray->vUnmap(); m_lpV = NULL; };	

		void										vSetPosition(const int index, const CVector3& pos);
		void										vGetPosition(const int index, CVector3& pos) const;
		void										vSetSize(const int index, const Math::CReal& w, const Math::CReal& h);
		void										vGetSize(const int index, Math::CReal& w, Math::CReal& h) const;
		void										vSetUV(const int index, LPLYNXUVPOINT lpuv);				
		void										SetColor(int index, LPLYNXCOLORRGBA lpc);		
		//void										SetUV(int index, LPLYNXVECTOR2D lpuv);
	};
}

#endif