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

#ifdef __DIRECTINPUT__
	static	LYNXINPUTREGISTER	VdInputRegister;
#endif
	
#ifdef __NUCLEUS__
	//#include    <plus\nu_sd.h> 		
	#ifdef __SIS_MOD__
		static volatile unsigned char keypad_key = 0xFF;
		static unsigned long sensitivity = 100;
	#endif
#endif

#ifdef __NUCLEUS__
	#ifdef __SIS_MOD__
			//-------------------------------------------------------------------------------------------------------
			/**
			*  @ingroup LynxIO 
			*  @brief 。
			*
			*	@param 。
			*	@param 。
			*	@param 。
			*	@return。 
			*/
			//-------------------------------------------------------------------------------------------------------
			static VOID ilynx_SIS_Keypad_LISR(INT vector)
			{
				unsigned short key_mask[4] = { 0x700, 0xB00, 0xD00, 0xE00 };
				unsigned char key_buf[4] = { 0, 0, 0, 0 };
				unsigned char key_table[4]={ 0x0, 0x4, 0x8, 0xC };	
				short i;

				*(volatile unsigned*) 0x56000050 &= 0xFF00;         // GPF0~3 are configured as input ports

				// Scan Keys
				for (i = 3; i >= 0; --i) {
					*(volatile unsigned*) 0x56000024 = key_mask[i]; // Set GPCDAT for output values
					key_buf[i] = *(volatile unsigned*) 0x56000054;  // Get GPFDAT
					key_buf[i] &= 0x0F;
				}

				// Convert Key Buffer to Key Value
				for(i = 0; i < 4; ++i) {
					if(key_buf[i] != 0x0F)
						break;
				}

				if (i == 4) {
					keypad_key = 0xFF;
				} else {
					switch (key_buf[i])
					{
					case 0x07:
						keypad_key = key_table[i] | 0x3;
						break;

					case 0x0B:
						keypad_key = key_table[i] | 0x2;
						break;

					case 0x0D:
						keypad_key = key_table[i] | 0x1;
						break;

					case 0x0E:
						keypad_key = key_table[i] | 0x0;
						break;
					}
				}

				// Must reset GPCDAT after scanning key
				*(volatile unsigned short*) 0x56000024 &= 0xF0;

				*(volatile unsigned*) 0x56000050 = 0x00AA;          // GPF0~3 are configured as interrupt EINT0~3
				*(volatile unsigned*) 0x56000058 &= 0xF0;           // Enable EINT0~3 pull-up function
				*(volatile unsigned*) 0x56000088 &= 0xFFFF0000;
				*(volatile unsigned*) 0x56000088 |= 0x00006666;     // EINT0~3 are configured as both edge triggered.

				INT_ClearPending(BIT_EINT0 | BIT_EINT1 | BIT_EINT2 | BIT_EINT3);    // Clear interrupt pending
			}
			//-------------------------------------------------------------------------------------------------------
			/**
			*  @ingroup LynxIO 
			*  @brief 。
			*
			*	@param 。
			*	@param 。
			*	@param 。
			*	@return。 
			*/
			//-------------------------------------------------------------------------------------------------------
			void ilynx_SIS_Keypad_Init(void)
			{
				VOID (*old_lisr)(INT);   /* old LISR */
				INT int_level;

				// Configurate IO Port
				int_level = NU_Local_Control_Interrupts(NU_DISABLE_INTERRUPTS);

				// set GPC
				// GPCCON
				// 16 to 23 as 0x55, pin 8,9,10,11 as output
				*(volatile unsigned*) 0x56000020 &= 0xFF00FFFF;
				*(volatile unsigned*) 0x56000020 |= 0x00550000;

				// GPCUP, set 1 to disable pull up
				*(volatile unsigned*) 0x56000028 |= 0x0F00;

				*(volatile unsigned*) 0x56000050 = 0x00AA;          // GPF0~3 are configured as interrupt EINT0~3
				*(volatile unsigned*) 0x56000058 &= 0xF0;           // Enable EINT0~3 pull-up function
				*(volatile unsigned*) 0x56000088 &= 0xFFFF0000;
				*(volatile unsigned*) 0x56000088 |= 0x00006666;     // EINT0~3 are configured as both edge triggered.

				// Register (Low Level)ISR
				NU_Register_LISR(0, ilynx_SIS_Keypad_LISR, &old_lisr);
				NU_Register_LISR(1, ilynx_SIS_Keypad_LISR, &old_lisr);
				NU_Register_LISR(2, ilynx_SIS_Keypad_LISR, &old_lisr);
				NU_Register_LISR(3, ilynx_SIS_Keypad_LISR, &old_lisr);

				// Enable Interrupt Trigger
				INT_Enable(BIT_EINT0 | BIT_EINT1 | BIT_EINT2 | BIT_EINT3);
				NU_Local_Control_Interrupts(int_level);
			}
			//-------------------------------------------------------------------------------------------------------
			/**
			*  @ingroup LynxIO 
			*  @brief 。
			*
			*	@param 。
			*	@param 。
			*	@param 。
			*	@return。 
			*/
			//-------------------------------------------------------------------------------------------------------	
			unsigned char ilynx_SIS_Keypad_ReadKey(void)
			//unsigned char keypad_readKey(void)			
			{
				static unsigned long pressed_time = 0;
				
				if (keypad_key != 0xFF) //Key is down
				{
					unsigned long current_time = NU_Retrieve_Clock();

					if(pressed_time == 0)
					{
						pressed_time = NU_Retrieve_Clock();
						return keypad_key;                                                                        
					}
					else if (current_time - pressed_time > sensitivity)
					{
						pressed_time = current_time;
						return keypad_key;
					}
					else
					{
						return 0xFF;
					}
				} else //Key is up
				{
					pressed_time = 0;
				}
				return 0xFF;
			}
	#endif
