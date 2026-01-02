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
#include <GraphicsSystem/LynxBlendState.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBlendState::Default(void)
		{				
			m_ResourceType = BLEND_STATE; 	

			m_Desc.bAlphaToCoverage	= LYNX_FALSE;
			m_Desc.bIndependentBlend = LYNX_FALSE;
			for (int i=0; i<8; i++)
			{
				m_Desc.RTBlendParam[i].bBlend	= LYNX_TRUE;
				m_Desc.RTBlendParam[i].SrcBlend	= CRenderer::BF_ONE;
				m_Desc.RTBlendParam[i].DestBlend	= CRenderer::BF_ZERO;
				m_Desc.RTBlendParam[i].BlendOp	= CRenderer::BO_ADD;
				m_Desc.RTBlendParam[i].RTWriteMask = CRenderer::RED_CHANNEL|CRenderer::GREEN_CHANNEL|CRenderer::BLUE_CHANNEL|CRenderer::ALPHA_CHANNEL;
			}
            m_Name.reserve(128);

			MakeName();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBlendState::CBlendState(CGraphicsSystem* pgs)
		: CResource(pgs->GetlpEngine())
		{
			m_lpGraphicsSystem = pgs;
			Default();

			SET_PARAM_FLAG(m_Name, CParam::PARAM_READONLY);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBlendState::~CBlendState(void)
		{			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBlendState::vCreate(const CRenderer::BLENDSTATEDESC& desc)
		{
			m_Desc.bAlphaToCoverage = desc.bAlphaToCoverage;
			m_Desc.bIndependentBlend = desc.bIndependentBlend;
			m_Desc.RTBlendParam[0] = desc.RTBlendParam[0];
			m_Desc.RTBlendParam[1] = desc.RTBlendParam[1];
			m_Desc.RTBlendParam[2] = desc.RTBlendParam[2];
			m_Desc.RTBlendParam[3] = desc.RTBlendParam[3];
			m_Desc.RTBlendParam[4] = desc.RTBlendParam[4];
			m_Desc.RTBlendParam[5] = desc.RTBlendParam[5];
			m_Desc.RTBlendParam[6] = desc.RTBlendParam[6];
			m_Desc.RTBlendParam[7] = desc.RTBlendParam[7];
			MakeName();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString CBlendState::MakeName(const CRenderer::BLENDSTATEDESC& desc)
		{
			LYNXCHAR A2C[8], IBlend[8], RTBlend[8][8], RTBlendMask[8][8];

			lynxItoA(desc.bAlphaToCoverage,			A2C,			10);
			lynxItoA(desc.bIndependentBlend,		IBlend,			10);
			for (int i=0; i<8; i++)
			{
				lynxItoA(desc.RTBlendParam[i].bBlend,       RTBlend[i],		10);
				lynxItoA(desc.RTBlendParam[i].RTWriteMask,	RTBlendMask[i],	10);
			}
            
            CString Name;
            
            Name.reserve(512);

			Name = CString(A2C)+CString(".") +
				   CString(IBlend)+CString(".");

			for (int i=0; i<8; i++)
			{
				Name += CString(RTBlend[i])+CString(".");
				Name += LynxEngine::GetEnumStr(desc.RTBlendParam[i].SrcBlend)+CString(".");
				Name += LynxEngine::GetEnumStr(desc.RTBlendParam[i].DestBlend)+CString(".");
				Name += LynxEngine::GetEnumStr(desc.RTBlendParam[i].BlendOp)+CString(".");
				Name += LynxEngine::GetEnumStr(desc.RTBlendParam[i].SrcBlendAlpha)+CString(".");
				Name += LynxEngine::GetEnumStr(desc.RTBlendParam[i].DestBlendAlpha)+CString(".");
				Name += LynxEngine::GetEnumStr(desc.RTBlendParam[i].BlendOpAlpha)+CString(".");
				if (i==7)
					Name += CString(RTBlendMask[i]);
				else
					Name += CString(RTBlendMask[i])+CString(".");
			}			
			LYNXCRC CRCCode = lynxComputeCRCFast((const unsigned char*)Name.c_str(), Name.size());
			LYNXCHAR CRCStr[64];
			lynxSprintf(CRCStr, _T("%x"), CRCCode);

			return CString(CRCStr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBlendState::MakeName()
		{
			SetName(MakeName(m_Desc));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CBlendState::vSave(CStream& s)
		{
			s.vWrite(&m_Desc.bAlphaToCoverage,	sizeof(LYNXBOOL), 1);	
			s.vWrite(&m_Desc.bIndependentBlend,	sizeof(LYNXBOOL), 1);	
			s.vWrite(m_Desc.RTBlendParam,		sizeof(CRenderer::RTBLENDPARAM), 8);	
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CBlendState::LoadDesc(CRenderer::BLENDSTATEDESC& desc, CStream& s, LYNXBOOL skip)
		{
			if (skip)
			{
				LYNXLONG SeekLen = sizeof(LYNXBOOL) +
								   sizeof(LYNXBOOL) +
								   sizeof(CRenderer::RTBLENDPARAM)*8;
				s.vSeek(SeekLen, CStream::SeekCur);	
			}
			else
			{
				s.vRead(&desc.bAlphaToCoverage,		sizeof(LYNXBOOL), 1);	
				s.vRead(&desc.bIndependentBlend,	sizeof(LYNXBOOL), 1);	
				s.vRead(desc.RTBlendParam,			sizeof(CRenderer::RTBLENDPARAM), 8);	
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		void CBlendState::Load(CRenderer::BLENDSTATEDESC& desc, CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			desc.Default();

			desc.bAlphaToCoverage = file->Read(_T("bAlphaToCoverage"), desc.bAlphaToCoverage, Root);
			desc.bIndependentBlend = file->Read(_T("bIndependentBlend"), desc.bIndependentBlend, Root);
			int i = 0;
			CXMLElement* Params = Root->FirstChildElement("BlendParams");
			if (Params)
			{
				CXMLElement* Param = Params->FirstChildElement("Param");
				while (Param)
				{
					desc.RTBlendParam[i].bBlend = file->Read(_T("bBlend"), desc.RTBlendParam[i].bBlend, Param);
					desc.RTBlendParam[i].SrcBlend = GetBlendFunc(file->Read(_T("SrcBlend"), GetEnumStr(desc.RTBlendParam[i].SrcBlend), Param));
					desc.RTBlendParam[i].DestBlend = GetBlendFunc(file->Read(_T("DestBlend"), GetEnumStr(desc.RTBlendParam[i].DestBlend), Param));
					desc.RTBlendParam[i].BlendOp = GetBlendOp(file->Read(_T("BlendOp"), GetEnumStr(desc.RTBlendParam[i].BlendOp), Param));
					desc.RTBlendParam[i].SrcBlendAlpha = GetBlendFunc(file->Read(_T("SrcBlendAlpha"), GetEnumStr(desc.RTBlendParam[i].SrcBlendAlpha), Param));
					desc.RTBlendParam[i].DestBlendAlpha = GetBlendFunc(file->Read(_T("DestBlendAlpha"), GetEnumStr(desc.RTBlendParam[i].DestBlendAlpha), Param));
					desc.RTBlendParam[i].RTWriteMask = file->Read(_T("RTWriteMask"), (LYNXUINT16)desc.RTBlendParam[i].RTWriteMask, Param);

					i++;
					Param = Param->NextSiblingElement("Param");
				}
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CBlendState::vLoad(CUserProfile* file, CXMLElement* node)
		{
			Load(m_Desc, file, node);			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CBlendState::vSave(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			file->Write(_T("bAlphaToCoverage"), m_Desc.bAlphaToCoverage, Root);
			file->Write(_T("bIndependentBlend"), m_Desc.bIndependentBlend, Root);

			CXMLElement* Params = file->GetDoc()->NewElement("BlendParams");
			Root->InsertEndChild(Params);
			for (int i = 0; i < 8; i++)
			{
				CXMLElement* Param = file->GetDoc()->NewElement("Param");
				Params->InsertEndChild(Param);

				file->Write(_T("bBlend"), m_Desc.RTBlendParam[i].bBlend, Param);
				file->Write(_T("SrcBlend"), GetEnumStr(m_Desc.RTBlendParam[i].SrcBlend), Param);
				file->Write(_T("DestBlend"), GetEnumStr(m_Desc.RTBlendParam[i].DestBlend), Param);
				file->Write(_T("BlendOp"), GetEnumStr(m_Desc.RTBlendParam[i].BlendOp), Param);
				file->Write(_T("SrcBlendAlpha"), GetEnumStr(m_Desc.RTBlendParam[i].SrcBlendAlpha), Param);
				file->Write(_T("DestBlendAlpha"), GetEnumStr(m_Desc.RTBlendParam[i].DestBlendAlpha), Param);
				file->Write(_T("RTWriteMask"), (LYNXUINT16)m_Desc.RTBlendParam[i].RTWriteMask, Param);
			}						

			return LYNX_TRUE;
		}
	}
}