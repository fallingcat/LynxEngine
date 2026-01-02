//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studio All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2003/06/22
//  Last Update : 
//
//###########################################################################

#ifndef	__LYNXMATH_H__
#define	__LYNXMATH_H__

//********************************** Lynx Math **************************************************************
/**
 *  @defgroup LynxMath <Mathematics module>
 *
 *	Mathematics related function Including vector/matrix operation 
 */
//************************************************************************************************************
#include <LynxCPU.h>
#include <LynxAssert.h>
#include <math.h>
#include <float.h>

#ifdef __WIN32__
	#if (defined __X86__ || defined __X64__)
		#include <xmmintrin.h>
	#endif
#endif

#define	LYNX_X_AXIS											0x01
#define	LYNX_Y_AXIS											0x02 
#define	LYNX_Z_AXIS											0x04
typedef WORD												LYNXAXISTYPE;

typedef enum {
	LYNX_MATH_PRE = 0,
	LYNX_MATH_POST, 
}LYNXMATHORDER;

#define	LYNXPI												(3.14159265358979323846f)
#define LYNXIPI												(1.0f / LYNXPI)
#define LYNXD2R												(LYNXPI / 180.0f)
#define LYNXR2D												(180.0f / LYNXPI)
#define LYNX_FLOAT_TOLERANCE								1e-06f
#define LYNX_DEGREE_TO_RAD(d)								(d*LYNXD2R)
#define LYNX_RAD_TO_DEGREE(r)								(r*180.0f*LYNXIPI)
#define LYNX_ROUND(a)										((int)((a)+0.5f))
#define LYNX_CEIL(a)										((int)(ceil((a)))) //((int)((a)+(1.0f-LYNX_FLOAT_TOLERANCE)))
#define LYNX_FLOOR(a)										((int)(floor((a)))) //((int)(a))
#define LYNX_MIN(a, b)										(a < b) ? a : b
#define LYNX_MAX(a, b)										(a > b) ? a : b
#define LYNX_CLAMP(a, b, c)									LYNX_MIN((LYNX_MAX(a, b)), c)
#define LYNX_ABS(f)											(f < 0) ? (-f) : f
#define LYNX_FABS(f)										(fabs(f))
#define LYNX_STEP(f)                                        ((f >= 0) ? 1 : -1)
#define IS_NAN(f)											(_isnan(f))

#define	LYNX_TEST_NEAR										1
#define	LYNX_TEST_FAR										2
#define	LYNX_TEST_LEFT										4
#define	LYNX_TEST_RIGHT										8
#define	LYNX_TEST_TOP										16
#define	LYNX_TEST_BOTTOM									32
#define LYNX_BACK_CLIP										64
#define	LYNX_3D_CLIP										128
#define LYNX_CLIPPLANE(n)									(1<<n)
/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXVECTOR2D
 *  @brief 一般使用之2D向量指標。 
 *
 *	@see LYNXVECTOR2D
 */
///**
// *  @ingroup LynxMath 
// *  @brief 一般使用之2D向量。

// *
// *  @see LPLYNXVECTOR2D
// */
LYNX_CACHE_ALIGN typedef struct {
	union {
		struct {
			LYNXREAL		x;	/*!< x 軸。 */  	
			LYNXREAL		y;	/*!< y 軸。 */  	
		};
		LYNXREAL v[2];	
	};
}LYNXVECTOR2D LYNXALIGNED(16), *LPLYNXVECTOR2D;
typedef const LYNXVECTOR2D* LPCLYNXVECTOR2D;

/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXVECTOR3D
 *  @brief 一般使用之3D向量指標。 
 *
 *	@see LYNXVECTOR3D
 */
///**
// *  @ingroup LynxMath 
// *  @brief 一般使用之3D向量。

// *
// *  @see LPLYNXVECTOR3D
// */
LYNX_CACHE_ALIGN typedef struct {
	union {
		struct {
			LYNXREAL		x;	/*!< x 軸。 */  	
			LYNXREAL		y;	/*!< y 軸。 */  	
			LYNXREAL		z;	/*!< z 軸。 */  	
		};
		struct {
			LYNXREAL		r;	 	
			LYNXREAL		g;	 	
			LYNXREAL		b;	 	
		};
		LYNXREAL v[3];	
	};
}LYNXVECTOR3D LYNXALIGNED(16), *LPLYNXVECTOR3D;
typedef const LYNXVECTOR3D* LPCLYNXVECTOR3D;

/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXVECTOR4D
 *  @brief 一般使用之4D向量指標。
 *
 *	LPLYNXVECTOR4D :: detail。
 *
 *  @see LYNXVECTOR4D
 */
/**
 *  @ingroup LynxMath 
 *  @brief 一般使用之4D向量。
 *
 *	<div class="fragment">
 *	<pre>	
 *	typedef	struct {	
 *		union {
 *			struct {
 *				LYNXREAL		x;		
 *				LYNXREAL		y;		
 *				LYNXREAL		z;		
 *				LYNXREAL		w;		
 *			}s;
 *			LYNXREAL v[4];
 *		};
 *	}LYNXVECTOR4D LYNXALIGNED(16),* LPLYNXVECTOR4D;
 *	</pre>
 *  </div>
 *
 *  @see LPLYNXVECTOR4D
 */
LYNX_CACHE_ALIGN typedef struct {
	union {
		struct {
			LYNXREAL		x;	/*!< x 軸。 */  	
			LYNXREAL		y;	/*!< y 軸。 */  	
			LYNXREAL		z;	/*!< z 軸。 */  	
			LYNXREAL		w;	/*!< w 軸。 */  	
		};
		struct {
			LYNXREAL		r;		
			LYNXREAL		g;		
			LYNXREAL		b;		
			LYNXREAL		a;		
		};
		LYNXREAL v[4];	
	};
}LYNXVECTOR4D LYNXALIGNED(16), *LPLYNXVECTOR4D;
typedef const LYNXVECTOR4D* LPCLYNXVECTOR4D;

#ifdef __PS2__
	#define	LYNXVECTOR3D							LYNXVECTOR4D
	#define	LPLYNXVECTOR3D							LPLYNXVECTOR4D	
	#define	LPCLYNXVECTOR3D							LPCLYNXVECTOR4D
#endif

