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

#ifndef __LYNXPARTICLE_H__
#define __LYNXPARTICLE_H__
	
#include <LynxRenderableObj.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CParticle : public CRenderableObj
	{
		LYNX_DECLARE_CLASS(CParticle);
	protected:		
		CPEmitterContainer*	m_lpEmitter;	
		CReal				m_LifeTime;
		CReal				m_LifeTimeLimit;	

		CVector3			m_Pos;
		CVector3			m_Rot;
		CVector3			m_VelR;
		CVector3			m_Vel0;
		CVector3			m_Vel;
		CReal				m_Radius;
		LYNXCOLORRGBA		m_Color;		
		float				m_RandTextureOffset;
		float				m_TextureTime;			
	public:
		CParticle();
		CParticle(CPEmitterContainer* ppe);
		~CParticle();		

		virtual LYNXBOOL				vCreate(CContainer::TYPE type, const CCreationDesc* desc);		
		void							Init(const CVector3& pos, const CVector3& vel, const CVector3& rot, const CVector3& rvel, CReal r, LYNXCOLORRGBA& c);
		LYNXINLINE void					SetRadius(CReal& r) {m_Radius = r; };
		LYNXINLINE LYNXCOLORRGBA&		GetColor() {return m_Color; };		
		LYNXINLINE void					SetColor(LYNXCOLORRGBA& c) {m_Color.PackedColor = c.PackedColor; };				
		void							vLoop(float step);

		LYNXFORCEINLINE void			Die() {m_LifeTime = (m_LifeTimeLimit+1); }
		LYNXFORCEINLINE float			GetLifeTime() {return m_LifeTime; }		

		friend class CPEmitterContainer;
	};			
}

#endif
