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

#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterialPhase.h>
#include <MaterialSystem/LynxMaterialPass.h>

namespace LynxEngine 
{	
	namespace MaterialSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhase::CPhase(void)
		{		
			m_Type = NULL_PHASE;
			m_lpMaterial = NULL;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhase::CPhase(CMaterial* lpmat, DWORD numpass)
		{	
			m_Type = NULL_PHASE;
			m_lpMaterial = lpmat;			
			if (numpass)
				CreatePass(numpass);
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhase::~CPhase(void)
		{
			vRelease();
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhase::vRelease(void)
		{
			m_PassArray.clear();		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhase& CPhase::operator =(const CPhase& m)
		{				
			m_Type = m.m_Type;
			m_lpMaterial = m.m_lpMaterial;
			lynxStrCpy(m_Name, m.m_Name);
			m_PassArray.resize(m.m_PassArray.size());						
			for (int i=0; i<m_PassArray.size(); i++)
			{
				m_PassArray[i] = LYNXNEW CPass(this);
				(*m_PassArray[i]) = (*m.m_PassArray[i]);
			}
			return *this;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CPhase::vLoad(const LYNXCHAR *name, LPLYNXFILE lpf, long offset)
		{
			DWORD		NumPass;
			char		AnsiName[64];	
			char		MapName[64];
			LYNXCHAR	UniMapName[64];	
			LPLYNXFILE	lpFile;
			LYNXBLENDFUNCINDEX BlendFuncIndex;

			if (lpf)
			{
				lpFile = lpf;
			}
			else
			{
			}

			//LYNX_READ_FILE(AnsiName, sizeof(char), 64, lpFile);
			//AnsiName[63] = 0x00;
			//lynxUniStr(m_Name, AnsiName);			
			LYNX_READ_FILE(&NumPass, sizeof(DWORD), 1, lpFile);				
			CreatePass(NumPass);		
			for (int i=0; i<NumPass ;i++)
			{
				m_PassArray[i] = LYNXNEW MaterialSystem::CPass(this);					
				
				LYNX_READ_FILE(&m_PassArray[i]->m_Diffuse, sizeof(LYNXCOLORRGBA), 1, lpFile);
				LYNX_READ_FILE(&m_PassArray[i]->m_Specular, sizeof(LYNXCOLORRGBA), 1, lpFile);
				LYNX_READ_FILE(&m_PassArray[i]->m_SpPower, sizeof(LYNXREAL), 1, lpFile);
				LYNX_READ_FILE(&m_PassArray[i]->m_bUseFog, sizeof(LYNXBOOL), 1, lpFile);
				LYNX_READ_FILE(&m_PassArray[i]->m_CullMode, sizeof(LYNXZBUFFERMODE), 1, lpFile);
				LYNX_READ_FILE(&m_PassArray[i]->m_ZBufferMode, sizeof(LYNXCULLMODE), 1, lpFile);
				LYNX_READ_FILE(&BlendFuncIndex, sizeof(LYNXBLENDFUNCINDEX), 1, lpFile);
				m_PassArray[i]->m_SrcBlend = lynxMapBlendFunc(BlendFuncIndex);
				LYNX_READ_FILE(&BlendFuncIndex, sizeof(LYNXBLENDFUNCINDEX), 1, lpFile);
				m_PassArray[i]->m_DestBlend = lynxMapBlendFunc(BlendFuncIndex);								
				
				m_PassArray[i]->m_ChannelParam.resize(3);
				for (int t=0; t<LYNX_MAX_USED_TEXTURE_CHANNEL; t++)
				{
					int NumTexs;
					LYNX_READ_FILE(&NumTexs, sizeof(int), 1, lpFile);
					if (NumTexs)
					{
						m_PassArray[i]->m_ChannelParam[t].TextureAnimation.Create(NumTexs);					
						for (int j=0; j<NumTexs; j++)
						{						
							LYNX_READ_FILE(MapName, sizeof(char), 64, lpFile);						
							lynxUniStr(UniMapName, MapName);
							//lynxStrCpy(lpTF->lplpTexList[j], "rock.tga");
							m_PassArray[i]->m_ChannelParam[t].TextureAnimation.LoadTexture(j, 0, UniMapName, NULL, 0, LYNX_NORMAL_TEXTURE);						
						}
						m_PassArray[i]->m_ChannelParam[t].AddressMode = LYNX_ADDRESS_WRAP;
					}
					if (t == 1)
					{
						m_PassArray[i]->m_ChannelParam[t].TextureAnimation.Create(1);					
						for (int j=0; j<1; j++)
						{				
							for (int s=0; s<lynxStrLen(UniMapName); s++)
							{
								if (UniMapName[s] == _T('.'))
									break;
							}
							lynxStrCpy(&UniMapName[s], _T("_bumpDOT3.tga"));						
							m_PassArray[i]->m_ChannelParam[t].TextureAnimation.LoadTexture(j, 0, UniMapName, NULL, 0, LYNX_NORMAL_TEXTURE);						
						}
						m_PassArray[i]->m_ChannelParam[t].AddressMode = LYNX_ADDRESS_WRAP;
					}
					else if (t == 2)
					{						
						m_PassArray[i]->m_ChannelParam[t].TextureAnimation.Create(1);					
						lynxUniStr(UniMapName, MapName);
						if (!lynxStriCmp(UniMapName, "G-2-new_D.tga"))
							lynxStrCpy(UniMapName, _T("G-2-new_S.tga"));						
						else
							lynxStrCpy(UniMapName, _T("white.tga"));						
						m_PassArray[i]->m_ChannelParam[t].TextureAnimation.LoadTexture(0, 0, UniMapName, NULL, 0, LYNX_NORMAL_TEXTURE);						
						m_PassArray[i]->m_ChannelParam[t].AddressMode = LYNX_ADDRESS_WRAP;
					}					
					LYNX_SEEK_FILE(lpFile, sizeof(LYNXTEXTUREADDRESSMODE), LYNX_SEEK_CUR);
					LYNX_SEEK_FILE(lpFile, sizeof(LYNXTEXTUREADDRESSMODE), LYNX_SEEK_CUR);
					LYNX_SEEK_FILE(lpFile, sizeof(LYNXTEXTUREADDRESSMODE), LYNX_SEEK_CUR);
					LYNX_SEEK_FILE(lpFile, sizeof(LYNXTEXTUREADDRESSMODE), LYNX_SEEK_CUR);

					LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);
					LYNX_SEEK_FILE(lpFile, sizeof(LYNXTEXTUREADDRESSMODE), LYNX_SEEK_CUR);
					LYNX_SEEK_FILE(lpFile, sizeof(LYNXTEXTUREADDRESSMODE), LYNX_SEEK_CUR);
					LYNX_SEEK_FILE(lpFile, sizeof(LYNXTEXTUREADDRESSMODE), LYNX_SEEK_CUR);
					LYNX_READ_FILE(&BlendFuncIndex, sizeof(LYNXBLENDFUNCINDEX), 1, lpFile);
					//lpTF->SrcBlend = lynxMapBlendFunc(BlendFuncIndex);
					LYNX_READ_FILE(&BlendFuncIndex, sizeof(LYNXBLENDFUNCINDEX), 1, lpFile);
					//lpTF->DestBlend = lynxMapBlendFunc(BlendFuncIndex);																		
				}					
			}	
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CPhase::LoadShaders(int pass, const LYNXCHAR *vsname, const LYNXCHAR *psname)
		{			
			if (vsname)
			{
				lynxReleaseVertexShader(m_PassArray[pass]->m_VS);
				lynxLoadCompiledVertexShader(&m_PassArray[pass]->m_VS, vsname, NULL, 0);									
			}
			if (psname)
			{
				lynxReleasePixelShader(m_PassArray[pass]->m_PS);
				lynxLoadCompiledPixelShader(&m_PassArray[pass]->m_PS, psname, NULL, 0);					
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhase::CreatePass(DWORD numpass)
		{				
			m_PassArray.resize(numpass);	
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPhase::vPlayAll(LYNXREAL skip, LYNXPLAYMODE mode)
		{
			for (int i=0; i<m_PassArray.size(); i++)
			{
				m_PassArray[i]->vPlayAll(skip, mode);
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPhase::vPlay(int channel, LYNXREAL skip, LYNXPLAYMODE mode)
		{
			LYNXBOOL			bRet = LYNX_TRUE;

			for (int i=0; i<m_PassArray.size(); i++)
			{
				bRet &= m_PassArray[i]->vPlay(channel, skip, mode);
			}
			return bRet;
		}		
	}
}