#ifndef __LINXENGINECLASSES_H__
#define __LINXENGINECLASSES_H__

#include <LynxConfig.h>
#include <LynxCore.h>

#ifdef __LYNXENGINECORE_LIB__
	#define	LYNXENGCORECLASS						
#elif __LYNXENGINECORE_DLL__
	#define	LYNXENGCORECLASS					LYNXEXPORT
#else
	#define LYNXENGCORECLASS					LYNXIMPORT
#endif

#ifdef __LYNXENGINE_LIB__
	#define	LYNXENGCLASS						
#elif __LYNXENGINE_DLL__
	#define	LYNXENGCLASS						LYNXEXPORT
#else
	#define LYNXENGCLASS						LYNXIMPORT
#endif

#ifdef __LYNXENGINE_LIB__
	#define	LYNXENGTEMPLATE						
#elif __LYNXENGINE_DLL__
	#define	LYNXENGTEMPLATE						LYNXEXPORT
#else
	#define	LYNXENGTEMPLATE						LYNXEXPORT
#endif

#ifdef __TOOL__
	//#define  PUBLIC								virtual public
	//#define  CONTAINER_CAST(t, c)				(dynamic_cast<t>(c))
	#define  PUBLIC								public
	#define  CONTAINER_CAST(t, c)				(static_cast<t>(c))
#else
	#define  PUBLIC								public
	#define  CONTAINER_CAST(t, c)				(static_cast<t>(c))
#endif

#ifdef _CPPRTTI
	#include <typeinfo.h>	
	#define LYNX_CLASSNAME(n)						(typeid(n).name())
	#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const char*	GetClassName()  {return (typeid(*this).name()); };	
#else
	#define LYNX_CLASSNAME(n)						(#n)
	#define LYNX_DECLARE_GETCLASSNAME(n)			virtual const char*	GetClassName()  {return (LYNX_CLASSNAME(n)); };	
#endif

#define LYNX_DECLARE_CLASS(TClass)\
	public:\
		LYNX_DECLARE_GETCLASSNAME(TClass)

#define FUNC_NAME(func)							static LYNXCHAR __FUNC_NAME__[] = _T(#func);

#include <LynxSDK/LynxMathWrap_Opt.h>
#include <LynxList.h>
#include <LynxArray.h>
#include <LynxTMap.h>
#include <LynxString.h>
#include <LynxCountedPtr.h>

namespace LynxEngine 
{	
    class CObj;		
		class CLogger;
		class CEvent;	
		class CResource;
			class CContainer;
				class CReferenceContainer;
				class CCameraContainer;
				class CSpriteContainer;
				class CSprite3DContainer;
				class CSprite3DClusterContainer;
				class CModelContainer;
				class CBoneContainer;
				class CMeshContainer;
				class CGeometryContainer;
				class CStaticMeshContainer;
				class CTerrainContainer;
				class CPEmitterContainer;
				class CVolumetricCloudContainer;
		class CSkyLightMap;
		class CGameObj;	
			class CObjProxy;
			class CRenderableObj;
				class CParticle;
		class CEngineCore;
			class CEngine;
		class CSystem;				
	class CParam;
	class CParamGroup;
	class CParamManager;	
	class CResourceManager;
	class CContainerManager;
	class CGameObjManager;
	class COutputDevice;

	typedef CCountedPtr<CResource>				CResourcePtr;
	typedef CCountedPtr<CContainer>				CContainerPtr;

	namespace StateMachine
	{		
		class CStateMachine;
		class CState;
		class CStateTranslation;
	}

	namespace Animation
	{
		class CAnimation;
		class CAnimationKey;
		class CAnimationKeys;
		class CAnimationSet;
	}

	namespace CollisionSystem
	{
		class CCollisionSystem;
		class CCollisionHierarchy;
		class CCollisionObj;
			class CAABB;
	}

	namespace GameConsoleSystem
	{
		class CGameConsoleSystem;
	}

	namespace InputSystem
	{
		class CInputSystem;
	}

	namespace SoundSystem
	{
		class CSoundSystem;
	}

	namespace GUISystem
	{
		class CGUISystem;
	}

	namespace GraphicsSystem
	{
		class CGraphicsSystem;
		class CRenderer;	
			class CD3D9Renderer;
			class CD3D10Renderer;

		class CDepthState;
			class CD3D9DepthState;
			class CD3D10DepthState;
		class CStencilState;
			class CD3D9StencilState;
			class CD3D10StencilState;
		class CBlendState;
			class CD3D9BlendState;
			class CD3D10BlendState;
		class CRasterizerState;
			class CD3D9RasterizerState;
			class CD3D10RasterizerState;
		class CSamplerState;
			class CD3D9SamplerState;
			class CD3D10SamplerState;

