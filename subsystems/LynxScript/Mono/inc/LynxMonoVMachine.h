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
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXMONOVMACHINE_H__
#define __LYNXMONOVMACHINE_H__

#include <LynxMathWrap.h>
#include <ScriptSystem/LynxVMachine.h>
#include <LynxList.h>

#ifdef __MONO__

	#include <mono/jit/jit.h>
	#include <mono/metadata/object.h>
	#include <mono/metadata/environment.h>
	#include <mono/metadata/assembly.h>
	#include <mono/metadata/debug-helpers.h>
	
	namespace LynxScript 
	{
		namespace Mono 
		{	
			class CClassObj;
			class CVMachine : public LynxEngine::ScriptSystem::CVMachine
			{	
				LYNX_DECLARE_CLASS(CVMachine);
			public:			
			protected:		
				MonoDomain*						m_Domain;
				MonoAssembly*					m_Assembly;
				MonoImage*						m_Image;
				MonoAssembly*					m_GameAssembly;
				MonoImage*						m_GameImage;
			public:			
				CVMachine(LynxEngine::ScriptSystem::CScriptSystem* lps);
				virtual ~CVMachine();		

				LYNXINLINE MonoDomain*			GetDomain() const {return m_Domain; }
				LYNXINLINE MonoAssembly*		GetAssembly() const {return m_Assembly; }
				LYNXINLINE MonoImage*			GetImage() const {return m_Image; }
				LYNXINLINE MonoAssembly*		GetGameAssembly() const {return m_GameAssembly; }
				LYNXINLINE MonoImage*			GetGameImage() const {return m_GameImage; }

				virtual	LYNXBOOL				vCreate(int size);		
				LYNXBOOL						LoadGameImage(LynxEngine::CString& filename);
				virtual	void					vDestory();		
				virtual LynxEngine::ScriptSystem::CScript* const  vCreateScript();						

				virtual LYNXBOOL				vRegisterDefaultClasses() {return LYNX_TRUE; }

				virtual void					vRegisterVarType() {return; }

				virtual void					vRegisterFunction() {return; }
				virtual void					vRegisterVar() {return; }
				virtual void					vDumpAllScripts(const LYNXCHAR* filename) {return; }
			};	
		}
	}

#endif

#endif
