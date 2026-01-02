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
#include <LynxEngine.h>
#include <LynxRenderableObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <Container/LynxCameraContainer.h>
#include <Shader/LynxSprite3DClusterGPassVS.h>
#include <Container/LynxSprite3DClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSprite3DClusterGPassVS::CSprite3DClusterGPassVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CSprite3DClusterVS(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSprite3DClusterGPassVS::~CSprite3DClusterGPassVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSprite3DClusterGPassVS::vCreate() 
		{ 
			GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
			LYNXCHAR Buffer[128];

			lynxSprintf(Buffer, _T("%d"), CSprite3DClusterContainer::m_BatchSize);
			ShaderMacro.Name = CAnsiString("SPRITE3DCLUSTER_BATCH_SIZE");
			ShaderMacro.Definition = CAnsiString(Buffer);
			m_ShaderMacros.push_back(ShaderMacro);
			
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/sprite3dcluster_gpass.vso"), &m_ShaderMacros);
		}		
	}
}
