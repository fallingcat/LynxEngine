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

#ifndef	__LYNXIO_H__
#define	__LYNXIO_H__

//********************************** Lynx I/O **************************************************************
/**
 *  @defgroup LynxIO <I/O module>
 *
 *	Input/Ouput related functions. Including file I/O, String and keyboard/mouse/joystick/gamepad I/O.
 */
//**********************************************************************************************************
#include <LynxType.h>
#include <time.h>
#include <sys/stat.h>
#ifdef __WIN32__
	#include <direct.h>
#elif defined __iOS__
	//#include <sys/dir.h>
#elif defined __ANDROID__
	LYNXEXTERN LYNXCHAR* gAndroidInternalPath;
#else
#endif

#define	LYNX_OPEN_INPUT_DEVICE_TIMEOUT							500
#define LYNX_JOYSTICK_THRESHOLD									90

#ifdef _XBOX
	#define LYNX_SEEK_SET										FILE_BEGIN
	#define LYNX_SEEK_END										FILE_END
	#define LYNX_SEEK_CUR										FILE_CURRENT
#elif defined __PS2__
	#define LYNX_SEEK_SET										SCE_SEEK_SET
	#define LYNX_SEEK_END										SCE_SEEK_END
	#define LYNX_SEEK_CUR										SCE_SEEK_CUR
#else
	#define LYNX_SEEK_SET										SEEK_SET
	#define LYNX_SEEK_END										SEEK_END
	#define LYNX_SEEK_CUR										SEEK_CUR
#endif

typedef enum {
	LYNX_FILE_STR = 0,
	LYNX_MEMORY_STR,
	LYNX_ZIPFILE_STR,
	LYNX_NUM_FILESTR_TYPE,
}LYNXFILESTRTYPE;

#define	LYNX_FOT_READ											0x01
#define LYNX_FOT_WRITE											0x02
#define	LYNX_FOT_TEXT											0x04										
#define LYNX_FOT_BINARY											0x08
#define	LYNX_FOT_APPEND											0x10

#define LYNX_ASCII                                              0x10
#define LYNX_UTF8                                               0x20
#define LYNX_UTF16                                              0x40
#define LYNX_UTF32                                              0x80

LYNX_CACHE_ALIGN typedef struct {
	char									*Buffer;
	char									*lpC;
	int										Len;
	int										Counter;		
}LYNXMEMSTR, *LPLYNXMEMSTR;
typedef const LYNXMEMSTR* LPCLYNXMEMSTR;

LYNX_CACHE_ALIGN typedef struct {
	#ifdef _XBOX
		HANDLE									fp;	
	#elif defined __PS2__
		int										fp;	
	#else
		FILE									*fp;	
	#endif
}LYNXFILESTR, *LPLYNXFILESTR;
typedef const LYNXFILESTR* LPCLYNXFILESTR;

LYNX_CACHE_ALIGN typedef struct {
	LYNXMEMSTR								MS;	
	unzFile									Zipfp;
}LYNXZIPFILESTR, *LPLYNXZIPFILESTR;
typedef const LYNXZIPFILESTR* LPCLYNXZIPFILESTR;

LYNX_CACHE_ALIGN typedef struct {
	LYNXFILESTRTYPE							Type;
	LYNXFILESTR								FS;
	LYNXMEMSTR								MS;
	LYNXZIPFILESTR							ZFS;
}LYNXFILE, *LPLYNXFILE;
typedef const LYNXFILE* LPCLYNXFILE;

LYNX_CACHE_ALIGN typedef struct {
	int										NumFounds;
	LYNXCHAR								**lpFounds;
}LYNXFINDDATA, *LPLYNXFINDDATA;
typedef const LYNXFINDDATA* LPCLYNXFINDDATA;

typedef enum{
	LYNX_KEYBOARD_INPUT,
	LYNX_MOUSE_INPUT,
	LYNX_JOYSTICK_INPUT,
	LYNX_GAMEPAD_INPUT,
	LYNX_NUM_INPUT_TYPE,
}LYNXINPUTTYPE;

LYNX_CACHE_ALIGN typedef struct {
	LYNXPOINT2D						CurrentPos, LastPos;
	LYNXBOOL						bLeftButtonDown, bRightButtonDown;	
}LYNXMOUSEINPUTDATA, *LPLYNXMOUSEINPUTDATA;
typedef const LYNXMOUSEINPUTDATA* LPCLYNXMOUSEINPUTDATA;

LYNX_CACHE_ALIGN typedef struct {
	int								KeyID;
	LYNXBOOL						bPressed;	
}LYNXKEYBOARDINPUTDATA, *LPLYNXKEYBOARDINPUTDATA;
typedef const LYNXKEYBOARDINPUTDATA* LPCLYNXKEYBOARDINPUTDATA;

