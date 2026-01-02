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

#ifndef __LYNXMAP_H__
#define __LYNXMAP_H__

#include <LynxArray.h>

namespace LynxEngine 
{
	template <class KeyType, class Type> class LYNXENGTEMPLATE CMap
	{			
	public:	
		struct Node {
			KeyType							m_Key;
			Type							m_Value;

			Node() {}; 
			Node(KeyType k, Type v) {m_Key = k; m_Value = v; }; 
		};
		class LYNXENGTEMPLATE CIterator
		{
		protected:
			Node*							m_lpNode;		
			unsigned int					m_Index;
		public:				
			CIterator() {m_lpNode = NULL; m_Index = 0; };
			CIterator(Node* pn, int index = 0) {m_lpNode = pn; m_Index = index; };
			virtual ~CIterator() {};

			LYNXFORCEINLINE void operator =(const CIterator& rhs) { m_lpNode = rhs.m_lpNode; m_Index = rhs.m_Index; };
			LYNXFORCEINLINE Type& operator *() {return (m_lpNode->m_Value); };
			LYNXFORCEINLINE CIterator& operator ++() { ++m_Index; return (*this); };
			//LYNXFORCEINLINE CIterator& operator ++(int) { m_lpNode = m_lpNode->m_lpNext; return (*this); };
			LYNXFORCEINLINE CIterator& operator --() { --m_Index; return (*this); };		

			LYNXFORCEINLINE bool operator ==(const CIterator& rhs) { return (m_Index == rhs.m_Index); };
			LYNXFORCEINLINE bool operator !=(const CIterator& rhs) { return (m_Index != rhs.m_Index); };

			friend class CHashTable<KeyType, Type>;
		};
	protected:		
		CArray<Node>						m_Array;
		CIterator							m_Begin;	
		CIterator							m_End;	

		LYNXFORCEINLINE unsigned int		Hash(unsigned int k) {return (k + (k >> 3)) & (m_Array.size()-1); };
	public:		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CMap(void)
		{
			m_Array.clear();		
			m_Begin.m_lpNode = 0;
			m_Begin.m_Index = 0;
			m_End.m_lpNode = 0;
			m_End.m_Index = m_Array.size();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CMap(int size)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		virtual ~CMap(void)
		{	
			clear();			
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE CIterator& begin() {return m_Begin; };	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE CIterator& end() {return m_End; };	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE void resize(const unsigned int s) {m_Array.resize(s); m_End.m_Index = m_Array.size();};
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE void clear() {m_Array.clear(); };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE void remove();
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE DWORD max_size() {return m_Array.size();};	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void insert(KeyType k, const Type& v)
		{
			unsigned int Pos = Hash(k);
			m_Array.insert(Pos, Node(k, v));
		}	
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//Type* find(const KeyType k)
		//{
		//	unsigned int Pos = Hash(k);
		//	LYNX_ASSERT((DWORD)Pos < m_Array.size() && Pos >= 0);
		//	for (DWORD i=Pos; i<m_Array.size(); ++i)
		//	{
		//		if (m_Array[i].m_Key == k)
		//			return &m_Array[i].m_Value;
		//	}
		//	return NULL;
		//}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		CIterator find(const KeyType k)
		{
			unsigned int Pos = Hash(k);
			LYNX_ASSERT((DWORD)Pos < m_Array.size() && Pos >= 0);
			for (DWORD i=Pos; i<m_Array.size(); ++i)
			{
				if (m_Array[i].m_Key == k)
					return CIterator(&m_Array[i], i);
			}
			return m_End;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE Type& get(const unsigned int i) {return m_Array[i].m_Value; };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE const Type&	operator [] (const unsigned int index) const {return get(index); };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXFORCEINLINE Type& operator [] (const unsigned int index) {return get(index); };		
	};				
}

#endif