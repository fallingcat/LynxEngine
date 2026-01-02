#ifndef __LYNXTAB_H__
#define __LYNXTAB_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class LYNXENGCLASS CTabCreationDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CTabCreationDesc);	
		public:
			LynxEngine::CSpriteCreationDesc m_SpriteDesc;	
			CString							m_SelectedTabFileName;
			int								m_NumTabs;
			LYNXRECT						m_ContextRect;
		public:				
			CTabCreationDesc() { Default(); };
			void Default()	{m_SpriteDesc.Default(); m_NumTabs = 0; };
		};

		class CTab : public CControl
		{
			LYNX_DECLARE_CLASS(CTab);	
			typedef struct {
				CContainer*				Container;       
				LYNXRECT				Rect;
				CContext*				Context;
			}TabData;
		protected:				
			CArray<TabData>				m_TabList;       
			int							m_CurrentTab;
		public:			
			CTab(CGUISystem* lpsys);
			virtual ~CTab();	
			
			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);
			void						vRender();
			void						vOnHit();

			CButton*					AddButton(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE);  
			CButton*					AddButton(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const float* uvoffset, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXCHAR* mat_name = NULL);
			CStateButton*				AddStateButton(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* pressedfilename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text = LYNX_FALSE, const LYNXCHAR* mat_name = NULL);  
			CSlider*					AddSlider(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* bar_filename, const LYNXRECT& rect, const Math::CVector2& size, int min, int max, DWORD id, const LYNXCHAR* text);
			CText*						AddText(const int tab, const LYNXANSICHAR* classname, const CString& text, CFontPtr& font, const float scale, const LYNXCOLORRGBA color, const LYNXCOLORRGBA shadow_color, const LYNXBOOL bshadow, const LYNXRECT& rect, const DWORD id, const LYNXBOOL buse_sprite_text = LYNX_FALSE);
			CImage*						AddImage(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id);
		};
	}
}
#endif

