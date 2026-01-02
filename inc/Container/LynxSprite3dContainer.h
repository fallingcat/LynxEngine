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

#ifndef __LYNXSPRITE3DCONTAINER_H__
#define __LYNXSPRITE3DCONTAINER_H__

#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class CEngine;

	class LYNXENGCLASS CSprite3DCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CSprite3DCreationDesc);	
	public:								
		GraphicsSystem::CRenderer::TEXTUREDESC m_TextureDesc;		
		Math::CVector2				m_UVOffset[2];
		Math::CReal					m_Width, m_Height; 			
		CString						m_FileExt;	
		int							m_Number;
		int							m_Length;
		LYNXBOOL					m_bUseBufferObject;
		LYNXBOOL					m_bInvertTexCoordV;
		int							m_NumSegX, m_NumSegY;

		CSprite3DCreationDesc() {Default(); };
		void Default() 
		{
			CCreationDesc::Default(); 
			m_FileExt.clear(); 
			m_TextureDesc.Default(); 
			m_TextureDesc.SamplerDesc.Default(); 
			m_TextureDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
			m_TextureDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
			m_TextureDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;			
			m_TextureDesc.SamplerDesc.Filter	 = GraphicsSystem::CRenderer::FM_MIN_MAG_MIP_LINEAR;
			m_UVOffset[0].x = m_UVOffset[0].y = -1.0f;
			m_UVOffset[1].x = m_UVOffset[1].y = -1.0f;
			m_Width = 0; m_Height = 0;
			m_Number = 1; 
			m_Length = 1; 
			m_bUseBufferObject = LYNX_TRUE; 
			m_bInvertTexCoordV = LYNX_FALSE; 
			m_NumSegX = m_NumSegY = 1; 
		}
		
		CSprite3DCreationDesc&		operator =(const CSprite3DCreationDesc& rhs)
		{
			*((CCreationDesc*)this) = (CCreationDesc&)rhs;
			m_FileExt			= rhs.m_FileExt;
			m_Number			= rhs.m_Number;
			m_Length			= rhs.m_Length;
			m_bUseBufferObject	= rhs.m_bUseBufferObject;
			m_NumSegX			= rhs.m_NumSegX;
			m_NumSegY			= rhs.m_NumSegY;
			m_UVOffset[0]		= rhs.m_UVOffset[0];
			m_UVOffset[1]		= rhs.m_UVOffset[1];

			return (*this);
		}
	};		

	class LYNXENGCLASS CSprite3DContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CSprite3DContainer);
	public:
		static const int ALIGNMENT_H_LEFT			= 0x0001;
		static const int ALIGNMENT_H_CENTER			= 0x0002;
		static const int ALIGNMENT_H_RIGHT			= 0x0004;
		static const int ALIGNMENT_V_TOP			= 0x0010;
		static const int ALIGNMENT_V_CENTER			= 0x0020;
		static const int ALIGNMENT_V_BOTTOM			= 0x0040;	
	protected:				
		int											m_AlignmentFlag;
		DECLAR_PARAM_FLOAT(m_Width);
		DECLAR_PARAM_FLOAT(m_Height);		
		GraphicsSystem::CVertexArrayPtr				m_VertexArray;
		WORD										m_LockedAxis;
		int											m_NumSegX, m_NumSegY;		
	public:		
		CSprite3DContainer(CEngine *lpengine);
		~CSprite3DContainer();				

		CSprite3DContainer&							operator =(const CSprite3DContainer& rhs);
		
		virtual LYNXBOOL							vCreate(const CCreationDesc* desc);
		//virtual LYNXBOOL							vCreate(const LYNXCHAR *name, int cluster_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);
		//virtual LYNXBOOL							vCreate(int num, int length, const LYNXCHAR *name, const LYNXCHAR *ext, int group_size = 1, LPLYNXFILE lpf = NULL, long offset = 0, LYNXBOOL bbo = LYNX_FALSE);		
		
		virtual LYNXBOOL							vPlay(CReal skip);
		virtual void								vRender(CCameraContainer* cam = NULL);		

		//void										SetAlignType(const WORD at);	
		//LYNXINLINE const WORD						GetAlignType() const {return m_AlignType;};	

		virtual void								vSetSize(const Math::CReal& w, const Math::CReal& h);
		virtual void								vGetSize(Math::CReal& w, Math::CReal& h) const {w = m_Width; h = m_Height; };

		void										SetColor(LPLYNXCOLORRGBA lpc);
		LYNXINLINE void								SetLockedAxis(WORD l) {m_LockedAxis = l; };
		LYNXINLINE const WORD&						GetLockedAxis() const {return m_LockedAxis; };
	};
}

#endif