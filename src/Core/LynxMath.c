
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

#include <LynxMath.h>
#include <LynxLog.h>
#include <LynxOS.h>
#include <LynxMem.h>

LYNXREAL							gCos[360*LYNX_DEG_STEP];
LYNXREAL							gSin[360*LYNX_DEG_STEP];	
LYNXMATRIX							gIdentityM;
int									gInstructionSet = LYNX_IS_FPU;
LYNXCHAR						    gMathModuleName[] = _T("\tLynxMath");

typedef void (*LPPOINTXMATRIXPROC)(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm);
typedef void (*LPSEQPOINTXMATRIXPROC)(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, int num);
typedef void (*LPVECTOR4XMATRIXPROC)(LPLYNXVECTOR4D lpb, LPCLYNXVECTOR4D lpa, LPCLYNXMATRIX lpm);
typedef void (*LPVECTORXMATRIXPROC)(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm);
typedef void (*LPVECTORXMATRIX3PROC)(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX3 lpm);
typedef void (*LPMATRIXXMATRIXPROC)(LPLYNXMATRIX lpd, LPCLYNXMATRIX lpm1, LPCLYNXMATRIX lpm2);
typedef void (*LPMATRIX3XMATRIX3PROC)(LPLYNXMATRIX3 lpd, LPCLYNXMATRIX3 lpm1, LPCLYNXMATRIX3 lpm2);

LPPOINTXMATRIXPROC					glpPointXMatrixProc = NULL;
LPSEQPOINTXMATRIXPROC				glpSeqPointXMatrixProc = NULL;
LPVECTOR4XMATRIXPROC				glpVector4XMatrixProc = NULL;
LPVECTORXMATRIXPROC					glpVectorXMatrixProc = NULL;
LPVECTORXMATRIX3PROC				glpVectorXMatrix3Proc = NULL;
LPMATRIXXMATRIXPROC					glpMatrixXMatrixProc = NULL;
LPMATRIX3XMATRIX3PROC				glpMatrix3XMatrix3Proc = NULL;

#ifdef __WIN32__
	#if !defined(_M_AMD64)
		#include <SSE_LynxMath.inl>
	#endif
#elif __PS2__
	#include <PS2_LynxMath.inl>
#endif
#include <FPU_LynxMath.inl>
#include <FP_LynxMath.inl>

