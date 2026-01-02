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

#ifndef __LYNXENGINE_H__
#define __LYNXENGINE_H__

#include <LynxCore.h>
#include <LynxMathWrap.h>

#include <LynxEngineClasses.h>
#include <LynxAlgorithm.h>
#include <LynxObj.h>
#include <LynxEvent.h>
#include <LynxFactory.h>
#include <LynxResource.h>
#include <LynxShaderClassManager.h>
#include <LynxContainerFactory.h>
#include <LynxResourceManager.h>
#include <LynxVertexLayoutManager.h>
#include <LynxException.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Container/LynxContainer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <Font/LynxTextureFont.h>

#include <PhysicsSystem/LynxPhysicsObj.h>
#include <PhysicsSystem/LynxPhysicsShape.h>
#include <PhysicsSystem/LynxPhysicsJoint.h>

#ifdef __WIN32__
	#ifdef __WINAPP__
	#else
		#include <ft2build.h>
		#include FT_FREETYPE_H
	#endif
#endif

namespace LynxEngine 
{
	class CIniReader;
}

namespace LynxEngine 
{
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	#ifdef __LOG__
		#define LYNX_LOG(l, e)					if (l) l->vLog(e);
		#define LYNX_NETWORK_LOG(l, e)			if (l) l->vLog(e);
	#else
		#define LYNX_LOG(l, e)
		#define LYNX_NETWORK_LOG(l, e)
	#endif
	
	class LYNXENGCLASS CEngine : public CObj
	{	
		LYNX_DECLARE_CLASS(CEngine);
	public:
		struct INIT_DESC {
			//graphics
			int							Width;
			int                         Height;
			int                         bpp;
			GraphicsSystem::CRenderer::RENDERERTYPE RendererType;
			GraphicsSystem::CRenderer::FORMAT FrameBufferFormat;
			GraphicsSystem::CRenderer::FORMAT ZBufferFormat;	
			LYNXBOOL					bHardware;	
			LYNXBOOL					bFullScreen;
			LYNXBOOL					bWaitVSync;
			DWORD						RefreshRate;
			//LYNXINTERLACETYPE			InterlaceType;
			//LYNXTVSCANMODE			TVScanMode;
			//LYNXFRAMEMODE				FrameMode;
			DWORD						NumMultiSamples;
			DWORD						ShaderDebuggingFlag;

			//subsystems
			LYNXCHAR					PhysicsSubsystemName[64];
			LYNXCHAR					ScriptSubsystemName[64];
		};		
		typedef CSystem* (*LPCREATESYSTEMPROC)(CEngine*);			

		typedef struct {
			int							Year;
			int							Month;
			int							Day;
			int							Hour;
			int							Minute;
			int							Second;
		}DATEDATA;

		typedef struct {
			LYNXBOOL					bFineStepMode;
			float						MinStep;
			int							MaxCountOfLoop;
		}FINESTEPDATA;

		LYNXBOOL										m_bUseTouchDevice;
        CIniReader*										m_ConfigIniReader;
		FINESTEPDATA									m_FineStepData;
	protected:						
		#ifdef __CALL_STACK__
			//static std::stack<CString>					m_CallStack;
		#endif		
		GameFramework::CKernel*							m_lpKernel;

        //CTextureFont*									m_lpSystemLargeFont;
		//CTextureFont*									m_lpSystemFont;
		//CTextureFont*                                   m_lpSystemSmallFont;
        CTextureFontString*								m_lpSystemFontString;
        //CPlatformTrueTypeFont*                          m_lpSystemLargeTTFont;
        //CPlatformTrueTypeFont*                          m_lpSystemTTFont;
        //CPlatformTrueTypeFont*                          m_lpSystemSmallTTFont;
        CTrueTypeFontString*							m_lpSystemTTFontString;
		CMap<CAnsiString, CFontPtr>						m_FontMap;
        #ifdef __WIN32__
			#ifdef __WINAPP__
			#else
				FT_Library								m_FreeTypeLibrary;  
			#endif
        #endif        
		CSpriteContainer*								m_lpLogoSprite;
		
		CString											m_ConfigFilename;		
		
		CShaderClassManager								m_ShaderClassManager;
		CContainerFactory								m_ContainerFactory;
		int												m_NumResources;
		CResourceManager								m_ResourceManager;	
		CVertexLayoutManager							m_VertexLayoutManager;

        //System input devices		
		InputSystem::CKeyboard*							m_lpSystemKeyboard;
		InputSystem::CMouse*							m_lpSystemMouse;
		CArray<InputSystem::CGamePad*>					m_SystemGamePadArray;
		InputSystem::CInputRecord*						m_lpSystemInputRecord;
		InputSystem::CTouch*							m_lpSystemTouch;
        InputSystem::CAccelerometer*                    m_lpSystemAccelerometer;
        InputSystem::CGyrometer*                        m_lpSystemGyrometer;
        
