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

#include <LynxSDKWrap.h>
#include <Container/LynxContainer.h>
#include <Container/LynxBoneContainer.h>
#include <Container/LynxMeshContainer.h>
#include <AdvMaterialSystem/LynxAdvMaterial.h>

using namespace LynxSDK;

namespace LynxEngine 
{
	class CEngine;
	class LYNXENGCLASS CMeshContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CModelContainer);
	protected:						
		AMS::CAdvMaterial*			m_lpBasicMaterialList;			
		int							m_Attribute;
		LYNXBOOL					m_bUseBufferObject;	
		int							m_Type;	
		LYNXSOURCETYPE				m_SourceType;
		
		DWORD						m_NumMaterials;
		CArray<LYNXVECTOR3D>		m_Vertex[LYNX_MAX_LOD];
		CArray<LYNXVECTOR3D>		m_VertexNormal[LYNX_MAX_LOD];
		CArray<LYNXWEIGHTDATA>		m_VertexWeight[LYNX_MAX_LOD];
		CArray<LYNXUVPOINT>			m_VertexUV[LYNX_MAX_LOD];
		CArray<LYNXCOLORRGBA>		m_VertexColor[LYNX_MAX_LOD];
		CArray<LYNXSIMPLETRIANGLE>	m_Triangle[LYNX_MAX_LOD];		
		
		LYNXVERTEXARRAY				m_VertexArray;
		LYNXINDEXARRAY				m_IndexArray;

		LPLYNXBLENDVERTEX			m_lpBV[LYNX_MAX_LOD];	
		LPLYNXMESHVERTEX			m_lpMV[LYNX_MAX_LOD];	

		DWORD						m_TraverseIndex;

		CArray<CMeshContianer>		m_Mesh;
		CMeshContianer*				lplpTraverseMeshs;

		CArray<CBoneContianer>		m_Bone;		
		CBoneContianer*				lplpTraverseBones;	
		
		WORD						m_NumLOD;	
		int							m_CurrentLOD;				
	public:		
		CModelContainer();
		CModelContainer(CEngine *lpengine);		
		virtual  ~CModelContainer();		

		CModelContainer&								operator =(CModelContainer& m);		
		
		LYNXINLINE virtual char *						vGetName() {return Mesh.Obj3D.Name; };							

		LYNXINLINE virtual LPLYNXOBJ3D					vGetlpObj3D() {return &Mesh.Obj3D; };

		virtual LYNXBOOL								vLoad(const char *name, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_TRUE);
		virtual void									vRelease();
		virtual void									vRender(void);

		virtual LYNXBOOL								vSetCurrentFrame(int id, LYNXBOOL bforcereset = FALSE);
		virtual LYNXBOOL								vSetCurrentFrame(char *name, LYNXBOOL bforcereset = FALSE);
		
		LYNXINLINE virtual LYNXBOOL						vPlay(LYNXREAL skip, LYNXPLAYMODE mode) {m_bPlayStop = lynxPlayModel((LPLYNXMODEL)this, skip, mode); return  m_bPlayStop; };
		LYNXINLINE virtual LYNXBOOL						vPlay(LYNXREAL skip) {m_bPlayStop = lynxPlayModel((LPLYNXMODEL)this, m_lpCurrentFrame->m_Skip*skip, m_lpCurrentFrame->m_PlayMode); return m_bPlayStop;};						

		LYNXINLINE CMeshContainer&						GetMesh(int index) {return m_lpMesh[index];};
		LYNXINLINE CBoneContainer&						GetBone(int index) {return m_lpBone[index];};

		LYNXINLINE virtual DWORD						vGetNumMaterials() const {return NumMaterials;};
		LYNXINLINE virtual AMS::CAdvMaterial*			vGetlpCurrentMaterial(int i) const {return (m_lpMesh[i].m_lpCurrentMaterial);};
		virtual	void									vChangeCurrentMaterial(int index, AdvMaterialSystem::CAdvMaterial* const lpm);

		LYNXINLINE void									SetModelAttribute(int a) {lynxSetModelAttribute((LPLYNXMODEL)this, a); };
		LYNXINLINE int									GetModelAttribute(void) const {return this->Attribute; };
		LYNXINLINE void									AddModelAttribute(int a) {lynxSetModelAttribute((LPLYNXMODEL)this, (GetModelAttribute()|a)); };		
	};
}

#endif