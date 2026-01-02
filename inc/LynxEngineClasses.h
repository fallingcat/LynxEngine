#ifndef __LYNXENGINECLASSES_H__
#define __LYNXENGINECLASSES_H__

#include <LynxConfig.h>
#include <LynxType.h>
#include <LynxCore.h>
#include <LynxIO.h>
#include <functional>
#include <LynxList.h>
#include <LynxArray.h>
#include <LynxStack.h>
#include <LynxTMap.h>
#include <LynxCryptic.h>
#include <LynxBitSet.h>
#include <LynxSet.h>
#include <LynxWeakPtr.h>
#include <LynxAlgorithm.h>
#include <LynxMathWrap.h>
#include <LynxCountedPtr.h>
#include <LynxEngineDefines.h>
#include <json/json.h>
#include <tinyxml2.h>

namespace LynxEngine 
{		
	class CCreationDesc;
	class CMemAllocator;
	class CDebugMemAllocator;
	class CSmallObjAllocator;
	class CReleaseMemAllocator;
	class CTextureFontString;
    class CTrueTypeFontString;
    class CFontLabel;
    class CIniReader;
	class CUserProfile;
		class CGameProfile;
		class CPlayerProfile;
		class CReadOnlyProfile;
	class CObj;		
		class CLogger;
		class CEvent;
        class CStaticText;
		class CResource;
            class CFont;
                class CTextureFont;
                class CTrueTypeFont;
			class CContainer;
				class CReferenceContainer;
				class CCameraContainer;
				class CSpriteContainer;
				class CSpriteClusterContainer;
				class CSprite3DContainer;
				class CSprite3DClusterContainer;
				class CVoxelClusterContainer;	
				class CModelContainer;
				class CBoneContainer;
				class CMeshContainer;
				class CGeometryContainer;
				class CTerrainContainer;
				class CPEmitterContainer;
				class CVolumetricCloudContainer;
		class CSkyLightMap;
		class CGameObj;	
			class CObjProxy;
			class CRenderableObj;
				class CParticle;
		class CEngine;
		class CSystem;				
		class CVideo;
	class CParam;
	class CParamGroup;
	class CParamManager;
	class CResourceManager;
	class CContainerManager;
	class CGameObjManager;
	class COutputDevice;
		class CFileOutputDevice;
		class CwxTextOutputDevice;
		class CHTMLOutputDevice;
		class CScreenOutputDevice;
	class CStream;
		class CMemoryStream;
		class CFileStream;
		class CZipStream;
		class CURLStream;	

	typedef CCountedPtr<CResource>				CResourcePtr;
	typedef CCountedPtr<CContainer>				CContainerPtr;
	typedef CCountedPtr<CObj>					CObjPtr;	
	typedef CCountedPtr<CFont>					CFontPtr;

	namespace StateMachine
	{		
		class CStateMachine;
		class CState;
		class CStateTranslation;

		typedef CCountedPtr<CStateMachine>		CStateMachinePtr;
	}
	namespace Logic
	{		
		class CTrigger;
	}

	namespace Animation
	{
		class CAnimation;
		class CAnimationKey;
		class CAnimationKeys;
		class CAnimationSet;
		class CVoxelAnimation;

		typedef CCountedPtr<CAnimationSet>		CAnimationSetPtr;
		typedef CCountedPtr<CAnimationKeys>		CAnimationKeysPtr;
	}

	namespace CollisionSystem
	{
		class CCollisionSystem;
		class CCollisionHierarchy;
		class CCollisionObj;
			class CAABB;
			class CSphere;
		class CBtCollisionObj;
			class CBtOBB;
			class CBtCapsule;
			class CBtSphere;
			class CBtPlane;
	}

	namespace GameConsoleSystem
	{
		class CGameConsoleSystem;
	}

	namespace FileSystem
	{
		class CFileSystem;
		class CPackage;
		class CUpdate;
	}	

	namespace SoundSystem
	{
		class CSoundSystem;
		class CSound;
			class C3DSound;
		class CMusic;
		class CListener;

		typedef CCountedPtr<CMusic>				CMusicPtr;		
		typedef CCountedPtr<CSound>				CSoundPtr;		
		typedef CCountedPtr<C3DSound>			C3DSoundPtr;		
	}

