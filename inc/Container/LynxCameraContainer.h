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

#ifndef __LYNXCAMERACONTAINER_H__
#define __LYNXCAMERACONTAINER_H__

#include <LynxMathWrap.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class CEngine;	
	class LYNXENGCLASS CCameraContainer : public CContainer
	{
	public:
		DECLAR_PARAM_FLOAT(m_Exposure)	
		DECLAR_PARAM_FLOAT(m_ExposureSpeed)
		DECLAR_PARAM_BOOL(m_bBlooming)	
		DECLAR_PARAM_BOOL(m_bDOF)		
		DECLAR_PARAM_NATIVE_ARRAY(Math::CReal, m_FocalRange, 4)
		DECLAR_PARAM_BOOL(m_bMotionBlur)
		DECLAR_PARAM_INT(m_MotionBlurKernel)
		DECLAR_PARAM_FLOAT(m_MotionBlurScale)
		DECLAR_PARAM_BOOL(m_bCameraMovingBlur)
		DECLAR_PARAM_INT(m_CameraMovingBlurKernel)
		DECLAR_PARAM_FLOAT(m_CameraMovingBlurScale)

		DECLAR_PARAM_INT(m_DOFBlurKernel)
		DECLAR_PARAM_FLOAT(m_DOFBlurScale)
		DECLAR_PARAM_FLOAT(m_DOFSamplingKernel)
		DECLAR_PARAM_INT(m_BloomingBlurKernel)
		DECLAR_PARAM_FLOAT(m_BloomingBlurScale)

		DECLAR_PARAM_FLOAT(m_Near)
		DECLAR_PARAM_FLOAT(m_Far)

		DECLAR_PARAM_FLOAT(m_SplitSchemeWeight)

		DECLAR_PARAM_RANGE_FLOAT(m_BrightPassThreshold)

		LYNXRECT								m_ViewportRect;
	protected:				
		WORD									m_FrustumFlag;				
		LYNXVECTOR3D							m_LookPoint, m_UpDir;
		Math::CMatrix4							m_ViewMatrix;
		Math::CMatrix4							m_ProjectionMatrix;
		LYNXBOOL								m_bMirror;
		Math::CMatrix4							m_MirrorMatrix;
		LYNXFRUSTUM								m_Frustum;		
		DECLAR_PARAM_FLOAT(m_XFOV)
		DECLAR_PARAM_FLOAT(m_YFOV)
		LYNXREAL								m_Sin_XFOV[2], m_Cos_XFOV[2];	
		LYNXREAL								m_Sin_YFOV[2], m_Cos_YFOV[2];	
		Math::CVector3							m_FarCornerDir[4];		
	public:
		LYNX_DECLARE_CLASS(CCameraContainer);		
		CCameraContainer(CEngine *lpengine);
		~CCameraContainer();				

		DECLARE_CLASSSCRIPT
		LYNXBOOL								vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
		LYNXINLINE void							_LookAt(float x, float y, float z) {LYNXVECTOR3D V; V.x = x; V.y = y; V.z = z; LookAt(&V); };

		LYNXINLINE void							SetMirror(LYNXBOOL b) {m_bMirror = b; };
		LYNXINLINE LYNXBOOL						IsMirror() const {return m_bMirror; };

		LYNXINLINE const LYNXRECT&				GetViewportRect() const {return m_ViewportRect; };
		LYNXINLINE void							SetupViewportRect(const long x1, const long y1, const long x2, const long y2) {m_ViewportRect.x1 = x1; m_ViewportRect.y1 = y1; m_ViewportRect.x2 = x2; m_ViewportRect.y2 = y2;};
		void									SetViewportRect();

		virtual LYNXBOOL						vCreate(const CCreationDesc* desc);
		//virtual LYNXBOOL						vCreate(const char *name, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL busevertexshader = LYNX_FALSE);
		//LYNXINLINE virtual LYNXBOOL				vPlay(LYNXREAL skip, LYNXPLAYMODE mode) {m_bPlayStop = lynxPlayCamera((LPLYNXCAMERA)this, skip, mode); return  m_bPlayStop; };
		//LYNXINLINE virtual LYNXBOOL				vPlay(LYNXREAL skip) {m_bPlayStop = lynxPlayCamera((LPLYNXCAMERA)this, m_lpCurrentFrame->m_Skip*skip, m_lpCurrentFrame->m_PlayMode); return m_bPlayStop;};				
		virtual LYNXBOOL						vPlay(CReal step);
		virtual void							vRender(CCameraContainer* cam = NULL) {};		

		LYNXINLINE void							SetFOV(LYNXREAL xfov, LYNXREAL yfov) {m_XFOV = xfov; m_YFOV = yfov; };
		LYNXINLINE LYNXREAL						GetXFOV() {return m_XFOV; };
		LYNXINLINE LYNXREAL						GetYFOV() {return m_YFOV; };
		LYNXINLINE Math::CVector3&				GetFarCornerDir(int i) {return m_FarCornerDir[i]; };
		void									CalculateFarCornerDir(Math::CReal ratio);
		void									CalculateFrustum();
		void									CalculateFrustumAABB(CollisionSystem::CAABB& aabb, const Math::CReal& nearp, const Math::CReal& farp);
		LYNXINLINE LYNXFRUSTUM&					GetFrustum() {return m_Frustum; };				
		void									LookAt(LPLYNXVECTOR3D const lpv);
		LYNXINLINE void							GetLookAtPoint(Math::CVector3& p) {p = m_LookPoint; };		
		LYNXINLINE const  Math::CMatrix4&		GetViewMatrix() const {return m_ViewMatrix; };		
		LYNXINLINE void							SetViewMatrix(Math::CMatrix4&  m) { m_ViewMatrix = m; };		
		LYNXINLINE const Math::CMatrix4&		GetProjectionMatrix() const {return m_ProjectionMatrix; };		
		LYNXINLINE void							SetProjectionMatrix(Math::CMatrix4&  m) {m_ProjectionMatrix = m; };		
		LYNXINLINE Math::CMatrix4&				GetMirrorMatrix() {return m_MirrorMatrix; };		
		LYNXINLINE void							SetMirrorMatrix(Math::CMatrix4&  m) {m_MirrorMatrix = m; };		
		void									CalculateSplitPositions(CArray<Math::CReal>& pdists, int num);
				
		void                                    UpdateProjectionMatrix(float ratio = -1.0f);		
		void									UpdateOrthoProjectionMatrix();
		void									UpdateViewMatrix();
	};
}

#endif