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

#ifndef __LYNXPROJECTEDMAP_H__
#define __LYNXPROJECTEDMAP_H__

#include <GraphicsSystem/LynxMap.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		class LYNXENGCLASS CProjectedMap : public CMap
		{	
			LYNX_DECLARE_CLASS(CProjectedMap);
		public:								
		protected:		
			CCameraContainer*						m_Camera;
			Math::CMatrix4							m_ViewProjMatrix;
			
		public:			
			CProjectedMap(CGraphicsSystem* lpgs);
			virtual ~CProjectedMap();		
			
			//void									Update
			LYNXBOOL								Create(CRenderer::FORMAT zformat, int mipmaplevel, const LYNXCHAR *name, int w, int h, CRenderer::FORMAT format, int utilsize = 0, int shift = 0);
		};	
	}
}
#endif