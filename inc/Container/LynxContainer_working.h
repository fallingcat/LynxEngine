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

#ifndef __LYNXCONTAINER_H__
#define __LYNXCONTAINER_H__

#include <LynxCore.h>
#include <LynxArray.h>
#include <LynxList.h>
#include <LynxResource.h>
#include <Animation/LynxAnimation.h>

namespace LynxEngine 
{
	namespace MaterialSystem
	{
		class CMaterial;
	}
	class CEngine;
	class CRenderableObj;
	class LYNXENGCLASS CContainer : public CResource
	{
		LYNX_DECLARE_CLASS(CContainer);
	public:
		enum TYPE{
			REFERENCE = 0,
			GEOMETRY,
			STATIC_MESH,
			STATIC_MODEL,
			CAMERA,
			MODEL,
			SPRITE3D,
			CROSS_SPRITE,	
			PEMITTER,	
			MESH,
			BONE,
			JOINT_BALL,	
			JOINT_HINGE,
			JOINT_SLIDER,
			JOINT_UNIVERSAL,
			JOINT_HINGE2,
			JOINT_FIXED,
			JOINT_ANGULAR_MOTOR,
			NUM_TYPE,
		};

		typedef enum {
			AMBIENT_LIGHTING_MATERIAL = 0,
			POINT_LIGHTING_MATERIAL,
			SPOT_LIGHTING_MATERIAL,
			DIRECTIONAL_LIGHTING_MATERIAL,
			ZPASS_MATERIAL,
			SHADOW_MATERIAL,
			NUM_MATERIAL_TYPE,
		}MATERIALTYPE;
			
		BOOL										m_bGlow;	
		float										m_BloomingIntensity;
		LYNXREAL									m_Visibility;	
	protected:		
		CEngine*									m_lpEngine;		
		TYPE										m_Type;				
		LYNXSOURCETYPE								m_SourceType;
		char										m_Name[64];
		LYNXMATRIX									m_LocalM;				/*!< Local transformation Matrix */			
		LYNXMATRIX									m_GlobalM;				/*!< Global transformation Matrix */		
		LYNXMATRIX									m_PivotM;				/*!< transfrom matrix from center to pivot */
		LYNXMATRIX									m_M;			

		LPLYNXVECTOR3D								m_lpCenter; 		
		LPLYNXVECTOR3D								m_lpDirection;			/*!< Object's World tramsformed Direction */
		LPLYNXVECTOR3D								m_lpUpDir;				/*!< Object's World tramsformed Up Direction	*/	
		LPLYNXVECTOR3D								m_lpRightDir;			/*!< Object's World tramsformed Right Direction */	

		//Hierarchy related data
		CContainer*									m_lpParent;
		CArray<CContainer*>							m_lpChildrenArray;

		//Animation related data		
		Animation::CAnimation						m_Animation;			
		
		CRenderableObj*								m_lpRenObj;
		CArray<CContainer*>							m_SubContainerArray;		

		CArray<MaterialSystem::CMaterial*>			m_MaterialArray;
		MaterialSystem::CMaterial*					m_lpCurrentMaterial;

		//DECLAR_PARAM_BOOL(m_bParam);
		//DECLAR_PARAM_FLOAT(m_Speed);
	public:		
		CContainer();
		CContainer(CEngine *lpengine);
		virtual ~CContainer();

		virtual char *								vGetName() = 0;							
		
		LYNXINLINE const TYPE						GetType() {return m_Type; };
		LYNXINLINE const char*						GetTypeStr() {return GetTypeStr(m_Type); };
		const char*									GetTypeStr(TYPE t);

		LYNXINLINE void								SetlpEngine(CEngine * const lpengine) {m_lpEngine = lpengine;};		
		LYNXINLINE CEngine*							GetlpEngine() const {return m_lpEngine;};

		LYNXINLINE void								SetlpRenObj(CRenderableObj* lpp) {m_lpRenObj = lpp;};		
		LYNXINLINE CRenderableObj*					GetlpRenObj() const {return m_lpRenObj;};

		//void										CreateFrame(DWORD num);
		//LYNXINLINE CArray<Animation::CAnimationFrame>& GetFrameArray(void) {return m_FrameArray;};
		//LYNXINLINE Animation::CAnimationFrame*		GetlpCurrentFrame(void) {return m_lpCurrentFrame;};
		//LYNXINLINE int								GetCurrentFrameID(void) {return m_lpCurrentFrame->m_ID;};
		//LYNXINLINE const char*						GetCurrentFrameName(void) {return m_lpCurrentFrame->m_Name;};

		LYNXINLINE const LYNXMATRIX&				GetTransformMatrix() const {return m_M; };	
		LYNXINLINE void								SetTransformMatrix(const LYNXMATRIX& m) {lynxMemCpy(&m_M, &m, sizeof(LYNXMATRIX)); };		

		LYNXINLINE const LYNXMATRIX&				GetLocalMatrix() {return m_LocalM; };		
		LYNXINLINE void								SetLocalMatrix(const LYNXMATRIX& m) {lynxMemCpy(&m_LocalM, &m, sizeof(LYNXMATRIX)); };		

		LYNXINLINE const LYNXMATRIX&				GetGlobalMatrix() {return m_GlobalM; };		
		LYNXINLINE void								SetGlobalMatrix(const LYNXMATRIX& m) {lynxMemCpy(&m_GlobalM, &m, sizeof(LYNXMATRIX)); };		

		void										UpdateTransform();
		void										ResetTransform();
		void										ResetOrientation();
		
