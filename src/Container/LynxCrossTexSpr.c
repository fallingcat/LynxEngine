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

#include <LynxCrossTexSpr.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxInitCrossTexSprite(LPLYNXCROSSTEXSPRITE lpts)
{
	LYNXREAL				W, H;
	
    lynxInitObject3D(&lpts->Obj3D);
	lynxInitTexSprite(&lpts->TexSprite);	

	W = LYNX_REAL(1);
	H = LYNX_REAL(1);
	lpts->Color.s.Red = lpts->Color.s.Green = lpts->Color.s.Blue = lpts->Color.s.Alpha = 255;
	lpts->Flag = LYNX_CROSSTEXSPRITE_X | LYNX_CROSSTEXSPRITE_Z;
	
	lpts->Obj3D.Play = (LPLYNXPLAYPROC)lynxPlayCrossTexSprite;
	lpts->Obj3D.Render = (LPLYNXRENDERPROC)lynxRenderCrossTexSpriteCenter;
	
	LYNX_RV_SET_UV(lpts->V[0], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[1], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[2], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[3], 0, LYNX_REAL(1), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[4], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[5], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[6], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[7], 0, LYNX_REAL(1), LYNX_REAL(1));
	LYNX_RV_SET_UV(lpts->V[8], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[9], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[10], 0, LYNX_REAL(1), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[11], 0, LYNX_REAL(0), LYNX_REAL(1));

	LYNX_RV_SET_XYZ(lpts->V[0], -W, H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->V[1], W, H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->V[2], -W, -H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->V[3], W, -H, LYNX_REAL(0)); 

	LYNX_RV_SET_XYZ(lpts->V[4], LYNX_REAL(0), H, -W); 
	LYNX_RV_SET_XYZ(lpts->V[5], LYNX_REAL(0), H, W); 
	LYNX_RV_SET_XYZ(lpts->V[6], LYNX_REAL(0), -H, -W); 
	LYNX_RV_SET_XYZ(lpts->V[7], LYNX_REAL(0), -H, W); 

	LYNX_RV_SET_XYZ(lpts->V[8], -W, LYNX_REAL(0), W); 
	LYNX_RV_SET_XYZ(lpts->V[9], W, LYNX_REAL(0), W); 
	LYNX_RV_SET_XYZ(lpts->V[10], -W, LYNX_REAL(0), -W); 
	LYNX_RV_SET_XYZ(lpts->V[11], W, LYNX_REAL(0), -W); 
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxReleaseCrossTexSprite(LPLYNXCROSSTEXSPRITE lpts)
{
	lynxReleaseTexSprite(&lpts->TexSprite);
	lynxReleaseObject3D(&lpts->Obj3D);
	lynxInitCrossTexSprite(lpts);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxDuplicateCrossTexSprite(LPLYNXCROSSTEXSPRITE lptsd, LPLYNXCROSSTEXSPRITE lpts)
{
	int								i;

	lynxMemCpy(&lptsd->Obj3D, &lpts->Obj3D, sizeof(LYNXOBJ3D));
	lynxDuplicateTexSprite(&lptsd->TexSprite, &lpts->TexSprite);
	lptsd->Color.PackedColor = lpts->Color.PackedColor;
	lptsd->Flag = lpts->Flag;
	lynxMemCpy(lptsd->V, lpts->V, sizeof(LYNXRENDERVERTEX)*24);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxLoadCrossTexSprite(LPLYNXCROSSTEXSPRITE lpts, const char *filename, LYNXREAL w, LYNXREAL h, LPLYNXCOLORRGBA lpc, int flag)
{
	int									i;
	LPLYNXTEXTUREINFO					lpTexInfo;
	LYNXREAL							W, H;

	lynxInitCrossTexSprite(lpts);
	lpts->Flag = flag;
	lynxLoadTexSprite(&lpts->TexSprite, filename, w, h, lpc, LYNX_NORMAL_TEXTURE);

	if (lpc != NULL)
	{
		lpts->Color.PackedColor = lpc->PackedColor;
	}
	LYNX_RV_SET_RGBA(lpts->V[0], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[1], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[2], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[3], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[4], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[5], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[6], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[7], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[8], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[9], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[10], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[11], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		

	LYNX_RV_SET_UV(lpts->V[0], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[1], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[2], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[3], 0, LYNX_REAL(1), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[4], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[5], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[6], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[7], 0, LYNX_REAL(1), LYNX_REAL(1));
	LYNX_RV_SET_UV(lpts->V[8], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[9], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[10], 0, LYNX_REAL(1), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[11], 0, LYNX_REAL(0), LYNX_REAL(1));
	
	W = LYNX_REAL_MUL(lpts->TexSprite.Width, LYNX_REAL_HALF);
	H = LYNX_REAL_MUL(lpts->TexSprite.Height, LYNX_REAL_HALF);

	if (lpts->Flag & LYNX_CROSSTEXSPRITE_Z)
	{	
		LYNX_RV_SET_XYZ(lpts->V[0], -W, H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[1], W, H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[2], -W, -H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[3], W, -H, LYNX_REAL(0)); 
	}
	if (lpts->Flag & LYNX_CROSSTEXSPRITE_X)
	{
		LYNX_RV_SET_XYZ(lpts->V[4], LYNX_REAL(0), H, -W); 
		LYNX_RV_SET_XYZ(lpts->V[5], LYNX_REAL(0), H, W); 
		LYNX_RV_SET_XYZ(lpts->V[6], LYNX_REAL(0), -H, -W); 
		LYNX_RV_SET_XYZ(lpts->V[7], LYNX_REAL(0), -H, W); 
	}
	if (lpts->Flag & LYNX_CROSSTEXSPRITE_Y)
	{
		LYNX_RV_SET_XYZ(lpts->V[8], -W, LYNX_REAL(0), W); 
		LYNX_RV_SET_XYZ(lpts->V[9], W, LYNX_REAL(0), W); 
		LYNX_RV_SET_XYZ(lpts->V[10], -W, LYNX_REAL(0), -W); 
		LYNX_RV_SET_XYZ(lpts->V[11], W, LYNX_REAL(0), -W); 		
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxLoadCrossTexSprites(LPLYNXCROSSTEXSPRITE lpts, int num, const char *filename, const char *ext, LYNXREAL w, LYNXREAL h, LPLYNXCOLORRGBA lpc)
{	
	int									i, Len;
	LPLYNXTEXTUREINFO					lpTexInfo;
	char								*Name, FullName[16];
	char								Token[] = ".";
	LYNXREAL							W, H;

	lynxInitCrossTexSprite(lpts);
	lynxLoadTexSprites(&lpts->TexSprite, num, filename, ext, w, h, lpc, LYNX_NORMAL_TEXTURE);
	
	if (lpc != NULL)
	{
		lpts->Color.PackedColor = lpc->PackedColor;
	}
	LYNX_RV_SET_RGBA(lpts->V[0], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[1], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[2], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[3], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[4], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[5], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[6], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[7], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[8], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[9], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[10], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[11], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		

	LYNX_RV_SET_UV(lpts->V[0], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[1], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[2], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[3], 0, LYNX_REAL(1), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[4], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[5], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[6], 0, LYNX_REAL(0), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[7], 0, LYNX_REAL(1), LYNX_REAL(1));
	LYNX_RV_SET_UV(lpts->V[8], 0, LYNX_REAL(1), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[9], 0, LYNX_REAL(0), LYNX_REAL(0)); 
	LYNX_RV_SET_UV(lpts->V[10], 0, LYNX_REAL(1), LYNX_REAL(1)); 
	LYNX_RV_SET_UV(lpts->V[11], 0, LYNX_REAL(0), LYNX_REAL(1));

	W = LYNX_REAL_MUL(lpts->TexSprite.Width, LYNX_REAL_HALF);
	H = LYNX_REAL_MUL(lpts->TexSprite.Height, LYNX_REAL_HALF);

	if (lpts->Flag & LYNX_CROSSTEXSPRITE_Z)
	{	
		LYNX_RV_SET_XYZ(lpts->V[0], -W, H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[1], W, H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[2], -W, -H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[3], W, -H, LYNX_REAL(0)); 
	}
	if (lpts->Flag & LYNX_CROSSTEXSPRITE_X)
	{
		LYNX_RV_SET_XYZ(lpts->V[4], LYNX_REAL(0), H, -W); 
		LYNX_RV_SET_XYZ(lpts->V[5], LYNX_REAL(0), H, W); 
		LYNX_RV_SET_XYZ(lpts->V[6], LYNX_REAL(0), -H, -W); 
		LYNX_RV_SET_XYZ(lpts->V[7], LYNX_REAL(0), -H, W); 
	}
	if (lpts->Flag & LYNX_CROSSTEXSPRITE_Y)
	{
		LYNX_RV_SET_XYZ(lpts->V[8], -W, LYNX_REAL(0), W); 
		LYNX_RV_SET_XYZ(lpts->V[9], W, LYNX_REAL(0), W); 
		LYNX_RV_SET_XYZ(lpts->V[10], -W, LYNX_REAL(0), -W); 
		LYNX_RV_SET_XYZ(lpts->V[11], W, LYNX_REAL(0), -W); 
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetCrossTexSpriteNumFrames(LPLYNXCROSSTEXSPRITE lpts, int num)
{
	lynxSetTexSpriteNumFrames(&lpts->TexSprite, num);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetCrossTexSpriteCacheType(LPLYNXCROSSTEXSPRITE lpts, LYNXTEXTURECACHETYPE type)
{
	lynxSetTexSpriteCacheType(&lpts->TexSprite, type);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxGetCrossTexSpriteSize(LPLYNXCROSSTEXSPRITE lpts, LYNXREAL *w, LYNXREAL *h)
{
	LYNXREAL			W, H;

	*w = lpts->TexSprite.Width;
	*h = lpts->TexSprite.Height;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetCrossTexSpriteSize(LPLYNXCROSSTEXSPRITE lpts, LYNXREAL w, LYNXREAL h)
{
	LYNXREAL				W, H;

	lpts->TexSprite.Width = w;
	lpts->TexSprite.Height = h;
	
	W = LYNX_REAL_MUL(lpts->TexSprite.Width, LYNX_REAL_HALF);
	H = LYNX_REAL_MUL(lpts->TexSprite.Height, LYNX_REAL_HALF);

	if (lpts->Flag & LYNX_CROSSTEXSPRITE_Z)
	{	
		LYNX_RV_SET_XYZ(lpts->V[0], -W, H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[1], W, H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[2], -W, -H, LYNX_REAL(0)); 
		LYNX_RV_SET_XYZ(lpts->V[3], W, -H, LYNX_REAL(0)); 
	}
	if (lpts->Flag & LYNX_CROSSTEXSPRITE_X)
	{
		LYNX_RV_SET_XYZ(lpts->V[4], LYNX_REAL(0), H, -W); 
		LYNX_RV_SET_XYZ(lpts->V[5], LYNX_REAL(0), H, W); 
		LYNX_RV_SET_XYZ(lpts->V[6], LYNX_REAL(0), -H, -W); 
		LYNX_RV_SET_XYZ(lpts->V[7], LYNX_REAL(0), -H, W); 
	}
	if (lpts->Flag & LYNX_CROSSTEXSPRITE_Y)
	{
		LYNX_RV_SET_XYZ(lpts->V[8], -W, LYNX_REAL(0), W); 
		LYNX_RV_SET_XYZ(lpts->V[9], W, LYNX_REAL(0), W); 
		LYNX_RV_SET_XYZ(lpts->V[10], -W, LYNX_REAL(0), -W); 
		LYNX_RV_SET_XYZ(lpts->V[11], W, LYNX_REAL(0), -W); 
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxGetCrossTexSpriteColor(LPLYNXCROSSTEXSPRITE lpts, LPLYNXCOLORRGBA lpc)
{
	lpc->PackedColor = lpts->Color.PackedColor;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetCrossTexSpriteColor(LPLYNXCROSSTEXSPRITE lpts, LPLYNXCOLORRGBA lpc)
{
	lpts->Color.PackedColor = lpc->PackedColor;
	LYNX_RV_SET_RGBA(lpts->V[0], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[1], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[2], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[3], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[4], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[5], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[6], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[7], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[8], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[9], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[10], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
	LYNX_RV_SET_RGBA(lpts->V[11], lpts->Color.s.Red, lpts->Color.s.Green, lpts->Color.s.Blue, lpts->Color.s.Alpha);		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 LYNXBOOL	lynxPlayCrossTexSprite(LPLYNXCROSSTEXSPRITE lpts, LYNXREAL skip, LYNXPLAYMODE mode)
{
	lynxUpdateObject3D(&lpts->Obj3D);
	return lynxPlayTexSprite(&lpts->TexSprite, skip, mode);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRenderCrossTexSpriteBottom(LPLYNXCROSSTEXSPRITE lpts)
{
	int						i;

	lynxSetWorldMatrix(&lpts->Obj3D.M);
	
	for (i=0; i<lpts->TexSprite.Material.NumPass; i++)
	{
		lynxSetMaterialPass(&lpts->TexSprite.Material.lpMaterialPass[i]);
		if (lpts->Flag & LYNX_CROSSTEXSPRITE_Z)
			lynxDrawTriangleStrip(&lpts->V[0]);
		if (lpts->Flag & LYNX_CROSSTEXSPRITE_X)
			lynxDrawTriangleStrip(&lpts->V[4]);
		if (lpts->Flag & LYNX_CROSSTEXSPRITE_Y)
			lynxDrawTriangleStrip(&lpts->V[8]);
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCrossSprite 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxRenderCrossTexSpriteCenter(LPLYNXCROSSTEXSPRITE lpts)
{
	int							i;

	lynxSetWorldMatrix(&lpts->Obj3D.M);

	for (i=0; i<lpts->TexSprite.Material.NumPass; i++)
	{
		lynxSetMaterialPass(&lpts->TexSprite.Material.lpMaterialPass[i]);
		if (lpts->Flag & LYNX_CROSSTEXSPRITE_Z)
			lynxDrawTriangleStrip(&lpts->V[0]);
		if (lpts->Flag & LYNX_CROSSTEXSPRITE_X)
			lynxDrawTriangleStrip(&lpts->V[4]);
		if (lpts->Flag & LYNX_CROSSTEXSPRITE_Y)
			lynxDrawTriangleStrip(&lpts->V[8]);
	}
}