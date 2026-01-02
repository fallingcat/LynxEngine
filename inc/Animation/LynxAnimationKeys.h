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
//  Created by Owen Wu : 2006/11/17
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXANIMATIONKEYS_H__
#define __LYNXANIMATIONKEYS_H__

#include <LynxCore.h>
#include <LynxArray.h>
#include <LynxList.h>
#include <LynxMathWrap.h>
#include <Animation/LynxAnimation.h>
#include <Animation/LynxAnimationKey.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{	
	namespace Animation
	{
		class LYNXENGCLASS CAnimationKeys: public CObj
		{	
			LYNX_DECLARE_CLASS(CAnimationKeys);
		public:			
			typedef enum {
				IT_LINEAR = 0,
				IT_BSPLINE,
				NUM_INTERPOLATIONTYPE,
			}INTERPOLATIONTYPE;
			CArray<CAnimationKey>		m_Keys;
		protected:		
			INTERPOLATIONTYPE			m_InterpolationType;
			CAnimation::KEYSTYPE		m_KeysType;
			int							m_NumKeyValues;
			CAnimation*					m_lpAnimation;
		public:			
			CAnimationKeys(CAnimation* lpani);			
			CAnimationKeys(CAnimation* lpani, int num, int values = 4);
			virtual ~CAnimationKeys();

			// Operators
			void						operator =(const CAnimationKeys& rhs);

			LYNXBOOL					vSave(CStream& s);
			LYNXBOOL					vLoad(CStream& s, LYNXBOOL skip = LYNX_FALSE);

			LYNXINLINE INTERPOLATIONTYPE GetInterpolationType() { return m_InterpolationType; };
			LYNXINLINE void				SetInterpolationType(INTERPOLATIONTYPE t) { m_InterpolationType = t; };
			LYNXINLINE CAnimation::KEYSTYPE	GetKeysType() {return m_KeysType; };
			LYNXINLINE void				SetKeysType(CAnimation::KEYSTYPE kt) {m_KeysType = kt; };
			LYNXINLINE void				SetlpAnimation(CAnimation* lpa) {m_lpAnimation = lpa; };
			LYNXINLINE CAnimation*		GetlpAnimation() {return m_lpAnimation; };
			virtual void				vInstance(CAnimationKeys& rhs);
			LYNXINLINE int				GetNumKeyValues() { return m_NumKeyValues; };
			void						CreateKeys(int num, int values = 4);
			void						InsertKey(int st, LYNXREAL* pv);
			void						InsertBeginKey(int st, int et, LYNXREAL* pv);
			void						AddDefaultKeys(int st, int et, LYNXREAL* pv, int numkeyvalues);
			void						Lerp(Math::CReal* values, const Math::CReal time);
			//void						Lerp(LYNXREAL* values, const Math::CReal time);
			void						ClearKeys();
		};
	}
}
#endif

