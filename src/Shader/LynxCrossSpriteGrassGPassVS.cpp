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
#include <Shader/LynxCrossSpriteGrassGPassVS.h>
#include <Container/LynxSprite3DClusterContainer.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCrossSpriteGrassGPassVS::CCrossSpriteGrassGPassVS(GraphicsSystem::CGraphicsSystem *pgs)
		: CCrossSpriteGrassVS(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCrossSpriteGrassGPassVS::~CCrossSpriteGrassGPassVS(void)
		{
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CCrossSpriteGrassGPassVS::vCreate() 
		{ 
			return m_lpGraphicsSystem->GetlpRenderer()->LoadVertexShader(m_lpShader, _T("scn/crossspritegrass_gpass.vso"), &m_ShaderMacros);
		}		
	}
}
