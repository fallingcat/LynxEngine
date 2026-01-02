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
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXUIPAGE_H__
#define __LYNXUIPAGE_H__

#include <LynxObj.h>
#include <LynxEngineClasses.h>
#include <GUISystem/LynxGUIContext.h>

namespace LynxEngine 
{	
	namespace GameFramework 
	{		
		class LYNXENGCLASS CUIPage : public CObj
		{	
			LYNX_DECLARE_CLASS(CUIPage);
		public:
			LYNXBOOL							m_bOnBack;
		protected:								
			CKernel*							m_lpKernel;
			LYNXBOOL							m_bNeedInit;
            LYNXBOOL                            m_bRenderToOffscreenForTransition; 
			LYNXBOOL                            m_bTransitionRendering;  //Is this the rendering for transition?
			LYNXBOOL							m_bQuit;	
			CString								m_LastUIPageName;
			CString								m_NextUIPageName;

            GUISystem::CContext*				m_Context;
			CSpriteContainer*					m_BackgroundSprite;	

			CMap<CString, int>					m_Defines;
			CUserProfile*						m_UIFile;							
		private:
			//void								InitMembers();						
		public:						
			CUIPage(CKernel* const lpk);
			virtual ~CUIPage();			

			DECLARE_CLASSSCRIPT
			GUISystem::CControl*				_GetHitControl(int x, int y) {return GetHitControl(x, y); }
			virtual LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			void								SetActiveControl(GUISystem::CControl* c);
			LYNXINLINE GUISystem::CControl*		GetActiveControl() {return m_Context->GetActiveControl(); };

			LYNXINLINE LYNXBOOL					IsQuitting() {return m_bQuit; };
			LYNXINLINE LYNXBOOL					DoesNeedInit() {return m_bNeedInit; };
			void								SetQuit(LYNXBOOL b);
			virtual LYNXBOOL					vCreate();			
			virtual LYNXBOOL					vInit();
			virtual void						vLoop(float step);
			void								ControlsLoop(float step);
			virtual void						vRender();
			void								RenderControls();
			virtual LYNXBOOL					vQuit();	

			LYNXFORCEINLINE void				SetLastUIPageName(const CString& name) {m_LastUIPageName = name; }
			LYNXFORCEINLINE CString&			GetLastUIPageName() {return m_LastUIPageName; }
			LYNXFORCEINLINE void				SetNextUIPageName(const CString& name) {m_NextUIPageName = name; }
			LYNXFORCEINLINE CString&			GetNextUIPageName() {return m_NextUIPageName; }

			void								QuitTo(const LYNXCHAR* name);
			int									GetNumControls();
			GUISystem::CControl*				GetControlByIndex(const int i);
			GUISystem::CControl*				FindControlByID(const DWORD id);  
			GUISystem::CControl*				FindControl(const LYNXCHAR* name);  			
			void								AddControl(GUISystem::CControl* c);
			LYNXFORCEINLINE void				RemoveControl(GUISystem::CControl* c) {m_Context->GetControlList().remove(c); };
			LYNXFORCEINLINE void				ReleaseControl(GUISystem::CControl* c) {m_Context->ReleaseControl(c); };
			
            GUISystem::CScrollGroup*            CreateScrollGroup(const CString& name, const LYNXRECT& rect);
            LYNXFORCEINLINE CList<GUISystem::CScrollGroup*>& GetScrollGroupList() {return m_Context->GetScrollGroupList(); }
            
			LYNXFORCEINLINE GUISystem::CContext* GetContext() {return m_Context; }
			
			GUISystem::CButton*					AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXBOOL buse_mask = LYNX_FALSE);
			GUISystem::CButton*					AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const float* uvoffset, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXCHAR* mat_name = NULL, const LYNXBOOL buse_mask = LYNX_FALSE);
			GUISystem::CStateButton*			AddStateButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* pressedfilename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXCHAR* mat_name = NULL, const LYNXBOOL buse_mask = LYNX_FALSE);
			GUISystem::CSlider*					AddSlider(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* bar_filename, const LYNXRECT& rect, const Math::CVector2& size, int min, int max, DWORD id, const LYNXCHAR* text);
			CSpriteContainer*					AddBackground(const LYNXCHAR* filename, const LYNXCOLORRGBA& color, float tex_ratio);
			GUISystem::CMessageBox*				AddMessageBox(int w, int h, DWORD id);
            GUISystem::CImage*                  AddImage(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text = NULL, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXBOOL buse_mask = LYNX_FALSE);
            
			void								RenderToOffscreen();			
			GUISystem::CControl*				GetHitControl(int x, int y);
			GUISystem::CControl*				GetHitControl(InputSystem::CTouch::TOUCHGESTURE gesture, int x, int y);
			int									GetHitControlID(int x, int y);
			LYNXBOOL							DoesAcceptInput();
			LYNXBOOL							WaitForQuit();
			void								GroupButtonPressed(GUISystem::CControl* b);			

			LYNXFORCEINLINE	int					GetDefinedControlID(const CString & key) {return m_Defines[key]; }
			virtual void						vLoadLayout(const CString& filename);
			void								ReleaseLayoutData();
		};
	}
}
#endif
