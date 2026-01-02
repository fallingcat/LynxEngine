//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Lynx SDK Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//  Compiled with Visual C++ 7.0. Best viewed with Tabs = 4.
//
//  Created by Owen Wu : 2003/07/06
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//
//###########################################################################

#include <LynxOSP.h>

//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 void	lynxInitOSP(LPLYNXOSP lposp)
{
	lynxInitOSPNode(&lposp->RootNode);
	lposp->NumNodes = 0;
	lposp->NumLeaves = 0;
	lposp->lpNodeList = NULL;
	lposp->lpLeafList = NULL;
	lposp->lpbLeafVisible = NULL;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 void	lynxReleaseOSP(LPLYNXOSP lposp)
{
	int				i;

	for (i=0; i<lposp->NumNodes; i++)
	{
		lynxReleaseOSPNode(&lposp->lpNodeList[i]);
	}
	for (i=0; i<lposp->NumLeaves; i++)
	{
		lynxReleaseOSPLeaf(&lposp->lpLeafList[i]);
	}
	LYNX_FREE(lposp->lpNodeList);
	LYNX_FREE(lposp->lpLeafList);	
	LYNX_FREE(lposp->lpbLeafVisible);	
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 void	lynxInitOSPNode(LPLYNXOSPNODE lpospnode)
{
	int							i;

	for (i=0; i<8; i++)
		lpospnode->lpChildrenID[i] = -1;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 void	lynxReleaseOSPNode(LPLYNXOSPNODE lpospnode)
{
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 void	lynxInitOSPLeaf(LPLYNXOSPLEAF lpospleaf)
{
	lpospleaf->NumElement = 0;
	lpospleaf->lpElement = NULL;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
 void	lynxReleaseOSPLeaf(LPLYNXOSPLEAF lpospleaf)
{
	LYNX_FREE(lpospleaf->lpElement);
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxOSPNodeInsidePlane(LPLYNXPLANE lpplane, LPLYNXOSPNODE lpn)		
{
	LYNXREAL					V;		

	V = LYNX_PLANE_POINT_DISTANCE((*lpplane), (lpn->Center));
	if (V <= 0)
		return TRUE;
	else
	{
		if (V > lpn->R)
		{
			return FALSE;
		}
		else
			return TRUE;
	}
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxOSPLeafInsidePlane(LPLYNXPLANE lpplane, LPLYNXOSPLEAF lpn)		
{
	LYNXREAL					V;		

	V = LYNX_PLANE_POINT_DISTANCE((*lpplane), (lpn->Center));
	if (V <= 0)
		return TRUE;
	else
	{
		if (V > lpn->R)
		{
			return FALSE;
		}
		else
			return TRUE;
	}
	return LYNX_TRUE;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
int		lynxOSPGetLeafByPosition(LPLYNXOSP lposp, LPLYNXVECTOR3D lppos)		
{
	int				i;

	for (i=0; i<lposp->NumLeaves; i++)
	{
		if (LYYNX_IS_POINT_IN_BBOX(lposp->lpLeafList[i].BBox, (*lppos)))
			return i;
	}	
	return -1;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxOSPNodeInsideFrustum(LPLYNXCAMERA lpc, LPLYNXOSPNODE lpn)		
{
	if (lpc->FrustumFlag & LYNX_TEST_FAR)
		if (!lynxOSPNodeInsidePlane(&lpc->Frustum.Far, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_NEAR)
		if (!lynxOSPNodeInsidePlane(&lpc->Frustum.Near, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_LEFT)
		if (!lynxOSPNodeInsidePlane(&lpc->Frustum.Left, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_RIGHT)
		if (!lynxOSPNodeInsidePlane(&lpc->Frustum.Right, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_TOP)
		if (!lynxOSPNodeInsidePlane(&lpc->Frustum.Top, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_BOTTOM)
		if (!lynxOSPNodeInsidePlane(&lpc->Frustum.Bottom, lpn))
			return FALSE;	

	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LYNXBOOL	lynxOSPLeafInsideFrustum(LPLYNXCAMERA lpc, LPLYNXOSPLEAF lpn)		
{
	if (lpc->FrustumFlag & LYNX_TEST_FAR)
		if (!lynxOSPLeafInsidePlane(&lpc->Frustum.Far, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_NEAR)
		if (!lynxOSPLeafInsidePlane(&lpc->Frustum.Near, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_LEFT)
		if (!lynxOSPLeafInsidePlane(&lpc->Frustum.Left, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_RIGHT)
		if (!lynxOSPLeafInsidePlane(&lpc->Frustum.Right, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_TOP)
		if (!lynxOSPLeafInsidePlane(&lpc->Frustum.Top, lpn))
			return FALSE;

	if (lpc->FrustumFlag & LYNX_TEST_BOTTOM)
		if (!lynxOSPLeafInsidePlane(&lpc->Frustum.Bottom, lpn))
			return FALSE;	

	return TRUE;
}