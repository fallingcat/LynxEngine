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

#ifndef __LYNXVOLUMETRICCLOUDCONTAINER_H__
#define __LYNXVOLUMETRICCLOUDCONTAINER_H__

//#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxParticle.h>
#include <Container/LynxPEmitterContainer.h>

namespace LynxEngine 
{	
	class LYNXENGCLASS CCloudParticle : public CParticle
	{
		LYNX_DECLARE_CLASS(CCloudParticle);
	protected:			
		//CVolumetricCloudContainer*				m_lpCloud;
		//CReal 									m_Size;
		CReal 									m_Angle;
		CVector3								m_Pos;
		CReal									m_Dist;
		LYNXCOLORRGBA 							m_Color;
		//CReal									m_Life;	
	public:
		CCloudParticle();
		CCloudParticle(CVolumetricCloudContainer*);
		~CCloudParticle();		

		virtual void							vBehave(LYNXREAL step);

		friend class CVolumetricCloudContainer;
	};

	class LYNXENGCLASS CVolumetricCloudCreationDesc : public CPEmitterCreationDesc //CSprite3DClusterCreationDesc
	{	
		LYNX_DECLARE_CLASS(CVolumetricCloudCreationDesc);	
	public:								
		CVolumetricCloudCreationDesc() {};
	};		

	class LYNXENGCLASS CVolumetricCloudContainer : public CPEmitterContainer//CSprite3DClusterContainer//PUBLIC CPEmitterContainer
	{
		LYNX_DECLARE_CLASS(CVolumetricCloudContainer);
	public:		
	protected:		
		CReal									m_Radius;		
	private:
		void									InitMembers();
	public:
		CVolumetricCloudContainer();
		CVolumetricCloudContainer(CEngine *lpengine);		
		virtual  ~CVolumetricCloudContainer();			

		CVolumetricCloudContainer&				operator =(const CVolumetricCloudContainer& rhs);

		virtual LYNXBOOL						vCreate(const CCreationDesc* desc);		
		//void									vRender(CCameraContainer* cam);
		virtual LYNXBOOL						vPlay(CReal step);
	};		
}

#endif
