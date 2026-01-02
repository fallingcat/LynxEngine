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

#include <LynxEngineClasses.h>
#include <LynxResource.h>
#include <Animation/LynxAnimation.h>
#include <Animation/LynxAnimationSet.h>
#include <Animation/LynxAnimationTree.h>
#include <LynxCreationDesc.h>
#include <CollisionSystem/LynxAABB.h>

using namespace LynxEngine::Math;

namespace LynxEngine 
{	
	class LYNXENGCLASS CContainer : public CResource
	{
		LYNX_DECLARE_CLASS(CContainer);
	public:
		// {6B68A76B-021D-4e0b-8A5F-6DFED74AD40D}
		static const LYNXGUID NULL_CONTAINER; 
		// {A77D81CA-556A-4079-B7F6-C203ACB180DE}
		static const LYNXGUID REFERENCE;
		// {D83E2495-73F0-4a5e-B1F5-7DCD7F05C353}
		static const LYNXGUID GEOMETRY;
		// {804FB2E3-E365-42d0-9B18-7FC3A69FA86D}
		static const LYNXGUID STATIC_MODEL;
		// {1B55E9C6-5CC6-42d6-8F3B-9D03717D06F3}
		static const LYNXGUID TERRAIN;
		// {9C80FA2C-FF96-4f47-BEEE-4D8C76B1E621}
		static const LYNXGUID PROGRESSIVE_TERRAIN;
		// {1044DA13-EE71-45e2-91DC-D77A25F02455}
		static const LYNXGUID SKYDOME;
		// {CAB00AC2-BA7A-48f6-9276-6FCBEA0E4CC9}
		static const LYNXGUID VOLUMETRIC_CLOUD;
		// {52224C72-E9E8-4dcc-B030-3BBDD1188242}
		static const LYNXGUID TREE;
		// {FE2D61D6-915C-4c64-8630-B69FF0747C65}
		static const LYNXGUID GRASS;
		// {B040C6FE-CE4B-46cb-B6D9-2D7CB3786244}
		static const LYNXGUID CROSS_GRASS; 
		// {8F0501A6-3E7C-43cc-86FE-66FC675E2496}
		static const LYNXGUID CAMERA;
		// {0DE31F8C-5597-4d48-B460-D4F997817E0B}
		static const LYNXGUID MODEL;
		// {4F9CEDCE-ACE5-4366-9C5A-95A6E75E15FF}
		static const LYNXGUID SPRITE;
		// {B0DDAC75-DA19-40d2-8AB9-81B332ADB976}
		static const LYNXGUID SPRITE3D;
		// {EB6B3EDE-6E9D-4bd0-B34B-E578EEAEE0DB}
		static const LYNXGUID SPRITE3D_CLUSTER;
		// {B3C3909E-BAD0-4221-9D4B-B668B94718E6}
		static const LYNXGUID SPRITE_CLUSTER; 
		// {D2132032-37A8-46a2-A742-0F8D721380B3}
		static const LYNXGUID VOXEL_CLUSTER;		
		// {CC8EE7B1-10D3-43ca-A1D0-EB582271775A}
		static const LYNXGUID CLUSTER_SUB;
		// {1C617B92-9AB9-4aab-9DBF-08583D286FD0}
		static const LYNXGUID POINT_SPRITE;
		// {F6AC9E26-410E-4fb2-B6DA-582B0C87D0FA}
		static const LYNXGUID CROSS_SPRITE;
		// {7E2EB7A9-F644-40a9-996F-AEB3260E9D36}
		static const LYNXGUID PEMITTER;
		// {4903AFE5-DCA1-407f-8E9C-E379D07C6C70}
		static const LYNXGUID MESH;
		// {9182D653-D80D-4f06-83B3-3F712038859D}
		static const LYNXGUID BONE;
		// {5045D040-8036-4a60-8244-4C4ED3D84CEE}
		static const LYNXGUID PORTAL; 
			
		typedef LYNXGUID					TYPE;
		
		/*		
		typedef enum {
			NULL_CONTAINER = -1,
			REFERENCE = 0,			
			GEOMETRY,
			STATIC_MODEL,
			TERRAIN,
			PROGRESSIVE_TERRAIN,
			SKYDOME,
			VOLUMETRIC_CLOUD,
			TREE,
			GRASS,
			CROSS_GRASS,
			CAMERA,
			MODEL,
			SPRITE,
			SPRITE3D,
			SPRITE3D_CLUSTER,
			POINT_SPRITE,
			CROSS_SPRITE,	
			PEMITTER,	
			MESH,
			BONE,			
			PORTAL,
			NUM_TYPE,
		}TYPE;
		*/

