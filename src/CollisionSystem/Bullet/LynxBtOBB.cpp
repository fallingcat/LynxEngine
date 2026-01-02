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

#include <LynxEngine_PCH.h>
#include <CollisionSystem/Bullet/LynxBtCollisionSystem.h>
#include <CollisionSystem/Bullet/LynxBtOBB.h>
#include <CollisionSystem/Bullet/LynxBtCapsule.h>
#include <CollisionSystem/Bullet/LynxBtPlane.h>

#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btTransformUtil.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
#include "BulletCollision/NarrowPhaseCollision/btPointCollector.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"

namespace LynxEngine 
{	
	namespace CollisionSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtOBB::CBtOBB(void) :
			m_Center(0.0f, 0.0f, 0.0f), 
			m_Extension(1.0f, 2.0f, 3.0f)
		{
			m_Type = OBB;				
			m_Shape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Extension.x, m_Extension.y, m_Extension.z));
			m_OffsetM.Identity();			
			m_Shape->setMargin(1.5f);
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtOBB::CBtOBB(const CVector3& c, const CVector3& e) :
			m_Center(c), 
			m_Extension(e)
		{
			m_Type = OBB;
			m_Shape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Extension.x, m_Extension.y, m_Extension.z));
			m_OffsetM.Identity();
			m_OffsetM.m41 = m_Center.x;
			m_OffsetM.m42 = m_Center.y;
			m_OffsetM.m43 = m_Center.z;
			m_Shape->setMargin(1.5f);
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtOBB::CBtOBB(const CVector3& e) :
			m_Center(0.0f, 0.0f, 0.0f), 
			m_Extension(e)
		{
			m_Type = OBB;
			m_Shape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Extension.x, m_Extension.y, m_Extension.z));
			m_OffsetM.Identity();
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtOBB::CBtOBB(const CVector3* p, const int Num)			
		{
			Set(p, Num);
			m_Type = OBB;
			m_Shape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Extension.x, m_Extension.y, m_Extension.z));
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtOBB::CBtOBB(const CBtOBB& b)
		{
			m_Type = OBB;
			m_Center = b.m_Center;
			m_Extension = b.m_Extension;
			m_Shape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Extension.x, m_Extension.y, m_Extension.z));			
			m_OffsetM = b.m_OffsetM;
			SetupV(); 
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBtOBB::vInstance(CCollisionObj& o)
		{
			CBtOBB& b = (CBtOBB&)o;

			m_Type = OBB;
			m_Center = b.m_Center;
			m_Extension = b.m_Extension;
			m_OffsetM = b.m_OffsetM;
			if (m_Shape)
				LYNXGLOBAL_DEL(m_Shape);
			m_Shape = LYNXGLOBAL_NEW btBoxShape(btVector3(m_Extension.x, m_Extension.y, m_Extension.z));
			SetupV(); 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtOBB::~CBtOBB(void)
		{
			if (m_Shape)
			{
				LYNXGLOBAL_DEL(m_Shape);
				m_Shape = NULL;
			}
		}					
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBtOBB::vSetupCollisionMatrix(CMatrix4& m)
		{
			CMatrix4 PM, CM, ICM, TM;
			CVector3 NoScale(1.0f), Scale(1.0f);

			m.Identity();
			if (m_lpParentContainer)
			{
				PM = m_lpParentContainer->GetTransformMatrix();
				PM.GetScale(Scale);
				PM.ReplaceScale(NoScale);
				if (!m_bSkipTransfomScale)
				{
					btVector3 S(Scale.x, Scale.y, Scale.z);
					m_Shape->setLocalScaling(S);
				}
				Math::MatrixXMatrix(m, m_OffsetM, PM);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBtOBB::vSetupRenderMatrix(CMatrix4& m)
		{
			CMatrix4 PM, CM, ICM, TM;
			CVector3 NoScale(1.0f), Scale(1.0f);

			m.Identity();
			if (m_lpParentContainer)
			{
				PM = m_lpParentContainer->GetTransformMatrix();
				if (m_bSkipTransfomScale)
				{
					PM.GetScale(Scale);
					PM.ReplaceScale(NoScale);
				}
				Math::MatrixXMatrix(m, m_OffsetM, PM);
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBtOBB::ScreenRect(LYNXRECT& rect, CCameraContainer& cam)
		{
			CMatrix4 M, PM, CM;
			CVector3 NoScale(1.0f), Scale(1.0f);

			SetupV();
			
			M.Identity();
			if (m_lpParentContainer)
            {                
                CM.Identity();
                CM.m41 = -m_Center.x;
                CM.m42 = -m_Center.y;
                CM.m43 = -m_Center.z;
                
                PM = m_lpParentContainer->GetTransformMatrix();
				if (m_bSkipTransfomScale)
					PM.ReplaceScale(NoScale);
                Math::MatrixXMatrix(M, CM, PM);			
				
				rect.x1 = 1<<30;
				rect.y1 = 1<<30;
				rect.x2 = -(1<<30);
				rect.y2 = -(1<<30);
				CVector3 TPos;
				CVector2 SPos;
				for (int i=0; i<8; i++)
				{
					lynxPointXMatrix(&TPos, &m_V[i], &M);
					m_lpEngine->GetScreenPosition(SPos, TPos, cam, cam.GetViewportRect());
					if (SPos.x > rect.x2)
						rect.x2 = SPos.x;
					else if (SPos.x < rect.x1)
						rect.x1 = SPos.x;

					if (SPos.y > rect.y2)
						rect.y2 = SPos.y;
					else if (SPos.y < rect.y1)
						rect.y1 = SPos.y;
				}
            }
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBtOBB::vRender(CCameraContainer& c)
		{
			LYNXCOLORRGBA Color = { 0, 0, 0, 255 };
			CMatrix4 M, PM, CM, ICM, TM, TM1, SM;
			CVector3 NoScale(1.0f), Scale(1.0f);
			CVector3 P[8], TP[8];
			CVector2 SP[8];

			vSetupRenderMatrix(M);

			for (int i = 0; i < 8; i++)
				P[i] = CVector3(0.0f);

			P[0].x -= m_Extension.x;
			P[0].y -= m_Extension.y;
			P[0].z -= m_Extension.z;

			P[1].x += m_Extension.x;
			P[1].y -= m_Extension.y;
			P[1].z -= m_Extension.z;

			P[2].x += m_Extension.x;
			P[2].y += m_Extension.y;
			P[2].z -= m_Extension.z;

			P[3].x -= m_Extension.x;
			P[3].y += m_Extension.y;
			P[3].z -= m_Extension.z;

			P[4].x -= m_Extension.x;
			P[4].y -= m_Extension.y;
			P[4].z += m_Extension.z;

			P[5].x += m_Extension.x;
			P[5].y -= m_Extension.y;
			P[5].z += m_Extension.z;

			P[6].x += m_Extension.x;
			P[6].y += m_Extension.y;
			P[6].z += m_Extension.z;

			P[7].x -= m_Extension.x;
			P[7].y += m_Extension.y;
			P[7].z += m_Extension.z;
			
			for (int i = 0; i < 8; i++)
			{
				Math::PointXMatrix(TP[i], P[i], M);
				m_lpEngine->GetScreenPosition(SP[i], TP[i], c, c.GetViewportRect());
			}
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0], SP[1], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[1], SP[2], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[2], SP[3], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[3], SP[0], Color);

			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[4], SP[5], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[5], SP[6], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[6], SP[7], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[7], SP[4], Color);

			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0], SP[4], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[1], SP[5], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[2], SP[6], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[3], SP[7], Color);		
		}
	}
}