//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2003/06/22
//  Last Update : 
//
//###########################################################################

//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFP_PointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm)
{
    lpb->x = LYNX_REAL_MUL(lpa->x, lpm->m11) + LYNX_REAL_MUL(lpa->y, lpm->m21) + LYNX_REAL_MUL(lpa->z, lpm->m31) + lpm->m41;
	lpb->y = LYNX_REAL_MUL(lpa->x, lpm->m12) + LYNX_REAL_MUL(lpa->y, lpm->m22) + LYNX_REAL_MUL(lpa->z, lpm->m32) + lpm->m42;
	lpb->z = LYNX_REAL_MUL(lpa->x, lpm->m13) + LYNX_REAL_MUL(lpa->y, lpm->m23) + LYNX_REAL_MUL(lpa->z, lpm->m33) + lpm->m43;

    return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFP_SequencePointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, int num)
{
    int						i;
	LPLYNXVECTOR3D			B = lpb;
	LPCLYNXVECTOR3D	A = lpa;

	for(i=num; i>0; i--)
	{
		B->x = LYNX_REAL_MUL(A->x, lpm->m11) + LYNX_REAL_MUL(A->y, lpm->m21) + LYNX_REAL_MUL(A->z, lpm->m31) + lpm->m41;
		B->y = LYNX_REAL_MUL(A->x, lpm->m12) + LYNX_REAL_MUL(A->y, lpm->m22) + LYNX_REAL_MUL(A->z, lpm->m32) + lpm->m42;
		B->z = LYNX_REAL_MUL(A->x, lpm->m13) + LYNX_REAL_MUL(A->y, lpm->m23) + LYNX_REAL_MUL(A->z, lpm->m33) + lpm->m43;		
		A++;
		B++;	
	}

	return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFP_VectorXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm)
{
	lpb->x = LYNX_REAL_MUL(lpa->x, lpm->m11) + LYNX_REAL_MUL(lpa->y, lpm->m21) + LYNX_REAL_MUL(lpa->z, lpm->m31);
	lpb->y = LYNX_REAL_MUL(lpa->x, lpm->m12) + LYNX_REAL_MUL(lpa->y, lpm->m22) + LYNX_REAL_MUL(lpa->z, lpm->m32);
	lpb->z = LYNX_REAL_MUL(lpa->x, lpm->m13) + LYNX_REAL_MUL(lpa->y, lpm->m23) + LYNX_REAL_MUL(lpa->z, lpm->m33);
	
    return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFP_MatrixXMatrix(LPLYNXMATRIX lpd, LPCLYNXMATRIX lpm1, LPCLYNXMATRIX lpm2)
{	
	lpd->m11 = LYNX_REAL_MUL(lpm1->m11, lpm2->m11) + LYNX_REAL_MUL(lpm1->m12, lpm2->m21) + LYNX_REAL_MUL(lpm1->m13, lpm2->m31) + LYNX_REAL_MUL(lpm1->m14, lpm2->m41); 
	lpd->m12 = LYNX_REAL_MUL(lpm1->m11, lpm2->m12) + LYNX_REAL_MUL(lpm1->m12, lpm2->m22) + LYNX_REAL_MUL(lpm1->m13, lpm2->m32) + LYNX_REAL_MUL(lpm1->m14, lpm2->m42); 
	lpd->m13 = LYNX_REAL_MUL(lpm1->m11, lpm2->m13) + LYNX_REAL_MUL(lpm1->m12, lpm2->m23) + LYNX_REAL_MUL(lpm1->m13, lpm2->m33) + LYNX_REAL_MUL(lpm1->m14, lpm2->m43); 
	lpd->m14 = LYNX_REAL_MUL(lpm1->m11, lpm2->m14) + LYNX_REAL_MUL(lpm1->m12, lpm2->m24) + LYNX_REAL_MUL(lpm1->m14, lpm2->m34) + LYNX_REAL_MUL(lpm1->m14, lpm2->m44); 

	lpd->m21 = LYNX_REAL_MUL(lpm1->m21, lpm2->m11) + LYNX_REAL_MUL(lpm1->m22, lpm2->m21) + LYNX_REAL_MUL(lpm1->m23, lpm2->m31) + LYNX_REAL_MUL(lpm1->m24, lpm2->m41); 
	lpd->m22 = LYNX_REAL_MUL(lpm1->m21, lpm2->m12) + LYNX_REAL_MUL(lpm1->m22, lpm2->m22) + LYNX_REAL_MUL(lpm1->m23, lpm2->m32) + LYNX_REAL_MUL(lpm1->m24, lpm2->m42); 
	lpd->m23 = LYNX_REAL_MUL(lpm1->m21, lpm2->m13) + LYNX_REAL_MUL(lpm1->m22, lpm2->m23) + LYNX_REAL_MUL(lpm1->m23, lpm2->m33) + LYNX_REAL_MUL(lpm1->m24, lpm2->m43); 
	lpd->m24 = LYNX_REAL_MUL(lpm1->m21, lpm2->m14) + LYNX_REAL_MUL(lpm1->m22, lpm2->m24) + LYNX_REAL_MUL(lpm1->m23, lpm2->m34) + LYNX_REAL_MUL(lpm1->m24, lpm2->m44); 

	lpd->m31 = LYNX_REAL_MUL(lpm1->m31, lpm2->m11) + LYNX_REAL_MUL(lpm1->m32, lpm2->m21) + LYNX_REAL_MUL(lpm1->m33, lpm2->m31) + LYNX_REAL_MUL(lpm1->m34, lpm2->m41); 
	lpd->m32 = LYNX_REAL_MUL(lpm1->m31, lpm2->m12) + LYNX_REAL_MUL(lpm1->m32, lpm2->m22) + LYNX_REAL_MUL(lpm1->m33, lpm2->m32) + LYNX_REAL_MUL(lpm1->m34, lpm2->m42); 
	lpd->m33 = LYNX_REAL_MUL(lpm1->m31, lpm2->m13) + LYNX_REAL_MUL(lpm1->m32, lpm2->m23) + LYNX_REAL_MUL(lpm1->m33, lpm2->m33) + LYNX_REAL_MUL(lpm1->m34, lpm2->m43); 
	lpd->m34 = LYNX_REAL_MUL(lpm1->m31, lpm2->m14) + LYNX_REAL_MUL(lpm1->m32, lpm2->m24) + LYNX_REAL_MUL(lpm1->m33, lpm2->m34) + LYNX_REAL_MUL(lpm1->m34, lpm2->m44); 

	lpd->m41 = LYNX_REAL_MUL(lpm1->m41, lpm2->m11) + LYNX_REAL_MUL(lpm1->m42, lpm2->m21) + LYNX_REAL_MUL(lpm1->m43, lpm2->m31) + LYNX_REAL_MUL(lpm1->m44, lpm2->m41); 
	lpd->m42 = LYNX_REAL_MUL(lpm1->m41, lpm2->m12) + LYNX_REAL_MUL(lpm1->m42, lpm2->m22) + LYNX_REAL_MUL(lpm1->m43, lpm2->m32) + LYNX_REAL_MUL(lpm1->m44, lpm2->m42); 
	lpd->m43 = LYNX_REAL_MUL(lpm1->m41, lpm2->m13) + LYNX_REAL_MUL(lpm1->m42, lpm2->m23) + LYNX_REAL_MUL(lpm1->m43, lpm2->m33) + LYNX_REAL_MUL(lpm1->m44, lpm2->m43); 
	lpd->m44 = LYNX_REAL_MUL(lpm1->m41, lpm2->m14) + LYNX_REAL_MUL(lpm1->m42, lpm2->m24) + LYNX_REAL_MUL(lpm1->m43, lpm2->m34) + LYNX_REAL_MUL(lpm1->m44, lpm2->m44); 	

	return;   
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//		lpDst = lpSrc1 * lpSrc2
//		lpDst can be equal to lpSrc1 or lpSrc2
//-------------------------------------------------------------------------------------------------------
void ilynxFP_Matrix3XMatrix3(LPLYNXMATRIX lpd, LPCLYNXMATRIX lpm1, LPCLYNXMATRIX lpm2)
{	
	lpd->m11 = LYNX_REAL_MUL(lpm1->m11, lpm2->m11) + LYNX_REAL_MUL(lpm1->m12, lpm2->m21) + LYNX_REAL_MUL(lpm1->m13, lpm2->m31);
	lpd->m12 = LYNX_REAL_MUL(lpm1->m11, lpm2->m12) + LYNX_REAL_MUL(lpm1->m12, lpm2->m22) + LYNX_REAL_MUL(lpm1->m13, lpm2->m32);
	lpd->m13 = LYNX_REAL_MUL(lpm1->m11, lpm2->m13) + LYNX_REAL_MUL(lpm1->m12, lpm2->m23) + LYNX_REAL_MUL(lpm1->m13, lpm2->m33);

	lpd->m21 = LYNX_REAL_MUL(lpm1->m21, lpm2->m11) + LYNX_REAL_MUL(lpm1->m22, lpm2->m21) + LYNX_REAL_MUL(lpm1->m23, lpm2->m31);
	lpd->m22 = LYNX_REAL_MUL(lpm1->m21, lpm2->m12) + LYNX_REAL_MUL(lpm1->m22, lpm2->m22) + LYNX_REAL_MUL(lpm1->m23, lpm2->m32);
	lpd->m23 = LYNX_REAL_MUL(lpm1->m21, lpm2->m13) + LYNX_REAL_MUL(lpm1->m22, lpm2->m23) + LYNX_REAL_MUL(lpm1->m23, lpm2->m33);

	lpd->m31 = LYNX_REAL_MUL(lpm1->m31, lpm2->m11) + LYNX_REAL_MUL(lpm1->m32, lpm2->m21) + LYNX_REAL_MUL(lpm1->m33, lpm2->m31);
	lpd->m32 = LYNX_REAL_MUL(lpm1->m31, lpm2->m12) + LYNX_REAL_MUL(lpm1->m32, lpm2->m22) + LYNX_REAL_MUL(lpm1->m33, lpm2->m32);
	lpd->m33 = LYNX_REAL_MUL(lpm1->m31, lpm2->m13) + LYNX_REAL_MUL(lpm1->m32, lpm2->m23) + LYNX_REAL_MUL(lpm1->m33, lpm2->m33);

	return;   
}