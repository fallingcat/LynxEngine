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

#include <LynxEngine.h>
#include <GraphicsSystem/LynxTerrainVertex.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		static const LYNXVERTEXSLOT gTerrainVertexSlot[] = {				
											{ 0, LYNX_VST_POSITION,		0, LYNX_VSF_FLOAT, 3 },	// Position in object space
											{ 0, LYNX_VST_TANGENT,		0, LYNX_VSF_FLOAT, 3 },	// Tangent in object space											
											//{ 0, LYNX_VST_BINORMAL,	 0, LYNX_VSF_FLOAT, 3 },	// Binormal in object space											
											{ 0, LYNX_VST_NORMAL,		0, LYNX_VSF_FLOAT, 3 },	// Normal in object space											
											{ 0, LYNX_VST_TEX_COORD,	0, LYNX_VSF_FLOAT, 2 },	// Texture coordinate 0											
											{ 0, LYNX_VST_TEX_COORD,	1, LYNX_VSF_FLOAT, 2 },	// Texture coordinate 0											
											{ 0, LYNX_VST_TEX_COORD,	2, LYNX_VSF_FLOAT, 2 },	// Texture coordinate 0											
											{ 0, LYNX_VST_TEX_COORD,	3, LYNX_VSF_FLOAT, 2 },	// Texture coordinate 0											
											{ 0, LYNX_VST_BLEND_WEIGHT, 0, LYNX_VSF_FLOAT, 4 },	// Texture coordinate 0											
		};
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTerrainVertex::CTerrainVertex(void)
		{
			//m_VertexSlot.resize( = LYNX
			//m_lpVertexSlot = &gTerrainVertexSlot[0];
			lynxMakeVertexLayout(&m_Layout, (LPLYNXVERTEXSLOT)gTerrainVertexSlot, LYNX_ARRAY_SIZE(gTerrainVertexSlot));	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTerrainVertex::CTerrainVertex(CGraphicsSystem *pgs)
		: CVertexDesc(pgs)
		{	
			lynxMakeVertexLayout(&m_Layout, (LPLYNXVERTEXSLOT)gTerrainVertexSlot, LYNX_ARRAY_SIZE(gTerrainVertexSlot));	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTerrainVertex::~CTerrainVertex(void)
		{
			Release();
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTerrainVertex::Release()
		{			
		}		
	}
}