		//Subsystems
		FileSystem::CFileSystem*						m_lpFileSystem;
		InputSystem::CInputSystem*						m_lpInputSystem;
		GameConsoleSystem::CGameConsoleSystem*			m_lpGameConsoleSystem;							
		NetworkSystem::CNetworkSystem*					m_lpNetworkSystem;
		SoundSystem::CSoundSystem*						m_lpSoundSystem;
		ScriptSystem::CScriptSystem*					m_lpScriptSystem;		
		PhysicsSystem::CPhysicsSystem*					m_lpPhysicsSystem;	
		float											m_PhysicsFrameTime;
		GUISystem::CGUISystem*							m_lpGUISystem;
		GraphicsSystem::CGraphicsSystem*				m_lpGraphicsSystem;						
		SceneSystem::CSceneSystem*						m_lpSceneSystem;					
		float											m_SceneFrameTime;
		CollisionSystem::CCollisionSystem*				m_lpCollisionSystem;
		PerfSystem::CPerfSystem*						m_lpPerfSystem;
		AdSystem::CAdSystem*							m_lpAdSystem;
		
		CList<CGameObj*>								m_EventRegisteredObjList[CEvent::NUM_TYPE];								
		
		float											m_StepMultiplier;
		LYNXBOOL										m_bRegularGC;
	public:
		//CArray<CViewport*>								m_ViewportArray;
		LYNXBOOL										m_bLockFrameRate;
		float											m_UpdateRate;
		float											m_Step;		
		SHOWMODE										m_ShowMode;
		GraphicsSystem::CMap*							m_ShowedMap;

	protected:
		void											InitMembers();		
		virtual void									vInitSystemFont();
		virtual void									vInitLogoSprite();
		virtual void									vInitGameScripts() {};
		void											RecursiveLoop(float step);
		
	public:		
		CEngine();
		CEngine(GameFramework::CKernel* lpk);
		virtual ~CEngine();		

		DECLARE_CLASSSCRIPT
		LYNXBOOL										vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
		void											Render();
		LYNXBOOL										OpenPackage(const LYNXCHAR* name);
		void											ClosePackage();
		LYNXFORCEINLINE void							ShowMode(int sm) {m_ShowMode = (SHOWMODE)sm; };
        
		static void										GetFileNameFromPath(LYNXCHAR* name, const LYNXCHAR* path);
		static void										GetFileNameFromPathAnsi(char* name, const char* path);
		static void										GetRelativePath(LYNXCHAR *path, const LYNXCHAR* work, const LYNXCHAR *fname);
		static void										GetLowerRelativePath(LYNXCHAR *path, const LYNXCHAR* work, const LYNXCHAR *fname);
		static void										FilenameBackslashToSlash(CString& name);
		static void										GetTexturePathFromFilename(LYNXCHAR *path, const LYNXCHAR *fname);
		
		static int										GetMajorVersion();
		static int										GetMinorVersion();
		static int										GetBuildVersion();

		static LYNXBOOL									GetJSONValues(Json::Value& root, const CAnsiString& doc);
		static LYNXBOOL									GetJSONValuesFromFile(Json::Value& root, const CString& filename);
		static void										GetJSONValues(CMap<CAnsiString, CString>& map, const CAnsiString& doc);
		static void										MakeJSON(CAnsiString& doc, const Json::Value& root);
		static void										LoadJSON(Json::Value& root, const CString& filename);
		static void										SaveJSON(const Json::Value& root, const CString& filename);

		static CString									GetLocalDateString(const LYNXLONGLONG& t);
		static CString									GetLocalDateStringWithFormat(const LYNXLONGLONG& t, const CString format = "YYYY-MM-DD");
		static const DATEDATA&							GetLocalDateWithData(const LYNXLONGLONG& t);
		static CString									GetLocalTimeStringWithoutSecond(const LYNXLONGLONG& t);
		static CString									GetFullLocalTimeString(const LYNXLONGLONG& t);
		static CString									GetCurrentLocalTimeStringForFilename();
		static CString									GetGMTDateStringWithFormat(const LYNXLONGLONG& t, const CString format);
		static void										EncryptFile2Zip(const CAnsiString& file_name, const CAnsiString& password);
		static LYNXBOOL									DecryptFileFromZip(CPlatformFileStream& out, const CAnsiString& file_name, const CAnsiString& password, int flag);

		static LYNXBOOL									GZipInflate(LynxEngine::CAnsiString& uncompressed, const LynxEngine::CAnsiString& compressed); 
				
		//LYNXINLINE CLogger*								GetLogger() {return m_lpLogger; };		
		
