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

#include <LynxFX.h>

static LYNXRENDERVERTEX						gBlurStripV[4];
static LYNXRENDERVERTEX						gBlurDestStripV[4];
static LYNXPIXELSHADERINDEX					gBlurPS;

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFX 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitTextureBlur(void)
{
	LYNX_RV_SET_XYZ(gBlurStripV[0], 0.0f, 0.0f, 0.0f);
	LYNX_RV_SET_XYZ(gBlurStripV[1], 256.0f, 0.0f, 0.0f);
	LYNX_RV_SET_XYZ(gBlurStripV[2], 0.0f, 256.0f, 0.0f);
	LYNX_RV_SET_XYZ(gBlurStripV[3], 256.0f, 256.0f, 0.0f);

	LYNX_RV_SET_UV(gBlurStripV[0], 0, 0.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurStripV[1], 0, 1.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurStripV[2], 0, 0.0f, 1.0f);
	LYNX_RV_SET_UV(gBlurStripV[3], 0, 1.0f, 1.0f);

	LYNX_RV_SET_UV(gBlurStripV[0], 1, 0.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurStripV[1], 1, 1.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurStripV[2], 1, 0.0f, 1.0f);
	LYNX_RV_SET_UV(gBlurStripV[3], 1, 1.0f, 1.0f);

	LYNX_RV_SET_UV(gBlurStripV[0], 2, 0.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurStripV[1], 2, 1.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurStripV[2], 2, 0.0f, 1.0f);
	LYNX_RV_SET_UV(gBlurStripV[3], 2, 1.0f, 1.0f);

	LYNX_RV_SET_UV(gBlurStripV[0], 3, 0.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurStripV[1], 3, 1.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurStripV[2], 3, 0.0f, 1.0f);
	LYNX_RV_SET_UV(gBlurStripV[3], 3, 1.0f, 1.0f);

	LYNX_RV_SET_RGBA(gBlurStripV[0], 255, 255, 255, 255);
	LYNX_RV_SET_RGBA(gBlurStripV[1], 255, 255, 255, 255);
	LYNX_RV_SET_RGBA(gBlurStripV[2], 255, 255, 255, 255);
	LYNX_RV_SET_RGBA(gBlurStripV[3], 255, 255, 255, 255);


	LYNX_RV_SET_XYZ(gBlurDestStripV[0], 0.0f, 0.0f, 0.0f);
	LYNX_RV_SET_XYZ(gBlurDestStripV[1], 256.0f, 0.0f, 0.0f);
	LYNX_RV_SET_XYZ(gBlurDestStripV[2], 0.0f, 256.0f, 0.0f);
	LYNX_RV_SET_XYZ(gBlurDestStripV[3], 256.0f, 256.0f, 0.0f);

	LYNX_RV_SET_UV(gBlurDestStripV[0], 0, 0.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[1], 0, 1.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[2], 0, 0.0f, 1.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[3], 0, 1.0f, 1.0f);

	LYNX_RV_SET_UV(gBlurDestStripV[0], 1, 0.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[1], 1, 1.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[2], 1, 0.0f, 1.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[3], 1, 1.0f, 1.0f);

	LYNX_RV_SET_UV(gBlurDestStripV[0], 2, 0.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[1], 2, 1.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[2], 2, 0.0f, 1.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[3], 2, 1.0f, 1.0f);

	LYNX_RV_SET_UV(gBlurDestStripV[0], 3, 0.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[1], 3, 1.0f, 0.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[2], 3, 0.0f, 1.0f);
	LYNX_RV_SET_UV(gBlurDestStripV[3], 3, 1.0f, 1.0f);

	LYNX_RV_SET_RGBA(gBlurDestStripV[0], 255, 255, 255, 255);
	LYNX_RV_SET_RGBA(gBlurDestStripV[1], 255, 255, 255, 255);
	LYNX_RV_SET_RGBA(gBlurDestStripV[2], 255, 255, 255, 255);
	LYNX_RV_SET_RGBA(gBlurDestStripV[3], 255, 255, 255, 255);

	gBlurPS = LYNX_NULL_PIXELSHADER;
	lynxLoadCompiledPixelShader(&gBlurPS, "buildin/common/lynxBoxBlur.pso", NULL, 0);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFX 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
int lynxBlurTexture(LYNXTEXTUREINDEX dest, LYNXTEXTUREINDEX src, LYNXTEXTUREINDEX *lpblurbuffer, LPLYNXUVPOINT lpuv, int pass)
{
	LYNXTEXTUREINDEX			BluredTexIndex;
	LPLYNXTEXTUREINFO			lpTexInfo;
	int							i, CurrentBlurTex, LastBlurTex;

	lpTexInfo = lynxGetTextureInfo(lpblurbuffer[0]);
	LYNX_RV_SET_XYZ(gBlurStripV[0], 0.0f, 0.0f, -1.0f);
	LYNX_RV_SET_XYZ(gBlurStripV[1], lpTexInfo->Width, 0.0f, -1.0f);
	LYNX_RV_SET_XYZ(gBlurStripV[2], 0.0f, lpTexInfo->Height, -1.0f);
	LYNX_RV_SET_XYZ(gBlurStripV[3], lpTexInfo->Width, lpTexInfo->Height, -1.0f);

	LYNX_RV_SET_UV(gBlurStripV[0], 0, 0.0f+lpuv[0].u, 0.0f+lpuv[0].v);
	LYNX_RV_SET_UV(gBlurStripV[1], 0, 1.0f+lpuv[0].u, 0.0f+lpuv[0].v);
	LYNX_RV_SET_UV(gBlurStripV[2], 0, 0.0f+lpuv[0].u, 1.0f+lpuv[0].v);
	LYNX_RV_SET_UV(gBlurStripV[3], 0, 1.0f+lpuv[0].u, 1.0f+lpuv[0].v);

	LYNX_RV_SET_UV(gBlurStripV[0], 1, 0.0f+lpuv[1].u, 0.0f+lpuv[1].v);
	LYNX_RV_SET_UV(gBlurStripV[1], 1, 1.0f+lpuv[1].u, 0.0f+lpuv[1].v);
	LYNX_RV_SET_UV(gBlurStripV[2], 1, 0.0f+lpuv[1].u, 1.0f+lpuv[1].v);
	LYNX_RV_SET_UV(gBlurStripV[3], 1, 1.0f+lpuv[1].u, 1.0f+lpuv[1].v);

	LYNX_RV_SET_UV(gBlurStripV[0], 2, 0.0f+lpuv[2].u, 0.0f+lpuv[2].v);
	LYNX_RV_SET_UV(gBlurStripV[1], 2, 1.0f+lpuv[2].u, 0.0f+lpuv[2].v);
	LYNX_RV_SET_UV(gBlurStripV[2], 2, 0.0f+lpuv[2].u, 1.0f+lpuv[2].v);
	LYNX_RV_SET_UV(gBlurStripV[3], 2, 1.0f+lpuv[2].u, 1.0f+lpuv[2].v);

	LYNX_RV_SET_UV(gBlurStripV[0], 3, 0.0f+lpuv[3].u, 0.0f+lpuv[3].v);
	LYNX_RV_SET_UV(gBlurStripV[1], 3, 1.0f+lpuv[3].u, 0.0f+lpuv[3].v);
	LYNX_RV_SET_UV(gBlurStripV[2], 3, 0.0f+lpuv[3].u, 1.0f+lpuv[3].v);
	LYNX_RV_SET_UV(gBlurStripV[3], 3, 1.0f+lpuv[3].u, 1.0f+lpuv[3].v);

	CurrentBlurTex = 0;
	lynxSetTexture(0, src);
	lynxSetTextureAddressMode(0, LYNX_ADDRESS_CLAMP);
	lynxSetTextureFilterMode(0, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
	lynxSetTexture(1, src);
	lynxSetTextureAddressMode(1, LYNX_ADDRESS_CLAMP);
	lynxSetTextureFilterMode(1, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
	lynxSetTexture(2, src);
	lynxSetTextureAddressMode(2, LYNX_ADDRESS_CLAMP);
	lynxSetTextureFilterMode(2, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
	lynxSetTexture(3, src);
	lynxSetTextureAddressMode(3, LYNX_ADDRESS_CLAMP);
	lynxSetTextureFilterMode(3, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
	
	lynxSetUseFog(LYNX_FALSE);
	lynxSetZBufferMode(LYNX_ZB_NONE);
	lynxSetHardwareCullMode(LYNX_CULL_NONE);
	lynxSetUseSpecular(LYNX_FALSE);
	lynxSetBlendMode(LYNX_DECAL_BLEND);
	lynxSetPixelShader(gBlurPS);
	
	if (dest == LYNX_NULL_TEXTURE)
		dest = lpblurbuffer[pass%2];
	
	if (pass == 1)
	{
		
		lynxSetRenderBuffer(lynxGetTexturelpSrcBuffer(lpblurbuffer[CurrentBlurTex]), NULL);	
		lynxDrawStrip(gBlurStripV, 0.0f, 0.0f);	
		BluredTexIndex = lpblurbuffer[0];

		lpTexInfo = lynxGetTextureInfo(dest);
		LYNX_RV_SET_XYZ(gBlurDestStripV[0], 0.0f, 0.0f, -1.0f);
		LYNX_RV_SET_XYZ(gBlurDestStripV[1], lpTexInfo->Width, 0.0f, -1.0f);
		LYNX_RV_SET_XYZ(gBlurDestStripV[2], 0.0f, lpTexInfo->Height, -1.0f);
		LYNX_RV_SET_XYZ(gBlurDestStripV[3], lpTexInfo->Width, lpTexInfo->Height, -1.0f);

		lynxSetTexture(0, BluredTexIndex);
		lynxSetTextureAddressMode(0, LYNX_ADDRESS_CLAMP);
		lynxSetTextureFilterMode(0, LYNX_TF_LINEAR, LYNX_TF_LINEAR, LYNX_TF_LINEAR); 
		lynxSetTextureBlend(1, LYNX_CF_NONE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE, LYNX_CA_TEXTURE);

		lynxSetUseFog(LYNX_FALSE);
		lynxSetZBufferMode(LYNX_ZB_NONE);
		lynxSetHardwareCullMode(LYNX_CULL_NONE);
		lynxSetUseSpecular(LYNX_FALSE);
		lynxSetBlendMode(LYNX_DECAL_BLEND);
		lynxSetPixelShader(LYNX_NULL_PIXELSHADER);
		
		lynxSetRenderBuffer(lynxGetTexturelpSrcBuffer(dest), NULL);	
		lynxDrawStrip(gBlurDestStripV, 0.0f, 0.0f);	
	}
	else
	{
		for (i=0; i<pass-1; i++)
		{
			lynxSetRenderBuffer(lynxGetTexturelpSrcBuffer(lpblurbuffer[CurrentBlurTex]), NULL);	

			lynxDrawStrip(gBlurStripV, 0.0f, 0.0f);	

			lynxSetTexture(0, lpblurbuffer[CurrentBlurTex]);
			lynxSetTexture(1, lpblurbuffer[CurrentBlurTex]);
			lynxSetTexture(2, lpblurbuffer[CurrentBlurTex]);
			lynxSetTexture(3, lpblurbuffer[CurrentBlurTex]);
			LastBlurTex = CurrentBlurTex;
			CurrentBlurTex = (CurrentBlurTex+1)%2;
		}
		BluredTexIndex = lpblurbuffer[LastBlurTex];

		lpTexInfo = lynxGetTextureInfo(dest);
		LYNX_RV_SET_XYZ(gBlurDestStripV[0], 0.0f, 0.0f, -1.0f);
		LYNX_RV_SET_XYZ(gBlurDestStripV[1], lpTexInfo->Width, 0.0f, -1.0f);
		LYNX_RV_SET_XYZ(gBlurDestStripV[2], 0.0f, lpTexInfo->Height, -1.0f);
		LYNX_RV_SET_XYZ(gBlurDestStripV[3], lpTexInfo->Width, lpTexInfo->Height, -1.0f);

		LYNX_RV_SET_UV(gBlurDestStripV[0], 0, 0.0f+lpuv[0].u, 0.0f+lpuv[0].v);
		LYNX_RV_SET_UV(gBlurDestStripV[1], 0, 1.0f+lpuv[0].u, 0.0f+lpuv[0].v);
		LYNX_RV_SET_UV(gBlurDestStripV[2], 0, 0.0f+lpuv[0].u, 1.0f+lpuv[0].v);
		LYNX_RV_SET_UV(gBlurDestStripV[3], 0, 1.0f+lpuv[0].u, 1.0f+lpuv[0].v);

		LYNX_RV_SET_UV(gBlurDestStripV[0], 1, 0.0f+lpuv[1].u, 0.0f+lpuv[1].v);
		LYNX_RV_SET_UV(gBlurDestStripV[1], 1, 1.0f+lpuv[1].u, 0.0f+lpuv[1].v);
		LYNX_RV_SET_UV(gBlurDestStripV[2], 1, 0.0f+lpuv[1].u, 1.0f+lpuv[1].v);
		LYNX_RV_SET_UV(gBlurDestStripV[3], 1, 1.0f+lpuv[1].u, 1.0f+lpuv[1].v);

		LYNX_RV_SET_UV(gBlurDestStripV[0], 2, 0.0f+lpuv[2].u, 0.0f+lpuv[2].v);
		LYNX_RV_SET_UV(gBlurDestStripV[1], 2, 1.0f+lpuv[2].u, 0.0f+lpuv[2].v);
		LYNX_RV_SET_UV(gBlurDestStripV[2], 2, 0.0f+lpuv[2].u, 1.0f+lpuv[2].v);
		LYNX_RV_SET_UV(gBlurDestStripV[3], 2, 1.0f+lpuv[2].u, 1.0f+lpuv[2].v);

		LYNX_RV_SET_UV(gBlurDestStripV[0], 3, 0.0f+lpuv[3].u, 0.0f+lpuv[3].v);
		LYNX_RV_SET_UV(gBlurDestStripV[1], 3, 1.0f+lpuv[3].u, 0.0f+lpuv[3].v);
		LYNX_RV_SET_UV(gBlurDestStripV[2], 3, 0.0f+lpuv[3].u, 1.0f+lpuv[3].v);
		LYNX_RV_SET_UV(gBlurDestStripV[3], 3, 1.0f+lpuv[3].u, 1.0f+lpuv[3].v);

		lynxSetTexture(0, BluredTexIndex);
		lynxSetTexture(1, BluredTexIndex);
		lynxSetTexture(2, BluredTexIndex);
		lynxSetTexture(3, BluredTexIndex);

		lynxSetRenderBuffer(lynxGetTexturelpSrcBuffer(dest), NULL);	

		lynxDrawStrip(gBlurDestStripV, 0.0f, 0.0f);			
	}
	if (dest == LYNX_NULL_TEXTURE)
		return (pass%2);
	else
		return -1;
}