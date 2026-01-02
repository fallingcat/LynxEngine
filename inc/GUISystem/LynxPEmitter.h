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

#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__

#include <LynxEngineClasses.h>
#include <GUISystem/LynxImage.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class LYNXENGCLASS CPEmitterControlCreationDesc : public CCreationDesc
		{
			LYNX_DECLARE_CLASS(CPEmitterControlCreationDesc);
		public:
			typedef enum
			{
				CM_FROM_LAYOUT = 0,
				CM_FROM_FILE,
				CM_RUNTIME
			} CREATEMETHOD;

			CREATEMETHOD						m_CreateMethod;
			LynxEngine::CPEmitterCreationDesc	m_PEDesc;
			LynxEngine::Animation::CAnimation*  m_lpAnimation;
			CUserProfile*						m_PEFile;
			CVector2							m_Delay;
			LYNXBOOL							m_bShow;
			LYNXBOOL							m_bActive;
			LYNXRECT							m_Rect;
			CVector2							m_Size;
			int									m_RenderOrder;
			int									m_NumOfEmitter;
			LYNXBOOL							m_bUseSpawnArea;
			LYNXRECT							m_SpawnArea;
		public:
			CPEmitterControlCreationDesc() { Default(); };
			void Default();
		};

		class LYNXENGCLASS CPEmitter : public GUISystem::CControl
		{
			LYNX_DECLARE_CLASS(CPEmitter);

		public:
			typedef CArray<CPEmitterContainer*>		CPEMITTERARRAY;
			typedef CPEMITTERARRAY::iterator		CPEMITTERARRAY_ITER;
			typedef CPEMITTERARRAY::const_iterator  CPEMITTERARRAY_CITER;

			LYNXBOOL					m_bUseSpawnArea;
			LYNXRECT					m_SpawnArea;

		protected:
			CPEMITTERARRAY				m_ContainerList;
			LynxEngine::CEngine*		m_lpEngine;

		public:
			CPEmitter(GUISystem::CGUISystem* lpsys);
			virtual ~CPEmitter();

			virtual CSpriteContainer::ORIGINTYPE vSetOriginType(const CSpriteContainer::ORIGINTYPE ot) { return CSpriteContainer::OT_UPPER_LEFT; }
			virtual DWORD				vSetAlignmentType(const DWORD type) { return type; }
			virtual void				vSetAlignmentRect(const LYNXRECT& r) { m_Rect = r; }
			virtual void				vGetAlignmentRect(LYNXRECT& r)  { r = m_Rect; }
			virtual void				vRender();
			virtual void				vSetSize(float w, float h) {}
			virtual void				vOnHit() {}
			virtual void				vSetPosition(const Math::CVector3& pos) {}
			virtual void				vGetPosition(Math::CVector3& pos) {}

			LYNXBOOL					vCreate(CCreationDesc* desc);
			void						vCreateFromLayout(CUserProfile* file, CXMLElement* node, GUISystem::CContext* context);
			void						vLoop(float step);

			void						Reset();

			LYNXFORCEINLINE CPEMITTERARRAY& GetlpContainerList() { return m_ContainerList; };

		protected:

		};
	}
}
#endif