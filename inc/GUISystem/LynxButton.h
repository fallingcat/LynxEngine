#ifndef __LYNXBUTTON_H__
#define __LYNXBUTTON_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{	
		#define BIND_EVENT(FUNCTION, CLASS)	std::bind(FUNCTION, CLASS, std::placeholders::_1, std::placeholders::_2)
		typedef std::function<void(const CString&, CControl*)> EVENTDELEGATE;
		typedef CMap<CString, EVENTDELEGATE> EVENTDELEGATEMAP;		
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		class LYNXENGCLASS CButtonCreationDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CButtonCreationDesc);	
		public:
			LynxEngine::CSpriteCreationDesc m_SpriteDesc;
            CFontPtr                        m_Font;
            LYNXBOOL                        m_bUseSpriteTextString;
			LYNXBOOL						m_bShowText;
            CVector2                        m_TextSize;
			LYNXBOOL						m_bLoadHitMask;
			DWORD							m_TextAlignment;
			LYNXBOOL						m_bFontShadow;
			LYNXCOLORRGBA					m_FontShadowColor;
			LYNXBOOL						m_bFontOutline;
			LYNXCOLORRGBA					m_FontOutlineColor;
			int								m_FontOutlineWidth;

		public:				
			CButtonCreationDesc() { Default(); };
			void Default()
			{ 
				m_SpriteDesc.Default(); 
				//m_Font = NULL; 
				m_bUseSpriteTextString = LYNX_FALSE; 
				m_bLoadHitMask = LYNX_FALSE; 
				m_TextSize.x = -1;
				m_TextSize.y = -1; 
				m_bShowText = LYNX_TRUE; 
				m_TextAlignment = CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER;
				m_FontShadowColor.Red = 0;
				m_FontShadowColor.Green = 0;
				m_FontShadowColor.Blue = 0;
				m_FontShadowColor.Alpha = 156;
				m_bFontShadow = LYNX_TRUE;
				m_bFontOutline = LYNX_FALSE;
				m_FontOutlineColor.Red = 255;
				m_FontOutlineColor.Green = 0;
				m_FontOutlineColor.Blue = 0;
				m_FontOutlineColor.Alpha = 255;
				m_FontOutlineWidth = 3;
			};
		};

		class CButton : public CControl
		{
			LYNX_DECLARE_CLASS(CButton);			

		public:					
			

		protected:			

			//CContainer*				m_lpContainer;            
			Math::CVector2				m_Size;
			Math::CReal					m_Scale;
			Math::CReal					m_TargetScale;
			Math::CReal					m_NumSteps;
			Math::CReal					m_NumRestSteps;
			int							m_Count;			

			LYNX_BITFIELD_BOOL(m_bUseCustomHitRect)
			LYNX_BITFIELD_BOOL(m_bScale)
			LYNXBOOL					m_bAnimationLoop;						 
			EVENTDELEGATEMAP			m_AnimationEvent;

			/*
			TRANSITIONSTATE				m_TransitionState;
            LYNX_BITFIELD_BOOL(m_bPauseTransition)
			float						m_TransitionSteps;
			CVector2					m_TransitionOffset;
			CVector2					m_OriginalTransitionOffset;
			CVector2					m_TotalOffset;
			LYNXRECT					m_TargetAlignmentRect;
			*/
		public:			
			CButton(CGUISystem* lpsys);
			virtual ~CButton();	

			DECLARE_CLASSSCRIPT
			LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			void						_SetTargetScale(const float target, const float steps) {SetTargetScale(target, steps); };			

			//LYNXFORCEINLINE TRANSITIONSTATE GetState() {return m_TransitionState; }

			LYNXBOOL					vInit();
			void						vCreateFromLayout(CUserProfile* file, CXMLElement* node, CContext* context);
			LYNXBOOL					vCreate(CCreationDesc* desc);
			void						SetInactiveTexture(CStream& fs, const CString& filename, GraphicsSystem::CRenderer::TEXTUREDESC* desc = NULL);
			CSpriteContainer::ORIGINTYPE vSetOriginType(const CSpriteContainer::ORIGINTYPE ot);
			DWORD						vSetAlignmentType(const DWORD type);
			void						vSetAlignmentRect(const LYNXRECT& r);
			void						vGetAlignmentRect(LYNXRECT& r);
			void						vLoop(float step);
			void						vRender();
			void						vSetupTexture();
			void						vSetSize(float w, float h);		
			void						vGetSize(float& w, float& h);
			void						vOnHit();
			LYNXFORCEINLINE void		vSetPosition(const Math::CVector3& pos) {m_lpContainer->vSetPosition(pos); };			
			LYNXFORCEINLINE void		vGetPosition(Math::CVector3& pos) {m_lpContainer->vGetPosition(pos); };			
			void						SetTargetScale(const Math::CReal& target, const Math::CReal& steps);
			LYNXFORCEINLINE Math::CReal GetTargetScale() { return m_TargetScale; };
            LYNXFORCEINLINE Math::CReal GetTargetScaleSteps() { return m_NumSteps; };
            
			/*
            LYNXFORCEINLINE void		PauseTransition(LYNXBOOL b) {m_bPauseTransition = b; }
            LYNXFORCEINLINE LYNXBOOL	IsTransitionPaused() const {return m_bPauseTransition; }
            void						SetTransitionOffset(const Math::CVector2& offset);
			void						ResetTransitionOffset() {SetTransitionOffset(m_OriginalTransitionOffset); }
			LYNXFORCEINLINE void		SetTransitionSteps(float s) {m_TransitionSteps = s; }
			LYNXFORCEINLINE LYNXBOOL	IsTransitionDone() { return (m_TransitionState == TS_DONE); }//(m_TotalOffset.x == -FLT_MAX && m_TotalOffset.y == -FLT_MAX); }
			void						MoveTo(const Math::CVector2& pos);
			void						TransitBack();

			void						TransitionLoop();
			*/
			
			void						UpdateText(const CString& t);	            

			LYNXFORCEINLINE	void		SetScale(LYNXBOOL b) {m_bScale = b; };		
			LYNXFORCEINLINE	float		GetScale() {return m_Scale; };		
			LYNXFORCEINLINE	void		Scale(const float s) {m_Scale = s; };					                    
			            
            LYNXFORCEINLINE void        UseCustomHitRect(const LYNXRECT* r)
            {
                if (r)
                {
                    m_bUseCustomHitRect = LYNX_TRUE;
                    m_Rect = *r;
                }
                else
                {
                    m_bUseCustomHitRect = LYNX_FALSE;
                }
            }

			LYNXFORCEINLINE CContainer* GetlpContainer() {return m_lpContainer; };

			void						ApplyAnimation(const CString& anim, const EVENTDELEGATE& event_stop = nullptr);			
			void						PlayAnimation(LYNXBOOL bReset = LYNX_TRUE);
			void						SeekAnimation(float seek_time);
			void						StopAnimation(LYNXBOOL bReset = LYNX_TRUE);
			LYNXBOOL					IsAnimationStop();

		protected:
			void						InvokeAnimationEvent(const CString& anim, LYNXBOOL remove);
			void						AnimationLoop(float step);			
		};
	}
}
#endif