	namespace GUISystem
	{
		class CGUISystem;
		class CScrollGroup;
		class CControl;
			class CDummyControl;
			class CText;
			class CImage;
			class CButton;
				class CStateButton;
			class CSlider;
			class CContext;
			class CTab;
			class CWindow;
				class CMessageBox;
				class CDialog;
			class CPEmitter;
		class CHitMask;

		// enums -------------------------------
		typedef enum {
			CT_DUMMY = -1,
			CT_TEXT = 0,
			CT_IMAGE,
			CT_SPRITE,
			CT_BUTTON,
			CT_STATEBUTTON,
			CT_SLIDER,
			CT_CHECK,
			CT_TAB,
			CT_WINDOW,
			CT_MESSAGE_BOX,
			CT_DIALOG,			
			NUM_CONTROLTYPE,
		}CONTROLTYPE;
	}

	namespace GraphicsSystem
	{
		class CGraphicsSystem;
		class CRenderer;	
			class CD3D9Renderer;
			class CD3D11Renderer;
		class CShaderResource;
			class CD3D9ShaderResource;
			class CD3D119ShaderResource;
		class CShaderConstant;
			class CD3D9ShaderConstant;
			class CD3D11ShaderConstant;
		class CShaderConstantGroup;
			class CD3D9ShaderConstantGroup;
			class CD3D11ShaderConstantGroup;
		class CDepthStencilState;
			class CD3D9DepthStencilState;
			class CD3D11DepthStencilState;
		class CBlendState;
			class CD3D9BlendState;
			class CD3D11BlendState;
		class CRasterizerState;
			class CD3D9RasterizerState;
			class CD3D11RasterizerState;
		class CSamplerState;
			class CD3D9SamplerState;
			class CD3D11SamplerState;
		class CQuery;
		class CBuffer;
		class CTexture;
			class CD3D9Texture;
			class CD3D11Texture;
		class CVertexArray;
		class CIndexArray;
		class CVertexLayout;
			class CD3D9VertexLayout;
			class CD3D11VertexLayout;
		class CShaderClass;
		class CShaderGroup;
		class CShader;
			class CVertexShader;
				class CD3D9VertexShader;
				class CD3D11VertexShader;
			class CPixelShader;
				class CD3D9PixelShader;
				class CD3D11PixelShader;
		class CMap;
			class COffscreen;
			class CDepthMap;
			class CGlowMap;
			class CVelocityMap;
			class CGlowMap;
			class CVelocityMap;
			class CShadowMap;
				class CCubeShadowMap;	
			class CProjectedMap;
		class CScreenQuad;
		class CPostProcessing;
		class CBitmapContext;
		
		typedef CCountedPtr<CShader>			CShaderPtr;
		typedef CCountedPtr<CVertexShader>		CVertexShaderPtr;
		typedef CCountedPtr<CPixelShader>		CPixelShaderPtr;
		typedef CCountedPtr<CVertexArray>		CVertexArrayPtr;
		typedef CCountedPtr<CIndexArray>		CIndexArrayPtr;
		typedef CCountedPtr<CBuffer>			CBufferPtr;
		typedef CCountedPtr<CTexture>			CTexturePtr;		
		typedef CCountedPtr<CDepthStencilState>	CDepthStencilStatePtr;
		typedef CCountedPtr<CBlendState>		CBlendStatePtr;
		typedef CCountedPtr<CRasterizerState>	CRasterizerStatePtr;
		typedef CCountedPtr<CSamplerState>		CSamplerStatePtr;
		typedef CCountedPtr<CVertexLayout>		CVertexLayoutPtr;
		typedef CCountedPtr<CShaderClass>		CShaderClassPtr;		
		typedef CCountedPtr<CShaderGroup>		CShaderGroupPtr;		

		#define __FAKE_VOXEL_AO__				1

		typedef struct {
			float						x, y;	    
			float						u, v;	
			float						Index;
		}SCREENQUADVERTEX;

		typedef struct {	
		#if 0
			float						VertexIndex;
		#else
			LYNXUVPOINT					TexCoord;
			float						Instance;
		#endif
		}FONTVERTEX;

