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

#ifndef __LYNXADVMATERIALSYSTEM_H__
#define __LYNXADVMATERIALSYSTEM_H__

#include <LynxSDK.h>
#include <LynxList.h>
#include <LynxSystem.h>
#include <LynxRenderableObj.h>
#include <AdvMaterialSystem/LynxAdvMaterial.h>

namespace LynxEngine 
{		
	namespace AdvMaterialSystem 
	{		
		class LYNXENGCLASS CAdvMaterialSystem : public CSystem
		{	
			LYNX_DECLARE_CLASS(CAdvMaterialSystem);
		public:			
			//float									m_FogConstant[4];				
		protected:
			LYNXMATRIX								m_WVPM;	/*!< world*view*projection matrix */
			LYNXMATRIX								m_WVM; /*!< world*view matrix */			
			
			CList<CAdvMaterial*>					m_MaterialList;

		public:			
			CAdvMaterialSystem();			
			~CAdvMaterialSystem();				

			LYNXBOOL								Init(void);
			void									Release(void);			
			
			void									SetGlobalParam();	
			
			LYNXINLINE DWORD						GetNumMaterials() {return (DWORD)m_MaterialList.size(); };
			LYNXINLINE CList<CAdvMaterial*>&		GetMaterialList() {return m_MaterialList; };
			LYNXINLINE CAdvMaterial* const 			GetMaterial(int i) {return m_MaterialList.get(i); };
			CAdvMaterial* const 					CAdvMaterialSystem::FindMaterial(const char *name);
			void									AddMaterial(CAdvMaterial *lpmat);
			void									InsertMaterial();
			void									DeleteMaterial();
			void									DumpMaterials() {};			
		};		
	}
}
#endif