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

#include <AdvMaterialSystem/LynxAdvMaterialSystem.h>

namespace LynxEngine 
{	
	namespace AdvMaterialSystem 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterialSystem::CAdvMaterialSystem(void)
		{				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterialSystem::~CAdvMaterialSystem(void)
		{			
			Release();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAdvMaterialSystem::Init(void)
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialSystem::Release(void)
		{			
		}							
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialSystem::SetGlobalParam()
		{
			lynxMatrixXMatrix(&m_WVM, &lynxGetlpRenderer()->WorldMatrix, &lynxGetlpRenderer()->ViewMatrix);	
			lynxMatrixXMatrix(&m_WVPM, &m_WVM, &lynxGetlpRenderer()->ProjectionMatrix);	
			lynxSetVertexShaderConstantTableMatrix(1, "gWorldViewProj", &m_WVPM);
			lynxSetVertexShaderConstantTableMatrix(1, "gWorldView", &m_WVM);	
			lynxSetVertexShaderConstantTableMatrix(1, "gWorld", &lynxGetlpRenderer()->WorldMatrix);	
			lynxSetSprite3DViewMatrix(&lynxGetlpRenderer()->ViewMatrix);
		}				
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void CAdvMaterialSystem::SetFogParam(LPLYNXPLANE lpp, LPLYNXFOGDATA lpfog1, LPLYNXFOGDATA lpfog2)
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
		CAdvMaterial* const CAdvMaterialSystem::FindMaterial(const char *name)
		{
			CList<CAdvMaterial*>::CIterator Mat;

			for (Mat = m_MaterialList.begin(); Mat != m_MaterialList.end(); ++Mat)
			{
				if (!lynxStrCmp((char *)(*Mat)->GetName(), name))
					return (*Mat);				
			}
			return NULL;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialSystem::AddMaterial(CAdvMaterial *lpmat)
		{
			lpmat->m_lpMaterialSystem = this;
			m_MaterialList.push_back(lpmat);			
		}				
	}
}