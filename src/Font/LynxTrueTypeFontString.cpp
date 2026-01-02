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
//  ?°?˙:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Font/LynxTrueTypeFont.h>
#include <Font/LynxFontLabel.h>
#include <Font/LynxTrueTypeFontString.h>

namespace LynxEngine
{
	//-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	CTrueTypeFontString::CTrueTypeFontString(CEngine *lpengine)
	: CObj(lpengine)
	{
        m_CurrentLabel = 0;
        m_CurrentFont = NULL;
        m_CurrentAlignmentType = CSpriteContainer::H_ALIGN_CENTER;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	CTrueTypeFontString::~CTrueTypeFontString(void)
	{
		vRelease();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	void CTrueTypeFontString::vRelease(void)
	{
        for (int i=0; i<m_FontLabelArray.size(); i++)
        {
            LYNXDEL m_FontLabelArray[i];
        }
        m_FontLabelArray.clear();
	}
    //-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	void CTrueTypeFontString::Reset()
	{
        m_CurrentLabel = 0;
        for (int i=0; i<m_FontLabelArray.size(); i++)
        {
            m_FontLabelArray[i]->vShow(LYNX_FALSE);
        }
	}
    //-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	void CTrueTypeFontString::CreateLabels(int count)
    {
        CFontLabel* Label = NULL;
        for (int i=0; i<count; i++)
        {
            Label = LYNXNEW CPlatformFontLabel(m_lpEngine);
            Label->vCreate();
            Label->vShow(LYNX_FALSE);
            m_FontLabelArray.push_back(Label);
        }
    }
	//-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CTrueTypeFontString::Create(const DWORD init_num_label)
	{
        m_FontLabelArray.clear();
        CreateLabels(init_num_label);
        m_CurrentLabel = 0;
        
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	void CTrueTypeFontString::SetFont(CFontPtr& lpfont)
	{
        m_CurrentFont = static_cast<CTrueTypeFont*>((CFont*)lpfont);
        //m_FontLabelArray[m_CurrentLabel]->vSetFont(m_CurrentFont);
    }
    //-------------------------------------------------------------------------------------------------------
    /**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
    //-------------------------------------------------------------------------------------------------------
    void CTrueTypeFontString::SetOriginType(const CSpriteContainer::ORIGINTYPE ot)
    {
    }
    //-------------------------------------------------------------------------------------------------------
    /**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
    //-------------------------------------------------------------------------------------------------------
    const CSpriteContainer::ORIGINTYPE CTrueTypeFontString::GetOriginType() const
    {
        return CSpriteContainer::OT_CENTER;
    }
    //-------------------------------------------------------------------------------------------------------
    /**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
    //-------------------------------------------------------------------------------------------------------
    void CTrueTypeFontString::SetAlignmentType(const WORD at)
    {
        m_CurrentAlignmentType = at;
    }
    //-------------------------------------------------------------------------------------------------------
    /**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
    //-------------------------------------------------------------------------------------------------------
    const WORD CTrueTypeFontString::GetAlignmentType()
    {
        return m_FontLabelArray[m_CurrentLabel]->GetAlignmentType();
    }
    //-------------------------------------------------------------------------------------------------------
    /**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
    //-------------------------------------------------------------------------------------------------------
    void CTrueTypeFontString::SetAlignmentRect(LYNXRECT* lpr)
    {
        m_CurrentAlignmentRect = *lpr;
    }
    //-------------------------------------------------------------------------------------------------------
    /**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
    //-------------------------------------------------------------------------------------------------------
    void CTrueTypeFontString::GetAlignmentRect(LYNXRECT* lpr)
    {
        m_FontLabelArray[m_CurrentLabel]->GetAlignmentRect(lpr);
    }
    //-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	void CTrueTypeFontString::AutoReturn(LYNXBOOL b)
    {
        m_FontLabelArray[m_CurrentLabel]->vAutoReturn(b);
    }
	//-------------------------------------------------------------------------------------------------------
	/**
     *	@brief °C
     *
     *	@param °C
     *	@param °C
     *	@return °C
     */
	//-------------------------------------------------------------------------------------------------------
	void CTrueTypeFontString::Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{
        LYNXCHAR	Buffer[512];
        va_list		argptr;
        
        va_start(argptr,fmt);
        lynxVsnPrintf(Buffer, 510, fmt, argptr);
        va_end(argptr);
        
        if (m_CurrentFont)
            m_FontLabelArray[m_CurrentLabel]->vSetFont(m_CurrentFont);
        m_FontLabelArray[m_CurrentLabel]->vShow(LYNX_TRUE);
        m_FontLabelArray[m_CurrentLabel]->vPrintf(x, y, lpc, Buffer);
        
        m_CurrentLabel++;
        if (m_CurrentLabel >= m_FontLabelArray.size())
        {
            CreateLabels(4);
        }
    }
	//-------------------------------------------------------------------------------------------------------
	/**
	 *	@brief °C
	 *
	 *	@param °C
	 *	@param °C
	 *	@return °C
	 */
	//-------------------------------------------------------------------------------------------------------
	void CTrueTypeFontString::Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{
        LYNXCHAR	Buffer[512];
        va_list		argptr;
        
        va_start(argptr,fmt);
        lynxVsnPrintf(Buffer, 510, fmt, argptr);
        va_end(argptr);
        
        if (m_CurrentFont)
            m_FontLabelArray[m_CurrentLabel]->vSetFont(m_CurrentFont);
        m_FontLabelArray[m_CurrentLabel]->SetAlignmentType(m_CurrentAlignmentType);
        m_FontLabelArray[m_CurrentLabel]->SetAlignmentRect(&m_CurrentAlignmentRect);
        m_FontLabelArray[m_CurrentLabel]->vShow(LYNX_TRUE);
        m_FontLabelArray[m_CurrentLabel]->vPrintf(lpc, Buffer);
        
        m_CurrentLabel++;
        if (m_CurrentLabel >= m_FontLabelArray.size())
        {
            CreateLabels(4);
        }
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	 *	@brief °C
	 *
	 *	@param °C
	 *	@param °C
	 *	@return °C
	 */
	//-------------------------------------------------------------------------------------------------------
	int CTrueTypeFontString::PrintfLength(const LYNXCHAR *fmt, ...)
	{
        LYNXCHAR	Buffer[512];
        va_list		argptr;
        
        va_start(argptr,fmt);
        lynxVsnPrintf(Buffer, 510, fmt, argptr);
        va_end(argptr);
        
		return m_FontLabelArray[m_CurrentLabel]->vPrintfLength(Buffer);
	}	
}