/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXMATRIX
 *  @brief 3x3矩陣指標。
 *
 *	LPLYNXMATRIX :: detail。
 *
 *  @see LYNXMATRIX
 */
/**
 *  @ingroup LynxMath 
 *  @brief 4x4矩陣。
 *
 *	<div class="fragment">
 *	<pre>	
 *	typedef	 struct {
 *		union {
 *			struct {
 *				LYNXREAL			m11, m12, m13;			
 *				LYNXREAL			m21, m22, m23;			
 *				LYNXREAL			m31, m32, m33;							
 *			}s;
 *			LYNXREAL m[3][3];
 *		};
 *	}LYNXMATRIX3 LYNXALIGNED(16), * LPLYNXMATRIX3;	
 *	</pre>
 *  </div> 
 */
//*******************this structure can't be modified, will cause read old model file crash**********
typedef	struct {
	union {
		struct {
			LYNXREAL		m11, m12, m13;			
			LYNXREAL		m21, m22, m23;			
			LYNXREAL		m31, m32, m33;					
		};
		LYNXREAL		m[3][3];	
		LYNXREAL		f[9];
	};
}LYNXMATRIX3 LYNXALIGNED(16), * LPLYNXMATRIX3;	
typedef const LYNXMATRIX3* LPCLYNXMATRIX3;

/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXMATRIX
 *  @brief 3x3矩陣指標。
 *
 *	LPLYNXMATRIX :: detail。
 *
 *  @see LYNXMATRIX
 */
/**
 *  @ingroup LynxMath 
 *  @brief 4x4矩陣。
 *
 *	<div class="fragment">
 *	<pre>	
 *	typedef	 struct {
 *		union {
 *			struct {
 *				LYNXREAL			m11, m12, m13;			
 *				LYNXREAL			m21, m22, m23;			
 *				LYNXREAL			m31, m32, m33;							
 *			}s;
 *			LYNXREAL m[3][3];
 *		};
 *	}LYNXMATRIX3 LYNXALIGNED(16), * LPLYNXMATRIX3;	
 *	</pre>
 *  </div> 
 */
//*******************this structure can't be modified, will cause read old model file crash**********
typedef	struct {
	union {
		struct {
			LYNXREAL		m11, m12, m13;			
			LYNXREAL		m21, m22, m23;			
			LYNXREAL		m31, m32, m33;					
			LYNXREAL		m41, m42, m43;					
		};
		LYNXREAL		m[4][3];	
		LYNXREAL		f[12];
	};
}LYNXMATRIX4x3 LYNXALIGNED(16), * LPLYNXMATRIX4x3;	
typedef const LYNXMATRIX4x3* LPCLYNXMATRIX4x3;

/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXMATRIX
 *  @brief 4x4矩陣指標。
 *
 *	LPLYNXMATRIX :: detail。
 *
 *  @see LYNXMATRIX
 */
/**
 *  @ingroup LynxMath 
 *  @brief 4x4矩陣。
 *
 *	<div class="fragment">
 *	<pre>	
 *	typedef	 struct {
 *		union {
 *			struct {
 *				LYNXREAL			m11, m12, m13, m14;			
 *				LYNXREAL			m21, m22, m23, m24;			
 *				LYNXREAL			m31, m32, m33, m34;			
 *				LYNXREAL			m41, m42, m43, m44;			
 *			}s;
 *			LYNXREAL m[4][4];
 *		};
 *	}LYNXMATRIX LYNXALIGNED(16), * LPLYNXMATRIX;	
 *	</pre>
 *  </div> 
 */
//*******************this structure can't be modified, will cause read old model file crash**********
typedef	struct {
	union {
		struct {
			LYNXREAL		m11, m12, m13, m14;			
			LYNXREAL		m21, m22, m23, m24;			
			LYNXREAL		m31, m32, m33, m34;			
			LYNXREAL		m41, m42, m43, m44;			
		};
		LYNXREAL		m[4][4];	
		LYNXREAL		f[16];
	};
}LYNXMATRIX LYNXALIGNED(16), * LPLYNXMATRIX;	
typedef const LYNXMATRIX* LPCLYNXMATRIX;

/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXLINE3D
 *  @brief 3D直線指標。 
 *
 *  @see LYNXLINE3D
 */
/**
 *  @ingroup LynxMath 
 *  @brief 3D直線。
 * 
 *	Line = Origin + t*Dir.
 *
 *  @see LPLYNXLINE3D
 */
LYNX_CACHE_ALIGN typedef struct {
	LYNXVECTOR3D		Origin;				/*!< 直線之起點。 */  	
	LYNXVECTOR3D		Dir;				/*!< 直線之方向。 */  		
}LYNXLINE3D, *LPLYNXLINE3D;
typedef const LYNXLINE3D* LPCLYNXLINE3D;

/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXPLANE
 *  @brief 3D平面指標。 
 *
 *  @see LYNXPLANE
 */
/**
 *  @ingroup LynxMath 
 *  @brief 3D平面。
 * 
 *	a*Normal.x + b*Normal.y + c*Normal.z = Distance.
 *
 *  @see LPLYNXPLANE
 */
//*******************this structure can't be modified, will cause read old model file crash**********
LYNX_CACHE_ALIGN typedef struct {
	LYNXVECTOR3D		Normal;				/*!< 平面之法向量。 */  	
	LYNXREAL			Distance;			/*!< 平面與原點之距離。 */  	
}LYNXPLANE, *LPLYNXPLANE;
typedef const LYNXPLANE* LPCLYNXPLANE;

LYNX_CACHE_ALIGN typedef struct  LynxQuaternion {
    LYNXREAL						w, x, y, z;
}LYNXQUATERNION, *LPLYNXQUATERNION;
typedef const LYNXQUATERNION* LPCLYNXQUATERNION;

LYNX_CACHE_ALIGN typedef struct {
	DWORD							v1, v2, v3;	
	LYNXPLANE						Plane;	
}LYNXTRIANGLE, *LPLYNXTRIANGLE;
typedef const LYNXTRIANGLE* LPCLYNXTRIANGLE;


/**
 *  @ingroup LynxMath 
 *	@struct LPLYNXFRUSTUM
 *  @brief View Frustum指標。 
 *
 *  @see LYNXFRUSTUM
 */
/**
 *  @ingroup LynxMath 
 *  @brief View Frustum。
 * 
 *	可視範圍六面體。
 *
 *  @see LPLYNXFRUSTUM
 */
