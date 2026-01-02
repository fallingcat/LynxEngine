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

#ifndef __LYNXANIMATIONKEY2_H__
#define __LYNXANIMATIONKEY2_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{	
	namespace Animation
	{
		class LYNXENGCLASS CAnimationKey// : public CObj
		{	
			LYNX_DECLARE_CLASS(CAnimationKey);
		protected:
			int								m_StartTime, m_EndTime;
			CArray<Math::CReal>				m_Values;		
		public:			
			CAnimationKey();			
			CAnimationKey(int num);			
			virtual ~CAnimationKey();		

			// Operators
			void							operator =(const CAnimationKey& rhs);

			LYNXINLINE void					SetStartTime(int s) {m_StartTime = s; };
			LYNXINLINE void					SetEndTime(int e) {m_EndTime = e; };
			LYNXINLINE int					GetStartTime() const {return m_StartTime; };
			LYNXINLINE int					GetEndTime() const {return m_EndTime; };
			LYNXINLINE CArray<Math::CReal>&	GetValues() { return m_Values; };
			LYNXINLINE Math::CReal			GetValue(int i) const { return m_Values[i]; };
			LYNXINLINE void					GetValues(Math::CReal* v, int num) { m_Values.getData(v, num); };
			LYNXINLINE void					GetValues(LYNXREAL* v, int num) { m_Values.getData((Math::CReal*)v, num); };
			LYNXINLINE void					SetValue(int i, Math::CReal v) { m_Values[i] = v; };			
			LYNXINLINE void					SetValues(Math::CReal* v, int num) { m_Values.setData(v, num); };			
			LYNXINLINE void					SetValues(LYNXREAL* v, int num) { m_Values.setData((Math::CReal*)v, num); };			
			LYNXINLINE void					SetNumValues(int num) { m_Values.resize(num); };

		friend class CAnimationKeys;
		};
	}
}
#endif

