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
//  Created by Owen Wu : 2006/11/16
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXANIMATIONSET_H__
#define __LYNXANIMATIONSET_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace Animation
	{
		class LYNXENGCLASS CAnimationSet : public CObj
		{	
			LYNX_DECLARE_CLASS(CAnimationSet);
		public:	
		protected:										
			CList<CAnimation*>					m_AnimationSet;	
			CAnimation*							m_lpCurrentAnimation;
		public:			
			CAnimationSet(CEngine* lpe);			
			virtual ~CAnimationSet();	

			LYNXINLINE CAnimation*				GetAnimation(int a) {return m_AnimationSet[a]; };
			LYNXINLINE CAnimation*				GetAnimation(const CString& name) {return Find(name); };
			LYNXINLINE CList<CAnimation*>&		GetAnimationList() {return m_AnimationSet; };
			LYNXINLINE DWORD					GetNumAnimations() {return (DWORD)m_AnimationSet.size(); };
			LYNXINLINE void						AddAnimation(CAnimation* a) { m_AnimationSet.push_back(a); };
			CAnimation*							Find(const CString& name);
			void								ReleaseAllAnimations(void);
			LYNXINLINE void						DeleteAnimation(CAnimation* a) {m_AnimationSet.remove(a); };
			void								DeleteAnimation(const CString& name);
			LYNXINLINE CAnimation*				GetCurrentAnimation() const { return m_lpCurrentAnimation; };			
			LYNXINLINE void						SetCurrentAnimation(CAnimation* lpa) {m_lpCurrentAnimation = lpa; };			
			void								SetCurrentAnimation(const CString& name, LYNXBOOL bresettime = LYNX_TRUE);
			void								ResetTime();

			virtual void						vInstance(CAnimationSet& rhs);

			virtual LYNXBOOL					vLoad(CUserProfile* file, CXMLElement* c);
			virtual LYNXBOOL					vSave(CUserProfile* file, CXMLElement* c);
		};
	}
}
#endif