LYNX_CACHE_ALIGN typedef struct {
	/*! 左右平面。*/    
	LYNXPLANE			Left, Right;	
	/*! 上下平面。*/
	LYNXPLANE			Top, Bottom;	
	/*! 前後平面。*/ 
	LYNXPLANE			Near, Far;		
	LYNXVECTOR3D		FarP, NearP;		
}LYNXFRUSTUM, *LPLYNXFRUSTUM;
typedef const LYNXFRUSTUM* LPCLYNXFRUSTUM;

LYNX_CACHE_ALIGN typedef struct {
	LYNXVECTOR3D			Min;	
	LYNXVECTOR3D			Max;	
}LYNXBOUNDARYBOX,* LPLYNXBOUNDARYBOX;
typedef const LYNXBOUNDARYBOX* LPCLYNXBOUNDARYBOX;

LYNX_CACHE_ALIGN typedef struct {	
	union {
		struct {
			DWORD				v1, v2, v3;
		};
		DWORD				v[3];	
	};
}LYNXSIMPLETRIANGLE, *LPLYNXSIMPLETRIANGLE;
typedef const LYNXSIMPLETRIANGLE* LPCLYNXSIMPLETRIANGLE;

/*
LYNX_CACHE_ALIGN typedef struct LynxQuad{	
	WORD							lpv[4];				
	LYNXPLANEBOUNDARY				PlaneBoundary;			
}LYNXQUAD, *LPLYNXQUAD;
*/

/*
LYNX_CACHE_ALIGN typedef struct LYNXQuadBoundary{	
	LYNXVECTOR3D					lpCheckV[4];
	LYNXVECTOR3D					lpTCheckV[4];
	LYNXQUAD						CheckQuad;		
}LYNXQUADBOUNDARY, *LPLYNXQUADBOUNDARY;
*/

LYNX_CACHE_ALIGN typedef struct LynxPlaneBoundary{	
	DWORD							NumVerts;		
	LYNXPLANE						Plane;
	LPLYNXPLANE						lpEdgePlane;
}LYNXPLANEBOUNDARY, *LPLYNXPLANEBOUNDARY;
typedef const LYNXPLANEBOUNDARY* LPCLYNXPLANEBOUNDARY;

LYNX_CACHE_ALIGN typedef struct {	
	LYNXPLANE						Plane;
	LYNXPLANE						lpEdgePlane[3];
}LYNXTRIANGLEBOUNDARY, *LPLYNXTRIANGLEBOUNDARY;
typedef const LYNXTRIANGLEBOUNDARY* LPCLYNXTRIANGLEBOUNDARY;

LYNX_CACHE_ALIGN typedef struct {	
	LYNXVECTOR3D					lpCheckV[4];
	LYNXVECTOR3D					lpTCheckV[4];
	WORD							lpv[4];				
	LYNXPLANE						Plane;
	LYNXPLANE						lpEdgePlane[4];
}LYNXQUADBOUNDARY, *LPLYNXQUADBOUNDARY;
typedef const LYNXQUADBOUNDARY* LPCLYNXQUADBOUNDARY;

LYNX_CACHE_ALIGN typedef struct LynxBoxBoundary{	
	LYNXVECTOR3D					lpCheckV[8];
	LYNXVECTOR3D					lpTCheckV[8];
	LYNXQUADBOUNDARY				lpCheckQuad[6];		
}LYNXBOXBOUNDARY, *LPLYNXBOXBOUNDARY;
typedef const LYNXBOXBOUNDARY* LPCLYNXBOXBOUNDARY;

LYNX_CACHE_ALIGN typedef struct {	
	DWORD							NumVerts;	
	WORD							lpv[4];								
	LYNXPLANEBOUNDARY				PlaneBoundary;		
}LYNXFAN, *LPLYNXFAN;
typedef const LYNXFAN* LPCLYNXFAN;

#ifdef __FIXED_POINT__		
	#define LYNX_DEG_STEP				1
#else
	#define LYNX_DEG_STEP				10
#endif

#define LYNX_FIXEDi(a)								((LYNXFIXED)((a)<<16))
#define LYNX_FIXED(a)								((LYNXFIXED)((a)*(1<<16)))
#define LYNX_FIXED_ABS(a)							(labs((LYNXFIXED)(a)))
#define LYNX_FIXED_BACK(a)							((a)>>16)
#define LYNX_FIXED_FLOOR(a)							((((LYNXFIXED)(a)) >> 16) << 16)
/* Result is undefined if sum > 7FFF.FFFF or sum < 8000.0000 */
#define LYNX_FIXED_ADD(a, b)						(((LYNXFIXED)(a)) + ((LYNXFIXED)(b)))
/* Result is undefined if difference > 7FFF.FFFF or difference < 8000.0000 */
#define LYNX_FIXED_SUB(a, b)						(((LYNXFIXED)(a)) - ((LYNXFIXED)(b)))
/* Result is undefined if product > 7FFF.FFFF or product < 8000.0000 */
#define LYNX_FIXED_MUL(a, b)						((LYNXFIXED)((((LYNXLONGLONG)(LYNXFIXED)(a)) * ((LYNXFIXED)(b))) >> 16))
#define LYNX_FIXED_DIV(a, b)						((LYNXFIXED)((((LYNXLONGLONG)(LYNXFIXED)(a)) << 16) / ((LYNXFIXED)(b))))
/* Result undefined if input > 7FFF.0000 */
#define LYNX_FIXED_CEIL(a)							(-LYNX_FIXED_FLOOR(-((LYNXFIXED)(a))))
#define LYNX_FIXED_RECIP(b)							((LYNXFIXED)((((LYNXLONGLONG)(0x10000)) << 16) / ((LYNXFIXED)(b))))
#define LYNX_FIXED_REMAINDER(a, b)					((LYNXFIXED)((((LYNXLONGLONG)(LYNXFIXED)(a)) << 16) % ((LYNXFIXED)(b))))


