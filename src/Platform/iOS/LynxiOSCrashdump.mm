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
//  Created by Kasaki Tzeng : 2016/07/18
//  Last Update :
//
//###########################################################################
#include <LynxEngine_PCH.h>
#include <Platform/iOS/LynxiOSCrashdump.h>

#import <client/ios/Breakpad.h>
#import <client/ios/BreakpadController.h>
#import <client/mac/sender/uploader.h>
#import <common/mac/HTTPMultipartUpload.h>

#if 0
    #define NSLOG(...) NSLog(__VA_ARGS__)
#else
    #define NSLOG(...) do{} while(0)
#endif

#define URL_DEV_LOG @"http://tool.qubot.dev.qubitgames.com/qubot_tool/log/upload?access_token=kasaki|1468382042|6fcc685b2472acf171eb2b1a946d18ae8c12c268"

#pragma mark override breakpad-iOS method

@interface Uploader(BreakpadExtension)
- (void)report;
- (void)uploadData:(NSData *)data name:(NSString *)name;
@end
@implementation Uploader(BreakpadExtension)
- (void)report
{
    NSLOG(@"[Crashdump] ready to report dump log...");
    
    if (minidumpContents_)
    {
        NSLog(@"[Crashdump] report crashdump...");
        NSLOG(@"[Crashdump] minidump length = %lu", (unsigned long)[minidumpContents_ length]);
        
        NSString* Contents_ = [[NSString alloc] initWithData:minidumpContents_ encoding:NSASCIIStringEncoding];
        unsigned char* Buf = LYNXNEW unsigned char[[minidumpContents_ length]];
        [minidumpContents_ getBytes:Buf length:[minidumpContents_ length]];
        
        LYNXGUID guid;
        lynxOSMakeGUID(&guid);
        const LynxEngine::CString& dmp_file = LynxEngine::CString(_T("../data/i-")) + lynxOSGUIDToString(&guid) + LynxEngine::CString(_T(".dmp"));
        
        LynxEngine::CFileStream File;
        File.vOpen(dmp_file, LynxEngine::CStream::Write | LynxEngine::CStream::Binary);
        File.vWrite(Buf, sizeof(unsigned char), (int)[minidumpContents_ length]);
        File.vClose();
        
        LYNXDEL []Buf;
        
        glpKernel->vCrashDump(LYNX_TRUE, dmp_file);
    }
}

- (void)uploadData:(NSData *)data name:(NSString *)name
{
    NSLOG(@"[Crashdump] override Uploader::uploadData.");
}
@end

@interface HTTPMultipartUpload(BreakpadExtension)
- (NSData *)send:(NSError **)error;
@end
@implementation HTTPMultipartUpload(BreakpadExtension)
- (NSData *)send:(NSError **)error
{
    NSLOG(@"[Crashdump] override HTTPMultipartUpload::send.");
    
    NSData *data = nil;
    return nil;
}
@end

#pragma mark LynxEngine::iOS::CCrashDumpSystem

namespace LynxEngine
{
    namespace iOS
    {
        LYNXBOOL CCrashDumpSystem::vInit(const LYNXCHAR* dumpfilepath)
        {
            NSDictionary* config = [NSDictionary dictionaryWithObjectsAndKeys:
                                    @"v1", @"k1",
                                    @"v2", @"k2",
                                    @"v3", @"k3",
                                    nil];
            
            BreakpadController* _controller = [BreakpadController sharedInstance];
            [_controller updateConfiguration:config];
            [_controller setUploadInterval:30];
            [_controller start:YES];
            [_controller getCrashReportCount:^(int reportCount)
             {
                 NSLOG(@"[Crashdump] has %d dump file to report.", reportCount);
             }];
            [_controller setUploadingEnabled:YES];
            
            return LYNX_TRUE;
        }
    }
}
