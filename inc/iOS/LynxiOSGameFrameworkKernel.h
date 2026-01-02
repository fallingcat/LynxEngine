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

#ifndef __LYNXIOSGAMEFRAMEWORKKERNEL_H__
#define __LYNXIOSGAMEFRAMEWORKKERNEL_H__

#include <LynxObj.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GameFramework/LynxUIPage.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#import <iOS/LynxView.h>
#import <iOS/LynxViewController.h>

namespace LynxEngine 
{
	namespace GameFramework 
	{		
		namespace iOS
		{
			class LYNXENGCLASS CKernel : public LynxEngine::GameFramework::CKernel
			{	
				LYNX_DECLARE_CLASS(CKernel);
			protected:					
				LynxViewController*					m_lpViewController;
				LynxView*							m_lpView;				
				LYNXBOOL							m_bGestureRecognization;
			public:			
				CKernel();
				virtual ~CKernel();			

				virtual LYNXBOOL					vInit(INIT_DESC* desc = NULL);			
				virtual CEngine*					vCreateEngine();		
								
				LYNXFORCEINLINE LynxViewController*	GetlpViewController() {return m_lpViewController; };
				LYNXFORCEINLINE void				SetlpViewController(LynxViewController* lpvc) {m_lpViewController = lpvc; };
				LYNXFORCEINLINE LynxView*			GetlpView() {return m_lpView; };
				LYNXFORCEINLINE void				SetlpView(LynxView* lpv) {m_lpView = lpv; };
				
				LYNXFORCEINLINE LYNXBOOL			IsGestureRecognization() {return m_bGestureRecognization; };
				void								SetGestureRecognization(LYNXBOOL b);
				
				void								PrintEngineInfo();
			};
		}
	}
}
#endif