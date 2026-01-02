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

#include <LynxTexSpr3D.h>

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
 LYNXBOOL	lynxPlayTexSprite3D(LPLYNXTEXSPRITE3D lpts, LYNXREAL skip, LYNXPLAYMODE mode)
{
	lynxUpdateObject3D(&lpts->Obj3D);
	return lynxPlayTexSprite(&lpts->TexSprite, skip, mode);
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
 void	lynxRenderTexSprite3DBottom(LPLYNXTEXSPRITE3D lpts)
{
	LYNXVECTOR3D			Pos;	
	LYNXMATRIX				M;
	LYNXREAL				w, h;		
	int						p;
	
	lynxPointXMatrix(&Pos, lpts->Obj3D.lpCenter, &lynxGetlpRenderer()->Sprite3DViewMatrix);		
	h = LYNX_REAL_MUL(lpts->TexSprite.Height, LYNX_REAL_HALF);;
	Pos.s.y += h;
	LYNX_IDENTITY_MATRIX(M);
	M.s.m41 = Pos.s.x;
	M.s.m42 = Pos.s.y;
	M.s.m43 = Pos.s.z;
	lynxSetWorldMatrix(&M);

	for (p=0; p<lpts->TexSprite.Material.NumPass; p++)
	{
		lynxSetMaterialPass(&lpts->TexSprite.Material.lpMaterialPass[p]);				
		lynxDrawTriangleStrip(lpts->TexSprite.V);				
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
 void	lynxRenderTexSprite3DCenter(LPLYNXTEXSPRITE3D lpts)
{
	LYNXVECTOR3D			Pos;
	LYNXMATRIX				M;
	int						p;
	
	lynxPointXMatrix(&Pos, lpts->Obj3D.lpCenter, &lynxGetlpRenderer()->Sprite3DViewMatrix);	

	LYNX_IDENTITY_MATRIX(M);
	M.s.m41 = Pos.s.x;
	M.s.m42 = Pos.s.y;
	M.s.m43 = Pos.s.z;
	lynxSetWorldMatrix(&M);

	for (p=0; p<lpts->TexSprite.Material.NumPass; p++)
	{
		lynxSetMaterialPass(&lpts->TexSprite.Material.lpMaterialPass[p]);				
		lynxDrawTriangleStrip(lpts->TexSprite.V);		
	}
}
//-------------------------------------------------------------------------------------------------------
//Virtual functions pointer table of LynxObj3D
//-------------------------------------------------------------------------------------------------------
/*
static LYNXOBJ3DVIRTUALTABLE				gTexSprite3D_vtbl = {lynxPlayTexSprite3D,
																 lynxRenderTexSprite3DCenter,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL,
																 NULL	};		
																 */
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
 void	lynxInitTexSprite3D(LPLYNXTEXSPRITE3D lpts)
{
	lynxInitObject3D(&lpts->Obj3D);
	lynxInitTexSprite(&lpts->TexSprite);		
	lpts->Obj3D.Play = (LPLYNXPLAYPROC)lynxPlayTexSprite3D;
	lynxSetTexSprite3DRenderType(lpts, LYNX_SPRITE3D_RENDER_CENTER);

	//lpts->Obj3D.vptr = &gTexSprite3D_vtbl;
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
 void	lynxReleaseTexSprite3D(LPLYNXTEXSPRITE3D lpts)
{	
	lynxReleaseTexSprite(&lpts->TexSprite);
	lynxReleaseObject3D(&lpts->Obj3D);
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
 void	lynxDuplicateTexSprite3D(LPLYNXTEXSPRITE3D lptsd, LPLYNXTEXSPRITE3D lpts)
{
	lynxMemCpy(&lptsd->Obj3D, &lpts->Obj3D, sizeof(LYNXOBJ3D));
	lynxDuplicateTexSprite(&lptsd->TexSprite, &lpts->TexSprite);
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
 void	lynxLoadTexSprite3D(LPLYNXTEXSPRITE3D lpts, const char *filename, LYNXREAL w, LYNXREAL h, LPLYNXCOLORRGBA lpc)
{
	LYNXREAL				W, H;

	lynxInitTexSprite3D(lpts);
	lynxLoadTexSprite(&lpts->TexSprite, filename, w, h, lpc, LYNX_NORMAL_TEXTURE);

	W = LYNX_REAL_MUL(w, LYNX_REAL_HALF);
	H = LYNX_REAL_MUL(h, LYNX_REAL_HALF);	
		
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[0], -W, H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[1], W, H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[2], -W, -H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[3], W, -H, LYNX_REAL(0)); 	

	return;
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
 void	lynxLoadTexSprite3Ds(LPLYNXTEXSPRITE3D lpts, int num, const char *filename, const char *ext, LYNXREAL w, LYNXREAL h, LPLYNXCOLORRGBA lpc)
{
	LYNXREAL				W, H;

	lynxInitTexSprite3D(lpts);
	lynxLoadTexSprites(&lpts->TexSprite, num, filename, ext, w, h, lpc, LYNX_NORMAL_TEXTURE);

	W = LYNX_REAL_MUL(w, LYNX_REAL_HALF);
	H = LYNX_REAL_MUL(h, LYNX_REAL_HALF);	
		
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[0], -W, H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[1], W, H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[2], -W, -H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[3], W, -H, LYNX_REAL(0)); 

	return;
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
 void	lynxSetTexSprite3DUVOffset(LPLYNXTEXSPRITE3D lpts, int x, int y, int w, int h)
{
	lynxSetTexSpriteUVOffset(&lpts->TexSprite, x, y, w, h);
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
 void	lynxSetTexSprite3DNumFrames(LPLYNXTEXSPRITE3D lpts, int num)
{
	lynxSetTexSpriteNumFrames(&lpts->TexSprite, num);
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
 void	lynxSetTexSprite3DCacheType(LPLYNXTEXSPRITE3D lpts, LYNXTEXTURECACHETYPE type)
{
	lynxSetTexSpriteCacheType(&lpts->TexSprite, type);		
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
 void	lynxGetTexSprite3DSize(LPLYNXTEXSPRITE3D lpts, LYNXREAL *w, LYNXREAL *h)
{
	lynxGetTexSpriteSize(&lpts->TexSprite, w, h);	
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
 void	lynxSetTexSprite3DSize(LPLYNXTEXSPRITE3D lpts, LYNXREAL w, LYNXREAL h)
{
	LYNXREAL				W, H;

	lynxSetTexSpriteSize(&lpts->TexSprite, w, h);		

	W = LYNX_REAL_MUL(w, LYNX_REAL_HALF);
	H = LYNX_REAL_MUL(h, LYNX_REAL_HALF);	
		
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[0], -W, H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[1], W, H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[2], -W, -H, LYNX_REAL(0)); 
	LYNX_RV_SET_XYZ(lpts->TexSprite.V[3], W, -H, LYNX_REAL(0)); 	
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
 LYNXSPRITE3DRENDERTYPE	lynxGetTexSprite3DRenderType(LPLYNXTEXSPRITE3D lpts)
{
	return (lpts->RenderType);
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
 void	lynxSetTexSprite3DRenderType(LPLYNXTEXSPRITE3D lpts, LYNXSPRITE3DRENDERTYPE type)
{
	lpts->RenderType = type;
	switch (type)
	{
		case LYNX_SPRITE3D_RENDER_CENTER:		
			//gTexSprite3D_vtbl.Render = (LPLYNXRENDERPROC)lynxRenderTexSprite3DCenter;
			lpts->Obj3D.Render = (LPLYNXRENDERPROC)lynxRenderTexSprite3DCenter;
			break;

		case LYNX_SPRITE3D_RENDER_BOTTOM:		
			//gTexSprite3D_vtbl.Render = (LPLYNXRENDERPROC)lynxRenderTexSprite3DBottom;
			lpts->Obj3D.Render = (LPLYNXRENDERPROC)lynxRenderTexSprite3DBottom;
			break;
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
 void	lynxGetTexSprite3DColor(LPLYNXTEXSPRITE3D lpts, LPLYNXCOLORRGBA lpc)
{
	lynxGetTexSpriteColor(&lpts->TexSprite, lpc);
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
 void	lynxSetTexSprite3DColor(LPLYNXTEXSPRITE3D lpts, LPLYNXCOLORRGBA lpc)
{
	lynxSetTexSpriteColor(&lpts->TexSprite, lpc);
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
 void	lynxRenderTexSprite3DCenterRotate(LPLYNXTEXSPRITE3D lpts, LYNXREAL deg)
{
	LYNXVECTOR3D			Pos;	
	LYNXMATRIX				M;
	LYNXREAL				sn, cs;	
	int						p;
	
	lynxPointXMatrix(&Pos, lpts->Obj3D.lpCenter, &lynxGetlpRenderer()->Sprite3DViewMatrix);		
	
	sn = lynxQuickSin(deg);
	cs = lynxQuickCos(deg);		
	
	LYNX_IDENTITY_MATRIX(M);
	M.s.m11 = cs;
	M.s.m12 = sn;
	M.s.m21 = -sn;
	M.s.m22 = cs;		
	M.s.m41 = Pos.s.x;
	M.s.m42 = Pos.s.y;
	M.s.m43 = Pos.s.z;
	lynxSetWorldMatrix(&M);

	for (p=0; p<lpts->TexSprite.Material.NumPass; p++)
	{
		lynxSetMaterialPass(&lpts->TexSprite.Material.lpMaterialPass[p]);				
		lynxDrawTriangleStrip(lpts->TexSprite.V);
	}
}