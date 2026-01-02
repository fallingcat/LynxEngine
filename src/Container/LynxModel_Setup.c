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

#include <LynxModel.h>

#ifdef __OPENGLES__
	//void	ilynxOpenGLRenderMeshStrip(void *lpsm);
	void	ilynxOpenGLESRenderMeshNormal(void *lpsm);
	//void	ilynxOpenGLRenderModelStrip(void *lpsm);
	void	ilynxOpenGLESRenderModelNormal(void *lpsm);
#endif

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSetupModelMeshVertex(LPLYNXMODEL lpsm)
{
	int								i, j, BI;

	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL:
			BI = BONE_START_CONSTANT;
			for (i=0; i<lpsm->NumMeshs; i++)
			{
				//skip constant -38 & -37
				if (BI >= -41 && BI <= -37)
				{
					BI = -36;				
				}
				lpsm->lpMeshs[i].VertexConstantIndex = BI;
				for (j=lpsm->lpMeshs[i].BaseVertex[lpsm->CurrentLOD];  j<lpsm->lpMeshs[i].BaseVertex[0]+lpsm->lpMeshs[i].NumVertices[lpsm->CurrentLOD]; j++)
				{
					lpsm->lpMV[lpsm->CurrentLOD][j].MatrixIndex = BI/3;
				}
				BI+=3;						
			}

			for (i=0; i<lpsm->NumVertices[0]; i++)
			{	
				LYNXCOLOR_SET_RGBA(lpsm->lpMV[0][i].Color, lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Red,
														   lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Green,
														   lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Blue,
													       lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Alpha); 				

				lpsm->lpMV[lpsm->CurrentLOD][i].TexCoord[0].u = lpsm->lpUVVertexList[lpsm->CurrentLOD][i].u;
				lpsm->lpMV[lpsm->CurrentLOD][i].TexCoord[0].v = lpsm->lpUVVertexList[lpsm->CurrentLOD][i].v;					
				
				lpsm->lpMV[lpsm->CurrentLOD][i].Pos.s.x = lpsm->lpVertexList[lpsm->CurrentLOD][i].s.x;
				lpsm->lpMV[lpsm->CurrentLOD][i].Pos.s.y = lpsm->lpVertexList[lpsm->CurrentLOD][i].s.y;
				lpsm->lpMV[lpsm->CurrentLOD][i].Pos.s.z = lpsm->lpVertexList[lpsm->CurrentLOD][i].s.z;									
				lpsm->lpMV[lpsm->CurrentLOD][i].Normal.s.x = lpsm->lpVertexNormalList[lpsm->CurrentLOD][i].s.x;
				lpsm->lpMV[lpsm->CurrentLOD][i].Normal.s.y = lpsm->lpVertexNormalList[lpsm->CurrentLOD][i].s.y;
				lpsm->lpMV[lpsm->CurrentLOD][i].Normal.s.z = lpsm->lpVertexNormalList[lpsm->CurrentLOD][i].s.z;				
			}
			break;

		case LYNX_VERTEX_ANIMATION_MODEL:				
			break;			
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
int	ilynxGetVertexRelatedBoneConstantIndex(LPLYNXMESH lpmesh, int bi)
{
	int							i;

	for (i=0; i<lpmesh->NumBones; i++)
	{
		if (bi == lpmesh->lpBoneShaderData[i].BoneIndex)
		{
			return (lpmesh->lpBoneShaderData[i].ConstantIndex)/3;
		}
	}
	return BONE_START_CONSTANT;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSetupModelBlendVertex(LPLYNXMODEL lpsm)
{
	int								m, i, j, BI;	
	
	for (m=0; m<lpsm->NumMeshs; m++)
	{
		BI = BONE_START_CONSTANT;
		for (i=0; i<lpsm->lpMeshs[m].NumBones; i++)
		{
			//skip constant -38 & -37
			if (BI >= -41 && BI <= -37)
			{
				BI = -36;				
			}
			lpsm->lpMeshs[m].lpBoneShaderData[i].ConstantIndex = BI;							
			BI+=3;	//we only need the first 3 rows of bone matrix 					
		}
		for (i=lpsm->lpMeshs[m].BaseVertex[lpsm->CurrentLOD]; i<lpsm->lpMeshs[m].BaseVertex[lpsm->CurrentLOD]+lpsm->lpMeshs[m].NumVertices[lpsm->CurrentLOD]; i++)
		{					
			LYNXCOLOR_SET_RGBA(lpsm->lpBV[lpsm->CurrentLOD][i].Color, lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Red,
														lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Green,
														lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Blue,
														lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Alpha); 				

			lpsm->lpBV[lpsm->CurrentLOD][i].TexCoord[0].u = lpsm->lpUVVertexList[lpsm->CurrentLOD][i].u;
			lpsm->lpBV[lpsm->CurrentLOD][i].TexCoord[0].v = lpsm->lpUVVertexList[lpsm->CurrentLOD][i].v;

			lpsm->lpBV[lpsm->CurrentLOD][i].TexCoord[1].u = lpsm->lpUVVertexList[lpsm->CurrentLOD][i].u;
			lpsm->lpBV[lpsm->CurrentLOD][i].TexCoord[1].v = lpsm->lpUVVertexList[lpsm->CurrentLOD][i].v;
			
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[0].s.x = 0.0f;
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[0].s.y = 0.0f;
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[0].s.z = 0.0f;		
			lpsm->lpBV[lpsm->CurrentLOD][i].Weight[0] = 0.0f;		
			lpsm->lpBV[lpsm->CurrentLOD][i].BoneIndex[0] = BONE_START_CONSTANT;

			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[1].s.x = 0.0f;
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[1].s.y = 0.0f;
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[1].s.z = 0.0f;		
			lpsm->lpBV[lpsm->CurrentLOD][i].Weight[1] = 0.0f;		
			lpsm->lpBV[lpsm->CurrentLOD][i].BoneIndex[1] = BONE_START_CONSTANT;

			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[2].s.x = 0.0f;
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[2].s.y = 0.0f;
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[2].s.z = 0.0f;		
			lpsm->lpBV[lpsm->CurrentLOD][i].Weight[2] = 0.0f;		
			lpsm->lpBV[lpsm->CurrentLOD][i].BoneIndex[2] = BONE_START_CONSTANT;

			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[3].s.x = 0.0f;
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[3].s.y = 0.0f;
			lpsm->lpBV[lpsm->CurrentLOD][i].Offset[3].s.z = 0.0f;		
			lpsm->lpBV[lpsm->CurrentLOD][i].Weight[3] = 0.0f;		
			lpsm->lpBV[lpsm->CurrentLOD][i].BoneIndex[3] = BONE_START_CONSTANT;

			for (j=0; j<lpsm->lpWeightList[lpsm->CurrentLOD][i].NumBones; j++)
			{
				if (j < 4)
				{
					lpsm->lpBV[lpsm->CurrentLOD][i].Weight[j] = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]*LYNX_WEIGHT_ISCALE;
					lpsm->lpBV[lpsm->CurrentLOD][i].BoneIndex[j] = ilynxGetVertexRelatedBoneConstantIndex(&lpsm->lpMeshs[m], lpsm->lpWeightList[lpsm->CurrentLOD][i].lpBoneIndex[j]);				

					lpsm->lpBV[lpsm->CurrentLOD][i].Offset[j].s.x = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[j].s.x;
					lpsm->lpBV[lpsm->CurrentLOD][i].Offset[j].s.y = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[j].s.y;
					lpsm->lpBV[lpsm->CurrentLOD][i].Offset[j].s.z = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[j].s.z;		
				}			
				lpsm->lpBV[lpsm->CurrentLOD][i].Normal.s.x = lpsm->lpVertexNormalList[lpsm->CurrentLOD][i].s.x;
				lpsm->lpBV[lpsm->CurrentLOD][i].Normal.s.y = lpsm->lpVertexNormalList[lpsm->CurrentLOD][i].s.y;
				lpsm->lpBV[lpsm->CurrentLOD][i].Normal.s.z = lpsm->lpVertexNormalList[lpsm->CurrentLOD][i].s.z;															
			}		
		}
	}
}
//-------------------------------------------------------------------------------------------------------
/**
*  @ingroup LynxModel 
*  @brief 。
*
*	@param 。
*	@param 。
*	@param 。
*	@return。 
*/
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxSetupModel(LPLYNXMODEL lpsm)
{
	int					i, j, k;
	WORD				*lpI;
	WORD				T1, T2, T3, C1, C2, C3;		
	LPLYNXRENDERVERTEX  lpTVertexList;
	DWORD				NumIndices;
			
	if (lpsm->bUseBufferObject)
	{
		switch (lpsm->Type)
		{
			case LYNX_HIE_MODEL:
				lynxCreateVertexArray(&lpsm->VertexArray, lynxGetBuiltinVertexLayout(LYNX_MESH_VERTEX), lpsm->NumVertices[lpsm->CurrentLOD], LYNX_BOF_STATIC, LYNX_TRUE);
				lynxLockVertexArray(&lpsm->VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);
				lpsm->lpMV[lpsm->CurrentLOD] = (LPLYNXMESHVERTEX)lpsm->VertexArray.lpBuffer;
				lynxSetupModelMeshVertex(lpsm);	
				lynxUnlockVertexArray(&lpsm->VertexArray);	
				break;

			case LYNX_HIE_BONESKIN_MODEL:
				lynxCreateVertexArray(&lpsm->VertexArray, lynxGetBuiltinVertexLayout(LYNX_BLEND_VERTEX), lpsm->NumVertices[lpsm->CurrentLOD], LYNX_BOF_STATIC, LYNX_TRUE);
				lynxLockVertexArray(&lpsm->VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);
				lpsm->lpBV[lpsm->CurrentLOD] = (LPLYNXBLENDVERTEX)lpsm->VertexArray.lpBuffer;
				lynxSetupModelBlendVertex(lpsm);	
				lynxUnlockVertexArray(&lpsm->VertexArray);	
				break;
		}							

		NumIndices = 0;
		for (i=0; i<lpsm->NumMeshs; i++)
		{
			for (j=0; j<lpsm->lpMeshs[i].NumPrimitive; j++)
			{
				NumIndices += lpsm->lpMeshs[i].lpPrimitive[j].NumIndex;
			}
			lpsm->lpMeshs[i].Obj3D.Render = lynxRenderMeshBO;
		}
		lpsm->Mesh.Obj3D.Render = lynxRenderModel;				
		
		lynxCreateIndexArray(&lpsm->IndexArray, sizeof(WORD), NumIndices, LYNX_LOCK_TO_WRITE, LYNX_TRUE);
		lynxLockIndexArray(&lpsm->IndexArray, 0, 0, LYNX_LOCK_TO_WRITE);
		lpI = (WORD *)lpsm->IndexArray.lpBuffer;			
		NumIndices = 0;
		for (i=0; i<lpsm->NumMeshs; i++)
		{
			for (j=0; j<lpsm->lpMeshs[i].NumPrimitive; j++)
			{
				lpsm->lpMeshs[i].lpPrimitive[j].BaseIndex = NumIndices;
				for (k=0; k<lpsm->lpMeshs[i].lpPrimitive[j].NumIndex; k++)
				{
					lpI[NumIndices++] = lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k] - lpsm->lpMeshs[i].BaseVertex[lpsm->CurrentLOD];					
				}							
			}			
		}		
		lynxUnlockIndexArray(&lpsm->IndexArray);
		
		LYNX_FREE(lpsm->lpVertexNormalList[lpsm->CurrentLOD]);
		LYNX_FREE(lpsm->lpUVVertexList[lpsm->CurrentLOD]);
		LYNX_FREE(lpsm->lpColorVertexList[lpsm->CurrentLOD]);						
	}
	else
	{				
		lynxCreateVertexArray(&lpsm->VertexArray, lynxGetBuiltinVertexLayout(LYNX_RENDER_VERTEX), lpsm->NumVertices[lpsm->CurrentLOD], LYNX_BOF_DYNAMIC | LYNX_BOF_WRITEONLY, LYNX_FALSE);
		lynxLockVertexArray(&lpsm->VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);
		lpTVertexList = (LPLYNXRENDERVERTEX)lpsm->VertexArray.lpBuffer;
		for (i=0; i<lpsm->NumVertices[lpsm->CurrentLOD]; i++)
		{	
			LYNX_RV_SET_XYZ(lpTVertexList[i], 
							lpsm->lpVertexList[lpsm->CurrentLOD][i].s.x,
							lpsm->lpVertexList[lpsm->CurrentLOD][i].s.y,
							lpsm->lpVertexList[lpsm->CurrentLOD][i].s.z);

			//LYNX_RV_SET_NXYZ(lpTVertexList[i], 
			//				lpsm->lpVertexNormalList[0][i].s.x,
			//				lpsm->lpVertexNormalList[0][i].s.y,
			//				lpsm->lpVertexNormalList[0][i].s.z);

			LYNX_RV_SET_UV(lpTVertexList[i], 0, 
							lpsm->lpUVVertexList[lpsm->CurrentLOD][i].u,
							lpsm->lpUVVertexList[lpsm->CurrentLOD][i].v);							

			LYNX_RV_SET_RGBA(lpTVertexList[i], lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Red,
											   lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Green,
											   lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Blue,
											   lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Alpha); 				
		}
		lynxUnlockVertexArray(&lpsm->VertexArray);	
		
		NumIndices = 0;
		for (i=0; i<lpsm->NumMeshs; i++)
		{
			for (j=0; j<lpsm->lpMeshs[i].NumPrimitive; j++)
			{
				NumIndices += lpsm->lpMeshs[i].lpPrimitive[j].NumIndex;
			}
			lpsm->lpMeshs[i].Obj3D.Render = lynxRenderMesh;
		}
		lpsm->Mesh.Obj3D.Render = lynxRenderModel;				
		
		lynxCreateIndexArray(&lpsm->IndexArray, sizeof(WORD), NumIndices, LYNX_LOCK_TO_WRITE, LYNX_FALSE);
		lynxLockIndexArray(&lpsm->IndexArray, 0, 0, LYNX_LOCK_TO_WRITE);
		lpI = (WORD *)lpsm->IndexArray.lpBuffer;			
		NumIndices = 0;
		for (i=0; i<lpsm->NumMeshs; i++)
		{
			for (j=0; j<lpsm->lpMeshs[i].NumPrimitive; j++)
			{
				lpsm->lpMeshs[i].lpPrimitive[j].BaseIndex = NumIndices;
				for (k=0; k<lpsm->lpMeshs[i].lpPrimitive[j].NumIndex; k++)
				{
					lpI[NumIndices++] = lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k] - lpsm->lpMeshs[i].BaseVertex[lpsm->CurrentLOD];					
				}							
			}			
		}		
		lynxUnlockIndexArray(&lpsm->IndexArray);
	}
	for (i=0; i<lpsm->NumMeshs; i++)
	{
		lynxInstanceVertexArray(&lpsm->lpMeshs[i].VertexArray, &lpsm->VertexArray);
		lynxInstanceIndexArray(&lpsm->lpMeshs[i].IndexArray, &lpsm->IndexArray);
	}

	//Yahoo!!
	return TRUE;
}