		typedef struct {			
			float						x, y, z;
			LYNXVECTOR4D				Color;
			LYNXUVPOINT					TexCoord;			
		}SPRITEVERTEX;

		typedef struct {			
			LYNXVECTOR3D				Pos;
			LYNXCOLORRGBA				Color;
			LYNXUVPOINT					TexCoord;			
		}SPRITE3DVERTEX;

		typedef struct {						
			LYNXUVPOINT					TexCoord;									
			float						Instance;						
		}SPRITE3DCLUSTERVERTEX;

		typedef struct {					
		#if __FAKE_VOXEL_AO__
			LYNXUVPOINT					TexCoord;
			float						FaceID;						
		#endif
			float						Instance;									
			LYNXVECTOR3D				Pos;			
			LYNXVECTOR3D				Normal;			
		}VOXELCLUSTERVERTEX;

		#define MAX_BONES_PER_VERTEX		4		

		typedef struct {
			LYNXVECTOR3D				Offset[MAX_BONES_PER_VERTEX];	    
			LYNXVECTOR3D				Tangent;
			LYNXVECTOR3D				Normal;
			LYNXVECTOR4D				Color;
			LYNXUVPOINT					TexCoord[1];
			short						BoneIndex[MAX_BONES_PER_VERTEX];
			LYNXREAL					Weight[MAX_BONES_PER_VERTEX];	
		}BLENDVERTEX;

		typedef struct {
			LYNXVECTOR3D				Pos;
			LYNXVECTOR3D				Tangent;
			LYNXVECTOR3D				Normal;	
			LYNXVECTOR4D				Color;
			LYNXUVPOINT					TexCoord[1];				
		}RIGIDVERTEX;					

		typedef struct {
			LYNXVECTOR3D				Pos;	
			LYNXVECTOR3D				Tangent;					
			LYNXVECTOR3D				Normal;		
			LYNXVECTOR4D				Color;
			LYNXUVPOINT					TexCoord[1]; 
		}STATICVERTEX;			

		typedef enum {
			VT_FIXED = 0,
			VT_VECTOR3,
			VT_SCREEN_QUAD,
			VT_SCREEN_DEPTH_QUAD,
			VT_SPRITE,
			VT_SPRITE3D,
			VT_SPRITE3D_CLUSTER,
			VT_VOXEL_CLUSTER,
			VT_STATIC,
			VT_RIGID,
			VT_BLEND,
			VT_FONT,
			VT_PROXY,
			NUM_VERTEXTYPE,
		}VERTEXTYPE;			

		typedef enum {
			BLT = 0,			
			ANIMATION_BLT,
			ONECHANNEL_BLT,
			FILL,	
			COLOR_BLT,
			ACC_BLT,			
			ALPHA_BLT,
			TINT_BLT,
			GRAY_BLT,
			ADD_TWO_CHANNEL,
			DOWN_SAMPLING,
			BOX_BLUR,
			BOX_BLUR_EDGE_CHECK,				
			BOX_MAX,
			GAUSSIN_BLUR,
			RADIAL_BLUR,
			HALO_RADIAL_BLUR,
			DIRECTIONAL_BLUR,
			VELOCITY_MAP_BLUR,
			BILATERAL_BLUR,				
			PROXY_RENDERING,
			LIGHT_PROXY_RENDERING,
			SCANLINE_NOISE,
			NUM_POSTPROCESSTYPE,
		}POSTPROCESSTYPE;

		typedef enum {
			OFFSCREEN = 0,
			UI_TRANSITION,
			//GLOWMAP,
			NUM_MAPTYPE,
		}MAPTYPE;					

		typedef enum {
			LOGO_TEXTURE = 0,
			DISTORTIONCHECKER_TEXTURE,
			BLACK_TEXTURE,
			WHITE_TEXTURE,
			DEPTHCOMPLEX_TEXTURE,
			DOT3_TEXTURE,								
			NOISE_TEXTURE,
			DETAIL_TEXTURE,
            CAMERA_CAPTURE_TEXTURE,
			NUM_SYSTEMTEXTURETYPE,
		}SYSTEMTEXTURETYPE;				
	}	

