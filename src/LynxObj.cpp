//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 1
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxCore.h>
#include <LynxObj.h>
#include <LynxLogger.h>
#include <LynxEngine.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CObj::CObj(void)	
	{
		m_lpEngine = NULL;
		m_ID = 0; 
		m_ClassScriptClassName = _T("CObj");				
		IMPLEMENT_PARAM_STRING(m_Name, 0)			
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CObj::CObj(CEngine* lpe)	
	{
		m_lpEngine = lpe;
		m_ID = 0; 
		m_ClassScriptClassName = _T("CObj");				
		IMPLEMENT_PARAM_STRING(m_Name, 0)
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CObj::~CObj(void)
	{
		m_Name.clear();
		m_ClassScriptClassName.clear();
		//vDestroy();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CObj::vInstance(const CObj& rhs)
	{
		m_lpEngine = rhs.m_lpEngine;
		m_Name = rhs.m_Name;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	const LYNXCRC CObj::ComputeCRCCode(const CString& str)
	{
		LYNXCRC CRCCode = lynxComputeCRCFast((BYTE*)str.c_str(), str.size()*sizeof(LYNXCHAR));
		return CRCCode;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CObj::vDispose(DWORD count)
	{			
		LYNXCHAR Count[32];

		lynxItoA(count, Count, 10);
		
		LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Dispose("))+GetName()+CString(_T(") : "))+CString((Count)));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CObj::CreateClassScript()
	{
		LYNX_ASSERT(m_lpEngine);

		if (m_lpEngine->GetlpScriptSystem())
		{
			m_lpEngine->GetlpScriptSystem()->CreateScript(m_lpClassScript);			
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//  Desc : Since CObj is the base class so there is no super class script for CObj needs to be laoded. 
	//  說明: 由於CObj是base class, 所以不需再載入任何super class script.  
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CObj::LoadSuperClassScript(ScriptSystem::CScript* lpclass)
	{			
		return LYNX_TRUE;
	}		
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CObj::ScriptBindObject()
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
		{
			#ifdef __LUA__
				if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpClassScript)
					m_lpClassScript->vBindObject(this);
			#endif

			#ifdef __MONO__
				//if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpClassScript)
				//	m_lpClassScript->vBindObject((CObj*)this);					
			#endif
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vRegisterScriptClassTable(ScriptSystem::CScript* const lps, const CAnsiString& classname)
	{
		lps->vRegisterClass(classname);	

		return LYNX_TRUE;		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
	{
		REGISTER_SCRIPT_VAR(lps, "m_ID", LynxEngine::CObj, m_ID)

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
	{		
		REGISTER_SCRIPT_METHOD(lps, "GetName",	LynxEngine::CObj, const LYNXCHAR*,	_GetName,	(void))
		REGISTER_SCRIPT_METHOD(lps, "SetName",	LynxEngine::CObj, void,				SetName,	(const LYNXCHAR *))
		REGISTER_SCRIPT_METHOD(lps, "GetID",	LynxEngine::CObj, OBJID,			GetID,		(void))
		REGISTER_SCRIPT_METHOD(lps, "SetID",	LynxEngine::CObj, void,				SetID,		(OBJID))
	
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::RegisterScriptClass(ScriptSystem::CScript* const lps, const CAnsiString& classname)
	{
        LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Register script class <"))+LynxEngine::CString(classname)+LynxEngine::CString(_T(">\r\n")));
		vRegisterScriptClassTable(lps, classname);		
		vRegisterScriptClassMethod(lps);
		vRegisterScriptClassVar(lps);

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vSave(CStream& s)
	{
		s.vWrite(&m_ID, sizeof(OBJID), 1);	
		s.vWrite(&m_CRCCode, sizeof(LYNXCRC), 1);		

	    LYNXUNICHAR* Name;
		Name = LYNXGLOBAL_NEW LYNXUNICHAR[m_Name.size()+1];
		lynxUniStr(Name, m_Name.c_str());
		s.vWrite(Name, sizeof(LYNXUNICHAR), MAX_STRING_LEN);

		s.vWrite(m_ClassScriptClassName.c_str(), sizeof(LYNXANSICHAR), MAX_STRING_LEN);		

		LYNXGLOBAL_DEL_ARRAY(Name);

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vLoad(CStream& s, LYNXBOOL skip)
	{
		if (skip)
		{
			s.vSeek(sizeof(OBJID), CStream::SeekCur);	
			s.vSeek(sizeof(LYNXCRC), CStream::SeekCur);	
			s.vSeek(sizeof(LYNXUNICHAR)*MAX_STRING_LEN, CStream::SeekCur);	
			s.vSeek(sizeof(LYNXANSICHAR)*MAX_STRING_LEN, CStream::SeekCur);				
		}
		else
		{
			s.vRead(&m_ID, sizeof(OBJID), 1);	
			s.vRead(&m_CRCCode, sizeof(LYNXCRC), 1);		

			LYNXUNICHAR UniName[MAX_STRING_LEN];
			LYNXANSICHAR AnsiName[MAX_STRING_LEN];
			LYNXCHAR Name[MAX_STRING_LEN];
			s.vRead(UniName, sizeof(LYNXUNICHAR), MAX_STRING_LEN);
			lynxFromUniStr(Name, UniName);
			m_Name = Name; 

			s.vRead(AnsiName, sizeof(LYNXANSICHAR), MAX_STRING_LEN);	
			m_ClassScriptClassName = AnsiName;
		}

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vLoad(CUserProfile* file, CXMLElement* c)
	{
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vSave(CUserProfile* file, CXMLElement* c)
	{
		return LYNX_TRUE;
	}
    //-------------------------------------------------------------------------------------------------------
	//
	//  說明:
	//-------------------------------------------------------------------------------------------------------
	void CObj::vConstruct()
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid())
		{
			ScriptBindObject();
			m_lpClassScript->vOnConstruct();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vCreate()
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid() && m_lpClassScript)
		{
			ScriptBindObject();			
			m_lpClassScript->vOnCreate();
		}
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vInit()
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid() && m_lpClassScript)
		{
			ScriptBindObject();			
			m_lpClassScript->vOnInit();
		}
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CObj::vLoop(float step)
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid() && m_lpClassScript)
		{
			ScriptBindObject();			
			m_lpClassScript->vOnLoop(step);
		}		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CObj::vRender(CCameraContainer* cam)
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid() && m_lpClassScript)
		{
			ScriptBindObject();			
			m_lpClassScript->vOnRender();
		}		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CObj::vDestroy()
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid() && m_lpClassScript)
		{
			ScriptBindObject();			
			m_lpClassScript->vOnDestroy();
		}
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CObj::AddUserScriptFunc(const CAnsiString& name)
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid() && m_lpClassScript)
		{
			m_lpClassScript->vAddUserHandler(name);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	void CObj::OnUserScriptFunc(const CAnsiString& name)
	{
		if (m_lpEngine->GetlpScriptSystem() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine() && m_lpEngine->GetlpScriptSystem()->GetlpVMachine()->vIsValid() && m_lpClassScript)
		{
			ScriptBindObject();			
			m_lpClassScript->vOnUserFunc(name);
		}
	}
}