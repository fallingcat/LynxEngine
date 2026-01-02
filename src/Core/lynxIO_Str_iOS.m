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
//  Created by Owen Wu : 2003/06/22
//  Last Update :
//
//###########################################################################
#include <LynxIO.h>
#include <LynxMem.h>
#import <UIKit/UIKit.h>

#ifdef _UNICODE
	#define STRING_ENCODING         NSUTF32LittleEndianStringEncoding
#else
	#define STRING_ENCODING         NSUTF8StringEncoding//NSASCIIStringEncoding
#endif
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxcString2NSString(NSString** out_str, const LYNXCHAR* str)
{
    //used ARC
    int Len = lynxStrLen(str);
    int Width = sizeof(LYNXCHAR);
    *out_str = [[NSString alloc] initWithBytes:str length:Len*Width encoding:STRING_ENCODING];
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxcString2NSStringUTF16(NSString** out_str, const LYNXCHAR* str)
{
    //used ARC
    int Len = lynxStrLen(str);
    int Width = sizeof(LYNXCHAR);
    *out_str = [[NSString alloc] initWithBytes:str length:Len*Width encoding:NSUTF16LittleEndianStringEncoding];
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxcString2NSStringUTF16BigEndian(NSString** out_str, const LYNXCHAR* str)
{
    //used ARC
    int Len = lynxStrLen(str);
    int Width = sizeof(LYNXCHAR);
    *out_str = [[NSString alloc] initWithBytes:str length:Len*Width encoding:NSUTF16BigEndianStringEncoding];
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxcString2NSStringUTF8(NSString** out_str, const LYNXCHAR* str)
{
    //used ARC
    int Len = lynxStrLen(str);
    int Width = sizeof(LYNXCHAR);
    *out_str = [[NSString alloc] initWithBytes:str length:Len*Width encoding:NSUTF8StringEncoding];//]NSUTF16LittleEndianStringEncoding];
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxNSString2cString(LYNXCHAR* out_str, const NSString* str)
{
    lynxStrCpy(out_str, (const LYNXCHAR *)[str cStringUsingEncoding:STRING_ENCODING]);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxNSString2cStringUTF16(LYNXCHAR* out_str, const NSString* str)
{
    lynxStrCpy(out_str, (const LYNXCHAR *)[str cStringUsingEncoding:NSUTF16LittleEndianStringEncoding]);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxcStringUTF162UTF32(LYNXCHAR* out_str, const LYNXCHAR* str)
{
#ifdef _UNICODE
    NSString* UTF16Str = nil;
    lynxcString2NSStringUTF16(&UTF16Str, str);
    lynxNSString2cString(out_str, UTF16Str);
    [UTF16Str release];
#else
    lynxStrCpy(out_str, str);
#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxcStringUTF322UTF16(LYNXCHAR* out_str, const LYNXCHAR* str)
{
#ifdef _UNICODE
    NSString* UTF32Str = nil;
    lynxcString2NSString(&UTF32Str, str);
    lynxNSString2cStringUTF16(out_str, UTF32Str);
    [UTF32Str release];
#else
    lynxStrCpy(out_str, str);
#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
int lynxiOSAtoi(const LYNXCHAR* str)
{
    NSString* Str = nil;
    int Number = 0;
    lynxcString2NSString(&Str, str);
    Number = [Str intValue];
    //[Str release];
    
    return Number;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
long lynxiOSAtol(const LYNXCHAR* str)
{
    NSString* Str = nil;
    long Number = 0;
    lynxcString2NSString(&Str, str);
    Number = [Str longLongValue];
    //[Str release];
    
    return Number;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
unsigned long lynxiOSAtoul(const LYNXCHAR* str)
{
    unsigned long Number = 0;
    Number = strtoul(str, NULL, 0);
    
    /*
     NSString* Str = nil;
    unsigned long Number = 0;
    lynxcString2NSString(&Str, str);
    Number = [Str longLongValue];
    [Str release];
    */
    
    return Number;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
float lynxiOSAtof(const LYNXCHAR* str)
{
    NSString* Str = nil;
    float Number = 0.0f;
    lynxcString2NSString(&Str, str);
    Number = [Str floatValue];
    //[Str release];
    
    return Number;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxiOSLog(const LYNXCHAR *str)
{
    NSString* Str = nil;
    lynxcString2NSString(&Str, str);
    NSLog(Str);
    //[Str release];
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO
 *  @brief 簞C
 *
 *	@param 簞C
 *	@param 簞C
 *	@param 簞C
 *	@return簞C
 */
//-------------------------------------------------------------------------------------------------------
void lynxiOSStrLower(LYNXCHAR *str)
{
    NSString* Str = nil;
    lynxcString2NSString(&Str, str);
    Str = [Str lowercaseString];
    lynxNSString2cString(str, Str);
}
