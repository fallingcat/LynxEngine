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

#include <LynxEngine_PCH.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShader::CShader(CGraphicsSystem *pgs)
		: CResource(pgs->GetlpEngine())
		{	
			m_lpGraphicsSystem = pgs;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShader::~CShader(void)
		{
			for (CArray<CShaderConstantGroup*>::CIterator i=m_ConstantGroupArray.begin(); i!=m_ConstantGroupArray.end(); ++i)			
			{
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstantGroup(*i);
			}
			m_ConstantGroupArray.clear();

			for (LynxEngine::CMap<CString, CShaderResource*>::CIterator i=m_ResourceMap.begin(); i!=m_ResourceMap.end(); ++i)			
			{
				if ( (*i).second )
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource((*i).second);				
			}
			m_ResourceMap.clear();

			for (LynxEngine::CMap<CString, CShaderConstant*>::CIterator i=m_ConstantMap.begin(); i!=m_ConstantMap.end(); ++i)			
			{
				if ( (*i).second )
					m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderConstant((*i).second);				
			}
			m_ConstantMap.clear();

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CShader::ComputeFilenameWithMacros(CString& outname, CRenderer* lpr, const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def)
		{
			//outname = filename;

			outname.clear();

			//Add macros to the end of filename
			//Global shader macros
			for (int i=0; i<lpr->m_ShaderMacros.size(); i++)
			{
				outname += CString(_T("_")) + CString(lpr->m_ShaderMacros[i].Name) + CString(_T("_")) + CString(lpr->m_ShaderMacros[i].Definition); 
			}
			//Local shader macros
			if (def)
			{
				for (int i=0; i<def->size(); i++)
					outname += CString(_T("_")) + CString((*def)[i].Name) + CString(_T("_")) + CString((*def)[i].Definition); 
			}			

			LYNXCHAR CRCString[16];
			LYNXCRC CRCCode = lynxComputeCRCFast((BYTE*)outname.c_str(), outname.size()*sizeof(LYNXCHAR));
			lynxSprintf(CRCString, _T("#%08X"), (int)CRCCode);
			outname = filename;
			outname += CRCString;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CShader::ComputeFilenameWithMacros(CString& outname, const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def)
		{
			CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			ComputeFilenameWithMacros(outname, lpR, filename, def);
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CShader::CreateResource(const char* name)
		{
			if (m_ResourceMap[name])
				m_lpGraphicsSystem->GetlpRenderer()->ReleaseShaderResource(m_ResourceMap[name]);
            
			m_ResourceMap[name] = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderResource(this, name);
            
			return LYNX_TRUE;
		}
	}	
}
