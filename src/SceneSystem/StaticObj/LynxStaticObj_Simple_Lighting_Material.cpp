#include <SceneSystem/StaticObj/LynxStaticObj_Simple_Lighting_Material.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{
		struct LIGHTDATA
		{
			float				Type;
			float				Color[3];
			float				Data[4];
		};
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObjSimpleLightingMaterial::CStaticObjSimpleLightingMaterial(void)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObjSimpleLightingMaterial::~CStaticObjSimpleLightingMaterial(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStaticObjSimpleLightingMaterial::vLoad(LPLYNXFILE lpf)
		{		
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
			m_lpMaterialPass[0].bUseFog = LYNX_TRUE;							
			lynxLoadCompiledVertexShader(&m_lpMaterialPass[0].VS, lynxGetRenderVertexDecl(), "buildin/scn/staticobj_simple_lighting.vso", NULL, 0);									
			lynxLoadCompiledPixelShader(&m_lpMaterialPass[0].PS, "buildin/scn/staticobj_simple_lighting.pso", NULL, 0);					
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStaticObjSimpleLightingMaterial::vSetParameter(void)
		{		
			LYNXMATRIX			WVPM, WVM;			
			int					NumLights;
			LIGHTDATA			LightData[2];	

			/*
			lynxMatrixXMatrix(&WVM, &lynxGetlpRender()->WorldMatrix, &lynxGetlpRender()->ViewMatrix);	
			lynxMatrixXMatrix(&WVPM, &WVM, &lynxGetlpRender()->ProjectionMatrix);	
			lynxSetVertexShaderConstantTableMatrix(m_VS, "gWorldViewProj", &WVPM);
			lynxSetVertexShaderConstantTableMatrix(m_VS, "gWorldView", &WVM);			
			*/			

			NumLights = 2;
			LightData[0].Type = 0;
			LightData[0].Color[0] = 1.2f;
			LightData[0].Color[1] = 1.2f;
			LightData[0].Color[2] = 1.2f;

			//lynxSetVertexShaderConstantTableI(m_VS, "gNumLights", &NumLights, 1);
			//lynxSetVertexShaderConstantTableI(m_VS, "gLightList[0].Type", &LightData[0].Type, 1);
			lynxSetVertexShaderConstantTable(m_lpMaterialPass[0].VS, "gLightList", (float *)LightData, sizeof(LIGHTDATA));
			//lynxSetVertexShaderConstant(100, (float *)LightData, 2);
			//lynxSetVertexShaderConstant(98, (float *)LightData, 2);
			//lynxSetVertexShaderConstant(102, (float *)LightData, 2);


			/*
			float				ClipPlane[4];

			ClipPlane[0] = 0.0f;
			ClipPlane[1] = 1.0f;
			ClipPlane[2] = 0.0f;
			ClipPlane[3] = 0.0f;			
			lynxSetVertexShaderConstantTable(m_VS, "gClipPlane", (float *)&ClipPlane, 4);
			*/
		}		
	}
}