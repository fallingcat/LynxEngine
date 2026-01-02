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

#ifndef __LYNXSTREAMINGTEXTURE_H__
#define __LYNXSTREAMINGTEXTURE_H__

#include <LynxResource.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CStreamingTexture : public CResource
		{
			LYNX_DECLARE_CLASS(CStreamingTexture);
		public:	
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;
			DECLAR_PARAM_INT(m_BeginningLOD)			
			CString							m_TexturePath;
			LYNXBOOL						m_bStreamingDone;
			CTexturePtr						m_lpLowRezTexture;
			CTexturePtr						m_lpFullRezTexture;
		public:						
			CStreamingTexture(CGraphicsSystem* lpsys);
			virtual ~CStreamingTexture();

			LYNXBOOL						Load(const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL, LPLYNXFILE lpfs = NULL, long offset = 0);
		};
	}		
}

#endif
