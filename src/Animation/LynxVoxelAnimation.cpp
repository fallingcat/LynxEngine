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
#include <Animation/LynxVoxelAnimation.h>
#include <Animation/LynxAnimationKeys.h>

namespace LynxEngine 
{	
	namespace Animation
	{
		//IMPLEMENT_CLASSSCRIPT(CVoxelAnimation, CObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelAnimation::CVoxelAnimation(CEngine* lpe)
		: CObj(lpe)
		{			
			//INIT_CLASSSCRIPT
			m_Voxel = 0;
			m_PositionAnimationKeys = NULL;
			m_ColorAnimationKeys = NULL;

			m_Time = m_LastTime = 0.0f;		
			m_StartTime = 0.0f;
			m_EndTime = 0.0f;
			m_Step = 1.0f;
			m_PlayMode = PM_LOOP;
			m_PlayDir = 1.0f;;
			
			IMPLEMENT_PARAM_FLOAT(m_StartTime, 0)
			IMPLEMENT_PARAM_FLOAT(m_EndTime, 0)
			IMPLEMENT_PARAM_FLOAT(m_Step, 0)
			//IMPLEMENT_PARAM_ENUM(m_PlayMode, PLAYMODE, 0);
			IMPLEMENT_PARAM_ENUM(m_PlayMode, 0)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVoxelAnimation::~CVoxelAnimation(void)
		{
			if (m_PositionAnimationKeys)
				LYNXDEL m_PositionAnimationKeys;

			if (m_ColorAnimationKeys)
				LYNXDEL m_ColorAnimationKeys;
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
		void CVoxelAnimation::Create(int voxel, int pos_key, int color_key)
		{
			m_Voxel = voxel;
			if (pos_key > 0)
			{
				m_PositionAnimationKeys = LYNXNEW CAnimationKeys(NULL);
				m_PositionAnimationKeys->CreateKeys(pos_key, 3);
			}

			if (color_key > 0)
			{
				m_ColorAnimationKeys = LYNXNEW CAnimationKeys(NULL);
				m_ColorAnimationKeys->CreateKeys(color_key, 4);
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
		void CVoxelAnimation::vInstance(CVoxelAnimation& rhs)
		{
			CObj::vInstance(rhs);

			if (rhs.m_PositionAnimationKeys)
			{
				m_PositionAnimationKeys = LYNXNEW CAnimationKeys(NULL);
				m_PositionAnimationKeys->vInstance(*(rhs.m_PositionAnimationKeys));
			}

			if (rhs.m_ColorAnimationKeys)
			{
				m_ColorAnimationKeys = LYNXNEW CAnimationKeys(NULL);
				m_ColorAnimationKeys->vInstance(*(rhs.m_ColorAnimationKeys));
			}

			m_Time = rhs.m_Time;
			m_LastTime = rhs.m_LastTime;
			m_M = rhs.m_M;			

			m_StartTime = rhs.m_StartTime;
			m_EndTime = rhs.m_EndTime;			
			m_Step = rhs.m_Step;
			m_PlayMode = rhs.m_PlayMode;
			m_Voxel = rhs.m_Voxel;
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
		void CVoxelAnimation::SetPositionKey(int key, int st, int et, Math::CVector3& pos)
		{
			LYNX_ASSERT(m_PositionAnimationKeys);

			Math::CReal Data[3];
			Data[0] = pos.x;
			Data[1] = pos.y;
			Data[2] = pos.z;

			m_PositionAnimationKeys->m_Keys[key].SetStartTime(st);
			m_PositionAnimationKeys->m_Keys[key].SetEndTime(et);
			m_PositionAnimationKeys->m_Keys[key].SetValues(Data, 3);
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
		void CVoxelAnimation::AddPositionKey(int st, Math::CVector3& pos)
		{
			LYNX_ASSERT(m_PositionAnimationKeys);

			LYNXREAL Data[3];
			Data[0] = pos.x;
			Data[1] = pos.y;
			Data[2] = pos.z;

			m_PositionAnimationKeys->InsertKey(st, Data);
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
		void CVoxelAnimation::SetColorKey(int key, int st, int et, LYNXCOLORRGBA& color)
		{
			LYNX_ASSERT(m_ColorAnimationKeys);

			Math::CReal Data[4];
			Data[0] = color.Red/255.0f;
			Data[1] = color.Green/255.0f;
			Data[2] = color.Blue/255.0f;
			Data[3] = color.Alpha/255.0f;

			m_ColorAnimationKeys->m_Keys[key].SetStartTime(st);
			m_ColorAnimationKeys->m_Keys[key].SetEndTime(et);
			m_ColorAnimationKeys->m_Keys[key].SetValues(Data, 4);
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
		void CVoxelAnimation::AddColorKey(int st, LYNXCOLORRGBA& color)
		{
			LYNX_ASSERT(m_ColorAnimationKeys);

			LYNXREAL Data[4];
			Data[0] = color.Red/255.0f;
			Data[1] = color.Green/255.0f;
			Data[2] = color.Blue/255.0f;
			Data[3] = color.Alpha/255.0f;

			m_ColorAnimationKeys->InsertKey(st, Data);
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
		LYNXBOOL CVoxelAnimation::Play(LYNXREAL step)
		{
			m_bPlayStop = FALSE;	
			if (m_EndTime == m_StartTime)
			{
				m_Time = m_EndTime;
				m_bPlayStop = TRUE;
			}
			else
			{
				m_Time+= m_Step*step*m_PlayDir;
				switch (m_PlayMode)
				{
					case PM_LOOP:			
						while (m_Time >= m_EndTime)
						{
							m_bPlayStop = TRUE;
							m_Time -= (m_EndTime);
							m_Time += (m_StartTime);
						}
						while (m_Time < m_StartTime)
						{
							m_bPlayStop = TRUE;
							m_Time -= (m_StartTime);
							m_Time += m_EndTime;
						}
						break;

					case PM_ONCE:
						if (m_Time >= m_EndTime)
						{
							m_bPlayStop = TRUE;
							m_Time = m_EndTime;
						}
						else if (m_Time < m_StartTime)
						{
							m_bPlayStop = TRUE;
							m_Time = m_StartTime;
						}
						break;

					case PM_BOUNCE:
						if (m_Time >= m_EndTime)
						{
							m_bPlayStop = TRUE;
							m_Time = m_EndTime-m_Step*step;
							m_PlayDir = -1.0f;
						}
						else if (m_Time < m_StartTime)
						{
							m_bPlayStop = TRUE;
							m_Time = m_StartTime;
							m_PlayDir = 1.0f;
						}
						break;
				}
			}
			m_LastTime = m_Time;

			return m_bPlayStop;
		}
	}
}	