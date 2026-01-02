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
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Container/LynxSpriteContainer.h>

namespace LynxEngine 
{	
	IMPLEMENT_CLASSSCRIPT(CSpriteContainer, CContainer)
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CSpriteContainer::CSpriteContainer(CEngine *lpengine)	
	: CContainer(lpengine)
	{
		m_ContainerType = SPRITE;		
		m_OriginType = OT_UPPER_LEFT;
		m_AlignmentType = 0;
		m_AlignmentOffset.x = m_AlignmentOffset.y = 0;
		m_lpSV = NULL;
		m_UV[0].u = 0.0f; m_UV[0].v = 0.0f;
		m_UV[1].u = 1.0f; m_UV[1].v = 0.0f;
		m_UV[2].u = 0.0f; m_UV[2].v = 1.0f;
		m_UV[3].u = 1.0f; m_UV[3].v = 1.0f;
		m_Scale.x = m_Scale.y = m_Scale.z = 1.0f;

		INIT_CLASSSCRIPT
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
	CSpriteContainer::~CSpriteContainer(void)
	{		
		if (m_lpSV)
			LYNXGLOBAL_DEL_ARRAY(m_lpSV);
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
	LYNXBOOL CSpriteContainer::vCreate(const CCreationDesc* desc)
	{
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;
		CStream* Stream = NULL;

		DDesc.Default();		
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		if (!desc->m_Name.empty())
			SetName(desc->m_Name);
		else
			SetName(desc->m_FileName);

		CSpriteCreationDesc* Desc = (CSpriteCreationDesc*)desc;		

		MaterialSystem::CMaterialPtr lpMat;

		if (desc->m_bCreateMaterial)
		{
			CString MatName;

			if (Desc->m_MaterialName.empty())
			{
				MatName = m_Name + CString(_T(".Material"));				
			}
			else
			{
				MatName = Desc->m_MaterialName;
			}

			if (MatName == CString("tutorial_char_base01.png.Material"))
				int xxx = 0;

			lpMat = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MatName);
			if (!lpMat)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpMat, MaterialSystem::CMaterial::STANDARD);
				lpMat->SetName(MatName);
				lpMat->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE));		
				lpMat->m_Diffuse.PackedColor = 0xffffffff;

				lpMat->vSetNumTextureAnimation(1);							
				
				if (Desc->m_lpStream)
					Stream = Desc->m_lpStream;
				else
					Stream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();

