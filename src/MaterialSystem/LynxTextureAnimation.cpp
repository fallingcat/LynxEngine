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
//  Created by Owen Wu : 2006/11/20
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxCreationDesc.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <MaterialSystem/LynxTextureAnimation.h>
#include <Animation/LynxAnimationKeys.h>

namespace LynxEngine 
{	
	namespace MaterialSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTextureAnimation::CTextureAnimation(GraphicsSystem::CGraphicsSystem* gs)
		: CResource(gs->GetlpEngine())
		{
			m_ResourceType = TEXTURE_ANIMATION;
			m_lpGraphicsSystem = gs;
			m_lpAnimation = NULL;
			m_CurrentTexture = 0;
			/*m_AddressModeU = GraphicsSystem::CRenderer::AM_WRAP;
			m_AddressModeV = GraphicsSystem::CRenderer::AM_WRAP;
			m_AddressModeW = GraphicsSystem::CRenderer::AM_WRAP;
			m_MinFilter = GraphicsSystem::CRenderer::TF_LINEAR;
			m_MagFilter = GraphicsSystem::CRenderer::TF_LINEAR;
			m_MipFilter = GraphicsSystem::CRenderer::TF_LINEAR;
			m_LODBias = 0;
			m_MipMapLevel = 0;
			m_CombineFunc = GraphicsSystem::CRenderer::CF_MODULATE;
			m_CombineArg[0] = GraphicsSystem::CRenderer::CA_CURRENT;
			m_CombineArg[1] = GraphicsSystem::CRenderer::CA_TEXTURE;*/

			m_TextureArray.clear();
			m_ShaderConstantName.clear();

			m_bUpdateAnimationParam = LYNX_FALSE;
			m_TexAnimationParam.Color[0] = 1.0f;
			m_TexAnimationParam.Color[1] = 1.0f;
			m_TexAnimationParam.Color[2] = 1.0f;
			m_TexAnimationParam.Color[3] = 1.0f;
			m_TexAnimationParam.UVMatrix.Identity();

			/*IMPLEMENT_PARAM_ENUM(m_AddressModeU, 0);
			IMPLEMENT_PARAM_ENUM(m_AddressModeV, 0);
			IMPLEMENT_PARAM_ENUM(m_AddressModeW, 0);*/

			//IMPLEMENT_PARAM_ENUM(m_CombineFunc, LYNXCOMBINEFUNC, 0);

			
			//IMPLEMENT_PARAM_ENUM(m_MinFilter, 0);
			//IMPLEMENT_PARAM_ENUM(m_MagFilter, 0);
			//IMPLEMENT_PARAM_ENUM(m_MipFilter, 0);

			IMPLEMENT_PARAM_ANSISTRING_COMMENT(m_ShaderConstantName, _T("Texture constant name in shader."), 0)
			IMPLEMENT_PARAM_ARRAY(m_TextureArray, (CParam::COUNTEDPTR), 0)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTextureAnimation::~CTextureAnimation(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));

			LYNXDEL m_lpAnimation;
			m_lpAnimation = NULL;
			