#ifdef __FIXED_POINT__	
	#define LYNX_REAL_HALF								((LYNXFIXED)(1<<15))
	#define LYNX_REAL(a)								LYNX_FIXED(a)
	#define LYNX_REAL_ABS(a)							LYNX_FIXED_ABS(a)
	#define LYNX_REAL_FLOOR(a)							LYNX_FIXED_FLOOR(a)
	#define LYNX_REAL_ADD(a, b)							LYNX_FIXED_ADD(a, b)
	#define LYNX_REAL_SUB(a, b)							LYNX_FIXED_SUB(a, b)
	#define LYNX_REAL_MUL(a, b)							LYNX_FIXED_MUL(a, b)
	#define LYNX_REAL_DIV(a, b)							LYNX_FIXED_DIV(a, b)
	#define LYNX_REAL_CEIL(a)							LYNX_FIXED_CEIL(a)
	#define LYNX_REAL_RECIP(b)							LYNX_FIXED_RECIP(b)
	#define LYNX_REAL_REMAINDER(a, b)					LYNX_FIXED_REMAINDER(a, b)
	#define LYNX_REAL_2_INT(a)							LYNX_FIXED_BACK(a)
	#define LYNX_REAL_ROUND_INT(a)						LYNX_FIXED_BACK(LYNX_REAL_ADD((a), LYNX_REAL_HALF))
	#define LYNX_REAL_BACK(a)							LYNX_FIXED_BACK(a)
#else	
	#define LYNX_REAL_HALF								(0.5f)
	#define LYNX_REAL(a)								(a)
	#define LYNX_REAL_ABS(a)							(fabs((a)))
	#define LYNX_REAL_FLOOR(a)							((LYNXREAL)((int)(a)))
	#define LYNX_REAL_ADD(a, b)							((a)+(b))
	#define LYNX_REAL_SUB(a, b)							((a)-(b))
	#define LYNX_REAL_MUL(a, b)							((a)*(b))
	#define LYNX_REAL_DIV(a, b)							((a)/(b))
	#define LYNX_REAL_CEIL(a)							(-LYNX_REAL_FLOOR(-(a))))
	#define LYNX_REAL_RECIP(b)							
	#define LYNX_REAL_REMAINDER(a, b)					
	#define LYNX_REAL_2_INT(a)							((int)(a))
	#define LYNX_REAL_ROUND_INT(a)						(LYNX_ROUND(a))
	#define LYNX_REAL_BACK(a)							(a)
#endif

/**
 *  @ingroup LynxMath 
 *  @brief 設定 3x3 矩陣。
 *
 *	設定一個 3x3 矩陣的元素值。
 *	@param m 欲設定的 3x3 Matrix【輸出】。  
 *	@param xm11~xm33 欲設定的 9 元素值【輸入】。  
 *	@see LYNXMATRIX3 
 */
#define LYNX_MATRIX3(m, xm11, xm12, xm13\
                      , xm21, xm22, xm23\
                      , xm31, xm32, xm33) \
m.m11 = xm11; m.m12 = xm12; m.m13 = xm13;\
m.m21 = xm21; m.m22 = xm22; m.m23 = xm23;\
m.m31 = xm31; m.m32 = xm32; m.m33 = xm33;

/**
 *  @ingroup LynxMath 
 *  @brief 設定 4x4 矩陣。
 *
 *	設定一個 4x4 矩陣的元素值。
 *	@param m 欲設定的 4x4 Matrix【輸出】。  
 *	@param xm11~xm44 欲設定的 16 元素值【輸入】。  
 *	@see LYNXMATRIX 
 */
#define LYNX_MATRIX(m, xm11, xm12, xm13, xm14\
                     , xm21, xm22, xm23, xm24\
                     , xm31, xm32, xm33, xm34\
                     , xm41, xm42, xm43, xm44) \
m.m11 = xm11; m.m12 = xm12; m.m13 = xm13; m.m14 = xm14;\
m.m21 = xm21; m.m22 = xm22; m.m23 = xm23; m.m24 = xm24;\
m.m31 = xm31; m.m32 = xm32; m.m33 = xm33; m.m34 = xm34;\
m.m41 = xm41; m.m42 = xm42; m.m43 = xm43; m.m44 = xm44;


/**
 *  @ingroup LynxMath 
 *  @brief 將4x4矩陣設為Identity Matrix。
 * 	
 *	@param m 欲設定成Identity的Matrix【輸出】。 
 *	@see LYNXMATRIX 
 */
#ifdef __FIXED_POINT__	
	#define LYNX_IDENTITY_MATRIX3(m) \
	m.m11 = LYNX_FIXEDi(1); m.m12 = LYNX_FIXEDi(0); m.m13 = LYNX_FIXEDi(0);\
	m.m21 = LYNX_FIXEDi(0); m.m22 = LYNX_FIXEDi(1); m.m23 = LYNX_FIXEDi(0);\
	m.m31 = LYNX_FIXEDi(0); m.m32 = LYNX_FIXEDi(0); m.m33 = LYNX_FIXEDi(1);	
	#define LYNX_IDENTITY_MATRIX(m) \
	m.m11 = LYNX_FIXEDi(1); m.m12 = LYNX_FIXEDi(0); m.m13 = LYNX_FIXEDi(0); m.m14 = LYNX_FIXEDi(0);\
	m.m21 = LYNX_FIXEDi(0); m.m22 = LYNX_FIXEDi(1); m.m23 = LYNX_FIXEDi(0); m.m24 = LYNX_FIXEDi(0);\
	m.m31 = LYNX_FIXEDi(0); m.m32 = LYNX_FIXEDi(0); m.m33 = LYNX_FIXEDi(1); m.m34 = LYNX_FIXEDi(0);\
	m.m41 = LYNX_FIXEDi(0); m.m42 = LYNX_FIXEDi(0); m.m43 = LYNX_FIXEDi(0); m.m44 = LYNX_FIXEDi(1);	
	#define LYNX_RESET_MATRIX_ORIENTATION(m) \
	m.m11 = LYNX_FIXEDi(1); m.m12 = LYNX_FIXEDi(0); m.m13 = LYNX_FIXEDi(0);\
	m.m21 = LYNX_FIXEDi(0); m.m22 = LYNX_FIXEDi(1); m.m23 = LYNX_FIXEDi(0);\
	m.m31 = LYNX_FIXEDi(0); m.m32 = LYNX_FIXEDi(0); m.m33 = LYNX_FIXEDi(1); 	