#ifdef __XWIN__
	#define VK_LBUTTON        0x01
	#define VK_RBUTTON        0x02
	#define VK_CANCEL         0x03
	#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

	#define VK_BACK           (XK_BackSpace&0xff)
	#define VK_TAB            (XK_Tab&0xff)

	#define VK_CLEAR		  (XK_Clear&0xff)
	#define VK_RETURN         (XK_Return&0xff)

	#define VK_SHIFT          0x10
	#define VK_CONTROL        0x11
	#define VK_MENU           0x12
	#define VK_PAUSE          (XK_Pause&0xff)
	#define VK_CAPITAL        (XK_Capital&0xff)

	#define VK_KANA           0x15
	#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
	#define VK_HANGUL         0x15
	#define VK_JUNJA          0x17
	#define VK_FINAL          0x18
	#define VK_HANJA          0x19
	#define VK_KANJI          0x19

	#define VK_ESCAPE         (XK_Escape&0xff)

	#define VK_CONVERT        0x1C
	#define VK_NONCONVERT     0x1D
	#define VK_ACCEPT         0x1E
	#define VK_MODECHANGE     0x1F

	#define VK_SPACE          (XK_space&0xff)
	#define VK_PRIOR          (XK_Prior&0xff)
	#define VK_NEXT           (XK_Next&0xff)
	#define VK_END            (XK_End&0xff)
	#define VK_HOME           (XK_Home&0xff)
	#define VK_LEFT           (XK_Left&0xff)
	#define VK_UP             (XK_Up&0xff)
	#define VK_RIGHT          (XK_Right&0xff)
	#define VK_DOWN           (XK_Down&0xff)
	#define VK_SELECT         (XK_Select&0xff)
	#define VK_PRINT          (XK_Print&0xff)
	#define VK_EXECUTE        0x2B
	#define VK_SNAPSHOT       0x2C
	#define VK_INSERT         (XK_Insert&0xff)
	#define VK_DELETE         (XK_Delete&0xff)
	#define VK_HELP           (XK_Help&0xff)
	
	#define VK_NUMPAD0        (XK_0&0xff)
	#define VK_NUMPAD1        (XK_1&0xff)
	#define VK_NUMPAD2        (XK_2&0xff)
	#define VK_NUMPAD3        (XK_3&0xff)
	#define VK_NUMPAD4        (XK_4&0xff)
	#define VK_NUMPAD5        (XK_5&0xff)
	#define VK_NUMPAD6        (XK_6&0xff)
	#define VK_NUMPAD7        (XK_7&0xff)
	#define VK_NUMPAD8        (XK_8&0xff)
	#define VK_NUMPAD9        (XK_9&0xff)
	#define VK_MULTIPLY       0x6A
	#define VK_ADD            (XK_plus&0xff)
	#define VK_SEPARATOR      0x6C
	#define VK_SUBTRACT       (XK_minus&0xff)
	#define VK_DECIMAL        0x6E
	#define VK_DIVIDE         0x6F
	#define VK_F1             (XK_F1&0xff)
	#define VK_F2             (XK_F2&0xff)
	#define VK_F3             (XK_F3&0xff)
	#define VK_F4             (XK_F4&0xff)
	#define VK_F5             (XK_F5&0xff)
	#define VK_F6             (XK_F6&0xff)
	#define VK_F7             (XK_F7&0xff)
	#define VK_F8             (XK_F8&0xff)
	#define VK_F9             (XK_F9&0xff)
	#define VK_F10            (XK_F10&0xff)
	#define VK_F11            (XK_F11&0xff)
	#define VK_F12            (XK_F12&0xff)
	#define VK_F13            (XK_F13&0xff)
	#define VK_F14            (XK_F14&0xff)
	#define VK_F15            (XK_F15&0xff)
	#define VK_F16            (XK_F16&0xff)
	#define VK_F17            (XK_F17&0xff)
	#define VK_F18            (XK_F18&0xff)
	#define VK_F19            (XK_F19&0xff)
	#define VK_F20            (XK_F20&0xff)
	#define VK_F21            (XK_F21&0xff)
	#define VK_F22            (XK_F22&0xff)
	#define VK_F23            (XK_F23&0xff)
	#define VK_F24            (XK_F24&0xff)

	#define VK_NUMLOCK        (XK_Num_Lock&0xff)
	#define VK_SCROLL         (XK_Scroll_Lock&0xff)	

	#define VK_LSHIFT         (XK_Shift_L&0xff)
	#define VK_RSHIFT         (XK_Shift_R&0xff)
	#define VK_LCONTROL       (XK_Control_L&0xff)
	#define VK_RCONTROL       (XK_Control_R&0xff)
	#define VK_LMENU          (XK_Alt_L&0xff)
	#define VK_RMENU          (XK_Alt_R&0xff)

#elif (defined __WIN32__ || defined __iOS__ || defined __ANDROID__)

	/*
	 * Virtual Keys, Standard Set
	 */
	#define VK_LBUTTON        0x01
	#define VK_RBUTTON        0x02
	#define VK_CANCEL         0x03
	#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

	#define VK_BACK           0x08
	#define VK_TAB            0x09

	#define VK_CLEAR          0x0C
	#define VK_RETURN         0x0D

	#define VK_SHIFT          0x10
	#define VK_CONTROL        0x11
	#define VK_MENU           0x12
	#define VK_PAUSE          0x13
	#define VK_CAPITAL        0x14

	#define VK_KANA           0x15
	#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
	#define VK_HANGUL         0x15
	#define VK_JUNJA          0x17
	#define VK_FINAL          0x18
	#define VK_HANJA          0x19
	#define VK_KANJI          0x19

	#define VK_ESCAPE         0x1B

	#define VK_CONVERT        0x1C
	#define VK_NONCONVERT     0x1D
	#define VK_ACCEPT         0x1E
	#define VK_MODECHANGE     0x1F

	#define VK_SPACE          0x20
	#define VK_PRIOR          0x21
	#define VK_NEXT           0x22
	#define VK_END            0x23
	#define VK_HOME           0x24
	#define VK_LEFT           0x25
	#define VK_UP             0x26
	#define VK_RIGHT          0x27
	#define VK_DOWN           0x28
	#define VK_SELECT         0x29
	#define VK_PRINT          0x2A
	#define VK_EXECUTE        0x2B
	#define VK_SNAPSHOT       0x2C
	#define VK_INSERT         0x2D
	#define VK_DELETE         0x2E
	#define VK_HELP           0x2F	
	
	#define VK_NUMPAD0        0x60
	#define VK_NUMPAD1        0x61
	#define VK_NUMPAD2        0x62
	#define VK_NUMPAD3        0x63
	#define VK_NUMPAD4        0x64
	#define VK_NUMPAD5        0x65
	#define VK_NUMPAD6        0x66
	#define VK_NUMPAD7        0x67
	#define VK_NUMPAD8        0x68
	#define VK_NUMPAD9        0x69
	#define VK_MULTIPLY       0x6A
	#define VK_ADD            0x6B
	#define VK_SEPARATOR      0x6C
	#define VK_SUBTRACT       0x6D
	#define VK_DECIMAL        0x6E
	#define VK_DIVIDE         0x6F
	#define VK_F1             0x70
	#define VK_F2             0x71
	#define VK_F3             0x72
	#define VK_F4             0x73
	#define VK_F5             0x74
	#define VK_F6             0x75
	#define VK_F7             0x76
	#define VK_F8             0x77
	#define VK_F9             0x78
	#define VK_F10            0x79
	#define VK_F11            0x7A
	#define VK_F12            0x7B
	#define VK_F13            0x7C
	#define VK_F14            0x7D
	#define VK_F15            0x7E
	#define VK_F16            0x7F
	#define VK_F17            0x80
	#define VK_F18            0x81
	#define VK_F19            0x82
	#define VK_F20            0x83
	#define VK_F21            0x84
	#define VK_F22            0x85
	#define VK_F23            0x86
	#define VK_F24            0x87

	#define VK_NUMLOCK        0x90
	#define VK_SCROLL         0x91	

	#define VK_LSHIFT         0xA0
	#define VK_RSHIFT         0xA1
	#define VK_LCONTROL       0xA2
	#define VK_RCONTROL       0xA3
	#define VK_LMENU          0xA4
	#define VK_RMENU          0xA5
#endif

#define	VK_0			0x30
#define	VK_1			0x31
#define	VK_2			0x32
#define	VK_3			0x33
#define	VK_4			0x34
#define	VK_5			0x35
#define	VK_6			0x36
#define	VK_7			0x37
#define	VK_8			0x38
#define	VK_9			0x39

#define	VK_A			0x41
#define	VK_B			0x42
#define	VK_C			0x43
#define	VK_D			0x44
#define	VK_E			0x45
#define	VK_F			0x46
#define	VK_G			0x47
#define	VK_H			0x48
#define	VK_I			0x49
#define	VK_J			0x4A
#define	VK_K			0x4B
#define	VK_L			0x4C
#define	VK_M			0x4D
#define	VK_N			0x4E
#define	VK_O			0x4F
#define	VK_P			0x50
#define	VK_Q			0x51
#define	VK_R			0x52
#define	VK_S			0x53
#define	VK_T			0x54
#define	VK_U			0x55
#define	VK_V			0x56
#define	VK_W			0x57
#define	VK_X			0x58
#define	VK_Y			0x59
#define	VK_Z			0x5A	