		enum SORTMODE{
			SM_NONE = 0,
			SM_DISTANCE,
			SM_MATERIAL,
		};				

		enum COLLISIONOBJTYPE{
			COT_CAPSULE = 0,
			COT_OBB,		
			COT_SPHERE,
		};				
				
		static const int MAX_COLLISION_LEVELS = 5;

		DECLAR_PARAM_BOOL(m_bSort)
		DECLAR_PARAM_BOOL(m_bTranslucent)
		DECLAR_PARAM_BOOL(m_bCastShadow)
		DECLAR_PARAM_BOOL(m_bReceiveShadow)
		DECLAR_PARAM_BOOL(m_bOccluder)
		DECLAR_PARAM_BOOL(m_bSelfIlluminated)
		DECLAR_PARAM_FLOAT(m_IlluminatingIntensity)
		DECLAR_PARAM_BOOL(m_bMotionBlur)
		DECLAR_PARAM_BOOL(m_bReflect)
		DECLAR_PARAM_BOOL(m_bRefract)
		DECLAR_PARAM_FLOAT(m_MinLODDistance)
		DECLAR_PARAM_FLOAT(m_MaxLODDistance)

		int											m_CurrentLOD;
		DECLAR_PARAM_INT(m_LODBias)

		LYNXPLANE									m_RNRPlane;			
		Math::CReal									m_Visibility;		
		LYNXBOOL									m_bUseVisibilty;

		SORTMODE									m_SortMode;

		//Animation related data		
		Animation::CAnimationSetPtr					m_AnimationSet;	
		Animation::CAnimationTree					m_AnimationTree;	

		CollisionSystem::CCollisionHierarchy*		m_lpCollisionHierarchy;
		//LYNXBOOL									m_bSkipTransfomScale;

		LYNXCOLORRGBA								m_Diffuse;

	protected:					
		TYPE										m_ContainerType;						
		LYNXBOOL									m_bRenderable;			/*!< is renderable container or not */ 
		Math::CMatrix4								m_LocalM;				/*!< Local transformation Matrix */			
		Math::CMatrix4								m_GlobalM;				/*!< Global transformation Matrix */	
		//CBufferingObj<Math::CMatrix4>				m_M;	
		Math::CMatrix4								m_M;	
		Math::CMatrix4								m_AnimationM;	
		Math::CMatrix4								m_TextureUVM;	

		CReal										m_Dist2Viewer;

		Math::CMatrix4								m_PushedLocalM;			
		Math::CMatrix4								m_PushedGlobalM;		
		Math::CMatrix4								m_PushedM;	

		Math::CMatrix4								m_PivotM;				/*!< transfrom matrix from center to pivot */
		Math::CMatrix4								m_InversePivotM;
		Math::CMatrix4								m_LastM;			
		Math::CMatrix4								m_LastWVPM;

		Math::CVector3*								m_lpCenter; 		
		Math::CVector3*								m_lpDirection;			/*!< Object's World tramsformed Direction */
		Math::CVector3*								m_lpUpDir;				/*!< Object's World tramsformed Up Direction	*/	
		Math::CVector3*								m_lpRightDir;			/*!< Object's World tramsformed Right Direction */	
		
		DWORD										m_DrawnPixels;
		LYNXBOOL									m_bOccluded;
		GraphicsSystem::CQuery*						m_lpOcclusionQuery;
		GraphicsSystem::CDepthStencilStatePtr		m_OcclusionDepthStencilState;		
		CList<CObjProxy*>							m_ProxyList;
		CollisionSystem::CAABB						m_ScreenSpaceAABB;

		//Hierarchy related data
		CContainer*									m_lpParent;
		CArray<CContainer*>							m_lpChildrenArray;		
		
		DWORD										m_NumLODs;
		LYNXBOUNDARYBOX								m_LocalAABB;

		//DECLAR_PARAM_LPOBJ(CRenderableObj*, m_lpRenObj);		
		CRenderableObj*								m_lpRenObj;
		DECLAR_PARAM_LPOBJ(CContainer, m_lpParentContainer)
		CList<CContainer*>							m_SubContainerList;		

