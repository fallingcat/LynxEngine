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

#ifndef __LYNXSHADERCLASS_H__
#define __LYNXSHADERCLASS_H__

#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxRenderer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CShaderClass : public CObj
		{	
			LYNX_DECLARE_CLASS(CShaderClass);
		public:			
			CArray<CRenderer::SHADERMACRO>		m_ShaderMacros;
		protected:		
			CGraphicsSystem*					m_lpGraphicsSystem;	
			CShaderPtr							m_lpShader;				
		public:			
			CShaderClass(CGraphicsSystem* lpg);
			virtual ~CShaderClass();

		#ifdef _UNICODE
			LYNXINLINE bool						operator == (const CAnsiString& name) {return (GetName() == CString(name)); };
		#endif
			LYNXINLINE bool						operator == (const CString& name) {return (GetName() == name); };
			LYNXINLINE bool						operator == (const LYNXCRC& crc) {return (m_CRCCode == crc); };
			LYNXINLINE CGraphicsSystem*			GetlpGraphicsSystem() {return m_lpGraphicsSystem; };
			void								SetlpGraphicsSystem(CGraphicsSystem* pgs) {m_lpGraphicsSystem = pgs; };
			LYNXINLINE CShader*					GetlpShader() {return m_lpShader; };

			virtual LYNXBOOL					vCreate() = 0;
			virtual LYNXBOOL					vCreate(const CString& filename) {return LYNX_FALSE; };
			virtual void						vCreateConstantAndResource() {};
			virtual void						vBindResource() {};
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam) = 0;
			LYNXBOOL							Recompile();
		};		
	}		
	LYNXINLINE bool operator == (GraphicsSystem::CShaderClassPtr& s, const LYNXCRC& crc) {return ((*s) == crc); };		

	LYNXINLINE bool operator == (GraphicsSystem::CShaderClass* lps, const CString& name) {return ((*lps) == name); };		
	LYNXINLINE bool operator == (GraphicsSystem::CShaderClassPtr& s, const CString& name) {return ((*s) == name); };		
	LYNXINLINE bool operator == (CWeakPtr<GraphicsSystem::CShaderClassPtr>& s, const CString& name) {return (s.IsValid()) ? ((*s) == name) : false; };	
#ifdef _UNICODE
	LYNXINLINE bool operator == (GraphicsSystem::CShaderClass* lps, const CAnsiString& name) {return ((*lps) == name); };	
	LYNXINLINE bool operator == (GraphicsSystem::CShaderClassPtr& s, const CAnsiString& name) {return ((*s) == name); };	
	LYNXINLINE bool operator == (CWeakPtr<GraphicsSystem::CShaderClassPtr>& s, const CAnsiString& name) {return (s.IsValid()) ? ((*s) == name) : false; };
#endif
}

#endif