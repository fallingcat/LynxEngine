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
#include <LynxMathWrap.h>
#include <Animation/LynxAnimationKeys.h>

namespace LynxEngine 
{	
	namespace Animation
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAnimationKeys::CAnimationKeys(CAnimation* lpani)
		{			
			m_InterpolationType = IT_LINEAR;
			m_KeysType = CAnimation::KT_POSITION;
			m_lpAnimation = lpani;
			m_NumKeyValues = 4;
			m_Keys.clear();			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAnimationKeys::CAnimationKeys(CAnimation* lpani, int num, int values)
		{	
			m_InterpolationType = IT_LINEAR;
			m_KeysType = CAnimation::KT_POSITION;
			m_lpAnimation = lpani;
			CreateKeys(num, values);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CAnimationKeys::~CAnimationKeys(void)
		{
			m_Keys.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationKeys::CreateKeys(int num, int values) 
		{ 
			m_Keys.resize(num); 
			m_NumKeyValues = values; 
			for (int i=0; i<num; i++)	
				m_Keys[i].SetNumValues(m_NumKeyValues); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationKeys::operator =(const CAnimationKeys& rhs)
		{
			CreateKeys((int)rhs.m_Keys.size(), rhs.m_NumKeyValues);
			for (int i=0; i<rhs.m_Keys.size(); i++)
			{
				m_Keys[i] = rhs.m_Keys[i]; 
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAnimationKeys::vSave(CStream& s)
		{
			int NumKeys;
			int StartTime, EndTime, NumValues; 
			float Value;

			NumKeys		= m_Keys.size();
			NumValues	= GetNumKeyValues();
			s.vWrite(&NumKeys, sizeof(int), 1);
			s.vWrite(&NumValues, sizeof(int), 1);

			s.vWrite(&m_InterpolationType, sizeof(INTERPOLATIONTYPE), 1);
			s.vWrite(&m_KeysType, sizeof(CAnimation::KEYSTYPE), 1);			

			for (int k=0; k<NumKeys; k++)
			{
				StartTime	= m_Keys[k].GetStartTime();
				EndTime		= m_Keys[k].GetEndTime();				
				s.vWrite(&StartTime, sizeof(int), 1);
				s.vWrite(&EndTime, sizeof(int), 1);				
				for (int v=0; v<NumValues; v++)
				{
					Value = m_Keys[k].GetValue(v);
					s.vWrite(&Value, sizeof(float), 1);
				}
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAnimationKeys::vLoad(CStream& s, LYNXBOOL skip)
		{
			int NumKeys;
			int StartTime, EndTime, NumValues; 
			float Value[8];
			LYNXLONG SeekLen;

			if (skip)
			{			
				s.vRead(&NumKeys, sizeof(int), 1);
				s.vRead(&NumValues, sizeof(int), 1);

				SeekLen = 0;				
				SeekLen += sizeof(INTERPOLATIONTYPE);
				SeekLen += sizeof(CAnimation::KEYSTYPE);
				for (int k=0; k<NumKeys; k++)
				{
					SeekLen += sizeof(int)*2;
					SeekLen += sizeof(float)*NumValues;
				}
				s.vSeek(SeekLen, CStream::SeekCur);
			}
			else
			{
				s.vRead(&NumKeys, sizeof(int), 1);
				s.vRead(&NumValues, sizeof(int), 1);

				s.vRead(&m_InterpolationType, sizeof(INTERPOLATIONTYPE), 1);
				s.vRead(&m_KeysType, sizeof(CAnimation::KEYSTYPE), 1);

				CreateKeys(NumKeys, NumValues);

				for (int k=0; k<NumKeys; k++)
				{
					s.vRead(&StartTime, sizeof(int), 1);
					s.vRead(&EndTime, sizeof(int), 1);
					m_Keys[k].SetStartTime(StartTime);
					m_Keys[k].SetEndTime(EndTime);

					s.vRead(Value, sizeof(float), NumValues);						
					m_Keys[k].SetValues(Value, NumValues);
				}
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationKeys::vInstance(CAnimationKeys& rhs)
		{
			m_lpAnimation = rhs.m_lpAnimation;
			m_Keys = rhs.m_Keys;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationKeys::InsertBeginKey(int st, int et, LYNXREAL* pv)
		{
			CAnimationKey NewKey(m_NumKeyValues);

			NewKey.SetStartTime(st);						
			NewKey.SetEndTime(et);
			for (int i=0; i<m_NumKeyValues; i++)
			{
				NewKey.SetValue(i, pv[i]);
			}			

			for (int i=0; i<m_Keys.size(); i++)
			{				
				m_Keys[i].SetStartTime(m_Keys[i].GetStartTime()+et);
				m_Keys[i].SetEndTime(m_Keys[i].GetEndTime()+et);					
			}
			m_Keys.insert(0, NewKey);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationKeys::InsertKey(int st, LYNXREAL* pv)
		{
			CAnimationKey NewKey(m_NumKeyValues);

			NewKey.SetStartTime(st);						
			for (int i=0; i<m_NumKeyValues; i++)
			{
				NewKey.SetValue(i, pv[i]);
			}

			for (int i=0; i<m_Keys.size(); i++)
			{
				if (st >= m_Keys[i].GetStartTime() && st < m_Keys[i].GetEndTime())
				{
					NewKey.SetEndTime(m_Keys[i+1].GetStartTime());
					m_Keys.insert(i+1, NewKey);
					m_Keys[i].SetEndTime(st);
					return;
				}
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationKeys::AddDefaultKeys(int st, int et, LYNXREAL* pv, int numkeyvalues)
		{
			CAnimationKey NewKey[2];

			NewKey[0].SetStartTime(st);			
			NewKey[0].SetEndTime(et);			
			NewKey[0].SetNumValues(numkeyvalues);

			NewKey[1].SetStartTime(et);			
			NewKey[1].SetEndTime(et+1);			
			NewKey[1].SetNumValues(numkeyvalues);

			for (int i = 0; i<numkeyvalues; i++)
			{
				NewKey[0].SetValue(i, pv[i]);
				NewKey[1].SetValue(i, pv[i]);
			}

			m_Keys.insert((unsigned int)0, NewKey[0]);
			m_Keys.insert((unsigned int)1, NewKey[1]);					
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationKeys::Lerp(Math::CReal* values, const Math::CReal time)
		{
			Math::CReal				Offset;
			int						i, Key;
			int						Top, Bottom, Mid;
			CAnimationKey			*lpK1, *lpK2;				
			
			if (m_Keys.size() >= 2)
			{
				// Binary search
				Top = 0;
				Key = (int)m_Keys.size();
				Bottom = (int)m_Keys.size() - 1;
				while (Top <= Bottom)
				{
					Mid = ((Top + Bottom) >> 1);					
					if (time > Math::CReal(m_Keys[Mid].m_EndTime))
					{
						Top = Mid + 1;
					}
					else if (time < Math::CReal(m_Keys[Mid].m_StartTime))			
					{
						Bottom = Mid - 1;
					}
					else
					{
						Key = Mid;
						break;
					}
				}			
				if (Key > m_Keys.size()-1)
				{
					m_Keys[m_Keys.size()-1].GetValues(values, m_NumKeyValues);					
					return;
				}
				else if (Key == m_Keys.size() - 1)
				{		
					lpK1 = &m_Keys[Key-1];
					lpK2 = &m_Keys[Key];
					Offset = (time - Math::CReal(lpK2->m_StartTime)) / (Math::CReal(lpK2->m_EndTime - lpK2->m_StartTime));
					for (i = 0; i<m_NumKeyValues; i++)
					{
						values[i] = lpK1->m_Values[i] + (Offset * (lpK2->m_Values[i] - lpK1->m_Values[i]));
					}
					return;
				}
				else
				{
					lpK1 = &m_Keys[Key];
					lpK2 = &m_Keys[Key+1];
					Offset = (time -  Math::CReal(lpK1->m_StartTime)) / (Math::CReal(lpK1->m_EndTime - lpK1->m_StartTime));		
					for (i=0; i<m_NumKeyValues; i++) 
					{		
						values[i] = lpK1->m_Values[i] + (Offset * (lpK2->m_Values[i] - lpK1->m_Values[i]));					
					}
					return;
				}
			}
			else
			{
				m_Keys[0].GetValues(values, m_NumKeyValues);					
				//lynxMemCpy(values, m_Keys[0].m_Values.getData(), sizeof(LYNXREAL)*m_Keys[0].m_Values.size());									
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CAnimationKeys::ClearKeys()
		{
			m_Keys.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		/*
		void CAnimationKeys::Lerp(LYNXREAL* values, const Math::CReal time)
		{
			static CReal Values[16];
			Lerp(Values, time);

			for (int i=0; i<m_NumKeyValues; i++)
			{
				values[i] = Values[i];
			}
		}
		*/
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void CAnimationKeys::Spline(LYNXREAL* values, LYNXREAL time)
		//{
		//	LYNXREAL				Offset;
		//	int						i, Key;
		//	int						Top, Bottom, Mid;
		//	CAnimationKey			*lpK0, *lpK1, *lpK2, *lpK3;				
		//	
		//	if (m_Keys.size() >= 2)
		//	{
		//		//Key = lynxBinarySearch(&time, lpanikey->lpKeys, lpanikey->NumKeys, sizeof(LYNXFLOATKEY), ilynxKeyTimeCompare);		
		//		Top = 0;
		//		Key = (int)m_Keys.size();
		//		Bottom = (int)m_Keys.size();// - 2;
		//		while (Top <= Bottom)
		//		{
		//			Mid = ((Top + Bottom) >> 1);					
		//			if (time > LYNX_REAL(m_Keys[Mid].m_EndTime))
		//			{
		//				Top = Mid + 1;
		//			}
		//			else if (time < LYNX_REAL(m_Keys[Mid].m_StartTime))			
		//			{
		//				Bottom = Mid - 1;
		//			}
		//			else
		//			{
		//				Key = Mid;
		//				break;
		//			}
		//		}
		//		if (Key-1 < 0)
		//			lpK0 = &m_Keys[0];
		//		else
		//			lpK0 = &m_Keys[Key-1];
		//		lpK1 = &m_Keys[Key];
		//		lpK2 = &m_Keys[Key+1];
		//		if (Key+2 > m_Keys.size()-1)
		//			lpK3 = &m_Keys[m_Keys.size()-1];
		//		else
		//			lpK3 = &m_Keys[Key+2];
		//		if (Key > m_Keys.size()-1)
		//		{
		//			//lynxMemCpy(values, m_Keys[Key].m_Values.getData(), sizeof(LYNXREAL)*m_Keys[Key].m_Values.size());					
		//			m_Keys[Key].GetValues(values, m_NumKeyValues);					
		//			return;
		//		}
		//		else
		//		{
		//			Offset = LYNX_REAL_DIV( (LYNX_REAL_SUB(time, LYNX_REAL(lpK1->m_StartTime))), LYNX_REAL((lpK1->m_EndTime - lpK1->m_StartTime)));		
		//			for (i=0; i<m_NumKeyValues; i++) 
		//			{		
		//				values[i] = LYNX_REAL_ADD((lpK1->m_Values[i]), LYNX_REAL_MUL(LYNX_REAL_SUB((lpK2->m_Values[i]), (lpK1->m_Values[i])), Offset));					
		//			}
		//			return;
		//		}
		//	}
		//	else
		//	{
		//		m_Keys[0].GetValues(values, m_NumKeyValues);					
		//		//lynxMemCpy(values, m_Keys[0].m_Values.getData(), sizeof(LYNXREAL)*m_Keys[0].m_Values.size());									
		//	}			
		//}
	}
}	