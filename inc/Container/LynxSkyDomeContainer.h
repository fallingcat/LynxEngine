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

#ifndef __LYNXSKYDOMECONTAINER_H__
#define __LYNXSKYDOMECONTAINER_H__

#include <Container/LynxContainer.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CSkyDomeCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CSkyDomeCreationDesc);	
	public:							
		CReal						m_Radius;
		int							m_Slices, m_Sides;	
		int							m_SkyLightMapSize;
		GraphicsSystem::CRenderer::FORMAT m_SkyLightMapFormat;

		CSkyDomeCreationDesc() { m_Radius = 1.0f, m_Slices = 8; m_Sides = 8; m_SkyLightMapSize = 1024; m_SkyLightMapFormat = GraphicsSystem::CRenderer::FORMAT_A16B16G16R16F; };
	};	

	class LYNXENGCLASS CSkyDomeContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CSkyDomeContainer);
	public:		
		typedef struct {
			LYNXVECTOR3D				Pos;	
			LYNXUVPOINT					TexCoord;			
		}SKYVERTEX;	
	protected:						
		static GraphicsSystem::CVertexLayoutPtr	m_VertexLayout;
		int										m_NumVertices;
		CArray<GraphicsSystem::CRenderer::PRIMITIVE>* m_PrimitiveList;		
		GraphicsSystem::CVertexArrayPtr*		m_VertexArray;
		GraphicsSystem::CIndexArrayPtr*			m_IndexArray;		

		SceneSystem::CLight*					m_lpSunLight;
		CSkyLightMap*							m_lpSkyLightMap;
		MaterialSystem::CMaterialPtr			m_lpSkyLightMaterial;		

		DECLAR_PARAM_FLOAT(m_SunIntensity);			// Sun brightness constant			
		DECLAR_PARAM_FLOAT(m_SunLightIntensityScale); // Sun light constant scale			
		DECLAR_PARAM_INT(m_NumSamples)
		DECLAR_PARAM_RANGE_FLOAT(m_Time)			// 
		DECLAR_PARAM_RANGE_FLOAT(m_Kr)				// Rayleigh scattering constant		
		DECLAR_PARAM_RANGE_FLOAT(m_Km)				// Mie scattering constant									
		DECLAR_PARAM_FLOAT(m_g)						// The Mie phase asymmetry factor
		//DECLAR_PARAM_FLOAT(m_Exposure);				// Exposure constant						
		DECLAR_PARAM_RANGE_FLOAT(m_RayleighScaleDepth)	//
		//DECLAR_PARAM_FLOAT(m_MieScaleDepth);		//
		DECLAR_PARAM_URGB(m_Color)
		DECLAR_PARAM_FLOAT(m_ColorPower)

		float									m_ESun;
		int										m_SkyLightMapSize;
		CVector3								m_EyePos;
		CVector3								m_SunDir;
		CVector3								m_InvWavelength;
		CVector3								m_HG;
		
		float									m_InnerRadius;	// Inner planetary radius 
		float									m_OuterRadius;	// Outer atmosphere radius
		float									m_Kr4PI;		
		float 									m_Km4PI;		
		float 									m_KrESun;
		float 									m_KmESun;			
		float 									m_Scale;					
	private:
		void									InitMembers();
		LYNXBOOL								Setup(CReal radius = 200.0f, int slices = 24, int sides = 32, LYNXBOOL exponential = LYNX_FALSE);
	public:
		CSkyDomeContainer(CEngine *lpengine);		
		virtual  ~CSkyDomeContainer();			

		static GraphicsSystem::CVertexLayout*	CreateVertexLayout(CEngine* e);
		void									CreateVertexLayout(void);

		LYNXINLINE void							SetlpSunLight(SceneSystem::CLight* l) {m_lpSunLight = l; };
		LYNXINLINE CSkyLightMap&				GetSkyLightMap() {return *m_lpSkyLightMap; };
		LYNXINLINE int							GetSkyLightMapSize() {return m_SkyLightMapSize; };	
		LYNXINLINE const CVector3&				GetSunDir() const { return m_SunDir; };			
		LYNXINLINE const CVector3&				GetEyePos() const { return m_EyePos; };			
		LYNXINLINE const CVector3&				GetInvWavelength() const { return m_InvWavelength; };			
		LYNXINLINE const CVector3&				GetHG() const { return m_HG; };			
		LYNXINLINE const float					GetKrESun() const { return m_KrESun; };			
		LYNXINLINE const float					GetKmESun() const { return m_KmESun; };			
		LYNXINLINE const int					GetSkyLightMapSize() const { return m_SkyLightMapSize; };		
		LYNXINLINE const float					GetInnerRadius() const { return m_InnerRadius; };		
		LYNXINLINE const float					GetOuterRadius() const { return m_OuterRadius; };		

		LYNXINLINE const float					GetSunIntensity() {return m_SunIntensity; };
		LYNXINLINE const float					GetKr() {return m_Kr; };
		LYNXINLINE const float					GetKm() {return m_Km; };
		LYNXINLINE const float					GetESun() {return m_ESun; };
		LYNXINLINE const float					Getg() {return m_g; };
		LYNXINLINE const float					GetTime() {return m_Time; };
		LYNXINLINE const LYNXCOLORRGB&			GetColor() {return m_Color; };
		LYNXINLINE const float					GetColorPower() {return m_ColorPower; };

		LYNXINLINE const float					GetKr4PI() const { return m_Kr4PI; };		
		LYNXINLINE const float					GetKm4PI() const { return m_Km4PI; };		

		LYNXINLINE const float					GetScale() const { return m_Scale; };		
		LYNXINLINE const float					GetRayleighScaleDepth() const { return m_RayleighScaleDepth; };		
		LYNXINLINE const int					GetNumSamples() const { return m_NumSamples; };		

		void									UpdateParam();
		void									UpdateSkyLightMap();
		virtual LYNXBOOL						vCreate(const CCreationDesc* desc);
		void									vSetNumLODs(int lod);
		virtual LYNXBOOL						vPlay(CReal step);
		void									vRender(CCameraContainer* cam);
	};	
}

#endif
