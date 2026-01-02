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

#include <LynxEngine_PCH.h>
#include <SceneSystem/HeightTable/LynxHeightTable.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		const LYNXREAL CHeightTable::NULL_HEIGHT = -999999.0f;
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHeightTable::CHeightTable()
		{
			m_GridUnit = 1.0f;
			m_NumXGrids = m_NumZGrids = 0;
			m_lpHeightUnit = NULL;	
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHeightTable::~CHeightTable()
		{
			Release();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CHeightTable::Release()
		{
			if (m_lpHeightUnit)
				delete [] m_lpHeightUnit;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CHeightTable::LoadHTB(LYNXCHAR *filename)
		{
			LYNXFILE					File;
			int							Index;

			Release();

			if (!LYNX_OPEN_FILE(&File, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_FILE_STR))
				return LYNX_FALSE;

			LYNX_READ_FILE(&m_GridUnit, sizeof(LYNXREAL), 1, &File);
			LYNX_READ_FILE(&m_BBox, sizeof(LYNXBOUNDARYBOX), 1, &File);
			LYNX_READ_FILE(&m_NumXGrids, sizeof(int), 1, &File);
			LYNX_READ_FILE(&m_NumZGrids, sizeof(int), 1, &File);
			m_lpHeightUnit = LYNXNEW CHeightUnit[m_NumXGrids*m_NumZGrids];
			for (int i=0; i<m_NumZGrids; i++)
			{	
				for (int j=0; j<m_NumXGrids; j++)
				{
					Index = i*m_NumXGrids+j;
					LYNX_READ_FILE(&m_lpHeightUnit[Index].m_NumHeight, sizeof(int), 1, &File);
					if (m_lpHeightUnit[Index].m_NumHeight)
					{
						m_lpHeightUnit[Index].m_lpHeight = LYNXGLOBAL_NEW float[m_lpHeightUnit[Index].m_NumHeight];
						LYNX_READ_FILE(m_lpHeightUnit[Index].m_lpHeight, sizeof(LYNXREAL), m_lpHeightUnit[Index].m_NumHeight, &File);
					}
				}
			}
			LYNX_CLOSE_FILE(&File);

			return LYNX_TRUE;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHeightUnit* CHeightTable::GetHeightUnit(LPLYNXVECTOR3D lppos)
		{
			int				X, Z;

			X = (lppos->x - m_BBox.Min.x)/m_GridUnit;
			Z = (lppos->z - m_BBox.Min.z)/m_GridUnit;

			if (X >= 0 && X < m_NumXGrids && Z >= 0 && Z < m_NumZGrids)
				return &m_lpHeightUnit[Z*m_NumXGrids+X];
			else
				return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXREAL CHeightTable::GetHeight(LPLYNXVECTOR3D lppos, SAMPLEMODE sm)
		{
			int				X, Z;
			LYNXREAL		XOffset, ZOffset, Offset, H0, H1, HZ0, HZ1;
			LYNXVECTOR3D	V;

			X = (lppos->x - m_BBox.Min.x)/m_GridUnit;
			Z = (lppos->z - m_BBox.Min.z)/m_GridUnit;

			if (X >= 0 && X < m_NumXGrids && Z >= 0 && Z < m_NumZGrids)
			{
				switch (sm)
				{
					default:
					case POINT_SAMPLED:
						return m_lpHeightUnit[Z*m_NumXGrids+X].GetHeight(0);
						break;

					case BILINEAR_SAMPLED:
						XOffset = (lppos->x - m_BBox.Min.x)/m_GridUnit - X;
						ZOffset = (lppos->z - m_BBox.Min.z)/m_GridUnit - Z;
						H0 = m_lpHeightUnit[Z*m_NumXGrids+X].GetHeight(0);
						H1 = m_lpHeightUnit[(Z)*m_NumXGrids+X+1].GetHeight(0);
						HZ0 = LYNX_INTERPOLATE(H0, H1, XOffset);
						H0 = m_lpHeightUnit[(Z+1)*m_NumXGrids+X].GetHeight(0);
						H1 = m_lpHeightUnit[(Z+1)*m_NumXGrids+X+1].GetHeight(0);
						HZ1 = LYNX_INTERPOLATE(H0, H1, XOffset);
						return LYNX_INTERPOLATE(HZ0, HZ1, ZOffset);
						break;
				}
			}
			else
				return NULL_HEIGHT;
		}
	}
}