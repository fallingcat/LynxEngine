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

#ifndef __LYNXTERRAINVERTEX_H__
#define __LYNXTERRAINVERTEX_H__

#include <LynxSDK/LynxRenderer.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>
#include <GraphicsSystem/LynxVertexDesc.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class CGraphicsSystem;
		class LYNXENGCLASS CTerrainVertex : public CVertexDesc
		{	
			LYNX_DECLARE_CLASS(CTerrainVertex);
		public:			
		protected:		
			CGraphicsSystem*					m_lpGraphicsSystem;							
		public:			
			CTerrainVertex();				
			CTerrainVertex(CGraphicsSystem* lpgs);
			virtual ~CTerrainVertex();

			void								Release();			
		};		
	}		
}

#endif