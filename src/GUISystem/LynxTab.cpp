#include <LynxEngine_PCH.h>

#include <GUISystem/LynxButton.h>
#include <LynxLogger.h>
#include <LynxFileStream.h>	
#include <LynxZipStream.h>	
#include <LynxFileOutputdevice.h>	
#include <LynxHTMLOutputDevice.h>
#include <LynxwxTextOutputDevice.h>
#include <LynxEngine.h>
#include <LynxResource.h>
#include <Font/LynxTextureFont.h>
#include <Font/LynxTextureFontString.h>
#include <GameConsoleSystem/LynxGameConsoleSystem.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/Camera/LynxCamera.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxBuffer.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <GraphicsSystem/LynxBlendState.h>
#include <GraphicsSystem/LynxRasterizerState.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <PhysicsSystem/LynxPhysicsSystem.h>
#include <PhysicsSystem/LynxPhysicsWorld.h>
#include <InputSystem/LynxKeyboard.h>
#include <InputSystem/LynxMouse.h>
#include <LynxScreenOutputDevice.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxVMachine.h>
#include <ScriptSystem/LynxVMachine.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <Shader/LynxGaussinBlurPS.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		//IMPLEMENT_CLASSSCRIPT(CTab, CControl)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CTab::CTab(CGUISystem* lpsys)
		: CControl(lpsys)
		{
			m_ControlType = CT_TAB;
			m_TabList.clear();
			m_CurrentTab = -1;

			//INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CTab::~CTab()
		{
			for (int i = 0; i < m_TabList.size(); i++)
			{
				if (m_TabList[i].Container)
				{
					m_lpEngine->DestroyContainer(m_TabList[i].Container);
				}

				if (m_TabList[i].Context)
				{
					LYNXDEL m_TabList[i].Context;
				}					
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTab::vCreate(CCreationDesc* desc)
		{	
			CTabCreationDesc* lpTabDesc = (CTabCreationDesc*)desc;
			CSpriteCreationDesc* lpSpriteDesc = (LynxEngine::CSpriteCreationDesc*)(&lpTabDesc->m_SpriteDesc);	

			int NumTabs = lpTabDesc->m_NumTabs;

			m_TabList.resize(NumTabs);

			for (int i=0; i<NumTabs; i++)
			{
				m_TabList[i].Context = LYNXNEW CContext(m_lpEngine, NULL, this, m_Rect);

			}
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CTab::vInit()
		{	
			CObj::vInit();

			m_CurrentTab = 0;

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CTab::vRender()
		{	
			m_TabList[m_CurrentTab].Context->vRender();

			if (m_bShow)
			{			
			}
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CTab::vOnHit()
		{
           	if (m_lpClassScript)
			{
				ScriptBindObject();			
				m_lpClassScript->vOnHit();
			}
			else
			{			
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CButton* CTab::AddButton(const int tab, const LYNXANSICHAR* classname,  const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text)
		{
            return m_TabList[tab].Context->AddButton(classname, filename, rect, size, id, text, buse_sprite_text);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CButton* CTab::AddButton(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const float* uvoffset, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXCHAR* mat_name)
		{
            return m_TabList[tab].Context->AddButton(classname, filename, uvoffset, rect, size, id, text, buse_sprite_text, mat_name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CStateButton* CTab::AddStateButton(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* pressedfilename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id, const LYNXCHAR* text, const LYNXBOOL buse_sprite_text, const LYNXCHAR* mat_name)
		{
            return m_TabList[tab].Context->AddStateButton(classname, filename, pressedfilename, rect, size, id, text, buse_sprite_text, mat_name);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSlider* CTab::AddSlider(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXCHAR* bar_filename, const LYNXRECT& rect, const Math::CVector2& size, int min, int max, DWORD id, const LYNXCHAR* text)
		{
            return m_TabList[tab].Context->AddSlider(classname, filename, bar_filename, rect, size, min, max, id, text);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CText* CTab::AddText(const int tab, const LYNXANSICHAR* classname, const CString& text, CFontPtr& font, const float scale, const LYNXCOLORRGBA color, const LYNXCOLORRGBA shadow_color, const LYNXBOOL bshadow, const LYNXRECT& rect, const DWORD id, const LYNXBOOL buse_sprite_text)
		{
            return m_TabList[tab].Context->AddText(classname, text, font, scale, color, shadow_color, bshadow, rect, id, buse_sprite_text);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CImage* CTab::AddImage(const int tab, const LYNXANSICHAR* classname, const LYNXCHAR* filename, const LYNXRECT& rect, const Math::CVector2& size, DWORD id)
		{
            return m_TabList[tab].Context->AddImage(classname, filename, rect, size, id);
		}
	}
}
