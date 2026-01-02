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

#ifndef __LYNXGEOMETRYCONTAINER_H__
#define __LYNXGEOMETRYCONTAINER_H__

#include <LynxEngineClasses.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CGeometryCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CGeometryCreationDesc);	
	public:							
		LYNXBOOL					m_bUseBufferObject;

		CGeometryCreationDesc() {Default(); };
		void Default() {CCreationDesc::Default(); m_bUseBufferObject = LYNX_TRUE; };
	};		

	class LYNXENGCLASS CGeometryContainer : public CContainer
	{	
		LYNX_DECLARE_CLASS(CGeometryContainer);
	public:		
		typedef	struct {
			LYNXVECTOR3D			Pos;
			LYNXVECTOR3D			Tangent;
			LYNXVECTOR3D			Binormal;
			LYNXVECTOR3D			Normal;
			LYNXUVPOINT				TexCoord[8];
			LYNXCOLORRGBA			Color;
		}VERTEX;

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

		struct MeshData : public CUnknown{
			CArray< CArray<VERTEX> >				VertexList;
			CArray< CArray<TRIANGLE> >				TriangleList;
			CArray< CArray<GraphicsSystem::CRenderer::PRIMITIVE> > PrimitiveList;	

			MeshData()
			{
				VertexList.clear();
				TriangleList.clear();
				PrimitiveList.clear();
			}

			~MeshData()
			{
				VertexList.clear();
				TriangleList.clear();
				PrimitiveList.clear();
			}
		};
	protected:	
		GraphicsSystem::STATICVERTEX*					m_lpV;
		CArray<VERTEX>									m_TVertexList;
		CCountedPtr<MeshData>							m_MeshData;

		CArray<GraphicsSystem::CVertexArrayPtr>			m_VertexArray;
		CArray<GraphicsSystem::CIndexArrayPtr>			m_IndexArray;		
	public:		
		CGeometryContainer(CEngine *lpengine);
		virtual ~CGeometryContainer();		

		virtual void									vInstance(const CContainer& rhs);
		
		virtual LYNXBOOL								vCreate(const CCreationDesc* desc);
		LYNXBOOL										Create(const int lod, CArray<VERTEX>& vertex, CArray<GraphicsSystem::CRenderer::PRIMITIVE> primitive, const CString& texture, const CString& matname);

		void											vSetNumLODs(int lod);
		virtual void									vAddLOD();

		void											SetVertexList(const int lod, const CArray<VERTEX>& v);
		void											SetPrimitiveList(const int lod, const CArray<GraphicsSystem::CRenderer::PRIMITIVE>& t);
		LYNXBOOL										SetupBuffers(const int lod, GraphicsSystem::CVertexLayoutPtr pvl = GraphicsSystem::CVertexLayoutPtr());	
		void											Setup(const int lod, const CArray<VERTEX>& v, const CArray<GraphicsSystem::CRenderer::PRIMITIVE>& t);
		void											SetupRNRPlane(void);
		LYNXINLINE CArray<VERTEX>&						GetVertexList(int lod) {return m_MeshData->VertexList[lod]; };
		LYNXINLINE const VERTEX&						GetVertex(int lod, int i) {return m_MeshData->VertexList[lod][i]; };
		LYNXINLINE CArray<TRIANGLE>&					GetTriangleList(int lod) {return m_MeshData->TriangleList[lod]; };
		LYNXINLINE const TRIANGLE&						GetTriangle(int lod, int i) {return m_MeshData->TriangleList[lod][i]; };
		LYNXINLINE CArray<GraphicsSystem::CRenderer::PRIMITIVE>& GetPrimitiveList(int lod) {return m_MeshData->PrimitiveList[lod]; };
		LYNXINLINE const GraphicsSystem::CRenderer::PRIMITIVE& GetPrimitive(int lod, int i) {return m_MeshData->PrimitiveList[lod][i]; };
		LYNXINLINE GraphicsSystem::CVertexArray*		GetVertexArray(int lod) {return m_VertexArray[lod]; };
		LYNXINLINE GraphicsSystem::CIndexArray*			GetIndexArray(int lod) {return m_IndexArray[lod]; };
		LYNXBOOL										vPlay(CReal step);
		virtual void									vRender(CCameraContainer* cam = NULL);						
		virtual void									vLighting(CCameraContainer* cam, SceneSystem::CLight* lit);

		void											vUpdateVertices(int lod);
		LYNXBOOL										vSetupTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc, int lod = 0, LYNXBOOL bapply_world = LYNX_TRUE);
		void											vReleaseTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc);
		LYNXBOOL										vSetupHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc, int lod = 0, LYNXBOOL bapply_world = LYNX_TRUE);
		void											vReleaseHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc);

		LYNXBOOL										vSave(CStream& s);
		LYNXBOOL										vLoad(CStream& stream, LYNXBOOL skip = LYNX_FALSE);

		void											TempSetupMaterial();
	protected:
		LYNXBOOL										LoadGEO(CStream* stream, const CString& name, LYNXBOOL bloadmaterial, const CString& mat_name, int lod = 0, LYNXBOOL bbo = LYNX_TRUE);

	friend class SceneSystem::CScene;		
	};
}

#endif