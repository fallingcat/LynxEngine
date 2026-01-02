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

#ifndef __LYNXHEIGHTUNIT_H__
#define __LYNXHEIGHTUNIT_H__

#include <LynxObj.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		class LYNXENGCLASS CHeightUnit : public CUnknown
		{	
			LYNX_DECLARE_CLASS(CHeightUnit);
		private:						
		protected:
			int							m_NumHeight;
			LYNXREAL					*m_lpHeight;	
		public:						
			CHeightUnit();
			~CHeightUnit();

			void							Release();
			LYNXINLINE int					GetNumHeights() {return m_NumHeight; };
			LYNXINLINE const LYNXREAL*		GetlpHeight() {return m_lpHeight; };
			LYNXINLINE LYNXREAL				GetHeight(int i) {return m_lpHeight[i]; };

			friend class CHeightTable;
		};
	}
}

#endif