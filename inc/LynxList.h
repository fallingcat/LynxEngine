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

#ifndef __LYNXLIST_H__
#define __LYNXLIST_H__

#include <LynxType.h>
#include <LynxMem.h>

#ifdef __USE_STL__
	#include <list>
#endif

namespace LynxEngine 
{
	#ifdef __USE_STL__
		template <class Type> class CList : public std::list<Type>		
		{				
		public:						
			LYNXFORCEINLINE Type&			get(const unsigned int index); 				
			LYNXFORCEINLINE Type&			operator [] (const unsigned int index) {return get(index);};			
		};
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		template <class Type>
		Type& CList<Type>::get(const unsigned int index)
		{
			typename std::list<Type>::iterator    		Pos;
			
			LYNX_ASSERT(index < (unsigned int)std::list<Type>::size() && index >= 0);				

			Pos = std::list<Type>::begin();
			std::advance(Pos, index);
			
			return (*Pos);
		}		
		#define CIterator					iterator
	#else
		template <class Type> class CList;

		template <class Type> class LYNXENGTEMPLATE CDualLinkNode
		{
		protected:
			Type								m_Value;
			CDualLinkNode<Type>*				m_lpLast;	
			CDualLinkNode<Type>*				m_lpNext;				
		public:			
			CDualLinkNode() {m_lpLast = m_lpNext = NULL; };
			virtual ~CDualLinkNode() {};

			//LYNXFORCEINLINE Type& operator *() {return m_Value; };

			friend class CList<Type>;
			friend CList<Type>::CIterator;
		};

		template <class Type> class LYNXENGTEMPLATE CList
		{			
		public:	
			class LYNXENGTEMPLATE CIterator
			{
			protected:
				CDualLinkNode<Type>*			m_lpNode;
			public:				
				CIterator() {m_lpNode = NULL; };
				CIterator(CDualLinkNode<Type>* lpn) {m_lpNode = lpn; };
				virtual ~CIterator() {};

				LYNXFORCEINLINE void operator =(const CIterator& rhs) { m_lpNode = rhs.m_lpNode; };
				LYNXFORCEINLINE Type& operator *() {return m_lpNode->m_Value; };
				LYNXFORCEINLINE CIterator& operator ++() { m_lpNode = m_lpNode->m_lpNext; return (*this); };
				//LYNXFORCEINLINE CIterator& operator ++(int) { m_lpNode = m_lpNode->m_lpNext; return (*this); };
				LYNXFORCEINLINE CIterator& operator --() { m_lpNode = m_lpNode->m_lpLast; return (*this); };						

				LYNXFORCEINLINE bool operator ==(const CIterator& rhs) { return (m_lpNode == rhs.m_lpNode); };
				LYNXFORCEINLINE bool operator !=(const CIterator& rhs) { return (m_lpNode != rhs.m_lpNode); };

				friend class CList<Type>;
			};
		protected:		
			DWORD								m_Size;
			CIterator							m_Begin;	
			CIterator							m_End;			
		public:			
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			CList(void)
			{
				m_Size = 0;			
				m_End.m_lpNode = NULL;			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			virtual ~CList(void)
			{	
				clear();			
			}	
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
			LYNXFORCEINLINE bool empty() const {return (m_Size) ? false : true;};
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			void push_back(const Type& v)
			{
				CDualLinkNode<Type> *lpNode = LYNXNEW CDualLinkNode<Type>;
				lpNode->m_Value = v;

				if (m_Size)
				{
					m_End.m_lpNode->m_lpLast->m_lpNext = lpNode;
					lpNode->m_lpLast = m_End.m_lpNode->m_lpLast;				
				}
				else
				{			
					m_End.m_lpNode = LYNXNEW CDualLinkNode<Type>;
					m_End.m_lpNode->m_lpNext = NULL;
					m_Begin.m_lpNode = lpNode;
					lpNode->m_lpLast = NULL;				
				}		
				lpNode->m_lpNext = m_End.m_lpNode;
				m_End.m_lpNode->m_lpLast = lpNode;
				m_Size++;
			}	
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void clear()
			{
				m_Size = 0;

				CDualLinkNode<Type> *lpO = m_End.m_lpNode;

				while (lpO)
				{
					CDualLinkNode<Type> *lpTemp = lpO;
					lpO = lpTemp->m_lpLast;
					delete lpTemp;
				}
				m_Begin.m_lpNode = NULL;
				m_End.m_lpNode = NULL;			
			}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------
			void remove(const Type& v)
			{
				CDualLinkNode<Type> *lpNode;

				lpNode = m_Begin.m_lpNode;
				if (lpNode->m_Value == v)
				{
					LYNX_ASSERT(lpNode->m_lpNext);
					lpNode->m_lpNext->m_lpLast = lpNode->m_lpLast;
					m_Begin.m_lpNode = lpNode->m_lpNext;
					delete lpNode;
					m_Size--;
					return;
				}
				while (lpNode)
				{
					if (lpNode->m_Value == v)
					{
						LYNX_ASSERT(lpNode->m_lpLast);
						lpNode->m_lpLast->m_lpNext = lpNode->m_lpNext;
						LYNX_ASSERT(lpNode->m_lpNext);
						lpNode->m_lpNext->m_lpLast = lpNode->m_lpLast;
						delete lpNode;
						m_Size--;
						return;
					}
					lpNode = lpNode->m_lpNext;
				}
			}
			////-------------------------------------------------------------------------------------------------------
			////
			////  說明:   
			////-------------------------------------------------------------------------------------------------------
			//template <class Type> 
			//LYNXENGCLASS void CList<Type>::insert(int i, const Type& v)
			//{
			//}
			////-------------------------------------------------------------------------------------------------------
			////
			////  說明:   
			////-------------------------------------------------------------------------------------------------------
			//template <class Type> 
			//LYNXENGCLASS void CList<Type>::erase(CIterator& where)
			//{
			//}
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE DWORD size() const {return m_Size;};		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			Type& get(const unsigned int i)
			{
				CDualLinkNode<Type> *lpO;
				int c;

				LYNX_ASSERT((DWORD)i < m_Size && i >= 0);

				lpO = m_Begin.m_lpNode;
				c = 0;
				while (lpO)
				{
					if (c == i)
						return lpO->m_Value;

					lpO = lpO->m_lpNext;			
					c++;
				}		
				return (*(m_Begin));		
			}		
			//-------------------------------------------------------------------------------------------------------
			//
			//  說明:   
			//-------------------------------------------------------------------------------------------------------	
			LYNXFORCEINLINE Type& operator [] (const unsigned int index) {return get(index); };
		};			
	#endif
}

#endif