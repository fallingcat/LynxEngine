//##########################################################################
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

#ifndef __LYNXMATERIALSYSTEM_H__
#define __LYNXMATERIALSYSTEM_H__

#include <LynxList.h>
#include <LynxCountedPtr.h>
#include <LynxSystem.h>
#include <LynxRenderableObj.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{		
	namespace MaterialSystem 
	{		
		class LYNXENGCLASS CMaterialSystem : public CSystem
		{	
			LYNX_DECLARE_CLASS(CMaterialSystem);
		public:	
			static CMaterialPtr						m_NULL_Material;
			//static const CAnsiString				m_MaterialMapString[NUM_MATERIALMAP];			
		protected:					
			CList<CMaterialPtr>						m_MaterialList;
			SceneSystem::CAmbientLight*				m_lpCurrentAmbientLight;
			SceneSystem::CLight*					m_lpCurrentLight;			
			CPass*									m_lpCurrentMaterialPass;
		public:			
			CMaterialSystem(CEngine* const lpengine);				
			virtual ~CMaterialSystem();				

			LYNXBOOL								vCreate();
			LYNXBOOL								vInit();

			LYNXINLINE void							SetlpCurrentAmbientLight(SceneSystem::CAmbientLight* lplit) {m_lpCurrentAmbientLight = lplit; };
			LYNXINLINE SceneSystem::CAmbientLight*	GetlpCurrentAmbientLight() {return m_lpCurrentAmbientLight; };
			
			LYNXINLINE void							SetlpCurrentLight(SceneSystem::CLight* lplit) {m_lpCurrentLight = lplit; };
			LYNXINLINE SceneSystem::CLight*			GetlpCurrentLight() {return m_lpCurrentLight; };

			LYNXINLINE void							SetCurrentMaterialPass(CPass* p) {m_lpCurrentMaterialPass = p; };
			LYNXINLINE CPass*						GetCurrentMaterialPass() {return m_lpCurrentMaterialPass; };

			LYNXBOOL								CreateMaterial(CMaterialPtr& m, CMaterial::TYPE type);
			LYNXBOOL								CreateMaterial(CMaterialPtr& m, CMaterial::TYPE type, const CString& name);
			LYNXINLINE DWORD						GetNumMaterials() {return (DWORD)m_MaterialList.size(); };

			static const CAnsiString				GetMapString(const MATERIALMAP map);
			LYNXINLINE CList<CMaterialPtr>&			GetMaterialList() {return m_MaterialList; };
			LYNXINLINE CMaterialPtr&				GetMaterial(int i) {return m_MaterialList.get(i); };
			void									AddMaterial(const CMaterialPtr& lpmat);
			void									AddMaterial(CResourcePtr& lpmat);		
			CMaterialPtr&							FindMaterial(const LYNXCHAR *name);
			CMaterialPtr&  							FindMaterial(const CString& name);			
			void									DeleteMaterial(CMaterialPtr& lpmat);
			void									DeleteMaterial(const CString& name);
			void									DeleteAllMaterials();
			void									InsertMaterial();
			void									DeleteMaterial();
			void									DumpMaterials() {};			
		};		
	}
	LYNXENGCLASS CString GetEnumStr(MaterialSystem::MATERIALMAP e);		
	LYNXINLINE bool operator == (CWeakPtr<MaterialSystem::CMaterialPtr>& s, const CString& name) {return ((s.IsValid()) ? (((*s)->GetName()) == name) : false); };
}
#endif