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
#include <SceneSystem/StaticObj/Geometry/LynxGeometry.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGeometry::CGeometry(void)		
		{			
			m_Type = GEOMETRY;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGeometry::CGeometry(CScene *lpscene)
		:CStaticObj(lpscene)
		{					
			m_Type = GEOMETRY;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CGeometry::~CGeometry(void)
		{
			vRelease();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CGeometry::vRelease(void)
		{			
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CGeometry::vCreate(void)
		{		
			return CRenderableObj::Create(CContainer::GEOMETRY);
		}	








		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//CGeometry::CGeometry(void)
		//{			
		//	CStaticObj::m_Type = GEOMETRY;
		//	m_BaseVert = 0;
		//	m_NumVertices = 0;
		//	m_BaseTriangle = 0;
		//	m_NumTriangles = 0;
		//	m_lpMaterial = NULL;			
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//CGeometry::CGeometry(CScene *lpscene)
		//:CStaticObj(lpscene)
		//{	
		//	CStaticObj::m_Type = GEOMETRY;
		//	m_BaseVert = 0;
		//	m_NumVertices = 0;
		//	m_BaseTriangle = 0;
		//	m_NumTriangles = 0;
		//	m_lpMaterial = NULL;						
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//CGeometry::~CGeometry(void)
		//{
		//	Release();
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//LYNXBOOL	CGeometry::vLoad(LPLYNXFILE lpf, AMS::CAdvMaterial **lpm)
		//{
		//	int							i, j, MI;	
		//	WORD						Type;

		//	LYNX_READ_FILE(m_Name, 64, sizeof(char), lpf);					
		//	LYNX_READ_FILE(&MI, 1, sizeof(int), lpf);
		//	m_lpMaterial = lpm[MI];
		//	m_lpMaterial->AddRenderableObj(this);

		//	//LYNX_READ_FILE(&Type, sizeof(WORD), 1, lpf);			
		//	LYNX_READ_FILE(&m_Visibility, sizeof(float),1,  lpf);					
		//	LYNX_READ_FILE(&m_Attribute, 1, sizeof(int), lpf);			
		//	LYNX_READ_FILE(m_lpCheckV, 8, sizeof(LYNXVECTOR3DFORIO), lpf);	
		//	for (j=0; j<6; j++)
		//	{
		//		LYNX_READ_FILE(&m_lpCheckFan[j].NumVerts, sizeof(DWORD), 1, lpf);						
		//		m_lpCheckFan[j].PlaneBoundary.lpEdgePlane = LYNXNEW LYNXPLANE[m_lpCheckFan[j].NumVerts];
		//		LYNX_READ_FILE(m_lpCheckFan[j].lpv, m_lpCheckFan[j].NumVerts, sizeof(WORD), lpf);			
		//		LYNX_READ_FILE(&m_lpCheckFan[j].PlaneBoundary.Plane, sizeof(LYNXPLANE), 1, lpf);			
		//		LYNX_READ_FILE(m_lpCheckFan[j].PlaneBoundary.lpEdgePlane, m_lpCheckFan[j].NumVerts, sizeof(LYNXPLANE), lpf);			
		//	}
		//	LYNX_READ_FILE(&m_BaseVert, sizeof(WORD), 1, lpf);			
		//	LYNX_READ_FILE(&m_NumVertices, sizeof(DWORD), 1, lpf);			
		//	LYNX_READ_FILE(&m_BaseTriangle, sizeof(WORD), 1, lpf);			
		//	LYNX_READ_FILE(&m_NumTriangles, sizeof(DWORD), 1, lpf);	

		//	LYNX_SEEK_FILE(lpf, sizeof(LYNXBOOL), LYNX_SEEK_CUR);	

		//	/*
		//	if (m_bStripify)
		//	{
		//		LYNX_READ_FILE(&lpscn->lpTerrainObjList[i].NumPrimitive, sizeof(unsigned short), 1, lpf);	
		//		lpscn->lpTerrainObjList[i].lpPrimitive = (LPLYNXPRIMITIVE)LYNX_MALLOC(lpscn->lpTerrainObjList[i].NumPrimitive*sizeof(LYNXPRIMITIVE));
		//		for (j=0; j<lpscn->lpTerrainObjList[i].NumPrimitive; j++)
		//		{
		//			LYNX_READ_FILE(&PType, sizeof(BYTE), 1, lpf);	
		//			lpscn->lpTerrainObjList[i].lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)PType;
		//			LYNX_READ_FILE(&lpscn->lpTerrainObjList[i].lpPrimitive[j].NumIndex, sizeof(unsigned int), 1, lpf);	
		//			lpscn->lpTerrainObjList[i].lpPrimitive[j].lpIndex = (unsigned short *)LYNX_MALLOC(lpscn->lpTerrainObjList[i].lpPrimitive[j].NumIndex*sizeof(unsigned short));
		//			LYNX_READ_FILE(lpscn->lpTerrainObjList[i].lpPrimitive[j].lpIndex, sizeof(unsigned short), lpscn->lpTerrainObjList[i].lpPrimitive[j].NumIndex, lpf);	
		//			for (k=0; k<lpscn->lpTerrainObjList[i].lpPrimitive[j].NumIndex; k++)
		//				lpscn->lpTerrainObjList[i].lpPrimitive[j].lpIndex[k] -= lpscn->lpTerrainObjList[i].BaseVert;
		//		}		
		//	}
		//	*/
		//	
		//	return TRUE;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void	CGeometry::Release(void)
		//{
		//	for (int j=0; j<6; j++)
		//	{
		//		delete [] m_lpCheckFan[j].PlaneBoundary.lpEdgePlane;				
		//	}			
		//	delete [] m_lpPrimitive;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//LYNXBOOL	CGeometry::vSetup(LPLYNXVERTEX3D lpv, LPLYNXFAN lpf)
		//{
		//	int						k;
		//	WORD					*lpI;

		//	lynxCreateVertexArray(&m_VertexArray, sizeof(LYNXRENDERVERTEX), m_NumVertices, LYNX_LOCK_TO_WRITE, LYNX_TRUE);
		//	lynxLockVertexArray(&m_VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);
		//	m_lpVertexArray = (LPLYNXRENDERVERTEX)m_VertexArray.lpBuffer;
		//	for (k=0; k<m_NumVertices; k++)
		//	{	
		//		LYNX_RV_SET_XYZ(m_lpVertexArray[k], lpv[m_BaseVert+k].x, lpv[m_BaseVert+k].y, lpv[m_BaseVert+k].z);
		//		m_lpVertexArray[k].nx = lpv[m_BaseVert+k].nx;
		//		m_lpVertexArray[k].ny = lpv[m_BaseVert+k].ny;
		//		m_lpVertexArray[k].nz = lpv[m_BaseVert+k].nz;
		//		LYNX_RV_SET_UV(m_lpVertexArray[k], 0, lpv[m_BaseVert+k].TexCoord[0].u, lpv[m_BaseVert+k].TexCoord[0].v);
		//		LYNX_RV_SET_UV(m_lpVertexArray[k], 1, lpv[m_BaseVert+k].TexCoord[1].u, lpv[m_BaseVert+k].TexCoord[1].v);
		//		LYNX_RV_SET_UV(m_lpVertexArray[k], 2, lpv[m_BaseVert+k].TexCoord[2].u, lpv[m_BaseVert+k].TexCoord[2].v);
		//		LYNX_RV_SET_UV(m_lpVertexArray[k], 3, lpv[m_BaseVert+k].TexCoord[3].u, lpv[m_BaseVert+k].TexCoord[3].v);
		//		LYNX_RV_SET_RGBA(m_lpVertexArray[k], (lpv[m_BaseVert+k].Color.s.Red), (lpv[m_BaseVert+k].Color.s.Green), (lpv[m_BaseVert+k].Color.s.Blue), (lpv[m_BaseVert+k].Color.s.Alpha));
		//	}
		//	lynxUnlockVertexArray(&m_VertexArray);					
		//				
		//	m_NumPrimitive = 1;
		//	m_lpPrimitive = LYNXNEW LYNXPRIMITIVE[m_NumPrimitive];
		//	m_lpPrimitive[0].Type = LYNX_PRIMITIVE_TRIANGLE_LIST;
		//	m_lpPrimitive[0].lpIndex = NULL;
		//	m_lpPrimitive[0].NumIndex = m_NumTriangles*3;								
		//	lynxCreateIndexArray(&m_lpPrimitive[0].IndexArray, sizeof(WORD), m_lpPrimitive[0].NumIndex, LYNX_LOCK_TO_WRITE, LYNX_TRUE);
		//	
		//	lynxLockIndexArray(&m_lpPrimitive[0].IndexArray, 0, 0, LYNX_LOCK_TO_WRITE);
		//	lpI = (WORD *)m_lpPrimitive[0].IndexArray.lpBuffer;							
		//	for (k=0; k<m_NumTriangles; k++)
		//	{
		//		lpI[k*3] = lpf[m_BaseTriangle+k].lpv[0] - m_BaseVert;
		//		lpI[k*3+1] = lpf[m_BaseTriangle+k].lpv[1] - m_BaseVert;
		//		lpI[k*3+2] = lpf[m_BaseTriangle+k].lpv[2] - m_BaseVert;
		//	}											
		//	lynxUnlockIndexArray(&m_lpPrimitive[0].IndexArray);																								

		//	return TRUE;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//LYNXBOOL	CGeometry::vSetup(LPLYNXVERTEX3D lpv, LPLYNXFAN lpf, LPLYNXUVPOINT lpuv, LPTRIANGLEFANDATA lplf)
		//{
		//	/*
		//	int						i, k;	

		//	lynxCreateVertexArray(&m_VertexArray, sizeof(LYNXRENDERVERTEX), m_NumTriangles*3);
		//	lynxLockVertexArray(&m_VertexArray, 0, 0, LYNX_LOCK_TO_BOTH);
		//	m_lpVertexArray = (LPLYNXRENDERVERTEX)m_VertexArray.lpBuffer;
		//	for (k=0; k<m_NumTriangles; k++)
		//	{	
		//		for (i=0; i<3; i++)
		//		{
		//			LYNX_RV_SET_XYZ(m_lpVertexArray[k*3+i], lpv[lpf[m_BaseTriangle+k].lpv[i]].x, lpv[lpf[m_BaseTriangle+k].lpv[i]].y, lpv[lpf[m_BaseTriangle+k].lpv[i]].z);
		//			//m_lpVertexArray[k].nx = lpv[m_BaseVert+k].nx;
		//			//m_lpVertexArray[k].ny = lpv[m_BaseVert+k].ny;
		//			//m_lpVertexArray[k].nz = lpv[m_BaseVert+k].nz;
		//			LYNX_RV_SET_UV(m_lpVertexArray[k*3+i], 0, lpv[lpf[m_BaseTriangle+k].lpv[i]].TexCoord[0].u, lpv[lpf[m_BaseTriangle+k].lpv[i]].TexCoord[0].v);			
		//			LYNX_RV_SET_UV(m_lpVertexArray[k*3+i], 1, lpuv[lplf[m_BaseTriangle+k].lpv[i]].u, lpuv[lplf[m_BaseTriangle+k].lpv[i]].v);			
		//			LYNX_RV_SET_RGBA(m_lpVertexArray[k*3+i], 255, 255, 255, 255);
		//		}
		//	}
		//	lynxUnlockVertexArray(&m_VertexArray);						
		//	*/

		//	return TRUE;
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void	CGeometry::vRender(void)
		//{
		//	int					i, j, Base, Num;										
		//
		//	lynxSetVertexArray(&m_VertexArray, 0);

		//	for (int p=0; p<m_lpMaterial->GetNumPass(); p++)
		//	{
		//		m_lpMaterial->GetMaterialPass(p).Set();
		//		#ifdef __DIRECT3D__
		//			for (i=0; i<m_NumPrimitive; i++)			
		//			{
		//				//if (m_lpPrimitive[i].lpIndex)
		//				{
		//					lynxGetlpRenderer()->SystemData3D.lpD3DDevice->SetIndices(m_lpPrimitive[i].IndexArray.lpD3DIB);										
		//					switch (m_lpPrimitive[i].Type)
		//					{
		//						case LYNX_PRIMITIVE_TRIANGLE_STRIP:
		//							lynxGetlpRenderer()->SystemData3D.lpD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_NumVertices, 0, m_lpPrimitive[i].NumIndex-2);							
		//							break;

		//						case LYNX_PRIMITIVE_TRIANGLE_LIST:								
		//							lynxGetlpRenderer()->SystemData3D.lpD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_NumVertices, 0, m_lpPrimitive[i].NumIndex/3);														
		//							break;
		//					}								
		//				}
		//			}
		//		#endif
		//	}
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void	CGeometry::Render(LPLYNXFAN lpf, LYNXBOOL btex)
		//{
		//	/*
		//	int					i;
		//	
		//	
		//	lynxSetTexture(0, 1);	
		//	lynxGetlpRenderer()->SystemData3D.lpD3DDevice->SetStreamSource(0, m_VertexArray.lpD3DVB, 0, sizeof(LYNXRENDERVERTEX));		
		//	for (i=0; i<m_NumTriangles; i++)
		//	{
		//		lynxGetlpRenderer()->SystemData3D.lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, i*3, 1);
		//	}
		//	*/
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void	CGeometry::RenderShadowVolume(LPLYNXVERTEX3D lpv, LPLYNXFAN lpf, LPLYNXVECTOR3D lpp)
		//{
		//	/*
		//	int								i;
		//	LYNXRENDERVERTEX				V[9];
		//	LYNXVECTOR3D					Dir;
		//	float							Len;

		//	for (i=0; i<3; i++)
		//	{
		//		LYNX_RV_SET_RGBA(V[i*3+0], 155, 155, 155, 255);
		//		LYNX_RV_SET_RGBA(V[i*3+1], 0, 0, 0, 255);
		//		LYNX_RV_SET_RGBA(V[i*3+2], 0, 0, 0, 255);						

		//		LYNX_RV_SET_UV(V[i*3+0], 0, 4.0f, 0.0f);
		//		LYNX_RV_SET_UV(V[i*3+1], 0, 8.0f, 8.0f);
		//		LYNX_RV_SET_UV(V[i*3+2], 0, 8.0f, 8.0f);		
		//	}

		//	Len = 3.0f;
		//	for (i=0; i<m_NumTriangles; i++)
		//	{
		//		LYNX_RV_SET_XYZ(V[0], (lpp->x), (lpp->y), (lpp->z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[0]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[1], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[1]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[2], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));		

		//		LYNX_RV_SET_XYZ(V[3], (lpp->x), (lpp->y), (lpp->z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[1]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[4], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[2]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[5], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));		

		//		LYNX_RV_SET_XYZ(V[6], (lpp->x), (lpp->y), (lpp->z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[2]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[7], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[0]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[8], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));		

		//		lynxDrawTriangles(V, 3);		
		//	}
		//	*/
		//}
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//void	CGeometry::RenderShadowVolume2(LPLYNXVERTEX3D lpv, LPLYNXFAN lpf, LPLYNXVECTOR3D lpp)
		//{
		//	/*
		//	int								i;
		//	LYNXRENDERVERTEX				V[9];
		//	LYNXVECTOR3D					Dir;
		//	float							Len;

		//	for (i=0; i<3; i++)
		//	{
		//		LYNX_RV_SET_RGBA(V[i*3+0], 0, 0, 0, 255);
		//		LYNX_RV_SET_RGBA(V[i*3+1], 0, 0, 0, 255);
		//		LYNX_RV_SET_RGBA(V[i*3+2], 0, 0, 0, 255);						

		//		LYNX_RV_SET_UV(V[i*3+0], 0, 4.0f, 0.0f);
		//		LYNX_RV_SET_UV(V[i*3+1], 0, 8.0f, 8.0f);
		//		LYNX_RV_SET_UV(V[i*3+2], 0, 8.0f, 8.0f);		
		//	}

		//	Len = 3.0f;
		//	for (i=0; i<m_NumTriangles; i++)
		//	{
		//		LYNX_RV_SET_XYZ(V[0], (lpp->x), (lpp->y), (lpp->z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[0]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[1], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[1]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[2], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));		

		//		LYNX_RV_SET_XYZ(V[3], (lpp->x), (lpp->y), (lpp->z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[1]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[4], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[2]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[5], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));		

		//		LYNX_RV_SET_XYZ(V[6], (lpp->x), (lpp->y), (lpp->z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[2]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[7], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));
		//		LYNX_VECTOR3D_SUB(Dir, lpv[lpf[m_BaseTriangle+i].lpv[0]], (*lpp));
		//		LYNX_RV_SET_XYZ(V[8], (lpp->x+Len*Dir.x), (lpp->y+Len*Dir.y), (lpp->z+Len*Dir.z));		

		//		lynxDrawTriangles(V, 3);		
		//	}
		//	*/
		//}
	}
}