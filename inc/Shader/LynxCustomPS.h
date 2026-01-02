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

#ifndef __LYNXCUSTOMPS_H__
#define __LYNXCUSTOMPS_H__

#include <GraphicsSystem/LynxShaderClass.h>

namespace LynxEngine 
{	
	namespace Shader 
	{		
		class LYNXENGCLASS CCustomPS : public GraphicsSystem::CShaderClass
		{	
			LYNX_DECLARE_CLASS(CCustomPS);
		public:						
		protected:		
			CList<GraphicsSystem::CShaderConstant*>			m_ConstantList;			
			CList<GraphicsSystem::CShaderResource*>			m_ResourceList;
		public:			
			CCustomPS(GraphicsSystem::CGraphicsSystem* lpg);
			virtual ~CCustomPS();

		#ifdef _UNICODE
			LYNXINLINE bool						operator == (const CAnsiString& name) {return (GetName() == CString(name)); };
		#endif
			LYNXINLINE bool						operator == (const CString& name) {return (GetName() == name); };

			virtual LYNXBOOL					vCreate(const CString& filename);
			virtual void						vSet(CContainer* lpc, CCameraContainer* cam);
		private:
			virtual LYNXBOOL					vCreate() {return LYNX_FALSE; };
		};		
	}		
}

#endif