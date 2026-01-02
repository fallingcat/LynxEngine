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
		CBtCapsule::CBtCapsule(void) :
			m_Center(CVector3(0.0f)),
			m_Radius(1.0f), 
			m_Height(1.0f)
		{
			m_Type = CAPSULE;				
			m_Shape = LYNXGLOBAL_NEW btCapsuleShape(m_Radius, m_Height);		
			m_Shape->setMargin(1.5f);
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtCapsule::CBtCapsule(const CVector3& c, const float r, const float h) :
			m_Center(c),
			m_Radius(r), 
			m_Height(h)
		{
			m_Type = CAPSULE;				
			m_Shape = LYNXGLOBAL_NEW btCapsuleShape(m_Radius, m_Height);	
			m_OffsetM.Identity();
			m_OffsetM.m41 = c.x;
			m_OffsetM.m42 = c.y;
			m_OffsetM.m43 = c.z;
			m_Shape->setMargin(1.5f);				
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtCapsule::CBtCapsule(const CBtCapsule& p)
		{
			m_Type = CAPSULE;				
			m_Radius = p.m_Radius;
			m_Height = p.m_Height;
			m_OffsetM = p.m_OffsetM;
			m_Shape = LYNXGLOBAL_NEW btCapsuleShape(m_Radius, m_Height);	
			m_Shape->setMargin(1.5f);
			if (glpKernel)
				m_lpEngine = glpKernel->GetlpEngine();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CBtCapsule::vInstance(CCollisionObj& o)
		{
			CBtCapsule& p = (CBtCapsule&)o;

			m_Type = CAPSULE;				
			m_Radius = p.m_Radius;
			m_Height = p.m_Height;
			m_OffsetM = p.m_OffsetM;
			if (m_Shape)
				LYNXGLOBAL_DEL(m_Shape);
			m_Shape = LYNXGLOBAL_NEW btCapsuleShape(m_Radius, m_Height);	
			m_Shape->setMargin(1.5f);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBtCapsule::~CBtCapsule(void)
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
		void CBtCapsule::vSetupCollisionMatrix(CMatrix4& m)
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
		void CBtCapsule::vSetupRenderMatrix(CMatrix4& m)
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
		void CBtCapsule::vRender(CCameraContainer& c)
		{
			LYNXCOLORRGBA Color = { 100, 100, 255, 255 };
			CMatrix4 M, PM, CM, ICM, TM, TM1, SM;
			CVector3 NoScale(1.0f), Scale(1.0f);
			CVector3 P[2][36], TP[2][36];
			CVector2 SP[2][36];

			vSetupRenderMatrix(M);

			for (int i = 0; i < 36; i++)
			{
				P[0][i] = CVector3(m_Radius*lynxQuickCos(i*10.0f), m_Height*0.5f, m_Radius*lynxQuickSin(i*10.0f));
				Math::PointXMatrix(TP[0][i], P[0][i], M);
				m_lpEngine->GetScreenPosition(SP[0][i], TP[0][i], c, c.GetViewportRect());
			}
			for (int i = 0; i < 36; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][i], SP[0][(i + 1) % 36], Color);
			}
			for (int i = 0; i < 36; i++)
			{
				P[1][i] = CVector3(m_Radius*lynxQuickCos(i*10.0f),  -m_Height*0.5f, m_Radius*lynxQuickSin(i*10.0f));
				Math::PointXMatrix(TP[1][i], P[1][i], M);
				m_lpEngine->GetScreenPosition(SP[1][i], TP[1][i], c, c.GetViewportRect());
			}
			for (int i = 0; i < 36; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[1][i], SP[1][(i + 1) % 36], Color);
			}
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][0], SP[1][0], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][9], SP[1][9], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][18], SP[1][18], Color);
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][27], SP[1][27], Color);

			// Cap +y
			for (int i = 0; i < 19; i++)
			{
				P[0][i] = CVector3(m_Radius*lynxQuickCos(i*10.0f), m_Radius*lynxQuickSin(i*10.0f) + m_Height*0.5f, 0.0f);
				Math::PointXMatrix(TP[0][i], P[0][i], M);
				m_lpEngine->GetScreenPosition(SP[0][i], TP[0][i], c, c.GetViewportRect());
			}
			for (int i = 0; i < 18; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][i], SP[0][(i + 1) % 36], Color);
			}
			for (int i = 0; i < 19; i++)
			{
				P[0][i] = CVector3(0.0f, m_Radius*lynxQuickSin(i*10.0f) + m_Height*0.5f, m_Radius*lynxQuickCos(i*10.0f));
				Math::PointXMatrix(TP[0][i], P[0][i], M);
				m_lpEngine->GetScreenPosition(SP[0][i], TP[0][i], c, c.GetViewportRect());
			}
			for (int i = 0; i < 18; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][i], SP[0][(i + 1) % 36], Color);
			}			

			// Cap -y
			for (int i = 0; i < 36; i++)
			{
				P[0][i] = CVector3(m_Radius*lynxQuickCos(i*10.0f), m_Radius*lynxQuickSin(i*10.0f) - m_Height*0.5f, 0.0f);
				Math::PointXMatrix(TP[0][i], P[0][i], M);
				m_lpEngine->GetScreenPosition(SP[0][i], TP[0][i], c, c.GetViewportRect());
			}
			for (int i = 18; i < 36; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][i], SP[0][(i + 1) % 36], Color);
			}
			for (int i = 0; i < 36; i++)
			{
				P[0][i] = CVector3(0.0f, m_Radius*lynxQuickSin(i*10.0f) - m_Height*0.5f, m_Radius*lynxQuickCos(i*10.0f));
				Math::PointXMatrix(TP[0][i], P[0][i], M);
				m_lpEngine->GetScreenPosition(SP[0][i], TP[0][i], c, c.GetViewportRect());
			}
			for (int i = 18; i < 36; i++)
			{
				m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->DrawLine(SP[0][i], SP[0][(i + 1) % 36], Color);
			}
		}
	}
}