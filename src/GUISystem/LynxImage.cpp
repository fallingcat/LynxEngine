#include <LynxEngine_PCH.h>

#include <GUISystem/LynxImage.h>
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
		IMPLEMENT_CLASSSCRIPT(CImage, CButton)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CImage::CImage(CGUISystem* lpsys)
		: CButton(lpsys)
		{
			m_ControlType = CT_IMAGE;
			
			m_bActive = LYNX_TRUE;
			m_bShow = LYNX_TRUE;    
			m_bAcceptInput = LYNX_TRUE;
            ShowText(LYNX_FALSE);

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CImage::~CImage()
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CImage::vCreate(CCreationDesc* desc)
		{	
			return CButton::vCreate(desc);

			CButtonCreationDesc* lpButtonDesc = (CButtonCreationDesc*)desc;
			CSpriteCreationDesc* lpSpriteDesc = (LynxEngine::CSpriteCreationDesc*)(&lpButtonDesc->m_SpriteDesc);	
			
			m_lpContainer = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
			m_lpContainer->SetName(desc->m_Name);
			m_lpContainer->vCreate(lpSpriteDesc);
			m_Size.x = ((CSpriteContainer*)m_lpContainer)->GetWidth();
			m_Size.y = ((CSpriteContainer*)m_lpContainer)->GetHeight();

			((CSpriteContainer*)m_lpContainer)->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);	
			((CSpriteContainer*)m_lpContainer)->SetOriginType(CSpriteContainer::OT_CENTER);				
			((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&lpSpriteDesc->m_AlignmentRect);
            
            if (lpButtonDesc->m_bUseSpriteTextString && lpButtonDesc->m_bShowText)
            {
                m_SpriteTextString = LYNXNEW CSpriteTextString(m_lpEngine);
                m_SpriteTextString->Create(-1, -1, m_Size.x, m_Size.y);
                if (lpButtonDesc->m_Font)
                    m_SpriteTextString->SetFont(m_lpEngine->GetSystemTTFont());
                else
                    m_SpriteTextString->SetFont(m_lpEngine->GetSystemTTFont());
                m_SpriteTextString->Printf(NULL, desc->m_Name.c_str());
            }

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CImage::vInit()
		{	
			return CButton::vInit();
			
			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CImage::vRender()
		{	
			return CButton::vRender();

			if (m_bShow && m_lpContainer)
			{
				m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
				
				if (!m_bActive && m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
				{
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
				}				
			}
			
			if (m_bShow && m_lpContainer)
			{
				m_lpContainer->vRender();
                m_Rect = ((CSpriteContainer*)m_lpContainer)->m_RenderRect;
                
                /*
				if (m_Name.size())
                {
                    if (m_Font)
                        m_lpEngine->GetSystemFontString()->SetFont(m_Font);
                    else
                        m_lpEngine->GetSystemFontString()->SetFont(m_lpEngine->GetSystemSmallFont());                    					

					float OldScale = m_lpEngine->GetSystemFontString()->GetScale();
					float NewScale = m_FontScale * m_Scale;
					m_lpEngine->GetSystemFontString()->SetScale(NewScale);
					
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
					m_lpEngine->GetSystemFontString()->SetScale(OldScale);
                }
				*/
			}
		}        		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CImage::vOnHit()
		{
			if (m_HitSound)
				m_HitSound->AddToPlayingList();
            
			if (m_lpClassScript)
			{
				ScriptBindObject();			
				m_lpClassScript->vOnHit();
			}
		}
		/*
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CImage::vSetSize(float w, float h)
		{
			((CSpriteContainer*)m_lpContainer)->vSetSize(w, h);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSpriteContainer::ORIGINTYPE CImage::vSetOriginType(const CSpriteContainer::ORIGINTYPE ot) 
		{
			return ((CSpriteContainer*)m_lpContainer)->SetOriginType(ot);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		DWORD  CImage::vSetAlignmentType(const DWORD type)
		{
			return ((CSpriteContainer*)m_lpContainer)->SetAlignmentType(type);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CImage::vSetAlignmentRect(const LYNXRECT& r)
		{
			return ((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&r);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CImage::vGetAlignmentRect(LYNXRECT& r)
		{
			return ((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&r);
		}
		*/
	}
}
