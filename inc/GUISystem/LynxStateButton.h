#ifndef __LYNXSTATEBUTTON_H__
#define __LYNXSTATEBUTTON_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class LYNXENGCLASS CStateButtonCreationDesc : public CButtonCreationDesc
		{	
			LYNX_DECLARE_CLASS(CStateButtonCreationDesc);	
		public:
			CString					m_PressedStateFileName;
            CFontPtr                m_Font;
            LYNXBOOL                m_bUseSpriteTextString;
		public:				
			CStateButtonCreationDesc() { Default(); };
			void Default()	{m_SpriteDesc.Default(); /*m_Font = NULL;*/ m_bUseSpriteTextString = LYNX_FALSE; };
		};

		class CStateButton : public CButton
		{
			LYNX_DECLARE_CLASS(CStateButton);

			LYNXCOLORRGBA				m_PressedFontColor;
		public:			
		protected:				
			LYNX_BITFIELD_BOOL(m_bPressed)
		public:			
			CStateButton(CGUISystem* lpsys);
			virtual ~CStateButton();	

			DECLARE_CLASSSCRIPT
			LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			//void						_SetTargetScale(const float target, const float steps) {SetTargetScale(target, steps); };			

			LYNXFORCEINLINE LYNXBOOL	IsPressed() const {return m_bPressed; };
			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);
			void						vLoop(float step);
			void						vRender();
			void						vOnHit();
			void						Pressed(LYNXBOOL b);
			void						SetPressedFontColor(LYNXCOLORRGBA c) { m_PressedFontColor = c; }
		};
	}
}
#endif

