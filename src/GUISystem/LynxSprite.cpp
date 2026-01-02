#include <LynxEngine_PCH.h>

#include <GUISystem/LynxSprite.h>
#include <LynxLogger.h>
#include <LynxFileStream.h>	
#include <LynxZipStream.h>	
#include <LynxFileOutputdevice.h>	
#include <LynxHTMLOutputDevice.h>
#include <LynxwxTextOutputDevice.h>
#include <LynxEngine.h>
#include <LynxResource.h>
#include <Font/LynxFont.h>
#include <Font/LynxFontString.h>
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
		//IMPLEMENT_CLASSSCRIPT(CSprite, CControl)
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSprite::CSprite(CGUISystem* lpsys)
		: CControl(lpsys)
		{
			m_ControlType = CT_SPRITE;
			m_lpContainer = NULL;
			m_bUseCustomHitRect = LYNX_FALSE;
			
			m_TransitionOffset = CVector2(0.0f, 0.0f);

			m_bActive = LYNX_TRUE;
			m_bShow = LYNX_TRUE;            
            
			//INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSprite::~CSprite()
		{
			if (m_lpContainer)
			{
				m_lpEngine->DestroyContainer(m_lpContainer);
				m_lpContainer = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CSprite::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);

			//REGISTER_SCRIPT_METHOD(lps, "SetTargetScale",	LynxEngine::GUISystem::CSprite, void, _SetTargetScale, (const float, const float))		

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSprite::vCreate(CCreationDesc* desc)
		{	
			CSpriteCreationDesc* lpButtonDesc = (CSpriteCreationDesc*)desc;
			CSpriteCreationDesc* lpSpriteDesc = (LynxEngine::CSpriteCreationDesc*)(&lpButtonDesc->m_SpriteDesc);	

			m_lpContainer = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
			m_lpContainer->SetName(desc->m_Name);	
			m_lpContainer->vCreate(lpSpriteDesc);
			m_Size.x = ((CSpriteContainer*)m_lpContainer)->GetWidth();
			m_Size.y = ((CSpriteContainer*)m_lpContainer)->GetHeight();

			((CSpriteContainer*)m_lpContainer)->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);	
			((CSpriteContainer*)m_lpContainer)->SetOriginType(CSpriteContainer::OT_CENTER);				
			((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&m_TargetAlignmentRect);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSprite::SetTransitionOffset(const Math::CVector2& offset)
		{
			m_TransitionOffset = offset;
			m_TotalOffset = CVector2(0.0f, 0.0f);
			LYNXRECT Rect = m_TargetAlignmentRect;
			Rect.x1 += m_TransitionOffset.x;
			Rect.x2 += m_TransitionOffset.x;
			Rect.y1 += m_TransitionOffset.y;
			Rect.y2 += m_TransitionOffset.y;
			if (m_lpContainer)
				((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&Rect);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSprite::vInit()
		{	
			CObj::vInit();
		
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSprite::TransitionLoop()
		{
			if (m_TransitionOffset.x != 0.0f || m_TransitionOffset.y != 0.0f)
			{
				LYNXRECT Rect;
				((CSpriteContainer*)m_lpContainer)->GetAlignmentRect(&Rect);
				CVector2 Dir;
				Dir.x = m_TargetAlignmentRect.x1 - Rect.x1;
				Dir.y = m_TargetAlignmentRect.y1 - Rect.y1;		

				if (Dir.Length() < 1.0f)
				{
					Rect = m_TargetAlignmentRect;
					m_TotalOffset = CVector2(0.0f, 0.0f);
				}
				else
				{
					Dir /= 3.0f;
					m_TotalOffset += Dir;
                    Rect.x1 = m_TargetAlignmentRect.x1 + m_TransitionOffset.x + m_TotalOffset.x;
					Rect.x2 = m_TargetAlignmentRect.x2 + m_TransitionOffset.x + m_TotalOffset.x;
					Rect.y1 = m_TargetAlignmentRect.y1 + m_TransitionOffset.y + m_TotalOffset.y;
					Rect.y2 = m_TargetAlignmentRect.y2 + m_TransitionOffset.y + m_TotalOffset.y;
				}
				((CSpriteContainer*)m_lpContainer)->SetAlignmentRect(&Rect);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSprite::vRender()
		{	
			if (m_bShow && m_lpContainer)
			{
				m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
				TransitionLoop();

				if (!m_bActive && m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
				{
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
				}
				/*
				if (m_bActive)
				{
					m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = NORMAL;
					TransitionLoop();
				}
				else
				{
					if (m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->GetNumTextures() > INACTIVE)
						m_lpContainer->GetlpCurrentMaterial()->GetTextureAnimation(0)->m_CurrentTexture = INACTIVE;
				}
				*/
			}

			if (m_bShow && m_lpContainer)
			{
				Math::CVector2 Size = m_Size * m_Scale;
				((CSpriteContainer*)m_lpContainer)->vSetSize(Size.x, Size.y);
			
				m_lpContainer->vRender();
                if (!m_bUseCustomHitRect)
                    m_Rect = ((CSpriteContainer*)m_lpContainer)->m_RenderRect;             
			}			
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		void CSprite::SetHitSound(const CString& name, const CString& filename)
        {
            m_lpEngine->GetlpSoundSystem()->LoadSound(m_HitSound, name, filename);
            m_HitSound->SetLoops(0);
            m_HitSound->SetVolume(1.0f);
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSprite::vHitTest(const LYNXPOINT2D* p)
		{
			if (m_bActive && m_bAcceptInput)
			{
				if (p->x >= m_Rect.x1 && p->x <= m_Rect.x2 &&
					p->y >= m_Rect.y1 && p->y <= m_Rect.y2)
				{
					return LYNX_TRUE;
				}
			}
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSprite::vOnHit()
		{
            if (m_HitSound)
                m_HitSound->vPlay();
            
			if (m_lpClassScript)
			{
				ScriptBindObject();			
				m_lpClassScript->vOnHit();
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSprite::vSetSize(float w, float h)
		{
			((CSpriteContainer*)m_lpContainer)->vSetSize(w, h);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CSprite::vSetAlignmentType(const DWORD type)
		{
			((CSpriteContainer*)m_lpContainer)->SetAlignmentType(type);
		}		
	}
}
