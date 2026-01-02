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

#ifndef __LYNXRENDERSTATE_H__
#define __LYNXRENDERSTATE_H__

#include <LynxResource.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CRenderState : public CResource
		{
			LYNX_DECLARE_CLASS(CRenderState);
		public:
			typedef enum {				
				NULL_DB_MODE = -1,
				DB_ALWAYS,
				DB_NONE,
				DB_CW,
				DB_W,
				DB_C,	
				NUM_DEPTHBUFFER_MODE,
			}DEPTHBUFFERMODE;

			typedef enum {				
				NULL_CULL = -1,
				CULL_NONE,
				CULL_CCW,
				CULL_CW,
				NUM_CULL_MODE,
			}CULLMODE;

			typedef enum {
				BF_ZERO = -1,					/*!< Blend factor is (0, 0, 0, 0). */  	
				BF_ONE,							/*!< Blend factor is (1, 1, 1, 1).  */  	
				BF_SRC_COLOR,					/*!< Blend factor is (Rs,Gs,Bs,As).  */  	
				BF_ONE_MINUS_SRC_COLOR,			/*!< Blend factor is (1 - Rs, 1 - Gs, 1 - Bs, 1 - As).  */  			
				BF_SRC_ALPHA,					/*!< Blend factor is (As, As, As, As).  */  	
				BF_ONE_MINUS_SRC_ALPHA,			/*!< Blend factor is ( 1 - As, 1 - As, 1 - As, 1 - As).  */  	
				BF_DEST_ALPHA,					/*!< Blend factor is (Ad, Ad, Ad, Ad).  */  	
				BF_ONE_MINUS_DEST_ALPHA,		/*!< Blend factor is (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad).  */  	
				BF_DEST_COLOR,					/*!< Blend factor is (Rd, Gd, Bd, Ad).  */  	
				BF_ONE_MINUS_DEST_COLOR,		/*!< Blend factor is (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad).  */  	
				BF_SRC_ALPHA_SAT,				/*!< Blend factor is (f, f, f, 1); f = min(A, 1 - Ad).  */  	
				BF_BOTH_SRC_ALPHA,				/*!< */  	
				BF_BOTH_ONE_MINUS_SRC_ALPHA,	/*!< Source blend factor is (1 - As, 1 - As, 1 - As, 1 - As), and destination blend factor is (As, As, As, As); the destination blend selection is overridden. This blend mode is supported only for the D3DRS_SRCBLEND render state.  */  	
				BF_BLEND_FACTOR,				/*!< Constant color blending factor used by the frame-buffer blender. This blend mode is supported only if D3DPBLEND_BLENDFACTOR is on. */  	
				BF_ONE_MINUS_BLEND_FACTOR,		/*!< Inverted constant color-blending factor used by the frame-buffer blender. This blend mode is supported only if the D3DPBLEND_BLENDFACTOR bit is set in the SrcBlendCaps or DestBlendCaps members of D3DCAPS9. */  	
			}BLENDFUNC;
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;

			DEPTHBUFFERMODE					m_DepthBufferMode;		
			CULLMODE						m_CullMode;
			BLENDFUNC						m_SrcBlend, m_DestBlend;
		public:						
			CRenderState();
			CRenderState(CGraphicsSystem* lpsys);
			virtual ~CRenderState();	

			virtual void					vSet() = 0;
		};
	}		
}

#endif