		//DECLAR_PARAM_LPOBJ(MaterialSystem::CMaterial, m_lpCurrentMaterial)
		DECLAR_PARAM_COUNTEDPTR(MaterialSystem::CMaterial, m_lpCurrentMaterial)
		int											m_CurrentMaterialTechnique;		

		LYNXBOOL									m_bGlowDataDirty;
	private:
		void										InitMembers();
	public:		
		CContainer(CEngine *lpengine);
		virtual ~CContainer();

		DECLARE_CLASSSCRIPT
		LYNXBOOL									vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
		LYNXINLINE void								_Render(CCameraContainer* cam) {vRender(cam); };
		LYNXINLINE LYNXBOOL							_Play(CReal step) {return vPlay(step); };
		CCameraContainer*							CCameraContainer_Cast(CContainer* c);
		CGeometryContainer*							CGeometryContainer_Cast(CContainer* c);
		CPEmitterContainer*							CPEmitterContainer_Cast(CContainer* c);
		static const LYNXGUID						ScriptType2EngineType(int type);
		Animation::CAnimation*						CreateAnimation(const LYNXCHAR* name);
		void										_SetCurrentAnimation(Animation::CAnimation* lpa) {SetCurrentAnimation(lpa); };
		
		CContainer&									operator=(CContainer& rhs);						
		LYNXFORCEINLINE bool						operator < (const CContainer& rhs) { return (m_Dist2Viewer < rhs.m_Dist2Viewer); };		
		LYNXFORCEINLINE bool						operator > (const CContainer& rhs) { return (m_Dist2Viewer > rhs.m_Dist2Viewer); };		
		LYNXFORCEINLINE bool						operator == (const CContainer& rhs) { return (m_Dist2Viewer == rhs.m_Dist2Viewer); };				
		LYNXFORCEINLINE const float					GetDist2Viewer() const {return m_Dist2Viewer; };

		static LYNXINLINE bool						compare_front2back(CContainer* e1, CContainer* e2) { return (*e1) < (*e2); };
		static LYNXINLINE bool						compare_back2front(CContainer* e1, CContainer* e2) { return (*e1) > (*e2);	};
		static LYNXINLINE bool						compare_material(CContainer* e1, CContainer* e2) { return (e1->GetlpCurrentMaterial()) > (e2->GetlpCurrentMaterial()); };
		static bool									compare_depthlayer_f2b(CContainer* e1, CContainer* e2);
		static bool									compare_depthlayer_b2f(CContainer* e1, CContainer* e2);
		
		LYNXINLINE void								CalculateDist(CVector3& v) {CVector3 D; vGetPosition(D); D -= v; m_Dist2Viewer = Dot(D, D); };		

		LYNXFORCEINLINE void						SetGlowDataDirty(LYNXBOOL b) { m_bGlowDataDirty = b; }

		virtual void								vUseVisibility(LYNXBOOL b) {m_bUseVisibilty = b; };
		virtual void								vInstance(const CContainer& rhs);
		LYNXINLINE const TYPE						GetContainerType() {return m_ContainerType; };
		
		LYNXFORCEINLINE const LYNXBOOL				IsRenderable() const {return m_bRenderable; };

		virtual void								vSetNumLODs(int lod) {m_NumLODs = lod; };
		virtual void								vAddLOD() {m_NumLODs++; };
		LYNXINLINE DWORD							GetNumLODs() const {return m_NumLODs; };
		LYNXINLINE void								SetCurrentLOD(int lod) {m_CurrentLOD = lod; };	
		virtual	void								vCalculateLOD(CCameraContainer* cam);

		void										SetlpRenObj(CRenderableObj* lpp);		
		LYNXINLINE CRenderableObj*					GetlpRenObj() const {return m_lpRenObj;};

		virtual Animation::CAnimation*				vCreateAnimation();
		virtual Animation::CAnimation*				vLoadAnimation(const CCreationDesc* desc);
		Animation::CAnimation*						LoadAnimation(const LYNXCHAR* filename, CStream* s = NULL);
		Animation::CAnimation*						LoadAnimation(const LYNXCHAR* name, const LYNXCHAR* filename);
		void										RenameAnimation(Animation::CAnimation* lpani, LynxEngine::CString& name);
		void										RenameAnimation(Animation::CAnimation* lpani, const LYNXCHAR* name);
		void										AddAnimationKey(int set, Animation::CAnimation::KEYSTYPE type, int num, Animation::ANIMATIONKEYDATA* key);
		void										AddAnimationKey(const CString& name, Animation::CAnimation::KEYSTYPE type, int num, Animation::ANIMATIONKEYDATA* key);
		void										SetCurrentAnimationUseIntTime(LYNXBOOL b);


