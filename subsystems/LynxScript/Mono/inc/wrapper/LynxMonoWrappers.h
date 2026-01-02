#ifndef __LYNXMONOWRAPPERS_H__
#define __LYNXMONOWRAPPERS_H__

#ifdef __MONO__

#include <LynxMonoVMachine.h>
#include <LynxMonoScript.h>
#include <LynxObj.h>
#include <LynxSystem.h>
#include <LynxRenderableObj.h>

#include <GraphicsSystem/LynxGraphicsSystem.h>

#include <GameFramework/LynxUIPage.h>

#include <GUISystem/LynxControl.h>
#include <GUISystem/LynxButton.h>

#include <InputSystem/LynxInputSystem.h>
#include <InputSystem/LynxInputDevice.h>

#include <SoundSystem/LynxSoundSystem.h>
#include <SoundSystem/LynxSound.h>

namespace LynxEngine
{
	namespace MonoWrapper
	{
		extern MonoImage* gImage;
		extern MonoDomain* gDomain;

		static void Setup(MonoImage* i, MonoDomain* d)
		{
			gImage = i;
			gDomain = d;
		}

		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of MonoObject to LYNXRECT.
		*	@param rhs.		
		*	@return Casted LYNXRECT structure. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static LYNXRECT Cast2Native(LYNXRECT* rhs)
		{
			LYNXRECT Rect;
			MonoObject* Obj = (MonoObject*)rhs;

			MonoClass* Class = mono_object_get_class(Obj);
			MonoClassField* Field = mono_class_get_field_from_name(Class, "x1");
			mono_field_get_value(Obj, Field, &Rect.x1);

			Field = mono_class_get_field_from_name(Class, "y1");
			mono_field_get_value(Obj, Field, &Rect.y1);

			Field = mono_class_get_field_from_name(Class, "x2");
			mono_field_get_value(Obj, Field, &Rect.x2);

			Field = mono_class_get_field_from_name(Class, "y2");
			mono_field_get_value(Obj, Field, &Rect.y2);
			
			return Rect;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of LYNXCOLORRGBA to pointer of MonoObject.
		*	@param rhs。		
		*	@return Casted pointer of MonoObject. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static MonoObject* Cast2CLR(LYNXCOLORRGBA& rhs)
		{
			MonoClass* Class = mono_class_from_name(gImage, "LynxEngine", "CColor4i");	
			MonoObject* Obj = mono_object_new(gDomain, Class);
			mono_runtime_object_init(Obj);			
			
			MonoClassField* Field = mono_class_get_field_from_name(Class, "r");
			mono_field_set_value(Obj, Field, &rhs.Red);

			Field = mono_class_get_field_from_name(Class, "g");
			mono_field_set_value(Obj, Field, &rhs.Green);

			Field = mono_class_get_field_from_name(Class, "b");
			mono_field_set_value(Obj, Field, &rhs.Blue);

			Field = mono_class_get_field_from_name(Class, "a");
			mono_field_set_value(Obj, Field, &rhs.Alpha);
			
			return Obj;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of MonoObject to LYNXCOLORRGBA.
		*	@param rhs.		
		*	@return Casted LYNXCOLORRGBA structure. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static LYNXCOLORRGBA Cast2Native(LYNXCOLORRGBA* rhs)
		{
			LYNXCOLORRGBA Color;
			MonoObject* Obj = (MonoObject*)rhs;

			MonoClass* Class = mono_object_get_class(Obj);
			MonoClassField* Field = mono_class_get_field_from_name(Class, "r");
			mono_field_get_value(Obj, Field, &Color.Red);

			Field = mono_class_get_field_from_name(Class, "g");
			mono_field_get_value(Obj, Field, &Color.Green);

			Field = mono_class_get_field_from_name(Class, "b");
			mono_field_get_value(Obj, Field, &Color.Blue);

			Field = mono_class_get_field_from_name(Class, "a");
			mono_field_get_value(Obj, Field, &Color.Alpha);
			
			return Color;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of LYNXRECT to pointer of MonoObject.
		*	@param rhs。		
		*	@return Casted pointer of MonoObject. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static MonoObject* Cast2CLR(LYNXRECT& rhs)
		{
			MonoClass* Class = mono_class_from_name(gImage, "LynxEngine", "CRect");	
			MonoObject* Obj = mono_object_new(gDomain, Class);
			mono_runtime_object_init(Obj);			
			
			MonoClassField* Field = mono_class_get_field_from_name(Class, "x1");
			mono_field_set_value(Obj, Field, &rhs.x1);

			Field = mono_class_get_field_from_name(Class, "y1");
			mono_field_set_value(Obj, Field, &rhs.y1);

			Field = mono_class_get_field_from_name(Class, "x2");
			mono_field_set_value(Obj, Field, &rhs.x2);

			Field = mono_class_get_field_from_name(Class, "y2");
			mono_field_set_value(Obj, Field, &rhs.y2);
			
			return Obj;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of MonoObject to Math::CVector2.
		*	@param rhs.		
		*	@return Casted Math::CVector2 class. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static LYNXPOINT2D Cast2Native(LYNXPOINT2D* rhs)
		{
			LYNXPOINT2D Vec;
			MonoObject* Obj = (MonoObject*)rhs;

			MonoClass* Class = mono_object_get_class(Obj);
			MonoClassField* Field = mono_class_get_field_from_name(Class, "x");
			mono_field_get_value(Obj, Field, &Vec.x);

			Field = mono_class_get_field_from_name(Class, "y");
			mono_field_get_value(Obj, Field, &Vec.y);
			
			return Vec;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of Math::CVector2 to pointer of MonoObject.
		*	@param rhs。		
		*	@return Casted pointer of MonoObject. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static MonoObject* Cast2CLR(LYNXPOINT2D& rhs)
		{
			MonoClass* Class = mono_class_from_name(gImage, "LynxEngine", "CPoint2");	
			MonoObject* Obj = mono_object_new(gDomain, Class);
			mono_runtime_object_init(Obj);			
			
			MonoClassField* Field = mono_class_get_field_from_name(Class, "x");
			mono_field_set_value(Obj, Field, &rhs.x);

			Field = mono_class_get_field_from_name(Class, "y");
			mono_field_set_value(Obj, Field, &rhs.y);
			
			return Obj;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of MonoObject to Math::CVector2.
		*	@param rhs.		
		*	@return Casted Math::CVector2 class. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static Math::CVector2 Cast2Native(Math::CVector2* rhs)
		{
			Math::CVector2 Vec;
			MonoObject* Obj = (MonoObject*)rhs;

			MonoClass* Class = mono_object_get_class(Obj);
			MonoClassField* Field = mono_class_get_field_from_name(Class, "x");
			mono_field_get_value(Obj, Field, &Vec.x);

			Field = mono_class_get_field_from_name(Class, "y");
			mono_field_get_value(Obj, Field, &Vec.y);
			
			return Vec;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of Math::CVector2 to pointer of MonoObject.
		*	@param rhs。		
		*	@return Casted pointer of MonoObject. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static MonoObject* Cast2CLR(Math::CVector2& rhs)
		{
			MonoClass* Class = mono_class_from_name(gImage, "LynxEngine", "CVector2");	
			MonoObject* Obj = mono_object_new(gDomain, Class);
			mono_runtime_object_init(Obj);			
			
			MonoClassField* Field = mono_class_get_field_from_name(Class, "x");
			mono_field_set_value(Obj, Field, &rhs.x);

			Field = mono_class_get_field_from_name(Class, "y");
			mono_field_set_value(Obj, Field, &rhs.y);
			
			return Obj;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of MonoObject to Math::CVector3.
		*	@param rhs.		
		*	@return Casted Math::CVector3 class. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static Math::CVector3 Cast2Native(Math::CVector3* rhs)
		{
			Math::CVector3 Vec;
			MonoObject* Obj = (MonoObject*)rhs;

			MonoClass* Class = mono_object_get_class(Obj);
			MonoClassField* Field = mono_class_get_field_from_name(Class, "x");
			mono_field_get_value(Obj, Field, &Vec.x);

			Field = mono_class_get_field_from_name(Class, "y");
			mono_field_get_value(Obj, Field, &Vec.y);

			Field = mono_class_get_field_from_name(Class, "z");
			mono_field_get_value(Obj, Field, &Vec.z);

			return Vec;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of Math::CVector3 to pointer of MonoObject.
		*	@param rhs。		
		*	@return Casted pointer of MonoObject. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static MonoObject* Cast2CLR(Math::CVector3& rhs)
		{
			MonoClass* Class = mono_class_from_name(gImage, "LynxEngine", "CVector3");	
			MonoObject* Obj = mono_object_new(gDomain, Class);
			mono_runtime_object_init(Obj);			
			
			MonoClassField* Field = mono_class_get_field_from_name(Class, "x");
			mono_field_set_value(Obj, Field, &rhs.x);

			Field = mono_class_get_field_from_name(Class, "y");
			mono_field_set_value(Obj, Field, &rhs.y);

			Field = mono_class_get_field_from_name(Class, "z");
			mono_field_set_value(Obj, Field, &rhs.z);

			return Obj;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast pointer of MonoObject to CString.
		*	@param rhs.		
		*	@return Casted CString class. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static CString Cast2Native(LYNXCHAR* rhs)
		{
			MonoString* Str = (MonoString*)rhs;
			#if defined (_UNICODE) || defined (UNICODE)
				CString Name = mono_string_to_utf16(Str);
			#else
				CString Name = mono_string_to_utf8(Str);
			#endif

			return Name;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	Cast CString to pointer of MonoObject.
		*	@param rhs.		
		*	@return Casted pointer of MonoObject. 
		*/
		//-------------------------------------------------------------------------------------------------------
		static MonoString* Cast2CLR(const CString& rhs)
		{
			MonoString* Str = mono_string_new(gDomain, CAnsiString(rhs).c_str());
			return Str;
		}

		template <class Type>
		static Type* Cast2Native(MonoObject* obj)
		{
			unsigned long Ptr = 0x00;
			if (obj)
			{
				MonoClass* Class = mono_object_get_class(obj);
				if (Class)
				{
					MonoClassField* Field = mono_class_get_field_from_name(Class, "_this");
					mono_field_get_value(obj, Field, &Ptr);
				}
			}
			return ((Type*)Ptr);
		}

		template <class Type>
		static MonoObject* Cast2CLR(Type* rhs)
		{
			if (rhs)
			{
				LynxScript::Mono::CScript* lpScript = ((LynxScript::Mono::CScript*)(rhs->GetlpClassScript()));
				if (lpScript && lpScript->GetClassObj())
					return ((LynxScript::Mono::CScript*)(rhs->GetlpClassScript()))->GetClassObj();
				else
					return NULL;
			}
			else
				return NULL;
		}

		struct CObj : public  LynxEngine::CObj
		{
			static void Init() 
			{
				mono_add_internal_call("LynxEngine.CObj::_SetID",	MonoWrapper::CObj::SetID);
				mono_add_internal_call("LynxEngine.CObj::_GetID",	MonoWrapper::CObj::GetID);
				mono_add_internal_call("LynxEngine.CObj::_SetName",	MonoWrapper::CObj::SetName);				
			}
			static const LYNXCHAR* GetName(LynxEngine::CObj* o) {return o->GetName(); } 						
			static void	SetName(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				CString Name = Cast2Native(name);				
				o->SetName(Name); 
			} 			
			static DWORD GetID(LynxEngine::CObj* o) {return o->GetID(); } 			
			static void	 SetID(LynxEngine::CObj* o, const DWORD id) {o->SetID(id); } 									
		};

		struct CContainer : public  LynxEngine::CContainer
		{
			static void Init() 
			{
				mono_add_internal_call("LynxEngine.CContainer::_Render",			MonoWrapper::CContainer::Render);
				mono_add_internal_call("LynxEngine.CContainer::_UpdateTransform",	MonoWrapper::CContainer::UpdateTransform);
				mono_add_internal_call("LynxEngine.CContainer::_SetPosition",		MonoWrapper::CContainer::SetPosition);	
			}

			static void Render(LynxEngine::CObj* o, MonoObject* camera) 
			{
				return ((LynxEngine::CContainer*)o)->vRender(Cast2Native<LynxEngine::CCameraContainer>(camera)); 
			}

			static void UpdateTransform(LynxEngine::CObj* o) 
			{
				return ((LynxEngine::CContainer*)o)->UpdateTransform(); 
			}

			static void SetPosition(LynxEngine::CObj* o, Math::CVector3* pos) 
			{
				return ((LynxEngine::CContainer*)o)->SetPosition(Cast2Native(pos)); 
			}
		};

		struct CCameraContainer : public  LynxEngine::CCameraContainer
		{
			static void Init() 
			{
				mono_add_internal_call("LynxEngine.CCameraContainer::_LookAt",					MonoWrapper::CCameraContainer::LookAt);
				mono_add_internal_call("LynxEngine.CCameraContainer::_UpdateViewMatrix",		MonoWrapper::CCameraContainer::UpdateViewMatrix);
				mono_add_internal_call("LynxEngine.CCameraContainer::_UpdateProjectionMatrix",	MonoWrapper::CCameraContainer::UpdateProjectionMatrix);	
			}

			static void LookAt(LynxEngine::CObj* o, Math::CVector3* pos) 
			{
				return ((LynxEngine::CCameraContainer*)o)->LookAt(pos); 
			}

			static void UpdateViewMatrix(LynxEngine::CObj* o) 
			{
				return ((LynxEngine::CCameraContainer*)o)->UpdateViewMatrix(); 
			}

			static void UpdateProjectionMatrix(LynxEngine::CObj* o, float ratio) 
			{
				return ((LynxEngine::CCameraContainer*)o)->UpdateProjectionMatrix(ratio); 
			}
		};

		struct CSpriteContainer : public  LynxEngine::CSpriteContainer
		{
			static void Init() 
			{
				mono_add_internal_call("LynxEngine.CSpriteContainer::_SetOriginType",		MonoWrapper::CSpriteContainer::SetOriginType);
				mono_add_internal_call("LynxEngine.CSpriteContainer::_SetAlignmentType",	MonoWrapper::CSpriteContainer::SetAlignmentType);				
			}

			static void SetOriginType(LynxEngine::CObj* o, int type) 
			{
				return ((LynxEngine::CSpriteContainer*)o)->SetOriginType((ORIGINTYPE)type); 
			}

			static void SetAlignmentType(LynxEngine::CObj* o, WORD type) 
			{
				return ((LynxEngine::CSpriteContainer*)o)->SetAlignmentType(type); 
			}			
		};

		struct CEngine : public  LynxEngine::CEngine
		{
			static void Init() 
			{	
				mono_add_internal_call("LynxEngine.CEngine::_OpenPackage",		MonoWrapper::CEngine::OpenPackage);
				mono_add_internal_call("LynxEngine.CEngine::_ClosePackage",		MonoWrapper::CEngine::ClosePackage);

				mono_add_internal_call("LynxEngine.CEngine::_CreateUIPage",		MonoWrapper::CEngine::CreateUIPage);
				mono_add_internal_call("LynxEngine.CEngine::_SetCurrentUIPage",	MonoWrapper::CEngine::SetCurrentUIPage);
				mono_add_internal_call("LynxEngine.CEngine::_FindUIPage",		MonoWrapper::CEngine::FindUIPage);
				mono_add_internal_call("LynxEngine.CEngine::_DeleteLastUIPage",	MonoWrapper::CEngine::DeleteLastUIPage);				
				mono_add_internal_call("LynxEngine.CEngine::_Render",			MonoWrapper::CEngine::Render);				

				mono_add_internal_call("LynxEngine.CEngine::_OpenScene",		MonoWrapper::CEngine::OpenScene);				
				mono_add_internal_call("LynxEngine.CEngine::_CreatePhysicsWorld", MonoWrapper::CEngine::CreatePhysicsWorld);								
			}	

			static bool OpenPackage(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				return (((LynxEngine::CEngine*)o)->OpenPackage(Cast2Native(name)) ? true : false); 				
			}

			static void ClosePackage(LynxEngine::CObj* o) 
			{
				((LynxEngine::CEngine*)o)->ClosePackage();
			}

			static MonoObject* CreateUIPage(LynxEngine::CObj* o, LYNXCHAR* name, LYNXCHAR* scriptfile, LYNXCHAR* scriptname) 
			{
				LynxEngine::GameFramework::CUIPage* lpPage = ((LynxEngine::CEngine*)o)->CreateUIPage(Cast2Native(name), Cast2Native(scriptfile), Cast2Native(scriptname)); 
				return (Cast2CLR(lpPage));
			}

			static void SetCurrentUIPage(LynxEngine::CObj* o, MonoObject* obj) 
			{
				LynxEngine::GameFramework::CUIPage* lpPage = Cast2Native<LynxEngine::GameFramework::CUIPage>(obj);
				((LynxEngine::CEngine*)o)->SetCurrentUIPage(lpPage);							
			}

			static MonoObject* FindUIPage(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				LynxEngine::GameFramework::CUIPage* lpPage = ((LynxEngine::CEngine*)o)->FindUIPage(Cast2Native(name)); 
				return (Cast2CLR(lpPage));
			}

			static void DeleteLastUIPage(LynxEngine::CObj* o) 
			{
				((LynxEngine::CEngine*)o)->DeleteLastUIPage(); 
			}

			static void Render(LynxEngine::CObj* o) 
			{
				((LynxEngine::CEngine*)o)->Render(); 
			}

			static void OpenScene(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				((LynxEngine::CEngine*)o)->OpenScene(Cast2Native(name)); 
			}

			static void CreatePhysicsWorld(LynxEngine::CObj* o) 
			{
				((LynxEngine::CEngine*)o)->CreatePhysicsWorld(); 
			}
		};		

		struct CSystem : public  LynxEngine::CSystem
		{
			static void Init() 
			{				
			}			
		};

		struct CGameConsoleSystem : public  LynxEngine::GameConsoleSystem::CGameConsoleSystem
		{
			static void Init() 
			{		
				mono_add_internal_call("LynxEngine.CGameConsoleSystem::_IsRunning",		MonoWrapper::CGameConsoleSystem::IsRunning);
				mono_add_internal_call("LynxEngine.CGameConsoleSystem::_SetPromptStr",	MonoWrapper::CGameConsoleSystem::SetPromptStr);				
			}			

			static bool IsRunning(LynxEngine::CObj* o) 
			{
				return (((LynxEngine::GameConsoleSystem::CGameConsoleSystem*)o)->IsRunning() ? true : false);
			}

			static void	SetPromptStr(LynxEngine::CObj* o, LYNXCHAR* str) 
			{
				((LynxEngine::GameConsoleSystem::CGameConsoleSystem*)o)->SetPromptStr(Cast2Native(str));
			}
		};
		
		struct CGrphicsSystem : public  LynxEngine::GraphicsSystem::CGraphicsSystem
		{
			static void Init() 
			{		
				mono_add_internal_call("LynxEngine.CGraphicsSystem::_Print",				MonoWrapper::CGrphicsSystem::Print);
				mono_add_internal_call("LynxEngine.CGraphicsSystem::_PrintWithColor",		MonoWrapper::CGrphicsSystem::PrintWithColor);
				mono_add_internal_call("LynxEngine.CGraphicsSystem::_GetBackbufferWidth",	MonoWrapper::CGrphicsSystem::GetBackbufferWidth);
				mono_add_internal_call("LynxEngine.CGraphicsSystem::_GetBackbufferHeight",	MonoWrapper::CGrphicsSystem::GetBackbufferHeight);
				mono_add_internal_call("LynxEngine.CGraphicsSystem::_ClearRenderBuffer",	MonoWrapper::CGrphicsSystem::ClearRenderBuffer);
			}			

			static void	Print(LynxEngine::CObj* o, int x, int y, LYNXCHAR* str) 
			{
				((LynxEngine::GraphicsSystem::CGraphicsSystem*)o)->Print(x, y, Cast2Native(str));
			}

			static void	PrintWithColor(LynxEngine::CObj* o, int x, int y, LYNXCHAR* str, LYNXCOLORRGBA* color) 
			{
				((LynxEngine::GraphicsSystem::CGraphicsSystem*)o)->PrintWithColor(x, y, Cast2Native(str), Cast2Native(color));
			}

			static int GetBackbufferWidth(LynxEngine::CObj* o) 
			{
				return ((LynxEngine::GraphicsSystem::CGraphicsSystem*)o)->GetBackbufferWidth();
			}

			static int GetBackbufferHeight(LynxEngine::CObj* o) 
			{
				return ((LynxEngine::GraphicsSystem::CGraphicsSystem*)o)->GetBackbufferHeight();
			}

			static void ClearRenderBuffer(LynxEngine::CObj* o, int flag, LYNXCOLORRGBA* color, float z, int s) 
			{
				((LynxEngine::GraphicsSystem::CGraphicsSystem*)o)->ClearRenderBuffer(flag, Cast2Native(color), z, s);
			}
		};

		struct CInputSystem : public  LynxEngine::InputSystem::CInputSystem
		{
			static void Init() 
			{		
				mono_add_internal_call("LynxEngine.CInputSystem::_FindDevice",	MonoWrapper::CInputSystem::FindDevice);
			}			

			static MonoObject* FindDevice(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				LynxEngine::InputSystem::CInputDevice *lpDevice = ((LynxEngine::InputSystem::CInputSystem*)o)->FindDevice(Cast2Native(name));
				return (Cast2CLR(lpDevice));
			}
		};

		struct CMouse : public  LynxEngine::InputSystem::CMouse
		{
			static void Init() 
			{		
				mono_add_internal_call("LynxEngine.CMouse::_Poll",			MonoWrapper::CMouse::Poll);
				mono_add_internal_call("LynxEngine.CMouse::_ButtonStatus",	MonoWrapper::CMouse::ButtonStatus);
				mono_add_internal_call("LynxEngine.CMouse::_GetPos",		MonoWrapper::CMouse::GetPos);
			}			

			static void Poll(LynxEngine::CObj* o) {((LynxEngine::InputSystem::CMouse*)o)->Poll(); }
			static bool ButtonStatus(LynxEngine::CObj* o, int flag) {return (((LynxEngine::InputSystem::CMouse*)o)->ButtonStatus(flag) ? true : false); }
			static MonoObject* GetPos(LynxEngine::CObj* o) 
			{
				LYNXPOINT2D Pos = ((LynxEngine::InputSystem::CMouse*)o)->GetPos();
				return Cast2CLR(Pos); 
			}
		};

		struct CSoundSystem : public  LynxEngine::SoundSystem::CSoundSystem
		{
			static void Init() 
			{
				mono_add_internal_call("LynxEngine.CSoundSystem::_LoadSound",	MonoWrapper::CSoundSystem::LoadSound);						
			}		

			static MonoObject* LoadSound(LynxEngine::CObj* o, LYNXCHAR* name, LYNXCHAR* filename) 
			{
				LynxEngine::SoundSystem::CSound* Sound = ((LynxEngine::SoundSystem::CSoundSystem*)o)->LoadSound(Cast2Native(name), Cast2Native(filename)); 
				return Cast2CLR(Sound);
			}
		};

		struct CSound : public  LynxEngine::SoundSystem::CSound
		{
			static void Init() 
			{
				mono_add_internal_call("LynxEngine.CSound::_SetLoops",	MonoWrapper::CSound::SetLoops);
				mono_add_internal_call("LynxEngine.CSound::_SetVolume",	MonoWrapper::CSound::SetVolume);
				mono_add_internal_call("LynxEngine.CSound::_Play",		MonoWrapper::CSound::Play);				
				mono_add_internal_call("LynxEngine.CSound::_Pause",		MonoWrapper::CSound::Pause);
				mono_add_internal_call("LynxEngine.CSound::_Stop",		MonoWrapper::CSound::Stop);
				mono_add_internal_call("LynxEngine.CSound::_IsPlaying",	MonoWrapper::CSound::IsPlaying);
			}

			static void SetLoops(LynxEngine::CObj* o, int loop) 
			{
				((LynxEngine::SoundSystem::CSound*)o)->SetLoops(loop);
			}

			static void SetVolume(LynxEngine::CObj* o, float vol) 
			{
				((LynxEngine::SoundSystem::CSound*)o)->SetVolume(vol);
			}

			static bool Play(LynxEngine::CObj* o) 
			{
				return (((LynxEngine::SoundSystem::CSound*)o)->Play() ? true : false);
			}

			static void Pause(LynxEngine::CObj* o) 
			{
				((LynxEngine::SoundSystem::CSound*)o)->Pause();
			}

			static void Stop(LynxEngine::CObj* o) 
			{
				((LynxEngine::SoundSystem::CSound*)o)->Stop();
			}

			static bool IsPlaying(LynxEngine::CObj* o) 
			{
				return (((LynxEngine::SoundSystem::CSound*)o)->IsPlaying() ? true : false);
			}
		};

		struct CAnimation : public  LynxEngine::Animation::CAnimation
		{
			static void Init() 
			{					
			}			
		};

		struct CGameObj : public  LynxEngine::CGameObj
		{
			static void Init() 
			{					
			}			
		};

		struct CRenderableObj : public  LynxEngine::CRenderableObj
		{
			static void Init() 
			{			
				mono_add_internal_call("LynxEngine.CRenderableObj::_GetContainer",				MonoWrapper::CRenderableObj::GetContainer);
				mono_add_internal_call("LynxEngine.CRenderableObj::_SetDepthLayer",				MonoWrapper::CRenderableObj::SetDepthLayer);
				mono_add_internal_call("LynxEngine.CRenderableObj::_Forward",					MonoWrapper::CRenderableObj::Forward);
				mono_add_internal_call("LynxEngine.CRenderableObj::_Yaw",						MonoWrapper::CRenderableObj::Yaw);
				mono_add_internal_call("LynxEngine.CRenderableObj::_Pitch",						MonoWrapper::CRenderableObj::Pitch);
				mono_add_internal_call("LynxEngine.CRenderableObj::_SetPosition",				MonoWrapper::CRenderableObj::SetPosition);
				mono_add_internal_call("LynxEngine.CRenderableObj::_GetPosition",				MonoWrapper::CRenderableObj::GetPosition);
				mono_add_internal_call("LynxEngine.CRenderableObj::_GetDirection",				MonoWrapper::CRenderableObj::GetDirection);
				mono_add_internal_call("LynxEngine.CRenderableObj::_UpdateTransform",			MonoWrapper::CRenderableObj::UpdateTransform);
				mono_add_internal_call("LynxEngine.CRenderableObj::_LoadAnimation",				MonoWrapper::CRenderableObj::LoadAnimation);
				mono_add_internal_call("LynxEngine.CRenderableObj::_SetCurrentAnimationByName",	MonoWrapper::CRenderableObj::SetCurrentAnimationByName);
				mono_add_internal_call("LynxEngine.CRenderableObj::_SetCurrentAnimation",		MonoWrapper::CRenderableObj::SetCurrentAnimation);
				mono_add_internal_call("LynxEngine.CRenderableObj::_IsCurrentAnimationStopped",	MonoWrapper::CRenderableObj::IsCurrentAnimationStopped);
				mono_add_internal_call("LynxEngine.CRenderableObj::_GetPhysicsObj",				MonoWrapper::CRenderableObj::GetPhysicsObj);
			}			

			static MonoObject* GetContainer(LynxEngine::CObj* o) 
			{
				LynxEngine::CContainer* lpC = ((LynxEngine::CRenderableObj*)o)->GetlpContainer(); 
				return Cast2CLR(lpC);
			}

			static void SetDepthLayer(LynxEngine::CObj* o, int dl) 
			{
				((LynxEngine::CRenderableObj*)o)->SetDepthLayer((LynxEngine::CRenderableObj::DEPTHLAYERTYPE)dl); 
			}

			static void Forward(LynxEngine::CObj* o, float f) 
			{
				((LynxEngine::CRenderableObj*)o)->Forward(f); 
			}

			static void Yaw(LynxEngine::CObj* o, float d, int order) 
			{
				((LynxEngine::CRenderableObj*)o)->Yaw(d, (LYNXMATHORDER)order); 
			}

			static void Pitch(LynxEngine::CObj* o, float d, int order) 
			{
				((LynxEngine::CRenderableObj*)o)->Pitch(d, (LYNXMATHORDER)order); 
			}

			static void SetPosition(LynxEngine::CObj* o, Math::CVector3* pos) 
			{
				((LynxEngine::CRenderableObj*)o)->SetPosition(Cast2Native(pos)); 
			}

			static MonoObject* GetPosition(LynxEngine::CObj* o) 
			{
				Math::CVector3 Pos;
				((LynxEngine::CRenderableObj*)o)->GetPosition(Pos); 
				return Cast2CLR(Pos);
			}

			static MonoObject* GetDirection(LynxEngine::CObj* o) 
			{
				Math::CVector3 Pos;
				((LynxEngine::CRenderableObj*)o)->GetDirection(Pos); 
				return Cast2CLR(Pos);
			}

			static void UpdateTransform(LynxEngine::CObj* o) 
			{
				((LynxEngine::CRenderableObj*)o)->UpdateTransform(); 				
			}

			static MonoObject* LoadAnimation(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				Animation::CAnimation* lpAni = ((LynxEngine::CRenderableObj*)o)->LoadAnimation(Cast2Native(name)); 				
				return Cast2CLR(lpAni);
			}

			static MonoObject* SetCurrentAnimationByName(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				Animation::CAnimation* lpAni = ((LynxEngine::CRenderableObj*)o)->SetCurrentAnimation(Cast2Native(name)); 				
				return Cast2CLR(lpAni);
			}

			static void SetCurrentAnimation(LynxEngine::CObj* o, MonoObject* ani) 
			{
				((LynxEngine::CRenderableObj*)o)->SetCurrentAnimation(Cast2Native<Animation::CAnimation>(ani)); 								
			}

			static bool IsCurrentAnimationStopped(LynxEngine::CObj* o) 
			{
				return (((LynxEngine::CRenderableObj*)o)->IsCurrentAnimationStopped() ? true : false); 								
			}

			static MonoObject* GetPhysicsObj(LynxEngine::CObj* o) 
			{
				return Cast2CLR(((LynxEngine::CRenderableObj*)o)->GetlpPhyObj());
			}
		};

		struct CUIPage : public  LynxEngine::GameFramework::CUIPage
		{
			static void Init() 
			{			
				mono_add_internal_call("LynxEngine.CUIPage::_Quit",					MonoWrapper::CUIPage::Quit);
				mono_add_internal_call("LynxEngine.CUIPage::_AddButton",			MonoWrapper::CUIPage::AddButton);
				mono_add_internal_call("LynxEngine.CUIPage::_GetNumControls",		MonoWrapper::CUIPage::GetNumControls);
				mono_add_internal_call("LynxEngine.CUIPage::_FindControl",			MonoWrapper::CUIPage::FindControl);
				mono_add_internal_call("LynxEngine.CUIPage::_GetControlByIndex",	MonoWrapper::CUIPage::GetControlByIndex);
				mono_add_internal_call("LynxEngine.CUIPage::_SetPressedControl",	MonoWrapper::CUIPage::SetPressedControl);
				mono_add_internal_call("LynxEngine.CUIPage::_GetPressedControl",	MonoWrapper::CUIPage::GetPressedControl);
				mono_add_internal_call("LynxEngine.CUIPage::_RenderToOffscreen",	MonoWrapper::CUIPage::RenderToOffscreen);
				mono_add_internal_call("LynxEngine.CUIPage::_RenderControls",		MonoWrapper::CUIPage::RenderControls);
			}

			static void	Quit(LynxEngine::CObj* o) {return ((LynxEngine::GameFramework::CUIPage*)o)->Quit(); } 			
			static MonoObject* AddButton(LynxEngine::CObj* o, LYNXCHAR* filename, LYNXRECT* rect, Math::CVector2* size, DWORD id, LYNXCHAR* text) 
			{
				GUISystem::CButton* Button = ((LynxEngine::GameFramework::CUIPage*)o)->AddButton(Cast2Native(filename), Cast2Native(rect), Cast2Native(size), id, Cast2Native(text)); 
				return (Cast2CLR(Button));
			} 			
			static int GetNumControls(LynxEngine::CObj* o) {return ((LynxEngine::GameFramework::CUIPage*)o)->GetNumControls(); } 			
			static MonoObject* FindControl(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				GUISystem::CControl* Control = (GUISystem::CButton*)(((LynxEngine::GameFramework::CUIPage*)o)->FindControl(Cast2Native(name))); 
				return (Cast2CLR(Control));
			} 
			static MonoObject* GetControlByIndex(LynxEngine::CObj* o, int i) 
			{
				GUISystem::CControl* Control = ((LynxEngine::GameFramework::CUIPage*)o)->GetControlByIndex(i); 
				return (Cast2CLR(Control));
			} 
			static void	SetPressedControl(LynxEngine::CObj* o, MonoObject* c) 
			{
				((LynxEngine::GameFramework::CUIPage*)o)->SetPressedControl(Cast2Native<GUISystem::CControl>(c));				
			} 
			static MonoObject* GetPressedControl(LynxEngine::CObj* o) 
			{
				GUISystem::CControl* Control = ((LynxEngine::GameFramework::CUIPage*)o)->GetPressedControl();				
				return Cast2CLR(Control);
			} 
			static void	RenderToOffscreen(LynxEngine::CObj* o) {return ((LynxEngine::GameFramework::CUIPage*)o)->RenderToOffscreen(); } 			
			static void	RenderControls(LynxEngine::CObj* o) {return ((LynxEngine::GameFramework::CUIPage*)o)->RenderControls(); } 			
		};

		struct CControl : public  LynxEngine::GUISystem::CControl
		{
			static void Init() 
			{			
				mono_add_internal_call("LynxEngine.CControl::_SetAlignmentType",	MonoWrapper::CControl::SetAlignmentType);
				mono_add_internal_call("LynxEngine.CControl::_SetPosition",			MonoWrapper::CControl::SetPosition);
				mono_add_internal_call("LynxEngine.CControl::_HitTest",				MonoWrapper::CControl::HitTest);
				mono_add_internal_call("LynxEngine.CControl::_OnHit",				MonoWrapper::CControl::OnHit);
				mono_add_internal_call("LynxEngine.CControl::_SetTargetName",		MonoWrapper::CControl::SetTargetName);
				mono_add_internal_call("LynxEngine.CControl::_GetTargetName",		MonoWrapper::CControl::GetTargetName);
				mono_add_internal_call("LynxEngine.CControl::_DoesAcceptInput",		MonoWrapper::CControl::DoesAcceptInput);						
			}			

			static void	SetAlignmentType(LynxEngine::CObj* o, DWORD type) {((LynxEngine::GUISystem::CControl*)o)->SetAlignmentType(type); } 			
			static void	SetPosition(LynxEngine::CObj* o, Math::CVector3* pos) {((LynxEngine::GUISystem::CControl*)o)->SetPosition(Cast2Native(pos)); } 			
			static bool HitTest(LynxEngine::CObj* o, LYNXPOINT2D* pos) {return (((LynxEngine::GUISystem::CControl*)o)->HitTest(Cast2Native(pos)) ? true : false); }
			static void OnHit(LynxEngine::CObj* o, LYNXPOINT2D* pos) {((LynxEngine::GUISystem::CControl*)o)->OnHit(); } 			
			static void SetTargetName(LynxEngine::CObj* o, LYNXCHAR* name) {((LynxEngine::GUISystem::CControl*)o)->SetTargetName(Cast2Native(name)); } 			
			static MonoString* GetTargetName(LynxEngine::CObj* o) {return Cast2CLR(((LynxEngine::GUISystem::CControl*)o)->GetTargetName()); } 			
			static bool DoesAcceptInput(LynxEngine::CObj* o) {return (((LynxEngine::GUISystem::CControl*)o)->DoesAcceptInput() ? true : false); } 									
		};

		struct CButton : public  LynxEngine::GUISystem::CButton
		{
			static void Init() 
			{		
			}			
		};


		struct CSceneSystem : public  LynxEngine::SceneSystem::CSceneSystem
		{
			static void Init() 
			{			
				mono_add_internal_call("LynxEngine.CSceneSystem::_AddScene",		MonoWrapper::CSceneSystem::AddScene);
				mono_add_internal_call("LynxEngine.CSceneSystem::_RemoveAllScenes",	MonoWrapper::CSceneSystem::RemoveAllScenes);
				mono_add_internal_call("LynxEngine.CSceneSystem::_GetCurrentScene",	MonoWrapper::CSceneSystem::GetCurrentScene);
				mono_add_internal_call("LynxEngine.CSceneSystem::_GetScene",		MonoWrapper::CSceneSystem::GetScene);
			}			

			static MonoObject* AddScene(LynxEngine::CObj* o) 
			{
				LynxEngine::SceneSystem::CScene* lpScene = ((LynxEngine::SceneSystem::CSceneSystem*)o)->AddScene(); 
				return Cast2CLR(lpScene);
			}

			static void RemoveAllScenes(LynxEngine::CObj* o) 
			{
				((LynxEngine::SceneSystem::CSceneSystem*)o)->RemoveAllScenes(); 
			}

			static MonoObject* GetCurrentScene(LynxEngine::CObj* o) 
			{
				LynxEngine::SceneSystem::CScene* lpScene = ((LynxEngine::SceneSystem::CSceneSystem*)o)->GetlpCurrentScene(); 
				return Cast2CLR(lpScene);
			}

			static MonoObject* GetScene(LynxEngine::CObj* o, int i) 
			{
				LynxEngine::SceneSystem::CScene* lpScene = ((LynxEngine::SceneSystem::CSceneSystem*)o)->GetlpScene(i); 
				return Cast2CLR(lpScene);
			}
		};

		struct CScene : public  LynxEngine::SceneSystem::CScene
		{
			static void Init() 
			{			
				mono_add_internal_call("LynxEngine.CScene::_GetVersion",					MonoWrapper::CScene::GetVersion);
				mono_add_internal_call("LynxEngine.CScene::_Setup",							MonoWrapper::CScene::Setup);
				mono_add_internal_call("LynxEngine.CScene::_SetRenderMode",					MonoWrapper::CScene::SetRenderMode);
				mono_add_internal_call("LynxEngine.CScene::_LoadBackgroundTexture",			MonoWrapper::CScene::LoadBackgroundTexture);
				mono_add_internal_call("LynxEngine.CScene::_SetBackgroundTextureUVOffset",	MonoWrapper::CScene::SetBackgroundTextureUVOffset);
				mono_add_internal_call("LynxEngine.CScene::_SetSortMode",					MonoWrapper::CScene::SetSortMode);

				mono_add_internal_call("LynxEngine.CScene::_CreateCamera",					MonoWrapper::CScene::CreateCamera);
				mono_add_internal_call("LynxEngine.CScene::_SetCurrentCamera",				MonoWrapper::CScene::SetCurrentCamera);

				//mono_add_internal_call("LynxEngine.CScene::_GetStaticObj",					MonoWrapper::CScene::GetStaticObj);

				mono_add_internal_call("LynxEngine.CScene::_CreateDynamicEntity",			MonoWrapper::CScene::CreateDynamicEntity);
				mono_add_internal_call("LynxEngine.CScene::_AddDynamicObj",					MonoWrapper::CScene::AddDynamicObj);
				mono_add_internal_call("LynxEngine.CScene::_GetDynamicObjByName",			MonoWrapper::CScene::GetDynamicObjByName);
				mono_add_internal_call("LynxEngine.CScene::_GetDynamicObjByIndex",			MonoWrapper::CScene::GetDynamicObjByIndex);			
			}	


			static DWORD GetVersion(LynxEngine::CObj* o) 
			{
				return ((LynxEngine::SceneSystem::CScene*)o)->GetVersion(); 
			}

			static bool Setup(LynxEngine::CObj* o) 
			{
				return (((LynxEngine::SceneSystem::CScene*)o)->Setup() ? true : false); 
			}

			static void SetRenderMode(LynxEngine::CObj* o, int mode) 
			{
				((LynxEngine::SceneSystem::CScene*)o)->SetRenderMode((SceneSystem::CRenderer::RENDERMODE)mode); 
			}

			static void LoadBackgroundTexture(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				return ((LynxEngine::SceneSystem::CScene*)o)->LoadBackgroundTexture(Cast2Native(name)); 
			}

			static void SetBackgroundTextureUVOffset(LynxEngine::CObj* o, float u0, float v0, float u1, float v1) 
			{
				return ((LynxEngine::SceneSystem::CScene*)o)->SetBackgroundTextureUVOffset(u0, v0, u1, v1); 
			}

			static void SetSortMode(LynxEngine::CObj* o, int mode) 
			{
				return ((LynxEngine::SceneSystem::CScene*)o)->SetSortMode((SceneSystem::CRenderer::SORTMODE)mode); 
			}

			static MonoObject* CreateCamera(LynxEngine::CObj* o) 
			{
				LynxEngine::SceneSystem::CCamera* lpCamera = ((LynxEngine::SceneSystem::CScene*)o)->vCreateCamera(); 
				return Cast2CLR(lpCamera);
			}

			static void SetCurrentCamera(LynxEngine::CObj* o, MonoObject* camera) 
			{
				((LynxEngine::SceneSystem::CScene*)o)->SetlpCurrentCamera(Cast2Native<SceneSystem::CCamera>(camera)); 				
			}

			static MonoObject* CreateDynamicEntity(LynxEngine::CObj* o) 
			{
				LynxEngine::SceneSystem::CDynamicObj* lpDynObj = ((LynxEngine::SceneSystem::CScene*)o)->CreateDynamicEntity(); 
				return Cast2CLR(lpDynObj);
			}

			static void AddDynamicObj(LynxEngine::CObj* o, MonoObject* dynobj) 
			{
				((LynxEngine::SceneSystem::CScene*)o)->AddDynamicObj(Cast2Native<SceneSystem::CDynamicObj>(dynobj)); 
			}

			static MonoObject* GetDynamicObjByName(LynxEngine::CObj* o, LYNXCHAR* name) 
			{
				LynxEngine::SceneSystem::CDynamicObj* lpDynObj = ((LynxEngine::SceneSystem::CScene*)o)->GetDynamicObjByName(Cast2Native(name)); 
				return Cast2CLR(lpDynObj);
			}

			static MonoObject* GetDynamicObjByIndex(LynxEngine::CObj* o, int i) 
			{
				LynxEngine::SceneSystem::CDynamicObj* lpDynObj = ((LynxEngine::SceneSystem::CScene*)o)->GetDynamicObjByIndex(i); 
				return Cast2CLR(lpDynObj);
			}
		};			

		struct CDynamicObj : public  LynxEngine::SceneSystem::CDynamicObj
		{
			static void Init() 
			{			
				mono_add_internal_call("LynxEngine.CDynamicObj::_CreateModel",		MonoWrapper::CDynamicObj::CreateModelContainer);				
				mono_add_internal_call("LynxEngine.CDynamicObj::_CreateSprite",		MonoWrapper::CDynamicObj::CreateSpriteContainer);				
				mono_add_internal_call("LynxEngine.CDynamicObj::_CreatePhysics",	MonoWrapper::CDynamicObj::CreatePhysics);				
			}	

			static void CreateModelContainer(LynxEngine::CObj* o, LYNXCHAR* name, LYNXCHAR* filename) 
			{
				((LynxEngine::SceneSystem::CDynamicObj*)o)->CreateModelContainer(Cast2Native(name), Cast2Native(filename)); 				
			}

			static void CreateSpriteContainer(LynxEngine::CObj* o, LYNXCHAR* name, LYNXCHAR* filename, float w, float h) 
			{
				((LynxEngine::SceneSystem::CDynamicObj*)o)->CreateSpriteContainer(Cast2Native(name), Cast2Native(filename), w, h); 				
			}

			static void CreatePhysics(LynxEngine::CObj* o, int type) 
			{
				((LynxEngine::SceneSystem::CDynamicObj*)o)->CreatePhysics((PhysicsSystem::CPhysicsShape::TYPE)type);
			}
		};

		struct CCamera : public  LynxEngine::SceneSystem::CCamera
		{
			static void Init() 
			{				
				mono_add_internal_call("LynxEngine.CCamera::_Create",	MonoWrapper::CCamera::Create);				
			}		

			static bool Create(LynxEngine::CObj* o) 
			{
				return (((LynxEngine::SceneSystem::CCamera*)o)->vCreate() ? true : false); 
			} 
		};

		struct CPhysicsSystem : public  LynxEngine::PhysicsSystem::CPhysicsSystem
		{
			static void Init() 
			{			
			}			
		};

		struct CPhysicsObj : public  LynxEngine::PhysicsSystem::CPhysicsObj
		{
			static void Init() 
			{			
			}			
		};

		static void InitClasses(MonoImage* i, MonoDomain* d)
		{
			Setup(i, d);
			CObj::Init();
			CAnimation::Init();
			CGameObj::Init();
			CRenderableObj::Init();			
			CUIPage::Init();
			CControl::Init();
			CButton::Init();
			CContainer::Init();
				CCameraContainer::Init();
				CSpriteContainer::Init();
			CEngine::Init();

			CSystem::Init();
			CGameConsoleSystem::Init();
			CGrphicsSystem::Init();

			CInputSystem::Init();
			CMouse::Init();

			CSoundSystem::Init();
			CSound::Init();
			
			CSceneSystem::Init();
			CScene::Init();
			CCamera::Init();
			CDynamicObj::Init();

			CPhysicsSystem::Init();
			CPhysicsObj::Init();
		}
	}
}

#endif

#endif