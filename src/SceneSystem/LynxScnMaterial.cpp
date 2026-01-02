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

#include <SceneSystem/LynxScnMaterial.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSceneMaterial::CSceneMaterial(void)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSceneMaterial::~CSceneMaterial(void)
		{
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSceneMaterial::vLoad(LPLYNXFILE lpf)
		{		
			char					Path[256];
		
			LYNX_READ_FILE(&m_MaterialPassArray[0]->m_Diffuse, sizeof(LYNXCOLORRGBA), 1, lpf);
			LYNX_READ_FILE(&m_MaterialPassArray[0]->m_Specular, sizeof(LYNXCOLORRGBA), 1, lpf);		
			LYNX_READ_FILE(&m_MaterialPassArray[0]->m_TextureFrame[0].MipMapLevel, sizeof(WORD), 1, lpf);														
			m_MaterialPassArray[0]->m_TextureFrame[0].MipMapLevel = 0;
			LYNX_READ_FILE(&m_MaterialPassArray[0]->m_TextureFrame[0].NumTexs, sizeof(int), 1, lpf);					
			if (m_MaterialPassArray[0]->m_TextureFrame[0].NumTexs > 0)
			{
				m_MaterialPassArray[0]->m_TextureFrame[0].lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*m_MaterialPassArray[0]->m_TextureFrame[0].NumTexs);
				m_MaterialPassArray[0]->m_TextureFrame[0].lpTexIndexList = LYNXNEW LYNXTEXTUREINDEX[m_MaterialPassArray[0]->m_TextureFrame[0].NumTexs];
				for (int j=0; j<m_MaterialPassArray[0]->m_TextureFrame[0].NumTexs; j++)
				{
					LYNX_READ_FILE(m_MaterialPassArray[0]->m_TextureFrame[0].lplpTexList[j], sizeof(char), 64, lpf);	
					lynxLoadTexture(&m_MaterialPassArray[0]->m_TextureFrame[0].lpTexIndexList[j], NULL, m_MaterialPassArray[0]->m_TextureFrame[0].MipMapLevel, m_MaterialPassArray[0]->m_TextureFrame[0].lplpTexList[j], NULL, 0, LYNX_NORMAL_TEXTURE);									
				}
			}			
			m_MaterialPassArray[0]->m_bUseFog = LYNX_TRUE;							
			m_MaterialPassArray[0]->LoadVS("buildin/scn/staticobj_simple_lighting.vso", NULL, 0);									
			m_MaterialPassArray[0]->LoadPS("buildin/scn/staticobj_simple_lighting.pso", NULL, 0);					
		}		
	}
}