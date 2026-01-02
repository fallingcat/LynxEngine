//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Lynx SDK Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//  Compiled with Visual C++ 7.0. Best viewed with Tabs = 4.
//
//  Created by Owen Wu : 2003/07/06
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//
//###########################################################################

#include <LynxSDK.h>
#include <LynxTexSpr.h>

#define LYNX_SDK_VERSION	100
LPLYNXSDK					glpSDK = NULL;
static char					gSDKModuleName[] = "LynxSDK";
#ifdef __NON_COMMERCIAL__
	LYNXEXTERN	void	lynxShowLynxSplashScreen(LPLYNXSDK lpsdk);
#endif

extern 	LYNXWINDOW			gWindow;

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief Get LynxSDK version number。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXINLINE const WORD lynxGetSDKVersion(void) 
{ 
	return (LYNX_SDK_VERSION); 
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXWINDOW lynxSDKGetMainWindow()
{
	return gWindow;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInitModelList(LPLYNXSDK lpsdk)
{
	lynxInitList(&lpsdk->ModelList);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInitModelElement(LPLYNXMODELELEMENT lpme)
{
	lpme->Counter = 0;
	lpme->lpOriginalModel = NULL;
	strcpy(lpme->Name, "Null");
	lpme->lpModel = NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LPLYNXMODEL lynxSDKAddModel(LPLYNXSDK lpsdk, const char *filename, LPLYNXFILE lpfs, int offset, LYNXBOOL bbo)
{
	int								i, ModelIndex;
	LYNXBOOL						bSpawn, bRet;	
	//LPLYNXMODELLIST				lpM;
	LPLYNXLISTELEMENT				lpCList, lpList;
	LPLYNXMODELELEMENT				lpM, lpCM;
	char							Buffer[256];

	lynxAddElementToList(&lpsdk->ModelList, sizeof(LYNXMODELELEMENT));	
	lpCList = lpsdk->ModelList.lpCurrentElement;
	lpCM = (LPLYNXMODELELEMENT)(lpCList->lpObj);
	lynxInitModelElement(lpCM);
	lpCM->lpModel = (LPLYNXMODEL)LYNX_MALLOC(sizeof(LYNXMODEL));;

	bSpawn = FALSE;	
	lpList = lpsdk->ModelList.lpRootElement;
	while (lpList)
	{
		lpM = (LPLYNXMODELELEMENT)(lpList->lpObj);
		if (!lynxStriCmp(filename, lpM->Name))
		{
			bSpawn = TRUE;
			break;
		}
		lpList = lpList->lpNext;
	}	
	
	if (bSpawn)
	{			
		bRet = lynxLoadModel(lpCM->lpModel, filename, lpfs, offset, bbo);
		//lynxInstanceModel(lpCM->lpModel, lpM->lpModel);						
		lynxSetLogf(gSDKModuleName, "Add Instanced Model(%s) to List\n", lpCM->lpModel->Mesh.Obj3D.Name);			
		lpCM->lpOriginalModel = lpList;				
		((LPLYNXMODELELEMENT)(lpList->lpObj))->Counter++;
		strcpy(lpCM->Name, filename);		
		if (lpfs && offset)
		{			
			LYNX_SEEK_FILE(lpfs, offset, SEEK_CUR);
		}
	}
	else
	{	
		bRet = lynxLoadModel(lpCM->lpModel, filename, lpfs, offset, bbo);
		if (!bRet)
		{
			lynxDeleteElementFromList(&lpsdk->ModelList, lpCList);			
			return NULL;
		}		
		lynxSetLogf(gSDKModuleName, "Add Model(%s) to List\n", lpCM->lpModel->Mesh.Obj3D.Name);			
		lpCM->lpOriginalModel = NULL;	
		lpCM->Counter++;
		strcpy(lpCM->Name, filename);				
	}		
	
	//Yahoo!!
	return lpCM->lpModel;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSDKDeleteModel(LPLYNXSDK lpsdk, LPLYNXMODEL	lpm)
{
	LPLYNXLISTELEMENT		lpList, lpE, lpE2;	
	LPLYNXMODELELEMENT		lpM, lpM2, lpTemp;

	lpE = NULL;
	lpList = lpsdk->ModelList.lpRootElement;
	while (lpList)
	{
		lpM = (LPLYNXMODELELEMENT)(lpList->lpObj);
		if (lpm == lpM->lpModel)
		{
			lpE = lpList;
			break;
		}
		lpList = lpList->lpNext;
	}

	if (lpE)
	{	
		lpM = (LPLYNXMODELELEMENT)(lpE->lpObj);
		if (lpM->lpOriginalModel)
		{			
			((LPLYNXMODELELEMENT)(lpM->lpOriginalModel->lpObj))->Counter--;								
			if (((LPLYNXMODELELEMENT)(lpM->lpOriginalModel->lpObj))->Counter <= 0)
			{
				lpE2 = lpM->lpOriginalModel;
				lpM2 = (LPLYNXMODELELEMENT)(lpE2->lpObj);
				lynxReleaseModel(lpM2->lpModel);
				LYNX_FREE(lpM->lpModel);
				lynxDeleteElementFromList(&lpsdk->ModelList, lpE2);								
				lpM->lpOriginalModel = NULL;
			}											
			lynxReleaseModel(lpM->lpModel);
			lynxDeleteElementFromList(&lpsdk->ModelList, lpE);	
		}
		else
		{
			if (lpM->Counter == 1)
			{
				lynxReleaseModel(lpM->lpModel);
				LYNX_FREE(lpM->lpModel);
				lynxDeleteElementFromList(&lpsdk->ModelList, lpE);											
			}
			else
			{
				lpM->Counter--;
			}
		}		
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSDKReleaseModels(LPLYNXSDK lpsdk)
{	
	LPLYNXLISTELEMENT					lpM, lpTemp;	
	
	lpM = lpsdk->ModelList.lpRootElement;		
	while (lpM)
	{		
		lpTemp = lpM;
		lpM = lpM->lpNext;
		lynxSDKDeleteModel(lpsdk, ((LPLYNXMODELELEMENT)lpTemp->lpObj)->lpModel);		
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxSDKInstanceModel(LPLYNXSDK lpsdk, LPLYNXMODEL lpm, LPLYNXMODEL lpsrcm)
{
	int								i, ModelIndex;
	LYNXBOOL						bRet;	
	LPLYNXLISTELEMENT				lpList;
	LPLYNXMODELELEMENT				lpM, lpCM;

	lynxAddElementToList(&lpsdk->ModelList, sizeof(LYNXMODELELEMENT));
	lpCM = (LPLYNXMODELELEMENT)(lpsdk->ModelList.lpCurrentElement->lpObj);	
	lynxInitModelElement(lpCM);
	lpCM->lpModel = lpm;

	lpList = lpsdk->ModelList.lpRootElement;
	lpM = NULL;
	while (lpList)
	{
		lpM = (LPLYNXMODELELEMENT)(lpList->lpObj);
		if (!lynxStriCmp(lpsrcm->Mesh.Obj3D.Name, lpM->Name))
		{			
			break;
		}
		lpList = lpList->lpNext;
	}	
	
	if (!lpM)
		return FALSE;
	
	lynxInstanceModel(lpCM->lpModel, lpM->lpModel);
	lpCM->lpOriginalModel = lpList;				
	lpCM->Counter++;			

	//Yahoo!!
	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSDKShowError(LYNXERRORNO error)
{       
	char			Title[] = "LynxEngine Error";
	char			ErrorStr[1024];

	lynxGetErrorString(ErrorStr, error);
	#if __WIN32__ && (!_XBOX)
		#ifndef __QT__
			MessageBeep(MB_ICONEXCLAMATION);    
			MessageBox(NULL, ErrorStr, Title, MB_OK|MB_ICONERROR);
		#endif
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSDKSetupTitle(LPLYNXSDK lpsdk, const char *title)
{
	const char	*APIName;
	char		OSName[64];	
	WORD		SDKVer;		

	APIName = lynxGetGraphicAPIString();
	
	#ifdef __WIN32__
		strcpy(OSName, "Windows");
	#elif __LINUX__
		strcpy(OSName, "Linux");
    #elif __OS_X__
		strcpy(OSName, "OS X");
	#endif	
	SDKVer = lynxGetSDKVersion();
    if (title)
        strcpy(lpsdk->Title, title);
    else
        lynxSprintf(lpsdk->Title, " API::%s, OS::%s(Lynx SDK Ver %01d.%02d). Copyright (C) 2003.fallingCAT Studios.", APIName, OSName, SDKVer/100, SDKVer%100);

	lynxOSSetTitle(lpsdk->Renderer.SystemData.MainWindow, lpsdk->Title);
}	
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LPLYNXSDK lynxSetCurrentSDK(LPLYNXSDK lpsdk)
{
	LPLYNXSDK			lpSDK;

	lpSDK = glpSDK;
	glpSDK = lpsdk;
	lynxSetCurrentRenderer(&lpsdk->Renderer);	

	return lpSDK;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void ilynxRenderDefaultSetting(void)
{
	LYNXCOLORRGBA			Color;
	LYNXCOLORRGB			Color2;		

	lynxSetRenderMode(LYNX_RENDER_SOLID);
	lynxSetAlphaBlend(TRUE);
	lynxSetAlphaReference(LYNX_REAL(0));
	lynxSetAlphaTestFunc(LYNX_GREATER);

	lynxSetBlendMode(LYNX_ALPHA_BLEND);		
	lynxSetWrapMode(FALSE, FALSE);	
	lynxSetAntialias(FALSE);
	Color.s.Red = Color.s.Green = Color.s.Blue = Color.s.Alpha = 255;
	lynxSetConstantValue(&Color);
	lynxSetZBufferMode(LYNX_ZB_CW);				
	Color2.Red = 255;
	Color2.Green = 0;
	Color2.Blue = 255;	
	lynxSetHardwareCullMode(LYNX_CULL_CCW);
	lynxSetCullMode(LYNX_CULL_CCW);
	lynxSetPerspectiveCorrect(TRUE);	
	lynxSetUseFog(FALSE);
	lynxSetUseDither(FALSE);	
	lynxSetUseSpecular(FALSE);	
	lynxSetCurrentTextureBPP(32);	
	lynxSetVertexShader(LYNX_NULL_VERTEXSHADER);
	lynxSetPixelShader(LYNX_NULL_PIXELSHADER);
	lynxSetFixedVertexShader(LYNX_VERTEX_TYPE);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
//  說明:   
//		Init a SDK Manager
//-------------------------------------------------------------------------------------------------------
void		lynxInitSDK(LPLYNXSDK lpsdk)
{
	int							i;
	
	strcpy(lpsdk->Title, "Lynx SDK");	
	lpsdk->bRunning = TRUE;	
	for (i=0; i<LYNX_KBD_ARRAY_SIZE; i++)
		LYNXCLEARKEYSTATUS(lpsdk, i);		
	
	lpsdk->bWaitMessage = FALSE;
	lynxInitModelList(lpsdk);			
	lpsdk->NumRegisteredEvents = 0;
	lpsdk->argc = 0;
	lpsdk->argv = NULL;
	lpsdk->lpSysFont = NULL;	

	lynxInitSystemData(&lpsdk->Renderer.SystemData);  
	lynxInit3DSystemData(&lpsdk->Renderer.SystemData3D);		
	lynxInitRenderer(&lpsdk->Renderer);		

	lynxSDKInitTimer(lpsdk);
	lynxInitLog(&lpsdk->Log);	
	lynxSetCurrentLog(&lpsdk->Log);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXERRORNO	lynxOpenSDK(LPLYNXSDK lpsdk, LYNXWINDOW wnd, LPLYNXINITGRAPHICPARAM lpcp)
{	
	LYNXERRORNO           ErrorNo;		
	LYNXTIMEDATA		  Time;
	LYNXWINDOW			  Window;	
	
	glpSDK = lpsdk;	
	LYNX_OPEN_LOG("Lynx_Debug.log");		
	//LYNX_SET_LOG(NULL, "************************Lynx Engine Log******************************\n");	
	//LYNX_SET_LOG(NULL, "\n");	
	LYNX_SET_LOG(NULL, "    __                    ____            __\n");	
	LYNX_SET_LOG(NULL, "   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____\n");	
	LYNX_SET_LOG(NULL, "  / /__/ // /  _ \\ \\/ / / __/  _ \\/ __ \\/ /  _ \\/  _/\n");	
	LYNX_SET_LOG(NULL, "  \\___/__, /__//_/_/\\_\\ \\__/__//_/\\__, /_/__//_/\\__/\n");	
	LYNX_SET_LOG(NULL, "      /___/                       /___/ \n");	
	LYNX_SET_LOG(NULL, " \n");	
	LYNX_SET_LOG(NULL, "************************Lynx Engine Log******************************\n");	

	lynxOSGetTime(&Time);
	lynxSetLogf(gSDKModuleName, "Start at %d.%02d.%02d %02d:%02d:%02d\n", Time.Year, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Second);				

	LYNX_SET_LOG(gSDKModuleName, "Set Renderer...\n");	
	lynxSetCurrentRenderer(&lpsdk->Renderer);	
	LYNX_SET_LOG(gSDKModuleName, "Munge FPCW...\n");	
	lynxMungeFPCW();		
	LYNX_SET_LOG(gSDKModuleName, "Init Math System...\n");	
	lynxInitMathSystem();
	LYNX_SET_LOG(gSDKModuleName, "Init Math System OK\n");	

	LYNX_SET_LOG(gSDKModuleName, "Init I/O System...\n");	
	lynxInitIOSystem();
	LYNX_SET_LOG(gSDKModuleName, "Init I/O System OK\n");	

	LYNX_SET_LOG(gSDKModuleName, "Init File System...\n");	
	if (lynxInitFileIOSystem())	
		LYNX_SET_LOG(gSDKModuleName, "Init File System OK\n");	

	LYNX_SET_LOG(gSDKModuleName, "Init Memory System...\n");		
	lynxInitMemSystem();
	LYNX_SET_LOG(gSDKModuleName, "Init Memory System OK\n");		

	if (lpsdk->bDisableMultiTask)
		lynxSetMultiTask(FALSE);	
	
	Window = (wnd) ? wnd : gWindow;	
	
	lynxSetSystemMainWindow(&lpsdk->Renderer.SystemData, Window);			
	lpsdk->Renderer.SystemData.bFullScreen = lpcp->bFullScreen;
	lpsdk->Renderer.SystemData3D.bHardware3D = lpcp->bHardware;			

	LYNX_SET_LOG(gSDKModuleName, "Init Graphics...\n");	
	ErrorNo = lynxInitGraphic(&lpsdk->Renderer.SystemData); 
    if (ErrorNo != LYNX_OK)
    {
		lynxSDKShowError(ErrorNo);
        return ErrorNo;
    }	    
    LYNX_SET_LOG(gSDKModuleName, "Init Graphics OK\n");	
	
	if (lpsdk->Renderer.SystemData.bFullScreen)
    {   
		LYNX_SET_LOG(gSDKModuleName, "Set Display Mode...\n");	
		ErrorNo = lynxSetDisplayModeFullScreen(lpsdk->Renderer.SystemData.MainWindow, &lpsdk->Renderer.SystemData, &lpsdk->Renderer.SystemData.CurrentVideoMode, lpcp->Width, lpcp->Height, lpcp->bpp);      
        //錯誤值為LYNX_OK代表無錯誤
        if (ErrorNo != LYNX_OK)
        {			
			lynxSDKShowError(ErrorNo);
            return ErrorNo;
        }
    }
    else
    {           
		LYNX_SET_LOG(gSDKModuleName, "Set Display Mode...\n");	
        ErrorNo = lynxSetDisplayModeWindow(lpsdk->Renderer.SystemData.MainWindow, &lpsdk->Renderer.SystemData, &lpsdk->Renderer.SystemData.CurrentVideoMode, lpcp->Width, lpcp->Height, lpcp->bpp);           
        //錯誤值為LYNX_OK代表無錯誤
        if (ErrorNo != LYNX_OK)
        {
			lynxSDKShowError(ErrorNo);
            return ErrorNo;
        }
    }	
    LYNX_SET_LOG(gSDKModuleName, "Set Display Mode OK\n");								
	
	/*
	Color2.Red = Color2.Green = 0;
	Color2.Blue = 0;
	lynxClearLYNXBUFFER(&lpsdk->Renderer.SystemData.PrimaryBuffer, NULL, &Color2);
	Color2.Red = Color2.Green = 0;
	Color2.Blue = 0;
	lynxClearLYNXBUFFER(&lpsdk->Renderer.SystemData.BackBuffer, NULL, &Color2);						
	*/	
	
	LYNX_SET_LOG(gSDKModuleName, "Init 3D Graphic...\n");	
	ErrorNo = lynxInit3DGraphic(&lpsdk->Renderer.SystemData, &lpsdk->Renderer.SystemData3D, lpcp);
	if (ErrorNo != LYNX_OK)
	{		
		lynxSDKShowError(ErrorNo);
		return ErrorNo;
	}
	lynxFindAllDrivers(&lpsdk->Renderer.SystemData);		
	LYNX_SET_LOG(gSDKModuleName, "Init 3D Graphic OK\n");		
		
	/*
	if (lpsdk->Renderer.SystemData.Drivers.CurrentDriver >= 0)
		lynxPrintf(&lpsdk->Renderer.SystemData.PrimaryBuffer, 0, 0, NULL, "Use %s Driver", lpsdk->Renderer.SystemData.Drivers.DriverInfo[lpsdk->Renderer.SystemData.Drivers.CurrentDriver].Name);			
	lynxPrintf(&lpsdk->Renderer.SystemData.PrimaryBuffer, 0, 20, NULL, "Create %d-bits Zbuffer... OK", lpsdk->Renderer.SystemData3D.ZBuffer.bpp);				

	if (lpsdk->Renderer.SystemData.CPUInfo.Feature & LYNX_CPU_FEATURE_SSE && lpsdk->Renderer.SystemData.CPUInfo.OSSupport & LYNX_CPU_FEATURE_SSE)
	{		
		lynxPrintf(&lpsdk->Renderer.SystemData.PrimaryBuffer, 0, 40, NULL, "Use SSE");			
	}
	else if (lpsdk->Renderer.SystemData.CPUInfo.Feature & LYNX_CPU_FEATURE_SSE2 && lpsdk->Renderer.SystemData.CPUInfo.OSSupport & LYNX_CPU_FEATURE_SSE2)
	{		
		lynxPrintf(&lpsdk->Renderer.SystemData.PrimaryBuffer, 0, 40, NULL, "Use SSE2");			
	}
	else if (lpsdk->Renderer.SystemData.CPUInfo.Feature & LYNX_CPU_FEATURE_3DNOW && lpsdk->Renderer.SystemData.CPUInfo.OSSupport & LYNX_CPU_FEATURE_3DNOW)
	{	
		lynxPrintf(&lpsdk->Renderer.SystemData.PrimaryBuffer, 0, 40, NULL, "Use 3D Now!");			
	} 
	else
	{
		lynxPrintf(&lpsdk->Renderer.SystemData.PrimaryBuffer, 0, 40, NULL, "Use FPU");			
	}
	*/

	lpsdk->bRunning = TRUE;			
		
	lynxSetViewportRect(0, 0, lpcp->Width, lpcp->Height);		
	lynxSetClipRect(0, 0, lpcp->Width, lpcp->Height);	
	ilynxRenderDefaultSetting();			
	
	//Setup network
	/*
	if (lpsdk->bNetwork)
	{
		lynxnInitNetSystem(&lpsdk->NetRender.SystemData);
		lynxnFindAllNetServices(&lpsdk->NetRender.SystemData);
	}

	//Setup sound
	if (lpsdk->bSound)
	{
		lynxsInitSoundRender.SystemData(&lpsdk->SoundRender.SystemData);
		lynxsInitSoundSystem(&lpsdk->SoundRender.SystemData);
	}
	*/
	
	LYNX_SET_LOG(gSDKModuleName, "Init Model System...\n");							
	lynxInitModelSystem();
	LYNX_SET_LOG(gSDKModuleName, "Init Model System OK\n");						
	
	#ifdef __NON_COMMERCIAL__		
		lynxShowLynxSplashScreen(lpsdk);

		lynxSetViewportRect(0, 0, lpcp->Width, lpcp->Height);	
		lynxSetClipRect(0, 0, lpcp->Width, lpcp->Height);	
		ilynxRenderDefaultSetting();
	#endif	

	//Yahoo!!
	return LYNX_OK;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
//		Show backbuffer on screen
//-------------------------------------------------------------------------------------------------------
LYNXERRORNO	lynxSDKShowBackBuffer(LPLYNXSDK lpsdk)
{	
	return lynxFlip(&lpsdk->Renderer.SystemData.PrimaryBuffer, &lpsdk->Renderer.SystemData.BackBuffer, 
				  lpsdk->Renderer.SystemData.Origin.x, lpsdk->Renderer.SystemData.Origin.y, 
				  lpsdk->Renderer.SystemData.BackBuffer.Width, lpsdk->Renderer.SystemData.BackBuffer.Height, 
				  lpsdk->Renderer.SystemData.bFullScreen);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXERRORNO	lynxCloseSDK(LPLYNXSDK lpsdk)
{
	int									i;
	LPLYNXLISTELEMENT					lpM, lpTemp;	

	LYNX_SET_LOG(NULL, "\n\n");	
	lynxSetMultiTask(TRUE);
	if (lpsdk->lpSysFont)
		LYNX_FREE(lpsdk->lpSysFont);
	
	LYNX_SET_LOG(gSDKModuleName, "Release Model List...\n");		
	lynxSDKReleaseModels(lpsdk);			
	LYNX_SET_LOG(gSDKModuleName, "Release Model List OK\n");		

	//if ( lpsdk->bSound )
	//	lynxsReleaseSoundSystem(&lpsdk->SoundRender.SystemData);

	//if ( lpsdk->bNetwork )
	//	lynxnReleaseNetSystem(&lpsdk->NetRender.SystemData);
	//			

	LYNX_SET_LOG(gSDKModuleName, "Release 3D System Data...\n");	
	lynxRelease3DSystemData(&lpsdk->Renderer.SystemData3D);
	LYNX_SET_LOG(gSDKModuleName, "Release 3D System Data OK\n");	

	LYNX_SET_LOG(gSDKModuleName, "Release System Data...\n");	
	lynxReleaseSystemData(&lpsdk->Renderer.SystemData);
	LYNX_SET_LOG(gSDKModuleName, "Release System Data OK\n");	

	LYNX_SET_LOG(gSDKModuleName, "Restore FPCW...\n");	
	lynxRestoreFPCW();	

	if (lynxGetAllocMemSize() > 0)
	{
		lynxClearBadMemBlock();
		//MessageBeep(MB_ICONEXCLAMATION);    
		//MessageBox(lpsdk->Renderer.SystemData.MainWindow, "記憶體未完全釋放", "警告", MB_OK | MB_ICONERROR);
	}
	lynxClearBadFileBlock();			
	LYNX_SET_LOG(NULL, "************************Lynx Engine Log******************************\n");					

	LYNX_CLOSE_LOG();		

	#ifdef __WIN32__	
		#if _WINDOWS || (_LIB && !_XBOX)
			#ifndef __QT__
				PostMessage(lpsdk->Renderer.SystemData.MainWindow, WM_CLOSE, 0L, 0L);
				DestroyWindow(lpsdk->Renderer.SystemData.MainWindow);						
			#endif
		#endif		
	#endif	

	//Yahoo!!
	return LYNX_OK;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSDKRegisterEvent(LPLYNXSDK lpsdk, DWORD id)
{
	lpsdk->RegisteredEvent[lpsdk->NumRegisteredEvents].ID = id;
	lpsdk->NumRegisteredEvents++;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxSDKGetEvent(LPLYNXSDK lpsdk)
{
	#ifdef WIN32
		MSG							Msg;
		DWORD						Res;
		static int					i = 0;
		int							Type, SI;
	#elif __XWIN__
		XEvent						Event;	
	#elif __OS_X__
        EventRecord					Event;
        short						Part;
        Boolean						Hit;
        char						Key;
        Rect						TempRect;
        WindowRef					WhichWindow;
        long						MenuResult;
        short						MenuID;		/* the resource ID of the selected menu */
        short						MenuItem;	/* the item number of the selected menu */
	#endif

	glpSDK = lpsdk;

	#ifdef  __WIN32__		
		/*----temp-------
		if (!lpsdk->bSoundLoopThread)
			lynxsSoundSystemLoop();
		*/		
		#ifndef __QT__
			if (lpsdk->bWaitMessage)
			{
				if (GetMessage(&Msg, NULL, 0, 0))
				{  			
					TranslateMessage(&Msg);						
					DispatchMessage(&Msg);
				}
				else
					return FALSE;
			}
			else
			{
				if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) ) 
				{  			
					TranslateMessage(&Msg);						
					DispatchMessage(&Msg);
				}
				else
					return FALSE;
			}				
		#endif
	#elif  __WINCE__		
		if (lpsdk->bWaitMessage)
		{
			if (GetMessage(&Msg, NULL, 0, 0))
			{  			
				TranslateMessage(&Msg);						
				DispatchMessage(&Msg);
			}
			else
				return FALSE;
		}
		else
		{
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) ) 
			{  			
				TranslateMessage(&Msg);						
				DispatchMessage(&Msg);
			}
			else
				return FALSE;
		}				
	#elif __XWIN__
		//do {
				if (XEventsQueued(lpsdk->Renderer.SystemData.Disp, QueuedAfterFlush))
				{
					XNextEvent(lpsdk->Renderer.SystemData.Disp,  &Event);
      			    switch(Event.type)
					{
						case ButtonPress:
				       		break;

						case KeyPress:
							lpsdk->KBD[(XLookupKeysym(&Event.xkey, 0))&0xff] = TRUE; 
							break;

						case KeyRelease:
							lpsdk->KBD[(XLookupKeysym(&Event.xkey, 0))&0xff] = FALSE; 
							break;
					}
				}
		//}while (XPending(lpsdk->Renderer.SystemData.BackBuffer.Disp));  //loop to compress events/
	#elif __OS_X__
 		if (!WaitNextEvent(everyEvent, &Event, 0, nil))
			return FALSE;
            
        switch (Event.what) 
        {
            case mouseDown:
                Part = FindWindow(Event.where, &WhichWindow);
                switch (Part)
                {
                    case inMenuBar:  /* process a moused menu command */
                        MenuResult = MenuSelect(Event.where);	
                        MenuID = HiWord(MenuResult);    /* use macros to get item & menu number */
                        MenuItem = LoWord(MenuResult);
	
                        switch (MenuID) 
                        {
                            case 128:
                                switch (MenuItem) 
                                {
                                    case 11://iQuit:
                                        //ExitToShell();
                                        lpsdk->bRunning = FALSE;
                                        break;
				
                                    default:
                                        break;
                                }
                                break;
        
                        }
                        HiliteMenu(0);	/* unhighlight what MenuSelect (or MenuKey) hilited */
                        break;
                    
                    case inSysWindow:
                        break;
                
                    case inContent:
                        if (WhichWindow != FrontWindow()) 
                            SelectWindow(WhichWindow);
                        break;
                
                    case inDrag:	/* pass screenBits.bounds */
                        GetRegionBounds(GetGrayRgn(), &TempRect);
                        DragWindow(WhichWindow, Event.where, &TempRect);
                        break;
                    
                    case inGrow:
                        break;

                    
                    case inGoAway:
                        if (TrackGoAway(WhichWindow, Event.where))
                        {
                            DisposeWindow(WhichWindow);
                            lpsdk->bRunning = FALSE;
                        }
                        break;
                    
                    case inZoomIn:
                    case inZoomOut:
                        Hit = TrackBox(WhichWindow, Event.where, Part);
                        if (Hit) 
                        {
                            SetPort(GetWindowPort(WhichWindow));   // window must be current port
                            EraseRect(GetWindowPortBounds(WhichWindow, &TempRect));   // inval/erase because of ZoomWindow bug
                            ZoomWindow(WhichWindow, Part, true);
                            InvalWindowRect(WhichWindow, GetWindowPortBounds(WhichWindow, &TempRect));	
                        }
                        break;
                }
                break;
                                        
            case kHighLevelEvent:
                AEProcessAppleEvent(&Event);
                break;
		
            case diskEvt:
                break;
        }

	#endif	

	//Yahoo!!
	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
/*
LYNXBOOL	lynxLoadLensFlare(void)
{
	float					lpW[11], lpH[11];
	float					lpU[11*4], lpV[11*4];
	LYNXCOLORRGBA				lpC[11];
	float					TW, TH;	
	int						i;

	lynxInitTexSprite2(&gLensFlare.FlareSprite);
	gLensFlare.NumFlares = 11;
	gLensFlare.lpFlareBrightFactor = (float *)LYNX_MALLOC(sizeof(float) * gLensFlare.NumFlares);
	gLensFlare.lpFlareOffset = (float *)LYNX_MALLOC(sizeof(float) * gLensFlare.NumFlares);

	gLensFlare.lpFlareOffset[0] = 1.1f;
	gLensFlare.lpFlareOffset[1] = 0.9f;
	gLensFlare.lpFlareOffset[2] = 0.7f;
	gLensFlare.lpFlareOffset[3] = 0.6f;
	gLensFlare.lpFlareOffset[4] = 0.5f;
	gLensFlare.lpFlareOffset[5] = 0.2f;
	gLensFlare.lpFlareOffset[6] = 0.0f;
	gLensFlare.lpFlareOffset[7] = -0.25f;
	gLensFlare.lpFlareOffset[8] = -0.4;
	gLensFlare.lpFlareOffset[9] = -0.6;
	gLensFlare.lpFlareOffset[10] = -1.0f;

	lpW[0] = 30.0f; lpH[0] = 30.0f;
	lpW[1] = 60.0f; lpH[1] = 60.0f;
	lpW[2] = 80.0f; lpH[2] = 80.0f;
	lpW[3] = 60.0f; lpH[3] = 60.0f;
	lpW[4] = 30.0f; lpH[4] = 30.0f;
	lpW[5] = 30.0f; lpH[5] = 30.0f;
	lpW[6] = 80.0f; lpH[6] = 80.0f;
	lpW[7] = 70.0f; lpH[7] = 70.0f;
	lpW[8] = 40.0f; lpH[8] = 40.0f;
	lpW[9] = 90.0f; lpH[9] = 90.0f;
	lpW[10] = 160.0f; lpH[10] = 160.0f;

	TW = 255.0f;
	TH = 255.0f;
	lpU[0*4+0] = (64.0f-1.0f)/TW;			lpV[0*4+0] = (64.0f-1.0f)/TH; 
	lpU[0*4+1] = (64.0f+64.0f-1.0f)/TW;		lpV[0*4+1] = (64.0f-1.0f)/TH; 
	lpU[0*4+2] = (64.0f-1.0f)/TW;			lpV[0*4+2] = (64.0f+64.0f-1.0f)/TH;  
	lpU[0*4+3] = (64.0f+64.0f-1.0f)/TW;		lpV[0*4+3] = (64.0f+64.0f-1.0f)/TH;  

	lpU[1*4+0] = (0.0f)/TW;					lpV[1*4+0] = (64.0f-1.0f)/TH; 
	lpU[1*4+1] = (64.0f-1.0f)/TW;			lpV[1*4+1] = (64.0f-1.0f)/TH; 
	lpU[1*4+2] = (0.0f)/TW;					lpV[1*4+2] = (64.0f+64.0f-1.0f)/TH; 
	lpU[1*4+3] = (64.0f-1.0f)/TW;			lpV[1*4+3] = (64.0f+64.0f-1.0f)/TH;

	lpU[2*4+0] = (64.0f-1.0f)/TW;			lpV[2*4+0] = (64.0f-1.0f)/TH; 
	lpU[2*4+1] = (64.0f+64.0f-1.0f)/TW;		lpV[2*4+1] = (64.0f-1.0f)/TH;  
	lpU[2*4+2] = (64.0f-1.0f)/TW;			lpV[2*4+2] = (64.0f+64.0f-1.0f)/TH;  
	lpU[2*4+3] = (64.0f+64.0f-1.0f)/TW;		lpV[2*4+3] = (64.0f+64.0f-1.0f)/TH;

	lpU[3*4+0] = (0.0f)/TW;					lpV[3*4+0] = (0.0f)/TH; 
	lpU[3*4+1] = (64.0f-1.0f)/TW;			lpV[3*4+1] = (0.0f)/TH;  
	lpU[3*4+2] = (0.0f)/TW;					lpV[3*4+2] = (64.0f-1.0f)/TH; 
	lpU[3*4+3] = (64.0f-1.0f)/TW;			lpV[3*4+3] = (64.0f-1.0f)/TH; 

	lpU[4*4+0] = lpU[2*4+0];				lpV[4*4+0] = lpV[2*4+0]; 
	lpU[4*4+1] = lpU[2*4+1];				lpV[4*4+1] = lpV[2*4+1];  
	lpU[4*4+2] = lpU[2*4+2];				lpV[4*4+2] = lpV[2*4+2]; 
	lpU[4*4+3] = lpU[2*4+3];				lpV[4*4+3] = lpV[2*4+3]; 

	lpU[5*4+0] = lpU[0*4+0];				lpV[5*4+0] = lpV[0*4+0]; 
	lpU[5*4+1] = lpU[0*4+1];				lpV[5*4+1] = lpV[0*4+1];  
	lpU[5*4+2] = lpU[0*4+2];				lpV[5*4+2] = lpV[0*4+2]; 
	lpU[5*4+3] = lpU[0*4+3];				lpV[5*4+3] = lpV[0*4+3]; 

	lpU[6*4+0] = (64.0f-1.0f)/TW;			lpV[6*4+0] = (0.0f)/TH; 
	lpU[6*4+1] = (64.0f+64.0f-1.0f)/TW;		lpV[6*4+1] = (0.0f)/TH;  
	lpU[6*4+2] = (64.0f-1.0f)/TW;			lpV[6*4+2] = (64.0f-1.0f)/TH; 
	lpU[6*4+3] = (64.0f+64.0f-1.0f)/TW;		lpV[6*4+3] = (64.0f-1.0f)/TH; 

	lpU[7*4+0] = lpU[3*4+0];				lpV[7*4+0] = lpV[3*4+0]; 
	lpU[7*4+1] = lpU[3*4+1];				lpV[7*4+1] = lpV[3*4+1];  
	lpU[7*4+2] = lpU[3*4+2];				lpV[7*4+2] = lpV[3*4+2]; 
	lpU[7*4+3] = lpU[3*4+3];				lpV[7*4+3] = lpV[3*4+3]; 

	lpU[8*4+0] = (0.0f)/TW;					lpV[8*4+0] = (0.0f)/TH; 
	lpU[8*4+1] = (64.0f-1.0f)/TW;			lpV[8*4+1] = (0.0f)/TH; 
	lpU[8*4+2] = (0.0f)/TW;					lpV[8*4+2] = (64.0f-1.0f)/TH; 
	lpU[8*4+3] = (64.0f-1.0f)/TW;			lpV[8*4+3] = (64.0f-1.0f)/TH;

	lpU[9*4+0] = (0.0f)/TW;					lpV[9*4+0] = (64.0f-1.0f)/TH; 
	lpU[9*4+1] = (64.0f-1.0f)/TW;			lpV[9*4+1] = (64.0f-1.0f)/TH;  
	lpU[9*4+2] = (0.0f)/TW;					lpV[9*4+2] = (64.0f+64.0f-1.0f)/TH; 
	lpU[9*4+3] = (64.0f-1.0f)/TW;			lpV[9*4+3] = (64.0f+64.0f-1.0f)/TH; 

	lpU[10*4+0] = (128.0f-1.0f)/TW;			lpV[10*4+0] = (0.0f)/TH; 
	lpU[10*4+1] = (128.0f+128.0f-1.0f)/TW;	lpV[10*4+1] = (0.0f)/TH;  
	lpU[10*4+2] = (128.0f-1.0f)/TW;			lpV[10*4+2] = (128.0f-1.0f)/TH;  
	lpU[10*4+3] = (128.0f+128.0f-1.0f)/TW;	lpV[10*4+3] = (128.0f-1.0f)/TH; 

	lpC[0].Red = 210;		lpC[0].Green = 85;		lpC[0].Blue = 0;		lpC[0].Alpha = 255;
	lpC[1].Red = 190;		lpC[1].Green = 240;		lpC[1].Blue = 80;		lpC[1].Alpha = 255;
	lpC[2].Red = 255;		lpC[2].Green = 80;		lpC[2].Blue = 120;		lpC[2].Alpha = 255;
	lpC[3].Red = 0;			lpC[3].Green = 62;		lpC[3].Blue = 236;		lpC[3].Alpha = 255;
	lpC[4].Red = 0;			lpC[4].Green = 62;		lpC[4].Blue = 236;		lpC[4].Alpha = 255;
	lpC[5].Red = 210;		lpC[5].Green = 85;		lpC[5].Blue = 0;		lpC[5].Alpha = 255;
	lpC[6].Red = 0;			lpC[6].Green = 200;		lpC[6].Blue = 110;		lpC[6].Alpha = 255;
	lpC[7].Red = 210;		lpC[7].Green = 85;		lpC[7].Blue = 0;		lpC[7].Alpha = 255;
	lpC[8].Red = 0;			lpC[8].Green = 200;		lpC[8].Blue = 110;		lpC[8].Alpha = 255;
	lpC[9].Red = 190;		lpC[9].Green = 240;		lpC[9].Blue = 80;		lpC[9].Alpha = 255;
	lpC[10].Red = 255;		lpC[10].Green = 255;	lpC[10].Blue = 255;		lpC[10].Alpha = 255;	

	for (i=0; i<gLensFlare.NumFlares; i++)
	{
		switch (i)
		{
			case 0:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.9f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.9f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.9f);
				break;

			case 1:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.5f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.5f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.5f);
				break;

			case 2:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.7f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.7f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.7f);
				break;

			case 3:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.8f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.8f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.8f);
				break;

			case 4:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.7f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.7f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.7f);
				break;

			case 5:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.6f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.6f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.6f);
				break;

			case 6:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.8f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.8f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.8f);
				break;
				
			case 7:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.9f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.9f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.9f);
				break;

			case 8:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.8f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.8f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.8f);
				break;

			case 9:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*0.6f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*0.6f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*0.6f);
				break;

			case 10:
				lpC[i].Red = LYNX_ROUND(lpC[i].Red*1.0f);
				lpC[i].Green = LYNX_ROUND(lpC[i].Green*1.0f);
				lpC[i].Blue = LYNX_ROUND(lpC[i].Blue*1.0f);
				break;
		}
		
	}	
	lynxLoadTexSprites2(&gLensFlare.FlareSprite, "len_fx.bmp", 11, lpW, lpH, lpU, lpV, lpC, LYNX_NORMAL_TEXTURE);

	return TRUE;
}
*/
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
/*
void	lynxMoveAndDrawLensFlare(LPLYNXVECTOR3D lplitpos, LPLYNXCAMERA lpc, float skip)
{
	static LYNXVECTOR3D	ViewDir = {0, 0, 1};
	LYNXVECTOR3D			LightDir, NewLightPos, CenterPos, AxisDir, Pos;
	int					i;
	float				f;
	BYTE				R, G, B;
	LPLYNXRENDERSTATUS	lpRS;
	DWORD				SW, SH;

	lynxSetBlendMode(LYNX_ACCUMULATE_BLEND);
	lpRS = lynxGetlpRenderStatus();
	SW = lpRS->VideoMode.Width >> 1;
	SH = lpRS->VideoMode.Height >> 1;

	//lynxPointXMatrix(&LightDir, lplitpos, &lpc->ViewMatrix);	
	

	//Test
	LightDir.x = (FLOAT)(sin(skip)*20 );
	LightDir.y = 20.0f;//(FLOAT)( 5.0f+10.0f*sin(fTimeKey*0.678) );
	LightDir.z = (FLOAT)(cos(skip)*20 );
	//Test

	lynxNormalise(&LightDir, 3);	


	// Compute the vector and center point for the lens flare axis
    f = LYNX_DOT_PRODUCT(LightDir, ViewDir);

	NewLightPos.x = 1.0f/f * LightDir.x * 1.01f;
	NewLightPos.y = 1.0f/f * LightDir.y * 1.01f;
	NewLightPos.z = 1.0f/f * LightDir.z * 1.01f;

	CenterPos.x = ViewDir.x*1.01f;
	CenterPos.y = ViewDir.y*1.01f;
	CenterPos.z = ViewDir.z*1.01f;

    AxisDir.x = NewLightPos.x - CenterPos.x;
	AxisDir.y = NewLightPos.y - CenterPos.y;
	AxisDir.z = NewLightPos.z - CenterPos.z;

	// Compute the offset of the lens flare along the flare axis	
	//LYNXVECTOR3D	dx;
	//dx.x = vAxisVec.x;
	//dx.y = vAxisVec.y;
	//dx.z = vAxisVec.z;
	//lynxNormalise(&dx, 3);

	//float     fDeltaX    = ( dx.x - dx.y );
	//float     fDeltaY    = ( dx.y + dx.x );
	//float     fViewScale = (FLOAT)sqrt( fDeltaX*fDeltaX + fDeltaY*fDeltaY );
	//


	lynxSetTexSprite2Texture(&gLensFlare.FlareSprite);

	if (LightDir.z > 0.0)
	{
		// Store the lens flares positions for each flare
		for(i=0; i<gLensFlare.NumFlares; i++ )
		{
			// Store the position of the flare along the axis
			Pos.x = CenterPos.x + AxisDir.x * gLensFlare.lpFlareOffset[i];
			Pos.y = CenterPos.y + AxisDir.y * gLensFlare.lpFlareOffset[i];
			Pos.z = CenterPos.z + AxisDir.z * gLensFlare.lpFlareOffset[i];
			
			R = LYNX_ROUND((f)*gLensFlare.FlareSprite.lpColor[i].Red);
			G = LYNX_ROUND((f)*gLensFlare.FlareSprite.lpColor[i].Green);
			B = LYNX_ROUND((f)*gLensFlare.FlareSprite.lpColor[i].Blue);

			LYNX_RV_SET_RGBA(gLensFlare.FlareSprite.lpV[i*4 + 0], R, G, B, 255); 
			LYNX_RV_SET_RGBA(gLensFlare.FlareSprite.lpV[i*4 + 1], R, G, B, 255); 
			LYNX_RV_SET_RGBA(gLensFlare.FlareSprite.lpV[i*4 + 2], R, G, B, 255); 
			LYNX_RV_SET_RGBA(gLensFlare.FlareSprite.lpV[i*4 + 3], R, G, B, 255); 

			lynxSetTexSprite2Time(&gLensFlare.FlareSprite, i);
			
			lynxDrawTexSprite2(&gLensFlare.FlareSprite, Pos.x*50/Pos.z+SW-(gLensFlare.FlareSprite.lpWidth[i]/2.0f), 
				                                      SH-(Pos.y*50/Pos.z)-(gLensFlare.FlareSprite.lpHeight[i]/2.0f));
		}
	}
}
*/
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
/*
void	lynxMoveAndDrawLensFlare(float skip)
{
	LYNXVECTOR3D			vLightPt;
	int					i;

	lynxSetBlendMode(LYNX_ACCUMULATE_BLEND);


	//vLightPt.x = (FLOAT)( sin(fTimeKey*0.73)*12 );
	//vLightPt.y = 10.0f;//(FLOAT)( 5.0f+10.0f*sin(fTimeKey*0.678) );
	//vLightPt.z = (FLOAT)( sin(fTimeKey*0.895)*12 );

	vLightPt.x = (FLOAT)(sin(skip)*20 );
	vLightPt.y = 20.0f;//(FLOAT)( 5.0f+10.0f*sin(fTimeKey*0.678) );
	vLightPt.z = (FLOAT)(cos(skip)*20 );

	LYNXVECTOR3D vLookatPt;
	LYNXVECTOR3D vFromPt;
	LYNXVECTOR3D vViewVec;

	vFromPt.x = 0.0;//30.0f * sin(fTimeKey);
	vFromPt.y = 0.0f;
	vFromPt.z = 0.0;//30.0f * cos(fTimeKey);

	vLookatPt.x = 0.0f * sin(skip);
	vLookatPt.y = 0.0f;
	vLookatPt.z = 20.0f;//0.0f * cos(fTimeKey);

	vViewVec.x  = vLookatPt.x - vFromPt.x;
	vViewVec.y  = vLookatPt.y - vFromPt.y;
	vViewVec.z  = vLookatPt.z - vFromPt.z;
	lynxNormalise(&vViewVec, 3);

	LYNXVECTOR3D vLightVec;
	vLightVec.x = vLightPt.x - vFromPt.x;
	vLightVec.y = vLightPt.y - vFromPt.y;
	vLightVec.z = vLightPt.z - vFromPt.z;
	lynxNormalise(&vLightVec, 3);

	// Compute the vector and center point for the lens flare axis
	float     fDotProduct = LYNX_DOT_PRODUCT(vLightVec, vViewVec);
	LYNXVECTOR3D vNewLightPt;
	vNewLightPt.x = vFromPt.x + 1.0f/fDotProduct * vLightVec.x * 1.01f;
	vNewLightPt.y = vFromPt.y + 1.0f/fDotProduct * vLightVec.y * 1.01f;
	vNewLightPt.z = vFromPt.z + 1.0f/fDotProduct * vLightVec.z * 1.01f;

	LYNXVECTOR3D vCenterPt;
	vCenterPt.x = vFromPt.x + vViewVec.x*1.01f;
	vCenterPt.y = vFromPt.y + vViewVec.y*1.01f;
	vCenterPt.z = vFromPt.z + vViewVec.z*1.01f;

	LYNXVECTOR3D vAxisVec;
    vAxisVec.x = vNewLightPt.x - vCenterPt.x;
	vAxisVec.y = vNewLightPt.y - vCenterPt.y;
	vAxisVec.z = vNewLightPt.z - vCenterPt.z;

	// Compute the offset of the lens flare along the flare axis
	LYNXVECTOR3D	dx;
	dx.x = vAxisVec.x;
	dx.y = vAxisVec.y;
	dx.z = vAxisVec.z;
	lynxNormalise(&dx, 3);

	float     fDeltaX    = ( dx.x - dx.y );
	float     fDeltaY    = ( dx.y + dx.x );
	float     fViewScale = (FLOAT)sqrt( fDeltaX*fDeltaX + fDeltaY*fDeltaY );
	LYNXVECTOR3D		Pos;


	lynxSetTexSprite2Texture(&gLensFlare.FlareSprite);

	if (vLightPt.z > 0.0)
	{
		// Store the lens flares positions for each flare
		for(i=0; i<gLensFlare.NumFlares; i++ )
		{
			// Store the position of the flare along the axis
			Pos.x = vCenterPt.x + vAxisVec.x * gLensFlare.lpFlareOffset[i];
			Pos.y = vCenterPt.y + vAxisVec.y * gLensFlare.lpFlareOffset[i];
			Pos.z = vCenterPt.z + vAxisVec.z * gLensFlare.lpFlareOffset[i];

			LYNX_RV_SET_RGBA(gLensFlare.FlareSprite.lpV[i*4 + 0], LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Red), 
																   LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Green), 
																   LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Blue), 
																   255);
			LYNX_RV_SET_RGBA(gLensFlare.FlareSprite.lpV[i*4 + 1], LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Red), 
																   LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Green), 
																   LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Blue), 
																   255);
			LYNX_RV_SET_RGBA(gLensFlare.FlareSprite.lpV[i*4 + 2], LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Red), 
																   LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Green), 
																   LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Blue), 
																   255);
			LYNX_RV_SET_RGBA(gLensFlare.FlareSprite.lpV[i*4 + 3], LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Red), 
																   LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Green), 
																   LYNX_ROUND((fDotProduct)*gLensFlare.FlareSprite.lpColor[i].Blue), 
																   255);

			lynxSetTexSprite2Time(&gLensFlare.FlareSprite, i);
			lynxDrawTexSprite2(&gLensFlare.FlareSprite, Pos.x*50 / Pos.z +320 -(gLensFlare.FlareSprite.lpWidth[i]/2.0f), 240-(Pos.y*50/Pos.z)-(gLensFlare.FlareSprite.lpHeight[i]/2.0f));
		}
	}
}
*/
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSDKLoadSysFont(LPLYNXSDK lpsdk, LYNXREAL h)
{
	static int	Offset1[] = {12, 16, 24, 28, 35}; 
	static int	Offset2[] = {28, 8, 12, 12, 12, 20, 12, 16, 12, 20, 24, 20, 24, 23}; 	
	static int	Offset3[] = {24, 24, 24, 24, 24, 24, 12, 12, 16, 20, 16, 20}; 	
	static int	Offset4[] = {28, 28, 24, 24, 24, 24, 20, 24, 28, 10}; 	
	static int	Offset5[] = {20, 28, 20, 32, 28, 28, 24, 28, 24}; 	
	static int	Offset6[] = {24, 24, 24, 28, 40, 28, 28, 24, 12, 20}; 	
	static int	Offset7[] = {12, 16, 16, 12, 20, 20, 20, 20, 20, 16, 20, 20, 8, 12, 20}; 	
	static int	Offset8[] = {8, 32, 20, 20, 20, 20, 20, 20, 20, 20, 24, 28}; 	
	static int	Offset9[] = {20, 20, 20, 12, 8, 12, 16}; 	
	LYNXREAL	Ratio;	

	if (lpsdk->lpSysFont)
		LYNX_FREE(lpsdk->lpSysFont);
	lpsdk->lpSysFont = (LPLYNXFONTSPRITES)LYNX_MALLOC(sizeof(LYNXFONTSPRITES));

	//lynxSetTexturePath("");		
	lynxInitFontSprites(lpsdk->lpSysFont);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 0, 5, 2, 2, Offset1, 26, LYNX_NORMAL_TEXTURE);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 5, 14, 2, 28, Offset2, 26, LYNX_NORMAL_TEXTURE);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 19, 12, 2, 54, Offset3, 26, LYNX_NORMAL_TEXTURE);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 31, 10, 2, 80, Offset4, 26, LYNX_NORMAL_TEXTURE);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 41, 9, 2, 106, Offset5, 26, LYNX_NORMAL_TEXTURE);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 50, 10, 2, 132, Offset6, 26, LYNX_NORMAL_TEXTURE);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 60, 15, 2, 158, Offset7, 26, LYNX_NORMAL_TEXTURE);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 75, 12, 2, 184, Offset8, 26, LYNX_NORMAL_TEXTURE);
	lynxLoadVarFontSprites(lpsdk->lpSysFont, "SysFont2.tga", 87, 7, 2, 210, Offset9, 26, LYNX_NORMAL_TEXTURE);
	
	if (lpsdk->lpSysFont->Height[0] != LYNX_REAL(0))
	{
		Ratio = LYNX_REAL_DIV(h, lpsdk->lpSysFont->Height[0]);
		lynxScaleFontSprites(lpsdk->lpSysFont, Ratio);
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSDKReleaseSysFont(LPLYNXSDK lpsdk)
{
	lynxReleaseFontSprites(lpsdk->lpSysFont);
	if (lpsdk->lpSysFont)
		LYNX_FREE(lpsdk->lpSysFont);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSDKDumpAllTextures(LPLYNXSDK lpsdk, LYNXBOOL bquit)
{
	static LYNXBOOL					bInit = FALSE;
	int								i, X, Y, W, H;
	LYNXMATRIX						VM, M;
	LPLYNXTEXTUREMANAGER			lpTexMan;
	LYNXCOLORRGBA					Color;
	static LYNXTEXSPRITE			Tex, TexBack;	
	static int						Offset = 0;
	DWORD							UsedTexSize;

	if (bquit)
	{
		if (bInit)
		{
			lynxReleaseTexSprite(&TexBack);		
			lynxReleaseTexSprite(&Tex);						
		}
		return;
	}
	W = 128;
	H = 128;

	if (!bInit)
	{
		Color.s.Red = 0;
		Color.s.Green = 0;
		Color.s.Blue = 0;
		Color.s.Alpha = 155;		
		lynxLoadTexSprite(&TexBack, "", (float)lpsdk->Renderer.SystemData.CurrentVideoMode.Width, (float)lpsdk->Renderer.SystemData.CurrentVideoMode.Height, &Color, LYNX_NORMAL_TEXTURE);		
		lynxLoadTexSprite(&Tex, "", (float)W, (float)H, NULL, LYNX_NORMAL_TEXTURE);		
		bInit = TRUE;
	}

	lpTexMan = lynxGetlpTextureManager();

	if (lpsdk->KBD[VK_K])
	{
		lpsdk->KBD[VK_K] = FALSE;
		Offset += 5;
	}	
	if (lpsdk->KBD[VK_L])
	{
		lpsdk->KBD[VK_L] = FALSE;
		Offset -= 5;
		if (Offset < 0)
			Offset = 0;
	}
	
	UsedTexSize = 0;
	for (i=0; i<lpTexMan->NumCacheTextures; i++)
	{
		if (lpTexMan->lpCacheTexture[i].TexInfo.Flag == LYNX_USED_ENTRY)
		{
			UsedTexSize += lpTexMan->lpCacheTexture[i].TexInfo.Width*lpTexMan->lpCacheTexture[i].TexInfo.Height*lpTexMan->lpCacheTexture[i].TexInfo.SrcBuffer.bpp/8;			
		}
	}

	lynxGetViewMatrix(&VM);
	LYNX_IDENTITY_MATRIX(M);
	lynxSetViewMatrix(&M);
	
	lynxRenderTexSprite(&TexBack, 0, 0);
	X = 0;
	Y = 0;
	for (i=Offset; i<lpTexMan->NumCacheTextures; i++)
	{
		if (lpTexMan->lpCacheTexture[i].TexInfo.Flag == LYNX_USED_ENTRY)
		{
			Tex.Material.lpMaterialPass[0].TextureFrame[0].lpTexIndexList[0] = lynxGetTextureIndexByName(lpTexMan->lpCacheTexture[i].TexInfo.Name);			
			lynxRenderTexSprite(&Tex, X, Y);
			lynxFontSpritesPrintf(lpsdk->lpSysFont, X, Y+H, NULL, "%s", lpTexMan->lpCacheTexture[i].TexInfo.Name);		
			lynxFontSpritesPrintf(lpsdk->lpSysFont, X, Y+H+15, NULL, "%dx%d", lpTexMan->lpCacheTexture[i].TexInfo.Width, lpTexMan->lpCacheTexture[i].TexInfo.Height);		
			switch (lpTexMan->lpCacheTexture[i].TexInfo.MipMapLevel)
			{
				case 1:
					lynxFontSpritesPrintf(lpsdk->lpSysFont, X, Y+H+30, NULL, "no mipmap");		
					break;

				default:
					lynxFontSpritesPrintf(lpsdk->lpSysFont, X, Y+H+30, NULL, "%d mipmaps", lpTexMan->lpCacheTexture[i].TexInfo.MipMapLevel);		
					break;

			}
			X += W+20;
			if (X > lpsdk->Renderer.SystemData.CurrentVideoMode.Width - W)
			{
				X = 0;
				Y += H+45;
			}			
		}
		else
		{
			lynxPrintf(&lpsdk->Renderer.SystemData.BackBuffer, NULL, X, Y+H, NULL, "Free");		
			lynxPrintf(&lpsdk->Renderer.SystemData.BackBuffer, NULL, X, Y+H+15, NULL, "0x0");		
			X += W+20;
			if (X > lpsdk->Renderer.SystemData.CurrentVideoMode.Width - W)
			{
				X = 0;
				Y += H+45;
			}
		}		
	}	
	X = 0;
	Y += H + 50;
	lynxFontSpritesPrintf(lpsdk->lpSysFont, X, Y, NULL, "Used Texture Memory : %d KB", UsedTexSize/1024);		
	Y += 15;
	lynxFontSpritesPrintf(lpsdk->lpSysFont, X, Y, NULL, "Available System Memory : %d KB", lynxGetAvailablePhysicalMem()/1024);		
	Y += 15;
	lynxFontSpritesPrintf(lpsdk->lpSysFont, X, Y, NULL, "Memory be Allocated by Veda System : %d KB", lynxGetAllocMemSize()/1024);		
	
	lynxSetViewMatrix(&VM);
	
	UsedTexSize = 0;
	for (i=0; i<lpTexMan->NumTextures; i++)
	{
		if (lpTexMan->lpTexture[i].Flag == LYNX_USED_ENTRY)
		{
			UsedTexSize += lpTexMan->lpTexture[i].Width*lpTexMan->lpTexture[i].Height*lpTexMan->lpTexture[i].SrcBuffer.bpp/8;			
		}
	}

	/*
	lynxGetViewMatrix(&VM);
	LYNX_IDENTITY_MATRIX(M);
	lynxSetViewMatrix(&M);
			
	lynxDrawTexSprite(&TexBack, 0, 0);
	X = 0;
	Y = 0;
	for (i=Offset; i<lpTexMan->NumTextures; i++)
	{
		if (lpTexMan->lpTexture[i].Flag == LYNX_USED_ENTRY)
		{
			Tex.TexIndex[0] = i;
			lynxDrawTexSprite(&Tex, X, Y);
			lynxFontSpritesPrintf(&lpg->GameMedia.UI.MsgFont, X, Y+H, NULL, "%s", lpTexMan->lpTexture[i].Name);		
			lynxFontSpritesPrintf(&lpg->GameMedia.UI.MsgFont, X, Y+H+10, NULL, "%dx%d", lpTexMan->lpTexture[i].Width, lpTexMan->lpTexture[i].Height);		
			X += W+20;
			if (X > lpg->Console.Render.SystemData.CurrentVideoMode.Width - W)
			{
				X = 0;
				Y += H+20;
			}			
		}
		else
		{
			lynxFontSpritesPrintf(&lpg->GameMedia.UI.MsgFont, X, Y+H, NULL, "Free");		
			lynxFontSpritesPrintf(&lpg->GameMedia.UI.MsgFont, X, Y+H+10, NULL, "0x0");		
			X += W+20;
			if (X > lpg->Console.Render.SystemData.CurrentVideoMode.Width - W)
			{
				X = 0;
				Y += H+20;
			}
		}
	}
	X = 0;
	Y += H + 40;
	lynxFontSpritesPrintf(&lpg->GameMedia.UI.MsgFont, X, Y, NULL, "Used Texture Memory : %d KB", UsedTexSize/1024);		
	Y += 15;
	lynxFontSpritesPrintf(&lpg->GameMedia.UI.MsgFont, X, Y, NULL, "Available System Memory : %d KB", lynxGetAvailablePhysicalMem()/1024);		
	*/
	lynxSetViewMatrix(&VM);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxSDKDisableTimer(LPLYNXSDK lpsdk, UINT event)
{	
	#ifdef	__WIN32__
		#if _WINDOWS || (_LIB && !_XBOX)
			#ifndef __QT__
				WORD		i;

				for ( i = 0; i < NUM_LYNX_TIMER; i ++ )
				{
					if( lpsdk->Timer[i].Event == event)
					{
						if ( KillTimer(lpsdk->Renderer.SystemData.MainWindow, lpsdk->Timer[i].Event) )
						{
							lpsdk->Timer[i].Event = 0x00;
							lpsdk->Timer[i].Timers = 0;
							lpsdk->Timer[i].lpTimerFunc = NULL;

							return TRUE;
						}
					}
				}					
				return FALSE; // Disable Timer Fail
			#else
				return TRUE;
			#endif
		#else
			return TRUE;
		#endif
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxSDKEnableTimer(LPLYNXSDK lpsdk, UINT event, UINT time, TIMERPROC lptimerfunc)
{	
	#ifdef	__WIN32__
		#if _WINDOWS || (_LIB && !_XBOX)
			#ifndef __QT__
				WORD		i;

				for ( i = 0; i < NUM_LYNX_TIMER; i ++ )
				{
					if( ! lpsdk->Timer[i].Event )
					{
						if( SetTimer(lpsdk->Renderer.SystemData.MainWindow, event, time, lptimerfunc) )
						{
							lpsdk->Timer[i].Event = event;
							lpsdk->Timer[i].Timers = time;
							lpsdk->Timer[i].lpTimerFunc = lptimerfunc;

							return TRUE;
						}
					}
				}				
				return FALSE; // Timer are full
			#else
				return TRUE;
			#endif
		#else
			return TRUE;
		#endif

	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSDKReleaseTimer(LPLYNXSDK lpsdk)
{
	#ifdef	__WIN32__
		#if _WINDOWS || (_LIB && !_XBOX)
			#ifndef __QT__
				WORD		i;

				for ( i = 0; i < NUM_LYNX_TIMER; i ++ )
				{
					if( lpsdk->Timer[i].Event )
					{			
						if ( KillTimer(lpsdk->Renderer.SystemData.MainWindow, lpsdk->Timer[i].Event) )
						{
							lpsdk->Timer[i].Event = 0x00;
							lpsdk->Timer[i].Timers = 0;
							lpsdk->Timer[i].lpTimerFunc = NULL;
						}
					}
				}
			#else					
			#endif
		#endif
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSDKInitTimer(LPLYNXSDK lpsdk)
{		
	lynxMemSet(lpsdk->Timer, sizeof(LYNXTIMER), NUM_LYNX_TIMER);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
VOID CALLBACK lynxDefaultEvent(LYNXWINDOW win, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	/*----temp-------
	lynxReadKeyboardData(&glpConsole->InputDevice, glpConsole->KBD);
	*/
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxSDK 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSDKSetDefaultEvent(LPLYNXSDK lpsdk)
{		
	lynxSDKEnableTimer(lpsdk, 0, 1000 / 12, (TIMERPROC)lynxDefaultEvent);
	//lynxEnableTimer(lpsdk, 0, 1000 / 6, (TIMERPROC)lynxDefaultEvent);
}


