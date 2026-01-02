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

#ifndef __LYNXRENERABLEOBJ_H__
#define __LYNXRENERABLEOBJ_H__

#include <LynxGameObj.h>
#include <Container/LynxContainer.h>
#include <PhysicsSystem/LynxPhysicsObj.h>

namespace LPCD {
	class FakeVector3;
}

namespace LynxEngine 
{
	class LYNXENGCLASS CRenderableObj : public CGameObj
	{			
	public:	
		enum SOURCETYPE{	
			ORIGIN,
			INSTANCE,	
		};

		typedef enum {	
			DEPTH_LAYER_0 = 0, //Nearest layer
			DEPTH_LAYER_1,
			DEPTH_LAYER_2,
			DEPTH_LAYER_3,
			DEPTH_LAYER_4,
			DEPTH_LAYER_5,
			DEPTH_LAYER_6,
			DEPTH_LAYER_7,
			DEPTH_LAYER_8,
			DEPTH_LAYER_9,
			DEPTH_LAYER_10,
			DEPTH_LAYER_11,
			DEPTH_LAYER_12,
			DEPTH_LAYER_13,
			DEPTH_LAYER_14,
			DEPTH_LAYER_15,
			DEPTH_LAYER_16,
			DEPTH_LAYER_17,
			DEPTH_LAYER_18,
			DEPTH_LAYER_19,
			DEPTH_LAYER_20,
			NUM_DEPTHLAYERTYPE,
		}DEPTHLAYERTYPE;
	protected:
		DECLAR_PARAM_ENUM(LynxEngine::CRenderableObj, DEPTHLAYERTYPE, m_DepthLayer);		
		SOURCETYPE							m_SourceType;
		CContainer*							m_lpContainer;						
		CList<CGameObj*>					m_VisStaticLightList;	
		CList<CGameObj*>					m_VisDynamicLightList;	

		DECLAR_PARAM_BOOL(m_bAttachedToTerrain);
		CTerrainContainer*					m_lpAttachedTerrain;	

		DECLAR_PARAM_BOOL(m_bShow);
		
		PhysicsSystem::CPhysicsObj*			m_lpPhyObj;
	public:
		LYNX_DECLARE_CLASS(CRenderableObj);
		CRenderableObj();
		CRenderableObj(CEngine* lpe);
		virtual ~CRenderableObj();						

		DECLARE_CLASSSCRIPT
		LYNXINLINE LYNXBOOL					IsCurrentAnimationStopped() const {return m_lpContainer->m_AnimationSet->GetCurrentAnimation()->m_bPlayStop; };
		LYNXINLINE Animation::CAnimation*	_LoadAnimation(const LYNXCHAR* filename) {return m_lpContainer->LoadAnimation(filename); };
		LYNXINLINE Animation::CAnimation*	SetCurrentAnimationByName(const LYNXCHAR* name) {return m_lpContainer->SetCurrentAnimation(name); };
		LYNXINLINE void						SetCurrentAnimationByPtr(Animation::CAnimation* a) {m_lpContainer->SetCurrentAnimation(a); };		

		LYNXFORCEINLINE	void				SetDepthLayer(const DEPTHLAYERTYPE d) {m_DepthLayer = d; };
		LYNXFORCEINLINE	DEPTHLAYERTYPE		GetDepthLayer() {return m_DepthLayer; };

		LYNXFORCEINLINE LYNXBOOL			IsAttachedToTerrain() const {return m_bAttachedToTerrain; };
		LYNXFORCEINLINE void				AttachedToTerrain(LYNXBOOL b) {m_bAttachedToTerrain = b; };
		void								SetAttachedTerrain(CTerrainContainer* t);
		LYNXFORCEINLINE CTerrainContainer*	GetAttachedTerrain() const {return m_lpAttachedTerrain; };

		virtual LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
		virtual LYNXBOOL					vRegisterScriptClassVar(ScriptSystem::CScript* const lps);

		LYNXFORCEINLINE bool				operator < (CRenderableObj& rhs) { return ((*m_lpContainer) < (*(rhs.GetlpContainer()))); };		
		LYNXFORCEINLINE bool				operator > (CRenderableObj& rhs) { return ((*m_lpContainer) > (*(rhs.GetlpContainer()))); };		
		LYNXFORCEINLINE bool				operator == (CRenderableObj& rhs) { return ((*m_lpContainer) == (*(rhs.GetlpContainer()))); };	
		
		static LYNXINLINE bool				compare_front2back(CRenderableObj* e1, CRenderableObj* e2) { return (*e1) < (*e2); };
		static LYNXINLINE bool				compare_back2front(CRenderableObj* e1, CRenderableObj* e2) { return (*e1) > (*e2); };		
		static LYNXINLINE bool				compare_material(CRenderableObj* e1, CRenderableObj* e2) { return (e1->GetlpContainer()->GetlpCurrentMaterial()) > (e2->GetlpContainer()->GetlpCurrentMaterial()); };