		LYNXFORCEINLINE const CString&					GetConfigFilename() {return m_ConfigFilename; };
		/*
		LYNXFORCEINLINE CFontPtr&                          GetSystemFont() {return static_cast<CFontPtr&>(m_lpSystemFont); };
		LYNXFORCEINLINE CFontPtr&                          GetSystemSmallFont() {return static_cast<CFontPtr&>(m_lpSystemSmallFont); };
		LYNXFORCEINLINE CFontPtr&                          GetSystemLargeFont() {return static_cast<CFontPtr&>(m_lpSystemLargeFont); };
		*/
		LYNXFORCEINLINE CFontPtr&                       GetSystemFont() {return m_FontMap.is_exist(SYSTEM_DEFAULT_TEXTURE_FONT) ? m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT] : m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT]; };
		LYNXFORCEINLINE CFontPtr&                       GetSystemSmallFont() { return m_FontMap.is_exist(SYSTEM_DEFAULT_TEXTURE_FONT_S) ? m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT_S] : m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT]; };
		LYNXFORCEINLINE CFontPtr&                       GetSystemLargeFont() { return m_FontMap.is_exist(SYSTEM_DEFAULT_TEXTURE_FONT_L) ? m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT_L] : m_FontMap[SYSTEM_DEFAULT_TEXTURE_FONT]; };
        LYNXFORCEINLINE CTextureFontString*				GetSystemFontString() {return m_lpSystemFontString; };
        #ifdef __WIN32__
            LYNXFORCEINLINE FT_Library						GetFreeTypeLibrary(){return m_FreeTypeLibrary;}
        #endif
		/*
		LYNXFORCEINLINE CPlatformTrueTypeFont*			GetSystemLargeTTFont() {return m_lpSystemLargeTTFont; };
        LYNXFORCEINLINE CPlatformTrueTypeFont*			GetSystemTTFont() {return m_lpSystemTTFont; };
        LYNXFORCEINLINE CPlatformTrueTypeFont*			GetSystemSmallTTFont() {return m_lpSystemSmallTTFont; };
		*/
		LYNXFORCEINLINE CFontPtr&                       GetSystemTTFont() { return m_FontMap.is_exist(SYSTEM_DEFAULT_TRUETYPE_FONT) ? m_FontMap[SYSTEM_DEFAULT_TRUETYPE_FONT] : m_FontMap[SYSTEM_DEFAULT_TRUETYPE_FONT]; };
		LYNXFORCEINLINE CFontPtr&                       GetSystemSmallTTFont() { return m_FontMap.is_exist(SYSTEM_DEFAULT_TRUETYPE_FONT_S) ? m_FontMap[SYSTEM_DEFAULT_TRUETYPE_FONT_S] : m_FontMap[SYSTEM_DEFAULT_TRUETYPE_FONT]; };
		LYNXFORCEINLINE CFontPtr&                       GetSystemLargeTTFont() { return m_FontMap.is_exist(SYSTEM_DEFAULT_TRUETYPE_FONT_L) ? m_FontMap[SYSTEM_DEFAULT_TRUETYPE_FONT_L] : m_FontMap[SYSTEM_DEFAULT_TRUETYPE_FONT]; };
        LYNXFORCEINLINE CTrueTypeFontString*			GetSystemTTFontString() {return m_lpSystemTTFontString; };
		LYNXFORCEINLINE CFontPtr&                       GetFont(CAnsiString& name) { return m_FontMap.is_exist(name) ? m_FontMap[name] : m_FontMap[SYSTEM_DEFAULT_TRUETYPE_FONT]; };
		
		virtual LYNXBOOL								vCreate(GameFramework::CKernel::INIT_DESC* desc = NULL);
		virtual LYNXBOOL								vCreateSubSystems(GameFramework::CKernel::INIT_DESC* desc = NULL);

		virtual LYNXBOOL								vInit(LYNXWINDOW &win, const int w = 0, const int h = 0, const LYNXBOOL bfull = LYNX_FALSE);			
		virtual LYNXBOOL								vInit(LYNXWINDOW &win, GraphicsSystem::CRenderer::INITGRAPHICPARAM* lpcp);				
		virtual LYNXBOOL								vInit(LYNXWINDOW &win, INIT_DESC& desc);		
        void											LoadConfig(const LYNXCHAR* filename);
		
		LYNXINLINE void									SetlpKernel(GameFramework::CKernel* lpk) {m_lpKernel = lpk; };						
		LYNXINLINE GameFramework::CKernel*				GetlpKernel() {return m_lpKernel; };						

		void											SetStepMultiplier(const float sf);
		LYNXINLINE float								GetStepMultiplier() const {return m_StepMultiplier; };
		LYNXINLINE void									SetFineStepMode(LYNXBOOL b) { m_FineStepData.bFineStepMode = b; };
		
		float											GetGUIStep() const;		
		virtual void									vLoop(float step);

		void											InitShowingData();
		void											ShowData();
		void											ShowMap(GraphicsSystem::CMap* map);

		virtual void									vRegisterDefaultResources();
		virtual void									vRegisterDefaultContainers();	
		virtual void									vRegisterDefaultVertexLayouts();
		virtual void									vRegisterDefaultShaderClasses();

		LYNXINLINE CResourceManager& 					GetResourceManager() {return m_ResourceManager;};	
		LYNXINLINE CContainerFactory& 					GetContainerFactory() {return m_ContainerFactory;};	

		// UI		
		GameFramework::CUIPage*							CreateUIPage(const LYNXCHAR* name, const LYNXCHAR* classname);
		GameFramework::CUIPage*							CreateUIPage(const LYNXCHAR* name, const LYNXCHAR* scriptfile, const LYNXCHAR* scriptname);
		void											DeleteLastUIPage();
		void											SetCurrentUIPage(GameFramework::CUIPage* page);
		GameFramework::CUIPage*							FindUIPage(const LYNXCHAR* name);
		void											SetUITransitionMode(UITRASITIONMODE mode);
		void											SetSwipeSpeed(float speed);		
		void											SetFadeOutSpeed(float speed);		
		void											GetScreenPosition(CVector2& spos, const CVector3& pos, const CCameraContainer& cam, const LYNXRECT& viewport);

		// Video
		virtual CVideo*									vCreateVideo(const CCreationDesc* desc) {return NULL; };
		virtual void									vReleaseVideo(const CVideo* video);

		// Resource
		template <class RT>	
		void CreateResource(RT& r, RESOURCETYPE type, LYNXBOOL brecord = LYNX_TRUE)			
		{
			m_ResourceManager.CreateObject(r, type, brecord);
		}
		void											DestroyResource(CResourcePtr& r);
		void											DestroyResource(const CString& name);
		CResourcePtr&									FindResource(const CString& name);
		CResourcePtr&									FindResource(const LYNXCRC& crc);
		void											ClearInvalidResource(void);
		void											GC(LYNXBOOL bforce = LYNX_TRUE);
		void											LogResourceList();
		void											LogUnreleasedResourceList();		
		LYNXBOOL										VerifyResources();

		static LYNXCRC									ComputeFileCRC(const CString& path);
		static void										GeneratePVRTextures(const CString& dest, const CString& src);

		static CString									GenerateUDID();		
		// Container
		CContainer*										CreateContainer(CContainer::TYPE type);
		CContainer*										_CreatePEmitterContainer(const LYNXCHAR* name, const LYNXCHAR* filename, PEMITTERTYPE petype, int type, int cluster_size, float life);
		CContainer*										CreatePEmitterContainer(const LYNXCHAR* name, const LYNXCHAR* filename, PEMITTERTYPE petype, const CContainer::TYPE& ptype, int cluster_size, float life);
		void											DestroyContainer(CContainer* const lpc);
		
		// Physics
		PhysicsSystem::CPhysicsWorld*					CreatePhysicsWorld();
		virtual PhysicsSystem::CPhysicsObj*				vCreatePhysicsObj(PhysicsSystem::CPhysicsObj::TYPE t, PhysicsSystem::CPhysicsShape* g = NULL);
		virtual void									vDestroyPhysicsObj(PhysicsSystem::CPhysicsObj* o);
		virtual PhysicsSystem::CPhysicsShape*			vCreatePhysicsShape(PhysicsSystem::CPhysicsShape::TYPE type, const CCreationDesc *desc, PhysicsSystem::CPhysicsObj* lpo = NULL);
		virtual void									vDestroyPhysicsShape(PhysicsSystem::CPhysicsShape* s);
		virtual PhysicsSystem::CPhysicsJoint*			vCreatePhysicsJoint(PhysicsSystem::CPhysicsJoint::TYPE type, const CCreationDesc* desc);		
		virtual void									vDestroyPhysicsJoint(PhysicsSystem::CPhysicsJoint* j);

		LYNXINLINE CVertexLayoutManager& 				GetVertexLayoutManager() {return m_VertexLayoutManager;};						
		LYNXBOOL										CreateVertexLayout(GraphicsSystem::CVertexLayoutPtr& v, const CString& type);
		void											DestroyVertexLayout(GraphicsSystem::CVertexLayoutPtr& v);
		void											DestroyVertexLayout(const CString& name);
		GraphicsSystem::CVertexLayoutPtr&				FindVertexLayout(const CString& name);
		void											ClearInvalidVertexLayout(void);

		LYNXINLINE CShaderClassManager& 				GetShaderClassManager() {return m_ShaderClassManager;};						
		LYNXBOOL										CreateShaderClass(GraphicsSystem::CShaderClassPtr& s, const CAnsiString& type);
		LYNXBOOL										CreateCustomShaderClass(GraphicsSystem::CShaderClassPtr& s, const CAnsiString& type, const CString& filename);
		void											DestroyShaderClass(GraphicsSystem::CShaderClassPtr& lps);
		void											DestroyShaderClass(const CAnsiString& name);
		GraphicsSystem::CShaderClassPtr&				FindShaderClass(const CAnsiString& name);
		void											ClearInvalidShaderClass(void);

		void											SwapBackBuffer(void);
		
		static void										ComputeNewStep(float* new_step, int* count, const float step, const float min, const int max_count);		

		void											OpenScene(const LYNXCHAR* filename);
		void											OpenScene(const CCreationDesc* desc);

		CSystem*										LoadSubSystem(const LYNXCHAR *name);
		LYNXBOOL										UnloadSubSystem(CSystem* lpss);
		
		LYNXFORCEINLINE InputSystem::CKeyboard*			GetlpSystemKeyboard() const { return m_lpSystemKeyboard; };
		LYNXFORCEINLINE InputSystem::CMouse*			GetlpSystemMouse() const { return m_lpSystemMouse; };	
		LYNXFORCEINLINE InputSystem::CGamePad*			GetSystemGamePad(int i) const { return ((i < m_SystemGamePadArray.size()) ? m_SystemGamePadArray[i] : NULL); }
		LYNXFORCEINLINE InputSystem::CInputRecord*		GetlpSystemInputRecord() const { return m_lpSystemInputRecord; };	
		LYNXFORCEINLINE InputSystem::CTouch*			GetlpSystemTouch() const { return m_lpSystemTouch; };	
        LYNXFORCEINLINE InputSystem::CAccelerometer*    GetlpSystemAccelerometer() const { return m_lpSystemAccelerometer; };
        LYNXFORCEINLINE InputSystem::CGyrometer*        GetlpSystemGyrometer() const { return m_lpSystemGyrometer; };
        
		LYNXFORCEINLINE GraphicsSystem::CGraphicsSystem* GetlpGraphicsSystem() const {return m_lpGraphicsSystem;};						
		LYNXFORCEINLINE SceneSystem::CSceneSystem*		GetlpSceneSystem() const {return m_lpSceneSystem;};			
		LYNXFORCEINLINE ScriptSystem::CScriptSystem*	GetlpScriptSystem() const {return (m_lpScriptSystem);};
		LYNXFORCEINLINE PhysicsSystem::CPhysicsSystem*	GetlpPhysicsSystem() const {return (m_lpPhysicsSystem);};
		LYNXFORCEINLINE GameConsoleSystem::CGameConsoleSystem* GetlpGameConsoleSystem() const {return m_lpGameConsoleSystem;};				
		LYNXFORCEINLINE InputSystem::CInputSystem*		GetlpInputSystem() const {return m_lpInputSystem;};	
		LYNXFORCEINLINE SoundSystem::CSoundSystem*		GetlpSoundSystem() const {return m_lpSoundSystem;};	
		LYNXFORCEINLINE CollisionSystem::CCollisionSystem* GetlpCollisionSystem() const {return m_lpCollisionSystem;};	
		LYNXFORCEINLINE PerfSystem::CPerfSystem*		GetlpPerfSystem() const {return m_lpPerfSystem;};	
		LYNXFORCEINLINE FileSystem::CFileSystem*		GetlpFileSystem() const {return m_lpFileSystem;};	
		LYNXFORCEINLINE GUISystem::CGUISystem*			GetlpGUISystem() const {return m_lpGUISystem;};		
        LYNXFORCEINLINE AdSystem::CAdSystem*			GetlpAdSystem() const {return m_lpAdSystem;};		
		LYNXFORCEINLINE NetworkSystem::CNetworkSystem*	GetlpNetworkSystem() const {return m_lpNetworkSystem;};	
		
		
		//LYNXINLINE CContainerManager&					GetContainerManager() {return m_ContainerManager;};						

		void											RegisterEvent(CEvent::TYPE t, CGameObj* obj);
		void											BroadcastEvent(CEvent& e);

		virtual void									vBeginRendering(CCameraContainer& camera);
		virtual void									vRender(CCameraContainer& camera);
		virtual void									vFinishRendering(CCameraContainer& camera);
		virtual void									vOnRenderMode(int num, const LYNXCHAR arg[][64]);		
		virtual void									vOnOpenScene(const LYNXCHAR* filename);
		virtual void									vOnExit();
		virtual void									vOnShow(int num, const LYNXCHAR arg[][64]);
		virtual void									vOnResize(const LYNXCHAR arg1[64], const LYNXCHAR arg2[64]);
		virtual void									vOnCommand(const LYNXCHAR* command, int num, const LYNXCHAR arg[][64]) {};		

		
		static CString									ProfanityFilterString(const CString& inputString, const CArray<CString>& ProfanityStringTable, const LYNXCHAR replaceChar = '*');		
		static CString									ReplaceFormatString(const LYNXCHAR* inputCString, ... /* token, value,  MUST END WITH NULL*/);
	};
	//bool operator < (const LYNXGUID& lhs, const LYNXGUID& rhs) {if (lynxOSComapreGUID(&lhs, &rhs) < 0) return true; else return false; };					
	#ifdef __CALL_STACK__		
		LYNXENGCLASS extern CStack<CAnsiString>			gCallStack;
		LYNXENGCLASS void								AddToCallStack(const LYNXANSICHAR* s);
		LYNXENGCLASS CString							GetCallStackString();
	#endif	

	LYNXENGCLASS LYNXFORCEINLINE bool Inside(const LYNXPOINT2D& p, const LYNXRECT& rect) {return (p.x >= rect.x1 && p.x <= rect.x2 && p.y >= rect.y1 && p.y <= rect.y2); };	

	LYNXENGCLASS GraphicsSystem::CRenderer::BLENDSTATEDESC TranslateBlendType(GraphicsSystem::CRenderer::BLENDTYPE e = GraphicsSystem::CRenderer::BLT_ALPHA);

	LYNXENGCLASS GraphicsSystem::CRenderer::FORMAT GetFormat(const CString& s);
	LYNXENGCLASS GraphicsSystem::CRenderer::BLENDTYPE GetBlendType(const CString& s);
	LYNXENGCLASS GraphicsSystem::CRenderer::BLENDTYPE GetBlendType(const GraphicsSystem::CBlendState& b);
	LYNXENGCLASS GraphicsSystem::CRenderer::BLENDFUNC GetBlendFunc(const CString& s);
	LYNXENGCLASS GraphicsSystem::CRenderer::BLENDOP GetBlendOp(const CString& s);
	LYNXENGCLASS GraphicsSystem::CRenderer::FILLMODE GetFillMode(const CString& s);
	LYNXENGCLASS GraphicsSystem::CRenderer::CULLMODE GetCullMode(const CString& s);
	LYNXENGCLASS GraphicsSystem::CRenderer::DEPTHBUFFERMODE GetDepthBufferMode(const CString& s);
	LYNXENGCLASS GraphicsSystem::CRenderer::STENCILOP GetStencilOp(const CString& s);
	LYNXENGCLASS GraphicsSystem::CRenderer::TESTFUNC GetTestFunc(const CString& s);
	LYNXENGCLASS TRANSPARENTTYPE GetTransparentType(const CString& s);

	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::RENDERERTYPE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::RENDERDEVICETYPE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::TESTFUNC e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::LOCKMODE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::SHADERTARGET e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::FORMAT e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::TEXTURETYPE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::ADDRESSMODE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::FILTERMODE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::BLENDFUNC e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::BLENDOP e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::BLENDTYPE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::DEPTHBUFFERMODE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::FILLMODE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::CULLMODE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::PRIMITIVETYPE e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::STENCILOP e);
	LYNXENGCLASS CString GetEnumStr(GraphicsSystem::CRenderer::VERTEXSLOTTYPE e);

	// 'GetEnumStr' should be declared prior to the call site or in namespace
	template <typename T>
	T GetEnum(const CString& s, const T& max)
	{
		for (int i = 0; i < (int)max; i++)
		{
			if (s == GetEnumStr((T)i))
				return (T)i;
		}
		return max;
	}
}

