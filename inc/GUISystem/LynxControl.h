#ifndef __LYNXCONTROL_H__
#define __LYNXCONTROL_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>
#include <Container/LynxSpriteContainer.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class CControl : public LynxEngine::CGameObj
		{
			LYNX_DECLARE_CLASS(CControl);	
		public:
			static const int			NORMAL		= 0;
			static const int			PRESSED		= 1;
			static const int			INACTIVE	= 2;

			typedef enum {
				TS_STOP,
				TS_STEP_IN,
				TS_STEP_OUT,
				TS_MOVING,
				TS_STEP_IN_DONE,
				TS_STEP_OUT_DONE,
			}TRANSITIONSTATE;
			
			unsigned int				m_InputGestureFilter;	
			InputSystem::CTouch::TOUCHGESTURE m_InputGesture;
			int							m_RenderOrder;
			LYNXBOOL					m_bClipped;
			float						m_HitAgainTime;
			float						m_HintScaleSpeed;
			float						m_HintScaleMin;
			float						m_HintScaleMax;			
		protected:		
			LYNX_BITFIELD_BOOL(m_bShow)
			LYNX_BITFIELD_BOOL(m_bAcceptInput)
			LYNX_BITFIELD_BOOL(m_bModeless)
            LYNX_BITFIELD_BOOL(m_bShowText)
			LYNX_BITFIELD_BOOL(m_bInitActive)
			LYNX_BITFIELD_BOOL(m_bInitShow)
			LYNX_BITFIELD_BOOL(m_bReadyToTransit)
			LYNX_BITFIELD_BOOL(m_bHitTest)
			LYNX_BITFIELD_BOOL(m_bHintScale)

			GUISystem::CContext*		m_ParentContext;
			CContext*					m_Context;       
			CONTROLTYPE					m_ControlType;						
			CString						m_TargetName;			
			int							m_GroupID;
			CScrollGroup*				m_ScrollGroup;						

			LYNXBOOL					m_bNoInput; // this control can't accept input if this flag is TRUE.

			CContainer*					m_lpContainer;            
			LYNXRECT					m_Rect;		// the rendered rect in absolute coord	
			LYNXBOOL					m_bUseCustomHitRect;
			LYNXRECT					m_CustomHitRect; 
			CHitMask*					m_HitMask;			
			LYNXULONGLONG				m_HitTime;
			CVector2					m_OriginalSize;			
			
			CFontPtr                    m_Font;
            LYNXCOLORRGBA               m_FontColor;
			LYNXBOOL					m_bFontShadow;
			LYNXCOLORRGBA               m_FontShadowColor;
			LYNXBOOL					m_bFontOutline;
			LYNXCOLORRGBA               m_FontOutlineColor;
			int							m_FontOutlineWidth;
			LYNXCOLORRGBA               m_FontStrokeColor;
			float			            m_FontStrokeWidth;
			float						m_FontScale;
            LYNXBOOL                    m_bCustomFontAlignmentRect;
			CControl*                   m_ParentControl;
			LYNXRECT                    m_FontAlignmentRect;
            LYNXRECT					m_TargetFontAlignmentRect;
            WORD                        m_FontAlignmentType;
            CSpriteContainer::ORIGINTYPE m_FontOriginType;
            CSpriteTextString*          m_SpriteTextString;

			//Transition
			float						m_TransitionVelocity;
			TRANSITIONSTATE				m_TransitionState;
			LYNX_BITFIELD_BOOL(m_bPauseTransition)
			float						m_TransitionSteps;
			CVector2					m_TransitionOffset;
			LYNXRECT					m_OriginalTargetAlignmentRect;
			CVector2					m_OriginalTransitionOffset;
			CVector2					m_TotalOffset;
			LYNXRECT					m_TargetAlignmentRect;
			SoundSystem::CSoundPtr      m_HitSound;
			SoundSystem::CSoundPtr      m_StepInSound;
			SoundSystem::CSoundPtr      m_StepOutSound;			
		public:			
			CControl(CGUISystem* lpsys);
			virtual ~CControl();		

			DECLARE_CLASSSCRIPT
			const LYNXCHAR*				_GetTargetName() const {return GetTargetName().c_str(); };
			
			LYNXFORCEINLINE CContainer* GetlpContainer() {return m_lpContainer; }
			LYNXFORCEINLINE GUISystem::CContext* GetContext() {return m_Context; }
			LYNXFORCEINLINE void		SetContext(GUISystem::CContext* c) { m_Context = c; }

			LYNXFORCEINLINE LYNXBOOL	IsNoInput() {return m_bNoInput; }			

			LYNXFORCEINLINE GUISystem::CContext* GetParentContext() {return m_ParentContext; }
			LYNXFORCEINLINE void		SetParentContext(GUISystem::CContext* c) { m_ParentContext = c; }

			LYNXFORCEINLINE void        SetParentControl(CControl* c) {m_ParentControl = c; }            			
			LYNXFORCEINLINE CControl*   GetParentControl() {return m_ParentControl; }            			

			LYNXFORCEINLINE void		ReadyToTransit(LYNXBOOL b) { m_bReadyToTransit = b; }
			LYNXFORCEINLINE LYNXBOOL	IsReadyToTransit() { return m_bReadyToTransit; }

			//virtual LYNXBOOL			vInit() = 0;
			LYNXFORCEINLINE CONTROLTYPE	GetControlType() const {return m_ControlType; }
			virtual LYNXBOOL			vCreate(LynxEngine::CCreationDesc* desc) = 0;
			virtual	void				vCreateFromLayout(CUserProfile* file, CXMLElement* node, CContext* context) {};			
			virtual CSpriteContainer::ORIGINTYPE vSetOriginType(const CSpriteContainer::ORIGINTYPE ot) = 0;
			virtual DWORD				vSetAlignmentType(const DWORD type) = 0;
			virtual void				vSetAlignmentRect(const LYNXRECT& r) = 0;
			//virtual void				vPushAlignmentRect()= 0;
			//virtual void				vPopAlignmentRect()= 0;
			virtual void				vGetAlignmentRect(LYNXRECT& r) = 0;
			virtual void				vRender()= 0;
			virtual void				vDebugRender();
			virtual void				vSetupTexture() {};
			virtual void				vSetSize(float w, float h) = 0;
			virtual void				vGetSize(float& w, float& h) {w = h = 0; };
			virtual LYNXBOOL			vHitTest(const LYNXPOINT2D* p);
			virtual void				vOnHit() = 0;
			virtual void				vOnLongPress() {};
			virtual void				vSetPosition(const Math::CVector3& pos) = 0;
			virtual void				vGetPosition(Math::CVector3& pos) = 0;
			virtual void				vLoadControl(CUserProfile* file, CXMLElement* c) {};
			void						SetTargetName(const LYNXCHAR* name) {m_TargetName = name; };
			const CString&				GetTargetName() const {return m_TargetName; };
			
			//LYNXFORCEINLINE void		SetParentOffset(int x, int y) { m_ParentOffset.x = x, m_ParentOffset.y = y; }
			LYNXFORCEINLINE LYNXBOOL	DoesAcceptInput() {return m_bAcceptInput; };
			LYNXFORCEINLINE void		SetGroupID(const int g) {m_GroupID = g; };
			LYNXFORCEINLINE const int 	GetGroupID() const {return m_GroupID; };

			LYNXFORCEINLINE void		SetRenderOrder(int o) {m_RenderOrder = o; };

			LYNXFORCEINLINE const LYNXBOOL IsModeless() const {return m_bModeless; };
			LYNXFORCEINLINE void		Modeless(LYNXBOOL b) {m_bModeless = b; };
            
			//Script ---------------------------------------------------------------------------------
			LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
			CSpriteContainer::ORIGINTYPE SetOriginType(const CSpriteContainer::ORIGINTYPE ot) {return vSetOriginType(ot); };
			DWORD						SetAlignmentType(const DWORD type) {return vSetAlignmentType(type); };
			void						SetAlignmentRect(const LYNXRECT& r) {return vSetAlignmentRect(r); };
			LYNXFORCEINLINE void		SetPosition(const Math::CVector3& pos) {return vSetPosition(pos); };
			LYNXFORCEINLINE LYNXBOOL	HitTest(const LYNXPOINT2D& pos) {return vHitTest(&pos); };
			LYNXFORCEINLINE void		OnHit() {return vOnHit(); };
			
			virtual void				Activate(LYNXBOOL b);
			//LYNXFORCEINLINE void		Show(LYNXBOOL b) {m_bShow = b; };
			virtual void				Show(LYNXBOOL b);
			LYNXFORCEINLINE LYNXBOOL	IsShown() const {return m_bShow; };

			LYNXFORCEINLINE void		EnableHitTest(LYNXBOOL b) {m_bHitTest = b; }
			//LYNXFORCEINLINE void		SetParentPos(const Math::CVector3& pos) {m_ParentPos = pos; };
			//LYNXFORCEINLINE CVector3&	GetParentPos() const {return m_ParentPos; };

            LYNXFORCEINLINE void		ShowText(LYNXBOOL b) {m_bShowText = b; };
			LYNXFORCEINLINE void        SetFont(CFontPtr& f) {m_Font = f; };
            LYNXFORCEINLINE void        SetFontColor(const LYNXCOLORRGBA c) {m_FontColor = c; };
			LYNXFORCEINLINE void        SetFontShadowColor(const LYNXCOLORRGBA c) {m_FontShadowColor = c; };
			LYNXFORCEINLINE void        UseFontShadow(const LYNXBOOL b) {m_bFontShadow = b; };
			LYNXFORCEINLINE void        SetFontOutlineColor(const LYNXCOLORRGBA c) { m_FontOutlineColor = c; };
			LYNXFORCEINLINE void        SetFontOutlineWidth(int w) { m_FontOutlineWidth = w; };
			LYNXFORCEINLINE void        UseFontOutline(const LYNXBOOL b) { m_bFontOutline = b; };
			LYNXFORCEINLINE void        SetFontScale(const float s) {m_FontScale = s; };
            void						SetFontOriginType(const CSpriteContainer::ORIGINTYPE ot) {m_FontOriginType = ot; };
            LYNXFORCEINLINE const CSpriteContainer::ORIGINTYPE GetFontOriginType() const {return m_FontOriginType;};
            void						SetFontAlignmentType(const WORD at) {m_FontAlignmentType = at; };
            LYNXFORCEINLINE const WORD  GetFontAlignmentType() const {return m_FontAlignmentType;};
			LYNXFORCEINLINE void        SetFontAlignmentRect(const LYNXRECT& r) {m_FontAlignmentRect = r;};
            LYNXFORCEINLINE void        GetFontAlignmentRect(LYNXRECT& r) {r = m_FontAlignmentRect;};
            LYNXFORCEINLINE void        GetTargetFontAlignmentRect(LYNXRECT& r) {r = m_TargetFontAlignmentRect;};
            LYNXFORCEINLINE void        UseCustomFontAlignmentRect(const LYNXRECT* r)
            {
                if (r)
                {
                    m_FontAlignmentRect = *r;
                    m_bCustomFontAlignmentRect = LYNX_TRUE;
                }
                else
                {
                    m_bCustomFontAlignmentRect = LYNX_FALSE;
                }
            }

			void						ComputeRenderRect();
			LYNXFORCEINLINE void        GetRenderRect(LYNXRECT& r) {r = m_Rect; };
			LYNXFORCEINLINE void        GetHitRect(LYNXRECT& r) {r = m_Rect; };
			LYNXFORCEINLINE void        SetCustomHitRect(LYNXBOOL b, LYNXRECT& r) { m_bUseCustomHitRect = b; m_CustomHitRect = r; };

			LYNXFORCEINLINE CScrollGroup* GetScrollGroup() const {return m_ScrollGroup; }
			void						SetScrollGroup(CScrollGroup* c);
			void						_SetScrollGroup(CScrollGroup* c);

			LYNXFORCEINLINE TRANSITIONSTATE GetState() {return m_TransitionState; }
			LYNXFORCEINLINE void		SetState(TRANSITIONSTATE s) {m_TransitionState = s; }
			LYNXFORCEINLINE void		PauseTransition(LYNXBOOL b) {m_bPauseTransition = b; }
			LYNXFORCEINLINE CVector2&	GetTransitionOffset() {return m_TransitionOffset; }
            LYNXFORCEINLINE LYNXBOOL	IsTransitionPaused() const {return m_bPauseTransition; }
            void						SetTransitionOffset(const Math::CVector2& offset);
			void						SetTransitionOffsetWithoutFactor(const Math::CVector2& offset);
			void						ResetTransitionOffset();
			LYNXFORCEINLINE void		SetTransitionSteps(float s) {m_TransitionSteps = s; }
			LYNXFORCEINLINE LYNXBOOL	IsStop() { return (m_TransitionState == TS_STOP); }
			LYNXFORCEINLINE LYNXBOOL	IsTransitionDone() { return (m_TransitionState == TS_STEP_OUT_DONE || m_TransitionState == TS_STEP_IN_DONE); }//(m_TotalOffset.x == -FLT_MAX && m_TotalOffset.y == -FLT_MAX); }
			LYNXFORCEINLINE LYNXBOOL	IsStepOutDone() { return (m_TransitionState == TS_STEP_OUT_DONE); }
			LYNXFORCEINLINE LYNXBOOL	IsStepInDone() { return (m_TransitionState == TS_STEP_IN_DONE); }
			void						MoveTo(const Math::CVector2& pos);
			void						Transit();
			void						TransitStop();
			void						TransitBack();
			void						TransitDone();

			void						Rerender();

			void						TransitionLoop();			
			//virtual void				ResetTransitionOffset() {};

			LYNXFORCEINLINE LYNXBOOL	GetInitActiveState() {return m_bInitActive; }
			LYNXFORCEINLINE void		SetInitActiveState(LYNXBOOL b) { m_bInitActive = b; }
			LYNXFORCEINLINE void		ResetActiveState() { Activate(m_bInitActive); }

			LYNXFORCEINLINE LYNXBOOL	GetInitShowState() { return m_bInitShow; }
			LYNXFORCEINLINE void		SetInitShowState(LYNXBOOL b) { m_bInitShow = b; }
			LYNXFORCEINLINE void		ResetShowState() { Activate(m_bInitShow); }

			void						SetContainerAlignmentRect(const LYNXRECT& r);
			void						GetContainerAlignmentRect(LYNXRECT& r);
			void						GetContainerRenderRect(LYNXRECT& r);

			LYNXFORCEINLINE void        GetTargetAlignmentRect(LYNXRECT& r) {r = m_TargetAlignmentRect; };
			void						SetTargetAlignmentRect(LYNXRECT& r);
            
            void                        ReleaseBitmapContextData();

			void                        SetHitSound(const CString& name, const CString& filename);
			void                        SetHitSound(const SoundSystem::CSoundPtr& s);
			void                        SetHitSound(const CString& name);
			void						SetStepInSound(const CString& sound);
			void						SetStepOutSound(const CString& sound);
			void						PlayStepInSound();
			void						PlayStepOutSound();
			void						StopStepInAndOutSound();

			LYNXFORCEINLINE LYNXBOOL	IsHintScale() { return m_bHintScale; }
			void						SetHintScale(LYNXBOOL b);
			void						HintScaleLoop(float step);
		};		
	}	
	LYNXFORCEINLINE bool operator == (const GUISystem::CControl* lhs, const CObj& rhs) {return (lhs->GetID() == rhs.GetID()); };
}
#endif

