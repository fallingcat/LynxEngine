//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXPHYSICSSHAPE_H__
#define __LYNXPHYSICSSHAPE_H__

#include <LynxGameObj.h>
#include <LynxEngineClasses.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{
	namespace PhysicsSystem 
	{
		class LYNXENGCLASS CShapeDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CShapeDesc);	
		public:						
			CMatrix4				m_LocalM;
			CMatrix4				m_TransformM;

			CShapeDesc() {m_LocalM.Identity(); m_TransformM.Identity(); };
		};

		class LYNXENGCLASS CBoxShapeDesc : public CShapeDesc
		{	
			LYNX_DECLARE_CLASS(CBoxShapeDesc);	
		public:						
			CVector3				m_Center;
			CVector3				m_Extension;

			CBoxShapeDesc() {m_Center = CVector3(0.0f, 0.0f, 0.0f); m_Extension = CVector3(1.0f, 1.0f, 1.0f); };
		};	

		class LYNXENGCLASS CSphereShapeDesc : public CShapeDesc
		{	
			LYNX_DECLARE_CLASS(CSphereShapeDesc);	
		public:	
			CVector3				m_Center;
			CReal					m_Radius;	

			CSphereShapeDesc() {m_Center = CVector3(0.0f, 0.0f, 0.0f);  m_Radius = 1.0f; };
		};

		class LYNXENGCLASS CPlaneShapeDesc : public CShapeDesc
		{	
			LYNX_DECLARE_CLASS(CPlaneShapeDesc);	
		public:	
			CVector3				m_Normal;
			CReal					m_Distance;	

			CPlaneShapeDesc() {m_Normal = CVector3(0.0f, 1.0f, 0.0f);  m_Distance = 0.0f; };
		};

		class LYNXENGCLASS CCapsuleShapeDesc : public CShapeDesc
		{	
			LYNX_DECLARE_CLASS(CCapsuleShapeDesc);	
		public:	
			CVector3				m_Center;
			CReal					m_Radius;	
			CReal					m_Length;	

			CCapsuleShapeDesc() {m_Center = CVector3(0.0f, 0.0f, 0.0f);  m_Radius = 1.0f; m_Length = 1.0f; };
		};

		class LYNXENGCLASS CHullShapeDesc : public CShapeDesc
		{	
			LYNX_DECLARE_CLASS(CHullShapeDesc);	
		public:	
			DWORD					m_NumVertices;
			DWORD					m_VertexStride;
			void*					m_VertexList;
			
			CHullShapeDesc() {m_NumVertices = 0; m_VertexStride = 0; m_VertexList = NULL; };
		};

		class LYNXENGCLASS CTriMeshShapeDesc : public CShapeDesc
		{	
			LYNX_DECLARE_CLASS(CTriMeshShapeDesc);	
		public:	
			DWORD					m_NumVertices;
			DWORD					m_VertexStride;
			void*					m_VertexList;
			DWORD					m_NumTriangles;
			DWORD					m_TriangleStride;
			void*					m_TriangleList;		

			CTriMeshShapeDesc() {m_NumVertices = 0; m_VertexStride = 0; m_VertexList = NULL; m_NumTriangles = 0; m_TriangleStride = 0; m_TriangleList = NULL; };
		};

		class LYNXENGCLASS CHeightFieldShapeDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CHeightFieldShapeDesc);	
		public:	
			DWORD					m_NumX;
			DWORD					m_NumZ;
			CReal					m_SizeX;
			CReal					m_SizeZ;			
			CReal*					m_HeightMap;			

			CHeightFieldShapeDesc() {m_NumX = 0; m_NumZ = 0; m_HeightMap = NULL; };
		};

		class LYNXENGCLASS CPhysicsShape : public CGameObj
		{	
		public:
			enum TYPE{
				NULL_SHAPE = -1,
				BOX,
				SPHERE,	
				CIRCLE = SPHERE,
				CAPSULE,	
				CYLINDER,
				PLANE,
				HULL,
				TRIMESH,
				HEIGHTFIELD,
				NUM_TYPE,	
			};
		protected:
			TYPE							m_PhysicsShapeType;	
			CContainer*						m_lpTargetContainer;
			CPhysicsSpace*					m_lpSpace;	
			CPhysicsObj*					m_lpObj;
			CMatrix4						m_M;
			CMatrix4						m_LocalM;
		public:
			LYNX_DECLARE_CLASS(CPhysicsShape);
			CPhysicsShape(CPhysicsSpace* const lps);
			virtual ~CPhysicsShape();

			LYNXINLINE TYPE					GetPhysicShapeType() const {return m_PhysicsShapeType; };

			LYNXFORCEINLINE CPhysicsSpace* const GetlpSpace() {return m_lpSpace; };

			LYNXFORCEINLINE CContainer*		GetlpTargetContainer() const {return m_lpTargetContainer;};
			LYNXFORCEINLINE void			SetlpTargetContainer(CContainer* lpc) {m_lpTargetContainer = lpc; };

			virtual LYNXBOOL				vCreate(const CCreationDesc* desc, CPhysicsObj* lpo = NULL) = 0;
			virtual void					vDestory() = 0;
			virtual void					vSetSize(LYNXREAL* lpdata) = 0;			
			virtual int						vGetSize(LYNXREAL* lpdata) const = 0;
			virtual void					vSetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z) = 0;
			virtual void					vSetPosition(LPLYNXVECTOR3D lppos) = 0;
			virtual void					vSetTransformMatrix(const CMatrix4& m) = 0;
			virtual CMatrix4&				vGetTransformMatrix() = 0;
			virtual void					vSetLocalTransformMatrix(const CMatrix4& m) = 0;			
			virtual CMatrix4&				vGetLocalTransformMatrix() = 0;			
			void							SetlpObj(CPhysicsObj* const lpo) {m_lpObj = lpo; };
			LYNXINLINE CPhysicsObj*			GetlpObj() const {return m_lpObj; };			

			virtual int						vGetNumVertices() {return 0;};
			virtual int						vGetNumTriangles() {return 0;};
			virtual float*					vGetVertices() {return NULL;};
			virtual int*					vGetTriangles() {return NULL;};

			LYNXBOOL						vSave(CStream& stream);
			static CCreationDesc*			LoadDesc(TYPE& type, CStream& s, LYNXBOOL skip);			

			friend class CPhysicsObj;
		};	
	}
	LYNXENGCLASS CString	GetEnumStr(PhysicsSystem::CPhysicsShape::TYPE e);
}

#endif