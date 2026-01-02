//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXCAMERA_H__
#define __LYNXCAMERA_H__

#include <SceneSystem/DynObj/LynxDynObj.h>
#include <GraphicsSystem/LynxShadowMap.h>
#include <Container/LynxCameraContainer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CCamera : public CDynamicObj
		{	
			LYNX_DECLARE_CLASS(CCamera);
		public:		
		protected:						
			CVector3					m_TransitionPos;
			CVector3					m_TransitionLook;
			float						m_TrasitionStep;
		public:						
			CCamera(CScene* const lpscene);			
			virtual ~CCamera();		

			DECLARE_CLASSSCRIPT		
			LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);

			virtual void				vRelease();			
			virtual LYNXBOOL			vCreate();	
			virtual void				vLoop(float step);

			LYNXFORCEINLINE CCameraContainer* GetCamera() {return (CCameraContainer*)m_lpContainer; }

			void						TransitTo(const CVector3& pos, const CVector3& look, const float& step);
		};		
	}
}

#endif