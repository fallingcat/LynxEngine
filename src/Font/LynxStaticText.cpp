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
//  ª°©˙:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <Font/LynxStaticText.h>

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
    CStaticText::CStaticText(CEngine *lpengine)
    : LynxEngine::CObj(lpengine)
    {
        m_FontLabel = NULL;
        m_FontString = m_lpEngine->GetSystemFontString();
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
    CStaticText::~CStaticText(void)
    {
        if (m_FontLabel)
            LYNXDEL m_FontLabel;
        
        m_FontLabel = NULL;
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
    LYNXBOOL CStaticText::Create(STATICTEXTTYPE type)
    {
        switch (type)
        {
            case STT_OS_FONT:
                m_FontType = STT_OS_FONT;
                m_FontLabel = LYNXNEW CPlatformFontLabel(m_lpEngine);
                m_FontLabel->vCreate();
                break;
            
            case STT_TEXTURE_FONT:
            default:
                m_FontType = STT_TEXTURE_FONT;
                break;
        }        
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
    void CStaticText::Show(LYNXBOOL b)
    {
        switch (m_FontType)
        {
            case STT_OS_FONT:
                m_FontLabel->vShow(b);
                break;
                
            case STT_TEXTURE_FONT:
                break;
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
    void CStaticText::SetFont(CResource* src)
    {
        switch (m_FontType)
        {
            case STT_OS_FONT:
                m_FontLabel->vSetFont(static_cast<COSFont*>(src));
                break;
                
            case STT_TEXTURE_FONT:
                m_FontString->SetFont(static_cast<CFont*>(src));
                break;
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
    void CStaticText::SetOriginType(const CSpriteContainer::ORIGINTYPE ot)
    {
        switch (m_FontType)
        {
            case STT_OS_FONT:
                //m_FontLabel->vSetFont(static_cast<COSFont*>(src));
                break;
                
            case STT_TEXTURE_FONT:
                m_FontString->SetOriginType(ot);
                break;
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
    const CSpriteContainer::ORIGINTYPE CStaticText::GetOriginType() const
    {
        switch (m_FontType)
        {
            case STT_OS_FONT:
                //m_FontLabel->vSetFont(static_cast<COSFont*>(src));
                return CSpriteContainer::OT_CENTER;
                break;
                
            case STT_TEXTURE_FONT:
                return m_FontString->GetOriginType();
                break;
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
    void CStaticText::SetAlignmentType(const WORD at)
    {
        switch (m_FontType)
        {
            case STT_OS_FONT:
                m_FontLabel->SetAlignmentType(at);
                break;
                
            case STT_TEXTURE_FONT:
                m_FontString->SetAlignmentType(at);
                break;
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
    const WORD CStaticText::GetAlignmentType()
    {
        switch (m_FontType)
        {
            case STT_OS_FONT:
                return m_FontLabel->GetAlignmentType();
                break;
                
            case STT_TEXTURE_FONT:
                return m_FontString->GetAlignmentType();
                break;
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
    void CStaticText::SetAlignmentRect(LYNXRECT* lpr)
    {
        switch (m_FontType)
        {
            case STT_OS_FONT:
                m_FontLabel->SetAlignmentRect(lpr);
                break;
                
            case STT_TEXTURE_FONT:
                m_FontString->SetAlignmentRect(lpr);
                break;
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
    void CStaticText::GetAlignmentRect(LYNXRECT* lpr)
    {
        switch (m_FontType)
        {
            case STT_OS_FONT:
                m_FontLabel->GetAlignmentRect(lpr);
                break;
                
            case STT_TEXTURE_FONT:
                m_FontString->GetAlignmentRect(lpr);
                break;
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
    void CStaticText::Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
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
    void CStaticText::Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
    {
        LYNXCHAR	Buffer[512];
		va_list		argptr;
		
		va_start(argptr,fmt);
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);
        
        switch (m_FontType)
        {
            case STT_OS_FONT:
                m_FontLabel->vPrintf(lpc, Buffer);
                break;
                
            case STT_TEXTURE_FONT:
                m_FontString->Printf(lpc, Buffer);
                break;
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
    int CStaticText::PrintfLength(const LYNXCHAR *fmt, ...)
    {
        LYNXCHAR	Buffer[512];
		va_list		argptr;
		
		va_start(argptr,fmt);
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);
        
        switch (m_FontType)
        {
            case STT_OS_FONT:
                return m_FontLabel->vPrintfLength(Buffer);
                break;
                
            case STT_TEXTURE_FONT:
                return m_FontString->PrintfLength(Buffer);
                break;
        }
    }
}