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

#include <LynxBone.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxBone 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInitBone(LPLYNXBONE lpbd)
{
	lynxInitObject3D(&lpbd->Obj3D);	
	/*
	LYNX_IDENTITY_MATRIX(lpbd->TransM);	
	lynxInitAnimationKey(&lpbd->TranslateKey);
	lynxInitAnimationKey(&lpbd->ScaleKey);		
	#ifdef __FIXED_POINT__
		lynxInitAnimationKey(&lpbd->RotateKey);		
	#else	
		lynxInitShortAnimationKey(&lpbd->RotateKey);		
	#endif
	lpbd->lpChildrenData = NULL;
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxBone 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxReleaseBone(LPLYNXBONE lpbd)
{
	lynxReleaseObject3D(&lpbd->Obj3D);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxBone 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInstanceBone(LPLYNXBONE lpd, LPLYNXBONE lps)
{
	int							i;

	lynxInitObject3D(&lpd->Obj3D);
	lynxInstanceObject3D(&lpd->Obj3D, &lps->Obj3D);
	/*
	lpd->ParentData.ChildIndex = lps->ParentData.ChildIndex;
	lpd->NumChildren = lps->NumChildren;
	if (lpd->NumChildren)
		lpd->lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpd->NumChildren*sizeof(LYNXLINKDATA));
	for (i=0; i<lpd->NumChildren; i++)
	{
		lpd->lpChildrenData[i].ChildIndex = lps->lpChildrenData[i].ChildIndex;
	}	
	lpd->TranslateKey.NumKeys = lps->TranslateKey.NumKeys; 
	lpd->TranslateKey.lpKeys = lps->TranslateKey.lpKeys; 
	lpd->RotateKey.NumKeys = lps->RotateKey.NumKeys;
	lpd->RotateKey.lpKeys = lps->RotateKey.lpKeys;
	lpd->ScaleKey.NumKeys = lps->ScaleKey.NumKeys;
	lpd->ScaleKey.lpKeys = lps->ScaleKey.lpKeys;
	lpd->VertexConstantIndex = lps->VertexConstantIndex;
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxBone 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxGetBoneTransM(LPLYNXBONE lpb, LYNXREAL time)
{
	lynxGetObject3DTransM(&lpb->Obj3D, time);

	/*
	LYNXREAL					lpValues[4];	
		
	LYNX_IDENTITY_MATRIX(lpb->TransM);		 	
	
	if (lpb->RotateKey.lpKeys)
	{
		#ifdef __FIXED_POINT__
			lynxGetAnimKeyValues(&lpb->RotateKey, time, lpValues, 4);			
		#else
			lynxGetShortAnimKeyValues(&lpb->RotateKey, time, lpValues, 4, LYNX_ROTATE_KEY_ISCALE);			
		#endif
		lynxQuaternion2Matrix(&lpb->TransM, (LPLYNXQUATERNION)lpValues);
	}				
	if (lpb->ScaleKey.lpKeys)
	{
		lynxGetAnimKeyValues(&lpb->ScaleKey, time, lpValues, 3);

		lpb->TransM.s.m11 = LYNX_REAL_MUL(lpb->TransM.s.m11, lpValues[0]);
		lpb->TransM.s.m21 = LYNX_REAL_MUL(lpb->TransM.s.m21, lpValues[1]);
		lpb->TransM.s.m31 = LYNX_REAL_MUL(lpb->TransM.s.m31, lpValues[2]);

		lpb->TransM.s.m12 = LYNX_REAL_MUL(lpb->TransM.s.m12, lpValues[0]);
		lpb->TransM.s.m22 = LYNX_REAL_MUL(lpb->TransM.s.m22, lpValues[1]);
		lpb->TransM.s.m32 = LYNX_REAL_MUL(lpb->TransM.s.m32, lpValues[2]);

		lpb->TransM.s.m13 = LYNX_REAL_MUL(lpb->TransM.s.m13, lpValues[0]);
		lpb->TransM.s.m23 = LYNX_REAL_MUL(lpb->TransM.s.m23, lpValues[1]);
		lpb->TransM.s.m33 = LYNX_REAL_MUL(lpb->TransM.s.m33, lpValues[2]);
	}				
	if (lpb->TranslateKey.lpKeys)
	{
		//lynxGetShortAnimKeyValues(&lpb->TranslateKey, time, lpValues, 3, LYNX_TRANSLATE_KEY_ISCALE);
		lynxGetAnimKeyValues(&lpb->TranslateKey, time, lpValues, 3);

		lpb->TransM.s.m41 += lpValues[0];
		lpb->TransM.s.m42 += lpValues[1];
		lpb->TransM.s.m43 += lpValues[2];
	}		
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxBone 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSetBoneAnimationKeys(LPLYNXBONE lpbone, LPLYNXANIMATIONKEY lptran, LPLYNXANIMATIONKEY lpscl, LPLYNXSHORTANIMATIONKEY lprot)
{	
}