#define REGISTER_RESOURCE(s, m) \
    LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Register ")) + GetEnumStr(s) + CString(" resource\r\n"));\
    m_ResourceManager.Register(s, m);	

#define REGISTER_CONTAINER(s, m) \
    LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Register ")) + GetEnumStr(s) + CString(" container\r\n"));\
    m_ContainerFactory.Register(s, m);		

#define IMPLEMENT_SHADERCLASS(s) \
    LYNXENGCLASS GraphicsSystem::CShaderClass* Create##s(CEngine* e)	{ return LYNXNEW Shader::C##s(e->GetlpGraphicsSystem()); }

#define DECLARE_SHADERCLASS(s) \
	GraphicsSystem::CShaderClass* Create##s(CEngine* e);

#define REGISTER_SHADERCLASS(s) \
	LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Register ")) + CString(#s) + CString(" shader class\r\n"));\
    m_ShaderClassManager.Register(CAnsiString(#s), Create##s);

#define REGISTER_VERTEXLAYOUT(s, m) \
    LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Register ")) + CString(s) + CString(" vertex layout\r\n"));\
    m_VertexLayoutManager.Register(CString(s), m);

/*namespace LynxEngine 
{
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------		
	IMPLEMENT_SHADERCLASS(CustomVS)
	IMPLEMENT_SHADERCLASS(CustomPS)

	IMPLEMENT_SHADERCLASS(FontStringVS)

	IMPLEMENT_SHADERCLASS(BltPS)
	IMPLEMENT_SHADERCLASS(ColorPS)
	IMPLEMENT_SHADERCLASS(ConstantColorPS)
	IMPLEMENT_SHADERCLASS(DiffuseMapPS)
	IMPLEMENT_SHADERCLASS(AnimationDiffuseMapPS)
	IMPLEMENT_SHADERCLASS(DiffuseMapXColorPS)
	IMPLEMENT_SHADERCLASS(DiffuseMapXDiffuseColorPS)
	IMPLEMENT_SHADERCLASS(OneChannelBltPS)
	IMPLEMENT_SHADERCLASS(AddTwoChannelPS)
	IMPLEMENT_SHADERCLASS(DistortionPS)
	
	IMPLEMENT_SHADERCLASS(BoxBlurPS)
	IMPLEMENT_SHADERCLASS(BoxBlurWithEdgeCheckPS)
	IMPLEMENT_SHADERCLASS(GaussinBlurPS)
	IMPLEMENT_SHADERCLASS(BilateralBlurPS)
	IMPLEMENT_SHADERCLASS(ScreenQuadVS)
	IMPLEMENT_SHADERCLASS(ScreenQuadUVOffsetVS)
	IMPLEMENT_SHADERCLASS(ScreenQuadBlurTexCoordVS)
	IMPLEMENT_SHADERCLASS(ScreenDepthQuadVS)
	
	IMPLEMENT_SHADERCLASS(AmbientLightingPS)
	IMPLEMENT_SHADERCLASS(SpotLightingPS)
	IMPLEMENT_SHADERCLASS(PointLightingPS)
	IMPLEMENT_SHADERCLASS(DirectionalLightingPS)
	IMPLEMENT_SHADERCLASS(SimpleLightingPS)
	IMPLEMENT_SHADERCLASS(OneLightingPS)

	IMPLEMENT_SHADERCLASS(DeferredAmbientLightingPS)
	IMPLEMENT_SHADERCLASS(DeferredDirectionalLightingPS)
	IMPLEMENT_SHADERCLASS(DeferredSpotLightingPS)
	IMPLEMENT_SHADERCLASS(DeferredPointLightingPS)
	IMPLEMENT_SHADERCLASS(GPassPS)

	IMPLEMENT_SHADERCLASS(SimplePassPS)

	IMPLEMENT_SHADERCLASS(PostAdaptLuminancePS)
	IMPLEMENT_SHADERCLASS(PostAverageLuminancePS)
	IMPLEMENT_SHADERCLASS(PostBrightPassPS)
	IMPLEMENT_SHADERCLASS(PostComputeLuminancePS)
	IMPLEMENT_SHADERCLASS(PostGetLuminancePS)
	IMPLEMENT_SHADERCLASS(PostToneMappingPS)
	IMPLEMENT_SHADERCLASS(PostMotionBlurPS)
	IMPLEMENT_SHADERCLASS(PostDOFPS)
	IMPLEMENT_SHADERCLASS(PostSSAOPS)
	IMPLEMENT_SHADERCLASS(PostSSAOLQPS)

	IMPLEMENT_SHADERCLASS(ProxyVS)
	IMPLEMENT_SHADERCLASS(ProxyPS)

	IMPLEMENT_SHADERCLASS(ShadowMapPS)
	IMPLEMENT_SHADERCLASS(ShadowMap_AlphaTestPS)
	IMPLEMENT_SHADERCLASS(ZPass_VelocityPS)
	IMPLEMENT_SHADERCLASS(ZPass_Velocity_AlphaTestPS)

	// Containers
	IMPLEMENT_SHADERCLASS(GeometryVS)
	IMPLEMENT_SHADERCLASS(GeometryDiffuseVS)
	IMPLEMENT_SHADERCLASS(GeometryLightmapVS)
	IMPLEMENT_SHADERCLASS(GeometryZPassVS)
	IMPLEMENT_SHADERCLASS(GeometryGPassVS)
	IMPLEMENT_SHADERCLASS(GeometryShadowMapVS)
	IMPLEMENT_SHADERCLASS(GeometrySimpleLightingVS)
	IMPLEMENT_SHADERCLASS(GeometryLightingVS)
	IMPLEMENT_SHADERCLASS(GeometryOneLightingVS)

	IMPLEMENT_SHADERCLASS(RigidModelVS)
	IMPLEMENT_SHADERCLASS(RigidModelZPassVS)
	IMPLEMENT_SHADERCLASS(RigidModelGPassVS)
	IMPLEMENT_SHADERCLASS(RigidModelShadowMapVS)
	IMPLEMENT_SHADERCLASS(RigidModelLightingVS)
	IMPLEMENT_SHADERCLASS(RigidModelOneLightingVS)

	IMPLEMENT_SHADERCLASS(SkinningModelVS)
	IMPLEMENT_SHADERCLASS(SkinningModelZPassVS)
	IMPLEMENT_SHADERCLASS(SkinningModelGPassVS)
	IMPLEMENT_SHADERCLASS(SkinningModelShadowMapVS)
	IMPLEMENT_SHADERCLASS(SkinningModelSimpleLightingVS)
	IMPLEMENT_SHADERCLASS(SkinningModelLightingVS)
    //IMPLEMENT_SHADERCLASS(SkinningModelOneLightingVS)

	IMPLEMENT_SHADERCLASS(SpriteVS)

	IMPLEMENT_SHADERCLASS(Sprite3DVS)
	IMPLEMENT_SHADERCLASS(Sprite3DZPassVS)
	IMPLEMENT_SHADERCLASS(Sprite3DShadowMapVS)

	IMPLEMENT_SHADERCLASS(Sprite3DClusterVS)
	IMPLEMENT_SHADERCLASS(Sprite3DClusterZPassVS)
	IMPLEMENT_SHADERCLASS(Sprite3DClusterGPassVS)
	IMPLEMENT_SHADERCLASS(Sprite3DClusterGPassPS)
	IMPLEMENT_SHADERCLASS(Sprite3DClusterShadowMapVS)

	IMPLEMENT_SHADERCLASS(SpriteClusterVS)

	IMPLEMENT_SHADERCLASS(VoxelClusterVS)
	IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingVS)
	IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingSpecularVS)
	IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingFakeAOVS)
	IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingFakeAOSpecularVS)
	IMPLEMENT_SHADERCLASS(VoxelClusterOneLightingFakeAOLevel2SpecularVS)
	IMPLEMENT_SHADERCLASS(VoxelColorPS)
	IMPLEMENT_SHADERCLASS(VoxelColorFakeAOPS)
	IMPLEMENT_SHADERCLASS(VoxelColorFakeAOLevel2PS)		

	IMPLEMENT_SHADERCLASS(SpriteGrassVS)
	IMPLEMENT_SHADERCLASS(SpriteGrassZPassVS)
	IMPLEMENT_SHADERCLASS(SpriteGrassGPassVS)
	IMPLEMENT_SHADERCLASS(SpriteGrassGPassPS)
	IMPLEMENT_SHADERCLASS(SpriteGrassShadowMapVS)
	IMPLEMENT_SHADERCLASS(SpriteGrassFadeOutAmbLightingPS)
	IMPLEMENT_SHADERCLASS(SpriteGrassFadeOutDirLightingPS)

	IMPLEMENT_SHADERCLASS(CrossSpriteGrassVS)
	IMPLEMENT_SHADERCLASS(CrossSpriteGrassZPassVS)
	IMPLEMENT_SHADERCLASS(CrossSpriteGrassGPassVS)
	IMPLEMENT_SHADERCLASS(CrossSpriteGrassShadowMapVS)
	IMPLEMENT_SHADERCLASS(CrossSpriteGrassFadeOutVS)

	IMPLEMENT_SHADERCLASS(TerrainVS)
	IMPLEMENT_SHADERCLASS(TerrainSimplePS)
	IMPLEMENT_SHADERCLASS(TerrainZPassVS)
	IMPLEMENT_SHADERCLASS(TerrainGPassVS)
	IMPLEMENT_SHADERCLASS(TerrainGPassPS)
	IMPLEMENT_SHADERCLASS(TerrainShadowMapVS)
	IMPLEMENT_SHADERCLASS(TerrainSimpleLightingVS)
	IMPLEMENT_SHADERCLASS(TerrainSimpleLightingPS)
	
	IMPLEMENT_SHADERCLASS(SkyDomeVS)
	IMPLEMENT_SHADERCLASS(SkyDomePS)
	IMPLEMENT_SHADERCLASS(SkyLightPS)

	IMPLEMENT_SHADERCLASS(SkyBoxVS)	
}*/

LYNXENGCLASS LYNXFORCEINLINE bool operator == (const LYNXGUID& lhs, const LYNXGUID& rhs) {return lynxOSComapreGUID(&lhs, &rhs)?false:true; };	
LYNXENGCLASS LYNXFORCEINLINE bool operator != (const LYNXGUID& lhs, const LYNXGUID& rhs) {return lynxOSComapreGUID(&lhs, &rhs)?true:false; };	
LYNXENGCLASS LYNXFORCEINLINE bool operator < (const LYNXGUID& lhs, const LYNXGUID& rhs) {return (lynxOSComapreGUID(&lhs, &rhs)==-1)?true:false; };		
LYNXENGCLASS LYNXFORCEINLINE bool operator > (const LYNXGUID& lhs, const LYNXGUID& rhs) {return (lynxOSComapreGUID(&lhs, &rhs)== 1)?true:false; };		

#endif