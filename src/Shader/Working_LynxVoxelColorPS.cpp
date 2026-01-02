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
#include <GraphicsSystem/LynxShaderClass.h>
#include <MaterialSystem/LynxMaterial.h>

namespace LynxEngine 
{	
	namespace Shader
	{
		template <LYNXBOOL bFakeAO, LYNXBOOL bFakeAOLevel2>	
		class CVoxelColorPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CVoxelColorPS);
		public:						
		protected:					
		public:			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CVoxelColorPS(GraphicsSystem::CGraphicsSystem *pgs)
			: CShaderClass(pgs)
			{	
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			~CVoxelColorPS(void)
			{
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL vCreate() 
			{ 
				GraphicsSystem::CRenderer::SHADERMACRO ShaderMacro;
	            
				m_ShaderMacros.clear();
	            
				ShaderMacro.Name = CAnsiString("FAKE_VOXEL_AO");
				#ifdef __FAKE_VOXEL_AO__
					ShaderMacro.Definition = CAnsiString("1");
				#else
					ShaderMacro.Definition = CAnsiString("0");
				#endif
				m_ShaderMacros.push_back(ShaderMacro);

				ShaderMacro.Name = CAnsiString("USE_FAKE_VOXEL_AO");
				ShaderMacro.Definition = bFakeAO ? CAnsiString("1") : CAnsiString("0");
				m_ShaderMacros.push_back(ShaderMacro);

				ShaderMacro.Name = CAnsiString("FAKE_AO_LEVEL2");
				ShaderMacro.Definition = bFakeAOLevel2 ? CAnsiString("1") : CAnsiString("0");
				m_ShaderMacros.push_back(ShaderMacro);
	            
				return m_lpGraphicsSystem->GetlpRenderer()->LoadPixelShader(m_lpShader, _T("scn/voxelcolor.pso"), &m_ShaderMacros);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void vSet(CContainer* lpc, CCameraContainer* cam) 
			{ 			
				GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
				lpR->SetPixelShader((GraphicsSystem::CPixelShader*)((GraphicsSystem::CShader*)m_lpShader));
			}
		};		
	}		
}
		
#define IMPLEMENT_VOXELCOLORPS(bFakeAO, bFakeAOLevel2) \
	namespace Shader{\
		typedef CVoxelColorPS<bFakeAO, bFakeAOLevel2>		 CVoxelColorPS_##bFakeAO##_##bFakeAOLevel2;\
	}\
	IMPLEMENT_SHADERCLASS(VoxelColorPS_##bFakeAO##_##bFakeAOLevel2)

namespace LynxEngine
{
	IMPLEMENT_VOXELCOLORPS(FALSE,	FALSE)	// FakeAO : 0, AOLevel2 : 0
	IMPLEMENT_VOXELCOLORPS(TRUE,	FALSE)	// FakeAO : 1, AOLevel2 : 0
	IMPLEMENT_VOXELCOLORPS(TRUE,	TRUE)	// FakeAO : 1, AOLevel2 : 1
}
