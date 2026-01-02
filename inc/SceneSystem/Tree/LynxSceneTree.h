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

#ifndef __LYNXSCENETREE_H__
#define __LYNXSCENETREE_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CTree : public CObj
		{	
			LYNX_DECLARE_CLASS(CTree);
		protected:
		public:						
			CTree() {};
			virtual ~CTree() {};

			//virtual LYNXINLINE int						vGetLeafIndex(LPLYNXVECTOR3D p) = 0;
			//virtual LYNXINLINE LYNXOSPLEAF&				vGetLeaf(int i) = 0;
			//virtual LYNXINLINE LYNXOSPLEAF&				vGetLeaf(LPLYNXVECTOR3D p) = 0;
		};
	}
}

#endif