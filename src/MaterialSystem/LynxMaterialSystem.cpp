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
#include <LynxEngine.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <MaterialSystem/LynxMaterialSystem.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(MaterialSystem::MATERIALMAP e)
	{
		CString	MapType = CString(MaterialSystem::CMaterialSystem::GetMapString(e));

		return MapType;
	}
	namespace MaterialSystem 
	{		
		/*const CAnsiString			CMaterialSystem::m_MaterialMapString[NUM_MATERIALMAP] = {"DiffuseMap",
																							 "NormalMap",
																							 "DetailMap",
																							 "SpecularMap",
																							 "AmbientMap",
																							 "GlowMap",
																							 "ShadowMap",
																							 "ProjectedLightMap",
																							 "LightMap",
																							 "DepthMap",
																							 "ReflectionMap",
																							 "RefractionMap",
																							 "TerrainLayer0",
																							 "TerrainLayer1",
																							 "TerrainLayer2",
																							 "TerrainLayer3",
		};*/
		CMaterialPtr				CMaterialSystem::m_NULL_Material((MaterialSystem::CMaterial*)(NULL));				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMaterialSystem::CMaterialSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{				
			m_lpCurrentAmbientLight = NULL;
			m_lpCurrentLight = NULL;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMaterialSystem::~CMaterialSystem(void)
		{			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));

			/*
			CList<CMaterialPtr>::CIterator Mat;
			for (Mat=m_MaterialList.begin(); Mat != m_MaterialList.end(); ++Mat)
			{
				GetlpEngine()->DestroyResource((*Mat)->GetName());			
				(*Mat).Reset();
			}
			*/

			DeleteAllMaterials();								
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		const CAnsiString CMaterialSystem::GetMapString(const MATERIALMAP map)
		{
			CAnsiString MapName;

			switch (map)
			{
				case DIFFUSEMAP:
					MapName = "DiffuseMap";
					break;
				case NORMALMAP:
					MapName = "NormalMap";
					break;
				case DETAILMAP:
					MapName = "DetailMap";
					break;
				case SPECULARMAP:
					MapName = "SpecularMap";
					break;
				case AMBIENTMAP:
					MapName = "AmbientMap";
					break;
				case GLOWMAP:
					MapName = "GlowMap";
					break;
				case DISPLACEMENTMAP:
					MapName = "DisplacementMap";
					break;
				case DISTORTIONMAP:
					MapName = "DistortionMap";
					break;
				case SHADOWMAP:
					MapName = "ShadowMap";
					break;
				case PROJECTED_LIGHTMAP:
					MapName = "ProjectedLightMap";
					break;
				case LIGHTMAP:
					MapName = "LightMap";
					break;
				case DEPTHMAP:
					MapName = "DepthMap";
					break;
				case REFLECTIONMAP:
					MapName = "ReflectionMap";
					break;
				case REFRACTIONMAP:
					MapName = "RefractionMap";
					break;
				case TERRAINLAYER0:
					MapName = "TerrainLayer0";
					break;
				case TERRAINLAYER1:
					MapName = "TerrainLayer1";
					break;
				case TERRAINLAYER2:
					MapName = "TerrainLayer2";
					break;
				case TERRAINLAYER3:
					MapName = "TerrainLayer3";
					break;
			}			
			return MapName;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMaterialSystem::vCreate()
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMaterialSystem::vInit()
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::vInit() OK\r\n")));

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMaterialSystem::CreateMaterial(CMaterialPtr& m, CMaterial::TYPE type)
		{
			CMaterialPtr Mat;

			switch (type)
			{
				case CMaterial::STANDARD:
					m_lpEngine->CreateResource(m, MATERIAL);
					break;
				default:
					m_lpEngine->CreateResource(m, MATERIAL);
					break;
			}
			if (!m)
				return LYNX_FALSE;

			m->SetMaterialType(type);
			AddMaterial(m_lpEngine->GetResourceManager().FindObj(m->GetName()));
            
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMaterialSystem::CreateMaterial(CMaterialPtr& m, CMaterial::TYPE type, const CString& name)
		{
			m = FindMaterial(name);
			if (!m)
			{
				if (!CreateMaterial(m, type))
					return LYNX_FALSE;

				m->SetName(name);
			}            
			return LYNX_TRUE;
		}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void CMaterialSystem::SetFogParam(LPLYNXPLANE lpp, LPLYNXFOGDATA lpfog1, LPLYNXFOGDATA lpfog2)
		//{
		//	m_FogPlane[0] = lpp->Normal.s.x;
		//	m_FogPlane[1] = lpp->Normal.s.y;
		//	m_FogPlane[2] = lpp->Normal.s.z;
		//	m_FogPlane[3] = -lpp->Distance;
		//	lynxSetVertexShaderConstantTable(1, "gFogPlane", m_FogPlane, 4);			

		//	lynxSetFogData(lpfog1);   
		//	m_FogConstant[0] = -1.0f/(lpfog1->End - lpfog1->Start);
		//	m_FogConstant[1] = lpfog1->End/(lpfog1->End - lpfog1->Start);
		//	m_FogConstant[2] = -1.0f/(lpfog2->End - lpfog2->Start);
		//	m_FogConstant[3] = lpfog2->End/(lpfog2->End - lpfog2->Start);
		//	lynxSetVertexShaderConstantTable(1, "gFogConstant", m_FogConstant, 4);			
		//}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMaterialPtr& CMaterialSystem::FindMaterial(const LYNXCHAR *name)
		{
			CList<CMaterialPtr>::CIterator Mat;

			Mat = LynxEngine::find(m_MaterialList.begin(), m_MaterialList.end(), CString(name));
			if (Mat == m_MaterialList.end())
				return m_NULL_Material;

			return (*Mat);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMaterialPtr& CMaterialSystem::FindMaterial(const CString& name)
		{
			CList<CMaterialPtr>::CIterator Mat;

			Mat = LynxEngine::find(m_MaterialList.begin(), m_MaterialList.end(), name);
			if (Mat == m_MaterialList.end())
				return m_NULL_Material;

			return (*Mat);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterialSystem::AddMaterial(const CMaterialPtr& lpmat)
		{
			CMaterialPtr& lpMat = FindMaterial(lpmat->GetName());
			if (!lpMat)
			{
				lpmat->m_lpMaterialSystem = this;
				m_MaterialList.push_back(lpmat);
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterialSystem::AddMaterial(CResourcePtr& lpmat)
		{
			CMaterialPtr& lpMat = FindMaterial(lpmat->GetName());
			if (!lpMat)
			{
				CMaterialPtr pMat;
				pMat = lpmat;
				pMat->m_lpMaterialSystem = this;
				m_MaterialList.push_back(pMat);				
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterialSystem::DeleteMaterial(CMaterialPtr& lpmat)
		{
			m_MaterialList.remove(lpmat);
			//lpmat.Reset();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterialSystem::DeleteMaterial(const CString& name)
		{
			CMaterialPtr& lpMat = FindMaterial(name);
			if (lpMat)
			{
				DeleteMaterial(lpMat);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMaterialSystem::DeleteAllMaterials()
		{
			//for (CList<CMaterialPtr>::CIterator Mat=m_MaterialList.begin(); Mat != m_MaterialList.end(); Mat++)
			//	(*Mat).Reset();

			m_MaterialList.clear();
		}
	}
}