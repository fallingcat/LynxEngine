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
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMap::CMap(void)
		{				
			m_lpGraphicsSystem = NULL;	
			m_bSRGB = LYNX_FALSE;
			m_bDepthTexture = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMap::CMap(CGraphicsSystem *pgs)
		: CObj(pgs->GetlpEngine())
		{					
			m_lpGraphicsSystem = pgs;			
			m_bSRGB = LYNX_FALSE;
			m_bDepthTexture = LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CMap::~CMap(void)
		{
			if (m_Texture)
			{				
				//m_lpGraphicsSystem->GetlpEngine()->DestroyResource(m_Texture->GetName());
				//m_Texture->Release();
			}			
			for (CArray<CTexturePtr>::CIterator i=m_UtilTextures.begin(); i!=m_UtilTextures.end(); ++i)			
			{				
				//m_lpGraphicsSystem->GetlpEngine()->DestroyResource((*i)->GetName());
				//(*i)->Release();
			}
			m_UtilTextures.clear();
			if (m_lpDepthBuffer)
			{
				//m_lpGraphicsSystem->GetlpEngine()->DestroyResource(m_lpDepthBuffer->GetName());
				//m_lpDepthBuffer->Release();
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMap::CreateUtilityTexture(const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize, int shift)
		{
			LYNXCHAR Name[64];

			int W = w;
			int H = h;	
			m_UtilTextures.resize(utilsize);
			W >>= shift;
			H >>= shift;
            
			for (int i=0; i<m_UtilTextures.size(); ++i)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_UtilTextures[i], TEXTURE);
				lynxSprintf(Name, _T("%s_down_sampled_%d"), name, i);
				if (!m_UtilTextures[i]->vCreateRT(NULL, CRenderer::FORMAT_NULL, Name, W, H, 1, CRenderer::TT_2D_TEXTURE, format, 0))
					return LYNX_FALSE;
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMap::Create(CRenderer::FORMAT zformat, int mipmaplevel, const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize, int shift, LYNXBOOL bSRGB, LYNXBOOL blockable)
		{			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create Map...\r\n")));

			m_bSRGB = bSRGB;
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_Texture, TEXTURE);
			if (zformat == CRenderer::FORMAT_NULL)
			{
				if (!m_Texture->vCreateRT(NULL, zformat, name, w, h, mipmaplevel, GraphicsSystem::CRenderer::TT_2D_TEXTURE, format, 0, blockable))
					return LYNX_FALSE;
			}
			else
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpDepthBuffer, BUFFER);
				if (!m_Texture->vCreateRT(&m_lpDepthBuffer, zformat, name, w, h, mipmaplevel, GraphicsSystem::CRenderer::TT_2D_TEXTURE, format, 0, blockable))
					return LYNX_FALSE;
				m_lpDepthBuffer->SetName(CString(name)+CString(_T(".DepthBuffer")));
			}						
			m_Current = m_Texture;			
	
			if (utilsize)
			{
				if (!CreateUtilityTexture(name, w, h, format, utilsize, shift))
					return LYNX_FALSE;
			}

			CRenderer::SAMPLERSTATEDESC SamDesc;
			SamDesc.Default();
			SamDesc.AddressU = CRenderer::AM_CLAMP;
			SamDesc.AddressV = CRenderer::AM_CLAMP;
			SamDesc.AddressW = CRenderer::AM_CLAMP;
			SamDesc.Filter	 = CRenderer::FM_MIN_MAG_MIP_LINEAR;
			SamDesc.MinLOD	 = 0;
			SamDesc.MaxLOD	 = 0;
			m_Texture->CreateSamplerState(SamDesc);
			for (int i=0; i<m_UtilTextures.size(); i++)
			{
				m_UtilTextures[i]->CreateSamplerState(SamDesc);
			}

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create Map OK.\r\n")));

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::DownSampled()
		{
			m_lpGraphicsSystem->DownSampled(m_UtilTextures[0], m_Texture);			
			m_Current = m_UtilTextures[0];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::DownSampled(CTexturePtr& t)
		{
			m_lpGraphicsSystem->DownSampled(m_UtilTextures[0], t);			
			m_Current = m_UtilTextures[0];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::DownSampled(LPLYNXRECT rect)
		{
			m_lpGraphicsSystem->DownSampled(m_UtilTextures[0], rect, m_Texture);			
			m_Current = m_UtilTextures[0];
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::BoxBlur(int times, float offset)
		{
            if (m_Current == m_UtilTextures[0])
				m_Current = (m_lpGraphicsSystem->BoxBlur(m_UtilTextures[0], m_UtilTextures[1], times, offset));			
			else
				m_Current = (m_lpGraphicsSystem->BoxBlur(m_UtilTextures[1], m_UtilTextures[0], times, offset));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::MaxPass()
		{
			DownSampled();
			if (m_Current == m_UtilTextures[0])
				m_Current = m_lpGraphicsSystem->MaxPass(m_UtilTextures[0], m_UtilTextures[1]);			
			else
				m_Current = m_lpGraphicsSystem->MaxPass(m_UtilTextures[1], m_UtilTextures[0]);			
		}		
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void CMap::Gaussin3x3Blur(int times, float offset)
		//{			
		//	m_Current = (m_lpGraphicsSystem->Gaussin3Blur(times, m_UtilTextures[0], m_UtilTextures[1], offset));			
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void CMap::Gaussin4x4Blur(int times, float offset)
		//{			
		//	m_Current = (m_lpGraphicsSystem->Gaussin4Blur(times, m_UtilTextures[0], m_UtilTextures[1], offset));			
		//}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::GaussinBlur(float radius, float offset)
		{
            if (m_Current == m_Texture)
				m_Current = (m_lpGraphicsSystem->GaussinBlur(m_Texture, m_UtilTextures[0], radius, offset));			
			else if (m_Current == m_UtilTextures[0])
				m_Current = (m_lpGraphicsSystem->GaussinBlur(m_UtilTextures[0], m_UtilTextures[1], radius, offset));			
			else
				m_Current = (m_lpGraphicsSystem->GaussinBlur(m_UtilTextures[1], m_UtilTextures[0], radius, offset));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::BoxBlurEdgeCheck(int times, float sharpness, float offset)
		{	
			if (m_Current == m_UtilTextures[0])
				m_Current = (m_lpGraphicsSystem->BoxBlurEdgeCheck(m_UtilTextures[0], m_UtilTextures[1], times, sharpness, offset));			
			else
				m_Current = (m_lpGraphicsSystem->BoxBlurEdgeCheck(m_UtilTextures[1], m_UtilTextures[0], times, sharpness, offset));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::RadialBlur(const CVector2& center, const float dist, const float weight)
		{	
			if (m_Current == m_Texture)
				m_Current = m_lpGraphicsSystem->RadialBlur(m_Texture, m_UtilTextures[0], center, dist, weight);			
			else if (m_Current == m_UtilTextures[0])
				m_Current = m_lpGraphicsSystem->RadialBlur(m_UtilTextures[0], m_UtilTextures[1], center, dist, weight);			
			else
				m_Current = m_lpGraphicsSystem->RadialBlur(m_UtilTextures[1], m_UtilTextures[0], center, dist, weight);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::HaloRadialBlur(const CVector2& center, const float halo_radius, const float dist, const float weight)
		{	
			if (m_Current == m_Texture)
				m_Current = m_lpGraphicsSystem->HaloRadialBlur(m_Texture, m_UtilTextures[0], center, halo_radius, dist, weight);			
			else if (m_Current == m_UtilTextures[0])
				m_Current = m_lpGraphicsSystem->HaloRadialBlur(m_UtilTextures[0], m_UtilTextures[1], center, halo_radius, dist, weight);			
			else
				m_Current = m_lpGraphicsSystem->HaloRadialBlur(m_UtilTextures[1], m_UtilTextures[0], center, halo_radius, dist, weight);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::DirectionalBlur(const CVector2& dir, const float dist, const float weight)
		{
			if (m_Current == m_Texture)
				m_Current = m_lpGraphicsSystem->DirectionalBlur(m_Texture, m_UtilTextures[0], dir, dist, weight);
			else if (m_Current == m_UtilTextures[0])
				m_Current = m_lpGraphicsSystem->DirectionalBlur(m_UtilTextures[0], m_UtilTextures[1], dir, dist, weight);
			else
				m_Current = m_lpGraphicsSystem->DirectionalBlur(m_UtilTextures[1], m_UtilTextures[0], dir, dist, weight);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::VelocityMapBlur(CTexturePtr& vel, const float dist, const float weight)
		{
			if (m_Current == m_Texture)
				m_Current = m_lpGraphicsSystem->VelocityMapBlur(m_Texture, m_UtilTextures[0], vel, dist, weight);
			else if (m_Current == m_UtilTextures[0])
				m_Current = m_lpGraphicsSystem->VelocityMapBlur(m_UtilTextures[0], m_UtilTextures[1], vel, dist, weight);
			else
				m_Current = m_lpGraphicsSystem->VelocityMapBlur(m_UtilTextures[1], m_UtilTextures[0], vel, dist, weight);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CMap::BilateralBlur(float radius, float sharpness, float offset)
		{	
			if (m_Current == m_Texture)
				m_Current = (m_lpGraphicsSystem->BilateralBlur(m_Texture, m_UtilTextures[0], radius, sharpness, offset));			
			else if (m_Current == m_UtilTextures[0])
				m_Current = (m_lpGraphicsSystem->BilateralBlur(m_UtilTextures[0], m_UtilTextures[1], radius, sharpness, offset));			
			else
				m_Current = (m_lpGraphicsSystem->BilateralBlur(m_UtilTextures[1], m_UtilTextures[0], radius, sharpness, offset));			
		}
	}
}