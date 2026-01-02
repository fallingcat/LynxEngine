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

#ifndef __LYNXARRAY_H__
#define __LYNXARRAY_H__

#include <LynxType.h>
#include <LynxMem.h>
//#include <LynxAllocator.h>

#ifdef __USE_STL__
	#include <vector>
#endif

namespace LynxEngine 
{
	#ifdef __USE_STL__
		#define CIterator					iterator
		template <class Type> class CArray : public std::vector<Type>		
		{				
		public:														
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void getData(Type* dest, int num = std::vector<Type>::size()) 
			{
				for (int i=0; i<num; ++i)
				{
					dest[i] = (*this)[i];
				}				
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void setData(const Type* data, int num = std::vector<Type>::size()) 
			{ 
				for (int i=0; i<num; ++i)
				{
					(*this)[i] = data[i];
				}				
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			typename std::vector<Type>::CIterator insert(const unsigned int pos, const Type& v) 
			{
				typename std::vector<Type>::CIterator Pos;

				Pos = std::vector<Type>::begin()+pos;
				return std::vector<Type>::insert(Pos, v);
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			typename std::vector<Type>::CIterator insert(typename std::vector<Type>::CIterator _where, const Type& v) 
			{
				return std::vector<Type>::insert(_where, v);
			}
		};		
	#else
		template <class Type> class LYNXENGTEMPLATE CArray
		{	
		public:
			class LYNXENGTEMPLATE CIterator
			{
			protected:
				Type*							m_lpArray;					
				unsigned int					m_Index;
			public:				
				CIterator() {m_lpArray = NULL; m_Index = 0; };
				CIterator(Type* pa, int index = 0) {m_lpArray = pa; m_Index = index; };
				virtual ~CIterator() {};

				LYNXFORCEINLINE void operator =(const CIterator& rhs) { m_lpArray = rhs.m_lpArray; m_Index = rhs.m_Index; };
				LYNXFORCEINLINE Type& operator *() {return m_lpArray[m_Index]; };								
				LYNXFORCEINLINE CIterator& operator ++() { ++m_Index; return (*this); };
				LYNXFORCEINLINE CIterator& operator +(int offset) { m_Index+=offset; return (*this); };
				//LYNXFORCEINLINE CIterator& operator ++(int) { m_lpNode = m_lpNode->m_lpNext; return (*this); };
				LYNXFORCEINLINE CIterator& operator --() { --m_Index; return (*this); };		

				LYNXFORCEINLINE bool operator ==(const CIterator& rhs) { return (m_Index == rhs.m_Index); };
				LYNXFORCEINLINE bool operator !=(const CIterator& rhs) { return (m_Index != rhs.m_Index); };

				friend class CArray<Type>;
			};
		protected:		
			DWORD								m_Size;
			Type*								m_lpArray;	
			CIterator							m_Begin;	
			CIterator							m_End;	
		public:		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			CArray() {m_Size = 0; m_lpArray = NULL; m_Begin.m_lpArray = m_lpArray; m_Begin.m_Index = 0; m_End.m_lpArray = m_lpArray; m_End.m_Index = m_Size; };		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			CArray(const int size) {m_Size = size; m_lpArray = LYNXNEW Type[m_Size]; m_Begin.m_lpArray = m_lpArray; m_Begin.m_Index = 0; m_End.m_lpArray = m_lpArray; m_End.m_Index = m_Size; };
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			virtual ~CArray() {clear(); };
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE CIterator& begin() {return m_Begin;};	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE CIterator& end() {return m_End;};	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void clear()
			{
				m_Size = 0;
				LYNXDEL [] m_lpArray;
				m_lpArray = NULL;
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE bool empty() const {return (m_Size) ? false : true;};
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE void insert(unsigned int pos, const Type& v) {m_lpArray[pos] = v; };		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE const DWORD size() const {return m_Size;};		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void resize(const unsigned int s)
			{
				if (s)
				{
					Type* m_lpNew = LYNXNEW Type[s];		
					for (DWORD i=0; i<m_Size; ++i)
					{
						m_lpNew[i] = m_lpArray[i];
					}
					LYNXDEL [] m_lpArray;
					m_lpArray = m_lpNew;
					m_Size = s;		
					m_Begin.m_lpArray = m_lpArray; 
					m_Begin.m_Index = 0; 
					m_End.m_lpArray = m_lpArray; 
					m_End.m_Index = m_Size;
				}
				else
				{
					clear();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			Type& get(const unsigned int i) const
			{
				LYNX_ASSERT((DWORD)i < m_Size && i >= 0);
				return m_lpArray[i];		
			}			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE const Type*	getData() {return m_lpArray; };		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE void getData(Type* dest) {lynxMemCpy(dest, m_lpArray, sizeof(Type)*m_Size); };		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE void setData(const Type* data, int num) {lynxMemCpy(m_lpArray, data, sizeof(Type)*num); };								
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE Type& operator [] (const unsigned int index) const {return get(index); };			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void operator =(const CArray<Type>& rhs)
			{
				if (rhs.size())
				{
					resize(rhs.size());
					for (int i=0; i<m_Size; ++i)
					{
						m_lpArray[i] = rhs.m_lpArray[i];
					}
				}
				else
				{
					clear();
				}
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			//LYNXFORCEINLINE const CIterato& operator [] (const unsigned int index) const {return CIterator(m_lpArray, index); };
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			//LYNXFORCEINLINE CIterator& operator [] (const unsigned int index) {return CIterator(m_lpArray, index); };			
		};					
	#endif
}

#endif