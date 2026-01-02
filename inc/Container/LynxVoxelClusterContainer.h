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

#ifndef __LYNXVOXELCLUSTERCONTAINER_H__
#define __LYNXVOXELCLUSTERCONTAINER_H__

#include <GraphicsSystem/LynxRenderer.h>
#include <Container/LynxSprite3DContainer.h>

namespace LynxEngine 
{
	#define VA_GLOW											0x1

	struct VOXELDATA {
		Math::CVector3					Pos;	
		LYNXCOLORRGBA					Color;
		DWORD							AOCode;
		DWORD							AOCode2;
		int								AttributeColorIndex;
		LYNXDWORD						Attribute;	
		LYNXDWORD						VisFaces;
	};

	class LYNXENGCLASS CVoxelClusterCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CVoxelClusterCreationDesc);	
	public:						
		CArray<float>					m_VoxelScale;
		int								m_NumVoxelData;
		CArray<CString>					m_SeqFilename;
		DWORD							m_Dimension;
		Math::CVector3					m_LightDir;	
		LYNXBOOL						m_bOverwriteClusterSize; // TRUE to use the actual size from file instead of the size in desc
		LYNXBOOL						m_bReadOccludedVoxels;
		int								m_ClusterSize;		
		int								m_DataSize;
		VOXELDATA*						m_lpData;  		
		LYNXBOOL						m_bSpriteRender;

		CVoxelClusterCreationDesc() {m_VoxelScale.clear(); m_VoxelScale.push_back(1.0f); m_NumVoxelData = 1; m_SeqFilename.clear(); m_Dimension = 0; m_ClusterSize = 0; m_LightDir = Math::CVector3(0.5f, 3.0f, -1.0f); m_bOverwriteClusterSize = LYNX_TRUE; m_bReadOccludedVoxels = LYNX_FALSE; m_DataSize = 0; m_lpData = NULL; m_bSpriteRender = LYNX_FALSE;};
		~CVoxelClusterCreationDesc() {m_VoxelScale.clear(); m_SeqFilename.clear(); };

		void Default() {CCreationDesc::Default(); m_VoxelScale.clear(); m_VoxelScale.push_back(1.0f); m_NumVoxelData = 1; m_SeqFilename.clear(); m_Dimension = 0; m_ClusterSize = 0; m_LightDir = Math::CVector3(0.5f, 3.0f, -1.0f); m_bOverwriteClusterSize = LYNX_TRUE; m_bReadOccludedVoxels = LYNX_FALSE; m_DataSize = 0; m_lpData = NULL; m_bSpriteRender = LYNX_FALSE;};

		CVoxelClusterCreationDesc&		operator =(const CVoxelClusterCreationDesc& rhs)
		{
			*((CCreationDesc*)this) = (CCreationDesc&)rhs;
			m_VoxelScale = rhs.m_VoxelScale;
			m_NumVoxelData = rhs.m_NumVoxelData;
			m_SeqFilename = rhs.m_SeqFilename; 
			m_bOverwriteClusterSize = rhs.m_bOverwriteClusterSize;
			m_bReadOccludedVoxels = rhs.m_bReadOccludedVoxels;
			m_ClusterSize = rhs.m_ClusterSize;
			m_LightDir = rhs.m_LightDir;
			m_bSpriteRender = rhs.m_bSpriteRender;
			return (*this);
		}
	};				

	class LYNXENGCLASS CVoxelClusterContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CVoxelClusterContainer);
	public:
		static const int							POS_X = 1;
		static const int							NEG_X = (1 << 1);
		static const int							POS_Y = (1 << 2);
		static const int							NEG_Y = (1 << 3);
		static const int							POS_Z = (1 << 4);
		static const int							NEG_Z = (1 << 5);

		static const float							VOXEL_UNIT;
        static const float                          VOXEL_HALFSIZE;
		static int									m_BatchSize; 
		static int									m_SpriteBatchSize; 

		struct VOXELARRAY : public CUnknown{
			CArray<LYNXVECTOR3D*>					PosArray;		
			CArray<LYNX_CACHE_ALIGN float*>			ColorArray;
		#if __FAKE_VOXEL_AO__
			CArray<LYNX_CACHE_ALIGN DWORD*>			AOCodeArray;
		#endif
			CArray<LYNX_CACHE_ALIGN int*>			ColorIndexArray;

			VOXELARRAY()
			{				
			}

			~VOXELARRAY()
			{
				for (int i=0 ;i<PosArray.size(); i++)				
					LYNXGLOBAL_DEL_ARRAY(PosArray[i]);

				for (int i=0 ;i<ColorArray.size(); i++)				
					LYNXGLOBAL_DEL_ARRAY(ColorArray[i]);

				for (int i=0 ;i<ColorIndexArray.size(); i++)				
					LYNXGLOBAL_DEL_ARRAY(ColorIndexArray[i]);	

				PosArray.clear();
				ColorArray.clear();				
				ColorIndexArray.clear();

			#if __FAKE_VOXEL_AO__
				for (int i=0 ;i<AOCodeArray.size(); i++)				
					LYNXGLOBAL_DEL_ARRAY(AOCodeArray[i]);	

				AOCodeArray.clear();							
			#endif
			}
		};				
		typedef struct {
			int										NumVoxels;
			float									Scale;
			LYNXDWORD*								VoxelList;
			LYNXVECTOR3D*							VoxelPosArray;
			float*									VoxelColorArray;
		}GLOWVOXELDATA;
				
		LYNXBOOL									m_bSpriteRender;
	protected:			
		LYNXBOOL									m_bRenderOccludedVoxels;

		CVector3									m_Center;
		CArray<float>								m_VoxelScale;
		DWORD										m_Dimension;
		CArray<int>									m_ClusterSize;
		CArray<int>									m_RenderedSize;		
		CArray<int>									m_OccludedSize;	
		int											m_CurrentVoxelArray;
		GraphicsSystem::VOXELCLUSTERVERTEX*			m_lpV;
		LYNXBOOL									m_bInstancingVoxelArray;
		CCountedPtr<VOXELARRAY>						m_VoxelArray;		
		GraphicsSystem::CVertexArrayPtr				m_VertexArray;
		DWORD										m_NumBatchs;
		DWORD										m_DisplayListSize;
		GraphicsSystem::CIndexArrayPtr				m_DisplayIndexArray;		
		GraphicsSystem::CIndexArrayPtr				m_OptimizedVoxelIndexArray;		

		static GraphicsSystem::CVertexArrayPtr		m_SharedVertexArray;
		static GraphicsSystem::CIndexArrayPtr		m_SharedIndexArray;		

		static GraphicsSystem::CVertexLayoutPtr		m_SpriteVertexLayout;
		static GraphicsSystem::CVertexArrayPtr		m_SharedSpriteVertexArray;
		static GraphicsSystem::CIndexArrayPtr		m_SharedSpriteIndexArray;	

		CArray<Animation::CVoxelAnimation*>			m_VoxelAnimationArray;
		LYNXVECTOR3D*								m_PositionBuffer;
		float*										m_ColorBuffer;
		
		CArray<GLOWVOXELDATA>						m_GlowVoxelData;		
	public:		
		CVoxelClusterContainer(CEngine *lpengine);
		~CVoxelClusterContainer();				

		static LYNXFORCEINLINE GraphicsSystem::CVertexLayoutPtr& GetSpriteVoxelVertexLayout() {return m_SpriteVertexLayout;}

		//CVoxelClusterContainer&					operator =(const CVoxelClusterContainer& rhs);
		void										vInstance(const CContainer& rhs);

		virtual LYNXBOOL							vCreate(const CCreationDesc* desc);
		//virtual LYNXBOOL							vCreate(const LYNXCHAR *name, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);
		//virtual LYNXBOOL							vCreate(int num, int length, const LYNXCHAR *name, const LYNXCHAR *ext, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);				
		
        static DWORD                                GetVXLDimension(CStream* lpstream, const CString& filename);
		static DWORD                                GetVXLVoxelAmount(CStream* lpstream, const CString& filename, LYNXBOOL boccluded = LYNX_TRUE);

        void										RenderOccludedVoxels(LYNXBOOL b);

		LYNXFORCEINLINE float						GetVoxelScale(const int i) const {return m_VoxelScale[i]; }
		LYNXFORCEINLINE float						GetCurrentVoxelScale() const {return m_VoxelScale[m_CurrentVoxelArray]; }
		LYNXFORCEINLINE void						SetVoxelScale(const int i, const float s) {m_VoxelScale[i] = s; }
		LYNXFORCEINLINE void						SetCurrentVoxelArray(const int c) 
		{

			if (c < m_VoxelScale.size())
				m_CurrentVoxelArray = c; 
			else
				m_CurrentVoxelArray = m_VoxelScale.size() - 1;
        }
		LYNXFORCEINLINE int							GetCurrentVoxelArray() {return m_CurrentVoxelArray; }
		LYNXFORCEINLINE int							GetNumVoxelArray() {return m_ClusterSize.size(); }		

		virtual LYNXBOOL							vPlay(CReal skip);
		virtual void								vRender(CCameraContainer* cam = NULL);	
		virtual void								vGlowRender(CCameraContainer* cam = NULL);
		void										UpdateGlowVoxels(int v);		
		void										SpriteRender(CCameraContainer* cam = NULL);
		void										SpriteRenderGlowVoxels(CCameraContainer* cam = NULL);

        LYNXFORCEINLINE void						InstancingVoxelArray(LYNXBOOL b) {m_bInstancingVoxelArray = b; };
		LYNXFORCEINLINE DWORD						GetDimension() const {return m_Dimension; };
		void										SetDimension(const int d);
		LYNXINLINE int								GetClusterSize() const {return m_ClusterSize[m_CurrentVoxelArray]; };
		void										SetRenderedSize(int size);
		LYNXINLINE int								GetRenderedSize() const {return m_RenderedSize[m_CurrentVoxelArray]; };
		int											GetMaxRenderedSize() const;

		LYNXINLINE int								GetOccludedSize() const {return m_OccludedSize[m_CurrentVoxelArray]; };

		//LYNXINLINE void								Lock(int start = 0, int size = 0, GraphicsSystem::CRenderer::LOCKMODE flag = GraphicsSystem::CRenderer::LOCK_TO_WRITE) { m_lpV = (GraphicsSystem::SPRITE3DCLUSTERVERTEX*)m_VertexArray->vMap(start, size, flag); };
		//LYNXINLINE void								Unlock() { m_VertexArray->vUnmap(); m_lpV = NULL; };	
		void										vSetPosition(const int index, const CVector3& pos);		
		void										vGetPosition(const int i, Math::CVector3& pos) const;		
		
		LYNXFORCEINLINE void						vSetColor(const int index, LPLYNXCOLORRGBA lpc) {SetColor(index, lpc); };				
		LYNXFORCEINLINE void						vGetColor(const int index, LPLYNXCOLORRGBA lpc) const {GetColor(index, lpc); };

		void										SetColorIndex(const int v, const int index);
		LYNXFORCEINLINE int							GetColorIndex(const int voxel) {return m_VoxelArray->ColorIndexArray[m_CurrentVoxelArray][voxel]; }

		void										SetAOCode(int index, DWORD code);				
		void										SetAOCode2(int index, DWORD code);

		LYNXFORCEINLINE LYNXVECTOR3D*				PosArray() {return m_VoxelArray->PosArray[m_CurrentVoxelArray]; }; 	
		LYNXFORCEINLINE LYNXVECTOR3D*				GlowPosArray() { return m_GlowVoxelData[m_CurrentVoxelArray].VoxelPosArray; };
		LYNXFORCEINLINE void						GetPosArray(LYNXVECTOR3D* dst, int s, int num) {lynxMemCpy(dst, &(m_VoxelArray->PosArray[m_CurrentVoxelArray][s]), sizeof(LYNXVECTOR3D)*num); }; 	
		LYNXFORCEINLINE void						SetPosArray(int s, int num, LYNXVECTOR3D* src) {lynxMemCpy(&(m_VoxelArray->PosArray[m_CurrentVoxelArray][s]), src, sizeof(LYNXVECTOR3D)*num); }; 	
		LYNXFORCEINLINE void						SetPosArray(CVoxelClusterContainer* src) {lynxMemCpy(m_VoxelArray->PosArray[m_CurrentVoxelArray], src->m_VoxelArray->PosArray[m_CurrentVoxelArray], sizeof(LYNXVECTOR3D)*m_ClusterSize[m_CurrentVoxelArray]); }; 	

		void										SetColor(const int index, LPLYNXCOLORRGBA lpc);				
		void										SetColorAlpha(const int index, LYNXBYTE a);
		void										SetColor(LPLYNXCOLORRGBA lpc);				
		void										SetColorAlpha(LYNXBYTE a);
		void										GetColor(const int index, LPLYNXCOLORRGBA lpc) const;					
		LYNXFORCEINLINE float*						ColorArray() {return m_VoxelArray->ColorArray[m_CurrentVoxelArray]; };
		LYNXFORCEINLINE float*						GlowColorArray() { return m_GlowVoxelData[m_CurrentVoxelArray].VoxelColorArray; };
		LYNXFORCEINLINE void						GetColorArray(float* dst, int s, int num) {lynxMemCpy(dst, &(m_VoxelArray->ColorArray[m_CurrentVoxelArray][s*4]), sizeof(float)*num*4); }; 	
		LYNXFORCEINLINE void						SetColorArray(int s, int num, float* src) {lynxMemCpy(&(m_VoxelArray->ColorArray[m_CurrentVoxelArray][s*4]), src, sizeof(float)*num*4); }; 	
		LYNXFORCEINLINE void						SetColorArray(CVoxelClusterContainer* src) {lynxMemCpy(m_VoxelArray->ColorArray[m_CurrentVoxelArray], src->m_VoxelArray->ColorArray[m_CurrentVoxelArray], sizeof(float)*m_ClusterSize[m_CurrentVoxelArray]*4); }; 	

		LYNXFORCEINLINE DWORD*						AOCodeArray() {return m_VoxelArray->AOCodeArray[m_CurrentVoxelArray]; };
	#ifdef __DESKTOP__
		LYNXFORCEINLINE void						SetAOCodeArray(int s, int num, int* src) {lynxMemCpy(&(m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][s*2]), src, sizeof(DWORD)*num*2); };
		LYNXFORCEINLINE void						SetAOCodeArray(CVoxelClusterContainer* src) {lynxMemCpy(m_VoxelArray->AOCodeArray[m_CurrentVoxelArray], src->m_VoxelArray->AOCodeArray[m_CurrentVoxelArray], sizeof(DWORD)*m_ClusterSize[m_CurrentVoxelArray]*2); };	
	#else
		LYNXFORCEINLINE void						SetAOCodeArray(int s, int num, int* src) {lynxMemCpy(&(m_VoxelArray->AOCodeArray[m_CurrentVoxelArray][s*4]), src, sizeof(DWORD)*num*4); };
		LYNXFORCEINLINE void						SetAOCodeArray(CVoxelClusterContainer* src) {lynxMemCpy(m_VoxelArray->AOCodeArray[m_CurrentVoxelArray], src->m_VoxelArray->AOCodeArray[m_CurrentVoxelArray], sizeof(DWORD)*m_ClusterSize[m_CurrentVoxelArray]*4); };
	#endif

		LYNXFORCEINLINE void						CopyAll(CVoxelClusterContainer* src) {lynxMemCpy(m_VoxelArray->PosArray[m_CurrentVoxelArray], src->m_VoxelArray->PosArray[m_CurrentVoxelArray], sizeof(LYNXVECTOR3D)*m_ClusterSize[m_CurrentVoxelArray]); lynxMemCpy(m_VoxelArray->ColorArray[m_CurrentVoxelArray], src->m_VoxelArray->ColorArray[m_CurrentVoxelArray], sizeof(float)*m_ClusterSize[m_CurrentVoxelArray]*4);}; 	

		Animation::CVoxelAnimation*					AddVoxelAnimation();
		LYNXFORCEINLINE Animation::CVoxelAnimation*	GetVoxelAnimation(int i) { return m_VoxelAnimationArray[i]; };
		Animation::CVoxelAnimation*					GetVoxelAnimationByVoxel(int voxel);
		LYNXFORCEINLINE void						SetVoxelColorBuffer(float* b) {m_ColorBuffer = b; };
		LYNXFORCEINLINE void						SetVoxelPositionBuffer(LYNXVECTOR3D* b) {m_PositionBuffer = b; };

		static LYNXFORCEINLINE void					ReleaseSharedStaticResource() {	m_SharedVertexArray.Reset(); m_SharedIndexArray.Reset(); m_SpriteVertexLayout.Reset(); m_SharedSpriteVertexArray.Reset(); m_SharedSpriteIndexArray.Reset(); };

		LYNXBOOL									ExportOBJFile(const CString& filename);	

		void										AddGlowVoxels(int v, CArray<int>& list, float scale = 1.0f);
		LYNXFORCEINLINE CArray<GLOWVOXELDATA>&		GetGlowVoxelArray() {return m_GlowVoxelData; }
		void										LoadVoxelAttribute(const CString& name, const CString& filename, int voxelarrayindex);
	protected:
		LYNXBOOL									LoadVXL(CStream* lpstream, const CString& filename, CVoxelClusterCreationDesc* desc);		
		void										WriteOBJCube(CPlatformFileStream& fs, int voxel, int v_base, int nv_base);

		friend class GraphicsSystem::CGraphicsSystem;
	};
}

#endif