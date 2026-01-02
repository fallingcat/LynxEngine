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

#ifndef __LYNXBOXPROXY_H__
#define __LYNXBOXPROXY_H__

#include <LynxObjProxy.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CBoxProxy : public CObjProxy
	{
		LYNX_DECLARE_CLASS(CBoxProxy);
	public:			
	protected:				
	public:			
		CBoxProxy();
		CBoxProxy(CEngine* lpe);
		virtual ~CBoxProxy();	

		void						Create(Math::CVector3& c, Math::CVector3& ext);
	private:			
	};
}

#endif
