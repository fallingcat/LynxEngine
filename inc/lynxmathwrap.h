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

#include <LynxMath.h>
#include <LynxEngineClasses.h>

namespace LynxEngine
{
	namespace Math
	{
		/**	
		*  @brief Real class of the engine.
		* 
		*  The basic object class of the entire engine, every class of the engine is drived from this class.
		*  It records a unique ID and a reference counter. 
		*	
		*/
		class LYNXENGCLASS CReal
		{			
		public:
			LYNXREAL				r;
		public:						
			LYNXFORCEINLINE CReal() {r = LYNX_REAL(0.0f); };
			LYNXFORCEINLINE CReal(LYNXREAL v) {r = v; };
			#ifndef __FIXED_POINT__
				LYNXFORCEINLINE CReal(int v) {r = LYNX_REAL(v); };
			#endif
			#ifndef __SINGLE__
				LYNXFORCEINLINE CReal(float v) {r = LYNX_REAL(v); };						
			#endif	
			#ifndef __DOUBLE__
				LYNXFORCEINLINE CReal(double v) {r = LYNX_REAL(v); };													
			#endif
			~CReal() {};	

			LYNXFORCEINLINE operator LYNXREAL() const {return r;};			
			LYNXFORCEINLINE operator LYNXREAL*() {return (&r);};						
			LYNXFORCEINLINE CReal& operator =(const CReal& rhs) { r = rhs.r; return (*this); };									
			LYNXFORCEINLINE void operator += (const CReal& rhs) { r = LYNX_REAL_ADD(r, rhs.r); };						
			LYNXFORCEINLINE void operator -= (const CReal& rhs) { r = LYNX_REAL_SUB(r, rhs.r); };						
			LYNXFORCEINLINE void operator *= (const CReal& rhs) { r = LYNX_REAL_MUL(r, rhs.r);};						
			LYNXFORCEINLINE void operator /= (const CReal& rhs) { r = LYNX_REAL_DIV(r, rhs.r);};						
			LYNXFORCEINLINE CReal& operator ++() { r = LYNX_REAL_ADD(r, LYNX_REAL(1)); return (*this); };						
			LYNXFORCEINLINE CReal& operator --() { r = LYNX_REAL_SUB(r, LYNX_REAL(1)); return (*this); };
			LYNXFORCEINLINE int Round() { return LYNX_ROUND(r); };
			LYNXFORCEINLINE int Floor() { return LYNX_FLOOR(r); };
			LYNXFORCEINLINE int Ceil() { return LYNX_CEIL(r); };

			//LYNXFORCEINLINE bool operator >(const CReal& rhs) { return (r > rhs.r); };
		};						
		// Return LYNXREAL instead of CReal and let the compiler choose proper cast operator, then the compiler will not use the call instruct for operator +.
		// This method can avoid the compiler to use the copy constructor of CReal.		
		LYNXFORCEINLINE LYNXREAL operator +(const CReal& lhs, const LYNXREAL& rhs) { return (LYNX_REAL_ADD(lhs.r, (rhs))); };
		LYNXFORCEINLINE LYNXREAL operator +(const CReal& lhs, const CReal& rhs) { return (LYNX_REAL_ADD(lhs.r, rhs.r)); };						
		LYNXFORCEINLINE LYNXREAL operator +(const LYNXREAL& lhs, const CReal& rhs) { return (LYNX_REAL_ADD((lhs), rhs.r)); };	

		LYNXFORCEINLINE LYNXREAL operator -(const CReal& lhs, const LYNXREAL& rhs) { return (LYNX_REAL_SUB(lhs.r, (rhs))); };
		LYNXFORCEINLINE LYNXREAL operator -(const CReal& lhs, const CReal& rhs) { return (LYNX_REAL_SUB(lhs.r, rhs.r)); };				
		LYNXFORCEINLINE LYNXREAL operator -(const LYNXREAL& lhs, const CReal& rhs) { return (LYNX_REAL_SUB((lhs), rhs.r)); };	

		LYNXFORCEINLINE LYNXREAL operator *(const CReal& lhs, const LYNXREAL& rhs) { return (LYNX_REAL_MUL(lhs.r, (rhs))); };
		LYNXFORCEINLINE LYNXREAL operator *(const CReal& lhs, const CReal& rhs) { return (LYNX_REAL_MUL(lhs.r, rhs.r)); };				
		LYNXFORCEINLINE LYNXREAL operator *(const LYNXREAL& lhs, const CReal& rhs) { return (LYNX_REAL_MUL((lhs), rhs.r)); };	

		LYNXFORCEINLINE LYNXREAL operator /(const CReal& lhs, const LYNXREAL& rhs) { return (LYNX_REAL_DIV(lhs.r, (rhs))); };
		LYNXFORCEINLINE LYNXREAL operator /(const CReal& lhs, const CReal& rhs) { return (LYNX_REAL_DIV(lhs.r, rhs.r)); };				
		LYNXFORCEINLINE LYNXREAL operator /(const LYNXREAL& lhs, const CReal& rhs) { return (LYNX_REAL_DIV((lhs), rhs.r)); };			

		LYNXFORCEINLINE bool operator ==(const CReal& lhs, const LYNXREAL& rhs) { return (lhs.r == rhs); };
		LYNXFORCEINLINE bool operator ==(const CReal& lhs, const CReal& rhs) { return (lhs.r == rhs.r); };				
		LYNXFORCEINLINE bool operator ==(const LYNXREAL& lhs, const CReal& rhs) { return (lhs == rhs.r); };

		LYNXFORCEINLINE bool operator !=(const CReal& lhs, const LYNXREAL& rhs) { return (lhs.r != rhs); };
		LYNXFORCEINLINE bool operator !=(const CReal& lhs, const CReal& rhs) { return (lhs.r != rhs.r); };					
		LYNXFORCEINLINE bool operator !=(const LYNXREAL& lhs, const CReal& rhs) { return (lhs != rhs.r); };

		LYNXFORCEINLINE bool operator >(const CReal& lhs, const LYNXREAL& rhs) { return (lhs.r > rhs); };
		LYNXFORCEINLINE bool operator >(const CReal& lhs, const CReal& rhs) { return (lhs.r > rhs.r); };				
		LYNXFORCEINLINE bool operator >(const LYNXREAL& lhs, const CReal& rhs) { return (lhs > rhs.r); };
				
		LYNXFORCEINLINE bool operator >=(const CReal& lhs, const LYNXREAL& rhs) { return (lhs.r >= rhs); };
		LYNXFORCEINLINE bool operator >=(const CReal& lhs, const CReal& rhs) { return (lhs.r >= rhs.r); };				
		LYNXFORCEINLINE bool operator >=(const LYNXREAL& lhs, const CReal& rhs) { return (lhs >= rhs.r); };

		LYNXFORCEINLINE bool operator <(const CReal& lhs, const LYNXREAL& rhs) { return (lhs.r < rhs); };
		LYNXFORCEINLINE bool operator <(const CReal& lhs, const CReal& rhs) { return (lhs.r < rhs.r); };				
		LYNXFORCEINLINE bool operator <(const LYNXREAL& lhs, const CReal& rhs) { return (lhs < rhs.r); };		

		LYNXFORCEINLINE bool operator <=(const CReal& lhs, const LYNXREAL& rhs) { return (lhs.r <= rhs); };
		LYNXFORCEINLINE bool operator <=(const CReal& lhs, const CReal& rhs) { return (lhs.r <= rhs.r); };				
		LYNXFORCEINLINE bool operator <=(const LYNXREAL& lhs, const CReal& rhs) { return (lhs <= rhs.r); };		