			m_TextureArray.clear();
			m_ShaderConstantName.clear();
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTextureAnimation& CTextureAnimation::operator =(const CTextureAnimation& t)
		{		
			/*m_AddressModeU = t.m_AddressModeU;
			m_AddressModeV = t.m_AddressModeV;
			m_AddressModeW = t.m_AddressModeW;
			m_MinFilter = t.m_MinFilter;
			m_MagFilter = t.m_MagFilter;
			m_MipFilter = t.m_MipFilter;
			m_MipMapLevel = t.m_MipMapLevel;
			m_CurrentTexture = t.m_CurrentTexture;
			m_TextureNameArray = t.m_TextureNameArray;
			m_TextureArray = t.m_TextureArray;		
			m_lpAnimation = t.m_lpAnimation;
			m_CombineFunc = t.m_CombineFunc;
			m_CombineArg[0] = t.m_CombineArg[0];
			m_CombineArg[1] = t.m_CombineArg[1];
			m_CombineArg[2] = t.m_CombineArg[2];*/

			m_CurrentTexture = t.m_CurrentTexture;
			m_TextureArray = t.m_TextureArray;
			if (m_lpAnimation)
				LYNXDEL m_lpAnimation;
			m_lpAnimation = t.m_lpAnimation;
			m_bUpdateAnimationParam = t.m_bUpdateAnimationParam;

			return (*this);
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTextureAnimation::Create(int num) 
		{
			m_TextureArray.resize(num); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTextureAnimation::AddTexture(CStream& fs, const LYNXCHAR *filename, GraphicsSystem::CRenderer::TEXTUREDESC* desc)
		{
			LynxEngine::GraphicsSystem::CTexturePtr Texture;
			m_lpGraphicsSystem->GetlpRenderer()->LoadTexture(Texture, fs, filename, desc);
			m_TextureArray.push_back(Texture);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		Animation::CAnimation* CTextureAnimation::vCreateAnimation() 
		{
			if (!m_lpAnimation)
			{				
				m_lpAnimation = LYNXNEW Animation::CAnimation(m_lpGraphicsSystem->GetlpEngine());
				m_lpAnimation->SetName(m_Name + CString(_T(".Animation")));
			}

			return m_lpAnimation;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTextureAnimation::LoadTexture(int i, CStream& fs, const LYNXCHAR *filename, GraphicsSystem::CRenderer::TEXTUREDESC* desc)
		{
			return m_lpGraphicsSystem->GetlpRenderer()->LoadTexture(m_TextureArray[i], fs, filename, desc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTextureAnimation::LoadTextureSeuence(int num, int length, CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ext, GraphicsSystem::CRenderer::TEXTUREDESC* desc)
		{
			LYNXCHAR		Path[256];
			LYNXREAL		Value;

			Create(num);		

			for (int i=0; i<num; i++)
			{
				lynxSprintf(Path, _T("%s%02d.%s"), filename, i, ext);
				if (!m_lpGraphicsSystem->GetlpRenderer()->LoadTexture(m_TextureArray[i], fs, Path, desc))
					return LYNX_FALSE;

				if (desc)
				{
					m_TextureArray[i]->m_AlphaReference = desc->AlphaReference;
					if (desc->MipMapLevel == 1)
					{
						desc->SamplerDesc.MinLOD = 0;
						desc->SamplerDesc.MaxLOD = 0;
					}
					m_TextureArray[i]->CreateSamplerState(desc->SamplerDesc);
				}
				else
				{
					m_TextureArray[i]->m_AlphaReference = 0.0f;
				}
			}

			vCreateAnimation();
			if (m_lpAnimation)
			{
				m_lpAnimation->CreateKeys(Animation::CAnimation::KT_TEXTURE, 2, 1);
				m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[0].SetStartTime(0);
				m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[0].SetEndTime(length);
				Value = 0.0f;
				m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[0].SetValue(0, Value);	

				m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[1].SetStartTime(length);
				m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[1].SetEndTime(length+1);
				Value = num-1;
				m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys[1].SetValue(0, Value);		

				m_lpAnimation->SetRange(0.0f, length);
				m_lpAnimation->SetStep(1.0f);
				m_lpAnimation->SetPlayMode(PM_LOOP);
			}
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
		LYNXBOOL CTextureAnimation::LoadAnimation(const CCreationDesc* desc) 
		{ 		
			char				Mark[] = "LYNXANI";	
			char				Check[8], Name[64];	
			DWORD				MainVer, Num;				
			LYNXFILE			MS, *lpFile;	
			LYNXREAL			KeyValues[4];
			int					MaxTime;

			LYNX_ASSERT(desc->m_lpStream);

			vCreateAnimation();
			if (!m_lpAnimation)
				return LYNX_FALSE;

			if (!desc->m_lpStream->IsValid())
			{
				if (!desc->m_lpStream->vOpen(desc->m_FileName.c_str()))
					return LYNX_FALSE;
			}
				
			desc->m_lpStream->vRead(&Check, 7, 1);
			Check[7] = 0x00;
			if (strcmp(Mark, Check))	
			{
				desc->m_lpStream->vClose();		
				return LYNX_FALSE;
			}
			desc->m_lpStream->vRead(&MainVer, sizeof(DWORD), 1);    				

			desc->m_lpStream->vRead(&Num, sizeof(DWORD), 1);			
			if (!Num)
			{
				desc->m_lpStream->vClose();
				return LYNX_FALSE;
			}
			MaxTime = m_lpAnimation->Load(*(desc->m_lpStream));			
			m_lpAnimation->SetRange(0.0f, MaxTime-1);				
			m_lpAnimation->SetStep(1.0f);				
			m_lpAnimation->SetPlayMode(PM_LOOP);						

			desc->m_lpStream->vClose();

			return LYNX_TRUE;						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTextureAnimation::vSet(int channel)
		{
			if (!m_TextureArray.size())
				return;

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			if (m_ShaderConstantName.empty())
			{
				lpR->SetTexture(channel, GetTexture());
				
				// Uplaod textexture animation parameters to shader
				char Name[128];
				sprintf(Name, "gbTextureAnimation[%d]", channel);
				lpR->GetCachedPixelShader()->vSetConstantB(Name, m_bUpdateAnimationParam);
				if (m_bUpdateAnimationParam)
				{
					sprintf(Name, "gTextureAnimation[%d].Color", channel);
					lpR->GetCachedPixelShader()->vSetConstantF(Name, m_TexAnimationParam.Color, 4);
					sprintf(Name, "gTextureAnimation[%d].UVMatrix", channel);
					lpR->GetCachedPixelShader()->vSetConstantF(Name, &m_TexAnimationParam.UVMatrix.m[0][0], 9);
				}
			}
			else
			{
				lpR->GetCachedPixelShader()->vSetTexture(m_ShaderConstantName.c_str(), GetTexture());
				
				// Uplaod textexture animation parameters to shader
				char Name[128];
				int Channel = lpR->GetCachedPixelShader()->vGetTextureIndex(m_ShaderConstantName.c_str());
				sprintf(Name, "gbTextureAnimation[%d]", Channel);
				lpR->GetCachedPixelShader()->vSetConstantB(Name, m_bUpdateAnimationParam);
				if (m_bUpdateAnimationParam)
				{
					sprintf(Name, "gTextureAnimation[%d].Color", Channel);
					lpR->GetCachedPixelShader()->vSetConstantF(Name, m_TexAnimationParam.Color, 4);
					sprintf(Name, "gTextureAnimation[%d].UVMatrix", Channel);
					lpR->GetCachedPixelShader()->vSetConstantF(Name, &m_TexAnimationParam.UVMatrix.m[0][0], 9);
				}
			}

			

			//else
			//{
			//	lynxSetTextureAddressMode(channel, LYNX_U_COORD, (LYNXTEXTUREADDRESSMODE)m_AddressModeU);
			//	lynxSetTextureAddressMode(channel, LYNX_V_COORD, (LYNXTEXTUREADDRESSMODE)m_AddressModeV);
			//	lynxSetTextureAddressMode(channel, LYNX_W_COORD, (LYNXTEXTUREADDRESSMODE)m_AddressModeW);
			//	//lynxSetTextureFilterMode(channel, m_MinFilter,  m_MagFilter,  m_MipFilter); 
			//	lynxSetTextureFilterMode(channel, LYNX_TF_LINEAR,  LYNX_TF_LINEAR,  LYNX_TF_POINT); 
			//	lynxSetTextureBlend(channel, GraphicsSystem::CD3D9Renderer::TranslateCombineFunc(m_CombineFunc), 
			//								 GraphicsSystem::CD3D9Renderer::TranslateCombineArg(m_CombineArg[0]), 
			//								 GraphicsSystem::CD3D9Renderer::TranslateCombineArg(m_CombineArg[1]), 
			//								 LYNX_CA_TEXTURE);													
			//}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTextureAnimation::vSet(GraphicsSystem::CShader* lps, int channel)
		{
			if (!m_TextureArray.size())
				return;

			GraphicsSystem::CRenderer* lpR = m_lpGraphicsSystem->GetlpRenderer();
			if (m_ShaderConstantName.empty())
			{
				lpR->SetTexture(channel, GetTexture());
				
                #if defined (__DESKTOP__) && !defined(__OGLES2__)
                    // Uplaod textexture animation parameters to shader
                    char Name[128];
                    sprintf(Name, "gbTextureAnimation[%d]", channel);
                    lps->vSetConstantB(Name, m_bUpdateAnimationParam);
                    if (m_bUpdateAnimationParam)
                    {
                        sprintf(Name, "gTextureAnimation[%d].Color", channel);
                        lps->vSetConstantF(Name, m_TexAnimationParam.Color, 4);
                        sprintf(Name, "gTextureAnimation[%d].UVMatrix", channel);
                        lps->vSetConstantF(Name, &m_TexAnimationParam.UVMatrix.m[0][0], 9);
                    }
                #endif
			}
			else
			{
				lps->vSetTexture(m_ShaderConstantName.c_str(), GetTexture());
				
                #if defined (__DESKTOP__) && !defined(__OGLES2__)
                    // Uplaod textexture animation parameters to shader
                    char Name[128];
                    int Channel = lps->vGetTextureIndex(m_ShaderConstantName.c_str());
                    if (Channel >= 0)
                    {
                        sprintf(Name, "gbTextureAnimation[%d]", Channel);
                        lps->vSetConstantB(Name, m_bUpdateAnimationParam);
                        if (m_bUpdateAnimationParam)
                        {
                            sprintf(Name, "gTextureAnimation[%d].Color", Channel);
                            lps->vSetConstantF(Name, m_TexAnimationParam.Color, 4);
                            sprintf(Name, "gTextureAnimation[%d].UVMatrix", Channel);
                            lps->vSetConstantF(Name, &m_TexAnimationParam.UVMatrix.m[0][0], 9);
                        }
                    }
                #endif
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTextureAnimation::Play(LYNXREAL step)
		{
			LYNXBOOL bRet = LYNX_TRUE;

			if (m_lpAnimation)
			{
				static Math::CReal Values[4];

				bRet = m_lpAnimation->Play(step);
				
				if (m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE] && m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->m_Keys.size())
				{
					m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_TEXTURE]->Lerp(Values, m_lpAnimation->m_Time);
					m_CurrentTexture = Values[0].Round();
				}

				m_bUpdateAnimationParam = LYNX_FALSE;
				if (m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_COLOR] && m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->m_Keys.size())
				{
					m_bUpdateAnimationParam = LYNX_TRUE;
					m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_COLOR]->Lerp(Values, m_lpAnimation->m_Time);
					m_TexAnimationParam.Color[0] = Values[0];
					m_TexAnimationParam.Color[1] = Values[1];
					m_TexAnimationParam.Color[2] = Values[2];
					m_TexAnimationParam.Color[3] = Values[3];
				}			

				m_TexAnimationParam.UVMatrix.Identity();
				
				if (m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_ROTATION] && m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->m_Keys.size())
				{
					m_bUpdateAnimationParam = LYNX_TRUE;
					m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_ROTATION]->Lerp(Values, m_lpAnimation->m_Time);
					MakeRotateMatrix3(m_TexAnimationParam.UVMatrix, Values[0]);					
				}			
				if (m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_SCALE] && m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->m_Keys.size())
				{
					m_bUpdateAnimationParam = LYNX_TRUE;
					m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_SCALE]->Lerp(Values, m_lpAnimation->m_Time);

					m_TexAnimationParam.UVMatrix.m11 *= Values[0];
					m_TexAnimationParam.UVMatrix.m21 *= Values[1];					

					m_TexAnimationParam.UVMatrix.m12 *= Values[0];
					m_TexAnimationParam.UVMatrix.m22 *= Values[1];
				}			
				if (m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_POSITION] && m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->m_Keys.size())
				{
					m_bUpdateAnimationParam = LYNX_TRUE;
					m_lpAnimation->m_AnimationKeys[Animation::CAnimation::KT_POSITION]->Lerp(Values, m_lpAnimation->m_Time);

					m_TexAnimationParam.UVMatrix.m31 += Values[0];
					m_TexAnimationParam.UVMatrix.m32 += Values[1];
				}				
				///*if (m_VisibilityKeys.m_Keys.size())
				//{
				//	m_VisibilityKeys.Lerp(Values, m_Time);
				//	m_Visibility = lpValues[0];
				//}*/

			}			
			return bRet;
		}
	}
}