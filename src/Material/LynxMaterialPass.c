//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Lynx SDK Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//  Compiled with Visual C++ 7.0. Best viewed with Tabs = 4.
//
//  Created by Owen Wu : 2003/07/06
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//
//###########################################################################

#include <LynxMaterial.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMaterial 
 *  @brief 。
 *
 *	@param 。
 *	@param 。lynxDuplicateMaterial
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInitMaterialPass(LPLYNXMATERIALPASS lpm)
{
	int								i;

	lpm->bDisable = LYNX_FALSE;	
	lpm->bGlow = LYNX_FALSE;
	lpm->Diffuse.PackedColor = lpm->Specular.PackedColor = 0xffffffff;
	lpm->SpPower = LYNX_REAL(0);
	lpm->bUseFog = LYNX_FALSE;
	lpm->ZBufferMode = LYNX_ZB_CW;
	lpm->CullMode = LYNX_CULL_CCW;
	lpm->SrcBlend = LYNX_BF_SRC_ALPHA;
	lpm->DestBlend = LYNX_BF_ONE_MINUS_SRC_ALPHA;
	lpm->FixedVS = LYNX_VERTEX_TYPE;
	lpm->VS = LYNX_NULL_VERTEXSHADER;
	lpm->PS = LYNX_NULL_PIXELSHADER;
	for (i=0; i<LYNX_MAX_USED_TEXTURE_CHANNEL; i++)
	{
		lynxInitTextureFrame(&lpm->TextureFrame[i]);
		lpm->CombineFunc[i] = LYNX_CF_MODULATE;
		lpm->AddressMode[i] = LYNX_ADDRESS_WRAP;
		lpm->CombineArg[i][0] = LYNX_CA_CURRENT;
		lpm->CombineArg[i][1] = LYNX_CA_TEXTURE;		
	}
	lynxDuplicateVertexLayout(&lpm->VertexLayout, lynxGetBuiltinVertexLayout(LYNX_RENDER_VERTEX));	
	lpm->lpSetParamProc = NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMaterial 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxDuplicateMaterialPass(LPLYNXMATERIALPASS lpdm, LPLYNXMATERIALPASS lpsm)
{
	int								i;

	lynxInitMaterialPass(lpdm);
	lpdm->Diffuse.PackedColor = lpsm->Diffuse.PackedColor;
	lpdm->Specular.PackedColor = lpsm->Specular.PackedColor;
	lpdm->SpPower = lpsm->SpPower;
	lpdm->bUseFog = lpsm->bUseFog;
	lpdm->ZBufferMode = lpsm->ZBufferMode;
	lpdm->CullMode = lpsm->CullMode;
	lpdm->SrcBlend = lpsm->SrcBlend;
	lpdm->DestBlend = lpsm->DestBlend;
	lpdm->FixedVS = lpsm->FixedVS;
	lynxDuplicateVertexShader(&lpdm->VS, lpsm->VS);
	lynxDuplicatePixelShader(&lpdm->PS, lpsm->PS);	
	for (i=0; i<LYNX_MAX_USED_TEXTURE_CHANNEL; i++)
	{
		lynxDuplicateTextureFrame(&lpdm->TextureFrame[i], &lpsm->TextureFrame[i]);
		lpdm->CombineFunc[i] = lpsm->CombineFunc[i];
		lpdm->AddressMode[i] = lpsm->AddressMode[i];
		lpdm->CombineArg[i][0] = lpsm->CombineArg[i][0];
		lpdm->CombineArg[i][1] = lpsm->CombineArg[i][1];		
	}
	lynxDuplicateVertexLayout(&lpdm->VertexLayout, &lpsm->VertexLayout);
	lpdm->bDisable = lpsm->bDisable;
	lpdm->bGlow = lpsm->bGlow;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMaterial 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxReleaseMaterialPass(LPLYNXMATERIALPASS lpm)
{	
	int							i, j;

	lynxReleaseVertexShader(lpm->VS);
	lynxReleasePixelShader(lpm->PS);
	for (i=0; i<LYNX_MAX_USED_TEXTURE_CHANNEL; i++)
	{
		for (j=0; j<lpm->TextureFrame[i].NumTexs; j++)
		{
			lynxReleaseTexture(lpm->TextureFrame[i].lpTexIndexList[j]);		
		}
		LYNX_FREE(lpm->TextureFrame[i].lplpTexList);
		LYNX_FREE(lpm->TextureFrame[i].lpTexIndexList);
	}		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMaterial 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxLoadMaterialPass(LPLYNXMATERIALPASS lpm, LPLYNXFILE lpf)
{	
	int					i, bLoop, MipMapLevel;
	
	/*
	LYNX_READ_FILE(lpm->Name, sizeof(char), 64, lpf);		
	LYNX_READ_FILE(&lpm->NumTexs, sizeof(int), 1, lpf);		
	if (lpm->NumTexs <= 1)
	{
		for (i=0; i<lpm->NumTexs; i++)
		{
			LYNX_READ_FILE(lpm->TextureName, sizeof(char), 64, lpf);		
		}
	}
	else
	{
		lpm->lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpm->NumTexs);
		lpm->lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*lpm->NumTexs);
		for (i=0; i<lpm->NumTexs; i++)
		{
			LYNX_READ_FILE(lpm->lplpTexList[i], sizeof(char), 64, lpf);		
		}
	}
	LYNX_READ_FILE(&lpm->StartTime, sizeof(float), 1, lpf);		
	LYNX_READ_FILE(&lpm->EndTime, sizeof(float), 1, lpf);		
	lpm->EndTime -= 1.0f;
	LYNX_READ_FILE(&lpm->Interval, sizeof(float), 1, lpf);		
	LYNX_READ_FILE(&bLoop, sizeof(int), 1, lpf);				
	lpm->bLoop = bLoop;		
	LYNX_READ_FILE(&MipMapLevel, sizeof(int), 1, lpf);		
	lpm->MipMapLevel = MipMapLevel;
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMaterial 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxPlayMaterialPass(LPLYNXMATERIALPASS lpm, int channel, LYNXREAL skip, LYNXPLAYMODE mode)
{
	if (lpm->bDisable)
		return LYNX_FALSE;

	return lynxPlayTextureFrame(&lpm->TextureFrame[channel], skip, mode);		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxGraphic 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */		            		
//-------------------------------------------------------------------------------------------------------
void lynxSetMaterialPassBlendMode(LPLYNXMATERIALPASS lpm, LYNXBLENDMODE bf)
{
	switch (bf)
	{
		case LYNX_INVERT_BLEND:
			lpm->SrcBlend = LYNX_BF_ONE_MINUS_DEST_COLOR;
			lpm->DestBlend = LYNX_BF_ONE_MINUS_SRC_COLOR;			
			break;

		case LYNX_ALPHA_BLEND:			
			lpm->SrcBlend = LYNX_BF_SRC_ALPHA;
			lpm->DestBlend = LYNX_BF_ONE_MINUS_SRC_ALPHA;		
		break;

		case LYNX_COLOR_BLEND:
			lpm->SrcBlend = LYNX_BF_DEST_COLOR;
			lpm->DestBlend = LYNX_BF_ZERO;	
		break;

		case LYNX_ACCUMULATE_BLEND:
			lpm->SrcBlend = LYNX_BF_SRC_ALPHA;
			lpm->DestBlend = LYNX_BF_ONE;	
		break;

		case LYNX_OVERLAYER_BLEND:
			lpm->SrcBlend = LYNX_BF_SRC_ALPHA;
			lpm->DestBlend = LYNX_BF_ONE;	
		break;

		case LYNX_NONE_BLEND:
			lpm->SrcBlend = LYNX_BF_ZERO;
			lpm->DestBlend = LYNX_BF_ONE;	
		break;

		case LYNX_ZERO_BLEND:
			lpm->SrcBlend = LYNX_BF_ZERO;
			lpm->DestBlend = LYNX_BF_ZERO;	
		break;

		case LYNX_GLOW_BLEND:
			lpm->SrcBlend = LYNX_BF_ONE;
			lpm->DestBlend = LYNX_BF_SRC_ALPHA;	
		break;

		case LYNX_DECAL_BLEND:
			lpm->SrcBlend = LYNX_BF_ONE;
			lpm->DestBlend = LYNX_BF_ZERO;	
		break;
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMaterial 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxSetMaterialPass(LPLYNXMATERIALPASS lpm)
{
	int						i;

	if (lpm->bDisable)
		return;

	lynxSetUseFog(lpm->bUseFog);
	lynxSetZBufferMode(lpm->ZBufferMode);
	lynxSetHardwareCullMode(lpm->CullMode);

	if (lpm->SpPower > 0.0f)
	{
		lynxSetUseSpecular(LYNX_TRUE);
	}
	else
	{
		lynxSetUseSpecular(LYNX_FALSE);
	}						

	if (lpm->VS)
	{
		lynxSetVertexLayout(&lpm->VertexLayout);
		lynxSetVertexShader(lpm->VS);						
	}
	else
	{
		lynxSetVertexShader(lpm->VS);						
		lynxSetFixedVertexShader(lpm->FixedVS);
	}

	if (!lpm->PS)
	{
		for (i=0; i<LYNX_MAX_USED_TEXTURE_CHANNEL; i++)
		{
			if (lpm->TextureFrame[i].NumTexs)
			{
				lynxSetTexture(i, lpm->TextureFrame[i].lpTexIndexList[LYNX_REAL_2_INT(lpm->TextureFrame[i].Time)]);			
				lynxSetTextureAddressMode(i, lpm->AddressMode[i]);
				lynxSetTextureFilterMode(i, lpm->TextureFrame[i].MinFilter, lpm->TextureFrame[i].MagFilter, lpm->TextureFrame[i].MipFilter); 
				lynxSetTextureBlend(i, lpm->CombineFunc[i], lpm->CombineArg[i][0], lpm->CombineArg[i][1], LYNX_CA_TEXTURE);													
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
		for (i=0; i<LYNX_MAX_USED_TEXTURE_CHANNEL; i++)
		{
			if (lpm->TextureFrame[i].NumTexs)
			{
				lynxSetTexture(i, lpm->TextureFrame[i].lpTexIndexList[LYNX_REAL_2_INT(lpm->TextureFrame[i].Time)]);			
				lynxSetTextureAddressMode(i, lpm->AddressMode[i]);
				lynxSetTextureFilterMode(i, lpm->TextureFrame[i].MinFilter, lpm->TextureFrame[i].MagFilter, lpm->TextureFrame[i].MipFilter); 				
			}					
			else
			{
				break;
			}
		}				
	}		
	lynxSetBlendFunc(lpm->SrcBlend, lpm->DestBlend);
	lynxSetPixelShader(lpm->PS);	
	if (lpm->lpSetParamProc)
		lpm->lpSetParamProc();

	
	/*
	//fast-------------------------------
	//lynxSetUseFog(lpm->bUseFog);
	lynxSetZBufferMode(lpm->ZBufferMode);
	lynxSetHardwareCullMode(lpm->CullMode);
	//lynxSetUseSpecular(LYNX_FALSE);
	//lynxSetVertexShaderDecl(lpm->VSDecl);
	//lynxSetVertexShader(lpm->VS);			
	lynxSetTexture(0, lpm->TextureFrame[0].lpTexIndexList[LYNX_REAL_2_INT(lpm->TextureFrame[0].Time)]);																									
	//lynxSetPixelShader(lpm->PS);
    lynxSetBlendFunc(lpm->SrcBlend, lpm->DestBlend);
	//if (lpm->lpSetParamProc)
	//	lpm->lpSetParamProc();
	*/
}