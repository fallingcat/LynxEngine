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

#include <LynxSDK.h>
#include <AdvMaterialSystem/LynxAdvMaterialSystem.h>
#include <AdvMaterialSystem/LynxAdvMaterial.h>

namespace LynxEngine 
{	
	namespace AdvMaterialSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterial::CAdvMaterial(void)
		{		
			m_lpMaterialSystem = NULL;			
			m_bPlayed = LYNX_FALSE;			
			m_RenderableObjList.clear();
			m_ParamArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterial::CAdvMaterial(DWORD numpass)
		{				
			m_lpMaterialSystem = NULL;			
			m_bPlayed = LYNX_FALSE;
			m_RenderableObjList.clear();
			m_ParamArray.clear();

			CreatePass(numpass);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterial::CAdvMaterial(LYNXMATERIAL &m)
		{				
			m_lpMaterialSystem = NULL;
			m_bPlayed = LYNX_FALSE;		
			m_RenderableObjList.clear();
			m_ParamArray.clear();
			Clone(m);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterial::~CAdvMaterial(void)
		{
			vRelease();
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterial::vRelease(void)
		{
			m_MaterialPassArray.clear();		
			m_ParamArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterial& CAdvMaterial::operator =(CAdvMaterial& m)
		{				
			m_lpMaterialSystem = m.m_lpMaterialSystem;
			m_RenderableObjList = m.m_RenderableObjList;
			m_bPlayed = m.m_bPlayed;
			strcpy(m_Name, m.m_Name);
			m_MaterialPassArray.resize(m.m_MaterialPassArray.size());						
			for (int i=0; i<m_MaterialPassArray.size(); i++)
			{
				m_MaterialPassArray[i] = LYNXNEW CAdvMaterialPass(this);
				(*m_MaterialPassArray[i]) = (*m.m_MaterialPassArray[i]);
			}
			return *this;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CAdvMaterial::vLoad(const char *name, LPLYNXFILE lpf, long offset)
		{
			int			NumPass;
			char		MapName[64];
			LPLYNXFILE	lpFile;
			LYNXBLENDFUNCINDEX BlendFuncIndex;

			if (lpf)
			{
				lpFile = lpf;
			}
			else
			{
			}

			LYNX_READ_FILE(m_Name, sizeof(char), 64, lpFile);
			LYNX_READ_FILE(&NumPass, sizeof(DWORD), 1, lpFile);				
			CreatePass(NumPass);		
			for (int i=0; i<NumPass ;i++)
			{
				m_MaterialPassArray[i] = LYNXNEW AMS::CAdvMaterialPass(this);					
				
				LYNX_READ_FILE(&m_MaterialPassArray[i]->Diffuse, sizeof(LYNXCOLORRGBA), 1, lpFile);
				LYNX_READ_FILE(&m_MaterialPassArray[i]->Specular, sizeof(LYNXCOLORRGBA), 1, lpFile);
				LYNX_READ_FILE(&m_MaterialPassArray[i]->SpPower, sizeof(LYNXREAL), 1, lpFile);
				LYNX_READ_FILE(&m_MaterialPassArray[i]->bUseFog, sizeof(LYNXBOOL), 1, lpFile);
				LYNX_READ_FILE(&m_MaterialPassArray[i]->CullMode, sizeof(LYNXZBUFFERMODE), 1, lpFile);
				LYNX_READ_FILE(&m_MaterialPassArray[i]->ZBufferMode, sizeof(LYNXCULLMODE), 1, lpFile);
				LYNX_READ_FILE(&BlendFuncIndex, sizeof(LYNXBLENDFUNCINDEX), 1, lpFile);
				m_MaterialPassArray[i]->SrcBlend = lynxMapBlendFunc(BlendFuncIndex);
				LYNX_READ_FILE(&BlendFuncIndex, sizeof(LYNXBLENDFUNCINDEX), 1, lpFile);
				m_MaterialPassArray[i]->DestBlend = lynxMapBlendFunc(BlendFuncIndex);								
				
				for (int t=0; t<LYNX_MAX_USED_TEXTURE_CHANNEL; t++)
				{
					LPLYNXTEXTUREFRAME lpTF = m_MaterialPassArray[i]->GetlpTextureFrame(t);
					LYNX_READ_FILE(&lpTF->NumTexs, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);
					lpTF->lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*lpTF->NumTexs);
					lpTF->lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpTF->NumTexs);
					for (int j=0; j<lpTF->NumTexs; j++)
					{						
						LYNX_READ_FILE(lpTF->lplpTexList[j], sizeof(char), 64, lpFile);						
						//strcpy(lpTF->lplpTexList[j], "rock.tga");
						m_MaterialPassArray[i]->LoadTexture(t, j, lpTF->lplpTexList[j]);						
					}
					if (t == 1)
					{
						lpTF->NumTexs = 1;
						lpTF->lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*lpTF->NumTexs);
						lpTF->lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpTF->NumTexs);
						for (int j=0; j<lpTF->NumTexs; j++)
						{				
							strcpy(lpTF->lplpTexList[j], m_MaterialPassArray[i]->GetlpTextureFrame(0)->lplpTexList[0]);
							for (int s=0; s<strlen(lpTF->lplpTexList[j]); s++)
							{
								if (lpTF->lplpTexList[j][s] == '.')
									break;
							}
							strcpy(&lpTF->lplpTexList[j][s], "_bumpDOT3.tga");						
							m_MaterialPassArray[i]->LoadTexture(t, j, lpTF->lplpTexList[j]);						
						}
					}
					else if (t == 2)
					{
						lpTF->NumTexs = 1;
						lpTF->lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*lpTF->NumTexs);
						lpTF->lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpTF->NumTexs);
						strcpy(lpTF->lplpTexList[j], "white.tga");						
						m_MaterialPassArray[i]->LoadTexture(t, j, lpTF->lplpTexList[j]);						
					}

					LYNX_READ_FILE(&lpTF->StartTime, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);
					LYNX_READ_FILE(&lpTF->EndTime, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);
					LYNX_READ_FILE(&lpTF->Interval, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);

					LYNX_READ_FILE(&lpTF->PlayMode, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);
					LYNX_READ_FILE(&lpTF->MipMapLevel, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);
					LYNX_READ_FILE(&lpTF->MinFilter, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);
					LYNX_READ_FILE(&lpTF->MagFilter, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);
					LYNX_READ_FILE(&lpTF->MipFilter, sizeof(LYNXTEXTUREADDRESSMODE), 1, lpFile);
					LYNX_READ_FILE(&BlendFuncIndex, sizeof(LYNXBLENDFUNCINDEX), 1, lpFile);
					lpTF->SrcBlend = lynxMapBlendFunc(BlendFuncIndex);
					LYNX_READ_FILE(&BlendFuncIndex, sizeof(LYNXBLENDFUNCINDEX), 1, lpFile);
					lpTF->DestBlend = lynxMapBlendFunc(BlendFuncIndex);													
				}					
			}	
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CAdvMaterial::LoadShaders(int pass, const char *vsname, const char *psname)
		{
			if (vsname)
				lynxLoadCompiledVertexShader(&m_MaterialPassArray[pass]->VS, vsname, NULL, 0);									
			if (psname)
				lynxLoadCompiledPixelShader(&m_MaterialPassArray[pass]->PS, psname, NULL, 0);					

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterial::CreatePass(DWORD numpass)
		{				
			m_MaterialPassArray.resize(numpass);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterial::vPlayAll(LYNXREAL skip, LYNXPLAYMODE mode)
		{
			for (int i=0; i<m_MaterialPassArray.size(); i++)
			{
				m_MaterialPassArray[i]->vPlayAll(skip, mode);
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAdvMaterial::vPlay(int channel, LYNXREAL skip, LYNXPLAYMODE mode)
		{
			LYNXBOOL			bRet = LYNX_TRUE;

			for (int i=0; i<m_MaterialPassArray.size(); i++)
			{
				bRet &= m_MaterialPassArray[i]->vPlay(channel, skip, mode);
			}
			return bRet;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterial::Clone(LYNXMATERIAL &m)
		{
			strcpy(m_Name, m.Name);
			CreatePass(m.NumPass);			
			for (int i=0; i<m_MaterialPassArray.size(); i++)
			{
				m_MaterialPassArray[i] = LYNXNEW CAdvMaterialPass(this);
				(*m_MaterialPassArray[i]) = m.lpMaterialPass[i];
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterial::vInit(void)
		{
			for (int i=0; i<m_MaterialPassArray.size(); i++)
			{
				m_MaterialPassArray[i]->vInit();
			}
		}				
	}
}