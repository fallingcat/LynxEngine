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

#ifndef __LYNXSCN_H__
#define __LYNXSCN_H__

#include <LynxString.h>
#include <LynxGameObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <SceneSystem/StaticObj//LynxStaticObj.h>
#include <SceneSystem/LynxScnRenderer.h>
#include <SceneSystem/DynObj/LynxDynObj.h>
#include <SceneSystem/Light/LynxAmbientLight.h>
#include <SceneSystem/Light/LynxDirectionalLight.h>
#include <SceneSystem/Light/LynxPointLight.h>
#include <SceneSystem/Light/LynxSpotLight.h>
#include <SceneSystem/HeightTable/LynxHeightTable.h>
//#include <SceneSystem/Tree/LynxSceneTree.h>
#include <Tree/LynxOSP.h>
#include <Container/LynxCameraContainer.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CSceneCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CSceneCreationDesc);	
	public:							
		SceneSystem::RENDERERTYPE			m_RendererType;
		LYNXBOOL							m_bUsePhysics;

		CSceneCreationDesc() {Default(); };
		void Default() {CCreationDesc::Default(); m_bUsePhysics = LYNX_FALSE; m_RendererType = SceneSystem::RT_DEFERRED; };
	};		

	namespace SceneSystem 
	{	
		class LYNXENGCLASS CScene : public CGameObj
		{	
			LYNX_DECLARE_CLASS(CScene);
		public:			
		protected:			
			CSceneSystem*								m_lpSceneSystem;			

			DWORD										m_Version;	
			LYNXBOOL									m_bPause;
			
			CAtmosphere*								m_lpAtmosphere;																
			CGameObj									m_Root;
			CLight*										m_lpCurrentAmbientLight;
			CArray<MaterialSystem::CMaterialPtr>		m_MaterialArray;
			CCamera*									m_lpCurrentCamera;
			CArray<CCamera*>							m_CameraArray;
			CArray<CLight*>								m_LightArray;
			CArray<CSurface*>							m_SurfaceArray;
			CArray<CStaticObj*>							m_StaticObjArray;
			CArray<CDynamicObj*>						m_DynamicObjArray;	

			CList<CRenderableObj*>						m_VisObjList;
			CList<CLight*>								m_VisLightList;			
			
			RENDERERTYPE								m_RendererType;
			CRenderer*									m_lpRenderer;

			CTree*										m_lpSpatialTree;						

			//Physics

			PhysicsSystem::CPhysicsWorld*				m_PhysicsWorld;
		public:			
			CScene();			
			CScene(CSceneSystem* ps);
			virtual ~CScene();

			DECLARE_CLASSSCRIPT
			virtual LYNXBOOL							vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			LYNXFORCEINLINE CStaticObj*					CreateStaticMesh() {return vCreateStaticObj(CStaticObj::STATIC_MESH); };	
			LYNXFORCEINLINE CStaticObj*					CreateStaticModel() {return vCreateStaticObj(CStaticObj::STATIC_MODEL); };	
			LYNXFORCEINLINE CStaticObj* const			GetStaticObj(const LYNXCHAR* name) {return GetlpStaticObj(name); };			
			LYNXFORCEINLINE void						ReleaseStaticObj(CStaticObj* s) {LYNXDEL s; };			
			LYNXFORCEINLINE CDynamicObj*				CreateDynamicEntity() {return vCreateDynamicObj(CDynamicObj::ENTITY); };	
			LYNXINLINE CDynamicObj* const				GetDynamicObjByIndex(int i) {return m_DynamicObjArray[i]; };		
			LYNXINLINE CDynamicObj* const				GetDynamicObjByName(const LYNXCHAR* name) {return GetlpDynamicObj(CString(name)); };					
			LYNXFORCEINLINE void						ReleaseDynamicObj(CDynamicObj* d) {LYNXDEL d; };			
			LYNXFORCEINLINE void						SetRenderMode(CRenderer::RENDERMODE mode) {m_lpRenderer->m_RenderingOptions.RenderMode = mode; };	
			void										LoadBackgroundTexture(const LYNXCHAR* filename, GraphicsSystem::CRenderer::TEXTUREDESC* desc = NULL);
			void										SetBackgroundTextureUVOffset(float u0, float v0, float u1, float v1);


			LYNXINLINE DWORD							GetVersion() {return m_Version; };

			LYNXFORCEINLINE LYNXBOOL					IsPaused() {return m_bPause; };
			void										Pause(LYNXBOOL b);

			static void									GetTexturePathFromModelPath(LYNXCHAR *lpr, const LYNXCHAR *lpc);
			LYNXBOOL									Create(const CCreationDesc* desc = NULL);
			LYNXBOOL									Release(void);

			void										InitXMLTag();
			LYNXBOOL									Load(CStream& stream, const LYNXCHAR *filename);		
			LYNXBOOL									LoadSCN(CStream& stream, const LYNXCHAR *filename);		
			LYNXBOOL									LoadISC(CStream& stream, const LYNXCHAR *filename);		
			LYNXBOOL									LoadLYNX(const LYNXCHAR *filename, LPLYNXFILE lpfs = NULL, int offset = 0);		
			LYNXBOOL									Setup(void);	
			
			LYNXINLINE CRenderer*						GetlpRenderer() {return m_lpRenderer; };
			LYNXINLINE CSceneSystem* const				GetlpSceneSystem() {return m_lpSceneSystem; };
				
			virtual	void								vLoop(float step);			
			//void										RenderReflection(void);
			void										BeginRendering();					
			void										Render(CCameraContainer& camera);	
			void										GlowRender(CCameraContainer& camera);
			void										EndRendering();							
			LYNXINLINE void								SetSortMode(CRenderer::SORTMODE m) {m_lpRenderer->SetSortMode(m); };
			
			LYNXINLINE CAtmosphere*						GetlpAtmosphere() {return m_lpAtmosphere; };
			LYNXINLINE CGameObj*						GetlpRoot() {return &m_Root; };		
			//Camera -------------------------------------------------------------------------------------------------------------------------------------------------------			
			virtual	CCamera*							vCreateCamera();	
			CCamera*									CreateCamera();
			LYNXINLINE void								AllocateCameras(int num) {m_CameraArray.resize(num); };
			LYNXINLINE void								SetlpCamera(int i, CCamera* const lpc) {m_CameraArray[i] = lpc; };
			LYNXINLINE CCamera* const					GetlpCamera(int i) {return m_CameraArray[i]; };		
			void										AddCamera(CCamera* lpc);			
			LYNXINLINE CCamera*							GetlpCurrentCamera() const {return m_lpCurrentCamera; };			
			LYNXINLINE void								SetlpCurrentCamera(CCamera* c) {m_lpCurrentCamera = c; };			

			//Light -------------------------------------------------------------------------------------------------------------------------------------------------------			
			virtual	CLight*								vCreateLight(CLight::TYPE type);	
			LYNXINLINE void								AllocateLights(int num) {m_LightArray.resize(num); };
			LYNXINLINE int								GetNumLights() {return (DWORD)m_LightArray.size(); };
			LYNXINLINE void								SetlpLight(int i, CLight* const lpl) {m_LightArray[i] = lpl; };
			LYNXINLINE CLight* const					GetlpLight(int i) {return m_LightArray[i]; };		
			void										AddLight(CLight* lpl);
			void										RemoveAllLights();
			void										RemoveLight(CLight* lpl);
			void										SetCurrentAmbientLight(CLight* lpa) {m_lpCurrentAmbientLight = lpa; };
			CLight*										GetCurrentAmbientLight() {return m_lpCurrentAmbientLight; };			

			//Static object ------------------------------------------------------------------------------------------------------------------------------------------------
			virtual	CStaticObj*							vCreateStaticObj(CStaticObj::TYPE type);	
			LYNXINLINE void								AllocateStaticObjs(int num) {m_StaticObjArray.resize(num); };
			LYNXINLINE int								GetNumStaticObjs() {return (DWORD)m_StaticObjArray.size(); };
			LYNXINLINE void								SetlpStaticObj(int i, CStaticObj* const lpo) {m_StaticObjArray[i] = lpo; };
			LYNXINLINE CStaticObj* const				GetlpStaticObj(int i) {return m_StaticObjArray[i]; };		
			CStaticObj* const							GetlpStaticObj(const CString& name);
			CStaticObj* const							GetlpStaticObj(const LYNXCHAR* name);
			void										AddStaticObj(CStaticObj* lpo);			
			void										RemoveStaticObj(CStaticObj* lpo);
			void										RemoveAllStaticObjs();
			void*										GetStaticObjPointer(const LYNXCHAR* name);

			//Dynamic object -----------------------------------------------------------------------------------------------------------------------------------------------
			virtual	CDynamicObj*						vCreateDynamicObj(CDynamicObj::TYPE type);	
			LYNXINLINE void								AllocateDynamicObjs(int num) {m_DynamicObjArray.resize(num); };
			LYNXINLINE int								GetNumDynamicObjs() {return (DWORD)m_DynamicObjArray.size(); };
			LYNXINLINE void								SetlpDynamicObj(int i, CDynamicObj* const lpo) {m_DynamicObjArray[i] = lpo; };
			LYNXINLINE CDynamicObj* const				GetlpDynamicObj(int i) {return m_DynamicObjArray[i]; };		
			CDynamicObj* const							GetlpDynamicObj(const CString& name);
			void										AddDynamicObj(CDynamicObj* lpo);
			void										RemoveAllDynamicObjs();
			void										RemoveDynamicObj(CDynamicObj* lpo);


			LYNXINLINE CList<CLight*>&					GetVisLightList() {return m_VisLightList; };
			LYNXINLINE CList<CRenderableObj*>&			GetVisObjList() {return m_VisObjList; };
			void										ComputeVisList(CCameraContainer& camera);
			void										ComputeVisObjLOD(CCameraContainer& camera);
			void										ComputeVisLightList();		
		protected:
			void										PreLoading();			
			void										PostLoading();			

			friend class SceneSystem;	
		};
	}
}

#endif