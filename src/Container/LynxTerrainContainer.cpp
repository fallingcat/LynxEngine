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
#include <LynxFileStream.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxScnRenderer.h>
#include <Container/LynxTerrainContainer.h>
#include <Shader/LynxTerrainVS.h>
#include <Shader/LynxTerrainSimplePS.h>
#include <Shader/LynxTerrainGPassVS.h>
#include <Shader/LynxTerrainGPassPS.h>
#include <Shader/LynxTerrainShadowMapVS.h>
#include <Shader/LynxShadowMapPS.h>
#include <Shader/LynxTerrainZPassVS.h>
#include <Shader/LynxZPass_velocityPS.h>
#include <Shader/LynxTerrainSimpleLightingVS.h>
#include <Shader/LynxTerrainSimpleLightingPS.h>
#include <CollisionSystem/LynxCollisionHierarchy.h>

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
	CTerrainContainerSegment::CTerrainContainerSegment()
	{
		m_lpTerrain = NULL;
		m_TextureAnimationArray.clear();
		m_AttachedObjectList.clear();

		IMPLEMENT_PARAM_ARRAY(m_TextureAnimationArray, CParam::COUNTEDPTR, 0)
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
	CTerrainContainerSegment::CTerrainContainerSegment(CTerrainContainer* c)
	{
		m_lpTerrain = c;
		m_TextureAnimationArray.clear();
		m_AttachedObjectList.clear();

		IMPLEMENT_PARAM_ARRAY(m_TextureAnimationArray, CParam::COUNTEDPTR, 0)
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
	CTerrainContainerSegment::~CTerrainContainerSegment()
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
	void CTerrainContainerSegment::ComputeBoundary()
	{
		int SegmentSize = m_lpTerrain->GetSegmentSize();
		int Index = 0;
		int Dir = 1;
		int CurrentX = 0;
		CVector3* lpV = LYNXGLOBAL_NEW CVector3[SegmentSize*SegmentSize*3*2];
		for (int z=0; z<SegmentSize; z++)
		{
			for (int x=0; x<SegmentSize; x++)
			{
				lpV[Index++] = m_lpTerrain->GetVertexList(0)[m_PrimitiveList[0].BaseVertex + z*(m_lpTerrain->GetWidth()+1) + CurrentX].Pos;
				lpV[Index++] = m_lpTerrain->GetVertexList(0)[m_PrimitiveList[0].BaseVertex + (z+1)*(m_lpTerrain->GetWidth()+1) + CurrentX].Pos;
				lpV[Index++] = m_lpTerrain->GetVertexList(0)[m_PrimitiveList[0].BaseVertex + z*(m_lpTerrain->GetWidth()+1) + CurrentX + 1].Pos;

				lpV[Index++] = m_lpTerrain->GetVertexList(0)[m_PrimitiveList[0].BaseVertex + z*(m_lpTerrain->GetWidth()+1) + CurrentX + 1].Pos; 
				lpV[Index++] = m_lpTerrain->GetVertexList(0)[m_PrimitiveList[0].BaseVertex + (z+1)*(m_lpTerrain->GetWidth()+1) + CurrentX].Pos;
				lpV[Index++] = m_lpTerrain->GetVertexList(0)[m_PrimitiveList[0].BaseVertex + (z+1)*(m_lpTerrain->GetWidth()+1) + CurrentX + 1].Pos;				

				CurrentX += Dir;
			}
			Dir *= -1;
			CurrentX += Dir;
		}
		m_AABB.Set(lpV, SegmentSize*SegmentSize*3*2);
		LYNXGLOBAL_DEL_ARRAY(lpV);
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
	void CTerrainContainerSegment::Create(int x, int y)
	{
		int SegmentSize, NumX, NumY;
		int	Width, Height;
			
		SegmentSize = m_lpTerrain->GetSegmentSize();

		Width = m_lpTerrain->GetWidth();
		Height = m_lpTerrain->GetHeight();

		m_PrimitiveList.resize(1);
		m_PrimitiveList[0].NumIndex = (SegmentSize*SegmentSize)*2*3;
		m_PrimitiveList[0].Type = GraphicsSystem::CRenderer::TRIANGLE_LIST;
		m_PrimitiveList[0].NumPrimitives = (SegmentSize*SegmentSize)*2;
		m_PrimitiveList[0].BaseIndex = 0;
		m_PrimitiveList[0].BaseVertex = (y*SegmentSize)*(Width+1)+(x*SegmentSize);			

		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		SamDesc;
		SamDesc.Default();
		SamDesc.AddressU = GraphicsSystem::CRenderer::AM_WRAP;
		SamDesc.AddressV = GraphicsSystem::CRenderer::AM_WRAP;
		SamDesc.AddressW = GraphicsSystem::CRenderer::AM_WRAP;
		SamDesc.Filter	 = GraphicsSystem::CRenderer::FM_MIN_POINT_MAG_LINEAR_MIP_POINT;
		
		ComputeBoundary();

		/*if (x%2)
		{
			m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
			m_TextureAnimationArray[0]->Create(1);				
			m_TextureAnimationArray[0]->SetTextureName(0, LynxEngine::CString(m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/grass001.png"));
			m_TextureAnimationArray[0]->SetShaderConstantName("TerrainLayer0");
			m_TextureAnimationArray[0]->vCreateAnimation();
			m_TextureAnimationArray[0]->LoadTexture(0, 0, _T("terrain/grass001.png"), NULL, 0);			

			m_TextureAnimationArray[1]->Create(1);					
			m_TextureAnimationArray[1]->SetTextureName(0, LynxEngine::CString(m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/layer1.png"));
			m_TextureAnimationArray[1]->SetShaderConstantName("TerrainLayer1");
			m_TextureAnimationArray[1]->vCreateAnimation();
			m_TextureAnimationArray[1]->LoadTexture(0, 0, _T("terrain/layer1.png"), NULL, 0);		

			m_TextureAnimationArray[2]->Create(1);					
			m_TextureAnimationArray[2]->SetTextureName(0, LynxEngine::CString(m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/layer2.png"));
			m_TextureAnimationArray[2]->SetShaderConstantName("TerrainLayer2");
			m_TextureAnimationArray[2]->vCreateAnimation();
			m_TextureAnimationArray[2]->LoadTexture(0, 0, _T("terrain/layer2.png"), NULL, 0);

			m_TextureAnimationArray[3]->Create(1);		
			m_TextureAnimationArray[3]->SetTextureName(0, LynxEngine::CString(m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/layer3.png"));
			m_TextureAnimationArray[3]->SetShaderConstantName("TerrainLayer3");
			m_TextureAnimationArray[3]->vCreateAnimation();
			m_TextureAnimationArray[3]->LoadTexture(0, 0, _T("terrain/layer3.png"), NULL, 0);	
		}
		else
		{
			m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
			m_TextureAnimationArray[0]->Create(1);				
			m_TextureAnimationArray[0]->SetTextureName(0, LynxEngine::CString(m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/grass001.png"));
			m_TextureAnimationArray[0]->SetShaderConstantName("TerrainLayer0");
			m_TextureAnimationArray[0]->vCreateAnimation();
			m_TextureAnimationArray[0]->LoadTexture(0, 0, _T("terrain/grass001.png"), NULL, 0);			

			m_TextureAnimationArray[1]->Create(1);					
			m_TextureAnimationArray[1]->SetTextureName(0, LynxEngine::CString(m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/layer1.png"));
			m_TextureAnimationArray[1]->SetShaderConstantName("TerrainLayer1");
			m_TextureAnimationArray[1]->vCreateAnimation();
			m_TextureAnimationArray[1]->LoadTexture(0, 0, _T("terrain/layer1.png"), NULL, 0);		

			m_TextureAnimationArray[2]->Create(1);					
			m_TextureAnimationArray[2]->SetTextureName(0, LynxEngine::CString(m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/grass003.png"));
			m_TextureAnimationArray[2]->SetShaderConstantName("TerrainLayer2");
			m_TextureAnimationArray[2]->vCreateAnimation();
			m_TextureAnimationArray[2]->LoadTexture(0, 0, _T("terrain/grass003.png"), NULL, 0);

			m_TextureAnimationArray[3]->Create(1);		
			m_TextureAnimationArray[3]->SetTextureName(0, LynxEngine::CString(m_lpTerrain->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/grass004.png"));
			m_TextureAnimationArray[3]->SetShaderConstantName("TerrainLayer3");
			m_TextureAnimationArray[3]->vCreateAnimation();
			m_TextureAnimationArray[3]->LoadTexture(0, 0, _T("terrain/grass004.png"), NULL, 0);	
		}*/
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
	GraphicsSystem::CVertexLayoutPtr CTerrainContainer::m_VertexLayout;	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CTerrainContainer::CTerrainContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_ResourceType = CONTAINER;
		m_ContainerType = TERRAIN;	
		m_lpHeightMap = NULL;
		m_VertexList = NULL;
		m_TriangleList = NULL;
		m_PrimitiveList = NULL;	
		
		CreateVertexLayout();

		for (int i=0; i<MAX_TERRAIN_LAYERS; i++)
		{
			m_uTile[i] = 50.0f;
			m_vTile[i] = 50.0f;
		}
		IMPLEMENT_PARAM_NATIVE_ARRAY(m_uTile, CParam::FLOAT, MAX_TERRAIN_LAYERS, CParam::PARAM_SIZE_READONLY)
		IMPLEMENT_PARAM_NATIVE_ARRAY(m_vTile, CParam::FLOAT, MAX_TERRAIN_LAYERS, CParam::PARAM_SIZE_READONLY) 
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
	CTerrainContainer::~CTerrainContainer(void)
	{		
		for (int i=0; i<m_SegmentList.size(); i++)
			LYNXDEL m_SegmentList[i];
		m_SegmentList.clear();

		for (int i=0; i<m_NumLODs; i++)
		{
			m_PrimitiveList[i].clear();
			m_VertexList[i].clear();	
			m_TriangleList[i].clear();
			//lynxReleaseVertexArray(&m_VertexArray[i]);
			//lynxReleaseIndexArray(&m_IndexArray[i]);
		}
		LYNXGLOBAL_DEL_ARRAY(m_VertexList);
		LYNXGLOBAL_DEL_ARRAY(m_TriangleList);
		LYNXGLOBAL_DEL_ARRAY(m_PrimitiveList);
		m_VertexArray.clear();
		m_IndexArray.clear();
		LYNXGLOBAL_DEL_ARRAY(m_lpHeightMap);
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
	DWORD CTerrainContainer::vSystemMemorySize() 
	{
		return (DWORD)(sizeof(*this)+(m_Name.length()*sizeof(LYNXCHAR)) + 
			           (sizeof(Math::CReal)*m_HeightMapWidth*m_HeightMapHeight) ); 
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
	GraphicsSystem::CVertexLayout* CTerrainContainer::CreateVertexLayout(CEngine* e)
	{
		GraphicsSystem::CRenderer::VERTEXSLOT Slots[8];

		Slots[0].Stream		= 0;	Slots[0].Type	= GraphicsSystem::CRenderer::VST_POSITION;		Slots[0].TypeIndex	= 0;	Slots[0].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[0].Number		= 3;  // Position in object space 
		Slots[1].Stream		= 0;	Slots[1].Type	= GraphicsSystem::CRenderer::VST_TANGENT;		Slots[1].TypeIndex	= 0;	Slots[1].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[1].Number		= 3;  // Tangent in object space	
		Slots[2].Stream		= 0;	Slots[2].Type	= GraphicsSystem::CRenderer::VST_NORMAL;		Slots[2].TypeIndex	= 0;	Slots[2].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[2].Number		= 3;  // Normal in object space		
		Slots[3].Stream		= 0;	Slots[3].Type	= GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[3].TypeIndex	= 0;	Slots[3].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[3].Number		= 2;  // Texture coordinate 0			 
		Slots[4].Stream		= 0;	Slots[4].Type	= GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[4].TypeIndex	= 1;	Slots[4].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[4].Number		= 2;  // Texture coordinate 1 
		Slots[5].Stream		= 0;	Slots[5].Type	= GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[5].TypeIndex	= 2;	Slots[5].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[5].Number		= 2;  // Texture coordinate 2 
		Slots[6].Stream		= 0;	Slots[6].Type	= GraphicsSystem::CRenderer::VST_TEX_COORD;		Slots[6].TypeIndex	= 3;	Slots[6].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[6].Number		= 2;  // Texture coordinate 3
		Slots[7].Stream		= 0;	Slots[7].Type	= GraphicsSystem::CRenderer::VST_BLEND_WEIGHT;	Slots[7].TypeIndex	= 0;	Slots[7].Format		= GraphicsSystem::CRenderer::VSF_FLOAT;	Slots[7].Number		= 4;  // Texture weight			 			
		GraphicsSystem::CVertexLayout* v = e->GetlpGraphicsSystem()->GetlpRenderer()->CreateVertexLayout();
		v->vCreate(Slots, 8);
		v->SetName(CString(_T("Terrain Vertex Layout")));

		return v;		
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
	void CTerrainContainer::CreateVertexLayout(void)
	{
		if (!m_VertexLayout && m_lpEngine->GetlpGraphicsSystem() && m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer())
		{
			m_lpEngine->CreateVertexLayout(m_VertexLayout, CString(_T("Terrain Vertex Layout")));			
		}
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
	Math::CReal CTerrainContainer::GetHeight(CVector3& pos)
	{
		float fx = (pos.x+m_HeightMapWidth*m_HeightMapCellSize*0.5f)/m_HeightMapCellSize;
		float fy = (pos.z+m_HeightMapHeight*m_HeightMapCellSize*0.5f)/m_HeightMapCellSize;				
		int x0, x1, y0, y1, h;					

		x0 = floor(fx); x1 = x0+1;
		y0 = floor(fy); y1 = y0+1;

		fx = fx-x0;
		fy = fy-y0;						
		if (x0 >= 0 && x0 <m_HeightMapWidth && y0 >= 0 && y0 <m_HeightMapHeight)
		{
			float h0 = HeightMap(y0, x0) + fx*(HeightMap(y0, x1) - HeightMap(y0, x0));
			float h1 = HeightMap(y1, x0) + fx*(HeightMap(y1, x1) - HeightMap(y1, x0));
			h = h0 + fy * (h1 - h0);
			//h = HeightMap(y0, x0) + t * (HeightMap(y1, x1) - HeightMap(y0, x0));
		}
		else
			h = 0.0f;

		return h;
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
	Math::CReal CTerrainContainer::GetHeight(const CReal& x, const CReal& z)
	{
		CVector3 Pos(x, 0, z);
		return GetHeight(Pos);
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
	Math::CReal& CTerrainContainer::GetHeightFromVertexIndex(int y, int x)
	{
		int Times = m_CellSize / m_HeightMapCellSize;

		return m_lpHeightMap[(y*(m_HeightMapWidth+1)+x)*Times];
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
	void CTerrainContainer::GetOrientation(CMatrix3& m, CVector3& pos)
	{
		CVector3 Normal, Tangent, Binormal;
		GetNormal(Normal, pos);
		GetTangent(Tangent, pos);
		Binormal = Math::Cross(Tangent, Normal);

		m.m[0][0] = Tangent.x;	m.m[1][0] = Tangent.y;	m.m[2][0] = Tangent.z;
		m.m[0][1] = Normal.x;	m.m[1][1] = Normal.y;	m.m[2][1] = Normal.z;
		m.m[0][2] = Binormal.x;	m.m[1][2] = Binormal.y;	m.m[2][2] = Binormal.z;		

		m.m[0][0] = Tangent.x;	m.m[0][1] = Tangent.y;	m.m[0][2] = Tangent.z;
		m.m[1][0] = Normal.x;	m.m[1][1] = Normal.y;	m.m[1][2] = Normal.z;
		m.m[2][0] = Binormal.x;	m.m[2][1] = Binormal.y;	m.m[2][2] = Binormal.z;		
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
	void CTerrainContainer::GetNormal(CVector3& normal, CVector3& pos)
	{
		float fx = (pos.x+m_Width*m_CellSize*0.5f)/m_CellSize;
		float fy = (pos.z+m_Height*m_CellSize*0.5f)/m_CellSize;				
		int x0, x1, y0, y1, h;					

		x0 = floor(fx); x1 = x0+1;
		y0 = floor(fy); y1 = y0+1;

		fx = fx-x0;
		fy = fy-y0;
		if (x0 >= 0 && x0 <m_Width && y0 >= 0 && y0 <m_Height)
		{
			CVector3 n0 = CVector3(m_VertexList[m_CurrentLOD][y0*(m_Width+1)+x0].Normal) + (CVector3(m_VertexList[m_CurrentLOD][y0*(m_Width+1)+x1].Normal) - CVector3(m_VertexList[m_CurrentLOD][y0*(m_Width+1)+x0].Normal))*fx;
			CVector3 n1 = CVector3(m_VertexList[m_CurrentLOD][y1*(m_Width+1)+x0].Normal) + (CVector3(m_VertexList[m_CurrentLOD][y1*(m_Width+1)+x1].Normal) - CVector3(m_VertexList[m_CurrentLOD][y1*(m_Width+1)+x0].Normal))*fx;			
			normal = n0 + (n1 - n0)*fy;			
			normal.Normalise();
		}
		else
		{
			normal.x = 0.0f;
			normal.y = 1.0f;
			normal.z = 0.0f;
		}
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
	void CTerrainContainer::GetTangent(CVector3& tangent, CVector3& pos)
	{
		float fx = (pos.x+m_Width*m_CellSize*0.5f)/m_CellSize;
		float fy = (pos.z+m_Height*m_CellSize*0.5f)/m_CellSize;				
		int x0, x1, y0, y1, h;					

		x0 = floor(fx); x1 = x0+1;
		y0 = floor(fy); y1 = y0+1;

		fx = fx-x0;
		fy = fy-y0;
		if (x0 >= 0 && x0 <m_Width && y0 >= 0 && y0 <m_Height)
		{
			CVector3 n0 = CVector3(m_VertexList[m_CurrentLOD][y0*(m_Width+1)+x0].Tangent) + (CVector3(m_VertexList[m_CurrentLOD][y0*(m_Width+1)+x1].Tangent) - CVector3(m_VertexList[m_CurrentLOD][y0*(m_Width+1)+x0].Tangent))*fx;
			CVector3 n1 = CVector3(m_VertexList[m_CurrentLOD][y1*(m_Width+1)+x0].Tangent) + (CVector3(m_VertexList[m_CurrentLOD][y1*(m_Width+1)+x1].Tangent) - CVector3(m_VertexList[m_CurrentLOD][y1*(m_Width+1)+x0].Tangent))*fx;			
			tangent = n0 + (n1 - n0)*fy;			
			tangent.Normalise();
		}
		else
		{
			tangent.x = 1.0f;
			tangent.y = 0.0f;
			tangent.z = 0.0f;
		}
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
	void CTerrainContainer::UpdateTextureCoord(int lod)
	{
		for (int i=0; i<=m_Height; i++)
		{
			for (int j=0; j<=m_Width; j++)
			{
				m_VertexList[lod][i*(m_Width+1)+j].TexCoord[0].u = ((float)j/m_Width)*(float)m_uTile[0];
				m_VertexList[lod][i*(m_Width+1)+j].TexCoord[0].v = ((float)i/m_Height)*(float)m_vTile[0];

				m_VertexList[lod][i*(m_Width+1)+j].TexCoord[1].u = ((float)j/m_Width)*(float)m_uTile[1];
				m_VertexList[lod][i*(m_Width+1)+j].TexCoord[1].v = ((float)i/m_Height)*(float)m_vTile[1];

				m_VertexList[lod][i*(m_Width+1)+j].TexCoord[2].u = ((float)j/m_Width)*(float)m_uTile[2];
				m_VertexList[lod][i*(m_Width+1)+j].TexCoord[2].v = ((float)i/m_Height)*(float)m_vTile[2];

				m_VertexList[lod][i*(m_Width+1)+j].TexCoord[3].u = ((float)j/m_Width)*(float)m_uTile[3];
				m_VertexList[lod][i*(m_Width+1)+j].TexCoord[3].v = ((float)i/m_Height)*(float)m_vTile[3];
			}
		}		
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
	void CTerrainContainer::UpdateVertexArrayTexCoord(int lod)
	{
		TERRAINVERTEX* lpV = reinterpret_cast<TERRAINVERTEX*>(m_VertexArray[lod]->vMap(0, 0, GraphicsSystem::CRenderer::LOCK_TO_WRITE));			
		for (int j=0; j<m_VertexList[lod].size(); j++)
		{	
			for (int k=0; k<MAX_TERRAIN_LAYERS; k++)
			{
				lpV[j].TexCoord[k].u = m_VertexList[lod][j].TexCoord[k].u;
				lpV[j].TexCoord[k].v = m_VertexList[lod][j].TexCoord[k].v;																		
			}
		}		
		m_VertexArray[lod]->vUnmap();
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
	void CTerrainContainer::ComputeBoundary()
	{
		CollisionSystem::CAABB* pAABB;
		
		// Add Collision Hierarchy level 0		
		if (m_lpCollisionHierarchy->GetNumLevels() == 0)	
		{
			m_lpCollisionHierarchy->Create(1);
			pAABB = LYNXNEW CollisionSystem::CAABB();		
			m_lpCollisionHierarchy->Add(0, pAABB);		
		}
		else
			pAABB = (CollisionSystem::CAABB*)m_lpCollisionHierarchy->GetCollisionObjList(0)[0];		

		pAABB->Set(&m_VertexList[0][0].Pos, sizeof(VERTEX), (int)m_VertexList[0].size());		

		for (int i=0; i<m_SegmentList.size(); i++)
			m_SegmentList[i]->ComputeBoundary();
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
	void CTerrainContainer::CreateDefaultMaterial(MaterialSystem::CMaterialPtr& lpm, CString& name)
	{
		CPlatformFileStream TexFileStream;
		GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;

		m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpm, MaterialSystem::CMaterial::STANDARD);			
		lpm->SetName(name);			
		
		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
		lpm->vSetNumTextureAnimation(6);	
		lpm->GetTextureAnimation(0)->Create(1);				
		//lpm->GetTextureAnimation(0)->SetTextureName(0, LynxEngine::CString(m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath())+_T("terrain/layer0.png"));
		lpm->GetTextureAnimation(0)->SetShaderConstantName("TerrainLayer0");
		TexDesc.Default();
		TexDesc.bSRGB = LYNX_TRUE;
		//if (!lpm->GetTextureAnimation(0)->LoadTexture(0, _T("terrain/layer0.png")))						
		if (!lpm->GetTextureAnimation(0)->LoadTexture(0, TexFileStream, _T("checker.png")))						
		{
			lpm->GetTextureAnimation(0)->LoadTexture(0, TexFileStream, _T("white.png"), &TexDesc);			
		}		

		lpm->GetTextureAnimation(1)->Create(1);					
		lpm->GetTextureAnimation(1)->SetShaderConstantName("TerrainLayer1");
		if (!lpm->GetTextureAnimation(1)->LoadTexture(0, TexFileStream, _T("terrain/layer1.png"), &TexDesc))						
		{
			lpm->GetTextureAnimation(1)->LoadTexture(0, TexFileStream, _T("white.png"));			
		}
		
		lpm->GetTextureAnimation(2)->Create(1);					
		lpm->GetTextureAnimation(2)->SetShaderConstantName("TerrainLayer2");
		if (!lpm->GetTextureAnimation(2)->LoadTexture(0, TexFileStream, _T("terrain/layer2.png"), &TexDesc))
		{
			lpm->GetTextureAnimation(2)->LoadTexture(0, TexFileStream, _T("white.png"));
		}
		
		lpm->GetTextureAnimation(3)->Create(1);		
		lpm->GetTextureAnimation(3)->SetShaderConstantName("TerrainLayer3");
		if (!lpm->GetTextureAnimation(3)->LoadTexture(0, TexFileStream, _T("terrain/layer3.png"), &TexDesc))						
		{
			lpm->GetTextureAnimation(3)->LoadTexture(0, TexFileStream, _T("white.png"));			
		}

		lpm->GetTextureAnimation(4)->Create(1);		
		lpm->GetTextureAnimation(4)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::NORMALMAP).c_str());
		//lpm->GetTextureAnimation(4)->LoadTexture(0, _T("checker_NM.png"));			
		lpm->GetTextureAnimation(4)->LoadTexture(0, TexFileStream, _T("dot3.png"));			

		lpm->GetTextureAnimation(5)->Create(1);		
		lpm->GetTextureAnimation(5)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DETAILMAP).c_str());
		//lpm->GetTextureAnimation(5)->LoadTexture(0, _T("perlin_noise_nm.png"));
		lpm->GetTextureAnimation(5)->LoadTexture(0, TexFileStream, _T("black.png"));		
		
		lpm->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
		lpm->vAddTechnique(MaterialSystem::CMaterial::WIREFRAME);				
		lpm->vAddTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);				
		lpm->vAddTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);				
		lpm->vAddTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);				
		lpm->vAddTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);				
		lpm->vAddTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);				
		lpm->vAddTechnique(MaterialSystem::CMaterial::ZPASS);				
		lpm->vAddTechnique(MaterialSystem::CMaterial::SHADOWMAP);				
		lpm->vAddTechnique(MaterialSystem::CMaterial::GPASS);				

		lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).vCreatePass(1);
		LynxEngine::MaterialSystem::CPass& Pass = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0);
		Pass.m_TextureAnimationArray = lpm->GetTextureAnimationArray();
		/*LynxEngine::CParam* PP;
		LynxEngine::CList<LynxEngine::CParamGroup*>& GroupList = Pass.m_TextureAnimationArray[0]->GetParamManager().GetParamGroupList();
		LynxEngine::CList<LynxEngine::CParamGroup*>::CIterator Group;
		for (Group = GroupList.begin(); Group != GroupList.end(); ++Group)
		{
			LynxEngine::CList<LynxEngine::CParam*>& ParamList = (*Group)->GetParamList();
			LynxEngine::CList<LynxEngine::CParam*>::CIterator Param;
			for (Param = ParamList.begin(); Param != ParamList.end(); ++Param)
			{
				PP = (*Param);				
			}
		}*/
		
		LynxEngine::GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC Desc;
		LynxEngine::GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
		LynxEngine::GraphicsSystem::CRenderer::RASTERIZERSTATEDESC RDesc;
		LynxEngine::GraphicsSystem::CRenderer::SAMPLERSTATEDESC SamDesc;

		Desc.Default();
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		lpm->SetVertexLayout(m_VertexLayout);

		LynxEngine::MaterialSystem::CTechnique* Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE);
		Technique->GetPass(0).vCreateShaderClasses("TerrainVS", "TerrainSimplePS");
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_CW;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);
	
		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);
		Technique->GetPass(0).m_bUseFog = LYNX_FALSE;			

		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE_LIGHTING);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainSimpleLightingVS", "TerrainSimpleLightingPS");			
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_CW;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);

		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ZPASS);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainZPassVS", "ZPass_VelocityPS");	
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_CW;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);

		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SHADOWMAP);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainShadowMapVS", "ShadowMapPS");	
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_CW;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
		BDesc.RTBlendParam[1].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[1].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[1].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[1].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
		BDesc.RTBlendParam[2].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[2].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[2].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[2].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
		BDesc.RTBlendParam[3].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[3].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[3].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[3].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);

		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::GPASS);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainGPassVS", "TerrainGPassPS");			
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_C;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);


		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::WIREFRAME);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainVS", "DiffuseMapXColorPS");	
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_CW;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_WIREFRAME;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);


		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::AMBIENT_LIGHTING);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainVS", "DiffuseMapXColorPS");	
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_C;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);


		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::POINT_LIGHTING);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainVS", "DiffuseMapXColorPS");	
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_C;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;		
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);
		

		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SPOT_LIGHTING);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainVS", "DiffuseMapXColorPS");	
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_C;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);

		
		Technique = &lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);
		Technique->vCreatePass(1);
		Technique->GetPass(0).m_TextureAnimationArray = lpm->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray;
		Technique->GetPass(0).vCreateShaderClasses("TerrainVS", "DiffuseMapXColorPS");	
		//Technique->GetPass(0).vLoadShaders(_T("scn/geometry_spotlight.vso"), _T("scn/geometry_spotlight.pso"));
		Desc.DepthMode = LynxEngine::GraphicsSystem::CRenderer::DB_C;
		Desc.DepthFunc = LynxEngine::GraphicsSystem::CRenderer::TF_LEQUAL;			
		Technique->GetPass(0).CreateDepthStencilState(Desc);

		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
		BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
		Technique->GetPass(0).CreateBlendState(BDesc);

		RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
		RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CCW;
		Technique->GetPass(0).CreateRasterizerState(RDesc);		
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
	LYNXBOOL CTerrainContainer::vCreate(const CCreationDesc* desc)
	{		
		CString MaterialName;		

		LYNX_ASSERT(desc);

		CreateVertexLayout();

		CTerrainCreationDesc* TDesc = (CTerrainCreationDesc*)desc;			

		if (TDesc->m_bCreateMaterial)
		{
			if (TDesc->m_MaterialName == _T(""))
			{
				MaterialName = m_Name+CString(_T(".Material"));				
			}
			else
			{
				MaterialName = TDesc->m_MaterialName;
			}			
			MaterialSystem::CMaterialPtr lpMat = ((CEngine *)m_lpEngine)->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MaterialName); 
			if ((MaterialSystem::CMaterial*)lpMat == NULL)
			{
				CreateDefaultMaterial(lpMat, MaterialName);
			}
			lpMat->SetVertexLayout(m_VertexLayout);
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);		

			//temp code
			MaterialSystem::CTechnique* Technique;
			LynxEngine::GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC Desc;		
			LynxEngine::GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
			LynxEngine::GraphicsSystem::CRenderer::RASTERIZERSTATEDESC RDesc;

			Desc.Default();		
			BDesc.Default();
			RDesc.Default();

			Technique = &m_lpCurrentMaterial->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SHADOWMAP);
			RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
			RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_CW;
			Technique->GetPass(0).CreateRasterizerState(RDesc);


			lpMat->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("TerrainVS", "TerrainSimplePS");	
			lpMat->GetTechnique(MaterialSystem::CMaterial::GPASS).GetPass(0).vCreateShaderClasses("TerrainGPassVS", "TerrainGPassPS");					
			lpMat->GetTechnique(MaterialSystem::CMaterial::SHADOWMAP).GetPass(0).vCreateShaderClasses("TerrainShadowMapVS", "ShadowMapPS");			
			lpMat->GetTechnique(MaterialSystem::CMaterial::ZPASS).GetPass(0).vCreateShaderClasses("TerrainZPassVS", "ZPass_VelocityPS");	
			lpMat->GetTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING).GetPass(0).vCreateShaderClasses("TerrainSimpleLightingVS", "TerrainSimpleLightingPS");	
			//temp code				
		}

		if (TDesc)
		{
			m_HeightMapWidth = TDesc->m_HeightmapWidth;
			m_HeightMapHeight = TDesc->m_HeightmapHeight;
			m_HeightMapCellSize = TDesc->m_HeightmapCellSize;

			m_CellSize = TDesc->m_TerrainMeshCellSize;
			float Times = m_HeightMapCellSize/m_CellSize;
			m_Width = m_HeightMapWidth*Times;
			m_Height = m_HeightMapHeight*Times;		

			m_CurrentLOD = 0;
			m_lpHeightMap = LYNXGLOBAL_NEW Math::CReal[(m_HeightMapWidth+1)*(m_HeightMapHeight+1)];
			vSetNumLODs(1);
			m_VertexList[m_CurrentLOD].resize((m_Width+1)*(m_Height+1));

			for (int i=0; i<=m_HeightMapHeight; i++)
			{
				for (int j=0; j<=m_HeightMapWidth; j++)
				{
					m_lpHeightMap[i*(m_HeightMapWidth+1)+j] = 0.0f;
				}
			}	

			for (int i=0; i<=m_Height; i++)
			{
				for (int j=0; j<=m_Width; j++)
				{
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].Pos.x = j*m_CellSize - m_Width*m_CellSize*0.5f;
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].Pos.y = GetHeightFromVertexIndex(i, j);
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].Pos.z = i*m_CellSize - m_Height*m_CellSize*0.5f;

					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].Normal.x = 0.0f;
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].Normal.y = 1.0;
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].Normal.z = 0.0f;
					
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].TexWeight[0] = 1.0f;
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].TexWeight[1] = 0.0f;
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].TexWeight[2] = 0.0f;
					m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].TexWeight[3] = 0.0f;				
				}
			}		

			UpdateTextureCoord(m_CurrentLOD);

			/*for (int i=0; i<m_Height; i++)
			{
				m_PrimitiveList[m_CurrentLOD][i].NumIndex = (m_Width+1)*2;
				m_PrimitiveList[m_CurrentLOD][i].Type = GraphicsSystem::CRenderer::TRIANGLE_STRIP;
				m_PrimitiveList[m_CurrentLOD][i].NumPrimitives = m_PrimitiveList[m_CurrentLOD][i].NumIndex-2;			
				m_PrimitiveList[m_CurrentLOD][i].BaseVertex = i*(m_Width+1);
				m_PrimitiveList[m_CurrentLOD][i].BaseIndex = 0;
				m_PrimitiveList[m_CurrentLOD][i].lpIndex = LYNXNEW unsigned short [m_PrimitiveList[m_CurrentLOD][i].NumIndex];
				for (int j=0; j<=m_Width; j++)
				{
					m_PrimitiveList[m_CurrentLOD][i].lpIndex[j*2] = j;
					m_PrimitiveList[m_CurrentLOD][i].lpIndex[j*2+1] = j + (m_Width+1);
				}			
			}	*/				
			vCreateSegments(TDesc->m_SegmentSize);	

			ComputeBoundary();
		}

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CTerrainContainer::vCreateSegments(int size)
	{
		int NumX, NumY, LOD;
		int	Width, Height;
			
		m_SegmentSize = size;
		LOD = 0;
		
		NumX = m_Width/m_SegmentSize;
		if (m_Width%m_SegmentSize)
			NumX++;
		NumY = m_Height/m_SegmentSize;
		if (m_Height%m_SegmentSize)
			NumY++;

		m_SegmentList.resize(NumX*NumY);		

		LYNXCHAR Buffer[256];
		// arrange segement this way to utilize vertex cache
		for (int x=0; x<NumX; x++)
		{
			for (int y=0; y<NumY; y++)
			{
				m_SegmentList[y*NumX+x] = LYNXNEW CTerrainContainerSegment(this);				
				lynxSprintf(Buffer, _T(".Segment[%d][%d]"), x, y);
				m_SegmentList[y*NumX+x]->SetName(GetName()+Buffer);
				m_SegmentList[y*NumX+x]->Create(x, y);				
			}
		}	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CTerrainContainer::vInstance(CContainer& rhs)
	{		
		CContainer::vInstance(rhs);
		CTerrainContainer& RHS = Container_Cast<CTerrainContainer&>(rhs);
		m_VertexList = RHS.m_VertexList;
		m_TriangleList = RHS.m_TriangleList;
		m_PrimitiveList = RHS.m_PrimitiveList;
		m_VertexArray = RHS.m_VertexArray;
		m_IndexArray = RHS.m_IndexArray;
		m_SourceType = CResource::INSTANCE;
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
	void CTerrainContainer::vSetNumLODs(int lod)
	{
		m_NumLODs = lod;
		m_VertexList = LYNXGLOBAL_NEW CArray<VERTEX>[lod];
		m_TriangleList = LYNXGLOBAL_NEW CArray<TRIANGLE>[lod];
		m_PrimitiveList = LYNXGLOBAL_NEW CArray<GraphicsSystem::CRenderer::PRIMITIVE>[lod];
		m_VertexArray.resize(m_NumLODs);
		m_IndexArray.resize(m_NumLODs);
		for (int i=0; i<m_NumLODs; i++)
		{
			m_lpEngine->CreateResource(m_VertexArray[i], VERTEX_ARRAY);
			m_lpEngine->CreateResource(m_IndexArray[i], INDEX_ARRAY);			
		}
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
	LYNXBOOL CTerrainContainer::SetupBuffers(void)
	{
		for (int i=0; i<=m_Height; i++)
		{
			for (int j=0; j<=m_Width; j++)
			{
				m_VertexList[m_CurrentLOD][i*(m_Width+1)+j].Pos.y = GetHeightFromVertexIndex(i, j);
			}
		}		
		UpdateTextureCoord(m_CurrentLOD);

		for (int i=0; i<m_NumLODs; i++)
		{
			TERRAINVERTEX* lpV = LYNXGLOBAL_NEW TERRAINVERTEX[m_VertexList[i].size()];
			for (int j=0; j<m_VertexList[i].size(); j++)
			{	
				lpV[j].Pos.x = m_VertexList[i][j].Pos.x;
				lpV[j].Pos.y = m_VertexList[i][j].Pos.y;
				lpV[j].Pos.z = m_VertexList[i][j].Pos.z;

				lpV[j].Tangent.x = m_VertexList[i][j].Tangent.x;
				lpV[j].Tangent.y = m_VertexList[i][j].Tangent.y;
				lpV[j].Tangent.z = m_VertexList[i][j].Tangent.z;

				//lpV[j].Binormal.x = m_VertexList[i][j].Binormal.x;
				//lpV[j].Binormal.y = m_VertexList[i][j].Binormal.y;
				//lpV[j].Binormal.z = m_VertexList[i][j].Binormal.z;

				lpV[j].Normal.x = m_VertexList[i][j].Normal.x;
				lpV[j].Normal.y = m_VertexList[i][j].Normal.y;
				lpV[j].Normal.z = m_VertexList[i][j].Normal.z;

				for (int k=0; k<MAX_TERRAIN_LAYERS; k++)
				{
					lpV[j].TexCoord[k].u = m_VertexList[i][j].TexCoord[k].u;
					lpV[j].TexCoord[k].v = m_VertexList[i][j].TexCoord[k].v;							
					lpV[j].TexWeight[k] = m_VertexList[i][j].TexWeight[k];			
				}				
			}
			m_VertexArray[i]->vCreate(m_VertexLayout, (int)m_VertexList[i].size(), GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpV);
			LYNXGLOBAL_DEL_ARRAY(lpV);
			
			// Arrange indices to utilize vertex cache
			DWORD *lpI = LYNXGLOBAL_NEW DWORD[(m_SegmentSize*m_SegmentSize)*6];			
			int Index = 0;
			int Dir = 1;
			int CurrentX = 0;
			for (int z=0; z<m_SegmentSize; z++)
			{
				for (int x=0; x<m_SegmentSize; x++)
				{
					lpI[Index++] = z*(m_Width+1) + CurrentX;
					lpI[Index++] = (z+1)*(m_Width+1) + CurrentX;
					lpI[Index++] = z*(m_Width+1) + CurrentX + 1;

					lpI[Index++] = z*(m_Width+1) + CurrentX + 1; 
					lpI[Index++] = (z+1)*(m_Width+1) + CurrentX;
					lpI[Index++] = (z+1)*(m_Width+1) + CurrentX + 1;

					CurrentX += Dir;
				}
				Dir *= -1;
				CurrentX += Dir;
			}
			m_IndexArray[i]->vCreate(sizeof(DWORD), (m_SegmentSize*m_SegmentSize)*6, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, LYNX_TRUE, lpI);		
			LYNXGLOBAL_DEL_ARRAY(lpI);
		}	

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
	LYNXBOOL CTerrainContainer::SetupHeightFieldShapeDesc(PhysicsSystem::CHeightFieldShapeDesc* desc, int lod)
	{
		desc->m_NumX	= m_Width+1;
		desc->m_NumZ	= m_Height+1;
		desc->m_SizeX	= m_Width*m_CellSize;
		desc->m_SizeZ	= m_Height*m_CellSize;
		desc->m_HeightMap = LYNXGLOBAL_NEW CReal[desc->m_NumX * desc->m_NumZ];

		for (int i=0; i<=m_Height; i++)
		{
			for (int j=0; j<=m_Width; j++)
			{
				desc->m_HeightMap[i*(m_Width+1)+j] = m_lpHeightMap[i*(m_Width+1)+j];
			}
		}	

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
	void CTerrainContainer::ReleaseHeightFieldShapeDesc(PhysicsSystem::CHeightFieldShapeDesc* desc)
	{
		LYNXGLOBAL_DEL_ARRAY(desc->m_HeightMap);
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
	void CTerrainContainer::AttachObject(CRenderableObj* o)
	{
		Math::CVector3 Pos, Max, Min;

		o->GetPosition(Pos);
		for (int i=0; i<m_SegmentList.size(); i++)
		{
			Max = m_SegmentList[i]->m_AABB.Max();
			Min = m_SegmentList[i]->m_AABB.Min();
			if (Pos.x >= Min.x && Pos.x <= Max.x &&
				Pos.y >= Min.y && Pos.y <= Max.y &&
				Pos.z >= Min.z && Pos.z <= Max.z )
			{
				m_SegmentList[i]->AttachObject(o);
			}
		}								
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
	void CTerrainContainer::RemoveAttachedObject(CRenderableObj* o)
	{
		for (int i=0; i<m_SegmentList.size(); i++)
		{
			m_SegmentList[i]->RemoveAttachedObject(o);			
			o->SetAttachedTerrain(NULL);
		}
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
	void CTerrainContainer::SetupRNRPlane(void)
	{
		LYNXVECTOR3D V1, V2;
		LYNX_VECTOR3D_SUB(V1, m_VertexList[0][m_TriangleList[0][0].v2].Pos, m_VertexList[0][m_TriangleList[0][0].v1].Pos);
		LYNX_VECTOR3D_SUB(V2, m_VertexList[0][m_TriangleList[0][0].v3].Pos, m_VertexList[0][m_TriangleList[0][0].v1].Pos);
		lynxCrossProduct(&m_RNRPlane.Normal, &V1, &V2);
		lynxNormalise(&m_RNRPlane.Normal, 3);						
		m_RNRPlane.Distance = LYNX_DOT_PRODUCT(m_RNRPlane.Normal, m_VertexList[0][m_TriangleList[0][0].v1].Pos);						
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
	void CTerrainContainer::ComputeVis(CCameraContainer* cam)
	{
		CVector3 CPos;
		
		cam->vGetPosition(CPos);
		for (int i=0; i<m_SegmentList.size(); i++)
		{
			m_SegmentList[i]->m_bVisible = LYNX_TRUE;						
		}
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
	void CTerrainContainer::vRender(CCameraContainer* cam)
	{	
		GraphicsSystem::CRenderer* lpR = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer();

		ComputeVis(cam);

		lpR->SetVertexArray(m_VertexArray[m_CurrentLOD], 0, 0);
		lpR->SetIndexArray(m_IndexArray[m_CurrentLOD], 0);	

		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)
		{
			Technique.GetPass(p).Set(this, cam);				
			for (int i=0; i<m_SegmentList.size(); i++)			
			{
				if (m_SegmentList[i]->m_bVisible)
				{
					for (int t=0; t<GetlpCurrentMaterial()->GetNumTextureAnimation(); t++)
					{
						GetlpCurrentMaterial()->GetTextureAnimation(t)->vSet(lpR->GetCachedPixelShader(), 0);
					}
					// Overwrite the texture animation array of material
					// 覆寫材質的貼圖動畫
					for (int t=0; t<m_SegmentList[i]->m_TextureAnimationArray.size(); t++)
					{
					#ifdef __TOOL__
						if (m_SegmentList[i]->m_TextureAnimationArray[t])
					#endif
							m_SegmentList[i]->m_TextureAnimationArray[t]->vSet(lpR->GetCachedPixelShader(), 0);
					}
					m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();

					for (int j=0; j<m_SegmentList[i]->m_PrimitiveList.size(); j++)
					{
						lpR->DrawPrimitiveBO(&m_SegmentList[i]->m_PrimitiveList[j], (m_Width+1)*(m_SegmentSize+1));
					}
				}
			}
		}					
	}
}