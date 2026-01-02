#ifndef __LYNXTEXT_H__
#define __LYNXTEXT_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class LYNXENGCLASS CTextCreationDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CTextCreationDesc);	
		public:
			CFontPtr						m_Font;
			LYNXCOLORRGBA					m_FontColor;
			LYNXBOOL						m_bFontShadow;
			LYNXCOLORRGBA					m_FontShadowColor;			
			float							m_FontScale;
			CString							m_Text;		
			DWORD							m_AlignmentType;
			CSpriteContainer::ORIGINTYPE	m_OriginType;
			LYNXRECT						m_AlignmentRect;
            LYNXBOOL                        m_bUseSpriteTextString;
			LYNXBOOL						m_bFontOutline;
			LYNXCOLORRGBA					m_FontOutlineColor;
			int								m_FontOutlineWidth;
			
		public:				
			CTextCreationDesc() { Default(); };
			void Default()	
			{
				//m_Font = NULL;
				m_FontColor.PackedColor = 0xffffffff;
				m_FontShadowColor.Red = 0;
				m_FontShadowColor.Green = 0;
				m_FontShadowColor.Blue = 0;
				m_FontShadowColor.Alpha = 156;
				m_bFontShadow = LYNX_TRUE;
				m_FontScale = 1.0f;
				m_Text = _T("");
				m_AlignmentType = CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER; 
				m_OriginType = CSpriteContainer::OT_CENTER;
                m_bUseSpriteTextString = LYNX_FALSE;
				m_bFontOutline = LYNX_FALSE;
				m_FontOutlineColor.Red = 255;
				m_FontOutlineColor.Green = 0;
				m_FontOutlineColor.Blue = 0;
				m_FontOutlineColor.Alpha = 255;
				m_FontOutlineWidth = 3;
			};
		};

		class CText : public CControl
		{
			LYNX_DECLARE_CLASS(CText);	
		public:			
		protected:
			CString						m_UserData;
			CString						m_Text;			
			LYNXBOOL					m_bAnimationLoop;
			EVENTDELEGATEMAP			m_AnimationEvent;

		public:			
			CText(CGUISystem* lpsys);
			virtual ~CText();	

			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);
			CSpriteContainer::ORIGINTYPE vSetOriginType(const CSpriteContainer::ORIGINTYPE ot);
			DWORD						vSetAlignmentType(const DWORD type);
			void						vSetAlignmentRect(const LYNXRECT& r);
			void						vGetAlignmentRect(LYNXRECT& r);
			void						vLoop(float step);
			void						vRender();
			void						vDebugRender();
			void						vOnHit();
            LYNXBOOL                    vHitTest(const LYNXPOINT2D* p);

			void						vSetSize(float w, float h) {};
			void						vSetPosition(const Math::CVector3& pos);
			void						vGetPosition(Math::CVector3& pos);

			void						SetText(const CString& text);	
			void						SetText(const CString& text, const float& x, const float& y);
			void						SetScale(const float s);
			float						GetScale(){return m_FontScale;}
			LYNXFORCEINLINE CString		GetText() { return m_Text; }
			
			void						SetUserData(const CString& text) { m_UserData = text; }
			LYNXFORCEINLINE CString		GetUserData() { return m_UserData; }

			void						ApplyAnimation(const CString& anim, const EVENTDELEGATE& event_end = nullptr);
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