#else
	#define LYNX_IDENTITY_MATRIX3(m) \
	m.m11 = LYNX_REAL(1.0f); m.m12 = LYNX_REAL(0.0f); m.m13 = LYNX_REAL(0.0f);\
	m.m21 = LYNX_REAL(0.0f); m.m22 = LYNX_REAL(1.0f); m.m23 = LYNX_REAL(0.0f);\
	m.m31 = LYNX_REAL(0.0f); m.m32 = LYNX_REAL(0.0f); m.m33 = LYNX_REAL(1.0f);	
	#define LYNX_IDENTITY_MATRIX(m) \
	m.m11 = LYNX_REAL(1.0f); m.m12 = LYNX_REAL(0.0f); m.m13 = LYNX_REAL(0.0f); m.m14 = LYNX_REAL(0.0f);\
	m.m21 = LYNX_REAL(0.0f); m.m22 = LYNX_REAL(1.0f); m.m23 = LYNX_REAL(0.0f); m.m24 = LYNX_REAL(0.0f);\
	m.m31 = LYNX_REAL(0.0f); m.m32 = LYNX_REAL(0.0f); m.m33 = LYNX_REAL(1.0f); m.m34 = LYNX_REAL(0.0f);\
	m.m41 = LYNX_REAL(0.0f); m.m42 = LYNX_REAL(0.0f); m.m43 = LYNX_REAL(0.0f); m.m44 = LYNX_REAL(1.0f);
	#define LYNX_ZERO_MATRIX3(m) \
	m.m11 = LYNX_REAL(0.0f); m.m12 = LYNX_REAL(0.0f); m.m13 = LYNX_REAL(0.0f);\
	m.m21 = LYNX_REAL(0.0f); m.m22 = LYNX_REAL(0.0f); m.m23 = LYNX_REAL(0.0f);\
	m.m31 = LYNX_REAL(0.0f); m.m32 = LYNX_REAL(0.0f); m.m33 = LYNX_REAL(0.0f);
	#define LYNX_ZERO_MATRIX(m) \
	m.m11 = LYNX_REAL(0.0f); m.m12 = LYNX_REAL(0.0f); m.m13 = LYNX_REAL(0.0f); m.m14 = LYNX_REAL(0.0f);\
	m.m21 = LYNX_REAL(0.0f); m.m22 = LYNX_REAL(0.0f); m.m23 = LYNX_REAL(0.0f); m.m24 = LYNX_REAL(0.0f);\
	m.m31 = LYNX_REAL(0.0f); m.m32 = LYNX_REAL(0.0f); m.m33 = LYNX_REAL(0.0f); m.m34 = LYNX_REAL(0.0f);\
	m.m41 = LYNX_REAL(0.0f); m.m42 = LYNX_REAL(0.0f); m.m43 = LYNX_REAL(0.0f); m.m44 = LYNX_REAL(0.0f);
	#define LYNX_RESET_MATRIX_ORIENTATION(m) \
	m.m11 = LYNX_REAL(1.0f); m.m12 = LYNX_REAL(0.0f); m.m13 = LYNX_REAL(0.0f);\
	m.m21 = LYNX_REAL(0.0f); m.m22 = LYNX_REAL(1.0f); m.m23 = LYNX_REAL(0.0f);\
	m.m31 = LYNX_REAL(0.0f); m.m32 = LYNX_REAL(0.0f); m.m33 = LYNX_REAL(1.0f);	
#endif

/**
 *  @ingroup LynxMath 
 *  @brief 複製 4x4 矩陣。
 *
 *	複製4x4矩陣到另一 4x4 矩陣。
 *	@param md 目的 4x4 Matrix【輸出】。 
 *	@param ms 來源 4x4 Matrix【輸入】。
 *	@see LYNXMATRIX 
 */
#define LYNX_MATRIX_COPY(md, ms)						lynxMemCpy(&md, &ms, sizeof(LYNXMATRIX))
#define LYNX_MATRIX3_COPY(md, ms)						lynxMemCpy(&md, &ms, sizeof(LYNXMATRIX3))

#ifdef __PS2__	
	#define LYNX_MATRIX_GET_RIGHTDIR(dir, ms)			(dir).x = (ms).m11; (dir).y = (ms).m12; (dir).z = (ms).m13; (dir).w = 1.0f;
	#define LYNX_MATRIX_GET_UPDIR(dir, ms)				(dir).x = (ms).m21; (dir).y = (ms).m22; (dir).z = (ms).m23; (dir).w = 1.0f;
	#define LYNX_MATRIX_GET_LOOKDIR(dir, ms)			(dir).x = (ms).m31; (dir).y = (ms).m32; (dir).z = (ms).m33; (dir).w = 1.0f;
	#define LYNX_MATRIX_GET_POS(pos, ms)				(pos).x = (ms).m41; (pos).y = (ms).m42; (pos).z = (ms).m43; (pos).w = 1.0f;

	#define LYNX_VECTOR3D(v, fx, fy, fz)				(v).x = fx; (v).y = fy; (v).z = fz;	(v).w = 1.0f;	
	#define LYNX_VECTOR3D_COPY(a, b)					(a).x = b.x; (a).y = b.y; (a).z = b.z; (a).w = b.w;	
	#define LYNX_VECTOR3D_ADD(n,va,vb)					ilynxVu0_AddVector(&n, &va, &vb)
	#define LYNX_VECTOR3D_SUB(n, va, vb)				(n).x = (va).x-(vb).x; (n).y = (va).y-(vb).y; (n).z=(va).z-(vb).z;
	#define LYNX_VECTOR3D_MUL(n,t,va)					ilynxVu0_ScaleVector(&n, &va, t)
	#define LYNX_DOT_PRODUCT(va,vb)						ilynxVu0_DotProduct((LPLYNXVECTOR3D)&(va), (LPLYNXVECTOR3D)&(vb))			
	#define LYNX_VECTOR3D_SET_W(a, b)					(a).w = b;	
	#define LYNX_INTERPOLATE(r, v1, v2, off)			ilynxVu0_Interpolate(r, v1, v2, off);