	namespace GameFramework
	{
		class CKernel;
		class CUIPage;
	}

	namespace ControlPanel
	{
		class CControlPanel;
		class CResourcePage;
	}

	namespace ScriptSystem
	{
		class CScriptSystem;
		class CScript;
		class CVMachine;
		class CScriptClass;

		typedef CCountedPtr<CScript>					CScriptPtr;
		//typedef CCountedPtr<CScriptClass>			CScriptClassPtr;		
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

		typedef enum {				
			DIFFUSEMAP = 0,
			NORMALMAP,
			DETAILMAP,
			SPECULARMAP,
			AMBIENTMAP,
			GLOWMAP,
			DISPLACEMENTMAP,
			DISTORTIONMAP,
			SHADOWMAP,
			PROJECTED_LIGHTMAP,
			LIGHTMAP,
			DEPTHMAP,
			REFLECTIONMAP,
			REFRACTIONMAP,
			TERRAINLAYER0,
			TERRAINLAYER1,
			TERRAINLAYER2,
			TERRAINLAYER3,
			NUM_MATERIALMAP,
		}MATERIALMAP;
	}	

	namespace SceneSystem
	{
		class CSceneSystem;
		class CInstanceGroup;
		class CAtmosphere;
		class CScene;
		class CRenderer;
			class CDeferredRenderer;
			class CLightPrePassRenderer;
		class CSurface;
		class CStaticObj;
			class CStaticMesh;
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

		// enums -------------------------------
		typedef enum {
			RT_SIMPLE = 0,
			RT_FORWARD,
			RT_DEFERRED,
			RT_PRELIGHTING,
			NUM_RENDERERTYPE,
		}RENDERERTYPE;			
	}
	//typedef CCountedPtr<SceneSystem::CDynamicObj>	CDynObjPtr;
	//typedef CWeakPtr<SceneSystem::CDynamicObj>		CDynObjWeakPtr;

	namespace PhysicsSystem
	{
		class CPhysicsSystem;
		class CPhysicsWorld;		
		class CPhysicsObj;		
		class CPhysicsJoint;
		class CPhysicsSpace;
		class CPhysicsShape;
		class CRagdoll;
		class CBoxShapeDesc;
		class CSphereShapeDesc;
		class CPlaneShapeDesc;
		class CCapsuleShapeDesc;
		class CHullShapeDesc;
		class CTriMeshShapeDesc;
	}	

	namespace InputSystem
	{
		class CInputSystem;
		class CInputDevice;
			class CKeyboard;
			class CMouse;
			class CInputRecord;
			class CGamePad;
			class CTouch;
            class CAccelerometer;
            class CGyrometer;
			class CTeslameter;			
		class CUserInputTranslator;
		class CInput;

		// enums -------------------------------
		typedef enum {
			KEYBOARD = 0,
			MOUSE,
			GAMEPAD,
			TOUCH,
			ACCELEROMETER,
			GYROMETER,
			MOTION_INPUT,
			INPUT_RECORD,
			NUM_INPUTDEVICETYPE,
		}INPUTDEVICETYPE;
	}

	namespace SocialSystem
	{
		class CFacebook;
		class CGameCenter;
		class CTwitter;				
	}

	namespace NetworkSystem
	{
		class CNetworkSystem;
		class CHttpRequest;
		class CHttpFormDataRequest;
		class CNetPlayer;
		class CNetClient;
		class CNetGame;
		namespace curl 
		{
			class CHttpRequest;
			class CHttpFormDataRequest;
		}
		namespace Photon
		{
			class CNetClient;
			class CNetListener;
		}
	}
    
    namespace GameAnalyticsSystem
    {
        class CGameAnaytics;
    }

	namespace PerfSystem
	{
		class CPerfSystem;
		class CCounter;
		class CTimer;
	}

	namespace AdSystem
	{
		class CAdSystem;
		class CAd;		

		typedef enum {
			AT_IAD = 0,
			AT_ADMOB,
			NUM_ADTYPE,
		}ADTYPE;
	}

