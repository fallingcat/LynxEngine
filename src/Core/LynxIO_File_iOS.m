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
    #define STRING_ENCODING         NSUTF8StringEncoding        
#else
    #define STRING_ENCODING         NSASCIIStringEncoding
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
void lynxiOSDeleteFile(const LYNXCHAR *name)
{
    int	Len, i, ci;
    LYNXCHAR Buffer[128], *lpStr, Name[32];
    
    lynxStrCpy(Buffer, name);    
    if (Buffer[0] == _T('.') && Buffer[1] == _T('.') && Buffer[2] == _T('/'))
        lpStr = &Buffer[3];
    else
        lpStr = Buffer;   
	
    NSURL* SearchURL = nil;
    NSString *NameKey[1] = {NSURLNameKey};        
    NSArray *Keys = [NSArray arrayWithObjects:NameKey count:1];        
    NSString* BasePath = [[NSString alloc] initWithCString:lpStr encoding:STRING_ENCODING];    
    NSString* BundleID = [[NSBundle mainBundle] bundleIdentifier];
    NSFileManager* FileManager = [NSFileManager defaultManager];    
    // Find the application support directory in the home directory.
    NSArray* AppSupportDir = [FileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
    if ([AppSupportDir count] > 0)
    {
        // Append the bundle ID to the URL for the
        // Application Support directory       
        SearchURL = [[AppSupportDir objectAtIndex:0] URLByAppendingPathComponent:BundleID];
        SearchURL = [SearchURL URLByAppendingPathComponent:BasePath];
        [FileManager removeItemAtURL:SearchURL error:NULL];
    }    
    //[BasePath release];
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
void lynxiOSRenameFile(const LYNXCHAR *new_name, const LYNXCHAR *old_name)
{
    int	Len, i, ci;
    LYNXCHAR Buffer[128], *lpStr, Name[32];
    
    lynxStrCpy(Buffer, old_name);    
    if (Buffer[0] == _T('.') && Buffer[1] == _T('.') && Buffer[2] == _T('/'))
        lpStr = &Buffer[3];
    else
        lpStr = Buffer;   
	
    NSURL* SearchURL = nil;
    NSURL* NewSearchURL = nil;
    NSString *NameKey[1] = {NSURLNameKey};        
    NSArray *Keys = [NSArray arrayWithObjects:NameKey count:1];        
    NSString* OldBasePath = [[NSString alloc] initWithCString:lpStr encoding:STRING_ENCODING];    
    NSString* BundleID = [[NSBundle mainBundle] bundleIdentifier];
    NSFileManager* FileManager = [NSFileManager defaultManager];    
    // Find the application support directory in the home directory.
    NSArray* AppSupportDir = [FileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
    if ([AppSupportDir count] > 0)
    {
		lynxStrCpy(Buffer, new_name);    
		if (Buffer[0] == _T('.') && Buffer[1] == _T('.') && Buffer[2] == _T('/'))
			lpStr = &Buffer[3];
		else
			lpStr = Buffer;   
		
		NSString* NewBasePath = [[NSString alloc] initWithCString:lpStr encoding:STRING_ENCODING];    
		
        // Append the bundle ID to the URL for the
        // Application Support directory       
        SearchURL = [[AppSupportDir objectAtIndex:0] URLByAppendingPathComponent:BundleID];
        SearchURL = [SearchURL URLByAppendingPathComponent:OldBasePath];
        
        NewSearchURL = [[AppSupportDir objectAtIndex:0] URLByAppendingPathComponent:BundleID];
        NewSearchURL = [NewSearchURL URLByAppendingPathComponent:NewBasePath];
        [FileManager moveItemAtURL:SearchURL toURL:NewSearchURL error:NULL];
        
        //[NewBasePath release];
    }    
    //[OldBasePath release];
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
void lynxiOSFindFiles(const LYNXCHAR *name, LPLYNXFINDDATA lpd)
{
    int	Len, i, ci;
    LYNXCHAR Buffer[128], *lpStr, Name[32];
    
    lynxStrCpy(Buffer, name);    
    if (Buffer[0] == _T('.') && Buffer[1] == _T('.') && Buffer[2] == _T('/'))
        lpStr = &Buffer[3];
    else
        lpStr = Buffer;   
	if (lpd->lpFounds)
        lynxFreeFounds(lpd);    
    lpd->NumFounds = 0;
    
    NSURL* SearchURL = nil;
    NSURL* SearchURL2 = nil;
    NSArray *Found = nil;
    NSArray *Found2 = nil;    
    NSString *NameKey[1] = {NSURLNameKey};        
    NSArray *Keys = [NSArray arrayWithObjects:NameKey count:1];        
    NSString* BasePath = [[NSString alloc] initWithCString:lpStr encoding:STRING_ENCODING];    
    NSString* BundleID = [[NSBundle mainBundle] bundleIdentifier];
    NSFileManager* FileManager = [NSFileManager defaultManager];    
    SearchURL = [[NSBundle mainBundle] bundleURL];
    SearchURL = [SearchURL URLByAppendingPathComponent:BasePath];
    SearchURL = [SearchURL URLByDeletingLastPathComponent];
    Found = [FileManager contentsOfDirectoryAtURL:SearchURL includingPropertiesForKeys:Keys options:NSDirectoryEnumerationSkipsHiddenFiles error:NULL];    
    // Find the application support directory in the home directory.
    NSArray* AppSupportDir = [FileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
    if ([AppSupportDir count] > 0)
    {
        // Append the bundle ID to the URL for the
        // Application Support directory       
        SearchURL2 = [[AppSupportDir objectAtIndex:0] URLByAppendingPathComponent:BundleID];
        SearchURL2 = [SearchURL2 URLByAppendingPathComponent:BasePath];
        SearchURL2 = [SearchURL2 URLByDeletingLastPathComponent];
        Found2 = [FileManager contentsOfDirectoryAtURL:SearchURL2 includingPropertiesForKeys:Keys options:NSDirectoryEnumerationSkipsHiddenFiles error:NULL];
    }
    
    NSArray* Lines = [BasePath componentsSeparatedByString: @"*"];
    
    lpd->NumFounds = [Found count] + [Found2 count];
    lpd->lpFounds = (LYNXCHAR **)LYNX_MALLOC(lpd->NumFounds*sizeof(LYNXCHAR *));
    
    bool bFound;
    lpd->NumFounds = 0;
    for (int i = 0; i < [Found count]; i++)
    {
        NSString* FoundStr = [[Found objectAtIndex:i] path];
        bFound = true;
        /*
		for (int j=0; j<[Lines count]; j++)
        {
            NSRange Range = [FoundStr rangeOfString:[Lines objectAtIndex:j] options:(NSCaseInsensitiveSearch)];
            if (Range.location == NSNotFound)
            {
                bFound = false;
                break;
            }
        }
		*/
        if (bFound)
        {
            NSURL* FoundURL = [Found objectAtIndex:i];
            NSString* Filename = [FoundURL lastPathComponent];  
            int Len = [Filename length];
            lpd->lpFounds[lpd->NumFounds] = (LYNXCHAR *)LYNX_MALLOC(sizeof(LYNXCHAR)*(Len+1));
            lynxStrCpy(lpd->lpFounds[lpd->NumFounds], [Filename cStringUsingEncoding:STRING_ENCODING]);        
        
            lpd->NumFounds++;
        }
    }
    for (int i = 0; i < [Found2 count]; i++)
    {
        /*
		NSString* FoundStr = [[Found2 objectAtIndex:i] path];
        bFound = true;
        for (int j=0; j<[Lines count]; j++)
        {
            NSRange Range = [FoundStr rangeOfString:[Lines objectAtIndex:j] options:(NSCaseInsensitiveSearch)];
            if (Range.location == NSNotFound)
            {
                bFound = false;
                break;
            }
        }
		*/
        if (bFound)
        {
            NSURL* FoundURL = [Found2 objectAtIndex:i];
            NSString* Filename = [FoundURL lastPathComponent];
            int Len = [Filename length];
            lpd->lpFounds[lpd->NumFounds] = (LYNXCHAR *)LYNX_MALLOC(sizeof(LYNXCHAR)*(Len+1));
            lynxStrCpy(lpd->lpFounds[lpd->NumFounds], [Filename cStringUsingEncoding:STRING_ENCODING]);            
        
            lpd->NumFounds++;
        }
    }
    
    //[BasePath release];
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
void lynxiOSCreateFolder(const LYNXCHAR *folder)
{
	LYNXCHAR Buffer[128], *lpStr, Name[32];
	NSURL* FullPath = nil;

	NSFileManager* FileManager = [NSFileManager defaultManager];            
    lynxStrCpy(Buffer, folder);    
    if (Buffer[0] == _T('.') && Buffer[1] == _T('.') && Buffer[2] == _T('/'))
        lpStr = &Buffer[3];
    else
        lpStr = Buffer;       
                    
	NSString* BasePath = [[NSString alloc] initWithCString:lpStr encoding:STRING_ENCODING];    
    NSString* BundleID = [[NSBundle mainBundle] bundleIdentifier];
	NSArray* AppSupportDir = [FileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
    if ([AppSupportDir count] > 0)
    {
        // Append the bundle ID to the URL for the Application Support directory   c
        FullPath = [[AppSupportDir objectAtIndex:0] URLByAppendingPathComponent:BundleID];
        FullPath = [FullPath URLByAppendingPathComponent:BasePath];
        FullPath = [FullPath URLByDeletingLastPathComponent];
        
        NSString *AbsPath = [FullPath absoluteString];
        BOOL IsDir = YES;
		if ([FileManager fileExistsAtPath:AbsPath isDirectory:&IsDir] != YES)
			[FileManager createDirectoryAtPath:AbsPath withIntermediateDirectories:YES attributes:nil error:NULL];
    }   
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
LYNXLONGLONG lynxiOSGetMaxPhysicalMemorySize()
{
    return [NSProcessInfo processInfo].physicalMemory;
}