		#ifndef __FIXED_POINT__			
			LYNXFORCEINLINE LYNXREAL operator +(const int& lhs, const CReal& rhs) { return (LYNX_REAL_ADD(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator +(const CReal& lhs, const int& rhs) { return (LYNX_REAL_ADD(lhs.r, LYNX_REAL(rhs))); };	
			LYNXFORCEINLINE LYNXREAL operator -(const int& lhs, const CReal& rhs) { return (LYNX_REAL_SUB(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator -(const CReal& lhs, const int& rhs) { return (LYNX_REAL_SUB(lhs.r, LYNX_REAL(rhs))); };	
			LYNXFORCEINLINE LYNXREAL operator *(const int& lhs, const CReal& rhs) { return (LYNX_REAL_MUL(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator *(const CReal& lhs, const int& rhs) { return (LYNX_REAL_MUL(lhs.r, LYNX_REAL(rhs))); };		
			LYNXFORCEINLINE LYNXREAL operator /(const CReal& lhs, const int& rhs) { return (LYNX_REAL_DIV(lhs.r, LYNX_REAL(rhs))); };	
			LYNXFORCEINLINE LYNXREAL operator /(const int& lhs, const CReal& rhs) { return (LYNX_REAL_DIV(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE bool operator ==(const int& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) ==  rhs.r); };
			LYNXFORCEINLINE bool operator ==(const CReal& lhs, const int& rhs) { return (lhs.r ==  LYNX_REAL(rhs)); };		
			LYNXFORCEINLINE bool operator !=(const int& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) !=  rhs.r); };
			LYNXFORCEINLINE bool operator !=(const CReal& lhs, const int& rhs) { return (lhs.r !=  LYNX_REAL(rhs)); };		

			LYNXFORCEINLINE LYNXREAL operator +(const unsigned int& lhs, const CReal& rhs) { return (LYNX_REAL_ADD(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator +(const CReal& lhs, const unsigned int& rhs) { return (LYNX_REAL_ADD(lhs.r, LYNX_REAL(rhs))); };	
			LYNXFORCEINLINE LYNXREAL operator -(const unsigned int& lhs, const CReal& rhs) { return (LYNX_REAL_SUB(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator -(const CReal& lhs, const unsigned int& rhs) { return (LYNX_REAL_SUB(lhs.r, LYNX_REAL(rhs))); };	
			LYNXFORCEINLINE LYNXREAL operator *(const unsigned int& lhs, const CReal& rhs) { return (LYNX_REAL_MUL(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator *(const CReal& lhs, const unsigned int& rhs) { return (LYNX_REAL_MUL(lhs.r, LYNX_REAL(rhs))); };		
			LYNXFORCEINLINE LYNXREAL operator /(const CReal& lhs, const unsigned int& rhs) { return (LYNX_REAL_DIV(lhs.r, LYNX_REAL(rhs))); };	
			LYNXFORCEINLINE LYNXREAL operator /(const unsigned int& lhs, const CReal& rhs) { return (LYNX_REAL_DIV(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE bool operator ==(const unsigned int& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) ==  rhs.r); };
			LYNXFORCEINLINE bool operator ==(const CReal& lhs, const unsigned int& rhs) { return (lhs.r ==  LYNX_REAL(rhs)); };		
			LYNXFORCEINLINE bool operator !=(const unsigned int& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) !=  rhs.r); };
			LYNXFORCEINLINE bool operator !=(const CReal& lhs, const unsigned int& rhs) { return (lhs.r !=  LYNX_REAL(rhs)); };		
		#endif
		#ifndef __SINGLE__			
			LYNXFORCEINLINE LYNXREAL operator +(const float& lhs, const CReal& rhs) { return (LYNX_REAL_ADD(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator +(const CReal& lhs, const float& rhs) { return (LYNX_REAL_ADD(lhs.r, LYNX_REAL(rhs))); };	
			LYNXFORCEINLINE LYNXREAL operator -(const float& lhs, const CReal& rhs) { return (LYNX_REAL_SUB(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator -(const CReal& lhs, const float& rhs) { return (LYNX_REAL_SUB(lhs.r, LYNX_REAL(rhs))); };		
			LYNXFORCEINLINE LYNXREAL operator *(const float& lhs, const CReal& rhs) { return (LYNX_REAL_MUL(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator *(const CReal& lhs, const float& rhs) { return (LYNX_REAL_MUL(lhs.r, LYNX_REAL(rhs))); };			
			LYNXFORCEINLINE LYNXREAL operator /(const float& lhs, const CReal& rhs) { return (LYNX_REAL_DIV(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator /(const CReal& lhs, const float& rhs) { return (LYNX_REAL_DIV(lhs.r, LYNX_REAL(rhs))); };		
			LYNXFORCEINLINE bool operator ==(const float& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) == rhs.r); };
			LYNXFORCEINLINE bool operator ==(const CReal& lhs, const float& rhs) { return (lhs.r == LYNX_REAL(rhs)); };		
			LYNXFORCEINLINE bool operator !=(const float& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) != rhs.r); };
			LYNXFORCEINLINE bool operator !=(const CReal& lhs, const float& rhs) { return (lhs.r != LYNX_REAL(rhs)); };		
		#endif	
		#ifndef __DOUBLE__			
			LYNXFORCEINLINE LYNXREAL operator +(const double& lhs, const CReal& rhs) { return (LYNX_REAL_ADD(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator +(const CReal& lhs, const double& rhs) { return (LYNX_REAL_ADD(lhs.r, LYNX_REAL(rhs))); };					
			LYNXFORCEINLINE LYNXREAL operator -(const double& lhs, const CReal& rhs) { return (LYNX_REAL_SUB(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator -(const CReal& lhs, const double& rhs) { return (LYNX_REAL_SUB(lhs.r, LYNX_REAL(rhs))); };		
			LYNXFORCEINLINE LYNXREAL operator *(const double& lhs, const CReal& rhs) { return (LYNX_REAL_MUL(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator *(const CReal& lhs, const double& rhs) { return (LYNX_REAL_MUL(lhs.r, LYNX_REAL(rhs))); };			
			LYNXFORCEINLINE LYNXREAL operator /(const double& lhs, const CReal& rhs) { return (LYNX_REAL_DIV(LYNX_REAL(lhs), rhs.r)); };
			LYNXFORCEINLINE LYNXREAL operator /(const CReal& lhs, const double& rhs) { return (LYNX_REAL_DIV(lhs.r, LYNX_REAL(rhs))); };		
			LYNXFORCEINLINE bool operator ==(const double& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) == rhs.r); };
			LYNXFORCEINLINE bool operator ==(const CReal& lhs, const double& rhs) { return (lhs.r == LYNX_REAL(rhs)); };		
			LYNXFORCEINLINE bool operator !=(const double& lhs, const CReal& rhs) { return (LYNX_REAL(lhs) != rhs.r); };
			LYNXFORCEINLINE bool operator !=(const CReal& lhs, const double& rhs) { return (lhs.r != LYNX_REAL(rhs)); };		
		#endif
		
		/**	
		*  @brief CVector2 class of the engine.
		* 
		*  The basic object class of the entire engine, every class of the engine is drived from this class.
		*  It records a unique ID and a reference counter. 
		*	
		*/
		class LYNXENGCLASS CVector2 : public LYNXVECTOR2D
		{			
		public:						
			LYNXFORCEINLINE CVector2() {x = y = 0; };
			LYNXFORCEINLINE CVector2(LYNXVECTOR2D& v) {x = v.x; y = v.y; };			
			LYNXFORCEINLINE CVector2(const CReal& _x, const CReal& _y) {x = _x; y = _y; };			
			LYNXFORCEINLINE CVector2(const CReal& v) {x = y = v; };			
			~CVector2() {};	
			
			LYNXFORCEINLINE operator LPLYNXVECTOR2D() const {return ((LPLYNXVECTOR2D)this);};						
			//LYNXFORCEINLINE operator LYNXVECTOR2D() const {return (*((LPLYNXVECTOR2D)this));};												
			LYNXFORCEINLINE CVector2& operator =(const CVector2& rhs) { x = rhs.x; y = rhs.y; return (*this); };						
			LYNXFORCEINLINE CVector2& operator =(const LYNXVECTOR3D& rhs) { x = rhs.x; y = rhs.y; return (*this); };									
			LYNXFORCEINLINE CVector2& operator =(const float& rhs) { x = y = rhs; return (*this); };

			LYNXFORCEINLINE void operator += (const CVector2& rhs) { x = LYNX_REAL_ADD(x, rhs.x); y = LYNX_REAL_ADD(y, rhs.y); };
			LYNXFORCEINLINE void operator += (const CReal& rhs) { x = LYNX_REAL_ADD(x, rhs.r); y = LYNX_REAL_ADD(y, rhs.r); };
			LYNXFORCEINLINE void operator -= (const CVector2& rhs) { x = LYNX_REAL_SUB(x, rhs.x); y = LYNX_REAL_SUB(y, rhs.y); };
			LYNXFORCEINLINE void operator -= (const CReal& rhs) { x = LYNX_REAL_SUB(x, rhs.r); y = LYNX_REAL_SUB(y, rhs.r); };
			LYNXFORCEINLINE void operator *= (const CVector2& rhs) { x = LYNX_REAL_MUL(x, rhs.x); y = LYNX_REAL_MUL(y, rhs.y); };
			LYNXFORCEINLINE void operator *= (const CReal& rhs) { x = LYNX_REAL_MUL(x, rhs.r); y = LYNX_REAL_MUL(y, rhs.r); };
			LYNXFORCEINLINE void operator /= (const CVector2& rhs) { x = LYNX_REAL_DIV(x, rhs.x); y = LYNX_REAL_DIV(y, rhs.y); };
			LYNXFORCEINLINE void operator /= (const CReal& rhs) { x = LYNX_REAL_DIV(x, rhs.r); y = LYNX_REAL_DIV(y, rhs.r); };

			LYNXFORCEINLINE void Normalise(const int dim = 2) { lynxNormalise2((LPLYNXVECTOR2D)this, dim); };
			LYNXFORCEINLINE LYNXREAL Length(const int dim = 2) { return lynxLength2((LPLYNXVECTOR2D)this, dim); };						
			LYNXFORCEINLINE void Abs() { x = fabs(x); y = fabs(y); };						
		};	

		LYNXFORCEINLINE CVector2 operator +(const CVector2& lhs, const CVector2& rhs) { CVector2 V; LYNX_VECTOR2D_ADD(V, (lhs), (rhs)); return V;};		
		LYNXFORCEINLINE CVector2 operator +(const CVector2& lhs, const CReal& rhs) { CVector2 V; LYNX_VECTOR2D(V, LYNX_REAL_ADD(lhs.x, rhs.r), LYNX_REAL_ADD(lhs.y, rhs.r)); return V; };
		LYNXFORCEINLINE CVector2 operator +(const CReal& lhs, const CVector2& rhs) { CVector2 V; LYNX_VECTOR2D(V, LYNX_REAL_ADD(lhs.r, rhs.x), LYNX_REAL_ADD(lhs.r, rhs.y)); return V; };		

		LYNXFORCEINLINE CVector2 operator -(const CVector2& lhs, const CVector2& rhs) { CVector2 V; LYNX_VECTOR2D_SUB(V, (lhs), (rhs)); return V;};		
		LYNXFORCEINLINE CVector2 operator -(const CVector2& lhs, const CReal& rhs) { CVector2 V; LYNX_VECTOR2D(V, LYNX_REAL_SUB(lhs.x, rhs.r), LYNX_REAL_SUB(lhs.y, rhs.r)); return V; };
		LYNXFORCEINLINE CVector2 operator -(const CReal& lhs, const CVector2& rhs) { CVector2 V; LYNX_VECTOR2D(V, LYNX_REAL_SUB(lhs.r, rhs.x), LYNX_REAL_SUB(lhs.r, rhs.y)); return V; };		

		LYNXFORCEINLINE CVector2 operator *(const CVector2& lhs, const CVector2& rhs) { CVector2 V;  LYNX_VECTOR2D(V, LYNX_REAL_MUL(lhs.x, rhs.x), LYNX_REAL_MUL(lhs.y, rhs.y)); return V;};		
		LYNXFORCEINLINE CVector2 operator *(const CVector2& lhs, const CReal& rhs) { CVector2 V; LYNX_VECTOR2D(V, LYNX_REAL_MUL(lhs.x, rhs.r), LYNX_REAL_MUL(lhs.y, rhs.r)); return V; };
		LYNXFORCEINLINE CVector2 operator *(const CReal& lhs, const CVector2& rhs) { CVector2 V; LYNX_VECTOR2D(V, LYNX_REAL_MUL(lhs.r, rhs.x), LYNX_REAL_MUL(lhs.r, rhs.y)); return V; };

		LYNXFORCEINLINE CVector2 operator /(const CVector2& lhs, const CVector2& rhs) { CVector2 V;  LYNX_VECTOR2D(V, LYNX_REAL_DIV(lhs.x, rhs.x), LYNX_REAL_DIV(lhs.y, rhs.y)); return V;};		
		LYNXFORCEINLINE CVector2 operator /(const CVector2& lhs, const CReal& rhs) { CVector2 V; LYNX_VECTOR2D(V, LYNX_REAL_DIV(lhs.x, rhs.r), LYNX_REAL_DIV(lhs.y, rhs.r)); return V; };
		LYNXFORCEINLINE CVector2 operator /(const CReal& lhs, const CVector2& rhs) { CVector2 V; LYNX_VECTOR2D(V, LYNX_REAL_DIV(lhs.r, rhs.x), LYNX_REAL_DIV(lhs.r, rhs.y)); return V; };
		
		LYNXFORCEINLINE void Vector2Add(CVector2& r, const CVector2& a, const CVector2& b) { LYNX_VECTOR2D(r, LYNX_REAL_ADD(a.x, b.x), LYNX_REAL_ADD(a.y, b.y)); };
		LYNXFORCEINLINE void Vector2Add(CVector2& r, const CVector2& a, const CReal& b) { LYNX_VECTOR2D(r, LYNX_REAL_ADD(a.x, b.r), LYNX_REAL_ADD(a.y, b.r)); };
		LYNXFORCEINLINE void Vector2Add(CVector2& r, const CReal& a, const CVector2& b) { LYNX_VECTOR2D(r, LYNX_REAL_ADD(a.r, b.x), LYNX_REAL_ADD(a.r, b.y)); };
		LYNXFORCEINLINE void Vector2Sub(CVector2& r, const CVector2& a, const CVector2& b) { LYNX_VECTOR2D(r, LYNX_REAL_SUB(a.x, b.x), LYNX_REAL_SUB(a.y, b.y)); };
		LYNXFORCEINLINE void Vector2Sub(CVector2& r, const CVector2& a, const CReal& b) { LYNX_VECTOR2D(r, LYNX_REAL_SUB(a.x, b.r), LYNX_REAL_SUB(a.y, b.r)); };
		LYNXFORCEINLINE void Vector2Sub(CVector2& r, const CReal& a, const CVector2& b) { LYNX_VECTOR2D(r, LYNX_REAL_SUB(a.r, b.x), LYNX_REAL_SUB(a.r, b.y)); };
		LYNXFORCEINLINE void Vector2Mul(CVector2& r, const CVector2& a, const CVector2& b) { LYNX_VECTOR2D(r, LYNX_REAL_MUL(a.x, b.x), LYNX_REAL_MUL(a.y, b.y)); };
		LYNXFORCEINLINE void Vector2Mul(CVector2& r, const CVector2& a, const CReal& b) { LYNX_VECTOR2D(r, LYNX_REAL_MUL(a.x, b.r), LYNX_REAL_MUL(a.y, b.r)); };
		LYNXFORCEINLINE void Vector2Mul(CVector2& r, const CReal& a, const CVector2& b) { LYNX_VECTOR2D(r, LYNX_REAL_MUL(a.r, b.x), LYNX_REAL_MUL(a.r, b.y)); };
		LYNXFORCEINLINE void Vector2Div(CVector2& r, const CVector2& a, const CVector2& b) { LYNX_VECTOR2D(r, LYNX_REAL_DIV(a.x, b.x), LYNX_REAL_DIV(a.y, b.y)); };		
		LYNXFORCEINLINE void Vector2Div(CVector2& r, const CVector2& a, const CReal& b) { LYNX_VECTOR2D(r, LYNX_REAL_DIV(a.x, b.r), LYNX_REAL_DIV(a.y, b.r)); };
		LYNXFORCEINLINE void Vector2Div(CVector2& r, const CReal& a, const CVector2& b) { LYNX_VECTOR2D(r, LYNX_REAL_DIV(a.r, b.x), LYNX_REAL_DIV(a.r, b.y)); };

		//LYNXFORCEINLINE LYNXREAL Dot(const CVector2& v1, const CVector3& v2) { return (LYNX_DOT_PRODUCT(v1, v2)); };
		//LYNXFORCEINLINE CVector2 Cross(const CVector2& a, const CVector3& b) { CVec	tor3 V; lynxCrossProduct(&V, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); return (V); };
		//LYNXFORCEINLINE void Cross(CVector3& r, const CVector3& a, const CVector3& b) { lynxCrossProduct((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); };

		/**	
		*  @brief CVector3 class of the engine.
		* 
		*  The basic object class of the entire engine, every class of the engine is drived from this class.
		*  It records a unique ID and a reference counter. 
		*	
		*/
		class LYNXENGCLASS CVector3 : public LYNXVECTOR3D
		{			
		public:						
			LYNXFORCEINLINE CVector3() {x = y = z = 0; };
			LYNXFORCEINLINE CVector3(LYNXVECTOR3D& v) {x = v.x; y = v.y; z= v.z; };		
			LYNXFORCEINLINE CVector3(LYNXCOLORRGB& c) {r = c.Red/255.0f; g = c.Green/255.0f; b = c.Blue/255.0f; };
			LYNXFORCEINLINE CVector3(const CReal& _x, const CReal& _y, const CReal& _z) {x = _x; y = _y; z = _z; };			
			LYNXFORCEINLINE CVector3(const CReal& v) {x = y = z = v; };			
			~CVector3() {};	
			
			LYNXFORCEINLINE operator LPLYNXVECTOR3D() const {return ((LPLYNXVECTOR3D)this);};						
			//LYNXFORCEINLINE operator LYNXVECTOR3D() const {return (*((LPLYNXVECTOR3D)this));};												
			LYNXFORCEINLINE CVector3& operator =(const CVector3& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return (*this); };						
			LYNXFORCEINLINE CVector3& operator =(const LYNXVECTOR3D& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return (*this); };						
			//LYNXFORCEINLINE CVector3& operator =(const LYNXVECTOR3D rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return (*this); };					
			LYNXFORCEINLINE CVector3& operator =(const float& rhs) { x = y = z = rhs; return (*this); };

			LYNXFORCEINLINE bool operator ==(const CVector3& rhs) { return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z)); };						
			LYNXFORCEINLINE void operator += (const CVector3& rhs) { x = LYNX_REAL_ADD(x, rhs.x); y = LYNX_REAL_ADD(y, rhs.y); z = LYNX_REAL_ADD(z, rhs.z);};
			LYNXFORCEINLINE void operator += (const CReal& rhs) { x = LYNX_REAL_ADD(x, rhs.r); y = LYNX_REAL_ADD(y, rhs.r); z = LYNX_REAL_ADD(z, rhs.r);};
			LYNXFORCEINLINE void operator -= (const CVector3& rhs) { x = LYNX_REAL_SUB(x, rhs.x); y = LYNX_REAL_SUB(y, rhs.y); z = LYNX_REAL_SUB(z, rhs.z);};
			LYNXFORCEINLINE void operator -= (const CReal& rhs) { x = LYNX_REAL_SUB(x, rhs.r); y = LYNX_REAL_SUB(y, rhs.r); z = LYNX_REAL_SUB(z, rhs.r);};
			LYNXFORCEINLINE void operator *= (const CVector3& rhs) { x = LYNX_REAL_MUL(x, rhs.x); y = LYNX_REAL_MUL(y, rhs.y); z = LYNX_REAL_MUL(z, rhs.z);};
			LYNXFORCEINLINE void operator *= (const CReal& rhs) { x = LYNX_REAL_MUL(x, rhs.r); y = LYNX_REAL_MUL(y, rhs.r); z = LYNX_REAL_MUL(z, rhs.r);};
			LYNXFORCEINLINE void operator /= (const CVector3& rhs) { x = LYNX_REAL_DIV(x, rhs.x); y = LYNX_REAL_DIV(y, rhs.y); z = LYNX_REAL_DIV(z, rhs.z);};
			LYNXFORCEINLINE void operator /= (const CReal& rhs) { x = LYNX_REAL_DIV(x, rhs.r); y = LYNX_REAL_DIV(y, rhs.r); z = LYNX_REAL_DIV(z, rhs.r);};

			LYNXFORCEINLINE void Normalise(const int dim = 3) { lynxNormalise((LPLYNXVECTOR3D)this, dim); };
			LYNXFORCEINLINE const LYNXREAL Length(const int dim = 3) const { return lynxLength((LPLYNXVECTOR3D)this, dim); };						
			LYNXFORCEINLINE void Abs() { x = fabs(x); y = fabs(y); z = fabs(z); };						
		};	
		
		//typedef struct {
		//	CReal x, y, z;
		//}VEC3;				

		//class LYNXENGCLASS CVector3
		//{			
		//public:			
		//	VEC3					s;
		//public:						
		//	LYNXFORCEINLINE CVector3() {x = y = z = 0; };
		//	LYNXFORCEINLINE CVector3(LYNXVECTOR3D& v) {x = v.x; y = v.y; z= v.z; };			
		//	LYNXFORCEINLINE CVector3(const CReal& vx, const CReal& vy, const CReal& vz) {x = vx.r; y = vy.r; z= vz.r; };			
		//	~CVector3() {};	
		//	
		//	//LYNXFORCEINLINE operator LPLYNXVECTOR3D() {return ((LPLYNXVECTOR3D)this);};						
		//	//LYNXFORCEINLINE operator LYNXVECTOR3D() {return (*((LPLYNXVECTOR3D)this));};												
		//	LYNXFORCEINLINE CVector3& operator =(const CVector3& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return (*this); };						
		//	LYNXFORCEINLINE CVector3& operator =(const LYNXVECTOR3D& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return (*this); };						
		//	//LYNXFORCEINLINE CVector3& operator =(const LYNXVECTOR3D rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return (*this); };						

		//	LYNXFORCEINLINE void operator += (const CVector3& rhs) { x = LYNX_REAL_ADD(x, rhs.x); y = LYNX_REAL_ADD(y, rhs.y); z = LYNX_REAL_ADD(z, rhs.z);};
		//	LYNXFORCEINLINE void operator += (const CReal& rhs) { x = LYNX_REAL_ADD(x, rhs.r); y = LYNX_REAL_ADD(y, rhs.r); z = LYNX_REAL_ADD(z, rhs.r);};
		//	LYNXFORCEINLINE void operator -= (const CVector3& rhs) { x = LYNX_REAL_SUB(x, rhs.x); y = LYNX_REAL_SUB(y, rhs.y); z = LYNX_REAL_SUB(z, rhs.z);};
		//	LYNXFORCEINLINE void operator -= (const CReal& rhs) { x = LYNX_REAL_SUB(x, rhs.r); y = LYNX_REAL_SUB(y, rhs.r); z = LYNX_REAL_SUB(z, rhs.r);};
		//	LYNXFORCEINLINE void operator *= (const CVector3& rhs) { x = LYNX_REAL_MUL(x, rhs.x); y = LYNX_REAL_MUL(y, rhs.y); z = LYNX_REAL_MUL(z, rhs.z);};
		//	LYNXFORCEINLINE void operator *= (const CReal& rhs) { x = LYNX_REAL_MUL(x, rhs.r); y = LYNX_REAL_MUL(y, rhs.r); z = LYNX_REAL_MUL(z, rhs.r);};
		//	LYNXFORCEINLINE void operator /= (const CVector3& rhs) { x = LYNX_REAL_DIV(x, rhs.x); y = LYNX_REAL_DIV(y, rhs.y); z = LYNX_REAL_DIV(z, rhs.z);};
		//	LYNXFORCEINLINE void operator /= (const CReal& rhs) { x = LYNX_REAL_DIV(x, rhs.r); y = LYNX_REAL_DIV(y, rhs.r); z = LYNX_REAL_DIV(z, rhs.r);};

		//	LYNXFORCEINLINE void Normalise(const int dim) { lynxNormalise((LPLYNXVECTOR3D)this, dim); };
		//	LYNXFORCEINLINE LYNXREAL Length(const int dim) { return lynxLength((LPLYNXVECTOR3D)this, dim); };						
		//};	

		LYNXFORCEINLINE CVector3 operator +(const CVector3& lhs, const CVector3& rhs) { CVector3 V; LYNX_VECTOR3D_ADD(V, (lhs), (rhs)); return V;};		
		LYNXFORCEINLINE CVector3 operator +(const CVector3& lhs, const CReal& rhs) { CVector3 V; LYNX_VECTOR3D(V, LYNX_REAL_ADD(lhs.x, rhs.r), LYNX_REAL_ADD(lhs.y, rhs.r), LYNX_REAL_ADD(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE CVector3 operator +(const CReal& lhs, const CVector3& rhs) { CVector3 V; LYNX_VECTOR3D(V, LYNX_REAL_ADD(lhs.r, rhs.x), LYNX_REAL_ADD(lhs.r, rhs.y), LYNX_REAL_ADD(lhs.r, rhs.z)); return V; };		

		LYNXFORCEINLINE CVector3 operator -(const CVector3& lhs, const CVector3& rhs) { CVector3 V; LYNX_VECTOR3D_SUB(V, (lhs), (rhs)); return V;};		
		LYNXFORCEINLINE CVector3 operator -(const CVector3& lhs, const CReal& rhs) { CVector3 V; LYNX_VECTOR3D(V, LYNX_REAL_SUB(lhs.x, rhs.r), LYNX_REAL_SUB(lhs.y, rhs.r), LYNX_REAL_SUB(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE CVector3 operator -(const CReal& lhs, const CVector3& rhs) { CVector3 V; LYNX_VECTOR3D(V, LYNX_REAL_SUB(lhs.r, rhs.x), LYNX_REAL_SUB(lhs.r, rhs.y), LYNX_REAL_SUB(lhs.r, rhs.z)); return V; };		

		LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const CVector3& rhs) { CVector3 V;  LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.x, rhs.x), LYNX_REAL_MUL(lhs.y, rhs.y), LYNX_REAL_MUL(lhs.z, rhs.z)); return V;};		
		LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const CReal& rhs) { CVector3 V; LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.x, rhs.r), LYNX_REAL_MUL(lhs.y, rhs.r), LYNX_REAL_MUL(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE CVector3 operator *(const CReal& lhs, const CVector3& rhs) { CVector3 V; LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.r, rhs.x), LYNX_REAL_MUL(lhs.r, rhs.y), LYNX_REAL_MUL(lhs.r, rhs.z)); return V; };

		LYNXFORCEINLINE CVector3 operator /(const CVector3& lhs, const CVector3& rhs) { CVector3 V;  LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.x, rhs.x), LYNX_REAL_DIV(lhs.y, rhs.y), LYNX_REAL_DIV(lhs.z, rhs.z)); return V;};		
		LYNXFORCEINLINE CVector3 operator /(const CVector3& lhs, const CReal& rhs) { CVector3 V; LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.x, rhs.r), LYNX_REAL_DIV(lhs.y, rhs.r), LYNX_REAL_DIV(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE CVector3 operator /(const CReal& lhs, const CVector3& rhs) { CVector3 V; LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.r, rhs.x), LYNX_REAL_DIV(lhs.r, rhs.y), LYNX_REAL_DIV(lhs.r, rhs.z)); return V; };
		
		LYNXFORCEINLINE void Vector3Add(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.x, b.x), LYNX_REAL_ADD(a.y, b.y), LYNX_REAL_ADD(a.z, b.z)); };
		LYNXFORCEINLINE void Vector3Add(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.x, b.r), LYNX_REAL_ADD(a.y, b.r), LYNX_REAL_ADD(a.z, b.r)); };
		LYNXFORCEINLINE void Vector3Add(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.r, b.x), LYNX_REAL_ADD(a.r, b.y), LYNX_REAL_ADD(a.r, b.z)); };
		LYNXFORCEINLINE void Vector3Sub(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.x, b.x), LYNX_REAL_SUB(a.y, b.y), LYNX_REAL_SUB(a.z, b.z)); };
		LYNXFORCEINLINE void Vector3Sub(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.x, b.r), LYNX_REAL_SUB(a.y, b.r), LYNX_REAL_SUB(a.z, b.r)); };
		LYNXFORCEINLINE void Vector3Sub(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.r, b.x), LYNX_REAL_SUB(a.r, b.y), LYNX_REAL_SUB(a.r, b.z)); };
		LYNXFORCEINLINE void Vector3Mul(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.x, b.x), LYNX_REAL_MUL(a.y, b.y), LYNX_REAL_MUL(a.z, b.z)); };
		LYNXFORCEINLINE void Vector3Mul(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.x, b.r), LYNX_REAL_MUL(a.y, b.r), LYNX_REAL_MUL(a.z, b.r)); };
		LYNXFORCEINLINE void Vector3Mul(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.r, b.x), LYNX_REAL_MUL(a.r, b.y), LYNX_REAL_MUL(a.r, b.z)); };
		LYNXFORCEINLINE void Vector3Div(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.x, b.x), LYNX_REAL_DIV(a.y, b.y), LYNX_REAL_DIV(a.z, b.z)); };		
		LYNXFORCEINLINE void Vector3Div(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.x, b.r), LYNX_REAL_DIV(a.y, b.r), LYNX_REAL_DIV(a.z, b.r)); };
		LYNXFORCEINLINE void Vector3Div(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.r, b.x), LYNX_REAL_DIV(a.r, b.y), LYNX_REAL_DIV(a.r, b.z)); };

		LYNXFORCEINLINE void SetVECTOR3D(LYNXVECTOR3D& lhs, const CVector3& rhs) { lhs.x = rhs.x; lhs.y = rhs.y; lhs.z = rhs.z; };
		LYNXFORCEINLINE LYNXREAL Dot(const CVector3& v1, const CVector3& v2) { return (LYNX_DOT_PRODUCT(v1, v2)); };						
		LYNXFORCEINLINE CVector3 Cross(const CVector3& a, const CVector3& b) { CVector3 V; lynxCrossProduct(&V, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); return (V); };
		LYNXFORCEINLINE void Cross(CVector3& r, const CVector3& a, const CVector3& b) { lynxCrossProduct((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); };


		/**	
		*  @brief CVector4 class of the engine.
		* 
		*  The basic object class of the entire engine, every class of the engine is drived from this class.
		*  It records a unique ID and a reference counter. 
		*	
		*/
		class LYNXENGCLASS CVector4 : public LYNXVECTOR4D
		{			
		public:						
			LYNXFORCEINLINE CVector4() {x = y = z = 0; w = 1.0f; };
			LYNXFORCEINLINE CVector4(LYNXVECTOR4D& v) {x = v.x; y = v.y; z = v.z; w = v.w; };
			LYNXFORCEINLINE CVector4(LYNXCOLORRGB& c) {r = c.Red/255.0f; g = c.Green/255.0f; b = c.Blue/255.0f; a = 1.0f; };		
			LYNXFORCEINLINE CVector4(LYNXCOLORRGBA& c) {r = c.Red/255.0f; g = c.Green/255.0f; b = c.Blue/255.0f; a = c.Alpha/255.0f; };			
			LYNXFORCEINLINE CVector4(const CReal& _x, const CReal& _y, const CReal& _z, const CReal& _w) {x = _x; y = _y; z= _z; w = _w; };			
			LYNXFORCEINLINE CVector4(const CVector3& v, const CReal& _w) {x = v.x; y = v.y; z= v.z; w = _w; };			
			~CVector4() {};	
			
			LYNXFORCEINLINE CVector4& operator =(const CVector4& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return (*this); };						
			LYNXFORCEINLINE CVector4& operator =(const LYNXVECTOR4D& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return (*this); };	
			LYNXFORCEINLINE CVector4& operator =(const float& rhs) { x = y = z = w = rhs; return (*this); };

			LYNXFORCEINLINE void operator += (const CVector4& rhs) { x = LYNX_REAL_ADD(x, rhs.x); y = LYNX_REAL_ADD(y, rhs.y); z = LYNX_REAL_ADD(z, rhs.z); w = LYNX_REAL_ADD(w, rhs.w);};
			LYNXFORCEINLINE void operator += (const CReal& rhs) { x = LYNX_REAL_ADD(x, rhs.r); y = LYNX_REAL_ADD(y, rhs.r); z = LYNX_REAL_ADD(z, rhs.r); w = LYNX_REAL_ADD(w, rhs.r);};
			LYNXFORCEINLINE void operator -= (const CVector4& rhs) { x = LYNX_REAL_SUB(x, rhs.x); y = LYNX_REAL_SUB(y, rhs.y); z = LYNX_REAL_SUB(z, rhs.z); w = LYNX_REAL_SUB(w, rhs.w);};
			LYNXFORCEINLINE void operator -= (const CReal& rhs) { x = LYNX_REAL_SUB(x, rhs.r); y = LYNX_REAL_SUB(y, rhs.r); z = LYNX_REAL_SUB(z, rhs.r); w = LYNX_REAL_SUB(w, rhs.r);};
			LYNXFORCEINLINE void operator *= (const CVector4& rhs) { x = LYNX_REAL_MUL(x, rhs.x); y = LYNX_REAL_MUL(y, rhs.y); z = LYNX_REAL_MUL(z, rhs.z); w = LYNX_REAL_MUL(w, rhs.w);};
			LYNXFORCEINLINE void operator *= (const CReal& rhs) { x = LYNX_REAL_MUL(x, rhs.r); y = LYNX_REAL_MUL(y, rhs.r); z = LYNX_REAL_MUL(z, rhs.r); w = LYNX_REAL_MUL(w, rhs.r);};
			LYNXFORCEINLINE void operator /= (const CVector4& rhs) { x = LYNX_REAL_DIV(x, rhs.x); y = LYNX_REAL_DIV(y, rhs.y); z = LYNX_REAL_DIV(z, rhs.z); w = LYNX_REAL_DIV(w, rhs.w);};
			LYNXFORCEINLINE void operator /= (const CReal& rhs) { x = LYNX_REAL_DIV(x, rhs.r); y = LYNX_REAL_DIV(y, rhs.r); z = LYNX_REAL_DIV(z, rhs.r); w = LYNX_REAL_DIV(w, rhs.r);};

			LYNXFORCEINLINE void Normalise(const int dim = 4) { lynxNormalise((LPLYNXVECTOR3D)this, dim); };
			LYNXFORCEINLINE LYNXREAL Length(const int dim = 4) { return lynxLength((LPLYNXVECTOR3D)this, dim); };						
		};	
		
		typedef CVector3						CColor3;
		typedef CVector4						CColor4;

		class LYNXENGCLASS CColor
		{			
		protected:
			LYNXCOLORRGBA			m_ByteColor;	
		public:
			CReal					r, g, b, a;		
		public:						
			LYNXFORCEINLINE CColor() {r = g = b = a = 1.0f; };
			LYNXFORCEINLINE CColor(const LYNXCOLORRGBA& c) {r = ((float)c.Red)/255.0f; g = ((float)c.Green)/255.0f; b = ((float)c.Blue)/255.0f; a = ((float)c.Alpha)/255.0f; };			
			LYNXFORCEINLINE CColor(const LYNXCOLORRGB& c) {r = ((float)c.Red)/255.0f; g = ((float)c.Green)/255.0f; b = ((float)c.Blue)/255.0f; };			
			LYNXFORCEINLINE CColor(const CReal& _r, const CReal& _g, const CReal& _b, const CReal& _a) {r = _r; g = _g; b = _b; a = _a; };			
			LYNXFORCEINLINE CColor(const float _r, const float _g, const float _b, const float _a) {r = _r; g = _g; b = _b; a = _a; };			
			LYNXFORCEINLINE CColor(const LYNXBYTE& _r, const LYNXBYTE& _g, const LYNXBYTE& _b, const LYNXBYTE& _a) {r = ((float)_r)/255.0f; g = ((float)_g)/255.0f; b = ((float)_b)/255.0f; a = ((float)_a)/255.0f; };			
			
			~CColor() {};	
			
			LYNXFORCEINLINE operator LPLYNXCOLORRGBA() { m_ByteColor.Red = (LYNXBYTE)(r*255.0f); m_ByteColor.Green = (LYNXBYTE)(g*255.0f); m_ByteColor.Blue = (LYNXBYTE)(b*255.0f); m_ByteColor.Alpha = (LYNXBYTE)(a*255.0f); return &(m_ByteColor); };									
			//LYNXFORCEINLINE operator LYNXCOLORRGBA() { m_ByteColor.Red = (LYNXBYTE)(r*255.0f); m_ByteColor.Green = (LYNXBYTE)(g*255.0f); m_ByteColor.Blue = (LYNXBYTE)(b*255.0f); m_ByteColor.Alpha = (LYNXBYTE)(a*255.0f); return (m_ByteColor); };									
			LYNXFORCEINLINE operator LYNXCOLORRGBA&() { m_ByteColor.Red = (LYNXBYTE)(r*255.0f); m_ByteColor.Green = (LYNXBYTE)(g*255.0f); m_ByteColor.Blue = (LYNXBYTE)(b*255.0f); m_ByteColor.Alpha = (LYNXBYTE)(a*255.0f); return (m_ByteColor); };									
			LYNXFORCEINLINE CColor& operator =(const CColor& rhs) { r = rhs.r; g = rhs.g; b = rhs.b; a = rhs.a; return (*this); };						
			LYNXFORCEINLINE CColor& operator =(const LYNXCOLORRGBA& rhs) { r = rhs.Red; g = rhs.Green; b = rhs.Blue; a = rhs.Alpha; return (*this); };									

			LYNXFORCEINLINE DWORD PackedColor() { m_ByteColor.Red = (LYNXBYTE)(r*255.0f); m_ByteColor.Green = (LYNXBYTE)(g*255.0f); m_ByteColor.Blue = (LYNXBYTE)(b*255.0f); m_ByteColor.Alpha = (LYNXBYTE)(a*255.0f); return (m_ByteColor.PackedColor); };

			//LYNXFORCEINLINE void operator += (const CColor& rhs) { x = LYNX_REAL_ADD(x, rhs.x); y = LYNX_REAL_ADD(y, rhs.y); z = LYNX_REAL_ADD(z, rhs.z); w = LYNX_REAL_ADD(w, rhs.w);};
			//LYNXFORCEINLINE void operator += (const CReal& rhs) { x = LYNX_REAL_ADD(x, rhs.r); y = LYNX_REAL_ADD(y, rhs.r); z = LYNX_REAL_ADD(z, rhs.r); w = LYNX_REAL_ADD(w, rhs.r);};
			//LYNXFORCEINLINE void operator -= (const CColor& rhs) { x = LYNX_REAL_SUB(x, rhs.x); y = LYNX_REAL_SUB(y, rhs.y); z = LYNX_REAL_SUB(z, rhs.z); w = LYNX_REAL_SUB(w, rhs.w);};
			//LYNXFORCEINLINE void operator -= (const CReal& rhs) { x = LYNX_REAL_SUB(x, rhs.r); y = LYNX_REAL_SUB(y, rhs.r); z = LYNX_REAL_SUB(z, rhs.r); w = LYNX_REAL_SUB(w, rhs.r);};
			//LYNXFORCEINLINE void operator *= (const CColor& rhs) { x = LYNX_REAL_MUL(x, rhs.x); y = LYNX_REAL_MUL(y, rhs.y); z = LYNX_REAL_MUL(z, rhs.z); w = LYNX_REAL_MUL(w, rhs.w);};
			//LYNXFORCEINLINE void operator *= (const CReal& rhs) { x = LYNX_REAL_MUL(x, rhs.r); y = LYNX_REAL_MUL(y, rhs.r); z = LYNX_REAL_MUL(z, rhs.r); w = LYNX_REAL_MUL(w, rhs.r);};
			//LYNXFORCEINLINE void operator /= (const CColor& rhs) { x = LYNX_REAL_DIV(x, rhs.x); y = LYNX_REAL_DIV(y, rhs.y); z = LYNX_REAL_DIV(z, rhs.z); w = LYNX_REAL_DIV(w, rhs.w);};
			//LYNXFORCEINLINE void operator /= (const CReal& rhs) { x = LYNX_REAL_DIV(x, rhs.r); y = LYNX_REAL_DIV(y, rhs.r); z = LYNX_REAL_DIV(z, rhs.r); w = LYNX_REAL_DIV(w, rhs.r);};

			//LYNXFORCEINLINE void Normalise(const int dim = 4) { lynxNormalise((LPLYNXVECTOR3D)this, dim); };
			//LYNXFORCEINLINE LYNXREAL Length(const int dim = 4) { return lynxLength((LPLYNXVECTOR3D)this, dim); };						
		};	

		/*LYNXFORCEINLINE LYNXVECTOR4D operator +(const CVector4& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D_ADD(V, (lhs), (rhs)); return V;};		
		LYNXFORCEINLINE LYNXVECTOR4D operator +(const CVector4& lhs, const CReal& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_ADD(lhs.x, rhs.r), LYNX_REAL_ADD(lhs.y, rhs.r), LYNX_REAL_ADD(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE LYNXVECTOR4D operator +(const CReal& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_ADD(lhs.r, rhs.x), LYNX_REAL_ADD(lhs.r, rhs.y), LYNX_REAL_ADD(lhs.r, rhs.z)); return V; };		

		LYNXFORCEINLINE LYNXVECTOR4D operator -(const CVector4& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D_SUB(V, (lhs), (rhs)); return V;};		
		LYNXFORCEINLINE LYNXVECTOR4D operator -(const CVector4& lhs, const CReal& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_SUB(lhs.x, rhs.r), LYNX_REAL_SUB(lhs.y, rhs.r), LYNX_REAL_SUB(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE LYNXVECTOR4D operator -(const CReal& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_SUB(lhs.r, rhs.x), LYNX_REAL_SUB(lhs.r, rhs.y), LYNX_REAL_SUB(lhs.r, rhs.z)); return V; };		

		LYNXFORCEINLINE LYNXVECTOR4D operator *(const CVector4& lhs, const CVector4& rhs) { LYNXVECTOR3D V;  LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.x, rhs.x), LYNX_REAL_MUL(lhs.y, rhs.y), LYNX_REAL_MUL(lhs.z, rhs.z)); return V;};		
		LYNXFORCEINLINE LYNXVECTOR4D operator *(const CVector4& lhs, const CReal& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.x, rhs.r), LYNX_REAL_MUL(lhs.y, rhs.r), LYNX_REAL_MUL(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE LYNXVECTOR4D operator *(const CReal& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.r, rhs.x), LYNX_REAL_MUL(lhs.r, rhs.y), LYNX_REAL_MUL(lhs.r, rhs.z)); return V; };

		LYNXFORCEINLINE LYNXVECTOR4D operator /(const CVector4& lhs, const CVector4& rhs) { LYNXVECTOR3D V;  LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.x, rhs.x), LYNX_REAL_DIV(lhs.y, rhs.y), LYNX_REAL_DIV(lhs.z, rhs.z)); return V;};		
		LYNXFORCEINLINE LYNXVECTOR4D operator /(const CVector4& lhs, const CReal& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.x, rhs.r), LYNX_REAL_DIV(lhs.y, rhs.r), LYNX_REAL_DIV(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE LYNXVECTOR4D operator /(const CReal& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.r, rhs.x), LYNX_REAL_DIV(lhs.r, rhs.y), LYNX_REAL_DIV(lhs.r, rhs.z)); return V; };
		
		LYNXFORCEINLINE void Vector4Add(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.x, b.x), LYNX_REAL_ADD(a.y, b.y), LYNX_REAL_ADD(a.z, b.z)); };
		LYNXFORCEINLINE void Vector4Add(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.x, b.r), LYNX_REAL_ADD(a.y, b.r), LYNX_REAL_ADD(a.z, b.r)); };
		LYNXFORCEINLINE void Vector4Add(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.r, b.x), LYNX_REAL_ADD(a.r, b.y), LYNX_REAL_ADD(a.r, b.z)); };
		LYNXFORCEINLINE void Vector4Sub(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.x, b.x), LYNX_REAL_SUB(a.y, b.y), LYNX_REAL_SUB(a.z, b.z)); };
		LYNXFORCEINLINE void Vector4Sub(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.x, b.r), LYNX_REAL_SUB(a.y, b.r), LYNX_REAL_SUB(a.z, b.r)); };
		LYNXFORCEINLINE void Vector4Sub(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.r, b.x), LYNX_REAL_SUB(a.r, b.y), LYNX_REAL_SUB(a.r, b.z)); };
		LYNXFORCEINLINE void Vector4Mul(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.x, b.x), LYNX_REAL_MUL(a.y, b.y), LYNX_REAL_MUL(a.z, b.z)); };
		LYNXFORCEINLINE void Vector4Mul(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.x, b.r), LYNX_REAL_MUL(a.y, b.r), LYNX_REAL_MUL(a.z, b.r)); };
		LYNXFORCEINLINE void Vector4Mul(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.r, b.x), LYNX_REAL_MUL(a.r, b.y), LYNX_REAL_MUL(a.r, b.z)); };
		LYNXFORCEINLINE void Vector4Div(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.x, b.x), LYNX_REAL_DIV(a.y, b.y), LYNX_REAL_DIV(a.z, b.z)); };		
		LYNXFORCEINLINE void Vector4Div(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.x, b.r), LYNX_REAL_DIV(a.y, b.r), LYNX_REAL_DIV(a.z, b.r)); };
		LYNXFORCEINLINE void Vector4Div(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.r, b.x), LYNX_REAL_DIV(a.r, b.y), LYNX_REAL_DIV(a.r, b.z)); };*/

		//LYNXFORCEINLINE LYNXREAL Dot(const CVector3& v1, const CVector3& v2) { return (LYNX_DOT_PRODUCT(v1, v2)); };						
		//LYNXFORCEINLINE CVector3 Cross(const CVector3& a, const CVector3& b) { CVector3 V; lynxCrossProduct(&V, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); return (V); };
		//LYNXFORCEINLINE void Cross(CVector3& r, const CVector3& a, const CVector3& b) { lynxCrossProduct((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); };

		/*LYNXFORCEINLINE LYNXVECTOR4D operator +(const CVector4& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D_ADD(V, (lhs), (rhs)); return V;};		
		LYNXFORCEINLINE LYNXVECTOR4D operator +(const CVector4& lhs, const CReal& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_ADD(lhs.x, rhs.r), LYNX_REAL_ADD(lhs.y, rhs.r), LYNX_REAL_ADD(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE LYNXVECTOR4D operator +(const CReal& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_ADD(lhs.r, rhs.x), LYNX_REAL_ADD(lhs.r, rhs.y), LYNX_REAL_ADD(lhs.r, rhs.z)); return V; };		

		LYNXFORCEINLINE LYNXVECTOR4D operator -(const CVector4& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D_SUB(V, (lhs), (rhs)); return V;};		
		LYNXFORCEINLINE LYNXVECTOR4D operator -(const CVector4& lhs, const CReal& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_SUB(lhs.x, rhs.r), LYNX_REAL_SUB(lhs.y, rhs.r), LYNX_REAL_SUB(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE LYNXVECTOR4D operator -(const CReal& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_SUB(lhs.r, rhs.x), LYNX_REAL_SUB(lhs.r, rhs.y), LYNX_REAL_SUB(lhs.r, rhs.z)); return V; };		

		LYNXFORCEINLINE LYNXVECTOR4D operator *(const CVector4& lhs, const CVector4& rhs) { LYNXVECTOR3D V;  LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.x, rhs.x), LYNX_REAL_MUL(lhs.y, rhs.y), LYNX_REAL_MUL(lhs.z, rhs.z)); return V;};		
		LYNXFORCEINLINE LYNXVECTOR4D operator *(const CVector4& lhs, const CReal& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.x, rhs.r), LYNX_REAL_MUL(lhs.y, rhs.r), LYNX_REAL_MUL(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE LYNXVECTOR4D operator *(const CReal& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_MUL(lhs.r, rhs.x), LYNX_REAL_MUL(lhs.r, rhs.y), LYNX_REAL_MUL(lhs.r, rhs.z)); return V; };

		LYNXFORCEINLINE LYNXVECTOR4D operator /(const CVector4& lhs, const CVector4& rhs) { LYNXVECTOR3D V;  LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.x, rhs.x), LYNX_REAL_DIV(lhs.y, rhs.y), LYNX_REAL_DIV(lhs.z, rhs.z)); return V;};		
		LYNXFORCEINLINE LYNXVECTOR4D operator /(const CVector4& lhs, const CReal& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.x, rhs.r), LYNX_REAL_DIV(lhs.y, rhs.r), LYNX_REAL_DIV(lhs.z, rhs.r)); return V; };
		LYNXFORCEINLINE LYNXVECTOR4D operator /(const CReal& lhs, const CVector4& rhs) { LYNXVECTOR3D V; LYNX_VECTOR3D(V, LYNX_REAL_DIV(lhs.r, rhs.x), LYNX_REAL_DIV(lhs.r, rhs.y), LYNX_REAL_DIV(lhs.r, rhs.z)); return V; };
		
		LYNXFORCEINLINE void Vector4Add(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.x, b.x), LYNX_REAL_ADD(a.y, b.y), LYNX_REAL_ADD(a.z, b.z)); };
		LYNXFORCEINLINE void Vector4Add(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.x, b.r), LYNX_REAL_ADD(a.y, b.r), LYNX_REAL_ADD(a.z, b.r)); };
		LYNXFORCEINLINE void Vector4Add(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_ADD(a.r, b.x), LYNX_REAL_ADD(a.r, b.y), LYNX_REAL_ADD(a.r, b.z)); };
		LYNXFORCEINLINE void Vector4Sub(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.x, b.x), LYNX_REAL_SUB(a.y, b.y), LYNX_REAL_SUB(a.z, b.z)); };
		LYNXFORCEINLINE void Vector4Sub(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.x, b.r), LYNX_REAL_SUB(a.y, b.r), LYNX_REAL_SUB(a.z, b.r)); };
		LYNXFORCEINLINE void Vector4Sub(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_SUB(a.r, b.x), LYNX_REAL_SUB(a.r, b.y), LYNX_REAL_SUB(a.r, b.z)); };
		LYNXFORCEINLINE void Vector4Mul(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.x, b.x), LYNX_REAL_MUL(a.y, b.y), LYNX_REAL_MUL(a.z, b.z)); };
		LYNXFORCEINLINE void Vector4Mul(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.x, b.r), LYNX_REAL_MUL(a.y, b.r), LYNX_REAL_MUL(a.z, b.r)); };
		LYNXFORCEINLINE void Vector4Mul(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_MUL(a.r, b.x), LYNX_REAL_MUL(a.r, b.y), LYNX_REAL_MUL(a.r, b.z)); };
		LYNXFORCEINLINE void Vector4Div(CVector3& r, const CVector3& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.x, b.x), LYNX_REAL_DIV(a.y, b.y), LYNX_REAL_DIV(a.z, b.z)); };		
		LYNXFORCEINLINE void Vector4Div(CVector3& r, const CVector3& a, const CReal& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.x, b.r), LYNX_REAL_DIV(a.y, b.r), LYNX_REAL_DIV(a.z, b.r)); };
		LYNXFORCEINLINE void Vector4Div(CVector3& r, const CReal& a, const CVector3& b) { LYNX_VECTOR3D(r, LYNX_REAL_DIV(a.r, b.x), LYNX_REAL_DIV(a.r, b.y), LYNX_REAL_DIV(a.r, b.z)); };*/

		//LYNXFORCEINLINE LYNXREAL Dot(const CVector3& v1, const CVector3& v2) { return (LYNX_DOT_PRODUCT(v1, v2)); };						
		//LYNXFORCEINLINE CVector3 Cross(const CVector3& a, const CVector3& b) { CVector3 V; lynxCrossProduct(&V, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); return (V); };
		//LYNXFORCEINLINE void Cross(CVector3& r, const CVector3& a, const CVector3& b) { lynxCrossProduct((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXVECTOR3D)&b); };

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
		//	CMatrix3(LYNXMATRIX& m) {m11 = m.m11; m12 = m.m12; m13 = m.m13; 
		//						     m21 = m.m21; m22 = m.m22; m23 = m.m23; 
		//						     m31 = m.m31; m32 = m.m32; m33 = m.m33; };			
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
		//	LYNXFORCEINLINE CMatrix3& operator =(const CMatrix3& rhs) { m11 = rhm11; m12 = rhm12; m13 = rhm13; 
		//														   m21 = rhm21; m22 = rhm22; m23 = rhm23; 
		//														   m31 = rhm31; m32 = rhm32; m33 = rhm33; return (*this); };						

		//	//LYNXFORCEINLINE void Normalise(int dim) { lynxNormalise((LPLYNXVECTOR3D)this, dim); };
		//	//LYNXFORCEINLINE CReal Length(int dim) { return CReal(lynxLength((LPLYNXVECTOR3D)this, dim)); };			
		//};				

		////LYNXFORCEINLINE CMatrix4 operator *(const CMatrix4& lhs, const CMatrix4& rhs) { LYNXMATRIX M; lynxMatrixXMatrix(&M, (LPLYNXMATRIX)&lhs, (LPLYNXMATRIX)&rhs); return CMatrix4(M); };		
		//LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const CMatrix3& rhs) { LYNXVECTOR3D V; lynxVectorXMatrix3(&V, (LPLYNXVECTOR3D)&lhs, (LPLYNXMATRIX3)&rhs); return CVector3(&V); };		

		/**	
		*  @brief CMatrix4 class of the engine.
		* 
		*  The basic object class of the entire engine, every class of the engine is drived from this class.
		*  It records a unique ID and a reference counter. 
		*	
		*/
		class LYNXENGCLASS CMatrix3 : public LYNXMATRIX3
		{			
		public:						
			LYNXFORCEINLINE CMatrix3() {Identity(); };
			LYNXFORCEINLINE CMatrix3(LYNXMATRIX3& m) { LYNX_MATRIX3_COPY((*this), m); }; 
			LYNXFORCEINLINE CMatrix3(const LYNXMATRIX3& m) { LYNX_MATRIX3_COPY((*this), m); }; 
			LYNXFORCEINLINE CMatrix3(CReal& m_11, CReal& m_12, CReal& m_13,
									 CReal& m_21, CReal& m_22, CReal& m_23,
									 CReal& m_31, CReal& m_32, CReal& m_33) { 
										m11 = m_11; m12 = m_12; m13 = m_13; 
										m21 = m_21; m22 = m_22; m23 = m_23; 
										m31 = m_31; m32 = m_32; m33 = m_33; };
			LYNXFORCEINLINE CMatrix3(LYNXREAL m_11, LYNXREAL m_12, LYNXREAL m_13,
									 LYNXREAL m_21, LYNXREAL m_22, LYNXREAL m_23,
									 LYNXREAL m_31, LYNXREAL m_32, LYNXREAL m_33) { 
										m11 = m_11; m12 = m_12; m13 = m_13; 
										m21 = m_21; m22 = m_22; m23 = m_23; 
										m31 = m_31; m32 = m_32; m33 = m_33; };
			~CMatrix3() {};	

			LYNXFORCEINLINE void Identity() { LYNX_IDENTITY_MATRIX3((*this));};						

			//LYNXFORCEINLINE operator LYNXMATRIX() {LYNXMATRIX M; LYNX_MATRIX(M, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44); return (M);};						
			LYNXFORCEINLINE operator LPLYNXMATRIX3() {return ((LPLYNXMATRIX3)&m11);};						
			LYNXFORCEINLINE CMatrix3& operator =(const CMatrix3& rhs) { LYNX_MATRIX3_COPY((*this), rhs); return (*this); };						
			LYNXFORCEINLINE CMatrix3& operator =(const LYNXMATRIX3& rhs) { LYNX_MATRIX3_COPY((*this), rhs); return (*this); };						

			LYNXFORCEINLINE void operator += (const CMatrix3& rhs) { m11 = LYNX_REAL_ADD(m11, rhs.m11); m12 = LYNX_REAL_ADD(m12, rhs.m12); m13 = LYNX_REAL_ADD(m13, rhs.m13); 
																	 m21 = LYNX_REAL_ADD(m21, rhs.m21); m22 = LYNX_REAL_ADD(m22, rhs.m22); m23 = LYNX_REAL_ADD(m23, rhs.m23); 
																	 m31 = LYNX_REAL_ADD(m31, rhs.m31); m32 = LYNX_REAL_ADD(m32, rhs.m32); m33 = LYNX_REAL_ADD(m33, rhs.m33); };
			LYNXFORCEINLINE void operator += (const CReal& rhs) { m11 = LYNX_REAL_ADD(m11, rhs.r); m12 = LYNX_REAL_ADD(m12, rhs.r); m13 = LYNX_REAL_ADD(m13, rhs.r);
																  m21 = LYNX_REAL_ADD(m21, rhs.r); m22 = LYNX_REAL_ADD(m22, rhs.r); m23 = LYNX_REAL_ADD(m23, rhs.r);
																  m31 = LYNX_REAL_ADD(m31, rhs.r); m32 = LYNX_REAL_ADD(m32, rhs.r); m33 = LYNX_REAL_ADD(m33, rhs.r); };
			LYNXFORCEINLINE void operator -= (const CMatrix3& rhs) { m11 = LYNX_REAL_SUB(m11, rhs.m11); m12 = LYNX_REAL_SUB(m12, rhs.m12); m13 = LYNX_REAL_SUB(m13, rhs.m13); 
																	 m21 = LYNX_REAL_SUB(m21, rhs.m21); m22 = LYNX_REAL_SUB(m22, rhs.m22); m23 = LYNX_REAL_SUB(m23, rhs.m23); 
																	 m31 = LYNX_REAL_SUB(m31, rhs.m31); m32 = LYNX_REAL_SUB(m32, rhs.m32); m33 = LYNX_REAL_SUB(m33, rhs.m33); };
			LYNXFORCEINLINE void operator -= (const CReal& rhs) { m11 = LYNX_REAL_SUB(m11, rhs.r); m12 = LYNX_REAL_SUB(m12, rhs.r); m13 = LYNX_REAL_SUB(m13, rhs.r);
																  m21 = LYNX_REAL_SUB(m21, rhs.r); m22 = LYNX_REAL_SUB(m22, rhs.r); m23 = LYNX_REAL_SUB(m23, rhs.r);
																  m31 = LYNX_REAL_SUB(m31, rhs.r); m32 = LYNX_REAL_SUB(m32, rhs.r); m33 = LYNX_REAL_SUB(m33, rhs.r); };
		    LYNXFORCEINLINE void operator *= (const CMatrix3& rhs) { m11 = LYNX_REAL_MUL(m11, rhs.m11); m12 = LYNX_REAL_MUL(m12, rhs.m12); m13 = LYNX_REAL_MUL(m13, rhs.m13); 
																	 m21 = LYNX_REAL_MUL(m21, rhs.m21); m22 = LYNX_REAL_MUL(m22, rhs.m22); m23 = LYNX_REAL_MUL(m23, rhs.m23); 
																	 m31 = LYNX_REAL_MUL(m31, rhs.m31); m32 = LYNX_REAL_MUL(m32, rhs.m32); m33 = LYNX_REAL_MUL(m33, rhs.m33); };
			LYNXFORCEINLINE void operator *= (const CReal& rhs) { m11 = LYNX_REAL_MUL(m11, rhs.r); m12 = LYNX_REAL_MUL(m12, rhs.r); m13 = LYNX_REAL_MUL(m13, rhs.r); 
																  m21 = LYNX_REAL_MUL(m21, rhs.r); m22 = LYNX_REAL_MUL(m22, rhs.r); m23 = LYNX_REAL_MUL(m23, rhs.r); 
																  m31 = LYNX_REAL_MUL(m31, rhs.r); m32 = LYNX_REAL_MUL(m32, rhs.r); m33 = LYNX_REAL_MUL(m33, rhs.r); };
		    LYNXFORCEINLINE void operator /= (const CMatrix3& rhs) { m11 = LYNX_REAL_DIV(m11, rhs.m11); m12 = LYNX_REAL_DIV(m12, rhs.m12); m13 = LYNX_REAL_DIV(m13, rhs.m13);
																	 m21 = LYNX_REAL_DIV(m21, rhs.m21); m22 = LYNX_REAL_DIV(m22, rhs.m22); m23 = LYNX_REAL_DIV(m23, rhs.m23);
																	 m31 = LYNX_REAL_DIV(m31, rhs.m31); m32 = LYNX_REAL_DIV(m32, rhs.m32); m33 = LYNX_REAL_DIV(m33, rhs.m33); };
			LYNXFORCEINLINE void operator /= (const CReal& rhs) { m11 = LYNX_REAL_DIV(m11, rhs.r); m12 = LYNX_REAL_DIV(m12, rhs.r); m13 = LYNX_REAL_DIV(m13, rhs.r); 
																  m21 = LYNX_REAL_DIV(m21, rhs.r); m22 = LYNX_REAL_DIV(m22, rhs.r); m23 = LYNX_REAL_DIV(m23, rhs.r); 
																  m31 = LYNX_REAL_DIV(m31, rhs.r); m32 = LYNX_REAL_DIV(m32, rhs.r); m33 = LYNX_REAL_DIV(m33, rhs.r); };

			LYNXFORCEINLINE void Mul(const CMatrix3& lhs, const CMatrix3& rhs) { lynxMatrix3XMatrix3(this, (LPLYNXMATRIX3)&lhs, (LPLYNXMATRIX3)&rhs); };							
			//LYNXFORCEINLINE void Transpose() { lynxTransposeMatrix3(this); };							
			//LYNXFORCEINLINE void Transpose(CMatrix4& r) { lynxTransposeMatrix32(&r, this); };		
			LYNXFORCEINLINE CVector3 GetRight() const {CVector3 V; LYNX_MATRIX_GET_RIGHTDIR(V, (*this)); return (V); };
			LYNXFORCEINLINE void GetRight(CVector3& r) const {LYNX_MATRIX_GET_RIGHTDIR(r, (*this)); };
			LYNXFORCEINLINE CVector3 GetUp() const {CVector3 V; LYNX_MATRIX_GET_UPDIR(V, (*this)); return (V); };
			LYNXFORCEINLINE void GetUp(CVector3& r) const {LYNX_MATRIX_GET_UPDIR(r, (*this)); };			
			LYNXFORCEINLINE CVector3 GetLook() const {CVector3 V; LYNX_MATRIX_GET_LOOKDIR(V, (*this)); return (V); };
			LYNXFORCEINLINE void GetLook(CVector3& r) const {LYNX_MATRIX_GET_LOOKDIR(r, (*this)); };

			LYNXFORCEINLINE CVector3 GetScale() const
			{
				CVector3 V, S;
				GetRight(V);
				S.x = V.Length(3);
				GetUp(V);
				S.y = V.Length(3);
				GetLook(V);
				S.z = V.Length(3);

				return S;
			}
			LYNXFORCEINLINE void GetScale(CVector3& r) const
			{
				CVector3 V;
				GetRight(V);
				r.x = V.Length(3);
				GetUp(V);
				r.y = V.Length(3);
				GetLook(V);
				r.z = V.Length(3);
			}
			LYNXFORCEINLINE void ReplaceScale(const CVector3& s) const
			{
				CVector3* RightDir = (CVector3*)&m11;
				CVector3* UpDir = (CVector3*)&m21;
				CVector3* Direction = (CVector3*)&m31;

				RightDir->Normalise(3);
				if (s.x == 0.0f)
					(*RightDir) *= 0.1f;
				else
					(*RightDir) *= s.x;			
									
				UpDir->Normalise(3);
				if (s.y == 0.0f)
					(*UpDir) *= 0.1f;
				else
					(*UpDir) *= s.y;
				
				Direction->Normalise(3);
				(*Direction) *= s.z;
			}
			LYNXFORCEINLINE void GetRotation(CVector3& r) const 
			{ 
				CVector3 X, Y, Z;

				GetRight(X);
				X.Normalise(3);

				GetUp(Y);
				Y.Normalise(3);

				GetLook(Z);
				Z.Normalise(3);

				LYNXMATRIX M;

				LYNX_IDENTITY_MATRIX(M);
				M.m[0][0] = X.x;
				M.m[0][1] = X.y;
				M.m[0][2] = X.z;

				M.m[1][0] = Y.x;
				M.m[1][1] = Y.y;
				M.m[1][2] = Y.z;

				M.m[2][0] = Z.x;
				M.m[2][1] = Z.y;
				M.m[2][2] = Z.z;

				lynxMatrix2EulerAnglesXYZ(&r, &M); 
			}
		};				
		LYNXFORCEINLINE LYNXMATRIX3 operator *(const CMatrix3& lhs, const CMatrix3& rhs) { LYNXMATRIX3 M; lynxMatrix3XMatrix3(&M, (LPLYNXMATRIX3)&lhs, (LPLYNXMATRIX3)&rhs); return (M); };				
		//LYNXFORCEINLINE LYNXVECTOR3D operator *(const CVector3& lhs, const CMatrix4& rhs) { LYNXVECTOR3D V; lynxPointXMatrix(&V, (LPLYNXVECTOR3D)&lhs, (LPLYNXMATRIX)&rhs); return (V); };				

		//LYNXFORCEINLINE void PointXMatrix(CVector3& r, const CVector3& v, const CMatrix4& m) { lynxPointXMatrix(&r, &v, &m); };				
		//LYNXFORCEINLINE void VectorXMatrix(CVector3& r, const CVector3& v, const CMatrix4& m) { lynxVectorXMatrix(&r, &v, &m); };
		//LYNXFORCEINLINE void VectorXMatrix(CVector4& r, const CVector4& v, const CMatrix4& m) { lynxVector4XMatrix(&r, &v, &m); };				
		//LYNXFORCEINLINE void TransposeMatrix(CMatrix4& r, const CMatrix4& a) { lynxTransposeMatrix2(&r, &a); };				
		//LYNXFORCEINLINE void InvertMatrix(CMatrix4& r, const CMatrix4& a) { lynxInvertMatrix(&r, &a); };
		//LYNXFORCEINLINE void InverseMatrix(CMatrix4& r, const CMatrix4& a) { lynxInverseMatrix(&r, &a); };
		LYNXFORCEINLINE void Matrix3XMatrix3(CMatrix3& r, const CMatrix3& a, const CMatrix3& b) { lynxMatrix3XMatrix3((LPLYNXMATRIX3)&r, (LPLYNXMATRIX3)&a, (LPLYNXMATRIX3)&b); };				
		LYNXFORCEINLINE void MakeRotateMatrix3(CMatrix3& r, CReal& deg) { lynxMakeRotateMatrix3(&r, deg); };				
		LYNXFORCEINLINE void MakeRotateMatrix3(CMatrix3& r, LYNXREAL& deg) { lynxMakeRotateMatrix3(&r, CReal(deg)); };				
		//LYNXFORCEINLINE CMatrix3& GetIdentityMatrix() { return (*((CMatrix3 *)lynxGetIdentityMatrix3())); };

		/**	
		*  @brief CMatrix4 class of the engine.
		* 
		*  The basic object class of the entire engine, every class of the engine is drived from this class.
		*  It records a unique ID and a reference counter. 
		*	
		*/
		class LYNXENGCLASS CMatrix4 : public LYNXMATRIX
		{			
		public:						
			LYNXFORCEINLINE CMatrix4() {Identity(); };
			LYNXFORCEINLINE CMatrix4(LYNXMATRIX& m) { LYNX_MATRIX_COPY((*this), m); }; 
			LYNXFORCEINLINE CMatrix4(const LYNXMATRIX& m) { LYNX_MATRIX_COPY((*this), m); }; 
			LYNXFORCEINLINE CMatrix4(CReal& m_11, CReal& m_12, CReal& m_13, CReal& m_14,
									 CReal& m_21, CReal& m_22, CReal& m_23, CReal& m_24,
									 CReal& m_31, CReal& m_32, CReal& m_33, CReal& m_34,
									 CReal& m_41, CReal& m_42, CReal& m_43, CReal& m_44) { 
										m11 = m_11; m12 = m_12; m13 = m_13; m14 = m_14;
										m21 = m_21; m22 = m_22; m23 = m_23; m24 = m_24;
										m31 = m_31; m32 = m_32; m33 = m_33; m34 = m_34;
										m41 = m_41; m42 = m_42; m43 = m_43; m44 = m_44; };
			LYNXFORCEINLINE CMatrix4(LYNXREAL m_11, LYNXREAL m_12, LYNXREAL m_13, LYNXREAL m_14,
									 LYNXREAL m_21, LYNXREAL m_22, LYNXREAL m_23, LYNXREAL m_24,
									 LYNXREAL m_31, LYNXREAL m_32, LYNXREAL m_33, LYNXREAL m_34,
									 LYNXREAL m_41, LYNXREAL m_42, LYNXREAL m_43, LYNXREAL m_44) { 
										m11 = m_11; m12 = m_12; m13 = m_13; m14 = m_14;
										m21 = m_21; m22 = m_22; m23 = m_23; m24 = m_24;
										m31 = m_31; m32 = m_32; m33 = m_33; m34 = m_34;
										m41 = m_41; m42 = m_42; m43 = m_43; m44 = m_44; };
			~CMatrix4() {};	

			LYNXFORCEINLINE void Identity() { LYNX_IDENTITY_MATRIX((*this));};		
			LYNXFORCEINLINE void Normalise(){ lynxNormaliseMatrix(this); };

			//LYNXFORCEINLINE operator LYNXMATRIX() {LYNXMATRIX M; LYNX_MATRIX(M, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44); return (M);};						
			LYNXFORCEINLINE operator LPLYNXMATRIX() {return ((LPLYNXMATRIX)&m11);};						
			LYNXFORCEINLINE CMatrix4& operator =(const CMatrix4& rhs) { LYNX_MATRIX_COPY((*this), rhs); return (*this); };						
			LYNXFORCEINLINE CMatrix4& operator =(const LYNXMATRIX& rhs) { LYNX_MATRIX_COPY((*this), rhs); return (*this); };						
			LYNXFORCEINLINE CMatrix4& operator =(const CMatrix3& rhs) { m11 = rhs.m11; m12 = rhs.m12; m13 = rhs.m13;
																		m21 = rhs.m21; m22 = rhs.m22; m23 = rhs.m23;
																		m31 = rhs.m31; m32 = rhs.m32; m33 = rhs.m33; 
																		return (*this); };						

			LYNXFORCEINLINE void operator += (const CMatrix4& rhs) { m11 = LYNX_REAL_ADD(m11, rhs.m11); m12 = LYNX_REAL_ADD(m12, rhs.m12); m13 = LYNX_REAL_ADD(m13, rhs.m13); m14 = LYNX_REAL_ADD(m14, rhs.m14);
																	 m21 = LYNX_REAL_ADD(m21, rhs.m21); m22 = LYNX_REAL_ADD(m22, rhs.m22); m23 = LYNX_REAL_ADD(m23, rhs.m23); m24 = LYNX_REAL_ADD(m24, rhs.m24);
																	 m31 = LYNX_REAL_ADD(m31, rhs.m31); m32 = LYNX_REAL_ADD(m32, rhs.m32); m33 = LYNX_REAL_ADD(m33, rhs.m33); m34 = LYNX_REAL_ADD(m34, rhs.m34);
																	 m41 = LYNX_REAL_ADD(m41, rhs.m41); m42 = LYNX_REAL_ADD(m42, rhs.m42); m43 = LYNX_REAL_ADD(m43, rhs.m43); m44 = LYNX_REAL_ADD(m44, rhs.m44);};
			LYNXFORCEINLINE void operator += (const CReal& rhs) { m11 = LYNX_REAL_ADD(m11, rhs.r); m12 = LYNX_REAL_ADD(m12, rhs.r); m13 = LYNX_REAL_ADD(m13, rhs.r); m14 = LYNX_REAL_ADD(m14, rhs.r);
																  m21 = LYNX_REAL_ADD(m21, rhs.r); m22 = LYNX_REAL_ADD(m22, rhs.r); m23 = LYNX_REAL_ADD(m23, rhs.r); m24 = LYNX_REAL_ADD(m24, rhs.r);
																  m31 = LYNX_REAL_ADD(m31, rhs.r); m32 = LYNX_REAL_ADD(m32, rhs.r); m33 = LYNX_REAL_ADD(m33, rhs.r); m34 = LYNX_REAL_ADD(m34, rhs.r);
																  m41 = LYNX_REAL_ADD(m41, rhs.r); m42 = LYNX_REAL_ADD(m42, rhs.r); m43 = LYNX_REAL_ADD(m43, rhs.r); m44 = LYNX_REAL_ADD(m44, rhs.r);};
			LYNXFORCEINLINE void operator -= (const CMatrix4& rhs) { m11 = LYNX_REAL_SUB(m11, rhs.m11); m12 = LYNX_REAL_SUB(m12, rhs.m12); m13 = LYNX_REAL_SUB(m13, rhs.m13); m14 = LYNX_REAL_SUB(m14, rhs.m14);
																	 m21 = LYNX_REAL_SUB(m21, rhs.m21); m22 = LYNX_REAL_SUB(m22, rhs.m22); m23 = LYNX_REAL_SUB(m23, rhs.m23); m24 = LYNX_REAL_SUB(m24, rhs.m24);
																	 m31 = LYNX_REAL_SUB(m31, rhs.m31); m32 = LYNX_REAL_SUB(m32, rhs.m32); m33 = LYNX_REAL_SUB(m33, rhs.m33); m34 = LYNX_REAL_SUB(m34, rhs.m34);
																	 m41 = LYNX_REAL_SUB(m41, rhs.m41); m42 = LYNX_REAL_SUB(m42, rhs.m42); m43 = LYNX_REAL_SUB(m43, rhs.m43); m44 = LYNX_REAL_SUB(m44, rhs.m44);};
			LYNXFORCEINLINE void operator -= (const CReal& rhs) { m11 = LYNX_REAL_SUB(m11, rhs.r); m12 = LYNX_REAL_SUB(m12, rhs.r); m13 = LYNX_REAL_SUB(m13, rhs.r); m14 = LYNX_REAL_SUB(m14, rhs.r);
																  m21 = LYNX_REAL_SUB(m21, rhs.r); m22 = LYNX_REAL_SUB(m22, rhs.r); m23 = LYNX_REAL_SUB(m23, rhs.r); m24 = LYNX_REAL_SUB(m24, rhs.r);
																  m31 = LYNX_REAL_SUB(m31, rhs.r); m32 = LYNX_REAL_SUB(m32, rhs.r); m33 = LYNX_REAL_SUB(m33, rhs.r); m34 = LYNX_REAL_SUB(m34, rhs.r);
																  m41 = LYNX_REAL_SUB(m41, rhs.r); m42 = LYNX_REAL_SUB(m42, rhs.r); m43 = LYNX_REAL_SUB(m43, rhs.r); m44 = LYNX_REAL_SUB(m44, rhs.r);};
		    LYNXFORCEINLINE void operator *= (const CMatrix4& rhs) { m11 = LYNX_REAL_MUL(m11, rhs.m11); m12 = LYNX_REAL_MUL(m12, rhs.m12); m13 = LYNX_REAL_MUL(m13, rhs.m13); m14 = LYNX_REAL_MUL(m14, rhs.m14);
																	 m21 = LYNX_REAL_MUL(m21, rhs.m21); m22 = LYNX_REAL_MUL(m22, rhs.m22); m23 = LYNX_REAL_MUL(m23, rhs.m23); m24 = LYNX_REAL_MUL(m24, rhs.m24);
																	 m31 = LYNX_REAL_MUL(m31, rhs.m31); m32 = LYNX_REAL_MUL(m32, rhs.m32); m33 = LYNX_REAL_MUL(m33, rhs.m33); m34 = LYNX_REAL_MUL(m34, rhs.m34);
																	 m41 = LYNX_REAL_MUL(m41, rhs.m41); m42 = LYNX_REAL_MUL(m42, rhs.m42); m43 = LYNX_REAL_MUL(m43, rhs.m43); m44 = LYNX_REAL_MUL(m44, rhs.m44);};
			LYNXFORCEINLINE void operator *= (const CReal& rhs) { m11 = LYNX_REAL_MUL(m11, rhs.r); m12 = LYNX_REAL_MUL(m12, rhs.r); m13 = LYNX_REAL_MUL(m13, rhs.r); m14 = LYNX_REAL_MUL(m14, rhs.r);
																  m21 = LYNX_REAL_MUL(m21, rhs.r); m22 = LYNX_REAL_MUL(m22, rhs.r); m23 = LYNX_REAL_MUL(m23, rhs.r); m24 = LYNX_REAL_MUL(m24, rhs.r);
																  m31 = LYNX_REAL_MUL(m31, rhs.r); m32 = LYNX_REAL_MUL(m32, rhs.r); m33 = LYNX_REAL_MUL(m33, rhs.r); m34 = LYNX_REAL_MUL(m34, rhs.r);
																  m41 = LYNX_REAL_MUL(m41, rhs.r); m42 = LYNX_REAL_MUL(m42, rhs.r); m43 = LYNX_REAL_MUL(m43, rhs.r); m44 = LYNX_REAL_MUL(m44, rhs.r);};
		    LYNXFORCEINLINE void operator /= (const CMatrix4& rhs) { m11 = LYNX_REAL_DIV(m11, rhs.m11); m12 = LYNX_REAL_DIV(m12, rhs.m12); m13 = LYNX_REAL_DIV(m13, rhs.m13); m14 = LYNX_REAL_DIV(m14, rhs.m14);
																	 m21 = LYNX_REAL_DIV(m21, rhs.m21); m22 = LYNX_REAL_DIV(m22, rhs.m22); m23 = LYNX_REAL_DIV(m23, rhs.m23); m24 = LYNX_REAL_DIV(m24, rhs.m24);
																	 m31 = LYNX_REAL_DIV(m31, rhs.m31); m32 = LYNX_REAL_DIV(m32, rhs.m32); m33 = LYNX_REAL_DIV(m33, rhs.m33); m34 = LYNX_REAL_DIV(m34, rhs.m34);
																	 m41 = LYNX_REAL_DIV(m41, rhs.m41); m42 = LYNX_REAL_DIV(m42, rhs.m42); m43 = LYNX_REAL_DIV(m43, rhs.m43); m44 = LYNX_REAL_DIV(m44, rhs.m44);};
			LYNXFORCEINLINE void operator /= (const CReal& rhs) { m11 = LYNX_REAL_DIV(m11, rhs.r); m12 = LYNX_REAL_DIV(m12, rhs.r); m13 = LYNX_REAL_DIV(m13, rhs.r); m14 = LYNX_REAL_DIV(m14, rhs.r);
																  m21 = LYNX_REAL_DIV(m21, rhs.r); m22 = LYNX_REAL_DIV(m22, rhs.r); m23 = LYNX_REAL_DIV(m23, rhs.r); m24 = LYNX_REAL_DIV(m24, rhs.r);
																  m31 = LYNX_REAL_DIV(m31, rhs.r); m32 = LYNX_REAL_DIV(m32, rhs.r); m33 = LYNX_REAL_DIV(m33, rhs.r); m34 = LYNX_REAL_DIV(m34, rhs.r);
																  m41 = LYNX_REAL_DIV(m41, rhs.r); m42 = LYNX_REAL_DIV(m42, rhs.r); m43 = LYNX_REAL_DIV(m43, rhs.r); m44 = LYNX_REAL_DIV(m44, rhs.r);};

			LYNXFORCEINLINE void Set(CReal& m_11, CReal& m_12, CReal& m_13, CReal& m_14,
									 CReal& m_21, CReal& m_22, CReal& m_23, CReal& m_24,
									 CReal& m_31, CReal& m_32, CReal& m_33, CReal& m_34,
									 CReal& m_41, CReal& m_42, CReal& m_43, CReal& m_44) 
			{ 
				m11 = m_11; m12 = m_12; m13 = m_13; m14 = m_14;
				m21 = m_21; m22 = m_22; m23 = m_23; m24 = m_24;
				m31 = m_31; m32 = m_32; m33 = m_33; m34 = m_34;
				m41 = m_41; m42 = m_42; m43 = m_43; m44 = m_44; 
			}
			LYNXFORCEINLINE void Set(float m_11, float m_12, float m_13, float m_14,
									 float m_21, float m_22, float m_23, float m_24,
									 float m_31, float m_32, float m_33, float m_34,
									 float m_41, float m_42, float m_43, float m_44) 
			{ 
				m11 = m_11; m12 = m_12; m13 = m_13; m14 = m_14;
				m21 = m_21; m22 = m_22; m23 = m_23; m24 = m_24;
				m31 = m_31; m32 = m_32; m33 = m_33; m34 = m_34;
				m41 = m_41; m42 = m_42; m43 = m_43; m44 = m_44; 
			}
			LYNXFORCEINLINE void Mul(const CMatrix4& lhs, const CMatrix4& rhs) { lynxMatrixXMatrix(this, (LPLYNXMATRIX)&lhs, (LPLYNXMATRIX)&rhs); };							
			LYNXFORCEINLINE void Transpose() { lynxTransposeMatrix(this); };							
			LYNXFORCEINLINE void Transpose(CMatrix4& r) { lynxTransposeMatrix2(&r, this); };		
			LYNXFORCEINLINE CVector3 GetRight() const {CVector3 V; LYNX_MATRIX_GET_RIGHTDIR(V, (*this)); return (V); };
			LYNXFORCEINLINE void GetRight(CVector3& r) const {LYNX_MATRIX_GET_RIGHTDIR(r, (*this)); };
			LYNXFORCEINLINE CVector3 GetUp() const {CVector3 V; LYNX_MATRIX_GET_UPDIR(V, (*this)); return (V); };
			LYNXFORCEINLINE void GetUp(CVector3& r) const {LYNX_MATRIX_GET_UPDIR(r, (*this)); };
			LYNXFORCEINLINE CVector3 GetLook() const {CVector3 V; LYNX_MATRIX_GET_LOOKDIR(V, (*this)); return (V); };
			LYNXFORCEINLINE void GetLook(CVector3& r) const {LYNX_MATRIX_GET_LOOKDIR(r, (*this)); };
			LYNXFORCEINLINE CVector3 GetPos() const {CVector3 V; LYNX_MATRIX_GET_POS(V, (*this)); return (V); };
			LYNXFORCEINLINE void GetPos(CVector3& r) const {LYNX_MATRIX_GET_POS(r, (*this)); };
			LYNXFORCEINLINE CVector3 GetScale() const
			{
				CVector3 V, S;
				GetRight(V);
				S.x = V.Length(3);
				GetUp(V);
				S.y = V.Length(3);
				GetLook(V);
				S.z = V.Length(3);

				return S;
			}
			LYNXFORCEINLINE void GetScale(CVector3& r) const
			{
				CVector3 V;
				GetRight(V);
				r.x = V.Length(3);
				GetUp(V);
				r.y = V.Length(3);
				GetLook(V);
				r.z = V.Length(3);
			}

			LYNXFORCEINLINE void ReplaceScale(const CVector3& s) const
			{
				CVector3* RightDir = (CVector3*)&m11;
				CVector3* UpDir = (CVector3*)&m21;
				CVector3* Direction = (CVector3*)&m31;

				RightDir->Normalise(3);
				(*RightDir) *= s.x;
				
				UpDir->Normalise(3);
				(*UpDir) *= s.y;
				
				Direction->Normalise(3);
				(*Direction) *= s.z;
			}

			LYNXFORCEINLINE void GetRotation(CVector3& r) const 
			{ 
				CVector3 X, Y, Z;

				GetRight(X);
				X.Normalise(3);

				GetUp(Y);
				Y.Normalise(3);

				GetLook(Z);
				Z.Normalise(3);

				CMatrix4 M;

				M.Identity();
				M.m[0][0] = X.x;
				M.m[0][1] = X.y;
				M.m[0][2] = X.z;

				M.m[1][0] = Y.x;
				M.m[1][1] = Y.y;
				M.m[1][2] = Y.z;

				M.m[2][0] = Z.x;
				M.m[2][1] = Z.y;
				M.m[2][2] = Z.z;

				lynxMatrix2EulerAnglesXYZ(&r, M); 
			}
			LYNXFORCEINLINE void GetRotation(LYNXQUATERNION& q) const 
			{ 
				CVector3 X, Y, Z;

				GetRight(X);
				X.Normalise(3);

				GetUp(Y);
				Y.Normalise(3);

				GetLook(Z);
				Z.Normalise(3);

				CMatrix4 M;

				M.Identity();
				M.m[0][0] = X.x;
				M.m[0][1] = X.y;
				M.m[0][2] = X.z;

				M.m[1][0] = Y.x;
				M.m[1][1] = Y.y;
				M.m[1][2] = Y.z;

				M.m[2][0] = Z.x;
				M.m[2][1] = Z.y;
				M.m[2][2] = Z.z;

				lynxMatrix2Quaternion(&q, M); 
			}

			LYNXFORCEINLINE void ResetOrientation()
			{
				CVector3 Scale;

				GetScale(Scale);

				m11 = Scale.x;
				m12 = 0.0f;
				m13 = 0.0f;

				m21 = 0.0f;
				m22 = Scale.y;
				m23 = 0.0f;

				m31 = 0.0f;
				m32 = 0.0f;
				m33 = Scale.z;				
			}

			LYNXFORCEINLINE void Normalise2()
			{
				CVector3 V;

				V.x = m11;	V.y = m21;	V.z = m31;
				V.Normalise();
				m11 = V.x;	m21 = V.y;	m31 = V.z;

				V.x = m12;	V.y = m22;	V.z = m32;
				V.Normalise();
				m12 = V.x;	m22 = V.y;	m32 = V.z;

				V.x = m13;	V.y = m23;	V.z = m33;
				V.Normalise();
				m13 = V.x;	m23 = V.y;	m33 = V.z;
			}
		};					
		
		LYNXFORCEINLINE void PointXMatrix(CVector3& r, const CVector3& v, const CMatrix4& m) { lynxPointXMatrix(&r, &v, &m); };				
		LYNXFORCEINLINE void VectorXMatrix(CVector3& r, const CVector3& v, const CMatrix4& m) { lynxVectorXMatrix(&r, &v, &m); };
		LYNXFORCEINLINE void VectorXMatrix(CVector4& r, const CVector4& v, const CMatrix4& m) { lynxVector4XMatrix(&r, &v, &m); };				
		LYNXFORCEINLINE void TransposeMatrix(CMatrix4& r, const CMatrix4& a) { lynxTransposeMatrix2(&r, &a); };				
		LYNXFORCEINLINE void InvertMatrix(CMatrix4& r, const CMatrix4& a) { lynxInvertMatrix(&r, &a); };
		LYNXFORCEINLINE void InverseMatrix(CMatrix4& r, const CMatrix4& a) { lynxInverseMatrix(&r, &a); };
		LYNXFORCEINLINE void MatrixXMatrix(CMatrix4& r, const CMatrix4& a, const CMatrix4& b) { lynxMatrixXMatrix((LPLYNXMATRIX)&r, (LPLYNXMATRIX)&a, (LPLYNXMATRIX)&b); };				
		LYNXFORCEINLINE void MakeRotateMatrix(CMatrix4& r, CReal& deg, LYNXAXISTYPE axis) { lynxMakeRotateMatrix(&r, deg, axis); };		
		
		//LYNXFORCEINLINE void Transform(CVector3& r, const CVector3& a, const CMatrix4& b) { lynxPointXMatrix((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXMATRIX)&b); };
		//LYNXFORCEINLINE CVector3 Transform(const CVector3& a, const CMatrix4& b) { LYNXVECTOR3D V; lynxPointXMatrix(&V, (LPLYNXVECTOR3D)&a, (LPLYNXMATRIX)&b); return CVector3(&V); };
		//LYNXFORCEINLINE void Orient(CVector3& r, const CVector3& a, const CMatrix4& b) { lynxVectorXMatrix((LPLYNXVECTOR3D)&r, (LPLYNXVECTOR3D)&a, (LPLYNXMATRIX)&b); };
		//LYNXFORCEINLINE CVector3 Orient(const CVector3& a, const CMatrix4& b) { LYNXVECTOR3D V; lynxVectorXMatrix(&V, (LPLYNXVECTOR3D)&a, (LPLYNXMATRIX)&b); return CVector3(&V); };

		LYNXFORCEINLINE CMatrix4& GetIdentityMatrix() { return (*((CMatrix4 *)lynxGetIdentityMatrix())); };

		LYNXFORCEINLINE CMatrix4 operator *(const CMatrix4& lhs, const CMatrix4& rhs) { CMatrix4 M; MatrixXMatrix(M, lhs, rhs); return M; };				
		LYNXFORCEINLINE CVector3 operator *(const CVector3& lhs, const CMatrix4& rhs) { CVector3 V; VectorXMatrix(V, lhs, rhs); return V; };				
		LYNXFORCEINLINE CVector4 operator *(const CVector4& lhs, const CMatrix4& rhs) { CVector4 V; VectorXMatrix(V, lhs, rhs); return V; };				


		/**	
		*  @brief CQuaternion class of the engine.
		* 
		*  The basic object class of the entire engine, every class of the engine is drived from this class.
		*  It records a unique ID and a reference counter. 
		*	
		*/
		class LYNXENGCLASS CQuaternion : public LYNXQUATERNION
		{			
		public:						
			LYNXFORCEINLINE CQuaternion() {x = y = z = 0.0f; w = 1.0f; };
			LYNXFORCEINLINE CQuaternion(LYNXQUATERNION& q) {w = q.w; x = q.x; y = q.y; z = q.z; };			
			LYNXFORCEINLINE CQuaternion(CReal iw, CReal ix, CReal iy, CReal iz) {w = iw; x = ix; y = iy; z= iz; };			
			~CQuaternion() {};							
		};	
	}
}

#endif