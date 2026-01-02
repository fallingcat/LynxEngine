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

#ifndef __LYNXRASTERIZERSTATE_H__
#define __LYNXRASTERIZERSTATE_H__

#include <LynxResource.h>
#include <GraphicsSystem/LynxRenderer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CRasterizerState : public CResource
		{
			LYNX_DECLARE_CLASS(CRasterizerState);
		public:			
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;
			CRenderer::RASTERIZERSTATEDESC	m_Desc;
			
			/*BOOL FrontCounterClockwise;
			INT DepthBias;
			FLOAT DepthBiasClamp;
			FLOAT SlopeScaledDepthBias;
			BOOL DepthClipEnable;
			BOOL ScissorEnable;
			BOOL MultisampleEnable;
			BOOL AntialiasedLineEnable;*/
		protected:
			void							Default();
			LYNXBOOL						vLoad(CStream& s, LYNXBOOL skip = LYNX_FALSE) {return LYNX_FALSE; };
		public:						
			CRasterizerState(CGraphicsSystem* lpsys);
			virtual ~CRasterizerState();	

			bool							operator ==(const CRenderer::RASTERIZERSTATEDESC& desc) 
			{
				return ( (m_Desc.FillMode == desc.FillMode) &&
						 (m_Desc.CullMode == desc.CullMode) );
			}
			LYNXINLINE const CRenderer::FILLMODE	GetFillMode() const {return m_Desc.FillMode; };
			LYNXINLINE const CRenderer::CULLMODE	GetCullMode() const {return m_Desc.CullMode; };
			void									MakeName();
			static CString							MakeName(const CRenderer::RASTERIZERSTATEDESC& desc);
			virtual LYNXBOOL						vCreate(const CRenderer::RASTERIZERSTATEDESC& desc);
			virtual void							vSet() = 0;

			LYNXBOOL								vSave(CStream& s);
			static LYNXBOOL							LoadDesc(CRenderer::RASTERIZERSTATEDESC& desc, CStream& s, LYNXBOOL skip);

			static void								Load(CRenderer::RASTERIZERSTATEDESC& desc, CUserProfile* file, CXMLElement* c);
			LYNXBOOL								vLoad(CUserProfile* file, CXMLElement* c);
			LYNXBOOL								vSave(CUserProfile* file, CXMLElement* c);
		};			
	}	
	LYNXINLINE bool operator ==(const GraphicsSystem::CRasterizerState* pst, const GraphicsSystem::CRenderer::RASTERIZERSTATEDESC& desc) 
	{
		return ( (pst->GetFillMode() == desc.FillMode) &&
	  			 (pst->GetCullMode() == desc.CullMode) );
	}
}

#endif