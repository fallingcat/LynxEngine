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
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Container/LynxPointSpriteContainer.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CPointSpriteContainer::CPointSpriteContainer()		
	{
		m_Type = POINT_SPRITE;		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CPointSpriteContainer::CPointSpriteContainer(CEngine *lpengine)	
	: CContainer(lpengine)
	{
		m_Type = POINT_SPRITE;		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CPointSpriteContainer::~CPointSpriteContainer(void)
	{				
		lynxReleaseVertexArray(&m_VertexArray);
	}			
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CPointSpriteContainer::vLoad(const LYNXCHAR *name, LPLYNXFILE lpf, long offset, LYNXBOOL bbo)
	{
		MaterialSystem::CMaterial* lpMat = ((CEngine *)m_lpEngine)->GetlpMaterialSystem()->vCreateMaterial(MaterialSystem::CMaterial::STANDARD);	
		lpMat->CreateTechnique(1);
		lpMat->SetTechnique(MaterialSystem::CMaterial::SIMPLE, LYNXNEW MaterialSystem::CTechnique(lpMat));
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).CreatePass(1);
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).SetPass(0, LYNXNEW MaterialSystem::CPass(&(lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0))));
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam.resize(1);
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].AddressMode = LYNX_ADDRESS_WRAP;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].CombineFunc = LYNX_CF_MODULATE;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].CombineArg[0] = LYNX_CA_CURRENT;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].CombineArg[1] = LYNX_CA_TEXTURE;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].CombineArg[2] = LYNX_CA_TEXTURE;		
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].TextureAnimation.Create(1);
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].TextureAnimation.LoadTexture(0, 0, name, lpf, offset, LYNX_NORMAL_TEXTURE);		
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_DepthBufferMode = LYNX_DB_NONE;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_CullMode = LYNX_CULL_CCW;

		lpMat->SetVertexLayout(GraphicsSystem::gPointSpriteVertexLayout);
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).LoadShaders(0, _T("builtin/scn/sprite.vso"), _T("builtin/common/lynxDiffuseMap.pso"));

		SetlpCurrentMaterial(lpMat);
		SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);

		lynxCreateVertexArray(&m_VertexArray, &GraphicsSystem::gSpriteVertexLayout, 4, LYNX_BOF_STATIC, LYNX_FALSE);
		lynxLockVertexArray(&m_VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);

			m_lpSV = (GraphicsSystem::SPRITEVERTEX*)m_VertexArray.lpBuffer;
			m_Width = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].TextureAnimation.GetTexture(0).GetWidth();
			m_Height = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].TextureAnimation.GetTexture(0).GetHeight();
			CReal W = m_Width*0.5f;
			CReal H = m_Height*0.5f;
			m_lpSV[0].x = -W;	m_lpSV[0].y = -H;	m_lpSV[0].TexCoord.u = 0.0f; m_lpSV[0].TexCoord.v = 0.0f; m_lpSV[0].Color.PackedColor = 0xffffffff;
			m_lpSV[1].x =  W;	m_lpSV[1].y = -H;	m_lpSV[1].TexCoord.u = 1.0f; m_lpSV[1].TexCoord.v = 0.0f; m_lpSV[1].Color.PackedColor = 0xffffffff;
			m_lpSV[2].x = -W;	m_lpSV[2].y =  H;	m_lpSV[2].TexCoord.u = 0.0f; m_lpSV[2].TexCoord.v = 1.0f; m_lpSV[2].Color.PackedColor = 0xffffffff;
			m_lpSV[3].x =  W;	m_lpSV[3].y =  H;	m_lpSV[3].TexCoord.u = 1.0f; m_lpSV[3].TexCoord.v = 1.0f; m_lpSV[3].Color.PackedColor = 0xffffffff;		

		lynxUnlockVertexArray(&m_VertexArray);			*/

		return LYNX_TRUE;
	}			
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CPointSpriteContainer::vLoad(int num, int length, const LYNXCHAR *name, const LYNXCHAR *ext, LPLYNXFILE lpf, long offset, LYNXBOOL bbo)
	{
		/*MaterialSystem::CMaterial* lpMat = ((CEngine *)m_lpEngine)->GetlpMaterialSystem()->vCreateMaterial(MaterialSystem::CMaterial::STANDARD);
		lpMat->CreateTechnique(1);
		lpMat->SetTechnique(MaterialSystem::CMaterial::SIMPLE, LYNXNEW MaterialSystem::CTechnique(lpMat));
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).CreatePass(1);
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).SetPass(0, LYNXNEW MaterialSystem::CPass(&(lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0))));
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam.resize(1);
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].AddressMode = LYNX_ADDRESS_WRAP;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].CombineFunc = LYNX_CF_MODULATE;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].CombineArg[0] = LYNX_CA_CURRENT;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].CombineArg[1] = LYNX_CA_TEXTURE;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].CombineArg[2] = LYNX_CA_TEXTURE;				
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].TextureAnimation.LoadTextureSeuence(num, length, 0, name, ext, lpf, offset, LYNX_NORMAL_TEXTURE);							
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].TextureAnimation.GetAnimation().SetCurrentFrame(0, LYNX_TRUE);										
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_DepthBufferMode = LYNX_DB_NONE;
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_CullMode = LYNX_CULL_CCW;

		lpMat->SetVertexLayout(GraphicsSystem::gSpriteVertexLayout);
		lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).LoadShaders(0, _T("builtin/scn/sprite.vso"), _T("builtin/common/lynxDiffuseMap.pso"));

		SetlpCurrentMaterial(lpMat);
		SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);

		lynxCreateVertexArray(&m_VertexArray, &GraphicsSystem::gSpriteVertexLayout, 4, LYNX_BOF_STATIC, LYNX_FALSE);
		lynxLockVertexArray(&m_VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);

			m_lpSV = (GraphicsSystem::SPRITEVERTEX*)m_VertexArray.lpBuffer;
			m_Width = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].TextureAnimation.GetTexture(0).GetWidth();
			m_Height = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_ChannelParam[0].TextureAnimation.GetTexture(0).GetHeight();
			CReal W = m_Width*0.5f;
			CReal H = m_Height*0.5f;
			m_lpSV[0].x = -W;	m_lpSV[0].y = -H;	m_lpSV[0].TexCoord.u = 0.0f; m_lpSV[0].TexCoord.v = 0.0f; m_lpSV[0].Color.PackedColor = 0xffffffff;
			m_lpSV[1].x =  W;	m_lpSV[1].y = -H;	m_lpSV[1].TexCoord.u = 1.0f; m_lpSV[1].TexCoord.v = 0.0f; m_lpSV[1].Color.PackedColor = 0xffffffff;
			m_lpSV[2].x = -W;	m_lpSV[2].y =  H;	m_lpSV[2].TexCoord.u = 0.0f; m_lpSV[2].TexCoord.v = 1.0f; m_lpSV[2].Color.PackedColor = 0xffffffff;
			m_lpSV[3].x =  W;	m_lpSV[3].y =  H;	m_lpSV[3].TexCoord.u = 1.0f; m_lpSV[3].TexCoord.v = 1.0f; m_lpSV[3].Color.PackedColor = 0xffffffff;		

		lynxUnlockVertexArray(&m_VertexArray);	*/
		
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CPointSpriteContainer::vPlay(CReal step)
	{		
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPointSpriteContainer::SetSize(CReal w, CReal h)
	{
		/*m_Width = w;
		m_Height = h;
		float W = m_Width*0.5f;
		float H = m_Height*0.5f;
		m_lpSV[0].x = -W;	m_lpSV[0].y = -H;	
		m_lpSV[1].x =  W;	m_lpSV[1].y = -H;	
		m_lpSV[2].x = -W;	m_lpSV[2].y =  H;	
		m_lpSV[3].x =  W;	m_lpSV[3].y =  H;	
		SetAlignType(m_AlignType);*/
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CPointSpriteContainer::vRender(CCameraContainer* cam)
	{		
		//CMatrix4 TM, PM, WPM;
		//
		//lynxOrthoProjectionMatrixYDown(&PM, 0, 640, 0, 480, 0, 1);
		//TM = m_M;
		//TM.m[3][0] += m_AlignOffset.s.x;
		//TM.m[3][1] += m_AlignOffset.s.y;
		//lynxMatrixXMatrix(&WPM, &TM, &PM);

		//lynxSetVertexArray(&m_VertexArray, 0, 0);				
		//MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		//for (int p=0; p<Technique.GetNumPass(); ++p)				
		//{
		//	Technique.GetPass(p).Set();		
		//	lynxSetVertexShaderConstantTableMatrix(lynxGetCurrentVertexShader(), "gWorldViewProj", &WPM);
		//	lynxDrawArray(LYNX_PRIMITIVE_TRIANGLE_STRIP, 2, 0);
		//	//lynxDrawArrayBO(LYNX_PRIMITIVE_TRIANGLE_STRIP, 2, 0);
		//}
	}
}