		LYNXBOUNDARYBOX&							GetLocalAABB() { return m_LocalAABB; }

		//void										CreateFrame(DWORD num);
		//LYNXINLINE CArray<Animation::CAnimationFrame>& GetFrameArray(void) {return m_FrameArray;};
		//LYNXINLINE Animation::CAnimationFrame*		GetlpCurrentFrame(void) {return m_lpCurrentFrame;};
		//LYNXINLINE int								GetCurrentFrameID(void) {return m_lpCurrentFrame->m_ID;};
		//LYNXINLINE const char*						GetCurrentFrameName(void) {return m_lpCurrentFrame->m_Name;};

		LYNXINLINE const Math::CMatrix4&			GetTextureUVMatrix() {return m_TextureUVM; };	
		LYNXINLINE void								SetTextureUVMatrix(const Math::CMatrix4& m) {m_TextureUVM = m; };		
		
		LYNXINLINE const Math::CMatrix4&			GetTransformMatrix() {return m_M; };	
		LYNXINLINE void								SetTransformMatrix(const Math::CMatrix4& m) {m_M = m; };		
		LYNXINLINE void								SetTransformMatrix(const Math::CMatrix4& m1, const Math::CMatrix4& m2) {Math::MatrixXMatrix(m_M, m1, m2); };		
		void										SaveTransformationMatrix();
		LYNXINLINE void								SetLastWVPM(const Math::CMatrix4& m) {m_LastWVPM = m; };		
		LYNXINLINE const Math::CMatrix4&			GetLastWVPM() {return m_LastWVPM; };		

		LYNXINLINE const Math::CMatrix4&			GetLocalMatrix() {return m_LocalM; };		
		LYNXINLINE void								SetLocalMatrix(const Math::CMatrix4& m) {m_LocalM = m; };		
		LYNXINLINE void								SetLocalMatrix(const Math::CMatrix4& m1, const Math::CMatrix4& m2) {Math::MatrixXMatrix(m_LocalM, m1, m2); };		

		LYNXINLINE void								SetAnimationMatrix(const Math::CMatrix4& m) {m_AnimationM = m; };
		LYNXINLINE void								SetAnimationMatrix(const Math::CMatrix4& m1, const Math::CMatrix4& m2) {Math::MatrixXMatrix(m_AnimationM, m1, m2); };		
		LYNXINLINE const Math::CMatrix4&			GetAnimationMatrix() {return m_AnimationM; };				

		LYNXINLINE const Math::CMatrix4&			GetGlobalMatrix() {return m_GlobalM; };		
		LYNXINLINE void								SetGlobalMatrix(const Math::CMatrix4& m) {m_GlobalM = m; };

		LYNXINLINE const Math::CMatrix4&			GetPivotMatrix() const {return m_PivotM; };	
		LYNXINLINE void								SetPivotMatrix(const Math::CMatrix4& m) {m_PivotM = m; };		

		LYNXINLINE const Math::CMatrix4&			GetInversePivotMatrix() const {return m_InversePivotM; };	
		LYNXINLINE void								SetInversePivotMatrix(const Math::CMatrix4& m) {m_InversePivotM = m; };		

	#ifdef __TOOL__
		virtual void								UpdateTransform();	
	#else
		void										UpdateTransform();
	#endif
		void										ResetTransform();
		void										ResetOrientation(LYNXBOOL breset_scale = LYNX_TRUE);
		LYNXINLINE void								ResetLocalMatrix() {LYNX_IDENTITY_MATRIX(m_LocalM); };
		LYNXINLINE void								ResetGlobalMatrix() {LYNX_IDENTITY_MATRIX(m_GlobalM); };
		void										PushTransform();
		void										PopTransform();
		
