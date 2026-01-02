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
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <Container/LynxBoneContainer.h>

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
	CBoneContainer::CBoneContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_ContainerType = BONE;		
		m_bRenderable = LYNX_FALSE;
		m_lpPhyObj = NULL;
		m_Physics2ContainerM.Identity();
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	CBoneContainer::~CBoneContainer(void)
	{
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @brief .
	*
	*	@param .	
	*	@return. 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CBoneContainer::vInstance(CContainer& rhs)
	{
		CContainer::vInstance(rhs);
		CBoneContainer* RHS = LynxEngine::Container_Cast<CBoneContainer*>(&rhs);
		m_Name = RHS->GetName();
		m_BoneID = RHS->m_BoneID;
		m_VertexConstantIndex = RHS->m_VertexConstantIndex;
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
	void CBoneContainer::vCalculateLOD(CCameraContainer* cam)
	{
		SetCurrentLOD(0);
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Copy function.
	*
	*	@param m <in>The right hand side object.	
	*	@return This object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CBoneContainer& CBoneContainer::operator =(const CBoneContainer& b)
	{	
		m_lpCurrentMaterial = b.m_lpCurrentMaterial;

		return (*this);
	}		
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief Create physics object.
	*
	*	@param t <in>Physics object type.	
	*	@param g <in>Physics shape which will be attched to physics object.	
	*	@return Created physics object。 
	*/
	//-------------------------------------------------------------------------------------------------------
	PhysicsSystem::CPhysicsObj*	CBoneContainer::CreatePhyObj(PhysicsSystem::CPhysicsObj::TYPE t, PhysicsSystem::CPhysicsShape* g)
	{
		m_lpPhyObj = m_lpEngine->vCreatePhysicsObj(t, g);
		return m_lpPhyObj;
	}
}