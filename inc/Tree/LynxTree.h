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

#ifndef __LYNXTREE_H__
#define __LYNXTREE_H__

#include <LynxArray.h>
#include <LynxObj.h>

namespace LynxEngine 
{
	class CTreeNode;
	class CTreeLeaf;
	class LYNXENGCLASS CTree : public CObj
	{	
		LYNX_DECLARE_CLASS(CTree);
	public:
		LynxEngine::CList<CTreeNode*>	m_Nodes;
		LynxEngine::CList<CTreeLeaf*>	m_Leave;
		CArray<LYNXBOOL>				m_bLeafVisibilities;
	protected:
		CTreeNode*						m_lpRootNode;						
	public:					
		CTree();
		virtual ~CTree();

		LYNXINLINE CTreeNode*					GetlpRootNode() const {return m_lpRootNode; };	
		LYNXINLINE void							SetlpRootNode(CTreeNode* n) {m_lpRootNode = n; };	
		LYNXINLINE const DWORD					GetNumNodes() const {return (DWORD)m_Nodes.size(); };	
		LYNXINLINE const DWORD					GetNumLeaves() const {return (DWORD)m_Leave.size(); };	

		//virtual LYNXINLINE int						vGetLeafIndex(LPLYNXVECTOR3D p) = 0;
		//virtual LYNXINLINE LYNXOSPLEAF&				vGetLeaf(int i) = 0;
		//virtual LYNXINLINE LYNXOSPLEAF&				vGetLeaf(LPLYNXVECTOR3D p) = 0;
	};
}

#endif