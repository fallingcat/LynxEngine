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

#ifndef __LYNXTREENODE_H__
#define __LYNXTREENODE_H__

#include <LynxArray.h>
#include <LynxObj.h>
#include <LynxCore.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CTreeNode : public CObj
	{
		LYNX_DECLARE_CLASS(CTreeNode);
	public:
		int									m_ID;	
		LYNXBOUNDARYBOX						m_BBox;		
		LYNXVECTOR3D						m_Center;
		LYNXREAL							m_R;	
		CArray<CTreeNode*>					m_Children;			
	public:		
		CTreeNode();
		virtual ~CTreeNode();								

		inline LYNXBOUNDARYBOX&				GetBBox() {return m_BBox; };
	};
}

#endif