		LYNXINLINE CContainer*				GetlpContainer() {return m_lpContainer; };
		LYNXINLINE void						SetlpContainer(CContainer* lpc) {m_lpContainer = lpc; };		
		LYNXBOOL							Create(CContainer::TYPE type);
		LYNXINLINE void						CalculateLOD(CCameraContainer* cam) {m_lpContainer->vCalculateLOD(cam); };

		LYNXINLINE void						SetSourceType(const SOURCETYPE st) {m_SourceType = st; };			
		LYNXINLINE const SOURCETYPE			GetSourceType() const {return m_SourceType; };					
		
		LYNXFORCEINLINE LYNXBOOL			IsShown() const {return m_bShow; };
		LYNXFORCEINLINE void				Show(const LYNXBOOL b) {m_bShow = b; };
		virtual LYNXFORCEINLINE void		vShow(const LYNXBOOL b) {Show(b); };		
		        
        LYNXFORCEINLINE void				Sort(const LYNXBOOL b) {m_lpContainer->Sort(b); };

		LYNXINLINE void						SetTransformMatrix(const Math::CMatrix4& m) { m_lpContainer->SetTransformMatrix(m);};
		LYNXINLINE const Math::CMatrix4&	GetTransformMatrix() const { return m_lpContainer->GetTransformMatrix();};
		LYNXINLINE void						SetLocalMatrix(const Math::CMatrix4& m) { return m_lpContainer->SetLocalMatrix(m);};
		LYNXINLINE const Math::CMatrix4&	GetLocalMatrix() const { return m_lpContainer->GetLocalMatrix();};
		LYNXINLINE void						SetGlobalMatrix(const Math::CMatrix4& m) { return m_lpContainer->SetGlobalMatrix(m);};
		LYNXINLINE const Math::CMatrix4&	GetGlobalMatrix() const { return m_lpContainer->GetGlobalMatrix();};
		LYNXINLINE void						UpdateTransform() { m_lpContainer->UpdateTransform();};
		LYNXINLINE void						ResetTransform() {m_lpContainer->ResetTransform(); };

		LYNXINLINE void						SetPosition(const Math::CVector3& pos) { m_lpContainer->vSetPosition(pos);};		
		LYNXINLINE void						GetPosition(Math::CVector3& pos) const { m_lpContainer->vGetPosition(pos);};
		LYNXINLINE void						GetScreenPosition(Math::CVector2& pos, CCameraContainer* cam, int w, int h) const { m_lpContainer->vGetScreenPosition(pos, cam, w, h);};
		LYNXINLINE void						GetScreenPosition(Math::CVector2& pos, CCameraContainer* cam) const { m_lpContainer->vGetScreenPosition(pos, cam);};
		LYNXINLINE void						GetDirection(Math::CVector3& dir) const { m_lpContainer->GetDirection(dir);};
		LYNXINLINE void						GetUpDirection(Math::CVector3& pos) const { m_lpContainer->GetUpDirection(pos);};
		LYNXINLINE void						GetRightDirection(Math::CVector3& pos) const { m_lpContainer->GetRightDirection(pos);};

		LYNXINLINE void						Forward(const Math::CReal dist) { m_lpContainer->Forward(dist);};							
		LYNXINLINE void						Translate(const Math::CVector3& v, LYNXMATHORDER order) { m_lpContainer->Translate(v, order);};			
		LYNXINLINE void						Scale(const Math::CVector3& v, LYNXMATHORDER order) {m_lpContainer->Scale(v, order);};
		LYNXINLINE void						Yaw(const Math::CReal deg, LYNXMATHORDER order) { m_lpContainer->Yaw(deg, order);};
		LYNXINLINE void						Pitch(const Math::CReal deg, LYNXMATHORDER order) { m_lpContainer->Pitch(deg, order);};
		LYNXINLINE void						Roll(const Math::CReal deg, LYNXMATHORDER order) { m_lpContainer->Roll(deg, order);};
		LYNXINLINE void						MulMatrix(const Math::CMatrix4& m, LYNXMATHORDER order) {m_lpContainer->MulMatrix(m, order);};
		LYNXINLINE void						Rotate(const Math::CReal ang, const Math::CVector3& axis, LYNXMATHORDER order) {m_lpContainer->Rotate(ang, axis, order);};		
		LYNXINLINE void						AttachToTerrain(CTerrainContainer& t, LYNXBOOL balign) {if (m_bAttachedToTerrain) m_lpContainer->vAttachToTerrain(t, balign); }

		LYNXINLINE CReal					GetTime(void) const {return m_lpContainer->GetTime();};
		LYNXINLINE void						SetTime(const Math::CReal t) {m_lpContainer->SetTime(t); };
		//LYNXINLINE LYNXREAL					GetStartTime(void) {return m_lpContainer->GetStartTime(); };
		//LYNXINLINE void						SetStartTime(LYNXREAL t) {m_lpContainer->SetStartTime(t); };
		//LYNXINLINE LYNXREAL					GetEndTime(void) {return m_lpContainer->GetEndTime(); };		
		//LYNXINLINE void						SetEndTime(LYNXREAL t) {m_lpContainer->SetEndTime(t); };
		//LYNXINLINE void						SetTimes(LYNXREAL st, LYNXREAL et, LYNXREAL t) {m_lpContainer->SetTimes(st, et, t); };

