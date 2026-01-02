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

#ifndef __LYNXBLENDSTATE_H__
#define __LYNXBLENDSTATE_H__

#include <LynxResource.h>
#include <GraphicsSystem/LynxRenderer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CBlendState : public CResource
		{
			LYNX_DECLARE_CLASS(CBlendState);
		public:	
			
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;
			CRenderer::BLENDSTATEDESC		m_Desc;			
		protected:
			void							Default();
			LYNXBOOL						vLoad(CStream& s, LYNXBOOL skip = LYNX_FALSE) {return LYNX_FALSE;};
		public:						
			CBlendState(CGraphicsSystem* lpsys);
			virtual ~CBlendState();	

			bool							operator ==(const CRenderer::BLENDSTATEDESC& desc) 
			{
				return ( (m_Desc.bAlphaToCoverage == desc.bAlphaToCoverage) &&
						 (m_Desc.bIndependentBlend == desc.bIndependentBlend) &&
						 (m_Desc.RTBlendParam[0] == desc.RTBlendParam[0]) &&
						 (m_Desc.RTBlendParam[1] == desc.RTBlendParam[1]) &&
						 (m_Desc.RTBlendParam[2] == desc.RTBlendParam[2]) &&
						 (m_Desc.RTBlendParam[3] == desc.RTBlendParam[3]) &&
						 (m_Desc.RTBlendParam[4] == desc.RTBlendParam[4]) &&
						 (m_Desc.RTBlendParam[5] == desc.RTBlendParam[5]) &&
						 (m_Desc.RTBlendParam[6] == desc.RTBlendParam[6]) &&
						 (m_Desc.RTBlendParam[7] == desc.RTBlendParam[7]) );
			}
			LYNXINLINE const LYNXBOOL				GetbAlphaToCoverage() const {return m_Desc.bAlphaToCoverage; };
			LYNXINLINE const LYNXBOOL				GetbIndependentBlend() const {return m_Desc.bIndependentBlend; };
			LYNXINLINE const CRenderer::RTBLENDPARAM GetRTBlendParam(int i) const {return m_Desc.RTBlendParam[i]; };
			const CRenderer::BLENDSTATEDESC&		GetDesc() { return m_Desc; };

			void									MakeName();
			static CString							MakeName(const CRenderer::BLENDSTATEDESC& desc);
			virtual LYNXBOOL						vCreate(const CRenderer::BLENDSTATEDESC& desc);
			virtual void							vSet() = 0;

			LYNXBOOL								vSave(CStream& s);
			static LYNXBOOL							LoadDesc(CRenderer::BLENDSTATEDESC& desc, CStream& s, LYNXBOOL skip);	

			static void								Load(CRenderer::BLENDSTATEDESC& desc, CUserProfile* file, CXMLElement* c);
			LYNXBOOL								vLoad(CUserProfile* file, CXMLElement* c);
			LYNXBOOL								vSave(CUserProfile* file, CXMLElement* c);
		};
		LYNXINLINE bool operator ==(const CBlendState* pst, const CRenderer::BLENDSTATEDESC& desc) 
		{
			return ( (pst->GetbAlphaToCoverage() == desc.bAlphaToCoverage) &&
					 (pst->GetbIndependentBlend() == desc.bIndependentBlend) &&
					 (pst->GetRTBlendParam(0) == desc.RTBlendParam[0]) &&
					 (pst->GetRTBlendParam(1) == desc.RTBlendParam[1]) &&
					 (pst->GetRTBlendParam(2) == desc.RTBlendParam[2]) &&
					 (pst->GetRTBlendParam(3) == desc.RTBlendParam[3]) &&
					 (pst->GetRTBlendParam(4) == desc.RTBlendParam[4]) &&
					 (pst->GetRTBlendParam(5) == desc.RTBlendParam[5]) &&
					 (pst->GetRTBlendParam(6) == desc.RTBlendParam[6]) &&
					 (pst->GetRTBlendParam(7) == desc.RTBlendParam[7]) );
		}		
	}		
}

#endif