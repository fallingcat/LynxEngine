#include <LynxEngine_PCH.h>

#include <GUISystem/LynxMessageBox.h>
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
#include <GUISystem/LynxButton.h>
#include <GUISystem/LynxSlider.h>
#include <GUISystem/LynxText.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CMessageBox::CMessageBox(CGUISystem* lpsys)
		: CWindow(lpsys)
		{
			m_ControlType = CT_MESSAGE_BOX;		
			m_Title = NULL;		
			m_Text = NULL;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CMessageBox::~CMessageBox()
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMessageBox::vCreate(CCreationDesc* desc)
		{
			CWindow::vCreate(desc);
			
			CString OldTexPath = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath();
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));

			LYNXRECT Rect;

			Rect.x1 = 0;
			Rect.x2 = m_Size.x;
			Rect.y2 = m_Size.y * 0.90f;	
			Rect.y1 = Rect.y2 - 55.0f;			
			m_OKButton = AddButton(NULL, _T("../texture/ui/button1.png"), Rect, Math::CVector2(122.0f, 55.0f), ID_OK, _T("OK"));  		
			
			/*
			LYNXCOLORRGBA Color, ShadowColor;

			Color.PackedColor = 0xffffffff;
			ShadowColor.Red = 0;
			ShadowColor.Green = 0;
			ShadowColor.Blue = 0;
			ShadowColor.Alpha = 156;
			Rect.x1 = m_Size.x * 0.08f; // m_lpEngine->GetlpGUISystem()->GetScaleFactor().x;
			Rect.x2 = m_Size.x * 0.92f; // m_lpEngine->GetlpGUISystem()->GetScaleFactor().x;
			Rect.y1 = m_Size.y * 0.10f; // m_lpEngine->GetlpGUISystem()->GetScaleFactor().y;
			Rect.y2 = m_Size.y * 0.90f; // m_lpEngine->GetlpGUISystem()->GetScaleFactor().y;
			m_Text = AddText(NULL, _T("MessageBox"), m_lpEngine->GetSystemFont(), 0.5f, Color, ShadowColor, LYNX_TRUE, Rect, 1);
			//m_Text->SetAlignmentType(CSpriteContainer::H_ALIGN_LEFT|CSpriteContainer::V_ALIGN_TOP);
			//m_Text->SetOriginType(CSpriteContainer::OT_CENTER);
			m_Text->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_TOP);
			m_Text->SetOriginType(CSpriteContainer::OT_CENTER);
			*/

			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(OldTexPath);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CMessageBox::vInit()
		{	
			CWindow::vInit();
			m_Context->SetActiveControl(NULL);

			m_Title = static_cast<CText*>(FindControl(_T("Title")));
			m_Text = static_cast<CText*>(FindControl(_T("Text")));
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMessageBox::vLoop(float step)
		{
            if (!m_Context->GetActiveControl() && m_bActive)
                m_Context->GetInput();
            
            m_Context->ControlLoop(step);
            
            if (m_lpContainer)
            {
                TransitionLoop();
            }

			if (m_Context->GetActiveControl() && (m_Context->GetActiveControl()->GetID() == ID_OK || m_Context->GetActiveControl()->GetID() == ID_CANCEL) && m_Context->GetActiveControl()->DoesAcceptInput())
			{
				m_Context->SetActiveControl(NULL);
				Dismiss();
			}
			m_Context->UpdateControlRects();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMessageBox::vRender()
		{	
			CWindow::vRender();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMessageBox::SetTitle(const CString& text)
		{
			if (m_Title)
				m_Title->SetText(text);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMessageBox::SetText(const CString& text)
		{
			if (m_Text)
				m_Text->SetText(text);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMessageBox::Display()
		{	
			if (m_lpEngine->GetlpKernel()->GetCurrentUIPage())
			{
				m_lpEngine->GetlpKernel()->GetCurrentUIPage()->SetActiveControl(this);
			}
			
			Activate(LYNX_TRUE);
			Show(LYNX_TRUE);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CMessageBox::Dismiss()
		{	
			if (m_lpEngine->GetlpKernel()->GetCurrentUIPage())
			{
				if (m_lpEngine->GetlpKernel()->GetCurrentUIPage()->GetActiveControl() == this)
					m_lpEngine->GetlpKernel()->GetCurrentUIPage()->SetActiveControl(NULL);

				m_lpEngine->GetlpKernel()->GetCurrentUIPage()->RemoveControl(this);
			}
			Activate(LYNX_FALSE);
			Show(LYNX_FALSE);					
		}
	}
}
