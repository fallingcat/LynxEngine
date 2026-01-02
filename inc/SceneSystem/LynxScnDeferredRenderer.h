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

#ifndef __LYNXSCNDEFERREDRENDERER_H__
#define __LYNXSCNDEFERREDRENDERER_H__

#include <LynxString.h>
#include <LynxGameObj.h>
#include <SceneSystem/LynxScnRenderer.h>

namespace LynxEngine 
{	
	namespace SceneSystem 
	{		
		class LYNXENGCLASS CDeferredRenderer : public CRenderer
		{	
			LYNX_DECLARE_CLASS(CDeferredRenderer);
		public:						
			typedef enum {
				DIFFUSE,
				NORMAL,
				SPECULAR,
				GLOW,
				NUM_GBUFFERTYPE,
			}GBUFFERTYPE;			

			typedef enum {
				AMBIENT_LIGHTING = 0,
				DIRECTIONAL_LIGHTING,
				SPOT_LIGHTING,
				POINT_LIGHTING,
				NUM_MATERIALTYPE,
			}MATERIALTYPE;			

		protected:		
			CArray<GraphicsSystem::CMap*>				m_GBufferArray;
			CArray<MaterialSystem::CMaterialPtr>		m_DeferredLightingMaterialArray;
		public:			
			CDeferredRenderer(CScene* s);
			virtual ~CDeferredRenderer();			
				
			virtual	LYNXBOOL							vCreate();	
			virtual LYNXBOOL							vResize(int x, int y);

			LYNXFORCEINLINE GraphicsSystem::CMap*		GetlpGBuffer(GBUFFERTYPE t) {return m_GBufferArray[t]; };
			LYNXFORCEINLINE MaterialSystem::CMaterialPtr& GetlpDeferredLightingMaterial(MATERIALTYPE t) {return m_DeferredLightingMaterialArray[t]; };

			// Passes
			virtual void								vLightingPass(CCameraContainer& camera);
			void										GPass(CCameraContainer& camera);
			void										OpaqueLightingPass(CCameraContainer& camera);
			void										TranslucentLightingPass(CCameraContainer& camera);			
		};
	}
}
#endif