	namespace GameAnalyticsSystem
	{
		typedef enum {
			PS_START = 1,
			PS_COMPLETE,
			PS_FAIL,
		}PROGRESSSTATUS;

		typedef enum {
			PFT_SOURCE = 1,
			RFT_SINK = 2,
		} RESOURCEFLOWTYPE;

		typedef enum {
			AE_RES_CURRENCIES = 0, // configureAvailableResourceCurrencies
			AE_RES_ITEMTYPES,	   // configureAvailableResourceItemTypes
			AE_CUSTOM_DIM01,       // configureAvailableCustomDimensions01
			AE_CUSTOM_DIM02,       // configureAvailableCustomDimensions02
			AE_CUSTOM_DIM03,       // configureAvailableCustomDimensions03
		} AVAILABLEEVENTTYPE;

		class CGameAnalytics;
		class CFirebaseAnalytics;
		class CFacebookAnalytics;
		class CAppsFlyerAnalytics;
	}
	
	#ifdef __WIN32__
		namespace Win32
		{
			class CTrueTypeFont;
            class CFontLabel;
			class CURLStream;
			class CBitmapContext;
			class CWebView;
		}
	
		namespace InputSystem
		{
			namespace Win32 
			{				
				class CGamePad;
			}			
		}
	#endif

	#ifdef __iOS__
		namespace iOS
		{
			class CFileStream;  
			class CZipStream;  
            class CURLStream;
			class CURLResumableStream;
            class CTrueTypeFont;
            class CFontLabel;
			class CBitmapContext;
			class CGameAnalytics;
			class CFirebaseAnalytics;
			class CFacebookAnalytics;
			class CAppsFlyerAnalytics;
		}
	
		namespace InputSystem
		{
			namespace iOS 
			{
				class CTouch;
				class CAccelerometer;
				class CMotionInput;
                class CGyrometer;
			}			
		}

		namespace SocialSystem
		{
			namespace iOS 
			{
				class CFacebook;
				class CGameCenter;
				class CTwitter;				
			}			
		}
    
        namespace NetworkSystem
        {
            namespace iOS
            {
                class CHttpRequest;
                class CHttpFormDataRequest;
            }			
        }
	#endif

	#ifdef __ANDROID__
		namespace Android
		{
			class CFileStream;       
			class CURLStream;
			class CURLResumableStream;
			class CFontLabel;
			class CBitmapContext;
			class CTrueTypeFont;
			class CGameAnalytics;
			class CFirebaseAnalytics;
			class CFacebookAnalytics;
			class CAppsFlyerAnalytics;
		}

		namespace InputSystem
		{
			namespace Android 
			{
				//class CTouch;
				//class CAccelerometer;
				//class CMotionInput;
			}			
		}	
	#endif
	
    typedef enum {
        DT_UNKNOWN = 0,
		DT_PC,

		DT_IPOD_START	= 100,
        DT_IPOD_1		= 100,
        DT_IPOD_2,
        DT_IPOD_3,
        DT_IPOD_4,
		DT_IPOD_5,
		DT_IPOD_6,
		DT_IPOD_7,
		DT_IPOD_8,
		DT_IPOD_9,
		DT_IPOD_END,

		DT_IPHONE_START	= 200,
        DT_IPHONE_1		= 200,
        DT_IPHONE_2,
        DT_IPHONE_3,
        DT_IPHONE_4,
		DT_IPHONE_5,
		DT_IPHONE_6,
		DT_IPHONE_7,
		DT_IPHONE_8,
		DT_IPHONE_9,
		DT_IPHONE_END,

		DT_IPAD_START	= 300,
        DT_IPAD_1		= 300,
        DT_IPAD_2,
        DT_IPAD_3,
		DT_IPAD_4,
		DT_IPAD_5,
		DT_IPAD_6,
		DT_IPAD_7,
		DT_IPAD_8,
		DT_IPAD_9,
		DT_IPAD_END,
		
		DT_WINPHONE_7 = 400,
		DT_WINPHONE_8,

		DT_ANDROID = 500,
		DT_PS_VITA = 600,		
		DT_N3DS = 700,
		DT_WINDOWS8 = 800,
    }DEVICETYPE;

