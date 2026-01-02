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
//  Created by Owen Wu : 2006/06/20
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>

#ifdef __LUA__	

	#include <LuaPlus.h>
	//#include <LuaPlusAddons.h>
	#include <LynxCore.h>
	#include <LynxLuaVMachine.h>
	#include <LynxLuaScript.h>
	#include <LynxGameObj.h>
	#include <LynxLuaBuiltinTypes.h>
	#include <InputSystem/LynxTouch.h>


	extern "C" int luaopen_bit(lua_State *L);

	using namespace LuaPlus;


	#if ((defined(DEBUG) || defined(_DEBUG)))
		#define __LUA_DEBUG__					0
	#else
		#define __LUA_DEBUG__					0
	#endif

	namespace LynxScript 
	{
		namespace Lua 
		{	
			class CBuiltinTypes {
			public:
				LYNXPOINT2D					POINT2D;
				LYNXRECT					RECT;
				LYNXCOLORRGBA				COLORRGBA;
				LYNXUVPOINT					UVPOINT;	
				LynxEngine::Math::CReal		Real;
				LynxEngine::Math::CVector2	Vector2;			
				LynxEngine::Math::CVector3	Vector3;		
				LynxEngine::InputSystem::CTouch::TOUCHDATA TOUCHDATA;
			};

			static CBuiltinTypes		gBuiltinTypes;
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			int LS_IsNull(LuaState* state)
			{
				LuaStack args(state);

				LYNX_ASSERT(args[1].IsUserData());
				bool bRet = (args[1].GetLuaPointer()) ? false : true;
				state->PushBoolean(bRet);		

				// 1 return value.
				return 1;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			int LS_GetPerfTime(LuaState* state)
			{
				float Ret = (lynxOSGetPerformanceCounter()* 1000.0f / ((float)lynxOSGetPerformanceFrequency()));
				state->PushNumber(Ret);
				
				// 1 return value.
				return 1;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			int LS_QuickSin(LuaState* state)
			{
				LuaStack args(state);

				LYNX_ASSERT(args[1].IsNumber());
				LYNXREAL Ret = lynxQuickSin(args[1].GetFloat());
				state->PushNumber(Ret);
				
				// 1 return value.
				return 1;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			int LS_QuickCos(LuaState* state)
			{
				LuaStack args(state);

				LYNX_ASSERT(args[1].IsNumber());
				LYNXREAL Ret = lynxQuickCos(args[1].GetFloat());
				state->PushNumber(Ret);
				
				// 1 return value.
				return 1;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			int LS_CReal(LuaState* state)
			{
				//void* Ret = LYNXNEW LynxEngine::Math::CReal();			
				void* Ret = LYNXGLOBAL_NEW CBuiltinTypes();			
				state->PushLightUserData(Ret);
				LYNXGLOBAL_DEL Ret;
				
				// 1 return value.
				return 1;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------

			CVMachine::LIBDATA gSystemLibData[] = { {"GetPerfTime",		LS_GetPerfTime},
			};											  
			
			CVMachine::LIBDATA gMathLibData[] = { {"QuickSin",			LS_QuickSin},
												  {"QuickCos",			LS_QuickCos},	
			};
			//CVMachine::LIBDATA gIOLibData[] =	{
			//};
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void *lynxLuaAlloc(void *ud, void *ptr, size_t osize, size_t nsize, const char* allocName, unsigned int flags) 
			{
				(void)osize;
				(void)ud;
				(void)allocName;
				(void)flags;
				if (nsize == 0 && ptr) 
				{
					//LynxEngine_Free(ptr, osize);
					//LynxEngine_Free(ptr);
					free(ptr);
					return NULL;
				}
				else
				{
					//if (ptr)
					//	LynxEngine_Free(ptr, osize);
					//return LynxEngine_Alloc(nsize, __FILE__, allocName, __LINE__);
					return realloc(ptr, nsize);				
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void luaHelper_FreeFunction(void* ptr, int oldsize, void* data)
			{
				//LynxEngine_Free(ptr, oldsize);
				//LynxEngine_Free(ptr);
				free(ptr);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CVMachine::CVMachine(LynxEngine::ScriptSystem::CScriptSystem* lps)		
			: LynxEngine::ScriptSystem::CVMachine(lps)
			{			
				m_State = NULL;			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			CVMachine::~CVMachine(void)
			{
				vDestory();
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CVMachine::vCreate(int size)		
			{
				lua_Alloc OldAlloc = NULL;
				void* OldUserData = NULL;

				//lua_getdefaultallocfunction(&OldAlloc, &OldUserData);			
				//lua_setdefaultallocfunction(lynxLuaAlloc, OldUserData);			

				m_State = LuaState::Create(true);
				if (!m_State)
					return LYNX_FALSE;				

				// Register built-in types
				// 註冊內建資料型別
				//------------------------------------------------------------------------------------------------
				// Register LYNXPOINT2D type
				LuaObject Point2DMetaTableObj = m_State->GetRegistry().CreateTable(typeid(LYNXPOINT2D).name()); 
				LPCD::MetaTable_IntegratePropertySupport(Point2DMetaTableObj); 	
				LPCD::PropertyCreate(Point2DMetaTableObj, "x", &LYNXPOINT2D::x); 
				LPCD::PropertyCreate(Point2DMetaTableObj, "y", &LYNXPOINT2D::y); 			
				//------------------------------------------------------------------------------------------------
				// Register LYNXRECT type
				LuaObject RectMetaTableObj = m_State->GetRegistry().CreateTable(typeid(LYNXRECT).name()); 
				LPCD::MetaTable_IntegratePropertySupport(RectMetaTableObj); 
				LPCD::PropertyCreate(RectMetaTableObj, "x1", &LYNXRECT::x1); 
				LPCD::PropertyCreate(RectMetaTableObj, "y1", &LYNXRECT::y1); 
				LPCD::PropertyCreate(RectMetaTableObj, "x2", &LYNXRECT::x2); 
				LPCD::PropertyCreate(RectMetaTableObj, "y2", &LYNXRECT::y2); 
				//------------------------------------------------------------------------------------------------
				// Register LYNXCOLOORGA type
				LuaObject ColorRGBAMetaTableObj = m_State->GetRegistry().CreateTable(typeid(LYNXCOLORRGBA).name()); 
				LPCD::MetaTable_IntegratePropertySupport(ColorRGBAMetaTableObj); 
				LPCD::PropertyCreate(ColorRGBAMetaTableObj, "Red", &LYNXCOLORRGBA::Red); 
				LPCD::PropertyCreate(ColorRGBAMetaTableObj, "Green", &LYNXCOLORRGBA::Green); 
				LPCD::PropertyCreate(ColorRGBAMetaTableObj, "Blue", &LYNXCOLORRGBA::Blue); 
				LPCD::PropertyCreate(ColorRGBAMetaTableObj, "Alpha", &LYNXCOLORRGBA::Alpha); 
				//------------------------------------------------------------------------------------------------
				// Register LYNXCOLOORGA type
				LuaObject UVPointMetaTableObj = m_State->GetRegistry().CreateTable(typeid(LYNXUVPOINT).name()); 
				LPCD::MetaTable_IntegratePropertySupport(UVPointMetaTableObj); 
				LPCD::PropertyCreate(UVPointMetaTableObj, "u", &LYNXUVPOINT::u); 
				LPCD::PropertyCreate(UVPointMetaTableObj, "v", &LYNXUVPOINT::v); 				
				//------------------------------------------------------------------------------------------------
				// Register CReal type
				LuaObject RealMetaTableObj = m_State->GetRegistry().CreateTable(typeid(LynxEngine::Math::CReal).name()); 
				LPCD::MetaTable_IntegratePropertySupport(RealMetaTableObj); 	
				LPCD::PropertyCreate(RealMetaTableObj, "r", &LynxEngine::Math::CReal::r); 
				//------------------------------------------------------------------------------------------------
				// Register CVector2 type
				LuaObject Vector2MetaTableObj = m_State->GetRegistry().CreateTable(typeid(LynxEngine::Math::CVector2).name()); 
				LPCD::MetaTable_IntegratePropertySupport(Vector2MetaTableObj); 	
				LPCD::PropertyCreate(Vector2MetaTableObj, "x", &LynxEngine::Math::CVector2::x); 
				LPCD::PropertyCreate(Vector2MetaTableObj, "y", &LynxEngine::Math::CVector2::y); 			
				//------------------------------------------------------------------------------------------------
				// Register CVector3 type
				LuaObject Vector3MetaTableObj = m_State->GetRegistry().CreateTable(typeid(LynxEngine::Math::CVector3).name()); 
				LPCD::MetaTable_IntegratePropertySupport(Vector3MetaTableObj); 	
				LPCD::PropertyCreate(Vector3MetaTableObj, "x", &LynxEngine::Math::CVector3::x); 
				LPCD::PropertyCreate(Vector3MetaTableObj, "y", &LynxEngine::Math::CVector3::y); 
				LPCD::PropertyCreate(Vector3MetaTableObj, "z", &LynxEngine::Math::CVector3::z); 
				//------------------------------------------------------------------------------------------------
				// Register Touch Data
				LuaObject TouchDataMetaTableObj = m_State->GetRegistry().CreateTable(typeid(LynxEngine::InputSystem::CTouch::TOUCHDATA).name()); 
				LPCD::MetaTable_IntegratePropertySupport(TouchDataMetaTableObj); 	
				LPCD::PropertyCreate(TouchDataMetaTableObj, "TimeStamp", &LynxEngine::InputSystem::CTouch::TOUCHDATA::TimeStamp); 
				LPCD::PropertyCreate(TouchDataMetaTableObj, "TapCount", &LynxEngine::InputSystem::CTouch::TOUCHDATA::TapCount); 
				LPCD::PropertyCreate(TouchDataMetaTableObj, "Pos", &LynxEngine::InputSystem::CTouch::TOUCHDATA::Pos); 			

				//------------------------------------------------------------------------------------------------			
				LuaObject BuitinTypesMetaTableObj = m_State->GetRegistry().CreateTable("CBuiltinTypes"); 
				LPCD::MetaTable_IntegratePropertySupport(BuitinTypesMetaTableObj); 
				LPCD::PropertyCreate(BuitinTypesMetaTableObj, "POINT2D", &CBuiltinTypes::POINT2D);
				LPCD::PropertyCreate(BuitinTypesMetaTableObj, "RECT", &CBuiltinTypes::RECT);
				LPCD::PropertyCreate(BuitinTypesMetaTableObj, "COLORRGBA", &CBuiltinTypes::COLORRGBA);
				LPCD::PropertyCreate(BuitinTypesMetaTableObj, "UVPOINT", &CBuiltinTypes::UVPOINT);
				LPCD::PropertyCreate(BuitinTypesMetaTableObj, "Real", &CBuiltinTypes::Real);			
				LPCD::PropertyCreate(BuitinTypesMetaTableObj, "Vector2", &CBuiltinTypes::Vector2);
				LPCD::PropertyCreate(BuitinTypesMetaTableObj, "Vector3", &CBuiltinTypes::Vector3);
				LPCD::PropertyCreate(BuitinTypesMetaTableObj, "TOUCHDATA", &CBuiltinTypes::TOUCHDATA);

				LuaObject LynxBuiltinTypesObj = m_State->GetGlobals().CreateTable("BuiltinTypes");
				LynxBuiltinTypesObj.SetLightUserData("__object", &gBuiltinTypes);
				LynxBuiltinTypesObj.SetMetaTable(BuitinTypesMetaTableObj);

				//LuaObject RealVarObj = m_State->GetGlobals().CreateTable("RealVar");
				//RealVarObj.SetLightUserData("__object", &gBuiltinTypes.Real);
				//RealVarObj.SetMetaTable(RealMetaTableObj);

				// load builtin type conversion functions

				LYNXBOOL bFoundScript = LYNX_FALSE;
				#if ((__LUA_DEBUG__) && defined(__WIN32__))
					for (LynxEngine::CList<LynxEngine::CString>::CIterator SrcPath = m_lpScriptSystem->GetScriptPathList().begin(); SrcPath != m_lpScriptSystem->GetScriptPathList().end(); SrcPath++)
					{
						LynxEngine::CAnsiString Filename = (*Path) + LynxEngine::CAnsiString("BuiltinTypes.lua");
						if (((CVMachine*)m_lpScriptSystem->GetlpVMachine())->GetState()->DoFile(Filename) == 0)							
						{
							bFoundScript = LYNX_TRUE;
							break;
						}
					}
					if (!bFoundScript)
						return LYNX_FALSE;
				#else
					char* String = NULL; 
					LynxEngine::CPlatformFileStream FileStream;
					LynxEngine::CStream* lpStream;
					
					if (m_lpEngine->GetlpFileSystem())
						lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
					else
						lpStream = &FileStream;

					for (LynxEngine::CList<LynxEngine::CString>::CIterator SrcPath = m_lpScriptSystem->GetScriptPathList().begin(); SrcPath != m_lpScriptSystem->GetScriptPathList().end(); SrcPath++)
					{
						LynxEngine::CString Filename = (*SrcPath) + LynxEngine::CString("BuiltinTypes.lua");
						if (lpStream->vOpen(Filename, LynxEngine::CStream::Binary|LynxEngine::CStream::Read))
						{
							bFoundScript = LYNX_TRUE;
							LYNXLONG Len = lpStream->vSize();
							String = LYNXGLOBAL_NEW char[Len+2];
							lpStream->vRead(String, sizeof(char), Len);
							String[Len] = NULL;
							lpStream->vClose();
							break;
						}
					}
					if (!bFoundScript)
						return LYNX_FALSE;
					
					LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Read script OK\r\n")));

					if (m_State->DoString(String) != 0)
					{
						LYNXGLOBAL_DEL_ARRAY(String);
						return LYNX_FALSE;
					}
					LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Run script OK\r\n")));
								
					LYNXGLOBAL_DEL_ARRAY(String);
				#endif

				// load lua class() functionchar* String;
				/*
				char* String; 
				LynxEngine::CPlatformFileStream FileStream;
				if (!FileStream.vOpen("../../script/Class.lua", LynxEngine::CStream::Binary|LynxEngine::CStream::Read))
				{
					if (!FileStream.vOpen("../script/Class.lua", LynxEngine::CStream::Binary|LynxEngine::CStream::Read))
						return LYNX_FALSE;
				}			
				LYNXLONG Len = FileStream.vSize();
				String = LYNXNEW char[Len+2];
				FileStream.vRead(String, sizeof(char), Len);
				String[Len] = NULL;
				FileStream.vClose();
				
				if (m_State->DoString(String) != 0)
					return LYNX_FALSE;
							
				LYNXDEL [] String;
				*/
				
				//::luaopen_bit(LuaState_to_lua_State(m_State));

				//m_State->GetGlobals().Register("bit", bit_funcs);

				m_State->GetGlobals().Register("IsNull", LS_IsNull);			
				//m_State->GetGlobals().Register("CreatCReal", LS_CReal);			

				LuaObject NativeLib = m_State->GetGlobals().CreateTable("NativeLib");
				m_NativeLib.System = NativeLib.CreateTable("System");			
				for (int i=0; i<LYNX_ARRAY_SIZE(gSystemLibData); i++)
					m_NativeLib.System.Register(gSystemLibData[i].Name.c_str(), gSystemLibData[i].Func);			

				m_NativeLib.Math = NativeLib.CreateTable("Math");			
				for (int i=0; i<LYNX_ARRAY_SIZE(gMathLibData); i++)
					m_NativeLib.Math.Register(gMathLibData[i].Name.c_str(), gMathLibData[i].Func);			

				//m_State->GetGlobals().Register("CreatCReal", LS_CReal);		

				LYNX_LOG(glpLogger, LynxEngine::CString(GetClassName())+LynxEngine::CString(_T("::Create OK\r\n")));
			
				return LYNX_TRUE;
			}		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CVMachine::vDestory()
			{
				if (m_State)
				{
					LuaState::Destroy(m_State);
					m_State = NULL;
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LynxEngine::ScriptSystem::CScript* const CVMachine::vCreateScript() 
			{
				return LYNXNEW CScript(m_lpScriptSystem); 
			}				
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------		
			void CVMachine::vDumpAllScripts(const LYNXCHAR* filename)
			{
				LynxEngine::CAnsiString Filename(filename);
				LynxEngine::CList<LynxEngine::ScriptSystem::CScriptPtr> SList = m_lpScriptSystem->GetScriptList();
				LynxEngine::CList<LynxEngine::ScriptSystem::CScriptPtr>::CIterator Script;

				for (Script=SList.begin(); Script!=SList.end(); ++Script)
				{
					m_State->DumpObject(Filename.c_str(), ((CScript*)&(*Script))->m_ClassMetaTable, ((CScript*)&(*Script))->m_ClassObjectTable, LuaPlus::LuaState::DUMP_WRITEALL|LuaPlus::LuaState::DUMP_ALPHABETICAL);
				}			
			}
			////-------------------------------------------------------------------------------------------------------
			////
			////  說明:   
			////-------------------------------------------------------------------------------------------------------		
			//LynxEngine::ScriptSystem::CScriptClass* const CVMachine::vCreateClass()				
			//{
			//	return LYNXNEW CScriptClass(this); 
			//}	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			LYNXBOOL CVMachine::vRegisterDefaultClasses()
			{
				/*LynxEngine::CObj Obj;
				LynxEngine::CGameObj GObj;

				LynxEngine::ScriptSystem::CClassObj* pClass = AddClass();
				pClass->SetName(Obj.GetClassName());
				pClass->vRegisterClass(LynxEngine::CString(TEXT("CObj")));
				((CClassObj*)pClass)->RegisterMethod("SetName", (LynxEngine::CObj*)0, &LynxEngine::CObj::Script_SetName);		

				LynxEngine::ScriptSystem::CClassObj* pGOClass = AddClass();
				pGOClass->SetName(GObj.GetClassName());
				pGOClass->vRegisterClass(LynxEngine::CString(TEXT("CGameObj")));
				((CClassObj*)pGOClass)->RegisterMethod("SetName", (LynxEngine::CGameObj*)0, (void (LynxEngine::CGameObj::*)(const LYNXCHAR *))&LynxEngine::CGameObj::Script_SetName);			
				((CClassObj*)pGOClass)->RegisterMethod("GetNumChildren", (LynxEngine::CGameObj*)0, &LynxEngine::CGameObj::GetNumChildren);			


				pClass->vSet(dynamic_cast<LynxEngine::CObj*>(&Obj));
				m_State->DoFile("../script/test.lua");
				LuaFunction<int>* m_OnInit = LYNXNEW LuaFunction<int>(m_State->GetGlobal("OnInit"));
				int xx = (*m_OnInit)();

				GObj.SetNumChildren(100);
				pGOClass->vSet(&GObj);
				m_State->DoFile("../script/test2.lua");
				LuaFunction<int>* m_OnInit2 = LYNXNEW LuaFunction<int>(m_State->GetGlobal("OnInit"));
				xx = (*m_OnInit2)();			*/

				return LYNX_TRUE;
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void CVMachine::RegisterFunction()//LynxEngine::CString& s, void* f) 
			{
				//LuaObject ObjectMetaTable = m_State->GetGlobals().CreateTable("ObjectMetaTable");
				//LPCD::MetaTable_IntegratePropertySupport(ObjectMetaTable);
				//ObjectMetaTable.SetObject("__index", ObjectMetaTable);			
				//ObjectMetaTable.RegisterObjectDirect("Test", (CVMachine*)0, &CVMachine::Test);
				//ObjectMetaTable.RegisterDirect("Test2", *this, &CVMachine::Test2);			
				//LPCD::PropertyCreate(ObjectMetaTable, "m_Value", &CVMachine::m_Value);


				////LuaObject vectorMetaTableObj = m_State->GetRegistry().CreateTable("CVector3"); 
				////LPCD::MetaTable_IntegratePropertySupport(vectorMetaTableObj); 
				////LPCD::PropertyCreate(vectorMetaTableObj, "x", &LynxSDK::Math::CVector3::v[0]); 
				////LPCD::PropertyCreate(vectorMetaTableObj, "y", &LynxSDK::Math::CVector3::s::y); 
				////LPCD::PropertyCreate(vectorMetaTableObj, "z", &LynxSDK::Math::CVector3::s.z); 
				////LuaObject ObjectMetaTable = m_State->GetRegistry().CreateTable("CVMachine"); 
				////LPCD::MetaTable_IntegratePropertySupport(ObjectMetaTable); 
				////LPCD::PropertyCreate(ObjectMetaTable, "m_Pos", &CVMachine::m_Pos); 

				//LuaObject GameObj = m_State->GetGlobals().CreateTable("GameObj");
				//GameObj.SetLightUserData("__object", this);
				//GameObj.SetMetaTable(ObjectMetaTable);

				////m_State->DoFile("../script/test.lua");
				////LuaFunction<int>* m_OnInit = LYNXNEW LuaFunction<int>(m_State->GetGlobal("OnInit"));


				/////*LuaObject metaTableObj = m_State->GetGlobals().CreateTable("MultiObjectMetaTable");
				////metaTableObj.SetObject("__index", metaTableObj);
				////metaTableObj.RegisterObjectFunctor("Print", &MultiObject::Print);
				////metaTableObj.RegisterObjectDirect("Print2", (CVMachine*)0, &CVMachine::Test);

				////LuaObject obj1Obj = m_State->BoxPointer(this);
				////obj1Obj.SetMetaTable(metaTableObj);
				////m_State->GetGlobals().SetObject("obj1", obj1Obj);			

				/*LynxEngine::CGameObj Obj, Obj2;

				LynxEngine::ScriptSystem::CClassObj* pClass = AddClass();
				pClass->vRegisterClass(LynxEngine::CString(TEXT("CObj")));
				((CClassObj*)pClass)->RegisterMethod("SetName", (LynxEngine::CObj*)0, &LynxEngine::CObj::Script_SetName);			

				LynxEngine::ScriptSystem::CClassObj* pGOClass = AddClass();
				pGOClass->vRegisterClass(LynxEngine::CString(TEXT("CGameObj")));
				((CClassObj*)pGOClass)->RegisterMethod("SetName", (LynxEngine::CGameObj*)0, (void (LynxEngine::CGameObj::*)(const LYNXCHAR *))&LynxEngine::CGameObj::Script_SetName);			
				((CClassObj*)pGOClass)->RegisterMethod("GetNumChildren", (LynxEngine::CGameObj*)0, &LynxEngine::CGameObj::GetNumChildren);			

				pClass->vSet(dynamic_cast<LynxEngine::CObj*>(&Obj));
				m_State->DoFile("../script/test.lua");
				LuaFunction<int>* m_OnInit = LYNXNEW LuaFunction<int>(m_State->GetGlobal("OnInit"));
				int xx = (*m_OnInit)();

				Obj2.SetNumChildren(100);
				pGOClass->vSet(&Obj2);
				m_State->DoFile("../script/test2.lua");
				LuaFunction<int>* m_OnInit2 = LYNXNEW LuaFunction<int>(m_State->GetGlobal("OnInit"));
				xx = (*m_OnInit2)();		*/	
			}		
		}
	}
#endif