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

#include <LynxObj3D.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxObject3DDefaultPlay(void *lpm, LYNXREAL skip, LYNXPLAYMODE mode)
{
	return LYNX_TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxObject3DDefaultRender(void *lpm)
{
}
/*
//-------------------------------------------------------------------------------------------------------
//Virtual functions pointer table of LynxObj3D
//-------------------------------------------------------------------------------------------------------
static LYNXOBJ3DVIRTUALTABLE				gObj3D_vtbl = {lynxObject3DDefaultPlay,
														   lynxObject3DDefaultRender,
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
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitObject3D(LPLYNXOBJ3D lpo)
{
	strcpy(lpo->Name, "");
	lpo->lpCenter = (LPLYNXVECTOR3D)&lpo->M.s.m41;
	lpo->lpRightDir = (LPLYNXVECTOR3D)&lpo->M.s.m11;
	lpo->lpUpDir = (LPLYNXVECTOR3D)&lpo->M.s.m21;
	lpo->lpDirection = (LPLYNXVECTOR3D)&lpo->M.s.m31;

	LYNX_IDENTITY_MATRIX(lpo->M);
	LYNX_IDENTITY_MATRIX(lpo->LocalM);
	LYNX_IDENTITY_MATRIX(lpo->GlobalM);
	LYNX_IDENTITY_MATRIX(lpo->PivotM);	
	
	lpo->ParentData.ChildIndex = -1;
	lpo->ParentData.lpObject = NULL;
	lpo->NumChildren = 0;
	lpo->lpChildrenData = NULL;

	lpo->Visibility = LYNX_REAL(1.0f);
	lpo->Time = lpo->StartTime = lpo->EndTime = lpo->LastTime = LYNX_REAL(0);
	LYNX_IDENTITY_MATRIX(lpo->TransM);
	lynxInitAnimationKey(&lpo->TranslateKey);
	lynxInitAnimationKey(&lpo->ScaleKey);		
	#ifdef __FIXED_POINT__
		lynxInitAnimationKey(&lpo->RotateKey);
		lynxInitAnimationKey(&lpo->VisKey);
	#else	
		lynxInitShortAnimationKey(&lpo->RotateKey);
		lynxInitShortAnimationKey(&lpo->VisKey);	
	#endif

	lpo->Play = lynxObject3DDefaultPlay;
	lpo->Render = lynxObject3DDefaultRender;	

	//lpo->vptr = &gObj3D_vtbl;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxReleaseObject3D(LPLYNXOBJ3D lpo)
{    
	lpo->NumChildren = 0;
	LYNX_FREE(lpo->lpChildrenData);
	
	lynxReleaseAnimationKey(&lpo->TranslateKey);
	lynxReleaseAnimationKey(&lpo->ScaleKey);		
	#ifdef __FIXED_POINT__
		lynxReleaseAnimationKey(&lpo->RotateKey);
		lynxReleaseAnimationKey(&lpo->VisKey);
	#else	
		lynxReleaseShortAnimationKey(&lpo->RotateKey);
		lynxReleaseShortAnimationKey(&lpo->VisKey);	
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxDuplicateObject3D(LPLYNXOBJ3D lpdo, LPCLYNXOBJ3D lpso)
{
	int							i;

	lynxMemCpy(lpdo, lpso, sizeof(LYNXOBJ3D));	
	lpdo->lpCenter = (LPLYNXVECTOR3D)&lpdo->M.s.m41;
	lpdo->lpRightDir = (LPLYNXVECTOR3D)&lpdo->M.s.m11;
	lpdo->lpUpDir = (LPLYNXVECTOR3D)&lpdo->M.s.m21;
	lpdo->lpDirection = (LPLYNXVECTOR3D)&lpdo->M.s.m31;	
	
	lpdo->lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpdo->NumChildren*sizeof(LYNXLINKDATA));	
	for (i=0; i<lpdo->NumChildren; i++)
	{
		lpdo->lpChildrenData[i].ChildIndex = lpso->lpChildrenData[i].ChildIndex;
	}

	lynxDuplicateAnimationKey(&lpdo->TranslateKey, &lpso->TranslateKey);
	lynxDuplicateAnimationKey(&lpdo->ScaleKey, &lpso->ScaleKey);
	#ifdef __FIXED_POINT__
		lynxDuplicateAnimationKey(&lpdo->RotateKey, &lpso->RotateKey);
		lynxDuplicateAnimationKey(&lpdo->VisKey, &lpso->VisKey);		
	#else
		lynxDuplicateShortAnimationKey(&lpdo->RotateKey, &lpso->RotateKey);
		lynxDuplicateShortAnimationKey(&lpdo->VisKey, &lpso->VisKey);		
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInstanceObject3D(LPLYNXOBJ3D lpdo, LPCLYNXOBJ3D lpso)
{
	int						i;

	lynxMemCpy(lpdo, lpso, sizeof(LYNXOBJ3D));	
	lpdo->lpCenter = (LPLYNXVECTOR3D)&lpdo->M.s.m41;
	lpdo->lpRightDir = (LPLYNXVECTOR3D)&lpdo->M.s.m11;
	lpdo->lpUpDir = (LPLYNXVECTOR3D)&lpdo->M.s.m21;
	lpdo->lpDirection = (LPLYNXVECTOR3D)&lpdo->M.s.m31;

	lpdo->lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpdo->NumChildren*sizeof(LYNXLINKDATA));	
	for (i=0; i<lpdo->NumChildren; i++)
	{
		lpdo->lpChildrenData[i].ChildIndex = lpso->lpChildrenData[i].ChildIndex;
	}

	lynxInstanceAnimationKey(&lpdo->TranslateKey, &lpso->TranslateKey);
	lynxInstanceAnimationKey(&lpdo->ScaleKey, &lpso->ScaleKey);
	#ifdef __FIXED_POINT__
		lynxInstanceAnimationKey(&lpdo->RotateKey, &lpso->RotateKey);
		lynxInstanceAnimationKey(&lpdo->VisKey, &lpso->VisKey);		
	#else
		lynxInstanceShortAnimationKey(&lpdo->RotateKey, &lpso->RotateKey);
		lynxInstanceShortAnimationKey(&lpdo->VisKey, &lpso->VisKey);		
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxResetObject3DOrientation(LPLYNXOBJ3D lpo)
{
	LYNX_RESET_MATRIX_ORIENTATION(lpo->M);
	LYNX_RESET_MATRIX_ORIENTATION(lpo->LocalM);
	LYNX_RESET_MATRIX_ORIENTATION(lpo->GlobalM);				
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxResetObject3DTransform(LPLYNXOBJ3D lpo)
{
	LYNX_IDENTITY_MATRIX(lpo->M);
	LYNX_IDENTITY_MATRIX(lpo->LocalM);
	LYNX_IDENTITY_MATRIX(lpo->GlobalM);				
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxScaleObject3D(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpv, LYNXMATHORDER order)
{
	/*
	LYNXMATRIX				T, R;

	LYNX_IDENTITY_MATRIX(T);
	T.s.m11 = lpv->s.x;
	T.s.m22 = lpv->s.z;
	T.s.m33 = lpv->s.y;

	lynxMatrixXMatrix(&R, &lpo->LocalM, &T);	
	lynxMemCpy(&lpo->LocalM, &R, sizeof(LYNXMATRIX));
	*/

	LYNXMATRIX				T, R;

	LYNX_IDENTITY_MATRIX(T);
	T.s.m11 = lpv->s.x;
	T.s.m22 = lpv->s.y;
	T.s.m33 = lpv->s.z;

	switch (order)
	{
		case LYNX_MATH_PRE:
			lynxMatrixXMatrix(&R, &T, &lpo->LocalM);	
			lynxMemCpy(&lpo->LocalM, &R, sizeof(LYNXMATRIX));
			break;

		case LYNX_MATH_POST:
			lynxMatrixXMatrix(&R, &lpo->GlobalM, &T);	
			lynxMemCpy(&lpo->GlobalM, &R, sizeof(LYNXMATRIX));
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetObject3DScale(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpv)
{
	lpo->LocalM.s.m11 = lpv->s.x;
	lpo->LocalM.s.m22 = lpv->s.z;
	lpo->LocalM.s.m33 = lpv->s.y;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxTranslateObject3D(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpv, LYNXMATHORDER order)
{	
	/*LYNXMATRIX				T, R;

	LYNX_IDENTITY_MATRIX(T);
	T.s.m41 = lpv->s.x;
	T.s.m42 = lpv->s.y;
	T.s.m43 = lpv->s.z;

	lynxMatrixXMatrix(&R, &lpo->GlobalM, &T);	
	lynxMemCpy(&lpo->GlobalM, &R, sizeof(LYNXMATRIX));
	*/

	LYNXMATRIX				T, R;

	LYNX_IDENTITY_MATRIX(T);
	T.s.m41 = lpv->s.x;
	T.s.m42 = lpv->s.y;
	T.s.m43 = lpv->s.z;

	switch (order)
	{
		case LYNX_MATH_PRE:
			lynxMatrixXMatrix(&R, &T, &lpo->LocalM);	
			lynxMemCpy(&lpo->LocalM, &R, sizeof(LYNXMATRIX));
			break;

		case LYNX_MATH_POST:
			lynxMatrixXMatrix(&R, &lpo->GlobalM, &T);	
			lynxMemCpy(&lpo->GlobalM, &R, sizeof(LYNXMATRIX));
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxRotateObject3D(LPLYNXOBJ3D lpo, LYNXREAL f, int axis, LYNXMATHORDER order)
{	
	/*LYNXMATRIX				 R, T;

	lynxMakeRotateMatrix(&R, f, axis);	
	lynxMatrixXMatrix(&T, &lpo->GlobalM, &R);	
	lynxMemCpy(&lpo->GlobalM, &T, sizeof(LYNXMATRIX));*/

	LYNXMATRIX				T, R;

	lynxMakeRotateMatrix(&T, f, axis);	
	switch (order)
	{
		case LYNX_MATH_PRE:
			lynxMatrixXMatrix(&R, &T, &lpo->LocalM);	
			lynxMemCpy(&lpo->LocalM, &R, sizeof(LYNXMATRIX));
			break;

		case LYNX_MATH_POST:
			lynxMatrixXMatrix(&R, &lpo->GlobalM, &T);	
			lynxMemCpy(&lpo->GlobalM, &R, sizeof(LYNXMATRIX));
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxRotateObject3DWithAxis(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpaxis, LYNXREAL ang, LYNXMATHORDER order)
{	
	/*LYNXMATRIX				 R, T;

	lynxMakeRotateMatrixRespectVector(&R, lpaxis, ang);	
	lynxMatrixXMatrix(&T, &lpo->GlobalM, &R);	
	lynxMemCpy(&lpo->GlobalM, &T, sizeof(LYNXMATRIX));*/
	LYNXMATRIX				T, R;

	lynxMakeRotateMatrixRespectVector(&T, lpaxis, ang);	
	switch (order)
	{
		case LYNX_MATH_PRE:
			lynxMatrixXMatrix(&R, &T, &lpo->LocalM);	
			lynxMemCpy(&lpo->LocalM, &R, sizeof(LYNXMATRIX));
			break;

		case LYNX_MATH_POST:
			lynxMatrixXMatrix(&R, &lpo->GlobalM, &T);	
			lynxMemCpy(&lpo->GlobalM, &R, sizeof(LYNXMATRIX));
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxUpdateObject3D(LPLYNXOBJ3D lpo)
{
	LYNXMATRIX				TM1;

	/*
	lynxGetObject3DTransM(lpo, lpo->Time);
	lynxMatrixXMatrix(&TM1, &lpo->LocalM, &lpo->TransM);	 	    						
	lynxMatrixXMatrix(&lpo->M, &TM1, &lpo->GlobalM);	 	
	lynxMemCpy(&lpo->TransM, &lpo->M, sizeof(LYNXMATRIX));	
	*/
	lynxMatrixXMatrix(&lpo->M, &lpo->LocalM, &lpo->GlobalM);
	lynxMemCpy(&lpo->TransM, &lpo->M, sizeof(LYNXMATRIX));	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetObject3DPosition(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpv)
{	
	LYNXVECTOR3D					V;

	V.s.x = lpv->s.x - lpo->lpCenter->s.x;
	V.s.y = lpv->s.y - lpo->lpCenter->s.y;
	V.s.z = lpv->s.z - lpo->lpCenter->s.z;

	lynxTranslateObject3D(lpo, &V, LYNX_MATH_POST);	
	lynxUpdateObject3D(lpo);	

	/*
	lpo->lpCenter->s.x = lpv->s.x;
	lpo->lpCenter->s.y = lpv->s.y;
	lpo->lpCenter->s.z = lpv->s.z;
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetObject3DPosition(LPLYNXVECTOR3D lpv, LPLYNXOBJ3D lpo)
{	
	lpv->s.x = lpo->lpCenter->s.x;
	lpv->s.y = lpo->lpCenter->s.y;
	lpv->s.z = lpo->lpCenter->s.z;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxForwardObject3D(LPLYNXOBJ3D lpo, LYNXREAL d)
{	
	LYNXVECTOR3D					V;
	
	V.s.x = LYNX_REAL_MUL(lpo->lpDirection->s.x, d);
	V.s.y = LYNX_REAL_MUL(lpo->lpDirection->s.y, d);
	V.s.z = LYNX_REAL_MUL(lpo->lpDirection->s.z, d);

	lynxTranslateObject3D(lpo, &V, LYNX_MATH_POST);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSideMoveObject3D(LPLYNXOBJ3D lpo, LYNXREAL d)
{	
	LYNXVECTOR3D					V;
	
	V.s.x = LYNX_REAL_MUL(lpo->lpRightDir->s.x, d);
	V.s.y = LYNX_REAL_MUL(lpo->lpRightDir->s.y, d);
	V.s.z = LYNX_REAL_MUL(lpo->lpRightDir->s.z, d);

	lynxTranslateObject3D(lpo, &V, LYNX_MATH_POST);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxUpObject3D(LPLYNXOBJ3D lpo, LYNXREAL d)
{	
	LYNXVECTOR3D					V;
	
	V.s.x = LYNX_REAL_MUL(lpo->lpUpDir->s.x, d);
	V.s.y = LYNX_REAL_MUL(lpo->lpUpDir->s.y, d);
	V.s.z = LYNX_REAL_MUL(lpo->lpUpDir->s.z, d);

	lynxTranslateObject3D(lpo, &V, LYNX_MATH_POST);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxObject3DXMatrix(LPLYNXOBJ3D lpo, LPCLYNXMATRIX lpm, LYNXMATHORDER order)
{
	LYNXMATRIX				T;

	switch (order)
	{
		case LYNX_MATH_PRE:
			lynxMatrixXMatrix(&T, lpm, &lpo->LocalM);	
			lynxMemCpy(&lpo->LocalM, &T, sizeof(LYNXMATRIX));
			break;

		case LYNX_MATH_POST:
			lynxMatrixXMatrix(&T, &lpo->GlobalM, lpm);	
			lynxMemCpy(&lpo->GlobalM, &T, sizeof(LYNXMATRIX));
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetObject3DTransM(LPLYNXOBJ3D lpo, LYNXREAL time)
{
	LYNXREAL					lpValues[4];	
	
	LYNX_IDENTITY_MATRIX(lpo->TransM);		 		
	if (lpo->RotateKey.lpKeys)
	{
		#ifdef __FIXED_POINT__
			lynxGetAnimKeyValues(&lpo->RotateKey, time, lpValues, 4);
		#else
			lynxGetShortAnimKeyValues(&lpo->RotateKey, time, lpValues, 4, LYNX_ROTATE_KEY_ISCALE);
		#endif
		lynxQuaternion2Matrix(&lpo->TransM, (LPLYNXQUATERNION)lpValues);
	}			
	if (lpo->ScaleKey.lpKeys)
	{
		lynxGetAnimKeyValues(&lpo->ScaleKey, time, lpValues, 3);

		lpo->TransM.s.m11 = LYNX_REAL_MUL(lpo->TransM.s.m11, lpValues[0]);
		lpo->TransM.s.m21 = LYNX_REAL_MUL(lpo->TransM.s.m21, lpValues[1]);
		lpo->TransM.s.m31 = LYNX_REAL_MUL(lpo->TransM.s.m31, lpValues[2]);

		lpo->TransM.s.m12 = LYNX_REAL_MUL(lpo->TransM.s.m12, lpValues[0]);
		lpo->TransM.s.m22 = LYNX_REAL_MUL(lpo->TransM.s.m22, lpValues[1]);
		lpo->TransM.s.m32 = LYNX_REAL_MUL(lpo->TransM.s.m32, lpValues[2]);

		lpo->TransM.s.m13 = LYNX_REAL_MUL(lpo->TransM.s.m13, lpValues[0]);
		lpo->TransM.s.m23 = LYNX_REAL_MUL(lpo->TransM.s.m23, lpValues[1]);
		lpo->TransM.s.m33 = LYNX_REAL_MUL(lpo->TransM.s.m33, lpValues[2]);
	}			
	if (lpo->TranslateKey.lpKeys)
	{
		lynxGetAnimKeyValues(&lpo->TranslateKey, time, lpValues, 3);

		lpo->TransM.s.m41 += lpValues[0];
		lpo->TransM.s.m42 += lpValues[1];
		lpo->TransM.s.m43 += lpValues[2];
	}	
	
	if (lpo->VisKey.lpKeys)
	{
		#ifdef __FIXED_POINT__
			lynxGetAnimKeyValues(&lpo->VisKey, time, lpValues, 1);
		#else
			lynxGetShortAnimKeyValues(&lpo->VisKey, time, lpValues, 1, LYNX_ROTATE_KEY_ISCALE);
		#endif
		lpo->Visibility = lpValues[0];
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxPlayObject3D(LPLYNXOBJ3D lpo, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LYNXBOOL					b;

	b = FALSE;	
	lpo->LastTime = lpo->Time;
	if (lpo->EndTime == lpo->StartTime)
	{
		lpo->Time = lpo->EndTime;
		b = TRUE;
	}
	else
	{
		lpo->Time+= skip;
		switch (mode)
		{
			case LYNX_PM_LOOP:			
				while (lpo->Time > lpo->EndTime)
				{
					b = TRUE;
					lpo->Time -= (lpo->EndTime);
					lpo->Time += (lpo->StartTime);
				}
				while (lpo->Time < lpo->StartTime)
				{
					b = TRUE;
					lpo->Time -= (lpo->StartTime);
					lpo->Time += lpo->EndTime;
				}
				break;

			case LYNX_PM_ONCE:
				if (lpo->Time > lpo->EndTime)
				{
					b = TRUE;
					lpo->Time = lpo->EndTime;
				}
				else if (lpo->Time < lpo->StartTime)
				{
					b = TRUE;
					lpo->Time = lpo->StartTime;
				}
				break;
		}
	}
	return b;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxObject3DLookAt(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpv)
{
	LYNXVECTOR3D			TN, TU, TV;
	LYNXVECTOR3D			dv,dn;
    LYNXREAL				f;
	LYNXMATRIX				R;

	TN.s.x = (lpo->lpCenter->s.x - lpv->s.x);
	TN.s.y = (lpo->lpCenter->s.y - lpv->s.y);
	TN.s.z = (lpo->lpCenter->s.z - lpv->s.z);
    lynxNormalise(&TN, 3);

    dv.s.x = TN.s.x; dv.s.y = LYNX_REAL_ADD(TN.s.y, LYNX_REAL(1)); dv.s.z = TN.s.z;
    lynxNormalise(&dv, 3);
    f = LYNX_DOT_PRODUCT(dv, TN);
    LYNX_VECTOR3D_MUL(dn, f, TN);
    LYNX_VECTOR3D_SUB(TV, dv, dn);
    lynxNormalise(&TV, 3);
    lynxCrossProduct(&TU, &TV, &TN);

	LYNX_MATRIX(lpo->LocalM,
				 TU.s.x , TU.s.y , TU.s.z , LYNX_REAL(0),
				 TV.s.x , TV.s.y , TV.s.z , LYNX_REAL(0),
                 TN.s.x , TN.s.y , TN.s.z , LYNX_REAL(0),				         
			     LYNX_REAL(0) , LYNX_REAL(0) , LYNX_REAL(0) , LYNX_REAL(1));				
	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxObj3D 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxAlignObject3DToDir(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpv)
{
	LYNXVECTOR3D		TN, TU, TV;
	LYNXVECTOR3D		dv,dn;
    float				f;
	LYNXMATRIX			R;
	

	TN.s.x = lpv->s.x;
	TN.s.y = lpv->s.y;
	TN.s.z = lpv->s.z;

    lynxNormalise(&TN, 3);

    dv.s.x = TN.s.x; dv.s.y = TN.s.y + 1.0f; dv.s.z = TN.s.z;
    lynxNormalise(&dv, 3);
    f = LYNX_DOT_PRODUCT(dv, TN);
    LYNX_VECTOR3D_MUL(dn, f, TN);
    LYNX_VECTOR3D_SUB(TV, dv, dn);
    lynxNormalise(&TV, 3);
    lynxCrossProduct(&TU, &TV, &TN);

	LYNX_MATRIX(lpo->LocalM,
                TU.s.x , TV.s.x , TN.s.x , 0.0f,
                TU.s.y , TV.s.y , TN.s.y , 0.0f,
                TU.s.z , TV.s.z , TN.s.z , 0.0f,
                0.0f , 0.0f , 0.0f , 1.0f);		

	LYNX_MATRIX(lpo->LocalM,
                TU.s.x , TU.s.y , TU.s.z , 0.0f,
                TV.s.x , TV.s.y , TV.s.z , 0.0f,
                TN.s.x , TN.s.y , TN.s.z , 0.0f,				   
                0.0f , 0.0f , 0.0f , 1.0f);		
				   

	/*
	lynxMemCpy(&lpo->LocalM, &lpo->R, sizeof(LYNXMATRIX));
	*/

	
	/*
	dv.s.x = (lpv->s.x - lpo->TCenter.s.x);
	dv.s.y = (lpv->s.y - lpo->TCenter.s.y);
	dv.s.z = (lpv->s.z - lpo->TCenter.s.z);	
    lynxNormalise(&dv, 3);

	lynxVectorXMatrix(&lpo->TDirection, &lpo->Direction, &lpo->LocalM);

	TN.s.x = lpo->TDirection.s.x;
	TN.s.y = -dv.s.y + 1.0f;
	TN.s.z = lpo->TDirection.s.z;
    //TN.s.x = -dv.s.x; TN.s.y = -dv.s.y + 1.0f; TN.s.z = -dv.s.z;
    lynxNormalise(&TN, 3);
    f = LYNX_DOT_PRODUCT(dv, TN);
    LYNX_VECTOR3D_MULTIPLY(dn, f, TN);
    LYNX_VECTOR3D_SUBTRACT(TV, dv, dn);
    lynxNormalise(&TV, 3);
    lynxCrossProduct(&TU, &TV, &TN);

	LYNX_MAKE_MATRIX(lpo->R,
                   TU.s.x , TV.s.x , TN.s.x , 0.0f,
                   TU.s.y , TV.s.y , TN.s.y , 0.0f,
                   TU.s.z , TV.s.z , TN.s.z , 0.0f,
                   0.0f , 0.0f , 0.0f , 1.0f);			

	//lynxMatrixXMatrix(&R, &lpo->R, &lpo->LocalM);	 
	lynxMemCpy(&lpo->LocalM, &lpo->R, sizeof(LYNXMATRIX));	
	*/
}