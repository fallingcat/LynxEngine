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

#ifdef __PS2__
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXEXTERN LYNXINLINE void lynxVu0_ProjectRenderVertices(LPLYNXRENDERVERTEX lpb, LPLYNXRENDERVERTEX lpa, LPLYNXMATRIX lpm, int num)
{
    __asm__ __volatile__("	
		lqc2    		vf4, 0x0(%2)
		lqc2    		vf5, 0x10(%2)
		lqc2    		vf6, 0x20(%2)
		lqc2    		vf7, 0x30(%2)
_loopMulMatrix:
		#lqc2    		vf8, 0x10(%1)
		#sqc2    		vf8, 0x10(%0)			
		lqc2    		vf8, 0x20(%1)
		vmulax.xyzw		ACC, vf4, vf8
		vmadday.xyzw	ACC, vf5, vf8
		vmaddaz.xyzw	ACC, vf6, vf8
		vmaddw.xyzw		vf9, vf7, vf8
		vdiv    		Q, vf8w, vf9w		
		vwaitq		
		vmulq.xyz       vf9, vf9, Q			
		vftoi4.xyz	    vf8, vf9				#addi    		%1, 0x30
		vmfir.w			vf9, $0					#addi			%3, -1			
		sqc2    		vf8, 0x20(%0)			
		addi    		%0, 0x30
		addi    		%1, 0x30				
		addi			%3, -1			
		bne				$0, %3, _loopMulMatrix"
		:: "r" (lpb) , "r" (lpa) , "r" (lpm), "r" (num) : "$0", "memory");
		//lpb->w = 0.0f;		
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXEXTERN LYNXINLINE void lynxVu0_ProjectVertices(LPLYNXRENDERVERTEX lpb, LPLYNXVECTOR3D lpa, LPLYNXMATRIX lpm, int num)
{
    __asm__ __volatile__("	
		lqc2    		vf4, 0x0(%2)
		lqc2    		vf5, 0x10(%2)
		lqc2    		vf6, 0x20(%2)
		lqc2    		vf7, 0x30(%2)
_loopMulMatrix:		
		lqc2    		vf8, 0x0(%1)
		vmulax.xyzw		ACC, vf4, vf8
		vmadday.xyzw	ACC, vf5, vf8
		vmaddaz.xyzw	ACC, vf6, vf8
		vmaddw.xyzw		vf9, vf7, vf8
		vdiv    		Q, vf8w, vf9w		
		vwaitq		
		vmulq.xyz       vf9, vf9, Q			
		vftoi4.xyz	    vf8, vf9				addi    		%1, 0x10
		vmfir.w			vf9, $0					addi			%3, -1			
		sqc2    		vf8, 0x20(%0)			
		addi    		%0, 0x30
		#addi    		%1, 0x10				
		#addi			%3, -1			
		bne				$0, %3, _loopMulMatrix"
		:: "r" (lpb) , "r" (lpa) , "r" (lpm), "r" (num) : "$0", "memory");
		//lpb->w = 0.0f;		
}

//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXEXTERN LYNXINLINE void lynxVu0_ProjectVertice(LPLYNXRENDERVERTEX lpb, LPLYNXVECTOR3D lpa, LPLYNXMATRIX lpm)
{
	__asm__ __volatile__("	
		lqc2    		vf4, 0x0(%2)
		lqc2    		vf5, 0x10(%2)
		lqc2    		vf6, 0x20(%2)
		lqc2    		vf7, 0x30(%2)
		lqc2    		vf8, 0x0(%1)
		vmulax.xyzw		ACC, vf4, vf8
		vmadday.xyzw	ACC, vf5, vf8
		vmaddaz.xyzw	ACC, vf6, vf8
		vmaddw.xyzw		vf9, vf7, vf8
		vdiv    		Q, vf8w, vf9w		
		vwaitq		
		vmulq.xyz       vf9, vf9, Q			
		vftoi4.xyz	    vf8, vf9	
		vmfir.w			vf9, $0								
		sqc2    		vf8, 0x20(%0)"
		:: "r" (lpb) , "r" (lpa) , "r" (lpm) : "$0", "memory");		
		//lpb->w = 0.0f;		
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXINLINE void ilynxVu0_AddVector(LPLYNXVECTOR3D lpv, LPLYNXVECTOR3D lpv1, LPLYNXVECTOR3D lpv2)
{
    __asm__ __volatile__("	
		lqc2			vf4, 0x0(%1)
		lqc2			vf5, 0x0(%2)
		vadd.xyz		vf6, vf4, vf5
		sqc2			vf6, 0x0(%0)"
		:: "r" (lpv) , "r" (lpv1), "r" (lpv2));		
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXINLINE void ilynxVu0_ScaleVector(LPLYNXVECTOR3D lpv, LPLYNXVECTOR3D lpv1, float t)
{
    //int ret = DI();
    __asm__ __volatile__("
		lqc2    		vf4, 0x0(%1)
		mfc1    		$8, %2
		qmtc2    		$8, vf5
		vmulx.xyzw		vf6, vf4, vf5
		sqc2    		vf6, 0x0(%0)"
		:: "r" (lpv) , "r" (lpv1), "f" (t) : "$8", "memory");
    //if (ret) EI();
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXINLINE void ilynxVu0_PointXMatrix(LPLYNXVECTOR3D lpb, LPLYNXVECTOR3D lpa, LPLYNXMATRIX lpm)
{
    //int ret = DI();
    //lpa->w = 1.0f;    
    __asm__ __volatile__("    	    	
    	#li				$7, 1				#//before <lqc2> becuase the upper instruct of writing to vfxx will discard the <vmfir>
		#vmfir.w		vf8w, $7			
		
		lqc2    		vf4, 0x0(%2)		
		lqc2    		vf5, 0x10(%2)		
		lqc2    		vf6, 0x20(%2)
		lqc2    		vf7, 0x30(%2)		
		lqc2   			vf8, 0x0(%1)							
		
		vmulax.xyzw		ACC, vf4, vf8
		vmadday.xyzw	ACC, vf5, vf8
		vmaddaz.xyzw	ACC, vf6, vf8
		vmaddw.xyzw		vf9, vf7, vf8
		sqc2    		vf9, 0x0(%0)"
		:: "r" (lpb) , "r" (lpa) , "r" (lpm) : "$7", "memory");
    //if (ret) EI();
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXINLINE void ilynxVu0_RenderVXMatrix(LPLYNXRENDERVERTEX lpb, LPLYNXVECTOR3D lpa, LPLYNXMATRIX lpm)
{
    //int ret = DI();
    __asm__ __volatile__("
		lqc2    		vf4, 0x0(%2)
		lqc2    		vf5, 0x10(%2)
		lqc2    		vf6, 0x20(%2)
		lqc2    		vf7, 0x30(%2)
		lqc2    		vf8, 0x0(%1)
		vmulax.xyzw		ACC, vf4, vf8
		vmadday.xyzw	ACC, vf5, vf8
		vmaddaz.xyzw	ACC, vf6, vf8
		vmaddw.xyzw		vf9, vf7, vf8
		sqc2    		vf9, 0x20(%0)"
		:: "r" (lpb) , "r" (lpa) , "r" (lpm): "memory");
    //if (ret) EI();
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXINLINE void ilynxVu0_MatrixXMatrix(LPLYNXMATRIX lpd, LPLYNXMATRIX lpm1, LPLYNXMATRIX lpm2)
{
    //int ret = DI();
    __asm__ __volatile__("
		lqc2			vf4, 0x0(%2)			li				$7, 4
		lqc2			vf5, 0x10(%2)
		lqc2			vf6, 0x20(%2)
		lqc2			vf7, 0x30(%2)		
_loopMulMatrix:
		lqc2			vf8, 0x0(%1)
		vmulax.xyzw		ACC, vf4, vf8
		vmadday.xyzw	ACC, vf5, vf8
		vmaddaz.xyzw	ACC, vf6, vf8
		vmaddw.xyzw		vf9, vf7, vf8
		sqc2			vf9, 0x0(%0)
		addi			$7, -1
		addi			%1, 0x10
		addi			%0, 0x10
		bne				$0, $7,_loopMulMatrix"
		:: "r" (lpd), "r" (lpm1), "r" (lpm2) : "$7", "memory");
	//if (ret) EI();	    
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXINLINE float ilynxVu0_DotProduct(LPLYNXVECTOR3D lpva, LPLYNXVECTOR3D lpvb)
{
    register float ret;

    //int r = DI();
    __asm__ __volatile__("
		lqc2			vf4, 0x0(%1)
		lqc2			vf5, 0x0(%2)
		vmul.xyz		vf5, vf4, vf5
		vaddy.x			vf5, vf5, vf5
		vaddz.x			vf5, vf5, vf5
		qmfc2			$2, vf5
		mtc1			$2, %0"
		: "=f" (ret) :"r" (lpva) ,"r" (lpvb) :"$2", "memory" );
    //if (r) EI();
    return ret;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXINLINE void ilynxVu0_CrossProduct(LPLYNXVECTOR3D lpd, LPLYNXVECTOR3D lpa, LPLYNXVECTOR3D lpb)
{
    //int ret = DI();
    __asm__ __volatile__("
		lqc2			vf4, 0x0(%1)
		lqc2			vf5, 0x0(%2)
		vopmula.xyz		ACC, vf4, vf5
		vopmsub.xyz		vf6, vf5, vf4
		vsub.w			vf6, vf6, vf6		
		sqc2			vf6, 0x0(%0)"
	: : "r" (lpd) , "r" (lpa) ,"r" (lpb) : "memory");
    //if (ret) EI();
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//			Quaternion : w, x, y, z
//-------------------------------------------------------------------------------------------------------
LYNXINLINE  void    ilynxVu0_Quaternion2Matrix(LPLYNXMATRIX lpm, LPLYNXQUATERNION lpq)
{
	__asm__ __volatile__("
		lqc2			vf4, 0x0(%1)							
		vmul.xyzw		vf5, vf4, vf4
		vaddy.x			vf5, vf5, vf5
		vaddz.x			vf5, vf5, vf5
		vaddw.x			vf5, vf5, vf5				li				$7, 2						
		vmfir.w			vf6w, $7			
		vdiv    		Q, vf6w, vf5x	
		vwaitq		
		
		#vmulq.xyz      vf5, vf4, Q					#vf5 -> xs, ys, zs	
		#vmul.xyz		vf6, vf4w, vf5				#vf6 -> wx, wy, wz	
		
		#vsub.xyzw		vf7, vf0, vf0"
		#vsub.xyzw		vf8, vf0, vf0"
		#vsub.xyzw		vf9, vf0, vf0"
		#vsub.xyzw		vf10, vf0, vf0"
		
		:: "r" (lpm), "r" (lpq) : "$7", "memory");	
		
		
	/*
	s = 2.0f / (lpq->x * lpq->x + lpq->y * lpq->y + lpq->z * lpq->z + lpq->w * lpq->w);

    xs = lpq->x * s;        ys = lpq->y * s;        zs = lpq->z * s;
    wx = lpq->w * xs;       wy = lpq->w * ys;       wz = lpq->w * zs;
	xx = lpq->x * xs;       xy = lpq->x * ys;       xz = lpq->x * zs;
	yy = lpq->y * ys;       yz = lpq->y * zs;       zz = lpq->z * zs;

	lpm->m11 = 1.0f - (yy + zz);
	lpm->m12 = xy + wz;
	lpm->m13 = xz - wy;
	lpm->m14 = 0.0f;

	lpm->m21 = xy - wz;
	lpm->m22 = 1.0f - (xx + zz);
	lpm->m23 = yz + wx;
	lpm->m24 = 0.0f;

	lpm->m31 = xz + wy;
	lpm->m32 = yz - wx;
	lpm->m33 = 1.0f - (xx + yy);
	lpm->m34 = 0.0f;

	lpm->m41 = lpm->m42 = lpm->m43 = 0.0f;
	lpm->m44 = 1.0f;
	*/
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//			
//-------------------------------------------------------------------------------------------------------
LYNXINLINE  void    ilynxVu0_Interpolate(float *lpr, float *lpv1, float *lpv2, float t)
{
	//*lpR = (*lpV1) + ((*lpV2) - (*lpV1))*Offset;		
				
	__asm__ __volatile__("
		lqc2    		vf4, 0x0(%1)
		lqc2    		vf5, 0x0(%2)
		vsub.xyzw		vf6, vf5, vf4
		mfc1    		$8, %3
		qmtc2    		$8, vf7		
		vmulx.xyzw		vf6, vf6, vf7
		vadd.xyzw		vf6, vf6, vf4
		sqc2    		vf6, 0x0(%0)"		
		:: "r" (lpr) , "r" (lpv1), "r" (lpv2), "f" (t) : "$8" , "memory");		
}

#endif