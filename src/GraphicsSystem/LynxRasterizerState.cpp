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
		void CRasterizerState::Default(void)
		{				
			m_ResourceType = RASTERIZER_STATE; 

			m_Desc.FillMode = CRenderer::FM_SOLID;
			m_Desc.CullMode = CRenderer::CULL_CCW;			

            m_Name.reserve(128);
            
			MakeName();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRasterizerState::CRasterizerState(CGraphicsSystem* pgs)
		: CResource(pgs->GetlpEngine())
		{
			m_lpGraphicsSystem = pgs;
			Default();

			m_Desc.FillMode = CRenderer::FM_SOLID;
			m_Desc.CullMode = CRenderer::CULL_CCW;			

			SET_PARAM_FLAG(m_Name, CParam::PARAM_READONLY)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CRasterizerState::~CRasterizerState(void)
		{			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRasterizerState::vCreate(const CRenderer::RASTERIZERSTATEDESC& desc)
		{
			m_Desc.FillMode = desc.FillMode;
			m_Desc.CullMode = desc.CullMode;			
			MakeName();

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString CRasterizerState::MakeName(const CRenderer::RASTERIZERSTATEDESC& desc)
		{
            CString Name;
            
            Name.reserve(128);
			Name = (LynxEngine::GetEnumStr(desc.FillMode)+CString(".") +
                    LynxEngine::GetEnumStr(desc.CullMode) );
            
            LYNXCRC CRCCode = lynxComputeCRCFast((const unsigned char*)Name.c_str(), Name.size());
			LYNXCHAR CRCStr[64];
			lynxSprintf(CRCStr, _T("%x"), CRCCode);

			return CString(CRCStr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRasterizerState::MakeName()
		{
			SetName(MakeName(m_Desc));			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CRasterizerState::vSave(CStream& s)
		{
			s.vWrite(&m_Desc.FillMode, sizeof(CRenderer::FILLMODE), 1);	
			s.vWrite(&m_Desc.CullMode, sizeof(CRenderer::CULLMODE), 1);	
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CRasterizerState::LoadDesc(CRenderer::RASTERIZERSTATEDESC& desc, CStream& s, LYNXBOOL skip)
		{
			if (skip)
			{
				LYNXLONG SeekLen = sizeof(CRenderer::FILLMODE) +
								   sizeof(CRenderer::FILLMODE);
				s.vSeek(SeekLen, CStream::SeekCur);	
			}
			else
			{
				s.vRead(&desc.FillMode, sizeof(CRenderer::FILLMODE), 1);	
				s.vRead(&desc.CullMode, sizeof(CRenderer::CULLMODE), 1);	
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		void CRasterizerState::Load(CRenderer::RASTERIZERSTATEDESC& desc, CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			desc.FillMode = LynxEngine::GetFillMode(file->Read(_T("FillMode"), GetEnumStr(desc.FillMode), Root));
			desc.CullMode = LynxEngine::GetCullMode(file->Read(_T("CullMode"), GetEnumStr(desc.CullMode), Root));
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CRasterizerState::vLoad(CUserProfile* file, CXMLElement* node)
		{
			Load(m_Desc, file, node);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CRasterizerState::vSave(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			file->Write(_T("FillMode"), GetEnumStr(m_Desc.FillMode), Root);
			file->Write(_T("CullMode"), GetEnumStr(m_Desc.CullMode), Root);
			
			return LYNX_TRUE;
		}
	}
}