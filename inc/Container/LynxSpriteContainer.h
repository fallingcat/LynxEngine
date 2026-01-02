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

#ifndef __LYNXSPRITECONTAINER_H__
#define __LYNXSPRITECONTAINER_H__

#include <LynxMathWrap.h>
#include <Container/LynxContainer.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CSpriteContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CSpriteContainer);
	public:
		const static int H_ALIGN_LEFT	= 0x01;			
		const static int H_ALIGN_CENTER = 0x02;
		const static int H_ALIGN_RIGHT	= 0x04;
		const static int V_ALIGN_TOP	= 0x10;			
		const static int V_ALIGN_CENTER = 0x20;
		const static int V_ALIGN_BOTTOM = 0x40;
		
		typedef enum {
			OT_UPPER_LEFT,
			OT_UPPER_RIGHT,
			OT_CENTER,
		}ORIGINTYPE;
		
		LYNXRECT									m_RenderRect;
		CVector3									m_Scale;
	protected:			
		ORIGINTYPE									m_OriginType;	
		WORD										m_AlignmentType;		
		CVector3									m_AlignmentOffset;
		LYNXRECT									m_AlignmentRect;
		CReal										m_OriginalWidth, m_OriginalHeight;
		CReal										m_Width, m_Height;		
		LYNXUVPOINT									m_UV[4];
		LYNXBOOL									m_bUpdateVertex;
		GraphicsSystem::CVertexArrayPtr				m_VertexArray;
		GraphicsSystem::SPRITEVERTEX*				m_lpSV;					
	public:		
		CSpriteContainer(CEngine *lpengine);
		virtual ~CSpriteContainer();				

		DECLARE_CLASSSCRIPT
		LYNXBOOL									vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);

		virtual LYNXBOOL							vCreate(const CCreationDesc* desc);
		//virtual LYNXBOOL							vCreate(const LYNXCHAR *name, int group_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_TRUE);
		//virtual LYNXBOOL							vCreate(int num, int length, const LYNXCHAR *name, const LYNXCHAR *ext, int group_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_TRUE);		
		
		void										ComputeRenderRect();

		virtual LYNXBOOL							vPlay(CReal skip);
		virtual void								vRender(CCameraContainer* cam = NULL);		

		ORIGINTYPE									SetOriginType(const ORIGINTYPE ot) {ORIGINTYPE OldOT = m_OriginType; m_OriginType = ot; return OldOT; };		
		LYNXFORCEINLINE const ORIGINTYPE			GetOriginType() const {return m_OriginType;};
    
		WORD										SetAlignmentType(const WORD at) {DWORD OldAT = m_AlignmentType; m_AlignmentType = at; return OldAT; };		
		LYNXFORCEINLINE const WORD					GetAlignmentType() const {return m_AlignmentType;};	
		LYNXFORCEINLINE void						SetAlignmentRect(const LYNXRECT* lpr) {m_AlignmentRect = (*lpr); };	
		LYNXFORCEINLINE void						GetAlignmentRect(LYNXRECT* lpr) {(*lpr) = m_AlignmentRect; };	
		void										Align(int width, int height);
		void										Align();

		void										vSetSize(const Math::CReal& w, const Math::CReal& h);
		void										vGetSize(Math::CReal& w, Math::CReal& h) const;

		void										vGetScreenPosition(Math::CVector2& pos, CCameraContainer* cam, int w, int h) const;

		LYNXFORCEINLINE const CReal&				GetWidth() const {return m_Width; };
		LYNXFORCEINLINE const CReal&				GetHeight() const {return m_Height; };
	};

	class LYNXENGCLASS CSpriteCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CSpriteCreationDesc);	
	public:								
		GraphicsSystem::CRenderer::TEXTUREDESC m_TextureDesc;		
		Math::CVector2				m_UVOffset[2]; 					
		Math::CReal					m_Width, m_Height; 			
		CString						m_FileExt;
		int							m_Number;	// Number of  textures
		int							m_Length;	// Texture animation length
		LYNXBOOL					m_bUseBufferObject;
		LYNXBOOL					m_bInvertTexCoordV;
		DWORD						m_AlignmentType;
		CSpriteContainer::ORIGINTYPE m_OriginType;
		LYNXRECT					m_AlignmentRect;
		LYNXBOOL					m_bApplyUIFactor;
		
		CSpriteCreationDesc() {Default(); };
		void Default() 
		{
			CCreationDesc::Default(); 			
			m_TextureDesc.Default(); 
			m_FileExt.clear();
			m_TextureDesc.MipMapLevel = 1;
			m_TextureDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
			m_TextureDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
			m_TextureDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;			
			m_TextureDesc.SamplerDesc.Filter   = GraphicsSystem::CRenderer::FM_MIN_MAG_MIP_LINEAR;
			m_TextureDesc.SamplerDesc.MinLOD   = 0;	
			m_TextureDesc.SamplerDesc.MaxLOD   = 0;	
			m_UVOffset[0].x = m_UVOffset[0].y = -1.0f;
			m_UVOffset[1].x = m_UVOffset[1].y = -1.0f;
			m_Width = 0; m_Height = 0; 
			m_Number = 0;
			m_Length = 0;  
			m_bUseBufferObject = LYNX_TRUE; 
			m_bInvertTexCoordV = LYNX_FALSE; 
			m_AlignmentType = CSpriteContainer::H_ALIGN_LEFT | CSpriteContainer::V_ALIGN_TOP; 
			m_OriginType = CSpriteContainer::OT_UPPER_LEFT; 
			m_bApplyUIFactor = LYNX_FALSE;
		}

		CSpriteCreationDesc&		operator =(const CSpriteCreationDesc& rhs)
		{
			*((CCreationDesc*)this) = (CCreationDesc&)rhs;
			m_TextureDesc		= rhs.m_TextureDesc;			
			m_Width				= rhs.m_Width;
			m_Height			= rhs.m_Height; 			
			m_FileExt			= rhs.m_FileExt;	
			m_Number			= rhs.m_Number;
			m_Length			= rhs.m_Length;
			m_bUseBufferObject	= rhs.m_bUseBufferObject;
			m_bInvertTexCoordV	= rhs.m_bInvertTexCoordV;
			m_AlignmentType		= rhs.m_AlignmentType;
			m_OriginType		= rhs.m_OriginType;
			m_AlignmentRect		= rhs.m_AlignmentRect;	
			m_UVOffset[0]		= rhs.m_UVOffset[0];	
			m_UVOffset[1]		= rhs.m_UVOffset[1];	
			m_bApplyUIFactor	= rhs.m_bApplyUIFactor;
			
			return (*this);
		}

		void						ComputeUVOffsetByXYRatio(float tex_ratio, float res_ratio);
	};		

	CSpriteContainer::ORIGINTYPE	GetOriginType(const CString& s);
	int								GetAligmentType(const CString& s);
	
}

#endif