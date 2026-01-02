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
#include <GraphicsSystem/LynxStreamingTexture.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStreamingTexture::CStreamingTexture(CGraphicsSystem *pgs)
		: CResource(pgs->GetlpEngine())
		{	
			m_ResourceType = STREAMING_TEXTURE;
			m_lpGraphicsSystem = pgs;		
			m_BeginningLOD = -1;			
			m_bStreamingDone = LYNX_FALSE;

			IMPLEMENT_PARAM_INT(m_BeginningLOD, 0)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStreamingTexture::~CStreamingTexture(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CStreamingTexture::Load(const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc, LPLYNXFILE lpfs, long offset)
		{			
			return LYNX_TRUE;
		}		
	}
}