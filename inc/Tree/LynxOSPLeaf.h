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

#ifndef __LYNXOSPLEAF_H__
#define __LYNXOSPLEAF_H__

#include <LynxArray.h>
#include <Tree/LynxTreeLeaf.h>

namespace LynxEngine 
{
	class LYNXENGCLASS COSPLeaf : public CTreeLeaf
	{
		LYNX_DECLARE_CLASS(COSPLeaf);
	public:					
		CArray<CObj*>		m_Elements;		
	public:		
		COSPLeaf();
		virtual ~COSPLeaf();								

		friend class COSP;
	};
}

#endif
