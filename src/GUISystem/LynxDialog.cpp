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
		IMPLEMENT_CLASSSCRIPT(CDialog, CWindow)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CDialog::CDialog(CGUISystem* lpsys)
		: CWindow(lpsys)
		{
			m_ControlType = CT_DIALOG;			
			m_bNoInput = LYNX_TRUE;

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CDialog::~CDialog()
		{		
			vDismiss();			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDialog::vCreate(CCreationDesc* desc)
		{
			CWindow::vCreate(desc);	
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDialog::vInit()
		{	
			CWindow::vInit();
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CDialog::vLoop(float step)
		{
			m_Context->SortControls();
			m_Context->SortGroups();

			if (!m_Context->GetActiveControl() && m_bActive)
				m_Context->GetInput();			

			m_Context->ControlLoop(step);            
			
			if (m_lpContainer)
			{
				TransitionLoop();
			}

			m_Context->UpdateControlRects();

			if (m_Context->GetActiveControl() && m_Context->GetActiveControl()->GetID() == ID_CLOSE && m_Context->GetActiveControl()->DoesAcceptInput())
			{
				m_Context->SetActiveControl(NULL);
				vDismiss();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CDialog::vRender()
		{	
			if (m_bShow && m_lpContainer)
			{
				LYNXRECT OldRect, NewRect, OldClipRect;
				LYNXBOOL bScissor;

				/*
				m_lpEngine->GetlpGraphicsSystem()->GetScissorRect(&OldClipRect);
				bScissor = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect(LYNX_TRUE);

				if (!bScissor)
				{
					LYNXRECT ClipRect = m_Rect;
					m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&ClipRect);
				}
				*/

				m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;				
				if (!m_bActive && m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
				{
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
				}				

				// Set size everytime will hurt the performance.....
				//Math::CVector2 Size = m_Size * m_Scale;
				//((CSpriteContainer*)m_lpContainer)->vSetSize(Size.x, Size.y);

				if (m_bUseBackground)
					m_lpContainer->vRender();        
				else
					static_cast<CSpriteContainer*>(m_lpContainer)->ComputeRenderRect();

				m_Rect = ((CSpriteContainer*)m_lpContainer)->m_RenderRect;

                if (m_Name.size() && m_bShowText)
                {
                    if (m_Font && m_Font->GetFontType() == CFont::FT_TEXTURE_FONT)
					{
						m_lpEngine->GetSystemFontString()->SetFont(m_Font);
					}
                    else
                        m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemSmallFont());                    					
					
                    if (m_bCustomFontAlignmentRect)
                    {
                        m_lpEngine->GetSystemFontString()->SetAlignmentType(m_FontAlignmentType);
                        m_lpEngine->GetSystemFontString()->SetOriginType(m_FontOriginType);
                        m_lpEngine->GetSystemFontString()->SetAlignmentRect(&m_TargetFontAlignmentRect);
                    }
                    else
                    {
                        m_lpEngine->GetSystemFontString()->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
                        m_lpEngine->GetSystemFontString()->SetOriginType(CSpriteContainer::OT_CENTER);
                        m_lpEngine->GetSystemFontString()->SetAlignmentRect(&m_Rect);
                    }
                    m_lpEngine->GetSystemFontString()->Printf(&m_FontColor, m_Name.c_str());
                    m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemFont());					
                }
				m_Context->RenderControls();

				//m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&OldClipRect);
				//m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect(bScissor);
			}
		} 
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CDialog::vDismiss()
		{
            Activate(LYNX_FALSE);
			Show(LYNX_FALSE);		
		}
	}
}
