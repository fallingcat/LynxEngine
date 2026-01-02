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
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVertexArray::CVertexArray(CGraphicsSystem* pgs)
		: CResource(pgs->GetlpEngine())
		{
			m_lpGraphicsSystem = pgs;
			m_Stride = 0;
			m_Size = 0;
			m_lpBuffer = NULL;
			m_ResourceType = VERTEX_ARRAY; 	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVertexArray::~CVertexArray()
		{		
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}		
	}
}