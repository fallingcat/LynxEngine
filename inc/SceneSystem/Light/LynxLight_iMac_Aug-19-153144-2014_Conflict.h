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

#ifndef __LYNXLIGHT_H__
#define __LYNXLIGHT_H__

#include <SceneSystem/DynObj/LynxDynObj.h>
#include <GraphicsSystem/LynxShadowMap.h>
#include <Container/LynxCameraContainer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CLight : public CDynamicObj
		{	
			LYNX_DECLARE_CLASS(CLight);
		public:
			typedef enum{
				AMBIENT,
				DIRECTIONAL,	
				POINT,						
				SPOT,				
				NUM_TYPE,
			}TYPE;			
			
			DECLAR_PARAM_BOOL(m_bCastShadow)
			DECLAR_PARAM_FLOAT(m_Intensity)
			DECLAR_PARAM_FLOAT(m_Radius)	
			DECLAR_PARAM_URGB(m_Color)
			DECLAR_PARAM_FLOAT(m_Deg)	
			DECLAR_PARAM_FLOAT(m_FadeOutDeg)
			CCameraContainer*			m_Camera;
			Math::CMatrix4				m_LightViewProj;
            Math::CMatrix4              m_LightProj;
			DECLAR_PARAM_FLOAT(m_DepthBias)
			GraphicsSystem::CShadowMap::SHADOWFILTERTYPE m_ShadowFilterType;
		protected:			
			CLight::TYPE				m_LightType;								
			static Math::CMatrix4		m_FaceViewM[6];
			LYNXBOOL					m_bDynamic;			
			CList<CRenderableObj*>		m_VisStaticOpaqueObjList;
			CList<CRenderableObj*>		m_VisDynamicOpaqueObjList;			

			LYNXBOOL					m_bShadowmapDirty;
			GraphicsSystem::CShadowMap*	m_lpShadowMap;	
			DECLAR_PARAM_INT(m_ShadowmapWidth)
			DECLAR_PARAM_INT(m_ShadowmapHeight)	
			int							m_ActualShadowMapWidth;
			int							m_ActualShadowMapHeight;			
			DECLAR_PARAM_BOOL(m_bProjectedLightMap)
			GraphicsSystem::CTexturePtr	m_lpProjectedLightMap;
		public:						
			CLight(CScene* const lpscene);			
			virtual ~CLight();		

			virtual void				vRelease();

			LYNXINLINE CLight::TYPE		GetLightType() {return m_LightType; };		
			LYNXINLINE LYNXBOOL			IsShadowmapDirty() const {return m_bShadowmapDirty; };
			LYNXINLINE void				ShadowmapDirty(LYNXBOOL b) {m_bShadowmapDirty = b; };
			LYNXINLINE GraphicsSystem::CShadowMap& GetShadowMap() {return *m_lpShadowMap; };
			LYNXINLINE int				GetShadowmapWidth() const {return m_ShadowmapWidth; };
			LYNXINLINE int				GetShadowmapHeight() const {return m_ShadowmapHeight; };
			virtual LYNXBOOL			vCreate(int w = 512, int h = 512);	

			DECLARE_CLASSSCRIPT
			virtual LYNXBOOL			vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			virtual  LYNXBOOL			vRegisterScriptClassVar(ScriptSystem::CScript* const lps);

			void						CalculateViewMatrix();
			void						CalculateViewMatrix(int face);
			static CMatrix4				BuildCropMatrix(const CVector3 &min, const CVector3 &max);
			static CollisionSystem::CAABB CreateScreenSpaceAABB(CollisionSystem::CAABB &bb, const CMatrix4& vpm);
			CMatrix4					CalculateCropMatrix(CollisionSystem::CAABB& aabb);
			void						ClearVisList();
			void						AddToVisStaticOpaqueObjList(CRenderableObj* o) {m_VisStaticOpaqueObjList.push_back(o); };
			void						AddToVisDynamicOpaqueObjList(CRenderableObj* o) {m_VisDynamicOpaqueObjList.push_back(o); };
			virtual void				vComputeVisObjList() {};		
			virtual void				vGenerateShadowMap(CCameraContainer* cam) {};
			virtual void				vGenerateShadowTexture() {};
			virtual void				vSetShaderParam(CCameraContainer* cam, CContainer* lpc) {};
			virtual void				vDeferredLighting(CCameraContainer* cam) {};			
			void						MaskLightedArea(CCameraContainer* cam);
			LYNXINLINE CCameraContainer& GetCamera() {return *m_Camera;};				

			LYNXINLINE LYNXBOOL			IsProjectedLightMapUsed() {return m_bProjectedLightMap; };
			LYNXINLINE GraphicsSystem::CTexturePtr& GetProjectedLightMap() {return m_lpProjectedLightMap; };
		};		
	}
	CString	GetEnumStr(SceneSystem::CLight::TYPE e);
}

#endif