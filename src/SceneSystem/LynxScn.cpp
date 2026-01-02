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
#include <LynxEngineClasses.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <LynxFileStream.h>
#include <LynxMemoryStream.h>

#include <GameFramework/LynxGameFrameworkKernel.h>
#include <LynxScreenOutputDevice.h>

#include <StateMachine/LynxStateMachine.h>
#include <StateMachine/LynxState.h>
#include <StateMachine/LynxStateTranslation.h>
#include <StateMachine/LynxRandTranslation.h>

#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxScnDeferredRenderer.h>
#include <SceneSystem/LynxScnLightPrePassRenderer.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxAtmosphere.h>
#include <SceneSystem/Camera/LynxCamera.h>
#include <SceneSystem/StaticObj/LynxStaticMesh.h>
#include <SceneSystem/Light/LynxAmbientLight.h>
#include <SceneSystem/Light/LynxPointLight.h>
#include <SceneSystem/Light/LynxSpotLight.h>
#include <SceneSystem/Light/LynxDirectionalLight.h>
#include <SceneSystem/DynObj/LynxEntity.h>
#include <SceneSystem/LynxAtmosphere.h>
#include <SceneSystem/StaticObj/LynxSky.h>
#include <SceneSystem/StaticObj/LynxTerrain.h>
#include <SceneSystem/StaticObj/LynxGrass.h>

#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxOffscreen.h>
#include <GraphicsSystem/LynxDepthMap.h>
#include <GraphicsSystem/LynxGlowMap.h>
#include <GraphicsSystem/LynxCubeShadowMap.h>
#include <GraphicsSystem/LynxVelocityMap.h>
#include <GraphicsSystem/LynxGBufferMap.h>
#include <GraphicsSystem/LynxVertexLayout.h>

#include <GameConsoleSystem/LynxGameConsoleSystem.h>

#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>

#include <Container/LynxReferenceContainer.h>
#include <Container/LynxModelContainer.h>
#include <Container/LynxSprite3dContainer.h>
#include <Container/LynxSprite3dClusterContainer.h>
#include <Container/LynxSpriteGrassContainer.h>
#include <Container/LynxPEmitterContainer.h>
#include <Container/LynxTerrainContainer.h>
#include <Container/LynxSkyDomeContainer.h>
//#include <LynxSpeedTreeContainer.h>

#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>

