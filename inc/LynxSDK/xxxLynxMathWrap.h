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

#ifndef __LYNXMATHWRAP_H__
#define __LYNXMATHWRAP_H__

#include <LynxGraphicsWrap.h>
#include <LynxMath.h>

namespace LynxSDK
{
	namespace Math
	{
		class LYNXENGCLASS CReal
		{			
		public:
			LYNXREAL				r;
		public:						
			LYNXFORCEINLINE CReal() {r = LYNX_REAL(0.0f); };
			//LYNXFORCEINLINE CReal(LYNXREAL v) {r = v; };						
			//template<typename TYPE> LYNXFORCEINLINE CReal(TYPE v) {r = LYNX_REAL(v); };						
			LYNXFORCEINLINE CReal(int v) {r = LYNX_REAL(v); };						
			LYNXFORCEINLINE CReal(float v) {r = LYNX_REAL(v); };						
			LYNXFORCEINLINE CReal(double v) {r = LYNX_REAL(v); };									
			~CReal() {};	

			LYNXFORCEINLINE operator LYNXREAL() {return r;};						
			LYNXFORCEINLINE operator LYNXREAL*() {return (&r);};
			LYNXFORCEINLINE CReal& operator =(const CReal& rhs) { r = rhs.r; return (*this); };
			//template<typename TYPE> LYNXFORCEINLINE CReal& operator =(const TYPE rhs) { r = LYNX_REAL(rhs); return (*this); };			
			LYNXFORCEINLINE void operator += (const CReal& rhs) { r = LYNX_REAL_ADD(r, rhs.r); };			
			//template<typename TYPE> LYNXFORCEINLINE void operator += (const TYPE& rhs) { r = LYNX_REAL_ADD(r, LYNX_REAL(rhs)); };			
			LYNXFORCEINLINE void operator -= (const CReal& rhs) { r = LYNX_REAL_SUB(r, rhs.r); };			
			//template<typename TYPE> LYNXFORCEINLINE void operator -= (const TYPE& rhs) { r = LYNX_REAL_SUB(r, LYNX_REAL(rhs)); };			
			LYNXFORCEINLINE void operator *= (const CReal& rhs) { r = LYNX_REAL_MUL(r, rhs.r);};			
			//template<typename TYPE> LYNXFORCEINLINE void operator *= (const TYPE& rhs) { r = LYNX_REAL_MUL(r, LYNX_REAL(rhs)); };			
			LYNXFORCEINLINE void operator /= (const CReal& rhs) { r = LYNX_REAL_DIV(r, rhs.r);};			
			//template<typename TYPE> LYNXFORCEINLINE void operator /= (const TYPE& rhs) { r = LYNX_REAL_DIV(r, LYNX_REAL(rhs)); };			
			LYNXFORCEINLINE CReal& operator ++() { r = LYNX_REAL_ADD(r, LYNX_REAL(1)); return (*this); };						
			LYNXFORCEINLINE CReal& operator --() { r = LYNX_REAL_SUB(r, LYNX_REAL(1)); return (*this); };
		};				
		LYNXFORCEINLINE CReal operator +(const CReal& lhs, const CReal& rhs) { return CReal(LYNX_REAL_ADD(lhs.r, rhs.r)); };
		LYNXFORCEINLINE CReal operator +(const CReal& lhs, const LYNXREAL& rhs) { return (lhs+CReal(rhs)); };
		LYNXFORCEINLINE CReal operator +(const LYNXREAL& lhs, const CReal& rhs) { return (CReal(lhs)+rhs); };	
		LYNXFORCEINLINE CReal operator +(const CReal& lhs, const int& rhs) { return (lhs+CReal(rhs)); };
		LYNXFORCEINLINE CReal operator +(const int& lhs, const CReal& rhs) { return (CReal(lhs)+rhs); };	
		LYNXFORCEINLINE CReal operator +(const CReal& lhs, const double& rhs) { return (lhs+CReal(rhs)); };
		LYNXFORCEINLINE CReal operator +(const double& lhs, const CReal& rhs) { return (CReal(lhs)+rhs); };				
		//template<typename TYPE> LYNXFORCEINLINE CReal operator +(const CReal& lhs, const TYPE& rhs) { return CReal(LYNX_REAL_ADD(lhs.r, LYNX_REAL(rhs))); };
		//template<typename TYPE> LYNXFORCEINLINE CReal operator +(const TYPE& lhs, const CReal& rhs) { return CReal(LYNX_REAL_ADD(LYNX_REAL(lhs), rhs.r)); };		
		LYNXFORCEINLINE CReal operator -(const CReal& lhs, const CReal& rhs) { return CReal(LYNX_REAL_SUB(lhs.r, rhs.r)); };
		LYNXFORCEINLINE CReal operator -(const CReal& lhs, const LYNXREAL& rhs) { return (lhs-CReal(rhs)); };
		LYNXFORCEINLINE CReal operator -(const LYNXREAL& lhs, const CReal& rhs) { return (CReal(lhs)-rhs); };	
		LYNXFORCEINLINE CReal operator -(const CReal& lhs, const int& rhs) { return (lhs-CReal(rhs)); };
		LYNXFORCEINLINE CReal operator -(const int& lhs, const CReal& rhs) { return (CReal(lhs)-rhs); };	
		LYNXFORCEINLINE CReal operator -(const CReal& lhs, const double& rhs) { return (lhs-CReal(rhs)); };
		LYNXFORCEINLINE CReal operator -(const double& lhs, const CReal& rhs) { return (CReal(lhs)-rhs); };				
		//template<typename TYPE> LYNXFORCEINLINE CReal operator -(const CReal& lhs, const TYPE& rhs) { return CReal(LYNX_REAL_SUB(lhs.r, LYNX_REAL(rhs))); };
		//template<typename TYPE> LYNXFORCEINLINE CReal operator -(const TYPE& lhs, const CReal& rhs) { return CReal(LYNX_REAL_SUB(LYNX_REAL(lhs), rhs.r)); };		
		LYNXFORCEINLINE CReal operator *(const CReal& lhs, const CReal& rhs) { return CReal(LYNX_REAL_MUL(lhs.r, rhs.r)); };
		LYNXFORCEINLINE CReal operator *(const CReal& lhs, const LYNXREAL& rhs) { return (lhs*CReal(rhs)); };
		LYNXFORCEINLINE CReal operator *(const LYNXREAL& lhs, const CReal& rhs) { return (CReal(lhs)*rhs); };	
		LYNXFORCEINLINE CReal operator *(const CReal& lhs, const int& rhs) { return (lhs*CReal(rhs)); };
		LYNXFORCEINLINE CReal operator *(const int& lhs, const CReal& rhs) { return (CReal(lhs)*rhs); };	
		LYNXFORCEINLINE CReal operator *(const CReal& lhs, const double& rhs) { return (lhs*CReal(rhs)); };
		LYNXFORCEINLINE CReal operator *(const double& lhs, const CReal& rhs) { return (CReal(lhs)*rhs); };				
		//template<typename TYPE> LYNXFORCEINLINE CReal operator *(const CReal& lhs, const TYPE& rhs) { return CReal(LYNX_REAL_MUL(lhs.r, LYNX_REAL(rhs))); };
		//template<typename TYPE> LYNXFORCEINLINE CReal operator *(const TYPE& lhs, const CReal& rhs) { return CReal(LYNX_REAL_MUL(LYNX_REAL(lhs), rhs.r)); };		
		LYNXFORCEINLINE CReal operator /(const CReal& lhs, const CReal& rhs) { return CReal(LYNX_REAL_DIV(lhs.r, rhs.r)); };		
		LYNXFORCEINLINE CReal operator /(const CReal& lhs, const LYNXREAL& rhs) { return (lhs/CReal(rhs)); };
		LYNXFORCEINLINE CReal operator /(const LYNXREAL& lhs, const CReal& rhs) { return (CReal(lhs)/rhs); };	
		LYNXFORCEINLINE CReal operator /(const CReal& lhs, const int& rhs) { return (lhs/CReal(rhs)); };
		LYNXFORCEINLINE CReal operator /(const int& lhs, const CReal& rhs) { return (CReal(lhs)/rhs); };	
		LYNXFORCEINLINE CReal operator /(const CReal& lhs, const double& rhs) { return (lhs/CReal(rhs)); };
		LYNXFORCEINLINE CReal operator /(const double& lhs, const CReal& rhs) { return (CReal(lhs)/rhs); };				
		//template<typename TYPE> LYNXFORCEINLINE CReal operator /(const CReal& lhs, const TYPE& rhs) { return CReal(LYNX_REAL_DIV(lhs.r, LYNX_REAL(rhs))); };
		//template<typename TYPE> LYNXFORCEINLINE CReal operator /(const TYPE& lhs, const CReal& rhs) { return CReal(LYNX_REAL_DIV(LYNX_REAL(lhs), rhs.r)); };		
		LYNXFORCEINLINE bool operator ==(const CReal& lhs, const CReal& rhs) { return (lhs.r == rhs.r); };		
		//template<typename TYPE> LYNXFORCEINLINE bool operator ==(const CReal& lhs, const TYPE& rhs) { return (lhs.r == LYNX_REAL(rhs)); };		
		//template<typename TYPE> LYNXFORCEINLINE bool operator ==(const TYPE& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) == rhs.r); };				
		LYNXFORCEINLINE bool operator !=(const CReal& lhs, const CReal& rhs) { return (lhs.r != rhs.r); };				
		//template<typename TYPE> LYNXFORCEINLINE bool operator !=(const TYPE& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) != rhs.r); };		
		//template<typename TYPE> LYNXFORCEINLINE bool operator !=(const CReal& lhs, const TYPE& rhs) { return (lhs.r != LYNX_REAL(rhs)); };		

