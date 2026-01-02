#include <AdvMaterialSystem/LynxAdvMaterialSystem.h>
#include <SceneSystem/StaticObj/LynxStaticObj_Simple_Lighting_ShadowMap_Material.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{
		struct LIGHTDATA
		{
			float				Type;
			float				Color[3];
			float				Data[3];
		};
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObjSimpleLightingShadowmapMaterial::CStaticObjSimpleLightingShadowmapMaterial(void)
		{	
			m_CustFactor = 3.0f;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObjSimpleLightingShadowmapMaterial::~CStaticObjSimpleLightingShadowmapMaterial(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStaticObjSimpleLightingShadowmapMaterial::vLoad(LPLYNXFILE lpf)
		{		
			LPLYNXTEXTUREFRAME							lpTexFrame;		
			char										Path[128];

			Create(1);
			LYNX_READ_FILE(&m_lpMaterialPass[0].Diffuse, sizeof(LYNXCOLORRGBA), 1, lpf);
			LYNX_READ_FILE(&m_lpMaterialPass[0].Specular, sizeof(LYNXCOLORRGBA), 1, lpf);		
			LYNX_READ_FILE(&m_lpMaterialPass[0].TextureFrame[0].MipMapLevel, sizeof(WORD), 1, lpf);														
			m_lpMaterialPass[0].TextureFrame[0].MipMapLevel = 0;
			LYNX_READ_FILE(&m_lpMaterialPass[0].TextureFrame[0].NumTexs, sizeof(int), 1, lpf);					
			if (m_lpMaterialPass[0].TextureFrame[0].NumTexs > 0)
			{
				m_lpMaterialPass[0].TextureFrame[0].lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*m_lpMaterialPass[0].TextureFrame[0].NumTexs);
				m_lpMaterialPass[0].TextureFrame[0].lpTexIndexList = LYNXNEW LYNXTEXTUREINDEX[m_lpMaterialPass[0].TextureFrame[0].NumTexs];
				for (int j=0; j<m_lpMaterialPass[0].TextureFrame[0].NumTexs; j++)
				{
					LYNX_READ_FILE(m_lpMaterialPass[0].TextureFrame[0].lplpTexList[j], sizeof(char), 64, lpf);	
					lynxLoadTexture(&m_lpMaterialPass[0].TextureFrame[0].lpTexIndexList[j], NULL, m_lpMaterialPass[0].TextureFrame[0].MipMapLevel, m_lpMaterialPass[0].TextureFrame[0].lplpTexList[j], NULL, 0, LYNX_NORMAL_TEXTURE);									
				}
			}			
			lpTexFrame = m_lpMaterialPass[0].GetlpTextureFrame(1);
			lpTexFrame->NumTexs = 1;
			lpTexFrame->lpTexIndexList = LYNXNEW LYNXTEXTUREINDEX[lpTexFrame->NumTexs];
			lpTexFrame->lpTexIndexList[0] = ((AMS::CAdvMaterialSystem *)m_lpMaterialSystem)->GetShadowMapTexIndex();			
			
			lpTexFrame = m_lpMaterialPass[0].GetlpTextureFrame(2);
			lynxLoadTextureFrameTextures(lpTexFrame, NULL, 0, 32, "caust", "tga", NULL, 0, LYNX_NORMAL_TEXTURE);
			m_lpMaterialPass[0].AddressMode[2] = LYNX_ADDRESS_WRAP;

			m_lpMaterialPass[0].bUseFog = LYNX_TRUE;							
			lynxLoadCompiledVertexShader(&m_lpMaterialPass[0].VS, lynxGetRenderVertexDecl(), "buildin/scn/staticobj_simple_lighting_Shadowmap.vso", NULL, 0);									
			lynxLoadCompiledPixelShader(&m_lpMaterialPass[0].PS, "buildin/scn/staticobj_simple_lighting_shadowmap.pso", NULL, 0);					
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStaticObjSimpleLightingShadowmapMaterial::vSetParameter(void)
		{		
			LYNXMATRIX			WVPM, WVM;			
			
			int					NumLights;
			LIGHTDATA			LightData[3];	

			NumLights = 2;
			LightData[0].Type = 0;
			LightData[0].Color[0] = 0.2f;
			LightData[0].Color[1] = 0.2f;
			LightData[0].Color[2] = 0.2f;
			LightData[1].Type = 2;
			LightData[1].Color[0] = 1.0f;
			LightData[1].Color[1] = 1.0f;
			LightData[1].Color[2] = 1.0f;
			LightData[1].Data[0] = -0.5773f;
			LightData[1].Data[1] = -0.5773f;
			LightData[1].Data[2] = 0.5773f;			

			LightData[1].Type = 2;
			LightData[1].Color[0] = 1.0f;
			LightData[1].Color[1] = 1.0f;
			LightData[1].Color[2] = 1.0f;
			LightData[1].Data[0] = -0.5773f;
			LightData[1].Data[1] = -0.5773f;
			LightData[1].Data[2] = 0.5773f;			

			lynxSetVertexShaderConstantTableI(m_lpMaterialPass[0].VS, "gNumLights", &NumLights, 1);			
			lynxSetVertexShaderConstantTable(m_lpMaterialPass[0].VS, "gLightList[0].Type", (float *)&LightData[0].Type, sizeof(float));			
			lynxSetVertexShaderConstantTable(m_lpMaterialPass[0].VS, "gLightList[0].Color", (float *)LightData[0].Color, sizeof(float)*3);			
			lynxSetVertexShaderConstantTable(m_lpMaterialPass[0].VS, "gLightList[0].Data", (float *)LightData[0].Data, sizeof(float)*3);			
			lynxSetVertexShaderConstantTable(m_lpMaterialPass[0].VS, "gLightList[1].Type", (float *)&LightData[1].Type, sizeof(float));			
			lynxSetVertexShaderConstantTable(m_lpMaterialPass[0].VS, "gLightList[1].Color", (float *)LightData[1].Color, sizeof(float)*3);						
			lynxSetVertexShaderConstantTable(m_lpMaterialPass[0].VS, "gLightList[1].Data", (float *)LightData[1].Data, sizeof(float)*3);						
			

			LYNXCAMERA				m_ShadowMapCamera;
			LYNXVECTOR3D			Pos;

			lynxInitCamera(&m_ShadowMapCamera);
			LYNX_VECTOR3D(Pos, -0.780297f, 0.468826f, -14.0453f);
			m_ShadowMapCamera.Obj3D.lpCenter->s.x = Pos.s.x + 10.0f; 
			m_ShadowMapCamera.Obj3D.lpCenter->s.y = Pos.s.y + 10.0f; 
			m_ShadowMapCamera.Obj3D.lpCenter->s.z = Pos.s.z + 10.0f; 
			lynxCameraLookAt(&m_ShadowMapCamera, &Pos);		


			LYNXMATRIX						RM, MAT, ProjM, M;			

			LYNX_MAKE_MATRIX(M, 0.5f, 0.0f, 0.0f, 0.0f,
								0.0f, -0.5f, 0.0f,0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.5f, 0.5f, 1.0f, 0.0f);		
			
			lynxMatrixXMatrix(&RM, &lynxGetlpRenderer()->ProjectionMatrix, &M);	//New projective matrix
			lynxMatrixXMatrix(&MAT, &m_ShadowMapCamera.ViewMatrix, &RM);												
			m_lpMaterialPass[0].SetVSParamMatrix("gProjectiveTextureMatrix", &MAT);// Vertex*MAT = projective texture coord		
			m_lpMaterialPass[0].SetVSParamFloat("gCustFactor", &m_CustFactor, 1);// Vertex*MAT = projective texture coord	

			lynxSetTextureAddressMode(1, LYNX_ADDRESS_CLAMP);			
		}		
	}
}