#include <PhysicsSystem/LynxPhysicsSystem.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{	
		IMPLEMENT_CLASSSCRIPT(CScene, CGameObj)					
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScene::CScene(void)
		{
			m_lpSceneSystem = NULL;

			m_bPause = LYNX_FALSE;

			m_lpCurrentAmbientLight = NULL;
			m_lpCurrentCamera = NULL;

			m_Version = 100;				

			m_LightArray.clear();
			m_SurfaceArray.clear();
			m_StaticObjArray.clear();
			m_DynamicObjArray.clear();		
			m_VisLightList.clear();
			m_VisObjList.clear();

			m_RendererType = RT_DEFERRED;
			m_lpRenderer = NULL;

			m_ClassScriptClassName = ClassScriptClassName();
			
			m_lpAtmosphere = LYNXNEW CAtmosphere(this);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScene::CScene(CSceneSystem* ps)
		: CGameObj(ps->GetlpEngine())
		{
			m_lpSceneSystem = ps;

			m_bPause = LYNX_FALSE;

			m_lpCurrentAmbientLight = NULL;
			m_lpCurrentCamera = NULL;

			m_Version = 100;
			
			m_CameraArray.clear();
			m_LightArray.clear();
			m_SurfaceArray.clear();
			m_StaticObjArray.clear();
			m_DynamicObjArray.clear();		
			m_VisLightList.clear();
			m_VisObjList.clear();

			m_Root.SetNumChildren(0);

			m_RendererType = RT_DEFERRED;
			m_lpRenderer = NULL;

			m_PhysicsWorld = NULL;

			INIT_CLASSSCRIPT

			m_lpAtmosphere = LYNXNEW CAtmosphere(this);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScene::~CScene(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy ("))+GetName()+CString(_T(") ----------------------------------------------------------------------------------\n")));
			Release();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::GetTexturePathFromModelPath(LYNXCHAR *lpr, const LYNXCHAR *lpc)
		{
			int				i, s;	
			
			for (i=0, s=0; i<lynxStrLen(lpc); i++)
			{
				if (lpc[i] == _T('m'))
				{
					if (lpc[i+1] == _T('o') && lpc[i+2] == _T('d') && lpc[i+3] == _T('e') && lpc[i+4] == _T('l'))
					{
						lpr[s++] = _T('t');
						lpr[s++] = _T('e');
						lpr[s++] = _T('x');
						lpr[s++] = _T('t');
						lpr[s++] = _T('u');
						lpr[s++] = _T('r');
						lpr[s++] = _T('e');
						i += 4;
					}
					else
					{
						lpr[s++] = lpc[i];
					}
				}
				else
				{
					lpr[s++] = lpc[i];
				}
			}	
			lpr[s] = NULL;

			for (int i=(int)lynxStrLen(lpr); i>=0; i--)
			{
				if (lpr[i] == _T('/') || lpr[i] == _T('\\'))
				{
					lpr[i+1] = NULL;
					return;
				}
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScene::Create(const CCreationDesc* desc)		
		{
			//if (m_lpEngine->GetlpGraphicsSystem()->GetConfig().bDeferredShading)
			//	m_RendererType = RT_DEFERRED;
				
			CSceneCreationDesc* Desc = (CSceneCreationDesc*)desc;
			if (Desc)
			{
				m_RendererType = Desc->m_RendererType;
			}

			switch (m_RendererType)
			{
				case RT_FORWARD:
					m_lpRenderer = LYNXNEW CRenderer(this);						
					break;
				default:
				case RT_DEFERRED:
					m_lpRenderer = LYNXNEW CDeferredRenderer(this);						
					break;
				case RT_PRELIGHTING:
					m_lpRenderer = LYNXNEW CLightPrePassRenderer(this);						
					break;
				case RT_SIMPLE:
					m_lpRenderer = LYNXNEW CRenderer(this);						
					break;
			}

			if (m_lpRenderer)
			{
				m_lpRenderer->vCreate();
			}

			if (Desc->m_bUsePhysics)
			{
				m_PhysicsWorld = m_lpEngine->GetlpPhysicsSystem()->vCreateWorld();
			}
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCamera* CScene::vCreateCamera()
		{
			return LYNXNEW CCamera(this);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::AddCamera(CCamera* lpc)
		{
			int Size = (int)m_CameraArray.size();
			Size++;
			m_CameraArray.resize(Size);
			m_CameraArray[Size-1] = lpc;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCamera* CScene::CreateCamera()
		{
			CCamera* lpCamera = LYNXNEW CCamera(this);			
			AddCamera(lpCamera);

			return lpCamera;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CLight*	CScene::vCreateLight(CLight::TYPE type)
		{
			switch (type)
			{
				case CLight::AMBIENT:
					return LYNXNEW CAmbientLight(this);
					break;
				case CLight::DIRECTIONAL:
					return LYNXNEW CDirectionalLight(this);
					break;
				case CLight::POINT:
					return LYNXNEW CPointLight(this);
					break;
				case CLight::SPOT:
					return LYNXNEW CSpotLight(this);
					break;
				default:
					return LYNXNEW CLight(this);
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObj*	CScene::vCreateStaticObj(CStaticObj::TYPE type)
		{
			switch (type)
			{
				case CStaticObj::STATIC_MESH:
					return LYNXNEW CStaticMesh(this);
					break;
				case CStaticObj::STATIC_MODEL:
					return LYNXNEW CStaticModel(this);
					break;
				case CStaticObj::TERRAIN:
					return LYNXNEW CTerrain(this);
					break;
				case CStaticObj::SKY:
					return LYNXNEW CSky(this);
					break;
				case CStaticObj::GRASS:
					return LYNXNEW CGrass(this);
					break;				
				default:
					return LYNXNEW CStaticObj(this);
					break;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDynamicObj* CScene::vCreateDynamicObj(CDynamicObj::TYPE type)
		{
			switch (type)
			{
				case CDynamicObj::ENTITY:
					return LYNXNEW CEntity(this);					
				default:
					return LYNXNEW CDynamicObj(this);
					break;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScene::Release(void)		
		{
			LYNXDEL m_lpRenderer;

			m_MaterialArray.clear();

			/*
			for (int i=0; i<m_CameraArray.size(); ++i)
			{
				LYNXDEL m_CameraArray[i];
			}
			m_CameraArray.clear();

			for (int i=0; i<m_StaticObjArray.size(); i++)
			{				
				LYNXDEL m_StaticObjArray[i];
			}
			m_StaticObjArray.clear();	

			for (int i=0; i<m_DynamicObjArray.size(); i++)
			{				
				LYNXDEL m_DynamicObjArray[i];
			}
			m_DynamicObjArray.clear();			

			for (int i=0; i<m_LightArray.size(); i++)
			{				
				LYNXDEL m_LightArray[i];
			}
			m_LightArray.clear();
			*/

			LYNXDEL m_lpAtmosphere;		
			m_lpEngine->ClearInvalidResource();

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScene::Load(CStream& stream, const LYNXCHAR *filename)		
		{
			LYNX_ASSERT(m_lpSceneSystem != NULL);

			LYNXBOOL bRet;
			LYNXCHAR Name[256], Ext[32];

			lynxSeparateFileExtByPath(Name, Ext, filename);	
			
			if (!lynxStriCmp(Ext, _T("scn")))
			{
				bRet = LoadSCN(stream, filename);
			}
			else if (!lynxStriCmp(Ext, _T("isc")))
			{
				bRet = LoadISC(stream, filename);
			}
			else if (!lynxStriCmp(Ext, _T("lynx")))
			{
				bRet = LYNX_FALSE;//LoadLYNX(filename, lpfs, offset);
			}			

			//((LynxScript::Lua::CVMachine*)(this->GetlpEngine()->GetlpScriptSystem()->GetlpVMachine()))->GetState()->DumpGlobals("e:/LuaPlus_Dump.txt");
			return bRet;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScene::LoadLYNX(const LYNXCHAR *filename, LPLYNXFILE lpfs, int offset)
		{	
			return LYNX_TRUE;

			//TiXmlElement	*lpEditorRoot;				
			//TiXmlElement	*lpSceneRoot;	
			//TiXmlElement	*lpElem;
			//const char		*lpValue;
			//int				Num;
			//CLight			*lpLO;			
			//CStaticObj		*lpSO, *lpOSO;
			//CDynamicObj		*lpDO;			
			//BOOL			bGlow;
			//float			BloomingIntensity;
			//TiXmlDocument*  lpXMLDoc;
			//char			Path[256], FileName[256], FullPath[256], TexturePath[256];				

			//LYNX_GUARD(CScene::LoadLYNXFile)
			//
			//lpXMLDoc = LYNXNEW TiXmlDocument;
			//if (!lpXMLDoc)
			//{
			//	LYNX_THROW(CException("Unable to create XML Doc!"));
			//}
			//if (!lpXMLDoc->LoadFile(filename)) 
			//{
			//	LYNX_THROW(CException("Fail to load file!"));				
			//}

			//lpElem = lpXMLDoc->FirstChildElement();
			//if (!lpElem) 
			//{
			//	LYNX_THROW(CException("Fail to find element!"));				
			//}
			//lpValue = lpElem->Attribute("Version");
			//lpEditorRoot = lpElem;
			//
			//lpElem = lpEditorRoot->FirstChildElement(m_XMLTag.SceneTag);				
			//if (!lpElem) 
			//{
			//	LYNX_THROW(CException("Fail to find element!"));
			//}
			//lpValue = lpElem->Attribute(m_XMLTag.LocationAttr);	
			//lpSceneRoot = lpElem;

			///*
			//lpElem = lpSceneRoot->FirstChildElement(m_XMLTag.LightsTag);				
			//if (lpElem) 
			//{					
			//	lpValue = lpElem->Attribute(m_XMLTag.NumberAttr);	
			//	Num = atoi(lpValue);
			//	lpElem = lpElem->FirstChildElement(m_XMLTag.LightTag);
			//	for (lpElem; lpElem; lpElem = lpElem->NextSiblingElement())
			//	{
			//		std::string Name, TypeName;
			//		LynxEngine::SceneSystem::CLight::TYPE LightType;

			//		Name = lpElem->Attribute(m_XMLTag.NameAttr);		
			//		TypeName = lpElem->Attribute(m_XMLTag.TypeAttr);		
			//		
			//		if (TypeName == std::string("AMBIENT"))
			//		{
			//			LightType = LynxEngine::SceneSystem::CLight::AMBIENT;
			//		}
			//		else if (TypeName == std::string("DIRECTIONAL"))
			//		{
			//			LightType = LynxEngine::SceneSystem::CLight::DIRECTIONAL;
			//		}
			//		else if (TypeName == std::string("POINT"))
			//		{
			//			LightType = LynxEngine::SceneSystem::CLight::POINT;
			//		}
			//		else if (TypeName == std::string("SPOT"))
			//		{
			//			LightType = LynxEngine::SceneSystem::CLight::SPOT;
			//		}
			//		lpLO = CreateLight(LightType, Name.c_str());						
			//		m_lpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentLight(dynamic_cast<LynxEngine::SceneSystem::CLight*>(lpLO));
			//		lpLO->SetSceneID(ReadSceneID(lpElem));
			//		if (lpLO)
			//		{							
			//			lpLO->vRead((*lpXMLDoc), (*lpElem));
			//		}						
			//	}
			//}
			//*/

			//lpElem = lpSceneRoot->FirstChildElement(m_XMLTag.StaticObjsTag);				
			//if (lpElem) 
			//{					
			//	lpValue = lpElem->Attribute(m_XMLTag.NumberAttr);	
			//	Num = atoi(lpValue);
			//	m_StaticObjArray.resize(Num);			
			//	lpElem = lpElem->FirstChildElement(m_XMLTag.StaticObjTag);
			//	for (lpElem, int i=0; lpElem; lpElem = lpElem->NextSiblingElement(), i++)
			//	{
			//		std::string Name, TypeName, ContainerName, TargetName, Location;

			//		Name = lpElem->Attribute(m_XMLTag.NameAttr);		
			//		//TypeName = lpElem->Attribute(m_XMLTag.TypeAttr);		
			//		ContainerName = lpElem->Attribute(m_XMLTag.ContainerAttr);
			//		Location = lpElem->Attribute(m_XMLTag.LocationAttr);
			//		TargetName = lpElem->Attribute(m_XMLTag.TargetNameAttr);								

			//		LynxEngine::SceneSystem::CDynamicObj::TYPE DOType;
			//		LynxEngine::CContainer::TYPE ContainerType;					

			//		if (ContainerName == CLASSNAME(CStaticMeshContainer))
			//		{
			//			ContainerType = LynxEngine::CContainer::STATIC_MESH;
			//		}
			//		else if (ContainerName == CLASSNAME(CGeometryContainer))
			//		{
			//			ContainerType = LynxEngine::CContainer::GEOMETRY;
			//		}
			//		else if (ContainerName == CLASSNAME(CTerrainContainer))
			//		{
			//			ContainerType = LynxEngine::CContainer::TERRAIN;
			//		}

			//		strcpy(FullPath, Location.c_str());
			//		//GetlpKernel()->GetPathAndName(Path, FileName, FullPath);	
			//		//GetlpKernel()->GetTexturePathFromModelPath(TexturePath, Path);										
			//		//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(TexturePath);
			//		
			//		switch (ContainerType)
			//		{
			//			default:											
			//				lpSO = CreateStaticObj(LynxEngine::SceneSystem::CStaticObj::TYPE::TERRAIN, LynxEngine::CContainer::TYPE::TERRAIN, NULL, Name.c_str());											
			//				dynamic_cast<LynxEditor::Kernel::CContainer *>(lpSO->GetlpContainer())->vSetupSubObjs(lpSO);										
			//				lpSO->GetlpContainer()->SetlpRenObj(lpSO);									
			//				break;

			//			case LynxEngine::CContainer::GEOMETRY:														
			//				m_StaticObjArray[i] = vCreateStaticObj(CStaticObj::GEOMETRY);
			//				m_StaticObjArray[i]->
			//				GetlpKernel()->GetPluginManager().LoadPlugins(GetlpKernel(), LynxEditor::Kernel::CPlugin::GEOMETRY_IMPORTER);
			//				lpRef = CreateReference(LynxEditor::Kernel::CReference::FILE, Location.c_str());
			//					GetlpKernel()->GetPluginManager().GeometryImport(this, lpRef, Location.c_str(), TargetName.c_str(), Name.c_str());																		
			//				GetlpKernel()->GetPluginManager().ReleasePlugins(LynxEditor::Kernel::CPlugin::GEOMETRY_IMPORTER);
			//				break;

			//			case LynxEngine::CContainer::STATIC_MESH:													
			//				if (Location.size())
			//				{
			//					GetlpKernel()->GetPluginManager().LoadPlugins(GetlpKernel(), LynxEditor::Kernel::CPlugin::STATIC_MESH_IMPORTER);
			//					lpRef = CreateReference(LynxEditor::Kernel::CReference::FILE, Location.c_str());																				
			//						GetlpKernel()->GetPluginManager().StaticMeshImport(this, lpRef, Location.c_str(), TargetName.c_str(), Name.c_str());																		
			//					GetlpKernel()->GetPluginManager().ReleasePlugins(LynxEditor::Kernel::CPlugin::STATIC_MESH_IMPORTER);
			//				}
			//				else
			//				{
			//					lpSO = CreateStaticObj(LynxEngine::SceneSystem::CStaticObj::TYPE::STATIC_MESH, LynxEngine::CContainer::TYPE::STATIC_MESH);
			//					lpOSO = dynamic_cast<CStaticObj*>(Find(TargetName.c_str()));
			//					if (lpOSO)
			//					{
			//						if (!lpSO->vInstance(lpOSO))
			//							return LYNX_FALSE;										
			//						lpSO->SetName(Name.c_str());
			//					}
			//				}
			//				break;
			//		}
			//		lpSO = GetStaticObj(Name.c_str());	
			//		if (lpSO)
			//		{							
			//			lpSO->SetSceneID(ReadSceneID(lpElem));
			//			lpSO->vRead((*lpXMLDoc), (*lpElem));
			//		}		
			//		else
			//		{
			//			GetlpKernel()->GetlpOutputWindow()->vOutput(std::string("Warning : Can't find static object <") + Name + std::string(">.\r\n"));
			//		}
			//	}
			//}
   //         
			//lpElem = lpSceneRoot->FirstChildElement(m_XMLTag.DynObjsTag);				
			//if (lpElem) 
			//{
			//	lpValue = lpElem->Attribute(m_XMLTag.NumberAttr);			
			//	Num = atoi(lpValue);

			//	lpElem = lpElem->FirstChildElement(m_XMLTag.DynObjTag);
			//	for (lpElem; lpElem; lpElem = lpElem->NextSiblingElement())
			//	{
			//		std::string Name, TypeName, ContainerName, Location;

			//		Name = lpElem->Attribute(m_XMLTag.NameAttr);		
			//		TypeName = lpElem->Attribute(m_XMLTag.TypeAttr);		
			//		ContainerName = lpElem->Attribute(m_XMLTag.ContainerAttr);
			//		Location = lpElem->Attribute(m_XMLTag.LocationAttr);						

			//		LynxEngine::SceneSystem::CDynamicObj::TYPE DOType;
			//		LynxEngine::CContainer::TYPE ContainerType;

			//		DOType = LynxEngine::SceneSystem::CDynamicObj::ENTITY;						

			//		if (ContainerName == CLASSNAME(CPEmitterContainer))
			//		{
			//			ContainerType = LynxEngine::CContainer::PEMITTER;
			//		}
			//		else if (ContainerName == CLASSNAME(CModelContainer))
			//		{
			//			ContainerType = LynxEngine::CContainer::MODEL;
			//		}						

			//		LynxEngine::CModelCreationDesc ModelDesc;
			//		switch (ContainerType)
			//		{
			//			case LynxEngine::CContainer::PEMITTER:
			//				//lpDO = CreateDynObj(DOType, ContainerType, Location.c_str(), Name.c_str());
			//				break;

			//			case LynxEngine::CContainer::MODEL:											
			//				GetlpKernel()->GetPluginManager().LoadPlugins(GetlpKernel(), LynxEditor::Kernel::CPlugin::MODEL_IMPORTER);
			//				ModelDesc.m_FileName = Location.c_str();
			//				lpDO = CreateDynObj(DOType, ContainerType, &ModelDesc, Name.c_str());						
			//				GetlpKernel()->GetPluginManager().ReleasePlugins(LynxEditor::Kernel::CPlugin::MODEL_IMPORTER);
			//				break;
			//		}						
			//		lpDO->SetSceneID(ReadSceneID(lpElem));
			//		lpDO->vRead((*lpXMLDoc), (*lpElem));
			//	}				
			//}

			//lpElem = lpSceneRoot->FirstChildElement(m_XMLTag.GroupsTag);				
			//if (lpElem) 
			//{					
			//	lpValue = lpElem->Attribute(m_XMLTag.NumberAttr);	
			//	Num = atoi(lpValue);
			//	lpElem = lpElem->FirstChildElement(m_XMLTag.GroupTag);
			//	for (lpElem; lpElem; lpElem = lpElem->NextSiblingElement())
			//	{
			//		std::string Name;
			//		CGroup*		lpGorup;

			//		Name = lpElem->Attribute(m_XMLTag.NameAttr);		
			//		lpGorup = CreateGroup(Name.c_str());												
			//		lpGorup->vRead((*lpXMLDoc), (*lpElem));
			//	}
			//}
			//LYNXDEL lpXMLDoc;			

			////SetAllSelectableParent();
			////SetupDynObjHierarchyData();
			//
			//return LYNX_TRUE;

			//LYNX_UNGUARD
			//catch (LynxEngine::CException& e) 
			//{ 
			//	LYNXDEL lpXMLDoc; 
			//	lynxDebugOutputPrintf(e.GetMessage().c_str()); 
			//	return LYNX_FALSE; 
			//}
			//LYNX_DEFAULTCATCH
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScene::LoadSCN(CStream& stream, const LYNXCHAR *filename)		
		{
			LYNXUNICHAR			UniStr[256];	
			char				Header[3];
			LYNXCHAR			ResourceName[64], MaterialName[64], MapName[64], Name[256], Path[128], Filename[64], TexPath[128];
			DWORD				ID;
			LYNXCHAR			TTexPath[256];	
			CString				TName;
			DWORD				NumRes, NumMaterials, NumPass, NumLights, NumStaticObjs, NumDynObjs, Num;
			CLight::TYPE		LightType;
			CStaticObj::TYPE	Type;	
			CContainer::TYPE	ContainerType;			
			BOOL				bGlow;
			float				BloomingIntensity;
			CStaticObj*			lpObj;
			CGeometryContainer*	lpGeo;
			CTerrainContainer*	lpTerrain;
			//CSpeedTreeContainer* lpSpeedTree;
			CSkyDomeContainer*	lpSkyDome;
			CDynamicObj*		lpDObj;
			CModelContainer*	lpModel;
			CPointLight*		lpPointLight;
			CSpotLight*			lpSpotLight;
			CReferenceContainer* lpRef;
			LYNXMATRIX			TM; 
			int					LOD = 0;
			MaterialSystem::CTechnique* Technique;
			CCreationDesc*		pCDesc;
			CModelCreationDesc	ModelDesc;								
			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Open scene ("))+CString(filename)+CString(_T(") ----------------------------------------------------------------------------------\n")));
			
			if (!stream.IsValid())
			{
				if (!stream.vOpen(filename, CStream::Read|CStream::Binary))	
					return FALSE;				
			}
			
			stream.vRead(&Header, 3, 1);
			if (Header[0] != 'S' || Header[1] != 'C' || Header[2] != 'N')
			{
				stream.vClose();	
				return FALSE;		
			}
			stream.vRead(&m_Version, sizeof(DWORD), 1);				
			
			// Create Physics world and space
			if (m_lpEngine->GetlpPhysicsSystem())
			{
				PhysicsSystem::CPhysicsWorld* lpWorld = m_lpEngine->GetlpPhysicsSystem()->vCreateWorld();				
				//lpWorld->vCreateSpace(NULL);
			}
			
			// Preload sources
			GraphicsSystem::CTexturePtr Texture;
			stream.vRead(&NumRes, sizeof(DWORD), 1);
			for (int i=0; i<NumRes; ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(ResourceName, UniStr);				
				//((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(Texture, 0, ResourceName, NULL, 0);				
			}
			
			stream.vRead(&m_lpRenderer->m_RenderingOptions.ClearColor, sizeof(LYNXCOLORRGBA), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.bUseBackgroundTexture, sizeof(LYNXBOOL), 1);
			stream.vRead(TexPath, sizeof(LYNXUNICHAR), 128);
			if (m_lpRenderer->m_RenderingOptions.bUseBackgroundTexture)
			{
				GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;
				TexDesc.Default();
				TexDesc.bSRGB = LYNX_FALSE;
				TexDesc.MipMapLevel = 1;				
				TexDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
				TexDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
				TexDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
				TexDesc.SamplerDesc.Filter	 = GraphicsSystem::CRenderer::FM_MIN_MAG_MIP_POINT;
				TexDesc.SamplerDesc.MinLOD = 0;
				TexDesc.SamplerDesc.MaxLOD = 0;
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpRenderer->m_RenderingOptions.BackgroundTexture, *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), TexPath, &TexDesc);
			}
			stream.vRead(&m_lpRenderer->m_RenderingOptions.bSSAOPass, sizeof(LYNXBOOL), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOSamplingKernel, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.bSSAOBlur, sizeof(LYNXBOOL), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOBlurKernel, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOBlurScale, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOBlurSharpness, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOThreshold, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.bGlowPass, sizeof(LYNXBOOL), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.NumSplittedShadowMap, sizeof(int), 1);
			
			// Create scene renderer according to the render option
			m_lpRenderer->vCreate();
			
			m_lpRenderer->SetNumSplittedShadowMap(m_lpRenderer->m_RenderingOptions.NumSplittedShadowMap);
			
			stream.vRead(&NumMaterials, sizeof(DWORD), 1);
			m_MaterialArray.clear();
			m_MaterialArray.resize(NumMaterials);
			for (int i=0; i<NumMaterials; ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(MaterialName, UniStr);				
				
				MaterialName[63] = 0x00;
				
				m_MaterialArray[i] = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MaterialName); 
				if (!m_MaterialArray[i])	
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[i], MaterialSystem::CMaterial::STANDARD);
					m_MaterialArray[i]->SetName(MaterialName);
					m_MaterialArray[i]->vLoad(stream, LYNX_FALSE);														
				}
				else
					m_MaterialArray[i]->vLoad(stream, LYNX_TRUE);									
			}		
			
			
			DWORD NumCameras;
			stream.vRead(&NumCameras, sizeof(DWORD), 1);
			m_CameraArray.resize(NumCameras);
			for (int i=0; i<NumCameras; i++)
			{
				m_CameraArray[i] = vCreateCamera();
				m_CameraArray[i]->vCreate();
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);		
				m_CameraArray[i]->SetName(Name);
				
				CCameraContainer* lpCamera = static_cast<CCameraContainer*>(m_CameraArray[i]->GetlpContainer());
				
				float XFOV, YFOV;
				
				stream.vRead(&XFOV, sizeof(float), 1);
				stream.vRead(&YFOV, sizeof(float), 1);
				lpCamera->SetFOV(XFOV, YFOV);
				stream.vRead(&lpCamera->m_Near, sizeof(float), 1);
				stream.vRead(&lpCamera->m_Far, sizeof(float), 1);
				
				stream.vRead(&lpCamera->m_bBlooming, sizeof(LYNXBOOL), 1);
				stream.vRead(&lpCamera->m_BloomingBlurKernel, sizeof(int), 1);
				stream.vRead(&lpCamera->m_BloomingBlurScale, sizeof(float), 1);
				
				stream.vRead(&lpCamera->m_bDOF, sizeof(LYNXBOOL), 1);
				stream.vRead(&lpCamera->m_DOFBlurKernel, sizeof(int), 1);
				stream.vRead(&lpCamera->m_DOFBlurScale, sizeof(float), 1);
				stream.vRead(&lpCamera->m_DOFSamplingKernel, sizeof(float), 1);
				stream.vRead(&lpCamera->m_FocalRange[0], sizeof(float), 4);
				
				stream.vRead(&lpCamera->m_bMotionBlur, sizeof(LYNXBOOL), 1);
				stream.vRead(&lpCamera->m_MotionBlurKernel, sizeof(int), 1);
				stream.vRead(&lpCamera->m_MotionBlurScale, sizeof(float), 1);
				
				stream.vRead(&lpCamera->m_bCameraMovingBlur, sizeof(LYNXBOOL), 1);
				stream.vRead(&lpCamera->m_CameraMovingBlurKernel, sizeof(int), 1);
				stream.vRead(&lpCamera->m_CameraMovingBlurScale, sizeof(float), 1);
				
				stream.vRead(&lpCamera->m_BrightPassThreshold, sizeof(float), 1);
				
				stream.vRead(&TM, sizeof(LYNXMATRIX), 1);
				lpCamera->SetTransformMatrix(TM);			
				lpCamera->SetLocalMatrix(TM);
				
				lpCamera->UpdateViewMatrix();
				lpCamera->UpdateProjectionMatrix((float)m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth()/(float)m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());		
			}
			stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
			lynxFromUniStr(Name, UniStr);	
			LynxEngine::CArray<CCamera*>::CIterator CurrentCamera = LynxEngine::find(m_CameraArray.begin(), m_CameraArray.end(), LynxEngine::CString(Name));
			if (CurrentCamera != m_CameraArray.end())
			{
				SetlpCurrentCamera(*CurrentCamera);
			}
			
			DWORD NumSky;
			stream.vRead(&NumSky, sizeof(DWORD), 1);
			if (NumSky)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				
				LynxEngine::CSkyDomeCreationDesc SkyDesc;
				stream.vRead(&SkyDesc.m_Radius, sizeof(float), 1);
				stream.vRead(&SkyDesc.m_Slices, sizeof(int), 1);
				stream.vRead(&SkyDesc.m_Sides, sizeof(int), 1);
				stream.vRead(&SkyDesc.m_SkyLightMapSize, sizeof(int), 1);
				
				lpObj = vCreateStaticObj(LynxEngine::SceneSystem::CStaticObj::SKY);				
				GetlpAtmosphere()->AddSky(lpObj);
				lpObj->vCreate(CString(Name), CContainer::SKYDOME, &SkyDesc);
			}
			
			LYNXCOLORRGBA	Color;
			float			Deg, Radius, Intensity;
			int				SMWidth, SMHeight;
			CLight*			lpLight;
			
			stream.vRead(&NumLights, sizeof(DWORD), 1);
			if (GetlpAtmosphere()->GetlpSky() && ((CSky*)GetlpAtmosphere()->GetlpSky())->GetSunLight())
				m_LightArray.resize(NumLights+1);
			else
				m_LightArray.resize(NumLights);
			
			for (int i=0; i<NumLights; ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				
				stream.vRead(&LightType, sizeof(CLight::TYPE), 1);
				
				//LightType = CLight::POINT;
				//LightType = CLight::SPOT;
				
				m_LightArray[i] = vCreateLight(LightType);
				m_LightArray[i]->SetName(Name);
				switch (LightType)
				{
					case CLight::AMBIENT:
						m_lpCurrentAmbientLight = m_LightArray[i];
						stream.vRead(&m_LightArray[i]->m_Color, sizeof(LYNXCOLORRGB), 1);						
						stream.vRead(&m_LightArray[i]->m_Intensity, sizeof(int), 1);											
						stream.vRead(&m_LightArray[i]->m_bCastShadow, sizeof(LYNXBOOL), 1);
						m_LightArray[i]->vCreate();
						break;
						
					case CLight::POINT:						
						stream.vRead(&Color, sizeof(LYNXCOLORRGB), 1);
						stream.vRead(&Radius, sizeof(float), 1);					
						stream.vRead(&Intensity, sizeof(int), 1);					
						stream.vRead(&SMWidth, sizeof(int), 1);						
						stream.vRead(&SMHeight, sizeof(int), 1);					
						m_LightArray[i]->vCreate(SMWidth, SMHeight);
						m_LightArray[i]->m_Color.Red = Color.Red;
						m_LightArray[i]->m_Color.Green = Color.Green;
						m_LightArray[i]->m_Color.Blue = Color.Blue;
						m_LightArray[i]->m_Radius = Radius;												
						m_LightArray[i]->m_Intensity = Intensity;
						stream.vRead(&m_LightArray[i]->m_bCastShadow, sizeof(LYNXBOOL), 1);
						lpRef = Container_Cast<CReferenceContainer*>(m_LightArray[i]->GetlpContainer());
						stream.vRead(&TM, sizeof(LYNXMATRIX), 1);
						lpRef->SetTransformMatrix(TM);			
						lpRef->SetLocalMatrix(TM);
						break;
						
					case CLight::DIRECTIONAL:						
						stream.vRead(&Color, sizeof(LYNXCOLORRGB), 1);
						stream.vRead(&Radius, sizeof(float), 1);					
						stream.vRead(&Intensity, sizeof(int), 1);					
						stream.vRead(&SMWidth, sizeof(int), 1);						
						stream.vRead(&SMHeight, sizeof(int), 1);					
						m_LightArray[i]->vCreate(SMWidth, SMHeight);
						m_LightArray[i]->m_Color.Red = Color.Red;
						m_LightArray[i]->m_Color.Green = Color.Green;
						m_LightArray[i]->m_Color.Blue = Color.Blue;
						m_LightArray[i]->m_Radius = Radius;												
						m_LightArray[i]->m_Intensity = Intensity;
						stream.vRead(&m_LightArray[i]->m_bCastShadow, sizeof(LYNXBOOL), 1);
						lpRef = Container_Cast<CReferenceContainer*>(m_LightArray[i]->GetlpContainer());
						stream.vRead(&TM, sizeof(LYNXMATRIX), 1);
						lpRef->SetTransformMatrix(TM);			
						lpRef->SetLocalMatrix(TM);
						break;
						
					case CLight::SPOT:						
						stream.vRead(&Color, sizeof(LYNXCOLORRGB), 1);
						stream.vRead(&Deg, sizeof(float), 1);					
						stream.vRead(&Radius, sizeof(float), 1);					
						stream.vRead(&Intensity, sizeof(int), 1);					
						stream.vRead(&SMWidth, sizeof(int), 1);						
						stream.vRead(&SMHeight, sizeof(int), 1);					
						m_LightArray[i]->vCreate(SMWidth, SMHeight);
						m_LightArray[i]->m_Color.Red = Color.Red;
						m_LightArray[i]->m_Color.Green = Color.Green;
						m_LightArray[i]->m_Color.Blue = Color.Blue;
						m_LightArray[i]->m_Deg = Deg;						
						m_LightArray[i]->m_Radius = Radius;						
						m_LightArray[i]->m_Intensity = Intensity;
						stream.vRead(&m_LightArray[i]->m_bCastShadow, sizeof(LYNXBOOL), 1);
						lpRef = Container_Cast<CReferenceContainer*>(m_LightArray[i]->GetlpContainer());
						stream.vRead(&TM, sizeof(LYNXMATRIX), 1);
						lpRef->SetTransformMatrix(TM);					
						lpRef->SetLocalMatrix(TM);
						//m_LightArray[i]->OverloadClassScript(_T("../script/COverloadedSpotLight.lua"), CAnsiString("_COverloadedSpotLight"));
						break;
				}				
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				
				stream.vRead(&Num, sizeof(DWORD), 1);				
				for (int j=0; j<Num; j++)
				{
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);									
				}				
				m_LightArray[i]->vComputeProxy();				
				m_LightArray[i]->vInit();
			}
			if (GetlpAtmosphere()->GetlpSky() && ((CSky*)GetlpAtmosphere()->GetlpSky())->GetSunLight())
				m_LightArray[NumLights] = (((CSky*)GetlpAtmosphere()->GetlpSky())->GetSunLight());
			
			stream.vRead(&NumStaticObjs, sizeof(DWORD), 1);
			m_StaticObjArray.resize(NumStaticObjs);			
			for (int i=0; i<m_StaticObjArray.size(); ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);								
				stream.vRead(&ID, sizeof(DWORD), 1);
				stream.vRead(&Type, sizeof(CStaticObj::TYPE), 1);
				stream.vRead(&ContainerType, sizeof(CContainer::TYPE), 1);			
				stream.vRead(&bGlow, sizeof(LYNXBOOL), 1);
				stream.vRead(&BloomingIntensity, sizeof(float), 1);				
				float MinDistance, MaxDistance;
				stream.vRead(&MinDistance, sizeof(float), 1);				
				stream.vRead(&MaxDistance, sizeof(float), 1);				
				
				lpObj = vCreateStaticObj(Type);	
				lpObj->SetID(ID);
				m_StaticObjArray[i] = lpObj;				
				
				switch (Type)
				{
					case CStaticObj::STATIC_MESH:
						lpObj->vCreate(CString(Name), ContainerType, NULL);
						lpGeo = Container_Cast<CGeometryContainer*>(m_StaticObjArray[i]->GetlpContainer());	
						lpGeo->vLoad(stream);		
						lpGeo->UpdateTransform();
						break;					
						
					case CStaticObj::TERRAIN:
					{
						LYNXMATRIX M;
						
						stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
						lynxFromUniStr(MaterialName, UniStr);				
						stream.vRead(&M, sizeof(LYNXMATRIX), 1);
						
						LynxEngine::CTerrainCreationDesc TerrainDesc;
						int Width, Height;
						
						stream.vRead(&Width, sizeof(int), 1);
						stream.vRead(&Height, sizeof(int), 1);
						stream.vRead(&TerrainDesc.m_TerrainMeshCellSize, sizeof(float), 1);
						
						stream.vRead(&TerrainDesc.m_HeightmapWidth, sizeof(int), 1);
						stream.vRead(&TerrainDesc.m_HeightmapHeight, sizeof(int), 1);
						stream.vRead(&TerrainDesc.m_HeightmapCellSize, sizeof(float), 1);
						
						TerrainDesc.m_SegmentSize = 128;
						lpObj->vCreate(CString(Name), ContainerType, &TerrainDesc);
						lpTerrain = Container_Cast<CTerrainContainer*>(m_StaticObjArray[i]->GetlpContainer());										
						lpTerrain->SetLocalMatrix(M);												
						lpTerrain->m_bSelfIlluminated = bGlow;
						lpTerrain->m_IlluminatingIntensity = BloomingIntensity;
						
						for (int i=0; i <4; i++)
						{
							stream.vRead(&lpTerrain->m_uTile[i], sizeof(float), 1);
							stream.vRead(&lpTerrain->m_vTile[i], sizeof(float), 1);
						}
						
						for (int y=0; y <Height; y++)
						{
							for (int x=0; x <Width; x++)
							{
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].Tangent, sizeof(float), 3);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].Normal, sizeof(float), 3);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].TexWeight[0], sizeof(float), 1);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].TexWeight[1], sizeof(float), 1);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].TexWeight[2], sizeof(float), 1);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].TexWeight[3], sizeof(float), 1);
							}
						}	
						stream.vRead(lpTerrain->m_lpHeightMap, sizeof(float), lpTerrain->m_HeightMapWidth*lpTerrain->m_HeightMapHeight);
						
						lpTerrain->SetlpCurrentMaterial(m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MaterialName));
						lpTerrain->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::ZPASS);
						lpTerrain->SetupBuffers();
						break;					
					}
						
					case CStaticObj::TREE:
					{
						//LYNXCHAR FileName[64];
						//LYNXMATRIX M;
						////CSpeedTreeCreationDesc Desc;
						
						//stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
						//lynxFromUniStr(FileName, UniStr);				
						//
						//stream.vRead(&Desc.m_Seed, sizeof(int), 1);
						//stream.vRead(&M, sizeof(LYNXMATRIX), 1);
						//
						////GetTexturePathFromModelPath(TexPath, FileName);
						////m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(TexPath);									
						////Desc.m_FileName = FileName;
						////lpObj->vCreate(CString(Name), ContainerType, &Desc);
						////lpSpeedTree = Container_Cast<CSpeedTreeContainer*>(lpObj->GetlpContainer());
						////lpSpeedTree->vCreate(&Desc);														
						////lpSpeedTree->SetLocalMatrix(M);
						////lpSpeedTree->UpdateTransform();								
						break;													
					}
						
				}			
				//Load physics
				LYNXBOOL bPhysics;
				stream.vRead(&bPhysics, sizeof(LYNXBOOL), 1);				
				if (bPhysics)
				{
					lpObj->vCreatePhysics(m_lpEngine->GetlpPhysicsSystem()->GetlpPhysicsWorld(0));
					lpObj->GetlpPhyObj()->vLoad(stream);
				}
				lpObj->vInit();
			}
			
			stream.vRead(&NumDynObjs, sizeof(DWORD), 1);
			m_DynamicObjArray.resize(NumDynObjs);			
			for (int i=0; i<m_DynamicObjArray.size(); ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				stream.vRead(&Type, sizeof(CDynamicObj::TYPE), 1);
				stream.vRead(&ContainerType, sizeof(CContainer::TYPE), 1);
				stream.vRead(&bGlow, sizeof(LYNXBOOL), 1);
				stream.vRead(&BloomingIntensity, sizeof(float), 1);
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 128);
				lynxFromUniStr(Filename, UniStr);				
				
				LYNXCHAR Buffer[256];
				lynxSprintf(Buffer, _T("%s"), Filename);				
				TName = Buffer;
				GetTexturePathFromModelPath(TexPath, Path);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(TexPath);
				
				lpDObj = vCreateDynamicObj(CDynamicObj::ENTITY);				
				if (ContainerType == CContainer::GEOMETRY)
				{	
					lpDObj->vCreate(CString(Name), ContainerType, NULL);				
					
					lpGeo = Container_Cast<CGeometryContainer*>(lpDObj->GetlpContainer());								
					lpGeo->vLoad(stream);
					lpGeo->UpdateTransform();
				}
				else if (ContainerType == CContainer::MODEL)
				{
					ModelDesc.m_bUseBufferObject = LYNX_TRUE;
					ModelDesc.m_FileName = TName;					
					ModelDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
					ModelDesc.m_Offset = 0;
					ModelDesc.m_bCreateMaterial = LYNX_FALSE;
					pCDesc = &ModelDesc;
					lpDObj->vCreate(CString(Name), ContainerType, pCDesc);				
					
					lpModel = Container_Cast<CModelContainer*>(lpDObj->GetlpContainer());								
					lpModel->m_bSelfIlluminated = bGlow;
					lpModel->m_IlluminatingIntensity = BloomingIntensity;
					for (int m=0; m<lpModel->GetNumMeshs(); ++m)
					{
						stream.vRead(UniStr, sizeof(LYNXUNICHAR), 256);
						lynxFromUniStr(Name, UniStr);
						lpModel->GetlpMesh(m)->SetlpCurrentMaterial(m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(Name)); 						
						stream.vRead(&lpModel->GetlpMesh(m)->m_bSelfIlluminated, sizeof(LYNXBOOL), 1);
						stream.vRead(&lpModel->GetlpMesh(m)->m_IlluminatingIntensity, sizeof(float), 1);					
					}
					LYNXMATRIX M;
					stream.vRead(&M, sizeof(LYNXMATRIX), 1);				
					lpModel->SetLocalMatrix(M);	
					lpModel->UpdateTransform();
					
					lpModel->m_AnimationSet->GetCurrentAnimation()->SetRange(0.0f, 0.0f);				
					lpModel->m_AnimationSet->GetCurrentAnimation()->SetStep(1.0f);				
					lpModel->m_AnimationSet->GetCurrentAnimation()->SetPlayMode(PM_LOOP);				
				}				
				m_DynamicObjArray[i] = lpDObj;	
				
				//Load physics
				LYNXBOOL bPhysics;
				stream.vRead(&bPhysics, sizeof(LYNXBOOL), 1);				
				if (bPhysics)
				{
					lpDObj->vCreatePhysics(m_lpEngine->GetlpPhysicsSystem()->GetlpPhysicsWorld(0));
					lpDObj->GetlpPhyObj()->vLoad(stream);					
				}
				
				// Load animations
				DWORD NumAnis;
				LYNXCHAR AniFileName[128], FileName[64], Name[64], EventName[64];
				float StartTime, EndTime, Step;
				PLAYMODE PlayMode;
				LynxEngine::CCreationDesc	AniDesc;
				Animation::CAnimation* lpAni;
				
				stream.vRead(&NumAnis, sizeof(DWORD), 1);
				for (int i=0; i<NumAnis; i++)
				{
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 128);
					lynxFromUniStr(FileName, UniStr);				
					
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);				
					
					AniDesc.m_FileName = LynxEngine::CString(FileName);
					lpAni = lpModel->vLoadAnimation(&AniDesc);
					lpModel->RenameAnimation(lpAni, LynxEngine::CString(Name));
					lpModel->SetCurrentAnimation(lpAni);
					
					stream.vRead(&lpAni->m_StartTime, sizeof(float), 1);
					stream.vRead(&lpAni->m_EndTime, sizeof(float), 1);
					stream.vRead(&lpAni->m_Step, sizeof(float), 1);
					stream.vRead(&lpAni->m_PlayMode, sizeof(LynxEngine::PLAYMODE), 1);
				}
				
				// Load state machine
				DWORD NumStates, NumTranslation, NumOutputs;
				stream.vRead(&NumStates, sizeof(DWORD), 1);
				for (int i=0; i<NumStates; i++)
				{
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);				
					
					StateMachine::CState* State = lpDObj->GetStateMachine().CreateState();
					lpDObj->GetStateMachine().AddState(State);					
					State->SetName(Name);
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);				
					Animation::CAnimation* lpAni = lpModel->m_AnimationSet->Find(CString(Name));
					if (lpAni)
						State->SetlpAnimation(lpAni);					
				}
				for (int i=0; i<NumStates; i++)
				{
					StateMachine::CState* State = lpDObj->GetStateMachine().GetState(i);
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(EventName, UniStr);				
					stream.vRead(&NumTranslation, sizeof(DWORD), 1);
					for (int j=0; j<NumTranslation; j++)
					{
						StateMachine::CStateTranslation::TRANSLATIONTYPE Type;
						stream.vRead(&Type, sizeof(StateMachine::CStateTranslation::TRANSLATIONTYPE), 1);
						StateMachine::CStateTranslation* lpTrans = lpDObj->GetStateMachine().CreateStateTranslation(Type);
						State->AddTranslation(lpTrans);
						stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
						lynxFromUniStr(Name, UniStr);				
						lpTrans->SetName(Name);
						stream.vRead(&lpTrans->m_Method, sizeof(StateMachine::CStateTranslation::TRANSLATIONMETHOD), 1);
						//Input name
						stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
						lynxFromUniStr(Name, UniStr);				
						StateMachine::CState* State = lpDObj->GetStateMachine().FindState(LynxEngine::CString(Name));
						lpTrans->SetlpInput(State);
						stream.vRead(&NumOutputs, sizeof(DWORD), 1);
						for (int k=0; k<NumOutputs; k++)
						{
							//Output name
							stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
							lynxFromUniStr(Name, UniStr);				
							State = lpDObj->GetStateMachine().FindState(LynxEngine::CString(Name));
							lpTrans->AddOutput(State);
						}
					}
					State->SetEventForAnimationStop(State->FindTranslation(CString(EventName)));
				}
				lpDObj->vSetCurrentState(CString(_T("idle0")));
				
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				stream.vRead(&Num, sizeof(DWORD), 1);
				lpDObj->SetNumChildren(Num);
				for (int j=0; j<Num; j++)
				{
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);														
				}	
				lpDObj->vInit();				
			}	
			//m_lpSpatialTree = LYNXNEW COSP();
			
			stream.vClose();
			
			PostLoading();
			
			Setup();			
			
			m_lpEngine->GetlpGraphicsSystem()->SetClipPlane(0, NULL);			
			
			//m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vDumpAllScripts(_T("@"));
			
			return TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScene::LoadISC(CStream& stream, const LYNXCHAR *filename)		
		{
			LYNXUNICHAR			UniStr[256];	
			char				Header[3];
			LYNXCHAR			ResourceName[64], MaterialName[64], MapName[64], Name[256], Path[128], Filename[64], TexPath[128];
			DWORD				ID;
			LYNXCHAR			TTexPath[256];	
			CString				TName;
			DWORD				NumRes, NumMaterials, NumPass, NumLights, NumStaticObjs, NumDynObjs, Num;
			CLight::TYPE		LightType;
			CStaticObj::TYPE	Type;	
			CContainer::TYPE	ContainerType;			
			BOOL				bGlow;
			float				BloomingIntensity;
			CStaticObj*			lpObj;
			CGeometryContainer*	lpGeo;
			CTerrainContainer*	lpTerrain;
			//CSpeedTreeContainer* lpSpeedTree;
			CSkyDomeContainer*	lpSkyDome;
			CDynamicObj*		lpDObj;
			CModelContainer*	lpModel;
			CPointLight*		lpPointLight;
			CSpotLight*			lpSpotLight;
			CReferenceContainer* lpRef;
			LYNXMATRIX			TM; 
			int					LOD = 0;
			MaterialSystem::CTechnique* Technique;
			CCreationDesc*		pCDesc;
			CModelCreationDesc	ModelDesc;		
						
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Open scene ("))+CString(filename)+CString(_T(") ----------------------------------------------------------------------------------\n")));

			if (!stream.IsValid())
			{
				if (!stream.vOpen(filename, CStream::Read|CStream::Binary))	
					return FALSE;				
			}

			stream.vRead(&Header, 3, 1);
			if (Header[0] != 'I' || Header[1] != 'S' || Header[2] != 'C')
			{
				stream.vClose();	
				return FALSE;		
			}
			stream.vRead(&m_Version, sizeof(DWORD), 1);				

			// Create Physics world and space
			if (m_lpEngine->GetlpPhysicsSystem())
			{
				PhysicsSystem::CPhysicsWorld* lpWorld = m_lpEngine->GetlpPhysicsSystem()->vCreateWorld();				
				//lpWorld->vCreateSpace(NULL);
			}

			// Preload sources
			GraphicsSystem::CTexturePtr Texture;
			stream.vRead(&NumRes, sizeof(DWORD), 1);
			for (int i=0; i<NumRes; ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(ResourceName, UniStr);				
				//((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(Texture, 0, ResourceName, NULL, 0);				
			}

			stream.vRead(&m_lpRenderer->m_RenderingOptions.ClearColor, sizeof(LYNXCOLORRGBA), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.bSSAOPass, sizeof(LYNXBOOL), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOSamplingKernel, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.bSSAOBlur, sizeof(LYNXBOOL), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOBlurKernel, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOBlurScale, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOBlurSharpness, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.SSAOThreshold, sizeof(float), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.bGlowPass, sizeof(LYNXBOOL), 1);
			stream.vRead(&m_lpRenderer->m_RenderingOptions.NumSplittedShadowMap, sizeof(int), 1);

			// Create scene renderer according to the render option
			m_lpRenderer->vCreate();

			m_lpRenderer->SetNumSplittedShadowMap(m_lpRenderer->m_RenderingOptions.NumSplittedShadowMap);

			stream.vRead(&NumMaterials, sizeof(DWORD), 1);
			m_MaterialArray.clear();
			m_MaterialArray.resize(NumMaterials);
			for (int i=0; i<NumMaterials; ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(MaterialName, UniStr);				

				MaterialName[63] = 0x00;

				m_MaterialArray[i] = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MaterialName); 
				if (!m_MaterialArray[i])	
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_MaterialArray[i], MaterialSystem::CMaterial::STANDARD);
					m_MaterialArray[i]->SetName(MaterialName);
					m_MaterialArray[i]->vLoad(stream, LYNX_FALSE);														
				}
				else
					m_MaterialArray[i]->vLoad(stream, LYNX_TRUE);									
			}		


			DWORD NumCameras;
			stream.vRead(&NumCameras, sizeof(DWORD), 1);
			m_CameraArray.resize(NumCameras);
			for (int i=0; i<NumCameras; i++)
			{
				m_CameraArray[i] = vCreateCamera();
				m_CameraArray[i]->vCreate();
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);		
				m_CameraArray[i]->SetName(Name);

				CCameraContainer* lpCamera = static_cast<CCameraContainer*>(m_CameraArray[i]->GetlpContainer());
				
				float XFOV, YFOV;

				stream.vRead(&XFOV, sizeof(float), 1);
				stream.vRead(&YFOV, sizeof(float), 1);
				lpCamera->SetFOV(XFOV, YFOV);
				stream.vRead(&lpCamera->m_Near, sizeof(float), 1);
				stream.vRead(&lpCamera->m_Far, sizeof(float), 1);

				stream.vRead(&lpCamera->m_bBlooming, sizeof(LYNXBOOL), 1);
				stream.vRead(&lpCamera->m_BloomingBlurKernel, sizeof(int), 1);
				stream.vRead(&lpCamera->m_BloomingBlurScale, sizeof(float), 1);

				stream.vRead(&lpCamera->m_bDOF, sizeof(LYNXBOOL), 1);
				stream.vRead(&lpCamera->m_DOFBlurKernel, sizeof(int), 1);
				stream.vRead(&lpCamera->m_DOFBlurScale, sizeof(float), 1);
				stream.vRead(&lpCamera->m_DOFSamplingKernel, sizeof(float), 1);
				stream.vRead(&lpCamera->m_FocalRange[0], sizeof(float), 4);

				stream.vRead(&lpCamera->m_bMotionBlur, sizeof(LYNXBOOL), 1);
				stream.vRead(&lpCamera->m_MotionBlurKernel, sizeof(int), 1);
				stream.vRead(&lpCamera->m_MotionBlurScale, sizeof(float), 1);

				stream.vRead(&lpCamera->m_bCameraMovingBlur, sizeof(LYNXBOOL), 1);
				stream.vRead(&lpCamera->m_CameraMovingBlurKernel, sizeof(int), 1);
				stream.vRead(&lpCamera->m_CameraMovingBlurScale, sizeof(float), 1);

				stream.vRead(&lpCamera->m_BrightPassThreshold, sizeof(float), 1);

				stream.vRead(&TM, sizeof(LYNXMATRIX), 1);
				lpCamera->SetTransformMatrix(TM);			
				lpCamera->SetLocalMatrix(TM);

				lpCamera->UpdateViewMatrix();
				lpCamera->UpdateProjectionMatrix((float)m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth()/(float)m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());		
			}
			stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
			lynxFromUniStr(Name, UniStr);	
			LynxEngine::CArray<CCamera*>::CIterator CurrentCamera = LynxEngine::find(m_CameraArray.begin(), m_CameraArray.end(), LynxEngine::CString(Name));
			if (CurrentCamera != m_CameraArray.end())
			{
				SetlpCurrentCamera(*CurrentCamera);
			}

			DWORD NumSky;
			stream.vRead(&NumSky, sizeof(DWORD), 1);
			if (NumSky)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				

				LynxEngine::CSkyDomeCreationDesc SkyDesc;
				stream.vRead(&SkyDesc.m_Radius, sizeof(float), 1);
				stream.vRead(&SkyDesc.m_Slices, sizeof(int), 1);
				stream.vRead(&SkyDesc.m_Sides, sizeof(int), 1);
				stream.vRead(&SkyDesc.m_SkyLightMapSize, sizeof(int), 1);

				lpObj = vCreateStaticObj(LynxEngine::SceneSystem::CStaticObj::SKY);				
				GetlpAtmosphere()->AddSky(lpObj);
				lpObj->vCreate(CString(Name), CContainer::SKYDOME, &SkyDesc);
			}

			LYNXCOLORRGBA	Color;
			float			Deg, Radius, Intensity;
			int				SMWidth, SMHeight;
			CLight*			lpLight;

			stream.vRead(&NumLights, sizeof(DWORD), 1);
			if (GetlpAtmosphere()->GetlpSky() && ((CSky*)GetlpAtmosphere()->GetlpSky())->GetSunLight())
				m_LightArray.resize(NumLights+1);
			else
				m_LightArray.resize(NumLights);

			for (int i=0; i<NumLights; ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				
				stream.vRead(&LightType, sizeof(CLight::TYPE), 1);

				//LightType = CLight::POINT;
				//LightType = CLight::SPOT;

				m_LightArray[i] = vCreateLight(LightType);
				m_LightArray[i]->SetName(Name);
				switch (LightType)
				{
					case CLight::AMBIENT:
						m_lpCurrentAmbientLight = m_LightArray[i];
						stream.vRead(&m_LightArray[i]->m_Color, sizeof(LYNXCOLORRGB), 1);						
						stream.vRead(&m_LightArray[i]->m_Intensity, sizeof(int), 1);											
						stream.vRead(&m_LightArray[i]->m_bCastShadow, sizeof(LYNXBOOL), 1);
						m_LightArray[i]->vCreate();
						break;

					case CLight::POINT:						
						stream.vRead(&Color, sizeof(LYNXCOLORRGB), 1);
						stream.vRead(&Radius, sizeof(float), 1);					
						stream.vRead(&Intensity, sizeof(int), 1);					
						stream.vRead(&SMWidth, sizeof(int), 1);						
						stream.vRead(&SMHeight, sizeof(int), 1);					
						m_LightArray[i]->vCreate(SMWidth, SMHeight);
						m_LightArray[i]->m_Color.Red = Color.Red;
						m_LightArray[i]->m_Color.Green = Color.Green;
						m_LightArray[i]->m_Color.Blue = Color.Blue;
						m_LightArray[i]->m_Radius = Radius;												
						m_LightArray[i]->m_Intensity = Intensity;
						stream.vRead(&m_LightArray[i]->m_bCastShadow, sizeof(LYNXBOOL), 1);
						lpRef = Container_Cast<CReferenceContainer*>(m_LightArray[i]->GetlpContainer());
						stream.vRead(&TM, sizeof(LYNXMATRIX), 1);
						lpRef->SetTransformMatrix(TM);			
						lpRef->SetLocalMatrix(TM);
						break;

					case CLight::DIRECTIONAL:						
						stream.vRead(&Color, sizeof(LYNXCOLORRGB), 1);
						stream.vRead(&Radius, sizeof(float), 1);					
						stream.vRead(&Intensity, sizeof(int), 1);					
						stream.vRead(&SMWidth, sizeof(int), 1);						
						stream.vRead(&SMHeight, sizeof(int), 1);					
						m_LightArray[i]->vCreate(SMWidth, SMHeight);
						m_LightArray[i]->m_Color.Red = Color.Red;
						m_LightArray[i]->m_Color.Green = Color.Green;
						m_LightArray[i]->m_Color.Blue = Color.Blue;
						m_LightArray[i]->m_Radius = Radius;												
						m_LightArray[i]->m_Intensity = Intensity;
						stream.vRead(&m_LightArray[i]->m_bCastShadow, sizeof(LYNXBOOL), 1);
						lpRef = Container_Cast<CReferenceContainer*>(m_LightArray[i]->GetlpContainer());
						stream.vRead(&TM, sizeof(LYNXMATRIX), 1);
						lpRef->SetTransformMatrix(TM);			
						lpRef->SetLocalMatrix(TM);
						break;

					case CLight::SPOT:						
						stream.vRead(&Color, sizeof(LYNXCOLORRGB), 1);
						stream.vRead(&Deg, sizeof(float), 1);					
						stream.vRead(&Radius, sizeof(float), 1);					
						stream.vRead(&Intensity, sizeof(int), 1);					
						stream.vRead(&SMWidth, sizeof(int), 1);						
						stream.vRead(&SMHeight, sizeof(int), 1);					
						m_LightArray[i]->vCreate(SMWidth, SMHeight);
						m_LightArray[i]->m_Color.Red = Color.Red;
						m_LightArray[i]->m_Color.Green = Color.Green;
						m_LightArray[i]->m_Color.Blue = Color.Blue;
						m_LightArray[i]->m_Deg = Deg;						
						m_LightArray[i]->m_Radius = Radius;						
						m_LightArray[i]->m_Intensity = Intensity;
						stream.vRead(&m_LightArray[i]->m_bCastShadow, sizeof(LYNXBOOL), 1);
						lpRef = Container_Cast<CReferenceContainer*>(m_LightArray[i]->GetlpContainer());
						stream.vRead(&TM, sizeof(LYNXMATRIX), 1);
						lpRef->SetTransformMatrix(TM);					
						lpRef->SetLocalMatrix(TM);
						//m_LightArray[i]->OverloadClassScript(_T("../script/COverloadedSpotLight.lua"), CAnsiString("_COverloadedSpotLight"));
						break;
				}				
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				
				stream.vRead(&Num, sizeof(DWORD), 1);				
				for (int j=0; j<Num; j++)
				{
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);									
				}				
				m_LightArray[i]->vComputeProxy();				
				m_LightArray[i]->vInit();
			}
			if (GetlpAtmosphere()->GetlpSky() && ((CSky*)GetlpAtmosphere()->GetlpSky())->GetSunLight())
				m_LightArray[NumLights] = (((CSky*)GetlpAtmosphere()->GetlpSky())->GetSunLight());

			stream.vRead(&NumStaticObjs, sizeof(DWORD), 1);
			m_StaticObjArray.resize(NumStaticObjs);			
			for (int i=0; i<m_StaticObjArray.size(); ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);								
				stream.vRead(&ID, sizeof(DWORD), 1);
				stream.vRead(&Type, sizeof(CStaticObj::TYPE), 1);
				stream.vRead(&ContainerType, sizeof(CContainer::TYPE), 1);			
				stream.vRead(&bGlow, sizeof(LYNXBOOL), 1);
				stream.vRead(&BloomingIntensity, sizeof(float), 1);				
				float MinDistance, MaxDistance;
				stream.vRead(&MinDistance, sizeof(float), 1);				
				stream.vRead(&MaxDistance, sizeof(float), 1);				

				lpObj = vCreateStaticObj(Type);	
				lpObj->SetID(ID);
				m_StaticObjArray[i] = lpObj;				
				
				switch (Type)
				{
					case CStaticObj::STATIC_MESH:
						lpObj->vCreate(CString(Name), ContainerType, NULL);
						lpGeo = Container_Cast<CGeometryContainer*>(m_StaticObjArray[i]->GetlpContainer());	
						lpGeo->vLoad(stream);		
						lpGeo->UpdateTransform();
						break;					

					case CStaticObj::TERRAIN:
					{
						LYNXMATRIX M;

						stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
						lynxFromUniStr(MaterialName, UniStr);				
						stream.vRead(&M, sizeof(LYNXMATRIX), 1);
						
						LynxEngine::CTerrainCreationDesc TerrainDesc;
						int Width, Height;

						stream.vRead(&Width, sizeof(int), 1);
						stream.vRead(&Height, sizeof(int), 1);
						stream.vRead(&TerrainDesc.m_TerrainMeshCellSize, sizeof(float), 1);

						stream.vRead(&TerrainDesc.m_HeightmapWidth, sizeof(int), 1);
						stream.vRead(&TerrainDesc.m_HeightmapHeight, sizeof(int), 1);
						stream.vRead(&TerrainDesc.m_HeightmapCellSize, sizeof(float), 1);

						TerrainDesc.m_SegmentSize = 128;
						lpObj->vCreate(CString(Name), ContainerType, &TerrainDesc);
						lpTerrain = Container_Cast<CTerrainContainer*>(m_StaticObjArray[i]->GetlpContainer());										
						lpTerrain->SetLocalMatrix(M);												
						lpTerrain->m_bSelfIlluminated = bGlow;
						lpTerrain->m_IlluminatingIntensity = BloomingIntensity;

						for (int i=0; i <4; i++)
						{
							stream.vRead(&lpTerrain->m_uTile[i], sizeof(float), 1);
							stream.vRead(&lpTerrain->m_vTile[i], sizeof(float), 1);
						}
						
						for (int y=0; y <Height; y++)
						{
							for (int x=0; x <Width; x++)
							{
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].Tangent, sizeof(float), 3);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].Normal, sizeof(float), 3);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].TexWeight[0], sizeof(float), 1);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].TexWeight[1], sizeof(float), 1);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].TexWeight[2], sizeof(float), 1);
								stream.vRead(&lpTerrain->m_VertexList[0][y*Width+x].TexWeight[3], sizeof(float), 1);
							}
						}	
						stream.vRead(lpTerrain->m_lpHeightMap, sizeof(float), lpTerrain->m_HeightMapWidth*lpTerrain->m_HeightMapHeight);

						lpTerrain->SetlpCurrentMaterial(m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MaterialName));
						lpTerrain->SetCurrentMaterialTechnique(MaterialSystem::CMaterial::ZPASS);
						lpTerrain->SetupBuffers();
						break;					
					}

					case CStaticObj::TREE:
					{
						//LYNXCHAR FileName[64];
						//LYNXMATRIX M;
						////CSpeedTreeCreationDesc Desc;

						//stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
						//lynxFromUniStr(FileName, UniStr);				
						//
						//stream.vRead(&Desc.m_Seed, sizeof(int), 1);
						//stream.vRead(&M, sizeof(LYNXMATRIX), 1);
						//
						////GetTexturePathFromModelPath(TexPath, FileName);
						////m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(TexPath);									
						////Desc.m_FileName = FileName;
						////lpObj->vCreate(CString(Name), ContainerType, &Desc);
						////lpSpeedTree = Container_Cast<CSpeedTreeContainer*>(lpObj->GetlpContainer());
						////lpSpeedTree->vCreate(&Desc);														
						////lpSpeedTree->SetLocalMatrix(M);
						////lpSpeedTree->UpdateTransform();								
						break;													
					}

				}			
				//Load physics
				LYNXBOOL bPhysics;
				stream.vRead(&bPhysics, sizeof(LYNXBOOL), 1);				
				if (bPhysics)
				{
					lpObj->vCreatePhysics(m_lpEngine->GetlpPhysicsSystem()->GetlpPhysicsWorld(0));
					lpObj->GetlpPhyObj()->vLoad(stream);
				}
				lpObj->vInit();
			}

			stream.vRead(&NumDynObjs, sizeof(DWORD), 1);
			m_DynamicObjArray.resize(NumDynObjs);			
			for (int i=0; i<m_DynamicObjArray.size(); ++i)
			{
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				stream.vRead(&Type, sizeof(CDynamicObj::TYPE), 1);
				stream.vRead(&ContainerType, sizeof(CContainer::TYPE), 1);
				stream.vRead(&bGlow, sizeof(LYNXBOOL), 1);
				stream.vRead(&BloomingIntensity, sizeof(float), 1);
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 128);
				lynxFromUniStr(Filename, UniStr);				
				
				LYNXCHAR Buffer[256];
				lynxSprintf(Buffer, _T("%s"), Filename);				
				TName = Buffer;
				GetTexturePathFromModelPath(TexPath, Path);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(TexPath);

				lpDObj = vCreateDynamicObj(CDynamicObj::ENTITY);				
				if (ContainerType == CContainer::GEOMETRY)
				{	
					lpDObj->vCreate(CString(Name), ContainerType, NULL);				

					lpGeo = Container_Cast<CGeometryContainer*>(lpDObj->GetlpContainer());								
					lpGeo->vLoad(stream);
					lpGeo->UpdateTransform();
				}
				else if (ContainerType == CContainer::MODEL)
				{
					ModelDesc.m_bUseBufferObject = LYNX_TRUE;
					ModelDesc.m_FileName = TName;
					ModelDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
					ModelDesc.m_Offset = 0;
					ModelDesc.m_bCreateMaterial = LYNX_FALSE;
					pCDesc = &ModelDesc;
					lpDObj->vCreate(CString(Name), ContainerType, pCDesc);				

					lpModel = Container_Cast<CModelContainer*>(lpDObj->GetlpContainer());								
					lpModel->m_bSelfIlluminated = bGlow;
					lpModel->m_IlluminatingIntensity = BloomingIntensity;
					for (int m=0; m<lpModel->GetNumMeshs(); ++m)
					{
						stream.vRead(UniStr, sizeof(LYNXUNICHAR), 256);
						lynxFromUniStr(Name, UniStr);
						lpModel->GetlpMesh(m)->SetlpCurrentMaterial(m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(Name)); 						
						stream.vRead(&lpModel->GetlpMesh(m)->m_bSelfIlluminated, sizeof(LYNXBOOL), 1);
						stream.vRead(&lpModel->GetlpMesh(m)->m_IlluminatingIntensity, sizeof(float), 1);					
					}
					LYNXMATRIX M;
					stream.vRead(&M, sizeof(LYNXMATRIX), 1);				
					lpModel->SetLocalMatrix(M);	
					lpModel->UpdateTransform();

					lpModel->m_AnimationSet->GetCurrentAnimation()->SetRange(0.0f, 0.0f);				
					lpModel->m_AnimationSet->GetCurrentAnimation()->SetStep(1.0f);				
					lpModel->m_AnimationSet->GetCurrentAnimation()->SetPlayMode(PM_LOOP);				
				}				
				m_DynamicObjArray[i] = lpDObj;	

				//Load physics
				LYNXBOOL bPhysics;
				stream.vRead(&bPhysics, sizeof(LYNXBOOL), 1);				
				if (bPhysics)
				{
					lpDObj->vCreatePhysics(m_lpEngine->GetlpPhysicsSystem()->GetlpPhysicsWorld(0));
					lpDObj->GetlpPhyObj()->vLoad(stream);					
				}

				// Load animations
				DWORD NumAnis;
				LYNXCHAR AniFileName[128], FileName[64], Name[64], EventName[64];
				float StartTime, EndTime, Step;
				PLAYMODE PlayMode;
				LynxEngine::CCreationDesc	AniDesc;
				Animation::CAnimation* lpAni;

				stream.vRead(&NumAnis, sizeof(DWORD), 1);
				for (int i=0; i<NumAnis; i++)
				{
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 128);
					lynxFromUniStr(FileName, UniStr);				

					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);				
										
					AniDesc.m_FileName = LynxEngine::CString(FileName);
					lpAni = lpModel->vLoadAnimation(&AniDesc);
					lpModel->RenameAnimation(lpAni, LynxEngine::CString(Name));
					lpModel->SetCurrentAnimation(lpAni);

					stream.vRead(&lpAni->m_StartTime, sizeof(float), 1);
					stream.vRead(&lpAni->m_EndTime, sizeof(float), 1);
					stream.vRead(&lpAni->m_Step, sizeof(float), 1);
					stream.vRead(&lpAni->m_PlayMode, sizeof(LynxEngine::PLAYMODE), 1);
				}

				// Load state machine
				DWORD NumStates, NumTranslation, NumOutputs;
				stream.vRead(&NumStates, sizeof(DWORD), 1);
				for (int i=0; i<NumStates; i++)
				{
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);				
					
					StateMachine::CState* State = lpDObj->GetStateMachine().CreateState();
					lpDObj->GetStateMachine().AddState(State);					
					State->SetName(Name);
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);				
					Animation::CAnimation* lpAni = lpModel->m_AnimationSet->Find(CString(Name));
					if (lpAni)
						State->SetlpAnimation(lpAni);					
				}
				for (int i=0; i<NumStates; i++)
				{
					StateMachine::CState* State = lpDObj->GetStateMachine().GetState(i);
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(EventName, UniStr);				
					stream.vRead(&NumTranslation, sizeof(DWORD), 1);
					for (int j=0; j<NumTranslation; j++)
					{
						StateMachine::CStateTranslation::TRANSLATIONTYPE Type;
						stream.vRead(&Type, sizeof(StateMachine::CStateTranslation::TRANSLATIONTYPE), 1);
						StateMachine::CStateTranslation* lpTrans = lpDObj->GetStateMachine().CreateStateTranslation(Type);
						State->AddTranslation(lpTrans);
						stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
						lynxFromUniStr(Name, UniStr);				
						lpTrans->SetName(Name);
						stream.vRead(&lpTrans->m_Method, sizeof(StateMachine::CStateTranslation::TRANSLATIONMETHOD), 1);
						//Input name
						stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
						lynxFromUniStr(Name, UniStr);				
						StateMachine::CState* State = lpDObj->GetStateMachine().FindState(LynxEngine::CString(Name));
						lpTrans->SetlpInput(State);
						stream.vRead(&NumOutputs, sizeof(DWORD), 1);
						for (int k=0; k<NumOutputs; k++)
						{
							//Output name
							stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
							lynxFromUniStr(Name, UniStr);				
							State = lpDObj->GetStateMachine().FindState(LynxEngine::CString(Name));
							lpTrans->AddOutput(State);
						}
					}
					State->SetEventForAnimationStop(State->FindTranslation(CString(EventName)));
				}
				lpDObj->vSetCurrentState(CString(_T("idle0")));

				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(Name, UniStr);				
				stream.vRead(&Num, sizeof(DWORD), 1);
				lpDObj->SetNumChildren(Num);
				for (int j=0; j<Num; j++)
				{
					stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
					lynxFromUniStr(Name, UniStr);														
				}	
				lpDObj->vInit();				
			}	
			//m_lpSpatialTree = LYNXNEW COSP();

			stream.vClose();

			PostLoading();

			Setup();			

			m_lpEngine->GetlpGraphicsSystem()->SetClipPlane(0, NULL);			

			//m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vDumpAllScripts(_T("@"));
			
			return TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::Pause(LYNXBOOL b)
		{
			m_bPause = b;
			m_lpEngine->GetlpKernel()->PauseGameTime(b);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::LoadBackgroundTexture(const LYNXCHAR* filename, GraphicsSystem::CRenderer::TEXTUREDESC* desc)
		{
			GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;

            if (!desc)
            {
                TexDesc.Default();
                TexDesc.bSRGB = LYNX_FALSE;
				TexDesc.MipMapLevel = 1;				
				TexDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
				TexDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
				TexDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
				TexDesc.SamplerDesc.Filter	 = GraphicsSystem::CRenderer::FM_MIN_MAG_MIP_POINT;
				TexDesc.SamplerDesc.MinLOD = 0;
				TexDesc.SamplerDesc.MaxLOD = 0;
                m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpRenderer->m_RenderingOptions.BackgroundTexture, *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), filename, &TexDesc);
            }
            else
            {
                m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpRenderer->m_RenderingOptions.BackgroundTexture, *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), filename, desc);
            }
			m_lpRenderer->m_RenderingOptions.bUseBackgroundTexture = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::SetBackgroundTextureUVOffset(float u0, float v0, float u1, float v1)
		{
			LYNXUVPOINT	SubPixelOffset;
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetSubPixelOffset(&SubPixelOffset);

			m_lpRenderer->m_BackgroundUVOffset[0].u = u0 + SubPixelOffset.u;  m_lpRenderer->m_BackgroundUVOffset[0].v = v0 + SubPixelOffset.v;
			m_lpRenderer->m_BackgroundUVOffset[1].u = u1 + SubPixelOffset.u;  m_lpRenderer->m_BackgroundUVOffset[1].v = v0 + SubPixelOffset.v;
			m_lpRenderer->m_BackgroundUVOffset[2].u = u0 + SubPixelOffset.u;  m_lpRenderer->m_BackgroundUVOffset[2].v = v1 + SubPixelOffset.v;
			m_lpRenderer->m_BackgroundUVOffset[3].u = u1 + SubPixelOffset.u;  m_lpRenderer->m_BackgroundUVOffset[3].v = v1 + SubPixelOffset.v;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::AddStaticObj(CStaticObj* lpo)
		{
			m_StaticObjArray.push_back(lpo);			

			if (lpo->m_HierarchyData.Parent.lpObj)
				lpo->m_HierarchyData.Parent.lpObj->AddChild(lpo);
			else
				m_Root.AddChild(lpo);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::RemoveStaticObj(CStaticObj* lpo)
		{
			CArray<CStaticObj*>::CIterator Pos;

			Pos = LynxEngine::find(m_StaticObjArray.begin(), m_StaticObjArray.end(), lpo);
			if (Pos == m_StaticObjArray.end())
				return;

			m_StaticObjArray.erase(Pos);

			if (lpo->m_HierarchyData.Parent.lpObj)
				lpo->m_HierarchyData.Parent.lpObj->RemoveChild(lpo);				
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::RemoveAllStaticObjs()
		{			
			m_StaticObjArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CDynamicObj* const CScene::GetlpDynamicObj(const CString& name)
		{
			CArray<CDynamicObj*>::CIterator Pos;

			Pos = LynxEngine::find(m_DynamicObjArray.begin(), m_DynamicObjArray.end(), name);
			if (Pos == m_DynamicObjArray.end())
				return NULL;
			return (*Pos);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::AddDynamicObj(CDynamicObj* lpo)
		{
			CArray<CDynamicObj*>::CIterator Pos;

			Pos = LynxEngine::find(m_DynamicObjArray.begin(), m_DynamicObjArray.end(), lpo);
			if (Pos == m_DynamicObjArray.end())
			{
				m_DynamicObjArray.push_back(lpo);

				if (lpo->m_HierarchyData.Parent.lpObj)
					lpo->m_HierarchyData.Parent.lpObj->AddChild(lpo);
				else
					m_Root.AddChild(lpo);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::RemoveDynamicObj(CDynamicObj* lpo)
		{
			CArray<CDynamicObj*>::CIterator Pos;

			if (!m_DynamicObjArray.empty())
			{
				Pos = LynxEngine::find(m_DynamicObjArray.begin(), m_DynamicObjArray.end(), lpo);
				if (Pos == m_DynamicObjArray.end())
					return;

				if (lpo->m_HierarchyData.Parent.lpObj)
				{
					lpo->m_HierarchyData.Parent.lpObj->RemoveChild(lpo);
				}
				m_DynamicObjArray.erase(Pos);									
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::RemoveAllDynamicObjs()
		{
			m_DynamicObjArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::AddLight(CLight* lpl)
		{
			m_LightArray.push_back(lpl);

			if (lpl->m_HierarchyData.Parent.lpObj)
				lpl->m_HierarchyData.Parent.lpObj->AddChild(lpl);
			else
				m_Root.AddChild(lpl);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::RemoveLight(CLight* lpl)
		{
			CArray<CLight*>::CIterator Pos;

			Pos = LynxEngine::find(m_LightArray.begin(), m_LightArray.end(), lpl);
			if (Pos == m_LightArray.end())
				return;

			if (lpl->m_HierarchyData.Parent.lpObj)
				lpl->m_HierarchyData.Parent.lpObj->RemoveChild(lpl);

			m_LightArray.erase(Pos);						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::RemoveAllLights()
		{
			m_LightArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CStaticObj* const CScene::GetlpStaticObj(const CString& name)
		{
			CArray<CStaticObj*>::CIterator Pos;

			Pos = LynxEngine::find(m_StaticObjArray.begin(), m_StaticObjArray.end(), name);
			if (Pos == m_StaticObjArray.end())
				return NULL;
			return (*Pos);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CStaticObj* const CScene::GetlpStaticObj(const LYNXCHAR* name)
		{
			return GetlpStaticObj(CString(name));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CScene::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "GetVersion",				LynxEngine::SceneSystem::CScene, DWORD,		GetVersion,				(void))
			REGISTER_SCRIPT_METHOD(lps, "Setup",					LynxEngine::SceneSystem::CScene, LYNXBOOL,	Setup,					(void))
			REGISTER_SCRIPT_METHOD(lps, "SetRenderMode",			LynxEngine::SceneSystem::CScene, void,		SetRenderMode,			(int))
			REGISTER_SCRIPT_METHOD(lps, "LoadBackgroundTexture",	LynxEngine::SceneSystem::CScene, void,		LoadBackgroundTexture,	(const LYNXCHAR *))
			REGISTER_SCRIPT_METHOD(lps, "SetBackgroundTextureUVOffset",	LynxEngine::SceneSystem::CScene, void,	SetBackgroundTextureUVOffset,	(float, float, float, float))			
			REGISTER_SCRIPT_METHOD(lps, "SetSortMode",				LynxEngine::SceneSystem::CScene, void,		SetSortMode,	(int))						
				
			REGISTER_SCRIPT_METHOD(lps, "CreateCamera",				LynxEngine::SceneSystem::CScene, CCamera*,	CreateCamera,			(void))			
			REGISTER_SCRIPT_METHOD(lps, "SetCurrentCamera",			LynxEngine::SceneSystem::CScene, void,		SetlpCurrentCamera,		(void*))
			REGISTER_SCRIPT_METHOD(lps, "GetCurrentCamera",			LynxEngine::SceneSystem::CScene, CCamera*,	GetlpCurrentCamera,		(void))			

			REGISTER_SCRIPT_METHOD(lps, "CreateStaticMesh",			LynxEngine::SceneSystem::CScene, void*,		CreateStaticMesh,		())
			REGISTER_SCRIPT_METHOD(lps, "CreateStaticModel",		LynxEngine::SceneSystem::CScene, void*,		CreateStaticModel,		())
			REGISTER_SCRIPT_METHOD(lps, "AddStaticObj",				LynxEngine::SceneSystem::CScene, void,		AddStaticObj,			(void *))						
			REGISTER_SCRIPT_METHOD(lps, "GetStaticObj",				LynxEngine::SceneSystem::CScene, void*,		GetStaticObj,			(const LYNXCHAR *))
			REGISTER_SCRIPT_METHOD(lps, "ReleaseStaticObj",			LynxEngine::SceneSystem::CScene, void,		ReleaseStaticObj,		(void *))			
			REGISTER_SCRIPT_METHOD(lps, "RemoveStaticObj",			LynxEngine::SceneSystem::CScene, void,		RemoveStaticObj,		(void *))						
			REGISTER_SCRIPT_METHOD(lps, "RemoveAllStaticObjs",		LynxEngine::SceneSystem::CScene, void,		RemoveAllStaticObjs,	())						

			REGISTER_SCRIPT_METHOD(lps, "CreateDynamicEntity",		LynxEngine::SceneSystem::CScene, void*,		CreateDynamicEntity,	())
			REGISTER_SCRIPT_METHOD(lps, "AddDynamicObj",			LynxEngine::SceneSystem::CScene, void,		AddDynamicObj,			(void *))						
			REGISTER_SCRIPT_METHOD(lps, "GetDynamicObj",			LynxEngine::SceneSystem::CScene, void*,		GetDynamicObjByName,	(const LYNXCHAR* name))						
			REGISTER_SCRIPT_METHOD(lps, "GetDynamicObjByIndex",		LynxEngine::SceneSystem::CScene, void*,		GetDynamicObjByIndex,	(int))						
			REGISTER_SCRIPT_METHOD(lps, "ReleaseDynamicObj",		LynxEngine::SceneSystem::CScene, void,		ReleaseDynamicObj,		(void *))			
			REGISTER_SCRIPT_METHOD(lps, "RemoveDynamicObj",			LynxEngine::SceneSystem::CScene, void,		RemoveDynamicObj,		(void *))						
			REGISTER_SCRIPT_METHOD(lps, "RemoveAllDynamicObjs",		LynxEngine::SceneSystem::CScene, void,		RemoveAllDynamicObjs,	())						
									
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::PreLoading()
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::PostLoading()
		{
			for (int i=0; i<m_LightArray.size(); ++i)
			{	
				if (m_LightArray[i]->GetLightType() !=  CLight::AMBIENT)
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->SetlpCurrentLight(m_LightArray[i]);														
					break;
				}
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScene::Setup(void)
		{
			m_Root.SetNumChildren(0);
			for (int i=0; i<m_LightArray.size(); ++i)
			{	
				if (!m_LightArray[i]->GetlpParent() || m_LightArray[i]->GetlpParent() == &m_Root)
					m_Root.AddChild(m_LightArray[i]);
			}
			
			for (int i=0; i<m_StaticObjArray.size(); ++i)
			{	
				if (!m_StaticObjArray[i]->GetlpParent() || m_StaticObjArray[i]->GetlpParent() == &m_Root)
					m_Root.AddChild(m_StaticObjArray[i]);

			}
			for (int i=0; i<m_DynamicObjArray.size(); ++i)
			{	
				if (!m_DynamicObjArray[i]->GetlpParent() || m_DynamicObjArray[i]->GetlpParent() == &m_Root)
					m_Root.AddChild(m_DynamicObjArray[i]);
			}
			
			return TRUE;	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::vLoop(float step)
		{	
			if (!m_bPause)
			{
				CGameObj::vLoop(step);

				CList<MaterialSystem::CMaterialPtr>::CIterator Mat;
				for (Mat = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList().begin(); Mat != m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->GetMaterialList().end(); ++Mat)
				{
                    if ((*Mat)->m_bPlayInMainLoop)
                        (*Mat)->vPlayAll(step);				
				}

				if (m_lpAtmosphere && m_lpAtmosphere->GetlpSky())
				{
					m_lpAtmosphere->GetlpSky()->GetlpContainer()->vPlay(step);
				}
				for (int i=0; i<m_Root.GetNumChildren(); i++)
				{				
					if (m_Root.GetlpChild(i))
						((CGameObj*)m_Root.GetlpChild(i))->vLoop(step);
				}				
				//LynxEngine::CSpeedTreeContainer::UpdateWind(step/15.0f);
			}
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::Render(CCameraContainer& camera)
		{	
			static float IntoConsoleTime = 0.0f;			

			PERF_COUNTER_RESET(_T("Scene"), _T("NumVisContainer"), 0)

			#ifdef __iOS__
				m_lpRenderer->Render(camera);
			#else
				if (!m_lpEngine->GetlpGameConsoleSystem()->IsRunning())
				{
					m_lpRenderer->Render(camera);
					/*
					IntoConsoleTime = 0;		
					
					#ifdef __WIN32__
						if (camera.GetViewportRect().x2 == camera.GetViewportRect().x1)  // that means the viewport is full screen, so just blt texture  
							m_lpEngine->GetlpGraphicsSystem()->BltTextureToBackBuffer(m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture());				
						else
						{
							m_lpEngine->GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);							
							m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetSRGBWrite(LYNX_FALSE);
							camera.SetViewportRect();
							//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, 1280, 720);
							m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
							m_lpEngine->GetlpGraphicsSystem()->GetPostProcessMaterial(GraphicsSystem::BLT)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetPS()->GetlpShader()->vSetTexture(0, m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetTexture());
							m_lpEngine->GetlpGraphicsSystem()->DrawScreenSpaceQuad();
						}	
					#endif
					*/
				}
				/*
				else
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetSRGBWrite(LYNX_FALSE);
					m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->ResetCurrent();
					m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->DownSampled();
					int NumBlur = LYNX_CLAMP(LYNX_ROUND(IntoConsoleTime), 1, 8);
					//m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->BoxBlur(NumBlur, 2.5f);									
					//m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->Gaussin4x4Blur(NumBlur, 2.0f);									
					m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GaussinBlur(NumBlur, 2.0f);									
					m_lpEngine->GetlpGraphicsSystem()->BltTextureToBackBuffer(m_lpEngine->GetlpGraphicsSystem()->GetlpOffscreen()->GetCurrent());
					IntoConsoleTime += 1.0f;//*m_lpEngine->m_Step;
					if (IntoConsoleTime > 8)
						IntoConsoleTime = 8;
				}
				*/
			#endif
			//m_lpEngine->GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);							
			//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::GlowRender(CCameraContainer& camera)
		{
			static float IntoConsoleTime = 0.0f;

			if (!m_lpEngine->GetlpGameConsoleSystem()->IsRunning())
			{
				m_lpRenderer->GlowRender(camera);					
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::ComputeVisObjLOD(CCameraContainer& camera)
		{
			CList<CRenderableObj*>::CIterator Obj;

			for (Obj=m_VisObjList.begin(); Obj!=m_VisObjList.end(); ++Obj)
			{
				(*Obj)->CalculateLOD(&camera);
			}									
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::ComputeVisList(CCameraContainer& camera)
		{
			PERF_TIMER_RESET_START(_T("Scene"), _T("ComputeVisList"))

			//m_RNRData.clear();					
			m_VisObjList.clear();
			m_lpRenderer->ClearVisOpaqueContainerList();
			m_lpRenderer->ClearVisTranslucentContainerList();

			for (int i=0; i<m_StaticObjArray.size(); i++)							
			{	
				if (m_StaticObjArray[i]->IsShown())
					m_lpRenderer->AddObjToVisList(m_StaticObjArray[i]);				
			}
			for (int i=0; i<m_DynamicObjArray.size(); i++)	
			{
				if (m_DynamicObjArray[i]->IsShown() && m_DynamicObjArray[i]->GetVisType() != CDynamicObj::VT_ALWAYS_FAIL)
					//m_lpRenderer->AddObjToVisList(m_DynamicObjArray[i]);												
					m_DynamicObjArray[i]->vAddToSceneVisList();
			}		
			//ComputeVisObjLOD(camera);
			PERF_TIMER_STOP(_T("Scene"), _T("ComputeVisList"))
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CScene::ComputeVisLightList()
		{
			CList<CRenderableObj*>::CIterator Obj;
			for (Obj = m_VisObjList.begin(); Obj != m_VisObjList.end(); ++Obj)
			{
				(*Obj)->ComputeVisLightList();				
				for (int i=0; i<m_LightArray.size(); ++i)
				{
					if (m_LightArray[i]->GetLightType() != CLight::AMBIENT)
					{
						(*Obj)->GetVisDynamicLightList().push_back(m_LightArray[i]);
						m_LightArray[i]->ShadowmapDirty(LYNX_TRUE);
					}
				}
			}	

			m_VisLightList.clear();
			for (int i=0; i<m_LightArray.size(); ++i)
			{
				if (m_LightArray[i]->GetLightType() != CLight::AMBIENT)
				{
					m_VisLightList.push_back(m_LightArray[i]);					
				}
				INC_COUNTER(m_lpRenderer->m_Profile.NumVisLight, 1);
			}			
		}	
	}
}
