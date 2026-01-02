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

#ifndef __LYNXSHADER_H__
#define __LYNXSHADER_H__

#include <LynxResource.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CShader : public CResource
		{	
			LYNX_DECLARE_CLASS(CShader);
		public:						
		protected:		
			CGraphicsSystem*					m_lpGraphicsSystem;	

			//CArray<CParam>					m_ParamArray;
			CArray<CShaderConstantGroup*>		m_ConstantGroupArray;

			LynxEngine::CMap<CString, GraphicsSystem::CShaderConstant*>	m_ConstantMap;			
			LynxEngine::CMap<CString, GraphicsSystem::CShaderResource*>	m_ResourceMap;
		public:			
			CShader(CGraphicsSystem* lpg);
			virtual ~CShader();

			LYNXFORCEINLINE CGraphicsSystem*	GetlpGraphicsSystem() {return m_lpGraphicsSystem; };
			static void							ComputeFilenameWithMacros(CString& outname, CRenderer* lpr, const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def);
			void								ComputeFilenameWithMacros(CString& outname, const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def);

			virtual LYNXBOOL					vIsValid() = 0;
			virtual LYNXBOOL					vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL) = 0;
			virtual LYNXBOOL					vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def = NULL, CStream* stream = NULL) = 0;			
			
            LYNXBOOL                            CreateResource(const char* name);			
			LYNXFORCEINLINE GraphicsSystem::CShaderResource* GetResource(const char* name) {return m_ResourceMap[name]; };

			virtual void						vSetConstantF(const char *name, const float f) = 0;
			virtual void						vSetConstantF(const char *name, const float *lpdata, int num) = 0;
			virtual void						vSetConstantI(const char *name, const int i) = 0;
			virtual void						vSetConstantI(const char *name, const int *lpdata, int num) = 0;
			virtual void						vSetConstantB(const char *name, const BOOL b) = 0;
			virtual void						vSetConstantB(const char *name, const BOOL *lpdata, int num) = 0;
			virtual void						vSetConstantV2(const char *name, const float *lpdata) = 0;
			virtual void						vSetConstantV2(const char *name, const float *lpdata, int num) = 0;
			virtual void						vSetConstantV3(const char *name, const float *lpdata) = 0;
			virtual void						vSetConstantV3(const char *name, const float *lpdata, int num) = 0;
			virtual void						vSetConstantV4(const char *name, const float *lpdata) = 0;
			virtual void						vSetConstantV4(const char *name, const float *lpdata, int num) = 0;
			virtual void						vSetConstantM(const char *name, LPCLYNXMATRIX lpm) = 0;
			virtual void						vSetConstantM(const char *name, LPCLYNXMATRIX lpm, int num) = 0;
			virtual void						vSetTexture(const int channel, CTexture* t) = 0;
			virtual	void						vSetTexture(const char *name, CTexture* t) = 0;
			virtual	void						vSetSampler(const char *name, CSamplerState* lps) = 0;
			virtual	int							vGetTextureIndex(const char *name) = 0;

		protected:
			virtual void						vSet() = 0;

		friend class CRenderer;
		};		
	}		
}

#endif