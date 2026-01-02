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

#include <LynxEngine_PCH.h>
#include <Platform/iOS/LynxiOSBitmapContext.h>

namespace LynxEngine 
{
	namespace iOS
	{
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBitmapContext::CBitmapContext(GraphicsSystem::CGraphicsSystem *pgs)
		: GraphicsSystem::CBitmapContext(pgs)
		{	
			m_Context = nil;
            m_Data = NULL;
            m_ParagraphStyle = NULL;
            m_Attributes = NULL;
            m_Shadow = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBitmapContext::~CBitmapContext(void)
		{
            if (m_Context)
            {
                CGContextRelease(m_Context);
                m_Context = nil;
            }

            if (m_Data)
            {
                LYNXDEL_ARRAY(m_Data);
                m_Data = NULL;
            }
        }
        //-------------------------------------------------------------------------------------------------------
        //
        //  說明:
        //-------------------------------------------------------------------------------------------------------
        void CBitmapContext::vReleaseContextData(void)
        {
            if (m_Context)
            {
                CGContextRelease(m_Context);
                m_Context = nil;
            }
            
            if (m_Data)
            {
                LYNXDEL_ARRAY(m_Data);
                m_Data = NULL;
            }
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CBitmapContext::vCreate(const int w, const int h, const int alinment)
		{
			const int BitsPerElement = 8;
			int SizeInBytes = w * h * 4;
			int TexturePitch = w * 4;
            
            if (m_Data)
            {
                LYNXDEL_ARRAY(m_Data);
                m_Data = NULL;
            }
            if (m_Context)
            {
                CGContextRelease(m_Context);
                m_Context = nil;
            }
            
			m_Data = LYNXNEW LYNXBYTE[SizeInBytes];
			lynxMemSet(m_Data, 0x00, SizeInBytes);
    			
			CGColorSpaceRef ColorSpace = CGColorSpaceCreateDeviceRGB();
			m_Context = CGBitmapContextCreate(m_Data, w, h, BitsPerElement, TexturePitch, ColorSpace, kCGImageAlphaPremultipliedLast);
            if (m_Context == nil)
            {
                if (ColorSpace)
                {
                    CGColorSpaceRelease(ColorSpace);
                    ColorSpace = nil;
                }
                return LYNX_FALSE;
            }
            
            if (ColorSpace)
            {
                CGColorSpaceRelease(ColorSpace);
                ColorSpace = nil;
            }

            LYNXGUID Guid;
            lynxOSMakeGUID(&Guid);
			m_lpEngine->CreateResource(m_Texture, TEXTURE);
			if (!m_Texture->vCreate(lynxOSGUIDToString(&Guid), w, h, 1, GraphicsSystem::CRenderer::TT_2D_TEXTURE, GraphicsSystem::CRenderer::FORMAT_A8R8G8B8))
				return LYNX_FALSE;
            
            m_Size.x = w;
            m_Size.y = h;
            
            m_TextAlignment = alinment;
            if (!m_ParagraphStyle)
            {
                /// Make a copy of the default paragraph style
                m_ParagraphStyle = [[NSMutableParagraphStyle alloc] init];
                //m_ParagraphStyle.lineHeightMultiple = 1.0f;
                //m_ParagraphStyle.lineSpacing = 2.0f;
                //m_ParagraphStyle.minimumLineHeight = 200.f;
                //m_ParagraphStyle.maximumLineHeight = 16.0f;
                //m_ParagraphStyle.lineBreakMode = NSLineBreakByWordWrapping;


                //m_ParagraphStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
                /// Set line break mode
                [m_ParagraphStyle setLineBreakMode:NSLineBreakByWordWrapping];
                /// Set text alignment
                if (alinment & CSpriteContainer::H_ALIGN_LEFT)
                    [m_ParagraphStyle setAlignment:NSTextAlignmentLeft];
                else if (alinment & CSpriteContainer::H_ALIGN_CENTER)
                    [m_ParagraphStyle setAlignment:NSTextAlignmentCenter];
                else if (alinment & CSpriteContainer::H_ALIGN_RIGHT)
                    [m_ParagraphStyle setAlignment:NSTextAlignmentRight];
            }
            
            if (!m_Shadow)
            {
                m_Shadow = [[NSShadow alloc] init];
                [m_Shadow setShadowBlurRadius:6.0];
                [m_Shadow setShadowOffset:CGSizeMake(2.0, 2.0)];
                [m_Shadow setShadowColor:[UIColor colorWithRed:0.0f green:0.0f blue:0.0f alpha:0.6f]];
            }
            
            if (!m_Attributes)
            {
                m_Attributes = [NSMutableDictionary dictionaryWithCapacity:10];	           
                [m_Attributes setObject:m_ParagraphStyle forKey:NSParagraphStyleAttributeName];
            }
            
            return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBitmapContext::vSetTextAlignment(const DWORD alignment)
		{
			m_TextAlignment = alignment;

			if (m_ParagraphStyle)
            {
                // Set text alignment
                if (alignment & CSpriteContainer::H_ALIGN_LEFT)
                    [m_ParagraphStyle setAlignment:NSTextAlignmentLeft];
                else if (alignment & CSpriteContainer::H_ALIGN_CENTER)
                    [m_ParagraphStyle setAlignment:NSTextAlignmentCenter];
                else if (alignment & CSpriteContainer::H_ALIGN_RIGHT)
                    [m_ParagraphStyle setAlignment:NSTextAlignmentRight];
            }
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBitmapContext::vPrint(const LYNXCHAR* text, const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth)		
		{
            CPlatformTrueTypeFont* Font = (CPlatformTrueTypeFont*)m_Font;
            NSString* Text = nil;
            lynxcString2NSString(&Text, text);
            
            LYNXCOLORRGBA Color = {255, 255, 255, 255};
            
            if (lpc)
                Color.PackedColor = lpc->PackedColor;
            
            float FontSize = Font->vGetCharacterHeight();
            float XOffset = 1;
            float YOffset = ((rect.y2 - rect.y1) - FontSize) * 0.5f;
            
            if (m_TextAlignment & CSpriteContainer::V_ALIGN_TOP)
                YOffset = 0;
            else if (m_TextAlignment & CSpriteContainer::V_ALIGN_CENTER)
                YOffset = ((rect.y2 - rect.y1) - FontSize) * 0.5f;
            else if (m_TextAlignment & CSpriteContainer::V_ALIGN_BOTTOM)
                YOffset = ((rect.y2 - rect.y1) - FontSize);
            
            CGContextClearRect(m_Context, CGRectMake(0, 0, m_Size.x, m_Size.y));
            int SizeInBytes = m_Size.x * m_Size.y * 4;
			lynxMemSet(m_Data, 0x00, SizeInBytes);
            //lynxMemSet(m_Data, 0x2f, SizeInBytes);
            
			UIGraphicsPushContext(m_Context);
    
			CGContextTranslateCTM(m_Context, 0.0f, 0.0f);
			CGContextScaleCTM(m_Context, 1.0f, 1.0f);
	
            //if ([Text respondsToSelector:@selector(drawInRect:withAttributes:)])
            if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0"))
            {
                //iOS 7 ---------------------------------------------------------------------------------------------------------
                [m_Attributes setObject:Font->GetFont() forKey:NSFontAttributeName];
                [m_Attributes removeObjectForKey:NSShadowAttributeName];                
				CGContextSetTextDrawingMode(m_Context, kCGTextFill);

				if (bshadow)
                {
					[m_Attributes setObject:[UIColor colorWithRed:0.0f green:0.0f blue:0.0f alpha:0.6f] forKey:NSForegroundColorAttributeName];
                    [Text drawInRect:CGRectMake(rect.x1+XOffset+2, rect.y1+YOffset+2, (rect.x2-rect.x1), (rect.y2-rect.y1)) withAttributes:m_Attributes];
                    [m_Attributes setObject:m_Shadow forKey:NSShadowAttributeName];
                }
                [m_Attributes setObject:[UIColor colorWithRed:Color.Red/255.0f green:Color.Green/255.0f blue:Color.Blue/255.0f alpha:Color.Alpha/255.0f] forKey:NSForegroundColorAttributeName];
                //[m_Attributes setObject:[UIColor colorWithRed:1.0f green:1.0f blue:1.0f alpha:1.0f] forKey:NSStrokeColorAttributeName];
                //[m_Attributes setObject:[NSNumber numberWithFloat:3.0] forKey:NSStrokeWidthAttributeName];               
                [Text drawInRect:CGRectMake(rect.x1+XOffset, rect.y1+YOffset, (rect.x2-rect.x1), (rect.y2-rect.y1)) withAttributes:m_Attributes];

				if (boutline)
				{
					CGContextSetLineWidth(m_Context, strokeWidth);
					CGContextSetLineJoin(m_Context, kCGLineJoinRound);
					CGContextSetTextDrawingMode(m_Context, kCGTextStroke);
					[m_Attributes setObject:[UIColor colorWithRed:lpstrokeColor->Red/255.0f green:lpstrokeColor->Green/255.0f blue:lpstrokeColor->Blue/255.0f alpha:lpstrokeColor->Alpha/255.0f] forKey:NSForegroundColorAttributeName];
					[Text drawInRect:CGRectMake(rect.x1+XOffset, rect.y1+YOffset, (rect.x2-rect.x1), (rect.y2-rect.y1)) withAttributes:m_Attributes];
				}
            }
            else
            {
                //iOS 6 -----------------------------------------------------------------------------------------------------------------
                CGContextSetAlpha(m_Context, 0.6);
                [[UIColor colorWithRed:0.0f green:0.0f blue:0.0f alpha:0.6f] set];
                [Text drawInRect:CGRectMake(rect.x1+XOffset+2, rect.y1+YOffset+2, (rect.x2-rect.x1), (rect.y2-rect.y1)) withFont:Font->GetFont() lineBreakMode:NSLineBreakByWordWrapping alignment:NSTextAlignmentCenter];
         
                CGContextSetAlpha(m_Context, 1.0);
                [[UIColor colorWithRed:Color.Red/255.0f green:Color.Green/255.0f blue:Color.Blue/255.0f alpha:Color.Alpha/255.0f] set];
                CGContextSetShadowWithColor(m_Context, CGSizeMake(2, 2), 10, (__bridge CGColorRef)([UIColor colorWithRed:0.0f green:0.0f blue:0.0f alpha:0.6f]));
                //[[UIColor initWithRed:CGFloat(lpc->Red/255.0f) green:CGFloat(lpc->Green/255.0f) blue:CGFloat(lpc->Blue/255.0f) alpha:CGFloat(lpc->Alpha/255.0f)] set];
                [Text drawInRect:CGRectMake(rect.x1+XOffset, rect.y1+YOffset, (rect.x2-rect.x1), (rect.y2-rect.y1)) withFont:Font->GetFont() lineBreakMode:NSLineBreakByWordWrapping alignment:NSTextAlignmentCenter];
            }
            
			UIGraphicsPopContext();
            CGContextFlush(m_Context);
            
			m_Texture->vUpdate(0, m_Data);

			/*
            if (m_Data)
            {
                LYNXDEL_ARRAY(m_Data);
                m_Data = NULL;
            }
            if (m_Context)
            {
                CGContextRelease(m_Context);
                m_Context = nil;
            }
            */
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		void CBitmapContext::vPrint(const LYNXCHAR* text, const int x, const int y, const LYNXCOLORRGBA* lpc, const LYNXBOOL bshadow, const LYNXBOOL boutline, const LYNXCOLORRGBA* lpstrokeColor, int strokeWidth)		
		{
            float XOffset = 1;

            CPlatformTrueTypeFont* Font = (CPlatformTrueTypeFont*)m_Font;
            NSString* Text = nil;
            lynxcString2NSString(&Text, text);
            
            LYNXCOLORRGBA Color = {255, 255, 255, 255};
            
            if (lpc)
                Color.PackedColor = lpc->PackedColor;
            
            CGContextClearRect(m_Context, CGRectMake(0, 0, m_Size.x, m_Size.y));
            int SizeInBytes = m_Size.x * m_Size.y * 4;
			lynxMemSet(m_Data, 0x1f, SizeInBytes);
            
			UIGraphicsPushContext(m_Context);
    
			CGContextTranslateCTM(m_Context, 0.0f, 0.0f);
			CGContextScaleCTM(m_Context, 1.0f, 1.0f);
	
            //if ([Text respondsToSelector:@selector(drawInRect:withAttributes:)])
            if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0"))
            {
                //iOS 7 ---------------------------------------------------------------------------------------------------------
                [m_Attributes setObject:Font->GetFont() forKey:NSFontAttributeName];
                [m_Attributes removeObjectForKey:NSShadowAttributeName];
				CGContextSetTextDrawingMode(m_Context, kCGTextFill);
                
				if (bshadow)
                {
					[m_Attributes setObject:[UIColor colorWithRed:0.0f green:0.0f blue:0.0f alpha:0.6f] forKey:NSForegroundColorAttributeName];
                    [Text drawInRect:CGRectMake(x+XOffset+2, y+2, m_Size.x-x, m_Size.y-y) withAttributes:m_Attributes];
                    [m_Attributes setObject:m_Shadow forKey:NSShadowAttributeName];
                }
                [m_Attributes setObject:[UIColor colorWithRed:Color.Red/255.0f green:Color.Green/255.0f blue:Color.Blue/255.0f alpha:Color.Alpha/255.0f] forKey:NSForegroundColorAttributeName];
                //[m_Attributes setObject:[UIColor colorWithRed:0.0f green:0.0f blue:0.0f alpha:1.0f] forKey:NSStrokeColorAttributeName];
                //[m_Attributes setObject:[NSNumber numberWithFloat:-5.0] forKey:NSStrokeWidthAttributeName];
                [Text drawInRect:CGRectMake(x+XOffset, y, m_Size.x-x, m_Size.y-y) withAttributes:m_Attributes];

				if (boutline)
				{
					CGContextSetLineWidth(m_Context, strokeWidth);
					CGContextSetLineJoin(m_Context, kCGLineJoinRound);
					CGContextSetTextDrawingMode(m_Context, kCGTextStroke);
					[m_Attributes setObject:[UIColor colorWithRed:lpstrokeColor->Red/255.0f green:lpstrokeColor->Green/255.0f blue:lpstrokeColor->Blue/255.0f alpha:lpstrokeColor->Alpha/255.0f] forKey:NSForegroundColorAttributeName];
					[Text drawInRect:CGRectMake(x+XOffset, y, m_Size.x-x, m_Size.y-y) withAttributes:m_Attributes];
				}
            }
            else
            {
                //iOS 6 ---------------------------------------------------------------------------------------------------------
                //CGContextSetAlpha(m_Context, 0.6);
                //[[UIColor blackColor] set];
                //[Text drawAtPoint:CGPointMake(x+2, y+2) withFont:Font->GetFont()];
                //[Text drawInRect:CGRectMake(x+2, y+2, m_Size.x-x, m_Size.y-y) withFont:Font->GetFont() lineBreakMode:UILineBreakModeWordWrap alignment:NSTextAlignmentCenter];
                
                CGContextSetAlpha(m_Context, 1.0);
                [[UIColor colorWithRed:Color.Red/255.0f green:Color.Green/255.0f blue:Color.Blue/255.0f alpha:Color.Alpha/255.0f] set];
                CGContextSetShadowWithColor(m_Context, CGSizeMake(8, 8), 10, (__bridge CGColorRef)([UIColor blackColor]));
                //[Text drawAtPoint:CGPointMake(x, y) withFont:Font->GetFont()];
                [Text drawInRect:CGRectMake(x+XOffset, y, m_Size.x-x, m_Size.y-y) withFont:Font->GetFont() lineBreakMode:NSLineBreakByWordWrapping alignment:NSTextAlignmentCenter];
            }
            
			UIGraphicsPopContext();
            CGContextFlush(m_Context);
			
			m_Texture->vUpdate(0, m_Data);

            /*
            if (m_Data)
            {
                LYNXDEL_ARRAY(m_Data);
                m_Data = NULL;
            }
            if (m_Context)
            {
                CGContextRelease(m_Context);
                m_Context = nil;
            }
            */
		}
	}						
}
