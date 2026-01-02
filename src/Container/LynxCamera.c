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

#include <LynxCamera.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInitCamera(LPLYNXCAMERA lpc)
{
	lynxInitObject3D(&lpc->Obj3D);
	lynxSetCameraFOV(lpc, LYNX_REAL(60),  LYNX_REAL(60));	
	lpc->Near = LYNX_REAL(50);
	lpc->Far = LYNX_REAL(600);
	lpc->FrustumFlag = LYNX_TEST_NEAR | LYNX_TEST_FAR | LYNX_TEST_LEFT | LYNX_TEST_RIGHT | LYNX_TEST_TOP | LYNX_TEST_BOTTOM;
	lynxCalculateCameraFrustum(lpc);
	LYNX_VECTOR3D(lpc->UpDir, LYNX_REAL(0), LYNX_REAL(1), LYNX_REAL(0));
	lpc->Obj3D.Play = (LPLYNXPLAYPROC)lynxPlayCamera;

	lpc->Time = 0.0f;
	lpc->StartTime = 0.0f;
	lpc->EndTime = 0.0f;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxReleaseCamera(LPLYNXCAMERA lpc)
{
	lynxReleaseObject3D(&lpc->Obj3D);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSetCameraFOV(LPLYNXCAMERA lpc, LYNXREAL xfov, LYNXREAL yfov)
{	
	LYNXREAL					d;

	lpc->XFOV = xfov;
	lpc->YFOV = yfov;	
	d = LYNX_REAL_SUB(LYNX_REAL(90), LYNX_REAL_MUL(lpc->XFOV, LYNX_REAL_HALF));
	lpc->Sin_XFOV[0] = lynxQuickSin(d);
	lpc->Cos_XFOV[0] = lynxQuickCos(d);
	d = -LYNX_REAL_SUB(LYNX_REAL(90), LYNX_REAL_MUL(lpc->XFOV, LYNX_REAL_HALF));	
	lpc->Sin_XFOV[1] = lynxQuickSin(d);
	lpc->Cos_XFOV[1] = lynxQuickCos(d);
	
	d = LYNX_REAL_SUB(LYNX_REAL(90), LYNX_REAL_MUL(lpc->YFOV, LYNX_REAL_HALF));
	lpc->Sin_YFOV[0] = lynxQuickSin(d);
	lpc->Cos_YFOV[0] = lynxQuickCos(d);
	d = -LYNX_REAL_SUB(LYNX_REAL(90), LYNX_REAL_MUL(lpc->YFOV, LYNX_REAL_HALF));
	lpc->Sin_YFOV[1] = lynxQuickSin(d);
	lpc->Cos_YFOV[1] = lynxQuickCos(d);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxCameraLookAt(LPLYNXCAMERA lpc, LPLYNXVECTOR3D lpv)
{
	LYNXVECTOR3D			dv,dn;	
    LYNXREAL				f;
	LYNXREAL				m41, m42, m43;

	LYNX_VECTOR3D_COPY((lpc->LookPoint), (*lpv));	
	LYNX_VECTOR3D_SUB((lpc->TN), (*lpv), (*lpc->Obj3D.lpCenter));    
    lynxNormalise(&lpc->TN, 3);

	LYNX_VECTOR3D_ADD((dv), (lpc->TN), (lpc->UpDir));        				
    lynxNormalise(&dv, 3);

    f = LYNX_DOT_PRODUCT(dv, lpc->TN);
	if (f == 0.0f)
	{
		f = 0.1f;
	}
	LYNX_VECTOR3D_MUL(dn, f, (lpc->TN));	
	LYNX_VECTOR3D_SUB((lpc->TV), dv, dn);	
    lynxNormalise(&lpc->TV, 3);	
    lynxCrossProduct(&lpc->TU, &lpc->TV, &lpc->TN);
	
	LYNX_VECTOR3D_COPY((lpc->U), (lpc->TU));
	LYNX_VECTOR3D_COPY((lpc->V), (lpc->TV));
	LYNX_VECTOR3D_COPY((lpc->N), (lpc->TN));	

	m41 = -LYNX_DOT_PRODUCT((lpc->TU), (*lpc->Obj3D.lpCenter));
	m42 = -LYNX_DOT_PRODUCT((lpc->TV), (*lpc->Obj3D.lpCenter));
	m43 = -LYNX_DOT_PRODUCT((lpc->TN), (*lpc->Obj3D.lpCenter));	

	LYNX_MATRIX(lpc->ViewMatrix,
		        lpc->TU.s.x , lpc->TV.s.x , lpc->TN.s.x , LYNX_REAL(0),
                lpc->TU.s.y , lpc->TV.s.y , lpc->TN.s.y , LYNX_REAL(0),
                lpc->TU.s.z , lpc->TV.s.z , lpc->TN.s.z , LYNX_REAL(0),
                m41		   , m42         , m43         , LYNX_REAL(1));					

	LYNX_MATRIX(lpc->Obj3D.M,
		        lpc->TU.s.x , lpc->TU.s.y , lpc->TU.s.z , LYNX_REAL(0),
                lpc->TV.s.x , lpc->TV.s.y , lpc->TV.s.z , LYNX_REAL(0),
                lpc->TN.s.x , lpc->TN.s.y , lpc->TN.s.z , LYNX_REAL(0),
                lpc->Obj3D.lpCenter->s.x, lpc->Obj3D.lpCenter->s.y, lpc->Obj3D.lpCenter->s.z, LYNX_REAL(1));								   
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxCalculateCameraFrustum(LPLYNXCAMERA lpc)
{	
	//CalculateFarPlane
	if (lpc->FrustumFlag & LYNX_TEST_FAR)
	{
		lpc->Frustum.Far.Normal.s.x = -lpc->TN.s.x;
		lpc->Frustum.Far.Normal.s.y = -lpc->TN.s.y;
		lpc->Frustum.Far.Normal.s.z = -lpc->TN.s.z;

		lpc->Frustum.FarP.s.x = lpc->Obj3D.lpCenter->s.x + LYNX_REAL_MUL(lpc->TN.s.x, lpc->Far);
		lpc->Frustum.FarP.s.y = lpc->Obj3D.lpCenter->s.y + LYNX_REAL_MUL(lpc->TN.s.y, lpc->Far);
		lpc->Frustum.FarP.s.z = lpc->Obj3D.lpCenter->s.z + LYNX_REAL_MUL(lpc->TN.s.z, lpc->Far);

		//Frustum.Far.Distance = lynxLength(&D, 3);
		lpc->Frustum.Far.Distance = LYNX_DOT_PRODUCT(lpc->Frustum.Far.Normal, lpc->Frustum.FarP);
	}

	//CalculateNearPlane
	if (lpc->FrustumFlag & LYNX_TEST_NEAR)
	{
		lpc->Frustum.Near.Normal.s.x = lpc->TN.s.x;
		lpc->Frustum.Near.Normal.s.y = lpc->TN.s.y;
		lpc->Frustum.Near.Normal.s.z = lpc->TN.s.z;

		lpc->Frustum.NearP.s.x = lpc->Obj3D.lpCenter->s.x + LYNX_REAL_MUL(lpc->TN.s.x, lpc->Near);
		lpc->Frustum.NearP.s.y = lpc->Obj3D.lpCenter->s.y + LYNX_REAL_MUL(lpc->TN.s.y, lpc->Near);
		lpc->Frustum.NearP.s.z = lpc->Obj3D.lpCenter->s.z + LYNX_REAL_MUL(lpc->TN.s.z, lpc->Near);

		//Frustum.Near.Distance = lynxLength(&D, 3);
		lpc->Frustum.Near.Distance = LYNX_DOT_PRODUCT(lpc->Frustum.Near.Normal, lpc->Frustum.NearP);
	}	
	
	//CalculateLeftPlane
	if (lpc->FrustumFlag & LYNX_TEST_LEFT)
	{		
		lpc->Frustum.Left.Normal.s.x = LYNX_REAL_MUL(lpc->TN.s.x, lpc->Cos_XFOV[0]) + LYNX_REAL_MUL(lpc->TN.s.z, lpc->Sin_XFOV[0]); 
		lpc->Frustum.Left.Normal.s.z = LYNX_REAL_MUL(-lpc->TN.s.x, lpc->Sin_XFOV[0]) + LYNX_REAL_MUL(lpc->TN.s.z, lpc->Cos_XFOV[0]);  
		lpc->Frustum.Left.Normal.s.y = lpc->TN.s.y;
		lpc->Frustum.Left.Distance = LYNX_DOT_PRODUCT(lpc->Frustum.Left.Normal, (*lpc->Obj3D.lpCenter));
	}

	//CalculateRightPlane
	if (lpc->FrustumFlag & LYNX_TEST_RIGHT)
	{	
		lpc->Frustum.Right.Normal.s.x = LYNX_REAL_MUL(lpc->TN.s.x, lpc->Cos_XFOV[1]) + LYNX_REAL_MUL(lpc->TN.s.z, lpc->Sin_XFOV[1]); 
		lpc->Frustum.Right.Normal.s.z = LYNX_REAL_MUL(-lpc->TN.s.x, lpc->Sin_XFOV[1]) + LYNX_REAL_MUL(lpc->TN.s.z, lpc->Cos_XFOV[1]);  
		lpc->Frustum.Right.Normal.s.y = lpc->TN.s.y;
		lpc->Frustum.Right.Distance = LYNX_DOT_PRODUCT(lpc->Frustum.Right.Normal, (*lpc->Obj3D.lpCenter));
	}

	//CalculateTopPlane
	if (lpc->FrustumFlag & LYNX_TEST_TOP)
	{	
		lpc->Frustum.Top.Normal.s.y = LYNX_REAL_MUL(lpc->TN.s.y, lpc->Cos_YFOV[0]) - LYNX_REAL_MUL(lpc->TN.s.z, lpc->Sin_YFOV[0]); 
		lpc->Frustum.Top.Normal.s.z = LYNX_REAL_MUL(lpc->TN.s.y, lpc->Sin_YFOV[0]) + LYNX_REAL_MUL(lpc->TN.s.z, lpc->Cos_YFOV[0]);  
		lpc->Frustum.Top.Normal.s.x = lpc->TN.s.x;
		lpc->Frustum.Top.Distance = LYNX_DOT_PRODUCT(lpc->Frustum.Top.Normal, (*lpc->Obj3D.lpCenter));
	}

	//CalculateBottomPlane
	if (lpc->FrustumFlag & LYNX_TEST_BOTTOM)
	{		
		lpc->Frustum.Bottom.Normal.s.y = LYNX_REAL_MUL(lpc->TN.s.y, lpc->Cos_YFOV[1]) - LYNX_REAL_MUL(lpc->TN.s.z, lpc->Sin_YFOV[1]); 
		lpc->Frustum.Bottom.Normal.s.z = LYNX_REAL_MUL(lpc->TN.s.y, lpc->Sin_YFOV[1]) + LYNX_REAL_MUL(lpc->TN.s.z, lpc->Cos_YFOV[1]);  
		lpc->Frustum.Bottom.Normal.s.x = lpc->TN.s.x;
		lpc->Frustum.Bottom.Distance = LYNX_DOT_PRODUCT(lpc->Frustum.Bottom.Normal, (*lpc->Obj3D.lpCenter));
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSetCameraPosition(LPLYNXCAMERA lpc, LPLYNXVECTOR3D lpv)
{		
	//lynxSetObject3DPosition(&lpc->Obj3D, lpv);	
	LYNX_VECTOR3D_COPY((*lpc->Obj3D.lpCenter), (*lpv));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxUpdateCameraViewMatrix(LPLYNXCAMERA lpc)
{	
	LYNXREAL			m41, m42, m43;

	lynxUpdateObject3D(&lpc->Obj3D);

	LYNX_VECTOR3D(lpc->TU, lpc->Obj3D.lpRightDir->s.x, lpc->Obj3D.lpRightDir->s.y, lpc->Obj3D.lpRightDir->s.z);
	LYNX_VECTOR3D(lpc->TV, lpc->Obj3D.lpUpDir->s.x, lpc->Obj3D.lpUpDir->s.y, lpc->Obj3D.lpUpDir->s.z);
	LYNX_VECTOR3D(lpc->TN, lpc->Obj3D.lpDirection->s.x, lpc->Obj3D.lpDirection->s.y, lpc->Obj3D.lpDirection->s.z);

	m41 = -LYNX_DOT_PRODUCT((lpc->TU), (*lpc->Obj3D.lpCenter));
	m42 = -LYNX_DOT_PRODUCT((lpc->TV), (*lpc->Obj3D.lpCenter));
	m43 = -LYNX_DOT_PRODUCT((lpc->TN), (*lpc->Obj3D.lpCenter));		

	LYNX_MATRIX(lpc->ViewMatrix,
		  	    lpc->Obj3D.lpRightDir->s.x, lpc->Obj3D.lpUpDir->s.x, lpc->Obj3D.lpDirection->s.x, LYNX_REAL(0),
                lpc->Obj3D.lpRightDir->s.y, lpc->Obj3D.lpUpDir->s.y, lpc->Obj3D.lpDirection->s.y, LYNX_REAL(0),
                lpc->Obj3D.lpRightDir->s.z, lpc->Obj3D.lpUpDir->s.z, lpc->Obj3D.lpDirection->s.z, LYNX_REAL(0),
                m41 , m42, m43, LYNX_REAL(1));			
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSetCameraMatrix(LPLYNXCAMERA lpc, LPLYNXMATRIX lpm)
{
	LYNXVECTOR3D			dv,dn;
	//LYNXMATRIX			T;	    
	LYNXREAL				m41, m42, m43;	

	lpc->TU.s.x = lpm->s.m11;
	lpc->TU.s.y = lpm->s.m21;
	lpc->TU.s.z = lpm->s.m31;

	lpc->TV.s.x = lpm->s.m12;
	lpc->TV.s.y = lpm->s.m22;
	lpc->TV.s.z = lpm->s.m32;

	lpc->TN.s.x = lpm->s.m13;
	lpc->TN.s.y = lpm->s.m23;
	lpc->TN.s.z = lpm->s.m33;

	lpc->Obj3D.lpCenter->s.x = lpm->s.m41;
	lpc->Obj3D.lpCenter->s.y = lpm->s.m42;
	lpc->Obj3D.lpCenter->s.z = lpm->s.m43;
	
	m41 = -LYNX_DOT_PRODUCT((lpc->TU), (*lpc->Obj3D.lpCenter));
	m42 = -LYNX_DOT_PRODUCT((lpc->TV), (*lpc->Obj3D.lpCenter));
	m43 = -LYNX_DOT_PRODUCT((lpc->TN), (*lpc->Obj3D.lpCenter));		

	LYNX_MATRIX(lpc->ViewMatrix,
		        lpc->TU.s.x , lpc->TV.s.x , lpc->TN.s.x , LYNX_REAL(0),
                lpc->TU.s.y , lpc->TV.s.y , lpc->TN.s.y , LYNX_REAL(0),
                lpc->TU.s.z , lpc->TV.s.z , lpc->TN.s.z , LYNX_REAL(0),
                m41		 , m42       , m43       , LYNX_REAL(1));		

	LYNX_MATRIX(lpc->Obj3D.M,
		        lpc->TU.s.x , lpc->TU.s.y , lpc->TU.s.z , LYNX_REAL(0),
                lpc->TV.s.x , lpc->TV.s.y , lpc->TV.s.z , LYNX_REAL(0),
                lpc->TN.s.x , lpc->TN.s.y , lpc->TN.s.z , LYNX_REAL(0),
                lpc->Obj3D.lpCenter->s.x, lpc->Obj3D.lpCenter->s.y, lpc->Obj3D.lpCenter->s.z, LYNX_REAL(1));			
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxPlayCamera(LPLYNXCAMERA lpc, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LYNXBOOL				b;	
	LYNXREAL				m41, m42, m43;

    b = FALSE;	
	if (lpc->EndTime == lpc->StartTime)
	{
		lpc->Time = lpc->EndTime;
		b = TRUE;
	}
	else
	{
		lpc->Time = LYNX_REAL_ADD(lpc->Time, skip);

		switch (mode)
		{
			case LYNX_PM_LOOP:
				while (lpc->Time > lpc->EndTime)
				{
					b = TRUE;
					lpc->Time -= (lpc->EndTime);
					lpc->Time += (lpc->StartTime);
				}
				while (lpc->Time < lpc->StartTime)
				{
					b = TRUE;
					lpc->Time -= (lpc->StartTime);
					lpc->Time += lpc->EndTime;
				}
				break;

			case LYNX_PM_ONCE:
				if (lpc->Time > lpc->EndTime)
				{
					b = TRUE;
					lpc->Time = lpc->EndTime;
				}
				else if (lpc->Time < lpc->StartTime)
				{
					b = TRUE;
					lpc->Time = lpc->StartTime;
				}
				break;
		}
	}

	LYNX_VECTOR3D(lpc->TU, lpc->Obj3D.lpRightDir->s.x, lpc->Obj3D.lpRightDir->s.y, lpc->Obj3D.lpRightDir->s.z);
	LYNX_VECTOR3D(lpc->TV, lpc->Obj3D.lpUpDir->s.x, lpc->Obj3D.lpUpDir->s.y, lpc->Obj3D.lpUpDir->s.z);
	LYNX_VECTOR3D(lpc->TN, lpc->Obj3D.lpDirection->s.x, lpc->Obj3D.lpDirection->s.y, lpc->Obj3D.lpDirection->s.z);

	m41 = -LYNX_DOT_PRODUCT((lpc->TU), (*lpc->Obj3D.lpCenter));
	m42 = -LYNX_DOT_PRODUCT((lpc->TV), (*lpc->Obj3D.lpCenter));
	m43 = -LYNX_DOT_PRODUCT((lpc->TN), (*lpc->Obj3D.lpCenter));		

	LYNX_MATRIX(lpc->ViewMatrix,
		  	    lpc->Obj3D.lpRightDir->s.x, lpc->Obj3D.lpUpDir->s.x, lpc->Obj3D.lpDirection->s.x, LYNX_REAL(0),
                lpc->Obj3D.lpRightDir->s.y, lpc->Obj3D.lpUpDir->s.y, lpc->Obj3D.lpDirection->s.y, LYNX_REAL(0),
                lpc->Obj3D.lpRightDir->s.z, lpc->Obj3D.lpUpDir->s.z, lpc->Obj3D.lpDirection->s.z, LYNX_REAL(0),
                m41 , m42, m43, LYNX_REAL(1));

	return b;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxCamera 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSetCameraAnimationKeys(LPLYNXCAMERA lpcamera, LPLYNXANIMATIONKEY lptran, LPLYNXANIMATIONKEY lpscl, LPLYNXSHORTANIMATIONKEY lprot)
{	
}