		void										SetPosition(LPLYNXVECTOR3D lppos);
		void										SetPosition(LYNXREAL x, LYNXREAL y, LYNXREAL z);
		void										GetPosition(LPLYNXVECTOR3D lppos);
		LYNXINLINE void								GetDirection(LPLYNXVECTOR3D lppos) {LYNX_VECTOR3D((*lppos), m_lpDirection->s.x, m_lpDirection->s.y, m_lpDirection->s.z);};
		LYNXINLINE void								GetUpDirection(LPLYNXVECTOR3D lppos) {LYNX_VECTOR3D((*lppos), m_lpUpDir->s.x, m_lpUpDir->s.y, m_lpUpDir->s.z);	};
		LYNXINLINE void								GetRightDirection(LPLYNXVECTOR3D lppos) {LYNX_VECTOR3D((*lppos), m_lpRightDir->s.x, m_lpRightDir->s.y, m_lpRightDir->s.z);	};		
		void										Forward(LYNXREAL dist);
		void										SideMove(LYNXREAL dist);
		void										Up(LYNXREAL dist);
		void										Scale(LPLYNXVECTOR3D lpv, LYNXMATHORDER order);
		void										SetScale(LPLYNXVECTOR3D lpv);
		void										Translate(LPLYNXVECTOR3D lpv, LYNXMATHORDER order);
		void										Rotate(LYNXREAL f, LYNXAXISTYPE axis, LYNXMATHORDER order);
		void										RotateWithAxis(LPLYNXVECTOR3D lpaxis, LYNXREAL ang, LYNXMATHORDER order);
		void										Yaw(LYNXREAL deg, LYNXMATHORDER order);
		void										Roll(LYNXREAL deg, LYNXMATHORDER order);
		void										Pitch(LYNXREAL deg, LYNXMATHORDER order);
		void										MulMatrix(const LYNXMATRIX& m, LYNXMATHORDER order);		

		//LYNXINLINE LYNXBOOL							Play(LYNXREAL skip, LYNXPLAYMODE mode) {m_bPlayStop = m_lpObj3D->Play(m_lpObj3D, skip, mode); return  m_bPlayStop; };
		//LYNXINLINE LYNXBOOL							Play(LYNXREAL skip) {m_bPlayStop = m_lpObj3D->Play(m_lpObj3D, m_lpCurrentFrame->m_Skip*skip, m_lpCurrentFrame->m_PlayMode); return m_bPlayStop;};
		//LYNXINLINE void								Render() {m_lpObj3D->Render(m_lpObj3D); };		

		LYNXINLINE LYNXREAL							GetTime(void) {return m_Animation.m_Time; };
		LYNXINLINE void								SetTime(LYNXREAL t) {m_Animation.m_Time = t; };		
		//LYNXINLINE LYNXREAL							GetStartTime(void) {return m_StartTime; };
		//LYNXINLINE void								SetStartTime(LYNXREAL t) {m_StartTime = t; };
		//LYNXINLINE LYNXREAL							GetEndTime(void) const {return m_lpObj3D->EndTime;};		
		//LYNXINLINE void								SetEndTime(LYNXREAL t) {m_EndTime = t;};		
		//LYNXINLINE void								SetTimes(LYNXREAL st, LYNXREAL et, LYNXREAL t) {m_StartTime = st; m_EndTime = et; m_Time = t;};		

		LYNXINLINE CContainer*						GetlpParent() {return m_lpParent; };			
		LYNXINLINE DWORD							GetNumChildren(void) {return m_lpChildrenArray.size(); };
		LYNXINLINE CContainer*						GetlpChild(int i) {return m_lpChildrenArray[i]; };		

		virtual LYNXBOOL							vSetCurrentFrame(int id, LYNXBOOL bforcereset = FALSE);
		virtual LYNXBOOL							vSetCurrentFrame(char *name, LYNXBOOL bforcereset = FALSE);		
		
		virtual	LYNXBOOL							vDuplicate(const CContainer* const) {return LYNX_TRUE; }; //should be pure virtual function
		virtual	LYNXBOOL							vInstance(const CContainer* const ) {return LYNX_TRUE; }; //should be pure virtual function
		virtual void								vSetSize(LYNXREAL w, LYNXREAL h) {}; //should be pure virtual function
		virtual void								vGetSize(LYNXREAL *w, LYNXREAL *h) {}; //should be pure virtual function

		LYNXINLINE void								CreateMaterialArray(int num) {m_MaterialArray.resize(num); };
		LYNXINLINE void								SetMaterial(MATERIALTYPE i, MaterialSystem::CMaterial* lpm) {m_MaterialArray[i] = lpm; };
		LYNXINLINE MaterialSystem::CMaterial*		GetMaterial(MATERIALTYPE i) { return m_MaterialArray[i]; };
		void										SetCurrentMaterialType(MATERIALTYPE i);		
		LYNXINLINE MaterialSystem::CMaterial* const GetlpCurrentMaterial() {return m_lpCurrentMaterial; };
		LYNXINLINE void								SetlpCurrentMaterial(MaterialSystem::CMaterial* const lpm) {m_lpCurrentMaterial = lpm; };
		void										AddToCurrentMaterialRenderList();
		
		virtual LYNXBOOL							vLoad(const char *name, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL busevertexshader = LYNX_FALSE) = 0;
		virtual LYNXBOOL							vCreate() {return LYNX_TRUE; };
		virtual void								vRelease() = 0;

		virtual LYNXBOOL							vPlay(LYNXREAL skip, LYNXPLAYMODE mode) = 0;
		virtual LYNXBOOL							vPlay(LYNXREAL skip) = 0;		
		virtual void								vRender() = 0;		
	};
}

#endif