#ifdef __DIRECTINPUT__
	#ifdef __DRAGON__
		#define JOYSTICKMAXBUTTONS      (USAGE_LAST_BUTTON - USAGE_FIRST_BUTTON + 1)
		#define JOYSTICK_A              (USAGE_A_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_B              (USAGE_B_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_C              (USAGE_C_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_START          (USAGE_START_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_LA             (USAGE_LA_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_RA             (USAGE_RA_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_DA             (USAGE_DA_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_UA             (USAGE_UA_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_X              (USAGE_X_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_Y              (USAGE_Y_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_Z              (USAGE_Z_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_D              (USAGE_D_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_LB             (USAGE_LB_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_RB             (USAGE_RB_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_DB             (USAGE_DB_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_UB             (USAGE_UB_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_LTRIG          (USAGE_LTRIG_BUTTON - USAGE_FIRST_BUTTON)
		#define JOYSTICK_RTRIG          (USAGE_RTRIG_BUTTON - USAGE_FIRST_BUTTON)
	#else
		#define JOYSTICKMAXBUTTONS      32
		#define JOYSTICK_A              VK_A
		#define JOYSTICK_B              VK_B
		#define JOYSTICK_C              VK_C
		#define JOYSTICK_START          VK_SPACE
		#define JOYSTICK_LA             VK_LEFT
		#define JOYSTICK_RA             VK_RIGHT
		#define JOYSTICK_DA             VK_DOWN
		#define JOYSTICK_UA             VK_UP
		#define JOYSTICK_X              VK_X
		#define JOYSTICK_Y              VK_Y
		#define JOYSTICK_Z              VK_Z
		#define JOYSTICK_D              VK_D
		#define JOYSTICK_LB             VK_F5
		#define JOYSTICK_RB             VK_F6
		#define JOYSTICK_DB             VK_F7
		#define JOYSTICK_UB             VK_F8
		#define JOYSTICK_LTRIG          VK_F9
		#define JOYSTICK_RTRIG          VK_F10
	#endif
#endif

#define		LYNX_INPUT_DEVICE_MAX			16

// 震動幅度
#define		LYNX_INPUT_VIBRATION_NONE		0
#define		LYNX_INPUT_VIBRATION_MAX		10000
#define		LYNX_INPUT_VIBRATION_1			1000
#define		LYNX_INPUT_VIBRATION_2			2000
#define		LYNX_INPUT_VIBRATION_3			3000
#define		LYNX_INPUT_VIBRATION_4			4000
#define		LYNX_INPUT_VIBRATION_5			5000
#define		LYNX_INPUT_VIBRATION_6			6000
#define		LYNX_INPUT_VIBRATION_7			7000
#define		LYNX_INPUT_VIBRATION_8			8000
#define		LYNX_INPUT_VIBRATION_9			9000
#define		LYNX_INPUT_VIBRATION_DEFAULT	LYNX_INPUT_VIBRATION_5	// 預設值

#define		LYNX_INPUT_VTIME_NORMAL			200000
#define		LYNX_INPUT_VTIME_SHORT			1000000		// MAX
#define		LYNX_INPUT_VTIME_LONG			2000

#define		LYNX_INPUT_JOYSTICK_1			0
#define		LYNX_INPUT_JOYSTICK_2			1
#define		LYNX_INPUT_JOYSTICK_3			2
#define		LYNX_INPUT_JOYSTICK_4			3
#define		LYNX_INPUT_JOYSTICK_MAX			4

#define		LYNX_INPUT_REPLAY_NONE			1
#ifdef __DIRECTINPUT__
#define		LYNX_INPUT_REPLAY_INFINITE		INFINITE
#else
#define		LYNX_INPUT_REPLAY_INFINITE		9999
#endif

#define		LYNX_INPUT_EFFECT_HIT			0
#define		LYNX_INPUT_EFFECT_CAMERA		1

#define		LYNX_INPUT_EFFECT_MAX			16

#define		LYNX_KEYBOARD_MODE_IMMEDIATE	1
#define		LYNX_KEYBOARD_MODE_BUFFER		2

#define		LYNX_KEYBOARDD_ARRAY_SIZE		256

LYNX_CACHE_ALIGN typedef struct LynxVibrationPackData{
	DWORD							Type;									
	#ifdef __DIRECTINPUT__
		GUID						Guid;			
		#ifdef __DRAGON__	
			PVIB						VibAPI;	
			VIB_INFO					lpVibInfo[16];
			DWORD						NumInfos, NumSimuSources;
		#endif
	#endif	
}LYNXVIBRATIONPACKDATA, *LPLYNXVIBRATIONPACKDATA;
typedef const LYNXVIBRATIONPACKDATA* LPCLYNXVIBRATIONPACKDATA;

#define		LYNX_GAMEPAD_UP											0
#define		LYNX_GAMEPAD_DOWN										1
#define		LYNX_GAMEPAD_LEFT										2
#define		LYNX_GAMEPAD_RIGHT										3

LYNX_CACHE_ALIGN typedef struct LynxInputData{ 	    
	LONG							lX;                   //X-axis goes here. 
    LONG							lY;                   //Y-axis goes here.     
	LONG							lX2;                  //X2-axis goes here. 
    LONG							lY2;                  //Y2-axis goes here.     
	BYTE							bButton[16];          //Must be DWORD multiple in size.     			
	LYNXBOOL						bButtonLock[16];          
	LYNXBOOL						bGamePadDir[4];	
	LYNXBOOL						bGamePadDirLock[4];	
}LYNXINPUTDATA, *LPLYNXINPUTDATA;  
typedef const LYNXINPUTDATA* LPCLYNXINPUTDATA;

LYNX_CACHE_ALIGN typedef struct LynxInputStatus{ 	    
	LYNXBOOL						bButtonLock[16];          
}LYNXINPUTSTATUS, *LPLYNXINPUTSTATUS;  
typedef const LYNXINPUTSTATUS* LPCLYNXINPUTSTATUS;

LYNX_CACHE_ALIGN typedef struct LynxForceEffect{
	#ifdef __DIRECTINPUT__
		LPDIRECTINPUTEFFECT				lpEffect;
	#else
		LPVOID							lpVoid;
	#endif		
}LYNXFORCEEFFECT, *LPLYNXFORCEEFFECT;
typedef const LYNXFORCEEFFECT* LPCLYNXFORCEEFFECT;

