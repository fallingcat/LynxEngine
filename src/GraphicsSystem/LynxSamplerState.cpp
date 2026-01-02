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
		void CSamplerState::Default(void)
		{				
			m_ResourceType = SAMPLER_STATE; 	

			m_Desc.Filter	= CRenderer::FM_MIN_MAG_MIP_LINEAR;
			m_Desc.AddressU	= CRenderer::AM_WRAP;
			m_Desc.AddressV	= CRenderer::AM_WRAP;
			m_Desc.AddressW	= CRenderer::AM_WRAP;

			m_Desc.ComparisonFunc = CRenderer::TF_ALWAYS;

			m_Desc.MaxAnisotropy = 0;
			m_Desc.MipLODBias = 0;
			m_Desc.MinLOD = 0;
			m_Desc.MaxLOD = 256;
			m_Desc.BorderColor.PackedColor = 0x0;
			
            m_Name.reserve(128);
            
			MakeName();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSamplerState::CSamplerState(CGraphicsSystem* pgs)
		: CResource(pgs->GetlpEngine())
		{
			m_lpGraphicsSystem = pgs;
			Default();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSamplerState::~CSamplerState(void)
		{		
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSamplerState::vCreate(const CRenderer::SAMPLERSTATEDESC& desc)
		{
			m_Desc.Filter			= desc.Filter;	
			m_Desc.AddressU			= desc.AddressU;
			m_Desc.AddressV			= desc.AddressV;
			m_Desc.AddressW			= desc.AddressW;
			m_Desc.MipLODBias		= desc.MipLODBias;
			m_Desc.MaxAnisotropy	= desc.MaxAnisotropy;
			m_Desc.ComparisonFunc	= desc.ComparisonFunc;
			m_Desc.BorderColor		= desc.BorderColor;
			m_Desc.MinLOD			= desc.MinLOD;
			m_Desc.MaxLOD			= desc.MaxLOD;	
			MakeName();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString CSamplerState::MakeName(const CRenderer::SAMPLERSTATEDESC& desc)
		{
			LYNXCHAR MipLODBias[32], MaxAS[16], Border[16], MinLOD[32], MaxLOD[32];			

			lynxSprintf(MipLODBias, _T("%f"),	desc.MipLODBias);
			lynxSprintf(MaxAS,		_T("%d"),	desc.MaxAnisotropy);
			lynxSprintf(Border,		_T("%d"),	desc.BorderColor.PackedColor);			
			lynxSprintf(MinLOD,		_T("%f"),	desc.MinLOD);
			lynxSprintf(MaxLOD,		_T("%f"),	desc.MaxLOD);

            CString Name;
            
            Name.reserve(128);
			Name = (
					LynxEngine::GetEnumStr(desc.Filter)+CString(".") +
					LynxEngine::GetEnumStr(desc.AddressU)+CString(".") +
					LynxEngine::GetEnumStr(desc.AddressV)+CString(".") +
					LynxEngine::GetEnumStr(desc.AddressW)+CString(".") +
                    CString(MipLODBias)+CString(".") +
					CString(MaxAS)+CString(".") +
					LynxEngine::GetEnumStr(desc.ComparisonFunc)+CString(".") +
					CString(Border)+CString(".") +
					CString(MinLOD)+CString(".") +
					CString(MaxLOD) );

            LYNXCRC CRCCode = lynxComputeCRCFast((const unsigned char*)Name.c_str(), Name.size());
			LYNXCHAR CRCStr[64];
			lynxSprintf(CRCStr, _T("%x"), CRCCode);

			return CString(CRCStr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CSamplerState::MakeName()
		{
			SetName(MakeName(m_Desc));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSamplerState::vSave(CStream& s)
		{
			s.vWrite(&m_Desc.Filter,		sizeof(CRenderer::FILTERMODE), 1);	
			s.vWrite(&m_Desc.AddressU,		sizeof(CRenderer::ADDRESSMODE), 1);	
			s.vWrite(&m_Desc.AddressV,		sizeof(CRenderer::ADDRESSMODE), 1);	
			s.vWrite(&m_Desc.AddressW,		sizeof(CRenderer::ADDRESSMODE), 1);	
			s.vWrite(&m_Desc.MipLODBias,	sizeof(float), 1);	
			s.vWrite(&m_Desc.MaxAnisotropy,	sizeof(int), 1);	
			s.vWrite(&m_Desc.ComparisonFunc,sizeof(CRenderer::TESTFUNC), 1);	
			s.vWrite(&m_Desc.BorderColor,	sizeof(LYNXCOLORRGBA), 1);	
			s.vWrite(&m_Desc.MinLOD,		sizeof(float), 1);	
			s.vWrite(&m_Desc.MaxLOD,		sizeof(float), 1);				
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSamplerState::LoadDesc(CRenderer::SAMPLERSTATEDESC& desc, CStream& s, LYNXBOOL skip)
		{
			if (skip)
			{
				LYNXLONG SeekLen = sizeof(CRenderer::FILTERMODE) +
								   sizeof(CRenderer::ADDRESSMODE) +
								   sizeof(CRenderer::ADDRESSMODE) +
								   sizeof(CRenderer::ADDRESSMODE) +
								   sizeof(float) +
								   sizeof(int) +
								   sizeof(CRenderer::TESTFUNC) +
								   sizeof(LYNXCOLORRGBA) +
								   sizeof(float) +
								   sizeof(float);
				s.vSeek(SeekLen, CStream::SeekCur);	
			}
			else
			{
				s.vRead(&desc.Filter,			sizeof(CRenderer::FILTERMODE), 1);	
				s.vRead(&desc.AddressU,			sizeof(CRenderer::ADDRESSMODE), 1);	
				s.vRead(&desc.AddressV,			sizeof(CRenderer::ADDRESSMODE), 1);	
				s.vRead(&desc.AddressW,			sizeof(CRenderer::ADDRESSMODE), 1);	
				s.vRead(&desc.MipLODBias,		sizeof(float), 1);	
				s.vRead(&desc.MaxAnisotropy,	sizeof(int), 1);	
				s.vRead(&desc.ComparisonFunc,	sizeof(CRenderer::TESTFUNC), 1);	
				s.vRead(&desc.BorderColor,		sizeof(LYNXCOLORRGBA), 1);	
				s.vRead(&desc.MinLOD,			sizeof(float), 1);	
				s.vRead(&desc.MaxLOD,			sizeof(float), 1);				
			}		

			return LYNX_TRUE;
		}
	}
}