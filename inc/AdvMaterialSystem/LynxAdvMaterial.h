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

#ifndef __LYNXADVMATERIAL_H__
#define __LYNXADVMATERIAL_H__

#include <LynxSDKWrap.h>
#include <LynxBaseObj.h>
#include <LynxSystem.h>
#include <LynxParam.h>
#include <AdvMaterialSystem/LynxAdvMaterialPass.h>

namespace LynxEngine 
{	
	class CContainer;
	namespace AdvMaterialSystem 
	{		
		class CAdvMaterialSystem;		
		class LYNXENGCLASS CAdvMaterial : public CBaseObj
		{	
			LYNX_DECLARE_CLASS(CAdvMaterial);
		public:
			LYNXVECTOR3D						m_LightPos;
			LYNXMATRIX							*m_lpLightView, *m_lpLightViewProj;
			LYNXVECTOR3D						m_ViewerPos;			
			LYNXCOLORRGBA						m_Color;
			float								m_Radius;
		protected:		
			CAdvMaterialSystem*					m_lpMaterialSystem;		
			CList<CContainer*>					m_RenderableObjList;
			LYNXBOOL							m_bPlayed;	

			char								m_Name[64];
			CArray<CAdvMaterialPass*>			m_MaterialPassArray;			

			CArray<CParam>						m_ParamArray;
		public:			
			CAdvMaterial();
			CAdvMaterial(DWORD numpass);
			CAdvMaterial(LYNXMATERIAL&);			
			virtual ~CAdvMaterial();							

			CAdvMaterial&							operator =(CAdvMaterial& m);

			void									Clone(LYNXMATERIAL &m);			

			LYNXINLINE const char*					GetName() {return m_Name;};
			LYNXINLINE void							SetName(char *name) {strcpy(m_Name, name);};

			void									CreatePass(DWORD numpass);
			LYNXINLINE const DWORD					GetNumPass() {return m_MaterialPassArray.size();};			
			LYNXINLINE CAdvMaterialPass&			GetMaterialPass(int i) {return (*m_MaterialPassArray[i]); };
			LYNXINLINE void							SetMaterialPass(int i, CAdvMaterialPass* lpmp) {m_MaterialPassArray[i] = lpmp; };

			LYNXINLINE void							SetlpMaterialSystem(CAdvMaterialSystem *lpsys) {m_lpMaterialSystem =  lpsys; };
			LYNXINLINE CAdvMaterialSystem* const	GetlpMaterialSystem() {return m_lpMaterialSystem; };
			virtual	void							vInit();	
			virtual LYNXBOOL						vLoad(const char *name, LPLYNXFILE lpf = NULL, long offset = 0);

			LYNXBOOL								LoadMtl(const char *name, LPLYNXFILE lpf = NULL, long offset = 0);
			LYNXBOOL								LoadShaders(int pass, const char *vsname, const char *psname);
			virtual void							vPlayAll(LYNXREAL skip, LYNXPLAYMODE mode);
			virtual LYNXBOOL						vPlay(int channel, LYNXREAL skip, LYNXPLAYMODE mode);
			virtual void							vRelease(void);						
			
			LYNXINLINE void							AddRenderableObj(CContainer* const lpo) {m_RenderableObjList.push_back(lpo); };
			LYNXINLINE CContainer* const			GetRenderableObj(int i) {return m_RenderableObjList.get(i); };
			LYNXINLINE DWORD						GetNumRenderableObj() {return (DWORD)m_RenderableObjList.size(); };
			LYNXINLINE CList<CContainer*>&			GetRenderableObjListj() {return m_RenderableObjList; };

			LYNXINLINE								SetPlayed(LYNXBOOL b) {m_bPlayed = b;};
            			
			friend class CAdvMaterialSystem;	
		};		
	}		
}

#endif