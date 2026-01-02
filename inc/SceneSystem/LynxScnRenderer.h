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

#ifndef __LYNXSCNRENDERER_H__
#define __LYNXSCNRENDERER_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

#define RESET_ALL_SCENE_COUNTERS			RESET_COUNTER(m_Profile.NumVisObj)\
											RESET_COUNTER(m_Profile.NumVisContainer)\
											RESET_COUNTER(m_Profile.NumRenderedContainer)\
											RESET_COUNTER(m_Profile.NumVisLight)\
											RESET_COUNTER(m_Profile.NumRenderedLight)
namespace LynxEngine 
{	
	namespace SceneSystem 
	{		
		class LYNXENGCLASS CRenderer : public CObj
		{	
			LYNX_DECLARE_CLASS(CRenderer);
		public:			
			typedef enum {
				CLT_OPAQUE = 0,
				CLT_TRANSLUCENT,
				CLT_DISTORTION,
				NUM_CONTAINERLISTTYPE,
			}CONTAINERLISTTYPE;		

			typedef enum {
				SORT_NONE = 0,
				FRONT_TO_BACK,
				BACK_TO_FRONT,
			}SORTTYPE;

			typedef enum {					
				SORT_BY_DISTANCE = 0,
				SORT_BY_MATERIAL,
				SORT_BY_DEPTH_LAYER,
			}SORTMODE;

			typedef enum {								
				RENDER_NORMAL = 0,
				RENDER_DEPTHCOMPLEX,
				RENDER_LIGHTINGONLY,
				RENDER_SSAOONLY,
				RENDER_SIMPLE,				
				RENDER_SIMPLELIGHTING,
				RENDER_WIREFRAME,
				RENDER_ONELIGHT,
				NUM_RENDERMODE,
			}RENDERMODE;

			typedef struct {			
				//DECLAR_PARAM_ENUM(LynxEngine::SceneSystem::CRenderer, RENDERMODE, RenderMode)
				RENDERMODE							RenderMode;
				DECLAR_PARAM_URGBA(ClearColor)
				DECLAR_PARAM_COUNTEDPTR(GraphicsSystem::CTexture, BackgroundTexture)
				DECLAR_PARAM_BOOL(bUseBackgroundTexture)
				DECLAR_PARAM_STRING(BackgroundTextureName)
				DECLAR_PARAM_BOOL(bDepthDownSampling)
				DECLAR_PARAM_BOOL(bSSAOPass)
				DECLAR_PARAM_FLOAT(SSAOSamplingKernel)
				DECLAR_PARAM_BOOL(bSSAOBlur)
				DECLAR_PARAM_FLOAT(SSAOBlurKernel)
				DECLAR_PARAM_FLOAT(SSAOBlurScale)				
				DECLAR_PARAM_FLOAT(SSAOBlurSharpness)
				DECLAR_PARAM_FLOAT(SSAOThreshold)
				DECLAR_PARAM_BOOL(bGlowPass)
				DECLAR_PARAM_INT(NumSplittedShadowMap)
			}RENDEROPTIONS,* LPRENDEROPTIONS;
			
			typedef struct {			
				DWORD									NumVisObj;
				DWORD									NumVisContainer;
				DWORD									NumRenderedContainer;
				DWORD									NumVisLight;
				DWORD									NumRenderedLight;
			}PROFILE;

			#define MAX_REFLECTION_PLANE				1
			#define MAX_REFRACTION_PLANE				1

			typedef struct {				
				LYNXPLANE		Plane;		
				CContainer*		lpContainer;
			}RNRDATA;

			typedef enum {
				LIGHTINGPASS_TARGET = 0,
				TRANSLUCENTLIGHTINGPASS_TARGET,
				SSAOMAP,
				DEPTHMAP,
				GLOWMAP,
				VELOCITYMAP,
				REFLECTIONMAP,
				REFRACTIONMAP,
				RNRGLOWMAP,				
				SHARED_SHADOWMAP,
				SHARED_CUBESHADOWMAP,
				TONEMAP,
				LUMINANCEMAP,
				NUM_MAPTYPE,
			}MAPTYPE;	

			typedef enum {
				MOTION_BLUR = 0,
				GET_LUMINANCE,
				COMPUTE_LUMINANCE,
				AVERAGE_LUMINANCE,
				TONE_MAPPING,
				ADAPT_LUMINANCE,
				DOF,
				BRIGHTPASS,
				SSAO,								
				NUM_MATERIALTYPE,
			}MATERIALTYPE;

			RENDEROPTIONS								m_RenderingOptions;		
			LYNXUVPOINT									m_BackgroundUVOffset[4];
			GraphicsSystem::CScreenQuad*				m_lpBackgroundQuad;
			PROFILE										m_Profile;
		protected:			
			CScene*										m_lpScene;			

			GraphicsSystem::CBlendStatePtr				m_DepthComplexBlendState;			

			SORTMODE									m_SortMode;
			CList<CContainer*>							m_DepthLayerContainerList[CRenderableObj::NUM_DEPTHLAYERTYPE];
			CList<CContainer*>							m_VisOpaqueContainerList;
			CList<CContainer*>							m_NonSortingVisOpaqueContainerList;
			CList<CContainer*>							m_VisTranslucentContainerList;						
			CList<CContainer*>							m_VisDistortionContainerList;									