#ifdef __OPENGLES__
	//-------------------------------------------------------------------------------------------------------
	/**
	*  @ingroup LynxModel 
	*  @brief 。
	*
	*	@param 。
	*	@param 。
	*	@param 。
	*	@return。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL	ilynxOGLES_SetupModel(LPLYNXMODEL lpsm)
	{			
		int								i;
		unsigned short					*lpI;

		lpsm->lpTVertexList[lpsm->CurrentLOD] = (LPLYNXRENDERVERTEX)LYNX_MALLOC(lpsm->NumVertices[lpsm->CurrentLOD]*sizeof(LYNXRENDERVERTEX));
		if (lpsm->lpTVertexList[lpsm->CurrentLOD] == NULL)
			return LYNX_FALSE;	
		//lpsm->lpTVertexNormalList[0]= (LPLYNXVECTOR3D)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));
		//if (lpsm->lpTVertexNormalList[0] == NULL)
		//	return LYNX_FALSE;		

		if (lpsm->bStripify)
		{	
			//lpsm->Mesh.Obj3D.lpRenderProc = ilynxOpenGLRenderModelStrip;		
			lynxCreateIndexArray(&lpsm->IndexArray, sizeof(unsigned short), lpsm->NumTriangles[lpsm->CurrentLOD]*3, LYNX_LOCK_TO_WRITE);			
			lpI = (unsigned short *)lpsm->IndexArray.lpBuffer;			
			for (i=0; i<lpsm->NumTriangles[lpsm->CurrentLOD]; i++)
			{								
				lpI[i*3] = lpsm->lpTriangleList[lpsm->CurrentLOD][i].v1;
				lpI[i*3+1] = lpsm->lpTriangleList[lpsm->CurrentLOD][i].v2;
				lpI[i*3+2] = lpsm->lpTriangleList[lpsm->CurrentLOD][i].v3;					
			}			
			for (i=0; i<lpsm->NumVertices[lpsm->CurrentLOD]; i++)
			{
				switch (lpsm->Type)
				{
					case LYNX_VERTEX_ANIMATION_MODEL:
					case LYNX_HIE_MODEL:
						lpsm->lpTVertexList[lpsm->CurrentLOD][i].x = lpsm->lpVertexList[lpsm->CurrentLOD][i].s.x;
						lpsm->lpTVertexList[lpsm->CurrentLOD][i].y = lpsm->lpVertexList[lpsm->CurrentLOD][i].s.y;
						lpsm->lpTVertexList[lpsm->CurrentLOD][i].z = lpsm->lpVertexList[lpsm->CurrentLOD][i].s.z;
						break;

					case LYNX_HIE_BONESKIN_MODEL:
						lpsm->lpTVertexList[lpsm->CurrentLOD][i].x = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[0].s.x;
						lpsm->lpTVertexList[lpsm->CurrentLOD][i].y = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[0].s.y;
						lpsm->lpTVertexList[lpsm->CurrentLOD][i].z = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[0].s.z;
						break;
				}
				lpsm->lpTVertexList[0][i].TexCoord[0].u = lpsm->lpUVVertexList[0][i].u;
				lpsm->lpTVertexList[0][i].TexCoord[0].v = lpsm->lpUVVertexList[0][i].v;
				lpsm->lpTVertexList[0][i].color.PackedColor = lpsm->lpColorVertexList[0][i].PackedColor;		
			}				
			for (i=0; i<lpsm->NumMeshs; i++)
			{
				lpsm->lpMeshs[i].Obj3D.Render = ilynxOpenGLESRenderMeshNormal;
			}
			lpsm->Mesh.Obj3D.Render = ilynxOpenGLESRenderModelNormal;
		}
		else
		{		
			lynxCreateIndexArray(&lpsm->IndexArray, sizeof(unsigned short), lpsm->NumTriangles[0]*3, LYNX_LOCK_TO_WRITE);			
			lpI = (unsigned short *)lpsm->IndexArray.lpBuffer;			
			for (i=0; i<lpsm->NumTriangles[0]; i++)
			{								
				lpI[i*3] = lpsm->lpTriangleList[0][i].v1;
				lpI[i*3+1] = lpsm->lpTriangleList[0][i].v2;
				lpI[i*3+2] = lpsm->lpTriangleList[0][i].v3;					
			}

			#ifdef __USE_VBO__		
				if (lpsm->SourceType == LYNX_ST_ORIGIN)
				{
					if (!strstr(lpsm->Mesh.Obj3D.Name, "camera") && !strstr(lpsm->Mesh.Obj3D.Name, "bullet"))
					{					
						//vertex buffer									
						glGenBuffers(1, &(lpsm->VertexBuffer));	
						glBindBuffer(GL_ARRAY_BUFFER, lpsm->VertexBuffer);		
						glBufferData(GL_ARRAY_BUFFER, lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D), lpsm->lpVertexList[0], GL_STATIC_DRAW);
						
						glGenBuffers(1, &(lpsm->UVBuffer));
						glBindBuffer(GL_ARRAY_BUFFER, lpsm->UVBuffer);
						glBufferData(GL_ARRAY_BUFFER, lpsm->NumVertices[0]*sizeof(LYNXUVPOINT), lpsm->lpUVVertexList[0], GL_STATIC_DRAW);		
						
						glGenBuffers(1, &(lpsm->ColorBuffer));
						glBindBuffer(GL_ARRAY_BUFFER, lpsm->ColorBuffer);
						glBufferData(GL_ARRAY_BUFFER, lpsm->NumVertices[0]*sizeof(LYNXCOLORRGBA), lpsm->lpColorVertexList[0], GL_STATIC_DRAW);	
						
						/*
						for (i=0; i<lpsm->NumVertices[0]; i++)
						{
							switch (lpsm->Type)
							{
								case LYNX_VERTEX_ANIMATION_MODEL:
								case LYNX_HIE_MODEL:
									lpsm->lpTVertexList[0][i].x = lpsm->lpVertexList[0][i].s.x;
									lpsm->lpTVertexList[0][i].y = lpsm->lpVertexList[0][i].s.y;
									lpsm->lpTVertexList[0][i].z = lpsm->lpVertexList[0][i].s.z;
									break;

								case LYNX_HIE_BONESKIN_MODEL:
									lpsm->lpTVertexList[0][i].x = lpsm->lpWeightList[0][i].lpOffset[0].s.x;
									lpsm->lpTVertexList[0][i].y = lpsm->lpWeightList[0][i].lpOffset[0].s.y;
									lpsm->lpTVertexList[0][i].z = lpsm->lpWeightList[0][i].lpOffset[0].s.z;
									break;
							}
							lpsm->lpTVertexList[0][i].TexCoord[0].u = lpsm->lpUVVertexList[0][i].u;
							lpsm->lpTVertexList[0][i].TexCoord[0].v = lpsm->lpUVVertexList[0][i].v;
							lpsm->lpTVertexList[0][i].color.PackedColor = lpsm->lpColorVertexList[0][i].PackedColor;		
						}	
						glGenBuffers(1, &(lpsm->VertexBuffer));	
						glBindBuffer(GL_ARRAY_BUFFER, lpsm->VertexBuffer);		
						glBufferData(GL_ARRAY_BUFFER, lpsm->NumVertices[0]*sizeof(LYNXRENDERVERTEX), lpsm->lpTVertexList[0], GL_STATIC_DRAW);
						*/						
						glBindBuffer(GL_ARRAY_BUFFER, 0);					
						LYNX_FREE(lpsm->lpTVertexList[0]);
					}
					else
					{
						for (i=0; i<lpsm->NumVertices[0]; i++)
						{
							switch (lpsm->Type)
							{
								case LYNX_VERTEX_ANIMATION_MODEL:
								case LYNX_HIE_MODEL:
									lpsm->lpTVertexList[0][i].x = lpsm->lpVertexList[0][i].s.x;
									lpsm->lpTVertexList[0][i].y = lpsm->lpVertexList[0][i].s.y;
									lpsm->lpTVertexList[0][i].z = lpsm->lpVertexList[0][i].s.z;
									break;

								case LYNX_HIE_BONESKIN_MODEL:
									lpsm->lpTVertexList[0][i].x = lpsm->lpWeightList[0][i].lpOffset[0].s.x;
									lpsm->lpTVertexList[0][i].y = lpsm->lpWeightList[0][i].lpOffset[0].s.y;
									lpsm->lpTVertexList[0][i].z = lpsm->lpWeightList[0][i].lpOffset[0].s.z;
									break;
							}
							if (lpsm->lpUVVertexList[0])
							{
								lpsm->lpTVertexList[0][i].TexCoord[0].u = lpsm->lpUVVertexList[0][i].u;
								lpsm->lpTVertexList[0][i].TexCoord[0].v = lpsm->lpUVVertexList[0][i].v;
							}
							if (lpsm->lpColorVertexList[0])
								lpsm->lpTVertexList[0][i].color.PackedColor = lpsm->lpColorVertexList[0][i].PackedColor;		
						}
					}
				}
			#else
				for (i=0; i<lpsm->NumVertices[0]; i++)
				{
					switch (lpsm->Type)
					{
						case LYNX_VERTEX_ANIMATION_MODEL:
						case LYNX_HIE_MODEL:
							lpsm->lpTVertexList[0][i].x = lpsm->lpVertexList[0][i].s.x;
							lpsm->lpTVertexList[0][i].y = lpsm->lpVertexList[0][i].s.y;
							lpsm->lpTVertexList[0][i].z = lpsm->lpVertexList[0][i].s.z;
							break;

						case LYNX_HIE_BONESKIN_MODEL:
							lpsm->lpTVertexList[0][i].x = lpsm->lpWeightList[0][i].lpOffset[0].s.x;
							lpsm->lpTVertexList[0][i].y = lpsm->lpWeightList[0][i].lpOffset[0].s.y;
							lpsm->lpTVertexList[0][i].z = lpsm->lpWeightList[0][i].lpOffset[0].s.z;
							break;
					}
					if (lpsm->lpUVVertexList[0])
					{
						lpsm->lpTVertexList[0][i].TexCoord[0].u = lpsm->lpUVVertexList[0][i].u;
						lpsm->lpTVertexList[0][i].TexCoord[0].v = lpsm->lpUVVertexList[0][i].v;
					}
					if (lpsm->lpColorVertexList[0])
						lpsm->lpTVertexList[0][i].color.PackedColor = lpsm->lpColorVertexList[0][i].PackedColor;		
				}
			#endif
			
			for (i=0; i<lpsm->NumMeshs; i++)
			{
				lpsm->lpMeshs[i].Obj3D.Render = ilynxOpenGLESRenderMeshNormal;
			}
			lpsm->Mesh.Obj3D.Render = ilynxOpenGLESRenderModelNormal;
		}

		LYNX_FREE(lpsm->lpUVVertexList[0]);
		LYNX_FREE(lpsm->lpColorVertexList[0]);
		LYNX_FREE(lpsm->lpTriangleList[0]);
		LYNX_FREE(lpsm->lpUVTriangleList[0]);
		LYNX_FREE(lpsm->lpColorTriangleList[0]);
		LYNX_FREE(lpsm->lpVertexNormalList[0]);

		//Yahoo!!
		return TRUE;
	}
#endif