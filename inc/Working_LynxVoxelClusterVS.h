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
#include <GraphicsSystem/LynxShaderClass.h>
#include <Container/LynxCameraContainer.h>
#include <Container/LynxVoxelClusterContainer.h>
#include <Shader/LynxVoxelClusterVS.h>


#define IMPLEMENT_VOXELCLUSTERVS(bFakeAO, bFakeAOLevel2, bSpecular) \
	namespace Shader{\
		typedef CVoxelClusterOneLightingVS<bFakeAO, bFakeAOLevel2, bSpecular>		 CVoxelClusterOneLightingVS_##bFakeAO##_##bFakeAOLevel2##_##bSpecular;\
	}\
	IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingVS_##bFakeAO##_##bFakeAOLevel2##_##bSpecular)

#define REGISTER_VOXELCLUSTERVS_CLASS(classname, bFakeAO, bFakeAOLevel2, bSpecular) \
	m_ShaderClassManager.Register(classname, Create##VoxelClusterOneLightingVS_##bFakeAO##_##bFakeAOLevel2##_##bSpecular);