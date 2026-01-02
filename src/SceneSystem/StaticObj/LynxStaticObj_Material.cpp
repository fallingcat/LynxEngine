#include <AdvMaterialSystem/LynxAdvMaterialSystem.h>
#include <SceneSystem/StaticObj/LynxStaticObj_Material.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObjMaterial::CStaticObjMaterial(void)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStaticObjMaterial::~CStaticObjMaterial(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStaticObjMaterial::vLoad(LPLYNXFILE lpf)
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
			lynxLoadCompiledVertexShader(&m_lpMaterialPass[0].VS, lynxGetRenderVertexDecl(), "buildin/scn/staticobj.vso", NULL, 0);									
			lynxLoadCompiledPixelShader(&m_lpMaterialPass[0].PS, "buildin/scn/staticobj_simple_lighting.pso", NULL, 0);					
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CStaticObjMaterial::vSetParameter(void)
		{				
		}		
	}
}