	typedef enum {
	    DC_UNKNOWN = 0,
		DC_LOW,
	    DC_MEDIUM,
	    DC_HIGH,
		DC_ULTRA,
		NUM_DEVICECLASS,
	}DEVICECLASS;	
	
    typedef enum {
        DO_UNKNOWN = -1,
        DO_PORTRAIT,                // Device oriented vertically, home button on the bottom
        DO_PORTRAIT_UPSIDE_DOWN,    // Device oriented vertically, home button on the top
        DO_LANDSCAPE_LEFT,          // Device oriented horizontally, home button on the right
        DO_LANDSCAPE_RIGHT,         // Device oriented horizontally, home button on the left
        DO_FACE_UP,                 // Device oriented flat, face up
        DO_FACE_DOWN,               // Device oriented flat, face down
        NUM_DEVICEORIENTATION,
    }DEVICEORIENTATION;

	typedef enum {
        DOM_PORTRAIT					= (1 << DO_PORTRAIT),                
        DOM_PORTRAIT_UPSIDE_DOWN		= (1 << DO_PORTRAIT_UPSIDE_DOWN),
        DOM_LANDSCAPE_LEFT				= (1 << DO_LANDSCAPE_LEFT),                
        DOM_LANDSCAPE_RIGHT				= (1 << DO_LANDSCAPE_RIGHT),                
        DOM_FACE_UP						= (1 << DO_FACE_UP),                
        DOM_FACE_DOWN					= (1 << DO_FACE_DOWN),                
    }DEVICEORIENTATIONMASK;
    
	typedef enum {
		TEXTURE = 0,
		STREAMING_TEXTURE,
		TEXTURE_ANIMATION,		
		DEPTHSTENCIL_STATE,
		BLEND_STATE,
		RASTERIZER_STATE,
		SAMPLER_STATE,
		VERTEX_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		GEOMETRY_SHADER,
		PIXEL_SHADER,
		COMPUTE_SHADER,
		BLEND_SHADER,
		TEXTURE_SHADER,	
		SHADER_GROUP,
		VERTEX_LAYOUT,
		VERTEX_ARRAY,
		INDEX_ARRAY,
		QUERY,
		STREAMOUT_BUFFER,
		BUFFER,
		MATERIAL,
		CONTAINER,
		GAME_OBJECT,
		SCRIPT,
		SOUND,
		MUSIC,
		NUM_RESOURCETYPE,
	}RESOURCETYPE;