LYNX_CACHE_ALIGN typedef struct LynxInputDeviceData{
	#ifdef __DIRECTINPUT__
		#if	IS_DIRECTINPUT_VERSION8

			GUID							Guid;	
			LPDIRECTINPUTDEVICE8			lpDID;
			DIDATAFORMAT					DataFormat;

			DIDEVCAPS						DevCaps;

			LPDIRECTINPUTEFFECT				lpEffect;
			DIEFFECT						Dieffect;
		#endif
	#elif defined _XBOX
		HANDLE							Handle;
		XINPUT_CAPABILITIES				DevCaps;
		XINPUT_STATE					InputState;
		XINPUT_FEEDBACK 				ForceEffect; 
		DWORD							ForceStartTime, ForceDuringTime;
	#elif defined __PS2__
		u_long128 LYNXALIGNED(64)		PadBuffer[scePadDmaBufferMax];
	#endif
	LYNXBOOL							bUseVibrate;	
	DWORD								NumButtons;	
	DWORD								NumForceFeedBackAxes;
	LYNXVIBRATIONPACKDATA				VibPackData;
	DWORD								ButtonIndex[32];
	LPLYNXINPUTDATA						lpBindData;	
}LYNXINPUTDEVICEDATA, *LPLYNXINPUTDEVICEDATA;
typedef const LYNXINPUTDEVICEDATA* LPCLYNXINPUTDEVICEDATA;

LYNX_CACHE_ALIGN typedef struct LynxKeyboardDeviceData{
	#ifdef __DIRECTINPUT__
		#if	IS_DIRECTINPUT_VERSION8
			LPDIRECTINPUTDEVICE8			lpDID;
			DIDEVICEOBJECTDATA				KeyBuffer[LYNX_KEYBOARDD_ARRAY_SIZE];
		#endif			
	#endif
	char							Mode;	
	short							NumBindData;
	short							BindKeyIndex[LYNX_KEYBOARDD_ARRAY_SIZE];
	BYTE							*lpBindData[LYNX_KEYBOARDD_ARRAY_SIZE];
}LYNXKEYBOARDDEVICEDATA, *LPLYNXKEYBOARDDEVICEDATA;
typedef const LYNXKEYBOARDDEVICEDATA* LPCLYNXKEYBOARDDEVICEDATA;

LYNX_CACHE_ALIGN typedef struct LynxMemoryUnit{
	int								Port;
	#ifdef _XBOX		
		char							Driver[8];
	#endif	
}LYNXMEMORYUNIT, *LPLYNXMEMORYUNIT;
typedef const LYNXMEMORYUNIT* LPCLYNXMEMORYUNIT;

LYNX_CACHE_ALIGN typedef struct LynxInputDevice{
	#ifdef __DIRECTINPUT__
		#if	IS_DIRECTINPUT_VERSION8
			LPDIRECTINPUT8					lpDI;
		#endif
	#endif
	DWORD								NumDevices;	
	LYNXINPUTDEVICEDATA					lpDeviceData[LYNX_INPUT_DEVICE_MAX];
	LYNXKEYBOARDDEVICEDATA				KeyboardDeviceData;
	BYTE								StartButton, SelectButton;
	BYTE								OKButton, CancelButton;
	LYNXMEMORYUNIT						MemoryUnit[8];		
}LYNXINPUTDEVICE, *LPLYNXINPUTDEVICE;
typedef const LYNXINPUTDEVICE* LPCLYNXINPUTDEVICE;

LYNX_CACHE_ALIGN typedef struct LynxInputRegister{
	#ifdef __DIRECTINPUT__
		LPLYNXINPUTDEVICE					lpid;
	#endif	
	WORD								JoyId;
	DWORD								Vibration;
	DWORD								Vtime;
}LYNXINPUTREGISTER, *LPLYNXINPUTREGISTER;
typedef const LYNXINPUTREGISTER* LPCLYNXINPUTREGISTER;

//typedef		LYNXBOOL (* LPLYNXVIDEOEVENTPROC)(LPLYNXSYSTEMDATA lpsd);

#ifdef __cplusplus
	extern "C"	DWORD									gPofileT0;
#else
	extern		DWORD									gPofileT0;
#endif

#define LYNX_PROFILE_START()							gPofileT0 = lynxClock();
#define LYNX_PROFILE_END(t)								t = lynxClock() - gPofileT0;

#define LYNX_OPEN_FILESTR(fs, name)						lynxReadFileToFileStr(fs, name)
#define LYNX_OPEN_TEXT_FILESTR(fs, name)				lynxReadTextFileToFileStr(fs, name)
#define LYNX_CLOSE_FILESTR(fs)							lynxFreeFileStr(fs)		
#define LYNX_READ_FILESTR(lpdst, size, len, fs)			lynxReadFileStr(lpdst, size, len, fs)		

#define LYNX_OPEN_FILE(fs, name, fotype, type)			lynxOpenFile(fs, name, fotype, type)	
#define LYNX_CLOSE_FILE(fs)								lynxCloseFile(fs)		
#define LYNX_READ_FILE(lpdst, size, len, fs)			lynxReadFile(lpdst, size, len, fs)		
#define LYNX_BYTE_ORDER_READ_FILE(lpdst, size, len, fs)	LYNX_READ_FILE(lpdst, size, len, fs); LYNX_BYTE_ORDER(lpdst, size*len)			
#define LYNX_WRITE_FILE(lpsrc, size, len, fs)			lynxWriteFile(lpsrc, size, len, fs)		
#define LYNX_SEEK_FILE(fs, len, flag)					lynxSeekFile(fs, len, flag)		
#define LYNX_TELL_FILE(fs)								lynxTellFile(fs)		
#define LYNX_FILE_GETSTR(str, len, fs)					lynxFileGetStr(str, len, fs)		
#define LYNX_FILE_PUTSTR(str, fs)						lynxFilePutStr(str, fs)		
#define LYNX_GET_FILE_SIZE(fs)							lynxGetFileSize(fs)		

#define	LYNX_MAX_FILE_BLOCK								2048*4	

#define	LYNXGETBUTTONSTATUS(di, i)						(di.bButton[(i)] && !di.bButtonLock[(i)])
#define	LYNXGETGAMEPADDIRSTATUS(di, i)					(di.bGamePadDir[i] && !di.bGamePadDirLock[i])

typedef struct {
	BYTE											Flag;
	LPLYNXFILE										lpFile;
	LYNXCHAR										FileName[64];	
}LYNXFILEBLOCK, *LPLYNXFILEBLOCK;
typedef const LYNXFILEBLOCK* LPCLYNXFILEBLOCK;
	
typedef struct {	
	LYNXFILEBLOCK									lpFileBlock[LYNX_MAX_FILE_BLOCK];			
}LYNXFILEMANAGER, *LPLYNXFILEMANAGER;
typedef const LYNXFILEMANAGER* LPCLYNXFILEMANAGER;

#ifdef	_UNICODE
		LYNXFORCEINLINE LYNXCHAR		lynxFromAnsi(LYNXANSICHAR In)	{ return (LYNXCHAR)In; }
		LYNXFORCEINLINE LYNXCHAR		lynxFromUnicode(LYNXUNICHAR In) { return In; }
		LYNXFORCEINLINE LYNXANSICHAR	lynxToAnsi(LYNXCHAR In)			{ return (WORD)In<0x100 ? In : LYNX_MAXSBYTE; }
		LYNXFORCEINLINE LYNXUNICHAR		lynxToUnicode(LYNXCHAR In)		{ return In; }