#else
	#define LYNX_MATRIX_GET_RIGHTDIR(dir, ms)			(dir).x = (ms).m11; (dir).y = (ms).m12; (dir).z = (ms).m13;
	#define LYNX_MATRIX_GET_UPDIR(dir, ms)				(dir).x = (ms).m21; (dir).y = (ms).m22; (dir).z = (ms).m23;
	#define LYNX_MATRIX_GET_LOOKDIR(dir, ms)			(dir).x = (ms).m31; (dir).y = (ms).m32; (dir).z = (ms).m33;
	#define LYNX_MATRIX_GET_POS(pos, ms)				(pos).x = (ms).m41; (pos).y = (ms).m42; (pos).z = (ms).m43;
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_VECTOR2D(v, fx, fy)					(v).x = fx; (v).y = fy;
	#define LYNX_VECTOR3D(v, fx, fy, fz)				(v).x = fx; (v).y = fy; (v).z = fz;	
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_VECTOR2D_COPY(a, b)					(a).x = b.x; (a).y = b.y;
	#define LYNX_VECTOR3D_COPY(a, b)					(a).x = b.x; (a).y = b.y; (a).z = b.z;		
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_VECTOR2D_ADD(n, va, vb)				(n).x = LYNX_REAL_ADD((va).x, (vb).x); (n).y = LYNX_REAL_ADD((va).y, (vb).y);
	#define LYNX_VECTOR3D_ADD(n, va, vb)				(n).x = LYNX_REAL_ADD((va).x, (vb).x); (n).y = LYNX_REAL_ADD((va).y, (vb).y); (n).z = LYNX_REAL_ADD((va).z, (vb).z);
	/**
	*  @ingroup LynxMath 	 	
	*/
	#define LYNX_VECTOR2D_SUB(n, va, vb)				(n).x = LYNX_REAL_SUB((va).x, (vb).x); (n).y = LYNX_REAL_SUB((va).y, (vb).y);
	#define LYNX_VECTOR3D_SUB(n, va, vb)				(n).x = LYNX_REAL_SUB((va).x, (vb).x); (n).y = LYNX_REAL_SUB((va).y, (vb).y); (n).z = LYNX_REAL_SUB((va).z, (vb).z);
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_VECTOR2D_MUL(n,t,va)					(n).x = LYNX_REAL_MUL(t, (va).x); (n).y = LYNX_REAL_MUL(t, (va).y);
	#define LYNX_VECTOR3D_MUL(n,t,va)					(n).x = LYNX_REAL_MUL(t, (va).x); (n).y = LYNX_REAL_MUL(t, (va).y); (n).z = LYNX_REAL_MUL(t, (va).z);
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_VECTOR2D_DIV(n,t,va)					(n).x = LYNX_REAL_DIV(t, (va).x); (n).y = LYNX_REAL_DIV(t, (va).y);
	#define LYNX_VECTOR3D_DIV(n,t,va)					(n).x = LYNX_REAL_DIV(t, (va).x); (n).y = LYNX_REAL_DIV(t, (va).y); (n).z = LYNX_REAL_DIV(t, (va).z);
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_DOT_PRODUCT2(va,vb)					 (LYNX_REAL_MUL((va).x, (vb).x) + LYNX_REAL_MUL((va).y, (vb).y))
	#define LYNX_DOT_PRODUCT(va,vb)						 (LYNX_REAL_MUL((va).x, (vb).x) + LYNX_REAL_MUL((va).y, (vb).y) + LYNX_REAL_MUL((va).z, (vb).z))//( LYNX_REAL_ADD(LYNX_REAL_ADD(LYNX_REAL_MUL((va).x, (vb).x), LYNX_REAL_MUL((va).y, (vb).y)), LYNX_REAL_MUL((va).z, (vb).z)) )
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_VECTOR3D_SET_W(a, b)					
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_INTERPOLATE(v1, v2, off)				LYNX_REAL_ADD((v1), LYNX_REAL_MUL(LYNX_REAL_SUB((v2), (v1)), off))	
#endif	

#ifdef __FIXED_POINT__
	/**
	*  @ingroup LynxMath 	
	*  @brief 複製 Detail。
	*/
	#define	LYNX_WRAP_DEGREE(d)							while ((d) < 0) (d) = LYNX_REAL_ADD((d), (LYNX_FIXEDi(360)));	while ((d) >= (LYNX_FIXEDi(360))) (d) = LYNX_REAL_SUB((d), (LYNX_FIXEDi(360)));		
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_RANGE_ONE_2_NEG_ONE(v)					if (v > 1.0f) v = 1.0; if (v < -1.0f) v = -1.0f;
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_PLANE_POINT_DISTANCE(p, v)				(LYNX_REAL_ADD(LYNX_REAL_ADD(LYNX_REAL_ADD(LYNX_REAL_MUL(-v.x, p.Normal.x), LYNX_REAL_MUL(-v.y, p.Normal.y)), LYNX_REAL_MUL(-v.z, p.Normal.z)), p.Distance))
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_PLANE_POINT_SIDE(p, v)					(LYNX_REAL_ADD(LYNX_REAL_ADD(LYNX_REAL_ADD(LYNX_REAL_MUL(v.x, p.Normal.x), LYNX_REAL_MUL(v.y, p.Normal.y)), LYNX_REAL_MUL(v.z, p.Normal.z)), -p.Distance))
#else
	/**
	*  @ingroup LynxMath 	
	*  @brief 複製 Detail。
	*/
	#define	LYNX_WRAP_DEGREE(d)							while (d < 0.0f) d += 360.0f;	while (d >= 360.0f) d -= 360.0f;		
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_RANGE_ONE_2_NEG_ONE(v)					if (v > 1.0f) v = 1.0; if (v < -1.0f) v = -1.0f;
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_PLANE_POINT_DISTANCE(p, v)				((-v.x*p.Normal.x)+(-v.y*p.Normal.y)+(-v.z*p.Normal.z)+p.Distance)
	/**
	*  @ingroup LynxMath 	
	*/
	#define LYNX_PLANE_POINT_SIDE(p, v)					((v.x*p.Normal.x)+(v.y*p.Normal.y)+(v.z*p.Normal.z)-p.Distance)
#endif
/**
 *  @ingroup LynxMath 	
 *  @brief 複製 Detail。
 */
#define LYNX_MATRIX_TRANSPOSE(lpm, t) \
t = lpm->m12; lpm->m12 = lpm->m21; lpm->m21 = t;\
t = lpm->m13; lpm->m13 = lpm->m31; lpm->m31 = t;\
t = lpm->m14; lpm->m14 = lpm->m41; lpm->m41 = t;\
t = lpm->m23; lpm->m23 = lpm->m32; lpm->m32 = t;\
t = lpm->m24; lpm->m24 = lpm->m42; lpm->m42 = t;\
t = lpm->m34; lpm->m34 = lpm->m43; lpm->m43 = t;													


