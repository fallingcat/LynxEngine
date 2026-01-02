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

#ifndef __LYNXMATERIALTECHNIQUE_H__
#define __LYNXMATERIALTECHNIQUE_H__

#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <LynxObj.h>
#include <LynxArray.h>
#include <LynxParam.h>

#include <GraphicsSystem/LynxRenderer.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	namespace MaterialSystem 
	{		
		class LYNXENGCLASS CTechnique : public CObj
		{	
			LYNX_DECLARE_CLASS(CTechnique);
		public:			
			typedef enum {				
				NULL_TECH = -1,
				LIGHTING,
				ZPASSING,
				SHADOWMAPPING,
				GPASSING,
				NUM_TYPE,
			}TYPE;			

		protected:		
			CMaterial*							m_lpMaterial;	

			CMaterial::TECHTYPE					m_Type;			
			CArray<CPass*>						m_PassArray;			
		public:			
			CTechnique(CMaterial* lpmat);
			virtual ~CTechnique();							

			CTechnique&							operator =(const CTechnique& m);
			
			LYNXINLINE void						SetType(CMaterial::TECHTYPE t) {m_Type = t; };
			LYNXINLINE CMaterial::TECHTYPE		GetType() {return m_Type; };

			virtual void						vCreatePass(DWORD numpass);
			virtual void						vAddPass(int num);
			LYNXINLINE const DWORD				GetNumPass() {return (const DWORD)m_PassArray.size();};			
			LYNXINLINE CPass&					GetPass(int i) {return (*m_PassArray[i]); };

			LYNXINLINE void						SetlpMaterial(CMaterial* lpmat) {m_lpMaterial = lpmat; };
			LYNXINLINE CMaterial* const			GetlpMaterial() {return m_lpMaterial; };

			LYNXBOOL							vSave(CStream& stream);
			LYNXBOOL							vLoad(CStream& stream, LYNXBOOL skip = LYNX_FALSE);

			LYNXBOOL							LoadMtl(const LYNXCHAR *name, LPLYNXFILE lpf = NULL, long offset = 0);
			virtual void						vPlayAll(LYNXREAL skip);
			virtual LYNXBOOL					vPlay(int channel, LYNXREAL skip);
			virtual void						vRelease(void);	

			LYNXBOOL							vLoad(CUserProfile* file, CXMLElement* c);
			LYNXBOOL							vSave(CUserProfile* file, CXMLElement* c);

			friend class CMaterial;
		};				
	}				
	LYNXENGCLASS bool operator ==(const CPair<MaterialSystem::CMaterial::TECHTYPE, MaterialSystem::CTechnique*>& t, const CString& name);
}

#endif