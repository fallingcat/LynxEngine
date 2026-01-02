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

#include <AdvMaterialSystem/LynxAdvMaterialPass.h>
#include <AdvMaterialSystem/LynxAdvMaterialSystem.h>

namespace LynxEngine 
{	
	namespace AdvMaterialSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterialPass::CAdvMaterialPass(void)
		{			
			lynxInitMaterialPass((LPLYNXMATERIALPASS)this);					
			m_lpMaterial = NULL;
			m_ParamArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterialPass::CAdvMaterialPass(CAdvMaterial* lpm)
		{				
			lynxInitMaterialPass((LPLYNXMATERIALPASS)this);					
			m_lpMaterial = lpm;
			m_ParamArray.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterialPass::CAdvMaterialPass(LYNXMATERIALPASS &m)
		{				
			lynxInitMaterialPass((LPLYNXMATERIALPASS)this);			
			lynxDuplicateMaterialPass(this, &m);					
			m_lpMaterial = NULL;
			m_ParamArray.clear();
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterialPass::~CAdvMaterialPass(void)
		{
			vRelease();
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialPass::vRelease(void)
		{
			lynxReleaseMaterialPass((LPLYNXMATERIALPASS)this);
			m_ParamArray.clear();
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterialPass& CAdvMaterialPass::operator =(const CAdvMaterialPass& mp)
		{		
			lynxDuplicateMaterialPass(this, (LPLYNXMATERIALPASS)&mp);				
			m_lpMaterial = mp.m_lpMaterial;		
			//m_ParamArray = mp.m_ParamArray;

			return *this;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAdvMaterialPass& CAdvMaterialPass::operator =(const LYNXMATERIALPASS& mp)
		{		
			lynxDuplicateMaterialPass(this, (LPLYNXMATERIALPASS)&mp);						

			return *this;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialPass::CreateTextureFrame(int channel, int num)
		{
			TextureFrame[channel].NumTexs = num;
			TextureFrame[channel].lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*TextureFrame[channel].NumTexs);
			TextureFrame[channel].lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*TextureFrame[channel].NumTexs);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialPass::LoadTexture(int channel, int frame, const char *filename)
		{
			lynxLoadTexture(&TextureFrame[channel].lpTexIndexList[frame], NULL, 0, filename, NULL, 0, LYNX_NORMAL_TEXTURE);		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialPass::vPlayAll(LYNXREAL skip, LYNXPLAYMODE mode)
		{
			for (int i=0; i<LYNX_MAX_USED_TEXTURE_CHANNEL; i++)
			{
				if (TextureFrame[i].NumTexs)
				{
					lynxPlayTextureFrame(&TextureFrame[i], skip, mode);								
				}													
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAdvMaterialPass::vPlay(int channel, LYNXREAL skip, LYNXPLAYMODE mode)
		{
			return lynxPlayTextureFrame(&TextureFrame[channel], skip, mode);											
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialPass::vInit(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialPass::Set(void)
		{
			lynxSetUseFog(bUseFog);
			lynxSetZBufferMode(ZBufferMode);
			if (CullMode == LYNX_CULL_NONE)
			{
				lynxSetHardwareCullMode(CullMode);
			}
			else
			{
				//if (m_lpMaterialSystem && ((CAdvMaterialSystem *)m_lpMaterialSystem)->IsRenderingReflection())
				//{
				//	lynxSetHardwareCullMode(LYNX_CULL_CW);
				//}
				//else
				{
					lynxSetHardwareCullMode(CullMode);
				}
			}
			if (SpPower > 0.0f)
			{
				lynxSetUseSpecular(LYNX_TRUE);
			}
			else
			{
				lynxSetUseSpecular(LYNX_FALSE);
			}						

			//VS = NULL;
			//PS = NULL;
			
			if (VS)
			{				
				lynxSetVertexShader(VS);						
				lynxSetVertexLayout(&VertexLayout);
			}
			else
			{
				lynxSetVertexShader(VS);						
				lynxSetFixedVertexShader(FixedVS);
			}

			lynxSetPixelShader(PS);									
			if (!PS)
			{
				for (int i=0; i<LYNX_MAX_USED_TEXTURE_CHANNEL; i++)
				{
					if (TextureFrame[i].NumTexs)
					{
						lynxSetTexture(i, TextureFrame[i].lpTexIndexList[(int)TextureFrame[i].Time]);	
						lynxSetTextureAddressMode(i, AddressMode[i]);
						lynxSetTextureFilterMode(i, TextureFrame[i].MinFilter, TextureFrame[i].MagFilter, TextureFrame[i].MipFilter); 
						lynxSetTextureBlend(i, CombineFunc[i], CombineArg[i][0], CombineArg[i][1], LYNX_CA_TEXTURE);													
					}
					else
					{				
						lynxSetTextureBlend(i, LYNX_CF_NONE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);													
						break;
					}								
				}					
			}
			else
			{
				for (int i=0; i<LYNX_MAX_USED_TEXTURE_CHANNEL; i++)				
				{
					if (TextureFrame[i].NumTexs)
					{
						lynxSetTexture(i, TextureFrame[i].lpTexIndexList[(int)TextureFrame[i].Time]);	
						lynxSetTextureAddressMode(i, AddressMode[i]);
						lynxSetTextureFilterMode(i, TextureFrame[i].MinFilter, TextureFrame[i].MagFilter, TextureFrame[i].MipFilter); 
					}							
					else
					{				
						lynxSetTextureBlend(i, LYNX_CF_NONE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);													
						break;
					}						
				}				
			}					
			lynxSetBlendFunc(SrcBlend, DestBlend);			

			vSetParameter();						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAdvMaterialPass::vSetParameter(void)
		{
			LYNX_ASSERT(m_lpMaterial);
			
			lynxSetVertexShaderConstantTable(VS, "gLightPos", &(m_lpMaterial->m_LightPos.s.x), 3);							

			float Color[3];

			Color[0] = m_lpMaterial->m_Color.s.Red/255.0f;
			Color[1] = m_lpMaterial->m_Color.s.Green/255.0f;
			Color[2] = m_lpMaterial->m_Color.s.Blue/255.0f;

			lynxSetPixelShaderConstantTable(PS, "gLightColor", Color, 3);
			lynxSetPixelShaderConstantTable(PS, "gRadius", &(m_lpMaterial->m_Radius), 1);								
		}
	}
}