		class CBuffer;
		class CTexture;
			class CD3D9Texture;
			class CD3D10Texture;
		class CVertexArray;
		class CIndexArray;
		class CVertexLayout;
			class CD3D9VertexLayout;
			class CD3D10VertexLayout;
		class Shader;
			class CVertexShader;
				class CD3D9VertexShader;
				class CD3D10VertexShader;
			class CPixelShader;
				class CD3D9PixelShader;
				class CD3D10PixelShader;
		class CMap;
			class COffscreen;
			class CDepthMap;
			class CGlowMap;
			class CVelocityMap;
			class CGlowMap;
			class CVelocityMap;
			class CShadowMap;
				class CCubeShadowMap;		
						
		typedef CCountedPtr<CVertexShader>			CVertexShaderPtr;
		typedef CCountedPtr<CPixelShader>			CPixelShaderPtr;
		typedef CCountedPtr<CVertexArray>			CVertexArrayPtr;
		typedef CCountedPtr<CIndexArray>			CIndexArrayPtr;
		typedef CCountedPtr<CBuffer>				CBufferPtr;
		typedef CCountedPtr<CTexture>				CTexturePtr;
		typedef CCountedPtr<CDepthState>			CDepthStatePtr;
		typedef CCountedPtr<CStencilState>			CStencilStatePtr;
		typedef CCountedPtr<CBlendState>			CBlendStatePtr;
		typedef CCountedPtr<CRasterizerState>		CRasterizerStatePtr;
		typedef CCountedPtr<CSamplerState>			CSamplerStatePtr;
		typedef CCountedPtr<CVertexLayout>			CVertexLayoutPtr;
	}	

	namespace GameFramework
	{
		class CKernel;
	}

	namespace ScriptSystem
	{
		class CScriptSystem;
		class CScript;
		class CVMachine;
		class CScriptClass;
	}

	namespace MaterialSystem
	{
		class CMaterialSystem;
		class CMaterial;
		class CTechnique;
		class CPass;
		class CTextureAnimation;

		typedef CCountedPtr<CMaterial>				CMaterialPtr;
		typedef CCountedPtr<CTextureAnimation>		CTextureAnimationPtr;
	}	

	namespace SceneSystem
	{
		class CSceneSystem;
		class CEnviron;
		class CScene;
		class CSurface;
		class CStaticObj;
			class CTerrain;
			class CSky;
			class CCloud;
		class CDynamicObj;
			class CLight;
				class CAmbientLight;
				class CPointLight;
				class CDirectionalLight;
				class CSpotLight;
			class CCamera;
	}

	namespace PhysicsSystem
	{
		class CPhysicsSystem;
		class CPhysicsWorld;
		class CPhysicsSpace;
		class CPhysicsObj;
		class CPhysicsGeometry;
		class CPhysicsJoint;
	}	

	namespace NetworkSystem
	{
		class CNetworkSystem;
	}

	typedef enum {
		TEXTURE = 0,
		TEXTURE_ANIMATION,
		DEPTH_STATE,
		STENCIL_STATE,
		BLEND_STATE,
		RASTERIZER_STATE,
		SAMPLER_STATE,
		VERTEX_SHADER,
		GEOMETRY_SHADER,
		PIXEL_SHADER,
		BLEND_SHADER,
		TEXTURE_SHADER,			
		VERTEX_LAYOUT,
		VERTEX_ARRAY,
		INDEX_ARRAY,
		STREAMOUT_BUFFER,
		BUFFER,
		MATERIAL,
		CONTAINER,
		SCRIPT,
		NUM_RESOURCETYPE,
	}RESOURCETYPE;

	typedef enum{
		PM_ONCE = 0,
		PM_LOOP,
		PM_BOUNCE,
		NUM_PLAYMODE,
	}PLAYMODE;

	typedef enum {
		TT_OPAQUE = 0,
		TT_TRANSPARENT,
		TT_TRANSLUCENT,
		NUM_TRANSPARENTTYPE,
	}TRANSPARENTTYPE;

	LYNXENGCORECLASS CString				GetEnumStr(PLAYMODE e);
	LYNXENGCORECLASS CString				GetEnumStr(TRANSPARENTTYPE e);		
	LYNXENGCORECLASS CString				GetEnumStr(RESOURCETYPE e);		
}

extern LYNXENGCORECLASS LynxEngine::CLogger*	glpLogger;

#endif