		virtual void								vSetPosition(const Math::CVector3& pos);
		virtual void								vGetPosition(Math::CVector3& pos) const;
		virtual void								vSetPosition(const int index, const Math::CVector3& pos) {};		
		virtual void								vGetPosition(const int i, Math::CVector3& pos) const {};
		virtual void								vGetScreenPosition(Math::CVector2& pos, CCameraContainer* cam, int w, int h) const;
		virtual void								vGetScreenPosition(Math::CVector2& pos, CCameraContainer* cam) const;
		virtual void								vGetOBBScreenRect(LYNXRECT& rect, CCameraContainer& cam);

		virtual void								vSetSize(const Math::CReal& w, const Math::CReal& h) {}; //should be pure virtual function
		virtual void								vGetSize(Math::CReal& w, Math::CReal& h) const {}; //should be pure virtual function
		virtual void								vSetSize(const int i, const Math::CReal& w, const Math::CReal& h) {}; //should be pure virtual function
		virtual void								vGetSize(const int i, Math::CReal& w, Math::CReal& h) const {}; //should be pure virtual function

		virtual void								vSetColor(LPLYNXCOLORRGBA lpc) {};				
		virtual void								vGetColor(LPLYNXCOLORRGBA lpc) const {};				
		virtual void								vSetColor(const int index, LPLYNXCOLORRGBA lpc) {};				
		virtual void								vGetColor(const int index, LPLYNXCOLORRGBA lpc) const {};				

		virtual void								vSetUV(LPLYNXUVPOINT lpuv) {};				
		virtual void								vGetUV(LPLYNXUVPOINT lpuv) const {};				
		virtual void								vSetUV(const int index, LPLYNXUVPOINT lpuv) {};				
		virtual void								vGetUV(const int index, LPLYNXUVPOINT lpuv) const {};				

		LYNXINLINE void								GetDirection(Math::CVector3& dir) {LYNX_VECTOR3D((dir), m_lpDirection->x, m_lpDirection->y, m_lpDirection->z); dir.Normalise(3); };
		LYNXINLINE void								GetUpDirection(Math::CVector3& pos) {LYNX_VECTOR3D((pos), m_lpUpDir->x, m_lpUpDir->y, m_lpUpDir->z); pos.Normalise(3);};
		LYNXINLINE void								GetRightDirection(Math::CVector3& pos) {LYNX_VECTOR3D((pos), m_lpRightDir->x, m_lpRightDir->y, m_lpRightDir->z); pos.Normalise(3);};		
		LYNXINLINE const Math::CVector3&			GetDirection() {return (*m_lpDirection); };
		LYNXINLINE const Math::CVector3&			GetUpDirection() {return (*m_lpUpDir); };
		LYNXINLINE const Math::CVector3&			GetRightDirection() {return (*m_lpRightDir); };		
		void										Forward(const Math::CReal dist);
		void										SideMove(const Math::CReal dist);
		void										Up(const Math::CReal dist);
		void										Scale(const Math::CVector3& v, LYNXMATHORDER order);
		void										SetScale(const Math::CVector3& v);
		void										Translate(const Math::CVector3& v, LYNXMATHORDER order);
		void										Rotate(const Math::CReal f, LYNXAXISTYPE axis, LYNXMATHORDER order);
		void										Rotate(const Math::CReal ang, const Math::CVector3& axis, LYNXMATHORDER order);
		void										Rotate(const Math::CVector3& euler, LYNXMATHORDER order);
		LYNXINLINE void								Yaw(const Math::CReal deg, LYNXMATHORDER order) {Rotate(deg, LYNX_Y_AXIS, order); };
		LYNXINLINE void								Roll(const Math::CReal deg, LYNXMATHORDER order) {Rotate(deg, LYNX_Z_AXIS, order); };
		LYNXINLINE void								Pitch(const Math::CReal deg, LYNXMATHORDER order) {Rotate(deg, LYNX_X_AXIS, order); };
		void										MulMatrix(const Math::CMatrix4& m, LYNXMATHORDER order);		
		void										LookAt(const Math::CVector3& v);
		
		virtual	void								vAttachToTerrain(CTerrainContainer& t, LYNXBOOL balign);
		virtual void								vComputeProxy() {};
		void										ComputeProxyScreenSpaceAABB(CCameraContainer* cam);
		LYNXINLINE CList<CObjProxy*>&				GetProxyList() {return m_ProxyList; };
		LYNXINLINE GraphicsSystem::CQuery*			GetlpOcclusionQuery() {return m_lpOcclusionQuery; };

