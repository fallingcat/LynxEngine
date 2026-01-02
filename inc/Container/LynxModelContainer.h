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

#ifndef __LYNXMODELCONTAINER_H__
#define __LYNXMODELCONTAINER_H__

#include <Container/LynxContainer.h>
#include <Container/LynxBoneContainer.h>
#include <Container/LynxMeshContainer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CModelCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CModelCreationDesc);	
	public:							
		LYNXBOOL					m_bUseBufferObject;

		CModelCreationDesc() {Default(); };
		void Default() {CCreationDesc::Default(); m_bUseBufferObject = LYNX_TRUE; };

		CModelCreationDesc&		operator =(const CModelCreationDesc& rhs)
		{
			*((CCreationDesc*)this) = (CCreationDesc&)rhs;
			m_bUseBufferObject	= rhs.m_bUseBufferObject;
			
			return (*this);
		}
	};		

	class LYNXENGCLASS CModelContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CModelContainer);
	public:
		#define BONE_START_CONSTANT						0

		#define UPDATE_TANGENT							0X0001
		#define UPDATE_BINORMAL							0X0002
		#define UPDATE_NORMAL							0X0004
		#define VERTEXANIMATION_INTERPOLATE				0X0008

		enum MODELTYPE{
			RIGID_MODEL = 0,
			VERTEX_MODEL,
			SKINNING_MODEL,
		};				

		int									m_CurrentLOD;		
	protected:					
		MODELTYPE							m_ModelType;
		int									m_Attribute;
		LYNXBOOL							m_bUseBufferObject;						
		
		GraphicsSystem::BLENDVERTEX*		m_lpBV;	
		GraphicsSystem::RIGIDVERTEX*		m_lpMV;		

		CArray<LYNXVECTOR3D>				m_TVertexList;

		DECLAR_PARAM_ARRAY(CContainer*, m_lpMeshArray)
		DECLAR_PARAM_ARRAY(CBoneContainer*, m_lpBoneArray)		
		CArray<CContainer*>					m_lpTraverseArray;
		CArray<CContainer*>					m_RenderArray;

		//temp
		//CArray<MaterialSystem::CMaterial*>	m_lpMaterials;
		CArray<MaterialSystem::CMaterialPtr>	m_lpMaterials;									
	public:
		CModelContainer(CEngine *lpengine);		
		virtual  ~CModelContainer();		

		CModelContainer&					operator =(const CModelContainer& rhs);				
		virtual void						vInstance(const CContainer& rhs);
		
		LYNXINLINE void						SetAttribute(int attr) { m_Attribute = attr; };
		LYNXINLINE LYNXBOOL					UseBufferObject() {return m_bUseBufferObject; };
		LYNXINLINE MODELTYPE				GetModelType() {return m_ModelType; };

		void								vSetNumLODs(int lod);	
		void								vAddLOD();
		
		void								vUseVisibility(LYNXBOOL b);

		CMeshContainer* 					CreateMeshContainer();
		CBoneContainer* 					CreateBoneContainer();
		virtual LYNXBOOL					vCreate(const CCreationDesc* desc);
		virtual void						vCreateDefaultResource();
		LYNXBOOL							LoadMDL(CStream& stream, const CString& name, const LYNXBOOL bloadmaterial, const CString& mat_name, const int lod = 0, const LYNXBOOL bbo = LYNX_TRUE);
		//virtual LYNXBOOL					vCreate(const LYNXCHAR *name, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_TRUE);
		virtual Animation::CAnimation*		vLoadAnimation(const CCreationDesc* desc);
		virtual void						vAfterLoad();
		virtual void						vRender(CCameraContainer* cam = NULL);

		//virtual LYNXBOOL					vSetCurrentFrame(int id, LYNXBOOL bforcereset = FALSE);
		//virtual LYNXBOOL					vSetCurrentFrame(char *name, LYNXBOOL bforcereset = FALSE);
		
		virtual void						vUpdateVertices(int lod);
		virtual LYNXBOOL					vSetupTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc, int lod = 0, LYNXBOOL bapply_world = LYNX_FALSE);
		virtual void						vReleaseTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc);
		virtual LYNXBOOL					vSetupHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc, int lod = 0, LYNXBOOL bapply_world = LYNX_FALSE);
		virtual void						vReleaseHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc);

		//virtual LYNXBOOL					vPlay(LYNXREAL skip, LYNXPLAYMODE mode);
		virtual LYNXBOOL					vPlay(CReal skip);
		virtual void						vRagDollUpdate(OBJUPDATETYPE type);

		LYNXINLINE DWORD					GetNumMaterials() {return (DWORD)m_lpMaterials.size();};
		LYNXINLINE MaterialSystem::CMaterialPtr& GetMaterial(int i) {return m_lpMaterials[i];};
		MaterialSystem::CMaterial*			GetMaterial(const CString& name);
		LYNXINLINE DWORD					GetNumMeshs() {return (DWORD)m_lpMeshArray.size();};
		void								AddMesh(CContainer* mesh);
		void								RemoveAllMeshes();		
		void								RemoveMesh(CContainer* mesh);		
		void								RemoveMesh(const CString& name);
		LYNXINLINE CContainer*				GetlpMesh(int index) {return m_lpMeshArray[index];};
		CContainer*							GetlpMesh(const CString& name);
		int									GetMeshIndex(const CString& name);		
		LYNXINLINE DWORD					GetNumBones() {return (DWORD)m_lpBoneArray.size();};
		LYNXINLINE CContainer*				GetlpBone(int index) {return m_lpBoneArray[index];};
		CContainer*							GetlpBone(const CString& name);
		int									GetBoneIndex(const CString& name);		
		CContainer*							GetlpContainer(const CString& name);

		void								CopyHierachy(const CContainer& rhs);
		void								MakeTraverseArray();

		void								CreateRenderArray();
		void								SortMesh(CCameraContainer* cam = NULL);

		//LYNXINLINE virtual AMS::CAdvMaterial*			vGetlpCurrentMaterial(int i) const {return (m_lpMesh[i].m_lpCurrentMaterial);};
		//virtual void									vChangeCurrentMaterial(int index, AdvMaterialSystem::CAdvMaterial* const lpm);

		//LYNXINLINE void									SetModelAttribute(int a) {lynxSetModelAttribute((LPLYNXMODEL)this, a); };
		//LYNXINLINE int									GetModelAttribute(void) const {return this->Attribute; };
		//LYNXINLINE void									AddModelAttribute(int a) {lynxSetModelAttribute((LPLYNXMODEL)this, (GetModelAttribute()|a)); };				
	protected:		
		void								CorrectWeightValue(CBoneContainer::WEIGHTDATA& w);

		LYNXBOOL							Setup(int lod = -1);
	};
}

#endif