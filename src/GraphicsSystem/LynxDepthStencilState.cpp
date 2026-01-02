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

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDepthStencilState::Default(void)
		{				
			m_ResourceType	= DEPTHSTENCIL_STATE; 

			m_Desc.DepthMode			= CRenderer::DB_CW;
			m_Desc.DepthFunc			= CRenderer::TF_LEQUAL;			

			m_Desc.StencilEnable		= LYNX_FALSE;
			m_Desc.StencilFailOp		= CRenderer::SO_KEEP;
			m_Desc.StencilDepthFailOp	= CRenderer::SO_KEEP;
			m_Desc.StencilPassOp		= CRenderer::SO_KEEP;
			m_Desc.StencilFunc			= CRenderer::TF_ALWAYS;
			m_Desc.StencilRef			= 0x0;
			m_Desc.StencilWriteMask		= 0xf;
			m_Desc.StencilReadMask		= 0xf;
            
            m_Name.reserve(128);

			MakeName();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDepthStencilState::CDepthStencilState(CGraphicsSystem* pgs)
		: CResource(pgs->GetlpEngine())
		{
			m_lpGraphicsSystem = pgs;
			Default();

			IMPLEMENT_PARAM_ENUM(m_DepthMode, 0)
			IMPLEMENT_PARAM_ENUM(m_DepthFunc, 0)			

			IMPLEMENT_PARAM_BOOL(m_StencilEnable, 0)
			IMPLEMENT_PARAM_ENUM(m_StencilFailOp, 0)
			IMPLEMENT_PARAM_ENUM(m_StencilDepthFailOp, 0)
			IMPLEMENT_PARAM_ENUM(m_StencilPassOp, 0)
			IMPLEMENT_PARAM_ENUM(m_StencilFunc, 0)		

			SET_PARAM_FLAG(m_Name, CParam::PARAM_READONLY)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDepthStencilState::~CDepthStencilState(void)
		{			
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDepthStencilState::vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc)
		{
			m_Desc.DepthMode			= desc.DepthMode;
			m_Desc.DepthFunc			= desc.DepthFunc;			
			m_Desc.StencilEnable		= desc.StencilEnable;
			m_Desc.StencilFailOp		= desc.StencilFailOp;
			m_Desc.StencilDepthFailOp	= desc.StencilDepthFailOp;
			m_Desc.StencilPassOp		= desc.StencilPassOp;
			m_Desc.StencilFunc			= desc.StencilFunc;
			m_Desc.StencilRef			= desc.StencilRef;
			m_Desc.StencilWriteMask		= desc.StencilWriteMask;
			m_Desc.StencilReadMask		= desc.StencilReadMask;
			MakeName();			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString CDepthStencilState::MakeName(const CRenderer::DEPTHSTENCILSTATEDESC& desc)
		{
			LYNXCHAR Enable[8], Ref[8], WMask[8], RMask[8];

			lynxSprintf(Enable, (LYNXCHAR*)_T("%d"),	desc.StencilEnable);
			lynxSprintf(Ref,	(LYNXCHAR*)_T("%d"),	desc.StencilRef);
			lynxSprintf(WMask,	(LYNXCHAR*)_T("%d"),	desc.StencilWriteMask);						
			lynxSprintf(RMask,	(LYNXCHAR*)_T("%d"),	desc.StencilReadMask);			

            CString Name;
            
            Name.reserve(128);
			Name = (
					LynxEngine::GetEnumStr(desc.DepthMode)+CString(".") +
					LynxEngine::GetEnumStr(desc.DepthFunc)+CString(".") +
					CString(Enable)+CString(".") +
					LynxEngine::GetEnumStr(desc.StencilFailOp)+CString(".") +
					LynxEngine::GetEnumStr(desc.StencilDepthFailOp)+CString(".") +
					LynxEngine::GetEnumStr(desc.StencilPassOp)+CString(".") +
					LynxEngine::GetEnumStr(desc.StencilFunc)+CString(".") +
					CString(Ref)+CString(".") +
					CString(WMask)+CString(".") +
					CString(RMask) );

			LYNXCRC CRCCode = lynxComputeCRCFast((const unsigned char*)Name.c_str(), Name.size());
			LYNXCHAR CRCStr[64];
			lynxSprintf(CRCStr, _T("%x"), CRCCode);

			return CString(CRCStr);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CDepthStencilState::MakeName()
		{
			SetName(MakeName(m_Desc));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CDepthStencilState::vSave(CStream& s)
		{
			s.vWrite(&m_Desc.DepthMode,			sizeof(CRenderer::DEPTHBUFFERMODE), 1);	
			s.vWrite(&m_Desc.DepthFunc,			sizeof(CRenderer::TESTFUNC), 1);	
			s.vWrite(&m_Desc.StencilEnable,		sizeof(LYNXBOOL), 1);	
			s.vWrite(&m_Desc.StencilFailOp,		sizeof(CRenderer::STENCILOP), 1);	
			s.vWrite(&m_Desc.StencilDepthFailOp,sizeof(CRenderer::STENCILOP), 1);	
			s.vWrite(&m_Desc.StencilPassOp,		sizeof(CRenderer::STENCILOP), 1);	
			s.vWrite(&m_Desc.StencilFunc,		sizeof(CRenderer::TESTFUNC), 1);	
			s.vWrite(&m_Desc.StencilRef,		sizeof(unsigned char), 1);	
			s.vWrite(&m_Desc.StencilWriteMask,	sizeof(unsigned char), 1);	
			s.vWrite(&m_Desc.StencilReadMask,	sizeof(unsigned char), 1);				
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CDepthStencilState::LoadDesc(CRenderer::DEPTHSTENCILSTATEDESC& desc, CStream& s, LYNXBOOL skip)
		{
			if (skip)
			{
				LYNXLONG SeekLen = sizeof(CRenderer::DEPTHBUFFERMODE) +
								   sizeof(CRenderer::TESTFUNC) +
								   sizeof(LYNXBOOL) +
								   sizeof(CRenderer::STENCILOP) +
								   sizeof(CRenderer::STENCILOP) +
								   sizeof(CRenderer::STENCILOP) +
								   sizeof(CRenderer::TESTFUNC) +
								   sizeof(LYNXCOLORRGBA) +
								   sizeof(unsigned char) +
								   sizeof(unsigned char);
				s.vSeek(SeekLen, CStream::SeekCur);	
			}
			else
			{
				s.vRead(&desc.DepthMode,			sizeof(CRenderer::DEPTHBUFFERMODE), 1);	
				s.vRead(&desc.DepthFunc,			sizeof(CRenderer::TESTFUNC), 1);	
				s.vRead(&desc.StencilEnable,		sizeof(LYNXBOOL), 1);	
				s.vRead(&desc.StencilFailOp,		sizeof(CRenderer::STENCILOP), 1);	
				s.vRead(&desc.StencilDepthFailOp,	sizeof(CRenderer::STENCILOP), 1);	
				s.vRead(&desc.StencilPassOp,		sizeof(CRenderer::STENCILOP), 1);	
				s.vRead(&desc.StencilFunc,			sizeof(CRenderer::TESTFUNC), 1);	
				s.vRead(&desc.StencilRef,			sizeof(unsigned char), 1);	
				s.vRead(&desc.StencilWriteMask,		sizeof(unsigned char), 1);	
				s.vRead(&desc.StencilReadMask,		sizeof(unsigned char), 1);				
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		void CDepthStencilState::Load(CRenderer::DEPTHSTENCILSTATEDESC& desc, CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			desc.DepthMode = GetDepthBufferMode(file->Read(_T("DepthMode"), GetEnumStr(desc.DepthMode), Root));
			desc.DepthFunc = GetTestFunc(file->Read(_T("DepthFunc"), GetEnumStr(desc.DepthFunc), Root));
			desc.StencilEnable = file->Read(_T("StencilEnable"), desc.StencilEnable, Root);
			desc.StencilDepthFailOp = GetStencilOp(file->Read(_T("StencilFailOp"), GetEnumStr(desc.StencilDepthFailOp), Root));
			desc.StencilDepthFailOp = GetStencilOp(file->Read(_T("StencilDepthFailOp"), GetEnumStr(desc.StencilDepthFailOp), Root));
			desc.StencilPassOp = GetStencilOp(file->Read(_T("StencilPassOp"), GetEnumStr(desc.StencilPassOp), Root));
			desc.StencilFunc = GetTestFunc(file->Read(_T("StencilFunc"), GetEnumStr(desc.StencilFunc), Root));
			desc.StencilRef = file->Read(_T("StencilRef"), desc.StencilRef, Root);
			desc.StencilWriteMask = file->Read(_T("StencilWriteMask"), (LYNXUINT16)desc.StencilWriteMask, Root);
			desc.StencilReadMask = file->Read(_T("StencilReadMask"), (LYNXUINT16)desc.StencilReadMask, Root);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CDepthStencilState::vLoad(CUserProfile* file, CXMLElement* node)
		{
			Load(m_Desc, file, node);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CDepthStencilState::vSave(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			file->Write(_T("DepthMode"), GetEnumStr(m_Desc.DepthMode), Root);
			file->Write(_T("DepthFunc"), GetEnumStr(m_Desc.DepthFunc), Root);
			file->Write(_T("StencilEnable"), m_Desc.StencilEnable, Root);
			file->Write(_T("StencilFailOp"), GetEnumStr(m_Desc.StencilDepthFailOp), Root);
			file->Write(_T("StencilDepthFailOp"), GetEnumStr(m_Desc.StencilDepthFailOp), Root);
			file->Write(_T("StencilPassOp"), GetEnumStr(m_Desc.StencilPassOp), Root);
			file->Write(_T("StencilFunc"), GetEnumStr(m_Desc.StencilFunc), Root);
			file->Write(_T("StencilRef"), m_Desc.StencilRef, Root);
			file->Write(_T("StencilWriteMask"), (LYNXUINT16)m_Desc.StencilWriteMask, Root);
			file->Write(_T("StencilReadMask"), (LYNXUINT16)m_Desc.StencilReadMask, Root);
			
			return LYNX_TRUE;
		}
	}
}