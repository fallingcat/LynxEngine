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

#ifndef __LYNXERRORS_H__
#define __LYNXERRORS_H__

#include <LynxType.h>
//********************************** Lynx Errors ****************************************************************
/**
 *  @defgroup LynxErrors <Errors Module>
 *
 *	Error handling related functions.
 */
//***************************************************************************************************************

//---------------------------------------------------------------------------
//
//  Error Message :
//
//      0000 ~ 0999 : Lynx System Error
//      1000 ~ 1999 : DirectDraw Errors
//      2000 ~ 2999 : Direct3D   Errors
//      3000 ~ 3999 : DirectPlay Errors
//      4000 ~ 4999 : DirectSound Errors
//      5000 ~ 5999 : OpenGL Errors
//      8000 ~ 8999 : GDI Errors
//	   10000 ~ 10999 : Windows Socket Errors
//	   11000 ~ 11999 : DirectShow Errors
//
//----------------------------------------------------------------------------
enum{
	LYNX_OK = -1,
	//-------------------------------------------------------------------------------------------------------
	//      Lynx System Errors
	//-------------------------------------------------------------------------------------------------------
	ERROR_LYNX_COMMON,
	ERROR_LYNX_NO_SELECTED_DISPLAYMODE, 
	ERROR_LYNX_SET_DISPLAYMODE,
	ERROR_LYNX_OPEN_TEXTURE,
	ERROR_LYNX_FILEFORMAT,
	ERROR_LYNX_NULL_POINTER,
	ERROR_LYNX_CAPTURE_BACKBUFFER,
	ERROR_LYNX_RELEASE_CAPTUREBUFFER,
	ERROR_LYNX_CREATE_BACK_BUFFER,
	ERROR_LYNX_CREATE_ZBUFFER,
	ERROR_LYNX_CREATE_RENDER_BUFFER,
	ERROR_LYNX_CREATE_TEXTURE_BUFFER,		
	ERROR_LYNX_CREATE_CUBE_TEXTURE_BUFFER,		
	ERROR_LYNX_INIT_GRAPHICS,
	ERROR_LYNX_INIT_3DSYSTEM,		
	ERROR_LYNX_UNKNOW_VDOBJECT,		
	ERROR_LYNX_FIND_TEXTUREFROMAT,	
	ERROR_LYNX_LOAD_TEXTURE,
	ERROR_LYNX_TEXTURE_IN_USE,
	ERROR_LOAD_VERTEXSHADER,
	ERROR_COMPILE_VERTEXSHADER,
	ERROR_CREATE_VERTEXSHADER,
	ERROR_LOAD_PIXELSHADER,
	ERROR_COMPILE_PIXELSHADER,
	ERROR_CREATE_PIXELSHADER,
	ERROR_LYNX_OPEN_PALETTE_FILE,
	ERROR_LYNX_RENDER_SPRITE3D,
	ERROR_LYNX_OPEN_W_FILE,	
	ERROR_LYNX_INVALID_W_FILE,
	ERROR_LYNX_INIT_INTERFACE,
	ERROR_LYNX_OUT_OF_MAX_TRIANGLES_IN_RENDER,
	ERROR_LYNX_ALLOCATE_WORLD,										
	ERROR_LYNX_FIND_ZBUFFERFORMAT,		
	ERROR_LYNX_REGISTER_OBJECT_OUT_RANGE,
	ERROR_LYNX_LOAD_SOUND = 300,
	ERROR_LYNX_RELEASE_SOUND,
	//-------------------------------------------------------------------------------------------------------
	//      DirectDraw Errors
	//-------------------------------------------------------------------------------------------------------
	ERROR_DDRAW_CREATE_DDRAW = 1000,
	ERROR_DDRAW_ENUM_DISPLAYMODE,
	ERROR_DDRAW_SET_COOPERATIVELEVEL,
	ERROR_DDRAW_ALLOCATE_PRIMARY,
	ERROR_DDRAW_ALLOCATE_BACK,
	ERROR_DDRAW_CREATE_DCLIPPER,
	ERROR_DDRAW_SET_WIN2_DCLIPPER,
	ERROR_DDRAW_SET_DCLIPPER2PRIMARY,
	ERROR_DDRAW_LOCK_SURFACE,
	ERROR_DDRAW_FLIP,
	ERROR_DDRAW_CREATE_SURFACE,
	ERROR_DDRAW_BLTFAST,
	ERROR_DDRAW_CREATE_PALETTE,
	ERROR_DDRAW_ATTACH_PALETTE_TO_PRIMARYBUFFER,
	ERROR_DDRAW_ATTACH_PALETTE_TO_BACKBUFFER,
	ERROR_DDRAW_BLT,
	ERROR_DDRAW_ATTACH_ZBUFFER,
	ERROR_DDRAW_CREATE_D3DTEXTURE2,
	ERROR_DDRAW_LOAD_TEXTURE,
	//-------------------------------------------------------------------------------------------------------
	//      Direct3D Error
	//-------------------------------------------------------------------------------------------------------
	ERROR_D3D_COMPILE_PIXEL_SHADER = 2000,	
	ERROR_D3D_CREATE_PIXEL_SHADER,
	ERROR_D3D_DELETE_PIXEL_SHADER,
	ERROR_D3D_SET_PIXEL_SHADER,
	ERROR_D3D_COMPILE_VERTEX_SHADER,
	ERROR_D3D_CREATE_VERTEX_SHADER,
	ERROR_D3D_DELETE_VERTEX_SHADER,
	ERROR_D3D_SET_VERTEX_SHADER,
	ERROR_D3D_CREATE_VERTEX_SHADER_DECL,
	ERROR_D3D_SET_VERTEX_SHADER_DECL,
	ERROR_D3D_CREATE_VERTEXBUFFER,
	ERROR_D3D_RELEASE_VERTEXBUFFER,
	ERROR_D3D_LOCK_VERTEXBUFFER,
	ERROR_D3D_CREATE_INDEXBUFFER,
	ERROR_D3D_RELEASE_INDEXBUFFER,
	ERROR_D3D_LOCK_INDEXBUFFER,
	//-------------------------------------------------------------------------------------------------------
	//      DirectPlay Error
	//-------------------------------------------------------------------------------------------------------
	ERROR_DPLAY_CREATE_DDPLAY = 3000,
	ERROR_DPLAY_CREATE_DDPLAYLOBBY,
	ERROR_DPLAY_INIT_CONNECTION,
	ERROR_DPLAY_OPEN_SESSION,
	ERROR_DPLAY_CLOSE_SESSION,
	ERROR_DPLAY_CREATE_PLAYER,
	ERROR_DPLAY_SEND,
	ERROR_DPLAY_RECEIVE,
	ERROR_DPLAY_DESTROY_PLAYER,
	//-------------------------------------------------------------------------------------------------------
	//      DirectSound Errors
	//-------------------------------------------------------------------------------------------------------
	ERROR_DSOUND_CREATE_DSOUND = 4000,
	//-------------------------------------------------------------------------------------------------------
	//      OpenGL Errors
	//-------------------------------------------------------------------------------------------------------
	ERROR_OPENGL_CREATE_RC = 5000,	
	//-------------------------------------------------------------------------------------------------------
	//      GDI Errors
	//-------------------------------------------------------------------------------------------------------
	ERROR_GDI_SET_DISPLAYMODE = 8000,	
	//-------------------------------------------------------------------------------------------------------
	//      Windows Socket Errors
	//-------------------------------------------------------------------------------------------------------
	ERROR_WINSOCKET_STRATUP	= 10000,
	ERROR_XBOX_NETSTARTUP,
	ERROR_WINSOCKET_CONNECTION,
	ERROR_WINSOCKET_OPEN_SESSION,
	ERROR_WINSOCKET_JOIN_SESSION,
	ERROR_WINSOCKET_NO_MORE_PLAYER,
	ERROR_WINSOCKET_SEND,
	ERROR_WINSOCKET_RECEIVE,
	ERROR_WINSOCKET_CREATE_PLAYER,
	ERROR_WINSOCKET_DESTORY_PLAYER,
	ERROR_WINSOCKET_GET_PLAYER_LIST,
	//-------------------------------------------------------------------------------------------------------
	//      Direct Show Errors
	//-------------------------------------------------------------------------------------------------------
	ERROR_DSHOW_CREATE_GB =	11000,
	//-------------------------------------------------------------------------------------------------------
	//      XBOX WMV Errors
	//-------------------------------------------------------------------------------------------------------
	ERROR_XBOX_CREATE_DECODER =	12000,
	ERROR_UNKNOW = 15000,
};

LYNXCOREAPI void lynxGetErrorString(char *str, LYNXERRORNO error);

#endif