		LYNXINLINE void								AddAnimation(Animation::CAnimation* a) {m_AnimationSet->AddAnimation(a); };	
		void										SetCurrentAnimationRange(const float s, const float e, const float step, const PLAYMODE mode);
		void										SetCurrentAnimation(Animation::CAnimation* lpa);
		Animation::CAnimation*						SetCurrentAnimation(const CString name);		
		void										SetAnimationName(Animation::CAnimation* lpa, CString& name);
		LYNXINLINE Math::CReal						GetTime(void) {return m_AnimationSet->GetCurrentAnimation()->m_Time; };
		LYNXINLINE void								SetTime(const Math::CReal t) {m_AnimationSet->GetCurrentAnimation()->m_Time = t; };		
		//LYNXINLINE LYNXREAL							GetStartTime(void) {return m_lpCurrentAnimation->m_StartTime; };
		//LYNXINLINE void								SetStartTime(LYNXREAL t) {m_lpCurrentAnimation->m_StartTime = t; };
		//LYNXINLINE LYNXREAL							GetEndTime(void) {return m_lpCurrentAnimation->m_EndTime;};		
		//LYNXINLINE void								SetEndTime(LYNXREAL t) {m_lpCurrentAnimation->m_EndTime = t;};		
		//LYNXINLINE void								SetTimes(LYNXREAL st, LYNXREAL et, LYNXREAL t) {m_lpCurrentAnimation->m_StartTime = st; m_lpCurrentAnimation->m_EndTime = et; m_lpCurrentAnimation->m_Time = t;};		

        void                                        Sort(LYNXBOOL b);        
		
		LYNXINLINE void								SetlpParent(CContainer* c) {m_lpParent = c; };			
		void										ResetChildrenArray();	
		void										ClearChildrenArray();
		LYNXINLINE void								AddChild(CContainer* c) 
		{
			if (LynxEngine::find(m_lpChildrenArray.begin(), m_lpChildrenArray.end(), c) == m_lpChildrenArray.end())
			{
				m_lpChildrenArray.push_back(c);
			}
			c->SetlpParent(this);
		}

		LYNXINLINE void								SetChild(int i, CContainer* c) {m_lpChildrenArray[i] = c; c->SetlpParent(this); };							
		void										RemoveChild(CContainer* c);
		LYNXINLINE CContainer*						GetlpParent() const {return m_lpParent; };			
		LYNXINLINE DWORD							GetNumChildren(void) {return (DWORD)m_lpChildrenArray.size(); };
		LYNXINLINE CContainer*						GetlpChild(int i) {return m_lpChildrenArray[i]; };		
		LYNXINLINE void								SetlpParentContainer(CContainer* c) {m_lpParentContainer = c; };
		LYNXINLINE CContainer*						GetlpParentContainer() const {return m_lpParentContainer; };

		LYNXINLINE CList<CContainer*>&				GetSubContainerList() {return m_SubContainerList; };
		LYNXINLINE DWORD							GetNumSubContainers(void) {return (DWORD)m_SubContainerList.size(); };
		void										AddSubContainer(CContainer* c);
		void										RemoveSubContainer(CContainer* c);
		CContainer*									FindContainer(const CString& name);
		
		
		//LYNXINLINE void								CreateMaterialArray(int num) {m_MaterialArray.resize(num); };
		//LYNXINLINE void								SetMaterial(MATERIALTYPE i, MaterialSystem::CMaterial* lpm) {m_MaterialArray[i] = lpm; };
		//LYNXINLINE MaterialSystem::CMaterial*		GetMaterial(MATERIALTYPE i) { return m_MaterialArray[i]; };
		

		LYNXINLINE int								GetCurrentMaterialTechnique() const {return m_CurrentMaterialTechnique; };
		void										SetCurrentMaterialTechnique(int p);		
		void										SetCurrentLight(SceneSystem::CLight& lit);		
		//LYNXINLINE MaterialSystem::CMaterial* const GetlpCurrentMaterial() {return m_lpCurrentMaterial; };
		//LYNXINLINE void								SetlpCurrentMaterial(MaterialSystem::CMaterial* const lpm) {m_lpCurrentMaterial = lpm; };
		LYNXINLINE MaterialSystem::CMaterialPtr&	GetlpCurrentMaterial() {return m_lpCurrentMaterial; };
		LYNXINLINE void								SetlpCurrentMaterial(const MaterialSystem::CMaterialPtr& lpm) {m_lpCurrentMaterial = lpm; };
		void										SetlpCurrentMaterialToAllSubContainers(const MaterialSystem::CMaterialPtr& lpm);
		void										AddToCurrentMaterialRenderList();
		void										AddToRenderList(CList<CContainer*>& list);
		
