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

#ifndef __LYNXCROSSSPRITEGRASSCONTAINER_H__
#define __LYNXCROSSSPRITEGRASSCONTAINER_H__

#include <GraphicsSystem/LynxRenderer.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CCrossSpriteGrassCreationDesc : public CSpriteGrassCreationDesc
	{	
		LYNX_DECLARE_CLASS(CCrossSpriteGrassCreationDesc);	
	public:				
		int							m_BaseWidth, m_BaseHeight;
		int							m_NumBlades;
		CCrossSpriteGrassCreationDesc() {m_BaseWidth = m_BaseHeight = 2.0f; m_NumBlades = 2; };
	};		

	class LYNXENGCLASS CCrossSpriteGrassContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CCrossSpriteGrassContainer);
	public:
		typedef struct {
			LYNXUVPOINT					TexCoord;
			float						Dir[2];
			float						Instance;						
		}GRASSVERTEX;	

		typedef struct {
			LYNXBOOL					bVisible;
			LYNXVECTOR4D				Pos;
			LYNXVECTOR4D				Normal;
			LYNXVECTOR4D				Rotation;
			float						SquareDist2Camera;
		}INSTANCE;	

		typedef struct {
			int							Index;
			Math::CReal					Distance2Camera;				
		}VISINSTANCE;	
		
		static int									m_BatchSize;		
	protected:			 		
		DECLAR_PARAM_FLOAT(m_Width);
		DECLAR_PARAM_FLOAT(m_Height);		
		DECLAR_PARAM_BOOL(m_bFadeOut);
		DECLAR_PARAM_BOOL(m_bSort);
		DECLAR_PARAM_FLOAT(m_VisDistance);
		DECLAR_PARAM_FLOAT(m_FadeOutDistance);
		int											m_NumBlades;
		int											m_NumSegX, m_NumSegY;
		static GraphicsSystem::CVertexLayoutPtr		m_VertexLayout;
		int											m_ClusterSize;
		int											m_RenderedSize;		
		int											m_VisRenderedSize;
		GraphicsSystem::CVertexArrayPtr				m_VertexArray;		
		GraphicsSystem::CIndexArrayPtr				m_IndexArray;			
		GRASSVERTEX*								m_lpV;

		CArray<INSTANCE>							m_InstanceArray;
		CArray<VISINSTANCE>							m_VisInstanceArray;
		LYNXVECTOR4D*								m_VisPosArray;
		LYNXVECTOR4D*								m_VisNormalArray;				
		LYNXVECTOR4D*								m_VisRotationArray;		

		float										m_ColliderData[4];  // x,y,z -> position, w -> radius
	public:		
		CCrossSpriteGrassContainer(CEngine *lpengine);
		~CCrossSpriteGrassContainer();				

		CCrossSpriteGrassContainer&					operator =(const CCrossSpriteGrassContainer& rhs);

		static GraphicsSystem::CVertexLayout*		CreateVertexLayout(CEngine* e);
		virtual LYNXBOOL							vCreate(const CCreationDesc* desc);
		//virtual LYNXBOOL							vCreate(const LYNXCHAR *name, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);
		//virtual LYNXBOOL							vCreate(int num, int length, const LYNXCHAR *name, const LYNXCHAR *ext, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);				
		
		virtual LYNXBOOL							vPlay(CReal skip);
		void										ComputeVis(CCameraContainer* cam);
		void										ComputeTranslucentVis(CCameraContainer* cam);
		virtual void								vRender(CCameraContainer* cam);		
		virtual void								vTranslucentRender(CCameraContainer* cam);

		LYNXINLINE void								SetCollider(Math::CVector3& pos, Math::CReal radius) { m_ColliderData[0] = pos.x; m_ColliderData[1] = pos.y; m_ColliderData[2] = pos.z; m_ColliderData[3] = radius.r; };
		//void										SetAlignType(const WORD at);	
		//LYNXINLINE const WORD						GetAlignType() const {return m_AlignType;};	

		LYNXINLINE int								GetClusterSize() const {return m_ClusterSize; };
		void										IncreaseClusterSize();
		void										SetRenderedSize(int size);
		LYNXINLINE int								GetRenderedSize() const {return m_RenderedSize; };

		LYNXINLINE void								SetVisDistance(float dist) {m_VisDistance = dist; };

		LYNXINLINE void								Lock(int start = 0, int size = 0, GraphicsSystem::CRenderer::LOCKMODE flag = GraphicsSystem::CRenderer::LOCK_TO_WRITE) { m_lpV = (GRASSVERTEX*)m_VertexArray->vMap(start, size, flag); };
		LYNXINLINE void								Unlock() { m_VertexArray->vUnmap(); m_lpV = NULL; };	

		void										Remove(int index);
		LYNXINLINE void								vGetPosition(const int index, CVector3& pos) const { pos.x = m_InstanceArray[index].Pos.x; pos.y = m_InstanceArray[index].Pos.y; pos.z = m_InstanceArray[index].Pos.z; };
		void										vSetPosition(const int index, const CVector3& pos);
		LYNXINLINE void								GetNormal(int index, CVector3& n) const { n.x = m_InstanceArray[index].Normal.x; n.y = m_InstanceArray[index].Normal.y; n.z = m_InstanceArray[index].Normal.z; };
		void										SetNormal(int index, CVector3& n);
		LYNXINLINE void								vGetSize(const int index, CReal& w, CReal& h) const {w = m_InstanceArray[index].Pos.w; h = -m_InstanceArray[index].Normal.w;};
		void										vSetSize(const int index, const CReal& w, const CReal& h);
		LYNXINLINE void								GetRotDeg(int index, CVector3& r) const {r.x = m_InstanceArray[index].Rotation.x*LYNXR2D; r.y = m_InstanceArray[index].Rotation.y*LYNXR2D; r.z = m_InstanceArray[index].Rotation.z*LYNXR2D;};
		void										SetRotDeg(int index, CVector3& r);	
		LYNXINLINE Math::CReal						GetAlpha(int index) const {return m_InstanceArray[index].Rotation.w; };
		void										SetAlpha(int index, Math::CReal& a);
		void										SetUV(int index, LPLYNXUVPOINT lpuv);

		virtual	void								vAttachToTerrain(CTerrainContainer& t, LYNXBOOL balign);		
	protected:
		void										CreateVertexLayout(void);

		static bool									compare_front2back(const VISINSTANCE* e1, const VISINSTANCE* e2);
		static bool									compare_back2front(const VISINSTANCE* e1, const VISINSTANCE* e2);
	};
	LYNXINLINE bool operator > (const CCrossSpriteGrassContainer::VISINSTANCE& lhs, const CCrossSpriteGrassContainer::VISINSTANCE& rhs) {return (lhs.Distance2Camera > rhs.Distance2Camera); };
	LYNXINLINE bool operator < (const CCrossSpriteGrassContainer::VISINSTANCE& lhs, const CCrossSpriteGrassContainer::VISINSTANCE& rhs) {return (lhs.Distance2Camera < rhs.Distance2Camera); };
	LYNXINLINE bool operator == (const CCrossSpriteGrassContainer::VISINSTANCE& lhs, const CCrossSpriteGrassContainer::VISINSTANCE& rhs) {return (lhs.Distance2Camera == rhs.Distance2Camera); };
	
	LYNXINLINE bool CCrossSpriteGrassContainer::compare_front2back(const VISINSTANCE* e1, const VISINSTANCE* e2) { return (*e1 < *e2); };
	LYNXINLINE bool CCrossSpriteGrassContainer::compare_back2front(const VISINSTANCE* e1, const VISINSTANCE* e2) { return (*e1 > *e2); };
}

#endif