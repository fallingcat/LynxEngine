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

#if defined(__WIN32__) && defined (_M_IX86)
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxSSE_VectorXMatrix3(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX3 lpm)
{
	__asm {
		mov ecx, lpa
		mov edx, lpm
		mov eax, lpb

		movss xmm0, dword ptr [ecx]
		movss xmm5, dword ptr [edx]
		movhps xmm5, qword ptr [edx+4]
		shufps xmm0, xmm0, 0
		movss xmm1, dword ptr [ecx+4]
		movss xmm3, dword ptr [edx+12]
		movhps xmm3, qword ptr [edx+26]
		shufps xmm1, xmm1, 0
		mulps xmm0, xmm5
		mulps xmm1, xmm3
		movss xmm2, dword ptr [ecx+8]
		shufps xmm2, xmm2, 0
		movss xmm4, dword ptr [edx+24]
		movhps xmm4, qword ptr [edx+28]
		addps xmm0, xmm1
		mulps xmm2, xmm4
		addps xmm0, xmm2
		movss dword ptr [eax], xmm0
		movhps qword ptr [eax+4], xmm0
	}
    return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxSSE_Vector4XMatrix(LPLYNXVECTOR4D lpb, LPCLYNXVECTOR4D lpa, LPCLYNXMATRIX lpm)
{
	
	lpb->x = lpa->x*lpm->m11 + lpa->y*lpm->m21 + lpa->z*lpm->m31 + lpa->w*lpm->m41;
    lpb->y = lpa->x*lpm->m12 + lpa->y*lpm->m22 + lpa->z*lpm->m32 + lpa->w*lpm->m42;
    lpb->z = lpa->x*lpm->m13 + lpa->y*lpm->m23 + lpa->z*lpm->m33 + lpa->w*lpm->m43;
	lpb->w = lpa->x*lpm->m14 + lpa->y*lpm->m24 + lpa->z*lpm->m34 + lpa->w*lpm->m44;
	
    return;
	

	/*
	__asm 
	{
		mov ecx, lpa
		mov edx, lpm
		mov eax, lpb

		movss xmm0, dword ptr [ecx]
		movss xmm5, dword ptr [edx]
		movhps xmm5, qword ptr [edx+4]
		shufps xmm0, xmm0, 0
		movss xmm1, dword ptr [ecx+4]
		movss xmm3, dword ptr [edx+16]
		movhps xmm3, qword ptr [edx+20]
		shufps xmm1, xmm1, 0
		mulps xmm0, xmm5
		mulps xmm1, xmm3
		movss xmm2, dword ptr [ecx+8]
		shufps xmm2, xmm2, 0
		movss xmm4, dword ptr [edx+32]
		movhps xmm4, qword ptr [edx+36]
		addps xmm0, xmm1
		mulps xmm2, xmm4
		addps xmm0, xmm2
		movss xmm1, dword ptr [ecx+12]
		shufps xmm1, xmm1, 0
		movss xmm2, dword ptr [edx+48]
		movhps xmm2, qword ptr [edx+52]
		mulps xmm2, xmm1
		addps xmm0, xmm2

		movss dword ptr [eax], xmm0
		movhps qword ptr [eax+4], xmm0					
	}
	*/
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxSSE_VectorXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm)
{
	__asm {
		mov ecx, lpa
		mov edx, lpm
		mov eax, lpb

		movss xmm0, dword ptr [ecx]
		movss xmm5, dword ptr [edx]
		movhps xmm5, qword ptr [edx+4]
		shufps xmm0, xmm0, 0
		movss xmm1, dword ptr [ecx+4]
		movss xmm3, dword ptr [edx+16]
		movhps xmm3, qword ptr [edx+20]
		shufps xmm1, xmm1, 0
		mulps xmm0, xmm5
		mulps xmm1, xmm3
		movss xmm2, dword ptr [ecx+8]
		shufps xmm2, xmm2, 0
		movss xmm4, dword ptr [edx+32]
		movhps xmm4, qword ptr [edx+36]
		addps xmm0, xmm1
		mulps xmm2, xmm4
		addps xmm0, xmm2
		movss dword ptr [eax], xmm0
		movhps qword ptr [eax+4], xmm0
	}
    return;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxSSE_PointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm)
{
	__asm 
	{
		mov ecx, lpa
		mov edx, lpm
		mov eax, lpb

		movss xmm0, dword ptr [ecx]
		movss xmm5, dword ptr [edx]
		movhps xmm5, qword ptr [edx+4]
		shufps xmm0, xmm0, 0
		movss xmm1, dword ptr [ecx+4]
		movss xmm3, dword ptr [edx+16]
		movhps xmm3, qword ptr [edx+20]
		shufps xmm1, xmm1, 0
		mulps xmm0, xmm5
		mulps xmm1, xmm3
		movss xmm2, dword ptr [ecx+8]
		shufps xmm2, xmm2, 0
		movss xmm4, dword ptr [edx+32]
		movhps xmm4, qword ptr [edx+36]
		addps xmm0, xmm1
		mulps xmm2, xmm4
		addps xmm0, xmm2
		movss xmm2, dword ptr [edx+48]
		movhps xmm2, qword ptr [edx+52]
		addps xmm0, xmm2

		movss dword ptr [eax], xmm0
		movhps qword ptr [eax+4], xmm0					
	}
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxSSE_SequencePointXMatrix(LPLYNXVECTOR3D lpb, LPCLYNXVECTOR3D lpa, LPCLYNXMATRIX lpm, int num)
{
    __asm 
	{
		mov ebx, lpa
		mov edx, lpm
		mov eax, lpb
		mov ecx, num

		movss xmm5, dword ptr [edx]
		movhps xmm5, qword ptr [edx+4]

		movss xmm3, dword ptr [edx+16]
		movhps xmm3, qword ptr [edx+20]

		movss xmm4, dword ptr [edx+32]
		movhps xmm4, qword ptr [edx+36]

		movss xmm6, dword ptr [edx+48]
		movhps xmm6, qword ptr [edx+52]

NextPoint:
		movss xmm0, dword ptr [ebx]
		add	ebx, 4			
		shufps xmm0, xmm0, 0		

		movss xmm1, dword ptr [ebx]
		add	ebx, 4			
		shufps xmm1, xmm1, 0

		mulps xmm0, xmm5
		mulps xmm1, xmm3		

		movss xmm2, dword ptr [ebx]
		add	ebx, 4	
		shufps xmm2, xmm2, 0
		
		addps xmm0, xmm1
		mulps xmm2, xmm4
		addps xmm0, xmm2
		
		addps xmm0, xmm6

		movss dword ptr [eax], xmm0
		add eax, 4
		movhps qword ptr [eax], xmm0
		add eax, 8

		dec ecx
		jnz NextPoint
	}
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//		lpDst = lpSrc1 * lpSrc2
//		lpDst can be equal to lpSrc1 or lpSrc2
//-------------------------------------------------------------------------------------------------------
void ilynxSSE_MatrixXMatrix(LPLYNXMATRIX lpd, LPCLYNXMATRIX lpm1, LPCLYNXMATRIX lpm2)
{	
	__asm 
	{
		mov edx, lpm1
		mov eax, lpd
		mov ecx, lpm2

		movss xmm0, dword ptr [edx]
		//movaps xmm1, xmmword ptr [ecx]
		movlps	xmm1, qword ptr[ecx]
		movhps	xmm1, qword ptr[ecx+8]

		shufps xmm0, xmm0, 0
		movss xmm2, dword ptr [edx+4]
		mulps xmm0, xmm1
		shufps xmm2, xmm2, 0
		//movaps xmm3, xmmword ptr [ecx+10h]
		movlps	xmm3, qword ptr[ecx+10h]
		movhps	xmm3, qword ptr[ecx+18h]
		movss xmm7, dword ptr [edx+8]
		mulps xmm2, xmm3
		shufps xmm7, xmm7, 0
		addps xmm0, xmm2
		//movaps xmm4, xmmword ptr [ecx+20h]
		movlps	xmm4, qword ptr[ecx+20h]
		movhps	xmm4, qword ptr[ecx+28h]
		movss xmm2, dword ptr [edx+0Ch]
		mulps xmm7, xmm4
		shufps xmm2, xmm2, 0
		addps xmm0, xmm7
		//movaps xmm5, xmmword ptr [ecx+30h]
		movlps	xmm5, qword ptr[ecx+30h]
		movhps	xmm5, qword ptr[ecx+38h]
		movss xmm6, dword ptr [edx+10h]
		mulps xmm2, xmm5
		movss xmm7, dword ptr [edx+14h]
		shufps xmm6, xmm6, 0
		addps xmm0, xmm2
		shufps xmm7, xmm7, 0		
		movlps qword ptr [eax], xmm0
		movhps qword ptr [eax+8], xmm0
		//movaps xmmword ptr [eax], xmm0

		mulps xmm7, xmm3
		movss xmm0, dword ptr [edx+18h]
		mulps xmm6, xmm1
		shufps xmm0, xmm0, 0
		addps xmm6, xmm7
		mulps xmm0, xmm4
		movss xmm2, dword ptr [edx+24h]
		addps xmm6, xmm0
		movss xmm0, dword ptr [edx+1Ch]
		movss xmm7, dword ptr [edx+20h]
		shufps xmm0, xmm0, 0
		shufps xmm7, xmm7, 0
		mulps xmm0, xmm5
		mulps xmm7, xmm1
		addps xmm6, xmm0
		shufps xmm2, xmm2, 0
		movlps qword ptr [eax+10h], xmm6
		movhps qword ptr [eax+18h], xmm6
		//movaps xmmword ptr [eax+10h], xmm6

		mulps xmm2, xmm3
		movss xmm6, dword ptr [edx+28h]
		addps xmm7, xmm2
		shufps xmm6, xmm6, 0
		movss xmm2, dword ptr [edx+2Ch]
		mulps xmm6, xmm4
		shufps xmm2, xmm2, 0
		addps xmm7, xmm6
		mulps xmm2, xmm5
		movss xmm0, dword ptr [edx+34h]
		addps xmm7, xmm2
		shufps xmm0, xmm0, 0
		movlps qword ptr [eax+20h], xmm7
		movhps qword ptr [eax+28h], xmm7
		//movaps xmmword ptr [eax+20h], xmm7

		movss xmm2, dword ptr [edx+30h]		
		mulps xmm0, xmm3
		shufps xmm2, xmm2, 0
		movss xmm6, dword ptr [edx+38h]
		mulps xmm2, xmm1
		shufps xmm6, xmm6, 0
		addps xmm2, xmm0
		mulps xmm6, xmm4
		movss xmm7, dword ptr [edx+3Ch]
		shufps xmm7, xmm7, 0
		addps xmm2, xmm6
		mulps xmm7, xmm5
		addps xmm2, xmm7
		//movaps xmmword ptr [eax+30h], xmm2	
		movlps	qword ptr[eax+30h], xmm2
		movhps	qword ptr[eax+38h], xmm2
	}
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void ilynxSSE_InverseMatrix(float* src)
{	 
	 __m128						minor0, minor1, minor2, minor3;
	 __m128						row0, row1, row2, row3;
	 __m128						tmp1;

	 tmp1 = _mm_set1_ps(0.0f);
	 row1 = _mm_set1_ps(0.0f);
	 row3 = _mm_set1_ps(0.0f);

	 tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
	 row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));
	 row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
	 row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
	 tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
	 row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));
	 row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
	 row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
	// -----------------------------------------------
	 tmp1 = _mm_mul_ps(row2, row3);
	 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	 minor0 = _mm_mul_ps(row1, tmp1);
	 minor1 = _mm_mul_ps(row0, tmp1);
	 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	 minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
	 minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
	 minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
	// -----------------------------------------------
	 tmp1 = _mm_mul_ps(row1, row2);
	 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	 minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
	 minor3 = _mm_mul_ps(row0, tmp1);
	 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
	 minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
	 minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
	// -----------------------------------------------
	 tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	 row2 = _mm_shuffle_ps(row2, row2, 0x4E);
	 minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
	 minor2 = _mm_mul_ps(row0, tmp1);
	 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
	 minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
	 minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
	// -----------------------------------------------
	 tmp1 = _mm_mul_ps(row0, row1);
}
#endif