			GraphicsSystem::CMap*						m_lpCurrentLightingPassTarget;
			GraphicsSystem::CMap*						m_lpCurrentTranslucentLightingPassTarget;
			GraphicsSystem::CBuffer*					m_lpTargetBuffer;
			GraphicsSystem::CMap*						m_lpCurrentGlowMap;
			
			RNRDATA*									m_lpCurrentRNRData;
			CList<RNRDATA>								m_RNRData;						

			CArray<Math::CReal>							m_ShadowMapSplittedDepthArray;

			CList<GraphicsSystem::CPostProcessing*>		m_PostFXList;							
			CArray<GraphicsSystem::CMap*>				m_MapArray;
			CArray<MaterialSystem::CMaterialPtr>		m_MaterialArray;
		public:			
			CRenderer(CScene* s);
			virtual ~CRenderer();
			
			LYNXINLINE CScene*							GetlpScene() {return m_lpScene; };
			LYNXINLINE PROFILE&							GetProfile() {return m_Profile; };			

			virtual LYNXBOOL							vCreate();
			virtual LYNXBOOL							vResize(int x, int y);			

			LYNXFORCEINLINE GraphicsSystem::CMap*		GetlpMap(MAPTYPE t) {return m_MapArray[t]; };
			LYNXFORCEINLINE MaterialSystem::CMaterialPtr& GetlpMaterial(MATERIALTYPE t) {return m_MaterialArray[t]; };

			// Parallel Split Shadow Map 
			void										SetNumSplittedShadowMap(int num);	
			LYNXINLINE CArray<Math::CReal>&				GetShadowMapSplittedDepthArray() {return m_ShadowMapSplittedDepthArray; };
			LYNXINLINE const Math::CReal&				GetShadowMapSplittedDepth(int i) const {return m_ShadowMapSplittedDepthArray[i]; };
			void										AdjustCameraPlanes(CCameraContainer& cam);
				
			// Passes
			virtual void								vLightingPass(CCameraContainer& camera);
			void										ZPass(CCameraContainer& camera);
			void										ShadowmapPass(CLight* lplit, CCameraContainer& camera);
			void										SSAOPass(CCameraContainer& camera);
			void										SSAO_SSGI_Pass(CCameraContainer& camera);
			void										OpaqueLightingPass(CCameraContainer& camera);		
			void										TranslucentLightingPass(CCameraContainer& camera);
			void										DistortionPass(CCameraContainer& camera);
			
			void										ReflectionPass(CCameraContainer& camera);
			void										RefractionPass(CCameraContainer& camera);
			void										BloomingPass(CCameraContainer& camera);
			void										MotionBlurPass(CCameraContainer& camera);			
			void										LuminancePass(CCameraContainer& camera);
			void										ToneMappingPass(CCameraContainer& camera);
			void										DOFPass(CCameraContainer& camera);
			void										PostProcessingPass(CCameraContainer& camera);
			void										SimplePass(CCameraContainer& camera);
			void										GlowPass(CCameraContainer& camera);
			void										SimpleLightingPass(CCameraContainer& camera);
			void										OneLightingPass(CCameraContainer& camera);

			void										RenderModeFilter();	
			void										Render(CCameraContainer& camera);
			void										NormalRender(CCameraContainer& camera);
			void										DepthComplexRender(CCameraContainer& camera);
			void										SSAORender(CCameraContainer& camera);			
			void										SimpleRender(CCameraContainer& camera);	
			void										GlowRender(CCameraContainer& camera);
			void										SimpleLightingRender(CCameraContainer& camera);			
			void										OneLightRender(CCameraContainer& camera);			

			// Visibility	
			void										ClearVisOpaqueContainerList();
			void										ClearVisTranslucentContainerList();
			void										AddContainerToVisList(CContainer* con);
			void										AddObjToVisList(CRenderableObj* obj);	
			void										SortRenderList(CList<CContainer*>& list, SORTTYPE type, SORTMODE mode, CVector3* lpv);
			void										SortVisList(CONTAINERLISTTYPE listtype, SORTTYPE type, SORTMODE mode, CVector3* lpv = NULL);			
			void										SetupDepthLayerContainerList();
			LYNXFORCEINLINE void						SetSortMode(SORTMODE m) {m_SortMode = m; };
			LYNXFORCEINLINE SORTMODE					GetSortMode() {return m_SortMode; };

			LYNXFORCEINLINE GraphicsSystem::CMap*		GetlpCurrentLightingPassTarget() {return m_lpCurrentLightingPassTarget; };
			LYNXFORCEINLINE void						SetlpCurrentLightingPassTarget(GraphicsSystem::CMap* map) {m_lpCurrentLightingPassTarget = map; };

			LYNXFORCEINLINE GraphicsSystem::CBuffer*	GetlpTargetBuffer() {return m_lpTargetBuffer; };
			LYNXFORCEINLINE void						SetlpTargetBuffer(GraphicsSystem::CBuffer* t) {m_lpTargetBuffer = t; };
		};
	}
}
#endif