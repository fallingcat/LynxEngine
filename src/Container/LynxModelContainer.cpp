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

#include <LynxEngine_PCH.h>
#include <LynxAlgorithm.h>
#include <LynxEngine.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <Animation/LynxAnimation.h>
#include <Animation/LynxAnimationKeys.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Container/LynxModelContainer.h>
#include <LynxBoxProxy.h>
#include <CollisionSystem/LynxCollisionHierarchy.h>
#include <CollisionSystem/LynxAABB.h>
#include <CollisionSystem/LynxSphere.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CModelContainer::CModelContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_ContainerType = MODEL;	
		m_bRenderable = LYNX_FALSE;
		m_CurrentLOD = 0;
		m_Attribute = 0x0;	
		m_bUseBufferObject = LYNX_FALSE;

		m_lpMeshArray.clear();		
		m_lpBoneArray.clear();			
		m_lpTraverseArray.clear();
		m_lpMaterials.clear();

		IMPLEMENT_PARAM_ARRAY(m_lpMeshArray, (CParam::LPOBJ), 0)
		IMPLEMENT_PARAM_ARRAY(m_lpBoneArray, (CParam::LPOBJ), 0)		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CModelContainer::~CModelContainer(void)
	{		
		CArray<CContainer *>::CIterator Mesh;
		CArray<CBoneContainer *>::CIterator Bone;

		for (Mesh = m_lpMeshArray.begin(); Mesh != m_lpMeshArray.end(); ++Mesh)
			m_lpEngine->DestroyContainer(*Mesh);
		m_lpMeshArray.clear();

		for (Bone = m_lpBoneArray.begin(); Bone != m_lpBoneArray.end(); ++Bone)
			m_lpEngine->DestroyContainer(*Bone);
		m_lpBoneArray.clear();
		
		m_RenderArray.clear();
		m_lpTraverseArray.clear();
		m_lpMaterials.clear();
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CMeshContainer* CModelContainer::CreateMeshContainer() 
	{
		return Container_Cast<CMeshContainer*>(m_lpEngine->CreateContainer(CContainer::MESH)); 
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CBoneContainer* CModelContainer::CreateBoneContainer() 
	{
		return Container_Cast<CBoneContainer*>(m_lpEngine->CreateContainer(CContainer::BONE)); 
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CModelContainer& CModelContainer::operator =(const CModelContainer& rhs)
	{		
		//lynxDuplicateModel(this, &m);
		//
		///*
		//m_MaterialArray.resize(m.m_MaterialArray.size());				
		//for (int i=0; i<m_MaterialArray.size(); i++)
		//{
		//	((CEngine *)m_lpEngine)->GetlpMaterialSystem()->AddMaterial(&m_MaterialArray[i]);			
		//	m_MaterialArray[i] = m.m_MaterialArray[i];						
		//}
		//*/
		//
		//m_lpMeshArray.resize(NumMeshs);		
		//for (int i=0; i<m_lpMeshArray.size(); i++)
		//{
		//	Container_Cast<CMeshContainer*>(m_lpMeshArray[i]) = m.Container_Cast<CMeshContainer*>(m_lpMeshArray[i]);						
		//}

		//m_lpBoneArray.resize(NumBones);		
		//for (int i=0; i<m_lpBoneArray.size(); i++)
		//	Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->SetlpBone(&lpBones[i]);

		//m_SubContainerArray.resize(m.m_SubContainerArray.size());		
		//for (int i=0; i<m_SubContainerArray.size(); i++)
		//{
		//	m_SubContainerArray[i] = m.m_SubContainerArray[i];						
		//}

		////(((CEngine *)m_lpEngine)->GetSDK()).AddModel(this);		

		return *this;
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vInstance(const CContainer& rhs)
	{		
		CContainer::vInstance(rhs);

		const CModelContainer* RHS = Container_Cast<const CModelContainer*>(&rhs);
		m_ModelType = RHS->m_ModelType;		
		m_ContainerType = RHS->m_ContainerType;		
		m_Attribute = RHS->m_Attribute;	
		m_bUseBufferObject = RHS->m_bUseBufferObject;		

		m_lpMeshArray.resize(RHS->m_lpMeshArray.size());
		for (int m=0; m<m_lpMeshArray.size(); m++)
		{
			m_lpMeshArray[m] = m_lpEngine->CreateContainer(RHS->m_lpMeshArray[m]->GetContainerType());			
			m_lpMeshArray[m]->SetlpEngine(m_lpEngine);
			m_lpMeshArray[m]->SetlpParentContainer(this);
			m_lpMeshArray[m]->vInstance(*(RHS->m_lpMeshArray[m]));
		}		
		for (int m=0; m<m_lpMeshArray.size(); m++)
		{
			if (m_lpMeshArray[m]->GetlpParent())
				m_lpMeshArray[m]->SetlpParent(GetlpMesh(m_lpMeshArray[m]->GetlpParent()->GetName()));
			for (int c=0; c<m_lpMeshArray[m]->GetNumChildren(); c++)			
				m_lpMeshArray[m]->SetChild(c, GetlpMesh(m_lpMeshArray[m]->GetlpChild(c)->GetName()));				
		}				

		m_lpBoneArray.resize(RHS->m_lpBoneArray.size());
		for (int b=0; b<m_lpBoneArray.size(); b++)
		{
			m_lpBoneArray[b] = CreateBoneContainer();			
			m_lpBoneArray[b]->SetlpEngine(m_lpEngine);
			m_lpBoneArray[b]->vInstance(*(RHS->m_lpBoneArray[b]));
		}
		for (int b=0; b<m_lpBoneArray.size(); b++)
		{
			if (Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpParent)
				Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpParent = GetlpBone(Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpParent->GetName());
			for (int c=0; c<Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpChildrenArray.size(); c++)			
				Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpChildrenArray[c] = GetlpBone(Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpChildrenArray[c]->GetName());				
		}		

		for (int i=0; i<m_lpChildrenArray.size(); i++)
		{
			CContainer* lpC = GetlpBone(m_lpChildrenArray[i]->GetName());
			if (!lpC)
				lpC = GetlpMesh(m_lpChildrenArray[i]->GetName());
			m_lpChildrenArray[i] = lpC;
			lpC->SetlpParent(this);
		}
		MakeTraverseArray();				
		CreateRenderArray();

		m_SubContainerList.clear();
		for (int i=0; i<m_lpMeshArray.size(); i++)
		{	
			AddSubContainer(m_lpMeshArray[i]); 
		}		
		for (int i=0; i<m_lpBoneArray.size(); i++)
		{			
			AddSubContainer(Container_Cast<CBoneContainer*>(m_lpBoneArray[i])); 
		}		
		m_lpBV = RHS->m_lpBV;
		m_lpMV = RHS->m_lpMV;
	}			
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::CopyHierachy(const CContainer& rhs)
	{		
		for (int m=0; m<m_lpMeshArray.size(); m++)
		{
			m_lpMeshArray[m]->SetlpParentContainer(this);			
		}		
		for (int m=0; m<m_lpMeshArray.size(); m++)
		{
			if (m_lpMeshArray[m]->GetlpParent())
				m_lpMeshArray[m]->SetlpParent(GetlpMesh(m_lpMeshArray[m]->GetlpParent()->GetName()));

			for (int c=0; c<m_lpMeshArray[m]->GetNumChildren(); c++)			
				m_lpMeshArray[m]->SetChild(c, GetlpMesh(m_lpMeshArray[m]->GetlpChild(c)->GetName()));				
		}		
		for (int b=0; b<m_lpBoneArray.size(); b++)
		{
			if (Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpParent)
				Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpParent = GetlpBone(Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpParent->GetName());
			for (int c=0; c<Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpChildrenArray.size(); c++)			
				Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpChildrenArray[c] = GetlpBone(Container_Cast<CBoneContainer*>(m_lpBoneArray[b])->m_lpChildrenArray[c]->GetName());				
		}		

		for (int i=0; i<m_lpChildrenArray.size(); i++)
		{
			CContainer* lpC = GetlpBone(m_lpChildrenArray[i]->GetName());
			if (!lpC)
				lpC = GetlpMesh(m_lpChildrenArray[i]->GetName());
			m_lpChildrenArray[i] = lpC;
			lpC->SetlpParent(this);
		}
		MakeTraverseArray();				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vUseVisibility(LYNXBOOL b)
	{
		m_bUseVisibilty = b;
		for (int i=0; i<m_lpMeshArray.size(); i++)
		{	
			m_lpMeshArray[i]->m_bUseVisibilty = b; 
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer* CModelContainer::GetlpMesh(const CString& name)
	{
		CArray<CContainer*>::CIterator Mesh;

		Mesh = LynxEngine::find(m_lpMeshArray.begin(), m_lpMeshArray.end(), name);
		if (Mesh != m_lpMeshArray.end())
			return (*Mesh);				

		CString Name = m_Name + CString(".") + name;
		Mesh = LynxEngine::find(m_lpMeshArray.begin(), m_lpMeshArray.end(), Name);
		if (Mesh != m_lpMeshArray.end())
			return (*Mesh);				

		return NULL;				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	int	CModelContainer::GetMeshIndex(const CString& name)
	{
		for (int i=0; i<m_lpMeshArray.size(); i++)
		{
			if (m_lpMeshArray[i]->GetName() == name)
				return i;
		}
		return -1;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer* CModelContainer::GetlpBone(const CString& name)
	{
		CArray<CBoneContainer*>::CIterator Bone;

		Bone = LynxEngine::find(m_lpBoneArray.begin(), m_lpBoneArray.end(), name);
		if (Bone == m_lpBoneArray.end())
			return NULL;

		return (*Bone);				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	int	CModelContainer::GetBoneIndex(const CString& name)
	{
		for (int i=0; i<m_lpBoneArray.size(); i++)
		{
			if (m_lpBoneArray[i]->GetName() == name)
				return m_lpBoneArray[i]->GetBoneIndex();
		}
		return -1;
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer* CModelContainer::GetlpContainer(const CString& name)
	{
		CContainer*lpMesh = GetlpMesh(name);
		if (!lpMesh)
		{
			CContainer*lpBone = GetlpBone(name);
			if (lpBone)
				return lpBone;
			else 
				return NULL;
		}
		return lpMesh;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vSetNumLODs(int lod)
	{
		m_NumLODs = lod;
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			Container_Cast<CMeshContainer*>((*Sub))->vSetNumLODs(lod);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vAddLOD()
	{
		m_NumLODs++;
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			Container_Cast<CMeshContainer*>((*Sub))->vAddLOD();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CModelContainer::Setup(int lod)
	{
		if (lod < 0)
		{
			for (int LOD=0; LOD<m_NumLODs; LOD++)
			{
				for (int i=0; i<m_lpMeshArray.size(); i++)
				{
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->Setup(LOD);
				}
			}
		}
		else
		{
			for (int i=0; i<m_lpMeshArray.size(); i++)
			{
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->Setup(lod);
			}
		}
		return TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CModelContainer::vSetupTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc, int lod, LYNXBOOL bapply_world)
	{
		if (m_lpMeshArray[0]->GetContainerType() == CContainer::MESH)
		{
			int LOD = lod;
			if (LOD >= m_NumLODs)
				LOD = m_NumLODs - 1;

			int NumV = 0;
			for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
				NumV += Container_Cast<CMeshContainer*>((*Sub))->GetVertexList(LOD).size();

			m_TVertexList.clear();
			m_TVertexList.resize(NumV);

			PushTransform();
			Math::CMatrix4 M;
			M.Identity();
			SetTransformMatrix(M);
			vPlay(0);
			vUpdateVertices(LOD);
			PopTransform();
			vPlay(0);

			desc->m_NumVertices = m_TVertexList.size();
			desc->m_VertexStride = sizeof(LYNXVECTOR3D);
			desc->m_VertexList = &m_TVertexList[0].x;
			desc->m_NumTriangles = Container_Cast<CMeshContainer*>(m_lpMeshArray[0])->m_MeshData->TriangleList[LOD].size();
			desc->m_TriangleStride = sizeof(LYNXSIMPLETRIANGLE);
			desc->m_TriangleList = &(Container_Cast<CMeshContainer*>(m_lpMeshArray[0]))->m_MeshData->TriangleList[LOD][0].v[0];

			return LYNX_TRUE;
		}
		else
		{
			return LYNX_FALSE;
		}

		//return Container_Cast<CMeshContainer*>(m_lpMeshArray[0])->vSetupTriMeshShapeDesc(desc, lod);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vReleaseTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc)
	{		
		return Container_Cast<CMeshContainer*>(m_lpMeshArray[0])->vReleaseTriMeshShapeDesc(desc);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CModelContainer::vSetupHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc, int lod, LYNXBOOL bapply_world)
	{
		int LOD = lod;
		if (LOD >= m_NumLODs)
			LOD = m_NumLODs - 1;

		int NumV = 0;
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			NumV += Container_Cast<CMeshContainer*>((*Sub))->GetVertexList(LOD).size();

		m_TVertexList.clear();
		m_TVertexList.resize(NumV);

		PushTransform();
		Math::CMatrix4 M;
		M.Identity();
		SetTransformMatrix(M);
		vPlay(0);
		vUpdateVertices(LOD);
		PopTransform();
		vPlay(0);

		desc->m_NumVertices = m_TVertexList.size();
		desc->m_VertexStride = sizeof(LYNXVECTOR3D);
		desc->m_VertexList = &m_TVertexList[0].x;
		
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vReleaseHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc)
	{		
		m_TVertexList.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::CorrectWeightValue(CBoneContainer::WEIGHTDATA& w)
	{
		LYNXREAL				Total, Offset;
		int						i;

		Total = 0.0f;
		for (i=0; i<w.NumBones; i++)
		{
			Total += w.lpWeight[i];
		}
		if (Total > 1.0f)
		{
			Offset = Total - 1.0f;
			Offset /= w.NumBones;
			for (i=0; i<w.NumBones; i++)
			{
				w.lpWeight[i] -= Offset;
			}
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::AddMesh(CContainer* mesh)
	{
		m_lpMeshArray.push_back(mesh);
		AddSubContainer(mesh);

		MakeTraverseArray();				
		CreateRenderArray();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::RemoveAllMeshes()
	{
		CArray<CContainer*>::CIterator Mesh;

		Mesh = m_lpMeshArray.begin();
		while (Mesh != m_lpMeshArray.end())
		{			
			RemoveSubContainer((*Mesh));
			Mesh++;			
		}
		m_lpMeshArray.clear();

		m_lpTraverseArray.clear();
		m_lpTraverseArray.push_back(this);
		CreateRenderArray();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::RemoveMesh(CContainer* mesh)
	{
		CArray<CContainer*>::CIterator Mesh;

		Mesh = LynxEngine::find(m_lpMeshArray.begin(), m_lpMeshArray.end(), mesh);
		if (Mesh != m_lpMeshArray.end())
		{
			m_lpMeshArray.erase(Mesh);
			RemoveSubContainer(mesh);
		}
		MakeTraverseArray();				
		CreateRenderArray();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::RemoveMesh(const CString& name)
	{
		CContainer* Mesh = GetlpMesh(name);
		RemoveMesh(Mesh);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::MakeTraverseArray()
	{	
		CContainer::MakeTraverseArray(m_lpTraverseArray, (int)(m_lpBoneArray.size()+m_lpMeshArray.size())+1, this);				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::CreateRenderArray()
	{
		m_RenderArray.clear();		
		if (m_lpTraverseArray.size() > 1)
		{
			m_RenderArray.resize(m_lpTraverseArray.size()-1);
			for (int i=1; i<m_lpTraverseArray.size(); i++)
			{
				m_RenderArray[i-1] = m_lpTraverseArray[i];
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CModelContainer::LoadMDL(CStream& stream, const CString& filename, const LYNXBOOL bloadmaterial, const CString& mat_name, const int lod, const LYNXBOOL bbo)
	{
		int	Type;
		char Mark[] = "LYNXMBS";	
		char Check[8], Name[128];	
		DWORD MainVer, NumBone, NumMesh;
		int	BoneIndex, MeshRoot;		
		DWORD CurrentLOD, Num;
		LYNXCHAR *Token;	
		WORD ObjMatIndex, wNumChildren, ChildIndex, wIndex;					
		LYNXREAL fR, fG, fB, Value[4];
		int	RootIndex;
		LYNXCHAR MName[256], TempMName[256];
		char AnsiName[128];
		LYNXCHAR TexturePath[128];	
		char TextureName[128];	
		int	 PrimitiveType;
		LYNXTRIANGLE TempTri;	
		LYNXBOOL bStripify;
		LYNXREAL KeyValues[4];
		int	ST, ET;
		LYNXCHAR TName[128];
		MaterialSystem::CTechnique* Technique;
		LYNXCHAR MaterialName[128];	
		LYNXCHAR DiffuseName[128];	
		LYNXCHAR NormalName[128];	
		LYNXCHAR SpecularName[128];	
		LYNXCHAR AOName[128];		
		LYNXCHAR CurrentPath[128];
		CString	HieName, MatName;
		GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;			
		CArray<LYNXBOOL> bMaterialSetup;
		float AnimationLength = 0.0f;
		
		LYNX_GUARD(CModelContainer::LoadMBS)

		CurrentLOD = lod;
		if (lod == 0)
			vSetNumLODs(1);

		m_bUseBufferObject = bbo;			
		m_SourceType = ORIGIN;
		MeshRoot = 0;
		RootIndex = 0;
		
		CStream* lpStream = &stream;

		/*
		OPEN_Z_FILE(lpStream, filename)
		else
		{
			if (!lpStream->IsValid())
			{
				if (!lpStream->vOpen(filename, CStream::Binary|CStream::Read))
					return LYNX_FALSE;			
			}
		}
		*/

		PACKAGE_OPEN_FILE(lpStream, filename, CStream::Read | CStream::Binary)
		if (!bResult)
		{
			return LYNX_FALSE;
		}



		/*
		if (lod == 0)
		{
			lynxStrCpy(TempMName, filename);
			Token = lynxStrTok(TempMName, _T("\\/"));
			while(Token)
			{
				Token = lynxStrTok(NULL, _T("\\/"));
				if (Token)
					lynxStrCpy(MName, Token);
			}
			lynxStrTok(MName, _T("."));
			SetName(GetName()+CString(_T("."))+CString(MName));			
			//SetName(filename);
		}
		*/

		lpStream->vRead(&Check, 7, 1);
		Check[7] = 0x00;
		if (strcmp(Mark, Check))	
		{
			lpStream->vClose();
			return FALSE;
		}
		lpStream->vRead(&MainVer, sizeof(DWORD), 1);    
		
		lpStream->vRead(&Num, sizeof(DWORD), 1);		
		if (lod == 0)
		{
			m_lpMaterials.clear();
			m_lpMaterials.resize(Num);
			
			bMaterialSetup.clear();
			bMaterialSetup.resize(Num);
			for (int i=0; i<Num; i++)
				bMaterialSetup[i] = LYNX_FALSE;
		}
		
		CString OldTexturePath(m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath());
		SceneSystem::CScene::GetTexturePathFromModelPath(TexturePath, filename);
		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(TexturePath);

		for (int i=0; i<Num; i++)
		{
			if (bloadmaterial)
			{
				lpStream->vRead(AnsiName, sizeof(char), 64);
				lynxFromAnsiStr(TName, AnsiName);
				if (mat_name.empty())
					MatName = m_Name + CString(_T(".")) + CString(TName);				
				else
					MatName = mat_name + CString(_T(".")) + CString(TName);				

				m_lpMaterials[i] = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MatName); 			
				if (lod == 0 && !m_lpMaterials[i])
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_lpMaterials[i], MaterialSystem::CMaterial::STANDARD);				
					m_lpMaterials[i]->SetName(MatName);								

					lpStream->vRead(Name, sizeof(char), 64);					
					lynxFromAnsiStr(AOName, Name);
					lpStream->vRead(Name, sizeof(char), 64);					
					lynxFromAnsiStr(DiffuseName, Name);
					lpStream->vRead(Name, sizeof(char), 64);					
					lynxFromAnsiStr(NormalName, Name);
					lpStream->vRead(Name, sizeof(char), 64);					
					lynxFromAnsiStr(SpecularName, Name);

					lynxStrCpy(CurrentPath, m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath());

					m_lpMaterials[i]->vSetNumTextureAnimation(1);
					m_lpMaterials[i]->GetTextureAnimation(0)->Create(1);			
					m_lpMaterials[i]->GetTextureAnimation(0)->SetName(m_lpMaterials[i]->GetName()+_T(".DiffuseMap.TexAni"));
					m_lpMaterials[i]->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
					TexDesc.Default();
					TexDesc.bSRGB = LYNX_TRUE;
					
					CStream* File = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpMaterials[i]->GetTextureAnimation(0)->GetTexture(0), *File, DiffuseName, &TexDesc);
					if (!m_lpMaterials[i]->GetTextureAnimation(0)->GetTexture(0) || !m_lpMaterials[i]->GetTextureAnimation(0)->GetTexture(0)->vIsValid())				
					{
						TexDesc.bSRGB = LYNX_FALSE;
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpMaterials[i]->GetTextureAnimation(0)->GetTexture(0), *File, _T("white.png"), &TexDesc);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}
					
				#if 0
					TexDesc.bSRGB = LYNX_FALSE;
					m_lpMaterials[i]->GetTextureAnimation(1)->Create(1);		
					m_lpMaterials[i]->GetTextureAnimation(1)->SetName(m_lpMaterials[i]->GetName()+_T(".NormalMap.TexAni"));
					m_lpMaterials[i]->GetTextureAnimation(1)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpMaterials[i]->GetTextureAnimation(1)->GetTexture(0), *File, NormalName);
					if (!m_lpMaterials[i]->GetTextureAnimation(1)->GetTexture(0) || !m_lpMaterials[i]->GetTextureAnimation(1)->GetTexture(0)->vIsValid())
					{
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpMaterials[i]->GetTextureAnimation(1)->GetTexture(0), *File, _T("dot3.png"), &TexDesc);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}

					TexDesc.bSRGB = LYNX_TRUE;
					m_lpMaterials[i]->GetTextureAnimation(2)->Create(1);		
					m_lpMaterials[i]->GetTextureAnimation(2)->SetName(m_lpMaterials[i]->GetName()+_T(".SpecularMap.TexAni"));
					m_lpMaterials[i]->GetTextureAnimation(2)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SPECULARMAP).c_str());
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpMaterials[i]->GetTextureAnimation(2)->GetTexture(0), *File, SpecularName);
					if (!m_lpMaterials[i]->GetTextureAnimation(2)->GetTexture(0) || !m_lpMaterials[i]->GetTextureAnimation(2)->GetTexture(0)->vIsValid())
					{
						TexDesc.bSRGB = LYNX_FALSE;
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpMaterials[i]->GetTextureAnimation(2)->GetTexture(0), *File, _T("white.png"), &TexDesc);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}

					TexDesc.bSRGB = LYNX_TRUE;
					m_lpMaterials[i]->GetTextureAnimation(3)->Create(1);		
					m_lpMaterials[i]->GetTextureAnimation(3)->SetName(m_lpMaterials[i]->GetName()+_T(".AmbientMap.TexAni"));
					m_lpMaterials[i]->GetTextureAnimation(3)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::AMBIENTMAP).c_str());
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpMaterials[i]->GetTextureAnimation(3)->GetTexture(0), *File, AOName);
					if (!m_lpMaterials[i]->GetTextureAnimation(3)->GetTexture(0) ||  !m_lpMaterials[i]->GetTextureAnimation(3)->GetTexture(0)->vIsValid())
					{
						TexDesc.bSRGB = LYNX_FALSE;
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpMaterials[i]->GetTextureAnimation(3)->GetTexture(0), *File, _T("white.png"), &TexDesc);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}
				#endif
					
					/*
					Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->Create(1);				
					Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->SetTextureName(0, LynxEngine::CString(m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+LynxEngine::CString(AOName));
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->GetTexture(0), 0, AOName, NULL, 0);
					if (!Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->GetTexture(0)->vIsValid())				
					{
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->GetTexture(0), 0, "../texture/white.png", NULL, 0);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}
					*/
					m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
					#if defined(__DESKTOP__) && !defined (__MOBILE_SIMULATION__)
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::WIREFRAME);				
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);				
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);				
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);				
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);				
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);										
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::ZPASS);				
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::SHADOWMAP);				
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::GPASS);			
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::SHADOW_TEXTURE);				
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::ONE_LIGHTING);				
					#elif defined (__MOBILE__) || defined (__MOBILE_SIMULATION__)
						m_lpMaterials[i]->vAddTechnique(MaterialSystem::CMaterial::ONE_LIGHTING);				
					#endif

					Technique = &m_lpMaterials[i]->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);						
					lynxStrCpy(TName, m_lpMaterials[i]->GetName().c_str());
					lynxStrCat(TName, _T("_SIMPLE"));			
					Technique->SetName(TName);
					Technique->vCreatePass(1);
					LynxEngine::MaterialSystem::CPass& Pass = m_lpMaterials[i]->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0);
					Pass.m_TextureAnimationArray = m_lpMaterials[i]->GetTextureAnimationArray();
									
					m_lpMaterials[i]->m_Diffuse.Red = 255;
					m_lpMaterials[i]->m_Diffuse.Green = 255;
					m_lpMaterials[i]->m_Diffuse.Blue = 255;
					m_lpMaterials[i]->m_Diffuse.Alpha = 255;		

					lpStream->vRead(&m_lpMaterials[i]->m_SpPower, sizeof(float), 1);					
				}
				else
				{			
					lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
					lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
					lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
					lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
					lpStream->vSeek(sizeof(float)*1, CStream::SeekCur);					
				}
			}
			else
			{
				lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
				lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
				lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
				lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
				lpStream->vSeek(sizeof(char)*64, CStream::SeekCur);
				lpStream->vSeek(sizeof(float)*1, CStream::SeekCur);
			}
		}		
		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(OldTexturePath);

		//m_lpEngine->GetlpFileSystem()->GetPackage()->ClearInvalidStream();

		AnimationLength = 0.0f;
		lpStream->vRead(&NumMesh, sizeof(DWORD), 1);	
		if (lod == 0)
		{
			m_lpMeshArray.resize(NumMesh);
			for (int i=0; i<m_lpMeshArray.size(); i++)
			{
				m_lpMeshArray[i] = CreateMeshContainer();			
				m_lpMeshArray[i]->SetlpEngine(m_lpEngine);			
			}
		}
		for (int i=0; i<NumMesh; i++)
		{			
			lpStream->vRead(Name, sizeof(char), 64);			
			((LynxEngine::CObj*)m_lpMeshArray[i])->SetName(GetName()+_T(".")+CString(Name));					
			if (lod == 0)
				m_lpMeshArray[i]->vSetNumLODs(1);
			
			lpStream->vRead(&m_lpMeshArray[i]->m_Visibility, sizeof(LYNXREAL), 1);
			lpStream->vRead(&(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex), sizeof(int), 1);			
			lpStream->vRead(&Type, sizeof(int), 1);			
			switch (Type)
			{
				case 0:
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetAnimationFlag(CMeshContainer::RIGID_ANIMATION);
					break;
				case 1:
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetAnimationFlag(CMeshContainer::BLEND_ANIMATION);
					break;
				case 2:
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetAnimationFlag(CMeshContainer::MORPH_ANIMATION);
					break;
			}			
			lpStream->vRead(&Num, sizeof(DWORD), 1);

			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexList[CurrentLOD].resize(Num);			
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexTangentList[CurrentLOD].resize(Num);			
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexBinormalList[CurrentLOD].resize(Num);			
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexNormalList[CurrentLOD].resize(Num);			
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD].resize(Num);			
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->UVVertexList[CurrentLOD].resize(Num);	
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD].resize(Num);	

			switch (Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_AnimationFlag)
			{
				case CMeshContainer::BLEND_ANIMATION:
					for (int j=0; j<Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexList[CurrentLOD].size(); j++)
					{
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpBoneIndex = NULL;
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpWeight = NULL;
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpOffset = NULL;			

						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexTangentList[CurrentLOD][j].x, sizeof(LYNXVECTOR3D), 1);				
						LYNX_VECTOR3D_SET_W(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexTangentList[CurrentLOD][j], 1.0f);
						lynxNormalise(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexTangentList[CurrentLOD][j], 3);		

						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexBinormalList[CurrentLOD][j].x, sizeof(LYNXVECTOR3D), 1);				
						LYNX_VECTOR3D_SET_W(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexBinormalList[CurrentLOD][j], 1.0f);
						lynxNormalise(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexBinormalList[CurrentLOD][j], 3);		

						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexNormalList[CurrentLOD][j].x, sizeof(LYNXVECTOR3D), 1);				
						LYNX_VECTOR3D_SET_W(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexNormalList[CurrentLOD][j], 1.0f);
						lynxNormalise(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexNormalList[CurrentLOD][j], 3);	

						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->UVVertexList[CurrentLOD][j], sizeof(LYNXUVPOINT), 1);						
						// Default vertex color
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].PackedColor = 0xffffffff;

						if (MainVer >= 400)
						{
							float Color[4];
							lpStream->vRead(Color, sizeof(float), 4);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].Red = LYNX_ROUND(Color[0]*255.0f);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].Green = LYNX_ROUND(Color[1]*255.0f);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].Blue = LYNX_ROUND(Color[2]*255.0f);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].Alpha = LYNX_ROUND(Color[3]*255.0f);
						}
												
						lpStream->vRead(&NumBone, sizeof(DWORD), 1);
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].NumBones = NumBone;
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpBoneIndex = LYNXGLOBAL_NEW BYTE[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].NumBones];
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpWeight = LYNXGLOBAL_NEW LYNXREAL[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].NumBones];
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpOffset = LYNXGLOBAL_NEW LYNXVECTOR3D[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].NumBones];
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpOffsetNormal = LYNXGLOBAL_NEW LYNXVECTOR3D[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].NumBones];
						if (!Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpBoneIndex ||
							!Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpWeight ||
							!Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpOffset)
						{
							lpStream->vClose();
							return FALSE;
						}

						for (int b=0; b<Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].NumBones; b++)
						{
							lpStream->vRead(&BoneIndex, sizeof(int), 1);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpBoneIndex[b] = BoneIndex;
							lpStream->vRead(&Value[0], sizeof(LYNXREAL), 1);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpWeight[b] = Value[0];
							lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpOffset[b], sizeof(LYNXVECTOR3D), 1);			
							LYNX_VECTOR3D_SET_W(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j].lpOffset[b], 1.0f);					
						}		
						CorrectWeightValue(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->WeightList[CurrentLOD][j]);						
					}
					int NumBones;
					lpStream->vRead(&NumBones, sizeof(int), 1);
					if (lod == 0)
					{
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_BoneShaderData.resize(NumBones);
						for (int j=0; j<Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_BoneShaderData.size(); j++)
						{
							lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_BoneShaderData[j].BoneIndex, sizeof(int), 1);
						}
					}
					else
					{
						lpStream->vSeek( sizeof(int)*NumBones, CStream::SeekCur);
					}
					break;

				case CMeshContainer::RIGID_ANIMATION:
					for (int j=0; j<Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexList[CurrentLOD].size(); j++)
					{
						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexList[CurrentLOD][j].x, sizeof(LYNXVECTOR3D), 1);				
						LYNX_VECTOR3D_SET_W(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexList[CurrentLOD][j], 1.0f);

						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexTangentList[CurrentLOD][j].x, sizeof(LYNXVECTOR3D), 1);				
						LYNX_VECTOR3D_SET_W(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexTangentList[CurrentLOD][j], 1.0f);
						lynxNormalise(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexTangentList[CurrentLOD][j], 3);		

						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexBinormalList[CurrentLOD][j].x, sizeof(LYNXVECTOR3D), 1);				
						LYNX_VECTOR3D_SET_W(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexBinormalList[CurrentLOD][j], 1.0f);
						lynxNormalise(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexBinormalList[CurrentLOD][j], 3);		

						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexNormalList[CurrentLOD][j].x, sizeof(LYNXVECTOR3D), 1);				
						LYNX_VECTOR3D_SET_W(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexNormalList[CurrentLOD][j], 1.0f);
						lynxNormalise(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexNormalList[CurrentLOD][j], 3);	

						lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->UVVertexList[CurrentLOD][j], sizeof(LYNXUVPOINT), 1);
						
						// Default vertex color
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].PackedColor = 0xffffffff;					

						if (MainVer >= 400)
						{
							float Color[4];
							lpStream->vRead(Color, sizeof(float), 4);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].Red = LYNX_ROUND(Color[0]*255.0f);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].Green = LYNX_ROUND(Color[1]*255.0f);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].Blue = LYNX_ROUND(Color[2]*255.0f);
							Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->VertexColorList[CurrentLOD][j].Alpha = LYNX_ROUND(Color[3]*255.0f);
						}
					}
					if (lod == 0)
					{
						Animation::CAnimation* lpAni = vCreateAnimation();
						lpAni->SetName(_T("Default"));
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->AddAnimation(lpAni);

						/* Read Rotation Animation Key */
						int NumKeys;
						lpStream->vRead(&NumKeys, sizeof(int), 1);
						lpAni->CreateKeys(Animation::CAnimation::KT_ROTATION, NumKeys, 4);
						for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys.size(); k++)
						{					
							lpStream->vRead(&ST, sizeof(int), 1);
							lpStream->vRead(&ET, sizeof(int), 1);
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetStartTime(ST);
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetEndTime(ET);
							if ((float)ET > AnimationLength)
								AnimationLength = ET;
							lpStream->vRead(KeyValues, sizeof(LYNXREAL), 4);	
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetValues(KeyValues, 4);
						}
						/* Read Translate Animation Key */
						lpStream->vRead(&NumKeys, sizeof(int), 1);
						lpAni->CreateKeys(Animation::CAnimation::KT_POSITION, NumKeys, 3);
						for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys.size(); k++)
						{					
							lpStream->vRead(&ST, sizeof(int), 1);
							lpStream->vRead(&ET, sizeof(int), 1);
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetStartTime(ST);
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetEndTime(ET);
							if ((float)ET > AnimationLength)
								AnimationLength = ET;
							lpStream->vRead(KeyValues, sizeof(LYNXREAL), 3);	
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetValues(KeyValues, 3);			
						}
						/* Read Scale Animation Key */
						lpStream->vRead(&NumKeys, sizeof(int), 1);
						lpAni->CreateKeys(Animation::CAnimation::KT_SCALE, NumKeys, 3);
						for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys.size(); k++)
						{					
							lpStream->vRead(&ST, sizeof(int), 1);
							lpStream->vRead(&ET, sizeof(int), 1);
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetStartTime(ST);
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetEndTime(ET);
							if ((float)ET > AnimationLength)
								AnimationLength = ET;
							lpStream->vRead(KeyValues, sizeof(LYNXREAL), 3);	
							lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetValues(KeyValues, 3);						
						}
						if (MainVer >= 300)
						{
							/* Read UV position Animation Key */
							lpStream->vRead(&NumKeys, sizeof(int), 1);
							lpAni->CreateKeys(Animation::CAnimation::KT_UV, NumKeys, 2);
							for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_UV]->m_Keys.size(); k++)
							{					
								lpStream->vRead(&ST, sizeof(int), 1);
								lpStream->vRead(&ET, sizeof(int), 1);
								lpAni->m_AnimationKeys[Animation::CAnimation::KT_UV]->m_Keys[k].SetStartTime(ST);
								lpAni->m_AnimationKeys[Animation::CAnimation::KT_UV]->m_Keys[k].SetEndTime(ET);
								if ((float)ET > AnimationLength)
									AnimationLength = ET;
								lpStream->vRead(KeyValues, sizeof(LYNXREAL), 2);	
								lpAni->m_AnimationKeys[Animation::CAnimation::KT_UV]->m_Keys[k].SetValues(KeyValues, 2);			
							}
							/* Read Visibility Animation Key */
							lpStream->vRead(&NumKeys, sizeof(int), 1);
							lpAni->CreateKeys(Animation::CAnimation::KT_VISIBILITY, NumKeys, 1);
							for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_VISIBILITY]->m_Keys.size(); k++)
							{					
								lpStream->vRead(&ST, sizeof(int), 1);
								lpStream->vRead(&ET, sizeof(int), 1);
								lpAni->m_AnimationKeys[Animation::CAnimation::KT_VISIBILITY]->m_Keys[k].SetStartTime(ST);
								lpAni->m_AnimationKeys[Animation::CAnimation::KT_VISIBILITY]->m_Keys[k].SetEndTime(ET);
								if ((float)ET > AnimationLength)
									AnimationLength = ET;
								lpStream->vRead(KeyValues, sizeof(LYNXREAL), 1);	
								lpAni->m_AnimationKeys[Animation::CAnimation::KT_VISIBILITY]->m_Keys[k].SetValues(KeyValues, 1);						
							}
							if (NumKeys > 0)
								m_lpMeshArray[i]->m_bUseVisibilty = LYNX_TRUE;
						}
					}
					else
					{
						/* Read Rotation Animation Key */
						int NumKeys;
						lpStream->vRead(&NumKeys, sizeof(int), 1);						
						for (int k=0; k<NumKeys; k++)
						{					
							lpStream->vRead(&ST, sizeof(int), 1);
							lpStream->vRead(&ET, sizeof(int), 1);							
							lpStream->vRead(KeyValues, sizeof(LYNXREAL), 4);								
						}
						/* Read Translate Animation Key */
						lpStream->vRead(&NumKeys, sizeof(int), 1);
						for (int k=0; k<NumKeys; k++)
						{					
							lpStream->vRead(&ST, sizeof(int), 1);
							lpStream->vRead(&ET, sizeof(int), 1);							
							lpStream->vRead(KeyValues, sizeof(LYNXREAL), 3);								
						}
						/* Read Scale Animation Key */
						lpStream->vRead(&NumKeys, sizeof(int), 1);
						for (int k=0; k<NumKeys; k++)
						{					
							lpStream->vRead(&ST, sizeof(int), 1);
							lpStream->vRead(&ET, sizeof(int), 1);							
							lpStream->vRead(KeyValues, sizeof(LYNXREAL), 3);								
						}
						if (MainVer >= 300)
						{
							/* Read UV position Animation Key */
							lpStream->vRead(&NumKeys, sizeof(int), 1);
							for (int k=0; k<NumKeys; k++)
							{					
								lpStream->vRead(&ST, sizeof(int), 1);
								lpStream->vRead(&ET, sizeof(int), 1);							
								lpStream->vRead(KeyValues, sizeof(LYNXREAL), 2);								
							}
							/* Read Visibility Animation Key */
							lpStream->vRead(&NumKeys, sizeof(int), 1);
							for (int k=0; k<NumKeys; k++)
							{					
								lpStream->vRead(&ST, sizeof(int), 1);
								lpStream->vRead(&ET, sizeof(int), 1);							
								lpStream->vRead(KeyValues, sizeof(LYNXREAL), 1);								
							}
						}
					}
					break;
			}			
            
			lpStream->vRead(&Num, sizeof(DWORD), 1);
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->TriangleList[CurrentLOD].resize(Num);		
			for (int t=0; t<Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->TriangleList[CurrentLOD].size(); t++)
			{
				lpStream->vRead(&TempTri.v1, sizeof(DWORD), 1);	
				lpStream->vRead(&TempTri.v2, sizeof(DWORD), 1);	
				lpStream->vRead(&TempTri.v3, sizeof(DWORD), 1);	
				lpStream->vRead(&TempTri.Plane.Normal, sizeof(LYNXVECTOR3D), 1);	
				LYNX_VECTOR3D_SET_W(TempTri.Plane.Normal, 1.0f);				
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->TriangleList[CurrentLOD][t].v[0] = TempTri.v1; 
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->TriangleList[CurrentLOD][t].v[1] = TempTri.v2; 
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->TriangleList[CurrentLOD][t].v[2] = TempTri.v3; 
			}

			DWORD NumPrimitives;
			DWORD Index;
			lpStream->vRead(&NumPrimitives, sizeof(DWORD), 1);
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD].resize(NumPrimitives);
			for (int j=0; j<Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD].size(); j++)
			{
				lpStream->vRead(&PrimitiveType, sizeof(int), 1);
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].BaseVertex = 0;
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].BaseIndex = 0;
				lpStream->vRead(&Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumIndex, sizeof(DWORD), 1);
				switch (PrimitiveType)
				{
					case 0:
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].Type = GraphicsSystem::CRenderer::TRIANGLE_LIST;
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumPrimitives = Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumIndex/3;
						break;
					case 1:
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].Type = GraphicsSystem::CRenderer::TRIANGLE_STRIP;
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumPrimitives = Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumIndex-2;
						break;
					case 2:
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].Type = GraphicsSystem::CRenderer::TRIANGLE_FAN;
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumPrimitives = Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumIndex-2;
						break;
				}	
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].lpIndex = LYNXGLOBAL_NEW unsigned short[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumIndex];					
				for (int k=0; k<Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].NumIndex; k++)
				{
					lpStream->vRead(&Index, sizeof(DWORD), 1);					
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MeshData->Primitives[CurrentLOD][j].lpIndex[k] = Index;
				}
			}		
			if (bloadmaterial)
			{
				if (lod == 0)
				{
					 if (!bMaterialSetup[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex])
					{
						Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetupMaterial(m_lpMaterials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex]);
						bMaterialSetup[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex] = LYNX_TRUE;
					}
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetlpCurrentMaterial(m_lpMaterials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex]);		
				}
			}
		}		
		bMaterialSetup.clear();

		if (lod == 0)
		{		
			DWORD NumBones;

			lpStream->vRead(&NumBones, sizeof(DWORD), 1);
			m_lpBoneArray.resize(NumBones);

			if (m_lpBoneArray.size())
			{
				for (int i=0; i<m_lpBoneArray.size(); i++)
				{
					m_lpBoneArray[i] = CreateBoneContainer();			
					Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->SetlpEngine(m_lpEngine);
				}			

				for (int i=0; i<m_lpBoneArray.size(); i++)
				{
					Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_BoneID = i;
					lpStream->vRead(Name, 1, 64);	
					((LynxEngine::CObj*)Container_Cast<CBoneContainer*>(m_lpBoneArray[i]))->SetName(GetName()+_T(".")+CString(Name));			
					/*
					lpStream->vRead(&BoneIndex, sizeof(int), 1);	
					if (BoneIndex == -1)
					{
						Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpParent = this;
						RootIndex = i;
					}
					else
					{
						Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpParent = CAST(CContainer*, m_lpBoneArray[BoneIndex]);
					}			
					lpStream->vRead(&Num, sizeof(DWORD), 1);
					Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpChildrenArray.resize(Num);
					for (int j=0; j<Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpChildrenArray.size(); j++)
					{
						lpStream->vRead(&BoneIndex, sizeof(int), 1);	
						Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpChildrenArray[j] = CAST(CContainer*, m_lpBoneArray[BoneIndex]);
					}*/

					Animation::CAnimation* lpAni = vCreateAnimation();
					lpAni->SetName(_T("Default"));
					Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->AddAnimation(lpAni);

					/* Read Rotation Animation Key */
					int NumKeys;					
					lpStream->vRead(&NumKeys, sizeof(int), 1);
					lpAni->CreateKeys(Animation::CAnimation::KT_ROTATION, NumKeys, 4);				
					for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys.size(); k++)
					{					
						lpStream->vRead(&ST, sizeof(int), 1);
						lpStream->vRead(&ET, sizeof(int), 1);
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetStartTime(ST);
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetEndTime(ET);
						if ((float)ET > AnimationLength)
							AnimationLength = ET;
						lpStream->vRead(KeyValues, sizeof(LYNXREAL), 4);	
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetValues(KeyValues, 4);
					}
					/* Read Translate Animation Key */
					lpStream->vRead(&NumKeys, sizeof(int), 1);
					lpAni->CreateKeys(Animation::CAnimation::KT_POSITION, NumKeys, 3);
					for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys.size(); k++)
					{					
						lpStream->vRead(&ST, sizeof(int), 1);
						lpStream->vRead(&ET, sizeof(int), 1);
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetStartTime(ST);
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetEndTime(ET);
						if ((float)ET > AnimationLength)
							AnimationLength = ET;
						lpStream->vRead(KeyValues, sizeof(LYNXREAL), 3);	
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetValues(KeyValues, 3);			
					}
					/* Read Scale Animation Key */
					lpStream->vRead(&NumKeys, sizeof(int), 1);
					lpAni->CreateKeys(Animation::CAnimation::KT_SCALE, NumKeys, 3);
					for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys.size(); k++)
					{					
						lpStream->vRead(&ST, sizeof(int), 1);
						lpStream->vRead(&ET, sizeof(int), 1);
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetStartTime(ST);
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetEndTime(ET);
						if ((float)ET > AnimationLength)
							AnimationLength = ET;
						lpStream->vRead(KeyValues, sizeof(LYNXREAL), 3);	
						lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetValues(KeyValues, 3);						
					}
				}
			}

			Animation::CAnimation* lpAni = vCreateAnimation();
			lpAni->SetName(_T("Default"));
			lpAni->SetRange(0.0f, AnimationLength-1.0f);
			AddAnimation(lpAni);

			m_SubContainerList.clear();
			for (int i=0; i<m_lpMeshArray.size(); i++)
			{	
				AddSubContainer(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])); 
			}		
			for (int i=0; i<m_lpBoneArray.size(); i++)
			{			
				AddSubContainer(Container_Cast<CBoneContainer*>(m_lpBoneArray[i])); 
			}		

			for (int i=0; i<m_lpMeshArray.size(); i++)
			{
				lpStream->vRead(Name, sizeof(char), 64);	
				HieName = (GetName()+_T(".")+CString(Name));					
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_lpParent = FindContainer(HieName);				
				lpStream->vRead(&Num, sizeof(DWORD), 1);
				Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_lpChildrenArray.resize(Num);
				for (int j=0; j<Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_lpChildrenArray.size(); j++)
				{
					lpStream->vRead(Name, sizeof(char), 64);	
					HieName = (GetName()+_T(".")+CString(Name));					
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_lpChildrenArray[j] = FindContainer(HieName);				
				}
			}

			for (int i=0; i<m_lpBoneArray.size(); i++)
			{
				lpStream->vRead(Name, sizeof(char), 64);	
				HieName = (GetName()+_T(".")+CString(Name));					
				Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpParent = FindContainer(HieName);				
				lpStream->vRead(&Num, sizeof(DWORD), 1);
				Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpChildrenArray.resize(Num);
				for (int j=0; j<Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpChildrenArray.size(); j++)
				{
					lpStream->vRead(Name, sizeof(char), 64);	
					HieName = (GetName()+_T(".")+CString(Name));					
					Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpChildrenArray[j] = FindContainer(HieName);				
				}
			}

			m_lpParent = NULL;
			m_lpChildrenArray.clear();
			for (int i=0; i<m_lpMeshArray.size(); i++)
			{
				if (!Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_lpParent)
				{
					m_lpChildrenArray.push_back(Container_Cast<CContainer*>(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])));
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetlpParent(this);		
				}
			}
			for (int i=0; i<m_lpBoneArray.size(); i++)
			{
				if (!Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpParent)
				{
					m_lpChildrenArray.push_back(Container_Cast<CContainer*>(Container_Cast<CBoneContainer*>(m_lpBoneArray[i])));		
					Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->SetlpParent(this);		
				}
			}
			MakeTraverseArray();				
		}
		CreateRenderArray();

		lpStream->vClose();
		
		////put material loading here avoid seek file when loading model		
		//for (i=0; i<m_NumMaterials; i++)
		//{
		//	lynxLoadTexture(&m_lpMaterials[i].lpPass[0].TextureFrame[0].lpTexIndexList[0], NULL, 0, m_lpMaterials[i].lpPass[0].TextureFrame[0].lplpTexList[0], NULL, 0);
		//}			
		
		Setup(lod);			

		// Temporary code---------------------------------------------------------------------------------------------
		if (lod == 0)
		{
			CObjProxy* lpProxy = LYNXNEW CBoxProxy(m_lpEngine);
			lpProxy->SetParent(m_lpRenObj, this);
			lpProxy->SetName(m_Name + CString(_T(".Proxy00")));
			m_ProxyList.push_back(lpProxy);

			/*
			for (int i=0; i<m_lpMeshArray.size(); i++)			
			{
				CList<CObjProxy*>::CIterator Proxy;
				for (Proxy=m_lpMeshArray[i]->GetProxyList().begin(); Proxy!=m_lpMeshArray[i]->GetProxyList().end(); ++Proxy)
					m_ProxyList.push_back(*Proxy);
			}
			*/

			/*
			Math::CVector3 Center(0.0f, 0.0f, 0.0f);
			Math::CReal Radius = -FLT_MAX;
			for (int i=0; i<m_lpMeshArray.size(); i++)			
			{
				Math::CVector3 C;
				Math::CReal R;
				m_lpMeshArray[i]->vGetCenterAndRadius(C, R);
				Radius = LYNX_MAX(R, Radius);
				Center += C;
			}
			Center /= (float)m_lpMeshArray.size();	
			
			m_lpCollisionHierarchy->Create(1);
			// Level 0
			CollisionSystem::CSphere* pSphere = LYNXNEW CollisionSystem::CSphere(Center, Radius);		
			m_lpCollisionHierarchy->Add(0, pSphere);				
			*/
			
		}

		if (lod == 0)
		{
			SetlpCurrentMaterial(m_lpMeshArray[0]->GetlpCurrentMaterial());
			SetCurrentAnimation(_T("Default"));	
			vPlay(0.0f);
		}
		// Temporary code---------------------------------------------------------------------------------------------
		
		//Yahoo!!!	
		return LYNX_TRUE;	
	
		#ifndef __NO_GUARD__
			LYNX_UNGUARD
			catch (CException& e) { lynxDebugOutputPrintf(e.GetMessage().c_str()); return LYNX_FALSE; }
			LYNX_DEFAULTCATCH
		#endif
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vCreateDefaultResource()
	{		
		Animation::CAnimation* lpAni = vCreateAnimation();
		lpAni->SetName(_T("Default"));
		lpAni->SetRange(0.0f, 1.0f);				
		lpAni->SetStep(1.0f);				
		lpAni->SetPlayMode(PM_LOOP);	
		AddAnimation(lpAni);
		SetCurrentAnimation(lpAni);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CModelContainer::vCreate(const CCreationDesc* desc)
	{
		LYNXBOOL bRet = LYNX_FALSE;		
		CModelCreationDesc* Desc = (CModelCreationDesc*)desc;

		/*
		const LYNXCHAR* lpExt = NULL;
		for (int i=(int)Desc->m_FileName.length()-1; i>=0; i--)
		{
			if (Desc->m_FileName[(unsigned int)i] == '.')
			{
				lpExt = &Desc->m_FileName[(unsigned int)(i+1)];
				break;
			}
		}
		if (lpExt)
		{
			if ((!lynxStriCmp(lpExt, _T("ms")) && lynxStrLen(lpExt) == 2) ||
				(!lynxStriCmp(lpExt, _T("msf")) && lynxStrLen(lpExt) == 3) )
			{
				//bbo = FALSE;
				//bRet = lynxLoadModelMS(lpsm, filename, lpf, offset, bbo);
				bRet = LYNX_TRUE;
			}
			else if ((!lynxStriCmp(lpExt, _T("mbs")) && lynxStrLen(lpExt) == 3) ||
					(!lynxStriCmp(lpExt, _T("mbsf")) && lynxStrLen(lpExt) == 4) )
			{
				bRet = LoadMBS(Desc->m_FileName.c_str(), Desc->m_lpFile, Desc->m_Offset, Desc->m_bUseBufferObject);
			}
			else if ((!lynxStriCmp(lpExt, _T("m")) && lynxStrLen(lpExt) == 1) ||
					(!lynxStriCmp(lpExt, _T("mf")) && lynxStrLen(lpExt) == 2) )
			{
				bRet = LoadMBS(Desc->m_FileName.c_str(), Desc->m_lpFile, Desc->m_Offset, Desc->m_bUseBufferObject);
			}
		}
		*/

		CPlatformFileStream FileStream;
		CStream* lpStream;										

		CContainer::vCreate(desc);

		if (desc->m_lpStream)
			lpStream = desc->m_lpStream;
		else
			lpStream = &FileStream;
		
		bRet = LoadMDL(*lpStream, Desc->m_FileName, Desc->m_bCreateMaterial, Desc->m_MaterialName, 0, Desc->m_bUseBufferObject);
		if (!bRet)
		{
			return LYNX_FALSE;			
		}				
		
		return bRet;
	}	
	////-------------------------------------------------------------------------------------------------------
	///**
	//*	@brief 。
	//*
	//*	@param 。
	//*	@param 。  
	//*	@return 。 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CModelContainer::vCreate(const LYNXCHAR *filename, int cluster_size, LPLYNXFILE lpf, long offset, LYNXBOOL bbo)
	//{
	//	const LYNXCHAR* lpExt;
	//	LYNXBOOL bRet = LYNX_FALSE;		

	//	lpExt = NULL;
	//	for (int i=(int)lynxStrLen(filename)-1; i>=0; i--)
	//	{
	//		if (filename[i] == '.')
	//		{
	//			lpExt = &filename[i+1];
	//			break;
	//		}
	//	}
	//	if (lpExt)
	//	{
	//		if ((!lynxStriCmp(lpExt, _T("ms")) && lynxStrLen(lpExt) == 2) ||
	//			(!lynxStriCmp(lpExt, _T("msf")) && lynxStrLen(lpExt) == 3) )
	//		{
	//			//bbo = FALSE;
	//			//bRet = lynxLoadModelMS(lpsm, filename, lpf, offset, bbo);
	//			bRet = LYNX_TRUE;
	//		}
	//		else if ((!lynxStriCmp(lpExt, _T("mbs")) && lynxStrLen(lpExt) == 3) ||
	//				(!lynxStriCmp(lpExt, _T("mbsf")) && lynxStrLen(lpExt) == 4) )
	//		{
	//			bRet = LoadMBS(filename, lpf, offset, bbo);		
	//		}
	//		else if ((!lynxStriCmp(lpExt, _T("m")) && lynxStrLen(lpExt) == 1) ||
	//				(!lynxStriCmp(lpExt, _T("mf")) && lynxStrLen(lpExt) == 2) )
	//		{
	//			bRet = LoadM(filename, lpf, offset, bbo);
	//		}
	//	}
	//	if (!bRet)
	//	{
	//		return LYNX_FALSE;
	//		
	//	}	

	//	m_SubContainerArray.resize(m_lpMeshArray.size());
	//	for (int i=0; i<m_lpMeshArray.size(); i++)
	//	{	
	//		m_SubContainerArray[i] = Container_Cast<CMeshContainer*>(m_lpMeshArray[i]);
	//		Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetlpRenObj(GetlpRenObj());												
	//	}		
	//	for (int i=0; i<m_lpBoneArray.size(); i++)
	//	{			
	//		Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->SetlpRenObj(GetlpRenObj());
	//	}
	//	SetlpCurrentMaterial(m_lpMeshArray[0]->GetlpCurrentMaterial());
	//
	//	return bRet;
	//}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vAfterLoad()
	{
		/*
		LYNXCHAR MaterialName[64];

		m_SubContainerArray.resize(m_lpMeshArray.size());
		for (int i=0; i<m_lpMeshArray.size(); i++)
		{	
			m_SubContainerArray[i] = &Container_Cast<CMeshContainer*>(m_lpMeshArray[i]);
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetlpRenObj(GetlpRenObj());						
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->CreateMaterialArray(CContainer::NUM_MATERIAL_TYPE);
			switch (m_ModelType)
			{
				case HIE_BONESKIN_MODEL:
			 		m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex].LoadShaders(0, "builtin/scn/model_mbs_pointlight.vso", "builtin/scn/model_pointlight.pso");					
					break;

				case HIE_MODEL:
					m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex].LoadShaders(0, "builtin/scn/model_m_pointlight.vso", "builtin/scn/model_pointlight.pso");						
					break;
			}					
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetMaterial(CContainer::AMBIENT_LIGHTING_MATERIAL, &m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex]);				
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetMaterial(CContainer::POINT_LIGHTING_MATERIAL, &m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex]);				
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetMaterial(CContainer::SPOT_LIGHTING_MATERIAL, &m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex]);				
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetMaterial(CContainer::DIRECTIONAL_LIGHTING_MATERIAL, &m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex]);				

			// Craete Z-pass material
			lynxStrCpy(MaterialName, m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex].GetName());
			lynxStrCat(MaterialName, _T("_ZPASS"));
			MaterialSystem::CMaterial* lpTMat = ((CEngine *)m_lpEngine)->GetlpMaterialSystem()->FindMaterial(MaterialName);
			if (!lpTMat)
			{
				lpTMat = LYNXNEW MaterialSystem::CMaterial;
				(*lpTMat) = m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex];								
				lpTMat->SetName(MaterialName);
				lpTMat->SetType(MaterialSystem::CMaterial::ZPASS);
				switch (m_ModelType)
				{
					case HIE_BONESKIN_MODEL:
						lpTMat->LoadShaders(0, _T("builtin/scn/model_mbs_zpass.vso"), _T("builtin/scn/model_zpass.pso"));					
						break;

					case HIE_MODEL:
						lpTMat->LoadShaders(0, _T("builtin/scn/model_m_zpass.vso"), _T("builtin/scn/model_zpass.pso"));
						break;
				}			
				((CEngine *)m_lpEngine)->GetlpMaterialSystem()->AddMaterial(lpTMat);
			}
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetMaterial(CContainer::ZPASS_MATERIAL, lpTMat);							

			// Craete shadow mapping material
			lynxStrCpy(MaterialName, m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex].GetName());
			lynxStrCat(MaterialName, _T("_SHADOWMAP"));
			lpTMat = ((CEngine *)m_lpEngine)->GetlpMaterialSystem()->FindMaterial(MaterialName);
			if (!lpTMat)
			{
				lpTMat = LYNXNEW MaterialSystem::CMaterial;
				(*lpTMat) = m_Materials[Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_MaterialIndex];		
				lpTMat->SetName(MaterialName);
				lpTMat->SetType(MaterialSystem::CTechnique::SHADOWMAPPING);
				lpTMat->LoadShaders(0, _T("builtin/scn/model_m_shadowmap.vso"), _T("builtin/scn/model_shadowmap.pso"));
				((CEngine *)m_lpEngine)->GetlpMaterialSystem()->AddMaterial(lpTMat);
			}
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetMaterial(CContainer::SHADOW_MATERIAL, lpTMat);	

			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->GetMaterial(CContainer::AMBIENT_LIGHTING_MATERIAL)->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ONE);
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->GetMaterial(CContainer::POINT_LIGHTING_MATERIAL)->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ONE);
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->GetMaterial(CContainer::SPOT_LIGHTING_MATERIAL)->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ONE);
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->GetMaterial(CContainer::DIRECTIONAL_LIGHTING_MATERIAL)->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ONE);
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->GetMaterial(CContainer::ZPASS_MATERIAL)->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ZERO);
			Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->GetMaterial(CContainer::SHADOW_MATERIAL)->GetPass(0).SetBlendFunc(GraphicsSystem::CRenderer::BF_ONE, GraphicsSystem::CRenderer::BF_ZERO);
		}
		SetCurrentMaterialType(CContainer::ZPASS_MATERIAL);
		*/		
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	Animation::CAnimation* CModelContainer::vLoadAnimation(const CCreationDesc* desc) 
	{ 		
		char				Mark[] = "LYNXANI";	
		char				Check[8], Name[64];	
		DWORD				MainVer, Num;				
		LYNXREAL			KeyValues[4];
		int					ST, ET, MaxTime;
		CString				DestName;
		CContainer*			lpContainer;		
		CPlatformFileStream FileStream;
		CStream*			lpStream;										
		Math::CMatrix4		Matrix;

		MaxTime = 0;
		
		if (desc->m_lpStream)
			lpStream = desc->m_lpStream;
		else
			lpStream = &FileStream;

		OPEN_Z_FILE(lpStream, desc->m_FileName)
		else
		{
			if (!lpStream->vOpen(desc->m_FileName, CStream::Read|CStream::Binary))
			{			
				lpStream->vClose();
				return NULL;
			}
		}

		lpStream->vRead(&Check, 7, 1);
		Check[7] = 0x00;
		if (strcmp(Mark, Check))	
		{
			lpStream->vClose();
			return NULL;
		}				

		lpStream->vRead(&MainVer, sizeof(DWORD), 1);    						
		lpStream->vRead(&Num, sizeof(DWORD), 1);			
		for (int i=0; i<Num; i++)
		{
			lpStream->vRead(Name, sizeof(char), 64);
			if (!lynxStrCmp(Name, _T("root")))
			{
				lpContainer = this;
			}
			else
			{
				DestName = (GetName()+_T(".")+CString(Name));			
				lpContainer = FindContainer(DestName);							
			}

			if (lpContainer)
			{		
				Animation::CAnimation* lpAni = vCreateAnimation();
				lpAni->SetName(desc->m_Name);
				//lpAni->Load(lpFile);

				lpStream->vRead(&Matrix , sizeof(LYNXMATRIX), 1);
				lpAni->m_PivotM = Matrix;
				lpContainer->SetPivotMatrix(Matrix);
				lpStream->vRead(&Matrix , sizeof(LYNXMATRIX), 1);		
				lpAni->m_InversePivotM = Matrix;
				lpContainer->SetInversePivotMatrix(Matrix);				
				
				int NumKeys;
				/* Read Rotation Animation Key */
				lpStream->vRead(&NumKeys, sizeof(int), 1);
				lpAni->CreateKeys(Animation::CAnimation::KT_ROTATION, NumKeys, 4);
				for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys.size(); k++)
				{					
					lpStream->vRead(&ST, sizeof(int), 1);
					lpStream->vRead(&ET, sizeof(int), 1);
					if (ET > MaxTime)
						MaxTime = ET;
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetStartTime(ST);
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetEndTime(ET);
					lpStream->vRead(KeyValues, sizeof(LYNXREAL), 4);						
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetValues(KeyValues, 4);
				}
				/* Read Translate Animation Key */
				lpStream->vRead(&NumKeys, sizeof(int), 1);
				lpAni->CreateKeys(Animation::CAnimation::KT_POSITION, NumKeys, 3);
				for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys.size(); k++)
				{					
					lpStream->vRead(&ST, sizeof(int), 1);
					lpStream->vRead(&ET, sizeof(int), 1);
					if (ET > MaxTime)
						MaxTime = ET;
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetStartTime(ST);
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetEndTime(ET);
					lpStream->vRead(KeyValues, sizeof(LYNXREAL), 3);											
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetValues(KeyValues, 3);			
				}
				/* Read Scale lnimation Key */
				lpStream->vRead(&NumKeys, sizeof(int), 1);
				lpAni->CreateKeys(Animation::CAnimation::KT_SCALE, NumKeys, 3);
				for (int k=0; k<lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys.size(); k++)
				{					
					lpStream->vRead(&ST, sizeof(int), 1);
					lpStream->vRead(&ET, sizeof(int), 1);
					if (ET > MaxTime)
						MaxTime = ET;
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetStartTime(ST);
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetEndTime(ET);
					lpStream->vRead(KeyValues, sizeof(LYNXREAL), 3);	
					lpAni->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetValues(KeyValues, 3);						
				}
				lpContainer->AddAnimation(lpAni);
				lpAni->SetRange(0.0f, MaxTime-1);				
				lpAni->SetStep(1.0f);				
				lpAni->SetPlayMode(PM_LOOP);						
			}
			else
			{
				Animation::CAnimation::SkipLoading(*(lpStream));
				
				///* Read Rotation Animation Key */
				//int NumKeys;
				//lpStream->vRead(&NumKeys, sizeof(int), 1);				
				//lpStream->vSeek( ((sizeof(int)*2)+(sizeof(LYNXREAL)*4))*NumKeys, CStream::SeekCur);				
				///* Read Translate Animation Key */
				//lpStream->vRead(&NumKeys, sizeof(int), 1);				
				//lpStream->vSeek( ((sizeof(int)*2)+(sizeof(LYNXREAL)*3))*NumKeys, CStream::SeekCur);				
				///* Read Scale Animation Key */
				//lpStream->vRead(&NumKeys, sizeof(int), 1);				
				//lpStream->vSeek( ((sizeof(int)*2)+(sizeof(LYNXREAL)*3))*NumKeys, CStream::SeekCur);				
			}
		}	

		if (MainVer >= 400)
		{
			// read hierarkey data
			DWORD Size;
			CString CurrentContainerName;
			CContainer* CurrentContainer;

			m_lpParent = NULL;
			m_lpChildrenArray.clear();		
			for (int i=0; i<m_lpMeshArray.size(); i++)
			{
				m_lpMeshArray[i]->m_lpParent = NULL;
				m_lpMeshArray[i]->m_lpChildrenArray.clear();
			}

			lpStream->vRead(&Size, sizeof(DWORD), 1);
			for (int i=0; i<Size; i++)
			{
				lpStream->vRead(Name, sizeof(char), 64);	
				CurrentContainerName = (GetName()+_T(".")+CString(Name));			
				CurrentContainer = FindContainer(CurrentContainerName);			
				if (CurrentContainer)
				{
					lpStream->vRead(Name, sizeof(char), 64);	
					DestName = (GetName()+_T(".")+CString(Name));			
					lpContainer = FindContainer(DestName);			
					CurrentContainer->m_lpParent = lpContainer;
					lpStream->vRead(&Num, sizeof(DWORD), 1);
					CurrentContainer->m_lpChildrenArray.clear();
					for (int j=0; j<Num; j++)
					{
						lpStream->vRead(Name, sizeof(char), 64);	
						DestName = (GetName()+_T(".")+CString(Name));			
						lpContainer = FindContainer(DestName);			
						if (lpContainer)
						{
							CurrentContainer->m_lpChildrenArray.push_back(lpContainer);				
						}
					}
				}
				else
				{
					lpStream->vRead(Name, sizeof(char), 64);	
					lpStream->vRead(&Num, sizeof(DWORD), 1);
					for (int j=0; j<Num; j++)
					{
						lpStream->vRead(Name, sizeof(char), 64);							
					}
				}
			}
			lpStream->vClose();

			for (int i=0; i<m_lpMeshArray.size(); i++)
			{
				if (!Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->m_lpParent)
				{
					m_lpChildrenArray.push_back(Container_Cast<CContainer*>(Container_Cast<CMeshContainer*>(m_lpMeshArray[i])));
					Container_Cast<CMeshContainer*>(m_lpMeshArray[i])->SetlpParent(this);		
				}
			}
			for (int i=0; i<m_lpBoneArray.size(); i++)
			{
				if (!Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->m_lpParent)
				{
					m_lpChildrenArray.push_back(Container_Cast<CContainer*>(Container_Cast<CBoneContainer*>(m_lpBoneArray[i])));		
					Container_Cast<CBoneContainer*>(m_lpBoneArray[i])->SetlpParent(this);		
				}
			}
			MakeTraverseArray();					
		}
		else
		{
			lpStream->vClose();
		}

		Animation::CAnimation* lpAni = NULL;
		lpAni = vCreateAnimation();
		lpAni->SetName(desc->m_Name);
		lpAni->SetRange(0.0f, MaxTime-1);				
		lpAni->SetStep(1.0f);				
		lpAni->SetPlayMode(PM_LOOP);	
		AddAnimation(lpAni);	
		
		return lpAni;	
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	MaterialSystem::CMaterial* CModelContainer::GetMaterial(const CString& name)
	{
		CArray<MaterialSystem::CMaterialPtr>::CIterator Mat;

		Mat = LynxEngine::find(m_lpMaterials.begin(), m_lpMaterials.end(), name);
		if (Mat != m_lpMaterials.end())
			return (*Mat);				
		
		CString Name = m_Name + CString(".") + name;
		Mat = LynxEngine::find(m_lpMaterials.begin(), m_lpMaterials.end(), Name);
		if (Mat != m_lpMaterials.end())
			return (*Mat);				

		return NULL;				
	}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*	@brief 。
	//*
	//*	@param 。
	//*	@param 。  
	//*	@return 。 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void CModelContainer::vChangeCurrentMaterial(int index, AdvMaterialSystem::CMaterial* const lpm)
	//{		
	//	m_lpMesh[index].m_lpCurrentMaterial = lpm;
	//}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*	@brief 。
	//*
	//*	@param 。
	//*	@param 。  
	//*	@return 。 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CModelContainer::vSetCurrentFrame(int id, LYNXBOOL bforcereset)
	//{
	//	m_AnimationSet->GetCurrentAnimation()->vSetCurrentFrame(id, bforcereset)


	//	/*if (CContainer::vSetCurrentFrame(id, bforcereset))
	//	{
	//		SetTimes(m_AnimationSet->GetCurrentAnimation()->GetlpCurrentFrame()->m_Start, m_AnimationSet->GetCurrentAnimation()->GetlpCurrentFrame()->m_End, m_AnimationSet->GetCurrentAnimation()->GetlpCurrentFrame()->m_Start);		
	//		return LYNX_TRUE;
	//	}*/
	//	return LYNX_FALSE;
	//}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*	@brief 。
	//*
	//*	@param 。
	//*	@param 。  
	//*	@return 。 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CModelContainer::vSetCurrentFrame(char *name, LYNXBOOL bforcereset)
	//{
	//	/*if (CContainer::vSetCurrentFrame(name, bforcereset))
	//	{
	//		SetTimes(m_lpCurrentFrame->m_Start, m_lpCurrentFrame->m_End, m_lpCurrentFrame->m_Start);		
	//		return LYNX_TRUE;
	//	}*/
	//	return LYNX_FALSE;
	//}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	//void CModelContainer::vChangeMaterial(int index, AdvMaterialSystem::CMaterial& lpm)
	//{
	//	m_lpMaterialList[index] = lpm;		
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vUpdateVertices(int lod)
	{		
		int NumV = 0;
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			CMeshContainer* lpMesh = Container_Cast<CMeshContainer*>((*Sub));
			switch (lpMesh->m_AnimationFlag)
			{
				case CMeshContainer::BLEND_ANIMATION:
					break;

				case CMeshContainer::RIGID_ANIMATION:
					for (int j=0; j<lpMesh->GetVertexList(lod).size(); j++)
					{
						lynxPointXMatrix(&m_TVertexList[NumV], &(lpMesh->GetVertex(lod, j)), &(lpMesh->m_M)); 
						//m_TVertexList[NumV] = lpMesh->GetVertex(lod, j);
						NumV++;
					}
					break;
			}
		}			
		

		//LYNXMATRIX						M1, M2;
		//LYNXREAL						Inf, Influence, MaxInf, Base, TU, TV, Dot;	
		//LYNXVECTOR3D					P, TP, Vec, LookDir, LitDir;
		//BYTE							C, Alpha;
		//int								v1, v2, v3;
		//int								i, j, k;		
		//LPLYNXRENDERVERTEX				lpP;
		//WORD							*lpT;
		//DWORD							NumV, NumT;
		//int								PV1, PV2, PV3, TV1, TV2, TV3, CV1, CV2, CV3;	
		//LYNXRENDERVERTEX				Tri[3];	
		//LYNXREAL						Weight;
		//LPLYNXRENDERVERTEX				lpTVertexList;

		//if (m_bUseBufferObject)
		//	return;

		//switch (m_ModelType)
		//{
		//	case HIE_MODEL:		
		//	case VERTEX_ANIMATION_MODEL:		
		//		//(*lplpGeo)->CurrentLOD;											
		//		/*if (m_Attribute&LYNX_MODEL_UPDATE_NORMAL)
		//		{
		//			for (i=0; i<m_NumMeshs; i++)	
		//			{						
		//				lynxSequenceVectorXMatrix(&m_lpTVertexNormalList[CurrentLOD][m_lpMeshs[i].BaseVertex[CurrentLOD]], &m_MeshData->VertexNormalList[CurrentLOD][m_lpMeshs[i].BaseVertex[CurrentLOD]], &m_lpMeshs[i].m_M, m_lpMeshs[i].NumVertices[CurrentLOD]);																				
		//			}
		//		}*/
		//		break;		

		//	case HIE_BONESKIN_MODEL:	
		//		lynxLockVertexArray(&m_VertexArray[m_CurrentLOD], 0, 0, LYNX_LOCK_TO_WRITE);
		//		lpTVertexList = (LPLYNXRENDERVERTEX)m_VertexArray[m_CurrentLOD].lpBuffer;

		//		for (i=0; i<m_NumVertices[m_CurrentLOD]; i++)
		//		{
		//			if (m_MeshData->WeightList[m_CurrentLOD][i].NumBones > 1)
		//			{
		//				P.x = P.y = P.z = LYNX_REAL(0);
		//				lpTVertexList[i].x = lpTVertexList[i].y = lpTVertexList[i].z = LYNX_REAL(0);

		//				for (j=0; j<m_MeshData->WeightList[m_CurrentLOD][i].NumBones; j++)
		//				{
		//					lynxPointXMatrix(&P, &m_MeshData->WeightList[m_CurrentLOD][i].lpOffset[j], &m_lpBoneArray[m_MeshData->WeightList[m_CurrentLOD][i].lpBoneIndex[j]]->GetTransformMatrix());																						
		//					P.x = LYNX_REAL_MUL(P.x, m_MeshData->WeightList[m_CurrentLOD][i].lpWeight[j]);
		//					P.y = LYNX_REAL_MUL(P.y, m_MeshData->WeightList[m_CurrentLOD][i].lpWeight[j]);
		//					P.z = LYNX_REAL_MUL(P.z, m_MeshData->WeightList[m_CurrentLOD][i].lpWeight[j]);							
		//					lpTVertexList[i].x = LYNX_REAL_ADD(lpTVertexList[i].x, P.x);
		//					lpTVertexList[i].y = LYNX_REAL_ADD(lpTVertexList[i].y, P.y);
		//					lpTVertexList[i].z = LYNX_REAL_ADD(lpTVertexList[i].z, P.z);						
		//				}
		//				if (m_Attribute&UPDATE_NORMAL)
		//				{
		//					lynxVectorXMatrix((LPLYNXVECTOR3D)&lpTVertexList[i].nx, &m_MeshData->VertexNormalList[m_CurrentLOD][i], &m_lpBoneArray[m_MeshData->WeightList[m_CurrentLOD][i].lpBoneIndex[0]]->m_M);
		//					lynxNormalise((LPLYNXVECTOR3D)&lpTVertexList[i].nx, 3);						
		//				}
		//			}
		//			else
		//			{
		//				lynxPointXMatrix((LPLYNXVECTOR3D)&lpTVertexList[i].x, &m_MeshData->WeightList[0][i].lpOffset[0], &m_lpBoneArray[m_MeshData->WeightList[0][i].lpBoneIndex[0]]->m_M);									
		//				if (m_Attribute&UPDATE_NORMAL)
		//				{
		//					lynxVectorXMatrix((LPLYNXVECTOR3D)&lpTVertexList[i].nx, &m_MeshData->VertexNormalList[0][i], &m_lpBoneArray[m_MeshData->WeightList[0][i].lpBoneIndex[0]]->m_M);
		//					lynxNormalise((LPLYNXVECTOR3D)&lpTVertexList[i].nx, 3);
		//				}
		//			}					
		//		}
		//		lynxUnlockVertexArray(&m_VertexArray[m_CurrentLOD]);
		//		break;
		//}	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CModelContainer::vPlay(CReal step)
	{		
		LYNXMATRIX              TM, TM1, TM2;
		LYNXBOOL				bStop = LYNX_FALSE;
			
		SaveTransformationMatrix();

		if (m_AnimationSet->GetCurrentAnimation())
		{
			bStop = m_AnimationSet->GetCurrentAnimation()->Play(step);			
			//m_AnimationSet->GetCurrentAnimation()->UpdateTransform();
			//m_M = m_AnimationSet->GetCurrentAnimation()->m_M;	
		}

		SetAnimationMatrix(m_M);
		
		//UpdateTransform();

		for (int i=1; i<m_lpTraverseArray.size(); ++i)	
		{	
			if (m_lpTraverseArray[i] && m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation())
			{
				#if 0
					m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_Time = m_AnimationSet->GetCurrentAnimation()->m_Time;				
					m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->UpdateTransform();
					lynxMatrixXMatrix(&TM1, &m_lpTraverseArray[i]->GetLocalMatrix(),  &m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_M);
					lynxMatrixXMatrix(&TM2, &TM1, &m_lpTraverseArray[i]->GetlpParent()->m_AnimationSet->GetCurrentAnimation()->m_M);
					lynxMatrixXMatrix(&TM1, &m_lpTraverseArray[i]->GetPivotMatrix(), &TM2);
					//Transform Matrix = PivotM * LocalM * TransM * ParentM * GlobalM; 
					m_lpTraverseArray[i]->SetTransformMatrix(TM1, m_lpTraverseArray[i]->GetGlobalMatrix());	 		
					//Parent Matrix = LocalM * TransM * ParentM * GlobalM; 
					lynxMatrixXMatrix(&m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_M, &TM2, &m_lpTraverseArray[i]->GetGlobalMatrix());	 
				#elif 0
					m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_Time = m_AnimationSet->GetCurrentAnimation()->m_Time;				
					m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->UpdateTransform();					
					lynxMatrixXMatrix(&TM, &m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_M, &m_lpTraverseArray[i]->GetlpParent()->m_AnimationSet->GetCurrentAnimation()->m_M);
					//Transform Matrix = PivotM * TransM * ParentM; 
					m_lpTraverseArray[i]->SetTransformMatrix(m_lpTraverseArray[i]->GetPivotMatrix(), TM);
					//Parent Matrix = TransM * ParentM; 
					lynxMemCpy(&m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_M, &TM, sizeof(LYNXMATRIX));	
				#else
					m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_Time = m_AnimationSet->GetCurrentAnimation()->m_Time;				
					m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->UpdateTransform();	
					m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->UpdateTextureUVTransform();
					m_lpTraverseArray[i]->SetTextureUVMatrix(m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_TextureUVM);
					m_lpTraverseArray[i]->m_Visibility = m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->UpdateVisibility();

					//lynxMatrixXMatrix(&TM, &(m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_M), &(m_lpTraverseArray[i]->GetPivotMatrix()));  
					//m_lpTraverseArray[i]->SetTransformMatrix(TM, m_lpTraverseArray[i]->GetlpParent()->GetTransformMatrix());										

					m_lpTraverseArray[i]->SetAnimationMatrix(m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_M, m_lpTraverseArray[i]->GetlpParent()->GetAnimationMatrix());					
					m_lpTraverseArray[i]->SetTransformMatrix(m_lpTraverseArray[i]->GetPivotMatrix(), m_lpTraverseArray[i]->GetAnimationMatrix());					
				#endif
			}
		}		
		return bStop;							
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vRagDollUpdate(OBJUPDATETYPE type)
	{		
		if (type == OBJUPDATE_PHYSICSOBJ)
		{
			for (int i=1; i<m_lpTraverseArray.size(); ++i)	
			{	
				if (m_lpTraverseArray[i])
				{
					if (m_lpTraverseArray[i]->GetContainerType() == CContainer::BONE && ((CBoneContainer*)m_lpTraverseArray[i])->GetlpPhyObj())
					{
						((CBoneContainer*)m_lpTraverseArray[i])->SetTransformMatrix(((CBoneContainer*)m_lpTraverseArray[i])->GetlpPhyObj()->vGetTransformMatrix());
					}
					else
					{
						if(m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation())
						{
							m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_Time = m_AnimationSet->GetCurrentAnimation()->m_Time;				
							m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->UpdateTransform();	
							m_lpTraverseArray[i]->SetTransformMatrix(m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_M, m_lpTraverseArray[i]->GetlpParent()->GetTransformMatrix());
						}
					}				
				}
			}
		}
		else
		{
			UpdateTransform();
			vPlay(0.0f);
			for (int i=1; i<m_lpTraverseArray.size(); ++i)	
			{	
				if (m_lpTraverseArray[i])
				{
					if (m_lpTraverseArray[i]->GetContainerType() == CContainer::BONE && ((CBoneContainer*)m_lpTraverseArray[i])->GetlpPhyObj())
					{
						((CBoneContainer*)m_lpTraverseArray[i])->GetlpPhyObj()->vSetTransformMatrix(((CBoneContainer*)m_lpTraverseArray[i])->GetTransformMatrix());
					}
					/*
					else
					{
						if(m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation())
						{
							m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_Time = m_AnimationSet->GetCurrentAnimation()->m_Time;				
							m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->UpdateTransform();	
							m_lpTraverseArray[i]->SetTransformMatrix(m_lpTraverseArray[i]->m_AnimationSet->GetCurrentAnimation()->m_M, m_lpTraverseArray[i]->GetlpParent()->GetTransformMatrix());
						}
					}
					*/
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::SortMesh(CCameraContainer* cam)
	{
		CVector3 CameraPos;

		switch (m_SortMode)
		{
			case SM_NONE:
				break;

			case SM_DISTANCE:
				cam->vGetPosition(CameraPos);
				for (int i=0; i<m_RenderArray.size(); ++i)
				{
					m_RenderArray[i]->CalculateDist(CameraPos);
				}
				LynxEngine::sort(m_RenderArray.begin(), m_RenderArray.end(), SortByDist<LYNX_SORT_DEC>());  
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CModelContainer::vRender(CCameraContainer* cam)
	{		
		if (!m_bUseBufferObject)
			vUpdateVertices(m_CurrentLOD);

		vCalculateLOD(cam);

		SortMesh(cam);

		for (int i=0; i<m_RenderArray.size(); ++i)		
		{
			if (m_RenderArray[i]->GetlpCurrentMaterial()->m_TransparentType == TT_OPAQUE)
				m_RenderArray[i]->vRender(cam);												
		}

		for (int i=0; i<m_RenderArray.size(); ++i)		
		{
			if (m_RenderArray[i]->GetlpCurrentMaterial()->m_TransparentType != TT_OPAQUE)
				m_RenderArray[i]->vRender(cam);												
		}

		//for (int i=1; i<m_lpTraverseArray.size(); ++i)		
			//m_lpTraverseArray[i]->vRender(cam);											
	}
}