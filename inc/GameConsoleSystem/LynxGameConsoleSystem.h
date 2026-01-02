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

#ifndef __LYNXGAMECONSOLESYSTEM_H__
#define __LYNXGAMECONSOLESYSTEM_H__

#include <LynxList.h>
#include <LynxSystem.h>

//#include <Font/LynxFont.h>
//#include <Font/LynxFontString.h>

namespace LynxEngine 
{
	namespace GameConsoleSystem 
	{
		class LYNXENGCLASS CGameConsoleSystem : public CSystem
		{	
			LYNX_DECLARE_CLASS(CGameConsoleSystem);
		public:
			class LYNXENGCLASS CCommandList{
			public:
				static CString			m_Open;
				static CString			m_RenderMode;
				static CString			m_Exit;
				static CString			m_Show;
				static CString			m_Resize;

				CCommandList();
			};
			
			//CFont								m_SystemFont;
			//CFontString							m_GeneralString;
		protected:	
			static CCommandList					m_CommandList;
			LYNXBOOL							m_bRunning;
			CString								m_Prompt;
			CString								m_CommandLine;
			LYNXCHAR							m_Command[64];
			int									m_NumArgs;
			LYNXCHAR							m_Arg[16][64];
			CList<CString>						m_CommandLineStack;
			CList<CString>::CIterator			m_CurrentStackPos;
		private:
			void								InitMembers();			
		public:			
			CGameConsoleSystem(CEngine* const lpengine);			
			~CGameConsoleSystem();	

			DECLARE_CLASSSCRIPT

			virtual LYNXBOOL					vInit(void);	
			virtual LYNXBOOL					vCreate(void);						
			virtual void						vLoop(float step);
			virtual void						vRender();
			
			LYNXINLINE void						Start() {m_bRunning = LYNX_TRUE;};
			LYNXINLINE void						End() {m_bRunning = LYNX_FALSE;};
			LYNXINLINE void						Trigger() {m_bRunning = !m_bRunning; m_CommandLine = CString(_T("")); m_CurrentStackPos = m_CommandLineStack.begin(); };
			LYNXINLINE LYNXBOOL					IsRunning() {return m_bRunning;};
			LYNXINLINE CString&					GetCommandLine() {return m_CommandLine;};
			LYNXINLINE void						CommandLineCat(LYNXCHAR c) {LYNXCHAR C[2]; C[0] = c; C[1] = (LYNXCHAR)NULL; m_CommandLine += C;};
			void								CommandLineBack();
			void								LastCommandLineFromStack();
			void								NextCommandLineFromStack();			
			void								ParseCommand();
			void								ExecuteCommandLine();
			void								DumpTextures();
			LYNXINLINE void						SetPromptStr(const LYNXCHAR* s) {m_Prompt = s; };						

			LYNXBOOL							vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			LYNXBOOL							vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
		};		
	}
}

#endif