#ifndef __LYNXWINDOW_H__
#define __LYNXWINDOW_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>
#include <GUISystem/LynxGUIContext.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class LYNXENGCLASS CWindowCreationDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CWindowCreationDesc);	
		public:
			LynxEngine::CSpriteCreationDesc m_SpriteDesc;		
			LYNXBOOL						m_bApplyLayoutFactory;
		public:				
			CWindowCreationDesc() { Default(); };
			void Default()	{m_SpriteDesc.Default(); m_bApplyLayoutFactory = LYNX_FALSE; };
		};

		class CWindow : public CControl
		{
			LYNX_DECLARE_CLASS(CWindow);	
			static const int			ID_OK		= 0;
			static const int			ID_CANCEL	= 1;
			static const int			ID_CLOSE	= 2;
			static const int			ID_YES		= 3;
			static const int			ID_NO		= 4;
			static const int			ID_BACK		= 100;

			static const int			ID_USER_CONTROL_BASE = 100;

			typedef enum {
				BT_FILE,
				BT_UI_OFFSCREEN,				
			}BACKGROUNDTYPE;
			
			BACKGROUNDTYPE				m_BackgroundType;
			LYNXBOOL					m_bUseBackground;
			LYNXBOOL					m_bCancelCanBack;	//畫面上有 cancel ui 按鈕時, 按實體 Back 鍵時是否代表按 back 按鈕.
			LYNXBOOL					m_bOnBackInput;		//針對某些頁面的 back ui 按鈕的 id 非 100 時, 可以手動寫 code 依這個 flag 判斷實體 back 是否被按下.
			LYNXBOOL					m_bOnBack;			//由系統 kernel 自動設定.
		protected:				
			Math::CVector2				m_Size;
			float						m_Scale;						
			LYNXBOOL					m_bScale;

			CMap<CString, int>			m_Defines;
			#ifdef __DESKTOP__
				CUserProfile*			m_UIFile;				
			#else
				CReadOnlyProfile*		m_UIFile;
			#endif			
		public:
			CWindow(CGUISystem* lpsys);
			virtual ~CWindow();	

			DECLARE_CLASSSCRIPT
			LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);			
			LYNXINLINE void						SetActiveControl(GUISystem::CControl* c) {m_Context->SetActiveControl(c); };
			LYNXINLINE GUISystem::CControl*		GetActiveControl() {return m_Context->GetActiveControl(); };			
			
			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);
			virtual LYNXBOOL			vPostCreate() {return LYNX_TRUE; };
			CSpriteContainer::ORIGINTYPE vSetOriginType(const CSpriteContainer::ORIGINTYPE ot);
			DWORD						vSetAlignmentType(const DWORD type);
			void						vSetAlignmentRect(const LYNXRECT& r);
			void						vGetAlignmentRect(LYNXRECT& r);
			void						vLoop(float step);
			void						vRender();
			void						vSetSize(float w, float h);			
			void						vGetSize(float& w, float& h);
			void						vOnHit();
			void						vSetPosition(const Math::CVector3& pos);
			void						vGetPosition(Math::CVector3& pos);
								
			CScrollGroup*				CreateScrollGroup(const CString& name, const LYNXRECT& rect);
            LYNXFORCEINLINE CList<CScrollGroup*>& GetScrollGroupList() {return m_Context->GetScrollGroupList(); }

			void						GroupButtonPressed(CControl* b);
			CControl*					GetHitControl(int x, int y);
			CControl*					FindControl(const CString& name);  
			CControl*					FindControl(const DWORD id);  
			void						ReleaseControl(const CString& name); 
			void						ReleaseControl(const DWORD id); 
			CButton*					AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXBOOL buse_mask = LYNX_FALSE, const int text_w = -1, const int text_h = -1, const int textalignment = CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER);
			CButton*					AddButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const float* uvoffset, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXCHAR* mat_name = NULL, const LYNXBOOL buse_mask = LYNX_FALSE, const int text_w = -1, const int text_h = -1, const int textalignment = CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER);
			CStateButton*				AddStateButton(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* pressedfilename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXCHAR* mat_name = NULL, const LYNXBOOL buse_mask = LYNX_FALSE);
			CSlider*					AddSlider(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* bar_filename, const LYNXRECT& rect, const Math::CVector2& size, int min, int max, DWORD id, const LYNXCHAR* text);
			CText*						AddText(const LYNXANSICHAR* classname, const CString& text, CFontPtr& font, const float scale, const LYNXCOLORRGBA color, const LYNXCOLORRGBA shadow_color, const LYNXBOOL bshadow, const LYNXRECT& rect, const DWORD id, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const int alignment = CSpriteContainer::H_ALIGN_CENTER | CSpriteContainer::V_ALIGN_CENTER);
			CImage*						AddImage(const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text = NULL, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXBOOL buse_mask = LYNX_FALSE, const int text_w = -1, const int text_h = -1, const LYNXBOOL bshow_text = LYNX_FALSE);			
			LYNXFORCEINLINE	int			GetDefinedControlID(const CString & key) 
			{
				if (m_Defines.is_exist(key))  
					return m_Defines[key]; 

				return -1;
			}
			virtual void				vLoadLayout(const CString& filename);
			virtual	void				vCreateFromLayout(CUserProfile* file, CXMLElement* w, CContext* context);
			void						ReleaseLayoutData();

			//LYNXFORCEINLINE CContext*	GetContext() {return m_Context; }
			
			LYNXFORCEINLINE CContainer* GetlpContainer() {return m_lpContainer; };
			
			void						SendEvent(const int id);

			void						GetInput();
            void                        GetInputFocus();
            void                        Activate(LYNXBOOL b);
			LYNXBOOL					AnyInput();
			LYNXBOOL					IsCancelCanBack(){ return m_bCancelCanBack; };
			void						SetCancleCanBack(LYNXBOOL canback){ m_bCancelCanBack = canback; };
			void						SetOnBackInput(LYNXBOOL backinput){ m_bOnBackInput = backinput; };
		};
	}
}
#endif

