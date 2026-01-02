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
#include <GraphicsSystem/LynxVertexLayout.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Container/LynxReferenceContainer.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CReferenceContainer::CReferenceContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_ContainerType = REFERENCE;	
		m_bRenderable = LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CReferenceContainer::~CReferenceContainer(void)
	{		
		vRelease();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CReferenceContainer::vPlay(CReal step)
	{
		LYNXBOOL bStop = LYNX_FALSE;
		static Math::CReal Values[4];
		CMatrix4 TM;

		CContainer::vPlay(step);			
			
		SaveTransformationMatrix();
				
		if (!m_lpParentContainer && m_AnimationSet->GetCurrentAnimation())
		{
			bStop = m_AnimationSet->GetCurrentAnimation()->Play(step);		
			m_AnimationSet->GetCurrentAnimation()->UpdateTransform();
			m_M = m_AnimationSet->GetCurrentAnimation()->m_M;
		}
		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CReferenceContainer::vRelease(void)
	{			
	}
    //-------------------------------------------------------------------------------------------------------
	/**
     *	@brief 。
     *
     *	@param 。
     *	@param 。  
     *	@return 。 
     */
	//-------------------------------------------------------------------------------------------------------
	void CReferenceContainer::vRender(CCameraContainer* cam)
	{		
		vCalculateLOD(cam);
        
		for (CList<CContainer*>::CIterator Sub = m_SubContainerList.begin(); Sub != m_SubContainerList.end(); ++Sub)
		{
			(*Sub)->vRender(cam);
		}
	}
}