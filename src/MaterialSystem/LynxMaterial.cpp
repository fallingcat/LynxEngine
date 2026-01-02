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
#include <LynxLogger.h>
#include <Animation/LynxAnimationKeys.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <MaterialSystem/LynxTextureAnimation.h>

namespace LynxEngine 
{		
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(MaterialSystem::CMaterial::TYPE e)
	{
		CString				MatType;

		switch (e)
		{
			case MaterialSystem::CMaterial::STANDARD:
				MatType = _T("Standard");
				break;
			case MaterialSystem::CMaterial::POSTPROCESSING:
				MatType = _T("Post Processing");
				break;
			case MaterialSystem::CMaterial::DISTORTION:
				MatType = _T("Distortion");
				break;
			default:
				MatType = _T("Unknown");
				break;
		}
		return MatType;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(MaterialSystem::CMaterial::LIGHTINGTYPE e)
	{
		CString				LightingType;

		switch (e)
		{
			case MaterialSystem::CMaterial::UNLIGHTING:
				LightingType = _T("Unlighting");
				break;
			case MaterialSystem::CMaterial::PHONG_LIGHTING:
				LightingType = _T("Phong Lighting");
				break;
			case MaterialSystem::CMaterial::CUSTOM_LIGHTING:
				LightingType = _T("Custom Lighting");
				break;
			default:
				LightingType = _T("Unknown");
				break;
		}
		return LightingType;
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(MaterialSystem::CMaterial::TECHTYPE e)
	{
		CString				Tech;

		switch (e)
		{
			case MaterialSystem::CMaterial::SIMPLE:
				Tech = _T("Simple");
				break;
			case MaterialSystem::CMaterial::WIREFRAME:
				Tech = _T("Wireframe");
				break;
			case MaterialSystem::CMaterial::SIMPLE_LIGHTING:
				Tech = _T("SimpleLighting");
				break;
			case MaterialSystem::CMaterial::AMBIENT_LIGHTING:
				Tech = _T("AmbientLighting");
				break;
			case MaterialSystem::CMaterial::POINT_LIGHTING:
				Tech = _T("PointLighting");
				break;
			case MaterialSystem::CMaterial::SPOT_LIGHTING:
				Tech = _T("SpotLighting");
				break;
			case MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING:
				Tech = _T("DirectionalLighting");
				break;
			case MaterialSystem::CMaterial::ZPASS:
				Tech = _T("ZPass");
				break;
			case MaterialSystem::CMaterial::SHADOWMAP:
				Tech = _T("Shadowmap");
				break;
			case MaterialSystem::CMaterial::VARIANCE_SHADOWMAP:
				Tech = _T("VarianceShadowmap");
				break;
			case MaterialSystem::CMaterial::GPASS:
				Tech = _T("GPass");
				break;
			case MaterialSystem::CMaterial::VERTEX_COLORED:
				Tech = _T("VertexColored");
				break;
			case MaterialSystem::CMaterial::LIGHTMAPPED:
				Tech = _T("Lightmapped");
				break;
			case MaterialSystem::CMaterial::ONE_LIGHTING:
				Tech = _T("OneLighting");
				break;
		}
		return Tech;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	MaterialSystem::CMaterial::TYPE TranslateMaterialType(const CString& s)
	{
		for (int i = 0; i < MaterialSystem::CMaterial::NUM_TYPE; ++i)
		{
			if (LynxEngine::GetEnumStr((MaterialSystem::CMaterial::TYPE)i) == s)
				return (MaterialSystem::CMaterial::TYPE)i;
		}
		return MaterialSystem::CMaterial::STANDARD;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	MaterialSystem::CMaterial::LIGHTINGTYPE TranslateLightingType(const CString& s)
	{
		for (int i = 0; i < MaterialSystem::CMaterial::NUM_LIGHTINGTYPE; ++i)
		{
			if (LynxEngine::GetEnumStr((MaterialSystem::CMaterial::LIGHTINGTYPE)i) == s)
				return (MaterialSystem::CMaterial::LIGHTINGTYPE)i;
		}
		return MaterialSystem::CMaterial::PHONG_LIGHTING;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	MaterialSystem::CMaterial::TECHTYPE TranslateTechType(const CString& s)
	{
		for (int i = 0; i < MaterialSystem::CMaterial::NUM_TECHTYPE; ++i)
		{
			if (LynxEngine::GetEnumStr((MaterialSystem::CMaterial::TECHTYPE)i) == s)
				return (MaterialSystem::CMaterial::TECHTYPE)i;
		}
		return MaterialSystem::CMaterial::SIMPLE;
	}

	namespace MaterialSystem 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMaterial::CMaterial(CMaterialSystem* lpsys)
		: CResource(lpsys->GetlpEngine())
		{	
			m_MaterialType = STANDARD;
			m_LightingType = PHONG_LIGHTING;
			m_TransparentType = TT_OPAQUE;
			m_ResourceType = MATERIAL;
			m_lpMaterialSystem = lpsys;			
			m_FixedVertexLayout = 0;			
			m_bTessellation = LYNX_FALSE;
            m_bPlayInMainLoop = LYNX_FALSE;
			m_TextureAnimationArray.clear();
			m_TechniqueArray.clear();
			m_RenderableObjList.clear();

			m_Diffuse.PackedColor = 0xffffffff;
			m_Specular.PackedColor = 0xffffffff;
			m_SpPower = 16.0f;
			m_ReflectionIndex = 0.6f;
			m_RefractionIndex = 0.6f;
			m_TransmissionRate = 0.7f;
			m_RefractionOffest = 0.1f;

			m_Diffuse.PackedColor = m_Specular.PackedColor = 0xffffffff;
			m_SpPower = LYNX_REAL(4.0f);
			
			//CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_STATIC));

			IMPLEMENT_PARAM_ENUM(m_MaterialType, 0)
			IMPLEMENT_PARAM_ENUM(m_LightingType, 0)
			IMPLEMENT_PARAM_ENUM(m_TransparentType, 0)
			IMPLEMENT_PARAM_URGBA(m_Diffuse, 0)
			IMPLEMENT_PARAM_URGBA(m_Specular, 0)
			IMPLEMENT_PARAM_RANGE_FLOAT(m_SpPower, 1.0, 128.0, 1000, 0)			
			IMPLEMENT_PARAM_FLOAT(m_ReflectionIndex, 0)
			IMPLEMENT_PARAM_FLOAT(m_RefractionIndex, 0)
			IMPLEMENT_PARAM_FLOAT(m_TransmissionRate, 0)
			IMPLEMENT_PARAM_FLOAT(m_RefractionOffest, 0)			
			IMPLEMENT_PARAM_ARRAY(m_TextureAnimationArray, CParam::COUNTEDPTR, 0)		

			IMPLEMENT_PARAM_BOOL(m_bTessellation, 0)
            IMPLEMENT_PARAM_BOOL(m_bPlayInMainLoop, 0)
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMaterial::~CMaterial(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
			
			Release();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterial::Release(void)
		{
			for (int i=0; i<m_TextureAnimationArray.size(); i++)
				m_TextureAnimationArray[i].Reset();
			m_TextureAnimationArray.clear();	

			CMap<TECHTYPE, CTechnique*>::CIterator Pos;
			for (Pos = m_TechniqueArray.begin(); Pos != m_TechniqueArray.end(); ++Pos)
			{
				if ( (*Pos).second )
					LYNXDEL (*Pos).second;
			}			
			m_TechniqueArray.clear();		
			m_RenderableObjList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMaterial& CMaterial::operator =(CMaterial& m)
		{
            m_TransparentType = m.m_TransparentType;
            m_Diffuse = m.m_Diffuse;
            m_Specular = m.m_Specular;
            m_SpPower = m.m_SpPower;
            m_RefractionIndex = m.m_RefractionIndex;
            m_TransmissionRate = m.m_TransmissionRate;
            m_RefractionOffest = m.m_RefractionOffest;
            m_bPlayInMainLoop = m.m_bPlayInMainLoop;
			m_lpMaterialSystem = m.m_lpMaterialSystem;
			m_RenderableObjList = m.m_RenderableObjList;
			m_FixedVertexLayout = m.m_FixedVertexLayout;
			m_VertexLayout = m.m_VertexLayout;
			for (int i=0; i<m_TextureAnimationArray.size(); i++)
				m_TextureAnimationArray[i].Reset();
			m_TextureAnimationArray.clear();	

			m_TextureAnimationArray.resize(m.m_TextureAnimationArray.size());						
			for (int i=0; i<m_TextureAnimationArray.size(); i++)
			{
				m_TextureAnimationArray[i] = m.m_TextureAnimationArray[i];
			}

			CMap<TECHTYPE, CTechnique*>::CIterator Pos;
			for (Pos = m_TechniqueArray.begin(); Pos != m_TechniqueArray.end(); ++Pos)
			{
				if ( (*Pos).second )
					LYNXDEL (*Pos).second;
			}
			
			for (Pos = m.m_TechniqueArray.begin(); Pos != m.m_TechniqueArray.end(); ++Pos)
			{
				if ( (*Pos).second )
				{
					m_TechniqueArray[(*Pos).second->m_Type] = LYNXNEW CTechnique(this);
					*(m_TechniqueArray[(*Pos).second->m_Type]) = *((*Pos).second);
				}
			}	
		
			return *this;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterial::CreateVertexLayout(CString& name)
		{
			m_lpEngine->CreateVertexLayout(m_VertexLayout, name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterial::CreateVertexLayout(const LYNXCHAR* name)
		{
			CString Name(name);
			m_lpEngine->CreateVertexLayout(m_VertexLayout, Name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterial::vSetNumTextureAnimation(int num)
		{
			CString Name;
			LYNXCHAR Sub[256];

			for (int i=0; i<m_TextureAnimationArray.size(); i++)
				m_TextureAnimationArray[i].Reset();
			m_TextureAnimationArray.clear();	

			m_TextureAnimationArray.resize(num);
			for (int i=0; i<num; i++)
			{
				lynxSprintf(Sub, _T(".TexAni.%d"), i);
				Name = GetName()+CString(Sub);
				GetlpEngine()->CreateResource(m_TextureAnimationArray[i], TEXTURE_ANIMATION);
				m_TextureAnimationArray[i]->SetName(Name);
				m_TextureAnimationArray[i]->vCreateAnimation();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CMaterial::Default()
		{
			CPlatformFileStream Filestream;
			MaterialSystem::CTechnique* Technique = NULL;		
			LynxEngine::MaterialSystem::CPass* Pass = NULL;
			GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;
			GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
			GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
			GraphicsSystem::CRenderer::RASTERIZERSTATEDESC RDesc;
			
			switch (m_MaterialType)
			{
				case MaterialSystem::CMaterial::STANDARD:
					CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_STATIC));
					vSetNumTextureAnimation(3);
					m_TextureAnimationArray[0]->Create(1);								
					m_TextureAnimationArray[0]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
					TexDesc.Default();
					TexDesc.bSRGB = LYNX_TRUE;
					m_TextureAnimationArray[0]->LoadTexture(0, Filestream, _T("checker.png"), &TexDesc);							
					m_TextureAnimationArray[0]->GetlpAnimation()->m_StartTime = 0.0f;
					m_TextureAnimationArray[0]->GetlpAnimation()->m_EndTime = 0.0f;
					m_TextureAnimationArray[0]->GetlpAnimation()->m_Step = 0.0f;

					m_TextureAnimationArray[1]->Create(1);								
					m_TextureAnimationArray[1]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());
					m_TextureAnimationArray[1]->LoadTexture(0, Filestream, _T("checker_NM.png"));							
					m_TextureAnimationArray[1]->GetlpAnimation()->m_StartTime = 0.0f;
					m_TextureAnimationArray[1]->GetlpAnimation()->m_EndTime = 0.0f;
					m_TextureAnimationArray[1]->GetlpAnimation()->m_Step = 0.0f;

					m_TextureAnimationArray[2]->Create(1);								
					m_TextureAnimationArray[2]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SPECULARMAP).c_str());
					m_TextureAnimationArray[2]->LoadTexture(0, Filestream, _T("white.png"));							
					m_TextureAnimationArray[2]->GetlpAnimation()->m_StartTime = 0.0f;
					m_TextureAnimationArray[2]->GetlpAnimation()->m_EndTime = 0.0f;
					m_TextureAnimationArray[2]->GetlpAnimation()->m_Step = 0.0f;

					vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
					m_TechniqueArray[MaterialSystem::CMaterial::SIMPLE]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::SIMPLE]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::WIREFRAME);
					m_TechniqueArray[MaterialSystem::CMaterial::WIREFRAME]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::WIREFRAME]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::ZPASS);
					m_TechniqueArray[MaterialSystem::CMaterial::ZPASS]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::ZPASS]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::GPASS);
					m_TechniqueArray[MaterialSystem::CMaterial::GPASS]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::GPASS]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::SHADOWMAP);
					m_TechniqueArray[MaterialSystem::CMaterial::SHADOWMAP]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::SHADOWMAP]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::VERTEX_COLORED);
					m_TechniqueArray[MaterialSystem::CMaterial::VERTEX_COLORED]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::VERTEX_COLORED]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);
					m_TechniqueArray[MaterialSystem::CMaterial::AMBIENT_LIGHTING]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::AMBIENT_LIGHTING]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);
					m_TechniqueArray[MaterialSystem::CMaterial::SPOT_LIGHTING]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::SPOT_LIGHTING]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);
					m_TechniqueArray[MaterialSystem::CMaterial::POINT_LIGHTING]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::POINT_LIGHTING]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);
					m_TechniqueArray[MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();

					vAddTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);
					m_TechniqueArray[MaterialSystem::CMaterial::SIMPLE_LIGHTING]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::SIMPLE_LIGHTING]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();
					
					/*
					vCreateTechnique(MaterialSystem::CMaterial::NUM_TECHTYPE);		
					for (int t=0; t<GetNumTechnique(); t++)
					{
						m_TechniqueArray[(TECHTYPE)t]->vCreatePass(1);
						m_TechniqueArray[(TECHTYPE)t]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();
					}
					*/								

					DDesc.Default();
					BDesc.Default();
					RDesc.Default();

					Technique = &GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
					//Technique->GetPass(0).vLoadShaders(_T("scn/geometry.vso"), _T("common/lynxDiffuseMapXColor.pso"));
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);			
					
					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);				

					RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique->GetPass(0).m_bUseFog = LYNX_FALSE;
					
					Technique = &GetTechnique(LynxEngine::MaterialSystem::CMaterial::SHADOWMAP);
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);					
							
					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);	

					RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique = &GetTechnique(LynxEngine::MaterialSystem::CMaterial::WIREFRAME);
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);					
							
					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);	

					RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_WIREFRAME;
					RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);


					Technique = &GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::AMBIENT_LIGHTING);
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);					
					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);		
					RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);

					Technique = &GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SPOT_LIGHTING);
					DDesc.DepthMode = GraphicsSystem::CRenderer::DB_C;
					DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
					Technique->GetPass(0).CreateDepthStencilState(DDesc);					
					BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
					BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE;
					BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
					Technique->GetPass(0).CreateBlendState(BDesc);		
					RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
					RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
					Technique->GetPass(0).CreateRasterizerState(RDesc);
					GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("GeometryVS", "DiffuseMapXColorPS");						
					GetTechnique(MaterialSystem::CMaterial::WIREFRAME).GetPass(0).vCreateShaderClasses("GeometryVS", "ConstantColorPS");	
					GetTechnique(MaterialSystem::CMaterial::ZPASS).GetPass(0).vCreateShaderClasses("GeometryZPassVS", "ZPass_Velocity_AlphaTestPS");//CZPass_VelocityPS"));	
					GetTechnique(MaterialSystem::CMaterial::GPASS).GetPass(0).vCreateShaderClasses("GeometryGPassVS", "GPassPS");					
					GetTechnique(MaterialSystem::CMaterial::SHADOWMAP).GetPass(0).vCreateShaderClasses("GeometryShadowMapVS", "ShadowMap_AlphaTestPS");//CShadowMapPS"));			

					GetTechnique(MaterialSystem::CMaterial::VERTEX_COLORED).GetPass(0).vCreateShaderClasses("GeometryDiffuseVS", "DiffuseMapXDiffuseColorPS");	

					GetTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING).GetPass(0).vCreateShaderClasses("GeometryVS", "AmbientLightingPS");					
					GetTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING).GetPass(0).vCreateShaderClasses("GeometryLightingVS", "SpotLightingPS");
					GetTechnique(MaterialSystem::CMaterial::POINT_LIGHTING).GetPass(0).vCreateShaderClasses("GeometryLightingVS", "PointLightingPS");
					GetTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING).GetPass(0).vCreateShaderClasses("GeometryLightingVS", "DirectionalLightingPS");
					GetTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING).GetPass(0).vCreateShaderClasses("GeometrySimpleLightingVS", "SimpleLightingPS");
					break;
				
				case MaterialSystem::CMaterial::POSTPROCESSING:
					break;

				case MaterialSystem::CMaterial::DISTORTION:
					CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_STATIC));
					/*
					vCreateTechnique(1);		
					m_TechniqueArray[(TECHTYPE)0]->vCreatePass(1);
					m_TechniqueArray[(TECHTYPE)0]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();		
					*/
					vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
					m_TechniqueArray[MaterialSystem::CMaterial::SIMPLE]->vCreatePass(1);
					m_TechniqueArray[MaterialSystem::CMaterial::SIMPLE]->m_PassArray[0]->m_TextureAnimationArray = GetTextureAnimationArray();
					m_TechniqueArray[MaterialSystem::CMaterial::SIMPLE]->GetPass(0).vCreateShaderClasses("GeometryLightingVS", "DistortionPS");	
					break;
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CMaterial::vSave(CStream& stream)
		{
			LYNXUNICHAR Name[128], TextureName[256];

			lynxUniStr(Name, GetName().c_str());		
			stream.vWrite(Name, sizeof(LYNXUNICHAR), 64);

			stream.vWrite(&m_Diffuse, sizeof(LYNXCOLORRGBA), 1);
			stream.vWrite(&m_Specular, sizeof(LYNXCOLORRGBA), 1);
			stream.vWrite(&m_SpPower, sizeof(LYNXREAL), 1);
			lynxUniStr(Name, GetVertexLayout()->GetName().c_str());
			stream.vWrite(Name, sizeof(LYNXUNICHAR), 64);

			int NumTAs;
			NumTAs = GetNumTextureAnimation();
			stream.vWrite(&NumTAs, sizeof(int), 1);
			for (int t=0; t<NumTAs; t++)
			{
				LynxEngine::MaterialSystem::CTextureAnimation& TA = *GetTextureAnimation(t);
				stream.vWrite(TA.GetShaderConstantName().c_str(), sizeof(char), 64);					
				int NumTexs;
				int Channel;
				NumTexs = TA.GetNumTextures();
				stream.vWrite(&NumTexs, sizeof(int), 1);
				for (int j=0; j<NumTexs; j++)
				{				
					lynxUniStr(TextureName, TA.GetTexture(j)->GetName().c_str());
					stream.vWrite(TextureName, sizeof(LYNXUNICHAR), 256);
					stream.vWrite(&TA.GetTexture(j)->m_MipMapLevel, sizeof(int), 1);				
					stream.vWrite(&TA.GetTexture(j)->m_LODBias, sizeof(int), 1);			
					stream.vWrite(&TA.GetTexture(j)->m_bSRGB, sizeof(LYNXBOOL), 1);
					//stream.vWrite(&TA.GetTexture(j)->m_AlphaReference, sizeof(float), 1);
					//write sampler state data
					TA.GetTexture(j)->GetSamplerState()->vSave(stream);
					
					stream.vWrite(&TA.GetlpAnimation()->m_StartTime, sizeof(float), 1);
					stream.vWrite(&TA.GetlpAnimation()->m_EndTime, sizeof(float), 1);
					stream.vWrite(&TA.GetlpAnimation()->m_Step, sizeof(float), 1);
					stream.vWrite(&TA.GetlpAnimation()->m_PlayMode, sizeof(LynxEngine::PLAYMODE), 1);
					
					// write position keys
					LYNXBOOL bKeys;
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_POSITION])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_POSITION]->vSave(stream);
					}															
					// write rotation keys
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_ROTATION])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_ROTATION]->vSave(stream);
					}
					// write scale keys
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_SCALE])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_SCALE]->vSave(stream);
					}
					// write color keys
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_COLOR])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_COLOR]->vSave(stream);
					}
					// write texture keys
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_TEXTURE])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_TEXTURE]->vSave(stream);
					}							
				}				
			}

			DWORD NumTech = GetNumTechnique();
			stream.vWrite(&NumTech, sizeof(DWORD), 1);
			for (int t=0; t<GetNumTechnique(); t++)
			{
				GetTechnique(t).vSave(stream);				
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CMaterial::vSaveMobileVersion(CStream& stream)
		{
			LYNXUNICHAR Name[128], TextureName[256];

			lynxUniStr(Name, GetName().c_str());		
			stream.vWrite(Name, sizeof(LYNXUNICHAR), 64);

			stream.vWrite(&m_Diffuse, sizeof(LYNXCOLORRGBA), 1);
			stream.vWrite(&m_Specular, sizeof(LYNXCOLORRGBA), 1);
			stream.vWrite(&m_SpPower, sizeof(LYNXREAL), 1);
			lynxUniStr(Name, GetVertexLayout()->GetName().c_str());
			stream.vWrite(Name, sizeof(LYNXUNICHAR), 64);

			int NumTAs;
			NumTAs = GetNumTextureAnimation();
			stream.vWrite(&NumTAs, sizeof(int), 1);
			for (int t=0; t<NumTAs; t++)
			{
				LynxEngine::MaterialSystem::CTextureAnimation& TA = *GetTextureAnimation(t);
				stream.vWrite(TA.GetShaderConstantName().c_str(), sizeof(char), 64);					
				int NumTexs;
				int Channel;
				NumTexs = TA.GetNumTextures();
				stream.vWrite(&NumTexs, sizeof(int), 1);
				for (int j=0; j<NumTexs; j++)
				{				
					lynxUniStr(TextureName, TA.GetTexture(j)->GetName().c_str());
					stream.vWrite(TextureName, sizeof(LYNXUNICHAR), 256);
					stream.vWrite(&TA.GetTexture(j)->m_MipMapLevel, sizeof(int), 1);				
					stream.vWrite(&TA.GetTexture(j)->m_LODBias, sizeof(int), 1);			
					stream.vWrite(&TA.GetTexture(j)->m_bSRGB, sizeof(LYNXBOOL), 1);
					//stream.vWrite(&TA.GetTexture(j)->m_AlphaReference, sizeof(float), 1);
					//write sampler state data
					TA.GetTexture(j)->GetSamplerState()->vSave(stream);
					
					stream.vWrite(&TA.GetlpAnimation()->m_StartTime, sizeof(float), 1);
					stream.vWrite(&TA.GetlpAnimation()->m_EndTime, sizeof(float), 1);
					stream.vWrite(&TA.GetlpAnimation()->m_Step, sizeof(float), 1);
					stream.vWrite(&TA.GetlpAnimation()->m_PlayMode, sizeof(LynxEngine::PLAYMODE), 1);
					
					// write position keys
					LYNXBOOL bKeys;
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_POSITION])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_POSITION]->vSave(stream);
					}															
					// write rotation keys
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_ROTATION])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_ROTATION]->vSave(stream);
					}
					// write scale keys
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_SCALE])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_SCALE]->vSave(stream);
					}
					// write color keys
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_COLOR])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_COLOR]->vSave(stream);
					}
					// write texture keys
					if (!TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_TEXTURE])
					{
						bKeys = LYNX_FALSE;						
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
					}
					else
					{
						bKeys = LYNX_TRUE;
						stream.vWrite(&bKeys, sizeof(LYNXBOOL), 1);
						TA.GetlpAnimation()->m_AnimationKeys[LynxEngine::Animation::CAnimation::KT_TEXTURE]->vSave(stream);
					}							
				}				
			}

			DWORD NumTech = 0;
			for (int t=0; t<GetNumTechnique(); t++)
			{
				if (GetTechnique(t).GetType() != CMaterial::AMBIENT_LIGHTING &&				
					GetTechnique(t).GetType() != CMaterial::POINT_LIGHTING && 
					GetTechnique(t).GetType() != CMaterial::SPOT_LIGHTING && 
					GetTechnique(t).GetType() != CMaterial::DIRECTIONAL_LIGHTING && 
					GetTechnique(t).GetType() != CMaterial::ZPASS && 
					GetTechnique(t).GetType() != CMaterial::VARIANCE_SHADOWMAP && 
					GetTechnique(t).GetType() != CMaterial::GPASS)
				{
					NumTech++;
				}
			}

			//DWORD NumTech = GetNumTechnique();
			stream.vWrite(&NumTech, sizeof(DWORD), 1);
			for (int t=0; t<GetNumTechnique(); t++)
			{
				if (GetTechnique(t).GetType() != CMaterial::AMBIENT_LIGHTING &&				
					GetTechnique(t).GetType() != CMaterial::POINT_LIGHTING && 
					GetTechnique(t).GetType() != CMaterial::SPOT_LIGHTING && 
					GetTechnique(t).GetType() != CMaterial::DIRECTIONAL_LIGHTING && 
					GetTechnique(t).GetType() != CMaterial::ZPASS && 
					GetTechnique(t).GetType() != CMaterial::VARIANCE_SHADOWMAP && 
					GetTechnique(t).GetType() != CMaterial::GPASS)
				{
					GetTechnique(t).vSave(stream);				
				}
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CMaterial::vLoad(CStream& stream, LYNXBOOL bskip)
		{			
			LYNXUNICHAR		UniStr[256];
			LYNXCHAR		CurrentPath[128];
			LYNXCHAR		AnsiName[256];	
			LYNXCHAR		MapName[256];
			LYNXANSICHAR	ShaderConstantName[64];
			LYNXCHAR		AddMapName[256];	
			DWORD			NumTech;	
			GraphicsSystem::CRenderer::SAMPLERSTATEDESC			SamplerStateDesc;			

			SamplerStateDesc.Default();

			if (bskip)
			{
				stream.vSeek(sizeof(LYNXCOLORRGBA), CStream::SeekCur);
				stream.vSeek(sizeof(LYNXCOLORRGBA), CStream::SeekCur);
				stream.vSeek(sizeof(LYNXREAL), CStream::SeekCur);
				stream.vSeek(sizeof(LYNXUNICHAR)*64, CStream::SeekCur);			

				int NumTexs;
				int NumTAs;
				stream.vRead(&NumTAs, sizeof(int), 1);				
				for (int t=0; t<NumTAs; t++)
				{
					stream.vSeek(sizeof(LYNXANSICHAR)*64, CStream::SeekCur);												
					stream.vRead(&NumTexs, sizeof(int), 1);
					if (NumTexs)
					{
						for (int j=0; j<NumTexs; j++)
						{								
							stream.vSeek(sizeof(LYNXUNICHAR)*256, CStream::SeekCur);

							stream.vSeek(sizeof(int), CStream::SeekCur);
							stream.vSeek(sizeof(int), CStream::SeekCur);
							stream.vSeek(sizeof(LYNXBOOL), CStream::SeekCur);
							//stream.vSeek(sizeof(float), CStream::SeekCur);
							GraphicsSystem::CSamplerState::LoadDesc(SamplerStateDesc, stream, LYNX_TRUE);

							stream.vSeek(sizeof(float), CStream::SeekCur);
							stream.vSeek(sizeof(float), CStream::SeekCur);
							stream.vSeek(sizeof(float), CStream::SeekCur);
							stream.vSeek(sizeof(PLAYMODE), CStream::SeekCur);

							int NumKeys;
							int ST, ET, NumValues; 
							float Value;
							// read position keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								for (int k=0; k<NumKeys; k++)
								{
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vRead(&NumValues, sizeof(int), 1);						
									stream.vSeek(sizeof(float)*NumValues, CStream::SeekCur);									
								}
							}
							// read rotation keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								for (int k=0; k<NumKeys; k++)
								{
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vRead(&NumValues, sizeof(int), 1);						
									stream.vSeek(sizeof(float)*NumValues, CStream::SeekCur);									
								}
							}
							// read scale keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								for (int k=0; k<NumKeys; k++)
								{
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vRead(&NumValues, sizeof(int), 1);						
									stream.vSeek(sizeof(float)*NumValues, CStream::SeekCur);									
								}
							}
							// read color keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								for (int k=0; k<NumKeys; k++)
								{
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vRead(&NumValues, sizeof(int), 1);						
									stream.vSeek(sizeof(float)*NumValues, CStream::SeekCur);									
								}
							}
							// read texture keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								for (int k=0; k<NumKeys; k++)
								{
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vSeek(sizeof(int), CStream::SeekCur);
									stream.vRead(&NumValues, sizeof(int), 1);						
									stream.vSeek(sizeof(float)*NumValues, CStream::SeekCur);									
								}
							}
						}			
					}										
				}				
				stream.vRead(&NumTech, sizeof(DWORD), 1);				
				for (int t=0; t<NumTech; t++)
				{
					stream.vSeek(sizeof(TECHTYPE), CStream::SeekCur);
					DWORD NumPass;
					stream.vRead(&NumPass, sizeof(DWORD), 1);				
					for (int i=0; i<NumPass ;i++)
					{
						stream.vSeek(sizeof(LYNXBOOL), CStream::SeekCur);
						stream.vSeek(sizeof(LYNXANSICHAR)*64, CStream::SeekCur);
						stream.vSeek(sizeof(LYNXANSICHAR)*64, CStream::SeekCur);										
					}
				}	
			}
			else
			{
				Release();

				LYNXCHAR VertexLayoutName[128];
				GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;
				
				stream.vRead(&m_Diffuse, sizeof(LYNXCOLORRGBA), 1);
				stream.vRead(&m_Specular, sizeof(LYNXCOLORRGBA), 1);
				stream.vRead(&m_SpPower, sizeof(LYNXREAL), 1);
				stream.vRead(UniStr, sizeof(LYNXUNICHAR), 64);
				lynxFromUniStr(VertexLayoutName, UniStr);								

				m_lpEngine->CreateVertexLayout(m_VertexLayout, CString(VertexLayoutName));

				int NumTAs;
				stream.vRead(&NumTAs, sizeof(int), 1);
				vSetNumTextureAnimation(NumTAs);
				for (int t=0; t<NumTAs; t++)
				{
					stream.vRead(ShaderConstantName, sizeof(LYNXANSICHAR), 64);						
					m_TextureAnimationArray[t]->SetShaderConstantName(ShaderConstantName);

					int NumTexs;
					int Channel;					
					stream.vRead(&NumTexs, sizeof(int), 1);
					if (NumTexs)
					{
						m_TextureAnimationArray[t]->Create(NumTexs);											
						for (int j=0; j<NumTexs; j++)
						{		
							stream.vRead(UniStr, sizeof(LYNXUNICHAR), 256);
							lynxFromUniStr(MapName, UniStr);																						
							TexDesc.Default();

							stream.vRead(&TexDesc.MipMapLevel, sizeof(int), 1);				
							stream.vRead(&TexDesc.LODBias, sizeof(int), 1);			
							stream.vRead(&TexDesc.bSRGB, sizeof(LYNXBOOL), 1);	
							//stream.vRead(&TexDesc.AlphaReference, sizeof(float), 1);

							CPlatformFileStream TexFilestream;
							m_TextureAnimationArray[t]->LoadTexture(j, TexFilestream, MapName, &TexDesc);																			
							GraphicsSystem::CSamplerState::LoadDesc(SamplerStateDesc, stream, bskip);
							m_TextureAnimationArray[t]->GetTexture()->CreateSamplerState(SamplerStateDesc);								
							
							stream.vRead(&m_TextureAnimationArray[t]->GetlpAnimation()->m_StartTime, sizeof(float), 1);
							stream.vRead(&m_TextureAnimationArray[t]->GetlpAnimation()->m_EndTime, sizeof(float), 1);
							stream.vRead(&m_TextureAnimationArray[t]->GetlpAnimation()->m_Step, sizeof(float), 1);
							stream.vRead(&m_TextureAnimationArray[t]->GetlpAnimation()->m_PlayMode, sizeof(PLAYMODE), 1);

							LYNXBOOL bKeys;

							stream.vRead(&bKeys, sizeof(LYNXBOOL), 1);
							if (bKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_POSITION);
								m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->vLoad(stream);
							}
							stream.vRead(&bKeys, sizeof(LYNXBOOL), 1);
							if (bKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_ROTATION);
								m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->vLoad(stream);
							}
							stream.vRead(&bKeys, sizeof(LYNXBOOL), 1);
							if (bKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_SCALE);
								m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->vLoad(stream);
							}
							stream.vRead(&bKeys, sizeof(LYNXBOOL), 1);
							if (bKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_COLOR);
								m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->vLoad(stream);
							}
							stream.vRead(&bKeys, sizeof(LYNXBOOL), 1);
							if (bKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_TEXTURE);
								m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->vLoad(stream);
							}

							/*
							int NumKeys;
							int ST, ET, NumValues; 
							float Value;
							// read position keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_POSITION, NumKeys, 2);
								for (int k=0; k<NumKeys; k++)
								{
									stream.vRead(&ST, sizeof(int), 1);
									stream.vRead(&ET, sizeof(int), 1);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetStartTime(ST);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetEndTime(ET);
									stream.vRead(&NumValues, sizeof(int), 1);						
									for (int v=0; v<NumValues; v++)
									{
										stream.vRead(&Value, sizeof(float), 1);
										m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys[k].SetValue(v, Value);
									}
								}
							}
							// read rotation keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_ROTATION, NumKeys, 1);
								for (int k=0; k<NumKeys; k++)
								{
									stream.vRead(&ST, sizeof(int), 1);
									stream.vRead(&ET, sizeof(int), 1);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetStartTime(ST);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetEndTime(ET);
									stream.vRead(&NumValues, sizeof(int), 1);						
									for (int v=0; v<NumValues; v++)
									{
										stream.vRead(&Value, sizeof(float), 1);
										m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys[k].SetValue(v, Value);
									}
								}
							}
							// read scale keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_SCALE, NumKeys, 2);
								for (int k=0; k<NumKeys; k++)
								{
									stream.vRead(&ST, sizeof(int), 1);
									stream.vRead(&ET, sizeof(int), 1);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetStartTime(ST);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetEndTime(ET);
									stream.vRead(&NumValues, sizeof(int), 1);						
									for (int v=0; v<NumValues; v++)
									{
										stream.vRead(&Value, sizeof(float), 1);
										m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys[k].SetValue(v, Value);
									}
								}
							}
							// read color keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_COLOR, NumKeys, 4);
								for (int k=0; k<NumKeys; k++)
								{
									stream.vRead(&ST, sizeof(int), 1);
									stream.vRead(&ET, sizeof(int), 1);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->m_Keys[k].SetStartTime(ST);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->m_Keys[k].SetEndTime(ET);
									stream.vRead(&NumValues, sizeof(int), 1);						
									for (int v=0; v<NumValues; v++)
									{
										stream.vRead(&Value, sizeof(float), 1);
										m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->m_Keys[k].SetValue(v, Value);
									}
								}
							}
							// read texture keys							
							stream.vRead(&NumKeys, sizeof(int), 1);
							if (NumKeys)
							{
								m_TextureAnimationArray[t]->GetlpAnimation()->CreateKeys(Animation::CAnimation::KT_TEXTURE, NumKeys, 4);
								for (int k=0; k<NumKeys; k++)
								{
									stream.vRead(&ST, sizeof(int), 1);
									stream.vRead(&ET, sizeof(int), 1);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[k].SetStartTime(ST);
									m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[k].SetEndTime(ET);
									stream.vRead(&NumValues, sizeof(int), 1);						
									for (int v=0; v<NumValues; v++)
									{
										stream.vRead(&Value, sizeof(float), 1);
										m_TextureAnimationArray[t]->GetlpAnimation()->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[k].SetValue(v, Value);
									}
								}
							}
							*/
						}			
					}										
				}

				TECHTYPE TechType;
				stream.vRead(&NumTech, sizeof(DWORD), 1);		
				for (int t=0; t<NumTech; t++)
				{
					stream.vRead(&TechType, sizeof(TECHTYPE), 1);		
					vAddTechnique(TechType);
					m_TechniqueArray[TechType]->vLoad(stream);
				}				

				if (MaterialSystem::DIFFUSEMAP < m_TextureAnimationArray.size())
				{
					if (!m_TextureAnimationArray[MaterialSystem::DIFFUSEMAP]->GetTexture(0)->vIsValid())						
					{
						m_TextureAnimationArray[MaterialSystem::DIFFUSEMAP]->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::WHITE_TEXTURE));					
						m_TextureAnimationArray[MaterialSystem::DIFFUSEMAP]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
					}
				}
				if (MaterialSystem::NORMALMAP < m_TextureAnimationArray.size())
				{
					if (!m_TextureAnimationArray[MaterialSystem::NORMALMAP]->GetTexture(0)->vIsValid())						
					{
						m_TextureAnimationArray[MaterialSystem::NORMALMAP]->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::DOT3_TEXTURE));					
						m_TextureAnimationArray[MaterialSystem::NORMALMAP]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());
					}
				}
				if (MaterialSystem::SPECULARMAP < m_TextureAnimationArray.size())
				{
					if (!m_TextureAnimationArray[MaterialSystem::SPECULARMAP]->GetTexture(0)->vIsValid())						
					{
						m_TextureAnimationArray[MaterialSystem::SPECULARMAP]->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::WHITE_TEXTURE));					
						m_TextureAnimationArray[MaterialSystem::SPECULARMAP]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SPECULARMAP).c_str());
					}
				}
				if (MaterialSystem::AMBIENTMAP < m_TextureAnimationArray.size())
				{
					if (!m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->GetTexture(0)->vIsValid())						
					{
						m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->SetTexture(0, GetlpEngine()->GetlpGraphicsSystem()->GetSystemTexture(GraphicsSystem::BLACK_TEXTURE));
						m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::GLOWMAP).c_str());
					}
				}
			}			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CMaterial::vLoad(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			m_Name = file->Read(_T("Name"), m_Name, Root);
			m_MaterialType = TranslateMaterialType(file->Read(_T("MaterialType"), GetEnumStr(m_MaterialType), Root));
			m_LightingType = TranslateLightingType(file->Read(_T("LightingType"), GetEnumStr(m_LightingType), Root));
			m_TransparentType = GetTransparentType(file->Read(_T("TransparentType"), GetEnumStr(m_TransparentType), Root));

			m_Diffuse = file->Read(_T("Diffuse"), m_Diffuse, Root);
			m_Specular = file->Read(_T("Specular"), m_Specular, Root);
			m_SpPower = file->Read(_T("SpecularPower"), m_SpPower, Root);
			m_ReflectionIndex = file->Read(_T("ReflectionIndex"), m_ReflectionIndex, Root);
			m_RefractionIndex = file->Read(_T("RefractionIndex"), m_RefractionIndex, Root);
			m_TransmissionRate = file->Read(_T("TransmissionRate"), m_TransmissionRate, Root);
			m_RefractionOffest = file->Read(_T("RefractionOffest"), m_RefractionOffest, Root);
			file->Read(_T("VertexLayout"), m_VertexLayout->GetName(), Root);
			
			CXMLElement* Techs = Root->FirstChildElement("Techniques");
			if (Techs)
			{
				CXMLElement* Tech = Techs->FirstChildElement("Technique");
				while (Tech)
				{		
					TECHTYPE TechType = (TECHTYPE)file->Read(_T("Type"), (int)0, Tech);
					vAddTechnique(TechType);
					m_TechniqueArray[TechType]->vLoad(file, Tech);
					Tech = Tech->NextSiblingElement("Technique");
				}
			}			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CMaterial::vSave(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();
			
			file->Write(_T("Name"), m_Name, Root);
			file->Write(_T("MaterialType"), GetEnumStr(m_MaterialType), Root);
			file->Write(_T("LightingType"), GetEnumStr(m_LightingType), Root);
			file->Write(_T("TransparentType"), GetEnumStr(m_TransparentType), Root);			

			file->Write(_T("Diffuse"), m_Diffuse, Root);
			file->Write(_T("Specular"), m_Specular, Root);
			file->Write(_T("SpecularPower"), m_SpPower, Root);
			file->Write(_T("ReflectionIndex"), m_ReflectionIndex, Root);
			file->Write(_T("RefractionIndex"), m_RefractionIndex, Root);
			file->Write(_T("TransmissionRate"), m_TransmissionRate, Root);
			file->Write(_T("RefractionOffest"), m_RefractionOffest, Root);

			file->Write(_T("VertexLayout"), m_VertexLayout->GetName(), Root);	

			CXMLElement* Techs = file->GetDoc()->NewElement("Techniques");
			Root->InsertEndChild(Techs);		

			CMap<TECHTYPE, CTechnique*>::CIterator Tech;
			for (Tech = m_TechniqueArray.begin(); Tech != m_TechniqueArray.end(); ++Tech)
			{
				CXMLElement* TechNode = file->GetDoc()->NewElement("Technique");
				Techs->InsertEndChild(TechNode);				
				(*Tech).second->vSave(file, TechNode);
			}
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CMaterial::Sync()
		{
			CMap<TECHTYPE, CTechnique*>::CIterator Tech;            
			for (Tech = m_TechniqueArray.begin(); Tech != m_TechniqueArray.end(); ++Tech)
			{
				for (int p=0; p<(*Tech).second->m_PassArray.size(); p++)
				{
					(*Tech).second->GetPass(p).m_TextureAnimationArray = m_TextureAnimationArray;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CMaterial::RebindResources()
		{
			CMap<TECHTYPE, CTechnique*>::CIterator Tech;            
			for (Tech = m_TechniqueArray.begin(); Tech != m_TechniqueArray.end(); ++Tech)
			{
				for (int p=0; p<(*Tech).second->m_PassArray.size(); p++)
				{
					(*Tech).second->GetPass(p).GetVS()->vBindResource();
					(*Tech).second->GetPass(p).GetPS()->vBindResource();
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CMaterial::SetDirty(LYNXBOOL b)
		{
			CMap<TECHTYPE, CTechnique*>::CIterator Tech;            
			for (Tech = m_TechniqueArray.begin(); Tech != m_TechniqueArray.end(); ++Tech)
			{
				for (int p=0; p<(*Tech).second->m_PassArray.size(); p++)
				{
					//(*Tech).second->GetPass(p).GetVS()->vBindResource();
					//(*Tech).second->GetPass(p).GetPS()->vBindResource();
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterial::vAddTechnique(TECHTYPE t)
		{
			if (!m_TechniqueArray.is_exist(t))
			{
				m_TechniqueArray[t] = LYNXNEW CTechnique(this);
				m_TechniqueArray[t]->SetType(t);
				m_TechniqueArray[t]->SetName(GetName() + CString(_T(".")) + LynxEngine::GetEnumStr(t));
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterial::DeleteTechnique(TECHTYPE t)
		{
			LYNXDEL m_TechniqueArray[t];
			m_TechniqueArray.erase(t);
		}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void CMaterial::vCreateTechnique(DWORD num)
		//{				
		//#ifdef __USE_TECH_MAP__
		//	for (int i=0; i<num; i++)
		//	{
		//		vAddTechnique((CMaterial::TECHTYPE)i);				
		//	}	
		//#else
		//	m_TechniqueArray.resize(num);	
		//	for (int i=0; i<m_TechniqueArray.size(); i++)
		//	{
		//		m_TechniqueArray[(CMaterial::TECHTYPE)i] = LYNXNEW CTechnique(this);
		//		m_TechniqueArray[(CMaterial::TECHTYPE)i]->SetType((CMaterial::TECHTYPE)i);
		//		m_TechniqueArray[(CMaterial::TECHTYPE)i]->SetName(GetName()+CString(_T("."))+LynxEngine::GetEnumStr((CMaterial::TECHTYPE)i));
		//	}
		//#endif
		//}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTechnique* CMaterial::FindTechnique(TECHTYPE t)
		{
			CMap<TECHTYPE, CTechnique*>::CIterator Tech = m_TechniqueArray.find(t);
			if (Tech == m_TechniqueArray.end())
				return NULL;
			else
				return (((*Tech).second));				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTechnique&	CMaterial::GetTechnique(const CString& name)
		{
			CMap<TECHTYPE, CTechnique*>::CIterator Tech;            
			Tech = LynxEngine::find(m_TechniqueArray.begin(), m_TechniqueArray.end(), name);

			return (*((*Tech).second));				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterial::vPlayAll(LYNXREAL skip)
		{
			for (int i=0; i<m_TextureAnimationArray.size(); i++)
			{
				/*
				if (i != 0)
				{
					if (m_TextureAnimationArray[0]->GetlpAnimation() && m_TextureAnimationArray[i]->GetlpAnimation())
					{
						m_TextureAnimationArray[i]->GetlpAnimation()->m_Time = m_TextureAnimationArray[0]->GetlpAnimation()->m_Time; 
					}
				}
				*/
				m_TextureAnimationArray[i]->Play(skip);
			}
			
			for (int i=0; i<m_TechniqueArray.size(); i++)
			{
				GetTechnique(i).vPlayAll(skip);
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMaterial::vPlay(int channel, LYNXREAL skip)
		{
			LYNXBOOL			bRet = LYNX_TRUE;

			m_TextureAnimationArray[channel]->Play(skip);
			for (int i=0; i<m_TechniqueArray.size(); i++)
			{
				bRet &= GetTechnique(i).vPlay(channel, skip);
			}
			return bRet;
		}				
	}
}