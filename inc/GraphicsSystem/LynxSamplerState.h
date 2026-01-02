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

#ifndef __LYNXSAMPLERSTATE_H__
#define __LYNXSAMPLERSTATE_H__

#include <LynxResource.h>
#include <GraphicsSystem/LynxRenderer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CSamplerState : public CResource
		{
			LYNX_DECLARE_CLASS(CSamplerState);
		public:			
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;
			CRenderer::SAMPLERSTATEDESC		m_Desc;
		protected:
			void							Default();
			LYNXBOOL						vLoad(CStream& s, LYNXBOOL skip = LYNX_FALSE) {return LYNX_FALSE; };
		public:						
			CSamplerState(CGraphicsSystem* lpsys);
			virtual ~CSamplerState();	

			bool							operator ==(const CRenderer::SAMPLERSTATEDESC& desc) 
			{
				return ( (m_Desc.Filter == desc.Filter) &&
						 (m_Desc.AddressU == desc.AddressU) &&
						 (m_Desc.AddressV == desc.AddressV) &&
						 (m_Desc.AddressW == desc.AddressW));
			}			
			void									MakeName();
			static CString							MakeName(const CRenderer::SAMPLERSTATEDESC& desc);
			virtual LYNXBOOL						vCreate(const CRenderer::SAMPLERSTATEDESC& desc);
			virtual void							vSet(int channel) = 0;

			LYNXINLINE CRenderer::ADDRESSMODE		GetAddressModeU() const {return m_Desc.AddressU; };
			LYNXINLINE CRenderer::ADDRESSMODE		GetAddressModeV() const {return m_Desc.AddressV; };
			LYNXINLINE CRenderer::ADDRESSMODE		GetAddressModeW() const {return m_Desc.AddressW; };
			LYNXINLINE float						GetMipLODBias() const {return m_Desc.MipLODBias; };
			LYNXINLINE int							GetMaxAnisotropy() const {return m_Desc.MaxAnisotropy; };
			LYNXINLINE CRenderer::TESTFUNC			GetComparisonFunc() const {return m_Desc.ComparisonFunc; };
			LYNXINLINE LYNXCOLORRGBA				GetBorderColor() const {return m_Desc.BorderColor; };
			LYNXINLINE float						GetMinLOD() const {return m_Desc.MinLOD; };
			LYNXINLINE float						GetMaxLOD() const {return m_Desc.MaxLOD; };

			LYNXBOOL								vSave(CStream& s);
			static LYNXBOOL							LoadDesc(CRenderer::SAMPLERSTATEDESC& desc, CStream& s, LYNXBOOL skip);
		};			
	}	
	//LYNXINLINE bool operator ==(const GraphicsSystem::CRasterizerState* pst, const GraphicsSystem::CRenderer::SAMPLERSTATEDESC& desc) 
	//{
	//	//return ( (pst->GetFillMode() == desc.Filter) &&
	//	//		 (pst->GetCullMode() == desc.AddressU) );
	//	return 1;
	//}
}

#endif