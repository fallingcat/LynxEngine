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

#ifndef __LYNXSPHERE_H__
#define __LYNXSPHERE_H__

#include <CollisionSystem/LynxCollisionObj.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{
	namespace CollisionSystem 
	{	
		class LYNXENGCLASS CSphere : public CCollisionObj
		{	
			LYNX_DECLARE_CLASS(CAABB);
		public:			
		protected:			
			CVector3				m_Center;
			CReal					m_Radius;		
		public:			
			CSphere();
			CSphere(CVector3& c, CReal& r);
			virtual ~CSphere();	

			LYNXINLINE const CVector3& 	GetCenter() {return m_Center; };
			LYNXINLINE const CReal& 	GetRadius() {return m_Radius; };			
			LYNXINLINE virtual void		vCreate(CVector3& c, CReal& r) {m_Center = c; m_Radius = r; };									
		};		
	}
}

#endif