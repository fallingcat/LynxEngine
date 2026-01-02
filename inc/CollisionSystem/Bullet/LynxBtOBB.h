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

#ifndef __LYNXBTOBB_H__
#define __LYNXBTOBB_H__

#include <CollisionSystem/Bullet/LynxBtCollisionObj.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{
	namespace CollisionSystem 
	{	
		class LYNXENGCLASS CBtOBB : public CBtCollisionObj
		{	
			LYNX_DECLARE_CLASS(CBtOBB);
		public:			
		protected:			
			CVector3				m_Center;
			CVector3				m_Extension;		
			CVector3				m_V[8];
		public:			
			CBtOBB();
			CBtOBB(const CVector3& e);
			CBtOBB(const CVector3& c, const CVector3& e);
			CBtOBB(const CVector3* p, int Num);
			CBtOBB(const CBtOBB& b);
			virtual ~CBtOBB();	
			
			LYNXINLINE const CVector3& 	GetCenter() {return m_Center; };
			LYNXINLINE const CVector3& 	GetExtension() {return m_Extension; };			
			LYNXINLINE void				Set(const CVector3& c, const CVector3& e) 
			{
				m_Center = c; 
				m_Extension = e; 
				if (m_Shape)
				{
					LYNXGLOBAL_DEL(m_Shape);
					m_Shape = NULL;
				}
				m_Shape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Extension.x, m_Extension.y, m_Extension.z));
				m_OffsetM.Identity();
				m_OffsetM.m41 = m_Center.x;
				m_OffsetM.m42 = m_Center.y;
				m_OffsetM.m43 = m_Center.z;
				SetupV(); 

			}						
			LYNXINLINE void				Set(const CVector3* p, const int Num);
			LYNXINLINE void				Set(const CVector3* p, const DWORD stride, const int Num);
			LYNXINLINE void				SetByBoundary(const CVector3& min, const CVector3& max);
			LYNXINLINE void				SetupV();
			LYNXINLINE const CVector3	Min() {return (m_Center-m_Extension); };			
			LYNXINLINE const CVector3	Max() {return (m_Center+m_Extension); };			
			LYNXINLINE const CVector3*	GetV() {return m_V; };			

			void						ScreenRect(LYNXRECT& rect, CCameraContainer& cam);

			void						vInstance(CCollisionObj& o);
			void						vRender(CCameraContainer& c);
			void						vSetupCollisionMatrix(CMatrix4& m);
			void						vSetupRenderMatrix(CMatrix4& m);
		};
		LYNXINLINE void	CBtOBB::SetByBoundary(const CVector3& min, const CVector3& max) 
		{	
			m_Center = (max+min)*0.5f; 			
			m_Extension = (max-min)*0.5f; 
			m_Extension.Abs();
			SetupV();
			if (m_Shape)
			{
				LYNXGLOBAL_DEL(m_Shape);
				m_Shape = NULL;
			}
			m_Shape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Extension.x, m_Extension.y, m_Extension.z));
			m_OffsetM.Identity();
			m_OffsetM.m41 = m_Center.x;
			m_OffsetM.m42 = m_Center.y;
			m_OffsetM.m43 = m_Center.z;
		}
		LYNXINLINE void	CBtOBB::SetupV() 
		{	
			m_V[0] = m_Center-m_Extension;
			m_V[1] = CVector3(m_Center.x+m_Extension.x, m_Center.y-m_Extension.y, m_Center.z-m_Extension.z);
			m_V[2] = CVector3(m_Center.x-m_Extension.x, m_Center.y-m_Extension.y, m_Center.z+m_Extension.z);
			m_V[3] = CVector3(m_Center.x+m_Extension.x, m_Center.y-m_Extension.y, m_Center.z+m_Extension.z);
			m_V[4] = CVector3(m_Center.x-m_Extension.x, m_Center.y+m_Extension.y, m_Center.z-m_Extension.z);
			m_V[5] = CVector3(m_Center.x+m_Extension.x, m_Center.y+m_Extension.y, m_Center.z-m_Extension.z);
			m_V[6] = CVector3(m_Center.x-m_Extension.x, m_Center.y+m_Extension.y, m_Center.z+m_Extension.z);
			m_V[7] = m_Center+m_Extension;
		}		
		LYNXINLINE void	CBtOBB::Set(const CVector3* p, const int Num) 		
		{
			// calculate min and max vectors
			CVector3 Min(FLT_MAX, FLT_MAX, FLT_MAX);
			CVector3 Max(-FLT_MAX,-FLT_MAX,-FLT_MAX);			
			for(int i=0; i<Num; i++)
			{
				if(p[i].x < Min.x) 
					Min.x = p[i].x;
				if(p[i].y < Min.y) 
					Min.y = p[i].y;
				if(p[i].z < Min.z) 
					Min.z = p[i].z;

				if(p[i].x > Max.x) 
					Max.x = p[i].x;
				if(p[i].y > Max.y) 
					Max.y = p[i].y;
				if(p[i].z > Max.z) 
					Max.z = p[i].z;				
			}
			// create from vectors
			SetByBoundary(Min, Max);
		}
		LYNXINLINE void	CBtOBB::Set(const CVector3* p, const DWORD stride, const int Num) 		
		{
			LYNXBYTE* lpC = (LYNXBYTE*)p; 
			// calculate min and max vectors
			CVector3 Min(FLT_MAX, FLT_MAX, FLT_MAX);
			CVector3 Max(-FLT_MAX,-FLT_MAX,-FLT_MAX);			
			for(int i=0; i<Num; i++)
			{
				CVector3* lpV = (CVector3*)lpC;
				if(lpV->x < Min.x) 
					Min.x = lpV->x;
				if(lpV->y < Min.y) 
					Min.y = lpV->y;
				if(lpV->z < Min.z) 
					Min.z = lpV->z;

				if(lpV->x > Max.x) 
					Max.x = lpV->x;
				if(lpV->y > Max.y) 
					Max.y = lpV->y;
				if(lpV->z > Max.z) 
					Max.z = lpV->z;		

				lpC += stride;
			}
			// create from vectors
			SetByBoundary(Min, Max);
		}		
	}
}

#endif