		LYNXINLINE CollisionSystem::CCollisionHierarchy* GetlpCollisionHierarchy() { return m_lpCollisionHierarchy; };			

		void										SetSelfIlluminated(LYNXBOOL b);
		void										SetMotionBlur(LYNXBOOL b);
		void										SetReflective(LYNXBOOL b);
		LYNXBOOL									IsReflective();
		void										SetRefractive(LYNXBOOL b);
		LYNXBOOL									IsRefractive();
		void										SetIlluminatingIntensity(CReal b);

		virtual LYNXBOOL							vCreate(const CCreationDesc* desc);
		virtual void								vCreateDefaultResource() {};		
		//LYNXINLINE virtual LYNXBOOL					vCreate(const LYNXCHAR *name, int group_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbufferobject = LYNX_FALSE) {return LYNX_TRUE; };						
		//virtual LYNXBOOL							vPlay(LYNXREAL skip, LYNXPLAYMODE mode);
		virtual LYNXBOOL							vPlay(CReal step);// {return LYNX_TRUE; };
		virtual void								vRagDollUpdate(OBJUPDATETYPE type) {};
		LYNXINLINE virtual void						vPreRender(CCameraContainer* cam = NULL) {};		
		LYNXINLINE virtual void						vRender(CCameraContainer* cam = NULL) {};
		LYNXINLINE virtual void						vGlowRender(CCameraContainer* cam = NULL) {};
		LYNXINLINE virtual void						vRender(CCameraContainer* cam, GraphicsSystem::CVertexLayoutPtr& v, GraphicsSystem::CShaderClassPtr& vs) {};
		LYNXINLINE virtual void						vRender(MaterialSystem::CPass* pass, CCameraContainer* cam, GraphicsSystem::CVertexLayoutPtr& v, GraphicsSystem::CShaderClassPtr& vs) {};		
		LYNXINLINE virtual void						vPostRender(CCameraContainer* cam = NULL) {};
		LYNXINLINE virtual void						vTranslucentRender(CCameraContainer* cam) {return vRender(cam); };

		LYNXINLINE LYNXBOOL							IsOccluded() {return m_bOccluded; };
		void										OcclusionRender(CCameraContainer* cam);

		virtual void								vGetCenterAndRadius(Math::CVector3& center, Math::CReal& radius);

		LYNXINLINE virtual void						vLighting(CCameraContainer* cam, SceneSystem::CLight* lit) {};

		LYNXINLINE virtual void						vUpdateVertices(int lod) {};
		LYNXINLINE virtual LYNXBOOL					vSetupTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc, int lod = 0, LYNXBOOL bapply_world = LYNX_TRUE) {return LYNX_FALSE; };
		LYNXINLINE virtual void						vReleaseTriMeshShapeDesc(PhysicsSystem::CTriMeshShapeDesc* desc) {};
		LYNXINLINE virtual LYNXBOOL					vSetupHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc, int lod = 0, LYNXBOOL bapply_world = LYNX_TRUE) {return LYNX_FALSE; };
		LYNXINLINE virtual void						vReleaseHullShapeDesc(PhysicsSystem::CHullShapeDesc* desc) {};

		LYNXBOOL									vLoad(CUserProfile* file, CXMLElement* c);
		LYNXBOOL									vSave(CUserProfile* file, CXMLElement* c);
	protected:
		void										RecursionTraverse(CArray<CContainer*>& a, int* i, CContainer* c);
		void										MakeTraverseArray(CArray<CContainer*>& a, int size, CContainer* root);

	friend class CModelContainer;
	};

	template <LYNXBOOL bDecending>
	struct SortByDist
	{  		
		bool operator()(const CContainer* lhs, const CContainer* rhs)
		{
			if (bDecending)
				return (lhs->GetDist2Viewer() > rhs->GetDist2Viewer());
			else
				return (lhs->GetDist2Viewer() < rhs->GetDist2Viewer());   	
		}
	};

	LYNXENGCLASS CString							GetEnumStr(CContainer::TYPE e);
	LYNXENGCLASS CContainer::TYPE					TranslateContainerType(const CString& s);
}

#endif