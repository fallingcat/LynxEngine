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

#ifndef __LYNXNULLSHADERCONSTANT_H__
#define __LYNXNULLSHADERCONSTANT_H__

#include <GraphicsSystem/LynxShaderConstant.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullShaderConstant : public CShaderConstant
		{	
			LYNX_DECLARE_CLASS(CNullShaderConstant);
		public:						
		protected:				
		public:			
			CNullShaderConstant(CShader* lps, const char* name):CShaderConstant(lps, name) {};
			virtual ~CNullShaderConstant() {};

			void*								vMap() {return NULL; };
			void								vUnmap() {};

			LYNXFORCEINLINE LYNXBOOL			vIsValid() {return LYNX_FALSE; };

			void								vSetFloat(const float f) {};
			void								vSetFloat(const float *lpdata, int num) {};
			void								vSetInt(const int i) {};
			void								vSetInt(const int *lpdata, int num) {};
			void								vSetInt2(const int* i) {};
			void								vSetInt2(const int *lpdata, int num) {};
			void								vSetInt3(const int* i) {};
			void								vSetInt3(const int *lpdata, int num) {};
			void								vSetInt4(const int* i) {};
			void								vSetInt4(const int *lpdata, int num) {};
			void								vSetBool(const BOOL b) {};
			void								vSetBool(const BOOL *lpdata, int num) {};
			void								vSetVector2(const float* lpdata) {};
			void								vSetVector2(const float *lpdata, int num) {};
			void								vSetVector3(const float* lpdata) {};
			void								vSetVector3(const float *lpdata, int num) {};
			void								vSetVector4(const float* lpdata) {};
			void								vSetVector4(const float *lpdata, int num) {};
			void								vSetMatrix(LPCLYNXMATRIX lpm) {};
			void								vSetMatrix(LPCLYNXMATRIX lpm, int num) {};
		};		
	}
}

#endif