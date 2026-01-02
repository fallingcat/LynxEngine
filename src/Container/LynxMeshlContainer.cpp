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
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <LynxRenderableObj.h>
#include <Container/LynxMeshContainer.h>

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
	CMeshContainer::CMeshContainer()		
	{
		m_Type = MESH;		
		m_lpModel = NULL;	
		m_DynamicAttr = 0;	
		for (int i=0; i<MAX_LOD; i++)
		{
			m_NumVertices[i] = 0;	
			m_NumTriangles[i] = 0;		
		}	
		m_BoneShaderData.clear();
		m_Primitives.clear();
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
	CMeshContainer::CMeshContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_Type = MESH;		
		m_lpModel = NULL;	
		m_DynamicAttr = 0;	
		for (int i=0; i<MAX_LOD; i++)
		{
			m_NumVertices[i] = 0;	
			m_NumTriangles[i] = 0;		
		}	
		m_BoneShaderData.clear();
		m_Primitives.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CMeshContainer& CMeshContainer::operator =(CMeshContainer& m)
	{	
		m_lpCurrentMaterial = m.m_lpCurrentMaterial;

		return (*this);
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Rendering function.
	*
	*	@param none.	
	*	@return none. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CMeshContainer::vRender(void)
	{	
		LYNXMATRIX WVPM, WVM;

		if (m_Visibility)
		{	
			lynxSetVertexArray(&m_VertexArray, 0, 0);
			lynxSetIndexArray(&m_IndexArray, 0);

			lynxMatrixXMatrix(&WVM, lynxGetIdentityMatrix(), &lynxGetlpRenderer()->ViewMatrix);	
			lynxMatrixXMatrix(&WVPM, &WVM, &lynxGetlpRenderer()->ProjectionMatrix);				

			for (int p=0; p<m_lpCurrentMaterial->GetNumPass(); ++p)
			{
				m_lpCurrentMaterial->GetMaterialPass(p).Set();
				lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldViewProj", &WVPM);
				lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldView", &WVM);	
				lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorld", lynxGetIdentityMatrix());	
				lynxSetPixelShaderConstantTableB(lynxGetCurrentPixelShader(), "gbGlow", &m_bGlow, 1);
				lynxSetPixelShaderConstantTable(lynxGetCurrentPixelShader(), "gBloomingIntensity", &m_BloomingIntensity, 1);					
				//m_Obj3D.Render(m_lpMesh);					
			}
		}
	}
}