#ifdef __LUA__

#ifndef __LYNXLUABUILTINTYPES_H__
#define __LYNXLUABUILTINTYPES_H__

#include <typeinfo>
#include <src/lua.h>
#include <InputSystem/LynxTouch.h>

//using namespace LuaPlus;

#define DECLARE_BUILTINTYPE(type)\
	void Push(lua_State* L, type& value);\
	void Push(lua_State* L, const type& value);\
	bool Match(TypeWrapper<type>, lua_State* L, int idx);\
	bool Match(TypeWrapper<type&>, lua_State* L, int idx);\
	bool Match(TypeWrapper<type*>, lua_State* L, int idx);\
	bool Match(TypeWrapper<const type>, lua_State* L, int idx);\
	bool Match(TypeWrapper<const type&>, lua_State* L, int idx);\
	bool Match(TypeWrapper<const type*>, lua_State* L, int idx);\
	type Get(TypeWrapper<type>, lua_State* L, int idx);\
	type& Get(TypeWrapper<type&>, lua_State* L, int idx);\
	type* Get(TypeWrapper<type*>, lua_State* L, int idx);\
	const type Get(TypeWrapper<const type>, lua_State* L, int idx);\
	const type& Get(TypeWrapper<const type&>, lua_State* L, int idx);\
	const type* Get(TypeWrapper<const type*>, lua_State* L, int idx);
	

namespace LPCD {
	template<class T> struct TypeWrapper;
}

namespace LPCD {
	#if (defined(__GNUC__) && defined(_UNICODE))
		void Push(lua_State* L, const wchar_t* value);
		void Push(lua_State* L, const wchar_t* value, int len);
		bool Match(TypeWrapper<const wchar_t*>, lua_State* L, int idx);
		const wchar_t* Get(TypeWrapper<const wchar_t*>, lua_State* L, int idx);			
	#endif

	DECLARE_BUILTINTYPE(LYNXPOINT2D)
	DECLARE_BUILTINTYPE(LYNXRECT)
	DECLARE_BUILTINTYPE(LYNXCOLORRGBA)
	DECLARE_BUILTINTYPE(LYNXUVPOINT)
	DECLARE_BUILTINTYPE(LynxEngine::Math::CReal)
	DECLARE_BUILTINTYPE(LynxEngine::Math::CVector2)
	DECLARE_BUILTINTYPE(LynxEngine::Math::CVector3)
	DECLARE_BUILTINTYPE(LynxEngine::InputSystem::CTouch::TOUCHDATA)
	DECLARE_BUILTINTYPE(LynxEngine::InputSystem::CTouch::TOUCHGESTURE)
	DECLARE_BUILTINTYPE(LynxEngine::CContainer)
	DECLARE_BUILTINTYPE(LynxEngine::CCameraContainer)	
}

#endif

#endif
