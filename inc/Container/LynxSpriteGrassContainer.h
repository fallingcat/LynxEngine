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

#ifndef __LYNXSPRITEGRASSCONTAINER_H__
#define __LYNXSPRITEGRASSCONTAINER_H__

#include <GraphicsSystem/LynxRenderer.h>
#include <Container/LynxSprite3DContainer.h>

namespace LynxEngine 
{
	class CEngine;

	class LYNXENGCLASS CSpriteGrassCreationDesc : public CSprite3DCreationDesc
	{	
		LYNX_DECLARE_CLASS(CSpriteGrassCreationDesc);	
	public:								
		int							m_ClusterSize;		

		CSpriteGrassCreationDesc() {m_ClusterSize = 1024; };
	};		

	class LYNXENGCLASS CSpriteGrassContainer : public CSprite3DContainer
	{
		LYNX_DECLARE_CLASS(CSpriteGrassContainer);
	public:
		typedef struct {
			LYNXUVPOINT					TexCoord;									
			float						Instance;						
		}GRASSVERTEX;	
		
		static int									m_BatchSize;
	protected:			 
		static GraphicsSystem::CVertexLayoutPtr		m_VertexLayout;
		int											m_ClusterSize;
		int											m_RenderedSize;
		float										m_VisDistance;
		int											m_VisRenderedSize;
		GraphicsSystem::CIndexArrayPtr				m_IndexArray;		
		GRASSVERTEX*								m_lpV;
		LYNXVECTOR4D*								m_PosArray;
		LYNXVECTOR4D*								m_NormalArray;
		float*										m_RotationArray;
		LYNXVECTOR4D*								m_VisPosArray;
		LYNXVECTOR4D*								m_VisNormalArray;				
		float*										m_VisRotationArray;				
	public:		
		CSpriteGrassContainer(CEngine *lpengine);
		~CSpriteGrassContainer();				

		CSpriteGrassContainer&						operator =(const CSpriteGrassContainer& rhs);

		static GraphicsSystem::CVertexLayout*		CreateVertexLayout(CEngine* e);
		virtual LYNXBOOL							vCreate(const CCreationDesc* desc);
		//virtual LYNXBOOL							vCreate(const LYNXCHAR *name, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);
		//virtual LYNXBOOL							vCreate(int num, int length, const LYNXCHAR *name, const LYNXCHAR *ext, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);				
		
		virtual LYNXBOOL							vPlay(CReal skip);
		void										ComputeVis(CCameraContainer* cam);
		virtual void								vRender(CCameraContainer* cam = NULL);		

		//void										SetAlignType(const WORD at);	
		//LYNXINLINE const WORD						GetAlignType() const {return m_AlignType;};	

		LYNXINLINE int								GetClusterSize() const {return m_ClusterSize; };
		void										IncreaseClusterSize();
		void										SetRenderedSize(int size);
		LYNXINLINE int								GetRenderedSize() const {return m_RenderedSize; };

		LYNXINLINE void								SetVisDistance(float dist) {m_VisDistance = dist; };

		LYNXINLINE void								Lock(int start = 0, int size = 0, GraphicsSystem::CRenderer::LOCKMODE flag = GraphicsSystem::CRenderer::LOCK_TO_WRITE) { m_lpV = (GRASSVERTEX*)m_VertexArray->vMap(start, size, flag); };
		LYNXINLINE void								Unlock() { m_VertexArray->vUnmap(); m_lpV = NULL; };	

		LYNXINLINE void								vGetPosition(const int index, CVector3& pos) const { pos.x = m_PosArray[index].x; pos.y = m_PosArray[index].y; pos.z = m_PosArray[index].z; };
		void										vSetPosition(const int index, const CVector3& pos);
		LYNXINLINE void								GetNormal(int index, CVector3& n) const { n.x = m_NormalArray[index].x; n.y = m_NormalArray[index].y; n.z = m_NormalArray[index].z; };
		void										SetNormal(int index, CVector3& n);
		LYNXINLINE void								vGetSize(const int index, CReal& w, CReal& h) const {w = m_PosArray[index].w; h = -m_NormalArray[index].w;};
		void										vSetSize(const int index, const CReal& w, const CReal& h);
		LYNXINLINE void								GetRotDeg(int index, CReal& deg) const {deg = m_RotationArray[index]*LYNXR2D; };
		void										SetRotDeg(int index, CReal deg);		
		void										SetUV(int index, LPLYNXUVPOINT lpuv);		
	protected:
		void										CreateVertexLayout(void);
	};
}

#endif