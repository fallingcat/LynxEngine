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

#ifndef __LYNXSHADERCONSTANT_H__
#define __LYNXSHADERCONSTANT_H__

#include <LynxUnknown.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CShaderConstant : public CUnknown
		{	
			LYNX_DECLARE_CLASS(CShaderConstant);
		public:							
			struct ConstantSlot {
				CRenderer::CONSTANTTYPE			Type;
				CAnsiString						Name;
				int								BindPoint;
				int								Size;
				void*							lpData;
			};
		protected:		
			CRenderer::CONSTANTTYPE				m_ConstantType;
			CShader*							m_lpShader;			
			CAnsiString							m_Name;		

			void*								m_lpData;
			int									m_Size;
		public:			
			CShaderConstant(CShader* lps, const char* name);
			virtual ~CShaderConstant();

			LYNXFORCEINLINE CRenderer::CONSTANTTYPE GetConstantType() { return m_ConstantType; };

			virtual void*						vMap() = 0;
			virtual void						vUnmap() = 0;
			virtual LYNXBOOL					vIsValid() = 0;
			virtual void						vSetFloat(const float f) = 0;
			virtual void						vSetFloat(const float *lpdata, int num) = 0;						
			virtual void						vSetInt(const int i) = 0;
			virtual void						vSetInt(const int *lpdata, int num) = 0;
			virtual void						vSetInt2(const int* i) = 0;
			virtual void						vSetInt2(const int *lpdata, int num) = 0;
			virtual void						vSetInt3(const int* i) = 0;
			virtual void						vSetInt3(const int *lpdata, int num) = 0;
			virtual void						vSetInt4(const int* i) = 0;
			virtual void						vSetInt4(const int *lpdata, int num) = 0;
			virtual void						vSetBool(const BOOL b) = 0;
			virtual void						vSetBool(const BOOL *lpdata, int num) = 0;
			virtual void						vSetVector2(const float* lpdata) = 0;
			virtual void						vSetVector2(const float *lpdata, int num) = 0;
			virtual void						vSetVector3(const float* lpdata) = 0;
			virtual void						vSetVector3(const float *lpdata, int num) = 0;
			virtual void						vSetVector4(const float* lpdata) = 0;
			virtual void						vSetVector4(const float *lpdata, int num) = 0;
			virtual void						vSetMatrix(LPCLYNXMATRIX lpm) = 0;
			virtual void						vSetMatrix(LPCLYNXMATRIX lpm, int num) = 0;

			virtual void						vSet() {};
		};		
	}
}

#endif