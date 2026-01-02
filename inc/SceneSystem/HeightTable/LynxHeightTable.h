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

#ifndef __LYNXHEIGHTTABLE_H__
#define __LYNXHEIGHTTABLE_H__

#include <LynxObj.h>
#include <LynxCore.h>
#include <SceneSystem/HeightTable/LynxHeightUnit.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CHeightTable : public CObj
		{	
			LYNX_DECLARE_CLASS(CHeightTable);
		public:
			static const LYNXREAL NULL_HEIGHT;
			enum SAMPLEMODE{
				POINT_SAMPLED = 0,
				BILINEAR_SAMPLED,
			};
		private:						
		protected:
			LYNXREAL						m_GridUnit;
			LYNXBOUNDARYBOX					m_BBox;
			int								m_NumXGrids, m_NumZGrids;	
			CHeightUnit						*m_lpHeightUnit;	
		public:				
			CHeightTable();
			~CHeightTable();

            void							Release();
			LYNXINLINE int					GetNumXGrids() {return m_NumXGrids;};
			LYNXBOOL						LoadHTB(LYNXCHAR *filename);
			CHeightUnit*					GetHeightUnit(LPLYNXVECTOR3D lppos);	
			LYNXREAL						GetHeight(LPLYNXVECTOR3D lppos, SAMPLEMODE sm = POINT_SAMPLED);	
		};
	}
}

#endif