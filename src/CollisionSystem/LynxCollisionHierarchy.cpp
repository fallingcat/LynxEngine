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
#include <CollisionSystem/LynxCollisionHierarchy.h>

namespace LynxEngine 
{	
	namespace CollisionSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCollisionHierarchy::CCollisionHierarchy(void)
		{
			m_CollisionHieList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CCollisionHierarchy::~CCollisionHierarchy(void)
		{
			for (int i=0; i<m_CollisionHieList.size(); i++)
			{
				for (CList<CCollisionObj*>::CIterator j=m_CollisionHieList[i].begin(); j!=m_CollisionHieList[i].end(); j++)
				{
					LYNXDEL (*j);
				}
				m_CollisionHieList[i].clear();
			}
			m_CollisionHieList.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CCollisionHierarchy::Create(int num) 
		{
			for (int i=0; i<m_CollisionHieList.size(); i++)
			{
				for (CList<CCollisionObj*>::CIterator j=m_CollisionHieList[i].begin(); j!=m_CollisionHieList[i].end(); ++j)
				{
					LYNXDEL (*j);
				}
				m_CollisionHieList[i].clear();
			}
			m_CollisionHieList.clear();

			m_CollisionHieList.resize(num); 
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*  @brief .
		*
		*	@param .	
		*	@return. 
		*/
		//-------------------------------------------------------------------------------------------------------
		CCollisionHierarchy& CCollisionHierarchy::operator=(CCollisionHierarchy& rhs)
		{
			for (int i=0; i<m_CollisionHieList.size(); i++)
			{
				for (CList<CCollisionObj*>::CIterator j=m_CollisionHieList[i].begin(); j!=m_CollisionHieList[i].end(); ++j)
				{
					LYNXDEL (*j);
				}
				m_CollisionHieList[i].clear();
			}
			m_CollisionHieList.clear();

			m_CollisionHieList.resize(rhs.m_CollisionHieList.size());
			for (int i=0; i<rhs.m_CollisionHieList.size(); i++)
			{
				for (CList<CCollisionObj*>::CIterator j=rhs.m_CollisionHieList[i].begin(); j!=rhs.m_CollisionHieList[i].end(); ++j)
				{
					
					CollisionSystem::CCollisionObj* CollObj = glpKernel->GetlpEngine()->GetlpCollisionSystem()->vCreateCollisionObj((*j)->GetType());
					CollObj->vInstance(*(*j));
					Add(i, CollObj);
					
					/*
					switch ((*j)->GetType())
					{
						case CollisionSystem::CCollisionObj::OBB:
						{
						#ifndef __ANDROID__
							CollisionSystem::CBtOBB* Box = (CollisionSystem::CBtOBB*)(*j);
							Add(i, LYNXNEW CollisionSystem::CBtOBB(Box->GetExtension()));
						#endif
							break;
						}

						case CollisionSystem::CCollisionObj::CAPSULE:
						{
						#ifndef __ANDROID__
							CollisionSystem::CBtCapsule* Cap = (CollisionSystem::CBtCapsule*)(*j);
							Add(i, LYNXNEW CollisionSystem::CBtCapsule(Cap->GetCenter(), Cap->GetRadius(), Cap->GetHeight()));
						#endif
							break;
						}
					}
					*/
				}				
			}			
			return (*this);
		}
	}
}