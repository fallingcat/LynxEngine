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

#ifndef __LYNXMATERIALPHASE_H__
#define __LYNXMATERIALPHASE_H__

#include <LynxSDK/LynxRenderer.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	class CContainer;	
	namespace MaterialSystem 
	{		
		class CMaterialSystem;
		class CPass;	
		class CMaterial;
		class LYNXENGCLASS CPhase : public CObj
		{	
			LYNX_DECLARE_CLASS(CPhase);
		public:
			typedef enum {				
				NULL_PHASE = -1,
				LIGHTING,
				ZPASS,
				SHADOWMAPPING,
				NUM_MATERIAL_TYPE,
			}TYPE;

			LYNXVECTOR3D						m_LightPos;
			LYNXMATRIX							*m_lpLightView, *m_lpLightViewProj;
			LYNXVECTOR3D						m_ViewerPos;			
			LYNXCOLORRGBA						m_Color;
			float								m_Radius;			
		protected:		
			CMaterial*							m_lpMaterial;	

			TYPE								m_Type;
			LYNXCHAR							m_Name[64];
			CArray<CPass*>						m_PassArray;			
		public:			
			CPhase();
			CPhase(CMaterial* lpmat, DWORD numpass = 1);
			virtual ~CPhase();							

			CPhase&								operator =(const CPhase& m);

			LYNXINLINE const LYNXCHAR*			GetName() {return m_Name;};
			LYNXINLINE void						SetName(LYNXCHAR *name) {lynxStrCpy(m_Name, name);};

			LYNXINLINE void						SetType(TYPE t) {m_Type = t; };
			LYNXINLINE TYPE						GetType() {return m_Type; };

			void								CreatePass(DWORD numpass);
			LYNXINLINE const DWORD				GetNumPass() {return m_PassArray.size();};			
			LYNXINLINE CPass&					GetPass(int i) {return (*m_PassArray[i]); };
			LYNXINLINE void						SetPass(int i, CPass* lpmp) {m_PassArray[i] = lpmp; };

			LYNXINLINE void						SetlpMaterial(CMaterial* lpmat) {m_lpMaterial = lpmat; };
			LYNXINLINE CMaterial* const			GetlpMaterial() {return m_lpMaterial; };
			virtual LYNXBOOL					vLoad(const LYNXCHAR *name, LPLYNXFILE lpf = NULL, long offset = 0);

			LYNXBOOL							LoadMtl(const LYNXCHAR *name, LPLYNXFILE lpf = NULL, long offset = 0);
			LYNXBOOL							LoadShaders(int pass, const LYNXCHAR *vsname, const LYNXCHAR *psname);
			virtual void						vPlayAll(LYNXREAL skip, LYNXPLAYMODE mode);
			virtual LYNXBOOL					vPlay(int channel, LYNXREAL skip, LYNXPLAYMODE mode);
			virtual void						vRelease(void);	

			friend class CMaterial;
		};		
	}		
}

#endif