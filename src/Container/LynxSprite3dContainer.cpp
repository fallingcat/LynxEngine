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
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Container/LynxSprite3DContainer.h>
#include <Container/LynxCameraContainer.h>
#include <SceneSystem/Light/LynxLight.h>

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
	CSprite3DContainer::CSprite3DContainer(CEngine *lpengine)	
	: CContainer(lpengine)
	{
		m_ContainerType = SPRITE3D;		
		m_AlignmentFlag = ALIGNMENT_H_CENTER | ALIGNMENT_V_CENTER;
		m_LockedAxis = LYNX_X_AXIS | LYNX_Y_AXIS | LYNX_Z_AXIS;
		m_Width = m_Height = 1.0f;
		m_NumSegX = m_NumSegY = 1;
		
		IMPLEMENT_PARAM_FLOAT(m_Width, 0)
		IMPLEMENT_PARAM_FLOAT(m_Height, 0)
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
	CSprite3DContainer::~CSprite3DContainer(void)
	{		
	}			
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CSprite3DContainer& CSprite3DContainer::operator =(const CSprite3DContainer& rhs)
	{	
		return (*this);
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
	LYNXBOOL CSprite3DContainer::vCreate(const CCreationDesc* desc)
	{
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC	 	 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;
		MaterialSystem::CMaterialPtr lpMat;

		DDesc.Default();
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		LYNX_ASSERT(desc);

		MaterialSystem::CTechnique* Technique;
		CSprite3DCreationDesc* Desc = (CSprite3DCreationDesc*)desc;		

		m_NumLODs = 1;		
		((CEngine*)m_lpEngine)->CreateResource(m_VertexArray, VERTEX_ARRAY);		

		if (Desc->m_bCreateMaterial)
		{
			CString MaterialName;

			if (Desc->m_MaterialName.empty())
			{
				MaterialName = m_Name+CString(_T(".Material"));				
			}
			else
			{
				MaterialName = Desc->m_MaterialName;
			}			
			lpMat = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MaterialName);
			if ((MaterialSystem::CMaterial*)lpMat == NULL)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(lpMat, MaterialSystem::CMaterial::STANDARD);			
				lpMat->SetName(MaterialName);
				lpMat->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE3D));	
				lpMat->vSetNumTextureAnimation(1);				
				if (Desc->m_Number > 1)
				{
					lpMat->GetTextureAnimation(0)->LoadTextureSeuence(Desc->m_Number, Desc->m_Length, *(Desc->m_lpStream), Desc->m_FileName.c_str(), Desc->m_FileExt.c_str(), &Desc->m_TextureDesc);										
				}
				else
				{
					lpMat->GetTextureAnimation(0)->Create(1);
					lpMat->GetTextureAnimation(0)->LoadTexture(0, *(Desc->m_lpStream), Desc->m_FileName.c_str(), &Desc->m_TextureDesc);						
				}
				lpMat->GetTextureAnimation(0)->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());		

				lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
                Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SIMPLE);
                lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)MaterialSystem::CMaterial::SIMPLE).vCreatePass(1);
                Technique->GetPass(0).m_TextureAnimationArray = lpMat->GetTextureAnimationArray();
                DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
                DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;
                lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateDepthStencilState(DDesc);
                BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
                //BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_SRC_ALPHA;
                //BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
                BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
                BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
                BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
                BDesc.RTBlendParam[0].SrcBlend		= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
                BDesc.RTBlendParam[0].DestBlend		= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
                BDesc.RTBlendParam[0].BlendOp		= GraphicsSystem::CRenderer::BO_ADD;
                lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).CreateBlendState(BDesc);
                lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).vCreateShaderClasses("Sprite3DVS", "DiffuseMapXColorPS");
                
                #if defined(__DESKTOP__) && !defined (__MOBILE_SIMULATION__)
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::WIREFRAME);
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::SIMPLE_LIGHTING);				
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::AMBIENT_LIGHTING);				
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::POINT_LIGHTING);				
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::SPOT_LIGHTING);				
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::DIRECTIONAL_LIGHTING);				
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::ZPASS);				
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::SHADOWMAP);				
                    lpMat->vAddTechnique(MaterialSystem::CMaterial::GPASS);				

                    Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::ZPASS);
                    Technique->vCreatePass(1);									
                    Technique->GetPass(0).vCreateShaderClasses("Sprite3DZPassVS", "ZPass_Velocity_AlphaTestPS");								
                    DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
                    DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
                    Technique->GetPass(0).CreateDepthStencilState(DDesc);		
                    BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
                    BDesc.RTBlendParam[0].SrcBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ONE;
                    BDesc.RTBlendParam[0].DestBlend	= LynxEngine::GraphicsSystem::CRenderer::BF_ZERO;
                    BDesc.RTBlendParam[0].BlendOp	= LynxEngine::GraphicsSystem::CRenderer::BO_ADD;
                    Technique->GetPass(0).CreateBlendState(BDesc);
                    RDesc.FillMode = LynxEngine::GraphicsSystem::CRenderer::FM_SOLID;
                    RDesc.CullMode = LynxEngine::GraphicsSystem::CRenderer::CULL_NONE;
                    Technique->GetPass(0).CreateRasterizerState(RDesc);

                    Technique = &lpMat->GetTechnique(LynxEngine::MaterialSystem::CMaterial::SHADOWMAP);
                    Technique->vCreatePass(1);		
                    Technique->GetPass(0).vCreateShaderClasses("Sprite3DShadowMapVS", "ShadowMap_AlphaTestPS");				
                    DDesc.DepthMode = GraphicsSystem::CRenderer::DB_CW;
                    DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_LEQUAL;			
                    Technique->GetPass(0).CreateDepthStencilState(DDesc);									
                    BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
                    BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_ONE;
                    BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ZERO;
                    BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
                    Technique->GetPass(0).CreateBlendState(BDesc);				
                    RDesc.FillMode = GraphicsSystem::CRenderer::FM_SOLID;
                    RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
                    Technique->GetPass(0).CreateRasterizerState(RDesc);
                    Technique->GetPass(0).m_bUseFog = LYNX_FALSE;
                #elif defined (__MOBILE__) || defined (__MOBILE_SIMULATION__)
                #endif
			}
			SetlpCurrentMaterial(lpMat);
			SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);			
		}
		if (lpMat && (Desc->m_Width == 0.0f || Desc->m_Height == 0.0f))
		{
			m_Width = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetTextureAnimation(0)->GetTexture(0)->vGetWidth();
			m_Height = lpMat->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).GetTextureAnimation(0)->GetTexture(0)->vGetHeight();
		}
		else
		{
			m_Width = Desc->m_Width;
			m_Height = Desc->m_Height;
		}

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

        GraphicsSystem::SPRITE3DVERTEX* lpV = LYNXGLOBAL_NEW GraphicsSystem::SPRITE3DVERTEX[4];		
		lpV[0].Pos.x = -m_Width*0.5f;	lpV[0].Pos.y = m_Height*0.5f;	lpV[0].Pos.z = 0.0f;	lpV[0].Color.PackedColor = 0xffffffff;
		lpV[1].Pos.x =  m_Width*0.5f;	lpV[1].Pos.y = m_Height*0.5f;	lpV[1].Pos.z = 0.0f;	lpV[1].Color.PackedColor = 0xffffffff;
		lpV[2].Pos.x = -m_Width*0.5f;	lpV[2].Pos.y = -m_Height*0.5f;	lpV[2].Pos.z = 0.0f;	lpV[2].Color.PackedColor = 0xffffffff;
		lpV[3].Pos.x =  m_Width*0.5f;	lpV[3].Pos.y = -m_Height*0.5f;	lpV[3].Pos.z = 0.0f;	lpV[3].Color.PackedColor = 0xffffffff;					

		lpV[0].TexCoord.u = LeftTop.x;		lpV[0].TexCoord.v = LeftTop.y; 
		lpV[1].TexCoord.u = RightBottom.x;	lpV[1].TexCoord.v = LeftTop.y; 
		lpV[2].TexCoord.u = LeftTop.x;		lpV[2].TexCoord.v = RightBottom.y; 
		lpV[3].TexCoord.u = RightBottom.x;	lpV[3].TexCoord.v = RightBottom.y; 

		if (Desc->m_bInvertTexCoordV)
		{
			lpV[0].TexCoord.v = 1.0f;
			lpV[1].TexCoord.v = 1.0f;
			lpV[2].TexCoord.v = 0.0f;
			lpV[3].TexCoord.v = 0.0f;		
		}
        m_VertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE3D)), 4, GraphicsSystem::CRenderer::DYNAMIC|GraphicsSystem::CRenderer::CPU_WRITE, Desc->m_bUseBufferObject, lpV);	
		LYNXGLOBAL_DEL_ARRAY(lpV);
        
		CContainer::vCreate(desc);

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
	LYNXBOOL CSprite3DContainer::vPlay(CReal step)
	{		
		if (m_lpCurrentMaterial)
			m_lpCurrentMaterial->vPlayAll(step);

		return CContainer::vPlay(step);
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
	void CSprite3DContainer::vSetSize(const Math::CReal& w, const Math::CReal& h)
	{
		GraphicsSystem::SPRITE3DVERTEX* lpV;

		m_Width = w;
		m_Height = h;
		CReal W = m_Width*0.5f;
		CReal H = m_Height*0.5f;
		
		lpV = (GraphicsSystem::SPRITE3DVERTEX*)m_VertexArray->vMap(0, 0, GraphicsSystem::CRenderer::LOCK_TO_WRITE_DISCARD_OLD);						
			lpV[0].Pos.x = -W;	lpV[0].Pos.y =  H;	
			lpV[1].Pos.x =  W;	lpV[1].Pos.y =  H;	
			lpV[2].Pos.x = -W;	lpV[2].Pos.y = -H;	
			lpV[3].Pos.x =  W;	lpV[3].Pos.y = -H;	
		m_VertexArray->vUnmap();
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
	void CSprite3DContainer::SetColor(LPLYNXCOLORRGBA lpc)
	{	
		GraphicsSystem::SPRITE3DVERTEX* lpV;

		lpV = (GraphicsSystem::SPRITE3DVERTEX*)m_VertexArray->vMap(0, 0, GraphicsSystem::CRenderer::LOCK_TO_WRITE_DISCARD_OLD);						
			lpV[0].Color.PackedColor = lpc->PackedColor;
			lpV[1].Color.PackedColor = lpc->PackedColor;
			lpV[2].Color.PackedColor = lpc->PackedColor;
			lpV[3].Color.PackedColor = lpc->PackedColor;
		m_VertexArray->vUnmap();				
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
	void CSprite3DContainer::vRender(CCameraContainer* cam)
	{		
		CVector3 Dir, Pos;
		CMatrix4 CM, TM, VM, WVM, WVPM;
		
		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();

		VM = cam->GetViewMatrix();		
		CM = cam->GetTransformMatrix();
		CM.m41 = CM.m42 = CM.m43 = 0.0f;
		lynxMatrixXMatrix(&TM, &m_M, &CM);
		TM.m41 = m_M.m41;
		TM.m42 = m_M.m42;
		TM.m43 = m_M.m43;		
		lynxMatrixXMatrix(&WVM, &TM, &VM);	
		lynxMatrixXMatrix(&WVPM, &WVM, &cam->GetProjectionMatrix());					
				
		lpR->SetVertexArray(m_VertexArray, 0, 0);			
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)(MaterialSystem::CMaterial::TECHTYPE)m_CurrentMaterialTechnique);
		for (int p=0; p<Technique.GetNumPass(); ++p)				
		{
			Technique.GetPass(p).Set(this, cam);		
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedVertexShader()->vSetConstantM("gWorldViewProj", &WVPM);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedVertexShader()->vSetConstantM("gWorldView", &WVM);	
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedVertexShader()->vSetConstantM("gWorld", &TM);									
			switch (m_CurrentMaterialTechnique)
			{
				case MaterialSystem::CMaterial::VARIANCE_SHADOWMAP:
				case MaterialSystem::CMaterial::SHADOWMAP:
					//m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->GetPosition(&Pos);
					//lynxSetVertexShaderConstantTableArray(lynxGetCurrentVertexShader(), "gLightPos", &Pos.x, 3);																	
					//lynxSetVertexShaderConstantTable(lynxGetCurrentVertexShader(), "gDepthScale", m_lpCurrentMaterial->GetlpMaterialSystem()->GetlpCurrentLight()->m_DepthScale);																		
					break;

				case MaterialSystem::CMaterial::ZPASS:					
					break;

				case MaterialSystem::CMaterial::POINT_LIGHTING:					
					break;

				default:
				case MaterialSystem::CMaterial::SIMPLE:					
					break;
			}			
			//lynxDrawArray(LYNX_PRIMITIVE_TRIANGLE_STRIP, 2, 0);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawArrayBO(GraphicsSystem::CRenderer::TRIANGLE_STRIP, 2, 0);
		}
	}
}
