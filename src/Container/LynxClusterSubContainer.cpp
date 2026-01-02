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
#include <Container/LynxClusterSubContainer.h>

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
	CClusterSubContainer::CClusterSubContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_ContainerType = CLUSTER_SUB;	
		m_bRenderable = LYNX_FALSE;
		m_SubContainerIndex = 0;
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
	CClusterSubContainer::~CClusterSubContainer(void)
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
	LYNXBOOL CClusterSubContainer::vCreate(const CCreationDesc* desc)
	{
		CClusterSubCreationDesc* Desc = (CClusterSubCreationDesc*)desc;			

		m_SubContainerIndex = Desc->m_SubContainerIndex;
		Desc->m_lpClusterContainer->AddSubContainer(this);

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
	void CClusterSubContainer::vSetPosition(const Math::CVector3& pos)
	{
		CContainer::vSetPosition(pos);
		m_lpParentContainer->vSetPosition(m_SubContainerIndex, pos);
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
	void CClusterSubContainer::vSetSize(const Math::CReal& w, const Math::CReal& h)	
	{
		m_lpParentContainer->vSetSize(m_SubContainerIndex, w, h);
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
	void CClusterSubContainer::vGetSize(Math::CReal& w, Math::CReal& h) const
	{
		m_lpParentContainer->vGetSize(m_SubContainerIndex, w, h);
	}
}