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

#include <LynxEngine_PCH.h>
#include <SceneSystem/StaticObj/LynxSky.h>
#include <Container/LynxSkyDomeContainer.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSky::CSky(CScene *lpscene)
		:CStaticObj(lpscene)
		{					
			m_StaticObjType = SKY;		
			m_SunLight = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CSky::~CSky(void)
		{
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CSky::vCreate(const LYNXCHAR *name, CContainer::TYPE mtype, CCreationDesc* desc)
		//LYNXBOOL CSky::vCreate(void)
		{				
			m_SunLight = m_lpScene->vCreateLight(CLight::DIRECTIONAL);
			//m_SunLight = m_lpScene->vCreateLight(CLight::SPOT);
			m_SunLight->SetName(_T("Sun Light"));
			m_SunLight->vCreate(1024, 1024);
			m_SunLight->m_Color.Red = 255;
			m_SunLight->m_Color.Green = 255;
			m_SunLight->m_Color.Blue = 255;
			m_SunLight->m_Radius = 2500.0f;												
			m_SunLight->m_Intensity = 1.0f;
			m_SunLight->m_Deg = 40.0f;
			m_SunLight->m_FadeOutDeg = 5.0f;
			m_SunLight->vComputeProxy();				
			m_SunLight->vInit();
			//m_lpScene->AddLight(m_SunLight);
			
			if (!CStaticObj::vCreate(name, mtype, desc))
				return LYNX_FALSE;

			if (m_lpContainer)
				((CSkyDomeContainer*)((CContainer*)m_lpContainer))->SetlpSunLight(m_SunLight);

			return LYNX_TRUE;
		}
	}
}