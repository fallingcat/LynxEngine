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

#ifndef __LYNXMESHCONTAINER_H__
#define __LYNXMESHCONTAINER_H__

#include <LynxArray.h>
#include <Container/LynxContainer.h>
#include <Container/LynxBoneContainer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

#ifdef __TOOL__
	namespace LynxEditor
	{
		namespace Kernel
		{
			class CMeshContainer;
		}
	}
#endif

namespace LynxEngine 
{
	class CEngine;
	class CModelContainer;	

	class LYNXENGCLASS CMeshContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CMeshContainer);
	public:
		static const int RIGID_ANIMATION =		0X0001;
		static const int BLEND_ANIMATION =		0X0002;
		static const int MORPH_ANIMATION =		0X0004;	

		static const int MAX_BONE_PER_MESH =	64;

		typedef struct{
			int									BoneIndex;	
			int									ConstantIndex;	
		}BONESHADERDATA;	

		struct MeshData : public CUnknown{
			CArray< CArray<LYNXVECTOR3D> >		VertexList;
			CArray< CArray<LYNXVECTOR3D> >		VertexTangentList;
			CArray< CArray<LYNXVECTOR3D> >		VertexBinormalList;
			CArray< CArray<LYNXVECTOR3D> >		VertexNormalList;
			CArray< CArray<LYNXUVPOINT> >		UVVertexList;
			CArray< CArray<LYNXCOLORRGBA> >		VertexColorList;		
			CArray< CArray<CBoneContainer::WEIGHTDATA> > WeightList;		
			CArray< CArray<LYNXSIMPLETRIANGLE> > TriangleList;
			CArray< CArray<GraphicsSystem::CRenderer::PRIMITIVE> > Primitives;	

			MeshData()
			{
				VertexList.clear();
				VertexTangentList.clear();
				VertexBinormalList.clear();
				VertexNormalList.clear();
				UVVertexList.clear();
				VertexColorList.clear();
				WeightList.clear();
				TriangleList.clear();
				Primitives.clear();
			}

			~MeshData()
			{
				VertexList.clear();
				VertexTangentList.clear();
				VertexBinormalList.clear();
				VertexNormalList.clear();
				UVVertexList.clear();
				VertexColorList.clear();
				WeightList.clear();
				TriangleList.clear();
				Primitives.clear();
			}
		};

		static LYNXMATRIX						m_BoneMatrixArray[MAX_BONE_PER_MESH];
		// temp
		int										m_MaterialIndex;
	protected:					
		int										m_AnimationFlag;		
		WORD									m_DynamicAttr;

		CCountedPtr<MeshData>					m_MeshData;
		
		CArray<LYNXVECTOR3D>					m_TVertexList;
		
		GraphicsSystem::BLENDVERTEX*			m_lpBV;	
		GraphicsSystem::RIGIDVERTEX*			m_lpMV;	

		//int*									m_BaseVertex;
		//DWORD*									m_NumVertices;

		//int*									m_BaseTriangle;
		//DWORD*									m_NumTriangles;

		CArray<BONESHADERDATA>					m_BoneShaderData;

		CArray<GraphicsSystem::CVertexArrayPtr>	m_VertexArray;
		CArray<GraphicsSystem::CIndexArrayPtr>	m_IndexArray;		
		
		int										m_VertexConstantIndex;
		PhysicsSystem::CPhysicsObj*				m_lpPhyObj;
		Math::CMatrix4							m_Physics2ContainerM;		
	public:
		CMeshContainer(CEngine *lpengine);
		virtual ~CMeshContainer();		
		
		CMeshContainer&							operator =(const CMeshContainer& rhs);	
		void									vInstance(const CContainer& rhs);
		
		void									vSetNumLODs(int lod);
		void									vAddLOD();

		void									ComputeOBB();

		LYNXINLINE void							SetAnimationFlag(int flag) {m_AnimationFlag = flag; };
		LYNXINLINE int 							GetAnimationFlag() {return m_AnimationFlag; };

		LYNXINLINE CArray<LYNXVECTOR3D>&		GetVertexList(int lod) {return m_MeshData->VertexList[lod]; };
		LYNXINLINE const LYNXVECTOR3D&			GetVertex(int lod, int i) {return m_MeshData->VertexList[lod][i]; };
		LYNXINLINE CArray<LYNXSIMPLETRIANGLE>&	GetTriangleList(int lod) {return m_MeshData->TriangleList[lod]; };
		LYNXINLINE const LYNXSIMPLETRIANGLE&	GetTriangle(int lod, int i) {return m_MeshData->TriangleList[lod][i]; };
		LYNXINLINE CArray<GraphicsSystem::CRenderer::PRIMITIVE>& GetPrimitiveList(int lod) {return m_MeshData->Primitives[lod]; };
		LYNXINLINE const GraphicsSystem::CRenderer::PRIMITIVE& GetPrimitive(int lod, int i) {return m_MeshData->Primitives[lod][i]; };
		LYNXINLINE GraphicsSystem::CVertexArray* GetVertexArray(int lod) {return m_VertexArray[lod]; };
		LYNXINLINE GraphicsSystem::CIndexArray*	GetIndexArray(int lod) {return m_IndexArray[lod]; };

		void									UpdateShaderConstant();
		CArray<BONESHADERDATA>&					GetBoneShaderData() {return m_BoneShaderData; };

		void									SetupMaterial(MaterialSystem::CMaterial* lpm);
		void									SetupRigidVertex(int LOD);
		void									SetupBlendVertex(int LOD);
		LYNXBOOL								Setup(int LOD);

		virtual LYNXBOOL						vLoad(const char *name, int group_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL busevertexshader = LYNX_TRUE) {return LYNX_TRUE; };		
		virtual void							vRelease() {};		
		virtual void							vRender(CCameraContainer* cam = NULL);
		virtual void							vLighting(CCameraContainer* cam, SceneSystem::CLight* lit);
		
		LYNXBOOL								vSetupTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc, int lod = 0, LYNXBOOL bapply_world = LYNX_TRUE);
		void									vReleaseTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc);
		LYNXBOOL								vSetupHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc, int lod = 0, LYNXBOOL bapply_world = LYNX_TRUE);
		void									vReleaseHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc);

		virtual LYNXBOOL						vSetCurrentFrame(int id, LYNXBOOL bforcereset = FALSE) { return LYNX_TRUE; };
		virtual LYNXBOOL						vSetCurrentFrame(char *name, LYNXBOOL bforcereset = FALSE) { return LYNX_TRUE; };

		LYNXINLINE virtual LYNXBOOL				vPlay(CReal skip, LYNXPLAYMODE mode) {return  LYNX_TRUE; };
		LYNXINLINE virtual LYNXBOOL				vPlay(CReal skip) {return  LYNX_TRUE;};				

		// Physics
		LYNXFORCEINLINE PhysicsSystem::CPhysicsObj*	GetlpPhyObj() {return m_lpPhyObj; };
		PhysicsSystem::CPhysicsObj*				CreatePhyObj(PhysicsSystem::CPhysicsObj::TYPE t, PhysicsSystem::CPhysicsShape* g);
		LYNXFORCEINLINE void					SetPhysics2ContainerM(Math::CMatrix4& m) {m_Physics2ContainerM = m; };
	protected:
		int										GetVertexRelatedBoneConstantIndex(int bi);
		void									vUpdateVertices(int lod);

		friend class CModelContainer;		
	};
}
#endif