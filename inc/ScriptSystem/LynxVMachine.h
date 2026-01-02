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

#ifndef __LYNXVMACHINE_H__
#define __LYNXVMACHINE_H__

#include <LynxObj.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace ScriptSystem 
	{
		class LYNXENGCLASS CVMachine : public CObj
		{	
			LYNX_DECLARE_CLASS(CVMachine);
		protected:					
			CScriptSystem*					m_lpScriptSystem;				
		public:			
			CVMachine(CScriptSystem* lps);
			virtual ~CVMachine();		

			virtual	LYNXBOOL				vCreate(int size) = 0;
			virtual	void					vDestory() = 0;		
			virtual CScript* const 			vCreateScript() = 0;	
			//virtual CScriptClass* const		vCreateClass() = 0;				
			//CScriptClass*					AddClass(const LynxEngine::CString& s);			
			//CScriptClass*					FindClass(const LynxEngine::CString& s);			
			virtual LYNXBOOL				vRegisterDefaultClasses() = 0;

			virtual void					vRegisterVarType() = 0;

			virtual void					vRegisterFunction() = 0;
			virtual void					vRegisterVar() = 0;
			virtual void					vDumpAllScripts(const LYNXCHAR* filename) = 0;

			virtual LYNXBOOL				vIsValid() = 0;
		};	
	}
}

#endif