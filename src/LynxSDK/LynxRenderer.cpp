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
//  Created by Owen Wu : 2006/11/07
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <LynxSDK/LynxRenderer.h>

namespace LynxSDK
{
	namespace Graphics
	{					
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRenderer::Open(LYNXWINDOW wnd, LPLYNXINITGRAPHICPARAM	lpcp)
		{
			LYNXERRORNO           ErrorNo;		
			LYNXTIMEDATA		  Time;								

			lynxOSGetTime(&Time);
			lynxSetLogf(GetClassName(), _T("Start at %d.%02d.%02d %02d:%02d:%02d\n"), Time.Year, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Second);				

			LYNX_SET_LOG(GetClassName(), _T("Set Renderer...\n"));	
			SetCurrent();	
			LYNX_SET_LOG(GetClassName(), _T("Munge FPCW...\n"));	
			lynxMungeFPCW();		
			LYNX_SET_LOG(GetClassName(), _T("Init Math System...\n"));	
			lynxInitMathSystem();
			LYNX_SET_LOG(GetClassName(), _T("Init Math System OK\n"));	

			LYNX_SET_LOG(GetClassName(), _T("Init I/O System...\n"));	
			lynxInitIOSystem();
			LYNX_SET_LOG(GetClassName(), _T("Init I/O System OK\n"));	

			LYNX_SET_LOG(GetClassName(), _T("Init File System...\n"));	
			if (lynxInitFileIOSystem())	
				LYNX_SET_LOG(GetClassName(), _T("Init File System OK\n"));	

			LYNX_SET_LOG(GetClassName(), _T("Init Memory System...\n"));		
			lynxInitMemSystem();
			LYNX_SET_LOG(GetClassName(), _T("Init Memory System OK\n"));		

			//if (m_SDK.bDisableMultiTask)
			//	lynxSetMultiTask(FALSE);			
			
			lynxSetSystemMainWindow(&SystemData, wnd);			
			SystemData.bFullScreen = lpcp->bFullScreen;
			SystemData3D.bHardware3D = lpcp->bHardware;			

			LYNX_SET_LOG(GetClassName(), _T("Init Graphics...\n"));	
			ErrorNo = lynxInitGraphic(&SystemData); 
			if (ErrorNo != LYNX_OK)
			{
				return LYNX_FALSE;
			}	    
			LYNX_SET_LOG(GetClassName(), _T("Init Graphics OK\n"));				

			if (SystemData.bFullScreen)
			{   
				LYNX_SET_LOG(GetClassName(), _T("Set Display Mode...\n"));	
				ErrorNo = lynxSetDisplayModeFullScreen(SystemData.MainWindow, &SystemData, &SystemData.CurrentVideoMode, lpcp->Width, lpcp->Height, lpcp->bpp);      
				//錯誤值為LYNX_OK代表無錯誤
				if (ErrorNo != LYNX_OK)
				{			
					return LYNX_FALSE;
				}
			}
			else
			{           
				LYNX_SET_LOG(GetClassName(), _T("Set Display Mode...\n"));	
				ErrorNo = lynxSetDisplayModeWindow(SystemData.MainWindow, &SystemData, &SystemData.CurrentVideoMode, lpcp->Width, lpcp->Height, lpcp->bpp);           
				//錯誤值為LYNX_OK代表無錯誤
				if (ErrorNo != LYNX_OK)
				{
					return LYNX_FALSE;
				}
			}	
			LYNX_SET_LOG(GetClassName(), _T("Set Display Mode OK\n"));								
			
			/*
			Color2.Red = Color2.Green = 0;
			Color2.Blue = 0;
			lynxClearLYNXBUFFER(SystemData.PrimaryBuffer, NULL, &Color2);
			Color2.Red = Color2.Green = 0;
			Color2.Blue = 0;
			lynxClearLYNXBUFFER(SystemData.BackBuffer, NULL, &Color2);						
			*/	
			
			LYNX_SET_LOG(GetClassName(), _T("Init 3D Graphic...\n"));				
			ErrorNo = lynxInit3DGraphic(&SystemData, &SystemData3D, lpcp);
			if (ErrorNo != LYNX_OK)
			{		
				return LYNX_FALSE;
			}			
			LYNX_SET_LOG(GetClassName(), _T("Init 3D Graphic OK\n"));		
				
			/*
			if (SystemData.Drivers.CurrentDriver >= 0)
				lynxPrintf(SystemData.PrimaryBuffer, 0, 0, NULL, "Use %s Driver", SystemData.Drivers.DriverInfo[SystemData.Drivers.CurrentDriver].Name);			
			lynxPrintf(SystemData.PrimaryBuffer, 0, 20, NULL, "Create %d-bits Zbuffer... OK", SystemData3D.DepthBuffer.bpp);				

			if (SystemData.CPUInfo.Feature & LYNX_CPU_FEATURE_SSE && SystemData.CPUInfo.OSSupport & LYNX_CPU_FEATURE_SSE)
			{		
				lynxPrintf(SystemData.PrimaryBuffer, 0, 40, NULL, "Use SSE");			
			}
			else if (SystemData.CPUInfo.Feature & LYNX_CPU_FEATURE_SSE2 && SystemData.CPUInfo.OSSupport & LYNX_CPU_FEATURE_SSE2)
			{		
				lynxPrintf(SystemData.PrimaryBuffer, 0, 40, NULL, "Use SSE2");			
			}
			else if (SystemData.CPUInfo.Feature & LYNX_CPU_FEATURE_3DNOW && SystemData.CPUInfo.OSSupport & LYNX_CPU_FEATURE_3DNOW)
			{	
				lynxPrintf(SystemData.PrimaryBuffer, 0, 40, NULL, "Use 3D Now!");			
			} 
			else
			{
				lynxPrintf(SystemData.PrimaryBuffer, 0, 40, NULL, "Use FPU");			
			}
			*/		
				
			lynxSetViewportRect(0, 0, lpcp->Width, lpcp->Height);		
			lynxSetClipRect(0, 0, lpcp->Width, lpcp->Height);				

			return LYNX_TRUE;
		}	

		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::Close()
		{
			LYNX_SET_LOG(GetClassName(), _T("Release 3D System Data...\n"));	
			lynxRelease3DSystemData(&SystemData3D);
			LYNX_SET_LOG(GetClassName(), _T("Release 3D System Data OK\n"));	

			LYNX_SET_LOG(GetClassName(), _T("Release System Data...\n"));	
			lynxReleaseSystemData(&SystemData);
			LYNX_SET_LOG(GetClassName(), _T("Release System Data OK\n"));	

			LYNX_SET_LOG(GetClassName(), _T("Restore FPCW...\n"));	
			lynxRestoreFPCW();				
			
		}		
	}
}