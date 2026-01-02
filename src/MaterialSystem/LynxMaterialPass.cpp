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
#include <LynxLogger.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <GraphicsSystem/LynxBlendState.h>
#include <GraphicsSystem/LynxRasterizerState.h>
#include <GraphicsSystem/LynxShaderClass.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <MaterialSystem/LynxMaterialSystem.h>

namespace LynxEngine 
{	
	namespace MaterialSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPass::CPass(CTechnique* lpp)
		{				
			m_bDisable = LYNX_FALSE;	
			m_bGlow = LYNX_FALSE;			
			m_bUseFog = LYNX_FALSE;
			m_lpTechnique = lpp;	
			//m_lpShaderGroup = NULL;
			m_lpEngine = lpp->GetlpMaterial()->GetlpEngine();

			GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC SDesc;
			GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
			GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;

			SDesc.Default();
			BDesc.Default();
			RDesc.Default();

			CreateDepthStencilState(SDesc);
			CreateBlendState(BDesc);
			CreateRasterizerState(RDesc);
			//vCreateShaderClasses("GeometryVS", "DiffuseMapXColorPS");				
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPass::CPass(const CPass& m)
		{		
			m_bUseFog = m.m_bUseFog;
			m_TextureAnimationArray = m.m_TextureAnimationArray;
			m_bDisable = m.m_bDisable;
			m_bGlow = m.m_bGlow;			
			m_DepthStencilState = m.m_DepthStencilState;			
			m_BlendState = m.m_BlendState;
			m_RasterizerState = m.m_RasterizerState;						
			//if (m_lpShaderGroup)
			//	m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->ReleaseShaderGroup(m_lpShaderGroup);
			//m_lpShaderGroup = m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->CreateShaderGroup();
			m_lpShaderGroup = (m.m_lpShaderGroup);
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPass::~CPass(void)
		{
			//if (m_lpShaderGroup)
			//	m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->ReleaseShaderGroup(m_lpShaderGroup);

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
			m_TextureAnimationArray.clear();
		}									
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPass& CPass::operator =(const CPass& m)
		{		
			m_bUseFog = m.m_bUseFog;
			m_TextureAnimationArray = m.m_TextureAnimationArray;
			m_bDisable = m.m_bDisable;
			m_bGlow = m.m_bGlow;			
			m_DepthStencilState = m.m_DepthStencilState;			
			m_BlendState = m.m_BlendState;
			m_RasterizerState = m.m_RasterizerState;						
			//if (m_lpShaderGroup)
			//	m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->ReleaseShaderGroup(m_lpShaderGroup);
			//m_lpShaderGroup = m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->CreateShaderGroup();
			m_lpShaderGroup = (m.m_lpShaderGroup);

			return (*this);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::vSetNumTextureAnimation(int num)
		{
			CString Name;
			LYNXCHAR Sub[256];

			m_TextureAnimationArray.resize(num);
			for (int i=0; i<num; i++)
			{
				lynxSprintf(Sub, _T(".TexAni.%d"), i);
				Name = GetName()+CString(Sub);
				m_lpTechnique->GetlpMaterial()->GetlpMaterialSystem()->GetlpEngine()->CreateResource(m_TextureAnimationArray[i], TEXTURE_ANIMATION);
				m_TextureAnimationArray[i]->SetName(Name);
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::AddTextureAnimation()
		{
			CString Name;
			LYNXCHAR Sub[256];
			CTextureAnimationPtr TextureAni;

			lynxSprintf(Sub, _T(".TexAni.%d"), m_TextureAnimationArray.size());
			Name = GetName() + CString(Sub);
			m_lpTechnique->GetlpMaterial()->GetlpMaterialSystem()->GetlpEngine()->CreateResource(TextureAni, TEXTURE_ANIMATION);
			TextureAni->SetName(Name);
			m_TextureAnimationArray.push_back(TextureAni);									
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CPass::vSave(CStream& stream)
		{
			stream.vWrite(&m_bUseFog, sizeof(LYNXBOOL), 1);					
			stream.vWrite(CAnsiString(m_lpShaderGroup->GetVertexShader()->GetName().c_str()), sizeof(LYNXANSICHAR), 64);		
			stream.vWrite(CAnsiString(m_lpShaderGroup->GetPixelShader()->GetName().c_str()), sizeof(LYNXANSICHAR), 64);		
			//write depth stencil state data
			m_DepthStencilState->vSave(stream);
			//write blend stencil state data
			m_BlendState->vSave(stream);
			//write rasterizer state data
			m_RasterizerState->vSave(stream);					
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CPass::vLoad(CStream& stream, LYNXBOOL skip)
		{
			char VSName[128], PSName[128];
			LYNXCHAR UniVSName[128], UniPSName[128];

			stream.vRead(&m_bUseFog, sizeof(LYNXBOOL), 1);
			stream.vRead(VSName, sizeof(LYNXANSICHAR), 64);
			stream.vRead(PSName, sizeof(LYNXANSICHAR), 64);					
			lynxFromAnsiStr(UniVSName, VSName);
			lynxFromAnsiStr(UniPSName, PSName);
			vCreateShaderClasses(VSName, PSName);

			GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC	DepthStateDesc;
			GraphicsSystem::CRenderer::BLENDSTATEDESC			BlendStateDesc;
			GraphicsSystem::CRenderer::RASTERIZERSTATEDESC		RasterizerStateDesc;

			GraphicsSystem::CDepthStencilState::LoadDesc(DepthStateDesc, stream, skip);
			CreateDepthStencilState(DepthStateDesc);

			GraphicsSystem::CBlendState::LoadDesc(BlendStateDesc, stream, skip);
			CreateBlendState(BlendStateDesc);

			GraphicsSystem::CRasterizerState::LoadDesc(RasterizerStateDesc, stream, skip);
			CreateRasterizerState(RasterizerStateDesc);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::vCreateShaderClasses(const CAnsiString& vsname, const CAnsiString& psname)
		{
			LYNXBOOL bCreateShaders = LYNX_TRUE;
			CString ResourceName;
			ResourceName.reserve(512);

			//ResourceName = m_lpTechnique->GetlpMaterial()->GetVertexLayout()->GetName() + CString(_T(".")) + vsname + CString(_T(".")) + psname;
			ResourceName = vsname + CString(_T(".")) + psname;
			m_lpShaderGroup = m_lpTechnique->GetlpMaterial()->GetlpEngine()->FindResource(CString(ResourceName));
		
			if (!m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->IsSharedShaderGroup() || !m_lpShaderGroup)
			{
				m_lpTechnique->GetlpMaterial()->GetlpEngine()->CreateResource(m_lpShaderGroup, SHADER_GROUP);	
				m_lpShaderGroup->SetName(ResourceName);
				m_lpShaderGroup->AddVertexLayout(m_lpTechnique->GetlpMaterial()->GetVertexLayout());			
				if (m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->IsCacheShaderGroup())
				{
					CResourcePtr Res;
					Res = m_lpShaderGroup;
					m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->CacheResource(Res);
				}
				
				if (!m_lpTechnique->GetlpMaterial()->GetlpEngine()->CreateShaderClass(m_lpShaderGroup->m_lpVertexShader, vsname))
				{
					bCreateShaders = LYNX_FALSE;
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Failed to create shader class<"))+CString(vsname.c_str())+CString(_T(">.\r\n")));						
				}
				if (!m_lpTechnique->GetlpMaterial()->GetlpEngine()->CreateShaderClass(m_lpShaderGroup->m_lpPixelShader, psname))
				{
					bCreateShaders = LYNX_FALSE;
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Failed to create shader class<"))+CString(psname.c_str())+CString(_T(">.\r\n")));
				}														
				
				if (bCreateShaders)
				{
					if (!m_lpShaderGroup->vCreate())
					{
						bCreateShaders = LYNX_FALSE;
					}

					if (!m_lpTechnique->GetlpMaterial()->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->IsSharedShaderGroup())
					{					
						LYNXGUID GUID;
						lynxOSMakeGUID(&GUID);
						m_lpShaderGroup->SetName(ResourceName + CString(_T(".")) + CString(lynxOSGUIDToString(&GUID)));
					}
					else
					{
						m_lpShaderGroup->SetName(ResourceName);
					}
				}

				if (bCreateShaders)
				{
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Create Shader Group("))+CString(ResourceName)+CString(_T(")\r\n")));
				}
				else
				{
					/*
					if (m_lpShaderGroup && m_lpShaderGroup->m_lpVertexShader)
					{
						m_lpShaderGroup->m_lpVertexShader.Reset();
						m_lpTechnique->GetlpMaterial()->GetlpEngine()->DestroyShaderClass(m_lpShaderGroup->m_lpVertexShader);						
					}
					if (m_lpShaderGroup && m_lpShaderGroup->m_lpPixelShader)
					{
						m_lpShaderGroup->m_lpPixelShader.Reset();
						m_lpTechnique->GetlpMaterial()->GetlpEngine()->DestroyShaderClass(m_lpShaderGroup->m_lpPixelShader);																		
					}
					*/
					m_lpShaderGroup.Reset();
					//m_lpTechnique->GetlpMaterial()->GetlpEngine()->DestroyResource(ResourceName);
										
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Failed to create Shader Group("))+CString(ResourceName)+CString(_T(") due to missing shaders!!\r\n")));
				}
			}
			else // FIXIT : this condition will cause crash
			{																
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Shader Group("))+CString(ResourceName)+CString(_T(") is already created\r\n")));
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::vCreateCustomShaderClasses(const CString& vsname, const CString& psname)
		{
			if (!m_lpTechnique->GetlpMaterial()->GetlpEngine()->CreateCustomShaderClass(m_lpShaderGroup->m_lpVertexShader, CAnsiString("CustomVS"), vsname))
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Failed to create custom shader class<"))+CString(vsname.c_str())+CString(_T(">.\r\n")));
			}
			if (!m_lpTechnique->GetlpMaterial()->GetlpEngine()->CreateCustomShaderClass(m_lpShaderGroup->m_lpPixelShader, CAnsiString("CustomPS"), psname))
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Failed to create custom shader class<"))+CString(psname.c_str())+CString(_T(">.\r\n")));
			}
			m_lpTechnique->GetlpMaterial()->GetlpEngine()->CreateResource(m_lpShaderGroup, SHADER_GROUP);	
			m_lpShaderGroup->AddVertexLayout(m_lpTechnique->GetlpMaterial()->GetVertexLayout());
			m_lpShaderGroup->AddVertexShader(m_lpShaderGroup->m_lpVertexShader);
			m_lpShaderGroup->AddPixelShader(m_lpShaderGroup->m_lpPixelShader);
			m_lpShaderGroup->vCreate();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::CreateDepthStencilState(const GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC& desc)
		{
			((CEngine*)m_lpTechnique->GetlpMaterial()->GetlpEngine())->GetlpGraphicsSystem()->GetlpRenderer()->CreateDepthStencilState(m_DepthStencilState, desc);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::CreateBlendState(const GraphicsSystem::CRenderer::BLENDSTATEDESC& desc)
		{
			((CEngine*)m_lpTechnique->GetlpMaterial()->GetlpEngine())->GetlpGraphicsSystem()->GetlpRenderer()->CreateBlendState(m_BlendState, desc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::CreateRasterizerState(const GraphicsSystem::CRenderer::RASTERIZERSTATEDESC& desc)
		{
			((CEngine*)m_lpTechnique->GetlpMaterial()->GetlpEngine())->GetlpGraphicsSystem()->GetlpRenderer()->CreateRasterizerState(m_RasterizerState, desc);
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::CreateTextureAnimation(int channel, int num)
		{
			m_TextureAnimationArray[channel]->Create(num);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::LoadTexture(int channel, int frame, const char *filename)
		{
			//lynxLoadTexture(&m_TextureFrame[channel].lpTexIndexList[frame], NULL, 0, filename, NULL, 0, LYNX_NORMAL_TEXTURE);		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::vPlayAll(LYNXREAL skip)
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPass::vPlay(int channel, LYNXREAL skip)
		{
			return LYNX_TRUE;
		}					
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::Set(CContainer* lpo, CCameraContainer* cam)
		{
			GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpTechnique->GetlpMaterial()->GetlpEngine())->GetlpGraphicsSystem()->GetlpRenderer();

            if (m_lpShaderGroup)
            {
                lpR->UseFog(m_bUseFog);									
                lpR->vSetShaderGroup(m_lpShaderGroup, lpo, cam);
                    
                for (int i=0; i<m_TextureAnimationArray.size(); i++)
                {
                    if (m_TextureAnimationArray[i])
                    {
                        m_TextureAnimationArray[i]->vSet(m_lpShaderGroup->m_lpPixelShader->GetlpShader(), i);
                    }                        								
                }				
            }
            lpR->SetDepthStencilState(m_DepthStencilState);
            lpR->SetBlendState(m_BlendState);
            lpR->SetRasterizerState(m_RasterizerState);
                
            m_lpTechnique->GetlpMaterial()->GetlpMaterialSystem()->SetCurrentMaterialPass(this);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CPass::vSetParameter(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CPass::vLoad(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();
						
			CString VSName, PSName;

			VSName = file->Read(_T("VS"), "", Root);
			PSName = file->Read(_T("PS"), "", Root);
			vCreateShaderClasses(VSName, PSName);
						
			GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
			CXMLElement* DepthStencil = Root->FirstChildElement("DepthStencil");
			GraphicsSystem::CDepthStencilState::Load(DDesc, file, DepthStencil);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->CreateDepthStencilState(m_DepthStencilState, DDesc);

			GetDepthStencilState()->vLoad(file, DepthStencil);

			GraphicsSystem::CRenderer::BLENDSTATEDESC BDesc;
			CXMLElement* Blend = Root->FirstChildElement("Blend");			
			GraphicsSystem::CBlendState::Load(BDesc, file, Blend);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->CreateBlendState(m_BlendState, BDesc);
			
			GraphicsSystem::CRenderer::RASTERIZERSTATEDESC RDesc;
			CXMLElement* Rasterizer = Root->FirstChildElement("Rasterizer");
			GraphicsSystem::CRasterizerState::Load(RDesc, file, Rasterizer);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->CreateRasterizerState(m_RasterizerState, RDesc);			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CPass::vSave(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			file->Write(_T("VS"), m_lpShaderGroup->GetVertexShader()->GetName(), Root);
			file->Write(_T("PS"), m_lpShaderGroup->GetPixelShader()->GetName(), Root);

			CXMLElement* DepthStencil = file->GetDoc()->NewElement("DepthStencil");
			Root->InsertEndChild(DepthStencil);
			GetDepthStencilState()->vSave(file, DepthStencil);

			CXMLElement* Blend = file->GetDoc()->NewElement("Blend");
			Root->InsertEndChild(Blend);
			GetBlendState()->vSave(file, Blend);

			CXMLElement* Rasterizer = file->GetDoc()->NewElement("Rasterizer");
			Root->InsertEndChild(Rasterizer);
			GetRasterizerState()->vSave(file, Rasterizer);

			return LYNX_TRUE;
		}
	}
}