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

#include <LynxTexSpr.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxInitTexSprite(LPLYNXTEXSPRITE lpts)
{
	lynxInitMaterial(&lpts->Material);
	lpts->Width = lpts->Height = LYNX_REAL(0);
	lpts->Color.s.Red = lpts->Color.s.Green = lpts->Color.s.Blue = lpts->Color.s.Alpha = 255;	
	
	LYNX_RV_SET_UV(lpts->V[0], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[1], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[2], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[3], 0, LYNX_REAL(1), LYNX_REAL(1)); 	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxDuplicateTexSprite(LPLYNXTEXSPRITE lptsd, LPLYNXTEXSPRITE lpts)
{
	int							i;

	lynxDuplicateMaterial(&lptsd->Material, &lpts->Material);
	lptsd->Width = lpts->Width; 
	lptsd->Height = lpts->Height;
	lptsd->Color.PackedColor = lpts->Color.PackedColor;	
	lynxMemCpy(lptsd->V, lpts->V, sizeof(LYNXRENDERVERTEX)*4);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxLoadTexSprite(LPLYNXTEXSPRITE lpts, const char *filename, LYNXREAL w, LYNXREAL h, LPLYNXCOLORRGBA lpc, LYNXTEXTURETYPE textype)
{
	int									i;
	LPLYNXTEXTUREINFO					lpTexInfo = NULL;
	
	lynxInitTexSprite(lpts);			
	lynxCreateMaterial(&lpts->Material, 1);	
	lynxLoadMaterialTexture(&lpts->Material, 0, 0, &lpTexInfo, 0, filename, NULL, 0, textype);
	lpts->Material.lpMaterialPass[0].AddressMode[0] = LYNX_ADDRESS_CLAMP;

	if (w == LYNX_REAL(-1) && lpTexInfo)
		lpts->Width = LYNX_REAL(lpTexInfo->Width);
	else
		lpts->Width = w;

	if (h == LYNX_REAL(-1) && lpTexInfo)
		lpts->Height = LYNX_REAL(lpTexInfo->Height);
	else
		lpts->Height = h;
	
	if (lpc != NULL)
	{
		lpts->Color.PackedColor = lpc->PackedColor;		
	}
	LYNX_RV_SET_RGBA(lpts->V[0], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[1], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[2], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[3], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_UV(lpts->V[0], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[1], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[2], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[3], 0, LYNX_REAL(1), LYNX_REAL(1)); 
	LYNX_RV_SET_XYZ(lpts->V[0], LYNX_REAL(0), LYNX_REAL(0), LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[1], lpts->Width, LYNX_REAL(0), LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[2], LYNX_REAL(0), lpts->Height, LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[3], lpts->Width, lpts->Height, LYNX_REAL(-1)); 
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxLoadTexSprites(LPLYNXTEXSPRITE lpts, int num, const char *filename, const char *ext, LYNXREAL w, LYNXREAL h, LPLYNXCOLORRGBA lpc, LYNXTEXTURETYPE textype)
{
	int									i, Len;
	LPLYNXTEXTUREINFO					lpTexInfo;

	lynxInitTexSprite(lpts);	
	lynxCreateMaterial(&lpts->Material, 1);		
	lynxLoadMaterialTextures(&lpts->Material, 0, 0, &lpTexInfo, 0, num, filename, ext, NULL, 0, textype);	
	lpts->Material.lpMaterialPass[0].AddressMode[0] = LYNX_ADDRESS_CLAMP;

	if (w == -1)
		lpts->Width = LYNX_REAL(lpTexInfo->Width);
	else
		lpts->Width = w;

	if (h == -1)
		lpts->Height = LYNX_REAL(lpTexInfo->Height);
	else
		lpts->Height = h;

	
	if (lpc != NULL)
	{
		lpts->Color.PackedColor = lpc->PackedColor;
	}
	LYNX_RV_SET_RGBA(lpts->V[0], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[1], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[2], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[3], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_UV(lpts->V[0], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[1], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[2], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[3], 0, LYNX_REAL(1), LYNX_REAL(1)); 
	LYNX_RV_SET_XYZ(lpts->V[0], LYNX_REAL(0), LYNX_REAL(0), LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[1], lpts->Width, LYNX_REAL(0), LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[2], LYNX_REAL(0), lpts->Height, LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[3], lpts->Width, lpts->Height, LYNX_REAL(-1)); 
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxGetTexSpriteSize(LPLYNXTEXSPRITE lpts, LYNXREAL *w, LYNXREAL *h)
{
	*w = lpts->Width;
	*h = lpts->Height;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSpriteSize(LPLYNXTEXSPRITE lpts, LYNXREAL w, LYNXREAL h)
{
	lpts->Width = w;
	lpts->Height = h;	

	LYNX_RV_SET_XYZ(lpts->V[1], LYNX_REAL(0), LYNX_REAL(0), LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[1], lpts->Width, LYNX_REAL(0), LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[2], LYNX_REAL(0), lpts->Height, LYNX_REAL(-1)); 
	LYNX_RV_SET_XYZ(lpts->V[3], lpts->Width, lpts->Height, LYNX_REAL(-1)); 
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSpriteCacheType(LPLYNXTEXSPRITE lpts, LYNXTEXTURECACHETYPE type)
{
	int										p, i;

	for (p=0; p<lpts->Material.NumPass; p++)
	{
		for (i=0; i<lpts->Material.lpMaterialPass[p].TextureFrame[0].NumTexs; i++)
		{
			lynxSetTextureCacheType(lpts->Material.lpMaterialPass[p].TextureFrame[0].lpTexIndexList[i], type);
		}	
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxGetTexSpriteColor(LPLYNXTEXSPRITE lpts, LPLYNXCOLORRGBA lpc)
{
	lpc->PackedColor = lpts->Color.PackedColor;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSpriteColor(LPLYNXTEXSPRITE lpts, LPLYNXCOLORRGBA lpc)
{
	lpts->Color.PackedColor = lpc->PackedColor;
	LYNX_RV_SET_RGBA(lpts->V[0], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha); 
	LYNX_RV_SET_RGBA(lpts->V[1], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha); 
	LYNX_RV_SET_RGBA(lpts->V[2], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha); 
	LYNX_RV_SET_RGBA(lpts->V[3], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha); 
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSpriteUVOffset(LPLYNXTEXSPRITE lpts, int x, int y, int w, int h)
{
	int							TW, TH;
	LYNXUVPOINT					UV[2];

	lynxGetTextureSize(lpts->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[0], &TW, &TH);
	UV[0].u = LYNX_REAL_DIV(LYNX_REAL(x), (LYNXREAL)TW);
	UV[0].v = LYNX_REAL_DIV(LYNX_REAL(y), (LYNXREAL)TH);
	UV[1].u = LYNX_REAL_DIV(LYNX_REAL(x+w), (LYNXREAL)TW);
	UV[1].v = LYNX_REAL_DIV(LYNX_REAL(y+h), (LYNXREAL)TH);

	LYNX_RV_SET_UV(lpts->V[0], 0, UV[0].u, UV[0].v); 
	LYNX_RV_SET_UV(lpts->V[1], 0, UV[1].u, UV[0].v); 
	LYNX_RV_SET_UV(lpts->V[2], 0, UV[0].u, UV[1].v); 
	LYNX_RV_SET_UV(lpts->V[3], 0, UV[1].u, UV[1].v); 	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxMirrorTexSpriteUV(LPLYNXTEXSPRITE lpts)
{
	/*
	LYNXUVPOINT					UV;

	UV.u = LYNX_RV_U(lpts->V[0], 0);
	LYNX_RV_U(lpts->V[0], 0) = LYNX_RV_U(lpts->V[1], 0);
	LYNX_RV_U(lpts->V[1], 0) = UV.u;

	UV.u = LYNX_RV_U(lpts->V[2], 0);
	LYNX_RV_U(lpts->V[2], 0) = LYNX_RV_U(lpts->V[3], 0);
	LYNX_RV_U(lpts->V[3], 0) = UV.u;	
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSpriteNumFrames(LPLYNXTEXSPRITE lpts, int num)
{
	int						i;

	lynxReleaseMaterial(&lpts->Material);
	lpts->Material.lpMaterialPass[0].TextureFrame[0].NumTexs = num;
	lpts->Material.lpMaterialPass[0].TextureFrame[0].Time = LYNX_REAL(0);
	lpts->Material.lpMaterialPass[0].TextureFrame[0].StartTime = LYNX_REAL(0);
	lpts->Material.lpMaterialPass[0].TextureFrame[0].EndTime = LYNX_REAL(num-1);	
	lpts->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpts->Material.lpMaterialPass[0].TextureFrame[0].NumTexs);
	for (i=0; i<lpts->Material.lpMaterialPass[0].TextureFrame[0].NumTexs; i++)
		lpts->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[i] = LYNX_NULL_TEXTURE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxLoadTexSpriteFrame(LPLYNXTEXSPRITE lpts, int frame, const char *filename, LYNXTEXTURETYPE textype)
{
	int									i, Len;
	LPLYNXTEXTUREINFO					lpTexInfo;
	char								*Name, FullName[16];
	char								Token[] = ".";	
		
	if (frame >= lpts->Material.lpMaterialPass[0].TextureFrame[0].NumTexs)
		return;

	lynxLoadTexture(&lpts->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[frame], &lpTexInfo, 1, filename, NULL, 0, textype);			
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxPlayTexSprite(LPLYNXTEXSPRITE lpts, LYNXREAL skip, LYNXPLAYMODE mode)
{
	return lynxPlayTextureFrame(&lpts->Material.lpMaterialPass[0].TextureFrame[0], skip, mode);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRenderTexSprite(LPLYNXTEXSPRITE lpts, int x, int y)
{
	int					p;

	for (p=0; p<lpts->Material.NumPass; p++)
	{
		lynxSetMaterialPass(&lpts->Material.lpMaterialPass[p]);
		lynxDrawStrip(lpts->V, x, y);		
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite3D
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRenderTexSpriteCenterRotate(LPLYNXTEXSPRITE lpts, int x, int y, LYNXREAL deg)
{
	LYNXVECTOR3D					Pos, C, C2;	
	LYNXREAL						w, h;		
	LYNXMATRIX						M;
	LYNXREAL						sn, cs;
	int								p;
	
	w = LYNX_REAL_MUL(lpts->Width, LYNX_REAL_HALF);
	h = LYNX_REAL_MUL(lpts->Height, LYNX_REAL_HALF);			

	sn = lynxQuickSin(deg);
	cs = lynxQuickCos(deg);		
	
	LYNX_IDENTITY_MATRIX(M);
	M.s.m11 = cs;
	M.s.m12 = sn;
	M.s.m21 = -sn;
	M.s.m22 = cs;		

	C.s.x = -w;
	C.s.y = -h;
	C.s.z = LYNX_REAL(0);
	lynxPointXMatrix(&C2, &C, &M);
	LYNX_RV_SET_XYZ(lpts->V[0], C2.s.x, C2.s.y, LYNX_REAL(0)); 

	C.s.x = +w;
	C.s.y = -h;	
	lynxPointXMatrix(&C2, &C, &M);
	LYNX_RV_SET_XYZ(lpts->V[1], C2.s.x, C2.s.y, LYNX_REAL(0)); 

	C.s.x = -w;
	C.s.y = h;	
	lynxPointXMatrix(&C2, &C, &M);
	LYNX_RV_SET_XYZ(lpts->V[2], C2.s.x, C2.s.y, LYNX_REAL(0)); 

	C.s.x = +w;
	C.s.y = h;	
	lynxPointXMatrix(&C2, &C, &M);
	LYNX_RV_SET_XYZ(lpts->V[3], C2.s.x, C2.s.y, LYNX_REAL(0)); 

	for (p=0; p<lpts->Material.NumPass; p++)
	{
		lynxSetMaterialPass(&lpts->Material.lpMaterialPass[p]);
		lynxDrawStrip(lpts->V, x, y);		
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRenderTexSpriteWithShadow(LPLYNXTEXSPRITE lpts, int x, int y, int offsetx, int offsety, LPLYNXCOLORRGBA lpsc)
{
	LYNXCOLORRGBA				Color;
	LYNXBLENDMODE				BM;
	int							p;

	lynxGetTexSpriteColor(lpts, &Color);
	lynxSetTexSpriteColor(lpts, lpsc);	
	for (p=0; p<lpts->Material.NumPass; p++)
	{
		lynxSetMaterialPass(&lpts->Material.lpMaterialPass[p]);		
		BM = lynxSetBlendMode(LYNX_ALPHA_BLEND);
		lynxDrawStrip(lpts->V, x+offsetx, y+offsety);	
	}	
	lynxSetTexSpriteColor(lpts, &Color);	
	for (p=0; p<lpts->Material.NumPass; p++)
	{
		lynxSetMaterialPass(&lpts->Material.lpMaterialPass[p]);		
		lynxDrawStrip(lpts->V, x, y);		
		}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxReleaseTexSprite(LPLYNXTEXSPRITE lpts)
{
	lynxReleaseMaterial(&lpts->Material);
	lynxInitTexSprite(lpts);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxInitTexSprite2(LPLYNXTEXSPRITE2 lpts)
{
	lpts->NumTextures = 0;
	lpts->Time = 0;
	lpts->StartTime = 0;
	lpts->EndTime = 0;
	lpts->lpWidth = lpts->lpHeight = NULL;
	lpts->lpColor = NULL;
	lpts->TexIndex = 0;	
	lpts->BlendMode = LYNX_ALPHA_BLEND;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxLoadTexSprites2(LPLYNXTEXSPRITE2 lpts, const char *filename, int num, float *w, float *h, float *u, float *v, LPLYNXCOLORRGBA lpc, LYNXTEXTURETYPE textype)
{
	LPLYNXTEXTUREINFO					lpTexInfo;
	int									i;

	lynxInitTexSprite2(lpts);	
	lpts->NumTextures = num;
	lpts->Time = 0;
	lpts->StartTime = 0;
	lpts->EndTime = (LYNXREAL)(num-1);	
		
	lynxLoadTexture(&lpts->TexIndex, &lpTexInfo, 1, filename, NULL, 0, textype);			

	lpts->lpHeight = (LYNXREAL *)LYNX_MALLOC(sizeof(LYNXREAL) * lpts->NumTextures);
	lpts->lpWidth = (LYNXREAL *)LYNX_MALLOC(sizeof(LYNXREAL) * lpts->NumTextures);
	lpts->lpColor = (LPLYNXCOLORRGBA)LYNX_MALLOC(sizeof(LYNXCOLORRGBA) * lpts->NumTextures);
	lpts->lpV = (LPLYNXRENDERVERTEX)LYNX_MALLOC(sizeof(LYNXRENDERVERTEX) * lpts->NumTextures * 4);

	for (i=0; i<lpts->NumTextures; i++)
	{
		if (w == NULL)
			lpts->lpWidth[i] = (float)(lpTexInfo->Width);
		else
			lpts->lpWidth[i] = w[i];

		if (h == NULL)
			lpts->lpHeight[i] = (float)(lpTexInfo->Height);
		else
			lpts->lpHeight[i] = h[i];
		
		if (lpc != NULL)
		{
			lpts->lpColor[i].PackedColor = lpc[i].PackedColor;		
		}
		LYNX_RV_SET_RGBA(lpts->lpV[i*4 + 0], lpts->lpColor[i].s.Red, lpts->lpColor[i].s.Green, lpts->lpColor[i].s.Blue, lpts->lpColor[i].s.Alpha);		
		LYNX_RV_SET_RGBA(lpts->lpV[i*4 + 1], lpts->lpColor[i].s.Red, lpts->lpColor[i].s.Green, lpts->lpColor[i].s.Blue, lpts->lpColor[i].s.Alpha);		
		LYNX_RV_SET_RGBA(lpts->lpV[i*4 + 2], lpts->lpColor[i].s.Red, lpts->lpColor[i].s.Green, lpts->lpColor[i].s.Blue, lpts->lpColor[i].s.Alpha);		
		LYNX_RV_SET_RGBA(lpts->lpV[i*4 + 3], lpts->lpColor[i].s.Red, lpts->lpColor[i].s.Green, lpts->lpColor[i].s.Blue, lpts->lpColor[i].s.Alpha);		

		LYNX_RV_SET_UV(lpts->lpV[i*4 + 0], 0, u[i*4 + 0], v[i*4 + 0]); 
		LYNX_RV_SET_UV(lpts->lpV[i*4 + 1], 0, u[i*4 + 1], v[i*4 + 1]); 
		LYNX_RV_SET_UV(lpts->lpV[i*4 + 2], 0, u[i*4 + 2], v[i*4 + 2]); 
		LYNX_RV_SET_UV(lpts->lpV[i*4 + 3], 0, u[i*4 + 3], v[i*4 + 3]); 

		LYNX_RV_SET_XYZ(lpts->lpV[i*4 + 0], 0.0f, 0.0f, 1.0f); 
		LYNX_RV_SET_XYZ(lpts->lpV[i*4 + 1], lpts->lpWidth[i], 0.0f, 1.0f); 
		LYNX_RV_SET_XYZ(lpts->lpV[i*4 + 2], 0.0f, lpts->lpHeight[i], 1.0f); 
		LYNX_RV_SET_XYZ(lpts->lpV[i*4 + 3], lpts->lpWidth[i], lpts->lpHeight[i], 1.0f); 
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSprite2Size(LPLYNXTEXSPRITE2 lpts, int time, float w, float h)
{
	lpts->lpWidth[time] = w;
	lpts->lpHeight[time] = h;
	
	LYNX_RV_SET_XYZ(lpts->lpV[time*4 + 1], lpts->lpWidth[time], 0.0f, 1.0f); 
	LYNX_RV_SET_XYZ(lpts->lpV[time*4 + 2], 0.0f, lpts->lpHeight[time], 1.0f); 
	LYNX_RV_SET_XYZ(lpts->lpV[time*4 + 3], lpts->lpWidth[time], lpts->lpHeight[time], 1.0f); 
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSprite2Color(LPLYNXTEXSPRITE2 lpts, int time, LPLYNXCOLORRGBA lpc)
{
	LYNX_RV_SET_RGBA(lpts->lpV[time*4 + 0], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha); 
	LYNX_RV_SET_RGBA(lpts->lpV[time*4 + 1], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha); 
	LYNX_RV_SET_RGBA(lpts->lpV[time*4 + 2], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha); 
	LYNX_RV_SET_RGBA(lpts->lpV[time*4 + 3], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha); 
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxPlayTexSprite2(LPLYNXTEXSPRITE2 lpts, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LYNXBOOL								b;

	b = FALSE;	
	lpts->Time+= skip;
	switch (mode)
	{
		case LYNX_PM_LOOP:	
			while (lpts->Time > lpts->EndTime)
			{
				lpts->Time-= (lpts->EndTime+1);
				lpts->Time += lpts->StartTime;
				b = TRUE;
			}
			while (lpts->Time < lpts->StartTime)
			{
				lpts->Time -= (lpts->StartTime-1);
				lpts->Time += lpts->EndTime;
				b = TRUE;
			}	
			break;
			
		case LYNX_PM_ONCE:	
			if (lpts->Time > lpts->EndTime)
			{
				b = TRUE;
				lpts->Time = lpts->EndTime;
			}
			else if (lpts->Time < lpts->StartTime)
			{
				b = TRUE;
				lpts->Time = lpts->StartTime;
			}
			break;
	}
	return b;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSprite2Time(LPLYNXTEXSPRITE2 lpts, float stime, float etime, float t)
{
	lpts->Time = t;
	lpts->StartTime = stime;
	lpts->EndTime = etime;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetTexSprite2Texture(LPLYNXTEXSPRITE2 lpts)
{
	lynxSetTexture(0, lpts->TexIndex);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSprite
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRenderTexSprite2(LPLYNXTEXSPRITE2 lpts, int x, int y)
{
	lynxDrawStrip(&lpts->lpV[((int)lpts->Time)<<2], x, y);	
}