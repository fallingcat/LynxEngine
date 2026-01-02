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

#include <LynxFontSpr.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxInitFontSprites(LPLYNXFONTSPRITES lpfs)
{
	int			i;

	lynxInitMaterial(&lpfs->Material);
	lynxCreateMaterial(&lpfs->Material, 1);
	lpfs->Material.lpMaterialPass[0].ZBufferMode = LYNX_ZB_NONE;
	lpfs->Material.lpMaterialPass[0].SrcBlend = LYNX_BF_SRC_ALPHA;
	lpfs->Material.lpMaterialPass[0].DestBlend = LYNX_BF_ONE_MINUS_SRC_ALPHA;
	for (i=0; i< 144; i++)
	{
		lpfs->Width[i] = lpfs->Height[i] = 0;
		lpfs->Color[i].s.Red = 255;
		lpfs->Color[i].s.Green = 255;
		lpfs->Color[i].s.Blue = 255;
		lpfs->Color[i].s.Alpha = 255;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetFontSpritesColor(LPLYNXFONTSPRITES lpfs, LPLYNXCOLORRGBA lpc)
{
	int			i;

	for (i=0; i< 144; i++)
	{
		lpfs->Color[i].s.Red = lpc->s.Red;
		lpfs->Color[i].s.Green = lpc->s.Green;
		lpfs->Color[i].s.Blue = lpc->s.Blue;
		lpfs->Color[i].s.Alpha = lpc->s.Alpha;

		LYNX_RV_SET_RGBA(lpfs->V[i*4], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
		LYNX_RV_SET_RGBA(lpfs->V[i*4+1], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
		LYNX_RV_SET_RGBA(lpfs->V[i*4+2], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
		LYNX_RV_SET_RGBA(lpfs->V[i*4+3], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxLoadFixFontSprites(LPLYNXFONTSPRITES lpfs, char *filename, int si, int num, int x, int y, int w, int h, LYNXTEXTURETYPE textype)
{
	LPLYNXTEXTUREINFO					lpTexInfo;
	int									i, NumX;
	LYNXREAL							OffsetU, OffsetV, U, V;	

	if (!lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList)
	{
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].Time = LYNX_REAL(0);
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].StartTime = LYNX_REAL(0);
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].EndTime = LYNX_REAL(0);		
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].NumTexs = 1;
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpfs->Material.lpMaterialPass[0].TextureFrame[0].NumTexs);
		for (i=0; i<lpfs->Material.lpMaterialPass[0].TextureFrame[0].NumTexs; i++)
			lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[i] = LYNX_NULL_TEXTURE;
		lynxLoadTexture(&lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[0], &lpTexInfo, 1, filename, NULL, 0, textype);	
		lpfs->Material.lpMaterialPass[0].AddressMode[0] = LYNX_ADDRESS_CLAMP;
	}
	else
	{
		lpTexInfo = lynxGetTextureInfo(lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[0]);
	}

	if (lpTexInfo)
	{
		OffsetU = LYNX_REAL(x);
		OffsetV = LYNX_REAL(y);
		for (i=si; i<si+num; i++)
		{
			LYNX_RV_SET_RGBA(lpfs->V[i*4], lpfs->Color[i].s.Red, lpfs->Color[i].s.Green, lpfs->Color[i].s.Blue, lpfs->Color[i].s.Alpha);		
			LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpfs->Color[i].s.Red, lpfs->Color[i].s.Green, lpfs->Color[i].s.Blue, lpfs->Color[i].s.Alpha);		
			LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpfs->Color[i].s.Red, lpfs->Color[i].s.Green, lpfs->Color[i].s.Blue, lpfs->Color[i].s.Alpha);		
			LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpfs->Color[i].s.Red, lpfs->Color[i].s.Green, lpfs->Color[i].s.Blue, lpfs->Color[i].s.Alpha);		

			lpfs->Width[i] = LYNX_REAL(w);
			lpfs->Height[i] = LYNX_REAL(h);		
			lpfs->CWidth[i] = LYNX_REAL(w);
			lpfs->CHeight[i] = LYNX_REAL(h);				

			U = LYNX_REAL(w);
			V = LYNX_REAL(h);
			LYNX_RV_SET_UV(lpfs->V[i*4 + 0], 0, LYNX_REAL_DIV((OffsetU), LYNX_REAL(lpTexInfo->Width)), LYNX_REAL_DIV((OffsetV), LYNX_REAL(lpTexInfo->Height))); 
			LYNX_RV_SET_UV(lpfs->V[i*4 + 1], 0, LYNX_REAL_DIV((OffsetU+U-LYNX_REAL(1)), LYNX_REAL(lpTexInfo->Width)), LYNX_REAL_DIV((OffsetV), LYNX_REAL(lpTexInfo->Height))); 
			LYNX_RV_SET_UV(lpfs->V[i*4 + 2], 0, LYNX_REAL_DIV((OffsetU), LYNX_REAL(lpTexInfo->Width)), LYNX_REAL_DIV((OffsetV+V), LYNX_REAL(lpTexInfo->Height))); 
			LYNX_RV_SET_UV(lpfs->V[i*4 + 3], 0, LYNX_REAL_DIV((OffsetU+U-LYNX_REAL(1)), LYNX_REAL(lpTexInfo->Width)), LYNX_REAL_DIV((OffsetV+V), LYNX_REAL(lpTexInfo->Height))); 

			LYNX_RV_SET_XYZ(lpfs->V[i*4 + 0], LYNX_REAL(0), LYNX_REAL(0), LYNX_REAL(-1)); 
			LYNX_RV_SET_XYZ(lpfs->V[i*4 + 1], lpfs->Width[i], LYNX_REAL(0), LYNX_REAL(-1)); 
			LYNX_RV_SET_XYZ(lpfs->V[i*4 + 2], LYNX_REAL(0), lpfs->Height[i], LYNX_REAL(-1)); 
			LYNX_RV_SET_XYZ(lpfs->V[i*4 + 3], lpfs->Width[i], lpfs->Height[i], LYNX_REAL(-1)); 	

			OffsetU = LYNX_REAL_ADD(OffsetU, LYNX_REAL(w));
		}
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxLoadVarFontSprites(LPLYNXFONTSPRITES lpfs, char *filename, int si, int num, int x, int y, int *w, int h, LYNXTEXTURETYPE textype)
{
	LPLYNXTEXTUREINFO					lpTexInfo = NULL;
	int									i, NumX;
	LYNXREAL							OffsetU, OffsetV, U, V;	
	LYNXPOINT2D							Tex;	

	if (!lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList)
	{
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].Time = LYNX_REAL(0);
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].StartTime = LYNX_REAL(0);
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].EndTime = LYNX_REAL(0);		
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].NumTexs = 1;
		lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpfs->Material.lpMaterialPass[0].TextureFrame[0].NumTexs);
		for (i=0; i<lpfs->Material.lpMaterialPass[0].TextureFrame[0].NumTexs; i++)
			lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[i] = LYNX_NULL_TEXTURE;
		lynxLoadTexture(&lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[0], &lpTexInfo, 1, filename, NULL, 0, textype);
		lpfs->Material.lpMaterialPass[0].AddressMode[0] = LYNX_ADDRESS_CLAMP;
	}
	else
	{
		lpTexInfo = lynxGetTextureInfo(lpfs->Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[0]);
	}

	if (lpTexInfo)
	{
		OffsetU = LYNX_REAL(x);
		OffsetV = LYNX_REAL(y);
		for (i=si; i<si+num; i++)
		{
			LYNX_RV_SET_RGBA(lpfs->V[i*4], lpfs->Color[i].s.Red, lpfs->Color[i].s.Green, lpfs->Color[i].s.Blue, lpfs->Color[i].s.Alpha);		
			LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpfs->Color[i].s.Red, lpfs->Color[i].s.Green, lpfs->Color[i].s.Blue, lpfs->Color[i].s.Alpha);		
			LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpfs->Color[i].s.Red, lpfs->Color[i].s.Green, lpfs->Color[i].s.Blue, lpfs->Color[i].s.Alpha);		
			LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpfs->Color[i].s.Red, lpfs->Color[i].s.Green, lpfs->Color[i].s.Blue, lpfs->Color[i].s.Alpha);		

			lpfs->Width[i] = LYNX_REAL(w[i-si]);
			lpfs->Height[i] = LYNX_REAL(h);				
			lpfs->CWidth[i] = LYNX_REAL(w[i-si]);
			lpfs->CHeight[i] = LYNX_REAL(h);				

			U = LYNX_REAL(w[i-si]);
			V = LYNX_REAL(h);
			LYNX_RV_SET_UV(lpfs->V[i*4 + 0], 0, LYNX_REAL_DIV((OffsetU), LYNX_REAL(lpTexInfo->Width)), LYNX_REAL_DIV((OffsetV), LYNX_REAL(lpTexInfo->Height))); 
			LYNX_RV_SET_UV(lpfs->V[i*4 + 1], 0, LYNX_REAL_DIV((OffsetU+U-LYNX_REAL(1)), LYNX_REAL(lpTexInfo->Width)), LYNX_REAL_DIV((OffsetV), LYNX_REAL(lpTexInfo->Height))); 
			LYNX_RV_SET_UV(lpfs->V[i*4 + 2], 0, LYNX_REAL_DIV((OffsetU), LYNX_REAL(lpTexInfo->Width)), LYNX_REAL_DIV((OffsetV+V), LYNX_REAL(lpTexInfo->Height))); 
			LYNX_RV_SET_UV(lpfs->V[i*4 + 3], 0, LYNX_REAL_DIV((OffsetU+U-LYNX_REAL(1)), LYNX_REAL(lpTexInfo->Width)), LYNX_REAL_DIV((OffsetV+V), LYNX_REAL(lpTexInfo->Height))); 

			LYNX_RV_SET_XYZ(lpfs->V[i*4 + 0], LYNX_REAL(0), LYNX_REAL(0), LYNX_REAL(-1)); 
			LYNX_RV_SET_XYZ(lpfs->V[i*4 + 1], lpfs->Width[i], LYNX_REAL(0), LYNX_REAL(-1)); 
			LYNX_RV_SET_XYZ(lpfs->V[i*4 + 2], LYNX_REAL(0), lpfs->Height[i], LYNX_REAL(-1)); 
			LYNX_RV_SET_XYZ(lpfs->V[i*4 + 3], lpfs->Width[i], lpfs->Height[i], LYNX_REAL(-1)); 	

			OffsetU = LYNX_REAL_ADD(OffsetU, LYNX_REAL(w[i-si]));
		}	
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxScaleFontSprites(LPLYNXFONTSPRITES lpfs, LYNXREAL s)
{
	int							i;

	for (i=0; i<144; i++)
	{
		lpfs->CWidth[i] = LYNX_REAL_MUL(lpfs->Width[i], s);
		lpfs->CHeight[i] = LYNX_REAL_MUL(lpfs->Height[i], s);

		LYNX_RV_SET_XYZ(lpfs->V[i*4 + 0], LYNX_REAL(0), LYNX_REAL(0), LYNX_REAL(-1)); 
		LYNX_RV_SET_XYZ(lpfs->V[i*4 + 1], lpfs->CWidth[i], LYNX_REAL(0), LYNX_REAL(-1)); 
		LYNX_RV_SET_XYZ(lpfs->V[i*4 + 2], LYNX_REAL(0), lpfs->CHeight[i], LYNX_REAL(-1)); 
		LYNX_RV_SET_XYZ(lpfs->V[i*4 + 3], lpfs->CWidth[i], lpfs->CHeight[i], LYNX_REAL(-1)); 	
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRenderFontSprites(LPLYNXFONTSPRITES lpfs, int index, int x, int y)
{
	lynxSetMaterialPass(&lpfs->Material.lpMaterialPass[0]);
	lynxDrawStrip(&lpfs->V[index*4], x, y);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRenderCharFromFontSprites(LPLYNXFONTSPRITES lpfs, char  c, int x, int y)
{
	int					i;
	
	i = c-33;	
	lynxDrawStrip(&lpfs->V[i*4], x, y);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 int	lynxFontSpritesPrintf(LPLYNXFONTSPRITES lpfs, int x, int y, LPLYNXCOLORRGBA lpc, char const *fmt, ...)
{
	
	#ifdef __PS2__
		return 0;	
	#else
		int						i, XPos, YPos;
		va_list					argptr;
		char					Buffer[256], *lpC;        	
		LYNXTEXTUREADDRESSMODE	AddressMode;
		LYNXMATRIX				M;

		LYNX_IDENTITY_MATRIX(M);
		lynxSetViewMatrix(&M);	
		lynxSetMaterialPass(&lpfs->Material.lpMaterialPass[0]);

		va_start(argptr,fmt);	
		vsprintf(Buffer, fmt, argptr);
		va_end(argptr);

		lpC = Buffer;
		XPos = x;
		YPos = y;
		while ((*lpC))
		{
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth['W']);
			}
			else if ((*lpC) == '\n')
			{
				YPos += LYNX_REAL_2_INT(lpfs->Height[0]);
			}
			else
			{
				i = ((*lpC) - 33);
				if (lpc)
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
				}			
				else
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], 255, 255, 255, 255);
				}
				lynxRenderCharFromFontSprites(lpfs, (*lpC), XPos, YPos);			
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);
			}
			lpC++;
		}
		return XPos;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 int	lynxGetFontSpritesPrintfWidth(LPLYNXFONTSPRITES lpfs, char const *fmt, ...)
{
	
	#ifdef __PS2__
		return 0;
	#else
		int						i, XPos, YPos;
		va_list					argptr;
		char					Buffer[256], *lpC;;

		va_start(argptr,fmt);	
		vsprintf(Buffer, fmt, argptr);
		va_end(argptr);

		lpC = Buffer;
		XPos = 0;
		while ((*lpC))
		{
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth['W']);
			}
			else
			{
				i = ((*lpC) -33);
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);
			}
			lpC++;
		}
		return XPos;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxFontSpritesAlignRightPrintf(LPLYNXFONTSPRITES lpfs, int x, int y, LPLYNXCOLORRGBA lpc, char const *fmt, ...)
{
	
	#ifdef __PS2__
	#else
		int						i, j, XPos, YPos, Len;
		va_list					argptr;
		char					Buffer[256], *lpC;
		LYNXTEXTUREADDRESSMODE	AddressMode;
		LYNXMATRIX				M;

		LYNX_IDENTITY_MATRIX(M);
		lynxSetViewMatrix(&M);	
		lynxSetMaterialPass(&lpfs->Material.lpMaterialPass[0]);

		va_start(argptr,fmt);	
		vsprintf(Buffer, fmt, argptr);
		va_end(argptr);

		XPos = x;
		YPos = y;
		Len = (int)strlen(Buffer);
		lpC = &Buffer[Len-1];

		for (j=Len-1; j>=0; j--)
		{
			if ((*lpC) == 32)
			{
				XPos -= LYNX_REAL_2_INT(lpfs->CWidth['W']);
			}
			else
			{
				i = ((*lpC) -33);
				if (lpc)
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
				}
				else
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], 255, 255, 255, 255);
				}
				XPos -= LYNX_REAL_2_INT(lpfs->CWidth[i]);
				lynxRenderCharFromFontSprites(lpfs, (*lpC), XPos, YPos);						
			}
			lpC--;
		}		
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 int	lynxFontSpritesCenterPrintf(LPLYNXFONTSPRITES lpfs, int width, int y, LPLYNXCOLORRGBA lpc, char const *fmt, ...)
{
	
	#ifdef __PS2__
		return 0;
	#else
		va_list					argptr;
		int						i, XPos, YPos;	
		char					Buffer[256], *lpC;
		LYNXTEXTUREADDRESSMODE	AddressMode;
		LYNXMATRIX				M;

		LYNX_IDENTITY_MATRIX(M);
		lynxSetViewMatrix(&M);	
		lynxSetMaterialPass(&lpfs->Material.lpMaterialPass[0]);

		va_start(argptr,fmt);
		vsprintf(Buffer, fmt, argptr);	
		va_end(argptr);		

		lpC = Buffer;
		XPos = 0;
		while ((*lpC))
		{
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[33]);
			}
			else
			{			
				i = ((*lpC) -33);
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);
			}
			lpC++;
		}

		XPos = (width - XPos)/2;

		lpC = Buffer;	

		while ((*lpC))
		{		
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[33]);
			}
			else
			{
				i = ((*lpC) -33);			
				if (lpc)
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
				}
				else
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], 255, 255, 255, 255);
				}
				YPos = y;//(int)(y - lpfs->CHeight[i]);
				lynxRenderCharFromFontSprites(lpfs, (*lpC), XPos, YPos);			
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);// + 3;
			}
			lpC++;
		}	
		return (XPos);
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxFontSpritesPrintfShadow(LPLYNXFONTSPRITES lpfs, int x, int y, LPLYNXCOLORRGBA lpc, LPLYNXCOLORRGBA lpsc, char const *fmt, ...)
{
	
	#ifdef __PS2__
	#else
		int						i, XPos, YPos;
		va_list					argptr;
		char					Buffer[256], *lpC;
		LYNXTEXTUREADDRESSMODE	AddressMode;
		LYNXMATRIX				M;

		LYNX_IDENTITY_MATRIX(M);
		lynxSetViewMatrix(&M);	
		lynxSetMaterialPass(&lpfs->Material.lpMaterialPass[0]);

		va_start(argptr,fmt);	
		vsprintf(Buffer, fmt, argptr);
		va_end(argptr);

		lpC = Buffer;
		XPos = x + 4;
		YPos = y + 4;
		while ((*lpC))
		{
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth['W']);
			}
			else
			{
				i = ((*lpC) -33);
				if (lpsc)
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], lpsc->s.Red, lpsc->s.Green, lpsc->s.Blue, lpsc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpsc->s.Red, lpsc->s.Green, lpsc->s.Blue, lpsc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpsc->s.Red, lpsc->s.Green, lpsc->s.Blue, lpsc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpsc->s.Red, lpsc->s.Green, lpsc->s.Blue, lpsc->s.Alpha);
				}
				else
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], 0, 0, 0, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], 0, 0, 0, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], 0, 0, 0, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], 0, 0, 0, 255);
				}
				lynxRenderCharFromFontSprites(lpfs, (*lpC), XPos, YPos);			
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);
			}
			lpC++;
		}

		lpC = Buffer;
		XPos = x;
		YPos = y;
		while ((*lpC))
		{
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth['W']);
			}
			else
			{
				i = ((*lpC) -33);
				if (lpc)
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
				}
				else
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], 255, 255, 255, 255);
				}
				lynxRenderCharFromFontSprites(lpfs, (*lpC), XPos, YPos);			
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);
			}
			lpC++;
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxFontSpritesCenterPrintfShadow(LPLYNXFONTSPRITES lpfs, int width, int y, LPLYNXCOLORRGBA lpc, LPLYNXCOLORRGBA lpsc, char const *fmt, ...)
{
	
	#ifdef __PS2__
	#else
		int						i, XPos, YPos, XStartPos;
		va_list					argptr;
		char					Buffer[256], *lpC;
		LYNXTEXTUREADDRESSMODE	AddressMode;
		LYNXMATRIX				M;

		LYNX_IDENTITY_MATRIX(M);
		lynxSetViewMatrix(&M);	
		lynxSetMaterialPass(&lpfs->Material.lpMaterialPass[0]);

		va_start(argptr,fmt);	
		vsprintf(Buffer, fmt, argptr);
		va_end(argptr);

		lpC = Buffer;
		XPos = 0;
		while ((*lpC))
		{
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[0]);
			}
			else
			{			
				i = ((*lpC) -33);
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);
			}
			lpC++;
		}

		XStartPos = (width - XPos)/2;

		XPos = XStartPos + 4;
		lpC = Buffer;
		YPos = y+4;

		while ((*lpC))
		{
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[0]);
			}
			else
			{
				i = ((*lpC) -33);			
				if (lpsc)
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], lpsc->s.Red, lpsc->s.Green, lpsc->s.Blue, lpsc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpsc->s.Red, lpsc->s.Green, lpsc->s.Blue, lpsc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpsc->s.Red, lpsc->s.Green, lpsc->s.Blue, lpsc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpsc->s.Red, lpsc->s.Green, lpsc->s.Blue, lpsc->s.Alpha);
				}
				else
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], 0, 0, 0, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], 0, 0, 0, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], 0, 0, 0, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], 0, 0, 0, 255);
				}
				lynxRenderCharFromFontSprites(lpfs, (*lpC), XPos, YPos);			
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);
			}
			lpC++;
		}

		lpC = Buffer;
		XPos = XStartPos;
		YPos = y;

		while ((*lpC))
		{
			if ((*lpC) == 32)
			{
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[0]);
			}
			else
			{
				i = ((*lpC) -33);			
				if (lpc)
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], lpc->s.Red, lpc->s.Green, lpc->s.Blue, lpc->s.Alpha);
				}
				else
				{
					LYNX_RV_SET_RGBA(lpfs->V[i*4], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 1], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 2], 255, 255, 255, 255);
					LYNX_RV_SET_RGBA(lpfs->V[i*4 + 3], 255, 255, 255, 255);
				}
				lynxRenderCharFromFontSprites(lpfs, (*lpC), XPos, YPos);			
				XPos += LYNX_REAL_2_INT(lpfs->CWidth[i]);
			}
			lpC++;
		}
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxGetUVFromFontSprites(LPLYNXFONTSPRITES lpfs, char  c, LPLYNXRENDERVERTEX lpv)
{
	int					i;
	
	i = c- 33;
	lynxSetMaterialPass(&lpfs->Material.lpMaterialPass[0]);
	LYNX_RV_SET_UV(lpv[0], 0, LYNX_RV_U(lpfs->V[i*4], 0), LYNX_RV_V(lpfs->V[i*4], 0));
	LYNX_RV_SET_UV(lpv[1], 0, LYNX_RV_U(lpfs->V[i*4+1], 0), LYNX_RV_V(lpfs->V[i*4+1], 0));
	LYNX_RV_SET_UV(lpv[2], 0, LYNX_RV_U(lpfs->V[i*4+2], 0), LYNX_RV_V(lpfs->V[i*4+2], 0));
	LYNX_RV_SET_UV(lpv[3], 0, LYNX_RV_U(lpfs->V[i*4+3], 0), LYNX_RV_V(lpfs->V[i*4+3], 0));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxFontSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxReleaseFontSprites(LPLYNXFONTSPRITES lpfs)
{
	int					i;
	
	lynxReleaseMaterial(&lpfs->Material);	
}
