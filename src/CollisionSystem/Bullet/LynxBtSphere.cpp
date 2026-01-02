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
#include <CollisionSystem/Bullet/LynxBtSphere.h>

#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btTransformUtil.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
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
		CBtSphere::CBtSphere(void) :
			m_Center(CVector3(0.0f)),
			m_Radius(1.0f)
		{
			m_Type = SPHERE;
			m_Shape = LYNXGLOBAL_NEW btSphereShape(m_Radius);		
			m_OffsetM.Identity();
			m_Shape->setMargin(1.5f);
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtSphere::CBtSphere(const float r) :
			m_Center(CVector3(0.0f)),
			m_Radius(r)			
		{
			m_Type = SPHERE;
			m_Shape = LYNXGLOBAL_NEW btSphereShape(m_Radius);
			m_OffsetM.Identity();
			m_Shape->setMargin(1.5f);
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtSphere::CBtSphere(const CVector3& c, const float r) :
			m_Center(c),
			m_Radius(r)
		{
			m_Type = SPHERE;
			m_Shape = LYNXGLOBAL_NEW btSphereShape(m_Radius);
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
		CBtSphere::CBtSphere(const CBtSphere& p)
		{
			m_Type = SPHERE;
			m_Radius = p.m_Radius;
			m_OffsetM = p.m_OffsetM;
			m_Shape = LYNXGLOBAL_NEW btSphereShape(m_Radius);
			m_Shape->setMargin(1.5f);
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBtSphere::vInstance(CCollisionObj& o)
		{
			CBtSphere& p = (CBtSphere&)o;

			m_Type = SPHERE;
			m_Radius = p.m_Radius;			
			m_OffsetM = p.m_OffsetM;
			if (m_Shape)
				LYNXGLOBAL_DEL(m_Shape);
			m_Shape = LYNXGLOBAL_NEW btSphereShape(m_Radius);
			m_Shape->setMargin(1.5f);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtSphere::~CBtSphere(void)
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
		void CBtSphere::vSetupCollisionMatrix(CMatrix4& m)
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
		void CBtSphere::vSetupRenderMatrix(CMatrix4& m)
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
		void CBtSphere::vRender(CCameraContainer& c)
		{
			LYNXCOLORRGBA Color = { 255, 100, 100, 255 };
			CMatrix4 M, PM, CM, ICM, TM, TM1, SM;
			CVector3 NoScale(1.0f), Scale(1.0f);
			CVector3 P[36], TP[36];
			CVector2 SP[36];

			vSetupRenderMatrix(M);

			for (int i = 0; i < 36; i++)
			{
				P[i] = CVector3(m_Radius*lynxQuickSin(i*10.0f), 0.0f, m_Radius*lynxQuickCos(i*10.0f));			
				Math::PointXMatrix(TP[i], P[i], M);
				m_lpEngine->GetScreenPosition(SP[i], TP[i], c, c.GetViewportRect());
			}
			for (int i = 0; i < 36; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[i], SP[(i + 1) % 36], Color);
			}			

			for (int i = 0; i < 36; i++)
			{
				P[i] = CVector3(m_Radius*lynxQuickSin(i*10.0f), m_Radius*lynxQuickCos(i*10.0f), 0.0f);
				Math::PointXMatrix(TP[i], P[i], M);
				m_lpEngine->GetScreenPosition(SP[i], TP[i], c, c.GetViewportRect());
			}
			for (int i = 0; i < 36; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[i], SP[(i + 1) % 36], Color);
			}

			for (int i = 0; i < 36; i++)
			{
				P[i] = CVector3(0.0f, m_Radius*lynxQuickSin(i*10.0f), m_Radius*lynxQuickCos(i*10.0f));
				Math::PointXMatrix(TP[i], P[i], M);
				m_lpEngine->GetScreenPosition(SP[i], TP[i], c, c.GetViewportRect());
			}
			for (int i = 0; i < 36; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[i], SP[(i + 1) % 36], Color);
			}
		}
	}
}