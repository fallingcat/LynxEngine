//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <InputSystem/LynxInputSystem.h>
#include <InputSystem/LynxInputDevice.h>
#include <InputSystem/LynxKeyboard.h>
#include <InputSystem/LynxMouse.h>

namespace LynxEngine 
{	
	namespace GUISystem 
	{
		IMPLEMENT_CLASSSCRIPT(CGUISystem, CSystem)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGUISystem::CGUISystem(CEngine* const lpengine)
		: CSystem(lpengine)
		{			
			m_ClipRect.x1 = 0;
			m_ClipRect.y1 = 0;
			m_ClipRect.x2 = 1024;
			m_ClipRect.y2 = 768;

			m_ReferenceWidth = 1024;
			m_ReferenceHeight = 768;

			m_PositionFactor.x = 1.0f;
			m_PositionFactor.y = 1.0f;

			m_ScaleFactor.x = 1.0f;
			m_ScaleFactor.y = 1.0f;

			m_OriginPos.x = 0;
			m_OriginPos.y = 0;

			m_ControlFactory.SetParam(this);

			//INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGUISystem::~CGUISystem(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGUISystem::vCreate() 
		{
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CGUISystem::CreateCButton(CGUISystem* s)
		{
			return LYNXNEW CButton(s);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CGUISystem::CreateCStateButton(CGUISystem* s)
		{
			return LYNXNEW CStateButton(s);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CGUISystem::CreateCImage(CGUISystem* s)
		{
			return LYNXNEW CImage(s);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CGUISystem::CreateCSlider(CGUISystem* s)
		{
			return LYNXNEW CSlider(s);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CGUISystem::CreateCText(CGUISystem* s)
		{
			return LYNXNEW CText(s);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGUISystem::vInit() 
		{	
			CSystem::vInit();

			RegisterControl("CButton", CreateCButton);
			RegisterControl("CStateButton", CreateCStateButton);
			RegisterControl("CImage", CreateCImage);
			RegisterControl("CSlider", CreateCSlider);
			RegisterControl("CText", CreateCText);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGUISystem::RegisterControl(const CAnsiString& t, CControl* (*func)(CGUISystem*))
		{
			m_ControlFactory.Register(t, func);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CControl* CGUISystem::CreateControl(const CAnsiString& classname)
		{
			return m_ControlFactory.CreateObject(classname);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGUISystem::ComputeLayoutFactor(LYNXBOOL bkeepratio, LYNXAXISTYPE major_axis)
		{
            int Width, Height;
			
            if (m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x2 == m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1)
            {
                Width = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedRenderBuffer(0)->vGetWidth();
                Height = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetCachedRenderBuffer(0)->vGetHeight();
            }
            else
            {
                Width = m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x2- m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().x1;
                Height = m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y2 - m_lpEngine->GetlpGraphicsSystem()->GetViewportRect().y1;
            }
			if (bkeepratio)
			{
                float ScaleFactorX = (float)(Width)/(float)(m_ReferenceWidth);
				float ScaleFactorY = (float)(Height)/(float)(m_ReferenceHeight);

				switch (major_axis)
				{
					case LYNX_X_AXIS:
						m_ScaleFactor.x = m_ScaleFactor.y = ScaleFactorX;
						m_PositionFactor.x = m_PositionFactor.y = ScaleFactorX;												
						break;

					case LYNX_Y_AXIS:
						m_ScaleFactor.x = m_ScaleFactor.y = ScaleFactorY;
						m_PositionFactor.x = m_PositionFactor.y = ScaleFactorY;						
						break;
						
					default:
						m_ScaleFactor.x = m_ScaleFactor.y = LYNX_MIN(ScaleFactorX, ScaleFactorY);
						m_PositionFactor.x = ScaleFactorX;
						m_PositionFactor.y = ScaleFactorY;
						break;
				}				
			}
			else
			{
				m_ScaleFactor.x = (float)(Width)/(float)(m_ReferenceWidth);
				m_ScaleFactor.y = (float)(Height)/(float)(m_ReferenceHeight);
				m_PositionFactor.x = m_ScaleFactor.x;
				m_PositionFactor.y = m_ScaleFactor.y;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CGUISystem::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{		
			RegisterSuperClassMethod(lps);			

			return LYNX_TRUE;
		}
	}
}