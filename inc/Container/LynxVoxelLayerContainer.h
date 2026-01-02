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

#ifndef __LYNXVOXELLAYERCONTAINER_H__
#define __LYNXVOXELLAYERCONTAINER_H__

#include <LynxEngineClasses.h>
#include <LynxArray.h>
#include <Container/LynxContainer.h>

namespace LynxEngine 
{
	/**	
	*  @brief Voxel layer container class。
	* 
	*  This class presents the basic rendering unit for a voxel based scene. The whole voxel scene will be renderered
	*  one layer by one layer. Each layer has a fixed 2D size whic is decided when the layer is created.
	*	
	*/
	class LYNXENGCLASS CVoxelLayerContainer : public CContainer
	{
		LYNX_DECLARE_CLASS(CVoxelLayerContainer);
	public:		
	protected:			
		WORD									m_SizeX, m_SizeY;
		GraphicsSystem::CVertexArrayPtr			m_VertexArray;
		GraphicsSystem::CIndexArrayPtr			m_IndexArray;
	public:
		CVoxelLayerContainer(CEngine *lpengine);
		virtual ~CVoxelLayerContainer();		
		
		CMeshContainer&							operator =(const CVoxelLayerContainer& rhs);	
		void									vInstance(CContainer& rhs);				
		
		LYNXINLINE GraphicsSystem::CVertexArray* GetVertexArray() {return m_VertexArray; };
		LYNXINLINE GraphicsSystem::CIndexArray*	GetIndexArray() {return m_IndexArray; };	
	};
}
#endif