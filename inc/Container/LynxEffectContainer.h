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

#ifndef __LYNXEFFECTCONTAINER_H__
#define __LYNXEFFECTCONTAINER_H__

#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CEffectContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CEffectContainer);
	public:					
	protected:		
	private:
		void					InitMembers();
	public:
		CEffectContainer();
		CEffectContainer(CEngine *lpengine);		
		virtual  ~CEffectContainer();		

		CEffectContainer&		operator =(const CEffectContainer& rhs);				
		
		virtual LYNXBOOL		vCreate(const CCreationDesc* desc);		
		virtual LYNXBOOL		vPlay(CReal step);	
	};

	class LYNXENGCLASS CEffectCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CEffectCreationDesc);	
	public:			
		
		CEffectCreationDesc() {};
	};		
}

#endif