		//LYNXINLINE LYNXBOOL					Play(LYNXREAL skip, LYNXPLAYMODE mode) {return  m_lpContainer->vPlay(skip, mode); };
		LYNXINLINE LYNXBOOL					Play(const Math::CReal step) {return  m_lpContainer->vPlay(step); };

		LYNXINLINE LYNXBOOL					IsReflective() const {return  m_lpContainer->IsReflective(); };
		LYNXINLINE LYNXBOOL					IsRefractive() const {return  m_lpContainer->IsRefractive(); };
		LYNXINLINE LYNXPLANE&				GetRNRPlane() const {return  m_lpContainer->m_RNRPlane; };				
		
		LYNXINLINE void						SetSize(const CReal& w, const CReal& h) { m_lpContainer->vSetSize(w, h);};
		
		LYNXINLINE void						SetCurrentMaterialTechnique(int i) {m_lpContainer->SetCurrentMaterialTechnique(i); };		
		//LYNXINLINE DWORD					GetNumMaterials() {return m_lpContainer->vGetNumMaterials(); };
		//LYNXINLINE AMS::CAdvMaterial*		GetlpMaterial(int i) {return m_lpContainer->vGetlpMaterial(i); };		

		LYNXINLINE Animation::CAnimation*	LoadAnimation(const LYNXCHAR* filename) {return m_lpContainer->LoadAnimation(filename); };
		LYNXINLINE Animation::CAnimation*	LoadAnimation(const LYNXCHAR* name, const LYNXCHAR* filename) {return m_lpContainer->LoadAnimation(name, filename); };
		LYNXINLINE Animation::CAnimation*	SetCurrentAnimation(const LYNXCHAR* name) {return m_lpContainer->SetCurrentAnimation(name); };
		LYNXINLINE void						SetCurrentAnimation(Animation::CAnimation* a) {m_lpContainer->SetCurrentAnimation(a); };

		LYNXINLINE void						vRender(CCameraContainer* cam = NULL) {m_lpContainer->vRender(cam); };
		LYNXINLINE void						_Render(CCameraContainer* cam = NULL) {vRender(cam); };
		virtual void						vRenderCollisionHie(CCameraContainer* cam);

		LYNXINLINE void						CalculateDist(CVector3& v) {return m_lpContainer->CalculateDist(v); };

		virtual LYNXINLINE void				vComputeProxy() {m_lpContainer->vComputeProxy(); };
		
		void								ComputeVisLightList();
		LYNXINLINE int						GetNumVisStaticLights() {return (int)m_VisStaticLightList.size(); };		
		LYNXINLINE int						GetNumVisDynamicLights() {return (int)m_VisDynamicLightList.size(); };		
		LYNXINLINE CList<CGameObj*>&		GetVisStaticLightList() {return m_VisStaticLightList; };
		LYNXINLINE CList<CGameObj*>&		GetVisDynamicLightList() {return m_VisDynamicLightList; };

		// Physics ------------------------------------------------------------------------------------------------
		LYNXINLINE LYNXBOOL					IsPhysics() {return (m_lpPhyObj) ? LYNX_TRUE : LYNX_FALSE; };	
		LYNXINLINE PhysicsSystem::CPhysicsObj* const GetlpPhyObj() {return (m_lpPhyObj); };		

		virtual LYNXBOOL					vCreatePhysics(PhysicsSystem::CPhysicsWorld* lpw, PhysicsSystem::CPhysicsShape* lps = NULL) {return LYNX_FALSE; };
		void								DestoryPhysics();		
		
		LYNXINLINE void						AddShape(PhysicsSystem::CPhysicsShape* lpg) { LYNX_ASSERT(m_lpPhyObj); m_lpPhyObj->vAddShape(lpg); };		
		LYNXINLINE void						RemoveShape(PhysicsSystem::CPhysicsShape* lpg) { LYNX_ASSERT(m_lpPhyObj); m_lpPhyObj->vRemoveShape(lpg); };		
		LYNXINLINE void						RemoveAllShapes() { LYNX_ASSERT(m_lpPhyObj); m_lpPhyObj->vRemoveAllShapes(); };					

		// State machine
		LYNXBOOL							vSetCurrentState(int s);
		LYNXBOOL							vSetCurrentState(const CString& s);
		void								vUpdateStateMachine(const CEvent& s);
		StateMachine::CState*				CreateState(const CString& name, const CString&filename);

		virtual void						vOnEvent(const CEvent& event);
	};	
	LYNXENGCLASS CString GetEnumStr(CRenderableObj::DEPTHLAYERTYPE e);
}

#endif