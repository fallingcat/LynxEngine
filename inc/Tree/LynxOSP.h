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

#ifndef __LYNXOSP_H__
#define __LYNXOSP_H__

#include <LynxArray.h>
#include <Tree/LynxTree.h>
#include <Tree/LynxOSPNode.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS COSP : public CTree
	{	
		LYNX_DECLARE_CLASS(COSP);
	protected:		
	public:			
		COSP();
		~COSP();


		/*
		void									LoadLeaf(LPLYNXOSPLEAF lpn, LPLYNXFILE lpf);
		void									LoadNode(LPLYNXOSPNODE lpn, LPLYNXFILE lpf);
		LYNXBOOL								Load(char *filename);
		LYNXBOOL								Release();

		LYNXINLINE int							GetLeafIndex(LPLYNXVECTOR3D p) {return lynxOSPGetLeafByPosition(&m_OSP, p); };
		LYNXINLINE LYNXOSPLEAF&					GetLeaf(int i) {return m_OSP.lpLeafList[i]; };
		LYNXINLINE LYNXOSPLEAF&					GetLeaf(LPLYNXVECTOR3D p) {return m_OSP.lpLeafList[lynxOSPGetLeafByPosition(&m_OSP, p)]; };

		friend class CScene;
		*/
	};
}

#endif