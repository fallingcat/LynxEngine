//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <Container/LynxContainer_working.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer::CContainer()		
	{
		m_lpEngine = NULL;
		m_lpRenObj = NULL;
		m_bGlow = LYNX_FALSE;
		m_BloomingIntensity = 0.0f;
		m_lpCurrentMaterial = NULL;
		m_SourceType = LYNX_ST_ORIGIN;
		m_Visibility = LYNX_REAL(1.0f);						

		strcpy(m_Name, "");
		m_lpCenter = (LPLYNXVECTOR3D)&m_M.s.m41;
		m_lpRightDir = (LPLYNXVECTOR3D)&m_M.s.m11;
		m_lpUpDir = (LPLYNXVECTOR3D)&m_M.s.m21;
		m_lpDirection = (LPLYNXVECTOR3D)&m_M.s.m31;

		LYNX_IDENTITY_MATRIX(m_M);
		LYNX_IDENTITY_MATRIX(m_LocalM);
		LYNX_IDENTITY_MATRIX(m_GlobalM);
		LYNX_IDENTITY_MATRIX(m_PivotM);							

		m_SubContainerArray.clear();
		m_lpChildrenArray.clear();
		m_MaterialArray.clear();

		//IMPLEMENT_PARAM_BOOL(m_bParam);
		//IMPLEMENT_PARAM_FLOAT(m_Speed);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer::CContainer(CEngine *lpengine)		
	{
		m_lpEngine = lpengine;
		m_lpRenObj = NULL;
		m_bGlow = LYNX_FALSE;
		m_BloomingIntensity = 0.0f;
		m_lpCurrentMaterial = NULL;
		m_SourceType = LYNX_ST_ORIGIN;
		m_Visibility = LYNX_REAL(1.0f);						

		strcpy(m_Name, "");
		m_lpCenter = (LPLYNXVECTOR3D)&m_M.s.m41;
		m_lpRightDir = (LPLYNXVECTOR3D)&m_M.s.m11;
		m_lpUpDir = (LPLYNXVECTOR3D)&m_M.s.m21;
		m_lpDirection = (LPLYNXVECTOR3D)&m_M.s.m31;

		LYNX_IDENTITY_MATRIX(m_M);
		LYNX_IDENTITY_MATRIX(m_LocalM);
		LYNX_IDENTITY_MATRIX(m_GlobalM);
		LYNX_IDENTITY_MATRIX(m_PivotM);							

		m_SubContainerArray.clear();
		m_lpChildrenArray.clear();
		m_MaterialArray.clear();

		//IMPLEMENT_PARAM_BOOL(m_bParam);
		//IMPLEMENT_PARAM_FLOAT(m_Speed);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	CContainer::~CContainer(void)
	{
		m_SubContainerArray.clear();
		m_lpChildrenArray.clear();
		m_MaterialArray.clear();
	}									
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void CContainer::vDuplicate(LPLYNXOBJ3D lpdo, LPCLYNXOBJ3D lpso)
	//{
	//	int							i;

	//	lynxMemCpy(lpdo, lpso, sizeof(LYNXOBJ3D));	
	//	lpdo->lpCenter = (LPLYNXVECTOR3D)&lpdo->M.s.m41;
	//	lpdo->lpRightDir = (LPLYNXVECTOR3D)&lpdo->M.s.m11;
	//	lpdo->lpUpDir = (LPLYNXVECTOR3D)&lpdo->M.s.m21;
	//	lpdo->lpDirection = (LPLYNXVECTOR3D)&lpdo->M.s.m31;	
	//	
	//	lpdo->lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpdo->NumChildren*sizeof(LYNXLINKDATA));	
	//	for (i=0; i<lpdo->NumChildren; i++)
	//	{
	//		lpdo->lpChildrenData[i].ChildIndex = lpso->lpChildrenData[i].ChildIndex;
	//	}

	//	lynxDuplicateAnimationKey(&lpdo->TranslateKey, &lpso->TranslateKey);
	//	lynxDuplicateAnimationKey(&lpdo->ScaleKey, &lpso->ScaleKey);
	//	#ifdef __FIXED_POINT__
	//		lynxDuplicateAnimationKey(&lpdo->RotateKey, &lpso->RotateKey);
	//		lynxDuplicateAnimationKey(&lpdo->VisKey, &lpso->VisKey);		
	//	#else
	//		lynxDuplicateShortAnimationKey(&lpdo->RotateKey, &lpso->RotateKey);
	//		lynxDuplicateShortAnimationKey(&lpdo->VisKey, &lpso->VisKey);		
	//	#endif
	//}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void CContainer::vInstance(LPLYNXOBJ3D lpdo, LPCLYNXOBJ3D lpso)
	//{
	//	int						i;

	//	lynxMemCpy(lpdo, lpso, sizeof(LYNXOBJ3D));	
	//	lpdo->lpCenter = (LPLYNXVECTOR3D)&lpdo->M.s.m41;
	//	lpdo->lpRightDir = (LPLYNXVECTOR3D)&lpdo->M.s.m11;
	//	lpdo->lpUpDir = (LPLYNXVECTOR3D)&lpdo->M.s.m21;
	//	lpdo->lpDirection = (LPLYNXVECTOR3D)&lpdo->M.s.m31;

	//	lpdo->lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpdo->NumChildren*sizeof(LYNXLINKDATA));	
	//	for (i=0; i<lpdo->NumChildren; i++)
	//	{
	//		lpdo->lpChildrenData[i].ChildIndex = lpso->lpChildrenData[i].ChildIndex;
	//	}

	//	lynxInstanceAnimationKey(&lpdo->TranslateKey, &lpso->TranslateKey);
	//	lynxInstanceAnimationKey(&lpdo->ScaleKey, &lpso->ScaleKey);
	//	#ifdef __FIXED_POINT__
	//		lynxInstanceAnimationKey(&lpdo->RotateKey, &lpso->RotateKey);
	//		lynxInstanceAnimationKey(&lpdo->VisKey, &lpso->VisKey);		
	//	#else
	//		lynxInstanceShortAnimationKey(&lpdo->RotateKey, &lpso->RotateKey);
	//		lynxInstanceShortAnimationKey(&lpdo->VisKey, &lpso->VisKey);		
	//	#endif
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::ResetOrientation()
	{
		LYNX_RESET_MATRIX_ORIENTATION(m_M);
		LYNX_RESET_MATRIX_ORIENTATION(m_LocalM);
		LYNX_RESET_MATRIX_ORIENTATION(m_GlobalM);				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::ResetTransform()
	{
		LYNX_IDENTITY_MATRIX(m_M);
		LYNX_IDENTITY_MATRIX(m_LocalM);
		LYNX_IDENTITY_MATRIX(m_GlobalM);				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Scale(LPLYNXVECTOR3D lpv, LYNXMATHORDER order)
	{
		LYNXMATRIX				T, R;

		LYNX_IDENTITY_MATRIX(T);
		T.s.m11 = lpv->s.x;
		T.s.m22 = lpv->s.y;
		T.s.m33 = lpv->s.z;

		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(LYNXMATRIX));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_GlobalM, &T);	
				lynxMemCpy(&m_GlobalM, &R, sizeof(LYNXMATRIX));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetScale(LPLYNXVECTOR3D lpv)
	{
		m_LocalM.s.m11 = lpv->s.x;
		m_LocalM.s.m22 = lpv->s.z;
		m_LocalM.s.m33 = lpv->s.y;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Translate(LPLYNXVECTOR3D lpv, LYNXMATHORDER order)
	{			
		LYNXMATRIX				T, R;

		LYNX_IDENTITY_MATRIX(T);
		T.s.m41 = lpv->s.x;
		T.s.m42 = lpv->s.y;
		T.s.m43 = lpv->s.z;

		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(LYNXMATRIX));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_GlobalM, &T);	
				lynxMemCpy(&m_GlobalM, &R, sizeof(LYNXMATRIX));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Rotate(LYNXREAL f, LYNXAXISTYPE axis, LYNXMATHORDER order)
	{	
		LYNXMATRIX				T, R;

		lynxMakeRotateMatrix(&T, f, axis);	
		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(LYNXMATRIX));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_GlobalM, &T);	
				lynxMemCpy(&m_GlobalM, &R, sizeof(LYNXMATRIX));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::RotateWithAxis(LPLYNXVECTOR3D lpaxis, LYNXREAL ang, LYNXMATHORDER order)
	{	
		/*LYNXMATRIX				 R, T;

		lynxMakeRotateMatrixRespectVector(&R, lpaxis, ang);	
		lynxMatrixXMatrix(&T, &m_GlobalM, &R);	
		lynxMemCpy(&m_GlobalM, &T, sizeof(LYNXMATRIX));*/
		LYNXMATRIX				T, R;

		lynxMakeRotateMatrixRespectVector(&T, lpaxis, ang);	
		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&R, &T, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &R, sizeof(LYNXMATRIX));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&R, &m_GlobalM, &T);	
				lynxMemCpy(&m_GlobalM, &R, sizeof(LYNXMATRIX));
				break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::UpdateTransform()
	{
		LYNXMATRIX				TM1;
		
		lynxMatrixXMatrix(&m_M, &m_LocalM, &m_GlobalM);
		//lynxMemCpy(&m_TransM, &m_M, sizeof(LYNXMATRIX));	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetPosition(LPLYNXVECTOR3D lpv)
	{	
		LYNXVECTOR3D					V;

		V.s.x = lpv->s.x - m_lpCenter->s.x;
		V.s.y = lpv->s.y - m_lpCenter->s.y;
		V.s.z = lpv->s.z - m_lpCenter->s.z;

		Translate(&V, LYNX_MATH_POST);	
		UpdateTransform();	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::GetPosition(LPLYNXVECTOR3D lpv)
	{	
		lpv->s.x = m_lpCenter->s.x;
		lpv->s.y = m_lpCenter->s.y;
		lpv->s.z = m_lpCenter->s.z;	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer:: Forward(LYNXREAL d)
	{	
		LYNXVECTOR3D					V;
		
		V.s.x = LYNX_REAL_MUL(m_lpDirection->s.x, d);
		V.s.y = LYNX_REAL_MUL(m_lpDirection->s.y, d);
		V.s.z = LYNX_REAL_MUL(m_lpDirection->s.z, d);

		Translate(&V, LYNX_MATH_POST);	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SideMove(LYNXREAL d)
	{	
		LYNXVECTOR3D					V;
		
		V.s.x = LYNX_REAL_MUL(m_lpRightDir->s.x, d);
		V.s.y = LYNX_REAL_MUL(m_lpRightDir->s.y, d);
		V.s.z = LYNX_REAL_MUL(m_lpRightDir->s.z, d);

		Translate(&V, LYNX_MATH_POST);	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::Up(LYNXREAL d)
	{	
		LYNXVECTOR3D					V;
		
		V.s.x = LYNX_REAL_MUL(m_lpUpDir->s.x, d);
		V.s.y = LYNX_REAL_MUL(m_lpUpDir->s.y, d);
		V.s.z = LYNX_REAL_MUL(m_lpUpDir->s.z, d);

		Translate(&V, LYNX_MATH_POST);	
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::MulMatrix(const LYNXMATRIX& m, LYNXMATHORDER order)
	{
		LYNXMATRIX				T;

		switch (order)
		{
			case LYNX_MATH_PRE:
				lynxMatrixXMatrix(&T, &m, &m_LocalM);	
				lynxMemCpy(&m_LocalM, &T, sizeof(LYNXMATRIX));
				break;

			case LYNX_MATH_POST:
				lynxMatrixXMatrix(&T, &m_GlobalM, &m);	
				lynxMemCpy(&m_GlobalM, &T, sizeof(LYNXMATRIX));
				break;
		}
	}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void GetObject3DTransM(LPLYNXOBJ3D lpo, LYNXREAL time)
	//{
	//	LYNXREAL					lpValues[4];	
	//	
	//	LYNX_IDENTITY_MATRIX(m_TransM);		 		
	//	if (m_RotateKey.lpKeys)
	//	{
	//		#ifdef __FIXED_POINT__
	//			lynxGetAnimKeyValues(&m_RotateKey, time, lpValues, 4);
	//		#else
	//			lynxGetShortAnimKeyValues(&m_RotateKey, time, lpValues, 4, LYNX_ROTATE_KEY_ISCALE);
	//		#endif
	//		lynxQuaternion2Matrix(&m_TransM, (LPLYNXQUATERNION)lpValues);
	//	}			
	//	if (m_ScaleKey.lpKeys)
	//	{
	//		lynxGetAnimKeyValues(&m_ScaleKey, time, lpValues, 3);

	//		m_TransM.s.m11 = LYNX_REAL_MUL(m_TransM.s.m11, lpValues[0]);
	//		m_TransM.s.m21 = LYNX_REAL_MUL(m_TransM.s.m21, lpValues[1]);
	//		m_TransM.s.m31 = LYNX_REAL_MUL(m_TransM.s.m31, lpValues[2]);

	//		m_TransM.s.m12 = LYNX_REAL_MUL(m_TransM.s.m12, lpValues[0]);
	//		m_TransM.s.m22 = LYNX_REAL_MUL(m_TransM.s.m22, lpValues[1]);
	//		m_TransM.s.m32 = LYNX_REAL_MUL(m_TransM.s.m32, lpValues[2]);

	//		m_TransM.s.m13 = LYNX_REAL_MUL(m_TransM.s.m13, lpValues[0]);
	//		m_TransM.s.m23 = LYNX_REAL_MUL(m_TransM.s.m23, lpValues[1]);
	//		m_TransM.s.m33 = LYNX_REAL_MUL(m_TransM.s.m33, lpValues[2]);
	//	}			
	//	if (m_TranslateKey.lpKeys)
	//	{
	//		lynxGetAnimKeyValues(&m_TranslateKey, time, lpValues, 3);

	//		m_TransM.s.m41 += lpValues[0];
	//		m_TransM.s.m42 += lpValues[1];
	//		m_TransM.s.m43 += lpValues[2];
	//	}	
	//	
	//	if (m_VisKey.lpKeys)
	//	{
	//		#ifdef __FIXED_POINT__
	//			lynxGetAnimKeyValues(&m_VisKey, time, lpValues, 1);
	//		#else
	//			lynxGetShortAnimKeyValues(&m_VisKey, time, lpValues, 1, LYNX_ROTATE_KEY_ISCALE);
	//		#endif
	//		m_Visibility = lpValues[0];
	//	}	
	//}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CContainer::vPlay(LYNXREAL skip, LYNXPLAYMODE mode)
	//{
	//	LYNXBOOL					b;

	//	b = FALSE;	
	//	m_LastTime = m_Time;
	//	if (m_EndTime == m_StartTime)
	//	{
	//		m_Time = m_EndTime;
	//		b = TRUE;
	//	}
	//	else
	//	{
	//		m_Time+= skip;
	//		switch (mode)
	//		{
	//			case LYNX_PM_LOOP:			
	//				while (m_Time > m_EndTime)
	//				{
	//					b = TRUE;
	//					m_Time -= (m_EndTime);
	//					m_Time += (m_StartTime);
	//				}
	//				while (m_Time < m_StartTime)
	//				{
	//					b = TRUE;
	//					m_Time -= (m_StartTime);
	//					m_Time += m_EndTime;
	//				}
	//				break;

	//			case LYNX_PM_ONCE:
	//				if (m_Time > m_EndTime)
	//				{
	//					b = TRUE;
	//					m_Time = m_EndTime;
	//				}
	//				else if (m_Time < m_StartTime)
	//				{
	//					b = TRUE;
	//					m_Time = m_StartTime;
	//				}
	//				break;
	//		}
	//	}
	//	return b;
	//}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void CContainer::LookAt(LPLYNXVECTOR3D lpv)
	//{
	//	LYNXVECTOR3D			TN, TU, TV;
	//	LYNXVECTOR3D			dv,dn;
	//	LYNXREAL				f;
	//	LYNXMATRIX				R;

	//	TN.s.x = (m_lpCenter->s.x - lpv->s.x);
	//	TN.s.y = (m_lpCenter->s.y - lpv->s.y);
	//	TN.s.z = (m_lpCenter->s.z - lpv->s.z);
	//	lynxNormalise(&TN, 3);

	//	dv.s.x = TN.s.x; dv.s.y = LYNX_REAL_ADD(TN.s.y, LYNX_REAL(1)); dv.s.z = TN.s.z;
	//	lynxNormalise(&dv, 3);
	//	f = LYNX_DOT_PRODUCT(dv, TN);
	//	LYNX_VECTOR3D_MUL(dn, f, TN);
	//	LYNX_VECTOR3D_SUB(TV, dv, dn);
	//	lynxNormalise(&TV, 3);
	//	lynxCrossProduct(&TU, &TV, &TN);

	//	LYNX_MATRIX(m_LocalM,
	//				TU.s.x , TU.s.y , TU.s.z , LYNX_REAL(0),
	//				TV.s.x , TV.s.y , TV.s.z , LYNX_REAL(0),
	//				TN.s.x , TN.s.y , TN.s.z , LYNX_REAL(0),				         
	//				LYNX_REAL(0) , LYNX_REAL(0) , LYNX_REAL(0) , LYNX_REAL(1));				
	//	
	//}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*  @brief .
	//*
	//*	@param .	
	//*	@return. 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//void lynxAlignObject3DToDir(LPLYNXOBJ3D lpo, LPLYNXVECTOR3D lpv)
	//{
	//	LYNXVECTOR3D		TN, TU, TV;
	//	LYNXVECTOR3D		dv,dn;
	//	float				f;
	//	LYNXMATRIX			R;
	//	

	//	TN.s.x = lpv->s.x;
	//	TN.s.y = lpv->s.y;
	//	TN.s.z = lpv->s.z;

	//	lynxNormalise(&TN, 3);

	//	dv.s.x = TN.s.x; dv.s.y = TN.s.y + 1.0f; dv.s.z = TN.s.z;
	//	lynxNormalise(&dv, 3);
	//	f = LYNX_DOT_PRODUCT(dv, TN);
	//	LYNX_VECTOR3D_MUL(dn, f, TN);
	//	LYNX_VECTOR3D_SUB(TV, dv, dn);
	//	lynxNormalise(&TV, 3);
	//	lynxCrossProduct(&TU, &TV, &TN);

	//	LYNX_MATRIX(m_LocalM,
	//				TU.s.x , TV.s.x , TN.s.x , 0.0f,
	//				TU.s.y , TV.s.y , TN.s.y , 0.0f,
	//				TU.s.z , TV.s.z , TN.s.z , 0.0f,
	//				0.0f , 0.0f , 0.0f , 1.0f);		

	//	LYNX_MATRIX(m_LocalM,
	//				TU.s.x , TU.s.y , TU.s.z , 0.0f,
	//				TV.s.x , TV.s.y , TV.s.z , 0.0f,
	//				TN.s.x , TN.s.y , TN.s.z , 0.0f,				   
	//				0.0f , 0.0f , 0.0f , 1.0f);		
	//				   

	//	/*
	//	lynxMemCpy(&m_LocalM, &m_R, sizeof(LYNXMATRIX));
	//	*/

	//	
	//	/*
	//	dv.s.x = (lpv->s.x - m_TCenter.s.x);
	//	dv.s.y = (lpv->s.y - m_TCenter.s.y);
	//	dv.s.z = (lpv->s.z - m_TCenter.s.z);	
	//	lynxNormalise(&dv, 3);

	//	lynxVectorXMatrix(&m_TDirection, &m_Direction, &m_LocalM);

	//	TN.s.x = m_TDirection.s.x;
	//	TN.s.y = -dv.s.y + 1.0f;
	//	TN.s.z = m_TDirection.s.z;
	//	//TN.s.x = -dv.s.x; TN.s.y = -dv.s.y + 1.0f; TN.s.z = -dv.s.z;
	//	lynxNormalise(&TN, 3);
	//	f = LYNX_DOT_PRODUCT(dv, TN);
	//	LYNX_VECTOR3D_MULTIPLY(dn, f, TN);
	//	LYNX_VECTOR3D_SUBTRACT(TV, dv, dn);
	//	lynxNormalise(&TV, 3);
	//	lynxCrossProduct(&TU, &TV, &TN);

	//	LYNX_MAKE_MATRIX(m_R,
	//				TU.s.x , TV.s.x , TN.s.x , 0.0f,
	//				TU.s.y , TV.s.y , TN.s.y , 0.0f,
	//				TU.s.z , TV.s.z , TN.s.z , 0.0f,
	//				0.0f , 0.0f , 0.0f , 1.0f);			

	//	//lynxMatrixXMatrix(&R, &m_R, &m_LocalM);	 
	//	lynxMemCpy(&m_LocalM, &m_R, sizeof(LYNXMATRIX));	
	//	*/
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	const char* CContainer::GetTypeStr(TYPE t)
	{
		switch (t)
		{
			case REFERENCE:
				return ("Reference");
				break;

			case GEOMETRY:
				return ("Geometry");
				break;

				break;
			case STATIC_MESH:
				return ("Static Mesh");
				break;

				break;
			case STATIC_MODEL:
				return ("Static Model");
				break;
				
			case CAMERA:
				return ("Camera");
				break;

			case MODEL:
				return ("Model");
				break;

			case SPRITE3D:
				return ("Sprite3D");
				break;

			case CROSS_SPRITE:	
				return ("Cross Sprite");
				break;

			case PEMITTER:	
				return ("Particle Emitter");
				break;

			case MESH:
				return ("Mesh");
				break;

			case BONE:
				return ("Bone");
				break;

			default:
				return ("Unknown");
				break;
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	//void CContainer::CreateFrame(DWORD num)
	//{
	//	m_FrameArray.resize(num);		
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::AddToCurrentMaterialRenderList() 
	{
		if (m_lpCurrentMaterial && !m_SubContainerArray.size()) 
			m_lpCurrentMaterial->AddRenderableObj(this); 
		for (int i=0; i<m_SubContainerArray.size(); ++i)
		{
			m_SubContainerArray[i]->AddToCurrentMaterialRenderList();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CContainer::SetCurrentMaterialType(MATERIALTYPE i) 
	{
		if (i < m_MaterialArray.size()) 
			SetlpCurrentMaterial(m_MaterialArray[i]); 

		for (int s=0; s<m_SubContainerArray.size(); s++)
		{
			m_SubContainerArray[s]->SetCurrentMaterialType(i);
		}
	}		
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CContainer::vSetCurrentFrame(int id, LYNXBOOL bforcereset) 
	{
		return m_Animation.SetCurrentFrame(id, bforcereset);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CContainer::vSetCurrentFrame(char *name, LYNXBOOL bforcereset)
	{
		return m_Animation.SetCurrentFrame(name, bforcereset);
	}		
}