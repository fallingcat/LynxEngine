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
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>
#ifdef __LUA__
	#include <LynxLuaScript.h>
#endif

namespace LynxEngine 
{	
	namespace GameConsoleSystem 
	{		
		IMPLEMENT_CLASSSCRIPT(CGameConsoleSystem, CSystem)

		CGameConsoleSystem::CCommandList CGameConsoleSystem::m_CommandList;
		CString	CGameConsoleSystem::CCommandList::m_Open			= "open";;
		CString	CGameConsoleSystem::CCommandList::m_RenderMode		= "rendermode";
		CString	CGameConsoleSystem::CCommandList::m_Exit			= "exit";
		CString	CGameConsoleSystem::CCommandList::m_Show			= "show";
		CString	CGameConsoleSystem::CCommandList::m_Resize			= "resize";
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGameConsoleSystem::CCommandList::CCommandList()
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::InitMembers(void)
		{				
			m_bRunning = LYNX_FALSE;
			m_Prompt = _T("LynxEngine");
			m_CommandLine = _T("");
			m_NumArgs = 0;
			m_CurrentStackPos = m_CommandLineStack.begin();
			SetName(_T("GameConsoleSystem"));
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGameConsoleSystem::CGameConsoleSystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{				
			InitMembers();			

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGameConsoleSystem::~CGameConsoleSystem(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGameConsoleSystem::vInit(void)
		{
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::vInit() OK\r\n")));

			return CSystem::vInit();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGameConsoleSystem::vCreate(void)
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::ParseCommand(void)
		{
			LYNXCHAR CommandLine[128];			
			LYNXCHAR Space[] = _T(" ");
			LYNXCHAR* Str;

			if (m_CommandLine.size())
			{
				lynxStrCpy(CommandLine, m_CommandLine.c_str());
				Str = lynxStrTok(CommandLine, Space);
				lynxStrCpy(m_Command, Str);
				m_NumArgs = 0;
				while (Str)
				{
					Str = lynxStrTok(NULL, Space);
					if (Str)
					{
						lynxStrCpy(m_Arg[m_NumArgs], Str);
						m_NumArgs++;
					}
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::ExecuteCommandLine() 
		{			
			m_CommandLineStack.push_back(m_CommandLine);
			ParseCommand();			
			if (!lynxStriCmp(m_Command, m_CommandList.m_RenderMode.c_str()))
			{
				m_lpEngine->vOnRenderMode(m_NumArgs, m_Arg);				
			}
			else if (!lynxStriCmp(m_Command, m_CommandList.m_Open.c_str()))
			{
				m_lpEngine->vOnOpenScene(m_Arg[0]);					
			}
			else if (!lynxStriCmp(m_Command, m_CommandList.m_Exit.c_str()))
			{
				m_lpEngine->vOnExit();					
			}
			else if (!lynxStriCmp(m_Command, m_CommandList.m_Show.c_str()))
			{
				m_lpEngine->vOnShow(m_NumArgs, m_Arg);				
			}
			else if (!lynxStriCmp(m_Command, m_CommandList.m_Resize.c_str()))
			{
				m_lpEngine->vOnResize(m_Arg[0], m_Arg[1]);				
			}
			else
			{
				m_lpEngine->vOnCommand(m_Command, m_NumArgs, m_Arg);				
			}
			Trigger();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::CommandLineBack()
		{			
			LYNXCHAR Str[512]; 			
			
			lynxStrCpy(Str, m_CommandLine.c_str());
			if (m_CommandLine.length() >= 1)
			{
				Str[m_CommandLine.length()-1] = NULL;
				m_CommandLine = Str;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::LastCommandLineFromStack()
		{	
			if (!m_CommandLineStack.empty())
			{
				if (m_CurrentStackPos == m_CommandLineStack.begin())			
				{
					m_CommandLine = (*m_CurrentStackPos);
					m_CurrentStackPos = m_CommandLineStack.end();
					--m_CurrentStackPos;
				}
				else
				{
					m_CommandLine = (*m_CurrentStackPos);
					--m_CurrentStackPos;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::NextCommandLineFromStack()
		{			
			if (!m_CommandLineStack.empty())
			{
				if (m_CurrentStackPos == m_CommandLineStack.end())			
				{
					m_CurrentStackPos = m_CommandLineStack.begin();
				}
				m_CommandLine = (*m_CurrentStackPos);
				++m_CurrentStackPos;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::vLoop(float step)
		{
			//lynxSDKGetEvent(&((CEngine *)m_lpEngine)->GetSDK());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::vRender()
		{
			static Math::CColor Color(204.0f/255.0f, 152.0f/255.0f, 49.0f/255.0f, 255.0f/255.0f), ShadowColor(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 255.0f/255.0f);
			static Math::CColor BackColor(62.0f/255.0f, 60.0f/255.0f, 58.0f/255.0f, 180.0f/255.0f);
			LYNXRECT Rect;
			static LYNXLONGLONG Frame = 0;
			CString	CommandLine;

			if (IsRunning())
			{
				m_lpEngine->GetlpGraphicsSystem()->SetSystemBackBufferAsRenderBuffer(0);
				m_lpEngine->GetlpGraphicsSystem()->DisableRenderBuffer(1);	
				//m_lpGraphicsSystem->Clear(LYNX_RENDER_BUFFER|LYNX_Z_BUFFER, Color, 1.0f, 0);				

				Rect.x1 = 0;
				Rect.x2 = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth();
				Rect.y1 = 0;
				Rect.y2 = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight();

				CommandLine = m_Prompt + CString("> ") + GetCommandLine();				
				if ((Frame/500)%2)				
				{
					CommandLine += CString("_");				
				}
				
				GraphicsSystem::CRenderer::FIXEDVERTEX V[4];
				LYNXVECTOR2D P0, P1;			
				int Height;

				V[0].x = 0;																		V[0].y = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight()-20;		V[0].z = 0;
				V[1].x = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth();	V[1].y = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight()-20;		V[1].z = 0;
				V[2].x = 0;																		V[2].y = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight();		V[2].z = 0;
				V[3].x = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth();	V[3].y = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight();		V[3].z = 0;

				V[0].color = BackColor.PackedColor();
				V[1].color = BackColor.PackedColor();
				V[2].color = BackColor.PackedColor();
				V[3].color = BackColor.PackedColor();
				
				m_lpEngine->GetlpGraphicsSystem()->GetConsoleMaterial()->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(NULL, NULL);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->UseSpecular(LYNX_FALSE);  				
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexture(0, NULL);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawStrip(V, 0, 0);						
				
				m_lpEngine->GetlpGraphicsSystem()->Printf(&Rect, 10, m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight()-20, Color, ShadowColor, CommandLine.c_str());
				Frame += m_lpEngine->GetlpKernel()->GetFrameElapsedTime();
			}
			else
			{
				Frame = 0;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGameConsoleSystem::DumpTextures(void)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CGameConsoleSystem::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			//((LynxScript::Lua::CScript*)lps)->RegisterVar("m_Prompt", &LynxEngine::GameConsoleSystem::CGameConsoleSystem::m_Prompt);						

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CGameConsoleSystem::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "IsRunning",	LynxEngine::GameConsoleSystem::CGameConsoleSystem, LYNXBOOL,	IsRunning,		(void))		
			REGISTER_SCRIPT_METHOD(lps, "SetPromptStr",	LynxEngine::GameConsoleSystem::CGameConsoleSystem, void*,		SetPromptStr,	(const LYNXCHAR *))		
			
			return LYNX_TRUE;
		}		
	}
}