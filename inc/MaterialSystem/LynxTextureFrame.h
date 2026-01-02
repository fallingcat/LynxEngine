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

#ifndef __LYNXTEXTUREFRAME_H__
#define __LYNXTEXTUREFRAME_H__

#include <LynxSDKWrap.h>
#include <LynxResource.h>
#include <LynxSystem.h>
#include <LynxParam.h>
#include <Animation/LynxAnimationKeys.h>
#include <Animation/LynxAnimationFrame.h>

namespace LynxEngine 
{	
	namespace MaterialSystem 
	{			
		class LYNXENGCLASS CTextureFrame : public CResource
		{	
			LYNX_DECLARE_CLASS(CTextureFrame);
		public:			
		protected:					
			//LYNXSOURCETYPE					SourceType;
			//int								NumTexs;
			//char								(*lplpTexList)[64];
			CArray<LYNXTEXTUREINDEX>			m_TextureArray;			
			Animation::CAnimationKeys			m_FrameKeys;

			CArray<Animation::CAnimationFrame>	m_FrameArray;		
			Animation::CAnimationFrame*			m_lpCurrentFrame;

			WORD								m_MipMapLevel;
			LYNXTEXTUREFILTERMODE				m_MinFilter, m_MagFilter, m_MipFilter;
			//LYNXBLENDFUNC						m_SrcBlend, m_DestBlend;
		public:			
			CTextureFrame();			
			virtual ~CTextureFrame();							
		};		
	}		
}

#endif