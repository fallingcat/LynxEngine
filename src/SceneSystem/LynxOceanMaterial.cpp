#include <AdvMaterialSystem/LynxAdvMaterialSystem.h>
#include <SceneSystem/LynxOceanMaterial.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COceanMaterial::COceanMaterial(void)
		{
			m_WaveSpeed = 0.0012f;
			m_Time = 0.0f;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COceanMaterial::~COceanMaterial(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COceanMaterial::vInit(void)
		{	
			LPLYNXTEXTUREFRAME		lpTexFrame;
			char					Path[256];

			Create(1);
			m_lpMaterialPass[0].bUseFog = LYNX_TRUE;
			m_lpMaterialPass[0].CullMode = LYNX_CULL_NONE;
			m_lpMaterialPass[0].AddressMode[0] = LYNX_ADDRESS_WRAP;
			m_lpMaterialPass[0].AddressMode[1] = LYNX_ADDRESS_WRAP;
			m_lpMaterialPass[0].AddressMode[2] = LYNX_ADDRESS_CLAMP;
			m_lpMaterialPass[0].AddressMode[3] = LYNX_ADDRESS_CLAMP;
			
			/*
			lpTexFrame = GetlpTextureFrame(1);
			lpTexFrame->NumTexs = 27;
			lpTexFrame->lpTexIndexList = LYNXNEW LYNXTEXTUREINDEX[lpTexFrame->NumTexs];
			for (int i=0; i<27; i++)
			{
				lynxSprintf(Path, "M0502_000%02d.tga", i);
				lynxLoadTexture(&lpTexFrame->lpTexIndexList[i], NULL, 0, Path, NULL, 0, LYNX_NORMAL_TEXTURE);
			}
			lpTexFrame->StartTime = 0.0f;
			lpTexFrame->EndTime = 27.0f;
			*/


			lpTexFrame = m_lpMaterialPass[0].GetlpTextureFrame(1);
			lpTexFrame->NumTexs = 1;
			lpTexFrame->lpTexIndexList = LYNXNEW LYNXTEXTUREINDEX[lpTexFrame->NumTexs];
			lpTexFrame->lpTexIndexList[0] = m_lpMaterialPass[0].GetlpTextureFrame(0)->lpTexIndexList[0];

			lpTexFrame = m_lpMaterialPass[0].GetlpTextureFrame(2);
			lpTexFrame->NumTexs = 1;
			lpTexFrame->lpTexIndexList = LYNXNEW LYNXTEXTUREINDEX[lpTexFrame->NumTexs];	
			lpTexFrame->lpTexIndexList[0] = ((AMS::CAdvMaterialSystem *)m_lpMaterialSystem)->GetReflectionTexIndex();
			//lpTexFrame->lpTexIndexList[0] = ((AdvMaterialSystem::CAdvMaterialSystem *)m_lpMaterialSystem)->GetBlurTexIndex(1);			

			lpTexFrame = m_lpMaterialPass[0].GetlpTextureFrame(3);
			lpTexFrame->NumTexs = 1;
			lpTexFrame->lpTexIndexList = LYNXNEW LYNXTEXTUREINDEX[lpTexFrame->NumTexs];	
			lpTexFrame->lpTexIndexList[0] = ((AMS::CAdvMaterialSystem *)m_lpMaterialSystem)->GetGlowTexIndex();

			lynxSprintf(Path, "buildin/scn/ocean.vso");
			m_lpMaterialPass[0].LoadVS(Path, lynxGetRenderVertexDecl(), NULL, 0);		

			lynxSprintf(Path, "buildin/scn/ocean.pso");
			m_lpMaterialPass[0].LoadPS(Path, NULL, 0);				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COceanMaterial::vPlayAll(LYNXREAL skip, LYNXPLAYMODE pm)
		{
			CAdvMaterial::vPlayAll(skip, pm);
			m_Time += m_WaveSpeed*skip;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COceanMaterial::vSetParameter(void)
		{	
			static float					gRefractionFactor[4] = {0.04f, 0.07f, 0.05f, 1.0f};
			static float					gLightDir[4] = {1.0f, -1.0f, 1.0f, 1.0f};
			LYNXMATRIX						RM, MAT, ProjM, M;

			LYNX_MAKE_MATRIX(M, 0.5f, 0.0f, 0.0f, 0.0f,
								0.0f, -0.5f, 0.0f,0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.5f, 0.5f, 1.0f, 0.0f);		
			
			lynxMatrixXMatrix(&RM, &lynxGetlpRenderer()->ProjectionMatrix, &M);	//New projective matrix
			lynxMatrixXMatrix(&MAT, &lynxGetlpRenderer()->ViewMatrix, &RM);									
			//lynxTransposeMatrix(&MAT);
			m_lpMaterialPass[0].SetVSParamFloat("gTime", &m_Time, 1);			
			m_lpMaterialPass[0].SetVSParamBool("gbUnderWater", &m_bUnderWater, 1);						
			m_lpMaterialPass[0].SetVSParamMatrix("gProjectiveTextureMatrix", &MAT);// Vertex*MAT = projective texture coord					

			/*
				D3DXVECTOR4 c16((float)g_fTime*0.75f, sinf(g_fTime), 0.0f, 0.0f);	
				D3DXVECTOR4 c17(0.031f, 0.04f, -0.03f, 0.02f);

				lynxSetVertexShaderConstant(16, c16, 1);
				lynxSetVertexShaderConstant(17, c17, 1);

				LYNX_MAKE_MATRIX(M, 0.5f, 0.0f, 0.0f, 0.0f,
									0.0f, -0.5f, 0.0f,0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.5f, 0.5f, 1.0f, 0.0f);		

				lynxMatrixXMatrix(&RM, &lynxGetlpRenderer()->ProjectionMatrix, &M);	//New projective matrix
				lynxMatrixXMatrix(&MAT, &lynxGetlpRenderer()->ViewMatrix, &RM);									
				lynxTransposeMatrix(&MAT);
				lynxSetVertexShaderConstant(10, (float *)&MAT, 4);	// Vertex*MAT = projective texture coord
				
				D3DXVECTOR4 c0p(0.0f, 0.5f, 1.0f, 0.25f);
				D3DXVECTOR4 c1p(0.8f, 0.76f, 0.62f, 1.0f);
				//D3DXVECTOR4 c1p(0.6f, 0.6f, 0.6f, 1.0f);	
				lynxSetPixelShaderConstant(0, c0p, 1);
				lynxSetPixelShaderConstant(1, c1p, 1);		
				lynxSetPixelShaderConstant(6, gRefractionFactor, 1);				
				lynxSetPixelShaderConstant(7, gLightDir, 1);						
				lynxGetlpRenderer()->SystemData3D.lpD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); 		
				*/

			/*
			m_WaveSpeed += (rand()%100-200)/100000.0f;
			if (m_WaveSpeed > 0.003f)
				m_WaveSpeed = 0.003f;
			else if (m_WaveSpeed < -0.003f)
				m_WaveSpeed = -0.003f;
				*/			
			
			
			/*
			LPLYNXTEXTUREFRAME		lpTexFrame;
			lpTexFrame = GetlpTextureFrame(1);
			lpTexFrame->Time += 1.0f;
			if (lpTexFrame->Time > 26.0f)
				lpTexFrame->Time = 0.0f;
			*/
			//g_fTime += 0.0002f;
		}		
	}
}