		class LYNXENGCLASS CVector3
		{			
		public:
			union {
				struct {
					CReal x, y, z;
				};
				//CReal v[3];
			};
		public:						
			CVector3() {x = y = z = 0; };
			CVector3(LYNXVECTOR3D& v) {x = v.s.x; y = v.s.y; z= v.s.z; };
			CVector3(LPLYNXVECTOR3D v) {x = v->s.x; y = v->s.y; z= v->s.z; };
			CVector3(CReal vx, CReal vy, CReal vz) {x = vx; y = vy; z= vz; };
			~CVector3() {};	
			
			LYNXFORCEINLINE operator LPLYNXVECTOR3D() {return ((LPLYNXVECTOR3D)&x);};						
			LYNXFORCEINLINE operator LYNXVECTOR3D() {return (*((LPLYNXVECTOR3D)this));};												
			LYNXFORCEINLINE CVector3& operator =(const CVector3& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return (*this); };						
			LYNXFORCEINLINE CVector3& operator =(const LYNXVECTOR3D& rhs) { x = rhs.s.x; y = rhs.s.y; z = rhs.s.z; return (*this); };						
			/*LYNXFORCEINLINE void operator += (const CVector3& rhs) {};
			LYNXFORCEINLINE void operator += (const CReal& rhs) {};
			LYNXFORCEINLINE void operator -= (const CVector3& rhs) {};
			LYNXFORCEINLINE void operator -= (const CReal& rhs) {};
			LYNXFORCEINLINE void operator *= (const CVector3& rhs) {};
			LYNXFORCEINLINE void operator *= (const CReal& rhs) {};
			LYNXFORCEINLINE void operator /= (const CVector3& rhs) {};
			LYNXFORCEINLINE void operator /= (const CReal& rhs) {};*/

