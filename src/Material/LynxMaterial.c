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
void	lynxInitMaterial(LPLYNXMATERIAL lpm)
{
	strcpy(lpm->Name, "null");
	lpm->NumPass = 0;
	lpm->lpMaterialPass = NULL;
	lpm->SourceType = LYNX_ST_ORIGIN;
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
void lynxReleaseMaterial(LPLYNXMATERIAL lpm)
{	
	int							i, j;

	switch (lpm->SourceType)
	{
		case LYNX_ST_ORIGIN:
			for (i=0; i<lpm->NumPass; i++)
			{
				lynxReleaseMaterialPass(&lpm->lpMaterialPass[i]);		
			}	
			LYNX_FREE(lpm->lpMaterialPass);
			break;
	}
	lpm->NumPass = 0;
	lpm->lpMaterialPass = NULL;
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
void	lynxInstanceMaterial(LPLYNXMATERIAL lpdm, LPLYNXMATERIAL lpsm)
{
	lynxReleaseMaterial(lpdm);
	lynxInitMaterial(lpdm);
	strcpy(lpdm->Name, lpsm->Name);
	lpdm->NumPass = lpsm->NumPass;
	lpdm->lpMaterialPass =lpsm->lpMaterialPass;
	lpdm->SourceType = LYNX_ST_INSTANCE;
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
void	lynxDuplicateMaterial(LPLYNXMATERIAL lpdm, LPLYNXMATERIAL lpsm)
{
	int								i;

	lynxReleaseMaterial(lpdm);
	lynxInitMaterial(lpdm);
	strcpy(lpdm->Name, lpsm->Name);
	lpdm->NumPass = lpsm->NumPass;
	lpdm->lpMaterialPass = LYNX_MALLOC(sizeof(LYNXMATERIALPASS)*lpdm->NumPass);
	for (i=0; i<lpdm->NumPass; i++)
	{
		lynxDuplicateMaterialPass(&lpdm->lpMaterialPass[i], &lpsm->lpMaterialPass[i]);		
	}
	lpdm->SourceType = LYNX_ST_ORIGIN;
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
void	lynxCreateMaterial(LPLYNXMATERIAL lpm, int numpass)
{
	int				i;

	lynxInitMaterial(lpm);
	lpm->NumPass = numpass;
	lpm->lpMaterialPass = LYNX_MALLOC(sizeof(LYNXMATERIALPASS)*lpm->NumPass);
	for (i=0; i<lpm->NumPass; i++)
	{
		lynxInitMaterialPass(&lpm->lpMaterialPass[i]);		
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
void	lynxLoadMaterial(LPLYNXMATERIAL lpm, LPLYNXFILE lpf)
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
LYNXBOOL lynxPlayMaterial(LPLYNXMATERIAL lpm, int channel, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LYNXBOOL			bRet;
	int					i;

	bRet = LYNX_TRUE;
	for (i=0; i<lpm->NumPass; i++)
		bRet &= lynxPlayMaterialPass(&lpm->lpMaterialPass[i], channel, skip, mode);

	return bRet;	
}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxMaterial 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxTextureMotion(LPLYNXRENDERVERTEX lprest, LYNXTEXTUREMOTION type, LPLYNXVERTEX3D lporg, int ch, int sv, int ev, float *lparg, float skip)
//{
//	int							i, j;
//	float						Phase, Amp, Speed, Step, Inc, AmpY, PhaseU, PhaseV;	
//	float						sn, cs, U, V, BaseU, BaseV;
//	static	float				Deg = 3.0f;
//	LYNXBOOL					bResetU, bResetV;	
//
//	switch (type)	
//	{
//		case LYNX_TM_SCROLL:
//			bResetU = FALSE;
//			bResetV = FALSE;
//			for (j=sv; j<ev; j++)
//			{
//				LYNX_RV_U(lprest[j], ch) += lparg[0]*skip;
//				if (LYNX_RV_U(lprest[j], ch) > 100.0f)
//				{
//					bResetU = TRUE;
//					BaseU = -100.0f;
//				}
//				else if (LYNX_RV_U(lprest[j], ch) < -100.0f)
//				{
//					bResetU = TRUE;
//					BaseU = 100.0f;
//				}
//
//				LYNX_RV_V(lprest[j], ch) += lparg[1]*skip;
//				if (LYNX_RV_V(lprest[j], ch) > 100.0f)
//				{
//					bResetV = TRUE;
//					BaseV = -100.0f;
//				}
//				else if (LYNX_RV_V(lprest[j], ch) < -100.0f)
//				{
//					bResetV = TRUE;
//					BaseV = 100.0f;
//				}
//			}
//
//			if (bResetU || bResetV)
//			{
//				for (j=sv; j<ev; j++)
//				{
//					if (bResetU)
//						LYNX_RV_U(lprest[j], ch) += BaseU;
//					if (bResetV)
//						LYNX_RV_V(lprest[j], ch) += BaseV;					
//				}
//			}
//			break;
//
//		case LYNX_TM_SINWAVE:			
//			Amp = lparg[0]/1000.0f;
//			Step = lparg[1]*LYNXPI;
//			Speed = lparg[2]*skip;
//			Phase = lparg[3];
//			PhaseU = lparg[4];
//			PhaseV = lparg[5];
//			lparg[6] += Speed;
//			if (lparg[6] > (LYNXPI*2))
//				lparg[6] -= (LYNXPI*2);
//			Inc = lparg[6];				
//			for (j=sv; j<ev; j++)
//			{
//				LYNX_RV_U(lprest[j], ch) = lporg[j-sv].TexCoord[ch].u + sin(lporg[j-sv].TexCoord[ch].v*Step+Inc+PhaseU*lporg[j-sv].TexCoord[ch].u)*Amp;
//				LYNX_RV_V(lprest[j], ch) = lporg[j-sv].TexCoord[ch].v + sin(lporg[j-sv].TexCoord[ch].u*Step+Inc+Phase+PhaseV*lporg[j-sv].TexCoord[ch].v)*Amp;														
//			}			
//			break;
//
//		case LYNX_TM_ROTATE:		
//			sn = sin(lparg[3]*LYNXD2R);
//			cs = cos(lparg[3]*LYNXD2R);			
//			for (j=sv; j<ev; j++)
//			{
//				U = lporg[j-sv].TexCoord[ch].u - lparg[1];
//				V = lporg[j-sv].TexCoord[ch].v - lparg[2];
//
//				LYNX_RV_U(lprest[j], ch) = U*cs - V*sn;
//				LYNX_RV_V(lprest[j], ch) = U*sn + V*cs;
//
//				LYNX_RV_U(lprest[j], ch) += lparg[1];
//				LYNX_RV_V(lprest[j], ch) += lparg[2];
//			}			
//			lparg[3] += lparg[0]*skip;			
//			break;
//	}			
//}