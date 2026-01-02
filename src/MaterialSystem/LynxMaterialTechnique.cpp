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
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <Animation/LynxAnimationKeys.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>

namespace LynxEngine 
{	
	bool operator ==(const CPair<MaterialSystem::CMaterial::TECHTYPE, MaterialSystem::CTechnique*>& t, const CString& name) { return ((t.second->GetName()) == name); }		

	namespace MaterialSystem 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTechnique::CTechnique(CMaterial* lpmat)
		{	
			m_Type = CMaterial::SIMPLE;
			m_lpMaterial = lpmat;			
			m_lpEngine = GetlpMaterial()->GetlpEngine();
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTechnique::~CTechnique(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));
			vRelease();
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTechnique::vRelease(void)
		{			
			for (CArray<CPass*>::CIterator i=m_PassArray.begin(); i!=m_PassArray.end(); ++i)			
			{
				LYNXDEL (*i);
			}
			m_PassArray.clear();		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTechnique& CTechnique::operator =(const CTechnique& m)
		{				
			vRelease();

			m_Type = m.m_Type;
			for (int i=0; i<m_PassArray.size(); i++)
			{
				if (m_PassArray[i])
					LYNXDEL m_PassArray[i];
			}
			vCreatePass(m.m_PassArray.size());					
			for (int i=0; i<m_PassArray.size(); i++)
			{
				(*m_PassArray[i]) = (*m.m_PassArray[i]);
			}
			return *this;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CTechnique::vSave(CStream& stream)
		{
			stream.vWrite(&m_Type, sizeof(LynxEngine::MaterialSystem::CMaterial::TECHTYPE), 1);
			DWORD NumPass = GetNumPass();
			stream.vWrite(&NumPass, sizeof(DWORD), 1);

			for (int p=0; p<GetNumPass(); p++)
			{
				GetPass(p).vSave(stream);				
			}

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CTechnique::vLoad(CStream& stream, LYNXBOOL skip)
		{
			DWORD		NumTech, NumPass;
			LYNXCHAR	CurrentPath[128];			
			GraphicsSystem::CRenderer::BLENDFUNC BlendFuncIndex;

			vRelease();

			lynxStrCpy(CurrentPath, m_lpMaterial->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath());

			stream.vRead(&NumPass, sizeof(DWORD), 1);				
			vCreatePass(NumPass);		

			for (int p=0; p<NumPass; p++)
			{
				m_PassArray[p]->vLoad(stream, skip);
				m_PassArray[p]->m_TextureAnimationArray = m_lpMaterial->GetTextureAnimationArray();
			}			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTechnique::vCreatePass(DWORD numpass)
		{				
			LYNXCHAR Pass[32];
			CString PassStr;

			for (CArray<CPass*>::CIterator i=m_PassArray.begin(); i!=m_PassArray.end(); ++i)			
			{
				LYNXDEL (*i);
			}
			m_PassArray.clear();		

			m_PassArray.resize(numpass);	
			for (int i=0; i<numpass; i++)			
			{
				m_PassArray[i] = LYNXNEW CPass(this);				
				lynxItoA(i, Pass, 10);
				PassStr = Pass;
				//m_PassArray[i]->SetName(GetName()+CString(_T(".P"))+PassStr);
				m_PassArray[i]->SetName(CString(_T(".P"))+PassStr);
			}
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CTechnique::vAddPass(int num)
		{
			LYNXCHAR Pass[32];
			CString PassStr;
			int Base = m_PassArray.size();

			CPass* NewPass;
			for (int i=Base; i<Base+num; i++)			
			{
				NewPass = LYNXNEW CPass(this);
				m_PassArray.push_back(NewPass);				
				lynxItoA(i, Pass, 10);
				PassStr = Pass;
				//m_PassArray[i]->SetName(GetName()+CString(_T(".P"))+PassStr);
				m_PassArray[i]->SetName(CString(_T(".P"))+PassStr);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CTechnique::vPlayAll(LYNXREAL skip)
		{
			for (CArray<CPass*>::CIterator i=m_PassArray.begin(); i!=m_PassArray.end(); ++i)			
			{
				(*i)->vPlayAll(skip);
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTechnique::vPlay(int channel, LYNXREAL skip)
		{
			LYNXBOOL			bRet = LYNX_TRUE;

			for (CArray<CPass*>::CIterator i=m_PassArray.begin(); i!=m_PassArray.end(); ++i)			
			{
				bRet &= (*i)->vPlay(channel, skip);
			}
			return bRet;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CTechnique::vLoad(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();
			
			m_Name = file->Read(_T("Name"), m_Name, Root);

			int i = 0;
			CXMLElement* Passes = Root->FirstChildElement("Passes");
			if (Passes)
			{
				i = 0;
				CXMLElement* Pass = Passes->FirstChildElement("Pass");
				while (Pass)
				{
					i++;
					Pass = Pass->NextSiblingElement("Pass");					
				}

				vCreatePass(i);

				i = 0;
				Pass = Passes->FirstChildElement("Pass");
				while (Pass)
				{
					m_PassArray[i]->vLoad(file, Pass);
					m_PassArray[i]->m_TextureAnimationArray = m_lpMaterial->GetTextureAnimationArray();
					i++;
					Pass = Pass->NextSiblingElement("Pass");
				}				
			}
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CTechnique::vSave(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			file->Write(_T("Type"), m_Type, Root);
			file->Write(_T("Name"), m_Name, Root);

			CXMLElement* Passes = file->GetDoc()->NewElement("Passes");
			Root->InsertEndChild(Passes);

			for (int p = 0; p<m_PassArray.size(); p++)
			{
				CXMLElement* Pass = file->GetDoc()->NewElement("Pass");
				Passes->InsertEndChild(Pass);
				m_PassArray[p]->vSave(file, Pass);
			}
			
			return LYNX_TRUE;
		}
	}
}