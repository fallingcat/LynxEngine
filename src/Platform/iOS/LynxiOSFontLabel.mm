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
#import <UIKit/UIKit.h>
#include <Platform/iOS/LynxView.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Platform/iOS/LynxiOSFontLabel.h>
#include <Platform/iOS/LynxiOSTrueTypeFont.h>

namespace LynxEngine
{
    namespace iOS
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
        CFontLabel::CFontLabel(LynxEngine::CEngine *lpengine)
        : LynxEngine::CFontLabel(lpengine)
        {
            m_Label = nil;
            m_Font = NULL;
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
        CFontLabel::~CFontLabel(void)
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
        void CFontLabel::vRelease(void)
        {
            if (m_Label)
            {
                vShow(LYNX_FALSE);
                
                [m_Label removeFromSuperview];
                
                //if (m_Label.text)
                //    [m_Label.text release];
                
                //if (m_Label.font)
                //    [m_Label.font release];
                
                //[m_Label release];
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
        LYNXBOOL CFontLabel::vCreate()
        {
            m_Label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
            if (!m_Label)
                return LYNX_FALSE;
            
            m_Label.textAlignment =  NSTextAlignmentCenter;
            m_Label.textColor = [UIColor whiteColor];
            m_Label.backgroundColor = [UIColor clearColor];
            m_Label.shadowColor = [UIColor blackColor];
            m_Label.shadowOffset = CGSizeMake(1, 1);
            //m_Label.font = [UIFont fontWithName:@"Arial Rounded MT Bold" size:(25.0f)];
            m_Label.text = [[NSString alloc] init];
            vShow(LYNX_TRUE);
            
            LynxView* View = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpEngine->GetlpKernel())->GetlpView();
            [View addSubview:m_Label];

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
        void CFontLabel::vShow(LYNXBOOL b)
        {
            LynxEngine::CFontLabel::vShow(b);
            m_Label.hidden = !b;
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
        void CFontLabel::vSetFont(LynxEngine::CTrueTypeFont* lpfont)
        {
            LynxEngine::iOS::CTrueTypeFont* Font = static_cast<LynxEngine::iOS::CTrueTypeFont*>(lpfont);
            
            m_Label.font = Font->m_Font;
            m_Font = Font;
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
        void CFontLabel::vAutoReturn(LYNXBOOL b)
        {
            if (b)
            {
                m_Label.lineBreakMode = NSLineBreakByWordWrapping;
            }
            else
            {
                m_Label.lineBreakMode = NSLineBreakByClipping;
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
        void CFontLabel::vPrintf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
        {
            LYNXCHAR	Buffer[512];
            va_list		argptr;
            
            if (m_Label)
            {
                va_start(argptr,fmt);
                lynxVsnPrintf(Buffer, 510, fmt, argptr);
                va_end(argptr);
                
                m_Label.text = [NSString stringWithCString:(const char*)Buffer encoding:iOS_STRING_ENCODING];
                if (lpc)
                {
                    m_Label.textColor = [UIColor colorWithRed:lpc->Red/255.0f green:lpc->Green/255.0f blue:lpc->Blue/255.0f alpha:lpc->Alpha/255.0f ];
                }
                else
                {
                    m_Label.textColor = [UIColor whiteColor];
                }
                
                m_AlignmentRect.x1 = x;
                m_AlignmentRect.y1 = y;
                m_AlignmentRect.x2 = m_AlignmentRect.x1 + vPrintfLength(Buffer);
                m_AlignmentRect.y2 = m_AlignmentRect.y1 + m_Font->vGetCharacterHeight();
                
                LynxView* View = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpEngine->GetlpKernel())->GetlpView();
                float Scale = 1.0f/View.contentScaleFactor;
                m_Label.frame = CGRectMake(m_AlignmentRect.x1*Scale, m_AlignmentRect.y1*Scale, (m_AlignmentRect.x2-m_AlignmentRect.x1)*Scale, (m_AlignmentRect.y2-m_AlignmentRect.y1)*Scale);
                m_Label.textAlignment =  NSTextAlignmentLeft;
            }
        }
        
        
        
        
        
        /* Some fundamental constants */
        #define UNI_REPLACEMENT_CHAR (LYNXCHAR)0x0000FFFD
        #define UNI_MAX_BMP (LYNXCHAR)0x0000FFFF
        #define UNI_MAX_UTF16 (LYNXCHAR)0x0010FFFF
        #define UNI_MAX_UTF32 (LYNXCHAR)0x7FFFFFFF
        #define UNI_MAX_LEGAL_UTF32 (LYNXCHAR)0x0010FFFF
        
        typedef enum
        {
            conversionOK,
            /* conversion successful */
            sourceExhausted,
            /* partial character in source, but hit end */
            targetExhausted,
            /* insuff. room in target for conversion */
            sourceIllegal           /* source sequence is illegal/malformed */
        } ConversionResult;
        
        typedef enum
        {
            strictConversion    = 0,
            lenientConversion
        } ConversionFlags;
        
        static const int halfShift = 10; /* used for shifting by 10 bits */
        
        static const LYNXCHAR halfBase = 0x0010000UL;
        static const LYNXCHAR halfMask = 0x3FFUL;
        
        #define UNI_SUR_HIGH_START  (LYNXCHAR)0xD800
        #define UNI_SUR_HIGH_END    (LYNXCHAR)0xDBFF
        #define UNI_SUR_LOW_START   (LYNXCHAR)0xDC00
        #define UNI_SUR_LOW_END     (LYNXCHAR)0xDFFF
        #define false      0
        #define true       1
        
        void ConvertUTF32toUTF16(const LYNXCHAR* source, WORD* target, int len, ConversionFlags flags)
        {
            for (int i=0; i<len; i++)
            {
                LYNXCHAR ch;
                ch = *source++;
                
                ch -= halfBase;
                *target++ = (WORD) ((ch >> halfShift) + UNI_SUR_HIGH_START);
                *target++ = (WORD) ((ch & halfMask) + UNI_SUR_LOW_START);
                
                /*
                 if (ch <= UNI_MAX_BMP)
                {
                    if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
                    {
                        if (flags == strictConversion)
                        {
                            --source;
                            break;
                        }
                        else
                        {
                            *target++ = UNI_REPLACEMENT_CHAR;
                        }
                    }
                    else
                    {
                        *target++ = (WORD) ch;
                    }
                }
                else if (ch > UNI_MAX_LEGAL_UTF32)
                {
                    if (flags == strictConversion)
                    {
                    }
                    else
                    {
                        *target++ = UNI_REPLACEMENT_CHAR;
                    }
                }
                else
                {
                    if (0)//target + 1 >= targetEnd)
                    {
                        --source;
                        break;
                    }
                    ch -= halfBase;
                    *target++ = (WORD) ((ch >> halfShift) + UNI_SUR_HIGH_START);
                    *target++ = (WORD) ((ch & halfMask) + UNI_SUR_LOW_START);
                }*/
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
        void CFontLabel::vPrintf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
        {
            LYNXCHAR	Buffer[512], UTF16Buffer[512];
            va_list		argptr;
            NSString*   Text = nil;
            
            if (m_Label)
            {
                va_start(argptr,fmt);
                lynxVsnPrintf(Buffer, 510, fmt, argptr);
                va_end(argptr);
                
                //Text = [NSString alloc] init
                //NSString* UnicodeStr = @"インターネットに接続できません。接続が再開されるまでいくつかのゲーム機能は使用できなくなります。";
                //lynxNSString2cString(UTF16Buffer, UnicodeStr);
                
                //ConvertUTF32toUTF16(Buffer, (WORD*)UTF16Buffer, lynxStrLen(Buffer), lenientConversion);
                
                lynxcString2NSString(&Text, Buffer);
                
                //NSString* NewText = [[NSString alloc] initWithString:Text];
                //lynxcString2NSStringUTF16(&Text, UTF16Buffer);
                //lynxcStringUTF322UTF16(UTF16Buffer, Buffer);
                //lynxcString2NSStringUTF16(&Text, UTF16Buffer);
                NSLog(Text);
                m_Label.text = Text;
                if (lpc)
                {
                    m_Label.textColor = [UIColor colorWithRed:lpc->Red/255.0f green:lpc->Green/255.0f blue:lpc->Blue/255.0f alpha:lpc->Alpha/255.0f ];
                }
                else
                {
                    m_Label.textColor = [UIColor whiteColor];
                }
                
                LynxView* View = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(m_lpEngine->GetlpKernel())->GetlpView();
                float Scale = 1.0f/View.contentScaleFactor;
                m_Label.frame = CGRectMake(m_AlignmentRect.x1*Scale, m_AlignmentRect.y1*Scale, (m_AlignmentRect.x2-m_AlignmentRect.x1)*Scale, (m_AlignmentRect.y2-m_AlignmentRect.y1)*Scale);                
                
                if (m_AlignmentType & CSpriteContainer::H_ALIGN_CENTER)
                    m_Label.textAlignment =  NSTextAlignmentCenter;
                else if (m_AlignmentType & CSpriteContainer::H_ALIGN_LEFT)
                    m_Label.textAlignment =  NSTextAlignmentLeft;
                else if (m_AlignmentType & CSpriteContainer::H_ALIGN_RIGHT)
                    m_Label.textAlignment =  NSTextAlignmentRight;
                
                //[Text release];
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
        int CFontLabel::vPrintfLength(const LYNXCHAR *fmt, ...)
        {
            CReal Offset = 0.0f;
            
            LYNXCHAR	Buffer[512];
            va_list		argptr;
            int         Len;
            
            va_start(argptr,fmt);
            lynxVsnPrintf(Buffer, 510, fmt, argptr);
            va_end(argptr);
            
            if (m_Font)
            {
                Len = lynxStrLen(Buffer);
                Offset = Len * m_Font->m_Size;
            }
            
            return Offset;
        }
    }
}