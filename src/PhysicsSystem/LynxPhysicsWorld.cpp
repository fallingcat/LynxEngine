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

namespace LynxEngine 
{	
	namespace PhysicsSystem 
	{
		// {B97D3EA6-B65F-4dc1-A9CA-91B1CD20D636}
		const LYNXGUID CPhysicsWorld::GUID_BALLJOINT = 
		{ 0xb97d3ea6, 0xb65f, 0x4dc1, { 0xa9, 0xca, 0x91, 0xb1, 0xcd, 0x20, 0xd6, 0x36 } };
		// {61561C43-0F81-4bc4-BEE3-CBB8005D8782}
		const LYNXGUID CPhysicsWorld::GUID_HINGJOINT = 
		{ 0x61561c43, 0xf81, 0x4bc4, { 0xbe, 0xe3, 0xcb, 0xb8, 0x0, 0x5d, 0x87, 0x82 } };
		// {862B3744-0EE6-425f-8772-028656B47FE4}
		const LYNXGUID CPhysicsWorld::GUID_SLIDERJOINT = 
		{ 0x862b3744, 0xee6, 0x425f, { 0x87, 0x72, 0x2, 0x86, 0x56, 0xb4, 0x7f, 0xe4 } };
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsWorld::CPhysicsWorld(CPhysicsSystem* const lpsys)
		: CGameObj(lpsys->GetlpEngine())
		{				
			m_lpPhysicsSystem = lpsys;
			m_lpRootSpace = NULL;
			LYNX_VECTOR3D(m_Gravity, 0.0f, -9.8f, 0.0f);
			m_bFastStep = LYNX_FALSE;			
			m_PhysicsMap2RendererType = PMRT_XY_PLANE;

			m_SpaceList.clear();
			m_ObjList.clear();
			m_JointList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsWorld::~CPhysicsWorld(void)
		{
			for (CList<CPhysicsSpace*>::CIterator Space = m_SpaceList.begin(); Space != m_SpaceList.end(); ++Space)
			{
				LYNXDEL (*Space);				
			}
			m_SpaceList.clear();

			for (CList<CPhysicsObj*>::CIterator Obj = m_ObjList.begin(); Obj != m_ObjList.end(); ++Obj)
			{
				LYNXDEL (*Obj);				
			}
			m_ObjList.clear();

			for (CList<CPhysicsJoint*>::CIterator Joint = m_JointList.begin(); Joint != m_JointList.end(); ++Joint)
			{
				LYNXDEL (*Joint);				
			}
			m_JointList.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPhysicsObj* CPhysicsWorld::FindObj(const CString& name)
		{
			CList<CPhysicsObj*>::CIterator Obj;

			Obj = LynxEngine::find(m_ObjList.begin(), m_ObjList.end(), name);
			if (Obj == m_ObjList.end())
				return NULL;

			return (*Obj);				
		}
	}
}