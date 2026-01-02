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

#ifndef __LYNXDEPTHSTENCILSTATE_H__
#define __LYNXDEPTHSTENCILSTATE_H__

#include <LynxResource.h>
#include <GraphicsSystem/LynxRenderer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CDepthStencilState : public CResource
		{
			LYNX_DECLARE_CLASS(CDepthStencilState);								
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;
			CRenderer::DEPTHSTENCILSTATEDESC m_Desc;			
		protected:
			void							Default();
			LYNXBOOL						vLoad(CStream& s, LYNXBOOL skip = LYNX_FALSE) {return LYNX_FALSE; };
		public:						
			CDepthStencilState(CGraphicsSystem* pgs);
			virtual ~CDepthStencilState();	

			bool							operator ==(const CRenderer::DEPTHSTENCILSTATEDESC& desc) 
			{
				return ( (m_Desc.DepthMode == desc.DepthMode) &&
						 (m_Desc.DepthFunc == desc.DepthFunc) &&
						 (m_Desc.StencilEnable == desc.StencilEnable) &&
						 (m_Desc.StencilFailOp == desc.StencilFailOp) &&
						 (m_Desc.StencilDepthFailOp == desc.StencilDepthFailOp) &&
						 (m_Desc.StencilPassOp == desc.StencilPassOp) &&
						 (m_Desc.StencilFunc == desc.StencilFunc) &&
						 (m_Desc.StencilRef == desc.StencilRef) &&
						 (m_Desc.StencilWriteMask == desc.StencilWriteMask) &&
						 (m_Desc.StencilReadMask == desc.StencilReadMask) );
			}
			LYNXINLINE const CRenderer::DEPTHBUFFERMODE	GetDepthMode() const {return m_Desc.DepthMode; };
			LYNXINLINE const CRenderer::TESTFUNC		GetDepthFunc() const {return m_Desc.DepthFunc; };
			LYNXINLINE const LYNXBOOL					GetStencilEnableHitTest() const {return m_Desc.StencilEnable; };
			LYNXINLINE const CRenderer::STENCILOP		GetStencilFailOp() const {return m_Desc.StencilFailOp; };
			LYNXINLINE const CRenderer::STENCILOP		GetStencilDepthFailOp() const {return m_Desc.StencilDepthFailOp; };
			LYNXINLINE const CRenderer::STENCILOP		GetStencilPassOp() const {return m_Desc.StencilPassOp; };
			LYNXINLINE const CRenderer::TESTFUNC		GetStencilFunc() const {return m_Desc.StencilFunc; };
			LYNXINLINE const unsigned char				GetStencilRef() const {return m_Desc.StencilRef; };
			LYNXINLINE const unsigned char				GetStencilWriteMask() const {return m_Desc.StencilWriteMask; };
			LYNXINLINE const unsigned char				GetStencilReadMask() const {return m_Desc.StencilReadMask; };
			void										MakeName();
			static CString								MakeName(const CRenderer::DEPTHSTENCILSTATEDESC& desc);
			virtual LYNXBOOL							vCreate(const CRenderer::DEPTHSTENCILSTATEDESC& desc);
			virtual void								vSet() = 0;

			LYNXBOOL									vSave(CStream& s);
			static LYNXBOOL								LoadDesc(CRenderer::DEPTHSTENCILSTATEDESC& desc, CStream& s, LYNXBOOL skip);

			static void									Load(CRenderer::DEPTHSTENCILSTATEDESC& desc, CUserProfile* file, CXMLElement* c);
			LYNXBOOL									vLoad(CUserProfile* file, CXMLElement* c);
			LYNXBOOL									vSave(CUserProfile* file, CXMLElement* c);
		};						
	}	
	LYNXINLINE bool operator ==(const GraphicsSystem::CDepthStencilState* pst, const GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC& desc) 
	{
		return ( (pst->GetDepthMode() == desc.DepthMode) &&
	  			 (pst->GetDepthFunc() == desc.DepthFunc) &&
				 (pst->GetStencilEnableHitTest() == desc.StencilEnable) &&
				 (pst->GetStencilFailOp() == desc.StencilFailOp) &&
				 (pst->GetStencilDepthFailOp() == desc.StencilDepthFailOp) &&
				 (pst->GetStencilPassOp() == desc.StencilPassOp) &&
				 (pst->GetStencilFunc() == desc.StencilFunc) &&
				 (pst->GetStencilRef() == desc.StencilRef) &&
				 (pst->GetStencilWriteMask() == desc.StencilWriteMask) &&
				 (pst->GetStencilReadMask() == desc.StencilReadMask) );
	}
}

#endif