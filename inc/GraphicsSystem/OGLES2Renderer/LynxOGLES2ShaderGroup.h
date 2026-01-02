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

#ifndef __LYNXOGLES2SHADERGROUP_H__
#define __LYNXOGLES2SHADERGROUP_H__

#include <LynxResource.h>
#include <LynxEngineClasses.h>
#include <GraphicsSystem/LynxShaderGroup.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS COGLES2ShaderGroup : public CShaderGroup
		{	
			LYNX_DECLARE_CLASS(COGLES2ShaderGroup);
		public:						
		protected:		
			GLuint								m_Program;
			GLint								m_TextureChannel;
		public:			
			COGLES2ShaderGroup(CGraphicsSystem* lpg);
			virtual ~COGLES2ShaderGroup();
						
			LYNXFORCEINLINE GLuint				GetProgram() {return m_Program; };
			LYNXFORCEINLINE GLint				GetTextureChannel() {return m_TextureChannel; };
			LYNXFORCEINLINE void				UseTextureChannel() {m_TextureChannel++; };
			
			LYNXBOOL							vCreate();
			void								vSet(CContainer* lpo, CCameraContainer* cam);
		};		
	}	
}

#endif

#endif