#define LYYNX_IS_POINT_IN_BBOX(bb, p)					((bb.Min.x <= p.x && bb.Max.x >= p.x) && (bb.Min.y <= p.y && bb.Max.y >= p.y) && (bb.Min.z <= p.z && bb.Max.z >= p.z))

//#define LYNX_DISTANCE_POINT_2_PLANE_3(p, o, v)		(((o.x-v.x)*p.Normal.x)+((o.y-v.y)*p.Normal.y)+((o.z-v.z)*p.Normal.z))
//#define LYNX_DISTANCE_POINT_2_PLANE_2(p, v)			((p.Distance-(v.x*p.Normal.x+v.y*p.Normal.y+v.z*p.Normal.z))/(p.Normal.x*p.Normal.x+p.Normal.y*p.Normal.y+p.Normal.z*p.Normal.z))
										
#ifdef __cplusplus
extern "C" {
#endif								
/*
LYNXCOREAPI  LPCLYNXMATRIX		lynxGetIdentityMatrix();
LYNXCOREAPI  void				lynxSetgInstructionSet(const int s);
LYNXCOREAPI  LYNXREAL			lynxSin(const LYNXREAL d);
LYNXCOREAPI  LYNXREAL			lynxCos(const LYNXREAL d);
LYNXCOREAPI  LYNXREAL			lynxQuickSin(const LYNXREAL d);
LYNXCOREAPI  LYNXREAL			lynxQuickCos(const LYNXREAL d);
LYNXCOREAPI  LYNXREAL			lynxSqrt(const LYNXREAL f);
*/

LYNXCOREAPI void				lynxInitMathSystem(void);
LYNXCOREAPI void				lynxSrand(unsigned int seed);
LYNXCOREAPI int					lynxRand();
LYNXCOREAPI LYNXBOOL			lynxCheckPowerOf2(int v, int max_power);
LYNXCOREAPI LYNXBOOL			lynxProbability(const int num, const int base);
LYNXCOREAPI LYNXBOOL			lynxProbabilityf(const float p);
LYNXCOREAPI void				lynxNormalise2(LPLYNXVECTOR2D v, const int dim);
LYNXCOREAPI void				lynxNormalise(LPLYNXVECTOR3D v, const int dim);
LYNXCOREAPI LYNXREAL			lynxLength2(LPLYNXVECTOR2D v, const int dim);
LYNXCOREAPI LYNXREAL			lynxLength(LPLYNXVECTOR3D v, const int dim);
LYNXCOREAPI LYNXREAL			lynxSquaredLength2(LPLYNXVECTOR2D v, const int dim);
LYNXCOREAPI LYNXREAL			lynxSquaredLength(LPLYNXVECTOR3D v, const int dim);
LYNXCOREAPI void				lynxFPUPointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxPointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxFPUSequencePointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, int num);
LYNXCOREAPI void				lynxSequencePointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, int num);
LYNXCOREAPI void				lynxVectorXMatrix3(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX3 lpm);
LYNXCOREAPI void				lynxVectorXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxVector4XMatrix(LPLYNXVECTOR4D lpb, LPCLYNXVECTOR4D lpa, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxSequenceVectorXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, const int num);
//LYNXCOREAPI  void				lynxSequenceRenderVertexXMatrix(LPLYNXRENDERVERTEX lpb, LPLYNXVERTEX3D lpa, LPLYNXMATRIX lpm, int num);
//LYNXCOREAPI  void				lynxSequenceRenderVertexXMatrixV(LPLYNXRENDERVERTEX lpb, LPLYNXVECTOR3D lpa, LPLYNXMATRIX lpm, int num);
LYNXCOREAPI LYNXREAL			lynxDotProduct(LPCLYNXVECTOR3D lpa, LPCLYNXVECTOR3D lpb);
LYNXCOREAPI void				lynxCrossProduct(LPLYNXVECTOR3D lpd, LPCLYNXVECTOR3D lpa, LPCLYNXVECTOR3D lpb);
LYNXCOREAPI void				lynxSequenceCrossProduct(LPLYNXVECTOR3D lpd, LPCLYNXVECTOR3D lpa, LPCLYNXVECTOR3D lpb, const int num);
LYNXCOREAPI void				lynxMatrixXMatrix(LPLYNXMATRIX lpd, LPCLYNXMATRIX lpm1, LPCLYNXMATRIX lpm2);
LYNXCOREAPI void				lynxMatrix3XMatrix3(LPLYNXMATRIX3 lpd, LPCLYNXMATRIX3 lpm1, LPCLYNXMATRIX3 lpm2);
LYNXCOREAPI void				lynxInvertMatrix(LPLYNXMATRIX d, LPCLYNXMATRIX a);
LYNXCOREAPI void				lynxInverseMatrix(LPLYNXMATRIX d, LPCLYNXMATRIX a);
LYNXCOREAPI void				lynxSetRotationMatrix(LPLYNXMATRIX lpm, LPCLYNXVECTOR3D lpd, LPCLYNXVECTOR3D lpu);
LYNXCOREAPI void				lynxSpline(LPLYNXVECTOR3D p, const float t, LPCLYNXVECTOR3D p1, LPCLYNXVECTOR3D p2, LPCLYNXVECTOR3D p3, LPCLYNXVECTOR3D p4);
LYNXCOREAPI void				lynxSplinef(float* p, const int n, const float t, const float* p1, const float* p2, const float* p3, const float* p4);
LYNXCOREAPI void				lynxMakeRotateMatrix(LPLYNXMATRIX m, const LYNXREAL deg, const LYNXAXISTYPE axis);
LYNXCOREAPI void				lynxMakeRotateMatrix3(LPLYNXMATRIX3 m, const LYNXREAL deg);
LYNXCOREAPI void				lynxShortestArcQuaternion(LPLYNXQUATERNION lpq, LPCLYNXVECTOR3D lpv0, LPCLYNXVECTOR3D lpv1);
LYNXCOREAPI void				lynxQuaternionXQuaternion(LPLYNXQUATERNION lpq, LPCLYNXQUATERNION lpq1, LPCLYNXQUATERNION lpq2);
LYNXCOREAPI void				lynxQuaternion2Matrix(LPLYNXMATRIX lpm, LPCLYNXQUATERNION lpq);
LYNXCOREAPI void			    lynxEulerAnglesXYZ2Matrix(LPLYNXMATRIX lpm, LPCLYNXVECTOR3D lpv);
LYNXCOREAPI void				lynxMatrix2EulerAnglesXYZ(LPLYNXVECTOR3D lpv, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxMatrix2Quaternion(LPLYNXQUATERNION lpq, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxTransposeMatrix(LPLYNXMATRIX lpm);
LYNXCOREAPI void				lynxTransposeMatrix2(LPLYNXMATRIX lpmo, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxTransposeMatrix2Matrix4x3(LPLYNXMATRIX4x3 lpmo, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxCopyMatrix2Matrix4x3(LPLYNXMATRIX4x3 lpmo, LPCLYNXMATRIX lpm);
LYNXCOREAPI void				lynxGetViewMatrixFromTransformMatrix(LPLYNXMATRIX lpmo, LPCLYNXMATRIX lpm);
LYNXCOREAPI	void				lynxPerspectiveProjectionMatrix(LPLYNXMATRIX lpm, const LYNXREAL near_plane, const LYNXREAL far_plane, const LYNXREAL fov, const LYNXREAL xyratio);
LYNXCOREAPI void				lynxOrthoProjectionMatrixOffset(LPLYNXMATRIX lpm, const LYNXREAL left, const LYNXREAL right, const LYNXREAL bottom, const LYNXREAL top, const LYNXREAL nearp, const LYNXREAL farp);
LYNXCOREAPI void				lynxOrthoProjectionMatrixOffsetYDown(LPLYNXMATRIX lpm, const LYNXREAL left, const LYNXREAL right, const LYNXREAL bottom, const LYNXREAL top, const LYNXREAL nearp, const LYNXREAL farp);
LYNXCOREAPI void				lynxOrthoProjectionMatrix(LPLYNXMATRIX lpm, const LYNXREAL width, const LYNXREAL height, const LYNXREAL nearp, const LYNXREAL farp);
LYNXCOREAPI void				lynxNormaliseMatrix(LPLYNXMATRIX lpm);
//extern  void					lynxSlerpQuaternion(LPLYNXQUATERNION lpqout, LPLYNXQUATERNION lpqin1, LPLYNXQUATERNION lpqin2, float t);
LYNXCOREAPI void				lynxMakeQuaternion(LPLYNXQUATERNION lpq, LPCLYNXVECTOR3D lpaxis, const LYNXREAL ang);
LYNXCOREAPI void				lynxMakeRotateMatrixRespectVector(LPLYNXMATRIX lpm, LPCLYNXVECTOR3D lpaxis, const LYNXREAL ang);
LYNXCOREAPI LYNXBOOL			lynxPointInsidePlane(LPLYNXVECTOR3D lpp, LPCLYNXPLANE lpplane);
LYNXCOREAPI LYNXBOOL			lynxPointInsidePlaneWithRadius(LPLYNXVECTOR3D lpp, LPCLYNXPLANE lpplane, LYNXREAL r);
LYNXCOREAPI LYNXBOOL			lynxLineInsertTriangleBoundary(LPLYNXVECTOR3D lppos, LPCLYNXTRIANGLEBOUNDARY lptb, LPCLYNXVECTOR3D lpv1, LPCLYNXVECTOR3D lpv2);
LYNXCOREAPI void				lynxSetupTriangleBoundary(LPLYNXTRIANGLEBOUNDARY lptb, LPCLYNXVECTOR3D lpv);
LYNXCOREAPI void				lynxSetupPlaneBoundary(LPLYNXPLANEBOUNDARY lppb, LPCLYNXVECTOR3D lpv, const int numv);
LYNXCOREAPI LYNXBOOL			lynxLineInsertPlaneBoundary(LPLYNXVECTOR3D lppos, LPCLYNXPLANEBOUNDARY lppb, LPCLYNXVECTOR3D lpv1, LPCLYNXVECTOR3D lpv2);
LYNXCOREAPI LYNXBOOL			lynxLineSegmentInsertPlaneBoundary(LPLYNXVECTOR3D lppos, LPCLYNXPLANEBOUNDARY lppb, LPCLYNXVECTOR3D lpv1, LPCLYNXVECTOR3D lpv2);
LYNXCOREAPI void				lynxGetMatrixReflectToPlane(LPLYNXMATRIX lpm, LPCLYNXPLANE lpp);
LYNXCOREAPI void				lynxGetReflectionVector(LPLYNXVECTOR3D lpr, LPCLYNXVECTOR3D lpv, LPCLYNXVECTOR3D lpn);
LYNXCOREAPI void				lynxMakeLookAtMatrix(LPLYNXMATRIX lpm, LPCLYNXVECTOR3D lpv);
LYNXCOREAPI LYNXREAL			lynxDistancePoint2Line(LPCLYNXVECTOR3D lpp, LPCLYNXLINE3D lpn);
LYNXCOREAPI LYNXREAL			lynxDistancePoint2Segment(LPCLYNXVECTOR3D lpp, LPCLYNXLINE3D lpn);
LYNXCOREAPI LYNXREAL			lynxSqrDistancePoint2Segment(LPCLYNXVECTOR3D lpp, LPCLYNXLINE3D lpn);
LYNXCOREAPI LYNXFORCEINLINE int lynxNextPowerOfTwo(int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

#ifdef __PS2__	
	LYNXEXTERN LYNXINLINE void lynxVu0_ProjectRenderVertices(LPLYNXRENDERVERTEX lpb, LPLYNXRENDERVERTEX lpa, LPLYNXMATRIX lpm, int num);
	LYNXEXTERN LYNXINLINE void lynxVu0_ProjectVertices(LPLYNXRENDERVERTEX lpb, LPLYNXVECTOR3D lpa, LPLYNXMATRIX lpm, int num);
	LYNXEXTERN LYNXINLINE void lynxVu0_ProjectVertice(LPLYNXRENDERVERTEX lpb, LPLYNXVECTOR3D lpa, LPLYNXMATRIX lpm);
#endif

#ifdef __cplusplus
}
#endif

#include <LynxMath.inl>

#endif
