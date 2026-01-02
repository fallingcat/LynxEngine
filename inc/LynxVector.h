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

#include <LynxObj.h>
#ifdef __USE_STL__
	#include <vector>
#endif

namespace LynxEngine 
{
	#ifdef __USE_STL__
		template <class Type> class LYNXENGTEMPLATE CArray : public std::vector<Type>		
		{						
		};		
		#define CIterator					iterator
	#else
		template <class Type> class CList;

		template <class Type> class LYNXENGTEMPLATE CIndexedNode : public CObj
		{
			LYNX_DECLARE_CLASS(CSingleLinkNode);
		protected:
			int									m_Index;
			Type								m_Value;			
			CSingleLinkNode<Type>*				m_lpNext;				
		public:			
			CSingleLinkNode() { m_lpNext = NULL; };
			virtual ~CSingleLinkNode() {};

			//LYNXFORCEINLINE Type& operator *() {return m_Value; };

			friend class CArray<Type>;
			friend CArray<Type>::CIterator;
		};

		template <class Type> class LYNXENGTEMPLATE CArray : public CObj
		{			
		public:	
			class LYNXENGTEMPLATE CIterator : public CObj
			{
				LYNX_DECLARE_CLASS(CIterator);
			protected:
				CSingleLinkNode<Type>*			m_lpNode;
			public:				
				CIterator() {m_lpNode = NULL; };
				CIterator(CSingleLinkNode<Type>* lpn) {m_lpNode = lpn; };
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
			LYNX_DECLARE_CLASS(CList);
		protected:		
			DWORD								m_Size;
			CIterator							m_Begin;	
			CIterator							m_End;			
		public:			
			CList() {m_Size = 0; };
			virtual ~CList() {clear(); };

			LYNXFORCEINLINE void				push_back(const Type& v);		
			LYNXFORCEINLINE void				clear();
			LYNXFORCEINLINE void				remove(const Type& v);
			//LYNXFORCEINLINE void				insert(int i, const Type& v);		
			LYNXFORCEINLINE DWORD				size() {return m_Size;};		
			LYNXFORCEINLINE Type&				get(const unsigned int i);
			LYNXFORCEINLINE Type&				operator [] (const unsigned int index) {return get(index); };
			LYNXFORCEINLINE CIterator&			begin() {return m_Begin;};	
			LYNXFORCEINLINE CIterator&			end() {return m_End;};	
		};				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		template <class Type> 
		void CList<Type>::push_back(const Type& v)
		{
			CDualLinkNode<Type> *lpNode = LYNXNEW CDualLinkNode<Type>;
			lpNode->m_Value = v;

			if (m_Size)
			{
				lpNode->m_lpLast = m_End.m_lpNode;
				lpNode->m_lpNext = NULL;
				m_End.m_lpNode->m_lpNext = lpNode;
			}
			else
			{			
				m_Begin.m_lpNode = lpNode;
				lpNode->m_lpLast = NULL;
				lpNode->m_lpNext = NULL;
			}		
			m_End.m_lpNode = lpNode;
			m_Size++;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		template <class Type> 
		void CList<Type>::remove(const Type& v)
		{
			CDualLinkNode<Type> *lpNode;

			lpNode = m_Begin.m_lpNode;
			while (lpNode)
			{
				if (lpNode->m_Value == v)
				{
					if (lpNode->m_lpLast)
					{
						lpNode->m_lpLast->m_lpNext = lpNode->m_lpNext;
					}
					if (lpNode->m_lpNext)
					{
						lpNode->m_lpNext->m_lpLast = lpNode->m_lpLast;
					}			
					if (m_End.m_lpNode == lpNode)
					{
						m_End.m_lpNode = lpNode->m_lpLast;
					}
					delete lpNode;
					m_Size--;
					return;
				}
				lpNode = lpNode->m_lpNext;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		template <class Type> 
		void CList<Type>::clear()
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
		template <class Type> 
		Type& CList<Type>::get(const unsigned int i)
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
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//template <class Type> 
		//LYNXENGCLASS void CList<Type>::insert(int i, const Type& v)
		//{
		//}


		//template <class Type> class LYNXEXPORT CIterator : public CObj
		//{
		//public:
		//protected:
		//	Type								m_Value;
		//	CIterator<Type>						*m_lpLast;	
		//	CIterator<Type>						*m_lpNext;				
		//public:
		//	LYNX_DECLARE_CLASS(CIterator);
		//	CIterator() {m_lpLast = m_lpNext = NULL; };
		//	virtual ~CIterator() {};

		//	LYNXFORCEINLINE Type&				GetValue() {return m_Value; };
		//	LYNXFORCEINLINE void				SetValue(const Type& v) {m_Value = v; };
		//	LYNXFORCEINLINE CIterator<Type>* 	GetLast() {return m_lpLast;};
		//	LYNXFORCEINLINE CIterator<Type>*	GetNext() {return m_lpNext;};
		//	//LYNXFORCEINLINE void				SetLast(CIterator<Type>* o) {m_lpLast = o;};
		//	//LYNXFORCEINLINE void				SetNext(CIterator<Type>* o) {m_lpNext = o;};	

		//	LYNXFORCEINLINE Type& operator *() {return m_Value; };

		//friend:
		//	class CList<Type>;
		//};

		//template <class Type> class LYNXEXPORT CList : public CObj
		//{			
		//public:		
		//protected:		
		//	DWORD								m_Size;
		//	CIterator<Type>						*m_lpBegin;	
		//	CIterator<Type>						*m_lpEnd;			
		//public:
		//	LYNX_DECLARE_CLASS(CList);
		//	CList();
		//	~CList();

		//	LYNXFORCEINLINE void				push_back(const Type& v);		
		//	void								clear();
		//	void								remove(const Type& v);
		//	void								insert(int i, const Type& v);		
		//	LYNXFORCEINLINE DWORD				size() {return m_Size;};
		//	LYNXFORCEINLINE CIterator<Type>*	get(int i);	
		//	LYNXFORCEINLINE Type&				get_value(int i);
		//	LYNXFORCEINLINE CIterator<Type>*	begin() {return m_lpBegin;};	
		//	LYNXFORCEINLINE CIterator<Type>*	end() {return m_lpEnd;};	
		//};			
		//
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------	
		//template <class Type> 
		//CList<Type>::CList(void)
		//{
		//	m_Size = 0;
		//	m_lpBegin = NULL;
		//	m_lpEnd = NULL;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------	
		//template <class Type> 
		//CList<Type>::~CList(void)
		//{
		//}	
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------	
		//template <class Type> 
		//void CList<Type>::push_back(const Type& v)
		//{
		//	CIterator<Type> *lpIter = LYNXNEW CIterator<Type>;
		//	lpIter->m_Value = v;

		//	if (m_lpEnd)
		//	{
		//		lpIter->m_lpLast = m_lpEnd;
		//		lpIter->m_lpNext = NULL;
		//		m_lpEnd->m_lpNext = lpIter;
		//	}
		//	else
		//	{			
		//		m_lpBegin = lpIter;
		//		lpIter->m_lpLast = NULL;
		//		lpIter->m_lpNext = NULL;
		//	}		
		//	m_lpEnd = lpIter;
		//	m_Size++;
		//}	
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//template <class Type> 
		//void	CList<Type>::remove(const Type& v)
		//{
		//	CIterator<Type> *lpIter;

		//	if (lpo)
		//	{
		//		lpIter = m_lpBegin;
		//		while (lpIter)
		//		{
		//			if (lpIter->m_Value == v)
		//			{
		//				if (lpIter->m_lpLast)
		//				{
		//					lpIter->m_lpLast->m_lpNext = lpIter->m_lpNext);
		//				}
		//				if (lpIter->m_lpNext)
		//				{
		//					lpIter->m_lpNext->m_lpLast = lpIter->m_lpLast);
		//				}			
		//				if (m_lpEnd == lpIter)
		//				{
		//					m_lpEnd = lpIter->m_lpLast;
		//				}
		//				m_Size--;
		//				return;
		//			}
		//			lpIter = lpIter->m_lpNext;
		//		}
		//	}		
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//template <class Type> 
		//void	CList<Type>::clear()
		//{
		//	m_Size = 0;

		//	CIterator<Type> *lpO = m_lpEnd;

		//	while (lpO)
		//	{
		//		CIterator<Type> *lpTemp = lpO;
		//		lpO = lpTemp->GetLast();
		//		delete lpTemp;
		//	}
		//	m_lpBegin = NULL;
		//	m_lpEnd = NULL;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------	
		//template <class Type> 
		//CIterator<Type> *CList<Type>::get(int i)
		//{
		//	CIterator<Type> *lpO;
		//	int c;

		//	if (i >= m_Size)
		//		return NULL;

		//	lpO = m_lpBegin;
		//	c = 0;
		//	while (lpO)
		//	{
		//		if (c == i)
		//			return lpO;

		//		lpO = lpO->GetNext();			
		//		c++;
		//	}		
		//	return NULL;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------	
		//template <class Type> 
		//Type& CList<Type>::get_value(int i)
		//{
		//	CIterator<Type> *lpO = get(i);
		//	if (lpO)
		//		return lpO->m_Value;		
		//	else
		//		return NULL;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//template <class Type> 
		//void	CList<Type>::insert(int i, const Type& v)
		//{
		//}
	#endif
}

#endif