#endif
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxShowCursor(LYNXBOOL b)
{
	#ifdef __WIN32__
		#if (!defined(__WINAPP__) && (_WINDOWS || _CONSOLE || (_LIB && !_XBOX)))
			ShowCursor(b);			
		#endif
	#elif defined __XWIN__
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetCursorPos(LPLYNXPOINT2D pos)
{
	#ifdef __WIN32__
		#if (!defined(__WINAPP__) && (_WINDOWS || _CONSOLE || (_LIB && !_XBOX)))
			POINT			Point;
			#ifndef __QT__
				GetCursorPos(&Point);
				pos->x = Point.x;
				pos->y = Point.y;
			#endif
		#endif
	#elif defined __XWIN__
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxGetClientCursorPos(LPLYNXPOINT2D pos, LYNXWINDOW wnd)
{
	#ifdef __WIN32__
		#if (!defined(__WINAPP__) && (_WINDOWS || _CONSOLE || (_LIB && !_XBOX)))			
			POINT Point;
			GetCursorPos(&Point);
			ScreenToClient(wnd, &Point);
			pos->x = Point.x;
			pos->y = Point.y;			
		#endif
	#elif defined __XWIN__
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetCursorPos(int x, int y)
{
	#ifdef __WIN32__
		#if (!defined(__WINAPP__) && (_WINDOWS || _CONSOLE || (_LIB && !_XBOX)))
			POINT Point;
			SetCursorPos(x, y);
		#endif
	#elif defined __XWIN__
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetClientCursorPos(int x, int y, LYNXWINDOW wnd)
{
	#ifdef __WIN32__
		#if (!defined(__WINAPP__) && (_WINDOWS || _CONSOLE || (_LIB && !_XBOX)))
			POINT Point;
			Point.x = x;
			Point.y = y;
			ClientToScreen(wnd, &Point);
			SetCursorPos(Point.x, Point.x);			
		#endif
	#elif defined __XWIN__
	#endif	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
int    lynxGetMaxTouches()
{
	#ifdef __WIN32__
		int value = GetSystemMetrics(SM_DIGITIZER);
		if (value & NID_READY)
		{ 
			return GetSystemMetrics(SM_MAXIMUMTOUCHES);
		}
		else
		{
			return 0;
		}
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void    lynxInitIOSystem(void)
{	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxGetKeyState(LYNXWINDOW w, int k)
{
	#ifdef __WIN32__
		if (GetFocus() == w)
			return (GetAsyncKeyState(k) & 0x8000) ? LYNX_TRUE : LYNX_FALSE;
		else
			return LYNX_FALSE;
	#else
		return LYNX_FALSE;
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxIO
*  @brief 。
*
*	@param 。
*	@param 。
*	@param 。
*	@return。
*/
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxGetMouseCursorState(LPLYNXPOINT2D pos, LYNXWINDOW wnd)
{
#ifdef __WIN32__
	if (GetFocus() == wnd)
	{
		POINT Point;
		GetCursorPos(&Point);
		ScreenToClient(wnd, &Point);
		pos->x = Point.x;
		pos->y = Point.y;
		return LYNX_TRUE;
	}
	else
		return LYNX_FALSE;
#else
	return LYNX_FALSE;
#endif
}
////###########################################################################
////  Input Section
////
////
////###########################################################################
//#ifdef __DIRECTINPUT__
//#if	IS_DIRECTINPUT_VERSION8
//
//	DIOBJECTDATAFORMAT gODF[18] = { 		
//		{ &GUID_XAxis, FIELD_OFFSET(LYNXINPUTDATA, lX),
//			DIDFT_AXIS | DIDFT_ANYINSTANCE, 0, }, 
//		{ &GUID_YAxis, FIELD_OFFSET(LYNXINPUTDATA, lY), 
//			DIDFT_AXIS | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[0]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[1]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 		
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[2]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[3]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[4]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[5]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[6]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[7]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[8]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 		
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[9]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[10]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[11]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[12]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[13]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[14]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 
//		{ 0, FIELD_OFFSET(LYNXINPUTDATA, bButton[15]),
//			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0, }, 										
//	}; 	
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////-------------------------------------------------------------------------------------------------------
//	BOOL CALLBACK ilynxDirectInputEnumDevObjsProc(LPCDIDEVICEOBJECTINSTANCE pDIDOI, LPVOID context)
//	{
//		LPLYNXINPUTDEVICE lpid =	(LPLYNXINPUTDEVICE)context;
//
//		gODF[lpid->lpDeviceData[lpid->NumDevices].NumButtons+2].pguid = &GUID_Button;
//		gODF[lpid->lpDeviceData[lpid->NumDevices].NumButtons+2].dwOfs = 
//										FIELD_OFFSET(LYNXINPUTDATA, 
//										bButton[lpid->lpDeviceData[lpid->NumDevices].NumButtons]);
//		gODF[lpid->lpDeviceData[lpid->NumDevices].NumButtons+2].dwType = DIDFT_BUTTON | DIDFT_ANYINSTANCE;
//		gODF[lpid->lpDeviceData[lpid->NumDevices].NumButtons+2].dwFlags = 0; 
//
//		lpid->lpDeviceData[lpid->NumDevices].NumButtons ++;
//
//		return TRUE;
//	}	
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////-------------------------------------------------------------------------------------------------------
//	BOOL CALLBACK ilynxDirectInputEnumFFBAxesProc(DIDEVICEOBJECTINSTANCE* pdidoi, LPVOID context)
//	{
//		LPLYNXINPUTDEVICE lpid	=	(LPLYNXINPUTDEVICE)context;
//
//		if ( ( pdidoi->dwFlags & DIDOI_FFACTUATOR ) != 0)
//			lpid->lpDeviceData[lpid->NumDevices].NumForceFeedBackAxes ++;
//
//		return DIENUM_CONTINUE;
//	}
//
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////-------------------------------------------------------------------------------------------------------
//	BOOL CALLBACK	ilynxDirectInputDevEnumProc(LPCDIDEVICEINSTANCE lpdidi, LPVOID pvRef)
//	{
//		LPLYNXINPUTDEVICE			lpid = (LPLYNXINPUTDEVICE)pvRef;
//
//		WORD					i;
//		DIPROPRANGE				DiPRange;
//		DIPROPDWORD				dipdw;
//
//		if( FAILED ( lpid->lpDI->lpVtbl->CreateDevice(
//												lpid->lpDI, 
//												&lpdidi->guidInstance, 
//												&lpid->lpDeviceData[lpid->NumDevices].lpDID,
//												NULL) ) )
//		{			
//			return DIENUM_CONTINUE; // If failed, try again
//		} // if( FAILED ( lpid->lpDI->lpVtbl->CreateDevice(
//
//		lpid->lpDeviceData[lpid->NumDevices].DevCaps.dwSize = sizeof(DIDEVCAPS);
//		if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->GetCapabilities(
//												lpid->lpDeviceData[lpid->NumDevices].lpDID,
//												&lpid->lpDeviceData[lpid->NumDevices].DevCaps) ) )
//        {
//			return DIENUM_CONTINUE;
//		} // if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->GetCapabilities(
//
//		if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->EnumObjects(
//												lpid->lpDeviceData[lpid->NumDevices].lpDID,
//												ilynxDirectInputEnumDevObjsProc, 
//                                                lpid, 
//												DIDFT_BUTTON) ) )
//		{			
//			return DIENUM_CONTINUE;
//		} // if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->EnumObjects(
//
//		lpid->lpDeviceData[lpid->NumDevices].NumButtons = 
//			lpid->lpDeviceData[lpid->NumDevices].DevCaps.dwButtons;
//		if (lpid->lpDeviceData[lpid->NumDevices].NumButtons > 16)
//			lpid->lpDeviceData[lpid->NumDevices].NumButtons = 16;
//
//		lpid->lpDeviceData[lpid->NumDevices].DataFormat.dwSize = sizeof(DIDATAFORMAT);       // this structure 
//		lpid->lpDeviceData[lpid->NumDevices].DataFormat.dwObjSize = sizeof(DIOBJECTDATAFORMAT); // size of object data format 
//		lpid->lpDeviceData[lpid->NumDevices].DataFormat.dwFlags = DIDF_ABSAXIS;               // absolute axis coordinates 
//		lpid->lpDeviceData[lpid->NumDevices].DataFormat.dwDataSize = sizeof(LYNXINPUTDATA);        // device data size 
//		lpid->lpDeviceData[lpid->NumDevices].DataFormat.dwNumObjs = 
//												lpid->lpDeviceData[lpid->NumDevices].NumButtons + 2;                // number of objects 
//		lpid->lpDeviceData[lpid->NumDevices].DataFormat.rgodf =	gODF;		                // and here they are 
//
//		if (FAILED( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetDataFormat(
//												lpid->lpDeviceData[lpid->NumDevices].lpDID, 
//												&lpid->lpDeviceData[lpid->NumDevices].DataFormat) ) )
//		{
//			return DIENUM_CONTINUE; // If failed, try again
//		} // if (FAILED( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetDataFormat(
//
//		if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetCooperativeLevel(
//												lpid->lpDeviceData[lpid->NumDevices].lpDID,
//												lynxGetlpRenderer()->SystemData.MainWindow, 
//												DISCL_EXCLUSIVE | DISCL_FOREGROUND) ) )
//		{
//			return DIENUM_CONTINUE;
//		} // if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetCooperativeLevel(
//
//		DiPRange.diph.dwSize		= sizeof(DIPROPRANGE);
//		DiPRange.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
//		DiPRange.diph.dwObj			= DIJOFS_X;
//		DiPRange.diph.dwHow			= DIPH_BYOFFSET;
//		DiPRange.lMin				= (-512);
//		DiPRange.lMax				= +512; 
//		if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetProperty(
//												lpid->lpDeviceData[lpid->NumDevices].lpDID, 
//												DIPROP_RANGE, 
//												&DiPRange.diph) ) )
//		{
//			return DIENUM_CONTINUE; // If failed, try again
//		} // if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetProperty(
//
//		DiPRange.diph.dwObj = DIJOFS_Y; 
//		if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetProperty(
//												lpid->lpDeviceData[lpid->NumDevices].lpDID,
//												DIPROP_RANGE, 
//												&DiPRange.diph) ) )
//		{
//			return DIENUM_CONTINUE; // If failed, try again
//		} // if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetProperty(
//		
//		//if(lpid->lpDeviceData[lpid->NumDevices].DevCaps.dwFlags & DIDC_FFATTACK)
//		{
//			// 力回匱搖桿
//
//			// Set the coop level
//			dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
//			dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
//			dipdw.diph.dwObj        = 0;
//			dipdw.diph.dwHow        = DIPH_DEVICE;
//			dipdw.dwData            = FALSE;
//
//			if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetProperty(
//												lpid->lpDeviceData[lpid->NumDevices].lpDID,
//												DIPROP_AUTOCENTER, 
//												&dipdw.diph) ) )
//			{
//				return DIENUM_CONTINUE;
//			} // if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->SetProperty(
//		} // if(lpid->lpDeviceData[lpid->NumDevices].DevCaps.dwFlags & DIDC_FFATTACK)
//
//		if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->Acquire(
//												lpid->lpDeviceData[lpid->NumDevices].lpDID) ) )
//		{
//			return DIENUM_CONTINUE;
//		} // if ( FAILED ( lpid->lpDeviceData[lpid->NumDevices].lpDID->lpVtbl->Acquire(
//
//		lpid->lpDeviceData[lpid->NumDevices].lpBindData = NULL;
//
//		lpid->NumDevices ++;
//
//		return DIENUM_CONTINUE;
//	} 
//
//	#ifdef __DRAGON__
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////-------------------------------------------------------------------------------------------------------
//		BOOL CALLBACK	ilynxDirectInputVibEnumProc(LPCMAPLEDEVICEINSTANCE lpmdi, LPVOID context)
//		{   
//			LPLYNXINPUTDEVICE			lpid = (LPLYNXINPUTDEVICE)context;
//			unsigned short			Buffer[128];	
//			HRESULT					hRes;
//			IUnknown				*lpIUnknown;
//			VIB_SETTINGS			VibSettings;
//
//
//			if (lpmdi->devType == MDT_VIBRATION)
//			{
//				wlynxSprintf(Buffer, _T("Find a Vibration Pack at port %d\n"), lpmdi->dwPort);
//				OutputDebugString(Buffer);		
//
//				lynxMemCpy((void *)&lpid->lpDeviceData[lpmdi->dwPort].VibPackData.Guid, (void *)&lpmdi->guidDevice, sizeof(GUID));
//				hRes = MapleCreateDevice(&lpid->lpDeviceData[lpmdi->dwPort].VibPackData.Guid, &lpIUnknown);
//				if (hRes != VIB_OK)
//				{
//					return FALSE;
//			    }
//				lpIUnknown->lpVtbl->QueryInterface(lpIUnknown, &IID_IVib, (PVOID*)&lpid->lpDeviceData[lpmdi->dwPort].VibPackData.VibAPI);
//				lpIUnknown->lpVtbl->Release(lpIUnknown);
//				if (lpid->lpDeviceData[lpmdi->dwPort].VibPackData.VibAPI == NULL)
//				{
//					return FALSE;
//				}	
//
//				lpid->lpDeviceData[lpmdi->dwPort].VibPackData.NumInfos = 16;
//				hRes = lpid->lpDeviceData[lpmdi->dwPort].VibPackData.VibAPI->lpVtbl->GetVibInfo(lpid->lpDeviceData[lpmdi->dwPort].VibPackData.VibAPI,
//																						 &lpid->lpDeviceData[lpmdi->dwPort].VibPackData.NumInfos, 
//																						 &lpid->lpDeviceData[lpmdi->dwPort].VibPackData.NumSimuSources, 				
//																						 lpid->lpDeviceData[lpmdi->dwPort].VibPackData.lpVibInfo); 
//
//				if (hRes != VIB_OK)
//				{
//					wlynxSprintf(Buffer, _T("Error at GetVibInfo\n"));
//					OutputDebugString(Buffer);		
//					return FALSE;
//				}
//
//				VibSettings.sourceId = lpid->lpDeviceData[lpmdi->dwPort].VibPackData.lpVibInfo[0].sourceId;
//				VibSettings.bContinuousVib = TRUE;
//				VibSettings.vibType = VIB_TYPE_CONSTANT;
//				VibSettings.initialPower = VIB_POWER_MIN;
//				VibSettings.direction = VIB_DIRECTION_FORWARD;
//				VibSettings.frequency = lpid->lpDeviceData[lpmdi->dwPort].VibPackData.lpVibInfo[0].maxFrequency;
//				VibSettings.wavesPerStep = 0;
//				lpid->lpDeviceData[lpmdi->dwPort].VibPackData.VibAPI->lpVtbl->Vibrate(lpid->lpDeviceData[lpmdi->dwPort].VibPackData.VibAPI, 1, &VibSettings, NULL);
//				
//				wlynxSprintf(Buffer, _T("%d VibInfos SourceID : %d\n"), lpid->lpDeviceData[lpmdi->dwPort].VibPackData.NumInfos, lpid->lpDeviceData[lpmdi->dwPort].VibPackData.lpVibInfo[0].sourceId);
//				OutputDebugString(Buffer);		
//			}        
//			return TRUE;
//		}		
//
//		//void	lynxVibrate(LPCMAPLEDEVICEINSTANCE lpmdi, LPVOID context)
//	#endif
//
//#endif
//#endif // __DIRECTINPUT__
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// LYNXBOOL	lynxOpenMemoryUnit(LPLYNXINPUTDEVICE lpid)
//{
//	#ifdef _XBOX
//		DWORD							T0;	
//		DWORD							DeviceMask;
//		int								i;		
//
//		T0 = lynxClock();
//		do {
//			lynxOSSleep(100);
//			DeviceMask = XGetDevices(XDEVICE_TYPE_MEMORY_UNIT);		
//			if (lynxClock()-T0 > 1000)
//				return FALSE; 
//		}while(!DeviceMask);
//
//		for(i=0; i < XGetPortCount(); i++ )
//		{			
//			if (DeviceMask & (1<<i) ) 
//			{
//				// Get a handle to the device
//				lpid->MemoryUnit[i*2].Port = i;
//				switch (i)
//				{
//					case 0:
//						XMountMU(XDEVICE_PORT0, XDEVICE_TOP_SLOT, lpid->MemoryUnit[i*2].Driver);
//						break;
//
//					case 1:
//						XMountMU(XDEVICE_PORT1, XDEVICE_TOP_SLOT, lpid->MemoryUnit[i*2].Driver);
//						break;
//
//					case 2:
//						XMountMU(XDEVICE_PORT2, XDEVICE_TOP_SLOT, lpid->MemoryUnit[i*2].Driver);
//						break;
//
//					case 3:
//						XMountMU(XDEVICE_PORT3, XDEVICE_TOP_SLOT, lpid->MemoryUnit[i*2].Driver);
//						break;
//				}
//			}
//			else
//			{
//				lpid->MemoryUnit[i*2].Port = -1;
//			}
//			if(DeviceMask & (1<<(i+16)) ) 
//			{
//				// Get a handle to the device
//				lpid->MemoryUnit[i*2+1].Port = i;
//				switch (i)
//				{
//					case 0:
//						XMountMU(XDEVICE_PORT0, XDEVICE_BOTTOM_SLOT, lpid->MemoryUnit[i*2+1].Driver);
//						break;
//
//					case 1:
//						XMountMU(XDEVICE_PORT1, XDEVICE_BOTTOM_SLOT, lpid->MemoryUnit[i*2+1].Driver);
//						break;
//
//					case 2:
//						XMountMU(XDEVICE_PORT2, XDEVICE_BOTTOM_SLOT, lpid->MemoryUnit[i*2+1].Driver);
//						break;
//
//					case 3:
//						XMountMU(XDEVICE_PORT3, XDEVICE_BOTTOM_SLOT, lpid->MemoryUnit[i*2+1].Driver);
//						break;
//				}
//			}
//			else
//			{
//				lpid->MemoryUnit[i*2+1].Port = -1;
//			}
//		}
//		return TRUE;
//	#endif
//
//	//Yahoo!!!
//	return TRUE;
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxCloseMemoryUnit(LPLYNXINPUTDEVICE lpid)
//{
//	#ifdef _XBOX
//		DWORD							DeviceMask;
//		int								i;		
//
//		for (i=0; i<8; i++)
//		{
//			if (lpid->MemoryUnit[i].Port >= 0)
//			{
//				switch (lpid->MemoryUnit[i].Port)
//				{
//					case 0:
//						if ((i%2))
//						{
//							XUnmountMU(XDEVICE_PORT0, XDEVICE_BOTTOM_SLOT);							
//						}
//						else
//						{
//							XUnmountMU(XDEVICE_PORT0, XDEVICE_TOP_SLOT);
//						}
//						break;
//
//					case 1:
//						if ((i%2))
//						{
//							XUnmountMU(XDEVICE_PORT1, XDEVICE_BOTTOM_SLOT);							
//						}
//						else
//						{
//							XUnmountMU(XDEVICE_PORT1, XDEVICE_TOP_SLOT);
//						}
//						break;
//
//					case 2:
//						if ((i%2))
//						{
//							XUnmountMU(XDEVICE_PORT2, XDEVICE_BOTTOM_SLOT);							
//						}
//						else
//						{
//							XUnmountMU(XDEVICE_PORT2, XDEVICE_TOP_SLOT);
//						}
//						break;
//
//					case 3:
//						if ((i%2))
//						{
//							XUnmountMU(XDEVICE_PORT3, XDEVICE_BOTTOM_SLOT);							
//						}
//						else
//						{
//							XUnmountMU(XDEVICE_PORT3, XDEVICE_TOP_SLOT);
//						}
//						break;
//				}
//			}
//			lpid->MemoryUnit[i].Port = -1;
//		}		
//	#endif
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		關閉振動效果裝置
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////
////			out :	
////
////-------------------------------------------------------------------------------------------------------
// LYNXBOOL	lynxOpenInputDevice(LPLYNXINPUTDEVICE lpid)
//{	
//
//	#ifdef __DIRECTINPUT__
//	#if	IS_DIRECTINPUT_VERSION8
//
//	DWORD			i;
//	FILE			*fp;
//
//	if ( ! lpid->lpDI )
//	{
//		if ( FAILED ( DirectInput8Create(
//										GetModuleHandle(NULL),
//										DIRECTINPUT_VERSION, 
//										&IID_IDirectInput8, 
//										(VOID**)&lpid->lpDI, 
//										NULL) ) )
//			return FALSE;
//	}
//		
//
//	// EnumDevices is how we determine if there are any controllers plugged in.
//	// It will call the DIEnumProc function once for each device.
//	//if ( FAILED ( lpid->lpDI->lpVtbl->EnumDevices(
//	//											lpid->lpDI, 
//	//											DI8DEVCLASS_GAMECTRL, 
//	//											(LPDIENUMDEVICESCALLBACK)ilynxDirectInputDevEnumProc, 
//	//											lpid, 												
//	//											DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK) ) )
//	//	return FALSE;
//	
//	//if (lpid->NumDevices <= 0)
//	{
//		if ( FAILED ( lpid->lpDI->lpVtbl->EnumDevices(
//												lpid->lpDI, 
//												DI8DEVCLASS_GAMECTRL, 
//												(LPDIENUMDEVICESCALLBACK)ilynxDirectInputDevEnumProc, 
//												lpid, 												
//												DIEDFL_ATTACHEDONLY) ) )
//			return FALSE;
//	}
//	// set default ForceEffect	
//	for ( i = 0; i < lpid->NumDevices; i ++ )
//		lynxCreateForceEffect(lpid, i);
//
//	lynxOpenKeyboard(lpid, LYNX_KEYBOARD_MODE_IMMEDIATE);
//
//	#endif
//	#endif // __DIRECTINPUT__
//
//	#ifdef _XBOX
//		DWORD							DeviceMask, T0;
//		int								i;		
//
//		T0 = lynxClock();
//		// Get a mask of all currently available devices
//		do {
//			lynxOSSleep(100);
//			DeviceMask = XGetDevices(XDEVICE_TYPE_GAMEPAD);		
//			if ( (lynxClock()- T0) > LYNX_OPEN_INPUT_DEVICE_TIMEOUT)
//			{
//				break;
//			}
//		}while(!DeviceMask);
//
//		// Open the devices
//		for(i=0; i < XGetPortCount(); i++ )
//		{
//			//ZeroMemory(&g_InputStates[i], sizeof(XINPUT_STATE) );
//			//ZeroMemory( &g_Gamepads[i], sizeof(XBGAMEPAD) );
//
//			if(DeviceMask & (1<<i) ) 
//			{
//				// Get a handle to the device
//				lpid->lpDeviceData[i].Handle = XInputOpen(XDEVICE_TYPE_GAMEPAD, i, XDEVICE_NO_SLOT, NULL );
//
//				// Store capabilities of the device
//				XInputGetCapabilities(lpid->lpDeviceData[i].Handle, &lpid->lpDeviceData[i].DevCaps);
//
//				lpid->NumDevices++;
//			}
//		}		
//	#endif
//
//	#ifdef __NUCLEUS__		
//		#ifdef __SIS_MOD__
//			#ifndef __LIB_IMAGE__	
//				/*
//				// set GPC
//				// GPCCON
//				// 16 to 23 as 0x55, pin 8,9,10,11 as output
//				*(volatile unsigned*) 0x56000020 = 0x00550000;
//				// GPCUP, disable all pull up
//				*(volatile unsigned short*) 0x56000028 = 0xFFFF;
//
//				// set GPF
//				// GPFCON
//				// set as input
//				*(volatile unsigned short*) 0x56000050 = 0x0;
//				// GPFUP, enable pull up for bit 0 to 3
//				*(volatile unsigned char*) 0x56000058 = 0xf0;
//				*/
//				ilynx_SIS_Keypad_Init();
//			#endif
//		#endif
//    #endif
//	
//	#ifdef __PS2__
//    	while(sceSifLoadModule("host:/usr/local/sce/iop/modules/sio2man.irx", 0, NULL) < 0)
//    	{
//        	printf("Retrying module sio2man");
//	    }
//	    while(sceSifLoadModule("host:/usr/local/sce/iop/modules/padman.irx", 0, NULL) < 0)
//	    {
//        	printf("Retrying module padman\n");
//	    }
//	    	    
//	    scePadInit(0);                      // Open controller
//	    scePadPortOpen(0, 0, lpid->lpDeviceData[0].PadBuffer);
//	    scePadPortOpen(1, 0, lpid->lpDeviceData[1].PadBuffer);
//	#endif
//
//	
//	return TRUE;
//}
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		關閉振動效果裝置
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////
////			out :	
////
////-------------------------------------------------------------------------------------------------------
// LYNXBOOL	lynxCloseInputDevice(LPLYNXINPUTDEVICE lpid)
//{
//	#ifdef __DIRECTINPUT__
//	#if	IS_DIRECTINPUT_VERSION8
//
//	DWORD		i;
//
//	if ( ! lpid->lpDI ) return TRUE;
//
//	for ( i = 0; i < lpid->NumDevices; i ++ )
//	{
//		if ( lpid->lpDeviceData[i].lpDID )
//		{
//			lpid->lpDeviceData[i].lpDID->lpVtbl->SendForceFeedbackCommand(
//												lpid->lpDeviceData[i].lpDID,
//												DISFFC_STOPALL);
//
//			lpid->lpDeviceData[i].lpDID->lpVtbl->Unacquire(lpid->lpDeviceData[i].lpDID);
//		} // if ( lpid->lpDeviceData[i].lpDID )
//	} // for ( i = 0; i < lpid->NumDevices; i ++ )
//
//	#ifdef __DRAGON__
//	#endif // __DRAGON__
//
//	#endif
//	#endif // __DIRECTINPUT__
//	
//	return TRUE;
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// LYNXBOOL	lynxIsInputDeviceConnect(LPLYNXINPUTDEVICE lpid, short joyid)
//{
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//		
//		if (joyid < 0) 
//			return FALSE;
//
//		if (!lpid->lpDI) 
//			return FALSE;
//
//		if (lpid->lpDeviceData[joyid].lpDID)
//			return TRUE;		
//
//		return FALSE;
//		#endif
//	#elif defined _XBOX
//		if (joyid < 0) 
//			return FALSE;
//
//		if (lpid->lpDeviceData[joyid].Handle)
//			return TRUE;
//
//		return FALSE;
//	#endif // __DIRECTINPUT__
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxBindInputData(LPLYNXINPUTDEVICE lpid, LPLYNXINPUTDATA lpidata, short joyid)
//{
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//
//		if (joyid < 0) 
//			return;
//
//		if (!lpid->lpDI) 
//			return;
//
//		if (lpid->lpDeviceData[joyid].lpDID)
//		{
//			lpid->lpDeviceData[joyid].lpBindData = lpidata;					
//		}
//		#endif
//	#elif defined _XBOX
//		if (joyid < 0) return;
//
//		lpid->lpDeviceData[joyid].lpBindData = lpidata;		
//	#endif // __DIRECTINPUT__
//	lynxInitInputData(lpidata);
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxBindKey(LPLYNXINPUTDEVICE lpid, BYTE *lpdata, short key)
//{
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//
//		if (!lpid->lpDI) 
//			return;
//
//		lpid->KeyboardDeviceData.lpBindData[key] = lpdata;		
//		lpid->KeyboardDeviceData.BindKeyIndex[lpid->KeyboardDeviceData.NumBindData] = key;
//		lpid->KeyboardDeviceData.NumBindData++;
//		#endif
//	#else
//		lpid->KeyboardDeviceData.lpBindData[key] = lpdata;		
//		lpid->KeyboardDeviceData.BindKeyIndex[lpid->KeyboardDeviceData.NumBindData] = key;
//		lpid->KeyboardDeviceData.NumBindData++;
//	#endif
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxGetBindInputData(LPLYNXINPUTDEVICE lpid, short joyid)
//{
//	if (joyid < 0) 
//		return;
//
//	if (lpid->lpDeviceData[joyid].lpBindData)
//		lynxGetInputData(lpid, lpid->lpDeviceData[joyid].lpBindData, joyid);
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxGetBindKeyData(LPLYNXINPUTDEVICE lpid)
//{
//	static BYTE			Data[LYNX_KEYBOARDD_ARRAY_SIZE];
//	int					i;
//
//	lynxReadKeyboardData(lpid, Data);	
//	for (i=0; i<lpid->KeyboardDeviceData.NumBindData; i++)
//	{
//		(*lpid->KeyboardDeviceData.lpBindData[lpid->KeyboardDeviceData.BindKeyIndex[i]]) = (Data[lpid->KeyboardDeviceData.BindKeyIndex[i]] & 0x80);
//	}
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// LPLYNXINPUTDATA	lynxGetlpBindInputData(LPLYNXINPUTDEVICE lpid, short joyid)
//{
//	return  (lpid->lpDeviceData[joyid].lpBindData);
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxClearInputData(LPLYNXINPUTDATA lpidata)
//{
//	lpidata->lY = 0;
//	lpidata->lX = 0;
//	lpidata->lY2 = 0;
//	lpidata->lX2 = 0;
//	lynxMemSet(lpidata->bButton, 0, 16);						
//	lynxMemSet(lpidata->bButtonLock, 0, 16);	
//
//	lynxMemSet(lpidata->bGamePadDir, 0, 4);						
//	lynxMemSet(lpidata->bGamePadDirLock, 0, 4);							
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxSetInputData(LPLYNXINPUTDATA lpidata)
//{	
//	lynxMemSet(lpidata->bButton, 1, 16);						
//	lynxMemSet(lpidata->bButtonLock, 1, 16);	
//
//	lynxMemSet(lpidata->bGamePadDir, 1, 4);						
//	lynxMemSet(lpidata->bGamePadDirLock, 1, 4);							
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxInitInputData(LPLYNXINPUTDATA lpidata)
//{
//	lynxClearInputData(lpidata);	
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxNoRepeatInput(LPLYNXINPUTDATA lpid)
//{
//	int							i;
//
//	for (i=0; i<16; i++)
//	{
//		if (lpid->bButton[i])
//		{
//			lpid->bButtonLock[i] = TRUE;
//		}
//		else
//		{
//			if (lpid->bButtonLock[i])
//				lpid->bButtonLock[i] = FALSE;
//		}		
//	}
//
//	for (i=0; i<4; i++)
//	{
//		if (lpid->bGamePadDir[i])
//		{
//			lpid->bGamePadDirLock[i] = TRUE;
//		}
//		else
//		{
//			if (lpid->bGamePadDirLock[i])
//				lpid->bGamePadDirLock[i] = FALSE;
//		}		
//	}
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxNoRepeatButton(LPLYNXINPUTDATA lpid, int b)
//{
//	if (lpid->bButton[b])
//	{
//		lpid->bButtonLock[b] = TRUE;
//	}
//	else
//	{
//		if (lpid->bButtonLock[b])
//			lpid->bButtonLock[b] = FALSE;
//	}		
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxNoRepeatGamePadDir(LPLYNXINPUTDATA lpid)
//{
//	int							i;
//
//	for (i=0; i<4; i++)
//	{
//		if (lpid->bGamePadDir[i])
//		{
//			lpid->bGamePadDirLock[i] = TRUE;
//		}
//		else
//		{
//			if (lpid->bGamePadDirLock[i])
//				lpid->bGamePadDirLock[i] = FALSE;
//		}		
//	}		
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxGetInputData(LPLYNXINPUTDEVICE lpid, LPLYNXINPUTDATA lpidata, short joyid)
//{
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//
//			HRESULT				hRes;		
//			int					i;
//
//			if (joyid < 0) 
//				return;
//
//			if ( ! lpid->lpDI ) 
//				return;
//
//			if ( lpid->lpDeviceData[joyid].lpDID )
//			{
//				lpid->lpDeviceData[joyid].lpDID->lpVtbl->Acquire(lpid->lpDeviceData[joyid].lpDID);
//				lpid->lpDeviceData[joyid].lpDID->lpVtbl->Poll(lpid->lpDeviceData[joyid].lpDID);
//
//				lpid->lpDeviceData[joyid].lpDID->lpVtbl->GetDeviceState(
//														lpid->lpDeviceData[joyid].lpDID, 
//														sizeof(LYNXINPUTDATA), 
//														(void *)lpidata);
//			}
//		#endif
//	#elif defined _XBOX
//		DWORD					dwInsertions, dwRemovals, Time;
//		int						i;
//
//		if (joyid < 0) return;
//		
//		XGetDeviceChanges(XDEVICE_TYPE_GAMEPAD, &dwInsertions, &dwRemovals);
//
//		// Loop through all gamepads
//		for(i=0; i<XGetPortCount(); i++)
//		{
//			// Handle removed devices.
//			if ((dwRemovals & (1<<i) ))
//			{
//				// if the controller was removed after XGetDeviceChanges but before
//				// XInputOpen, the device handle will be NULL
//				if (lpid->lpDeviceData[i].Handle)
//					XInputClose(lpid->lpDeviceData[i].Handle);
//				lpid->lpDeviceData[i].Handle = NULL;				
//				lpid->NumDevices --;
//			}
//
//			// Handle inserted devices
//			if (( dwInsertions & (1<<i) ))
//			{
//				// TCR 1-14 Device Types
//				lpid->lpDeviceData[i].Handle = XInputOpen(XDEVICE_TYPE_GAMEPAD, i, 
//											   XDEVICE_NO_SLOT, NULL );
//
//				// if the controller is removed after XGetDeviceChanges but before
//				// XInputOpen, the device handle will be NULL
//				if (lpid->lpDeviceData[i].Handle)
//					XInputGetCapabilities(lpid->lpDeviceData[i].Handle, &lpid->lpDeviceData[i].DevCaps);
//				lpid->NumDevices ++;
//			}
//		}		
//		if (lpid->lpDeviceData[joyid].Handle)
//        {
//            // Read the input state
//            XInputGetState(lpid->lpDeviceData[joyid].Handle, &lpid->lpDeviceData[joyid].InputState);
//
//			lpidata->lX = (lpid->lpDeviceData[joyid].InputState.Gamepad.sThumbLX>>7);
//			lpidata->lY = (-lpid->lpDeviceData[joyid].InputState.Gamepad.sThumbLY>>7);            
//
//			lpidata->lX2 = (lpid->lpDeviceData[joyid].InputState.Gamepad.sThumbRX>>7);
//			lpidata->lY2 = (-lpid->lpDeviceData[joyid].InputState.Gamepad.sThumbRY>>7);            
//
//			lynxMemSet(lpidata->bButton, 0, 16);									
//
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.bAnalogButtons[XINPUT_GAMEPAD_A] > 30)
//			{
//				lpidata->bButton[0] = TRUE;				
//			}
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.bAnalogButtons[XINPUT_GAMEPAD_B] > 30)
//			{
//				lpidata->bButton[1] = TRUE;				
//			}		
//
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.bAnalogButtons[XINPUT_GAMEPAD_X] > 30)
//			{
//				lpidata->bButton[2] = TRUE;
//			}
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.bAnalogButtons[XINPUT_GAMEPAD_Y] > 30)
//			{
//				lpidata->bButton[3] = TRUE;
//			}
//
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.bAnalogButtons[XINPUT_GAMEPAD_BLACK] > 30)
//			{
//				lpidata->bButton[4] = TRUE;
//			}
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.bAnalogButtons[XINPUT_GAMEPAD_WHITE] > 30)
//			{
//				lpidata->bButton[5] = TRUE;
//			}
//
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 30)
//			{
//				lpidata->bButton[6] = TRUE;
//			}
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 30)
//			{
//				lpidata->bButton[7] = TRUE;
//			}				
//
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.wButtons & XINPUT_GAMEPAD_START)
//			{
//				lpidata->bButton[8] = TRUE;
//			}					
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
//			{
//				lpidata->bButton[9] = TRUE;				
//			}
//
//			lynxMemSet(lpidata->bGamePadDir, 0, 4);						
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
//			{
//				lpidata->bGamePadDir[LYNX_GAMEPAD_UP] = TRUE;
//			}
//			else if (lpid->lpDeviceData[joyid].InputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
//			{
//				lpidata->bGamePadDir[LYNX_GAMEPAD_DOWN] = TRUE;
//			}
//			if (lpid->lpDeviceData[joyid].InputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
//			{
//				lpidata->bGamePadDir[LYNX_GAMEPAD_LEFT] = TRUE;
//			}
//			else if (lpid->lpDeviceData[joyid].InputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
//			{
//				lpidata->bGamePadDir[LYNX_GAMEPAD_RIGHT] = TRUE;
//			}			
//			
//			if (lpid->lpDeviceData[joyid].ForceDuringTime)
//			{
//				Time = lynxClock() - lpid->lpDeviceData[joyid].ForceStartTime;
//				if ((lynxClock() - lpid->lpDeviceData[joyid].ForceStartTime) > lpid->lpDeviceData[joyid].ForceDuringTime)
//				{
//					lynxStopForceEffect(lpid, joyid);
//				}
//			}
//
//			if (lpid->lpDeviceData[joyid].ForceEffect.Header.dwStatus != ERROR_IO_PENDING )
//            {
//                // We can alter the motor values                
//                XInputSetState(lpid->lpDeviceData[joyid].Handle, &lpid->lpDeviceData[joyid].ForceEffect);
//            }
//        }
//	#elif defined __NUCLEUS__		
//		#ifdef __SIS_MOD__
//			unsigned char Key;
//
//			lynxMemSet(lpidata->bButton, 0, 16);	
//			lynxMemSet(lpidata->bGamePadDir, 0, 4);						
//			#ifdef __LIB_IMAGE__	
//				Key = keypad_readKey();
//			#else
//				Key = ilynx_SIS_Keypad_ReadKey();
//			#endif
//			
//			lpidata->bButton[Key] = LYNX_TRUE;
//			switch (Key)
//			{
//				case 8:
//					lpidata->bGamePadDir[LYNX_GAMEPAD_UP] = TRUE;
//					break;
//
//				case 10:
//					lpidata->bGamePadDir[LYNX_GAMEPAD_DOWN] = TRUE;
//					break;
//
//				case 5:
//					lpidata->bGamePadDir[LYNX_GAMEPAD_RIGHT] = TRUE;
//					break;
//
//				case 13:
//					lpidata->bGamePadDir[LYNX_GAMEPAD_LEFT] = TRUE;
//					break;
//			}
//			//unsigned short key_mask[4] = { 0x700, 0xB00, 0xD00, 0xE00 };
//			//unsigned char key_buf[4] = { 0, 0, 0, 0};
//			//unsigned char key_table[4]={ 0x0, 0x4, 0x8, 0xC };
//			//unsigned char temp, key = 0xFF;
//			//int i;
//
//			//lynxMemSet(lpidata->bButton, 0, 16);	
//			//lynxMemSet(lpidata->bGamePadDir, 0, 4);						
//
//			//*(volatile unsigned short*) 0x56000024 = 0x0;
//			//temp = (*(volatile unsigned char*) 0x56000054)&0xf;
//
//			//if (temp == 0xF)
//			//	return;
//
//			//for(i = 3; i >= 0; --i) 
//			//{
//			//	//GPCDAT
//			//	*(volatile unsigned short*) 0x56000024 = key_mask[i];
//
//			//	//get data from the GPF
//			//	key_buf[i] = (*(volatile unsigned char*)0x56000054)&0xf;
//			//}
//
//			//for(i = 0; i < 4; ++i) 
//			//{
//			//	if(key_buf[i] != 0xF)
//			//	{				
//			//		if (i == 4) 
//			//		{
//			//			return;
//			//		} 
//			//		else 
//			//		{
//			//			switch(key_buf[i]) 
//			//			{
//			//				case 0x7:
//			//					key = key_table[i] | 0x3;
//			//					break;
//
//			//				case 0xB:
//			//					key = key_table[i] | 0x2;
//			//					break;
//
//			//				case 0xD:
//			//					key = key_table[i] | 0x1;
//			//					break;
//
//			//				case 0xE:
//			//					key = key_table[i] | 0x0;
//			//					break;
//			//			}						
//			//			lpidata->bButton[key] = LYNX_TRUE;
//
//			//			switch (key)
//			//			{
//			//				case 8:
//			//					lpidata->bGamePadDir[LYNX_GAMEPAD_UP] = TRUE;
//			//					break;
//
//			//				case 10:
//			//					lpidata->bGamePadDir[LYNX_GAMEPAD_DOWN] = TRUE;
//			//					break;
//
//			//				case 5:
//			//					lpidata->bGamePadDir[LYNX_GAMEPAD_RIGHT] = TRUE;
//			//					break;
//
//			//				case 13:
//			//					lpidata->bGamePadDir[LYNX_GAMEPAD_LEFT] = TRUE;
//			//					break;
//			//			}
//			//		}
//			//	}
//			//}			
//		#endif
//    #elif defined __PS2__
//    	BYTE					Data[32];
//    	u_int					PadData;
//    	
//    	if (scePadRead(joyid, 0, Data) == 0)
//    	{
//    		PadData = 0;    		
//    	}
//    	else
//    	{
//    		PadData = 0xffff ^ ((Data[2] << 8) | Data[3]);    		
//    	}
//    	
//    	if (PadData & SCE_PADstart)
//    	{
//    		lpidata->bButton[8] = TRUE;
//    	}
//    	else
//    	{
//	    	lpidata->bButton[8] = FALSE;
//    	}
//
//	#endif // __DIRECTINPUT__		
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxGetInputDataAll(LPLYNXINPUTDEVICE lpid, LPLYNXINPUTDATA lpidata)
//{
//	#ifdef __DIRECTINPUT__
//	#if	IS_DIRECTINPUT_VERSION8
//
//	HRESULT				hRes;
//
//	int					i;
//	//LYNXINPUTDATA			*p;
//
//	if ( ! lpid->lpDI ) return;
//
//	for ( i = 0; i < lpid->NumDevices; i ++ )
//	{
//		if ( lpid->lpDeviceData[i].lpDID )
//		{
//			lpid->lpDeviceData[i].lpDID->lpVtbl->Acquire(lpid->lpDeviceData[i].lpDID);
//			hRes = lpid->lpDeviceData[i].lpDID->lpVtbl->Poll(lpid->lpDeviceData[i].lpDID);
//			/*
//			if (hRes == DIERR_UNPLUGGED)
//			{
//				//DbgPrintf(_T("poll found port %d unplugged\n"), m_port);
//				return;
//			}
//			*/
//
//			// 如果要取得全部，這裡要改，資料結構不同，尚不能改
//			//p = lpidata + (sizeof(LYNXINPUTDATA) * i);
//			hRes = lpid->lpDeviceData[i].lpDID->lpVtbl->GetDeviceState(
//												lpid->lpDeviceData[i].lpDID, 
//												sizeof(LYNXINPUTDATA), 
//												(void *)lpidata);
//			if ( FAILED ( hRes ) ) return;
//					
//		} // if ( lpid->lpDeviceData[i].lpDID )
//	} // for ( i = 0; i < lpid->NumDevices; i ++ )
//
//	#endif
//	#endif // #ifdef __DIRECTINPUT__
//}
//
//#ifdef __DIRECTINPUT__
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		回呼振動效果
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	詳閱 DirectX 8.0a SDK Document
////
////			out :	
////
////-------------------------------------------------------------------------------------------------------
//BOOL CALLBACK DIEnumEffectsCallback(LPCDIEFFECTINFO pdei, LPVOID pvRef)
//{
//	#if	IS_DIRECTINPUT_VERSION8
//
//	#ifdef	_DEBUG
//
//	FILE				*fp;
//
//	#endif // _DEBUG
//
//	//LPLYNXINPUTREGISTER	lpregister	=	(LPLYNXINPUTREGISTER)pvRef;
//	LPLYNXINPUTREGISTER		lpregister	=	&VdInputRegister;
//	LPLYNXINPUTDEVICE		lpid		=	lpregister->lpid;
//	WORD					JoyId		=	lpregister->JoyId;
//	HRESULT					hRes;
//	WORD					i;
//	// This application needs only one effect: Applying raw forces.
//	DWORD					rgdwAxes[2]     = {DIJOFS_X, DIJOFS_Y};
//	LONG					rglDirection[2] = {1, 1};
//	DIPERIODIC				cf;
//
//	if ( !lpid->lpDeviceData[JoyId].lpDID )
//		return DIENUM_CONTINUE;
//	//ZeroMemory(&Eff, sizeof(Eff));
//
//	cf.dwMagnitude = 10000;
//	cf.lOffset = 0;
//	cf.dwPhase = 0;
//	cf.dwPeriod = 100000;
//
//	lpid->lpDeviceData[JoyId].Dieffect.dwSize                  = sizeof(DIEFFECT);
//	lpid->lpDeviceData[JoyId].Dieffect.dwSamplePeriod          = 0;
//	lpid->lpDeviceData[JoyId].Dieffect.dwTriggerButton         = DIEB_NOTRIGGER;
//	lpid->lpDeviceData[JoyId].Dieffect.dwTriggerRepeatInterval = 0;
//	lpid->lpDeviceData[JoyId].Dieffect.rgdwAxes                = rgdwAxes;
//	lpid->lpDeviceData[JoyId].Dieffect.rglDirection            = rglDirection;
//	lpid->lpDeviceData[JoyId].Dieffect.dwGain                  = LYNX_INPUT_VIBRATION_DEFAULT;
//	lpid->lpDeviceData[JoyId].Dieffect.dwFlags                 = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
//	lpid->lpDeviceData[JoyId].Dieffect.dwDuration              = LYNX_INPUT_VTIME_NORMAL;
//	lpid->lpDeviceData[JoyId].Dieffect.cAxes                   = 2;
//	lpid->lpDeviceData[JoyId].Dieffect.lpEnvelope              = 0;
//	lpid->lpDeviceData[JoyId].Dieffect.cbTypeSpecificParams    = sizeof(DIPERIODIC);
//	lpid->lpDeviceData[JoyId].Dieffect.lpvTypeSpecificParams   = &cf;
//	lpid->lpDeviceData[JoyId].Dieffect.dwStartDelay            = 0;
//
//	lpid->lpDeviceData[JoyId].lpEffect = NULL ;
//
//	// Create the prepared effect		
//	hRes = lpid->lpDeviceData[JoyId].lpDID->lpVtbl->CreateEffect(
//								lpid->lpDeviceData[JoyId].lpDID,
//								&GUID_Square,
//                                &lpid->lpDeviceData[JoyId].Dieffect,
//								&lpid->lpDeviceData[JoyId].lpEffect, 
//								NULL);
//
//	if ( FAILED ( hRes ) )
//	{
//		lpid->lpDeviceData[JoyId].lpEffect = NULL;
//			
//		#ifdef	_DEBUG
//
//		#include <stdlib.h>
//
//		fp=fopen("d://debug1.txt", "wt");
//			
//		switch ( hRes )
//		{
//			case DIERR_DEVICEFULL  :	fprintf(fp, "1\n"); break;
//			case DIERR_DEVICENOTREG  :	fprintf(fp, "2\n"); break;
//			case DIERR_INVALIDPARAM  :	fprintf(fp, "3\n"); break;
//			case DIERR_NOTINITIALIZED  :	fprintf(fp, "4\n"); break;
//		} // switch ( hRes )
//
//		fprintf(fp, "create error!!");
//		fclose(fp);
//		exit(1);
//
//		#endif // _DEBUG
//
//		return DIENUM_CONTINUE;
//	} // if ( FAILED ( hRes ) )
//		
//	#endif
//	return DIENUM_CONTINUE;
//}
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		回呼從檔案建立振動效果
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	詳閱 DirectX 8.0a SDK Document
////
////			out :	
////
//
////-------------------------------------------------------------------------------------------------------
//BOOL CALLBACK EnumAndCreateEffectsCallback( LPCDIFILEEFFECT pDIFileEffect, VOID* pvRef )
//{
//	#if	IS_DIRECTINPUT_VERSION8
//
//	//LPLYNXINPUTREGISTER	lpregister	=	(LPLYNXINPUTREGISTER)pvRef;
//	LPLYNXINPUTREGISTER	lpregister	=	&VdInputRegister;
//	LPLYNXINPUTDEVICE		lpid		=	lpregister->lpid;
//	WORD				JoyId		=	lpregister->JoyId;
//
//    HRESULT				hRes;
//
//	WORD				i;
//
//	#ifdef	_DEBUG
//
//	FILE				*fp;
//
//	#endif // _DEBUG
//
//	lpid->lpDeviceData[JoyId].lpEffect = NULL;
//
//    // Create the file effect
//	if ( lpid->lpDeviceData[JoyId].lpDID )
//	{
//		hRes = lpid->lpDeviceData[JoyId].lpDID->lpVtbl->CreateEffect(
//									lpid->lpDeviceData[JoyId].lpDID,
//									&pDIFileEffect->GuidEffect, 
//									pDIFileEffect->lpDiEffect, 
//									&lpid->lpDeviceData[JoyId].lpEffect,
//									NULL);
//		if ( FAILED ( hRes ) )
//		{
//			lpid->lpDeviceData[JoyId].lpEffect = NULL;
//
//			#ifdef	_DEBUG
//
//			#include <stdlib.h>
//
//			fp=fopen("d://debug1.txt", "wt");
//
//			switch ( hRes )
//			{
//				case DIERR_DEVICEFULL  :	fprintf(fp, "1\n"); break;
//				case DIERR_DEVICENOTREG  :	fprintf(fp, "2\n"); break;
//				case DIERR_INVALIDPARAM  :	fprintf(fp, "3\n"); break;
//				case DIERR_NOTINITIALIZED  :	fprintf(fp, "4\n"); break;
//			} // switch
//
//			fprintf(fp, "create error!!");
//			fclose(fp);
//			exit(1);
//
//			#endif // 	_DEBUG
//
//			return DIENUM_CONTINUE;
//		} // if ( FAILED ( hRes ) )
//
//	} // if ( lpid->lpDeviceData[JoyId].lpDID )
//
//	#endif		
//	return DIENUM_CONTINUE;
//}
//#endif // __DIRECTINPUT__
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		建立振動效果
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////					WORD joyid					搖桿 id
////
////			out :	NONE
////
////-------------------------------------------------------------------------------------------------------
// void		lynxCreateForceEffect(LPLYNXINPUTDEVICE lpid, short joyid)
//{
//	#ifdef __DIRECTINPUT__
//	#if	IS_DIRECTINPUT_VERSION8
//
//	if (joyid < 0) return;
//
//	if ( ! lpid->lpDI ) return;
//
//	if ( lpid->lpDeviceData[joyid].lpDID )
//	{
//		VdInputRegister.lpid = lpid;
//		VdInputRegister.JoyId = joyid;
//			
//		lpid->lpDeviceData[joyid].lpDID->lpVtbl->EnumEffects(
//									lpid->lpDeviceData[joyid].lpDID, 
//									DIEnumEffectsCallback,
//									&VdInputRegister,
//									DI8DEVCLASS_GAMECTRL);
//	} // if ( lpid->lpDeviceData[joyid].lpDID )
//
//	#endif
//	#endif // __DIRECTINPUT__
//}
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		從檔案建立振動效果
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////					WORD joyid					搖桿 id
////					char *fname					檔名
////
////			out :	NONE
////
////-------------------------------------------------------------------------------------------------------
// void		lynxCreateForceEffectFromFile(LPLYNXINPUTDEVICE lpid, short joyid, char *fname)
//{
//	#ifdef __DIRECTINPUT__	
//	#if	IS_DIRECTINPUT_VERSION8
//
//	if (joyid < 0) return;
//
//	if ( ! lpid->lpDI ) return;
//
//	if ( lpid->lpDeviceData[joyid].lpDID )
//	{
//		VdInputRegister.lpid = lpid;
//		VdInputRegister.JoyId = joyid;
//		
//		lpid->lpDeviceData[joyid].lpDID->lpVtbl->EnumEffectsInFile(
//									lpid->lpDeviceData[joyid].lpDID,
//									fname,
//									EnumAndCreateEffectsCallback,
//									&VdInputRegister,
//									DIFEF_DEFAULT);
//	} // if ( lpid->lpDeviceData[joyid].lpDID )
//	else
//		return;
//
//	#endif
//	#endif // __DIRECTINPUT__
//}
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		停止所有裝置振動效果
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////
////			out :	NONE
////
////-------------------------------------------------------------------------------------------------------
// void		lynxStopAllForceEffect(LPLYNXINPUTDEVICE lpid)
//{
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//
//		WORD			i;
//
//		if ( ! lpid->lpDI ) return;
//
//		for ( i = 0; i < LYNX_INPUT_DEVICE_MAX; i ++ )
//		{
//			if ( lpid->lpDeviceData[i].lpDID )
//				lpid->lpDeviceData[i].lpDID->lpVtbl->SendForceFeedbackCommand(
//										lpid->lpDeviceData[i].lpDID,
//										DISFFC_STOPALL);
//		} // for ( i = 0; i < LYNX_INPUT_DEVICE_MAX; i ++ )
//
//		#endif
//	#elif defined _XBOX
//		int						i;		
//
//		for ( i = 0; i <XGetPortCount(); i ++ )
//		{
//			if (lpid->lpDeviceData[i].Handle)
//			{
//				lpid->lpDeviceData[i].ForceEffect.Rumble.wLeftMotorSpeed = 0;
//				lpid->lpDeviceData[i].ForceEffect.Rumble.wRightMotorSpeed = 0;
//			}
//		}
//	#endif
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		停止振動效果
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////					WORD joyid					搖桿 id
////
////			out :	NONE
////
////-------------------------------------------------------------------------------------------------------
// void		lynxStopForceEffect(LPLYNXINPUTDEVICE lpid, short joyid)
//{
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//
//		WORD			NewId;
//
//		if (joyid < 0) 
//			return;
//
//		if ( ! lpid->lpDI ) 
//			return;
//
//		if ( lpid->lpDeviceData[joyid].lpDID )
//		{
//			if ( lpid->lpDeviceData[joyid].lpEffect )
//				 lpid->lpDeviceData[joyid].lpEffect->lpVtbl->Stop(lpid->lpDeviceData[joyid].lpEffect);						
//		} // if ( lpid->lpDeviceData[joyid].lpDID )
//		#endif
//	#elif defined _XBOX
//		if (joyid < 0) 
//			return;
//
//		if (lpid->lpDeviceData[joyid].Handle)
//		{
//			lpid->lpDeviceData[joyid].ForceEffect.Rumble.wLeftMotorSpeed = 0;
//			lpid->lpDeviceData[joyid].ForceEffect.Rumble.wRightMotorSpeed = 0;
//			lpid->lpDeviceData[joyid].ForceDuringTime = 0;
//		}
//	#endif
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		播放振動效果
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////					WORD joyid					搖桿 id
////					WORD effect_id				效果 id
////					DWORD replay				重播次數
////
////			out :	NONE
////
////-------------------------------------------------------------------------------------------------------
// void		lynxPlayForceEffect(LPLYNXINPUTDEVICE lpid, short joyid, DWORD Vibration, DWORD Vtime, DWORD replay)
//{
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//
//		WORD			NewId;
//
//		if (joyid < 0) return;
//
//		if ( ! lpid->lpDI ) return;
//
//		if (!lpid->lpDeviceData[joyid].bUseVibrate) return;
//
//		if ( lpid->lpDeviceData[joyid].lpDID )
//		{
//			if ( lpid->lpDeviceData[joyid].lpEffect )
//			{
//				lpid->lpDeviceData[joyid].lpEffect->lpVtbl->Stop(
//										lpid->lpDeviceData[joyid].lpEffect);
//				
//				lpid->lpDeviceData[joyid].Dieffect.dwGain = Vibration;
//				lpid->lpDeviceData[joyid].Dieffect.dwDuration = Vtime;
//		
//				lpid->lpDeviceData[joyid].lpEffect->lpVtbl->SetParameters(
//										lpid->lpDeviceData[joyid].lpEffect,
//										&lpid->lpDeviceData[joyid].Dieffect,
//										DIEP_DURATION | DIEP_GAIN);
//
//				
//				lpid->lpDeviceData[joyid].lpEffect->lpVtbl->Start(lpid->lpDeviceData[joyid].lpEffect, replay, 0);
//				
//			} // if ( lpid->lpDeviceData[joyid].lpEffect )
//		} // if ( lpid->lpDeviceData[joyid].lpDID )
//
//
//		#endif
//	#elif defined _XBOX
//		float							Ratio;
//
//		if (joyid < 0) 
//			return;
//
//		if (!lpid->lpDeviceData[joyid].bUseVibrate) 
//			return;
//
//		Ratio = (Vibration/10000.0f)*65535.0f;
//		if (lpid->lpDeviceData[joyid].Handle)
//		{
//			if (LYNX_INPUT_REPLAY_INFINITE == replay)
//			{
//				lpid->lpDeviceData[joyid].ForceEffect.Rumble.wLeftMotorSpeed = LYNX_ROUND(Ratio);
//				lpid->lpDeviceData[joyid].ForceEffect.Rumble.wRightMotorSpeed = lpid->lpDeviceData[joyid].ForceEffect.Rumble.wLeftMotorSpeed;
//				lpid->lpDeviceData[joyid].ForceDuringTime = 0;
//			}
//			else
//			{
//				lpid->lpDeviceData[joyid].ForceEffect.Rumble.wLeftMotorSpeed = LYNX_ROUND(Ratio);
//				lpid->lpDeviceData[joyid].ForceEffect.Rumble.wRightMotorSpeed = lpid->lpDeviceData[joyid].ForceEffect.Rumble.wLeftMotorSpeed;
//				lpid->lpDeviceData[joyid].ForceStartTime = lynxClock();
//				lpid->lpDeviceData[joyid].ForceDuringTime = Vtime/400;
//			}
//		}
//	#endif // __DIRECTINPUT__
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void		lynxClearInput(LYNXINPUTTYPE type, void *lpdata)
//{
//	LPLYNXMOUSEINPUTDATA						lpMouseData;
//	LPLYNXKEYBOARDINPUTDATA					lpKeyData;
//
//	switch (type)		
//	{
//		case LYNX_MOUSE_INPUT:
//			lpMouseData = (LPLYNXMOUSEINPUTDATA)lpdata;
//			lpMouseData->bLeftButtonDown = lpMouseData->bRightButtonDown = FALSE;
//			break;
//
//		case LYNX_KEYBOARD_INPUT: 
//			lpKeyData = (LPLYNXKEYBOARDINPUTDATA)lpdata;
//			lpKeyData->bPressed = FALSE; 
//			break;
//	}
//	return;
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// LYNXERRORNO	lynxReadInput(LYNXWINDOW window, LYNXINPUTTYPE type, void *lpdata)
//{	
//	LPLYNXMOUSEINPUTDATA					lpMouseData;
//	LPLYNXKEYBOARDINPUTDATA					lpKeyData;
//	#ifdef WIN32
//		POINT								Point;
//	#endif	
//		
//	switch (type)		
//	{
//		case (LYNX_MOUSE_INPUT) :
//			lpMouseData = (LPLYNXMOUSEINPUTDATA)lpdata;
//
//			#ifdef WIN32
//				#if _WINDOWS || _CONSOLE || (_LIB && !_XBOX)
//					#ifndef __QT__
//						lpMouseData->LastPos.x = lpMouseData->CurrentPos.x;
//						lpMouseData->LastPos.y = lpMouseData->CurrentPos.y;
//						
//						GetCursorPos(&Point);
//
//						lpMouseData->CurrentPos.x = Point.x;
//						lpMouseData->CurrentPos.y = Point.y;
//					#endif
//				#endif
//			#endif		
//		break;
//
//		case (LYNX_KEYBOARD_INPUT) : 
//			lpKeyData = (LPLYNXKEYBOARDINPUTDATA)lpdata;
//
//			#ifdef WIN32
//				#if _WINDOWS || _CONSOLE || (_LIB && !_XBOX)
//					#ifndef __QT__
//						lpKeyData->bPressed = (GetAsyncKeyState(lpKeyData->KeyID) & 0x8000); 					
//					#endif
//				#endif		
//			#endif		
//		break;
//	}
//
//	return LYNX_OK;
//}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 /*
 LYNXBOOL	lynxGetKeyStatus(int k)
{
	#ifdef WIN32
		#if _WINDOWS || _CONSOLE || (_LIB && !_XBOX)
			#ifndef __WINAPP__
				if (GetAsyncKeyState(k) & 0x8000)
					return TRUE;
				else
					return FALSE;
			#else
				return FALSE;
			#endif
		#else
			return FALSE;
		#endif		
	#endif
}
*/
//-------------------------------------------------------------------------------------------------------
 /**
 *  @ingroup LynxIO
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。
 */
 //-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxGetKeyStatus(LYNXWINDOW w, int k)
{
#ifdef WIN32
	#if _WINDOWS || _CONSOLE || (_LIB && !_XBOX)
		#ifndef __WINAPP__
			if (w)
			{
				if ((GetFocus() == w) && (GetAsyncKeyState(k) & 0x8000))
					return TRUE;
				else
					return FALSE;
			}
			else
			{
				if (GetAsyncKeyState(k) & 0x8000)
					return TRUE;
				else
					return FALSE;
			}
		#else
			 return FALSE;
		#endif
	#else
		 return FALSE;
	#endif		
#endif
 }

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 WORD		lynxGetCH(void)
{
	#ifdef WIN32
		#if !defined(__WINAPP__) && (_WINDOWS || _CONSOLE || (_LIB && !_XBOX))
			#ifndef __QT__
				MSG							Msg;

				if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) ) 
				{  			
					TranslateMessage(&Msg);			
					DispatchMessage(&Msg);
					return Msg.wParam;
				}
				else
					return 0;		
			#else
				return 0;
			#endif
		#endif

		return 0;		
	#endif

	return 0;		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 DWORD		lynxClock(void)
{		
	#ifdef __PS2__
		u_long				Clock;
		DWORD				Ret;

		Clock = GetTimerSystemTime();
		Ret = Clock/147456;
		return Ret;
	#elif defined __WINCE__		
		DWORD				T;

		T = GetTickCount();		
		return T;					
	#elif defined __NUCLEUS__
		DWORD				Time, T;
		
		Time = NU_Retrieve_Clock();		

		return Time;				
	#else
		DWORD				Time, T;

		Time = clock();
		T = Time*1000/CLOCKS_PER_SEC;
		return T;					
	#endif
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxSetMultiTask(LYNXBOOL b)
{
	if (b)
	{
		#ifdef __WIN32__
			#if !defined(__WINAPP__) && (_WINDOWS || _CONSOLE || (_LIB && !_XBOX))
				#ifndef __QT__
					BOOL								Old;

					UnregisterHotKey(0, 0);
					UnregisterHotKey(0, 1);
					UnregisterHotKey(0, 2);
					SystemParametersInfo(SPI_SCREENSAVERRUNNING, 0, &Old, 0);
				#endif
			#endif
		#endif
	}
	else
	{
		#ifdef __WIN32__
			#if _WINDOWS || _CONSOLE || (_LIB && !_XBOX)
				#ifndef __WINAPP__
					BOOL								Old;
			
					RegisterHotKey(0, 0, MOD_ALT, VK_TAB);
					RegisterHotKey(0, 1, MOD_ALT, VK_RETURN);
					RegisterHotKey(0, 2, MOD_CONTROL, VK_SPACE);
					SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1, &Old, 0);
				#endif
			#endif
		#endif
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxIO 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
 void	lynxItoA(int num, LYNXCHAR *str, int base)
{
	char Str[256];
	#ifdef __WIN32__
		itoa(num, Str, base);
		lynxFromAnsiStr(str, Str);
	#else
		lynxSprintf(str, _T("%d"), num);
	#endif
}
///*
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////-------------------------------------------------------------------------------------------------------
// LYNXBOOL		lynxGetEvent(void)
//{
//	#ifdef WIN32
//		MSG							Msg;
//		WORD w ;
//
//		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) ) 
//		{  			
//			switch(Msg.message) 
//			{
//				case WM_KEYDOWN:
//					w = 0;	
//					break;
//
//			}
//			TranslateMessage(&Msg);			
//			DispatchMessage(&Msg);
//			return Msg.wParam;
//		}
//		else
//			return 0;		
//	#endif
//}
//*/
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// LYNXBOOL	lynxReadKeyboardData(LPLYNXINPUTDEVICE lpid, BYTE *kbd)
//{    
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//			HRESULT					hRes;
//
//
//			DWORD					Elements;
//
//			if( ! lpid->KeyboardDeviceData.lpDID ) return FALSE;
//    
//			if( lpid->KeyboardDeviceData.Mode == LYNX_KEYBOARD_MODE_IMMEDIATE)
//			{
//				lynxMemSet(kbd, 0, LYNX_KEYBOARDD_ARRAY_SIZE);
//				hRes = lpid->KeyboardDeviceData.lpDID->lpVtbl->GetDeviceState(
//													lpid->KeyboardDeviceData.lpDID,
//													LYNX_KEYBOARDD_ARRAY_SIZE, 
//													kbd);
//			}
//			else if ( lpid->KeyboardDeviceData.Mode == LYNX_KEYBOARD_MODE_BUFFER)
//			{
//				Elements = LYNX_KEYBOARDD_ARRAY_SIZE;
//				hRes = lpid->KeyboardDeviceData.lpDID->lpVtbl->GetDeviceData(
//													lpid->KeyboardDeviceData.lpDID,
//													sizeof(DIDEVICEOBJECTDATA),
//													lpid->KeyboardDeviceData.KeyBuffer, 
//													&Elements, 
//													0);
//			}
//
//			if( hRes != DI_OK ) 
//			{
//				hRes = lpid->KeyboardDeviceData.lpDID->lpVtbl->Acquire(lpid->KeyboardDeviceData.lpDID);
//				while( hRes == DIERR_INPUTLOST ) 
//					hRes = lpid->KeyboardDeviceData.lpDID->lpVtbl->Acquire(lpid->KeyboardDeviceData.lpDID);
//
//				return TRUE;
//			}
//
//			if( FAILED ( hRes ) )  
//				return hRes;
//
//			return TRUE;
//		#endif
//		return TRUE;
//	#else
//		//lynxMemCpy(kbd, LYNX_KBD_ARRAY_SIZE);
//	#endif
//	return TRUE;
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/14/2001	by Jim		取得鍵盤模式
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE		lpid	裝置 id
////
////			out :	char						LYNX_KEYBOARD_MODE_IMMEDIATE	即時模式 (預設)
////												LYNX_KEYBOARD_MODE_BUFFER		緩衝區模式
////
////-------------------------------------------------------------------------------------------------------
// char	lynxGetKeyboardMode(LPLYNXINPUTDEVICE lpid)
//{
//	return ( lpid->KeyboardDeviceData.Mode );
//}
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/14/2001	by Jim		關閉鍵盤裝置
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE		lpid	裝置 id
////
////			out :	
////
////-------------------------------------------------------------------------------------------------------
// void	lynxCloseKeyboard(LPLYNXINPUTDEVICE lpid)
//{
//	#ifdef __DIRECTINPUT__
//	#if	IS_DIRECTINPUT_VERSION8
//
//	if ( ! lpid->lpDI ) return;
//	
//	if( lpid->KeyboardDeviceData.lpDID )
//        lpid->KeyboardDeviceData.lpDID->lpVtbl->Unacquire(lpid->KeyboardDeviceData.lpDID);
//    
//	#endif
//	#endif
//}
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/14/2001	by Jim		開啟鍵盤裝置
////
////-------------------------------------------------------------------------------------------------------
////
//
////			in :	LPLYNXINPUTDEVICE		lpid	裝置 id
////					char				mode	模式
////												LYNX_KEYBOARD_MODE_IMMEDIATE	即時模式 (預設)
////												LYNX_KEYBOARD_MODE_BUFFER		緩衝區模式
////
////			out :	
////
////-------------------------------------------------------------------------------------------------------
// LYNXBOOL	lynxOpenKeyboard(LPLYNXINPUTDEVICE lpid, char mode)
//{
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//			if( FAILED ( lpid->lpDI->lpVtbl->CreateDevice(
//														lpid->lpDI, 
//														&GUID_SysKeyboard, 
//														&lpid->KeyboardDeviceData.lpDID,
//														NULL) ) )
//				return FALSE;
//
//			if( FAILED ( lpid->KeyboardDeviceData.lpDID->lpVtbl->SetDataFormat(
//														lpid->KeyboardDeviceData.lpDID,
//														&c_dfDIKeyboard) ) )
//				return FALSE;
//
//			if( FAILED ( lpid->KeyboardDeviceData.lpDID->lpVtbl->SetCooperativeLevel(
//														lpid->KeyboardDeviceData.lpDID,
//														lynxGetlpRenderer()->SystemData.MainWindow, 
//														DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY) ) )
//														// disable windows key -> | DISCL_NOWINKEY) ) )
//				return FALSE;
//				
//			if( mode == LYNX_KEYBOARD_MODE_IMMEDIATE )
//			{
//				DIPROPDWORD dipdw;
//
//				dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
//				dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
//				dipdw.diph.dwObj        = 0;
//				dipdw.diph.dwHow        = DIPH_DEVICE;
//				dipdw.dwData            = 256; // Arbitary buffer size
//
//				if ( FAILED ( lpid->KeyboardDeviceData.lpDID->lpVtbl->SetProperty(
//														lpid->KeyboardDeviceData.lpDID,
//														DIPROP_BUFFERSIZE,
//														&dipdw.diph) ) )
//					return FALSE;
//
//				lpid->KeyboardDeviceData.Mode = LYNX_KEYBOARD_MODE_IMMEDIATE;
//			}
//			else
//			{
//				lpid->KeyboardDeviceData.Mode = LYNX_KEYBOARD_MODE_BUFFER;
//			}
//
//			lpid->KeyboardDeviceData.lpDID->lpVtbl->Acquire(lpid->KeyboardDeviceData.lpDID);
//
//			return TRUE;
//		#endif
//		return TRUE;
//	#endif // __DIRECTINPUT__
//	return TRUE;
//}
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/10/2001	by Jim		驅動振動效果裝置
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////
////			out :	
////
////-------------------------------------------------------------------------------------------------------
// void	lynxUninitInputDevice(LPLYNXINPUTDEVICE lpid)
//{
//	#ifdef __DIRECTINPUT__
//	#if	IS_DIRECTINPUT_VERSION8
//
//	if ( ! lpid->lpDI ) return;
//
//	lynxCloseKeyboard(lpid);
//	lynxCloseInputDevice(lpid);
//
//	if ( FAILED ( lpid->lpDI->lpVtbl->Release(lpid->lpDI) ) )
//		return;
//
//	lpid->lpDI = NULL;
//	
//	#endif
//	#endif // __DIRECTINPUT__
//}
//
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////			11/14/2001	by Jim		初始化輸入
////
////-------------------------------------------------------------------------------------------------------
////
////			in :	LPLYNXINPUTDEVICE lpid		裝置 id
////
////			out :	
////
////-------------------------------------------------------------------------------------------------------
// void	lynxInitInputDevice(LPLYNXINPUTDEVICE lpid)
//{
//	int									i;
//	#ifdef _XBOX
//		XDEVICE_PREALLOC_TYPE DeviceTypes[] =
//		{
//			{XDEVICE_TYPE_GAMEPAD, 4},
//			{XDEVICE_TYPE_MEMORY_UNIT, 2}
//		};
//	#endif
//
//	lpid->NumDevices = 0;
//	lpid->StartButton = 8;
//	lpid->SelectButton = 9;
//	lpid->OKButton = 1;
//	lpid->CancelButton = 2;	
//
//	#ifdef __DIRECTINPUT__
//		#if	IS_DIRECTINPUT_VERSION8
//			lpid->lpDI = NULL;
//
//			for (i=0; i<LYNX_INPUT_DEVICE_MAX; i++)
//			{
//				lpid->lpDeviceData[i].lpDID = NULL;
//				lpid->lpDeviceData[i].lpEffect = NULL;
//				lynxSetUseVibrate(lpid, i, TRUE);
//			}
//
//			// default keyboard
//			lpid->KeyboardDeviceData.lpDID = NULL;	
//			lpid->KeyboardDeviceData.NumBindData = 0;
//		#endif
//	#elif defined _XBOX		
//		XInitDevices(sizeof(DeviceTypes) / sizeof(XDEVICE_PREALLOC_TYPE), DeviceTypes);	
//
//		for (i=0; i<LYNX_INPUT_DEVICE_MAX; i++)
//		{
//			lpid->lpDeviceData[i].Handle = NULL;
//			lpid->lpDeviceData[i].ForceEffect.Rumble.wLeftMotorSpeed = 0;
//			lpid->lpDeviceData[i].ForceEffect.Rumble.wRightMotorSpeed = 0;
//			lynxSetUseVibrate(lpid, i, TRUE);
//		}
//	#endif // __DIRECTINPUT__
//}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxIO 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
////
////  說明:   
////
////-------------------------------------------------------------------------------------------------------
// void	lynxSetUseVibrate(LPLYNXINPUTDEVICE lpid, short joyid, LYNXBOOL b)
//{
//	if (joyid < 0) return;
//
//	lpid->lpDeviceData[joyid].bUseVibrate = b;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
