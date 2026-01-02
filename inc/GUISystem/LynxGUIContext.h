#ifndef __LYNXGUICONTEXT_H__
#define __LYNXGUICONTEXT_H__

#include <LynxEngineClasses.h>
#include <InputSystem/LynxTouch.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class CContext : public LynxEngine::CObj
		{
		public:
			typedef enum {
				STATE_NORMAL = 0,
				STATE_GROUP_STEP_OUT,
				STATE_GROUP_STEP_IN,
				STATE_GROUP_TRANSITION_OVER,
			}STATE;							
			LYNXBOOL					m_bDebugRender;
			float						m_DefaultFontScale;
			Animation::CAnimationSetPtr m_AnimationSet;
		protected:
			STATE						m_State;
			GameFramework::CUIPage*		m_ParentPage;
            CControl*                   m_Parent;
            LYNXRECT					m_Rect;            		

			LYNXBOOL					m_bApplyLayoutFactor;

			CControl*					m_DummyControl;
			CList<CControl*>			m_ControlList;			
			CControl*					m_ActiveControl;
            CList<CScrollGroup*>		m_ScrollGroupList;			
			int							m_FocusControlID;
			CControl*					m_FocusControl;

			CArray<CControl*>			m_OutControlList;
			CArray<CControl*>			m_InControlList;			
		public:
			CContext(LynxEngine::CEngine* lpe, GameFramework::CUIPage* page, CControl* parent, LYNXRECT& rect);
			virtual ~CContext();
            
			void						ApplyLayoutFactor(LYNXBOOL b);
			LYNXFORCEINLINE LYNXBOOL	DoesApplyLayoutFactor() {return m_bApplyLayoutFactor; }

			LYNXFORCEINLINE CControl*	GetFocusControl() { return m_FocusControl;  }

			void						Activate(LYNXBOOL b);
			void						Show(LYNXBOOL b);
			LYNXBOOL					vInit();
            void						GetInput();
            void                        vLoop(float step);
			void						ControlLoop(float step);
            
			LYNXFORCEINLINE GameFramework::CUIPage*	GetParentPage() {return m_ParentPage; };	
			LYNXFORCEINLINE GUISystem::CControl* GetParent() {return m_Parent; };	
			LYNXFORCEINLINE void		SetParent(GUISystem::CControl* p) {m_Parent = p; };			
				
			void						SetFocusControl(const int id);

			void						HitControl(const DWORD id);
            void                        SetActiveControl(GUISystem::CControl* c);
			LYNXFORCEINLINE GUISystem::CControl* GetActiveControl() {return m_ActiveControl; };
            LYNXFORCEINLINE CList<CControl*>& GetControlList() {return m_ControlList; }
            
			LYNXFORCEINLINE LYNXRECT&	GetRect() {return m_Rect; };
			LYNXFORCEINLINE void		SetRect(const LYNXRECT& r) {m_Rect = r; };

			static CVector2				GetAbsoluteCoordOrigin(GUISystem::CContext* c);

			void						GroupButtonPressed(CControl* b);
			CControl*					GetHitControl(int x, int y);
            CControl*                   GetHitControl(InputSystem::CTouch::TOUCHGESTURE gesture, int x, int y);
			CControl*					FindControl(const CString& name);
			CControl*					FindControl(const DWORD id);
			void						ReleaseControl(const CString& name);
			void						ReleaseControl(const DWORD id);
            void						ReleaseControl(const CControl* c);
			void						ReleaseAllControls(CScrollGroup* g = NULL);			
			CButton*					AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXBOOL buse_mask = LYNX_FALSE, const int text_w = -1, const int text_h = -1, const int textalignment = CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER);
			CButton*					AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const float* uvoffset, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXCHAR* mat_name = NULL, const LYNXBOOL buse_mask = LYNX_FALSE, const int text_w = -1, const int text_h = -1, const int textalignment = CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER);
			CStateButton*				AddStateButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* pressedfilename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXCHAR* mat_name = NULL, const LYNXBOOL buse_mask = LYNX_FALSE);
			CSlider*					AddSlider(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* bar_filename, const LYNXRECT& rect, const Math::CVector2& size, int min, int max, DWORD id, const LYNXCHAR* text);
            CText*						AddText(const LYNXANSICHAR* classname, const CString& text, CFontPtr& font, const float scale, const LYNXCOLORRGBA color, const LYNXCOLORRGBA shadow_color, const LYNXBOOL bshadow, const LYNXRECT& rect, const DWORD id, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const int alignment = CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER);
			CImage*						AddImage(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text = NULL, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXBOOL buse_mask = LYNX_FALSE, const int text_w = -1, const int text_h = -1, const LYNXBOOL bshow_text = LYNX_FALSE);
			CPEmitter*					AddPEmitter(const LYNXANSICHAR* classname, const LYNXCHAR* filename);
			void						AddControl(CControl* c);
			void						RearrangeControl(GUISystem::CControl* control,  const LYNXRECT& rect);
			
			CTab*						AddTab(const LYNXANSICHAR* classname, const int number, const LYNXCHAR* filename, const LYNXCHAR* selected_filename, const LYNXRECT& tab_rect, const Math::CVector2& tab_size, const LYNXRECT& context_rect, DWORD id);
            
			LYNXFORCEINLINE STATE		GetState() {return m_State; }

			void						SendEvent(const int id);

			static LYNXBOOL				IsControlArrayTransitionDone(CArray<CControl*>& list);
			LYNXBOOL					IsControlTransitionDone();
			static void					ShowControlArray(CArray<CControl*>& list, LYNXBOOL b);
			static void					ActivateControlArray(CArray<CControl*>& list, LYNXBOOL b);
			static void					ResetControlArrayActiveState(CArray<CControl*>& list);
			void						AddStepOutControl(CControl* c);
			void						AddStepOutGroup(CScrollGroup* g);			
			void						AddStepInGroup(CScrollGroup* g);
		
			void						AddStepInControl(CControl* c);
			void						InitStepOut();
			void						StepOutGroup(CScrollGroup* out, CArray<WORD>* out_ex_list, CScrollGroup* in, CArray<WORD>* in_ex_list);

			void						ResetControlTransitionOffset();
			void						RenderControls(LYNXBOOL bupdateonly = LYNX_FALSE);
			void						UpdateControlRects();
			void						SortControls();
			void						SortGroups();
            
            CScrollGroup*				CreateScrollGroup(const CString& name, const LYNXRECT& rect);
            LYNXFORCEINLINE CList<CScrollGroup*>& GetScrollGroupList() {return m_ScrollGroupList; }
			CScrollGroup*				FindScrollGroup(const CString& name);

			LYNXBOOL					IsInputFocused();
			void						GetInputFocus();
			void						ClearMouseInputData();
			void						ClearTouchInputData();
			void						ClearAllInputData();
            void                        ClearTouchGesture();

			const Animation::CAnimationSet&   GetAnimationSet(){ return (*m_AnimationSet); }
			
			int							GetDefinedControlID(const CString & key);
			void						LoadGroup(CUserProfile* file, CXMLElement* group);
			void						LoadControl(CUserProfile* file, CXMLElement* c);
			void						LoadAnimation(CUserProfile* file, CXMLElement* c);
			void						LoadButton(CUserProfile* file, CXMLElement* c);
			void						LoadStateButton(CUserProfile* file, CXMLElement* c);
			void						LoadImage(CUserProfile* file, CXMLElement* c);
			void						LoadText(CUserProfile* file, CXMLElement* c);
			void						LoadWindow(CUserProfile* file, CXMLElement* c);

		private:			
		};

		typedef	bool (*CMPCONTROLFUNC)(const GUISystem::CControl*, const GUISystem::CControl*); 
		typedef	bool (*CMPGROUPFUNC)(const GUISystem::CScrollGroup*, const GUISystem::CScrollGroup*); 

		bool							CmpRenderOrder(const GUISystem::CControl* lhs, const GUISystem::CControl* rhs); 		
		bool							CmpRenderOrder(const GUISystem::CScrollGroup* lhs, const GUISystem::CScrollGroup* rhs); 
	}
}
#endif
