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
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxScreenOutputDevice.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CScreenOutputDevice::CScreenOutputDevice(void)	
	{
		m_lpGraphicsSystem = NULL;
		m_StartLine = 0;
		m_NumLines = 0;
		m_CurrentPage = 0;
		m_NumPages = 0;
		m_NumLinesPerPage = 0;
		m_Viewport.x1 = 0;
		m_Viewport.x2 = 0;
		m_Viewport.y1 = 0;
		m_Viewport.y2 = 0;
		m_Origin.x = 0;
		m_Origin.y = 0;
		m_Lines.clear();
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	CScreenOutputDevice::~CScreenOutputDevice(void)
	{
		m_Lines.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CScreenOutputDevice::vOpen(const CString& name) 
	{				
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CScreenOutputDevice::vClose() 
	{		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CScreenOutputDevice::vClear() 
	{
		m_Lines.clear();
		m_StartLine = 0;
		m_NumLines = 0;
		m_CurrentPage = 0;
		m_NumPages = 0;
		m_NumLinesPerPage = 0;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CScreenOutputDevice::vAppend(const CString& str) 
	{			
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CScreenOutputDevice::vOutput(const CString& str) 
	{		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	int CScreenOutputDevice::SetupNumLinesPerPage(int line_height) 
	{
		int Height = m_Viewport.y2-m_Viewport.y1; 
		m_NumLinesPerPage = Height/line_height; 		
		m_LineHeight = line_height;

		return m_NumLinesPerPage; 
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CScreenOutputDevice::Append(int x, const CString& str, LYNXCOLORRGBA* lpc, LYNXCOLORRGBA* lpsc) 
	{		
		CLineData LineData;

		LineData.m_Str = str;
		LineData.m_x = x;
		LineData.m_Color.PackedColor = lpc->PackedColor;
		LineData.m_ShadowColor.PackedColor = lpsc->PackedColor;
		m_Lines.push_back(LineData);
		m_NumLines = (int)m_Lines.size();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CScreenOutputDevice::vOutput(int x, int y, const CString& str, LYNXCOLORRGBA* lpc, LYNXCOLORRGBA* lpsc)
	{
		m_lpGraphicsSystem->Printf(&m_Viewport, m_Origin.x+x, m_Origin.y+y, lpc, lpsc, str.c_str());
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void CScreenOutputDevice::Show()
	{
		for (int i=m_StartLine; i<m_StartLine+m_NumLines; i++)
		{
			m_lpGraphicsSystem->Printf(&m_Viewport, m_Origin.x+m_Lines[i].m_x, m_Origin.y+(i-m_StartLine)*m_LineHeight, &m_Lines[i].m_Color, &m_Lines[i].m_ShadowColor, m_Lines[i].m_Str.c_str());
		}
	}
}