#include <LynxEngine_PCH.h>

#include <GUISystem/LynxDummyControl.h>
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

namespace LynxEngine
{
	namespace GUISystem
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CDummyControl::CDummyControl(CGUISystem* lpsys)
		: CControl(lpsys)
		{
			m_ControlType = CT_DUMMY;		
			m_bActive = LYNX_TRUE;
			m_bShow = LYNX_TRUE;
            m_bAcceptInput = LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CDummyControl::~CDummyControl()
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDummyControl::vCreate(CCreationDesc* desc)
		{	
			
            return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDummyControl::vInit()
		{	
			CObj::vInit();
			
			return LYNX_TRUE;
		}						
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CDummyControl::vRender()
		{				
		}        		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CDummyControl::vOnHit()
		{            
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  :
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CDummyControl::vHitTest(const LYNXPOINT2D* p)
		{			
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		CSpriteContainer::ORIGINTYPE CDummyControl::vSetOriginType(const CSpriteContainer::ORIGINTYPE ot) 
		{
			return CSpriteContainer::OT_CENTER;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		DWORD  CDummyControl::vSetAlignmentType(const DWORD type)
		{
			return 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CDummyControl::vSetAlignmentRect(const LYNXRECT& r)
		{			
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void  CDummyControl::vGetAlignmentRect(LYNXRECT& r)
		{			
		}		
	}
}
