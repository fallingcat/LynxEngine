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

#ifndef __LYNXOGLES3SHADERCONSTANT_H__
#define __LYNXOGLES3SHADERCONSTANT_H__

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/LynxShaderConstant.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES3ShaderConstant : public CShaderConstant
		{	
			LYNX_DECLARE_CLASS(COGLES3ShaderConstant);
		public:						
		protected:	
			GLint								m_Location;
		public:			
			COGLES3ShaderConstant(CShader* lps, const char* name);
			virtual ~COGLES3ShaderConstant();

			void*								vMap() {return NULL; };
			void								vUnmap() {};

			LYNXFORCEINLINE LYNXBOOL			vIsValid() {return ((m_Location >= 0) ? LYNX_TRUE : LYNX_FALSE); };

			void								vSetFloat(const float f);
			void								vSetFloat(const float *lpdata, int num);
			void								vSetInt(const int i);
			void								vSetInt(const int *lpdata, int num);
			void								vSetInt2(const int* i);
			void								vSetInt2(const int *lpdata, int num);
			void								vSetInt3(const int* i);
			void								vSetInt3(const int *lpdata, int num);
			void								vSetInt4(const int* i);
			void								vSetInt4(const int *lpdata, int num);
			void								vSetBool(const BOOL b);
			void								vSetBool(const BOOL *lpdata, int num);
			void								vSetVector2(const float* lodata);
			void								vSetVector2(const float *lpdata, int num);
			void								vSetVector3(const float* lodata);
			void								vSetVector3(const float *lpdata, int num);
			void								vSetVector4(const float* lodata);
			void								vSetVector4(const float *lpdata, int num);
			void								vSetMatrix(LPCLYNXMATRIX lpm);
			void								vSetMatrix(LPCLYNXMATRIX lpm, int num);
		};		
	}
}

#endif

#endif