//-------------------------------------------------------------------------------------------------------
/**
 //*  @ingroup LynxMath 
 //*  @brief 。
 //*
 //*	@param 。
 //*	@param 。  
 //*	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void ilynxSetupMathProcPointer(void)
{
	#ifdef __PS2__
		#ifdef __FIXED_POINT__
		#else
			glpPointXMatrixProc = ilynxVu0_PointXMatrix;		
			glpMatrixXMatrixProc = ilynxVu0_MatrixXMatrix;		
			glpMatrix3XMatrix3Proc = ilynxVu0_MatrixXMatrix;		
		#endif	
	#elif defined(__WIN32__)
		if ((gInstructionSet & LYNX_IS_SSE2) || (gInstructionSet & LYNX_IS_SSE) )
		{
			#ifdef __FIXED_POINT__
				glpPointXMatrixProc = ilynxFP_PointXMatrix;		
				glpSeqPointXMatrixProc = ilynxFP_SequencePointXMatrix;	
				glpVectorXMatrixProc = ilynxFP_VectorXMatrix;
				glpVectorXMatrix3Proc = ilynxFP_VectorXMatrix3;
				glpMatrixXMatrixProc = ilynxFP_MatrixXMatrix;
				glpMatrix3XMatrix3Proc = ilynxFP_Matrix3XMatrix3;
			#else
				#ifdef __WIN32__
					#if defined(__WIN32__) && defined (_M_IX86)
						glpPointXMatrixProc = ilynxSSE_PointXMatrix;	
						glpSeqPointXMatrixProc = ilynxSSE_SequencePointXMatrix;	
						glpVector4XMatrixProc = ilynxSSE_Vector4XMatrix;
						glpVectorXMatrixProc = ilynxSSE_VectorXMatrix;
						glpVectorXMatrix3Proc = ilynxSSE_VectorXMatrix3;
						glpMatrixXMatrixProc = ilynxSSE_MatrixXMatrix;
						glpMatrix3XMatrix3Proc = ilynxFPU_Matrix3XMatrix3;//ilynxSSE_Matrix3XMatrix3;
					#else
						glpPointXMatrixProc = ilynxFPU_PointXMatrix;		
						glpSeqPointXMatrixProc = ilynxFPU_SequencePointXMatrix;	
						glpVector4XMatrixProc = ilynxFPU_Vector4XMatrix;
						glpVectorXMatrixProc = ilynxFPU_VectorXMatrix;
						glpVectorXMatrix3Proc = ilynxFPU_VectorXMatrix3;
						glpMatrixXMatrixProc = ilynxFPU_MatrixXMatrix;
						glpMatrix3XMatrix3Proc = ilynxFPU_Matrix3XMatrix3;
					#endif
				#endif
			#endif
		}
		else
		{
			#ifdef __FIXED_POINT__
				glpPointXMatrixProc = ilynxFP_PointXMatrix;		
				glpSeqPointXMatrixProc = ilynxFP_SequencePointXMatrix;	
				glpVectorXMatrixProc = ilynxFP_VectorXMatrix;
				glpVectorXMatrix3Proc = ilynxFP_VectorXMatrix3;
				glpMatrixXMatrixProc = ilynxFP_MatrixXMatrix;
				glpMatrix3XMatrix3Proc = ilynxFP_Matrix3XMatrix3;
			#else
				glpPointXMatrixProc = ilynxFPU_PointXMatrix;		
				glpSeqPointXMatrixProc = ilynxFPU_SequencePointXMatrix;	
				glpVector4XMatrixProc = ilynxFPU_Vector4XMatrix;
				glpVectorXMatrixProc = ilynxFPU_VectorXMatrix;
				glpVectorXMatrix3Proc = ilynxFPU_VectorXMatrix3;
				glpMatrixXMatrixProc = ilynxFPU_MatrixXMatrix;
				glpMatrix3XMatrix3Proc = ilynxFPU_Matrix3XMatrix3;
			#endif
		}		
	#else
		#ifdef __FIXED_POINT__
			glpPointXMatrixProc = ilynxFP_PointXMatrix;		
			glpSeqPointXMatrixProc = ilynxFP_SequencePointXMatrix;	
			glpVectorXMatrixProc = ilynxFP_VectorXMatrix;
			glpVectorXMatrix3Proc = ilynxFP_VectorXMatrix3;
			glpMatrixXMatrixProc = ilynxFP_MatrixXMatrix;
			glpMatrix3XMatrix3Proc = ilynxFP_Matrix3XMatrix3;
		#else
			glpPointXMatrixProc = ilynxFPU_PointXMatrix;		
			glpSeqPointXMatrixProc = ilynxFPU_SequencePointXMatrix;	
			glpVector4XMatrixProc = ilynxFPU_Vector4XMatrix;
			glpVectorXMatrixProc = ilynxFPU_VectorXMatrix;
			glpVectorXMatrix3Proc = ilynxFPU_VectorXMatrix3;
			glpMatrixXMatrixProc = ilynxFPU_MatrixXMatrix;
			glpMatrix3XMatrix3Proc = ilynxFPU_Matrix3XMatrix3;
		#endif
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 //*  @ingroup LynxMath 
 //*  @brief 。
 //*
 //*	@param 。
 //*	@param 。  
 //*	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void ilynxInitSinTable(void)
{
    int                 i;

	for (i=0; i<LYNX_DEG_STEP*360; i++)
		gSin[i] = LYNX_REAL(sin(((float)i*LYNXD2R)/((float)LYNX_DEG_STEP)));	

	gSin[0] = LYNX_REAL(sin(0.0f));	
}
//-------------------------------------------------------------------------------------------------------
/**
 //*  @ingroup LynxMath 
 //*  @brief 。
 //*
 //*	@param 。
 //*	@param 。  
 //*	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void ilynxInitCosTable(void)
{
    int                 i;

	for (i=0; i<LYNX_DEG_STEP*360; i++)
		gCos[i] = LYNX_REAL(cos(((float)i*LYNXD2R)/((float)LYNX_DEG_STEP)));	
}
 //-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath 
*  @brief 。
*
*	@param 。
*	@param 。  
*	@return 。 
*/     
//-------------------------------------------------------------------------------------------------------
void lynxSetgInstructionSet(const int s)
{
	gInstructionSet = s;
	ilynxSetupMathProcPointer();
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 初始化數學模組。
 *
 *	@param 無。 
 *	@return 無。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitMathSystem(void)
{
	LYNXCPUINFO					CPUInfo;

	lynxGetCPUInfo(&CPUInfo);
	if (CPUInfo.Feature & LYNX_CPU_FEATURE_SSE2 && CPUInfo.OSSupport & LYNX_CPU_FEATURE_SSE2)
	{		
		lynxSetLogf(gMathModuleName, _T("Instruction Set : SSE2\n"));
		lynxSetgInstructionSet(LYNX_IS_SSE2);			
		#ifdef __WIN32__
			#if defined(__WIN32__) && (_M_IX86_FP >= 1)
				_set_SSE2_enable(TRUE);
			#endif
		#endif
	}
	else if (CPUInfo.Feature & LYNX_CPU_FEATURE_SSE && CPUInfo.OSSupport & LYNX_CPU_FEATURE_SSE)
	{		
		lynxSetLogf(gMathModuleName, _T("Instruction Set : SSE\n"));
		lynxSetgInstructionSet(LYNX_IS_SSE);					
	}
	else if (CPUInfo.Feature & LYNX_CPU_FEATURE_3DNOW && CPUInfo.OSSupport & LYNX_CPU_FEATURE_3DNOW)
	{	
		//lynxSetLogf(gMathModuleName, _T("Instruction Set : 3D Now!\n"));
		lynxSetLogf(gMathModuleName, _T("Instruction Set : FPU\n"));
		lynxSetgInstructionSet(LYNX_IS_FPU);									
	}
	else
	{
		lynxSetLogf(gMathModuleName, _T("Instruction Set : FPU\n"));
		lynxSetgInstructionSet(LYNX_IS_FPU);											
	}
	LYNX_IDENTITY_MATRIX(gIdentityM);

	gIdentityM.m11 = 1.0f; gIdentityM.m12 = 0.0f; gIdentityM.m13 = 0.0f; gIdentityM.m14 = 0.0f;\
	gIdentityM.m21 = 0.0f; gIdentityM.m22 = 1.0f; gIdentityM.m23 = 0.0f; gIdentityM.m24 = 0.0f;\
	gIdentityM.m31 = 0.0f; gIdentityM.m32 = 0.0f; gIdentityM.m33 = 1.0f; gIdentityM.m34 = 0.0f;\
	gIdentityM.m41 = 0.0f; gIdentityM.m42 = 0.0f; gIdentityM.m43 = 0.0f; gIdentityM.m44 = 1.0f;

	ilynxInitSinTable();
    ilynxInitCosTable();	
	lynxSrand(lynxOSGetPerformanceCounter());
	//ilynxSetupMathProcPointer();	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 
 *
 */   
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxCheckPowerOf2(int v, int max_power)
{
	LYNXLONGLONG Power = 1;
	int i;

	for (i=0; i<=max_power; i++)
	{
		if (v == Power)
			return LYNX_TRUE;

		Power <<= 1;
	}

	return LYNX_FALSE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief Fast inverse square root function. A clever and fast code from Quake3 src code, but no one knows who wrote it。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
float lynxFastInvSqrt(const LYNXREAL x)
{
	LYNXREAL	xhalf, r; 
	int			i;
	
	xhalf = 0.5f*x;
	i = *(int*)&x;
	i = 0x5f3759df - (i>>1);
	r = *(const LYNXREAL*)&i;
	r = r*(1.5f - xhalf*r*r);

   return r;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSrand(unsigned int seed)
{
	srand(seed);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
int lynxRand()
{
	return rand();
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxProbability(const int num, const int base)
{
	//if (!((rand()%num)%num))
	if ((rand()%base) < num)
		return TRUE;

	return FALSE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxProbabilityf(const float p)
{
	if ( ((float)(rand()%100000)/100000.0f) < p)
		return TRUE;

	return FALSE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 將向量單位化。
 *
 *	@param v 欲單位化之向量指標【輸入/輸出】。
 *	@param dim 單位化之軸數(1~3)。1代表只將x軸單位化，2代表只將x,y軸單位化，3代表將x,y,z軸單位化【輸入】。  
 *	@return 無。
 *	@see LYNXVECTOR3D 
 */
//-------------------------------------------------------------------------------------------------------
void lynxNormalise2(LPLYNXVECTOR2D v, const int dim)
{
    LYNXREAL			vx, vy, vz, inv_mod, Base;
	LYNXINT64			Dot;	

    vx = v->x;
    vy = v->y;

	switch (dim)
	{
		case 1:
			if ((vx == 0.0f))
				return;
			v->x = 1.0f;
			return;
	
		case  2:
			inv_mod = LYNX_REAL(lynxFastInvSqrt((vx * vx + vy * vy)));								
			v->x = vx * inv_mod;
			v->y = vy * inv_mod;		
			return;		
	}
	return;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 將向量單位化。
 *
 *	@param v 欲單位化之向量指標【輸入/輸出】。
 *	@param dim 單位化之軸數(1~3)。1代表只將x軸單位化，2代表只將x,y軸單位化，3代表將x,y,z軸單位化【輸入】。  
 *	@return 無。
 *	@see LYNXVECTOR3D 
 */
//-------------------------------------------------------------------------------------------------------
void lynxNormalise(LPLYNXVECTOR3D v, const int dim)
{
    LYNXREAL			vx, vy, vz, inv_mod, Base;
	LYNXINT64			Dot;	

    vx = v->x;
    vy = v->y;
    vz = v->z;    

	#ifdef __FIXED_POINT__
		switch (dim)
		{
			case 1:				
				Dot = ((LYNXINT64)vx * (LYNXINT64)vx);
				Base = (LYNXFIXED)(sqrt((float)Dot));								
				if (Base == 0)
					return;
				inv_mod = LYNX_REAL_DIV(LYNX_FIXEDi(1), Base);				
				v->x = LYNX_REAL_MUL(vx, inv_mod);
				return;
		
			case  2:
				Dot = ((LYNXINT64)vx * (LYNXINT64)vx + (LYNXINT64)vy * (LYNXINT64)vy);
				Base = (LYNXFIXED)(sqrt((float)Dot));								
				if (Base == 0)
					return;
				inv_mod = LYNX_REAL_DIV(LYNX_FIXEDi(1), Base);				
				v->x = LYNX_REAL_MUL(vx, inv_mod);
				v->y = LYNX_REAL_MUL(vy, inv_mod);				
				return;

			default:
			case 3:			
				Dot = ((LYNXINT64)vx * (LYNXINT64)vx + (LYNXINT64)vy * (LYNXINT64)vy + (LYNXINT64)vz * (LYNXINT64)vz);
				Base = (LYNXFIXED)(sqrt((float)Dot));								
				if (Base == 0)
					return;				
				inv_mod = LYNX_REAL_DIV((LYNX_FIXEDi(1)), Base);				
				v->x = LYNX_REAL_MUL(vx, inv_mod);
				v->y = LYNX_REAL_MUL(vy, inv_mod);								
				v->z = LYNX_REAL_MUL(vz, inv_mod);				
				return;
		}
	#else
		switch (dim)
		{
			case 1:
				if ((vx == 0.0f))
					return;
				v->x = 1.0f;
				return;
		
			case  2:
				Base = lynxSqrt(vx * vx + vy * vy);
				if (Base == 0.0f)
					return;
				inv_mod = LYNX_REAL((1.0f / Base));				
				//inv_mod = LYNX_REAL(lynxFastInvSqrt((vx * vx + vy * vy)));								
				v->x = vx * inv_mod;
				v->y = vy * inv_mod;		
				return;

			default:
			case 3:					
				Base = lynxSqrt(vx * vx + vy * vy + vz * vz);
				if (Base == 0.0f)
					return;
				inv_mod = LYNX_REAL((1.0f / Base));
				//inv_mod = LYNX_REAL(lynxFastInvSqrt((vx * vx + vy * vy + vz * vz)));								
				v->x = vx * inv_mod;
				v->y = vy * inv_mod;
				v->z = vz * inv_mod;
				return;
		}
	#endif
	return;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求向量長度。
 *
 *	@param v 欲求長度之向量指標【輸入】。
 *	@param dim 向量之軸數(1~3)。1代表只求x軸長度，2代表只求x,z軸長度，3代表求x,y,z軸長度【輸入】。
 *	@return 向量長度。
 *	@see LYNXVECTOR3D 
 */
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxLength2(LPLYNXVECTOR2D v, const int dim)
{
    LYNXREAL			vx, vy, vz;
	LYNXINT64			Dot;

    vx = v->x;
    vy = v->y;

   if ((vx == LYNX_REAL(0)) && (vy == LYNX_REAL(0)))
        return LYNX_REAL(0);

   #ifdef __FIXED_POINT__
		switch (dim)
		{
			case 1:				
				Dot = ((LYNXINT64)vx * (LYNXINT64)vx);				
				break;
		
			default:
			case  2:
				Dot = ((LYNXINT64)vx * (LYNXINT64)vx + (LYNXINT64)vz * (LYNXINT64)vz);				
				break;
		}
		return ((LYNXFIXED)(sqrt((float)Dot)));														
	#else
		switch (dim)
		{
			case 1:
				return (LYNXREAL)(lynxSqrt(vx * vx));
				break;

			default:
			case 2:
				return (LYNXREAL)(lynxSqrt(vx * vx + vy * vy));		
				break;
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求向量長度。
 *
 *	@param v 欲求長度之向量指標【輸入】。
 *	@param dim 向量之軸數(1~3)。1代表只求x軸長度，2代表只求x,z軸長度，3代表求x,y,z軸長度【輸入】。
 *	@return 向量長度。
 *	@see LYNXVECTOR3D 
 */
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxLength(LPLYNXVECTOR3D v, const int dim)
{
    LYNXREAL			vx, vy, vz;
	LYNXINT64			Dot;

    vx = v->x;
    vy = v->y;
    vz = v->z;

   if ((vx == LYNX_REAL(0)) && (vy == LYNX_REAL(0)) && (vz == LYNX_REAL(0)))
        return LYNX_REAL(0);

   #ifdef __FIXED_POINT__
		switch (dim)
		{
			case 1:				
				Dot = ((LYNXINT64)vx * (LYNXINT64)vx);				
				break;
		
			case  2:
				Dot = ((LYNXINT64)vx * (LYNXINT64)vx + (LYNXINT64)vz * (LYNXINT64)vz);				
				break;

			case 3:			
				Dot = ((LYNXINT64)vx * (LYNXINT64)vx + (LYNXINT64)vy * (LYNXINT64)vy + (LYNXINT64)vz * (LYNXINT64)vz);				
				break;
		}
		return ((LYNXFIXED)(sqrt((float)Dot)));														
	#else
		switch (dim)
		{
			case 1:
				return (LYNXREAL)(lynxSqrt(vx * vx));
				break;

			case 2:
				return (LYNXREAL)(lynxSqrt(vx * vx + vz * vz));		
				break;

			default:
			case 3:
				return (LYNXREAL)(lynxSqrt(vx * vx + vy * vy + vz * vz));		
				break;		
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求向量長度平方。
 *
 *	@param v 欲求長度之向量指標【輸入】。
 *	@param dim 向量之軸數(1~3)。1代表只求x軸長度，2代表只求x,y軸長度，3代表求x,y,z軸長度【輸入】。
 *	@return 向量長度平方。
 *	@see LYNXVECTOR3D 
 */
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxSquaredLength2(LPLYNXVECTOR2D v, const int dim)
{
	LYNXREAL			vx, vy, vz;
	LYNXREAL			Dot;

    vx = v->x;
    vy = v->y;

   if ((vx == LYNX_REAL(0)) && (vy == LYNX_REAL(0)))
        return LYNX_REAL(0);

   #ifdef __FIXED_POINT__
		switch (dim)
		{
			case 1:				
				Dot = LYNX_REAL_MUL(vx, vx);				
				break;
		
			case  2:
				Dot = LYNX_REAL_MUL(vx, vx) + LYNX_REAL_MUL(vy, vy);				
				break;

			case 3:			
				Dot = LYNX_REAL_MUL(vx, vx) + LYNX_REAL_MUL(vy, vy) + LYNX_REAL_MUL(vz, vz);				
				break;
		}
		return Dot;														
	#else
		switch (dim)
		{
			case 1:
				return (LYNXREAL)((vx * vx));
				break;

			default:
			case 2:
				return (LYNXREAL)((vx * vx + vy * vy));		
				break;
		}
	#endif    
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求向量長度平方。
 *
 *	@param v 欲求長度之向量指標【輸入】。
 *	@param dim 向量之軸數(1~3)。1代表只求x軸長度，2代表只求x,y軸長度，3代表求x,y,z軸長度【輸入】。
 *	@return 向量長度平方。
 *	@see LYNXVECTOR3D 
 */
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxSquaredLength(LPLYNXVECTOR3D v, const int dim)
{
	LYNXREAL			vx, vy, vz;
	LYNXREAL			Dot;

    vx = v->x;
    vy = v->y;
    vz = v->z;

   if ((vx == LYNX_REAL(0)) && (vy == LYNX_REAL(0)) && (vz == LYNX_REAL(0)))
        return LYNX_REAL(0);

   #ifdef __FIXED_POINT__
		switch (dim)
		{
			case 1:				
				Dot = LYNX_REAL_MUL(vx, vx);				
				break;
		
			case  2:
				Dot = LYNX_REAL_MUL(vx, vx) + LYNX_REAL_MUL(vy, vy);				
				break;

			case 3:			
				Dot = LYNX_REAL_MUL(vx, vx) + LYNX_REAL_MUL(vy, vy) + LYNX_REAL_MUL(vz, vz);				
				break;
		}
		return Dot;														
	#else
		switch (dim)
		{
			case 1:
				return (LYNXREAL)((vx * vx));
				break;

			case 2:
				return (LYNXREAL)((vx * vx + vy * vy));		
				break;

			default:
			case 3:
				return (LYNXREAL)((vx * vx + vy * vy + vz * vz));		
				break;		
		}
	#endif    
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 點乘上矩陣。
 *
 *	@param lpb 轉換後的點之指標【輸出】。
 *	@param lpa 欲轉換的點之指標【輸入】。
 *	@param lpm 轉換矩陣之指標【輸入】。
 *	@return 無。
 *	@see LYNXVECTOR3D, LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxPointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm)
{	
	LYNX_ASSERT(glpPointXMatrixProc != NULL);
	(glpPointXMatrixProc(lpb, lpa, lpm));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 點矩陣中的點乘上矩陣。
 *
 *	@param lpb 轉換後的點矩陣【輸出】。
 *	@param lpa 欲轉換的點矩陣【輸入】。
 *	@param lpm 轉換矩陣之指標【輸入】。
 *	@return 無。
 *	@see LYNXVECTOR3D, LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSequencePointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, int num)
{
	LYNX_ASSERT(glpSeqPointXMatrixProc != NULL);
	(glpSeqPointXMatrixProc(lpb, lpa, lpm, num));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 向量乘上矩陣。
 *
 *	@param lpb 轉換後的向量指標【輸出】。
 *	@param lpa 欲轉換的向量指標【輸入】。
 *	@param lpm 轉換矩陣之指標【輸入】。
 *	@return 無。
 *	@see LYNXVECTOR3D, LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxVector4XMatrix(LPLYNXVECTOR4D lpb, LPCLYNXVECTOR4D lpa, LPCLYNXMATRIX lpm)
{		
	LYNX_ASSERT(glpVector4XMatrixProc != NULL);
    (glpVector4XMatrixProc(lpb, lpa, lpm));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 向量乘上矩陣。
 *
 *	@param lpb 轉換後的向量指標【輸出】。
 *	@param lpa 欲轉換的向量指標【輸入】。
 *	@param lpm 轉換矩陣之指標【輸入】。
 *	@return 無。
 *	@see LYNXVECTOR3D, LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxVectorXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm)
{		
	LYNX_ASSERT(glpVectorXMatrixProc != NULL);
    (glpVectorXMatrixProc(lpb, lpa, lpm));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 向量乘上矩陣。
 *
 *	@param lpb 轉換後的向量指標【輸出】。
 *	@param lpa 欲轉換的向量指標【輸入】。
 *	@param lpm 轉換矩陣之指標【輸入】。
 *	@return 無。
 *	@see LYNXVECTOR3D, LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxVectorXMatrix3(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX3 lpm)
{		
	LYNX_ASSERT(glpVectorXMatrix3Proc != NULL);
    (glpVectorXMatrix3Proc(lpb, lpa, lpm));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 向量矩陣中的向量乘上矩陣。
 *
 *	@param lpb 轉換後的點矩陣【輸出】。
 *	@param lpa 欲轉換的點矩陣【輸入】。
 *	@param lpm 轉換矩陣之指標【輸入】。
 *	@return 無。
 *	@see LYNXVECTOR3D, LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSequenceVectorXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, const int num)
{
    int					i;
	LPLYNXVECTOR3D		B = lpb;
	LPCLYNXVECTOR3D		A = lpa;

	for(i=num; i>0; i--)
	{
		B->x = A->x*lpm->m11 + A->y*lpm->m21 + A->z*lpm->m31;
		B->y = A->x*lpm->m12 + A->y*lpm->m22 + A->z*lpm->m32;
		B->z = A->x*lpm->m13 + A->y*lpm->m23 + A->z*lpm->m33;
		A++;
		B++;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
// void	lynxSequenceRenderVertexXMatrix(LPLYNXRENDERVERTEX lpb, LPLYNXVERTEX3D lpa, LPLYNXMATRIX lpm, int num)
//{
//	int					i;
//	LPLYNXRENDERVERTEX			B = lpb;
//	LPLYNXVERTEX3D				A = lpa;
//
//	#ifdef __FIXED_POINT__
//		for(i=num; i>0; i--)	
//		{
//			LYNX_RV_X((*B)) = LYNX_REAL_ADD(LYNX_REAL_MUL(A->x, lpm->m11), LYNX_REAL_ADD(LYNX_REAL_MUL(A->y, lpm->m21), LYNX_REAL_ADD(LYNX_REAL_MUL(A->z, lpm->m31), lpm->m41)));
//			LYNX_RV_Y((*B)) = LYNX_REAL_ADD(LYNX_REAL_MUL(A->x, lpm->m12), LYNX_REAL_ADD(LYNX_REAL_MUL(A->y, lpm->m22), LYNX_REAL_ADD(LYNX_REAL_MUL(A->z, lpm->m32), lpm->m42)));
//			LYNX_RV_Z((*B)) = LYNX_REAL_ADD(LYNX_REAL_MUL(A->x, lpm->m13), LYNX_REAL_ADD(LYNX_REAL_MUL(A->y, lpm->m23), LYNX_REAL_ADD(LYNX_REAL_MUL(A->z, lpm->m33), lpm->m43)));			
//			//LYNX_RV_SET_RGBA((*B), A->Color.Red, A->Color.Green, A->Color.Blue, A->Color.Alpha);
//			LYNX_RV_SET_UV((*B), 0, A->TexCoord[0].u, A->TexCoord[0].v);
//			B++;
//			A++;
//		}	
//	#else
//		for(i=num; i>0; i--)	
//		{
//			LYNX_RV_X((*B)) = A->x*lpm->m11 + A->y*lpm->m21 + A->z*lpm->m31 + lpm->m41;
//			LYNX_RV_Y((*B)) = A->x*lpm->m12 + A->y*lpm->m22 + A->z*lpm->m32 + lpm->m42;
//			LYNX_RV_Z((*B)) = A->x*lpm->m13 + A->y*lpm->m23 + A->z*lpm->m33 + lpm->m43;
//			//LYNX_RV_SET_RGBA((*B), A->Color.Red, A->Color.Green, A->Color.Blue, A->Color.Alpha);
//			LYNX_RV_SET_UV((*B), 0, A->TexCoord[0].u, A->TexCoord[0].v);
//			B++;
//			A++;
//		}	
//	#endif
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxMath 
// *  @brief 。
// *
// *	@param 。
// *	@param 。  
// *	@return 。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxSequenceRenderVertexXMatrixV(LPLYNXRENDERVERTEX lpb, LPLYNXVECTOR3D lpa, LPLYNXMATRIX lpm, int num)
//{
//	int							i;
//	LPLYNXRENDERVERTEX			B = lpb;
//	LPLYNXVECTOR3D				A = lpa;
//
//	for(i=num; i>0; i--)	
//	{
//		#ifdef __PS2__
//			//ilynxVu0_RenderVXMatrix(B, A, lpm);
//			B->x = A->x*lpm->m11 + A->y*lpm->m21 + A->z*lpm->m31 + lpm->m41;
//			B->y = A->x*lpm->m12 + A->y*lpm->m22 + A->z*lpm->m32 + lpm->m42;
//			B->z = A->x*lpm->m13 + A->y*lpm->m23 + A->z*lpm->m33 + lpm->m43;			
//			//B->w = A->x*lpm->m14 + A->y*lpm->m24 + A->z*lpm->m34 + lpm->m44;			
//			B++;
//			A++;
//		#else
//			LYNX_RV_X((*B)) = LYNX_REAL_ADD(LYNX_REAL_MUL(A->x, lpm->m11), LYNX_REAL_ADD(LYNX_REAL_MUL(A->y, lpm->m21), LYNX_REAL_ADD(LYNX_REAL_MUL(A->z, lpm->m31), lpm->m41)));
//			LYNX_RV_Y((*B)) = LYNX_REAL_ADD(LYNX_REAL_MUL(A->x, lpm->m12), LYNX_REAL_ADD(LYNX_REAL_MUL(A->y, lpm->m22), LYNX_REAL_ADD(LYNX_REAL_MUL(A->z, lpm->m32), lpm->m42)));
//			LYNX_RV_Z((*B)) = LYNX_REAL_ADD(LYNX_REAL_MUL(A->x, lpm->m13), LYNX_REAL_ADD(LYNX_REAL_MUL(A->y, lpm->m23), LYNX_REAL_ADD(LYNX_REAL_MUL(A->z, lpm->m33), lpm->m43)));			
//			B++;
//			A++;
//		#endif
//	}	
//}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 計算兩項量的內積。
 *	
 *	@param lpa 向量1【輸入】。
 *	@param lpb 向量2【輸入】。
 *	@return 內積值
 *	@see LYNXVECTOR3D  
 */
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxDotProduct(LPCLYNXVECTOR3D lpa, LPCLYNXVECTOR3D lpb)
{
	return ((lpa->x * lpb->x) + (lpa->y * lpb->y) + (lpa->z * lpb->z));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 計算兩項量的外積。
 *
 *	@param lpd 計算出的外積向量【輸出】。
 *	@param lpa 向量1【輸入】。
 *	@param lpb 向量2【輸入】。
 *	@return 無。
 *	@see LYNXVECTOR3D  
 */
//-------------------------------------------------------------------------------------------------------
void lynxCrossProduct(LPLYNXVECTOR3D lpd, LPCLYNXVECTOR3D lpa, LPCLYNXVECTOR3D lpb)
{
	#ifdef __PS2__
		ilynxVu0_CrossProduct(lpd, lpa, lpb);
	#else
		#ifdef __FIXED_POINT__
			lpd->x = LYNX_REAL_MUL((lpa->y), (lpb->z)) -  LYNX_REAL_MUL((lpa->z), (lpb->y));
			lpd->y = LYNX_REAL_MUL((lpa->z), (lpb->x)) -  LYNX_REAL_MUL((lpa->x), (lpb->z));
			lpd->z = LYNX_REAL_MUL((lpa->x), (lpb->y)) -  LYNX_REAL_MUL((lpa->y), (lpb->x));
		#else
			lpd->x = lpa->y * lpb->z - lpa->z * lpb->y;
			lpd->y = lpa->z * lpb->x - lpa->x * lpb->z;
			lpd->z = lpa->x * lpb->y - lpa->y * lpb->x;
		#endif
	#endif
    return;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSequenceCrossProduct(LPLYNXVECTOR3D lpd, LPCLYNXVECTOR3D lpa, LPCLYNXVECTOR3D lpb, const int num)
{
	int				i;
	LPLYNXVECTOR3D	D = lpd;
	LPCLYNXVECTOR3D	A = lpa;
	LPCLYNXVECTOR3D	B = lpb;

	for (i=num; i>0; i--)
	{
	    D->x = A->y * B->z - A->z * B->y;
		D->y = A->z * B->x - A->x * B->z;
		D->z = A->x * B->y - A->y * B->x;

		A++;
		B++;
		D++;
	}
    return;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 4X4矩陣相乘。
 *
 *	@param lpd 計算出新矩陣【輸出】。
 *	@param lpm1 矩陣1【輸入】。  
 *	@param lpm2 矩陣2【輸入】。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMatrixXMatrix(LPLYNXMATRIX lpd, LPCLYNXMATRIX lpm1, LPCLYNXMATRIX lpm2)
{	
	LYNX_ASSERT(glpMatrixXMatrixProc != NULL);
	(glpMatrixXMatrixProc(lpd, lpm1, lpm2));	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 3X3矩陣相乘。
 *
 *	@param lpd 計算出新矩陣【輸出】。
 *	@param lpm1 矩陣1【輸入】。  
 *	@param lpm2 矩陣2【輸入】。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMatrix3XMatrix3(LPLYNXMATRIX3 lpd, LPCLYNXMATRIX3 lpm1, LPCLYNXMATRIX3 lpm2)
{	
	LYNX_ASSERT(glpMatrix3XMatrix3Proc != NULL);
	(glpMatrix3XMatrix3Proc(lpd, lpm1, lpm2));		
}
//-------------------------------------------------------------------------------------------------------
//
/**
 *  @ingroup LynxMath 
 *  @brief 求一矩陣之倒矩陣(invert matrix)。
 * 
 *  求一矩陣之倒矩陣(invert matrix)，-1 * d = a。
 *	@param d 求得的倒矩陣【輸出】。 
 *	@param a 求倒矩陣之矩陣【輸入】。 
 *	@return 無。
 *	@see LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInvertMatrix(LPLYNXMATRIX d, LPCLYNXMATRIX a)
{
    d->m11 = a->m11;
    d->m12 = a->m21;
    d->m13 = a->m31;
    d->m14 = a->m14;

    d->m21 = a->m12;
    d->m22 = a->m22;
    d->m23 = a->m32;
    d->m24 = a->m24;

    d->m31 = a->m13;
    d->m32 = a->m23;
    d->m33 = a->m33;
    d->m34 = a->m34;

	//d->m41 = a->m14;
    //d->m42 = a->m24;
    //d->m43 = a->m34;
    //d->m44 = a->m44;

    d->m41 = -a->m41;
    d->m42 = -a->m42;
    d->m43 = -a->m43;
    d->m44 = a->m44;

    return;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求一矩陣之反矩陣(inverse matrix)，a * d  = I。
 *
 *	注意，如果輸入矩陣為singular，則傳回之矩陣為單位矩陣。
 *	@param d 求得的反矩陣【輸出】。 
 *	@param a 求反矩陣之矩陣【輸入】。 
 *	@return 無。
 *	@see LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInverseMatrix(LPLYNXMATRIX d, LPCLYNXMATRIX a)
{
	LYNXREAL fInvDet, fDet;
	LYNXREAL fA0, fA1, fA2, fA3, fA4, fA5, fB0, fB1, fB2, fB3, fB4, fB5;


	fA0 = a->f[ 0]*a->f[ 5] - a->f[ 1]*a->f[ 4];
    fA1 = a->f[ 0]*a->f[ 6] - a->f[ 2]*a->f[ 4];
    fA2 = a->f[ 0]*a->f[ 7] - a->f[ 3]*a->f[ 4];
    fA3 = a->f[ 1]*a->f[ 6] - a->f[ 2]*a->f[ 5];
    fA4 = a->f[ 1]*a->f[ 7] - a->f[ 3]*a->f[ 5];
    fA5 = a->f[ 2]*a->f[ 7] - a->f[ 3]*a->f[ 6];
    fB0 = a->f[ 8]*a->f[13] - a->f[ 9]*a->f[12];
    fB1 = a->f[ 8]*a->f[14] - a->f[10]*a->f[12];
    fB2 = a->f[ 8]*a->f[15] - a->f[11]*a->f[12];
    fB3 = a->f[ 9]*a->f[14] - a->f[10]*a->f[13];
    fB4 = a->f[ 9]*a->f[15] - a->f[11]*a->f[13];
    fB5 = a->f[10]*a->f[15] - a->f[11]*a->f[14];

    fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
	if (fabs(fDet) <= LYNX_FLOAT_TOLERANCE)  //singular
    {
        LYNX_ZERO_MATRIX((*d));
    }
	
    d->f[ 0] =
        + a->f[ 5]*fB5 - a->f[ 6]*fB4 + a->f[ 7]*fB3;
    d->f[ 4] =
        - a->f[ 4]*fB5 + a->f[ 6]*fB2 - a->f[ 7]*fB1;
    d->f[ 8] =
        + a->f[ 4]*fB4 - a->f[ 5]*fB2 + a->f[ 7]*fB0;
    d->f[12] =
        - a->f[ 4]*fB3 + a->f[ 5]*fB1 - a->f[ 6]*fB0;
    d->f[ 1] =
        - a->f[ 1]*fB5 + a->f[ 2]*fB4 - a->f[ 3]*fB3;
    d->f[ 5] =
        + a->f[ 0]*fB5 - a->f[ 2]*fB2 + a->f[ 3]*fB1;
    d->f[ 9] =
        - a->f[ 0]*fB4 + a->f[ 1]*fB2 - a->f[ 3]*fB0;
    d->f[13] =
        + a->f[ 0]*fB3 - a->f[ 1]*fB1 + a->f[ 2]*fB0;
    d->f[ 2] =
        + a->f[13]*fA5 - a->f[14]*fA4 + a->f[15]*fA3;
    d->f[ 6] =
        - a->f[12]*fA5 + a->f[14]*fA2 - a->f[15]*fA1;
    d->f[10] =
        + a->f[12]*fA4 - a->f[13]*fA2 + a->f[15]*fA0;
    d->f[14] =
        - a->f[12]*fA3 + a->f[13]*fA1 - a->f[14]*fA0;
    d->f[ 3] =
        - a->f[ 9]*fA5 + a->f[10]*fA4 - a->f[11]*fA3;
    d->f[ 7] =
        + a->f[ 8]*fA5 - a->f[10]*fA2 + a->f[11]*fA1;
    d->f[11] =
        - a->f[ 8]*fA4 + a->f[ 9]*fA2 - a->f[11]*fA0;
    d->f[15] =
        + a->f[ 8]*fA3 - a->f[ 9]*fA1 + a->f[10]*fA0;

    fInvDet = ((LYNXREAL)1.0)/fDet;
    d->f[ 0] *= fInvDet;
    d->f[ 1] *= fInvDet;
    d->f[ 2] *= fInvDet;
    d->f[ 3] *= fInvDet;
    d->f[ 4] *= fInvDet;
    d->f[ 5] *= fInvDet;
    d->f[ 6] *= fInvDet;
    d->f[ 7] *= fInvDet;
    d->f[ 8] *= fInvDet;
    d->f[ 9] *= fInvDet;
    d->f[10] *= fInvDet;
    d->f[11] *= fInvDet;
    d->f[12] *= fInvDet;
    d->f[13] *= fInvDet;
    d->f[14] *= fInvDet;
    d->f[15] *= fInvDet;

	
	//float				fDet, fInvdet;
	//int					Row, Col;

	//LYNX_IDENTITY_MATRIX((*d));
	//d->m[0][0] = a->m[1][1]*a->m[2][2] - a->m[1][2]*a->m[2][1];
 //   d->m[0][1] = a->m[0][2]*a->m[2][1] - a->m[0][1]*a->m[2][2];
 //   d->m[0][2] = a->m[0][1]*a->m[1][2] - a->m[0][2]*a->m[1][1];
 //   d->m[1][0] = a->m[1][2]*a->m[2][0] - a->m[1][0]*a->m[2][2];
 //   d->m[1][1] = a->m[0][0]*a->m[2][2] - a->m[0][2]*a->m[2][0];
 //   d->m[1][2] = a->m[0][2]*a->m[1][0] - a->m[0][0]*a->m[1][2];
 //   d->m[2][0] = a->m[1][0]*a->m[2][1] - a->m[1][1]*a->m[2][0];
 //   d->m[2][1] = a->m[0][1]*a->m[2][0] - a->m[0][0]*a->m[2][1];
 //   d->m[2][2] = a->m[0][0]*a->m[1][1] - a->m[0][1]*a->m[1][0];
	//
 //   fDet = a->m[0][0]*d->m[0][0] + a->m[0][1]*d->m[1][0] + a->m[0][2]*d->m[2][0];
 //   if (fabs(fDet) <= LYNX_FLOAT_TOLERANCE)  //singular
 //       return;

 //   fInvdet = 1.0f/fDet;
 //   for (Row = 0; Row < 3; Row++)
 //   {
 //       for (Col = 0; Col < 3; Col++)
 //       {
 //           d->m[Row][Col] *= fInvdet;
 //       }
 //   }
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//  說明:   
//		Set the rotation part of a matrix such that the vector lpd is the new
//		z-axis and lpu is the new y-axis.
//-------------------------------------------------------------------------------------------------------
void lynxSetRotationMatrix(LPLYNXMATRIX lpm, LPCLYNXVECTOR3D lpd, LPCLYNXVECTOR3D lpu)
{
    float t;
    LYNXVECTOR3D d, u, r;

    /*
     * Normalise the direction vector.
     */
    d.x = lpd->x;
    d.y = lpd->y;
    d.z = lpd->z;
    lynxNormalise(&d, 3);

    u.x = lpu->x;
    u.y = lpu->y;
    u.z = lpu->z;
    /*
     * Project u into the plane defined by d and normalise.
     */
    t = u.x * d.x + u.y * d.y + u.z * d.z;
    u.x -= d.x * t;
    u.y -= d.y * t;
    u.z -= d.z * t;
    lynxNormalise(&u, 3);

    /*
     * Calculate the vector pointing along the matrix x axis (in a right
     * handed coordinate system) using cross product.
     */
    lynxCrossProduct(&r, &u, &d);

	LYNX_IDENTITY_MATRIX((*lpm));
    lpm->m11 = r.x;
    lpm->m12 = r.y, lpm->m13 = r.z;
    lpm->m21 = u.x;
    lpm->m22 = u.y, lpm->m23 = u.z;
    lpm->m31 = d.x;
    lpm->m32 = d.y;
    lpm->m33 = d.z;

    return;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//  說明:   
//		Calculates a point along a B-Spline curve defined by four points. p
//		n output, contain the point. 
//		Position along the curve between p2 and p3.  This position is a float between 0
//		and 1. p1, p2, p3, p4    Points defining spline curve. p, at parameter
//		t along the spline curve
//-------------------------------------------------------------------------------------------------------
void lynxSpline(LPLYNXVECTOR3D p, const float t, LPCLYNXVECTOR3D p1, LPCLYNXVECTOR3D p2, LPCLYNXVECTOR3D p3, LPCLYNXVECTOR3D p4)
{
    double t2, t3;
    float m1, m2, m3, m4;

    t2 = (double)(t * t);
    t3 = t2 * (double)t;

    m1 = (float)((-1.0 * t3) + (2.0 * t2) + (-1.0 * (double)t));
    m2 = (float)((3.0 * t3) + (-5.0 * t2) + (0.0 * (double)t) + 2.0);
    m3 = (float)((-3.0 * t3) + (4.0 * t2) + (1.0 * (double)t));
    m4 = (float)((1.0 * t3) + (-1.0 * t2) + (0.0 * (double)t));

    m1 /= (float)2.0;
    m2 /= (float)2.0;
    m3 /= (float)2.0;
    m4 /= (float)2.0;

    p->x = p1->x * m1 + p2->x * m2 + p3->x * m3 + p4->x * m4;
    p->y = p1->y * m1 + p2->y * m2 + p3->y * m3 + p4->y * m4;
    p->z = p1->z * m1 + p2->z * m2 + p3->z * m3 + p4->z * m4;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//  說明:   
//		Calculates a point along a B-Spline curve defined by four points. p
//		n output, contain the point. 
//		Position along the curve between p2 and p3.  This position is a float between 0
//		and 1. p1, p2, p3, p4    Points defining spline curve. p, at parameter
//		t along the spline curve
//-------------------------------------------------------------------------------------------------------
void lynxSplinef(float* p, const int n, const float t, const float* p1, const float* p2, const float* p3, const float* p4)
{
	int		i;		
    double	t2, t3;
    float	m1, m2, m3, m4;

    t2 = (double)(t * t);
    t3 = t2 * (double)t;

    m1 = (float)((-1.0 * t3) + (2.0 * t2) + (-1.0 * (double)t));
    m2 = (float)((3.0 * t3) + (-5.0 * t2) + (0.0 * (double)t) + 2.0);
    m3 = (float)((-3.0 * t3) + (4.0 * t2) + (1.0 * (double)t));
    m4 = (float)((1.0 * t3) + (-1.0 * t2) + (0.0 * (double)t));

    m1 /= (float)2.0;
    m2 /= (float)2.0;
    m3 /= (float)2.0;
    m4 /= (float)2.0;

	for (i=0; i<n; i++)
		p[i] = p1[i] * m1 + p2[i] * m2 + p3[i] * m3 + p4[i] * m4;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 產生一個旋轉矩陣。
 *
 *	@param m 產生的旋轉矩陣【輸出】。
 *	@param deg 旋轉角度【輸入】。  
 *	@param axis 選轉軸向【輸入】。  
 *	@return 無。 
  *	@see LYNXMATRIX, LYNXAXISTYPE 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMakeRotateMatrix(LPLYNXMATRIX m, const LYNXREAL deg, const LYNXAXISTYPE axis)
{
	LYNXREAL sn, cs;
	
	sn = lynxSin(deg);
    cs = lynxCos(deg);
	//sn = sin(LYNXD2R * (*lpdegree));
    //cs = cos(LYNXD2R * (*lpdegree));

	LYNX_IDENTITY_MATRIX((*m));
	switch (axis)
	{
		case  LYNX_X_AXIS:		
			m->m22 = cs;
			m->m23 = sn;
			m->m32 = -sn;
			m->m33 = cs;
			break;
	
		case LYNX_Y_AXIS:
    		m->m11 = cs;
			m->m13 = -sn;
			m->m31 = sn;
			m->m33 = cs;
			break;

		case LYNX_Z_AXIS:			
			m->m11 = cs;
			m->m12 = sn;
			m->m21 = -sn;
			m->m22 = cs;
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 產生一個旋轉矩陣。
 *
 *	@param m 產生的旋轉矩陣【輸出】。
 *	@param deg 旋轉角度【輸入】。  
 *	@param axis 選轉軸向【輸入】。  
 *	@return 無。 
  *	@see LYNXMATRIX, LYNXAXISTYPE 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMakeRotateMatrix3(LPLYNXMATRIX3 m, const LYNXREAL deg)
{
	LYNXREAL           sn, cs;
	
	sn = lynxSin(deg);
    cs = lynxCos(deg);
	//sn = sin(LYNXD2R * (*lpdegree));
    //cs = cos(LYNXD2R * (*lpdegree));

	LYNX_IDENTITY_MATRIX3((*m));
	m->m11 = cs;
	m->m12 = sn;
	m->m21 = -sn;
	m->m22 = cs;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 將一個四元數(Quaternion)轉換成旋轉矩陣。
 *
 *	@param lpm 產生的旋轉矩陣【輸出】。
 *	@param lpq 四元數【輸入】。   
 *	@return 無。 
  *	@see LYNXMATRIX, LYNXQUATERNION 
 */
//-------------------------------------------------------------------------------------------------------
void lynxQuaternion2Matrix(LPLYNXMATRIX lpm, LPCLYNXQUATERNION lpq)
{
	LYNXREAL					s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

	//D3DXMATRIX					M;	
	//D3DXQUATERNION				Q;
	
	/*
	Q.x = lpq->x;
	Q.y = lpq->y;
	Q.z = lpq->z;
	Q.w = lpq->w;

	D3DXMatrixRotationQuaternion(&M, &Q);
	LYNX_MAKE_MATRIX((*lpm), M._11, M._12, M._13, M._14,
		                   M._21, M._22, M._23, M._24,
						   M._31, M._32, M._33, M._34,
						   M._41, M._42, M._43, M._44);	

	return;	
	*/
		 


	/*if (lpq->w == 0.0f)
	{
		lynxIdentityMatrix(lpm);
	}
	else*/
	
	/*
	Deg = acos(lpq->w)*2.0f*LYNXR2D;

	if (Deg > 140.0f)
	{
		Deg1 = 90.0f;
		Q1.w = cos(Deg1*0.5f*LYNXD2R);
		Deg = Deg - Deg1;
		Q2.w = cos(Deg*0.5f*LYNXD2R);
		Q1.x = lpq->x;
		Q1.y = lpq->y;
		Q1.x = lpq->z;

		Q2.x = lpq->x;
		Q2.y = lpq->y;
		Q2.x = lpq->z;

		lynxQuaternion2Matrix(&M1, &Q1);
		lynxQuaternion2Matrix(&M2, &Q2);
		lynxMatrixXMatrix(lpm, &M1, &M2);
	}
	else		
	*/
	
	{
		//s = LYNX_REAL_DIV(LYNX_REAL(2), LYNX_REAL_ADD(LYNX_REAL_MUL(lpq->x, lpq->x), LYNX_REAL_ADD(LYNX_REAL_MUL(lpq->y, lpq->y), LYNX_REAL_ADD(LYNX_REAL_MUL(lpq->z, lpq->z), LYNX_REAL_MUL(lpq->w, lpq->w)))));
		s = LYNX_REAL(2);

	    xs = LYNX_REAL_MUL(lpq->x, s);        ys = LYNX_REAL_MUL(lpq->y, s);        zs = LYNX_REAL_MUL(lpq->z, s);
	    wx = LYNX_REAL_MUL(lpq->w, xs);       wy = LYNX_REAL_MUL(lpq->w, ys);       wz = LYNX_REAL_MUL(lpq->w, zs);
		xx = LYNX_REAL_MUL(lpq->x, xs);       xy = LYNX_REAL_MUL(lpq->x, ys);       xz = LYNX_REAL_MUL(lpq->x, zs);
		yy = LYNX_REAL_MUL(lpq->y, ys);       yz = LYNX_REAL_MUL(lpq->y, zs);       zz = LYNX_REAL_MUL(lpq->z, zs);

		lpm->m11 = LYNX_REAL_SUB(LYNX_REAL(1), LYNX_REAL_ADD(yy, zz));
		lpm->m12 = LYNX_REAL_ADD(xy, wz);
		lpm->m13 = LYNX_REAL_SUB(xz, wy);
		lpm->m14 = LYNX_REAL(0);

		lpm->m21 = LYNX_REAL_SUB(xy, wz);
		lpm->m22 = LYNX_REAL_SUB(LYNX_REAL(1), LYNX_REAL_ADD(xx, zz));
		lpm->m23 = LYNX_REAL_ADD(yz, wx);
		lpm->m24 = LYNX_REAL(0);
		
		lpm->m31 = LYNX_REAL_ADD(xz, wy);
		lpm->m32 = LYNX_REAL_SUB(yz, wx);
		lpm->m33 = LYNX_REAL_SUB(LYNX_REAL(1), LYNX_REAL_ADD(xx, yy));
		lpm->m34 = LYNX_REAL(0);

		lpm->m41 = lpm->m42 = lpm->m43 = LYNX_REAL(0);
		lpm->m44 = LYNX_REAL(1);		
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 將一個旋轉矩陣轉換成四元數(Quaternion)。
 *
 *	@param lpq 產生的四元數【輸出】。
 *	@param lpm 旋轉矩陣【輸入】。   
 *	@return 無。 
  *	@see LYNXMATRIX, LYNXQUATERNION 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMatrix2Quaternion(LPLYNXQUATERNION lpq, LPCLYNXMATRIX lpm)
{
	float			tr, s;
	int				i, j, k;
	float			lpM[4][4], lpQ[3];
	int				nxt[3] = {1, 2, 0};

	/*
	D3DXMATRIX		M;
	D3DXQUATERNION	Q;		

	M._11 = lpm->m11; M._12 = lpm->m12; M._13 = lpm->m13; M._14 = lpm->m14;
	M._21 = lpm->m21; M._22 = lpm->m22; M._23 = lpm->m23; M._24 = lpm->m24;
	M._31 = lpm->m31; M._32 = lpm->m32; M._33 = lpm->m33; M._34 = lpm->m34;
	M._41 = lpm->m41; M._42 = lpm->m42; M._43 = lpm->m43; M._44 = lpm->m44;

	D3DXQuaternionRotationMatrix(&Q, &M);
	lpq->x = Q.x;
	lpq->y = Q.y;
	lpq->z = Q.z;
	lpq->w = Q.w;
	return;	
	*/
			
	tr = lpm->m11 + lpm->m22 + lpm->m33;
	if (tr > 0.0f)
	{
		s = (float)lynxSqrt(tr + 1.0f);
		lpq->w = s * 0.5f;

		s = 0.5f/s;
		lpq->x = (lpm->m23 - lpm->m32)*s;
		lpq->y = (lpm->m31 - lpm->m13)*s;
		lpq->z = (lpm->m12 - lpm->m21)*s;		
	}
	else
	{
		lynxMemCpy(lpM, lpm, sizeof(LYNXMATRIX));
		i = 0;
		if (lpM[1][1] > lpM[0][0])
			i = 1;
		if (lpM[2][2] > lpM[i][i])
			i = 2;
		j = nxt[i];
		k = nxt[j];

		s = (float)lynxSqrt( lpM[i][i] - ( lpM[j][j] + lpM[k][k] ) + 1.0f);

		lpQ[i] = s*0.5f;
		s = 0.5f/s;
		lpq->w = (lpM[j][k] - lpM[k][j])*s;		

		lpQ[j] = (lpM[i][j] + lpM[j][i])*s;
		lpQ[k] = (lpM[i][k] + lpM[k][i])*s;
		lpq->x = lpQ[0];
		lpq->y = lpQ[1];
		lpq->z = lpQ[2];				
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 將EulerAngles轉換成旋轉矩陣。
 *
 *	@param lpm 產生的旋轉矩陣【輸出】。
 *	@param lpv EulerAngles，x, y, z 各代表各軸向的旋轉角度【輸入】。   
 *	@return 無。 
  *	@see LYNXMATRIX, LYNXVECTOR3D 
 */
//-------------------------------------------------------------------------------------------------------
void lynxEulerAnglesXYZ2Matrix(LPLYNXMATRIX lpm, LPCLYNXVECTOR3D lpv)
{
	float				fCos, fSin;
	LYNXMATRIX			MX, MY, MZ, TM;	
	
	#ifdef __FIXED_POINT__
	#else
		fCos = lynxCos(lpv->x);
		fSin = lynxSin(lpv->x);
		LYNX_MATRIX(MX, 
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, fCos, fSin, 0.0f,
					0.0f, -fSin, fCos, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
		fCos = lynxCos(lpv->y);
		fSin = lynxSin(lpv->y);
		LYNX_MATRIX(MY, 
					fCos, 0.0f, -fSin, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					fSin, 0.0f, fCos, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
		fCos = lynxCos(lpv->z);
		fSin = lynxSin(lpv->z);
		LYNX_MATRIX(MZ, 
					fCos,  fSin, 0.0f, 0.0f,
					-fSin, fCos, 0.0f, 0.0f,
					0.0f,  0.0f, 1.0f, 0.0f,
					0.0f,  0.0f, 0.0f, 1.0f);    
		lynxMatrixXMatrix(&TM, &MX, &MY);
		lynxMatrixXMatrix(lpm, &TM, &MZ);    
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 將旋轉矩陣轉換成EulerAngles。
 *
 *	@param lpv EulerAngles，x, y, z 各代表各軸向的旋轉角度【輸出】。   
 *	@param lpm 產生的旋轉矩陣【輸入】。 
 *	@return 無。 
 *	@see LYNXMATRIX, LYNXVECTOR3D 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMatrix2EulerAnglesXYZ(LPLYNXVECTOR3D lpv, LPCLYNXMATRIX lpm)
{
	float cx,sx,x;
	float cy,sy,y,yr;
	float cz,sz,z;

	sy = -lpm->m[0][2];
	cy = sqrt(1 - (sy * sy));
	lpv->y = atan2(sy,cy) * LYNXR2D;

	if (sy != 1.0f && sy != -1.0f)
	{
		cx = lpm->m[2][2] / cy;
		sx = lpm->m[1][2] / cy;
		lpv->x = atan2(sx,cx) * LYNXR2D;

		cz = lpm->m[0][0] / cy;
		sz = lpm->m[0][1] / cy;
		lpv->z = atan2(sz,cz) * LYNXR2D;
	}
	else
	{
		cx = lpm->m[1][1];
		sx = -lpm->m[2][1];
		lpv->x = atan2(sx,cx) * LYNXR2D;

		cz = 1.0f;
		sz = 0.0f;
		lpv->z = atan2(sz,cz) * LYNXR2D;
	}

	// rot =  cy*cz          -cy*sz           sy
    //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
    //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

    /*
	rfPAngle = MgcMath::ASin(m_aafEntry[0][2]);
    if ( rfPAngle < MgcMath::HALF_PI )
    {
        if ( rfPAngle > -MgcMath::HALF_PI )
        {
            rfYAngle = MgcMath::ATan2(-m_aafEntry[1][2],m_aafEntry[2][2]);
            rfRAngle = MgcMath::ATan2(-m_aafEntry[0][1],m_aafEntry[0][0]);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRmY = MgcMath::ATan2(m_aafEntry[1][0],m_aafEntry[1][1]);
            rfRAngle = 0.0;  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
        float fRpY = MgcMath::ATan2(m_aafEntry[1][0],m_aafEntry[1][1]);
        rfRAngle = 0.0;  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief Transpose一個矩陣的值。
 *
 *	@param lpm 欲transpose的矩陣【輸出/輸入】。
 *	@return 無。 
 *	@see LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxTransposeMatrix(LPLYNXMATRIX lpm)
{
	LYNXREAL			t;

	t = lpm->m12;
	lpm->m12 = lpm->m21;
	lpm->m21 = t;

	t = lpm->m13;
	lpm->m13 = lpm->m31;
	lpm->m31 = t;

	t = lpm->m14;
	lpm->m14 = lpm->m41;
	lpm->m41 = t;

	t = lpm->m23;
	lpm->m23 = lpm->m32;
	lpm->m32 = t;

	t = lpm->m24;
	lpm->m24 = lpm->m42;
	lpm->m42 = t;

	t = lpm->m34;
	lpm->m34 = lpm->m43;
	lpm->m43 = t;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief Transpose一個矩陣的值。
 *
 *	@param lpmo transpose後的矩陣【輸出】。
 *	@param lpm 欲transpose的矩陣【輸入】。
 *	@return 無。 
 *	@see LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxTransposeMatrix2(LPLYNXMATRIX lpmo, LPCLYNXMATRIX lpm)
{
	lpmo->m12 = lpm->m21;
	lpmo->m21 = lpm->m12;

	lpmo->m13 = lpm->m31;
	lpmo->m31 = lpm->m13;

	lpmo->m14 = lpm->m41;
	lpmo->m41 = lpm->m14;

	lpmo->m23 = lpm->m32;
	lpmo->m32 = lpm->m23;

	lpmo->m24 = lpm->m42;
	lpmo->m42 = lpm->m24;

	lpmo->m34 = lpm->m43;
	lpmo->m43 = lpm->m34;

	lpmo->m11 = lpm->m11;
	lpmo->m22 = lpm->m22;
	lpmo->m33 = lpm->m33;
	lpmo->m44 = lpm->m44;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief Transpose一個矩陣的值。
 *
 *	@param lpmo transpose後的矩陣【輸出】。
 *	@param lpm 欲transpose的矩陣【輸入】。
 *	@return 無。 
 *	@see LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxTransposeMatrix2Matrix4x3(LPLYNXMATRIX4x3 lpmo, LPCLYNXMATRIX lpm)
{
	lpmo->m12 = lpm->m21;
	lpmo->m21 = lpm->m12;

	lpmo->m13 = lpm->m31;
	lpmo->m31 = lpm->m13;

	lpmo->m41 = lpm->m14;

	lpmo->m23 = lpm->m32;
	lpmo->m32 = lpm->m23;

	lpmo->m42 = lpm->m24;

	lpmo->m43 = lpm->m34;

	lpmo->m11 = lpm->m11;
	lpmo->m22 = lpm->m22;
	lpmo->m33 = lpm->m33;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief Transpose一個矩陣的值。
 *
 *	@param lpmo transpose後的矩陣【輸出】。
 *	@param lpm 欲transpose的矩陣【輸入】。
 *	@return 無。 
 *	@see LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxCopyMatrix2Matrix4x3(LPLYNXMATRIX4x3 lpmo, LPCLYNXMATRIX lpm)
{
	lpmo->m12 = lpm->m12;
	lpmo->m21 = lpm->m21;

	lpmo->m13 = lpm->m13;
	lpmo->m31 = lpm->m31;

	lpmo->m41 = lpm->m41;

	lpmo->m23 = lpm->m23;
	lpmo->m32 = lpm->m32;

	lpmo->m42 = lpm->m42;

	lpmo->m43 = lpm->m43;

	lpmo->m11 = lpm->m11;
	lpmo->m22 = lpm->m22;
	lpmo->m33 = lpm->m33;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief Transpose一個矩陣的值。
 *
 *	@param lpm 欲transpose的矩陣【輸出/輸入】。
 *	@return 無。 
 *	@see LYNXMATRIX 
 */
//-------------------------------------------------------------------------------------------------------
void lynxNormaliseMatrix(LPLYNXMATRIX lpm)
{
	LPLYNXVECTOR3D RightDir, UpDir, Direction;

	RightDir = (LPLYNXVECTOR3D)&lpm->m11;
	UpDir = (LPLYNXVECTOR3D)&lpm->m21;
	Direction = (LPLYNXVECTOR3D)&lpm->m31;
	lynxNormalise(Direction, 3);
	lynxNormalise(UpDir, 3);
	lynxNormalise(RightDir, 3);
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
/* void    lynxSlerpQuaternion(LPLYNXQUATERNION lpqout, LPLYNXQUATERNION lpqin1, LPLYNXQUATERNION lpqin2, float t)
{	
}*/
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 找出兩個向量間，最短距離的四元數。
 *
 *	@param lpq 求出的四元數【輸出】。
 *	@param lpv0 向量0【輸入】。
 *	@param lpv1 向量1【輸入】。
 *	@return 無。 
 *	@see LYNXQUATERNION, LYNXVECTOR3D
 */
//-------------------------------------------------------------------------------------------------------
void lynxShortestArcQuaternion(LPLYNXQUATERNION lpq, LPCLYNXVECTOR3D lpv0, LPCLYNXVECTOR3D lpv1)
{
	float						d, s;
	LYNXVECTOR3D				V0, V1, C;

	LYNX_VECTOR3D_COPY(V0, (*lpv0));
	LYNX_VECTOR3D_COPY(V1, (*lpv1));
	lynxNormalise(&V0, 3);
	lynxNormalise(&V1, 3);
	lynxCrossProduct(&C, &V0, &V1);
	d = LYNX_DOT_PRODUCT(V0, V1);
	s  = (float)lynxSqrt((1+d)*2);
	lpq->x = C.x/s;
	lpq->y = C.y/s;
	lpq->z = C.z/s;
	lpq->w = s/2.0f;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 四元樹相乘。
 *
 *	@param lpq 求出的四元數【輸出】。
 *	@param lpq1 四元數0【輸入】。
 *	@param lpq2 四元數1【輸入】。
 *	@return 無。 
 *	@see LYNXQUATERNION
 */
//-------------------------------------------------------------------------------------------------------
void lynxQuaternionXQuaternion(LPLYNXQUATERNION lpq, LPCLYNXQUATERNION lpq1, LPCLYNXQUATERNION lpq2)
{	
	LPLYNXVECTOR3D						lpV1;
	LPLYNXVECTOR3D						lpV2;
	LYNXVECTOR3D						V;

	/*
	D3DXQUATERNION							Q1, Q2, Q;

	Q1.x = lpq1->x;
	Q1.y = lpq1->y;
	Q1.z = lpq1->z;
	Q1.w = lpq1->w;

	Q2.x = lpq2->x;
	Q2.y = lpq2->y;
	Q2.z = lpq2->z;
	Q2.w = lpq2->w;
	D3DXQuaternionMultiply(&Q, &Q1, &Q2);
	
	lpq->x = Q.x;
	lpq->y = Q.y;
	lpq->z = Q.z;
	lpq->w = Q.w;									
	*/

	lpV1 = (LPLYNXVECTOR3D)&lpq1->x;
	lpV2 = (LPLYNXVECTOR3D)&lpq2->x;
	lynxCrossProduct(&V, lpV2, lpV1);
	lpq->w = lpq1->w*lpq2->w - LYNX_DOT_PRODUCT((*lpV1), (*lpV2));
	lpq->x = lpq1->w*lpV2->x+lpq2->w*lpV1->x+V.x;
	lpq->y = lpq1->w*lpV2->y+lpq2->w*lpV1->y+V.y;
	lpq->z = lpq1->w*lpV2->z+lpq2->w*lpV1->z+V.z;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 從旋轉軸向及旋轉角度求出對應的四元數四元數。
 *
 *	@param lpq 求出的四元數【輸出】。
 *	@param lpaxis 旋轉軸向【輸入】。
 *	@param ang 旋轉角度【輸入】。
 *	@return 無。 
 *	@see LYNXQUATERNION
 */
//-------------------------------------------------------------------------------------------------------
void lynxMakeQuaternion(LPLYNXQUATERNION lpq, LPCLYNXVECTOR3D lpaxis, const LYNXREAL ang)
{	
	LYNXREAL					r;

	#ifdef __FIXED_POINT__
		r = ang/2;
	#else
		r = ang * 0.5f;
	#endif
	lpq->x = lpaxis->x * lynxSin(r);
	lpq->y = lpaxis->y * lynxSin(r);
	lpq->z = lpaxis->z * lynxSin(r);
	lpq->w = lynxCos(r);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 從旋轉軸向及旋轉角度求出對應的旋轉矩陣。
 *
 *	@param lpm 求出的旋轉矩陣【輸出】。
 *	@param lpaxis 旋轉軸向【輸入】。
 *	@param ang 旋轉角度【輸入】。
 *	@return 無。 
 *	@see LYNXQUATERNION
 */
//-------------------------------------------------------------------------------------------------------
void lynxMakeRotateMatrixRespectVector(LPLYNXMATRIX lpm, LPCLYNXVECTOR3D lpaxis, const LYNXREAL ang)
{
	LYNXQUATERNION					Q;

	lynxMakeQuaternion(&Q, lpaxis, ang);
	lynxQuaternion2Matrix(lpm, &Q);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 從一個平面方程式求出對應此平面的反射矩陣。
 *
 *	@param lpm 求出的反射矩陣【輸出】。
 *	@param lpp 反射平面【輸入】。 
 *	@return 無。 
 *	@see LYNXMATRIX, LYNXPLANE
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetMatrixReflectToPlane(LPLYNXMATRIX lpm, LPCLYNXPLANE lpp)
{
	LYNXVECTOR3D					Pos, X, Y, Z, D;
	LYNXREAL						Dot1, Dot2, t;

	Dot2 = 1.0f/LYNX_DOT_PRODUCT(lpp->Normal, lpp->Normal);
	t = - lpp->Normal.x * Dot2;									
	X.x = 1.0f + 2*t*lpp->Normal.x;
	X.y = 0.0f;
	X.z = 0.0f;

	t = - lpp->Normal.y * Dot2;									
	Y.x = 0.0f;
	Y.y = 1.0f + 2*t*lpp->Normal.y;
	Y.z = 0.0f;

	t = - lpp->Normal.z * Dot2;									
	Z.x = 0.0f;
	Z.y = 0.0f;
	Z.z = 1.0f + 2*t*lpp->Normal.z;

	lynxNormalise(&X, 3);
	lynxNormalise(&Y, 3);
	lynxNormalise(&Z, 3);

	Pos.x = lpp->Normal.x*lpp->Distance;
	Pos.y = lpp->Normal.y*lpp->Distance;
	Pos.z = lpp->Normal.z*lpp->Distance;
	Dot1 = LYNX_DOT_PRODUCT(lpp->Normal, Pos);

	t = (lpp->Distance - Dot1) * Dot2;									

	D.x = Pos.x + 2*t*lpp->Normal.x;
	D.y = Pos.y + 2*t*lpp->Normal.y;
	D.z = Pos.z + 2*t*lpp->Normal.z;

	LYNX_MATRIX((*lpm),
                 X.x , X.y , X.z , 0.0f,
                 Y.x , Y.y , Y.z , 0.0f,
                 Z.x , Z.y , Z.z , 0.0f,				   
                 D.x*2 , D.y*2 , D.z*2 , 1.0f);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 
 *
 *	@param 
 *	@param 
 *	@return 
 *	@see LYNXMATRIX, LYNXPLANE
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetReflectionVector(LPLYNXVECTOR3D lpr, LPCLYNXVECTOR3D lpv, LPCLYNXVECTOR3D lpn)
{
	//R = 2*(V dot N)*N - V
	float Dot = lynxDotProduct(lpv, lpn) * 2.0f;
	LYNXVECTOR3D D;
	D.x = lpn->x * Dot;
	D.y = lpn->y * Dot;
	D.z = lpn->z * Dot;
	lpr->x = D.x - lpv->x;
	lpr->y = D.y - lpv->y;
	lpr->z = D.z - lpv->z;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 
 *
 *	@param lpmo
 *	@param lpm 
 *	@return 無。 
 *	@see LYNXMATRIX
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetViewMatrixFromTransformMatrix(LPLYNXMATRIX lpmo, LPCLYNXMATRIX lpm)
{
	lpmo->m11 = lpm->m11;
	lpmo->m12 = lpm->m21;
	lpmo->m13 = lpm->m31;
	lpmo->m14 = lpm->m14;

	lpmo->m21 = lpm->m12;
	lpmo->m22 = lpm->m22;
	lpmo->m23 = lpm->m32;
	lpmo->m24 = lpm->m24;

	lpmo->m31 = lpm->m13;
	lpmo->m32 = lpm->m23;
	lpmo->m33 = lpm->m33;
	lpmo->m34 = lpm->m34;

	lpmo->m41 = -(lpm->m11*lpm->m41 + lpm->m12*lpm->m42 + lpm->m13*lpm->m43);
	lpmo->m42 = -(lpm->m21*lpm->m41 + lpm->m22*lpm->m42 + lpm->m23*lpm->m43);
	lpmo->m43 = -(lpm->m31*lpm->m41 + lpm->m32*lpm->m42 + lpm->m33*lpm->m43);
	lpmo->m44 = lpm->m44;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */		            
//-------------------------------------------------------------------------------------------------------
void lynxPerspectiveProjectionMatrix(LPLYNXMATRIX lpm, const LYNXREAL near_plane, const LYNXREAL far_plane, const LYNXREAL fov, const LYNXREAL xyratio)
{	
	#ifdef __PS2__
		sceVu0FMATRIX			PM;
		float					Rad;
		
		Rad = fov*LYNXD2R/2;
		if (glpRenderer->FrameMode == LYNX_FRAME)
		{
			sceVu0ViewScreenMatrix(
			PM,
			glpRenderer->SystemData.CurrentVideoMode.Width*0.5f*(float)(cos(Rad)/sin(Rad)),
			1.0f,
			0.5f,
			2048.0f,
			2048.0f,
			1.0f,
			16777215.0f,
			near_plane,
			far_plane
			);
		}
		else
		{
			sceVu0ViewScreenMatrix(
			PM,
			glpRenderer->SystemData.CurrentVideoMode.Width*0.5f*(float)(cos(Rad)/sin(Rad)),
			1.0f,
			1.0f,
			2048.0f,
			2048.0f,
			1.0f,
			16777215.0f,
			near_plane,
			far_plane
			);
		}		
		lynxMemCpy(lpm, &PM, sizeof(LYNXMATRIX));
	#else				
		LYNXREAL					Q, QZ, w, h;
		LYNXREAL					Deg;

		/*
		Deg = fov/2;				
		w = LYNX_REAL_DIV((lynxQuickCos(Deg)), (lynxQuickSin(Deg)));				
		h = LYNX_REAL_MUL(xyratio, w);				
		Q = LYNX_REAL_DIV(far_plane, (LYNX_REAL_SUB(far_plane, near_plane)));				
		LYNX_MATRIX((*lpm),  w,	   0.0f, 0.0f,							  0.0f,
							 0.0f, h,    0.0f,							  0.0f,
							 0.0f, 0.0f, Q,							      1.0f,
							 0.0f, 0.0f, LYNX_REAL_MUL((-Q), near_plane), 0.0f ); */   
	
		/*
		// Left hand		
		Deg = fov/2;
		w = LYNX_REAL_DIV((lynxCos(Deg)), (lynxSin(Deg)));				
		h = LYNX_REAL_MUL(xyratio, w);				
		Q = LYNX_REAL_DIV((LYNX_REAL_ADD(far_plane, near_plane)),  (LYNX_REAL_SUB(far_plane, near_plane)));		
		QZ = LYNX_REAL_DIV((LYNX_REAL_MUL(far_plane, near_plane)), (LYNX_REAL_SUB(far_plane, near_plane)));		
		LYNX_MATRIX((*lpm),  w,	   0.0f, 0.0f,					   0.0f,
					         0.0f, h,    0.0f,					   0.0f,
					         0.0f, 0.0f, Q,				           1.0f,
					         0.0f, 0.0f, LYNX_REAL_MUL(-2.0f, QZ), 0.0f );*/

		
		Deg = fov/2;
		w = LYNX_REAL_DIV((lynxCos(Deg)), (lynxSin(Deg)));				
		h = LYNX_REAL_MUL(xyratio, w);				
		Q = LYNX_REAL_DIV(far_plane,  (LYNX_REAL_SUB(far_plane, near_plane)));		
		QZ = LYNX_REAL_DIV((LYNX_REAL_MUL(far_plane, near_plane)), (LYNX_REAL_SUB(far_plane, near_plane)));		

		LYNX_MATRIX((*lpm),  w,	   0.0f, 0.0f,	0.0f,
					         0.0f, h,    0.0f,	0.0f,
					         0.0f, 0.0f, Q,		1.0f,
					         0.0f, 0.0f, -QZ,	0.0f );		
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */		            
//-------------------------------------------------------------------------------------------------------
void lynxRHPerspectiveProjectionMatrix(LPLYNXMATRIX lpm, const LYNXREAL near_plane, const LYNXREAL far_plane, const LYNXREAL fov, const LYNXREAL xyratio)
{	
	#ifdef __PS2__		
	#else				
		LYNXREAL					Q, QZ, w, h;
		LYNXREAL					Deg;
		
		// Right hand
		Deg = fov/2;
		w = LYNX_REAL_DIV((lynxCos(Deg)), (lynxSin(Deg)));				
		h = LYNX_REAL_MUL(xyratio, w);				
		Q = far_plane / ( near_plane - far_plane);
		QZ = (far_plane *  near_plane) / ( near_plane - far_plane);
		LYNX_MATRIX((*lpm),  w,	   0.0f, 0.0f,	0.0f,
					         0.0f, h,    0.0f,	0.0f,
					         0.0f, 0.0f, Q,		-1.0f,
					         0.0f, 0.0f, QZ,	0.0f );		
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */		            
//-------------------------------------------------------------------------------------------------------
void lynxOrthoProjectionMatrix(LPLYNXMATRIX lpm, const LYNXREAL width, const LYNXREAL height, const LYNXREAL nearp, const LYNXREAL farp)
{	
	LYNXREAL			dX, dY, dZ;

	dX = LYNX_REAL_DIV(1.0f, width);
	dY = LYNX_REAL_DIV(1.0f, height);
	dZ = LYNX_REAL_DIV(1.0f, LYNX_REAL_SUB(farp, nearp));
	LYNX_MATRIX((*lpm),  LYNX_REAL_MUL(LYNX_REAL(2.0f), dX),	0.0f,								 0.0f,								 0.0f,
						 0.0f,									LYNX_REAL_MUL(LYNX_REAL(2.0f), dY),	 0.0f,								 0.0f,
						 0.0f,									0.0f,								 LYNX_REAL_MUL(LYNX_REAL(1.0f), dZ), 0.0f,
						 //0.0f,								 0.0f,								 LYNX_REAL_MUL(nearp, dZ),			 1.0f );	
						 0.0f,									0.0f,								 -LYNX_REAL_MUL(nearp, dZ),			 1.0f );	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */		            
//-------------------------------------------------------------------------------------------------------
void lynxOrthoProjectionMatrixOffset(LPLYNXMATRIX lpm, const LYNXREAL left, const LYNXREAL right, const LYNXREAL bottom, const LYNXREAL top, const LYNXREAL nearp, const LYNXREAL farp)
{	
	LYNXREAL			dX, dY, dZ;

	dX = LYNX_REAL_DIV(1.0f, LYNX_REAL_SUB(right, left));
	dY = LYNX_REAL_DIV(1.0f, LYNX_REAL_SUB(top, bottom));
	dZ = LYNX_REAL_DIV(1.0f, LYNX_REAL_SUB(farp, nearp));
	LYNX_MATRIX((*lpm),  LYNX_REAL_MUL(LYNX_REAL(2.0f), dX),				0.0f,											0.0f,											0.0f,
						 0.0f,												LYNX_REAL_MUL(LYNX_REAL(2.0f), dY),				0.0f,											0.0f,
						 0.0f,												0.0f,											LYNX_REAL_MUL(LYNX_REAL(1.0f), dZ),				0.0f,
						 -LYNX_REAL_MUL(LYNX_REAL_ADD(right, left), dX),	-LYNX_REAL_MUL(LYNX_REAL_ADD(top, bottom), dY), -LYNX_REAL_MUL(nearp, dZ),						1.0f );	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */		            
//-------------------------------------------------------------------------------------------------------
void lynxOrthoProjectionMatrixOffsetYDown(LPLYNXMATRIX lpm, const LYNXREAL left, const LYNXREAL right, const LYNXREAL bottom, const LYNXREAL top, const LYNXREAL nearp, const LYNXREAL farp)
{	
	LYNXREAL			dX, dY, dZ;

	dX = LYNX_REAL_DIV(1.0f, LYNX_REAL_SUB(right, left));
	dY = LYNX_REAL_DIV(1.0f, LYNX_REAL_SUB(top, bottom));
	dZ = LYNX_REAL_DIV(1.0f, LYNX_REAL_SUB(farp, nearp));
	LYNX_MATRIX((*lpm),  LYNX_REAL_MUL(LYNX_REAL(2.0f), dX),			 0.0f,								 0.0f,								 0.0f,
						 0.0f,								 -LYNX_REAL_MUL(LYNX_REAL(2.0f), dY),			 0.0f,								 0.0f,
						 0.0f,								 0.0f,								 LYNX_REAL_MUL(LYNX_REAL(-2.0f), dZ),			 0.0f,
						 -LYNX_REAL_MUL(LYNX_REAL_ADD(right, left), dX), LYNX_REAL_MUL(LYNX_REAL_ADD(top, bottom), dY), -LYNX_REAL_MUL(LYNX_REAL_ADD(farp, nearp), dZ), 1.0f );
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxMakeLookAtMatrix(LPLYNXMATRIX lpm, LPCLYNXVECTOR3D lpv)
{
	LYNXVECTOR3D			dv, dn, TN, TV, TU;
	//LYNXMATRIX			T;	
    float					f;	

	
    TN.x = lpv->x;
    TN.y = lpv->y;
    TN.z = lpv->z;
    lynxNormalise(&TN, 3);

    dv.x = TN.x; dv.y = TN.y + 1.0f; dv.z = TN.z;
    lynxNormalise(&dv, 3);
    f = LYNX_DOT_PRODUCT(dv, TN);
	if (f == 0.0f)
	{
		f = 0.1f;
	}
    dn.x = f * TN.x;
	dn.y = f * TN.y;
	dn.z = f * TN.z;
    TV.x = dv.x - dn.x;
	TV.y = dv.y - dn.y;
	TV.z = dv.z - dn.z;
    lynxNormalise(&TV, 3);
    lynxCrossProduct(&TU, &TV, &TN);		

	
	LYNX_MATRIX((*lpm),
			  	 TU.x , TU.y , TU.z , 0.0f,
				 TV.x , TV.y , TV.z , 0.0f,
				 TN.x , TN.y , TN.z , 0.0f,
				 0.0f , 0.0f , 0.0f , 1.0f);			
						
						
	//LYNX_MAKE_MATRIX((*lpm),
	//					TU.x , TV.x , TN.x , 0.0f,
	//					TU.y , TV.y , TN.y , 0.0f,
	//					TU.z , TV.z , TN.z , 0.0f,
	//					0.0f , 0.0f , 0.0f , 1.0f);									
						
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxPointInsidePlane(LPLYNXVECTOR3D lpp, LPCLYNXPLANE lpplane)
{
	LYNXREAL					CurrentDot;

	CurrentDot = LYNX_DOT_PRODUCT((*lpp), lpplane->Normal);
	if (CurrentDot >= lpplane->Distance)
	{
		return TRUE;
	}	
	return FALSE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxPointInsidePlaneWithRadius(LPLYNXVECTOR3D lpp, LPCLYNXPLANE lpplane, LYNXREAL r)
{
	LYNXREAL					CurrentDot;

	CurrentDot = LYNX_DOT_PRODUCT((*lpp), lpplane->Normal);
	if (CurrentDot >= (lpplane->Distance + r))
	{
		return TRUE;
	}	
	return FALSE;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetupTriangleBoundary(LPLYNXTRIANGLEBOUNDARY lptb, LPCLYNXVECTOR3D lpv)
{
	int								i;
	LYNXVECTOR3D					V1, V2;

	LYNX_VECTOR3D_SUB(V1, lpv[1], lpv[0]);
	LYNX_VECTOR3D_SUB(V2, lpv[2], lpv[0]);

	lynxCrossProduct(&lptb->Plane.Normal, &V1, &V2);
	lynxNormalise(&lptb->Plane.Normal, 3);	
	lptb->Plane.Distance = (lpv[0].x*lptb->Plane.Normal.x) + (lpv[0].y*lptb->Plane.Normal.y) + (lpv[0].z*lptb->Plane.Normal.z); 															

	for (i=0; i<3; i++)
	{
		V1.x = lpv[(i+1)%3].x - lpv[i].x;
		V1.y = lpv[(i+1)%3].y - lpv[i].y;
		V1.z = lpv[(i+1)%3].z - lpv[i].z;		

		lptb->lpEdgePlane[i].Normal.x = (lptb->Plane.Normal.y*V1.z) - (lptb->Plane.Normal.z*V1.y);
		lptb->lpEdgePlane[i].Normal.y = (lptb->Plane.Normal.z*V1.x) - (lptb->Plane.Normal.x*V1.z);
		lptb->lpEdgePlane[i].Normal.z = (lptb->Plane.Normal.x*V1.y) - (lptb->Plane.Normal.y*V1.x);
		lynxNormalise(&lptb->lpEdgePlane[i].Normal, 3);
		lptb->lpEdgePlane[i].Distance = lpv[i].x*lptb->lpEdgePlane[i].Normal.x +
									    lpv[i].y*lptb->lpEdgePlane[i].Normal.y +
									    lpv[i].z*lptb->lpEdgePlane[i].Normal.z;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxLineInsertTriangleBoundary(LPLYNXVECTOR3D lppos, LPCLYNXTRIANGLEBOUNDARY lptb, LPCLYNXVECTOR3D lpv1, LPCLYNXVECTOR3D lpv2)
{
	int						i;
	LYNXBOOL				bCollide;
	float					Dot, Dot1, Dot2, t;
	LYNXVECTOR3D			Dir;

	bCollide = FALSE;
	Dot1 = LYNX_DOT_PRODUCT(lptb->Plane.Normal, (*lpv1)) - lptb->Plane.Distance;
	Dot2 = LYNX_DOT_PRODUCT(lptb->Plane.Normal, (*lpv2)) - lptb->Plane.Distance;
	if (Dot2*Dot1 <= 0)
	{
		LYNX_VECTOR3D_SUB(Dir, (*lpv2), (*lpv1));
		Dot1 = LYNX_DOT_PRODUCT(lptb->Plane.Normal, (*lpv1));
		Dot2 = LYNX_DOT_PRODUCT(lptb->Plane.Normal, Dir);
		if (Dot2 != 0.0f)
		{
			t = (lptb->Plane.Distance - Dot1)/Dot2;
			if (t >= 0.0f && t <= 1.0f)
			{
				lppos->x = lpv1->x + t*Dir.x; 
				lppos->y = lpv1->y + t*Dir.y; 
				lppos->z = lpv1->z + t*Dir.z; 
				bCollide = TRUE;
				for (i=0; i<3; i++)
				{
					Dot = LYNX_DOT_PRODUCT(lptb->lpEdgePlane[i].Normal, (*lppos));
					if (Dot < lptb->lpEdgePlane[i].Distance)
					{
						bCollide = FALSE;
						break;
					}
				}		
			}
		}		
	}
	return bCollide;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 。
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetupPlaneBoundary(LPLYNXPLANEBOUNDARY lppb, LPCLYNXVECTOR3D lpv, const int numv)
{
	int								i;
	LYNXVECTOR3D					V1, V2;

	LYNX_VECTOR3D_SUB(V1, lpv[1], lpv[0]);
	LYNX_VECTOR3D_SUB(V2, lpv[2], lpv[0]);

	lynxCrossProduct(&lppb->Plane.Normal, &V1, &V2);
	lynxNormalise(&lppb->Plane.Normal, 3);	
	lppb->Plane.Distance = (lpv[0].x*lppb->Plane.Normal.x) + (lpv[0].y*lppb->Plane.Normal.y) + (lpv[0].z*lppb->Plane.Normal.z);
	
	for (i=0; i<numv; i++)
	{
		V1.x = lpv[(i+1)%numv].x - lpv[i].x;
		V1.y = lpv[(i+1)%numv].y - lpv[i].y;
		V1.z = lpv[(i+1)%numv].z - lpv[i].z;		

		lppb->lpEdgePlane[i].Normal.x = (lppb->Plane.Normal.y*V1.z) - (lppb->Plane.Normal.z*V1.y);
		lppb->lpEdgePlane[i].Normal.y = (lppb->Plane.Normal.z*V1.x) - (lppb->Plane.Normal.x*V1.z);
		lppb->lpEdgePlane[i].Normal.z = (lppb->Plane.Normal.x*V1.y) - (lppb->Plane.Normal.y*V1.x);
		lynxNormalise(&lppb->lpEdgePlane[i].Normal, 3);
		lppb->lpEdgePlane[i].Distance = lpv[i].x*lppb->lpEdgePlane[i].Normal.x +
									    lpv[i].y*lppb->lpEdgePlane[i].Normal.y +
									    lpv[i].z*lppb->lpEdgePlane[i].Normal.z;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief	If line (v1 to v2, unlimited length) insert boundary
 *
 *	@param 。
 *	@param 。  
 *	@return 。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxLineInsertPlaneBoundary(LPLYNXVECTOR3D lppos, LPCLYNXPLANEBOUNDARY lppb, LPCLYNXVECTOR3D lpv1, LPCLYNXVECTOR3D lpv2)
{
	int							i;
	LYNXBOOL					bCollide;
	float						Dot, Dot1, Dot2, t;
	LYNXVECTOR3D				Dir;

	*lppos = *lpv2;
	bCollide = LYNX_FALSE;
	Dot1 = LYNX_DOT_PRODUCT(lppb->Plane.Normal, (*lpv1)) - lppb->Plane.Distance;
	Dot2 = LYNX_DOT_PRODUCT(lppb->Plane.Normal, (*lpv2)) - lppb->Plane.Distance;
	if (Dot2*Dot1 <= 0)
	{
		bCollide = LYNX_TRUE;
		LYNX_VECTOR3D_SUB(Dir, (*lpv2), (*lpv1));
		Dot1 = LYNX_DOT_PRODUCT(lppb->Plane.Normal, (*lpv1));
		Dot2 = LYNX_DOT_PRODUCT(lppb->Plane.Normal, Dir);
		//if (Dot2 != 0.0f)
		{
			t = (lppb->Plane.Distance - Dot1)/Dot2;
			//if (t >= 0.0f && t <= 1.0f)
			{
				lppos->x = lpv1->x + t*Dir.x; 
				lppos->y = lpv1->y + t*Dir.y; 
				lppos->z = lpv1->z + t*Dir.z; 				

				/*
				for (i=0; i<lppb->NumVerts; i++)
				{
					Dot = LYNX_DOT_PRODUCT(lppb->lpEdgePlane[i].Normal, (*lppos));
					if (Dot < lppb->lpEdgePlane[i].Distance)
					{
						bCollide = LYNX_FALSE;
						return bCollide;
					}
				}
				*/
			}
		}
	}
	
	return bCollide;
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxMath
*  @brief	If line segment (v1 ~ v2) insert boundary
*
*	@param 。
*	@param 。
*	@return 。
*/
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxLineSegmentInsertPlaneBoundary(LPLYNXVECTOR3D lppos, LPCLYNXPLANEBOUNDARY lppb, LPCLYNXVECTOR3D lpv1, LPCLYNXVECTOR3D lpv2)
{
	int							i;
	LYNXBOOL					bCollide;
	float						Dot, Dot1, Dot2, t;
	LYNXVECTOR3D				Dir;

	*lppos = *lpv2;
	bCollide = LYNX_FALSE;
	Dot1 = LYNX_DOT_PRODUCT(lppb->Plane.Normal, (*lpv1)) - lppb->Plane.Distance;
	Dot2 = LYNX_DOT_PRODUCT(lppb->Plane.Normal, (*lpv2)) - lppb->Plane.Distance;
	if (Dot2*Dot1 <= 0)
	{		
		LYNX_VECTOR3D_SUB(Dir, (*lpv2), (*lpv1));
		Dot1 = LYNX_DOT_PRODUCT(lppb->Plane.Normal, (*lpv1));
		Dot2 = LYNX_DOT_PRODUCT(lppb->Plane.Normal, Dir);
		//if (Dot2 != 0.0f)
		{
			t = (lppb->Plane.Distance - Dot1) / Dot2;
			if (t >= 0.0f && t <= 1.0f)
			{
				bCollide = LYNX_TRUE;
			}

			lppos->x = lpv1->x + t*Dir.x;
			lppos->y = lpv1->y + t*Dir.y;
			lppos->z = lpv1->z + t*Dir.z;			
		}
	}

	return bCollide;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 由兩個端點求出線的資料。
 *
 *	@param lppn 求得之線的指標【輸出】。
 *	@param lpp1 點的指標【輸入】。
 *	@param lpp2 線的指標【輸入】。  
 *	@return 無。
 *	@see LYNXVECTOR3D, LYNXLINE3D 
 */   
//-------------------------------------------------------------------------------------------------------
void lynxSetupLine3D(LPLYNXLINE3D lpn, LPCLYNXVECTOR3D lpp1, LPCLYNXVECTOR3D lpp2)
{
	LYNX_VECTOR3D((lpn->Origin), lpp1->x, lpp1->y, lpp1->z);
	LYNX_VECTOR3D_SUB((lpn->Dir), (*lpp2), (*lpp1));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求點到線的距離平方。
 *
 *	@param lpp 點的指標【輸入】。
 *	@param lpn 線的指標【輸入】。  
 *	@return 點到線的距離平方。
 *	@see LYNXVECTOR3D, LYNXLINE3D 
 */   
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxSqrDistancePoint2Line(LPCLYNXVECTOR3D lpp, LPCLYNXLINE3D lpn)
{
	LYNXVECTOR3D			Diff;
	float					Base, t;

	LYNX_VECTOR3D_SUB(Diff, (*lpp), (lpn->Origin));
	Base = LYNX_DOT_PRODUCT((lpn->Dir), (lpn->Dir));
	t = LYNX_DOT_PRODUCT(Diff, (lpn->Dir))/Base;
	Diff.x -= t*lpn->Dir.x;
	Diff.y -= t*lpn->Dir.y;
	Diff.z -= t*lpn->Dir.z;
	return (LYNX_DOT_PRODUCT(Diff, Diff));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求點到線的距離。
 *
 *	@param lpp 點的指標【輸入】。
 *	@param lpn 線的指標【輸入】。  
 *	@return 點到線的距離。
 *	@see LYNXVECTOR3D, LYNXLINE3D 
 */   
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxDistancePoint2Line(LPCLYNXVECTOR3D lpp, LPCLYNXLINE3D lpn)
{
	LYNXVECTOR3D			Diff;
	float					Base, t;

	LYNX_VECTOR3D_SUB(Diff, (*lpp), (lpn->Origin));
	Base = LYNX_DOT_PRODUCT((lpn->Dir), (lpn->Dir));
	t = LYNX_DOT_PRODUCT(Diff, (lpn->Dir))/Base;
	Diff.x -= t*lpn->Dir.x;
	Diff.y -= t*lpn->Dir.y;
	Diff.z -= t*lpn->Dir.z;
	return (lynxLength(&Diff, 3));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求點到線段的距離。
 *
 *	@param lpp 點段的指標【輸入】。
 *	@param lpn 線的指標【輸入】。  
 *	@return 點到線段的距離。
 *	@see LYNXVECTOR3D, LYNXLINE3D 
 */   
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxDistancePoint2Segment(LPCLYNXVECTOR3D lpp, LPCLYNXLINE3D lpn)
{
	LYNXVECTOR3D			Diff;
	LYNXREAL				t, SquaredLen;

	LYNX_VECTOR3D_SUB(Diff, (*lpp), (lpn->Origin));	
	t = LYNX_DOT_PRODUCT(Diff, (lpn->Dir));
	if (t <= LYNX_REAL(0))
	{
		t = LYNX_REAL(0);
	}
	else
	{
		SquaredLen = LYNX_DOT_PRODUCT((lpn->Dir), (lpn->Dir));		
		if (t >= SquaredLen)
		{
			t = LYNX_REAL(1);
			LYNX_VECTOR3D_SUB(Diff, Diff, (lpn->Dir));	
		}
		else
		{
			t = LYNX_REAL_DIV(t, SquaredLen);
			Diff.x -= LYNX_REAL_MUL(t, lpn->Dir.x);
			Diff.y -= LYNX_REAL_MUL(t, lpn->Dir.y);
			Diff.z -= LYNX_REAL_MUL(t, lpn->Dir.z);
		}
	}
	return (lynxLength(&Diff, 3));
}	
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求點到線段的距離。
 *
 *	@param lpp 點段的指標【輸入】。
 *	@param lpn 線的指標【輸入】。  
 *	@return 點到線段的距離。
 *	@see LYNXVECTOR3D, LYNXLINE3D 
 */   
//-------------------------------------------------------------------------------------------------------
LYNXREAL lynxSqrDistancePoint2Segment(LPCLYNXVECTOR3D lpp, LPCLYNXLINE3D lpn)
{
	LYNXVECTOR3D			Diff;
	LYNXREAL				t, SquaredLen;

	LYNX_VECTOR3D_SUB(Diff, (*lpp), (lpn->Origin));	
	t = LYNX_DOT_PRODUCT(Diff, (lpn->Dir));
	if (t <= LYNX_REAL(0))
	{
		t = LYNX_REAL(0);
	}
	else
	{
		SquaredLen = LYNX_DOT_PRODUCT((lpn->Dir), (lpn->Dir));		
		if (t >= SquaredLen)
		{
			t = LYNX_REAL(1);
			LYNX_VECTOR3D_SUB(Diff, Diff, (lpn->Dir));	
		}
		else
		{
			t = LYNX_REAL_DIV(t, SquaredLen);
			Diff.x -= LYNX_REAL_MUL(t, lpn->Dir.x);
			Diff.y -= LYNX_REAL_MUL(t, lpn->Dir.y);
			Diff.z -= LYNX_REAL_MUL(t, lpn->Dir.z);
		}
	}
	return (LYNX_DOT_PRODUCT(Diff, Diff));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求線到線的距離平方。
 *
 *	@param lpn0 線的指標【輸入】。
 *	@param lpn1 線的指標【輸入】。  
 *	@return 線到線的距離平方。
 *	@see LYNXLINE3D 
 */   
//-------------------------------------------------------------------------------------------------------
float lynxSqrDistanceLine2Line(LPCLYNXLINE3D lpn0, LPCLYNXLINE3D lpn1)
{
	LYNXVECTOR3D		Diff;
	float				fA00, fA01, fA11, fB0, fC, fDet, fB1, fS, fT, fSqrDist;
	float				fInvDet;

	LYNX_VECTOR3D_SUB(Diff, (lpn0->Origin), (lpn1->Origin));
	fA00 = LYNX_DOT_PRODUCT(lpn0->Dir, lpn0->Dir);
    fA01 = -LYNX_DOT_PRODUCT(lpn0->Dir, lpn1->Dir);
    fA11 = LYNX_DOT_PRODUCT(lpn1->Dir, lpn1->Dir);
    fB0 = LYNX_DOT_PRODUCT(lpn0->Dir, Diff);
    fC = LYNX_DOT_PRODUCT(Diff, Diff);
    fDet = LYNX_ABS((fA00*fA11-fA01*fA01));    

    if (fDet >= LYNX_FLOAT_TOLERANCE)
    {
        // lines are not parallel
        fB1 = -LYNX_DOT_PRODUCT(Diff, (lpn1->Dir));
        fInvDet = 1.0/fDet;
        fS = (fA01*fB1-fA11*fB0)*fInvDet;
        fT = (fA01*fB0-fA00*fB1)*fInvDet;
        fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) + fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
    }
    else
    {
        // lines are parallel, select any closest pair of points
        fS = -fB0/fA00;
        fT = 0.0;
        fSqrDist = fB0*fS+fC;
    }
    return (LYNX_ABS(fSqrDist));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMath 
 *  @brief 求線到線的距離。
 *
 *	@param lpn0 線的指標【輸入】。
 *	@param lpn1 線的指標【輸入】。  
 *	@return 線到線的距離。
 *	@see LYNXLINE3D 
 */   
//-------------------------------------------------------------------------------------------------------
float lynxDistanceLine2Line(LPCLYNXLINE3D lpn0, LPCLYNXLINE3D lpn1)
{
	LYNXVECTOR3D		Diff;
	float				fA00, fA01, fA11, fB0, fC, fDet, fB1, fS, fT, fSqrDist;
	float				fInvDet;

	LYNX_VECTOR3D_SUB(Diff, (lpn0->Origin), (lpn1->Origin));
	fA00 = LYNX_DOT_PRODUCT(lpn0->Dir, lpn0->Dir);
    fA01 = -LYNX_DOT_PRODUCT(lpn0->Dir, lpn1->Dir);
    fA11 = LYNX_DOT_PRODUCT(lpn1->Dir, lpn1->Dir);
    fB0 = LYNX_DOT_PRODUCT(lpn0->Dir, Diff);
    fC = LYNX_DOT_PRODUCT(Diff, Diff);
    fDet = LYNX_ABS((fA00*fA11-fA01*fA01));    

    if (fDet >= LYNX_FLOAT_TOLERANCE)
    {
        // lines are not parallel
        fB1 = -LYNX_DOT_PRODUCT(Diff, (lpn1->Dir));
        fInvDet = 1.0/fDet;
        fS = (fA01*fB1-fA11*fB0)*fInvDet;
        fT = (fA01*fB0-fA00*fB1)*fInvDet;
        fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) + fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
    }
    else
    {
        // lines are parallel, select any closest pair of points
        fS = -fB0/fA00;
        fT = 0.0;
        fSqrDist = fB0*fS+fC;
    }
    return (lynxSqrt((LYNX_ABS(fSqrDist))));
}
	
#define mi(w, t, i, j) 4 * ((i * w + j) * (1-t) + (j * w + i) * t)
// Load & multiply.
#define flm(k, i, j, m, n, a, b) \
		__asm fld dword ptr [edx + mi(m, a, i, k)] \
		__asm fmul dword ptr [ecx + mi(n, b, k, j)]

// Load, multiply & add.
#define e4(i, j, l, m, n, a, b) \
		flm(0, i, j, m, n, a, b) \
		flm(1, i, j, m, n, a, b) \
		flm(2, i, j, m, n, a, b) \
		flm(3, i, j, m, n, a, b) \
		__asm faddp st(1), st(0) \
		__asm fxch st(2) \
		__asm faddp st(1), st(0) \
		__asm faddp st(1), st(0) \
		__asm fstp dword ptr [eax + mi(l, 0, i, j)]
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
/*
	void lynxMatrixXMatrix(float *dst, float *src1, float *src2)
{
	__asm mov edx, DWORD PTR src1
	__asm mov ecx, DWORD PTR src2
	__asm mov eax, DWORD PTR dst
	e4(0, 0, 4, 4, 4, 0, 0)
	e4(0, 1, 4, 4, 4, 0, 0)
	e4(0, 2, 4, 4, 4, 0, 0)
	e4(0, 3, 4, 4, 4, 0, 0)
	e4(1, 0, 4, 4, 4, 0, 0)
	e4(1, 1, 4, 4, 4, 0, 0)
	e4(1, 2, 4, 4, 4, 0, 0)
	e4(1, 3, 4, 4, 4, 0, 0)
	e4(2, 0, 4, 4, 4, 0, 0)
	e4(2, 1, 4, 4, 4, 0, 0)
	e4(2, 2, 4, 4, 4, 0, 0)
	e4(2, 3, 4, 4, 4, 0, 0)
	e4(3, 0, 4, 4, 4, 0, 0)
	e4(3, 1, 4, 4, 4, 0, 0)
	e4(3, 2, 4, 4, 4, 0, 0)
	e4(3, 3, 4, 4, 4, 0, 0)
}
*/

/*
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
  void lynxVertex3DXMatrixNoColor(LPLYNXVERTEX3D lpb, LPLYNXVERTEX3D lpa, LPLYNXMATRIX lpm)
{
	LYNXVERTEX3D   V;

    V.x = lpa->x*lpm->m11 + lpa->y*lpm->m21 + lpa->z*lpm->m31 + lpm->m41;
    V.y = lpa->x*lpm->m12 + lpa->y*lpm->m22 + lpa->z*lpm->m32 + lpm->m42;
    V.z = lpa->x*lpm->m13 + lpa->y*lpm->m23 + lpa->z*lpm->m33 + lpm->m43;

	V.nx = lpa->nx*lpm->m11 + lpa->ny*lpm->m21 + lpa->nz*lpm->m31;
    V.ny = lpa->nx*lpm->m12 + lpa->ny*lpm->m22 + lpa->nz*lpm->m32;
    V.nz = lpa->nx*lpm->m13 + lpa->ny*lpm->m23 + lpa->nz*lpm->m33;

	V.bLighting = lpa->bLighting;
	V.TexCoord[0].u = lpa->TexCoord[0].u;
	V.TexCoord[0].v = lpa->TexCoord[0].v;
    lynxAlignMemCpy(lpb, &V, sizeof(LYNXVERTEX3D));

    return;  
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
  void lynxSequenceVertex3DXMatrix(LPLYNXVERTEX3D lpb, LPLYNXVERTEX3D lpa, LPLYNXMATRIX lpm, int num)
{
	int					i;
	LPLYNXVERTEX3D		B = lpb;
	LPLYNXVERTEX3D		A = lpa;

	for(i=num; i>0; i--)
	{
		B->x = A->x*lpm->m11 + A->y*lpm->m21 + A->z*lpm->m31 + lpm->m41;
		B->y = A->x*lpm->m12 + A->y*lpm->m22 + A->z*lpm->m32 + lpm->m42;
		B->z = A->x*lpm->m13 + A->y*lpm->m23 + A->z*lpm->m33 + lpm->m43;
	
		B->nx = A->nx*lpm->m11 + A->ny*lpm->m21 + A->nz*lpm->m31;
		B->ny = A->nx*lpm->m12 + A->ny*lpm->m22 + A->nz*lpm->m32;
		B->nz = A->nx*lpm->m13 + A->ny*lpm->m23 + A->nz*lpm->m33;
	
		B->TexCoord[0].u = A->TexCoord[0].u;
		B->TexCoord[0].v = A->TexCoord[0].v;	

		B->Color.PackedColor = A->Color.PackedColor;
		B->bLighting = A->bLighting;

		A++;
		B++;
	}
    return;  
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
  void lynxSequenceVertex3DXMatrixNoColor(LPLYNXVERTEX3D lpb, LPLYNXVERTEX3D lpa, LPLYNXMATRIX lpm, int num)
{
	int					i;
	LPLYNXVERTEX3D		B = lpb;
	LPLYNXVERTEX3D		A = lpa;

	for(i=num; i>0; i--)
	{
		B->x = A->x*lpm->m11 + A->y*lpm->m21 + A->z*lpm->m31 + lpm->m41;
		B->y = A->x*lpm->m12 + A->y*lpm->m22 + A->z*lpm->m32 + lpm->m42;
		B->z = A->x*lpm->m13 + A->y*lpm->m23 + A->z*lpm->m33 + lpm->m43;
	
		B->nx = A->nx*lpm->m11 + A->ny*lpm->m21 + A->nz*lpm->m31;
		B->ny = A->nx*lpm->m12 + A->ny*lpm->m22 + A->nz*lpm->m32;
		B->nz = A->nx*lpm->m13 + A->ny*lpm->m23 + A->nz*lpm->m33;
	
		B->TexCoord[0].u = A->TexCoord[0].u;
		B->TexCoord[0].v = A->TexCoord[0].v;	
		
		B->bLighting = A->bLighting;

		A++;
		B++;
	}
    return;  
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
  void lynxSequenceVertex3DXMatrixNoColorNoUV(LPLYNXVERTEX3D lpb, LPLYNXVERTEX3D lpa, LPLYNXMATRIX lpm, int num)
{
	int					i;
	LPLYNXVERTEX3D		B = lpb;
	LPLYNXVERTEX3D		A = lpa;
	
	for(i=0; i<num; i++)
	{
		lpb[i].x = lpa[i].x*lpm->m11 + lpa[i].y*lpm->m21 + lpa[i].z*lpm->m31 + lpm->m41;
		lpb[i].y = lpa[i].x*lpm->m12 + lpa[i].y*lpm->m22 + lpa[i].z*lpm->m32 + lpm->m42;
		lpb[i].z = lpa[i].x*lpm->m13 + lpa[i].y*lpm->m23 + lpa[i].z*lpm->m33 + lpm->m43;

		lpb[i].nx = lpa[i].nx*lpm->m11 + lpa[i].ny*lpm->m21 + lpa[i].nz*lpm->m31;
		lpb[i].ny = lpa[i].nx*lpm->m12 + lpa[i].ny*lpm->m22 + lpa[i].nz*lpm->m32;
		lpb[i].nz = lpa[i].nx*lpm->m13 + lpa[i].ny*lpm->m23 + lpa[i].nz*lpm->m33;		
	}
    return;  
}
*/