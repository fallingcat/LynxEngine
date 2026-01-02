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
#include <LynxAlgorithm.h>
#include <Animation/LynxAnimationSet.h>
#include <Animation/LynxAnimation.h>

namespace LynxEngine 
{	
	namespace Animation
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAnimationSet::CAnimationSet(CEngine* lpe)
		: CObj(lpe)
		{
			m_AnimationSet.clear();
			m_lpCurrentAnimation = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAnimationSet::~CAnimationSet(void)
		{
			for (CList<CAnimation*>::CIterator Ani=m_AnimationSet.begin(); Ani!=m_AnimationSet.end(); ++Ani)
				LYNXDEL (*Ani);
			m_AnimationSet.clear();

			m_lpCurrentAnimation = NULL;	
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
		void CAnimationSet::vInstance(CAnimationSet& rhs)
		{
			for (CList<CAnimation*>::CIterator Ani = rhs.m_AnimationSet.begin(); Ani != rhs.m_AnimationSet.end(); Ani++)
			{
				Animation::CAnimation* NewAni = LYNXNEW Animation::CAnimation(rhs.GetlpEngine()); 
				NewAni->vInstance(*(*Ani));
				NewAni->SetName((*Ani)->GetName());
				m_AnimationSet.push_back(NewAni);
			}			
			if (rhs.GetCurrentAnimation())
				SetCurrentAnimation(rhs.GetCurrentAnimation()->GetName());			
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
		CAnimation* CAnimationSet::Find(const CString& name)
		{
			CList<CAnimation*>::CIterator A;

			A = LynxEngine::find(m_AnimationSet.begin(), m_AnimationSet.end(), name);
			if (A != m_AnimationSet.end())
				return (*A);

			return NULL;
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
		void CAnimationSet::ResetTime()
		{
			for (CList<CAnimation*>::CIterator Ani = m_AnimationSet.begin(); Ani != m_AnimationSet.end(); ++Ani)
			{
				(*Ani)->ResetTime();
			}
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
		void CAnimationSet::DeleteAnimation(const CString& name)
		{
			CAnimation* A = Find(name);
			if (A)
				DeleteAnimation(A);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationSet::ReleaseAllAnimations(void)
		{
			for (CList<CAnimation*>::CIterator Ani=m_AnimationSet.begin(); Ani!=m_AnimationSet.end(); ++Ani)
				LYNXDEL (*Ani);
			m_AnimationSet.clear();

			m_lpCurrentAnimation = NULL;	
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
		void CAnimationSet::SetCurrentAnimation(const CString& name, LYNXBOOL bresettime)
		{
			CList<CAnimation*>::CIterator A;

			A = LynxEngine::find(m_AnimationSet.begin(), m_AnimationSet.end(), name);
			if (A != m_AnimationSet.end())
			{
				SetCurrentAnimation(*A);
				if (bresettime)
					(*A)->ResetTime();
			}
			else
				SetCurrentAnimation(NULL);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAnimationSet::vLoad(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();
			
			CXMLElement* AnimationKey = Root->FirstChildElement(_T("Animation"));
			while (AnimationKey)
			{
				CAnimation* lpAni = LYNXNEW Animation::CAnimation(m_lpEngine);
				lpAni->vLoad(file, AnimationKey);
				AddAnimation(lpAni);
				
				AnimationKey = AnimationKey->NextSiblingElement();
			}	

			SetCurrentAnimation(GetAnimation(0));

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAnimationSet::vSave(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			for (CList<CAnimation*>::iterator iter = m_AnimationSet.begin(); iter != m_AnimationSet.end(); ++iter)
			{
				CXMLElement* AnimationElem = file->GetDoc()->NewElement(_T("Animation"));
				Root->InsertEndChild(AnimationElem);

				(*iter)->vSave(file, AnimationElem);
			}

			return LYNX_TRUE;
		}
	}
}	