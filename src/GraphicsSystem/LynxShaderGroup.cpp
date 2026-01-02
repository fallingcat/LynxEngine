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
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//TODO : CShaderGroup need to be managed by CEngine so np duplicated shader group will be created for OGL 
		//		 renderer. Currently, for OGL renderer. New ShaderGroup will be created when CreateShaderGroup()
		//		 is called no matter the shaders are duplicated or not.

		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShaderGroup::CShaderGroup(CGraphicsSystem *pgs)
		: CResource(pgs->GetlpEngine())
		{	
			m_lpGraphicsSystem = pgs; 
			m_ResourceType	= SHADER_GROUP; 			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CShaderGroup::~CShaderGroup(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CShaderGroup::vCreate()
		{
			//CString Name = m_lpVertexLayout->GetName() + CString(_T(".")) + m_lpVertexShader->GetName() + CString(_T(".")) + m_lpPixelShader->GetName();
			CString Name = m_lpVertexShader->GetName() + CString(_T(".")) + m_lpPixelShader->GetName();
			SetName(Name);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CShaderGroup::vSet(CContainer* lpo, CCameraContainer* cam)
		{
			m_lpGraphicsSystem->GetlpRenderer()->SetVertexLayout(m_lpVertexLayout);
			m_lpVertexShader->vSet(lpo, cam);	
			m_lpPixelShader->vSet(lpo, cam);
		}
	}
}