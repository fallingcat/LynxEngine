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
#include <LynxEngine.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <Container/LynxSkyBoxContainer.h>
#include <LynxBoxProxy.h>
#include <CollisionSystem/LynxCollisionHierarchy.h>
#include <CollisionSystem/LynxAABB.h>
#include <CollisionSystem/LynxSphere.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CSkyBoxContainer::CSkyBoxContainer(CEngine *lpengine)	
	:CGeometryContainer(lpengine)
	{		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CSkyBoxContainer::~CSkyBoxContainer(void)
	{		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::vInstance(const CContainer& rhs)
	{		
		CGeometryContainer::vInstance(rhs);		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::vSetNumLODs(int lod)
	{
		CGeometryContainer::vSetNumLODs(1);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::vAddLOD()
	{		
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CSkyBoxContainer::LoadGEO(CStream& stream, const LYNXCHAR *filename, LYNXBOOL bloadmaterial, int lod, LYNXBOOL bbo)
	{
		char Name[64];
		LYNXCHAR MName[128];
		LYNXCHAR TName[128];
		char AnsiName[128];
		LYNXCHAR TexturePath[128];	
		char TextureName[128];	
		DWORD Num, NumVertices, NumTriangles;
		LynxEngine::MaterialSystem::CMaterialPtr lpMat;
		LynxEngine::MaterialSystem::CMaterialPtr lpNewMat;		

		DWORD CurrentLOD = lod;
		if (lod == 0)
			vSetNumLODs(1);

		m_SourceType = ORIGIN;
		
		if (!stream.IsValid())
		{
			if (!stream.vOpen(filename, CStream::Binary|CStream::Read))
				return LYNX_FALSE;			
		}	
	
		char Mark[] = "LYNXGEO";	
		char Check[8];		
		DWORD Version;

		stream.vRead(&Check, 7, 1);
		Check[7] = 0x00;
		if (strcmp(Mark, Check))
		{
			stream.vClose();	
			return FALSE;		
		}	
		stream.vRead(&Version, sizeof(DWORD), 1);	

		float Float;		
		LYNXCHAR CurrentPath[128];
		LYNXCHAR Ext[16];	
		LYNXCHAR MaterialName[64];	
		LYNXCHAR DiffuseName[64];	
		LYNXCHAR NormalName[64];	
		LYNXCHAR SpecularName[64];	
		LYNXCHAR AOName[64];	
		LynxEngine::GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;
		LynxEngine::MaterialSystem::CTechnique* Technique;
		LynxEngine::GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC Desc;	
		LynxEngine::GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
		LynxEngine::GraphicsSystem::CRenderer::RASTERIZERSTATEDESC RDesc;
		LynxEngine::GraphicsSystem::CRenderer::SAMPLERSTATEDESC SamDesc;

		Desc.Default();	
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		stream.vRead(&Num, sizeof(DWORD), 1);
		LYNX_ASSERT(Num == 1);

		for (int i=0; i<Num; i++)
		{
			if (bloadmaterial)
			{
				stream.vRead(AnsiName, sizeof(char), 64);
				lynxFromAnsiStr(TName, AnsiName);
				m_lpCurrentMaterial = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(TName); 			
				if (lod == 0 && !m_lpCurrentMaterial)
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_lpCurrentMaterial, MaterialSystem::CMaterial::STANDARD);				
					m_lpCurrentMaterial->SetName(TName);								

					stream.vRead(&Float, 1, sizeof(float));
					m_lpCurrentMaterial->m_Diffuse.Red = LYNX_ROUND(Float*255.0f);
					stream.vRead(&Float, 1, sizeof(float));
					m_lpCurrentMaterial->m_Diffuse.Green = LYNX_ROUND(Float*255.0f);
					stream.vRead(&Float, 1, sizeof(float));
					m_lpCurrentMaterial->m_Diffuse.Blue = LYNX_ROUND(Float*255.0f);
					stream.vRead(&Float, 1, sizeof(float));
					m_lpCurrentMaterial->m_Diffuse.Alpha = LYNX_ROUND(Float*255.0f);

					stream.vRead(&Float, 1, sizeof(float));
					m_lpCurrentMaterial->m_Specular.Red = LYNX_ROUND(Float*255.0f);
					stream.vRead(&Float, 1, sizeof(float));
					m_lpCurrentMaterial->m_Specular.Green = LYNX_ROUND(Float*255.0f);
					stream.vRead(&Float, 1, sizeof(float));
					m_lpCurrentMaterial->m_Specular.Blue = LYNX_ROUND(Float*255.0f);
					stream.vRead(&Float, 1, sizeof(float));
					m_lpCurrentMaterial->m_Specular.Alpha = LYNX_ROUND(Float*255.0f);

					stream.vRead(&m_lpCurrentMaterial->m_SpPower, 1, sizeof(float));

					stream.vRead(Name, sizeof(char), 64);					
					lynxFromAnsiStr(AOName, Name);
					stream.vRead(Name, sizeof(char), 64);					
					lynxFromAnsiStr(DiffuseName, Name);
					stream.vRead(Name, sizeof(char), 64);					
					lynxFromAnsiStr(NormalName, Name);
					stream.vRead(Name, sizeof(char), 64);					
					lynxFromAnsiStr(SpecularName, Name);

					lynxStrCpy(CurrentPath, m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath());

					m_lpCurrentMaterial->vSetNumTextureAnimation(4);
					m_lpCurrentMaterial->GetTextureAnimation(0)->Create(1);			
					m_lpCurrentMaterial->GetTextureAnimation(0)->SetName(m_lpCurrentMaterial->GetName()+_T(".DiffuseMap.TexAni"));
					m_lpCurrentMaterial->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
					TexDesc.Default();
					TexDesc.bSRGB = LYNX_TRUE;
					
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpCurrentMaterial->GetTextureAnimation(0)->GetTexture(0), *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), DiffuseName, &TexDesc);
					if (!m_lpCurrentMaterial->GetTextureAnimation(0)->GetTexture(0)->vIsValid())				
					{
						TexDesc.bSRGB = LYNX_FALSE;
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpCurrentMaterial->GetTextureAnimation(0)->GetTexture(0), *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("checker.png"), &TexDesc);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}

					TexDesc.bSRGB = LYNX_FALSE;
					m_lpCurrentMaterial->GetTextureAnimation(1)->Create(1);		
					m_lpCurrentMaterial->GetTextureAnimation(1)->SetName(m_lpCurrentMaterial->GetName()+_T(".NormalMap.TexAni"));
					m_lpCurrentMaterial->GetTextureAnimation(1)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpCurrentMaterial->GetTextureAnimation(1)->GetTexture(0), *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), NormalName);
					if (!m_lpCurrentMaterial->GetTextureAnimation(1)->GetTexture(0)->vIsValid())				
					{
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpCurrentMaterial->GetTextureAnimation(1)->GetTexture(0), *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("dot3.png"), &TexDesc);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}

					TexDesc.bSRGB = LYNX_TRUE;
					m_lpCurrentMaterial->GetTextureAnimation(2)->Create(1);		
					m_lpCurrentMaterial->GetTextureAnimation(2)->SetName(m_lpCurrentMaterial->GetName()+_T(".SpecularMap.TexAni"));
					m_lpCurrentMaterial->GetTextureAnimation(2)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::SPECULARMAP).c_str());
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpCurrentMaterial->GetTextureAnimation(2)->GetTexture(0), *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), SpecularName);
					if (!m_lpCurrentMaterial->GetTextureAnimation(2)->GetTexture(0)->vIsValid())				
					{
						TexDesc.bSRGB = LYNX_FALSE;
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpCurrentMaterial->GetTextureAnimation(2)->GetTexture(0), *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("white.png"), &TexDesc);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}

					TexDesc.bSRGB = LYNX_TRUE;
					m_lpCurrentMaterial->GetTextureAnimation(3)->Create(1);		
					m_lpCurrentMaterial->GetTextureAnimation(3)->SetName(m_lpCurrentMaterial->GetName()+_T(".AmbientMap.TexAni"));
					m_lpCurrentMaterial->GetTextureAnimation(3)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::AMBIENTMAP).c_str());
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpCurrentMaterial->GetTextureAnimation(3)->GetTexture(0), *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), AOName);
					if (!m_lpCurrentMaterial->GetTextureAnimation(3)->GetTexture(0)->vIsValid())				
					{
						TexDesc.bSRGB = LYNX_FALSE;
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(m_lpCurrentMaterial->GetTextureAnimation(3)->GetTexture(0), *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), _T("white.png"), &TexDesc);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}
					
					/*
					Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->Create(1);				
					Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->SetTextureName(0, LynxEngine::CString(m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+LynxEngine::CString(AOName));
					m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->GetTexture(0), 0, AOName, NULL, 0);
					if (!Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->GetTexture(0)->vIsValid())				
					{
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->LoadTexture(Pass.m_TextureAnimationArray[MaterialSystem::AMBIENTMAP]->GetTexture(0), 0, "../texture/white.png", NULL, 0);
						m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(CurrentPath);			
					}
					*/


					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::WIREFRAME);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::ONE_LIGHTING);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::ZPASS);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::SHADOWMAP);				
					m_lpCurrentMaterial->vAddTechnique(MaterialSystem::CMaterial::GPASS);				

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::WIREFRAME);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE_LIGHTING);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::AMBIENT_LIGHTING);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::POINT_LIGHTING);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SPOT_LIGHTING);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ONE_LIGHTING);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::ZPASS);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SHADOWMAP);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					Technique = &m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::GPASS);						
					Technique->vCreatePass(1);					
					Technique->GetPass(0).m_TextureAnimationArray = m_lpCurrentMaterial->GetTextureAnimationArray();														

					TempSetupMaterial();				
				}
				else
				{			
					stream.vSeek(sizeof(char)*64, CStream::SeekCur);
					stream.vSeek(sizeof(char)*64, CStream::SeekCur);
					stream.vSeek(sizeof(char)*64, CStream::SeekCur);
					stream.vSeek(sizeof(char)*64, CStream::SeekCur);
					stream.vSeek(sizeof(float)*9, CStream::SeekCur);					
				}
			}
			else
			{
				stream.vSeek(sizeof(char)*64, CStream::SeekCur);
				stream.vSeek(sizeof(char)*64, CStream::SeekCur);
				stream.vSeek(sizeof(char)*64, CStream::SeekCur);
				stream.vSeek(sizeof(char)*64, CStream::SeekCur);
				stream.vSeek(sizeof(char)*64, CStream::SeekCur);
				stream.vSeek(sizeof(float)*9, CStream::SeekCur);
			}
		}

		DWORD NumObjects;
		stream.vRead(&NumObjects, sizeof(DWORD), 1);
		LYNX_ASSERT(NumObjects == 1);

		for (int i=0; i<NumObjects; i++)
		{	
			stream.vRead(Name, 64, sizeof(char));				
			
			int MaterialIndex;
			stream.vRead(&MaterialIndex, sizeof(int), 1);

			if (bloadmaterial)
			{		
				SetCurrentMaterialTechnique(LynxEngine::MaterialSystem::CMaterial::POINT_LIGHTING);
			}

			CString CName = /*lpSObj->GetName() + CString(TEXT(".")) + */CString(Name) + CString(_T(".Container"));
			SetName(CName);				
			
			LYNXMATRIX M;
			stream.vRead(&M , sizeof(LYNXMATRIX), 1);
			SetLocalMatrix(LynxEngine::Math::CMatrix4(M));

			stream.vSeek(sizeof(DWORD), CStream::SeekCur);
			stream.vRead(&NumVertices , sizeof(DWORD), 1);
			stream.vSeek(sizeof(DWORD), CStream::SeekCur);
			stream.vRead(&NumTriangles , sizeof(DWORD), 1);

			m_MeshData->VertexList[lod].resize(NumVertices);																				
			for (int j=0; j<GetVertexList(lod).size(); j++)
			{
				stream.vRead(&GetVertexList(lod)[j].Pos, sizeof(float)*3, 1);				
				stream.vRead(&GetVertexList(lod)[j].Normal, sizeof(float)*3, 1);				
				stream.vRead(&GetVertexList(lod)[j].Tangent, sizeof(float)*3, 1);				
				stream.vRead(&GetVertexList(lod)[j].Binormal, sizeof(float)*3, 1);							

				lynxNormalise(&GetVertexList(lod)[j].Normal, 3);
				lynxNormalise(&GetVertexList(lod)[j].Tangent, 3);
				lynxNormalise(&GetVertexList(lod)[j].Binormal, 3);
			}
			for (int j=0; j<GetVertexList(lod).size(); j++)
				stream.vRead(&GetVertexList(lod)[j].TexCoord[0].u, sizeof(float)*2, 1);				
					
			for (int j=0; j<GetVertexList(lod).size(); j++)
			{
				float fR, fG, fB, fA;
				BYTE bR, bG, bB, bA;

				stream.vRead(&fR, sizeof(float), 1);
				stream.vRead(&fG, sizeof(float), 1);
				stream.vRead(&fB, sizeof(float), 1);
				stream.vRead(&fA, sizeof(float), 1);

				bR = LYNX_ROUND(fR * 255);
				bG = LYNX_ROUND(fG * 255);
				bB = LYNX_ROUND(fB * 255);
				bA = LYNX_ROUND(fA * 255);

				GetVertexList(lod)[j].Color.Red = bR;
				GetVertexList(lod)[j].Color.Green = bG;
				GetVertexList(lod)[j].Color.Blue =  bB;
				GetVertexList(lod)[j].Color.Alpha =  255;				
			}		
			
			m_MeshData->TriangleList[lod].resize(NumTriangles);			
			for (int j=0; j<GetTriangleList(lod).size(); j++)
			{
				stream.vRead(&GetTriangleList(lod)[j].v1, sizeof(DWORD), 1);
				stream.vRead(&GetTriangleList(lod)[j].v2, sizeof(DWORD), 1);
				stream.vRead(&GetTriangleList(lod)[j].v3, sizeof(DWORD), 1);			
				stream.vRead(&GetTriangleList(lod)[j].Plane.Normal, sizeof(float)*3, 1);
			}

			//read primitive data
			DWORD NumPrimitives;
			stream.vRead(&NumPrimitives, sizeof(DWORD), 1);
			GetPrimitiveList(lod).resize(NumPrimitives);		
			for (int j= 0; j<GetPrimitiveList(lod).size(); j++)
			{
				int Type;
				stream.vRead(&Type, sizeof(int), 1);						
				stream.vRead(&GetPrimitiveList(lod)[j].NumIndex, sizeof(DWORD), 1);
				switch (Type)
				{
					case 0:
						GetPrimitiveList(lod)[j].Type = LynxEngine::GraphicsSystem::CRenderer::TRIANGLE_LIST;
						GetPrimitiveList(lod)[j].NumPrimitives = GetPrimitiveList(lod)[j].NumIndex/3;
						break;
					case 1:
						GetPrimitiveList(lod)[j].Type = LynxEngine::GraphicsSystem::CRenderer::TRIANGLE_STRIP;
						GetPrimitiveList(lod)[j].NumPrimitives = GetPrimitiveList(lod)[j].NumIndex-2;
						break;
					case 2:
						GetPrimitiveList(lod)[j].Type = LynxEngine::GraphicsSystem::CRenderer::TRIANGLE_FAN;
						GetPrimitiveList(lod)[j].NumPrimitives = GetPrimitiveList(lod)[j].NumIndex-2;
						break;					
				}
				GetPrimitiveList(lod)[j].lpIndex = new unsigned short[GetPrimitiveList(lod)[j].NumIndex];
				for (int k=0; k<GetPrimitiveList(lod)[j].NumIndex; k++)
				{
					GetPrimitiveList(lod)[j].BaseVertex = 0;
					GetPrimitiveList(lod)[j].BaseIndex = 0;
					DWORD Index;
					stream.vRead(&Index, sizeof(DWORD), 1);
					GetPrimitiveList(lod)[j].lpIndex[k] = Index;
				}
			}						
		}	
		stream.vClose();

		SetupBuffers(lod);

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CSkyBoxContainer::vCreate(const CCreationDesc* desc)
	{
		LYNXBOOL bRet = LYNX_FALSE;		
		CGeometryCreationDesc* Desc = (CGeometryCreationDesc*)desc;		
		
		bRet = LoadGEO(*desc->m_lpStream, Desc->m_FileName.c_str(), Desc->m_bCreateMaterial, 0, Desc->m_bUseBufferObject);
		if (!bRet)
		{
			return LYNX_FALSE;			
		}		
		CContainer::vCreate(desc);
		
		return bRet;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::SetupRNRPlane(void)
	{
		LYNXVECTOR3D V1, V2;
		LYNX_VECTOR3D_SUB(V1, m_MeshData->VertexList[0][m_MeshData->TriangleList[0][0].v2].Pos, m_MeshData->VertexList[0][m_MeshData->TriangleList[0][0].v1].Pos);
		LYNX_VECTOR3D_SUB(V2, m_MeshData->VertexList[0][m_MeshData->TriangleList[0][0].v3].Pos, m_MeshData->VertexList[0][m_MeshData->TriangleList[0][0].v1].Pos);
		lynxCrossProduct(&m_RNRPlane.Normal, &V1, &V2);
		lynxNormalise(&m_RNRPlane.Normal, 3);						
		m_RNRPlane.Distance = LYNX_DOT_PRODUCT(m_RNRPlane.Normal, m_MeshData->VertexList[0][m_MeshData->TriangleList[0][0].v1].Pos);						
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::vRender(CCameraContainer* cam)
	{	
		GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();

		vCalculateLOD(cam);

		/*
		GraphicsSystem::STATICVERTEX* lpTVertex = LYNXCORE_NEW GraphicsSystem::STATICVERTEX[m_MeshData->VertexList[m_CurrentLOD].size()];
		lynxMemCpy(lpTVertex, m_lpV, sizeof(GraphicsSystem::STATICVERTEX)*m_MeshData->VertexList[m_CurrentLOD].size());
		for (int i=0; i<m_MeshData->VertexList[m_CurrentLOD].size(); i++)
			lynxPointXMatrix(&lpTVertex[i].Pos, &m_lpV[i].Pos, &m_M);
		
		GraphicsSystem::STATICVERTEX* lpMV = (GraphicsSystem::STATICVERTEX*)m_VertexArray[m_CurrentLOD]->vMap(0, sizeof(GraphicsSystem::RIGIDVERTEX)*m_MeshData->VertexList[m_CurrentLOD].size(), GraphicsSystem::CRenderer::LOCK_TO_WRITE_DISCARD_OLD);
		lynxMemCpy(lpMV, lpTVertex, sizeof(GraphicsSystem::STATICVERTEX)*m_MeshData->VertexList[m_CurrentLOD].size());
		m_VertexArray[m_CurrentLOD]->vUnmap();
		LYNXCORE_DEL [] lpTVertex;
		ResetTransform();
		*/
		
		//lpR->SetWorldMatrix(m_M);
		lpR->SetVertexArray(m_VertexArray[m_CurrentLOD], 0, 0);
		lpR->SetIndexArray(m_IndexArray[m_CurrentLOD], 0);	

		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)
		{
			Technique.GetPass(p).Set(this, cam);				
			m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();			
			for (int i=0; i<m_MeshData->PrimitiveList[m_CurrentLOD].size(); ++i)
			{						
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawPrimitiveBO(&m_MeshData->PrimitiveList[m_CurrentLOD][i], (DWORD)m_MeshData->VertexList[m_CurrentLOD].size());
			}
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::vLighting(CCameraContainer* cam, SceneSystem::CLight* lit)
	{
		LYNXMATRIX MM, WVPM, WVM, WLPM, WLM, TM1;
		float ZScale;
		float ZBias = 0.0005f;
		float TexelGrad[2];
		float Color[3];
		Math::CVector3 Pos, Dir;
		float Rad;
		float WireframeColor[4];			

		LYNX_ASSERT(cam);					
		
		GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();

		lpR->SetVertexArray(m_VertexArray[m_CurrentLOD], 0, 0);
		lpR->SetIndexArray(m_IndexArray[m_CurrentLOD], 0);
			
		switch (lit->GetLightType())
		{
			case SceneSystem::CLight::AMBIENT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);
				break;
			case SceneSystem::CLight::DIRECTIONAL:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);
				break;
			case SceneSystem::CLight::POINT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);
				break;
			case SceneSystem::CLight::SPOT:
				SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);
				break;			
		}
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)				
		{
			Technique.GetPass(p).Set(this, cam);			
			m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();

			for (int i=0; i<m_MeshData->PrimitiveList[m_CurrentLOD].size(); ++i)
			{						
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawPrimitiveBO(&m_MeshData->PrimitiveList[m_CurrentLOD][i], (DWORD)m_MeshData->VertexList[m_CurrentLOD].size());
			}						
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::vUpdateVertices(int lod)
	{
		LYNX_ASSERT(m_MeshData->VertexList[lod].size() == m_TVertexList.size());

		for (int i=0; i<m_MeshData->VertexList[lod].size(); i++)
		{
			lynxPointXMatrix(&m_TVertexList[i].Pos, &m_MeshData->VertexList[lod][i].Pos, &m_M); 
		}		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CSkyBoxContainer::vSetupTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc, int lod, LYNXBOOL bapply_world)
	{
		int LOD = lod;
		if (LOD >= m_NumLODs)
			LOD = m_NumLODs - 1;

		if (bapply_world)
		{
			m_TVertexList.clear();
			m_TVertexList.resize(m_MeshData->VertexList[LOD].size());
			vUpdateVertices(LOD);

			desc->m_NumVertices = m_MeshData->VertexList[LOD].size();
			desc->m_VertexStride = sizeof(VERTEX);
			desc->m_VertexList = &m_TVertexList[0].Pos.x;
			desc->m_NumTriangles = m_MeshData->TriangleList[LOD].size();
			desc->m_TriangleStride = sizeof(TRIANGLE);
			desc->m_TriangleList = &m_MeshData->TriangleList[LOD][0].v[0];
		}
		else
		{
			desc->m_NumVertices = m_MeshData->VertexList[LOD].size();
			desc->m_VertexStride = sizeof(VERTEX);
			desc->m_VertexList = &m_MeshData->VertexList[LOD][0].Pos.x;
			desc->m_NumTriangles = m_MeshData->TriangleList[LOD].size();
			desc->m_TriangleStride = sizeof(TRIANGLE);
			desc->m_TriangleList = &m_MeshData->TriangleList[LOD][0].v[0];
		}

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::vReleaseTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc)
	{
		//m_TVertexList.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CSkyBoxContainer::vSetupHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc, int lod, LYNXBOOL bapply_world)
	{
		int LOD = lod;
		if (LOD >= m_NumLODs)
			LOD = m_NumLODs - 1;

		if (bapply_world)
		{
			m_TVertexList.clear();
			m_TVertexList.resize(m_MeshData->VertexList[LOD].size());
			vUpdateVertices(LOD);

			desc->m_NumVertices = m_TVertexList.size();
			desc->m_VertexStride = sizeof(VERTEX);
			desc->m_VertexList = &m_TVertexList[0].Pos.x;
		}
		else
		{
			desc->m_NumVertices = m_MeshData->VertexList[LOD].size();
			desc->m_VertexStride = sizeof(VERTEX);
			desc->m_VertexList = &m_MeshData->VertexList[LOD][0].Pos.x;
		}

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSkyBoxContainer::vReleaseHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc)
	{		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CSkyBoxContainer::vSave(CStream& s)
	{
		LYNXUNICHAR Name[128];
		DWORD Num;

		lynxUniStr(Name, GetlpCurrentMaterial()->GetName().c_str());
		s.vWrite(Name, sizeof(LYNXUNICHAR), 64);					
		s.vWrite(&m_M, sizeof(LYNXMATRIX), 1);				

		s.vWrite(&m_NumLODs, sizeof(DWORD), 1);
		for (int Lod=0; Lod<m_NumLODs ;Lod++)
		{
			Num = (DWORD)m_MeshData->VertexList[Lod].size();
			s.vWrite(&Num, sizeof(DWORD), 1);
			for (int i=0; i<m_MeshData->VertexList[Lod].size(); i++)
			{
				s.vWrite(&m_MeshData->VertexList[Lod][i].Pos, sizeof(LYNXVECTOR3D), 1);
				s.vWrite(&m_MeshData->VertexList[Lod][i].Tangent, sizeof(LYNXVECTOR3D), 1);
				s.vWrite(&m_MeshData->VertexList[Lod][i].Binormal, sizeof(LYNXVECTOR3D), 1);
				s.vWrite(&m_MeshData->VertexList[Lod][i].Normal, sizeof(LYNXVECTOR3D), 1);
				s.vWrite(&m_MeshData->VertexList[Lod][i].TexCoord[0], sizeof(LYNXUVPOINT), 1);
				s.vWrite(&m_MeshData->VertexList[Lod][i].Color, sizeof(LYNXCOLORRGBA), 1);
			}
			Num = (DWORD)m_MeshData->TriangleList[Lod].size();
			s.vWrite(&Num, sizeof(DWORD), 1);
			for (int i=0; i<m_MeshData->TriangleList[Lod].size(); i++)
			{
				s.vWrite(&m_MeshData->TriangleList[Lod][i].v1, sizeof(DWORD), 1);
				s.vWrite(&m_MeshData->TriangleList[Lod][i].v2, sizeof(DWORD), 1);
				s.vWrite(&m_MeshData->TriangleList[Lod][i].v3, sizeof(DWORD), 1);
			}
			Num = (DWORD)m_MeshData->PrimitiveList[Lod].size();
			s.vWrite(&Num, sizeof(DWORD), 1);			
			for (int j= 0; j<m_MeshData->PrimitiveList[Lod].size(); j++)
			{
				s.vWrite(&m_MeshData->PrimitiveList[Lod][j].Type, sizeof(LynxEngine::GraphicsSystem::CRenderer::PRIMITIVETYPE), 1);			
				s.vWrite(&m_MeshData->PrimitiveList[Lod][j].NumPrimitives, sizeof(DWORD), 1);			
				s.vWrite(&m_MeshData->PrimitiveList[Lod][j].NumIndex, sizeof(DWORD), 1);				
				s.vWrite(m_MeshData->PrimitiveList[Lod][j].lpIndex, sizeof(unsigned short), m_MeshData->PrimitiveList[Lod][j].NumIndex);				
			}
		}
		
		return LYNX_TRUE;
	}		
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CSkyBoxContainer::vLoad(CStream& s, LYNXBOOL skip)
	{
		DWORD NumLODs, Num, NumIndex;
		LYNXLONG Len;
		LYNXUNICHAR MaterialName[128];

		if (skip)
		{
			s.vSeek(sizeof(LYNXUNICHAR)*64, CStream::SeekCur);					
			s.vSeek(sizeof(LYNXMATRIX), CStream::SeekCur);	

			s.vRead(&NumLODs, sizeof(DWORD), 1);								
			for (int lod=0; lod<NumLODs; lod++)
			{
				s.vRead(&Num, sizeof(DWORD), 1);
				Len = 0;
				Len += sizeof(LYNXVECTOR3D);
				Len += sizeof(LYNXVECTOR3D);
				Len += sizeof(LYNXVECTOR3D);
				Len += sizeof(LYNXVECTOR3D);
				Len += sizeof(LYNXUVPOINT);
				Len += sizeof(LYNXCOLORRGBA);
				Len *= Num;
				s.vSeek(Len, CStream::SeekCur);
				
				s.vRead(&Num, sizeof(DWORD), 1);
				Len = 0;
				Len += sizeof(DWORD);
				Len += sizeof(DWORD);
				Len += sizeof(DWORD);
				Len *= Num;
				s.vSeek(Len, CStream::SeekCur);
				
				s.vRead(&Num, sizeof(DWORD), 1);
				Len = 0;
				Len += sizeof(GraphicsSystem::CRenderer::PRIMITIVETYPE);
				Len += sizeof(DWORD);
				Len += sizeof(DWORD);
				Len *= Num;
				s.vSeek(Len, CStream::SeekCur);

				m_MeshData->PrimitiveList[lod].resize(Num);
				for (int j=0; j<m_MeshData->PrimitiveList[lod].size(); ++j)
				{
					s.vSeek(sizeof(GraphicsSystem::CRenderer::PRIMITIVETYPE), CStream::SeekCur);
					s.vSeek(sizeof(DWORD), CStream::SeekCur);
					s.vRead(&NumIndex, sizeof(DWORD), 1);	
					s.vSeek(sizeof(unsigned short)*m_MeshData->PrimitiveList[lod][j].NumIndex, CStream::SeekCur);
				}
			}
		}
		else
		{
			s.vRead(MaterialName, sizeof(LYNXUNICHAR), 64);					
			s.vRead(&m_LocalM, sizeof(LYNXMATRIX), 1);	

			s.vRead(&NumLODs, sizeof(DWORD), 1);					
			vSetNumLODs(NumLODs);
			for (int lod=0; lod<NumLODs; lod++)
			{
				s.vRead(&Num, sizeof(DWORD), 1);
				m_MeshData->VertexList[lod].resize(Num);
				for (int j=0; j<m_MeshData->VertexList[lod].size(); ++j)
				{
					s.vRead(&m_MeshData->VertexList[lod][j].Pos, sizeof(LYNXVECTOR3D), 1);	
					s.vRead(&m_MeshData->VertexList[lod][j].Tangent, sizeof(LYNXVECTOR3D), 1);
					s.vRead(&m_MeshData->VertexList[lod][j].Binormal, sizeof(LYNXVECTOR3D), 1);
					s.vRead(&m_MeshData->VertexList[lod][j].Normal, sizeof(LYNXVECTOR3D), 1);
					s.vRead(&m_MeshData->VertexList[lod][j].TexCoord[0], sizeof(LYNXUVPOINT), 1);
					s.vRead(&m_MeshData->VertexList[lod][j].Color, sizeof(LYNXCOLORRGBA), 1);
				}
				s.vRead(&Num, sizeof(DWORD), 1);
				m_MeshData->TriangleList[lod].resize(Num);
				for (int j=0; j<m_MeshData->TriangleList[lod].size(); ++j)
				{
					s.vRead(&m_MeshData->TriangleList[lod][j].v[0], sizeof(DWORD), 1);
					s.vRead(&m_MeshData->TriangleList[lod][j].v[1], sizeof(DWORD), 1);
					s.vRead(&m_MeshData->TriangleList[lod][j].v[2], sizeof(DWORD), 1);					
				}
				s.vRead(&Num, sizeof(DWORD), 1);
				m_MeshData->PrimitiveList[lod].resize(Num);
				for (int j=0; j<m_MeshData->PrimitiveList[lod].size(); ++j)
				{
					m_MeshData->PrimitiveList[lod][j].BaseVertex = 0;
					m_MeshData->PrimitiveList[lod][j].BaseIndex = 0;
					s.vRead(&m_MeshData->PrimitiveList[lod][j].Type, sizeof(GraphicsSystem::CRenderer::PRIMITIVETYPE), 1);			
					s.vRead(&m_MeshData->PrimitiveList[lod][j].NumPrimitives, sizeof(DWORD), 1);			
					s.vRead(&m_MeshData->PrimitiveList[lod][j].NumIndex, sizeof(DWORD), 1);				
					m_MeshData->PrimitiveList[lod][j].lpIndex = NULL;
					m_MeshData->PrimitiveList[lod][j].lpIndex = LYNXCORE_NEW unsigned short[m_MeshData->PrimitiveList[lod][j].NumIndex];
					s.vRead(m_MeshData->PrimitiveList[lod][j].lpIndex, sizeof(unsigned short), m_MeshData->PrimitiveList[lod][j].NumIndex);																
				}
			}
			LYNXCHAR MaterialNameStr[128];
			lynxFromUniStr(MaterialNameStr, MaterialName);
			SetlpCurrentMaterial(m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(CString(MaterialNameStr)));						
			if (!GetlpCurrentMaterial())
			{
				MaterialSystem::CMaterialPtr lpMat = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(_T("Null Material"));
				if (!lpMat)
				{
					m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpMat, MaterialSystem::CMaterial::STANDARD);
					lpMat->Default();
					lpMat->SetName(_T("Null Material"));							
				}
				SetlpCurrentMaterial(lpMat);
			}
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::ZPASS);

			for (int lod=0; lod<NumLODs; lod++)
				SetupBuffers(lod);
		}

		return LYNX_TRUE;
	}
}