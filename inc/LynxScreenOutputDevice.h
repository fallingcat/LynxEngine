//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXSCREENOUTPUTDEVICE_H__
#define __LYNXSCREENOUTPUTDEVICE_H__

#include <LynxOutputDevice.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CScreenOutputDevice : public COutputDevice
	{	
		LYNX_DECLARE_CLASS(CScreenOutputDevice);
	public:
		class CLineData{
		public:
			CString								m_Str;
			int									m_x;
			LYNXCOLORRGBA						m_Color;
			LYNXCOLORRGBA						m_ShadowColor;
		};
		LYNXPOINT2D								m_Origin;
	protected:						
		GraphicsSystem::CGraphicsSystem*		m_lpGraphicsSystem;
		LYNXRECT								m_Viewport;
		int										m_StartLine;
		int										m_NumLines;	
		int										m_LineHeight;
		int										m_CurrentPage;
		int										m_NumPages;
		int										m_NumLinesPerPage;
		CArray<CLineData>						m_Lines;		
	public:		
		CScreenOutputDevice();
		virtual ~CScreenOutputDevice();		

		LYNXINLINE void			SetlpGraphicsSystem(GraphicsSystem::CGraphicsSystem* lps) {m_lpGraphicsSystem = lps; };
		LYNXINLINE void			SetupViewport(int x1, int y1, int x2, int y2) {m_Viewport.x1 = y1; m_Viewport.y1 = y1; m_Viewport.x2 = x2; m_Viewport.y2 = y2; };
		int						SetupNumLinesPerPage(int line_height);
		virtual void			vOpen(const CString& name);
		virtual void			vClose();
		virtual void			vClear();
		virtual void			vAppend(const CString& str);
		virtual void			vOutput(const CString& str);
		virtual void			vOutput(int x, int y, const CString& str, LYNXCOLORRGBA* lpc, LYNXCOLORRGBA* lpsc);

		void					Append(int x, const CString& str, LYNXCOLORRGBA* lpc, LYNXCOLORRGBA* lpsc);
		void					Show();
	};		
}

#endif