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
#include <LynxFileStream.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <LynxEngine.h>
#include <LynxContainerManager.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <GraphicsSystem/LynxQuery.h>
#include <CollisionSystem/LynxCollisionHierarchy.h>
#include <Container/LynxContainer.h>
#include <Container/LynxCameraContainer.h>
#include <Container/LynxTerrainContainer.h>
#include <LynxBoxProxy.h>

namespace LynxEngine 
{	
	// {6B68A76B-021D-4e0b-8A5F-6DFED74AD40D}
	const LYNXGUID CContainer::NULL_CONTAINER = 
	{ 0x6b68a76b, 0x21d, 0x4e0b, { 0x8a, 0x5f, 0x6d, 0xfe, 0xd7, 0x4a, 0xd4, 0xd } };

	// {A77D81CA-556A-4079-B7F6-C203ACB180DE}
	const LYNXGUID CContainer::REFERENCE = 
	{ 0xa77d81ca, 0x556a, 0x4079, { 0xb7, 0xf6, 0xc2, 0x3, 0xac, 0xb1, 0x80, 0xde } };

	// {D83E2495-73F0-4a5e-B1F5-7DCD7F05C353}
	const LYNXGUID CContainer::GEOMETRY = 
	{ 0xd83e2495, 0x73f0, 0x4a5e, { 0xb1, 0xf5, 0x7d, 0xcd, 0x7f, 0x5, 0xc3, 0x53 } };
	
	// {804FB2E3-E365-42d0-9B18-7FC3A69FA86D}
	const LYNXGUID CContainer::STATIC_MODEL = 
	{ 0x804fb2e3, 0xe365, 0x42d0, { 0x9b, 0x18, 0x7f, 0xc3, 0xa6, 0x9f, 0xa8, 0x6d } };

	// {1B55E9C6-5CC6-42d6-8F3B-9D03717D06F3}
	const LYNXGUID CContainer::TERRAIN = 
	{ 0x1b55e9c6, 0x5cc6, 0x42d6, { 0x8f, 0x3b, 0x9d, 0x3, 0x71, 0x7d, 0x6, 0xf3 } };

	// {9C80FA2C-FF96-4f47-BEEE-4D8C76B1E621}
	const LYNXGUID CContainer::PROGRESSIVE_TERRAIN = 
	{ 0x9c80fa2c, 0xff96, 0x4f47, { 0xbe, 0xee, 0x4d, 0x8c, 0x76, 0xb1, 0xe6, 0x21 } };

	// {1044DA13-EE71-45e2-91DC-D77A25F02455}
	const LYNXGUID CContainer::SKYDOME = 
	{ 0x1044da13, 0xee71, 0x45e2, { 0x91, 0xdc, 0xd7, 0x7a, 0x25, 0xf0, 0x24, 0x55 } };

	// {CAB00AC2-BA7A-48f6-9276-6FCBEA0E4CC9}
	const LYNXGUID CContainer::VOLUMETRIC_CLOUD = 
	{ 0xcab00ac2, 0xba7a, 0x48f6, { 0x92, 0x76, 0x6f, 0xcb, 0xea, 0xe, 0x4c, 0xc9 } };

	// {52224C72-E9E8-4dcc-B030-3BBDD1188242}
	const LYNXGUID CContainer::TREE = 
	{ 0x52224c72, 0xe9e8, 0x4dcc, { 0xb0, 0x30, 0x3b, 0xbd, 0xd1, 0x18, 0x82, 0x42 } };

	// {FE2D61D6-915C-4c64-8630-B69FF0747C65}
	const LYNXGUID CContainer::GRASS = 
	{ 0xfe2d61d6, 0x915c, 0x4c64, { 0x86, 0x30, 0xb6, 0x9f, 0xf0, 0x74, 0x7c, 0x65 } };

	// {B040C6FE-CE4B-46cb-B6D9-2D7CB3786244}
	const LYNXGUID CContainer::CROSS_GRASS = 
	{ 0xb040c6fe, 0xce4b, 0x46cb, { 0xb6, 0xd9, 0x2d, 0x7c, 0xb3, 0x78, 0x62, 0x44 } };

	// {8F0501A6-3E7C-43cc-86FE-66FC675E2496}
	const LYNXGUID CContainer::CAMERA = 
	{ 0x8f0501a6, 0x3e7c, 0x43cc, { 0x86, 0xfe, 0x66, 0xfc, 0x67, 0x5e, 0x24, 0x96 } };

	// {0DE31F8C-5597-4d48-B460-D4F997817E0B}
	const LYNXGUID CContainer::MODEL = 
	{ 0xde31f8c, 0x5597, 0x4d48, { 0xb4, 0x60, 0xd4, 0xf9, 0x97, 0x81, 0x7e, 0xb } };

	// {4F9CEDCE-ACE5-4366-9C5A-95A6E75E15FF}
	const LYNXGUID CContainer::SPRITE = 
	{ 0x4f9cedce, 0xace5, 0x4366, { 0x9c, 0x5a, 0x95, 0xa6, 0xe7, 0x5e, 0x15, 0xff } };

	// {B0DDAC75-DA19-40d2-8AB9-81B332ADB976}
	const LYNXGUID CContainer::SPRITE3D = 
	{ 0xb0ddac75, 0xda19, 0x40d2, { 0x8a, 0xb9, 0x81, 0xb3, 0x32, 0xad, 0xb9, 0x76 } };

	// {EB6B3EDE-6E9D-4bd0-B34B-E578EEAEE0DB}
	const LYNXGUID CContainer::SPRITE3D_CLUSTER = 
	{ 0xeb6b3ede, 0x6e9d, 0x4bd0, { 0xb3, 0x4b, 0xe5, 0x78, 0xee, 0xae, 0xe0, 0xdb } };

	// {B3C3909E-BAD0-4221-9D4B-B668B94718E6}
	const LYNXGUID CContainer::SPRITE_CLUSTER = 
	{ 0xb3c3909e, 0xbad0, 0x4221, { 0x9d, 0x4b, 0xb6, 0x68, 0xb9, 0x47, 0x18, 0xe6 } };

	// {D2132032-37A8-46a2-A742-0F8D721380B3}
	const LYNXGUID CContainer::VOXEL_CLUSTER = 
	{ 0xd2132032, 0x37a8, 0x46a2, { 0xa7, 0x42, 0xf, 0x8d, 0x72, 0x13, 0x80, 0xb3 } };

	// {CC8EE7B1-10D3-43ca-A1D0-EB582271775A}
	const LYNXGUID CContainer::CLUSTER_SUB = 
	{ 0xcc8ee7b1, 0x10d3, 0x43ca, { 0xa1, 0xd0, 0xeb, 0x58, 0x22, 0x71, 0x77, 0x5a } };
	
	// {1C617B92-9AB9-4aab-9DBF-08583D286FD0}
	const LYNXGUID CContainer::POINT_SPRITE = 
	{ 0x1c617b92, 0x9ab9, 0x4aab, { 0x9d, 0xbf, 0x8, 0x58, 0x3d, 0x28, 0x6f, 0xd0 } };

	// {F6AC9E26-410E-4fb2-B6DA-582B0C87D0FA}
	const LYNXGUID CContainer::CROSS_SPRITE = 
	{ 0xf6ac9e26, 0x410e, 0x4fb2, { 0xb6, 0xda, 0x58, 0x2b, 0xc, 0x87, 0xd0, 0xfa } };

	// {7E2EB7A9-F644-40a9-996F-AEB3260E9D36}
	const LYNXGUID CContainer::PEMITTER = 
	{ 0x7e2eb7a9, 0xf644, 0x40a9, { 0x99, 0x6f, 0xae, 0xb3, 0x26, 0xe, 0x9d, 0x36 } };

	// {4903AFE5-DCA1-407f-8E9C-E379D07C6C70}
	const LYNXGUID CContainer::MESH = 
	{ 0x4903afe5, 0xdca1, 0x407f, { 0x8e, 0x9c, 0xe3, 0x79, 0xd0, 0x7c, 0x6c, 0x70 } };