			LYNXFORCEINLINE void Normalise(int dim) { lynxNormalise((LPLYNXVECTOR3D)this, dim); };
			LYNXFORCEINLINE CReal Length(int dim) { return CReal(lynxLength((LPLYNXVECTOR3D)this, dim)); };			
		};		
		LYNXFORCEINLINE CVector3 operator +(const CVector3& lhs, const CVector3& rhs) { return CVector3((lhs.x+rhs.x), (lhs.y+rhs.y), (lhs.z+rhs.z)); };
		LYNXFORCEINLINE CVector3 operator +(const CVector3& lhs, const CReal& rhs) { return CVector3((lhs.x+rhs.r), (lhs.y+rhs.r), (lhs.z+rhs.r)); };
		LYNXFORCEINLINE CVector3 operator +(const CReal& lhs, const CVector3& rhs) { return CVector3((lhs.r+rhs.x), (lhs.r+rhs.y), (lhs.r+rhs.z)); };
		//template<typename TYPE> LYNXFORCEINLINE CVector3 operator +(const CVector3& lhs, const TYPE& rhs) { return CVector3((lhs.x+CReal(rhs)), (lhs.y+CReal(rhs)), (lhs.z+CReal(rhs))); };
		//template<typename TYPE> LYNXFORCEINLINE CVector3 operator +(const TYPE& lhs, const CVector3& rhs) { return CVector3((CReal(lhs)+rhs.x), (CReal(lhs)+rhs.y), (CReal(lhs)+rhs.z)); };
		LYNXFORCEINLINE CVector3 operator -(const CVector3& lhs, const CVector3& rhs) { return CVector3((lhs.x-rhs.x), (lhs.y-rhs.y), (lhs.z-rhs.z)); };		
		LYNXFORCEINLINE CVector3 operator -(const CVector3& lhs, const CReal& rhs) { return CVector3((lhs.x-rhs.r), (lhs.y-rhs.r), (lhs.z-rhs.r)); };		
		LYNXFORCEINLINE CVector3 operator -(const CReal& lhs, const CVector3& rhs) { return CVector3((lhs.r-rhs.x), (lhs.r-rhs.y), (lhs.r-rhs.z)); };
		//template<typename TYPE> LYNXFORCEINLINE CVector3 operator -(const CVector3& lhs, const TYPE& rhs) { return CVector3((lhs.x-CReal(rhs)), (lhs.y-CReal(rhs)), (lhs.z-CReal(rhs))); };
		//template<typename TYPE> LYNXFORCEINLINE CVector3 operator -(const TYPE& lhs, const CVector3& rhs) { return CVector3((CReal(lhs)-rhs.x), (CReal(lhs)-rhs.y), (CReal(lhs)-rhs.z)); };
		LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const CVector3& rhs) { return CVector3((lhs.x*rhs.x), (lhs.y*rhs.y), (lhs.z*rhs.z)); };		
		LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const CReal& rhs) { return CVector3((lhs.x*rhs.r), (lhs.y*rhs.r), (lhs.z*rhs.r)); };		
		LYNXFORCEINLINE CVector3 operator *(const CReal& lhs, const CVector3& rhs) { return CVector3((lhs.r*rhs.x), (lhs.r*rhs.y), (lhs.r*rhs.z)); };
		//template<typename TYPE> LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const TYPE& rhs) { return CVector3((lhs.x*CReal(rhs)), (lhs.y*CReal(rhs)), (lhs.z*CReal(rhs))); };
		//template<typename TYPE> LYNXFORCEINLINE CVector3 operator *(const TYPE& lhs, const CVector3& rhs) { return CVector3((CReal(lhs)*rhs.x), (CReal(lhs)*rhs.y), (CReal(lhs)*rhs.z)); };
		LYNXFORCEINLINE CVector3 operator /(const CVector3& lhs, const CVector3& rhs) { return CVector3((lhs.x/rhs.x), (lhs.y/rhs.y), (lhs.z/rhs.z)); };		
		LYNXFORCEINLINE CVector3 operator /(const CVector3& lhs, const CReal& rhs) { return CVector3((lhs.x/rhs.r), (lhs.y/rhs.r), (lhs.z/rhs.r)); };		
		LYNXFORCEINLINE CVector3 operator /(const CReal& lhs, const CVector3& rhs) { return CVector3((lhs.r/rhs.x), (lhs.r/rhs.y), (lhs.r/rhs.z)); };
		//template<typename TYPE> LYNXFORCEINLINE CVector3 operator /(const CVector3& lhs, const TYPE& rhs) { return CVector3((lhs.x/CReal(rhs)), (lhs.y/CReal(rhs)), (lhs.z/CReal(rhs))); };
		//template<typename TYPE> LYNXFORCEINLINE CVector3 operator /(const TYPE& lhs, const CVector3& rhs) { return CVector3((CReal(lhs)/rhs.x), (CReal(lhs)/rhs.y), (CReal(lhs)/rhs.z)); };
		LYNXFORCEINLINE bool operator ==(const CVector3& lhs, const CVector3& rhs) { return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z)); };				

		LYNXFORCEINLINE CReal Dot(const CVector3& v1, const CVector3& v2) { return ((v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z)); };
		LYNXFORCEINLINE CVector3 Cross(const CVector3& a, const CVector3& b) { LYNXVECTOR3D V; lynxCrossProduct(&V, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); return CVector3(&V); };
		LYNXFORCEINLINE void Cross(CVector3& r, const CVector3& a, const CVector3& b) { lynxCrossProduct((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); };

		//class LYNXENGCLASS CMatrix3
		//{			
		//public:
		//	union {
		//		struct {
		//			CReal m11, m12, m13;	
		//			CReal m21, m22, m23;	
		//			CReal m31, m32, m33;						
		//		};
		//		//CReal m[3][3];
		//	};
		//public:						
		//	CMatrix3() {Identity(); };
		//	CMatrix3(LYNXMATRIX& m) {m11 = m.s.m11; m12 = m.s.m12; m13 = m.s.m13; 
		//						     m21 = m.s.m21; m22 = m.s.m22; m23 = m.s.m23; 
		//						     m31 = m.s.m31; m32 = m.s.m32; m33 = m.s.m33; };			
		//	CMatrix3(CReal& m_11, CReal& m_12, CReal& m_13,
		//			 CReal& m_21, CReal& m_22, CReal& m_23,
		//			 CReal& m_31, CReal& m_32, CReal& m_33) { 
		//				m11 = m_11; m12 = m_12; m13 = m_13; 
		//				m21 = m_21; m22 = m_22; m23 = m_23; 
		//				m31 = m_31; m32 = m_32; m33 = m_33; };
		//	~CMatrix3() {};	

		//	LYNXFORCEINLINE void Identity() { LYNX_IDENTITY_MATRIX3((*((LPLYNXMATRIX)(this))));};						

		//	LYNXFORCEINLINE operator LYNXMATRIX3() {LYNXMATRIX3 M; LYNX_MATRIX3(M, m11, m12, m13, m21, m22, m23, m31, m32, m33); return (M);};						
		//	LYNXFORCEINLINE operator LPLYNXMATRIX3() {return ((LPLYNXMATRIX3)&m11);};						
		//	LYNXFORCEINLINE CMatrix3& operator =(const CMatrix3& rhs) { m11 = rhs.m11; m12 = rhs.m12; m13 = rhs.m13; 
		//														   m21 = rhs.m21; m22 = rhs.m22; m23 = rhs.m23; 
		//														   m31 = rhs.m31; m32 = rhs.m32; m33 = rhs.m33; return (*this); };						

		//	//LYNXFORCEINLINE void Normalise(int dim) { lynxNormalise((LPLYNXVECTOR3D)this, dim); };
		//	//LYNXFORCEINLINE CReal Length(int dim) { return CReal(lynxLength((LPLYNXVECTOR3D)this, dim)); };			
		//};				

		////LYNXFORCEINLINE CMatrix4 operator *(const CMatrix4& lhs, const CMatrix4& rhs) { LYNXMATRIX M; lynxMatrixXMatrix(&M, (LPLYNXMATRIX)&lhs, (LPLYNXMATRIX)&rhs); return CMatrix4(M); };		
		//LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const CMatrix3& rhs) { LYNXVECTOR3D V; lynxVectorXMatrix3(&V, (LPLYNXVECTOR3D)&lhs, (LPLYNXMATRIX3)&rhs); return CVector3(&V); };		

		//class LYNXENGCLASS CMatrix4
		//{			
		//public:
		//	union {
		//		struct {
		//			CReal m11, m12, m13, m14;	
		//			CReal m21, m22, m23, m24;	
		//			CReal m31, m32, m33, m34;	
		//			CReal m41, m42, m43, m44;	
		//		};
		//		//CReal m[4][4];
		//	};
		//public:						
		//	CMatrix4() {Identity(); };
		//	CMatrix4(LYNXMATRIX& m) {m11 = m.s.m11; m12 = m.s.m12; m13 = m.s.m13; m13 = m.s.m14;
		//						     m21 = m.s.m21; m22 = m.s.m22; m23 = m.s.m23; m23 = m.s.m24;
		//						     m31 = m.s.m31; m32 = m.s.m32; m33 = m.s.m33; m33 = m.s.m34;
		//						     m41 = m.s.m41; m42 = m.s.m42; m43 = m.s.m43; m43 = m.s.m44; };			
		//	CMatrix4(CReal& m_11, CReal& m_12, CReal& m_13, CReal& m_14,
		//			 CReal& m_21, CReal& m_22, CReal& m_23, CReal& m_24,
		//			 CReal& m_31, CReal& m_32, CReal& m_33, CReal& m_34,
		//			 CReal& m_41, CReal& m_42, CReal& m_43, CReal& m_44) { 
		//				m11 = m_11; m12 = m_12; m13 = m_13; m13 = m_14;
		//				m21 = m_21; m22 = m_22; m23 = m_23; m23 = m_24;
		//				m31 = m_31; m32 = m_32; m33 = m_33; m33 = m_34;
		//				m41 = m_41; m42 = m_42; m43 = m_43; m43 = m_44; };
		//	~CMatrix4() {};	

		//	LYNXFORCEINLINE void Identity() { LYNX_IDENTITY_MATRIX((*((LPLYNXMATRIX)(this))));};						

		//	LYNXFORCEINLINE operator LYNXMATRIX() {LYNXMATRIX M; LYNX_MATRIX(M, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44); return (M);};						
		//	LYNXFORCEINLINE operator LPLYNXMATRIX() {return ((LPLYNXMATRIX)&m11);};						
		//	LYNXFORCEINLINE CMatrix4& operator =(const CMatrix4& rhs) { m11 = rhs.m11; m12 = rhs.m12; m13 = rhs.m13; m14 = rhs.m14;
		//														   m21 = rhs.m21; m22 = rhs.m22; m23 = rhs.m23; m24 = rhs.m24;
		//														   m31 = rhs.m31; m32 = rhs.m32; m33 = rhs.m33; m34 = rhs.m34;
		//														   m41 = rhs.m41; m42 = rhs.m42; m43 = rhs.m43; m44 = rhs.m44; return (*this); };						

		//	//LYNXFORCEINLINE void Normalise(int dim) { lynxNormalise((LPLYNXVECTOR3D)this, dim); };
		//	//LYNXFORCEINLINE CReal Length(int dim) { return CReal(lynxLength((LPLYNXVECTOR3D)this, dim)); };			
		//};				

		//LYNXFORCEINLINE CMatrix4 operator *(const CMatrix4& lhs, const CMatrix4& rhs) { LYNXMATRIX M; lynxMatrixXMatrix(&M, (LPLYNXMATRIX)&lhs, (LPLYNXMATRIX)&rhs); return CMatrix4(M); };		
		//LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const CMatrix4& rhs) { LYNXVECTOR3D V; lynxPointXMatrix(&V, (LPLYNXVECTOR3D)&lhs, (LPLYNXMATRIX)&rhs); return CVector3(&V); };				

		//LYNXFORCEINLINE void Transform(CVector3& r, const CVector3& a, const CMatrix4& b) { lynxPointXMatrix((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXMATRIX)&b); };
		//LYNXFORCEINLINE CVector3 Transform(const CVector3& a, const CMatrix4& b) { LYNXVECTOR3D V; lynxPointXMatrix(&V, (LPLYNXVECTOR3D)&a, (LPLYNXMATRIX)&b); return CVector3(&V); };
		//LYNXFORCEINLINE void Orient(CVector3& r, const CVector3& a, const CMatrix4& b) { lynxVectorXMatrix((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXMATRIX)&b); };
		//LYNXFORCEINLINE CVector3 Orient(const CVector3& a, const CMatrix4& b) { LYNXVECTOR3D V; lynxVectorXMatrix(&V, (LPLYNXVECTOR3D)&a, (LPLYNXMATRIX)&b); return CVector3(&V); };
	}
}

#endif