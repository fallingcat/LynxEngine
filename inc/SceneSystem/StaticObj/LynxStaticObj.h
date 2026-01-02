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

#ifndef __LYNXSTATICOBJ_H__
#define __LYNXSTATICOBJ_H__

#include <LynxRenderableObj.h>
#include <SceneSystem/LynxSurface.h>
#include <CollisionSystem/LynxAABB.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{		
		class LYNXENGCLASS CStaticObj : public CRenderableObj
		{
			LYNX_DECLARE_CLASS(CStaticObj);
		public:
			enum TYPE{					
				STATIC_MESH = 0,
				STATIC_MODEL,
				SKY,
				CLOUD,
				TERRAIN,
				TREE,
				GRASS,
				NUM_TYPE,
			};			
			
			enum ATTRTYPE{	
				NORMAL,
				BOUNDARY,	
				WATER,	
				LAVA,
				ICE,					
				JUMP,
				DAMAGE,
				DESTROY,
				NUM_ATTRTYPE,
			};			

		protected:
			CScene*								m_lpScene;					
			TYPE								m_StaticObjType;										
		public:			
			float								m_Visibility;
			int									m_Attribute;										
	
			CollisionSystem::CAABB				m_LocalAABB;
			LYNXVECTOR3D						m_lpCheckV[8];
			LYNXFAN								m_lpCheckFan[6];				
		public:			
			CStaticObj(CScene* const lpscene);				
			virtual ~CStaticObj();

			DECLARE_CLASSSCRIPT
			LYNXBOOL							vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			LYNXBOOL							vRegisterScriptClassVar(ScriptSystem::CScript* const lps);		
			LYNXBOOL							CreateStaticMesh(const LYNXCHAR *name, const LYNXCHAR *filename);
			LYNXBOOL							CreateStaticModel(const LYNXCHAR *name, const LYNXCHAR *filename);
			
			LYNXINLINE void						SetlpScene(CScene* const lps) {m_lpScene = lps; };						
			LYNXINLINE const TYPE				GetStaticObjType() const {return m_StaticObjType; };			
			
			virtual LYNXBOOL					vCreate(const LYNXCHAR *name, LynxEngine::CContainer::TYPE mtype, CCreationDesc* desc, LYNXBOOL bphy = LYNX_FALSE, PhysicsSystem::CPhysicsWorld* lpw = NULL);
			virtual LYNXBOOL					vInstance(const LYNXCHAR *name, CStaticObj& rhs);			
			virtual void						vLoop(float step);			

			LYNXBOOL							vCreatePhysics(PhysicsSystem::CPhysicsWorld* lpw, PhysicsSystem::CPhysicsShape* lps = NULL);					
		private:
			friend class CScene;			
		};
	}
	LYNXENGCLASS CString GetEnumStr(SceneSystem::CStaticObj::TYPE e);
}

#endif