	// {9182D653-D80D-4f06-83B3-3F712038859D}
	const LYNXGUID CContainer::BONE = 
	{ 0x9182d653, 0xd80d, 0x4f06, { 0x83, 0xb3, 0x3f, 0x71, 0x20, 0x38, 0x85, 0x9d } };

	// {5045D040-8036-4a60-8244-4C4ED3D84CEE}
	const LYNXGUID CContainer::PORTAL = 
	{ 0x5045d040, 0x8036, 0x4a60, { 0x82, 0x44, 0x4c, 0x4e, 0xd3, 0xd8, 0x4c, 0xee } };
	//-------------------------------------------------------------------------------------------------------
	//
	//  ????:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(CContainer::TYPE e)
	{
		CString				ContainerType;

		if (e == CContainer::REFERENCE)
			ContainerType = _T("Reference");
		else if (e == CContainer::GEOMETRY)
			ContainerType = _T("Geometry");
		else if (e == CContainer::STATIC_MODEL)
			ContainerType = _T("Static Model");
		else if (e == CContainer::TERRAIN)
			ContainerType = _T("Terrain");
		else if (e == CContainer::PROGRESSIVE_TERRAIN)
			ContainerType = _T("Progressive Terrain");
		else if (e == CContainer::SKYDOME)
			ContainerType = _T("Skydome");
		else if (e == CContainer::VOLUMETRIC_CLOUD)
			ContainerType = _T("Volumetric Cloud");
		else if (e == CContainer::TREE)
			ContainerType = _T("Tree");
		else if (e == CContainer::GRASS)
			ContainerType = _T("Grass");
		else if (e == CContainer::CROSS_GRASS)
			ContainerType = _T("Cross Grass");
		else if (e == CContainer::CAMERA)
			ContainerType = _T("Camera");
		else if (e == CContainer::MODEL)
			ContainerType = _T("Model");
		else if (e == CContainer::SPRITE)
			ContainerType = _T("Sprite");
		else if (e == CContainer::SPRITE3D)
			ContainerType = _T("Sprite3D");
		else if (e == CContainer::SPRITE3D_CLUSTER)
			ContainerType = _T("Sprite3D Cluster");
		else if (e == CContainer::SPRITE_CLUSTER)
			ContainerType = _T("Sprite Cluster");
		else if (e == CContainer::POINT_SPRITE)
			ContainerType = _T("Point Sprite");
		else if (e == CContainer::CROSS_SPRITE)
			ContainerType = _T("Cross Sprite");
		else if (e == CContainer::PEMITTER)
			ContainerType = _T("Particle Emitter");
		else if (e == CContainer::MESH)
			ContainerType = _T("Mesh");
		else if (e == CContainer::BONE)
			ContainerType = _T("Bone");
		

		/*switch (e)
		{
			case CContainer::REFERENCE:
				ContainerType = _T("Reference");
				break;
			case CContainer::GEOMETRY:
				ContainerType = _T("Geometry");
				break;
			case CContainer::STATIC_MODEL:
				ContainerType = _T("Static Model");
				break;
			case CContainer::TERRAIN:
				ContainerType = _T("Terrain");
				break;
			case CContainer::PROGRESSIVE_TERRAIN:
				ContainerType = _T("Progressive Terrain");
				break;
			case CContainer::SKYDOME:
				ContainerType = _T("Skydome");
				break;
			case CContainer::VOLUMETRIC_CLOUD:
				ContainerType = _T("Volumetric Cloud");
				break;
			case CContainer::TREE:
				ContainerType = _T("Tree");
				break;
			case CContainer::GRASS:
				ContainerType = _T("Grass");
				break;
			case CContainer::CROSS_GRASS:
				ContainerType = _T("Cross Grass");
				break;
			case CContainer::CAMERA:
				ContainerType = _T("Camera");
				break;
			case CContainer::MODEL:
				ContainerType = _T("Model");
				break;
			case CContainer::SPRITE:
				ContainerType = _T("Sprite");
				break;
			case CContainer::SPRITE3D:
				ContainerType = _T("Sprite3D");
				break;
			case CContainer::SPRITE3D_CLUSTER:
				ContainerType = _T("Sprite3D Cluster");
				break;
			case CContainer::POINT_SPRITE:
				ContainerType = _T("Point Sprite");
				break;
			case CContainer::CROSS_SPRITE:
				ContainerType = _T("Cross Sprite");
				break;
			case CContainer::PEMITTER:
				ContainerType = _T("Particle Emitter");
				break;
			case CContainer::MESH:
				ContainerType = _T("Mesh");
				break;
			case CContainer::BONE:
				ContainerType = _T("Bone");
				break;		
		}*/
		return ContainerType;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	CContainer::TYPE TranslateContainerType(const CString& s)
	{
		CContainer::TYPE Type(CContainer::NULL_CONTAINER);

		if (s == _T("Reference"))
			Type = CContainer::REFERENCE;
		else if (s == _T("Geometry"))
			Type = CContainer::GEOMETRY;
		else if (s == _T("Static Model"))
			Type = CContainer::STATIC_MODEL;
		else if (s == _T("Terrain"))
			Type = CContainer::TERRAIN;
		else if (s == _T("Progressive Terrain"))
			Type = CContainer::PROGRESSIVE_TERRAIN;
		else if (s == _T("Skydome"))
			Type = CContainer::SKYDOME;
		else if (s == _T("Volumetric Cloud"))
			Type = CContainer::VOLUMETRIC_CLOUD;
		else if (s == _T("Tree"))
			Type = CContainer::TREE;
		else if (s == _T("Grass"))
			Type = CContainer::GRASS;
		else if (s == _T("Cross Grass"))
			Type = CContainer::CROSS_GRASS;
		else if (s == _T("Camera"))
			Type = CContainer::CAMERA;
		else if (s == _T("Model"))
			Type = CContainer::MODEL;
		else if (s == _T("Sprite"))
			Type = CContainer::SPRITE;
		else if (s == _T("Sprite3D"))
			Type = CContainer::SPRITE3D;
		else if (s == _T("Sprite3D Cluster"))
			Type = CContainer::SPRITE3D_CLUSTER;
		else if (s == _T("Sprite Cluster"))
			Type = CContainer::SPRITE_CLUSTER;
		else if (s == _T("Point Sprite"))
			Type = CContainer::POINT_SPRITE;
		else if (s == _T("Cross Sprite"))
			Type = CContainer::CROSS_SPRITE;
		else if (s == _T("Particle Emitter"))
			Type = CContainer::PEMITTER;
		else if (s == _T("Mesh"))
			Type = CContainer::MESH;
		else if (s == _T("Bone"))
			Type = CContainer::BONE;
		
		return Type;
	}
	IMPLEMENT_CLASSSCRIPT(CContainer, CObj)
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	bool CContainer::compare_depthlayer_b2f(CContainer* e1, CContainer* e2) 
	{ 
		return (e1->GetlpRenObj()->GetDepthLayer()) > (e2->GetlpRenObj()->GetDepthLayer()); 
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	bool CContainer::compare_depthlayer_f2b(CContainer* e1, CContainer* e2) 
	{ 
		return (e1->GetlpRenObj()->GetDepthLayer()) < (e2->GetlpRenObj()->GetDepthLayer()); 
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::InitMembers()		
	{
		m_ResourceType = CONTAINER;
		m_bRenderable = LYNX_TRUE;
		m_lpRenObj = NULL;
		m_bSelfIlluminated = LYNX_FALSE;
		m_IlluminatingIntensity = 3.0f;
		m_bReflect = LYNX_FALSE;
		m_bRefract = LYNX_FALSE;
		m_bMotionBlur = LYNX_FALSE; 
		m_bSort = LYNX_FALSE;
		m_bTranslucent = LYNX_FALSE;
		m_bCastShadow = LYNX_TRUE;
		m_bReceiveShadow = LYNX_TRUE;
		m_bOccluder = LYNX_FALSE;

		m_bOccluded = LYNX_FALSE;
		m_lpOcclusionQuery = NULL;
		m_DrawnPixels = 0;
		m_ProxyList.clear();

		m_Diffuse.Red = 255;
		m_Diffuse.Green = 255;
		m_Diffuse.Blue = 255;
		m_Diffuse.Alpha = 255;

		//m_lpCurrentMaterial = NULL;
		m_CurrentMaterialTechnique = MaterialSystem::CMaterial::SIMPLE;
		m_Visibility = LYNX_REAL(1.0f);		
		m_bUseVisibilty = LYNX_FALSE;

		m_lpCenter = (Math::CVector3*)&m_M.m41;
		m_lpRightDir = (Math::CVector3*)&m_M.m11;
		m_lpUpDir = (Math::CVector3*)&m_M.m21;
		m_lpDirection = (Math::CVector3*)&m_M.m31;
		
		LYNX_IDENTITY_MATRIX(m_LastM);
		LYNX_IDENTITY_MATRIX(m_LastWVPM);
		LYNX_IDENTITY_MATRIX(m_LocalM);
		LYNX_IDENTITY_MATRIX(m_GlobalM);
		LYNX_IDENTITY_MATRIX(m_M);		
		LYNX_IDENTITY_MATRIX(m_AnimationM);
		LYNX_IDENTITY_MATRIX(m_TextureUVM);
		LYNX_IDENTITY_MATRIX(m_PushedLocalM);
		LYNX_IDENTITY_MATRIX(m_PushedGlobalM);
		LYNX_IDENTITY_MATRIX(m_PushedM);
		m_PivotM.Identity();
		m_InversePivotM.Identity();
		
		m_SortMode = SM_NONE;

		m_NumLODs = 0;	
		m_CurrentLOD = 0;
		m_LODBias = 0;
		m_MinLODDistance = 0;
		m_MaxLODDistance = 300;

		m_lpParentContainer = NULL;
		m_SubContainerList.clear();
		m_lpParent = NULL;
		m_lpChildrenArray.clear();					
		
		//m_bSkipTransfomScale = LYNX_FALSE;		

		IMPLEMENT_PARAM_BOOL(m_bSelfIlluminated, 0)
		IMPLEMENT_PARAM_FLOAT(m_IlluminatingIntensity, 0)
		IMPLEMENT_PARAM_BOOL(m_bMotionBlur, 0)
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bReflect, _T("Is this container reflective?"), 0)
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bRefract, _T("Is this container refractive?"), 0);
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bSort, _T("Does this object need to be sorted"), 0)
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bTranslucent, _T("Is this object translucent"), 0)
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bCastShadow, _T("Does this object cast shadow or not"), 0)
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bReceiveShadow, _T("Does this object receive shadow or not"), 0)
		IMPLEMENT_PARAM_BOOL_COMMENT(m_bOccluder, _T("Is this object used as occluder"), 0)
		IMPLEMENT_PARAM_INT_COMMENT(m_LODBias, _T("LOD bias"), 0)
		IMPLEMENT_PARAM_FLOAT_COMMENT(m_MinLODDistance, _T("The min distance to use the first LOD"), 0)
		IMPLEMENT_PARAM_FLOAT_COMMENT(m_MaxLODDistance, _T("The max distance to use the last LOD"), 0)

		READYONLY_PARAM(m_Name);

		//IMPLEMENT_PARAM_LPOBJ(m_lpRenObj, 0)
		IMPLEMENT_PARAM_LPOBJ(m_lpParentContainer, 0)
		//IMPLEMENT_PARAM_LPOBJ(m_lpCurrentMaterial, 0)	
		IMPLEMENT_PARAM_COUNTEDPTR(m_lpCurrentMaterial, 0)			
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer::CContainer(CEngine *lpengine)		
	: CResource(lpengine)
	{		
		InitMembers();	
		m_lpCollisionHierarchy = LYNXNEW CollisionSystem::CCollisionHierarchy;
		m_AnimationSet = Animation::CAnimationSetPtr(LYNXNEW Animation::CAnimationSet(lpengine));
		INIT_CLASSSCRIPT
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer::~CContainer(void)
	{
		LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));

		if (m_lpOcclusionQuery)
			LYNXDEL m_lpOcclusionQuery;

		for (int i=0; i<m_ProxyList.size(); i++)
		{				
			LYNXDEL m_ProxyList[i];
		}		
		m_ProxyList.clear();		

		m_SubContainerList.clear();
		m_lpChildrenArray.clear();							

		if (m_lpCollisionHierarchy)
		{
			LYNXDEL m_lpCollisionHierarchy;
			m_lpCollisionHierarchy = NULL;
		}

		m_AnimationSet.Reset();

		//if (m_lpEngine)
		//	((CEngine*)m_lpEngine)->GetContainerManager().Remove(this);		
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CCameraContainer* CContainer::CCameraContainer_Cast(CContainer* c) 
	{ 
		return dynamic_cast<CCameraContainer*>(c); 
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CGeometryContainer*	CContainer::CGeometryContainer_Cast(CContainer* c) 
	{ 
		return dynamic_cast<CGeometryContainer*>(c); 
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	CPEmitterContainer*	CContainer::CPEmitterContainer_Cast(CContainer* c)
	{
		return dynamic_cast<CPEmitterContainer*>(c); 
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CContainer::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
	{		
		RegisterSuperClassMethod(lps);

		REGISTER_SCRIPT_METHOD(lps, "CCameraContainer_Cast",	LynxEngine::CContainer,	void*,		CCameraContainer_Cast,		(void*))		
		REGISTER_SCRIPT_METHOD(lps, "CGeometryContainer_Cast",	LynxEngine::CContainer,	void*,		CGeometryContainer_Cast,	(void*))		
		REGISTER_SCRIPT_METHOD(lps, "CPEmitterContainer_Cast",	LynxEngine::CContainer,	void*,		CPEmitterContainer_Cast,	(void*))		
		REGISTER_SCRIPT_METHOD(lps, "Render",					LynxEngine::CContainer,	void,		_Render,					(void*))		
		REGISTER_SCRIPT_METHOD(lps, "UpdateTransform",			LynxEngine::CContainer,	void,		UpdateTransform,			(void))		
		REGISTER_SCRIPT_METHOD(lps, "ResetTransform",			LynxEngine::CContainer,	void,		ResetTransform,				(void))
		REGISTER_SCRIPT_METHOD(lps, "Yaw",						LynxEngine::CContainer,	void,		Yaw,						(const Math::CReal, int))
		REGISTER_SCRIPT_METHOD(lps, "Pitch",					LynxEngine::CContainer,	void,		Pitch,						(const Math::CReal, int))
		REGISTER_SCRIPT_METHOD(lps, "SetPosition",				LynxEngine::CContainer,	void,		vSetPosition,				(const Math::CVector3&))		
		REGISTER_SCRIPT_METHOD(lps, "Play",						LynxEngine::CContainer,	LYNXBOOL,	_Play,						(const Math::CReal))		

		REGISTER_SCRIPT_METHOD(lps, "CreateAnimation",			LynxEngine::CContainer,	void*,		CreateAnimation,			(const LYNXCHAR*))		
		REGISTER_SCRIPT_METHOD(lps, "SetCurrentAnimation",		LynxEngine::CContainer,	void,		_SetCurrentAnimation,		(void*))		
		

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	const LYNXGUID CContainer::ScriptType2EngineType(int type)
	{
		static LYNXGUID TypeMap[] = { NULL_CONTAINER,
									  REFERENCE,
									  GEOMETRY,
									  STATIC_MODEL,
									  TERRAIN,
									  PROGRESSIVE_TERRAIN,
									  SKYDOME,
									  VOLUMETRIC_CLOUD,
									  TREE,
									  GRASS,
									  CROSS_GRASS,
									  CAMERA,
									  MODEL,
									  SPRITE,
									  SPRITE3D,
									  SPRITE3D_CLUSTER,
									  SPRITE_CLUSTER,
									  VOXEL_CLUSTER,
									  POINT_SPRITE,
									  CROSS_SPRITE,
									  PEMITTER,
									  MESH,
									  BONE,
									  PORTAL };
		return TypeMap[type];		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer& CContainer::operator=(CContainer& rhs)
	{
		m_lpEngine = rhs.m_lpEngine;		
		m_ContainerType = rhs.m_ContainerType;				
		m_SourceType = rhs.m_SourceType;

		m_NumLODs = rhs.m_NumLODs;	
		m_CurrentLOD = rhs.m_CurrentLOD;

		LYNX_MATRIX_COPY(m_M, rhs.m_M);
		LYNX_MATRIX_COPY(m_LastM, rhs.m_LastM);
		LYNX_MATRIX_COPY(m_LastWVPM, rhs.m_LastWVPM);
		LYNX_MATRIX_COPY(m_LocalM, rhs.m_LocalM);
		LYNX_MATRIX_COPY(m_GlobalM, rhs.m_GlobalM);
		LYNX_MATRIX_COPY(m_PivotM, rhs.m_PivotM);							
		LYNX_MATRIX_COPY(m_InversePivotM, rhs.m_InversePivotM);							
		
		m_bSelfIlluminated = rhs.m_bSelfIlluminated;
		m_IlluminatingIntensity = rhs.m_IlluminatingIntensity;
		m_bMotionBlur = rhs.m_bMotionBlur;
		m_bReflect = rhs.m_bReflect;
		m_bRefract = rhs.m_bRefract;
		lynxMemCpy(&m_RNRPlane, &rhs.m_RNRPlane, sizeof(LYNXPLANE));			
		m_Visibility = rhs.m_Visibility;			

		m_AnimationSet = rhs.m_AnimationSet;

		m_lpParent = rhs.m_lpParent;
		m_lpChildrenArray = rhs.m_lpChildrenArray;

		m_lpRenObj = rhs.m_lpRenObj;
		m_SubContainerList = rhs.m_SubContainerList;		

		m_lpCurrentMaterial = rhs.m_lpCurrentMaterial;
		m_CurrentMaterialTechnique = rhs.m_CurrentMaterialTechnique;
		
		//m_bSkipTransfomScale = rhs.m_bSkipTransfomScale;
		*(m_lpCollisionHierarchy) = *(rhs.m_lpCollisionHierarchy);
		for (int i=0; i<m_lpCollisionHierarchy->GetNumLevels(); i++)
		{
			for (CList<CollisionSystem::CCollisionObj*>::CIterator j=m_lpCollisionHierarchy->GetCollisionObjList(i).begin(); j!=m_lpCollisionHierarchy->GetCollisionObjList(i).end(); ++j)
			{
				(*j)->SetParentContainer(this);
			}			
		}

		return (*this);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vInstance(const CContainer& rhs)
	{		
		CResource::vInstance(rhs);

		m_ContainerType = rhs.m_ContainerType;								

		m_NumLODs = rhs.m_NumLODs;	
		m_CurrentLOD = rhs.m_CurrentLOD;

		LYNX_MATRIX_COPY(m_M, rhs.m_M);
		LYNX_MATRIX_COPY(m_LastM, rhs.m_LastM);
		LYNX_MATRIX_COPY(m_LastWVPM, rhs.m_LastWVPM);
		LYNX_MATRIX_COPY(m_LocalM, rhs.m_LocalM);
		LYNX_MATRIX_COPY(m_GlobalM, rhs.m_GlobalM);
		LYNX_MATRIX_COPY(m_PivotM, rhs.m_PivotM);							
		LYNX_MATRIX_COPY(m_InversePivotM, rhs.m_InversePivotM);							
		
		m_bSelfIlluminated = rhs.m_bSelfIlluminated;
		m_IlluminatingIntensity = rhs.m_IlluminatingIntensity;
		m_bMotionBlur = rhs.m_bMotionBlur;
		m_bReflect = rhs.m_bReflect;
		m_bRefract = rhs.m_bRefract;
		lynxMemCpy(&m_RNRPlane, &rhs.m_RNRPlane, sizeof(LYNXPLANE));			
		m_Visibility = rhs.m_Visibility;			
		m_bUseVisibilty = rhs.m_bUseVisibilty;

		//m_AnimationSet = rhs.m_AnimationSet;
		m_AnimationSet->vInstance(*(rhs.m_AnimationSet));
		
		m_lpCurrentMaterial = rhs.m_lpCurrentMaterial;
		m_CurrentMaterialTechnique = rhs.m_CurrentMaterialTechnique;

		// copy first, then overwrite it later with the pointer of the instanced containers.
		m_lpParent = rhs.m_lpParent;
		m_lpChildrenArray = rhs.m_lpChildrenArray;		

		//m_bSkipTransfomScale = rhs.m_bSkipTransfomScale;
		*(m_lpCollisionHierarchy) = *(rhs.m_lpCollisionHierarchy);
		for (int i=0; i<m_lpCollisionHierarchy->GetNumLevels(); i++)
		{
			for (CList<CollisionSystem::CCollisionObj*>::CIterator j=m_lpCollisionHierarchy->GetCollisionObjList(i).begin(); j!=m_lpCollisionHierarchy->GetCollisionObjList(i).end(); ++j)
			{
				(*j)->SetParentContainer(this);
			}			
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::ResetChildrenArray() 
	{
		for (int i=0; i<m_lpChildrenArray.size(); i++)
		{
			m_lpChildrenArray[i]->SetlpParent(NULL);
		}
		m_lpChildrenArray.clear(); 
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::ClearChildrenArray() 
	{
		m_lpChildrenArray.clear(); 
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::RemoveChild(CContainer* c)
	{
		CArray<CContainer*>::CIterator Con;
		Con = LynxEngine::find(m_lpChildrenArray.begin(), m_lpChildrenArray.end(), c);
		if (Con != m_lpChildrenArray.end())
		{			
			if ((*Con)->GetlpParent())
			{
				(*Con)->SetlpParent(NULL);
			}
			m_lpChildrenArray.erase(Con);			
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::RecursionTraverse(CArray<CContainer*>& a, int* index, CContainer* c)
	{	
		a[(*index)++] = c;
		for (int i=0; i<c->GetNumChildren(); i++)
		{
			RecursionTraverse(a, index, c->GetlpChild(i));
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::MakeTraverseArray(CArray<CContainer*>& a, int size, CContainer* root)
	{	
		int TraverseIndex = 0;
		a.clear();
		a.resize(size);
		RecursionTraverse(a, &TraverseIndex, root);
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::ResetOrientation(LYNXBOOL breset_scale)
	{
		CVector3 ScaleVec;
		if (!breset_scale)
		{		
			m_M.GetScale(ScaleVec);
		}

		LYNX_RESET_MATRIX_ORIENTATION(m_M);
		LYNX_RESET_MATRIX_ORIENTATION(m_LocalM);
		LYNX_RESET_MATRIX_ORIENTATION(m_GlobalM);				

		if (!breset_scale)
		{
			Scale(ScaleVec, LYNX_MATH_PRE);
			UpdateTransform();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::ResetTransform()
	{
		LYNX_IDENTITY_MATRIX(m_M);
		LYNX_IDENTITY_MATRIX(m_LocalM);
		LYNX_IDENTITY_MATRIX(m_GlobalM);				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::PushTransform()
	{
		lynxMemCpy(&m_PushedLocalM, &m_LocalM, sizeof(Math::CMatrix4));
		lynxMemCpy(&m_PushedGlobalM, &m_GlobalM, sizeof(Math::CMatrix4));
		lynxMemCpy(&m_PushedM, &m_M, sizeof(Math::CMatrix4));
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::PopTransform()
	{
		lynxMemCpy(&m_LocalM, &m_PushedLocalM, sizeof(Math::CMatrix4));
		lynxMemCpy(&m_GlobalM, &m_PushedGlobalM, sizeof(Math::CMatrix4));
		lynxMemCpy(&m_M, &m_PushedM, sizeof(Math::CMatrix4));
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Scale(const Math::CVector3& v, LYNXMATHORDER order)
	{
		Math::CMatrix4				T, R;

		LYNX_IDENTITY_MATRIX(T);
		T.m11 = v.x;
		T.m22 = v.y;
		T.m33 = v.z;

		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_LocalM, &T);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetScale(const Math::CVector3& v)
	{
		m_LocalM.m11 = v.x;
		m_LocalM.m22 = v.z;
		m_LocalM.m33 = v.y;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Translate(const Math::CVector3& v, LYNXMATHORDER order)
	{			
		Math::CMatrix4 T, R;

		LYNX_IDENTITY_MATRIX(T);
		T.m41 = v.x;
		T.m42 = v.y;
		T.m43 = v.z;

		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_LocalM, &T);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Rotate(const Math::CReal f, LYNXAXISTYPE axis, LYNXMATHORDER order)
	{	
		Math::CMatrix4 T, R;

		lynxMakeRotateMatrix(&T, f, axis);	
		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_LocalM, &T);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Rotate(const Math::CReal ang, const Math::CVector3& axis, LYNXMATHORDER order)
	{	
		Math::CMatrix4 T, R;

		lynxMakeRotateMatrixRespectVector(&T, &axis, ang);	
		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_LocalM, &T);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Rotate(const Math::CVector3& euler, LYNXMATHORDER order)
	{	
		Math::CMatrix4 T, R;

		lynxEulerAnglesXYZ2Matrix(&T, &euler);	
		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_LocalM, &T);	
				lynxMemCpy(&m_LocalM, &R, sizeof(Math::CMatrix4));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::UpdateTransform()
	{
		Math::CMatrix4				TM1;
		
		SaveTransformationMatrix();

		lynxMatrixXMatrix(&m_M, &m_LocalM, &m_GlobalM);
		if (m_AnimationSet->GetCurrentAnimation())			
			m_AnimationSet->GetCurrentAnimation()->m_M = m_M;	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vSetPosition(const Math::CVector3& pos)
	{	
		Math::CVector3 V;

		V.x = pos.x - m_lpCenter->x;
		V.y = pos.y - m_lpCenter->y;
		V.z = pos.z - m_lpCenter->z;

		Translate(V, LYNX_MATH_POST);	
		UpdateTransform();	
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vGetPosition(Math::CVector3& pos) const
	{	
		pos.x = m_lpCenter->x;
		pos.y = m_lpCenter->y;
		pos.z = m_lpCenter->z;	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vGetScreenPosition(Math::CVector2& pos, CCameraContainer* cam, int w, int h) const
	{
		float HalfWidth, HalfHeight;

		HalfWidth = float(w) * 0.5f;//m_lpEngine->GetlpGraphicsSystem()->GetCurrentRenderBuffer(0)->vGetWidth() * 0.5f;
		HalfHeight = float(h) * 0.5f;//m_lpEngine->GetlpGraphicsSystem()->GetCurrentRenderBuffer(0)->vGetHeight() * 0.5f;
		Math::CMatrix4  WVPM;
		Math::CMatrix4 ViewM = cam->GetViewMatrix();
		Math::CMatrix4 ProjM = cam->GetProjectionMatrix();			
		lynxMatrixXMatrix(WVPM, ViewM, ProjM);		

		Math::CVector4 Pos4 = Math::CVector4(m_lpCenter->x, m_lpCenter->y, m_lpCenter->z, 1.0f);
		Math::CVector4 ScreenPos = Pos4 * WVPM;
		ScreenPos.x /= ScreenPos.w;
		ScreenPos.y /= ScreenPos.w;      
		pos.x = (ScreenPos.x * HalfWidth + HalfWidth);
		pos.y = (HalfHeight - ScreenPos.y * HalfHeight);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vGetScreenPosition(Math::CVector2& pos, CCameraContainer* cam) const
	{
		m_lpEngine->GetScreenPosition(pos, *m_lpCenter, *cam, cam->GetViewportRect());
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vGetOBBScreenRect(LYNXRECT& rect, CCameraContainer& cam)
	{
		CollisionSystem::CCollisionObj* ObjB = *(GetlpCollisionHierarchy()->GetCollisionObjList(1).begin());		//OBB
		if (ObjB->GetType() == CollisionSystem::CCollisionObj::OBB)
		{
			CollisionSystem::CBtOBB* OBB = static_cast<CollisionSystem::CBtOBB*>(ObjB);
			OBB->ScreenRect(rect, cam);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer:: Forward(const Math::CReal d)
	{	
		Math::CVector3 V;
		
		V.x = LYNX_REAL_MUL(m_lpDirection->x, d);
		V.y = LYNX_REAL_MUL(m_lpDirection->y, d);
		V.z = LYNX_REAL_MUL(m_lpDirection->z, d);

		Translate(V, LYNX_MATH_POST);	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SideMove(const Math::CReal d)
	{	
		Math::CVector3 V;
		
		V.x = LYNX_REAL_MUL(m_lpRightDir->x, d);
		V.y = LYNX_REAL_MUL(m_lpRightDir->y, d);
		V.z = LYNX_REAL_MUL(m_lpRightDir->z, d);

		Translate(V, LYNX_MATH_POST);	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Up(const Math::CReal d)
	{	
		Math::CVector3 V;
		
		V.x = LYNX_REAL_MUL(m_lpUpDir->x, d);
		V.y = LYNX_REAL_MUL(m_lpUpDir->y, d);
		V.z = LYNX_REAL_MUL(m_lpUpDir->z, d);

		Translate(V, LYNX_MATH_POST);	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::MulMatrix(const Math::CMatrix4& m, LYNXMATHORDER order)
	{
		Math::CMatrix4 T;

		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&T, &m, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &T, sizeof(Math::CMatrix4));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&T, &m_LocalM, &m);	
				lynxMemCpy(&m_LocalM, &T, sizeof(Math::CMatrix4));
				break;
		}
	}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void GetObject3DTransM(LPLYNXOBJ3D lpo, LYNXREAL time)
	//{
	//	LYNXREAL					lpValues[4];	
	//	
	//	LYNX_IDENTITY_MATRIX(m_M);		 		
	//	if (m_RotateKey.lpKeys)
	//	{
	//		#ifdef __FIXED_POINT__
	//			lynxGetAnimKeyValues(&m_RotateKey, time, lpValues, 4);
	//		#else
	//			lynxGetShortAnimKeyValues(&m_RotateKey, time, lpValues, 4, LYNX_ROTATE_KEY_ISCALE);
	//		#endif
	//		lynxQuaternion2Matrix(&m_M, (LPLYNXQUATERNION)lpValues);
	//	}			
	//	if (m_ScaleKey.lpKeys)
	//	{
	//		lynxGetAnimKeyValues(&m_ScaleKey, time, lpValues, 3);

	//		m_M.m11 = LYNX_REAL_MUL(m_M.m11, lpValues[0]);
	//		m_M.m21 = LYNX_REAL_MUL(m_M.m21, lpValues[1]);
	//		m_M.m31 = LYNX_REAL_MUL(m_M.m31, lpValues[2]);

	//		m_M.m12 = LYNX_REAL_MUL(m_M.m12, lpValues[0]);
	//		m_M.m22 = LYNX_REAL_MUL(m_M.m22, lpValues[1]);
	//		m_M.m32 = LYNX_REAL_MUL(m_M.m32, lpValues[2]);

	//		m_M.m13 = LYNX_REAL_MUL(m_M.m13, lpValues[0]);
	//		m_M.m23 = LYNX_REAL_MUL(m_M.m23, lpValues[1]);
	//		m_M.m33 = LYNX_REAL_MUL(m_M.m33, lpValues[2]);
	//	}			
	//	if (m_TranslateKey.lpKeys)
	//	{
	//		lynxGetAnimKeyValues(&m_TranslateKey, time, lpValues, 3);

	//		m_M.m41 += lpValues[0];
	//		m_M.m42 += lpValues[1];
	//		m_M.m43 += lpValues[2];
	//	}	
	//	
	//	if (m_VisKey.lpKeys)
	//	{
	//		#ifdef __FIXED_POINT__
	//			lynxGetAnimKeyValues(&m_VisKey, time, lpValues, 1);
	//		#else
	//			lynxGetShortAnimKeyValues(&m_VisKey, time, lpValues, 1, LYNX_ROTATE_KEY_ISCALE);
	//		#endif
	//		m_Visibility = lpValues[0];
	//	}	
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vGetCenterAndRadius(Math::CVector3& center, Math::CReal& radius)
	{
		vGetPosition(center);		
		radius = 10.0f;

		if (m_lpCollisionHierarchy->GetNumLevels() > 0)
		{
			CollisionSystem::CSphere* lpSphere = dynamic_cast<CollisionSystem::CSphere*>(m_lpCollisionHierarchy->GetCollisionObjList(0)[0]);
			if (lpSphere)
			{
				Math::CVector3 Center = lpSphere->GetCenter();
				Math::PointXMatrix(center, Center, m_M);
				Math::CReal R = lpSphere->GetRadius();
				Math::CVector4 Radius(R, R, R, 0);
				Math::CVector4 TR = Radius * m_M;
				radius = LYNX_MAX(LYNX_MAX(TR.x, TR.y), TR.z);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//**
	//*  @brief .
	//*
	//*	@param  v: lookat dir.	
	//*	@return. 
	//*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::LookAt(const Math::CVector3& v)
	{
        CVector3 Pos;
        CMatrix4 LookAtMatrix;        

		//LYNX_ASSERT(v.Length(3) != 0.0f);

		//if (m_ContainerType != CContainer::PEMITTER && m_ContainerType != CContainer::REFERENCE)
			//LYNX_ASSERT(v.y == 0.0f);

		if (v.Length(3) == 0.0f)
			return;

		CVector3 ScaleVec;
		m_M.GetScale(ScaleVec);
		
	    vGetPosition(Pos);
        lynxMakeLookAtMatrix(&LookAtMatrix, &v);
        LookAtMatrix.m41 = Pos.x;
        LookAtMatrix.m42 = Pos.y;
        LookAtMatrix.m43 = Pos.z;
        ResetTransform();
        SetTransformMatrix(LookAtMatrix);
        SetLocalMatrix(LookAtMatrix);
		Scale(ScaleVec, LYNX_MATH_PRE);
        UpdateTransform();
	}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void lynxAlignObject3DToDir(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpv)
	//{
	//	LYNXVECTOR3D		TN, TU, TV;
	//	LYNXVECTOR3D		dv,dn;
	//	float				f;
	//	Math::CMatrix4			R;
	//	

	//	TN.x = lpv->x;
	//	TN.y = lpv->y;
	//	TN.z = lpv->z;

	//	lynxNormalise(&TN, 3);

	//	dv.x = TN.x; dv.y = TN.y + 1.0f; dv.z = TN.z;
	//	lynxNormalise(&dv, 3);
	//	f = LYNX_DOT_PRODUCT(dv, TN);
	//	LYNX_VECTOR3D_MUL(dn, f, TN);
	//	LYNX_VECTOR3D_SUB(TV, dv, dn);
	//	lynxNormalise(&TV, 3);
	//	lynxCrossProduct(&TU, &TV, &TN);

	//	LYNX_MATRIX(m_LocalM,
	//				TU.x , TV.x , TN.x , 0.0f,
	//				TU.y , TV.y , TN.y , 0.0f,
	//				TU.z , TV.z , TN.z , 0.0f,
	//				0.0f , 0.0f , 0.0f , 1.0f);		

	//	LYNX_MATRIX(m_LocalM,
	//				TU.x , TU.y , TU.z , 0.0f,
	//				TV.x , TV.y , TV.z , 0.0f,
	//				TN.x , TN.y , TN.z , 0.0f,				   
	//				0.0f , 0.0f , 0.0f , 1.0f);		
	//				   

	//	/*
	//	lynxMemCpy(&m_LocalM, &m_R, sizeof(Math::CMatrix4));
	//	*/

	//	
	//	/*
	//	dv.x = (lpv->x - m_TCenter.x);
	//	dv.y = (lpv->y - m_TCenter.y);
	//	dv.z = (lpv->z - m_TCenter.z);	
	//	lynxNormalise(&dv, 3);

	//	lynxVectorXMatrix(&m_TDirection, &m_Direction, &m_LocalM);

	//	TN.x = m_TDirection.x;
	//	TN.y = -dv.y + 1.0f;
	//	TN.z = m_TDirection.z;
	//	//TN.x = -dv.x; TN.y = -dv.y + 1.0f; TN.z = -dv.z;
	//	lynxNormalise(&TN, 3);
	//	f = LYNX_DOT_PRODUCT(dv, TN);
	//	LYNX_VECTOR3D_MULTIPLY(dn, f, TN);
	//	LYNX_VECTOR3D_SUBTRACT(TV, dv, dn);
	//	lynxNormalise(&TV, 3);
	//	lynxCrossProduct(&TU, &TV, &TN);

	//	LYNX_MAKE_MATRIX(m_R,
	//				TU.x , TV.x , TN.x , 0.0f,
	//				TU.y , TV.y , TN.y , 0.0f,
	//				TU.z , TV.z , TN.z , 0.0f,
	//				0.0f , 0.0f , 0.0f , 1.0f);			

	//	//lynxMatrixXMatrix(&R, &m_R, &m_LocalM);	 
	//	lynxMemCpy(&m_LocalM, &m_R, sizeof(Math::CMatrix4));	
	//	*/
	//}		
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	//void CContainer::CreateFrame(DWORD num)
	//{
	//	m_FrameArray.resize(num);		
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SaveTransformationMatrix()
	{
		LYNX_MATRIX_COPY(m_LastM, m_M);
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			(*Sub)->SaveTransformationMatrix();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetlpCurrentMaterialToAllSubContainers(const MaterialSystem::CMaterialPtr& lpm)
	{
		SetlpCurrentMaterial(lpm);
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			(*Sub)->SetlpCurrentMaterial(lpm);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::AddToCurrentMaterialRenderList() 
	{
		if (m_lpCurrentMaterial && m_bRenderable) 
			m_lpCurrentMaterial->AddRenderableObj(this); 

		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			(*Sub)->AddToCurrentMaterialRenderList();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::AddToRenderList(CList<CContainer*>& list)
	{
		if (m_bRenderable)
			list.push_back(this); 

		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			(*Sub)->AddToRenderList(list);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetlpRenObj(CRenderableObj* lpp)		
	{
		m_lpRenObj = lpp;		
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			(*Sub)->SetlpRenObj(lpp);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetCurrentMaterialTechnique(int p) 
	{
		m_CurrentMaterialTechnique = p;		
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			(*Sub)->SetCurrentMaterialTechnique(p);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
     *	@brief ?C
     *
     *	@param ?C
     *	@param ?C  
     *	@return ?C 
     */
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Sort(LYNXBOOL b)
	{
		m_bSort = b;
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			(*Sub)->Sort(b);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetCurrentLight(SceneSystem::CLight& lit)
	{
		m_lpCurrentMaterial->GetlpMaterialSystem()->SetlpCurrentLight(&lit);
		switch (lit.GetLightType())
		{
			case SceneSystem::CLight::AMBIENT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);
				break;
			case SceneSystem::CLight::POINT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);
				break;
			case SceneSystem::CLight::SPOT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);
				break;
			case SceneSystem::CLight::DIRECTIONAL:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);
				break;
		}
	}		
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetReflective(LYNXBOOL b)
	{
		m_bReflect = b;		
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			(*Sub)->SetReflective(b);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CContainer::IsReflective()
	{
		if (m_bReflect) 
			return LYNX_TRUE;

		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			if ((*Sub)->IsReflective())
				return LYNX_TRUE;

		return LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetRefractive(LYNXBOOL b)
	{
		m_bRefract = b;		
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			(*Sub)->SetRefractive(b);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CContainer::IsRefractive()
	{
		if (m_bRefract)
			return LYNX_TRUE;

		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			if ((*Sub)->IsReflective())
				return LYNX_TRUE;
		}
		return LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetSelfIlluminated(LYNXBOOL b)
	{
		m_bSelfIlluminated = b;		
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			(*Sub)->SetSelfIlluminated(b);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetIlluminatingIntensity(CReal r)
	{
		m_IlluminatingIntensity = r;		
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			(*Sub)->SetIlluminatingIntensity(r);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetMotionBlur(LYNXBOOL b)
	{
		m_bMotionBlur = b;		
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
			(*Sub)->SetMotionBlur(b);
	}		
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CContainer::vCreate(const CCreationDesc* desc) 
	{ 		
		if (desc && !desc->m_Name.empty())
			SetName(desc->m_Name);
		else
			SetName(desc->m_FileName);
		
		return LYNX_TRUE; 
	}						
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	Animation::CAnimation* CContainer::vCreateAnimation() 
	{
		return LYNXNEW Animation::CAnimation(m_lpEngine); 
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	Animation::CAnimation* CContainer::CreateAnimation(const LYNXCHAR* name)
	{
		Animation::CAnimation* lpAni = LYNXNEW Animation::CAnimation(m_lpEngine); 
		lpAni->SetName(name);
		AddAnimation(lpAni);

		return lpAni;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	Animation::CAnimation* CContainer::vLoadAnimation(const CCreationDesc* desc) 
	{ 		
		char				Mark[] = "LYNXANI";	
		char				Check[8], Name[64];	
		DWORD				MainVer, Num;				
		LYNXREAL			KeyValues[4];
		int					ST, ET;
		Math::CMatrix4		Matrix;
		CStream*			lpStream = 	desc->m_lpStream;

#if 1
		OPEN_Z_FILE(lpStream, desc->m_FileName)
		else
		{
			if (!lpStream->vOpen(desc->m_FileName.c_str()))
				return NULL;
		}
#else
        PACKAGE_OPEN_FILE(lpStream, desc->m_FileName, CStream::Read|CStream::Binary)
		if (!bResult)
		{
			return NULL;
		}
#endif
		lpStream->vRead(&Check, 7, 1);
		Check[7] = 0x00;
		if (strcmp(Mark, Check))	
			goto ErrorExit;					
		lpStream->vRead(&MainVer, sizeof(DWORD), 1);    				

		lpStream->vRead(&Num, sizeof(DWORD), 1);			
		for (int i=0; i<Num; i++)
		{
			lpStream->vRead(Name, sizeof(char), 64);
			Animation::CAnimation* lpAni = NULL;
			lpAni = vCreateAnimation();
			AddAnimation(lpAni);
			lpAni->SetName(desc->m_Name);
			lpStream->vRead(&Matrix , sizeof(LYNXMATRIX), 1);
			SetPivotMatrix(Matrix);
			lpStream->vRead(&Matrix , sizeof(LYNXMATRIX), 1);		
			SetInversePivotMatrix(Matrix);								

			lpAni->Load((*lpStream));						
			//lpAni->SetRange(0.0f, MaxTime-1);				
			//lpAni->SetStep(1.0f);				
			//lpAni->SetPlayMode(PM_LOOP);						

			lpStream->vClose();

			return lpAni;						
		}
	ErrorExit:
		lpStream->vClose();
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	Animation::CAnimation* CContainer::LoadAnimation(const LYNXCHAR* filename, CStream* s) 
	{ 		
		CCreationDesc Desc;		
		LYNXCHAR Name[128];

		Desc.m_FileName = filename;
		lynxGetFilenameOnlyByPath(Name, filename);
		Desc.m_Name = Name;
		if (s)
			Desc.m_lpStream = s;
		else
			Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
		Animation::CAnimation* lpAni = vLoadAnimation(&Desc);
		if (lpAni)
			lpAni->SetName(Name);
		SetCurrentAnimation(lpAni);
		
		return lpAni;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	Animation::CAnimation* CContainer::LoadAnimation(const LYNXCHAR* name, const LYNXCHAR* filename) 
	{ 		
		CCreationDesc Desc;		
		LYNXCHAR Name[128];

		Desc.m_FileName = filename;		
		Desc.m_Name = name;
		Desc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
		Animation::CAnimation* lpAni = vLoadAnimation(&Desc);
		if (lpAni)
			lpAni->SetName(name);
		SetCurrentAnimation(lpAni);		

		return lpAni;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::RenameAnimation(Animation::CAnimation* lpani, LynxEngine::CString& name)
	{
		CString OldName = lpani->GetName();
		Animation::CAnimation* lpAni = m_AnimationSet->Find(OldName);
		if (lpAni)
			lpAni->SetName(name);

		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			lpAni = (*Sub)->m_AnimationSet->Find(OldName);
			if (lpAni)
				lpAni->SetName(name);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::RenameAnimation(Animation::CAnimation* lpani, const LYNXCHAR* name)
	{
		CString Name(name);
		RenameAnimation(lpani, Name);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetCurrentAnimation(Animation::CAnimation* lpa) 
	{
		m_AnimationSet->SetCurrentAnimation(lpa->GetName()); 
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			(*Sub)->SetCurrentAnimation(lpa);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetCurrentAnimationRange(const float s, const float e, const float step, const PLAYMODE mode)
	{
		if (m_AnimationSet->GetCurrentAnimation())
		{
			m_AnimationSet->GetCurrentAnimation()->SetRange(s, e);
			m_AnimationSet->GetCurrentAnimation()->SetStep(step);
			m_AnimationSet->GetCurrentAnimation()->SetPlayMode(mode);
		}
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			if ((*Sub)->m_AnimationSet->GetCurrentAnimation())
			{
				(*Sub)->m_AnimationSet->GetCurrentAnimation()->SetRange(s, e);
				(*Sub)->m_AnimationSet->GetCurrentAnimation()->SetStep(step);
				(*Sub)->m_AnimationSet->GetCurrentAnimation()->SetPlayMode(mode);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetCurrentAnimationUseIntTime(LYNXBOOL b)
	{
		if (m_AnimationSet->GetCurrentAnimation())
			m_AnimationSet->GetCurrentAnimation()->m_bIntTime = b; 
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			if ((*Sub)->m_AnimationSet->GetCurrentAnimation())
				(*Sub)->m_AnimationSet->GetCurrentAnimation()->m_bIntTime = b; 
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	Animation::CAnimation* CContainer::SetCurrentAnimation(const CString name) 
	{
		Animation::CAnimation* A = m_AnimationSet->GetCurrentAnimation(); 
		m_AnimationSet->SetCurrentAnimation(name); 
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			(*Sub)->SetCurrentAnimation(name);
		}

		return A; 
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetAnimationName(Animation::CAnimation* lpa, CString& name) 
	{
		CString OldName = lpa->GetName();
		lpa->SetName(name); 
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			Animation::CAnimation* A = (*Sub)->m_AnimationSet->Find(OldName);
			if (A)
				(*Sub)->SetAnimationName(A, name); 
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
	void CContainer::AddAnimationKey(int set, Animation::CAnimation::KEYSTYPE type, int num, Animation::ANIMATIONKEYDATA* key)
	{	
		Animation::CAnimation* lpAni = m_AnimationSet->GetAnimation(set);

		if (lpAni)
		{
			lpAni->CreateKeys(type, num, key[0].Values.size());			

			for (int i=0; i<num; i++)
			{
				lpAni->m_AnimationKeys[type]->m_Keys[i].SetStartTime(key[i].StartTime);
				lpAni->m_AnimationKeys[type]->m_Keys[i].SetEndTime(key[i].EndTime);
				for (int v=0 ;v<key[0].Values.size(); v++)
				{
					lpAni->m_AnimationKeys[type]->m_Keys[i].SetValue(v, key[i].Values[v]);
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
	void CContainer::AddAnimationKey(const CString& name, Animation::CAnimation::KEYSTYPE type, int num, Animation::ANIMATIONKEYDATA* key)
	{		
		m_AnimationSet->GetAnimation(name)->CreateKeys(type, num, key[0].Values.size());			

		for (int i=0; i<num; i++)
		{
			m_AnimationSet->GetAnimation(name)->m_AnimationKeys[type]->m_Keys[i].SetStartTime(key[i].StartTime);
			m_AnimationSet->GetAnimation(name)->m_AnimationKeys[type]->m_Keys[i].SetEndTime(key[i].EndTime);
			for (int v=0 ;v<key[0].Values.size(); v++)
			{
				m_AnimationSet->GetAnimation(name)->m_AnimationKeys[type]->m_Keys[i].SetValue(v, key[i].Values[v]);
			}
		}		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CContainer::vPlay(CReal step)
	{
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::AddSubContainer(CContainer* c)
	{
		c->SetlpParentContainer(this);
		m_SubContainerList.push_back(c);
		// if there is no render parent object for this container
		if (!c->GetlpRenObj())
			c->SetlpRenObj(GetlpRenObj());	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::RemoveSubContainer(CContainer* c)
	{
		CList<CContainer*>::CIterator Con;
		Con = LynxEngine::find(m_SubContainerList.begin(), m_SubContainerList.end(), c);
		if (Con != m_SubContainerList.end())
		{
			m_SubContainerList.erase(Con);			
		}
		return;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer*	CContainer::FindContainer(const CString& name)
	{
		CList<CContainer*>::CIterator Con;
		Con = LynxEngine::find(m_SubContainerList.begin(), m_SubContainerList.end(), name);
		if (Con != m_SubContainerList.end())
			return (*Con);

		CString Name = m_Name + CString(".") + name;
		Con = LynxEngine::find(m_SubContainerList.begin(), m_SubContainerList.end(), Name);
		if (Con != m_SubContainerList.end())
			return (*Con);

		Name = name + CString(_T(".Container"));
		Con = LynxEngine::find(m_SubContainerList.begin(), m_SubContainerList.end(), Name);
		if (Con != m_SubContainerList.end())
			return (*Con);		

		return NULL;
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vCalculateLOD(CCameraContainer* cam)
	{
		if (cam)
		{
			Math::CVector3 Pos, CPos, Dir;

			vGetPosition(Pos);
			cam->vGetPosition(CPos);
			Dir = CPos - Pos;
			Math::CReal Dist = Dir.Length(3);
			int LOD = m_LODBias;
			LOD += LYNX_ROUND(((float)(m_NumLODs-1)*(Dist-m_MinLODDistance))/(m_MaxLODDistance - m_MinLODDistance));
			LOD = LYNX_CLAMP(LOD, 0, m_NumLODs-1);
			SetCurrentLOD(LOD);
			for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
				(*Sub)->vCalculateLOD(cam); 		
		}
		else
			SetCurrentLOD(0);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::vAttachToTerrain(CTerrainContainer& t, LYNXBOOL balign)
	{
		LynxEngine::Math::CVector3 Euler, Normal, Pos, ScaleV;
		LynxEngine::Math::CVector3 OTangent, TTangent;
		float Deg;

		m_M.GetRight(OTangent);
		m_M.GetRotation(Euler);
		m_M.GetScale(ScaleV);
		vGetPosition(Pos);
		Pos.y = t.GetHeight(Pos); 	
		t.GetNormal(Normal, Pos);	

		if (balign)
		{
			LynxEngine::Math::CMatrix3 M, RM3, OM;
			LynxEngine::Math::CMatrix4 RM;
			t.GetOrientation(M, Pos);									
			M.GetRight(TTangent);
			Deg = acosf(LynxEngine::Math::Dot(TTangent, OTangent))*LYNXR2D;
			lynxMakeRotateMatrixRespectVector(RM, Normal, Deg);
			RM3.m[0][0] = RM.m[0][0];	RM3.m[0][1] = RM.m[0][1];	RM3.m[0][2] = RM.m[0][2];
			RM3.m[1][0] = RM.m[1][0];	RM3.m[1][1] = RM.m[1][1];	RM3.m[1][2] = RM.m[1][2];
			RM3.m[2][0] = RM.m[2][0];	RM3.m[2][1] = RM.m[2][1];	RM3.m[2][2] = RM.m[2][2];									
			OM = M*RM3;			
			OM.GetRotation(Euler);			
		}
		m_LocalM.Identity();
		m_GlobalM.Identity();		
		Rotate(Euler, LYNX_MATH_POST);							
		Scale(ScaleV, LYNX_MATH_POST);				
		UpdateTransform();
		m_LocalM.m41 = Pos.x;
		m_LocalM.m42 = Pos.y;
		m_LocalM.m43 = Pos.z;
		UpdateTransform();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  ????:   
	//-------------------------------------------------------------------------------------------------------
	void CContainer::ComputeProxyScreenSpaceAABB(CCameraContainer* cam)
	{
		CVector4 Transformed;
		Math::CVector3 Min(FLT_MAX, FLT_MAX, FLT_MAX);
		Math::CVector3 Max(-FLT_MAX,-FLT_MAX,-FLT_MAX);			

		LYNXMATRIX WVM, WVPM;
		lynxMatrixXMatrix(&WVM, &GetTransformMatrix(), &cam->GetViewMatrix());	
		lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());													

		CList<CObjProxy*>::CIterator Pos;			
		for (Pos = m_ProxyList.begin(); Pos != m_ProxyList.end(); ++Pos)
		{
			for (int i=0; i<(*Pos)->GetNumVertices(); i++)
			{
				// transform to projection space
				Math::VectorXMatrix(Transformed, CVector4((*Pos)->GetVertex(i), 1.0f), WVPM); 				
				Transformed /= Transformed.w;

				if(Transformed.x < Min.x) 
					Min.x = Transformed.x;
				if(Transformed.y < Min.y) 
					Min.y = Transformed.y;
				if(Transformed.z < Min.z) 
					Min.z = Transformed.z;

				if(Transformed.x > Max.x) 
					Max.x = Transformed.x;
				if(Transformed.y > Max.y) 
					Max.y = Transformed.y;
				if(Transformed.z > Max.z) 
					Max.z = Transformed.z;						
			}
		}			
		m_ScreenSpaceAABB.SetByBoundary(Min, Max);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::OcclusionRender(CCameraContainer* cam)
	{
		if (m_ProxyList.size())
		{
			if (!m_lpOcclusionQuery)
			{
				m_lpOcclusionQuery = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->CreateQuery();
				m_lpOcclusionQuery->vCreate(GraphicsSystem::CRenderer::QT_OCCLUSION);			
				GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
				DDesc.Default();
				DDesc.StencilEnable = LYNX_FALSE;
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
				DDesc.StencilEnable = LYNX_FALSE;
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->CreateDepthStencilState(m_OcclusionDepthStencilState, DDesc);			
			}

			m_DrawnPixels = 10;
			m_lpOcclusionQuery->vGetData(&m_DrawnPixels, sizeof(DWORD), 50);
			if (m_DrawnPixels)
				m_bOccluded = LYNX_FALSE;
			else
				m_bOccluded = LYNX_TRUE;

			// If this container was occluded on last frame then just render the proxy for occlusion test, otherwise render the container for occlusion test
			// 如果這個container在上個frame是被遮蔽的, 則只render proxy來作遮蔽測試. 否則render container本身來作遮蔽測試.
			if (m_bOccluded)
			{
				//m_lpEngine->GetlpGraphicsSystem()->SetColorWriteMask(0);				
				m_lpOcclusionQuery->vStart();					
					m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::PROXY_RENDERING)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(this, cam);
					GraphicsSystem::CDepthStencilState* LastDS = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedDepthStencilState();
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetDepthStencilState(m_OcclusionDepthStencilState);					
					CList<CObjProxy*>::CIterator Proxy;
					for (Proxy=m_ProxyList.begin(); Proxy!=m_ProxyList.end(); ++Proxy)
						(*Proxy)->vRender(cam);
				m_lpOcclusionQuery->vEnd();		
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetDepthStencilState(LastDS);
			}
			else
			{
				m_lpOcclusionQuery->vStart();
					vRender(cam);
				m_lpOcclusionQuery->vEnd();		

				INC_COUNTER(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_Profile.NumRenderedContainer, 1)
			}
		}
		else
		{
			vRender(cam);

			INC_COUNTER(m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->m_Profile.NumRenderedContainer, 1)
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C
	*	@return ?C
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CContainer::vLoad(CUserProfile* file, CXMLElement* node)
	{
		CXMLElement* Root = node;
		if (!Root)
			Root = file->GetDoc()->RootElement();

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C
	*	@return ?C
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CContainer::vSave(CUserProfile* file, CXMLElement* node)
	{
		CXMLElement* Root = node;
		if (!Root)
			Root = file->GetDoc()->RootElement();

		file->Write(_T("Type"), GetEnumStr(m_ContainerType), Root);
		file->Write(_T("Name"), m_Name, Root);
		
		return LYNX_TRUE;

	}
}