#else	/* Not _UNICODE */
		LYNXFORCEINLINE LYNXCHAR		lynxFromAnsi(LYNXANSICHAR In)	{ return In; }
		LYNXFORCEINLINE LYNXCHAR		lynxFromUnicode(LYNXUNICHAR In) { return (WORD)In<0x100 ? In : LYNX_MAXSBYTE; }
		LYNXFORCEINLINE LYNXANSICHAR	lynxToAnsi(LYNXCHAR In)			{ return In; }
		LYNXFORCEINLINE LYNXUNICHAR		lynxToUnicode(LYNXCHAR In)		{ return (LYNXUNICHAR)In; }
#endif

#ifdef __iOS__
    #define LYNX_SCRIPT_TO_C_STR_CONVERT  lynxcStringUTF162UTF32
    #define LYNX_C_TO_SCRIPT_STR_CONVERT  lynxcStringUTF322UTF16
#else
    #define LYNX_SCRIPT_To_C_STR_CONVERT  lynxStrCpy
    #define LYNX_C_TO_SCRIPT_STR_CONVERT  lynxStrCpy
#endif

#ifdef _MSC_VER
	#include <tchar.h>
#elif __GNUC__
	#include <wchar.h>
	#define _fstat      _fstat64i32
	#define _fstati64   _fstat64
	#define _stat       _stat64i32
	#define _stati64    _stat64
	#define _wstat      _wstat64i32
	#define _wstati64   _wstat64

	#ifdef	_UNICODE
		#undef TEXT
		#define _T(x)			L ## x
		
		/*
		* Unicode functions
		*/

		#define _tmkdir			_wmkdir
		#define	_tprintf		wprintf
		#define	_ftprintf		fwprintf
		#define	_stprintf		swprintf
		#define	_sntprintf		_snwprintf
		#define	_vtprintf		vwprintf
		#define	_vftprintf		vfwprintf
		#define	_vstprintf		vswprintf
		#define	_vsntprintf		_vsnwprintf
		#define	_tscanf			wscanf
		#define	_ftscanf		fwscanf
		#define	_stscanf		swscanf
		#define	_fgettc			fgetwc
		#define	_fgettchar		_fgetwchar
		#define	_fgetts			fgetws
		#define	_fputtc			fputwc
		#define	_fputtchar		_fputwchar
		#define	_fputts			fputws
		#define	_gettc			getwc
		#define	_getts			getws
		#define	_puttc			putwc
		#define	_putts			putws
		#define	_ungettc		ungetwc
		#define	_tcstod			wcstod
		#define	_tcstol			wcstol
		#define	_tcstoul		wcstoul
		#define	_tcscat			wcscat
		#define	_tcschr			wcschr
		#define	_tcscmp			wcscmp
		#define	_tcscpy			wcscpy
		#define	_tcscspn		wcscspn
		#define	_tcslen			wcslen
		#define	_tcsncat		wcsncat
		#define	_tcsncmp		wcsncmp
		#define	_tcsncpy		wcsncpy
		#define	_tcspbrk		wcspbrk
		#define	_tcsrchr		wcsrchr
		#define	_tcsspn			wcsspn
		#define	_tcsstr			wcsstr
		#define	_tcstok			wcstok
		#define	_tcsdup			_wcsdup
		#define	_tcsicmp		_wcsicmp
		#define	_tcsnicmp		_wcsnicmp
		#define	_tcsnset		_wcsnset
		#define	_tcsrev			_wcsrev
		#define	_tcsset			_wcsset
		#define	_tcslwr			_wcslwr
		#define	_tcsupr			_wcsupr
		#define	_tcsxfrm		wcsxfrm
		#define	_tcscoll		wcscoll
		#define	_tcsicoll		_wcsicoll
		#define	_istalpha		iswalpha
		#define	_istupper		iswupper
		#define	_istlower		iswlower
		#define	_istdigit		iswdigit
		#define	_istxdigit		iswxdigit
		#define	_istspace		iswspace
		#define	_istpunct		iswpunct
		#define	_istalnum		iswalnum
		#define	_istprint		iswprint
		#define	_istgraph		iswgraph
		#define	_istcntrl		iswcntrl
		#define	_istascii		iswascii
		#define	_totupper		towupper
		#define	_totlower		towlower
		#define	_ttoi			_wtoi
		#define	_tcsftime		wcsftime

		/* Io functions */
		#define _taccess		_waccess
		#define _tchmod			_wchmod
		#define _tcreat			_wcreat
		#define _tfindfirst		_wfindfirst
		#define _tfindfirst64   _wfindfirst64
		#define _tfindfirsti64  _wfindfirsti64
		#define _tfindnext		_wfindnext
		#define _tfindnext64    _wfindnext64
		#define _tfindnexti64   _wfindnexti64
		#define _tmktemp		_wmktemp
		#define _topen			_wopen
		#define _tremove		_wremove
		#define _trename		_wrename
		#define _tsopen			_wsopen
		#define _tunlink		_wunlink

		#define _tfinddata_t    _wfinddata_t
		#define _tfinddata64_t  __wfinddata64_t
		#define _tfinddatai64_t _wfinddatai64_t

		/* Stdio functions */
		#define _tfdopen		_wfdopen
		#define _tfsopen		_wfsopen
		#define _tfopen			fopen
		#define _tfreopen		_wfreopen
		#define _tperror		_wperror
		#define _tpopen			_wpopen
		#define _ttempnam		_wtempnam
		#define _ttmpnam		_wtmpnam

		/* Stat functions */
		#define _tstat      _wstat
		#define _tstat32    _wstat32
		#define _tstat32i64 _wstat32i64
		#define _tstat64    _wstat64
		#define _tstat64i32 _wstat64i32
		#define _tstati64   _wstati64
	#else	/* Not _UNICODE */
		#undef TEXT
		#define _T(x)			x

		/*
		* Non-unicode (standard) functions
		*/

		#define _tmkdir			_mkdir
		#define	_tprintf		printf
		#define _ftprintf		fprintf
		#define	_stprintf		sprintf
		#define	_sntprintf		_snprintf
		#define	_vtprintf		vprintf
		#define	_vftprintf		vfprintf
		#define _vstprintf		vsprintf
		#define	_vsntprintf		vsnprintf
		#define	_tscanf			scanf
		#define	_ftscanf		fscanf
		#define	_stscanf		sscanf
		#define	_fgettc			fgetc
		#define	_fgettchar		_fgetchar
		#define	_fgetts			fgets
		#define	_fputtc			fputc
		#define	_fputtchar		_fputchar
		#define	_fputts			fputs
		#define	_gettc			getc
		#define	_getts			gets
		#define	_puttc			putc
		#define	_putts			puts
		#define	_ungettc		ungetc
		#define	_tcstod			strtod
		#define	_tcstol			strtol
		#define _tcstoul		strtoul
		#define	_tcscat			strcat
		#define _tcschr			strchr
		#define _tcscmp			strcmp
		#define _tcscpy			strcpy
		#define _tcscspn		strcspn
		#define	_tcslen			strlen
		#define	_tcsncat		strncat
		#define	_tcsncmp		strncmp
		#define	_tcsncpy		strncpy
		#define	_tcspbrk		strpbrk
		#define	_tcsrchr		strrchr
		#define _tcsspn			strspn
		#define	_tcsstr			strstr
		#define _tcstok			strtok
		#define	_tcsdup			_strdup
		#define	_tcsicmp		_stricmp
		#define	_tcsnicmp		_strnicmp
		#define	_tcsnset		_strnset
		#define	_tcsrev			_strrev
		#define _tcsset			_strset
		#define	_tcslwr			_strlwr
		#define	_tcsupr			_strupr
		#define	_tcsxfrm		strxfrm
		#define	_tcscoll		strcoll
		#define	_tcsicoll		_stricoll
		#define	_istalpha		isalpha
		#define	_istupper		isupper
		#define	_istlower		islower
		#define	_istdigit		isdigit
		#define	_istxdigit		isxdigit
		#define	_istspace		isspace
		#define	_istpunct		ispunct
		#define	_istalnum		isalnum
		#define	_istprint		isprint
		#define	_istgraph		isgraph
		#define	_istcntrl		iscntrl
		#define	_istascii		isascii
		#define _totupper		toupper
		#define	_totlower		tolower
		#define	_ttoi			atoi
		#define _tcsftime		strftime

		/* Io functions */
		#define _tchmod			_chmod
		#define _tcreat			_creat
		#define _tfindfirst		_findfirst
		#define _tfindfirst64   _findfirst64
		#define _tfindfirsti64  _findfirsti64
		#define _tfindnext		_findnext
		#define _tfindnext64    _findnext64
		#define _tfindnexti64   _findnexti64
		#define _tmktemp		_mktemp

		#ifdef  _POSIX_
		#define _topen			open
		#define _taccess		access
		#else
		#define _topen			_open
		#define _taccess		_access
		#endif

		#define _tremove		remove
		#define _trename		rename
		#define _tsopen			_sopen
		#define _tunlink		_unlink

		#define _tfinddata_t    _finddata_t
		#define _tfinddata64_t  __finddata64_t
		#define _tfinddatai64_t _finddatai64_t

		/* Stdio functions */
		#ifdef  _POSIX_
		#define _tfdopen	    fdopen
		#else
		#define _tfdopen		_fdopen
		#endif
		#define _tfsopen		_fsopen
		#define _tfopen			fopen
		#define _tfreopen		freopen
		#define _tperror		perror
		#define _tpopen			_popen
		#define _ttempnam		_tempnam
		#define _ttmpnam		tmpnam

		/* Stat functions */
		#define _tstat      _stat
		#define _tstat32    _stat32
		#define _tstat32i64 _stat32i64
		#define _tstat64    _stat64
		#define _tstat64i32 _stat64i32
		#define _tstati64   _stati64
	#endif	/* Not _UNICODE */
