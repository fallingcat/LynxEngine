//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.com
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>

#ifdef __LUA__

#include <LuaPlus.h>

#define BUILTINTYPE(type)\
	namespace LPCD {\
		void Push(lua_State* L, type& value)\
		{\
			LuaState* state = lua_State_To_LuaState(L);\
			LuaObject obj = state->BoxPointer((void*)&value);\
			obj.SetMetaTable(state->GetRegistry()[typeid(type).name()]);\
		}\
		void Push(lua_State* L, const type& value)\
		{\
			LuaState* state = lua_State_To_LuaState(L);\
			LuaObject obj = state->BoxPointer((void*)&value);\
			obj.SetMetaTable(state->GetRegistry()[typeid(type).name()]);\
		}\
		bool	Match(TypeWrapper<type>, lua_State* L, int idx)\
		{\
			LuaState* state = lua_State_To_LuaState(L);\
			LuaObject obj = state->Stack(idx);\
			return obj.GetMetaTable() == state->GetRegistry()[typeid(type).name()];\
		}\
		bool	Match(TypeWrapper<type&>, lua_State* L, int idx)\
		{\
			LuaState* state = lua_State_To_LuaState(L);\
			LuaObject obj = state->Stack(idx);\
			return obj.GetMetaTable() == state->GetRegistry()[typeid(type).name()];\
		}\
		bool	Match(TypeWrapper<const type>, lua_State* L, int idx)\
		{\
			LuaState* state = lua_State_To_LuaState(L);\
			LuaObject obj = state->Stack(idx);\
			return obj.GetMetaTable() == state->GetRegistry()[typeid(type).name()];\
		}\
		bool	Match(TypeWrapper<const type&>, lua_State* L, int idx)\
		{\
			LuaState* state = lua_State_To_LuaState(L);\
			LuaObject obj = state->Stack(idx);\
			return obj.GetMetaTable() == state->GetRegistry()[typeid(type).name()];\
		}\
		bool	Match(TypeWrapper<const type*>, lua_State* L, int idx)\
		{\
			LuaState* state = lua_State_To_LuaState(L);\
			LuaObject obj = state->Stack(idx);\
			return obj.GetMetaTable() == state->GetRegistry()[typeid(type).name()];\
		}\
		type Get(TypeWrapper<type>, lua_State* L, int idx)\
		{\
			return *(type*)lua_unboxpointer(L, idx);\
		}\
		type& Get(TypeWrapper<type&>, lua_State* L, int idx)\
		{\
			return *(type*)lua_unboxpointer(L, idx);\
		}\
		type* Get(TypeWrapper<type*>, lua_State* L, int idx)\
		{\
			return (type*)lua_unboxpointer(L, idx);\
		}\
		const type Get(TypeWrapper<const type>, lua_State* L, int idx)\
		{\
			return *(const type*)lua_unboxpointer(L, idx);\
		}\
		const type& Get(TypeWrapper<const type&>, lua_State* L, int idx)\
		{\
			return *(const type*)lua_unboxpointer(L, idx);\
		}\
		const type* Get(TypeWrapper<const type*>, lua_State* L, int idx)\
		{\
			return (const type*)lua_unboxpointer(L, idx);\
		}\
	}

BUILTINTYPE(LYNXPOINT2D)
BUILTINTYPE(LYNXRECT)
BUILTINTYPE(LYNXCOLORRGBA)
BUILTINTYPE(LYNXUVPOINT)
BUILTINTYPE(LynxEngine::Math::CReal)
BUILTINTYPE(LynxEngine::Math::CVector2)
BUILTINTYPE(LynxEngine::Math::CVector3)
BUILTINTYPE(LynxEngine::InputSystem::CTouch::TOUCHDATA)
BUILTINTYPE(LynxEngine::InputSystem::CTouch::TOUCHGESTURE)
BUILTINTYPE(LynxEngine::CContainer)
BUILTINTYPE(LynxEngine::CCameraContainer)

#if (defined(__GNUC__) && defined(_UNICODE))
	namespace LPCD
	{
        // in 4 bytes wchar system, it converts from UTF32 to UTF16
		void Push(lua_State* L, const wchar_t* value)
		{
            LYNX_LOG(glpLogger, LynxEngine::CString(_T("Push(")) + LynxEngine::CString(value) + LynxEngine::CString(_T(")\r\n")));
            wchar_t Converted[1024];
            LYNX_C_TO_SCRIPT_STR_CONVERT(Converted, value);
            lua_pushwstring(L, (const lua_WChar*)Converted);
			//lua_pushwstring(L, (const lua_WChar*)value);
		}
        // in 4 bytes wchar system, it converts from UTF32 to UTF16
		void Push(lua_State* L, const wchar_t* value, int len)
		{
            LYNX_LOG(glpLogger, LynxEngine::CString(_T("Push(")) + LynxEngine::CString(value) + LynxEngine::CString(_T(")\r\n")));
            wchar_t Converted[1024];
            LYNX_C_TO_SCRIPT_STR_CONVERT(Converted, value);
            lua_pushlwstring(L, (const lua_WChar*)Converted, len);
			//lua_pushlwstring(L, (const lua_WChar*)value, len);
		}
		bool Match(TypeWrapper<const wchar_t*>, lua_State* L, int idx)
		{
			return lua_type(L, idx) == LUA_TWSTRING;  
		}
        // in 4 bytes wchar system, it converts from UTF16 to UTF32
		const wchar_t* Get(TypeWrapper<const wchar_t*>, lua_State* L, int idx)
		{
            LYNX_LOG(glpLogger, LynxEngine::CString(_T("Get(")) + LynxEngine::CString(_T("wstring")) + LynxEngine::CString(_T(")\r\n")));
            
            static wchar_t Converted[16][512];
            static int Index = 0;
            LYNX_SCRIPT_TO_C_STR_CONVERT(Converted[Index], ((const wchar_t*)(lua_towstring(L, idx))));
            int OldIndex = Index;
            Index ++;
            if (Index > 15)
                Index = 0;
            return (const wchar_t*)(Converted[OldIndex]);
            
            
			//return (const wchar_t*)(lua_towstring(L, idx));
		}
	}
#endif

#endif