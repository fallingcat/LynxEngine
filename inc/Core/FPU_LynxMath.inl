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
void ilynxFPU_PointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm)
{
    lpb->x = lpa->x*lpm->m11 + lpa->y*lpm->m21 + lpa->z*lpm->m31 + lpm->m41;
    lpb->y = lpa->x*lpm->m12 + lpa->y*lpm->m22 + lpa->z*lpm->m32 + lpm->m42;
    lpb->z = lpa->x*lpm->m13 + lpa->y*lpm->m23 + lpa->z*lpm->m33 + lpm->m43;

    return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFPU_SequencePointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, int num)
{
    int						i;
	LPLYNXVECTOR3D			B = lpb;
	LPCLYNXVECTOR3D	A = lpa;

	for(i=num; i>0; i--)
	{
		B->x = A->x*lpm->m11 + A->y*lpm->m21 + A->z*lpm->m31 + lpm->m41;
		B->y = A->x*lpm->m12 + A->y*lpm->m22 + A->z*lpm->m32 + lpm->m42;
		B->z = A->x*lpm->m13 + A->y*lpm->m23 + A->z*lpm->m33 + lpm->m43;
		A++;
		B++;	
	}

	return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFPU_Vector4XMatrix(LPLYNXVECTOR4D lpb, LPCLYNXVECTOR4D lpa, LPCLYNXMATRIX lpm)
{
	lpb->x = lpa->x*lpm->m11 + lpa->y*lpm->m21 + lpa->z*lpm->m31 + lpa->w*lpm->m41;
    lpb->y = lpa->x*lpm->m12 + lpa->y*lpm->m22 + lpa->z*lpm->m32 + lpa->w*lpm->m42;
    lpb->z = lpa->x*lpm->m13 + lpa->y*lpm->m23 + lpa->z*lpm->m33 + lpa->w*lpm->m43;
	lpb->w = lpa->x*lpm->m14 + lpa->y*lpm->m24 + lpa->z*lpm->m34 + lpa->w*lpm->m44;
	
    return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFPU_VectorXMatrix3(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX3 lpm)
{
	lpb->x = lpa->x*lpm->m11 + lpa->y*lpm->m21 + lpa->z*lpm->m31;
    lpb->y = lpa->x*lpm->m12 + lpa->y*lpm->m22 + lpa->z*lpm->m32;
    lpb->z = lpa->x*lpm->m13 + lpa->y*lpm->m23 + lpa->z*lpm->m33;
	
    return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFPU_VectorXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm)
{
	lpb->x = lpa->x*lpm->m11 + lpa->y*lpm->m21 + lpa->z*lpm->m31;
    lpb->y = lpa->x*lpm->m12 + lpa->y*lpm->m22 + lpa->z*lpm->m32;
    lpb->z = lpa->x*lpm->m13 + lpa->y*lpm->m23 + lpa->z*lpm->m33;
	
    return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxFPU_MatrixXMatrix(LPLYNXMATRIX lpd, LPCLYNXMATRIX lpm1, LPCLYNXMATRIX lpm2)
{	
	lpd->m11 = lpm1->m11*lpm2->m11 + lpm1->m12*lpm2->m21 + lpm1->m13*lpm2->m31 + lpm1->m14*lpm2->m41; 
	lpd->m12 = lpm1->m11*lpm2->m12 + lpm1->m12*lpm2->m22 + lpm1->m13*lpm2->m32 + lpm1->m14*lpm2->m42; 
	lpd->m13 = lpm1->m11*lpm2->m13 + lpm1->m12*lpm2->m23 + lpm1->m13*lpm2->m33 + lpm1->m14*lpm2->m43; 
	lpd->m14 = lpm1->m11*lpm2->m14 + lpm1->m12*lpm2->m24 + lpm1->m14*lpm2->m34 + lpm1->m14*lpm2->m44; 

	lpd->m21 = lpm1->m21*lpm2->m11 + lpm1->m22*lpm2->m21 + lpm1->m23*lpm2->m31 + lpm1->m24*lpm2->m41; 
	lpd->m22 = lpm1->m21*lpm2->m12 + lpm1->m22*lpm2->m22 + lpm1->m23*lpm2->m32 + lpm1->m24*lpm2->m42; 
	lpd->m23 = lpm1->m21*lpm2->m13 + lpm1->m22*lpm2->m23 + lpm1->m23*lpm2->m33 + lpm1->m24*lpm2->m43; 
	lpd->m24 = lpm1->m21*lpm2->m14 + lpm1->m22*lpm2->m24 + lpm1->m23*lpm2->m34 + lpm1->m24*lpm2->m44; 

	lpd->m31 = lpm1->m31*lpm2->m11 + lpm1->m32*lpm2->m21 + lpm1->m33*lpm2->m31 + lpm1->m34*lpm2->m41; 
	lpd->m32 = lpm1->m31*lpm2->m12 + lpm1->m32*lpm2->m22 + lpm1->m33*lpm2->m32 + lpm1->m34*lpm2->m42; 
	lpd->m33 = lpm1->m31*lpm2->m13 + lpm1->m32*lpm2->m23 + lpm1->m33*lpm2->m33 + lpm1->m34*lpm2->m43; 
	lpd->m34 = lpm1->m31*lpm2->m14 + lpm1->m32*lpm2->m24 + lpm1->m33*lpm2->m34 + lpm1->m34*lpm2->m44; 

	lpd->m41 = lpm1->m41*lpm2->m11 + lpm1->m42*lpm2->m21 + lpm1->m43*lpm2->m31 + lpm1->m44*lpm2->m41; 
	lpd->m42 = lpm1->m41*lpm2->m12 + lpm1->m42*lpm2->m22 + lpm1->m43*lpm2->m32 + lpm1->m44*lpm2->m42; 
	lpd->m43 = lpm1->m41*lpm2->m13 + lpm1->m42*lpm2->m23 + lpm1->m43*lpm2->m33 + lpm1->m44*lpm2->m43; 
	lpd->m44 = lpm1->m41*lpm2->m14 + lpm1->m42*lpm2->m24 + lpm1->m43*lpm2->m34 + lpm1->m44*lpm2->m44; 	

	return;   
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//		lpDst = lpSrc1 * lpSrc2
//		lpDst can be equal to lpSrc1 or lpSrc2
//-------------------------------------------------------------------------------------------------------
void ilynxFPU_Matrix3XMatrix3(LPLYNXMATRIX3 lpd, LPCLYNXMATRIX3 lpm1, LPCLYNXMATRIX3 lpm2)
{	
	lpd->m11 = lpm1->m11*lpm2->m11 + lpm1->m12*lpm2->m21 + lpm1->m13*lpm2->m31; 
	lpd->m12 = lpm1->m11*lpm2->m12 + lpm1->m12*lpm2->m22 + lpm1->m13*lpm2->m32; 
	lpd->m13 = lpm1->m11*lpm2->m13 + lpm1->m12*lpm2->m23 + lpm1->m13*lpm2->m33; 			

	lpd->m21 = lpm1->m21*lpm2->m11 + lpm1->m22*lpm2->m21 + lpm1->m23*lpm2->m31; 
	lpd->m22 = lpm1->m21*lpm2->m12 + lpm1->m22*lpm2->m22 + lpm1->m23*lpm2->m32; 
	lpd->m23 = lpm1->m21*lpm2->m13 + lpm1->m22*lpm2->m23 + lpm1->m23*lpm2->m33; 			

	lpd->m31 = lpm1->m31*lpm2->m11 + lpm1->m32*lpm2->m21 + lpm1->m33*lpm2->m31; 
	lpd->m32 = lpm1->m31*lpm2->m12 + lpm1->m32*lpm2->m22 + lpm1->m33*lpm2->m32; 
	lpd->m33 = lpm1->m31*lpm2->m13 + lpm1->m32*lpm2->m23 + lpm1->m33*lpm2->m33; 			

	return;   
}