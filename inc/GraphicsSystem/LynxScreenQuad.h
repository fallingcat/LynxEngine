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

#ifndef __LYNXSCREENQUAD_H__
#define __LYNXSCREENQUAD_H__

#include <LynxObj.h>
#include <LynxEngineClasses.h>

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
		class LYNXENGCLASS CScreenQuad : public CObj
		{	
			LYNX_DECLARE_CLASS(CScreenQuad);
		public:					
		protected:						
			CGraphicsSystem*						m_lpGraphicsSystem;							
			CVertexArrayPtr							m_VertexArray;
		public:			
			CScreenQuad(CGraphicsSystem* lpgs);
			virtual ~CScreenQuad();		

			LYNXBOOL								Create(LYNXBOOL bInvertTexCoordV = LYNX_FALSE);
			void									Render();
		};	
	}
}

#endif