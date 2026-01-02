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

#ifndef __LYNXPOSTPROCESSING_H__
#define __LYNXPOSTPROCESSING_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace GraphicsSystem 
	{
		/**	
		*  @brief CMap is an object which can be used as texture and render targe.
		* 
		*  CMap is the object which can be used as texture and render target.
		*	
		*/
		class LYNXENGCLASS CPostProcessing : public CObj
		{	
			LYNX_DECLARE_CLASS(CPostProcessing);
		public:					
		protected:						
			CGraphicsSystem*						m_lpGraphicsSystem;							

			CList<CMap*>							m_InputMapList;
			CMap*									m_OutputMap;
			MaterialSystem::CMaterialPtr			m_lpMaterial;
		public:			
			CPostProcessing(CGraphicsSystem* lpgs);
			virtual ~CPostProcessing();			

			virtual	LYNXBOOL						vCreate(CMap& output) = 0;
			virtual	void							vAddSourceMap(CMap& src) = 0;
			void									SetOutputMap(CMap& out) {m_OutputMap = &out; };
			virtual	void							vRender(CCameraContainer* cam = NULL) = 0;					
		};	
	}
}

#endif