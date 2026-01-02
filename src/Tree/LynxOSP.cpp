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
#include <Tree/LynxOSP.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	COSP::COSP(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	COSP::~COSP(void)
	{		
	}		
	/*
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void COSP::LoadLeaf(LPLYNXOSPLEAF lpn, LPLYNXFILE lpf)
	{
		int							i;
		LYNXVECTOR3D				V;

		LYNX_READ_FILE(&lpn->ID, sizeof(int), 1, lpf);
		LYNX_READ_FILE(&lpn->BBox, sizeof(LYNXBOUNDARYBOX), 1, lpf);
		LYNX_READ_FILE(&lpn->NumElement, sizeof(WORD), 1, lpf);
		lpn->lpElement = LYNX_MALLOC(lpn->NumElement*sizeof(WORD));
		if (lpn->NumElement)
		{
			LYNX_READ_FILE(lpn->lpElement, sizeof(WORD), lpn->NumElement, lpf);
		}
		lpn->Center.s.x = (lpn->BBox.Min.s.x + lpn->BBox.Max.s.x)/2.0f;
		lpn->Center.s.y = (lpn->BBox.Min.s.y + lpn->BBox.Max.s.y)/2.0f;
		lpn->Center.s.z = (lpn->BBox.Min.s.z + lpn->BBox.Max.s.z)/2.0f;
		LYNX_VECTOR3D_SUB(V, lpn->Center, lpn->BBox.Min);
		lpn->R = lynxLength(&V, 3);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	void COSP::LoadNode(LPLYNXOSPNODE lpn, LPLYNXFILE lpf)
	{
		int							i;

		LYNX_READ_FILE(&lpn->ID, sizeof(int), 1, lpf);
		LYNX_READ_FILE(&lpn->BBox, sizeof(LYNXBOUNDARYBOX), 1, lpf);	
		LYNX_READ_FILE(&lpn->Center, sizeof(LYNXVECTOR3DFORIO), 1, lpf);	
		LYNX_READ_FILE(&lpn->R, sizeof(LYNXREAL), 1, lpf);	
		LYNX_READ_FILE(lpn->lpChildrenID, sizeof(short), 8, lpf);			
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL COSP::Load(char *filename)
	{
		LYNXFILE					*lpFile, File;	
		int							i;	

		lynxInitOSP(&m_OSP);

		if (lynxGetUseZipFile())
		{
			lpFile = &File;
			if (!LYNX_OPEN_FILE(lpFile, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_ZIPFILE_STR))
				return LYNX_FALSE;			
		}
		else
		{
			lpFile = &File;
			if (!LYNX_OPEN_FILE(&File, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_MEMORY_STR))
			{		
				return LYNX_FALSE;		
			}	
		}

		LYNX_READ_FILE(&m_OSP.NumNodes, sizeof(DWORD), 1, lpFile);
		LYNX_READ_FILE(&m_OSP.NumLeaves, sizeof(DWORD), 1, lpFile);	

		m_OSP.lpNodeList = (LPLYNXOSPNODE)LYNX_MALLOC(m_OSP.NumNodes*sizeof(LYNXOSPNODE));
		m_OSP.lpLeafList = (LPLYNXOSPLEAF)LYNX_MALLOC(m_OSP.NumLeaves*sizeof(LYNXOSPLEAF));
		m_OSP.lpbLeafVisible = (LYNXBOOL *)LYNX_MALLOC(m_OSP.NumLeaves*sizeof(LYNXBOOL));	

		for (i=0; i<m_OSP.NumNodes; i++)
			LoadNode(&m_OSP.lpNodeList[i], lpFile);
		for (i=0; i<m_OSP.NumLeaves; i++)
			LoadLeaf(&m_OSP.lpLeafList[i], lpFile);

		LYNX_CLOSE_FILE(lpFile);

		return LYNX_TRUE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL COSP::Release(void)
	{
		lynxReleaseOSP(&m_OSP);

		return LYNX_TRUE;
	}
	*/
}