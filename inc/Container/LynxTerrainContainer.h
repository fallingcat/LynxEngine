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

#ifndef __LYNXTERRAINCONTAINER_H__
#define __LYNXTERRAINCONTAINER_H__

#include <LynxEngineClasses.h>
#include <MaterialSystem/LynxTextureAnimation.h>
#include <CollisionSystem/LynxAABB.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CTerrainCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CTerrainCreationDesc);	
	public:									
		LYNXBOOL					m_bUseBufferObject;
		int							m_HeightmapWidth, m_HeightmapHeight;
		float						m_HeightmapCellSize, m_TerrainMeshCellSize;
		int							m_SegmentSize;
		float						m_HeightmapScale;
		float						m_HeightmapBase;

		CTerrainCreationDesc() {m_bUseBufferObject = LYNX_TRUE; };
	};	

	class LYNXENGCLASS CTerrainContainerSegment : public CObj
	{
		LYNX_DECLARE_CLASS(CTerrainContainerSegment);
	public:
		CollisionSystem::CAABB							m_AABB;
	protected:
		CTerrainContainer*								m_lpTerrain;
		CArray<GraphicsSystem::CRenderer::PRIMITIVE>	m_PrimitiveList;
		DECLAR_PARAM_ARRAY(LynxEngine::MaterialSystem::CTextureAnimationPtr, m_TextureAnimationArray)			
		LYNXBOOL										m_bVisible;
		CList<CRenderableObj*>							m_AttachedObjectList;	
	public:
		CTerrainContainerSegment();
		CTerrainContainerSegment(CTerrainContainer* c);
		~CTerrainContainerSegment();

		LYNXINLINE int									GetNumTextureAnimation() const {return m_TextureAnimationArray.size(); };
		LYNXINLINE MaterialSystem::CTextureAnimationPtr& GetTextureAnimation(int channel) {return m_TextureAnimationArray[channel]; };						
		LYNXINLINE void									SetlpTerrain(CTerrainContainer* c) {m_lpTerrain = c; };
		void											Create(int x, int y);
		LYNXINLINE int									GetNumPrimitives() {return m_PrimitiveList.size(); };
		LYNXINLINE GraphicsSystem::CRenderer::PRIMITIVE& GetPrimitive(int i) {return m_PrimitiveList[i]; };
		void											ComputeBoundary();

		LYNXINLINE CList<CRenderableObj*>&				GetAttachedObjectList() {return m_AttachedObjectList; };
		LYNXINLINE void									AttachObject(CRenderableObj* o) {m_AttachedObjectList.push_back(o); };
		LYNXINLINE void									RemoveAttachedObject(CRenderableObj* o) {m_AttachedObjectList.remove(o); };

	friend class CTerrainContainer;
	};

	class LYNXENGCLASS CTerrainContainer : public CContainer
	{	
		LYNX_DECLARE_CLASS(CTerrainContainer);
	public:
		static const int LAYER0_CHANNEL =	0;
		static const int LAYER1_CHANNEL = LAYER0_CHANNEL + 1;
		static const int LAYER2_CHANNEL = LAYER0_CHANNEL + 2;
		static const int LAYER3_CHANNEL = LAYER0_CHANNEL + 3;			
		static const int LAYER4_CHANNEL = LAYER0_CHANNEL + 4;			
		static const int LAYER5_CHANNEL = LAYER0_CHANNEL + 5;			
		static const int LAYER6_CHANNEL = LAYER0_CHANNEL + 6;			
		static const int LAYER7_CHANNEL = LAYER0_CHANNEL + 7;							
		static const int MAX_TERRAIN_LAYERS = 4;

		typedef struct {
			Math::CVector3				Pos;
			Math::CVector3				Tangent;
			Math::CVector3				Binormal;
			Math::CVector3				Normal;
			LYNXUVPOINT					TexCoord[MAX_TERRAIN_LAYERS];			
			Math::CReal					TexWeight[MAX_TERRAIN_LAYERS];
		}VERTEX;				

		typedef struct {
			LYNXVECTOR3D				Pos;	
			LYNXVECTOR3D				Tangent;				
			LYNXVECTOR3D				Normal;					
			LYNXUVPOINT					TexCoord[4]; 
			LYNXFLOAT					TexWeight[4];
		}TERRAINVERTEX;	

		typedef struct {
			union {
				struct {
					DWORD				v1, v2, v3;				
				};
				DWORD				v[3];	
			};
			LYNXPLANE				Plane;		/*!< The plane data of this triangle */
			SceneSystem::CSurface*	lpSurface;	/*!< Pointer to corresponding surface */
		}TRIANGLE;				
	protected:	
		static GraphicsSystem::CVertexLayoutPtr			m_VertexLayout;
		int												m_Width, m_Height;
		float											m_CellSize;

		int												m_HeightMapWidth, m_HeightMapHeight;
		float											m_HeightMapCellSize;

		DECLAR_PARAM_NATIVE_ARRAY(Math::CReal, m_uTile, MAX_TERRAIN_LAYERS)				
		DECLAR_PARAM_NATIVE_ARRAY(Math::CReal, m_vTile, MAX_TERRAIN_LAYERS)						
		Math::CReal*									m_lpHeightMap;
		CArray<VERTEX>*									m_VertexList;
		CArray<TRIANGLE>*								m_TriangleList;
		CArray<GraphicsSystem::CRenderer::PRIMITIVE>*	m_PrimitiveList;
		CArray<GraphicsSystem::CVertexArrayPtr>			m_VertexArray;
		CArray<GraphicsSystem::CIndexArrayPtr>			m_IndexArray;
		int												m_SegmentSize;	
		CArray<CTerrainContainerSegment*>				m_SegmentList;
		CList<MaterialSystem::CTextureAnimationPtr>		m_LayerTextureList;
	public:		
		CTerrainContainer(CEngine *lpengine);
		virtual ~CTerrainContainer();		

		static GraphicsSystem::CVertexLayout*			CreateVertexLayout(CEngine* e);
		void											CreateVertexLayout(void);
		virtual LYNXBOOL								vCreate(const CCreationDesc* desc);
		virtual void									vCreateSegments(int size);
		virtual void									vInstance(CContainer& rhs);
		void											vSetNumLODs(int lod);
		void											UpdateTextureCoord(int lod);
		void											UpdateVertexArrayTexCoord(int lod);
		LYNXBOOL										SetupBuffers(void);	
		void											SetupRNRPlane(void);
		LYNXINLINE CArray<VERTEX>&						GetVertexList(int lod) {return m_VertexList[lod]; };
		LYNXINLINE const VERTEX&						GetVertex(int lod, int i) {return m_VertexList[lod][i]; };
		LYNXINLINE CArray<TRIANGLE>&					GetTriangleList(int lod) {return m_TriangleList[lod]; };
		LYNXINLINE const TRIANGLE&						GetTriangle(int lod, int i) {return m_TriangleList[lod][i]; };
		LYNXINLINE CArray<GraphicsSystem::CRenderer::PRIMITIVE>& GetPrimitiveList(int lod) {return m_PrimitiveList[lod]; };
		LYNXINLINE const GraphicsSystem::CRenderer::PRIMITIVE& GetPrimitive(int lod, int i) {return m_PrimitiveList[lod][i]; };
		LYNXINLINE GraphicsSystem::CVertexArrayPtr&		GetVertexArray(int lod) {return m_VertexArray[lod]; };
		LYNXINLINE GraphicsSystem::CIndexArrayPtr&		GetIndexArray(int lod) {return m_IndexArray[lod]; };
		void											ComputeVis(CCameraContainer* cam);
		virtual void									vRender(CCameraContainer* cam = NULL);

		void											ComputeBoundary();

		LYNXINLINE const Math::CReal					GetUTile(int i) {return m_uTile[i]; };
		LYNXINLINE const Math::CReal					GetVTile(int i) {return m_vTile[i]; };

		LYNXINLINE const Math::CReal*					GetHeightMap() {return m_lpHeightMap; };
		LYNXINLINE Math::CReal&							HeightMap(int y, int x) {return m_lpHeightMap[y*(m_HeightMapWidth+1)+x]; };
		Math::CReal										GetHeight(CVector3& pos);	
		Math::CReal										GetHeight(const CReal& x, const CReal& z);
		void											GetOrientation(CMatrix3& m, CVector3& pos);
		void											GetNormal(CVector3& normal, CVector3& pos);
		void											GetTangent(CVector3& tangent, CVector3& pos);
		Math::CReal&									GetHeightFromVertexIndex(int y, int x);
		LYNXINLINE int									GetWidth() const {return m_Width; };
		LYNXINLINE int									GetHeight() const {return m_Height; };
		LYNXINLINE float								GetCellSize() const {return m_CellSize; };
		LYNXINLINE int									GetHeightMapWidth() const {return m_HeightMapWidth; };
		LYNXINLINE int									GetHeightMapHeight() const {return m_HeightMapHeight; };
		LYNXINLINE float								GetHeightMapCellSize() const {return m_HeightMapCellSize; };
		LYNXINLINE int									GetSegmentSize() const {return m_SegmentSize; };
		LYNXINLINE CTerrainContainerSegment&			GetSegment(int i) {return *(m_SegmentList[i]); };
		LYNXINLINE const int							GetNumSegments() const {return (int)m_SegmentList.size(); };	

		LYNXBOOL										SetupHeightFieldShapeDesc(PhysicsSystem::CHeightFieldShapeDesc* desc, int lod = 0);
		void											ReleaseHeightFieldShapeDesc(PhysicsSystem::CHeightFieldShapeDesc* desc);

		virtual	void									vAttachToTerrain(CTerrainContainer& t, LYNXBOOL balign) {};
		void											AttachObject(CRenderableObj* o);
		void											RemoveAttachedObject(CRenderableObj* o);

		virtual DWORD									vSystemMemorySize();

	protected:
		void											CreateDefaultMaterial(MaterialSystem::CMaterialPtr& lpm, CString& name);

	friend class SceneSystem::CScene;		
	};
}

#endif