	typedef enum {
		SYSTEM_DEFINED = 0,
		TOOL_DEFINED,
		USER_DEFINED,				
		NUM_RESOURCEFLAG,
	}RESOURCEFLAG;

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
		TT_OPAQUE_TRANSLUCENT,
		NUM_TRANSPARENTTYPE,
	}TRANSPARENTTYPE;
	
	typedef enum {
		OBJUPDATE_RENDERABLEOBJ,				
		OBJUPDATE_PHYSICSOBJ,										
		NUM_OBJUPDATETYPE,
	}OBJUPDATETYPE;

	typedef enum {				
		SHOW_NONE = -1,
        SHOW_FPS,
		SHOW_RENDERPROFILE,
		SHOW_RESOURCEDATA,
		SHOW_BUFFERDATA,
		SHOW_TEXTUREDATA,
		SHOW_SHADERDATA,
		SHOW_SHADERCLASSDATA,
		SHOW_MATERIALDATA,
		SHOW_MAPS,
	}SHOWMODE;

	typedef enum {				
		MAIN_THREAD = 0,
		RENDERING_THREAD,
		PHYSICS_THREAD,
		AI_THREAD,
		NUM_THREADID,			
	}THREADID;

	typedef enum {				
		UTM_NONE = 0,
		UTM_SWIPE,
		UTM_FADEOUT,
		UTM_ALPHA_FADEOUT,	
		UTM_SCALE_FADEOUT,
        UTM_SCALE_ROLL_FADEOUT,
		NUM_UITRASITIONMODE,			
	}UITRASITIONMODE;

	typedef enum{
		PE_POINT,
		PE_PLANE,
		PE_CIRCLE,
		PE_MESH,
		PE_CIRCLE_PLANE,
		PE_RING_PLANE,
		NUM_PETYPE,
	}PEMITTERTYPE;		

	typedef enum{
		NOISE_NONE,
		NOISE_RANDOM,
		NOISE_ZEGMA,	
		NUM_NOISETYPE,
	}NOISETYPE;
	
	typedef int							OBJID;

	LYNXENGCLASS CString				GetEnumStr(PLAYMODE e);
	LYNXENGCLASS CString				GetEnumStr(TRANSPARENTTYPE e);		
	LYNXENGCLASS CString				GetEnumStr(RESOURCETYPE e);
    LYNXENGCLASS CString                GetEnumStr(DEVICETYPE e);
	LYNXENGCLASS CString				GetEnumStr(DEVICECLASS e);	

	template <class T>
	T ResourcePtr_Cast(CResourcePtr& ptr) { return (dynamic_cast<T>((LynxEngine::CResource*)ptr)); };			

	#ifdef __TOOL__
		template <class T, class CT>
		T Container_Cast(CT ptr) { return (dynamic_cast<T>(ptr)); };			
	#else
		template <class T, class CT>
		T Container_Cast(CT ptr) { return (static_cast<T>(ptr)); };			
	#endif	
	
	#ifdef __CURL__
		typedef LynxEngine::NetworkSystem::curl::CHttpRequest			CPlatformHttpRequest;
		typedef LynxEngine::NetworkSystem::curl::CHttpFormDataRequest	CPlatformFormDataHttpRequest;
	#else
		typedef LynxEngine::NetworkSystem::CHttpRequest					CPlatformHttpRequest;
		typedef LynxEngine::NetworkSystem::CHttpFormDataRequest			CPlatformFormDataHttpRequest;
	#endif

	#ifdef __iOS__
 		typedef LynxEngine::iOS::CFileStream					CPlatformFileStream;        
		typedef LynxEngine::iOS::CZipStream						CPlatformZipStream;        
		typedef LynxEngine::iOS::CURLStream						CPlatformURLStream;
		typedef LynxEngine::iOS::CURLResumableStream			CPlatformURLResumableStream;
        typedef LynxEngine::iOS::CTrueTypeFont					CPlatformTrueTypeFont;
        typedef LynxEngine::iOS::CFontLabel						CPlatformFontLabel;
		typedef LynxEngine::iOS::CBitmapContext					CPlatformBitmapContext;

		typedef LynxEngine::SocialSystem::iOS::CFacebook		CPlatformFacebook;
		typedef LynxEngine::iOS::CGameAnalytics					CPlatformGameAnalytics;     
		typedef LynxEngine::iOS::CFirebaseAnalytics				CPlatformFirebaseAnalytics;
		typedef LynxEngine::iOS::CFacebookAnalytics				CPlatformFacebookAnalytics;
		typedef LynxEngine::iOS::CAppsFlyerAnalytics			CPlatformAppsFlyerAnalytics;
	#elif defined( __ANDROID__)
 		typedef LynxEngine::Android::CFileStream				CPlatformFileStream;    
		typedef LynxEngine::CZipStream							CPlatformZipStream;
		typedef LynxEngine::Android::CURLStream					CPlatformURLStream; 
		typedef LynxEngine::Android::CURLResumableStream		CPlatformURLResumableStream;
		typedef LynxEngine::Android::CTrueTypeFont				CPlatformTrueTypeFont;
        typedef LynxEngine::Android::CFontLabel					CPlatformFontLabel;
		typedef LynxEngine::Android::CBitmapContext				CPlatformBitmapContext;

		typedef LynxEngine::SocialSystem::CFacebook				CPlatformFacebook;
		typedef LynxEngine::Android::CGameAnalytics				CPlatformGameAnalytics;
		typedef LynxEngine::Android::CFirebaseAnalytics			CPlatformFirebaseAnalytics;
		typedef LynxEngine::Android::CFacebookAnalytics			CPlatformFacebookAnalytics;
		typedef LynxEngine::Android::CAppsFlyerAnalytics		CPlatformAppsFlyerAnalytics;
	#else
		typedef LynxEngine::CFileStream							CPlatformFileStream;       
		typedef LynxEngine::CZipStream							CPlatformZipStream;        
		typedef LynxEngine::Win32::CURLStream					CPlatformURLStream;
		typedef LynxEngine::Win32::CURLStream					CPlatformURLResumableStream; // Win32 NOT implement resumable URLStream
		typedef LynxEngine::Win32::CTrueTypeFont				CPlatformTrueTypeFont;
        typedef LynxEngine::Win32::CFontLabel					CPlatformFontLabel;
		typedef LynxEngine::Win32::CBitmapContext               CPlatformBitmapContext;
		typedef LynxEngine::Win32::CWebView						CPlatformWebView;

		typedef LynxEngine::SocialSystem::CFacebook				CPlatformFacebook;
		typedef LynxEngine::GameAnalyticsSystem::CGameAnalytics	CPlatformGameAnalytics;              
		typedef LynxEngine::GameAnalyticsSystem::CFirebaseAnalytics			CPlatformFirebaseAnalytics;      
		typedef LynxEngine::GameAnalyticsSystem::CFacebookAnalytics			CPlatformFacebookAnalytics;      
		typedef LynxEngine::GameAnalyticsSystem::CAppsFlyerAnalytics		CPlatformAppsFlyerAnalytics;   
	#endif

	typedef Json::Value											CJsonValue;
	#ifdef _xxDEBUG
		#define JSON_VALUE_AS_CSTRING()							asCString()
	#else
		#define JSON_VALUE_AS_CSTRING()							asString().c_str()
	#endif

	#ifdef __LYNX_DEBUG_MEM_ALLOCATOR__
		#define LYNXMEMALLOCATOR 								LynxEngine::CDebugMemAllocator		
	#elif defined __LYNX_SMALLOBJ_MEM_ALLOCATOR__
		#define LYNXMEMALLOCATOR 								LynxEngine::CSmallObjAllocator	
	#elif defined __LYNX_RELEASE_MEM_ALLOCATOR__
		#define LYNXMEMALLOCATOR 								LynxEngine::CReleaseAllocator
	#endif	

	typedef	tinyxml2::XMLDocument								CXMLDoc;
	typedef	tinyxml2::XMLElement								CXMLElement;
	typedef	tinyxml2::XMLDeclaration							CXMLDecl;
	typedef tinyxml2::XMLText									CXMLText;
	typedef tinyxml2::XMLNode									CXMLNode;

	typedef struct NETGAMEDESC{
		LYNXBOOL							IsVisible;
		LYNXBOOL							IsOpen;
		LYNXBYTE							MaxPlayers;

		LYNXFORCEINLINE void Default()
		{
			IsVisible = true;
			IsOpen = true;
			MaxPlayers = 0;
		}

		NETGAMEDESC() { Default(); }
	}NETGAMEDESC;
}

