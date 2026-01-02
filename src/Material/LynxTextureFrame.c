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
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInitTextureFrame(LPLYNXTEXTUREFRAME lptf)
{
	lptf->NumTexs = 0;
	lptf->lplpTexList = NULL;
	lptf->lpTexIndexList = NULL;	
	lptf->Time = LYNX_REAL(0);
	lptf->StartTime = LYNX_REAL(0);
	lptf->EndTime = LYNX_REAL(0);
	lptf->PlayMode = LYNX_PM_ONCE;
	lptf->MipMapLevel = 1;
	lptf->SrcBlend = LYNX_BF_SRC_ALPHA;
	lptf->DestBlend = LYNX_BF_ONE_MINUS_SRC_ALPHA;	
	lptf->MinFilter = LYNX_TF_LINEAR;
	lptf->MagFilter = LYNX_TF_LINEAR;
	lptf->MipFilter = LYNX_TF_LINEAR;
	lptf->SourceType = LYNX_ST_ORIGIN;
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
void	lynxReleaseTextureFrame(LPLYNXTEXTUREFRAME lptf)
{
	switch (lptf->SourceType)
	{
		case LYNX_ST_ORIGIN:
			LYNX_FREE(lptf->lplpTexList);
			LYNX_FREE(lptf->lpTexIndexList);		
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
void	lynxInstanceTextureFrame(LPLYNXTEXTUREFRAME lpdtf, LPLYNXTEXTUREFRAME lpstf)
{
	int						i;

	lpdtf->NumTexs = lpstf->NumTexs;
	lpdtf->lpTexIndexList = lpstf->lpTexIndexList;
	lpdtf->lplpTexList =  lpstf->lplpTexList;
	lpdtf->Time = lpstf->Time;
	lpdtf->StartTime = lpstf->StartTime;
	lpdtf->EndTime = lpstf->EndTime;
	lpdtf->PlayMode = lpstf->PlayMode;
	lpdtf->MipMapLevel = lpstf->MipMapLevel;
	lpdtf->SrcBlend = lpstf->SrcBlend;
	lpdtf->DestBlend = lpstf->DestBlend;	
	lpdtf->MinFilter = lpstf->MinFilter;
	lpdtf->MagFilter = lpstf->MagFilter;
	lpdtf->SourceType = LYNX_ST_INSTANCE;
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
void	lynxDuplicateTextureFrame(LPLYNXTEXTUREFRAME lpdtf, LPLYNXTEXTUREFRAME lpstf)
{
	int						i;

	lpdtf->NumTexs = lpstf->NumTexs;
	lpdtf->lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(lpstf->NumTexs*sizeof(LYNXTEXTUREINDEX));
	lpdtf->lplpTexList =  (char (*)[64])LYNX_MALLOC(sizeof(char)*64*lpstf->NumTexs);
	for (i=0; i<lpstf->NumTexs; i++)
	{
		lynxDuplicateTexture(&lpdtf->lpTexIndexList[i], lpstf->lpTexIndexList[i]);
		strcpy(lpdtf->lplpTexList[i], "");
		if (lpstf->lplpTexList)
			strcpy(lpdtf->lplpTexList[i], lpstf->lplpTexList[i]);
	}
	lpdtf->Time = lpstf->Time;
	lpdtf->StartTime = lpstf->StartTime;
	lpdtf->EndTime = lpstf->EndTime;
	lpdtf->PlayMode = lpstf->PlayMode;
	lpdtf->MipMapLevel = lpstf->MipMapLevel;
	lpdtf->SrcBlend = lpstf->SrcBlend;
	lpdtf->DestBlend = lpstf->DestBlend;	
	lpdtf->MinFilter = lpstf->MinFilter;
	lpdtf->MagFilter = lpstf->MagFilter;
	lpdtf->SourceType = LYNX_ST_ORIGIN;
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
void	lynxLoadTextureFrameTexture(LPLYNXTEXTUREFRAME lptf, LPLYNXTEXTUREINFO *lptinfo, int mipmap, const char *filename, LPLYNXFILE lpf, long offset, LYNXTEXTURETYPE type)
{
	lptf->Time = LYNX_REAL(0);
	lptf->StartTime = LYNX_REAL(0);
	lptf->EndTime = LYNX_REAL(0);		
	lptf->NumTexs = 1;
	lptf->lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lptf->NumTexs);
	lptf->lpTexIndexList[0] = LYNX_NULL_TEXTURE;
	lynxLoadTexture(&lptf->lpTexIndexList[0], lptinfo, 1, filename, lpf, offset, type);	
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
void	lynxLoadTextureFrameTextures(LPLYNXTEXTUREFRAME lptf, LPLYNXTEXTUREINFO *lptinfo, int mipmap, int num, const char *filename, const char *ext, LPLYNXFILE lpf, long offset, LYNXTEXTURETYPE type)
{
	int									i, Len;
	char								FullName[64];

	strcpy(FullName, filename);
	Len = (int)strlen(FullName);	

	lptf->Time = LYNX_REAL(0);
	lptf->StartTime = LYNX_REAL(0);
	lptf->EndTime = LYNX_REAL(num-1);
	lptf->NumTexs = num;
	lptf->lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lptf->NumTexs);
	for (i=0; i<lptf->NumTexs; i++)
		lptf->lpTexIndexList[i] = LYNX_NULL_TEXTURE;
	for (i=0; i<num; i++)
	{
		FullName[Len + 0] = (i / 100) + 0x30;
		FullName[Len + 1] = (i / 10) + 0x30;
		FullName[Len + 2] = (i % 10) + 0x30;
		FullName[Len + 3] = '.';
		FullName[Len + 4] = ext[0];
		FullName[Len + 5] = ext[1];
		FullName[Len + 6] = ext[2];
		FullName[Len + 7] = 0x00;
		
		lynxLoadTexture(&lptf->lpTexIndexList[i], lptinfo, 1, FullName, lpf, offset, type);				
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
void	lynxSetTextureFrameTimes(LPLYNXTEXTUREFRAME lptf, LYNXREAL st, LYNXREAL et, LYNXREAL t)
{	
	lptf->StartTime = st;
	lptf->EndTime = et;
	lptf->Time = t;
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
LYNXBOOL	lynxPlayTextureFrame(LPLYNXTEXTUREFRAME lptf, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LYNXBOOL						b;	

	b = FALSE;	
	lptf->Time = LYNX_REAL_ADD(lptf->Time, skip);

	switch (mode)
	{
		case LYNX_PM_LOOP :	
			while (lptf->Time > lptf->EndTime)
			{
				lptf->Time = LYNX_REAL_SUB(lptf->Time, LYNX_REAL_ADD(lptf->EndTime, LYNX_REAL(1)));
				lptf->Time = LYNX_REAL_ADD(lptf->Time, lptf->StartTime);
				b = TRUE;
			}
			while (lptf->Time < lptf->StartTime)
			{
				lptf->Time = LYNX_REAL_SUB(lptf->Time, LYNX_REAL_SUB(lptf->StartTime, LYNX_REAL(1)));
				lptf->Time = LYNX_REAL_ADD(lptf->Time, lptf->EndTime);			
				b = TRUE;
			}	
			break;

		case LYNX_PM_ONCE:
			if (lptf->Time > lptf->EndTime)
			{
				b = TRUE;
				lptf->Time = lptf->EndTime;
			}
			else if (lptf->Time < lptf->StartTime)
			{
				b = TRUE;
				lptf->Time = lptf->StartTime;
			}
			break;
	}	
	return b;
}