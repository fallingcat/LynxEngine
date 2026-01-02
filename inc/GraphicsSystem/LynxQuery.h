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

#ifndef __LYNXQUERY_H__
#define __LYNXQUERY_H__

#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CQuery : public CResource
		{
			LYNX_DECLARE_CLASS(CQuery);
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;				
		public:						
			CQuery(CGraphicsSystem* pgs):CResource(pgs->GetlpEngine()) {m_lpGraphicsSystem = pgs; };
			virtual ~CQuery() {LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Destroy("))+GetName()+CString(_T(")\r\n")));};				
			
			virtual LYNXBOOL				vIsValid() = 0;			
			void							SetlpGraphicsSystem(CGraphicsSystem* pgs) {m_lpGraphicsSystem = pgs; };			
			virtual void					vCreate(CRenderer::QUERYTYPE type) = 0;
			virtual void					vStart() = 0;			
			virtual void					vEnd() = 0;		
			virtual LYNXBOOL				vGetData(void* data, DWORD size, LYNXLONG wait_time) = 0;		
		};
	}		
}

#endif