extern LYNXENGCLASS LynxEngine::CLogger*                        glpLogger;
extern LYNXENGCLASS LynxEngine::CLogger*                        glpNetworkLogger;
extern LYNXENGCLASS LynxEngine::COutputDevice*					glpOutputDevice;
extern LYNXENGCLASS LynxEngine::ControlPanel::CControlPanel*	gControlPanelWnd;
#if (!defined(__TOOL__))
    extern LYNXENGCLASS LynxEngine::GameFramework::CKernel*     glpKernel;
#endif

extern LYNXENGCLASS void lynxInit(LYNXBOOL blog = LYNX_FALSE, LYNXBOOL bcontrol_panel = LYNX_FALSE);
extern LYNXENGCLASS void lynxRelease();

#ifdef _MSC_VER 		
	#ifdef _USRDLL
		#pragma warning ( disable : 4251 ) 
	#endif
#endif
/*
#ifdef _DEBUG	
	#include <LynxMemAllocator.h>
	LYNXINLINE void	*operator new(size_t s, const char *file, const char *func, int line) {return glpMemAllocator?glpMemAllocator->vAlloc(s, file, func, line):lynxMemAlloc_d(s, file, func, line); };
	LYNXINLINE void	operator  delete(void *p, size_t s) {return glpMemAllocator?glpMemAllocator->vFree(p, s):lynxMemFree_d(p); };			
#else
#endif
*/

#endif
