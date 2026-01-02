#include <LynxEngine_PCH.h>

#include <GUISystem/LynxScrollGroup.h>

#define SCROLL_GROUP_BORDER_X			10
#define SCROLL_GROUP_BORDER_Y			10

namespace LynxEngine
{
	namespace GUISystem
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScrollGroup::CScrollGroup(CEngine* lpe)
		: CObj(lpe)
		{            
			m_Context = NULL;
            m_BouncingSpeed = 2.0f;
            m_BouncingType = 0x00;
            m_Offset.x = 0;
            m_Offset.y = 0;
            m_PanVelocity = Math::CVector2(0.0f, 0.0f);
            m_bBouncingX = LYNX_TRUE;
            m_bBouncingY = LYNX_TRUE;
			m_bShow = LYNX_TRUE;
			m_PanVelocityScale = 1.0f;
			m_ScrollFlag = X_SCROLL | Y_SCROLL;
			m_SnapSize = CVector2(-1.0f, -1.0f);
            m_bClip = LYNX_TRUE;
			m_bActive = LYNX_TRUE;
			m_RenderOrder = 0;
			m_bAutoMoving = LYNX_FALSE;
			m_bSnappingStop = LYNX_FALSE;

			m_BoundRatioLeft = 0.4f;
			m_BoundRatioRight = 0.4f;
			m_BoundRatioTop = 0.4f;
			m_BoundRatioDown = 0.4f;

			m_BottomExtentionHight = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CScrollGroup::~CScrollGroup()
		{
			m_ControlList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CScrollGroup::vInit()
		{
            m_BouncingType = 0x00;
			m_bAutoMoving = LYNX_FALSE;
			m_bSnappingStop = LYNX_FALSE;
            
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::ResetOffset()
		{
			m_Offset.x = m_Offset.y = 0.0f;
		}


		//-------------------------------------------------------------------------------------------------------
		//
		//  :   設定 SetTransition 時的邊界比率.
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::SetTransitionBoundRatio(float left, float right, float top, float down)
		{

			m_BoundRatioLeft = left;
			m_BoundRatioRight = right;
			m_BoundRatioTop = top;
			m_BoundRatioDown = down;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
        int CScrollGroup::GetCurrentPage()
		{
			if (m_SnapSize.x > 0)
			{
				int Page = LYNX_ROUND((-m_Offset.x)/m_SnapSize.x);
				if (Page < 0)
					Page = 0;
				return Page;
			}
			else if (m_SnapSize.y > 0)
			{
				int Page = LYNX_ROUND((-m_Offset.y)/m_SnapSize.y);
				if (Page < 0)
					Page = 0;
				return Page;
			}
			else
			{
				return 0;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
        void CScrollGroup::SetCurrentPage(int p)
		{
			CVector2 SnapSize;

			SnapSize = m_SnapSize;
			if (SnapSize.x < 0)
				SnapSize.x = 0.0f;
			if (SnapSize.y < 0)
				SnapSize.y = 0.0f;

			SetOffset(CVector2(-1.0f*p*SnapSize.x, -1.0f*p*SnapSize.y));	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::GoNextPage(const float dir)
		{
			static CVector2 TotalOffset;

			if (!m_bAutoMoving)
			{
				if (dir >= 0)
				{
					m_bAutoMoving = LYNX_TRUE;
					m_AutoMovingTargetPage = GetCurrentPage() - 1;
					m_AutoMovingDir = dir;
					TotalOffset.x = 0.0f;
					TotalOffset.y = 0.0f;
				}
				else
				{
					m_bAutoMoving = LYNX_TRUE;
					m_AutoMovingTargetPage = GetCurrentPage() + 1;
					m_AutoMovingDir = dir;
					TotalOffset.x = 0.0f;
					TotalOffset.y = 0.0f;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::GoOffset(const CVector2& offset)
		{
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
        LYNXBOOL CScrollGroup::IsHit(Math::CVector2& pos)
        {
            LYNXRECT PRect, Rect;
            
			Rect = m_Rect;
			CVector2 Origin = GUISystem::CContext::GetAbsoluteCoordOrigin(m_Context);
			Rect.x1 += Origin.x;
			Rect.x2 += Origin.x;
			Rect.y1 += Origin.y;
			Rect.y2 += Origin.y;
            
            if (pos.x >= Rect.x1 && pos.x <= Rect.x2 && pos.y >= Rect.y1 && pos.y <= Rect.y2)
            {
                return LYNX_TRUE;
            }
            return LYNX_FALSE;
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		/*
		void CScrollGroup::vLoop(const float step)
		{	
			if (!(m_ScrollFlag & X_SCROLL))
				m_PanVelocity.x = 0;
			if (!(m_ScrollFlag & Y_SCROLL))
				m_PanVelocity.y = 0;
									
			if (m_bShow)
			{
				m_Offset += m_PanVelocity*LYNX_CEIL(step-0.09f)/60.0f;

				m_PanVelocity *= 0.9f;
				if (m_PanVelocity.Length(2) <= 5.0f)
					m_PanVelocity = Math::CVector2(0.0f, 0.0f);
	        
				Math::CVector2 DisplaySize;
				DisplaySize.x = m_Rect.x2 - m_Rect.x1;
				DisplaySize.y = m_Rect.y2 - m_Rect.y1;
            
				CVector2 SnapBase(0.0f, 0.0f);
				CVector2 SnapBoundary(m_Size.x, m_Size.y);
			
				if ((m_ScrollFlag & X_SCROLL))
				{
					SnapBase.x = -640;
					SnapBoundary.x = 640*2;
				}

				if (DisplaySize.x >= m_Size.x)
					m_bBouncingX = LYNX_FALSE;
            
				if (DisplaySize.y >= m_Size.y)
					m_bBouncingY = LYNX_FALSE;
            
				if (m_Offset.x > SnapBase.x)
				{
					if (m_bBouncingX)// && !(m_BouncingType & BOUNCE_BACK_TO_RIGHT))
					{
						float BounceLen = m_Offset.x;
						m_PanVelocity.x -= BounceLen * m_BouncingSpeed;
						m_BouncingType &= 0xC;
						m_BouncingType |= BOUNCE_BACK_TO_LEFT;
					}
					else
					{
						m_Offset.x = SnapBase.x;
						m_PanVelocity.x = 0.0f;
						m_BouncingType &= 0xC;
					}
				}
				else if (m_Offset.x < SnapBase.x)
				{
					if (m_bBouncingX)// && !(m_BouncingType & BOUNCE_BACK_TO_LEFT))
					{
						float BounceLen = (m_Offset.x + SnapBoundary.x) - DisplaySize.x;
						if (BounceLen < 0)
						{
							m_PanVelocity.x -= BounceLen * m_BouncingSpeed;
							m_BouncingType &= 0xC;
							m_BouncingType |= BOUNCE_BACK_TO_RIGHT;
						}
					}
					else
					{
						if (m_Offset.x < (DisplaySize.x - SnapBoundary.x))
						{
							m_Offset.x = (DisplaySize.x - SnapBoundary.x);
							m_PanVelocity.x = 0.0f;
							m_BouncingType &= 0xC;
						}
					}
				}
            
				if (m_Offset.y > SnapBase.y)
				{
					if (m_bBouncingY && !(m_BouncingType & BOUNCE_BACK_TO_BOTTOM))
					{
						float BounceLen = m_Offset.y;
						m_PanVelocity.y -= BounceLen * m_BouncingSpeed;
						m_BouncingType &= 0x3;
						m_BouncingType |= BOUNCE_BACK_TO_TOP;
					}
					else
					{
						m_Offset.y = SnapBase.y;
						m_PanVelocity.y = 0.0f;
						m_BouncingType &= 0x3;
					}
				}
				else if (m_Offset.y < SnapBase.y)
				{
					if (m_bBouncingY && !(m_BouncingType & BOUNCE_BACK_TO_TOP))
					{
						float BounceLen = (m_Offset.y + SnapBoundary.y) - DisplaySize.y;
						if (BounceLen < 0)
						{
							m_PanVelocity.y -= BounceLen * m_BouncingSpeed;
							m_BouncingType &= 0x3;
							m_BouncingType |= BOUNCE_BACK_TO_BOTTOM;
						}
					}
					else
					{
						if (m_Offset.y < (DisplaySize.y - SnapBoundary.y))
						{
							m_Offset.y = (DisplaySize.y - SnapBoundary.y);
							m_PanVelocity.y = 0.0f;
							m_BouncingType &= 0x3;
						}
					}
				}
            
				
				if ((m_BouncingType & BOUNCE_BACK_TO_LEFT) && m_Offset.x < SnapBase.x)
				{
					m_Offset.x = SnapBase.x;
					m_PanVelocity.x = 0.0f;
					m_BouncingType &= 0xC;
				}
				else if ((m_BouncingType & BOUNCE_BACK_TO_RIGHT) && m_Offset.x > (DisplaySize.x - SnapBoundary.x))
				{
					m_Offset.x = (DisplaySize.x - SnapBoundary.x);
					m_PanVelocity.x = 0.0f;
					m_BouncingType &= 0xC;
				}
            
				if ((m_BouncingType & BOUNCE_BACK_TO_TOP) && m_Offset.y < SnapBase.y)
				{
					m_Offset.y = SnapBase.y;
					m_PanVelocity.y = 0.0f;
					m_BouncingType &= 0x3;
				}
				else if ((m_BouncingType & BOUNCE_BACK_TO_BOTTOM) && m_Offset.y > (DisplaySize.y - SnapBoundary.y))
				{
					m_Offset.y = (DisplaySize.y - SnapBoundary.y);
					m_PanVelocity.y = 0.0f;
					m_BouncingType &= 0x3;
				}
			}
        }
		*/

		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		/*
		void CScrollGroup::vLoop(const float step)
		{	
			if (!(m_ScrollFlag & X_SCROLL))
			{
				m_PanVelocity.x = 0;
			}
			if (!(m_ScrollFlag & Y_SCROLL))
			{
				m_PanVelocity.y = 0;
			}
									
			if (m_bShow && (IsTransitionDone() || IsStop()))
			{
				m_Offset += m_PanVelocity*LYNX_CEIL(step-0.09f)/60.0f;

				if (m_Offset.x > (m_Rect.x2 - m_Rect.x1)*0.5f)
					m_Offset.x = (m_Rect.x2 - m_Rect.x1)*0.5f;
				else if (m_Offset.x < ((m_Rect.x2 - m_Rect.x1)*-0.5f-m_Size.x))
					m_Offset.x = ((m_Rect.x2 - m_Rect.x1)*-0.5f-m_Size.x);

				if (m_Offset.y > (m_Rect.y2 - m_Rect.y1)*0.5f)
					m_Offset.y = (m_Rect.y2 - m_Rect.y1)*0.5f;
				else if (m_Offset.y < ((m_Rect.y2 - m_Rect.y1)*-0.5f-m_Size.y))
					m_Offset.y = ((m_Rect.y2 - m_Rect.y1)*-0.5f-m_Size.y);

				m_PanVelocity *= 0.9f;
				if (m_PanVelocity.Length(2) <= 5.0f)
					m_PanVelocity = Math::CVector2(0.0f, 0.0f);
	        
				LYNXRECT DisplayRect;
				DisplayRect.x1 = 0;
				DisplayRect.y1 = 0;
				DisplayRect.x2 = m_Rect.x2 - m_Rect.x1;
				DisplayRect.y2 = m_Rect.y2 - m_Rect.y1;
				
				m_ViewableRect.x1 = 0;
				m_ViewableRect.y1 = 0;
				m_ViewableRect.x2 = m_ViewableRect.x1 + m_Size.x;
				m_ViewableRect.y2 = m_ViewableRect.y1 + m_Size.y;						
				
				CVector2 Offset = m_Offset;
				if (m_SnapSize.x > 0 && (m_ScrollFlag & X_SCROLL))
				{
					m_ViewableRect.x1 = 0;
					m_ViewableRect.x2 = m_ViewableRect.x1 + DisplayRect.x2;					

					while (Offset.x < -m_SnapSize.x)
					{
						m_ViewableRect.x1 += m_SnapSize.x;
						m_ViewableRect.x2 += m_SnapSize.x;
						Offset.x += m_SnapSize.x;
					}
					if (Offset.x < -(m_SnapSize.x*0.5f))
					{
						if (m_ViewableRect.x2 <= m_Size.x)
						{
							m_ViewableRect.x1 += m_SnapSize.x;
							m_ViewableRect.x2 += m_SnapSize.x;
						}
					}					
				}
				if (m_SnapSize.y > 0 && (m_ScrollFlag & Y_SCROLL))
				{
					m_ViewableRect.y1 = 0;
					m_ViewableRect.y2 = m_ViewableRect.y1 + DisplayRect.y2;					

					while (Offset.y < -m_SnapSize.y)
					{
						m_ViewableRect.y1 += m_SnapSize.y;
						m_ViewableRect.y2 += m_SnapSize.y;
						Offset.y += m_SnapSize.y;
					}
					if (Offset.y < -(m_SnapSize.y*0.5f))
					{
						if (m_ViewableRect.y2 <= m_Size.y)
						{
							m_ViewableRect.y1 += m_SnapSize.y;
							m_ViewableRect.y2 += m_SnapSize.y;
						}
					}					
				}
								
				if (DisplayRect.x2 >= m_Size.x || !(m_ScrollFlag & X_SCROLL))
					m_bBouncingX = LYNX_FALSE;
            
				if (DisplayRect.y2 >= m_Size.y || !(m_ScrollFlag & Y_SCROLL))
					m_bBouncingY = LYNX_FALSE;
            
				if ((m_Offset.x + m_ViewableRect.x1) > DisplayRect.x1 && (m_ScrollFlag & X_SCROLL))
				{
					if (m_bBouncingX)// && !(m_BouncingType & BOUNCE_BACK_TO_RIGHT))
					{
						float BounceLen = ((m_Offset.x + m_ViewableRect.x1) - DisplayRect.x1);
						m_PanVelocity.x -= (BounceLen * m_BouncingSpeed);
						m_BouncingType &= 0xC;
						m_BouncingType |= BOUNCE_BACK_TO_LEFT;
					}
					else
					{
						m_Offset.x = (DisplayRect.x1 - m_ViewableRect.x1);
						m_PanVelocity.x = 0.0f;
						m_BouncingType &= 0xC;
					}
				}
				else if ((m_Offset.x + m_ViewableRect.x2) < DisplayRect.x2 && (m_ScrollFlag & X_SCROLL))
				{
					if (m_bBouncingX)// && !(m_BouncingType & BOUNCE_BACK_TO_LEFT))
					{
						float BounceLen = DisplayRect.x2 - (m_Offset.x + m_ViewableRect.x2);
						m_PanVelocity.x += BounceLen * m_BouncingSpeed;
						m_BouncingType &= 0xC;
						m_BouncingType |= BOUNCE_BACK_TO_RIGHT;						
					}
					else
					{
						m_Offset.x = (DisplayRect.x2 - m_ViewableRect.x2);
						m_PanVelocity.x = 0.0f;
						m_BouncingType &= 0xC;						
					}
				}


				if ((m_Offset.y + m_ViewableRect.y1) > DisplayRect.y1 && (m_ScrollFlag & Y_SCROLL))
				{
					if (m_bBouncingY)// && !(m_BouncingType & BOUNCE_BACK_TO_RIGHT))
					{
						float BounceLen = ((m_Offset.y + m_ViewableRect.y1) - DisplayRect.y1);
						m_PanVelocity.y -= (BounceLen * m_BouncingSpeed);
						m_BouncingType &= 0x3;
						m_BouncingType |= BOUNCE_BACK_TO_TOP;
					}
					else
					{
						m_Offset.x = (DisplayRect.y1 - m_ViewableRect.y1);
						m_PanVelocity.y = 0.0f;
						m_BouncingType &= 0x3;
					}
				}
				else if ((m_Offset.y + m_ViewableRect.y2) < DisplayRect.y2 && (m_ScrollFlag & Y_SCROLL))
				{
					if (m_bBouncingY)// && !(m_BouncingType & BOUNCE_BACK_TO_LEFT))
					{
						float BounceLen = DisplayRect.y2 - (m_Offset.y + m_ViewableRect.y2);
						m_PanVelocity.y += BounceLen * m_BouncingSpeed;
						m_BouncingType &= 0x3;
						m_BouncingType |= BOUNCE_BACK_TO_BOTTOM;						
					}
					else
					{
						m_Offset.y = (DisplayRect.y2 - m_ViewableRect.y2);
						m_PanVelocity.x = 0.0f;
						m_BouncingType &= 0x3;						
					}
				}
				
				if ((m_BouncingType & BOUNCE_BACK_TO_LEFT) && (m_Offset.x + m_ViewableRect.x1) <= DisplayRect.x1 && (m_ScrollFlag & X_SCROLL))
				{
					m_Offset.x = (DisplayRect.x1 - m_ViewableRect.x1);
					m_PanVelocity.x = 0.0f;
					m_BouncingType &= 0xC;
				}
				else if ((m_BouncingType & BOUNCE_BACK_TO_RIGHT) && (m_Offset.x + m_ViewableRect.x2) >= DisplayRect.x2 && (m_ScrollFlag & X_SCROLL))
				{
					m_Offset.x = (DisplayRect.x2 - m_ViewableRect.x2);
					m_PanVelocity.x = 0.0f;
					m_BouncingType &= 0xC;
				}
            
				if ((m_BouncingType & BOUNCE_BACK_TO_TOP) && (m_Offset.y + m_ViewableRect.y1) <= DisplayRect.y1 && (m_ScrollFlag & Y_SCROLL))
				{
					m_Offset.x = (DisplayRect.x1 - m_ViewableRect.x1);
					m_PanVelocity.y = 0.0f;
					m_BouncingType &= 0x3;
				}
				else if ((m_BouncingType & BOUNCE_BACK_TO_BOTTOM) && (m_Offset.y + m_ViewableRect.y2) >= DisplayRect.y2 && (m_ScrollFlag & Y_SCROLL))
				{
					m_Offset.y = (DisplayRect.y2 - m_ViewableRect.y2);
					m_PanVelocity.y = 0.0f;
					m_BouncingType &= 0x3;
				}
			}
        }
		*/
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CScrollGroup::vLoop(const float step)
		{	
			if (m_bAutoMoving)
			{
				if (m_SnapSize.x > 0)
				{
					m_Offset.x += m_AutoMovingDir * (m_SnapSize.x/25.0f);
				}
				else if (m_SnapSize.y > 0)
				{
					m_Offset.y += m_AutoMovingDir * (m_SnapSize.x/25.0f);
				}

				if (GetCurrentPage() == m_AutoMovingTargetPage)
				{
					int Mod = int(-m_Offset.x)%(int)m_SnapSize.x;
					if (abs(Mod) <= (m_SnapSize.x/25.0f))
					{
						m_bAutoMoving = LYNX_FALSE;
						SetCurrentPage(m_AutoMovingTargetPage);
						m_PanVelocity.x = 0;
						m_PanVelocity.y = 0;
					}
				}
			}
			else
			{
				m_bSnappingStop = LYNX_FALSE;

				InputSystem::CTouch* lpTouch = m_lpEngine->GetlpSystemTouch();					

				if (!(m_ScrollFlag & X_SCROLL))
				{
					m_PanVelocity.x = 0;
				}
				if (!(m_ScrollFlag & Y_SCROLL))
				{
					m_PanVelocity.y = 0;
				}
									
				if (m_bShow && (IsTransitionDone() || IsStop()))
				{
					//m_Offset += m_PanVelocity*LYNX_CEIL(step-0.09f)/60.0f;

					if (m_Offset.x > (m_Rect.x2 - m_Rect.x1)*m_BoundRatioLeft)
					{
						m_Offset.x = (m_Rect.x2 - m_Rect.x1)*m_BoundRatioLeft;
						m_PanVelocity.x = 0.0f;
					}
					else if (m_Offset.x < ((m_Rect.x2 - m_Rect.x1)*-m_BoundRatioRight - m_Size.x))
					{
						m_PanVelocity.x = 0.0f;
						m_Offset.x = ((m_Rect.x2 - m_Rect.x1)*-m_BoundRatioRight - m_Size.x);
					}

					if (m_Offset.y >(m_Rect.y2 - m_Rect.y1)*m_BoundRatioTop)
					{
						m_Offset.y = (m_Rect.y2 - m_Rect.y1)*m_BoundRatioTop;
						m_PanVelocity.y = 0.0f;
					}
					else if (m_Offset.y < ((m_Rect.y2 - m_Rect.y1)*-m_BoundRatioDown - (m_Size.y + m_BottomExtentionHight)))
					{
						m_Offset.y = ((m_Rect.y2 - m_Rect.y1)*-m_BoundRatioDown - (m_Size.y + m_BottomExtentionHight));
						m_PanVelocity.y = 0.0f;
					}
					m_Offset += (m_PanVelocity*step)/60.0f;
					m_PanVelocity *= 0.9f;
					if (m_PanVelocity.Length(2) <= 5.0f)
						m_PanVelocity = Math::CVector2(0.0f, 0.0f);
	        
					LYNXRECT DisplayRect;
					DisplayRect.x1 = 0;
					DisplayRect.y1 = 0;
					DisplayRect.x2 = m_Rect.x2 - m_Rect.x1;
					DisplayRect.y2 = m_Rect.y2 - m_Rect.y1;
				
					m_ViewableRect.x1 = 0;
					m_ViewableRect.y1 = 0;
					m_ViewableRect.x2 = m_ViewableRect.x1 + m_Size.x;
					m_ViewableRect.y2 = m_ViewableRect.y1 + (m_Size.y + m_BottomExtentionHight);
				
					CVector2 Offset = m_Offset;				

					if (m_ScrollFlag & X_SCROLL)
					{
						if (m_SnapSize.x > 0)
						{
							if (fabs(m_PanVelocity.x) < 210.0f && (!lpTouch || (lpTouch && lpTouch->GetNumTouches() == 0)))
							{
								/*
								int SnapOffset = 0;
								while (SnapOffset > -640*5)
								{
									if (fabs(m_Offset.x - SnapOffset) < 90.0f)
									{
										m_Offset.x = SnapOffset;
										break;
									}
									SnapOffset -= m_SnapSize.x;
								}
								*/

								int CurrentSnap = int(-m_Offset.x)/(int)m_SnapSize.x;
								int NextSnap = CurrentSnap + 1;
								int Mod = int(-m_Offset.x)%(int)m_SnapSize.x;
								int TargetSnap;
								if (Mod != 0)
								{
									if (Mod < (m_SnapSize.x*0.5f))
										TargetSnap = CurrentSnap;
									else
										TargetSnap = NextSnap;

									float TargetOffset = -TargetSnap * m_SnapSize.x;									

									if (m_Offset.x > TargetOffset)
									{
										m_Offset.x -= 30*step;
										if (m_Offset.x <= TargetOffset)
										{
											m_Offset.x = TargetOffset;
											m_PanVelocity.x = 0.0f;
											m_bSnappingStop = LYNX_TRUE;
										}
									}
									else if (m_Offset.x < TargetOffset)
									{
										m_Offset.x += 30*step;
										if (m_Offset.x >= TargetOffset)
										{
											m_Offset.x = TargetOffset;
											m_PanVelocity.x = 0.0f;
											m_bSnappingStop = LYNX_TRUE;
										}
									}
								}							
							}
						}
					}

					if (m_ScrollFlag & Y_SCROLL)
					{
						if (m_SnapSize.y > 0)
						{
							if (fabs(m_PanVelocity.y) < 210.0f && (!lpTouch || (lpTouch && lpTouch->GetNumTouches() == 0)))
							{
								int CurrentSnap = int(-m_Offset.y)/(int)m_SnapSize.y;
								int NextSnap = CurrentSnap + 1;
								int Mod = int(-m_Offset.y)%(int)m_SnapSize.y;
								int TargetSnap;
								if (Mod != 0)
								{
									if (Mod < (m_SnapSize.y*0.5f))
										TargetSnap = CurrentSnap;
									else
										TargetSnap = NextSnap;

									float TargetOffset = -TargetSnap * m_SnapSize.x;

									if (m_Offset.y > TargetOffset)
									{
										m_Offset.y -= 35*step;
										if (m_Offset.y <= TargetOffset)
										{
											m_Offset.y = TargetOffset;
											m_PanVelocity.y = 0.0f;
											m_bSnappingStop = LYNX_TRUE;
										}
									}
									else if (m_Offset.y < TargetOffset)
									{
										m_Offset.y += 35*step;
										if (m_Offset.y >= TargetOffset)
										{
											m_Offset.y = TargetOffset;
											m_PanVelocity.y = 0.0f;
											m_bSnappingStop = LYNX_TRUE;
										}
									}
								}
							}
						}
						else
						{
							if (m_Offset.y > 0)
							{
								float UpForce = 0.0f;

								if (0)//m_bBouncingY)
								{
									if (m_Offset.y > 10.0f)
									{
										UpForce = m_Offset.y * 30.0f * step;								
									}
									m_PanVelocity.y -= UpForce*step; 							
							
									/*
									if (UpForce > 600.0f)
									{
										m_PanVelocity.y = 0.0f;
									}
									*/

									if (m_PanVelocity.y <= 0.0f)
									{
										m_PanVelocity.y = -900.0f;
										if (m_Offset.y < 0.0f)
										{
											m_PanVelocity.y = 0.0f;
											m_Offset.y = 0.0f;
										}
									}
								}
								else
								{
									m_PanVelocity.y = 0.0f;
									m_Offset.y = 0.0f;
								}
							}
							else if (m_Offset.y < (DisplayRect.y2 - (m_ViewableRect.y2 + SCROLL_GROUP_BORDER_Y)))
							{
								float UpForce = 0.0f;

								if (0)//m_bBouncingY)
								{
									if (m_Offset.y > 10.0f)
									{
										UpForce = m_Offset.y * 30.0f * step;								
									}
									m_PanVelocity.y -= UpForce*step; 							
							
									/*
									if (UpForce > 600.0f)
									{
										m_PanVelocity.y = 0.0f;
									}
									*/

									if (m_PanVelocity.y <= 0.0f)
									{
										m_PanVelocity.y = -900.0f;
										if (m_Offset.y < 0.0f)
										{
											m_PanVelocity.y = 0.0f;
											m_Offset.y = 0.0f;
										}
									}
								}
								else
								{
									m_PanVelocity.y = 0.0f;
									m_Offset.y = (DisplayRect.y2 - (m_ViewableRect.y2 + SCROLL_GROUP_BORDER_Y));
								}
							}
						}
					}


					/*
					if ((m_Offset.x + m_ViewableRect.x1) > DisplayRect.x1 && (m_ScrollFlag & X_SCROLL))
					{
						m_Offset.x = (DisplayRect.x1 - m_ViewableRect.x1);
						m_PanVelocity.x = 0.0f;
						m_BouncingType &= 0xC;					
					}
					else if ((m_Offset.x + m_ViewableRect.x2) < DisplayRect.x2 && (m_ScrollFlag & X_SCROLL))
					{
						m_Offset.x = (DisplayRect.x2 - m_ViewableRect.x2);
						m_PanVelocity.x = 0.0f;
						m_BouncingType &= 0xC;					
					
					}

					if ((m_Offset.y + m_ViewableRect.y1) > DisplayRect.y1 && (m_ScrollFlag & Y_SCROLL))
					{
						m_Offset.x = (DisplayRect.y1 - m_ViewableRect.y1);
						m_PanVelocity.y = 0.0f;
						m_BouncingType &= 0x3;					
					}
					else if ((m_Offset.y + m_ViewableRect.y2) < DisplayRect.y2 && (m_ScrollFlag & Y_SCROLL))
					{
						m_Offset.y = (DisplayRect.y2 - m_ViewableRect.y2);
						m_PanVelocity.x = 0.0f;
						m_BouncingType &= 0x3;											
					}
					*/
				}
			}
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  a°c˙:
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::SortControls()
		{
			//LynxEngine::sort<CList<CControl*>::CIterator, CMPCONTROLFUNC>(m_ControlList.begin(), m_ControlList.end(), CmpRenderOrder);
			m_ControlList.sort<CMPCONTROLFUNC>(CmpRenderOrder);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::vRender()
		{	
			LYNXRECT OldRect, NewRect, OldClipRect;
			LYNXBOOL bScissor;
			
			SortControls();

			if (m_bShow)
			{
				CVector2 Origin = GUISystem::CContext::GetAbsoluteCoordOrigin(m_Context);

				m_lpEngine->GetlpGraphicsSystem()->GetScissorRect(&OldClipRect);
				bScissor = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->IsScissorRectEnabled();

				LYNXRECT ClipRect = GetRect();
					
				ClipRect.x1 += Origin.x;
				ClipRect.x2 += Origin.x;
				ClipRect.y1 += Origin.y;
				ClipRect.y2 += Origin.y;
               
                 //m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect(m_bClip);
				//m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&ClipRect);
                
				for (CList<CControl*>::CIterator Control = GetControlList().begin(); Control != GetControlList().end(); ++Control)
				{
					if ((*Control) != m_Context->GetActiveControl())
					{
						// Reposition controls according to the position of CWindow
						(*Control)->vGetAlignmentRect(OldRect);
						NewRect = OldRect;

						NewRect.x1 += Origin.x;
						NewRect.x2 += Origin.x;
						NewRect.y1 += Origin.y;
						NewRect.y2 += Origin.y;

						if ((*Control)->GetScrollGroup())
						{
							NewRect.x1 += (*Control)->GetScrollGroup()->GetOffset().x;
							NewRect.x2 += (*Control)->GetScrollGroup()->GetOffset().x;
							NewRect.y1 += (*Control)->GetScrollGroup()->GetOffset().y;
							NewRect.y2 += (*Control)->GetScrollGroup()->GetOffset().y;
						}
						(*Control)->vSetAlignmentRect(NewRect);
						(*Control)->vRender();
						(*Control)->vSetAlignmentRect(OldRect);					
					}						
				}
			
				if (m_Context->GetActiveControl())
				{				
                    //m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect(m_bClip);
					//LYNXRECT ClipRect = m_Context->GetActiveControl()->GetScrollGroup()->GetRect();
					ClipRect.x1 += m_Rect.x1;
					ClipRect.x2 += m_Rect.x1;
					ClipRect.y1 += m_Rect.y1;
					ClipRect.y2 += m_Rect.y1;
                    //m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&ClipRect);

					// Reposition controls according to the position of CWindow
					m_Context->GetActiveControl()->vGetAlignmentRect(OldRect);
					NewRect = OldRect;

					NewRect.x1 += Origin.x;
					NewRect.x2 += Origin.x;
					NewRect.y1 += Origin.y;
					NewRect.y2 += Origin.y;

					NewRect.x1 += m_Context->GetActiveControl()->GetScrollGroup()->GetOffset().x;
					NewRect.x2 += m_Context->GetActiveControl()->GetScrollGroup()->GetOffset().x;
					NewRect.y1 += m_Context->GetActiveControl()->GetScrollGroup()->GetOffset().y;
					NewRect.y2 += m_Context->GetActiveControl()->GetScrollGroup()->GetOffset().y;						
					m_Context->GetActiveControl()->vSetAlignmentRect(NewRect);
					m_Context->GetActiveControl()->vRender();
					m_Context->GetActiveControl()->vSetAlignmentRect(OldRect);					
				}
				m_lpEngine->GetlpGraphicsSystem()->SetScissorRect(&OldClipRect);
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->EnableScissorRect(bScissor);
			}
		}  
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CScrollGroup::vCreate()
		{	
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CScrollGroup::Create(const LYNXRECT& rect)
		{
			m_Rect = rect;

			return LYNX_TRUE;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::AddControl(CControl* con, LYNXBOOL bcompute_size)
		{
			m_ControlList.push_back(con);
			con->SetScrollGroup(this);

			if (bcompute_size)
			{
				ComputeSizeWithControlList();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   計算含 ControlList 的 大小.
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::ComputeSizeWithControlList()
		{
			LYNXRECT Rect;
			CVector2 MaxSize, Offset;

			MaxSize.x = 0;
			MaxSize.y = 0;
			CList<CControl*>::CIterator Control;
			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				(*Control)->vGetAlignmentRect(Rect);
				Offset = (*Control)->GetTransitionOffset();
				Rect.x1 -= Offset.x;
				Rect.x2 -= Offset.x;
				Rect.y1 -= Offset.y;
				Rect.y2 -= Offset.y;

				if (Rect.x2 > MaxSize.x)
				{
					MaxSize.x = Rect.x2;
				}
				if (Rect.y2 > MaxSize.y)
				{
					MaxSize.y = Rect.y2;
				}
			}

			CVector2 Size = m_Size;

			if (m_ScrollFlag & X_SCROLL)
			{
				Size.x = (MaxSize.x - m_Rect.x1);
				if (Size.x < (m_Rect.x2 - m_Rect.x1))
					Size.x = m_Rect.x2 - m_Rect.x1;
			}
			else
			{
				Size.x = m_Rect.x2 - m_Rect.x1;
			}

			if (m_ScrollFlag & Y_SCROLL)
			{
				Size.y = (MaxSize.y - m_Rect.y1);
				if (Size.y < (m_Rect.y2 - m_Rect.y1))
					Size.y = m_Rect.y2 - m_Rect.y1;
			}
			else
			{
				Size.y = m_Rect.y2 - m_Rect.y1;
			}

			SetSize(Size.x, Size.y);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::ComputeSizeByRect(LYNXRECT& rect)
		{
			LYNXRECT Rect = rect;			
			CVector2 Size = CVector2(Rect.x2, Rect.y2);

			if (Size.x < (m_Rect.x2 - m_Rect.x1))
				Size.x = (m_Rect.x2 - m_Rect.x1);			
			else			
				Size.x = (m_Rect.x2 - m_Rect.x1);			

			if (Size.y < m_Rect.y2)			
				Size.y = (m_Rect.y2 - m_Rect.y1);	
			else
				Size.y = (Size.y - m_Rect.y1);

			SetSize(Size.x, Size.y);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   RemoveControl
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::RemoveControl(CControl* con)
		{
			m_ControlList.remove(con);			
			con->_SetScrollGroup(NULL);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   RemoveAllControls
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::RemoveAllControls()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				(*Control)->SetScrollGroup(NULL);
			}
			m_ControlList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   RemoveAllControls
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::ReleaseAllControls()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				(*Control)->SetScrollGroup(NULL);
				(*Control)->GetParentContext()->ReleaseControl((*Control));
			}
			m_ControlList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::ResetControlTransitionOffset()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				(*Control)->ResetTransitionOffset();				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::SetInitShow(LYNXBOOL b)
		{
			m_bInitShow = b;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::SetInitActiveState(LYNXBOOL b)
		{
			m_bInitActive = b;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::Show(LYNXBOOL b)
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				(*Control)->Show(b);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::Activate(LYNXBOOL b)
		{
			CList<CControl*>::CIterator Control;
			LYNXBOOL bActive;

			m_bActive = b;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				if ((*Control)->GetInitActiveState())
				{
					(*Control)->Activate(b);                
				}
				else
				{
					(*Control)->Activate(LYNX_FALSE);                
				}
				//bActive = b & (*Control)->GetInitActiveState();
				//(*Control)->Activate(bActive);                
				//(*Control)->Activate(b);                
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::ResetShow()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				if ((*Control)->GetInitShowState())
				{
					(*Control)->Show(m_bInitShow);
				}
				else
				{
					(*Control)->Show(LYNX_FALSE);
				}				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::ResetActiveState()
		{
			Activate(m_bInitActive);
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::Transit()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				(*Control)->Transit();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::TransitBack(CArray<WORD>* list)
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				LYNXBOOL bFound = LYNX_FALSE;
				if (list)
				{
					for (int i=0; i<list->size(); i++)
					{
						if ((*Control)->GetID() == (*list)[i])
						{
							bFound = LYNX_TRUE;
							break;
						}
					}
				}
				if (!bFound)
					(*Control)->TransitBack();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::SetupContextStepOutList(CArray<WORD>* list)
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				LYNXBOOL bFound = LYNX_FALSE;
				if (list)
				{
					for (int i=0; i<list->size(); i++)
					{
						if ((*Control)->GetID() == (*list)[i])
						{
							bFound = LYNX_TRUE;
							break;
						}
					}
				}
				if (!bFound)
				{
					m_Context->AddStepOutControl(*Control);					
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		void CScrollGroup::SetupContextStepIntList(CArray<WORD>* list)
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				LYNXBOOL bFound = LYNX_FALSE;
				if (list)
				{
					for (int i=0; i<list->size(); i++)
					{
						if ((*Control)->GetID() == (*list)[i])
						{
							bFound = LYNX_TRUE;
							break;
						}
					}
				}
				if (!bFound)
				{
					m_Context->AddStepInControl(*Control);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScrollGroup::IsStop()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				if ((*Control)->IsShown() && !(*Control)->IsStop())
					return LYNX_FALSE;
			}
			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScrollGroup::IsTransitionDone()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				if ((*Control)->IsShown() && !(*Control)->IsTransitionDone() && !(*Control)->IsStop())
					return LYNX_FALSE;
			}
			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScrollGroup::IsStepInDone()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				if ((*Control)->IsShown() && !(*Control)->IsStepInDone())
					return LYNX_FALSE;
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  :   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CScrollGroup::IsStepOutDone()
		{
			CList<CControl*>::CIterator Control;

			for (Control = m_ControlList.begin(); Control != m_ControlList.end(); Control++)
			{
				if ((*Control)->IsShown() && !(*Control)->IsStepOutDone())
					return LYNX_FALSE;
			}
			return LYNX_TRUE;
		}
	}
}