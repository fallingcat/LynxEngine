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

#ifndef __LYNXMATERIAL_H__
#define __LYNXMATERIAL_H__

#include <LynxEngineClasses.h>
#include <LynxResource.h>
#include <LynxCreationDesc.h>
#include <GraphicsSystem/LynxVertexLayout.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CMaterialCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CMaterialCreationDesc);	
	public:									
		CString						m_VertexLayoutName;
		LYNXDWORD					m_NumTextureAnimations;
		LYNXDWORD					m_NumTechniques;
		
		CMaterialCreationDesc() {};
	};

	namespace MaterialSystem 
	{		
		class LYNXENGCLASS CMaterial : public CResource
		{	
			LYNX_DECLARE_CLASS(CMaterial);
		public:
			typedef enum {				
				NULL_MATERIAL = -1,				
				STANDARD,
				POSTPROCESSING,
				DISTORTION,
				NUM_TYPE,
			}TYPE;					

			typedef enum {				
				UNLIGHTING = 0,				
				PHONG_LIGHTING,
				CUSTOM_LIGHTING,
				NUM_LIGHTINGTYPE,
			}LIGHTINGTYPE;		

			typedef enum {				
				SIMPLE = 0,
				WIREFRAME,								
				SIMPLE_LIGHTING,				
				AMBIENT_LIGHTING,
				POINT_LIGHTING,
				SPOT_LIGHTING,
				DIRECTIONAL_LIGHTING,
				ZPASS,
				SHADOWMAP,
				VARIANCE_SHADOWMAP,
				SHADOW_TEXTURE,
				GPASS,	
				VERTEX_COLORED,
				LIGHTMAPPED,
				ONE_LIGHTING, //one directional light	 
				TWO_LIGHTING, //one directional and one point light
				DIFFUSEMAP_X_SHADOW,
				NUM_TECHTYPE,
			}TECHTYPE;
			
			DECLAR_PARAM_ENUM(LynxEngine, TRANSPARENTTYPE, m_TransparentType)
			DECLAR_PARAM_URGBA(m_Diffuse)
			DECLAR_PARAM_URGBA(m_Specular)
			DECLAR_PARAM_RANGE_FLOAT(m_SpPower)
			DECLAR_PARAM_FLOAT(m_ReflectionIndex)
			DECLAR_PARAM_FLOAT(m_RefractionIndex)
			DECLAR_PARAM_FLOAT(m_TransmissionRate)		
			DECLAR_PARAM_FLOAT(m_RefractionOffest)
            DECLAR_PARAM_BOOL(m_bPlayInMainLoop)
		protected:					
			DECLAR_PARAM_BOOL(m_bTessellation)
			DECLAR_PARAM_ENUM(LynxEngine::MaterialSystem::CMaterial, LIGHTINGTYPE, m_LightingType)				
			DECLAR_PARAM_ENUM(LynxEngine::MaterialSystem::CMaterial, TYPE,	m_MaterialType)
			CMaterialSystem*					m_lpMaterialSystem;		
			CList<CContainer*>					m_RenderableObjList;
			
			GraphicsSystem::CRenderer::FIXEDVERTEXLAYOUT m_FixedVertexLayout;
			GraphicsSystem::CVertexLayoutPtr	m_VertexLayout;
			DECLAR_PARAM_ARRAY(LynxEngine::MaterialSystem::CTextureAnimationPtr, m_TextureAnimationArray)			
			
			CMap<TECHTYPE, CTechnique*>			m_TechniqueArray;
		public:			
			CMaterial(CMaterialSystem* lpsys);			
			virtual ~CMaterial();							

			//CMaterial&							operator =(const CMaterial& m);
			CMaterial&							operator =(CMaterial& m);
	
			LYNXINLINE void						SetMaterialType(TYPE t) {m_MaterialType = t; };
			LYNXINLINE TYPE						GetMaterialType() const {return m_MaterialType; };

			LYNXINLINE void						SetLightingType(LIGHTINGTYPE t) {m_LightingType = t; };
			LYNXINLINE LIGHTINGTYPE				GetLightingType() const {return m_LightingType; };

			void								Default();

			virtual void						vSetNumTextureAnimation(int num);			
			LYNXINLINE int						GetNumTextureAnimation() {return (DWORD)m_TextureAnimationArray.size(); };
			LYNXINLINE CTextureAnimationPtr&	GetTextureAnimation(int channel) {return m_TextureAnimationArray[channel]; };						
			LYNXINLINE const CArray<CTextureAnimationPtr>& GetTextureAnimationArray() {return m_TextureAnimationArray; };			
			LYNXINLINE void						SetTextureAnimation(int i, CTextureAnimationPtr& lpa) {m_TextureAnimationArray[i] = lpa; };			

			//virtual void						vCreateTechnique(DWORD numptech);
			virtual void						vAddTechnique(TECHTYPE t);
			void								DeleteTechnique(TECHTYPE t);
			LYNXINLINE const DWORD				GetNumTechnique() {return (DWORD)m_TechniqueArray.size();};			
			LYNXINLINE CTechnique&				GetTechnique(TECHTYPE i) {return (*m_TechniqueArray[i]); };
			LYNXINLINE CTechnique*				FindTechnique(TECHTYPE i);
			LYNXINLINE CTechnique&				GetTechnique(int i) 
			{
				CMap<TECHTYPE, CTechnique*>::CIterator Pos = m_TechniqueArray.begin(); 
				advance(Pos, i); 
				return *((*Pos).second);
			}
			CTechnique&							GetTechnique(const CString& name);

			LYNXINLINE void						SetlpMaterialSystem(CMaterialSystem *lpsys) {m_lpMaterialSystem =  lpsys; };
			LYNXINLINE CMaterialSystem* const	GetlpMaterialSystem() {return m_lpMaterialSystem; };

			LYNXBOOL							vSave(CStream& stream);
			LYNXBOOL							vSaveMobileVersion(CStream& stream);
			LYNXBOOL							vLoad(CStream& stream, LYNXBOOL skip = LYNX_FALSE);

			LYNXBOOL							vLoad(CUserProfile* file, CXMLElement* c);
			LYNXBOOL							vSave(CUserProfile* file, CXMLElement* c);

			LYNXBOOL							LoadMtl(const LYNXCHAR *name, LPLYNXFILE lpf = NULL, long offset = 0);
			virtual void						vPlayAll(LYNXREAL skip);
			virtual LYNXBOOL					vPlay(int channel, LYNXREAL skip);

			LYNXINLINE GraphicsSystem::CRenderer::FIXEDVERTEXLAYOUT& GetFixedVertexLayout() {return m_FixedVertexLayout;};
			LYNXINLINE void						SetFixeVertexLayout(GraphicsSystem::CRenderer::FIXEDVERTEXLAYOUT fs) {m_FixedVertexLayout = fs;};
			LYNXINLINE GraphicsSystem::CVertexLayoutPtr& GetVertexLayout() {return m_VertexLayout; };
			LYNXINLINE void						SetVertexLayout(GraphicsSystem::CVertexLayoutPtr& vl) {m_VertexLayout = vl; };
			void								CreateVertexLayout(CString& name);
			void								CreateVertexLayout(const LYNXCHAR* name);
						
			LYNXINLINE void						AddRenderableObj(CContainer* const lpo) {m_RenderableObjList.push_back(lpo); };
			LYNXINLINE CContainer* const		GetRenderableObj(int i) {return m_RenderableObjList.get(i); };
			LYNXINLINE DWORD					GetNumRenderableObj() {return (DWORD)m_RenderableObjList.size(); };
			LYNXINLINE CList<CContainer*>&		GetRenderableObjList() {return m_RenderableObjList; };

			void								Sync();
			void								RebindResources();
			void								SetDirty(LYNXBOOL b);
		protected:
			void								Release();

			friend class CMaterialSystem;	
		};				
	}			
	LYNXENGCLASS CString GetEnumStr(MaterialSystem::CMaterial::TYPE e);
	LYNXENGCLASS CString GetEnumStr(MaterialSystem::CMaterial::LIGHTINGTYPE e);
	LYNXENGCLASS CString GetEnumStr(MaterialSystem::CMaterial::TECHTYPE e);

	LYNXENGCLASS MaterialSystem::CMaterial::TYPE TranslateMaterialType(const CString& s);
	LYNXENGCLASS MaterialSystem::CMaterial::LIGHTINGTYPE TranslateLightingType(const CString& s);
	LYNXENGCLASS MaterialSystem::CMaterial::TECHTYPE TranslateTechType(const CString& s);
}

#endif