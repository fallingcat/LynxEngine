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

#ifndef __LYNXCLUSTESUBCONTAINER_H__
#define __LYNXCLUSTESUBCONTAINER_H__

#include <LynxEngineClasses.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CClusterSubCreationDesc : public CCreationDesc
	{	
		LYNX_DECLARE_CLASS(CClusterSubCreationDesc);	
	public:								
		CContainer*						m_lpClusterContainer;
		int								m_SubContainerIndex;
		LYNXUVPOINT						m_UV[4];
		LYNXCOLORRGBA					m_Color;
		Math::CVector2					m_Size;

		CClusterSubCreationDesc() 
		{
			m_lpClusterContainer = NULL; 
			m_SubContainerIndex = 0; 
			m_Color.PackedColor = 0xffffffff;
			m_Size.x = m_Size.y = -1.0f;
		}

		CClusterSubCreationDesc&		operator =(const CClusterSubCreationDesc& rhs)
		{	
			*((CCreationDesc*)this) = (CCreationDesc&)rhs;			
			m_lpClusterContainer = rhs.m_lpClusterContainer;
			m_SubContainerIndex = rhs.m_SubContainerIndex;
			m_Color = rhs.m_Color;
			m_Size = rhs.m_Size;

			return (*this);
		}
	};		
	/**	
	*  @brief Reference container class。
	* 
	*  This class is used as a spatial reference of a 3D world. It would be rendered since is doen't present any physical object. 
	*	
	*/
	class LYNXENGCLASS CClusterSubContainer : public CContainer
	{		
		LYNX_DECLARE_CLASS(CClusterSubContainer);
	protected:						
		int									m_SubContainerIndex;
	public:		
		CClusterSubContainer(CEngine *lpengine);
		virtual ~CClusterSubContainer();				


		virtual LYNXBOOL					vCreate(const CCreationDesc* desc);

		LYNXFORCEINLINE int					GetIndex() const {return m_SubContainerIndex; };

		virtual void						vSetPosition(const Math::CVector3& pos);		
		virtual void						vSetSize(const Math::CReal& w, const Math::CReal& h);
		virtual void						vGetSize(Math::CReal& w, Math::CReal& h) const;
	};
}

#endif