				if (Desc->m_Number > 1)
				{
					lpMat->GetTextureAnimation(0)->LoadTextureSeuence(Desc->m_Number, Desc->m_Length, *(Stream), Desc->m_FileName.c_str(), Desc->m_FileExt.c_str(), &Desc->m_TextureDesc);										
				}
				else
				{
					lpMat->GetTextureAnimation(0)->Create(1);
					lpMat->GetTextureAnimation(0)->LoadTexture(0, *(Stream), Desc->m_FileName.c_str(), &Desc->m_TextureDesc);							
				}
				lpMat->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());		
				lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);				
				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).vCreatePass(1);
				RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateRasterizerState(RDesc);		

				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
				
				DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
				DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateDepthStencilState(DDesc);		

				BDesc.RTBlendParam[0].bBlend		= LYNX_TRUE;
				BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
				BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
				BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
				BDesc.RTBlendParam[0].RTWriteMask	= GraphicsSystem::CRenderer::ALL_CHANNELS;
				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateBlendState(BDesc);
				
				lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).vCreateShaderClasses("SpriteVS", "DiffuseMapXColorPS");						

				//lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);
				//lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE_LIGHTING) = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE);
			}
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);			
		}

		Math::CVector2 PositionFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetPositionFactor() : Math::CVector2(1.0f, 1.0f);		
		Math::CVector2 ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);
		if (!Desc->m_bApplyUIFactor)
		{
			PositionFactor = Math::CVector2(1.0f, 1.0f);		
			ScaleFactor = Math::CVector2(1.0f, 1.0f);
		}

		if (lpMat && (Desc->m_Width <= 0.0f || Desc->m_Height <= 0.0f) && lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetTextureAnimation(0)->GetTexture(0))
		{
			m_Width = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetTextureAnimation(0)->GetTexture(0)->vGetWidth() * ScaleFactor.x;
			m_Height = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetTextureAnimation(0)->GetTexture(0)->vGetHeight() * ScaleFactor.y;
		}
		else
		{
			m_Width = Desc->m_Width;
			m_Height = Desc->m_Height;
		}

		m_OriginalWidth = m_Width;
		m_OriginalHeight = m_Height;
				
		m_lpEngine->CreateResource(m_VertexArray, VERTEX_ARRAY);
		m_VertexArray->SetName(m_Name+CString(_T(".VertexArray")));
		
		CVector2 LeftTop, RightBottom;
		if (Desc->m_UVOffset[0].x >= 0.0f && Desc->m_UVOffset[0].y >= 0.0f)
		{
			LeftTop.x = Desc->m_UVOffset[0].x;
			LeftTop.y = Desc->m_UVOffset[0].y;
			RightBottom.x = Desc->m_UVOffset[1].x;
			RightBottom.y = Desc->m_UVOffset[1].y;
		}
		else
		{
			LeftTop.x = LeftTop.y = 0.0f;
			RightBottom.x = RightBottom.y = 1.0f;
		}
		m_lpSV = LYNXGLOBAL_NEW GraphicsSystem::SPRITEVERTEX[4];
		m_lpSV[0].x = -m_Width*0.5f+m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().x;	m_lpSV[0].y =  m_Height*0.5f-m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().y;	m_lpSV[0].z = 0;
		m_lpSV[0].TexCoord.u = LeftTop.x; m_lpSV[0].TexCoord.v = LeftTop.y; 
		m_lpSV[0].Color.r = 1.0f; m_lpSV[0].Color.g = 1.0f; m_lpSV[0].Color.b = 1.0f; m_lpSV[0].Color.a = 1.0f;
		m_UV[0].u = m_lpSV[0].TexCoord.u; m_UV[0].v = m_lpSV[0].TexCoord.v;

		m_lpSV[1].x =  m_Width*0.5f+m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().x;	m_lpSV[1].y =  m_Height*0.5f-m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().y;	m_lpSV[1].z = 1;
		m_lpSV[1].TexCoord.u = RightBottom.x; m_lpSV[1].TexCoord.v = LeftTop.y; 
		m_lpSV[1].Color.r = 1.0f; m_lpSV[1].Color.g = 1.0f; m_lpSV[1].Color.b = 1.0f; m_lpSV[1].Color.a = 1.0f;
		m_UV[1].u = m_lpSV[1].TexCoord.u; m_UV[1].v = m_lpSV[1].TexCoord.v;

		m_lpSV[2].x = -m_Width*0.5f+m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().x;	m_lpSV[2].y = -m_Height*0.5f-m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().y;	m_lpSV[2].z = 2;
		m_lpSV[2].TexCoord.u = LeftTop.x; m_lpSV[2].TexCoord.v = RightBottom.y; 
		m_lpSV[2].Color.r = 1.0f; m_lpSV[2].Color.g = 1.0f; m_lpSV[2].Color.b = 1.0f; m_lpSV[2].Color.a = 1.0f;
		m_UV[2].u = m_lpSV[2].TexCoord.u; m_UV[2].v = m_lpSV[2].TexCoord.v;

		m_lpSV[3].x =  m_Width*0.5f+m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().x;	m_lpSV[3].y = -m_Height*0.5f-m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().y;	m_lpSV[3].z = 3;
		m_lpSV[3].TexCoord.u = RightBottom.x; m_lpSV[3].TexCoord.v = RightBottom.y; 
		m_lpSV[3].Color.r = 1.0f; m_lpSV[3].Color.g = 1.0f; m_lpSV[3].Color.b = 1.0f; m_lpSV[3].Color.a = 1.0f;
		m_UV[3].u = m_lpSV[3].TexCoord.u; m_UV[3].v = m_lpSV[3].TexCoord.v;

		if (Desc->m_bInvertTexCoordV)
		{
            float Temp0 = m_lpSV[0].TexCoord.v;
            float Temp1 = m_lpSV[1].TexCoord.v;
			m_lpSV[0].TexCoord.v = m_lpSV[2].TexCoord.v;
			m_lpSV[1].TexCoord.v = m_lpSV[3].TexCoord.v;
			m_lpSV[2].TexCoord.v = Temp0;
			m_lpSV[3].TexCoord.v = Temp1;
		}
		m_VertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE)), 4, GraphicsSystem::CRenderer::DYNAMIC|GraphicsSystem::CRenderer::CPU_WRITE, Desc->m_bUseBufferObject, m_lpSV);				
		m_bUpdateVertex = LYNX_FALSE;

		Desc->m_Width *= ScaleFactor.x;
		Desc->m_Height *= ScaleFactor.y;
		float Len = Desc->m_AlignmentRect.x2 - Desc->m_AlignmentRect.x1;
		Desc->m_AlignmentRect.x1 *= PositionFactor.x;
		Desc->m_AlignmentRect.x2 = (Desc->m_AlignmentRect.x1 + Len*ScaleFactor.x);
		Len = Desc->m_AlignmentRect.y2 - Desc->m_AlignmentRect.y1;
		Desc->m_AlignmentRect.y1 *= PositionFactor.y;
		Desc->m_AlignmentRect.y2 = (Desc->m_AlignmentRect.y1 + Len*ScaleFactor.y);

		SetAlignmentType(Desc->m_AlignmentType);
		SetAlignmentRect(&Desc->m_AlignmentRect);
		SetOriginType(Desc->m_OriginType);
		
		m_Scale.x = m_Scale.y = m_Scale.z = 1.0f;

		CContainer::vCreate(desc);

		return LYNX_TRUE;
	}
	////-------------------------------------------------------------------------------------------------------
	///**
	//*	@brief 。
	//*
	//*	@param 。
	//*	@param 。  
	//*	@return 。 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CSpriteContainer::vCreate(const LYNXCHAR *name, int group_size, LPLYNXFILE lpf, long offset, LYNXBOOL bbo)
	//{
	//	MaterialSystem::CMaterial* lpMat = ((CEngine *)m_lpEngine)->GetlpMaterialSystem()->vCreateMaterial(MaterialSystem::CMaterial::STANDARD);	
	//	lpMat->vCreateTechnique(1);
	//	lpMat->SetTechnique(MaterialSystem::CMaterial::SIMPLE, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).vCreatePass(1);
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).SetPass(0, LYNXNEW MaterialSystem::CPass(&(lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0))));
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).vSetNumTextureAnimation(1);
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->AddressMode = GraphicsSystem::CRenderer::AM_WRAP;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->CombineFunc = GraphicsSystem::CRenderer::CF_MODULATE;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->CombineArg[0] = LYNX_CA_CURRENT;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->CombineArg[1] = LYNX_CA_TEXTURE;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->CombineArg[2] = LYNX_CA_TEXTURE;		
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->Create(1);
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->LoadTexture(0, 0, name, lpf, offset);		
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_DepthBufferMode = LYNX_GraphicsSystem::CRenderer::DB_NONE;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_CullMode = LYNX_GraphicsSystem::CRenderer::CULL_CCW;

	//	lpMat->SetVertexLayout(GraphicsSystem::gSpriteVertexLayout);
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).vLoadShaders(_T("builtin/scn/sprite.vso"), _T("builtin/common/lynxDiffuseMap.pso"));

	//	SetlpCurrentMaterial(lpMat);
	//	SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);

	//	lynxCreateVertexArray(&m_VertexArray, &GraphicsSystem::gSpriteVertexLayout, 4, LYNX_BOF_STATIC|LYNX_BOF_WRITEONLY, bbo);
	//	lynxLockVertexArray(&m_VertexArray, 0, 0, LYNX_LOCK_TO_WRITE_DISCARD_OLD);

	//		m_lpSV = (GraphicsSystem::SPRITEVERTEX*)m_VertexArray.lpBuffer;
	//		m_Width = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0).GetWidth();
	//		m_Height = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0).GetHeight();
	//		CReal W = m_Width*0.5f;
	//		CReal H = m_Height*0.5f;
	//		m_lpSV[0].x = -W;	m_lpSV[0].y = -H;	m_lpSV[0].TexCoord.u = 0.0f; m_lpSV[0].TexCoord.v = 0.0f; m_lpSV[0].Color.PackedColor = 0xffffffff;
	//		m_lpSV[1].x = W;	m_lpSV[1].y = -H;	m_lpSV[1].TexCoord.u = 1.0f; m_lpSV[1].TexCoord.v = 0.0f; m_lpSV[1].Color.PackedColor = 0xffffffff;
	//		m_lpSV[2].x = -W;	m_lpSV[2].y = H;	m_lpSV[2].TexCoord.u = 0.0f; m_lpSV[2].TexCoord.v = 1.0f; m_lpSV[2].Color.PackedColor = 0xffffffff;
	//		m_lpSV[3].x = W;	m_lpSV[3].y = H;	m_lpSV[3].TexCoord.u = 1.0f; m_lpSV[3].TexCoord.v = 1.0f; m_lpSV[3].Color.PackedColor = 0xffffffff;		

	//	lynxUnlockVertexArray(&m_VertexArray);			

	//	return LYNX_TRUE;
	//}			
	////-------------------------------------------------------------------------------------------------------
	///**
	//*	@brief 。
	//*
	//*	@param 。
	//*	@param 。  
	//*	@return 。 
	//*/
	////-------------------------------------------------------------------------------------------------------
	//LYNXBOOL CSpriteContainer::vCreate(int num, int length, const LYNXCHAR *name, const LYNXCHAR *ext, int group_size, LPLYNXFILE lpf, long offset, LYNXBOOL bbo)
	//{
	//	MaterialSystem::CMaterial* lpMat = ((CEngine *)m_lpEngine)->GetlpMaterialSystem()->vCreateMaterial(MaterialSystem::CMaterial::STANDARD);
	//	lpMat->vCreateTechnique(1);
	//	lpMat->SetTechnique(MaterialSystem::CMaterial::SIMPLE, LYNXNEW MaterialSystem::CTechnique(lpMat));
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).vCreatePass(1);
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).SetPass(0, LYNXNEW MaterialSystem::CPass(&(lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0))));
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).vSetNumTextureAnimation(1);
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->AddressMode = GraphicsSystem::CRenderer::AM_WRAP;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->CombineFunc = GraphicsSystem::CRenderer::CF_MODULATE;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->CombineArg[0] = LYNX_CA_CURRENT;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->CombineArg[1] = LYNX_CA_TEXTURE;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->CombineArg[2] = LYNX_CA_TEXTURE;				
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->LoadTextureSeuence(num, length, 0, name, ext, lpf, offset);							
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->GetAnimation().SetCurrentFrame(0, LYNX_TRUE);										
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_DepthBufferMode = LYNX_GraphicsSystem::CRenderer::DB_NONE;
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_CullMode = LYNX_GraphicsSystem::CRenderer::CULL_CCW;

	//	lpMat->SetVertexLayout(GraphicsSystem::gSpriteVertexLayout);
	//	lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).vLoadShaders(_T("builtin/scn/sprite.vso"), _T("builtin/common/lynxDiffuseMap.pso"));

	//	SetlpCurrentMaterial(lpMat);
	//	SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);

	//	lynxCreateVertexArray(&m_VertexArray, &GraphicsSystem::gSpriteVertexLayout, 4, LYNX_BOF_STATIC|LYNX_BOF_WRITEONLY, bbo);
	//	lynxLockVertexArray(&m_VertexArray, 0, 0, LYNX_LOCK_TO_WRITE_DISCARD_OLD);

	//		m_lpSV = (GraphicsSystem::SPRITEVERTEX*)m_VertexArray.lpBuffer;
	//		m_Width = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0).GetWidth();
	//		m_Height = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0).GetHeight();
	//		CReal W = m_Width*0.5f;
	//		CReal H = m_Height*0.5f;
	//		m_lpSV[0].x = -W;	m_lpSV[0].y = -H;	m_lpSV[0].TexCoord.u = 0.0f; m_lpSV[0].TexCoord.v = 0.0f; m_lpSV[0].Color.PackedColor = 0xffffffff;
	//		m_lpSV[1].x = W;	m_lpSV[1].y = -H;	m_lpSV[1].TexCoord.u = 1.0f; m_lpSV[1].TexCoord.v = 0.0f; m_lpSV[1].Color.PackedColor = 0xffffffff;
	//		m_lpSV[2].x = -W;	m_lpSV[2].y = H;	m_lpSV[2].TexCoord.u = 0.0f; m_lpSV[2].TexCoord.v = 1.0f; m_lpSV[2].Color.PackedColor = 0xffffffff;
	//		m_lpSV[3].x = W;	m_lpSV[3].y = H;	m_lpSV[3].TexCoord.u = 1.0f; m_lpSV[3].TexCoord.v = 1.0f; m_lpSV[3].Color.PackedColor = 0xffffffff;		

	//	lynxUnlockVertexArray(&m_VertexArray);	
	//	
	//	return LYNX_TRUE;
	//}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSpriteContainer::vGetScreenPosition(Math::CVector2& pos, CCameraContainer* cam, int w, int h) const
	{
		pos.x = m_lpCenter->x;
		pos.y = m_lpCenter->y;
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
	LYNXBOOL CSpriteContainer::vPlay(CReal step)
	{		
		LYNXBOOL bStop = LYNX_FALSE;
		Animation::CAnimation* lpAni = m_AnimationSet->GetCurrentAnimation();
		if (!m_lpParentContainer && lpAni)
		{
			bStop = lpAni->Play(step); 
			lpAni->UpdateTransform();
			m_M = lpAni->m_M;
			
			lpAni->UpdateTextureUVTransform();
			m_TextureUVM = lpAni->m_TextureUVM;			
		}

		return bStop;
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
	void CSpriteContainer::vSetSize(const Math::CReal& w, const Math::CReal& h)
	{
		if (w == m_Width && h == m_Height)
			return;

		m_Scale.x = w / m_OriginalWidth;
		m_Scale.y = h / m_OriginalHeight;

		m_Width = w;
		m_Height = h;

		//m_LocalM.ReplaceScale(Scale);		
		//UpdateTransform();

		//m_M.ReplaceScale(m_Scale);		

		SetAlignmentType(m_AlignmentType);
		
		/*
		if (w == m_Width && h == m_Height)
			return;

		m_Width = w;
		m_Height = h;

		m_lpSV[0].x = -m_Width*0.5f+m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().x;	m_lpSV[0].y =  m_Height*0.5f-m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().y;	m_lpSV[0].z = 0;
		m_lpSV[1].x =  m_Width*0.5f+m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().x;	m_lpSV[1].y =  m_Height*0.5f-m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().y;	m_lpSV[1].z = 1;
		m_lpSV[2].x = -m_Width*0.5f+m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().x;	m_lpSV[2].y = -m_Height*0.5f-m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().y;	m_lpSV[2].z = 2;
		m_lpSV[3].x =  m_Width*0.5f+m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().x;	m_lpSV[3].y = -m_Height*0.5f-m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetPixel2TexelOffset().y;	m_lpSV[3].z = 3;

		m_bUpdateVertex = LYNX_TRUE;
			
		SetAlignmentType(m_AlignmentType);	
		*/
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
	void CSpriteContainer::vGetSize(Math::CReal& w, Math::CReal& h) const
	{		
		w = m_Width;
		h = m_Height;						
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
	void CSpriteContainer::Align(int width, int height)
	{	
		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		LYNXRECT Viewport;
		lpR->GetViewportRect(&Viewport);
		int VPWidth = Viewport.x2 - Viewport.x1;
		int VPHeight = Viewport.y2 - Viewport.y1;

		if (m_AlignmentType == 0)
		{
			switch (m_OriginType)
			{
				case OT_UPPER_LEFT:				
					m_AlignmentOffset.x = (width*0.5f);
					m_AlignmentOffset.y = (height*0.5f);
					break;

				case OT_UPPER_RIGHT:				
					m_AlignmentOffset.x = -(width*0.5f);
					m_AlignmentOffset.y = (height*0.5f);
					break;

				case OT_CENTER:					
					m_AlignmentOffset.x = 0.0f;
					m_AlignmentOffset.y = 0.0f;
					break;
			}
			m_M.m41 += m_AlignmentOffset.x;
			m_M.m42 += m_AlignmentOffset.y;
			m_RenderRect.x1 = m_M.m41 - (int)(width*0.5);;
			m_RenderRect.x2 = m_RenderRect.x1 + (int)width;
			m_RenderRect.y1 = m_M.m42 - (int)(height*0.5);;
			m_RenderRect.y2 = m_RenderRect.y1 + (int)height;
			m_M.m41 = m_M.m41 - (VPWidth*0.5f);
			m_M.m42 = (VPHeight*0.5f) - m_M.m42;
		}
		else
		{
			WORD Align = (m_AlignmentType&0x0f);
			if (Align & H_ALIGN_LEFT)
			{
				m_AlignmentOffset.x = m_AlignmentRect.x1;
			}
			else if (Align & H_ALIGN_CENTER)
			{
				m_AlignmentOffset.x = (((m_AlignmentRect.x2-m_AlignmentRect.x1))*0.5 + m_AlignmentRect.x1);
			}
			else if (Align & H_ALIGN_RIGHT)
			{
				m_AlignmentOffset.x = m_AlignmentRect.x2;
			}
			
			Align = (m_AlignmentType&0xf0);
			if (Align & V_ALIGN_TOP)
			{
				m_AlignmentOffset.y = m_AlignmentRect.y1;
			}
			else if (Align & V_ALIGN_CENTER)
			{
				m_AlignmentOffset.y = (((m_AlignmentRect.y2-m_AlignmentRect.y1))*0.5 + m_AlignmentRect.y1);
			}
			else if (Align & V_ALIGN_BOTTOM)
			{
				m_AlignmentOffset.y = m_AlignmentRect.y2;
			}							
			switch (m_OriginType)
			{
				case OT_UPPER_LEFT:				
					m_AlignmentOffset.x += (width*0.5f);
					m_AlignmentOffset.y += (height*0.5f);					
					break;

				case OT_UPPER_RIGHT:				
					m_AlignmentOffset.x -= (width*0.5f);
					m_AlignmentOffset.y += (height*0.5f);					
					break;

				case OT_CENTER:										
					break;
			}
			m_RenderRect.x1 = m_AlignmentOffset.x - (int)(width*0.5);
			m_RenderRect.x2 = m_RenderRect.x1 + (int)width;
			m_RenderRect.y1 = m_AlignmentOffset.y - (int)(height*0.5);
			m_RenderRect.y2 = m_RenderRect.y1 + (int)height;

			m_M.m41 = m_AlignmentOffset.x;
			m_M.m42 = m_AlignmentOffset.y;			
			m_M.m41 = m_M.m41 - (VPWidth*0.5f);
			m_M.m42 = (VPHeight*0.5f) - m_M.m42;						
		}
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
	void CSpriteContainer::Align()
	{	
		Math::CReal W, H;
		vGetSize(W, H);
		Align(W, H);	
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
	void CSpriteContainer::vRender(CCameraContainer* cam)
	{		
		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		
		/*if (m_bUpdateVertex)
		{
			GraphicsSystem::SPRITEVERTEX* lpV = (GraphicsSystem::SPRITEVERTEX*)m_VertexArray->vMap(0, sizeof(GraphicsSystem::SPRITEVERTEX)*4, GraphicsSystem::CRenderer::LOCK_TO_WRITE_DISCARD_OLD);
				lynxMemCpy(lpV, m_lpSV, sizeof(GraphicsSystem::SPRITEVERTEX)*4);
			m_VertexArray->vUnmap();
			m_bUpdateVertex = LYNX_FALSE;			
		}
		*/
				
		PushTransform();
		
		//m_M.ReplaceScale(m_Scale);		

		m_AlignmentOffset.x = m_AlignmentOffset.y = 0.0f;
		//Align();
		Align(m_Width, m_Height);		
		
		lpR->SetVertexArray(m_VertexArray, 0, 0);
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)				
		{
			Technique.GetPass(p).Set(this, cam);
			if (m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene())
				m_lpEngine->GetlpSceneSystem()->GetlpCurrentScene()->GetlpRenderer()->RenderModeFilter();
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawArrayBO(GraphicsSystem::CRenderer::TRIANGLE_STRIP, 2, 0);
		}	

		PopTransform();
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
	void CSpriteContainer::ComputeRenderRect()
	{		
		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		
		PushTransform();
		
		m_AlignmentOffset.x = m_AlignmentOffset.y = 0.0f;
		Align(m_Width, m_Height);				
		//Align();
		
		PopTransform();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CSpriteContainer::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
	{		
		RegisterSuperClassMethod(lps);

		REGISTER_SCRIPT_METHOD(lps, "SetOriginType",			LynxEngine::CSpriteContainer,	void,	SetOriginType,			(const int))		
		REGISTER_SCRIPT_METHOD(lps, "SetAlignmentType",			LynxEngine::CSpriteContainer,	void,	SetAlignmentType,		(const WORD))					
		
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param  tex_ratio : the X/Y ratio of texture.
	*	@param  res_ratio : the X/Y ratio of resolution.  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CSpriteCreationDesc::ComputeUVOffsetByXYRatio(float tex_ratio, float res_ratio)
	{
		if (tex_ratio == res_ratio)
		{
			m_UVOffset[0].x = 0.0f;
			m_UVOffset[0].y = 0.0f;
			m_UVOffset[1].x = 1.0f;
			m_UVOffset[1].y = 1.0f;
		}
		else if (tex_ratio < res_ratio) // 
		{
			m_UVOffset[0].x = 0.0f;
			m_UVOffset[1].x = 1.0f;
			float Tex = 1.0f / tex_ratio;
			float Res = 1.0f / res_ratio;
			float y0 = (Tex - Res) / 2.0f;

			m_UVOffset[0].y = y0 / Tex;
			m_UVOffset[1].y = (y0 + Res) / Tex;
		}
		else
		{
			m_UVOffset[0].y = 0.0f;
			m_UVOffset[1].y = 1.0f;
			float Tex = tex_ratio;
			float Res = res_ratio;
			float x0 = (Tex - Res) / 2.0f;

			m_UVOffset[0].x = x0 / Tex;
			m_UVOffset[1].x = (x0 + Res) / Tex;
		}
	}
}