#endif

#ifdef __cplusplus
extern "C" {
#endif
//###########################################################################
//  String I/O Section
//###########################################################################
LYNXCOREAPI void    		lynxInitIOSystem(void);
LYNXCOREAPI void			lynxShowCursor(LYNXBOOL b);
LYNXCOREAPI void			lynxGetCursorPos(LPLYNXPOINT2D pos);
LYNXCOREAPI void			lynxGetClientCursorPos(LPLYNXPOINT2D pos, LYNXWINDOW wnd);
LYNXCOREAPI void			lynxSetCursorPos(int x, int y);
LYNXCOREAPI void			lynxSetClientCursorPos(int x, int y, LYNXWINDOW wnd);
LYNXCOREAPI LYNXBOOL		lynxGetKeyState(LYNXWINDOW w, int k);
LYNXCOREAPI LYNXBOOL		lynxGetMouseCursorState(LPLYNXPOINT2D pos, LYNXWINDOW wnd);
LYNXCOREAPI int				lynxGetMaxTouches();

LYNXCOREAPI void			lynxDebugOutputPrintf(const LYNXCHAR *fmt, ...);
LYNXCOREAPI void			lynxUniStr(LYNXUNICHAR *lpustr, const LYNXCHAR *lpstr);
LYNXCOREAPI void			lynxAnsiStr(LYNXANSICHAR *lpastr, const LYNXCHAR *lpstr);
LYNXCOREAPI void			lynxFromUniStr(LYNXCHAR *lpustr, const LYNXUNICHAR *lpstr);
LYNXCOREAPI void			lynxFromAnsiStr(LYNXCHAR *lpastr, const LYNXANSICHAR *lpstr);
LYNXCOREAPI int				lynxAtoi(const LYNXCHAR *lpstr);
LYNXCOREAPI long            lynxAtol(const LYNXCHAR *lpstr);
LYNXCOREAPI unsigned long	lynxAtoul(const LYNXCHAR *lpstr);
LYNXCOREAPI LYNXULONGLONG	lynxAtoull(const LYNXCHAR *lpstr);
LYNXCOREAPI float			lynxAtof(const LYNXCHAR *lpstr);
LYNXCOREAPI unsigned int	lynxXtoi(const LYNXCHAR* xs);
LYNXCOREAPI int				lynxStriCmp(const LYNXCHAR *str1, const LYNXCHAR *str2);
LYNXCOREAPI int				lynxStrCmp(const LYNXCHAR *str1, const LYNXCHAR *str2);
LYNXCOREAPI int				lynxStrnCmp(const LYNXCHAR *str1, const LYNXCHAR *str2, const int max);
LYNXCOREAPI LYNXCHAR*		lynxStrStr(const LYNXCHAR* string, const LYNXCHAR* strCharSet);
LYNXCOREAPI LYNXCHAR*		lynxStriStr(const LYNXCHAR* string, const LYNXCHAR* strCharSet);
LYNXCOREAPI LYNXCHAR*		lynxStrCpy(LYNXCHAR *str1, const LYNXCHAR *str2);
LYNXCOREAPI LYNXCHAR*		lynxStrCat(LYNXCHAR *str1, const LYNXCHAR *str2);
LYNXCOREAPI LYNXCHAR*		lynxStrTok(LYNXCHAR* str, const LYNXCHAR *delimiters);
LYNXCOREAPI int				lynxStrLen(const LYNXCHAR *str);
LYNXCOREAPI int				lynxUTF8StrLen(const LYNXCHAR *str);
LYNXCOREAPI LYNXBOOL		lynxIsUnicodeString(const LYNXCHAR *str);
LYNXCOREAPI void			lynxSprintf(LYNXCHAR *buffer, const LYNXCHAR *fmt,...);
LYNXCOREAPI void			lynxVsnPrintf(LYNXCHAR *buffer, size_t max, const LYNXCHAR *fmt, va_list argptr);
LYNXCOREAPI void			lynxStrLower(LYNXCHAR *str);
LYNXCOREAPI LYNXBOOL		lynxFindStrWild(LYNXCHAR *str, LYNXCHAR *find);
LYNXCOREAPI void			lynxGetNameByPath(LYNXCHAR *name, const LYNXCHAR *path);
LYNXCOREAPI void			lynxSeparateFileExtByPath(LYNXCHAR *name, LYNXCHAR *ext, const LYNXCHAR *path);
LYNXCOREAPI void			lynxGetFilenameOnlyByPath(LYNXCHAR *filename, const LYNXCHAR *path);
LYNXCOREAPI void			lynxSeparatePathFilenameExtByFullPath(LYNXCHAR *path, LYNXCHAR *name, LYNXCHAR *ext, const LYNXCHAR *full_path);
LYNXCOREAPI void			lynxRemoveWhiteSpaceAndTab(LYNXCHAR *str);
LYNXCOREAPI DWORD           lynxDecodeUnicode(LYNXCHAR **str_pos, int decoder);
LYNXCOREAPI LYNXBOOL		lynxCopyUnicodeStrByLen(LYNXCHAR *new_str, const LYNXCHAR *src_str, const int len, const int decoder);
LYNXCOREAPI LYNXDWORD		lynxUnicodeStrLen(const LYNXCHAR *str, const int decoder);
LYNXCOREAPI void			lynxModifiedUTF8String(LYNXCHAR *new_str, const LYNXCHAR *src_str);
        
//###########################################################################
//  File I/O Section
//###########################################################################
LYNXCOREAPI void			lynxSetBaseDirectory(const LYNXCHAR *dir);
LYNXCOREAPI void			lynxGetBaseDirectory(LYNXCHAR *dir);

LYNXCOREAPI void			lynxSetCurrentDriverLetter(const LYNXCHAR *drv);
LYNXCOREAPI void			lynxGetCurrentDriverLetter(LYNXCHAR *drv);
LYNXCOREAPI void			lynxSetDefaultDriverLetter(void);

LYNXCOREAPI LYNXBOOL		lynxInitFileIOSystem(void);

LYNXCOREAPI LYNXBOOL		lynxInitMemStr(LPLYNXMEMSTR lpm);
LYNXCOREAPI LYNXBOOL		lynxCreateMemStr(LPLYNXMEMSTR lpm, DWORD size);
LYNXCOREAPI LYNXBOOL		lynxCreateMemStrByArray(LPLYNXMEMSTR lpm, LYNXUCHAR *lpc, DWORD size);
LYNXCOREAPI LYNXBOOL		lynxReadFileToMemStr(LPLYNXMEMSTR lpm, const LYNXCHAR *filename);
LYNXCOREAPI LYNXBOOL		lynxReadFileToMemStrByPointer(LPLYNXFILE lpf, LYNXUCHAR *lpc, DWORD size);
LYNXCOREAPI LYNXBOOL		lynxReadMemoryToMemStr(LPLYNXMEMSTR lpm, void *lpc, DWORD size);
LYNXCOREAPI LYNXBOOL		lynxWriteFileToFileStr(LPLYNXFILESTR lpf, const LYNXCHAR *filename);
LYNXCOREAPI LYNXBOOL		lynxWriteMemStrToFile(const LYNXCHAR *filename, LPLYNXMEMSTR lpm);
LYNXCOREAPI LYNXBOOL		lynxWriteMemStrToFileLowerCase(const LYNXCHAR *filename, LPLYNXMEMSTR lpm);
LYNXCOREAPI int				lynxReadMemStr(void *lpdst, int size, int len, LPLYNXMEMSTR lpm);
LYNXCOREAPI void			lynxWriteMemStr(const void *lpdst, int size, int len, LPLYNXMEMSTR lpm);
LYNXCOREAPI void			lynxFreeMemStr(LPLYNXMEMSTR lpm);
LYNXCOREAPI int				lynxSeekMemStr(LPLYNXMEMSTR lpm, long len, int flag);
LYNXCOREAPI LYNXLONG		lynxTellMemStr(LPLYNXMEMSTR lpm);

LYNXCOREAPI LYNXCHAR*		lynxFileStrGetStr(LYNXCHAR *str, int len, LPLYNXFILESTR lpf);
LYNXCOREAPI int				lynxFileStrPutStr(const LYNXCHAR *str, LPLYNXFILESTR lpf);
LYNXCOREAPI LYNXBOOL		lynxCreateFileStr(LPLYNXFILESTR lpf, DWORD size);
LYNXCOREAPI void			lynxFreeFileStr(LPLYNXFILESTR lpf);
LYNXCOREAPI LYNXBOOL		lynxReadFileToFileStr(LPLYNXFILESTR lpf, const LYNXCHAR *filename);
LYNXCOREAPI LYNXBOOL		lynxReadTextFileToFileStr(LPLYNXFILESTR lpf, const LYNXCHAR *filename);
LYNXCOREAPI int				lynxReadFileStr(void *lpdst, int size, int len, LPLYNXFILESTR lpf);
LYNXCOREAPI int				lynxWriteFileStr(const void *lpsrc, int size, int len, LPLYNXFILESTR lpf);
LYNXCOREAPI int				lynxSeekFileStr(LPLYNXFILESTR lpf, long len, int flag);
LYNXCOREAPI LYNXLONG		lynxTellFileStr(LPLYNXFILESTR lpf);
LYNXCOREAPI DWORD			lynxGetFileStrSize(LPLYNXFILESTR lpf);

LYNXCOREAPI void			lynxSetCurrentZipFile(LPLYNXFILE lpf);
LYNXCOREAPI LPLYNXFILE		lynxGetCurrentZipFile(void);
LYNXCOREAPI void			lynxSetUseZipFile(LYNXBOOL b);
LYNXCOREAPI LYNXBOOL		lynxGetUseZipFile(void);
LYNXCOREAPI void			lynxInitZipFileStr(LPLYNXZIPFILESTR lpzfs);
LYNXCOREAPI LYNXBOOL		lynxOpenZipFileStr(LPLYNXZIPFILESTR lpzfs, const LYNXCHAR *filename);
LYNXCOREAPI LYNXBOOL		lynxOpenZipFileStrByPointer(LPLYNXZIPFILESTR lpzfs, LYNXUCHAR *lpd, DWORD size);
LYNXCOREAPI void			lynxCloseZipFileStr(LPLYNXZIPFILESTR lpzfs);
LYNXCOREAPI void			lynxCloseZipFileStrCreatedByPointer(LPLYNXZIPFILESTR lpzfs);
LYNXCOREAPI LYNXBOOL		lynxOpenZipFileAsDefault(LPLYNXFILE lpf, const LYNXCHAR *filename);
LYNXCOREAPI void			lynxCloseZipFileAsDefault(LPLYNXFILE lpf);
LYNXCOREAPI LYNXBOOL		lynxReadFileToZipFileStr(LPLYNXZIPFILESTR lpzfs, const LYNXCHAR *name);
LYNXCOREAPI LYNXBOOL		lynxReadFileToMemStrFromZipFileStr(LPLYNXMEMSTR lpms, LPLYNXZIPFILESTR lpzfs, const LYNXCHAR *name);
LYNXCOREAPI void			lynxFreeZipFileStr(LPLYNXZIPFILESTR lpzfs);

LYNXCOREAPI void			lynxInitFile(LPLYNXFILE lpf);
LYNXCOREAPI LYNXBOOL		lynxOpenFile(LPLYNXFILE lpf, const LYNXCHAR *name, int opentype, LYNXFILESTRTYPE type);
LYNXCOREAPI void			lynxCloseFile(LPLYNXFILE lpf);
LYNXCOREAPI LYNXBOOL		lynxIsValidFile(LPLYNXFILE lpf);
LYNXCOREAPI int				lynxReadFile(void *lpdst, int size, int len, LPLYNXFILE lpf);
LYNXCOREAPI int				lynxWriteFile(const void *lpsrc, int size, int len, LPLYNXFILE lpf);
LYNXCOREAPI LYNXCHAR*		lynxFileGetStr(LYNXCHAR *str, int len, LPLYNXFILE lpf);
LYNXCOREAPI int				lynxFilePutStr(const LYNXCHAR *str, LPLYNXFILE lpf);
LYNXCOREAPI DWORD			lynxGetFileSize(LPLYNXFILE lpf);
LYNXCOREAPI int				lynxSeekFile(LPLYNXFILE lpf, long len, int flag);
LYNXCOREAPI LYNXLONG		lynxTellFile(LPLYNXFILE lpf);

LYNXCOREAPI void			lynxClearBadFileBlock(void);

LYNXCOREAPI LYNXLONG		lynxFileModifyTime(LYNXCHAR* filename);
LYNXCOREAPI void			lynxDeleteFile(const LYNXCHAR *file);
LYNXCOREAPI void			lynxRenameFile(const LYNXCHAR *new_file, const LYNXCHAR *old_file);
LYNXCOREAPI void			lynxInitFindData(LPLYNXFINDDATA lpd);
LYNXCOREAPI LYNXBOOL		lynxFindFile(const LYNXCHAR *name);
LYNXCOREAPI void			lynxFindFiles(const LYNXCHAR *name, LPLYNXFINDDATA lpd);
LYNXCOREAPI void			lynxFindFolders(const LYNXCHAR *name, LPLYNXFINDDATA lpd);
LYNXCOREAPI void			lynxFreeFounds(LPLYNXFINDDATA lpd);
LYNXCOREAPI void			lynxCreateFolder(const LYNXCHAR *folder);
LYNXCOREAPI void			lynxCopyFile(const LYNXCHAR *dest, const LYNXCHAR *src);

//###########################################################################
//  Memory Unit Section
//###########################################################################
LYNXCOREAPI LYNXBOOL		lynxOpenMemoryUnit(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI void			lynxCloseMemoryUnit(LPLYNXINPUTDEVICE lpid);

//###########################################################################
//  Input Section
//###########################################################################

//LYNXCOREAPI LYNXBOOL		lynxGetKeyStatus(int k);
LYNXCOREAPI LYNXBOOL		lynxGetKeyStatus(LYNXWINDOW w, int k);
LYNXCOREAPI void			lynxInitInputDevice(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI void			lynxUninitInputDevice(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI void			lynxSetUseVibrate(LPLYNXINPUTDEVICE lpid, short joyid, LYNXBOOL b);

LYNXCOREAPI LYNXBOOL		lynxOpenInputDevice(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI LYNXBOOL		lynxCloseInputDevice(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI LYNXBOOL		lynxIsInputDeviceConnect(LPLYNXINPUTDEVICE lpid, short joyid);
LYNXCOREAPI void			lynxBindKey(LPLYNXINPUTDEVICE lpid, BYTE *lpdata, short key);
LYNXCOREAPI void			lynxBindInputData(LPLYNXINPUTDEVICE lpid, LPLYNXINPUTDATA lpidata, short joyid);
LYNXCOREAPI LPLYNXINPUTDATA	lynxGetlpBindInputData(LPLYNXINPUTDEVICE lpid, short joyid);
LYNXCOREAPI void			lynxInitInputData(LPLYNXINPUTDATA lpidata);
LYNXCOREAPI void			lynxGetBindInputData(LPLYNXINPUTDEVICE lpid, short joyid);
LYNXCOREAPI void			lynxGetBindKeyData(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI void			lynxClearInputData(LPLYNXINPUTDATA lpidata);
LYNXCOREAPI void			lynxSetInputData(LPLYNXINPUTDATA lpidata);
LYNXCOREAPI void			lynxNoRepeatInput(LPLYNXINPUTDATA lpid);
LYNXCOREAPI void			lynxNoRepeatButton(LPLYNXINPUTDATA lpid, int b);
LYNXCOREAPI void			lynxNoRepeatGamePadDir(LPLYNXINPUTDATA lpid);
LYNXCOREAPI void			lynxGetInputData(LPLYNXINPUTDEVICE lpid, LPLYNXINPUTDATA lpidata, short joyid);
LYNXCOREAPI void			lynxGetInputDataAll(LPLYNXINPUTDEVICE lpid, LPLYNXINPUTDATA lpidata);
LYNXCOREAPI void			lynxClearInput(LYNXINPUTTYPE type, void *lpdata);
LYNXCOREAPI LYNXERRORNO		lynxReadInput(LYNXWINDOW window, LYNXINPUTTYPE type, void *lpdata);

LYNXCOREAPI void			lynxCreateForceEffect(LPLYNXINPUTDEVICE lpid, short joyid);
LYNXCOREAPI void			lynxCreateForceEffectFromFile(LPLYNXINPUTDEVICE lpid, short joyid, char *fname);
LYNXCOREAPI void			lynxStopAllForceEffect(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI void			lynxStopForceEffect(LPLYNXINPUTDEVICE lpid, short joyid);
LYNXCOREAPI void			lynxPlayForceEffect(LPLYNXINPUTDEVICE lpid, short joyid, DWORD Vibration, DWORD Vtime, DWORD replay);

LYNXCOREAPI LYNXBOOL		lynxReadKeyboardData(LPLYNXINPUTDEVICE lpid, BYTE *kbd);
LYNXCOREAPI char			lynxGetKeyboardMode(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI void			lynxCloseKeyboard(LPLYNXINPUTDEVICE lpid);
LYNXCOREAPI LYNXBOOL		lynxOpenKeyboard(LPLYNXINPUTDEVICE lpid, char mode);


LYNXCOREAPI WORD			lynxGetCH(void);
LYNXCOREAPI DWORD			lynxClock(void);
LYNXCOREAPI void			lynxSetMultiTask(LYNXBOOL b);
LYNXCOREAPI void			lynxItoA(int num, LYNXCHAR *str, int base);

#ifdef __cplusplus
}
#endif

#endif

