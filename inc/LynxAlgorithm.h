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

#ifndef __LYNXALGORITHM_H__
#define __LYNXALGORITHM_H__

#include <LynxType.h>
#include <LynxMem.h>

#ifdef __USE_STL__
	#include <algorithm>	
#endif

#ifdef __USE_STL__
	namespace LynxEngine 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		// find first matching val
		//-------------------------------------------------------------------------------------------------------	
		template<class It, class Type> 
		It find(It first, It last, const Type& val)
		{	
			return std::find(first, last, val);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// find first matching val
		//-------------------------------------------------------------------------------------------------------	
		template<class It, class Type>
		It find_if(It first, It last, const Type& val)
		{
			return std::find_if(first, last, val);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// binary search val
		//-------------------------------------------------------------------------------------------------------	
		template<class It, class Type> 
		It binary_search(It first, It last, const Type& val)
		{	
			return std::binary_search(first, last, val);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		// sort elements
		//-------------------------------------------------------------------------------------------------------	
		template<class It> 
		void sort(It first, It last)
		{	
			return std::sort(first, last);
		}		
		////-------------------------------------------------------------------------------------------------------
		////
		//// sort elements by comp
		////-------------------------------------------------------------------------------------------------------	
		//template<class It> 
		//LYNXENGTEMPLATE bool sort_away(It e1, It e2)
		//{	
		//	return e1 < e2;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		//// sort elements by comp
		////-------------------------------------------------------------------------------------------------------	
		//template<class It> 
		//LYNXENGTEMPLATE bool sort_toward(It e1, It e2)
		//{	
		//	return e1 > e2;
		//}
		//-------------------------------------------------------------------------------------------------------
		//
		// sort elements by comp
		//-------------------------------------------------------------------------------------------------------	
		template<class It, class Comp> 
		LYNXFORCEINLINE void sort(It first, It last, Comp comp)
		{	
			return std::sort(first, last, comp);
		}
		//-------------------------------------------------------------------------------------------------------
		//		
		//-------------------------------------------------------------------------------------------------------	
		template<class It, class Comp> 
		LYNXFORCEINLINE It lower_bound(It first, It last, const Comp& comp)
		{	
			return std::lower_bound(first, last, comp);
		}
		//-------------------------------------------------------------------------------------------------------
		//		
		//-------------------------------------------------------------------------------------------------------	
		template<class It, class Comp> 
		LYNXENGTEMPLATE LYNXFORCEINLINE It upper_bound(It first, It last, const Comp& comp)
		{	
			return std::upper_bound(first, last, comp);
		}
	}
#else
	namespace LynxEngine 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		// find first matching val
		//-------------------------------------------------------------------------------------------------------	
		template<class It, class Type> 
		LYNXENGTEMPLATE It find(It first, It last, const Type& val)
		{			
			for (; first != last; ++first)
				if ((*first) == val)
					break;
			return (first);
		}
	}
#endif

#endif