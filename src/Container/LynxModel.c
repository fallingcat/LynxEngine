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

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 初始化LYNXMODEL資料結構。
 *
 *	@param lpsm 欲初始化之LYNXMODEL資料指標【輸入/輸出】。
 *	@return 無。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxInitModel(LPLYNXMODEL lpsm)
{
	int						i;

	lynxInitMesh(&lpsm->Mesh);
	lpsm->Attribute = 0x0;	
	lpsm->bUseBufferObject = LYNX_FALSE;
	lpsm->lpMaterials = NULL;
	lpsm->NumBones = 0;
	lpsm->lpBones = NULL;
	lpsm->NumMeshs = 0;
	lpsm->lpMeshs = NULL;
	for (i=0; i<LYNX_MAX_LOD; i++)
	{
		lpsm->NumVertices[i] = 0;
		lpsm->lpVertexList[i] = NULL;				
		lpsm->lpVertexTangentList[i] = NULL;
		lpsm->lpVertexBinormalList[i] = NULL;				
		lpsm->lpVertexNormalList[i] = NULL;				
		lpsm->NumTriangles[i] = 0;
		lpsm->lpTriangleList[i] = NULL;	
		lpsm->lpWeightList[i] = NULL;
	}
	lynxInitFile(&lpsm->AniStr); 	
	lpsm->NumLOD = 1;	
	lpsm->CurrentLOD = 0;
	/*
	lpsm->Time = LYNX_REAL(0);
	lpsm->StartTime = LYNX_REAL(0);
	lpsm->EndTime = LYNX_REAL(0);
	*/
	lpsm->lplpTraverseBones = NULL;
	lpsm->lplpTraverseMeshs = NULL;	
	lpsm->SkipFrames = 1;
	LYNX_IDENTITY_MATRIX(lpsm->Mesh.Obj3D.TransM);
	//lynxInitMemStr(&lpsm->AniStr);		
	lpsm->bUseBufferObject = FALSE;
	lynxInitVertexArray(&lpsm->VertexArray);
	lynxInitIndexArray(&lpsm->IndexArray);	
	lpsm->Mesh.Obj3D.Play = (LPLYNXPLAYPROC)lynxPlayModel;
	lpsm->Mesh.Obj3D.Render = (LPLYNXRENDERPROC)lynxRenderModel;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 釋放LYNXMODEL資料結構所包含資料。
 *
 *	@param lpd 欲初始化之LYNXMODEL資料指標【輸入/輸出】。
 *	@return 無。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxReleaseModel(LPLYNXMODEL lpd)
{
	int							i, j;

	if (lpd->SourceType == LYNX_ST_INSTANCE)
	{
		lynxSetLogf("\tLynxGraphics", "Release instance model <%s>\n", lpd->Mesh.Obj3D.Name);
		if (lpd->lpMaterials)
		{
			for (i=0; i<lpd->NumMaterials; i++)
			{
				lynxReleaseMaterial(&lpd->lpMaterials[i]);								
			}
			LYNX_FREE(lpd->lpMaterials);
		}
		switch (lpd->Type)
		{
			case LYNX_HIE_MODEL :				
				if (lpd->lpUVVertexList[0])
					LYNX_FREE(lpd->lpUVVertexList[0]);
				if (lpd->lpColorVertexList[0])
					LYNX_FREE(lpd->lpColorVertexList[0]);
				break;			

			case LYNX_HIE_BONESKIN_MODEL:				
				if (lpd->lpUVVertexList[0])
					LYNX_FREE(lpd->lpUVVertexList[0]);
				if (lpd->lpColorVertexList[0])
					LYNX_FREE(lpd->lpColorVertexList[0]);
				break;

			case LYNX_VERTEX_ANIMATION_MODEL :
				if (lpd->lpVertexList[0])
					LYNX_FREE(lpd->lpVertexList[0]);					
				if (lpd->lpUVVertexList[0])
					LYNX_FREE(lpd->lpUVVertexList[0]);
				if(lpd->lpColorVertexList[0])
					LYNX_FREE(lpd->lpColorVertexList[0]);
				break;
		}					
		for (i=0; i<lpd->NumMeshs; i++)
		{				
			if (lpd->lpMeshs[i].Obj3D.lpChildrenData)
				LYNX_FREE(lpd->lpMeshs[i].Obj3D.lpChildrenData);
		}			
		if (lpd->lpMeshs)
			LYNX_FREE(lpd->lpMeshs);

		if (lpd->Mesh.Obj3D.lpChildrenData)
			LYNX_FREE(lpd->Mesh.Obj3D.lpChildrenData);
		
		for (i=0; i<lpd->NumBones; i++)
		{			
			if (lpd->lpBones[i].Obj3D.lpChildrenData)
				LYNX_FREE(lpd->lpBones[i].Obj3D.lpChildrenData);
		}
		LYNX_FREE(lpd->lpBones);
		if (lpd->lplpTraverseMeshs)
			LYNX_FREE(lpd->lplpTraverseMeshs);
		if (lpd->lplpTraverseBones)
			LYNX_FREE(lpd->lplpTraverseBones);		
	}	
	else
	{		
		lynxSetLogf("\tLynxGraphics", "Release original model <%s>\n", lpd->Mesh.Obj3D.Name);
		if (lpd->lpMaterials)
		{
			for (i=0; i<lpd->NumMaterials; i++)
			{
				lynxReleaseMaterial(&lpd->lpMaterials[i]);												
			}
			LYNX_FREE(lpd->lpMaterials);
		}

		switch (lpd->Type)
		{
			default:
			case LYNX_HIE_MODEL :			
			case LYNX_HIE_BONESKIN_MODEL:
			case LYNX_VERTEX_ANIMATION_MODEL :
				if (lpd->lpVertexList[0])
					LYNX_FREE(lpd->lpVertexList[0]);												
				if (lpd->lpVertexNormalList[0])
					LYNX_FREE(lpd->lpVertexNormalList[0]);								
				if (lpd->lpUVVertexList[0])
					LYNX_FREE(lpd->lpUVVertexList[0]);				
				if (lpd->lpColorVertexList[0])
					LYNX_FREE(lpd->lpColorVertexList[0]);
				if (lpd->lpTriangleList[0])
					LYNX_FREE(lpd->lpTriangleList[0]);				
				break;
		}				
		for (i=0; i<lpd->NumMeshs; i++)
		{	
			if (lpd->lpMeshs[i].TextureMotionArg.lpArg)
				LYNX_FREE(lpd->lpMeshs[i].TextureMotionArg.lpArg);			
			if (lpd->lpMeshs[i].Obj3D.lpChildrenData)
				LYNX_FREE(lpd->lpMeshs[i].Obj3D.lpChildrenData);
			LYNX_FREE(lpd->lpMeshs[i].lpBoneShaderData);

			lynxReleaseAnimationKey(&lpd->lpMeshs[i].Obj3D.TranslateKey);
			lynxReleaseAnimationKey(&lpd->lpMeshs[i].Obj3D.ScaleKey);
			#ifdef __FIXED_POINT__
				lynxReleaseAnimationKey(&lpd->lpMeshs[i].Obj3D.RotateKey);
				lynxReleaseAnimationKey(&lpd->lpMeshs[i].Obj3D.VisKey);			
			#else
				lynxReleaseShortAnimationKey(&lpd->lpMeshs[i].Obj3D.RotateKey);
				lynxReleaseShortAnimationKey(&lpd->lpMeshs[i].Obj3D.VisKey);			
			#endif			
			if (lpd->lpMeshs[i].lpPrimitive)
			{
				for (j=0; j<lpd->lpMeshs[i].NumPrimitive; j++)
				{
					if (lpd->lpMeshs[i].lpPrimitive[j].lpIndex)
					{
						LYNX_FREE(lpd->lpMeshs[i].lpPrimitive[j].lpIndex);						
					}
				}
				LYNX_FREE(lpd->lpMeshs[i].lpPrimitive);
			}
		}	
		if (lpd->lpMeshs)
			LYNX_FREE(lpd->lpMeshs);

		if (lpd->Mesh.Obj3D.lpChildrenData)
			LYNX_FREE(lpd->Mesh.Obj3D.lpChildrenData);	

		if (lpd->lpWeightList[lpd->CurrentLOD])
		{
			for (j=0; j<lpd->NumVertices[lpd->CurrentLOD]; j++)
			{
				if (lpd->lpWeightList[lpd->CurrentLOD][j].lpBoneIndex)
					LYNX_FREE(lpd->lpWeightList[lpd->CurrentLOD][j].lpBoneIndex);
				if (lpd->lpWeightList[lpd->CurrentLOD][j].lpWeight)
					LYNX_FREE(lpd->lpWeightList[lpd->CurrentLOD][j].lpWeight);				
				if (lpd->lpWeightList[lpd->CurrentLOD][j].lpOffset)
					LYNX_FREE(lpd->lpWeightList[lpd->CurrentLOD][j].lpOffset);				
				if (lpd->lpWeightList[lpd->CurrentLOD][j].lpOffsetNormal)
					LYNX_FREE(lpd->lpWeightList[lpd->CurrentLOD][j].lpOffsetNormal);					
			}
			LYNX_FREE(lpd->lpWeightList[lpd->CurrentLOD]);
		}

		for (i=0; i<lpd->NumBones; i++)
		{	
			lynxReleaseAnimationKey(&lpd->lpBones[i].Obj3D.TranslateKey);
			lynxReleaseAnimationKey(&lpd->lpBones[i].Obj3D.ScaleKey);			
			#ifdef __FIXED_POINT__
				lynxReleaseAnimationKey(&lpd->lpBones[i].Obj3D.RotateKey);				
			#else
				lynxReleaseShortAnimationKey(&lpd->lpBones[i].Obj3D.RotateKey);
			#endif			
			
			if (lpd->lpBones[i].Obj3D.lpChildrenData)
				LYNX_FREE(lpd->lpBones[i].Obj3D.lpChildrenData);
		}
		lynxReleaseVertexArray(&lpd->VertexArray);				
		lynxReleaseIndexArray(&lpd->IndexArray);				
		LYNX_FREE(lpd->lpBones);
		if (lpd->lplpTraverseMeshs)
			LYNX_FREE(lpd->lplpTraverseMeshs);
		if (lpd->lplpTraverseBones)
			LYNX_FREE(lpd->lplpTraverseBones);		
		LYNX_CLOSE_FILE(&lpd->AniStr);
		
		#ifdef __USE_VBO__		
			glDeleteBuffers(1, &(lpd->VertexBuffer));	
			glDeleteBuffers(1, &(lpd->UVBuffer));	
			glDeleteBuffers(1, &(lpd->ColorBuffer));	
		#endif		
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 製做一個LYNXMODEL資料的副本。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
  *	@see lynxDuplicateModel 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInstanceModel(LPLYNXMODEL lpd, LPCLYNXMODEL lpsm)
{
	int					i, j;
	int					RootBone, RootMesh;
	int					T1, T2, T3, C1, C2, C3;

	lynxInitModel(lpd);
	lynxInstanceMesh(&lpd->Mesh, &lpsm->Mesh);	
	lpd->Type = lpsm->Type;
	lpd->Attribute = lpsm->Attribute;
	lpd->SourceType = LYNX_ST_INSTANCE;	
	lpd->NumFrames = lpsm->NumFrames;
	lpd->NumLOD = lpsm->NumLOD;
	lpd->NumMaterials = lpsm->NumMaterials;	
	lpd->SkipFrames = lpsm->SkipFrames;

	lpd->NumVertices[0] = lpsm->NumVertices[0];
	lpd->NumTriangles[0] = lpsm->NumTriangles[0];		
	
	if (lpd->NumMaterials)
	{
		lpd->lpMaterials = (LPLYNXMATERIAL)LYNX_MALLOC(lpd->NumMaterials*sizeof(LYNXMATERIAL));
		if (lpd->lpMaterials)
		{
			for (i=0; i<lpsm->NumMaterials; i++)
			{
				lynxInstanceMaterial(&lpd->lpMaterials[i], &lpsm->lpMaterials[i]);								
			}
		}
	}

	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL :
			lpd->lpVertexList[0] = lpsm->lpVertexList[0];	
			lpd->lpVertexNormalList[0] = lpsm->lpVertexNormalList[0];			
			if (lpsm->lpUVVertexList[0])
			{
				lpd->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXUVPOINT));
				lynxMemCpy(lpd->lpUVVertexList[0], lpsm->lpUVVertexList[0], sizeof(LYNXUVPOINT)*lpsm->NumVertices[0]);	
			}
			if (lpsm->lpColorVertexList[0])
			{
				lpd->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXCOLORRGBA));
				lynxMemCpy(lpd->lpColorVertexList[0], lpsm->lpColorVertexList[0], sizeof(LYNXCOLORRGBA)*lpsm->NumVertices[0]);	
			}
			lynxInstanceVertexArray(&lpd->VertexArray, &lpsm->VertexArray);
			lynxInstanceIndexArray(&lpd->IndexArray, &lpsm->IndexArray);
			lpd->lpMV[0] = lpsm->lpMV[0];
			break;
		
		case LYNX_HIE_BONESKIN_MODEL:
			lpd->lpVertexList[0] = lpsm->lpVertexList[0];	
			lpd->lpVertexNormalList[0] = lpsm->lpVertexNormalList[0];						
			if (lpsm->lpUVVertexList[0])
			{
				lpd->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXUVPOINT));
				lynxMemCpy(lpd->lpUVVertexList[0], lpsm->lpUVVertexList[0], sizeof(LYNXUVPOINT)*lpsm->NumVertices[0]);	
			}

			if (lpsm->lpColorVertexList[0])
			{
				lpd->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXCOLORRGBA));
				lynxMemCpy(lpd->lpColorVertexList[0], lpsm->lpColorVertexList[0], sizeof(LYNXCOLORRGBA)*lpsm->NumVertices[0]);	
			}			

			lpd->lpWeightList[lpd->CurrentLOD] = lpsm->lpWeightList[lpd->CurrentLOD];			
			lynxInstanceVertexArray(&lpd->VertexArray, &lpsm->VertexArray);
			lynxInstanceIndexArray(&lpd->IndexArray, &lpsm->IndexArray);
			lpd->lpBV[lpd->CurrentLOD] = lpsm->lpBV[lpd->CurrentLOD];
			break;

		case LYNX_VERTEX_ANIMATION_MODEL :
			lpd->lpVertexList[0] = (LPLYNXVECTOR3D)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));				
			lpd->lpVertexNormalList[0] = lpsm->lpVertexNormalList[0];
			if (lpsm->lpUVVertexList[0])
			{
				lpd->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXUVPOINT));
				lynxMemCpy(lpd->lpUVVertexList[0], lpsm->lpUVVertexList[0], sizeof(LYNXUVPOINT)*lpsm->NumVertices[0]);	
			}

			if (lpsm->lpColorVertexList[0])
			{
				lpd->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXCOLORRGBA));
				lynxMemCpy(lpd->lpColorVertexList[0], lpsm->lpColorVertexList[0], sizeof(LYNXCOLORRGBA)*lpsm->NumVertices[0]);	
			}
			break;
	}			
	lpd->lpTriangleList[0] = lpsm->lpTriangleList[0];	
	lynxMemCpy(&lpd->AniStr, &lpsm->AniStr, sizeof(LYNXFILE));
	lpd->bUseBufferObject = lpsm->bUseBufferObject;	
	
	lpd->NumMeshs = lpsm->NumMeshs;
	lpd->lpMeshs = (LPLYNXMESH)LYNX_MALLOC(lpd->NumMeshs*sizeof(LYNXMESH));
	for (i=0; i<lpd->NumMeshs; i++)
	{
		lynxInitMesh(&lpd->lpMeshs[i]);
		lynxInstanceMesh(&lpd->lpMeshs[i], &lpsm->lpMeshs[i]);
		lpd->lpMeshs[i].lpModel = lpd;		
		if (lpd->lpMeshs[i].Obj3D.ParentData.ChildIndex == -1)
		{
			RootMesh = i;			
			lpd->lpMeshs[i].Obj3D.ParentData.lpObject = (LPVOID)&lpd->Mesh;		
		}
		else
		{
			lpd->lpMeshs[i].Obj3D.ParentData.lpObject = (LPVOID)&lpd->lpMeshs[lpd->lpMeshs[i].Obj3D.ParentData.ChildIndex];		
		}
		for (j=0; j<lpd->lpMeshs[i].Obj3D.NumChildren; j++)
		{
			lpd->lpMeshs[i].Obj3D.lpChildrenData[j].lpObject = (LPVOID)&lpd->lpMeshs[lpd->lpMeshs[i].Obj3D.lpChildrenData[j].ChildIndex];		
		}
		lpd->lpMeshs[i].NumPrimitive = lpsm->lpMeshs[i].NumPrimitive;
		lpd->lpMeshs[i].lpPrimitive = lpsm->lpMeshs[i].lpPrimitive;
	}		

	if (lpd->Mesh.Obj3D.lpChildrenData)
		lpd->Mesh.Obj3D.lpChildrenData[0].lpObject = (LPVOID)&lpd->lpMeshs[RootMesh];	

	lpd->NumBones = lpsm->NumBones;
	lpd->lpBones = (LPLYNXBONE)LYNX_MALLOC(sizeof(LYNXBONE)*lpsm->NumBones);
	for (i=0; i<lpd->NumBones; i++)
	{
		lynxInitBone(&lpd->lpBones[i]);
		lynxInstanceBone(&lpd->lpBones[i], &lpsm->lpBones[i]);
		if (lpd->lpBones[i].Obj3D.ParentData.ChildIndex == -1)
		{
			RootBone = i;			
			lpd->lpBones[i].Obj3D.ParentData.lpObject = (LPVOID)&lpd->Mesh;		
		}
		else
		{
			lpd->lpBones[i].Obj3D.ParentData.lpObject = (LPVOID)&lpd->lpBones[lpd->lpBones[i].Obj3D.ParentData.ChildIndex];		
		}		
		for (j=0; j<lpd->lpBones[i].Obj3D.NumChildren; j++)
		{
			lpd->lpBones[i].Obj3D.lpChildrenData[j].lpObject = &lpd->lpBones[lpd->lpBones[i].Obj3D.lpChildrenData[j].ChildIndex];				
		}
	}
	lpd->lpWeightList[0] = lpsm->lpWeightList[0];
	//lynxMemCpy(&lpd->BaseTM, &lpsm->BaseTM, sizeof(LYNXMATRIX)); 
	//lynxMemCpy(&lpd->InverseBaseTM, &lpsm->InverseBaseTM, sizeof(LYNXMATRIX)); 

	if (lpd->NumMeshs)
		lynxTraverseModelMesh(lpd, &lpd->lpMeshs[RootMesh]);	
	if (lpd->NumBones)
		lynxTraverseModelBone(lpd, &lpd->lpBones[RootBone]);

	lpd->Mesh.Obj3D.Play = lpsm->Mesh.Obj3D.Play;
	lpd->Mesh.Obj3D.Render = lpsm->Mesh.Obj3D.Render;
}	
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 複製一個LYNXMODEL資料。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
  *	@see lynxInstanceModel 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxDuplicateModel(LPLYNXMODEL lpd, LPCLYNXMODEL lpsm)
{
	int					i, j;
	int					RootBone, RootMesh;
	int					T1, T2, T3, C1, C2, C3;

	lynxInitModel(lpd);
	lpd->Type = lpsm->Type;
	lpd->Attribute = lpsm->Attribute;
	lpd->SourceType = LYNX_ST_INSTANCE;

	lynxDuplicateObject3D(&lpd->Mesh.Obj3D, &lpsm->Mesh.Obj3D);
	//strcpy(lpd->Mesh.Obj3D.Name, lpsm->Mesh.Obj3D.Name);
	lpd->NumFrames = lpsm->NumFrames;
	lpd->NumLOD = lpsm->NumLOD;
	lpd->NumMaterials = lpsm->NumMaterials;	
	//lpd->StartTime = lpsm->StartTime;
	//lpd->EndTime = lpsm->EndTime;
	//lpd->Time = lpsm->Time;
	lpd->SkipFrames = lpsm->SkipFrames;

	lpd->NumVertices[0] = lpsm->NumVertices[0];	
	lpd->NumTriangles[0] = lpsm->NumTriangles[0];		

	if (lpd->NumMaterials)
	{
		lpd->lpMaterials = (LPLYNXMATERIAL)LYNX_MALLOC(lpd->NumMaterials*sizeof(LYNXMATERIAL));
		if (lpd->lpMaterials)
		{
			for (i=0; i<lpsm->NumMaterials; i++)
			{
				lynxDuplicateMaterial(&lpd->lpMaterials[i], &lpsm->lpMaterials[i]);								
			}
		}
	}

	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL :
			lpd->lpVertexList[0] = lpsm->lpVertexList[0];	
			lpd->lpVertexNormalList[0] = lpsm->lpVertexNormalList[0];			
			if (lpsm->lpUVVertexList[0])
			{
				lpd->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXUVPOINT));
				lynxMemCpy(lpd->lpUVVertexList[0], lpsm->lpUVVertexList[0], sizeof(LYNXUVPOINT)*lpsm->NumVertices[0]);	
			}
			if (lpsm->lpColorVertexList[0])
			{
				lpd->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXCOLORRGBA));
				lynxMemCpy(lpd->lpColorVertexList[0], lpsm->lpColorVertexList[0], sizeof(LYNXCOLORRGBA)*lpsm->NumVertices[0]);	
			}
			lynxInstanceVertexArray(&lpd->VertexArray, &lpsm->VertexArray);
			lynxInstanceIndexArray(&lpd->IndexArray, &lpsm->IndexArray);
			lpd->lpMV[0] = lpsm->lpMV[0];
			break;
		
		case LYNX_HIE_BONESKIN_MODEL:
			lpd->lpVertexList[0] = lpsm->lpVertexList[0];	
			lpd->lpVertexNormalList[0] = lpsm->lpVertexNormalList[0];						
			if (lpsm->lpUVVertexList[0])
			{
				lpd->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXUVPOINT));
				lynxMemCpy(lpd->lpUVVertexList[0], lpsm->lpUVVertexList[0], sizeof(LYNXUVPOINT)*lpsm->NumVertices[0]);	
			}

			if (lpsm->lpColorVertexList[0])
			{
				lpd->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXCOLORRGBA));
				lynxMemCpy(lpd->lpColorVertexList[0], lpsm->lpColorVertexList[0], sizeof(LYNXCOLORRGBA)*lpsm->NumVertices[0]);	
			}			

			lpd->lpWeightList[lpd->CurrentLOD] = lpsm->lpWeightList[lpd->CurrentLOD];			
			lynxInstanceVertexArray(&lpd->VertexArray, &lpsm->VertexArray);
			lynxInstanceIndexArray(&lpd->IndexArray, &lpsm->IndexArray);
			lpd->lpBV[lpd->CurrentLOD] = lpsm->lpBV[lpd->CurrentLOD];
			break;

		case LYNX_VERTEX_ANIMATION_MODEL :
			lpd->lpVertexList[0] = (LPLYNXVECTOR3D)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));				
			lpd->lpVertexNormalList[0] = lpsm->lpVertexNormalList[0];			
			if (lpsm->lpUVVertexList[0])
			{
				lpd->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXUVPOINT));
				lynxMemCpy(lpd->lpUVVertexList[0], lpsm->lpUVVertexList[0], sizeof(LYNXUVPOINT)*lpsm->NumVertices[0]);	
			}

			if (lpsm->lpColorVertexList[0])
			{
				lpd->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXCOLORRGBA));
				lynxMemCpy(lpd->lpColorVertexList[0], lpsm->lpColorVertexList[0], sizeof(LYNXCOLORRGBA)*lpsm->NumVertices[0]);	
			}
			break;
	}			
	lpd->lpTriangleList[0] = lpsm->lpTriangleList[0];	
	lynxMemCpy(&lpd->AniStr, &lpsm->AniStr, sizeof(LYNXFILE));	
	lpd->bUseBufferObject = lpsm->bUseBufferObject;	
	
	lpd->NumMeshs = lpsm->NumMeshs;
	lpd->lpMeshs = (LPLYNXMESH)LYNX_MALLOC(lpd->NumMeshs*sizeof(LYNXMESH));
	for (i=0; i<lpd->NumMeshs; i++)
	{
		lynxInitMesh(&lpd->lpMeshs[i]);
		lynxInstanceMesh(&lpd->lpMeshs[i], &lpsm->lpMeshs[i]);
		lpd->lpMeshs[i].lpModel = lpd;		
		if (lpd->lpMeshs[i].Obj3D.ParentData.ChildIndex == -1)
		{
			RootMesh = i;			
			lpd->lpMeshs[i].Obj3D.ParentData.lpObject = (LPVOID)&lpd->Mesh;		
		}
		else
		{
			lpd->lpMeshs[i].Obj3D.ParentData.lpObject = (LPVOID)&lpd->lpMeshs[lpd->lpMeshs[i].Obj3D.ParentData.ChildIndex];		
		}
		for (j=0; j<lpd->lpMeshs[i].Obj3D.NumChildren; j++)
		{
			lpd->lpMeshs[i].Obj3D.lpChildrenData[j].lpObject = (LPVOID)&lpd->lpMeshs[lpd->lpMeshs[i].Obj3D.lpChildrenData[j].ChildIndex];		
		}
		lpd->lpMeshs[i].NumPrimitive = lpsm->lpMeshs[i].NumPrimitive;
		lpd->lpMeshs[i].lpPrimitive = lpsm->lpMeshs[i].lpPrimitive;
	}	
	lynxInstanceMesh(&lpd->Mesh, &lpsm->Mesh);
	if (lpd->Mesh.Obj3D.lpChildrenData)
		lpd->Mesh.Obj3D.lpChildrenData[0].lpObject = (LPVOID)&lpd->lpMeshs[RootMesh];

	lpd->NumBones = lpsm->NumBones;
	lpd->lpBones = (LPLYNXBONE)LYNX_MALLOC(sizeof(LYNXBONE)*lpsm->NumBones);
	for (i=0; i<lpd->NumBones; i++)
	{
		lynxInitBone(&lpd->lpBones[i]);
		lynxInstanceBone(&lpd->lpBones[i], &lpsm->lpBones[i]);
		if (lpd->lpBones[i].Obj3D.ParentData.ChildIndex == -1)
		{
			RootBone = i;			
			lpd->lpBones[i].Obj3D.ParentData.lpObject = (LPVOID)&lpd->Mesh;		
		}
		else
		{
			lpd->lpBones[i].Obj3D.ParentData.lpObject = (LPVOID)&lpd->lpBones[lpd->lpBones[i].Obj3D.ParentData.ChildIndex];		
		}		
		for (j=0; j<lpd->lpBones[i].Obj3D.NumChildren; j++)
		{
			lpd->lpBones[i].Obj3D.lpChildrenData[j].lpObject = &lpd->lpBones[lpd->lpBones[i].Obj3D.lpChildrenData[j].ChildIndex];				
		}
	}
	lpd->lpWeightList[0] = lpsm->lpWeightList[0];
	//lynxMemCpy(&lpd->BaseTM, &lpsm->BaseTM, sizeof(LYNXMATRIX)); 
	//lynxMemCpy(&lpd->InverseBaseTM, &lpsm->InverseBaseTM, sizeof(LYNXMATRIX)); 

	if (lpd->NumMeshs)
		lynxTraverseModelMesh(lpd, &lpd->lpMeshs[RootMesh]);	
	if (lpd->NumBones)
		lynxTraverseModelBone(lpd, &lpd->lpBones[RootBone]);	
	
	lpd->Mesh.Obj3D.Play = lpsm->Mesh.Obj3D.Play;
	lpd->Mesh.Obj3D.Render = lpsm->Mesh.Obj3D.Render;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 重置LYNXMODEL的虛擬函式指標。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxResetModelFunctionPointers(LPLYNXMODEL lpsm)
{
	lpsm->Mesh.Obj3D.Play = (LPLYNXPLAYPROC)lynxPlayModel;
	lpsm->Mesh.Obj3D.Render = (LPLYNXRENDERPROC)lynxRenderModel;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 重置LYNXMODEL的轉換矩陣。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxResetModelTransform(LPLYNXMODEL lpsm)
{
	int							i;

	for (i=0; i<lpsm->NumMeshs; i++)
	{
		lynxResetObject3DTransform(&lpsm->lpMeshs[i].Obj3D);
	}
}	
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 播放一個LYNXMODEL的動畫。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxPlayModel(LPLYNXMODEL lpsm, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LPLYNXMESH				*lplpMeshs;
	long					Offset;
	LPLYNXBONE				*lplpBone;
	LYNXMATRIX              TM1, TM2;
	int						i;
	LYNXBOOL				b;	
	LYNXREAL				d1, d2;
	static LYNXREAL			DD = 0;

	LYNX_ASSERT(lpsm != NULL);	
		
	b = lynxPlayObject3D(&lpsm->Mesh.Obj3D, skip, mode);		
	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL :			
			//lynxGetMeshTransM(&lpsm->Mesh, lpsm->Mesh.Obj3D.Time);
			lynxUpdateObject3D(&lpsm->Mesh.Obj3D);
			
			lplpMeshs = lpsm->lplpTraverseMeshs;	
			for (i=lpsm->NumMeshs; i>0; i--)	
			{			
				//lynxfxTextureMotion(lpsm->lpUVVertexList[0], (*lplpMeshs)->TextureMotionMode, lpsm->lpOrgUVVertexList[0], (*lplpMeshs)->BaseUVVertex[0], (*lplpMeshs)->BaseUVVertex[0]+(*lplpMeshs)->NumUVVertices[0], (*lplpMeshs)->TextureMotionArg.lpArg, skip);

				lynxGetMeshTransM(*lplpMeshs, lpsm->Mesh.Obj3D.Time);
				lynxMatrixXMatrix(&TM1, &(*lplpMeshs)->Obj3D.LocalM,  &(*lplpMeshs)->Obj3D.TransM);
				lynxMatrixXMatrix(&TM2, &TM1, &((LPLYNXMESH)(*lplpMeshs)->Obj3D.ParentData.lpObject)->Obj3D.TransM);
				lynxMatrixXMatrix(&TM1, &(*lplpMeshs)->Obj3D.PivotM, &TM2);
				//Transform Matrix = PivotM * LocalM * TransM * ParentM * GlobalM; 
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.M, &TM1, &(*lplpMeshs)->Obj3D.GlobalM);	 		
				//Parent Matrix = LocalM * TransM * ParentM * GlobalM; 
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.TransM, &TM2, &(*lplpMeshs)->Obj3D.GlobalM);	 										
				lplpMeshs++;
			}
			break;

		case LYNX_VERTEX_ANIMATION_MODEL:			
			if (lpsm->Attribute&LYNX_MODEL_VERTEXANIMATION_INTERPOLATE)
			{
				/*
				if ((lpsm->Time-((int)lpsm->Time)))
				{	
					//Offset = (((int)lpsm->Time))*lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D)*2;
					//LYNX_SEEK_FILE(&lpsm->AniStr, Offset, SEEK_SET);
					//LYNX_READ_FILE(&lpsm->lpVertexList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);
					//LYNX_READ_FILE(&lpsm->lpVertexNormalList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);

					Offset = (((int)lpsm->Time)+1)*lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D)*2;
					LYNX_SEEK_FILE(&lpsm->AniStr, Offset, SEEK_SET);
					LYNX_READ_FILE(&lpsm->lpVertexList2[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);

					//d1 = lpsm->Time - (int)lpsm->Time;				
					d1 = lpsm->Time - LastTime;
					d2 = (((int)lpsm->Time)+1) - LastTime;
					d1 /= d2;
					for (i=0; i<lpsm->NumVertices[0]; i++)
					{					
						lpsm->lpVertexList[0][i].s.x += (lpsm->lpVertexList2[0][i].s.x - lpsm->lpVertexList[0][i].s.x)*d1;
						lpsm->lpVertexList[0][i].s.y += (lpsm->lpVertexList2[0][i].s.y - lpsm->lpVertexList[0][i].s.y)*d1;
						lpsm->lpVertexList[0][i].s.z += (lpsm->lpVertexList2[0][i].s.z - lpsm->lpVertexList[0][i].s.z)*d1;
					}				
				}
				else											
				*/
				{					
					Offset = ((int)lpsm->Mesh.Obj3D.Time)*lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D)*2;
					LYNX_SEEK_FILE(&lpsm->AniStr, Offset, SEEK_SET);

					//---- use world matrix to transform --------------------------------------------------
					//for (i=0; i<lpsm->NumVertices[0]; i++)
					//	lynxReadMemStr(&lpsm->lpTVertexList[0][i], sizeof(LYNXVECTOR3DFORIO), 1, &lpsm->AniStr);
					//---- use world matrix to transform --------------------------------------------------

					LYNX_READ_FILE(&lpsm->lpVertexList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);					
					if (lpsm->Attribute&LYNX_MODEL_UPDATE_NORMAL)
						LYNX_READ_FILE(&lpsm->lpVertexNormalList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);
				}
			}
			else
			{
				Offset = ((int)lpsm->Mesh.Obj3D.Time)*lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D)*2;
				LYNX_SEEK_FILE(&lpsm->AniStr, Offset, SEEK_SET);

				//---- use world matrix to transform --------------------------------------------------
				//for (i=0; i<lpsm->NumVertices[0]; i++)
				//	lynxReadMemStr(&lpsm->lpTVertexList[0][i], sizeof(LYNXVECTOR3DFORIO), 1, &lpsm->AniStr);
				//---- use world matrix to transform --------------------------------------------------

				LYNX_READ_FILE(&lpsm->lpVertexList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);					
				if (lpsm->Attribute&LYNX_MODEL_UPDATE_NORMAL)
					LYNX_READ_FILE(&lpsm->lpVertexNormalList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);
			}					
			lynxMatrixXMatrix(&TM1, &lpsm->Mesh.Obj3D.LocalM, &lpsm->Mesh.Obj3D.TransM);	 	    
			lynxMatrixXMatrix(&lpsm->Mesh.Obj3D.M, &TM1, &lpsm->Mesh.Obj3D.GlobalM);	 	    								

			lplpMeshs = lpsm->lplpTraverseMeshs;
			for (i=lpsm->NumMeshs; i>0; i--)	
			{
				lynxGetMeshTransM(*lplpMeshs, lpsm->Mesh.Obj3D.Time*lpsm->SkipFrames);
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.M, &(*lplpMeshs)->Obj3D.LocalM, &(*lplpMeshs)->Obj3D.TransM);	 	    
				//lynxMatrixXMatrix(&TM2, &(*lplpMeshs)->Obj3D.M, &((LPLYNXMESH)((*lplpMeshs)->ParentData.lpObject))->Obj3D.WorldM);	 	    			
				lynxMatrixXMatrix(&TM2, &(*lplpMeshs)->Obj3D.M, &lpsm->Mesh.Obj3D.M);	 	    			
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.M, &TM2, &(*lplpMeshs)->Obj3D.GlobalM);	 				
				//LYNX_MATRIX_GET_POS(((*lplpMeshs)->Obj3D.TCenter), ((*lplpMeshs)->Obj3D.M));
				lplpMeshs++;
			}						
			break;
		
		case LYNX_HIE_BONESKIN_MODEL:
			//lynxGetMeshTransM(&lpsm->Mesh, lpsm->Mesh.Obj3D.Time);
			lynxUpdateObject3D(&lpsm->Mesh.Obj3D);
			
			lplpBone = lpsm->lplpTraverseBones;	
			for (i=lpsm->NumBones; i>0; i--)	
			{						
				lynxGetBoneTransM(*lplpBone, lpsm->Mesh.Obj3D.Time);
				lynxMatrixXMatrix(&TM1, &(*lplpBone)->Obj3D.LocalM,  &(*lplpBone)->Obj3D.TransM);					 					
				if ((*lplpBone)->Obj3D.ParentData.lpObject)
					lynxMatrixXMatrix(&TM2, &TM1, &((LPLYNXBONE)(*lplpBone)->Obj3D.ParentData.lpObject)->Obj3D.TransM);
				else
				{
					lynxMatrixXMatrix(&TM2, &TM1, &lpsm->Mesh.Obj3D.TransM);
				}
				lynxMatrixXMatrix(&TM1, &(*lplpBone)->Obj3D.PivotM, &TM2);
				//Transform Matrix = PivotM * LocalM * TransM * ParentM * GlobalM; 
				lynxMatrixXMatrix(&(*lplpBone)->Obj3D.M, &TM1, &(*lplpBone)->Obj3D.GlobalM);	 
				//Parent Matrix = LocalM * TransM * ParentM * GlobalM; 
				lynxMatrixXMatrix(&(*lplpBone)->Obj3D.TransM, &TM2, &(*lplpBone)->Obj3D.GlobalM);	 																

				lplpBone++;
			}			
			break;
	}
	return b;
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
void		lynxPlayPhysicalModel(LPLYNXMODEL lpsm, LPLYNXMATRIX *lppm)
{
	LPLYNXMESH					*lplpMeshs;
	long						Offset;
	LPLYNXBONE					*lplpBone;
	LYNXMATRIX                  TM1, TM2;
	int							i;
	LYNXBOOL					b;	
	LYNXREAL					d1, d2;
	static LYNXREAL				DD = 0;


	LYNX_ASSERT(lpsm != NULL);
	
	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL :			
			//lynxGetMeshTransM(&lpsm->Mesh, lpsm->Time);
			lynxMatrixXMatrix(&lpsm->Mesh.Obj3D.TransM, &lpsm->Mesh.Obj3D.LocalM, &lpsm->Mesh.Obj3D.GlobalM);	 	    			
			lynxMemCpy(&lpsm->Mesh.Obj3D.M, &lpsm->Mesh.Obj3D.TransM, sizeof(LYNXMATRIX));  		
			
			lplpMeshs = lpsm->lplpTraverseMeshs;	
			for (i=lpsm->NumMeshs; i>0; i--)	
			{			
				//lynxfxTextureMotion(lpsm->lpUVVertexList[0], (*lplpMeshs)->TextureMotionMode, lpsm->lpOrgUVVertexList[0], (*lplpMeshs)->BaseUVVertex[0], (*lplpMeshs)->BaseUVVertex[0]+(*lplpMeshs)->NumUVVertices[0], (*lplpMeshs)->TextureMotionArg.lpArg, skip);
				lynxGetMeshTransM(*lplpMeshs, lpsm->Mesh.Obj3D.Time);
				lynxMatrixXMatrix(&TM1, &(*lplpMeshs)->Obj3D.LocalM,  &(*lplpMeshs)->Obj3D.TransM);
				lynxMatrixXMatrix(&TM2, &TM1, &((LPLYNXMESH)(*lplpMeshs)->Obj3D.ParentData.lpObject)->Obj3D.TransM);
				lynxMatrixXMatrix(&TM1, &(*lplpMeshs)->Obj3D.PivotM, &TM2);
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.M, &TM1, &(*lplpMeshs)->Obj3D.GlobalM);	 		
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.TransM, &TM2, &(*lplpMeshs)->Obj3D.GlobalM);	 									
				lplpMeshs++;
			}
			break;		
		
		case LYNX_HIE_BONESKIN_MODEL:
			//lynxGetMeshTransM(&lpsm->Mesh, lpsm->Time);
			lynxMatrixXMatrix(&lpsm->Mesh.Obj3D.TransM, &lpsm->Mesh.Obj3D.LocalM, &lpsm->Mesh.Obj3D.GlobalM);	 	    						
			lynxMemCpy(&lpsm->Mesh.Obj3D.M, &lpsm->Mesh.Obj3D.TransM, sizeof(LYNXMATRIX));  					
			
			lplpBone = lpsm->lplpTraverseBones;	
			for (i=lpsm->NumBones; i>0; i--)	
			{	
				if (lppm[(*lplpBone)->ID])
				{
					LYNX_MATRIX_COPY((*lplpBone)->Obj3D.M, (*lppm[(*lplpBone)->ID]));
					LYNX_MATRIX_COPY((*lplpBone)->Obj3D.TransM, (*lppm[(*lplpBone)->ID]));
				}
				else
				{
					lynxGetBoneTransM(*lplpBone, lpsm->Mesh.Obj3D.Time);
					lynxMatrixXMatrix(&TM1, &(*lplpBone)->Obj3D.LocalM,  &(*lplpBone)->Obj3D.TransM);					 
					
					if ((*lplpBone)->Obj3D.ParentData.lpObject)
						lynxMatrixXMatrix(&TM2, &TM1, &((LPLYNXBONE)(*lplpBone)->Obj3D.ParentData.lpObject)->Obj3D.TransM);
					else
					{
						lynxMatrixXMatrix(&TM2, &TM1, &lpsm->Mesh.Obj3D.TransM);
					}

					lynxMatrixXMatrix(&TM1, &(*lplpBone)->Obj3D.PivotM, &TM2);
					lynxMatrixXMatrix(&(*lplpBone)->Obj3D.M, &TM1, &(*lplpBone)->Obj3D.GlobalM);	 		
					lynxMatrixXMatrix(&(*lplpBone)->Obj3D.TransM, &TM2, &(*lplpBone)->Obj3D.GlobalM);	 												
				}								
				lplpBone++;
			}			
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
LYNXBOOL		lynxPlayNoTransformModel(LPLYNXMODEL lpsm, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LPLYNXMESH					*lplpMeshs;
	long						Offset;
	LPLYNXBONE					*lplpBone;
	LYNXMATRIX                  TM1, TM2;
	int							i;
	LYNXBOOL					b;
	LYNXREAL					LastTime;
	float						d1, d2;
	static LYNXREAL				DD = 0;

	
	b = lynxPlayObject3D(&lpsm->Mesh.Obj3D, skip, mode);
	
	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL :			
			LYNX_IDENTITY_MATRIX(lpsm->Mesh.Obj3D.M);				
			LYNX_IDENTITY_MATRIX(lpsm->Mesh.Obj3D.TransM);				

			lplpMeshs = lpsm->lplpTraverseMeshs;	
			for (i=lpsm->NumMeshs; i>0; i--)	
			{			
				lynxGetMeshTransM(*lplpMeshs, lpsm->Mesh.Obj3D.Time);

				lynxMatrixXMatrix(&TM1, &(*lplpMeshs)->Obj3D.LocalM,  &(*lplpMeshs)->Obj3D.TransM);
				lynxMatrixXMatrix(&TM2, &TM1, &((LPLYNXMESH)(*lplpMeshs)->Obj3D.ParentData.lpObject)->Obj3D.TransM);
				lynxMatrixXMatrix(&TM1, &(*lplpMeshs)->Obj3D.PivotM, &TM2);
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.M, &TM1, &(*lplpMeshs)->Obj3D.GlobalM);	 		
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.TransM, &TM2, &(*lplpMeshs)->Obj3D.GlobalM);	 							

				lplpMeshs++;
			}
			break;

		case LYNX_VERTEX_ANIMATION_MODEL:			
			if (lpsm->Attribute&LYNX_MODEL_VERTEXANIMATION_INTERPOLATE)
			{
				/*
				if ((lpsm->Time-((int)lpsm->Time)))
				{	
					//Offset = (((int)lpsm->Time))*lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D)*2;
					//LYNX_SEEK_FILE(&lpsm->AniStr, Offset, SEEK_SET);
					//LYNX_READ_FILE(&lpsm->lpVertexList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);
					//LYNX_READ_FILE(&lpsm->lpVertexNormalList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);

					Offset = (((int)lpsm->Time)+1)*lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D)*2;
					LYNX_SEEK_FILE(&lpsm->AniStr, Offset, SEEK_SET);
					LYNX_READ_FILE(&lpsm->lpVertexList2[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);

					//d1 = lpsm->Time - (int)lpsm->Time;				
					d1 = lpsm->Time - LastTime;
					d2 = (((int)lpsm->Time)+1) - LastTime;
					d1 /= d2;
					for (i=0; i<lpsm->NumVertices[0]; i++)
					{					
						lpsm->lpVertexList[0][i].s.x += (lpsm->lpVertexList2[0][i].s.x - lpsm->lpVertexList[0][i].s.x)*d1;
						lpsm->lpVertexList[0][i].s.y += (lpsm->lpVertexList2[0][i].s.y - lpsm->lpVertexList[0][i].s.y)*d1;
						lpsm->lpVertexList[0][i].s.z += (lpsm->lpVertexList2[0][i].s.z - lpsm->lpVertexList[0][i].s.z)*d1;
					}				
				}
				else										
				*/
				{
					Offset = ((int)lpsm->Mesh.Obj3D.Time)*lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D)*2;
					LYNX_SEEK_FILE(&lpsm->AniStr, Offset, SEEK_SET);
					LYNX_READ_FILE(&lpsm->lpVertexList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);
					LYNX_READ_FILE(&lpsm->lpVertexNormalList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);					
				}			
			}
			else
			{
				Offset = ((int)lpsm->Mesh.Obj3D.Time)*lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D)*2;
				LYNX_SEEK_FILE(&lpsm->AniStr, Offset, SEEK_SET);
				LYNX_READ_FILE(&lpsm->lpVertexList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);
				LYNX_READ_FILE(&lpsm->lpVertexNormalList[0][0], sizeof(LYNXVECTOR3DFORIO)*lpsm->NumVertices[0], 1, &lpsm->AniStr);					
			}							
			
			lplpMeshs = lpsm->lplpTraverseMeshs;
			for (i=lpsm->NumMeshs; i>0; i--)	
			{			
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.M, &(*lplpMeshs)->Obj3D.LocalM, &(*lplpMeshs)->Obj3D.TransM);	 	    
				//lynxMatrixXMatrix(&TM2, &(*lplpMeshs)->Obj3D.M, &((LPLYNXMESH)((*lplpMeshs)->ParentData.lpObject))->Obj3D.WorldM);	 	    			
				lynxMatrixXMatrix(&TM2, &(*lplpMeshs)->Obj3D.M, &lpsm->Mesh.Obj3D.M);	 	    			
				lynxMatrixXMatrix(&(*lplpMeshs)->Obj3D.M, &TM2, &(*lplpMeshs)->Obj3D.GlobalM);	 							
				//LYNX_MATRIX_GET_POS(((*lplpMeshs)->Obj3D.TCenter), ((*lplpMeshs)->Obj3D.M));

				lplpMeshs++;
			}						
			break;
		
		case LYNX_HIE_BONESKIN_MODEL:
			LYNX_IDENTITY_MATRIX(lpsm->Mesh.Obj3D.M);				
			LYNX_IDENTITY_MATRIX(lpsm->Mesh.Obj3D.TransM);												
			
			lplpBone = lpsm->lplpTraverseBones;	
			for (i=lpsm->NumBones; i>0; i--)	
			{			
				lynxGetBoneTransM(*lplpBone, lpsm->Mesh.Obj3D.Time);

				lynxMatrixXMatrix(&TM1, &(*lplpBone)->Obj3D.LocalM,  &(*lplpBone)->Obj3D.TransM);
				if ((*lplpBone)->Obj3D.ParentData.lpObject)
					lynxMatrixXMatrix(&TM2, &TM1, &((LPLYNXBONE)(*lplpBone)->Obj3D.ParentData.lpObject)->Obj3D.TransM);
				else
				{
					lynxMatrixXMatrix(&TM2, &TM1, &lpsm->Mesh.Obj3D.TransM);
				}
				lynxMatrixXMatrix(&TM1, &(*lplpBone)->Obj3D.PivotM, &TM2);
				lynxMatrixXMatrix(&(*lplpBone)->Obj3D.M, &TM1, &(*lplpBone)->Obj3D.GlobalM);	 		
				lynxMatrixXMatrix(&(*lplpBone)->Obj3D.TransM, &TM2, &(*lplpBone)->Obj3D.GlobalM);	 												

				lplpBone++;
			}			
			break;
	}
	return b;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 更新LYNXMODEL端點資料，並Render。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxUpdateModel(LPLYNXMODEL lpsm)
{
	int					o, p;		
	LPLYNXMESH			lpMesh;	

	if (lpsm->Mesh.Obj3D.Visibility > LYNX_REAL(0) && lpsm->Mesh.bInFrustum)
	{
		lynxUpdateModelVertices(lpsm);			

		lynxSetVertexArray(&lpsm->VertexArray, 0, 0);
		lynxSetIndexArray(&lpsm->IndexArray, 0);
		for (o=0; o<lpsm->NumMeshs; o++)
		{
			lpMesh = &lpsm->lpMeshs[o];		
			
			if (lpMesh->Obj3D.Visibility)
			{	
				for (p=0; p<lpsm->lpMaterials[lpMesh->MaterialIndex].NumPass; p++)
				{
					lynxSetMaterialPass(&lpsm->lpMaterials[lpMesh->MaterialIndex].lpMaterialPass[p]);			
					lpMesh->Obj3D.Render(lpMesh);					
				}			
			}
		}
	}		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 更新LYNXMODEL指定端點資料。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxUpdateModelVertex(LPLYNXVECTOR3D lppos, LPLYNXMODEL lpsm, int vi)
{
	LYNXMATRIX						M1, M2;
	LYNXREAL						Inf, Influence, MaxInf, Base, TU, TV, Dot;	
	LYNXVECTOR3D					P, TP, Vec, LookDir, LitDir;
	BYTE							C, Alpha;
	int								v1, v2, v3;
	int								i, j, k;
	LPLYNXMESH						*lplpMeshs;
	LPLYNXRENDERVERTEX				lpP;
	WORD							*lpT;
	DWORD							NumV, NumT;	
	LYNXRENDERVERTEX				Tri[3];
	LYNXREAL						Weight;

	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL:		
		case LYNX_VERTEX_ANIMATION_MODEL:			
			for (i=0; i<lpsm->NumMeshs; i++)	
			{				
				if (vi >= lpsm->lpMeshs[i].BaseVertex[lpsm->CurrentLOD] && vi < lpsm->lpMeshs[i].BaseVertex[lpsm->CurrentLOD]+lpsm->lpMeshs[i].NumVertices[lpsm->CurrentLOD])
				{
					if (lpsm->bUseBufferObject)
						lynxPointXMatrix(lppos, (LPLYNXVECTOR3D)&lpsm->lpMV[0][vi].Pos.s.x, &lpsm->lpMeshs[i].Obj3D.M);				
					else						
						lynxPointXMatrix(lppos, &lpsm->lpVertexList[lpsm->CurrentLOD][vi], &lpsm->lpMeshs[i].Obj3D.M);				
				}
			}
			break;				

		case LYNX_HIE_BONESKIN_MODEL:			
			if (lpsm->lpWeightList[lpsm->CurrentLOD][vi].NumBones > 1)
			{
				lppos->s.x = lppos->s.y = lppos->s.z = LYNX_REAL(0);		

				for (j=0; j<lpsm->lpWeightList[lpsm->CurrentLOD][vi].NumBones; j++)
				{
					lynxPointXMatrix(&P,(LPLYNXVECTOR3D)&lpsm->lpWeightList[lpsm->CurrentLOD][vi].lpOffset[j], &lpsm->lpBones[lpsm->lpWeightList[lpsm->CurrentLOD][vi].lpBoneIndex[j]].Obj3D.M);				
					#ifdef __FIXED_POINT__						
						P.s.x = LYNX_REAL_MUL(P.s.x, lpsm->lpWeightList[lpsm->CurrentLOD][vi].lpWeight[j]);
						P.s.y = LYNX_REAL_MUL(P.s.y, lpsm->lpWeightList[lpsm->CurrentLOD][vi].lpWeight[j]);
						P.s.z = LYNX_REAL_MUL(P.s.z, lpsm->lpWeightList[lpsm->CurrentLOD][vi].lpWeight[j]);									
					#else
						Weight = lpsm->lpWeightList[lpsm->CurrentLOD][vi].lpWeight[j]*LYNX_WEIGHT_ISCALE;
						P.s.x = LYNX_REAL_MUL(P.s.x, Weight);
						P.s.y = LYNX_REAL_MUL(P.s.y, Weight);
						P.s.z = LYNX_REAL_MUL(P.s.z, Weight);								
					#endif					
					lppos->s.x = LYNX_REAL_ADD(lppos->s.x, P.s.x);
					lppos->s.y = LYNX_REAL_ADD(lppos->s.y, P.s.y);
					lppos->s.z = LYNX_REAL_ADD(lppos->s.z, P.s.z);
				}	
				//LYNX_VECTOR3D_SET_W((*lppos), 1.0f);											
			}
			else
			{
				lynxPointXMatrix(lppos, (LPLYNXVECTOR3D)&lpsm->lpWeightList[lpsm->CurrentLOD][vi].lpOffset[0], &lpsm->lpBones[lpsm->lpWeightList[lpsm->CurrentLOD][vi].lpBoneIndex[0]].Obj3D.M);									
			}																							
			break;
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 更新LYNXMODEL所有端點資料。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxUpdateModelAllVertex(LPLYNXVECTOR3D lptv, LPLYNXMODEL lpsm)
{
	LYNXMATRIX						M1, M2;
	LYNXREAL						Inf, Influence, MaxInf, Base, TU, TV, Dot;	
	LYNXVECTOR3D					P, TP, Vec, LookDir, LitDir;
	BYTE							C, Alpha;
	int								v1, v2, v3;
	int								i, j, k;
	LPLYNXMESH						*lplpMeshs;
	LPLYNXRENDERVERTEX				lpP;
	WORD							*lpT;
	DWORD							NumV, NumT;		
	LYNXRENDERVERTEX				Tri[3];
	LYNXREAL						Weight;

	if (!lpsm->bUseBufferObject)
		return;

	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL:		
		case LYNX_VERTEX_ANIMATION_MODEL:						
			for (i=0; i<lpsm->NumMeshs; i++)	
				lynxSequencePointXMatrix(&lptv[lpsm->lpMeshs[i].BaseVertex[lpsm->CurrentLOD]], &lpsm->lpVertexList[lpsm->CurrentLOD][lpsm->lpMeshs[i].BaseVertex[lpsm->CurrentLOD]], &lpsm->lpMeshs[i].Obj3D.M, lpsm->lpMeshs[i].NumVertices[lpsm->CurrentLOD]);						
			break;				

		case LYNX_HIE_BONESKIN_MODEL:			
			for (i=0; i<lpsm->NumVertices[0]; i++)
			{
				if (lpsm->lpWeightList[0][i].NumBones > 1)
				{
					P.s.x = P.s.y = P.s.z = LYNX_REAL(0);
					lptv[i].s.x = lptv[i].s.y = lptv[i].s.z = LYNX_REAL(0);

					for (j=0; j<lpsm->lpWeightList[0][i].NumBones; j++)
					{
						lynxPointXMatrix(&P, (LPLYNXVECTOR3D)&lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[j], &lpsm->lpBones[lpsm->lpWeightList[lpsm->CurrentLOD][i].lpBoneIndex[j]].Obj3D.M);				
						#ifdef __FIXED_POINT__							
							P.s.x = LYNX_REAL_MUL(P.s.x, lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]);
							P.s.y = LYNX_REAL_MUL(P.s.y, lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]);
							P.s.z = LYNX_REAL_MUL(P.s.z, lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]);
						#else
							Weight = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]*LYNX_WEIGHT_ISCALE;
							P.s.x = LYNX_REAL_MUL(P.s.x, Weight);
							P.s.y = LYNX_REAL_MUL(P.s.y, Weight);
							P.s.z = LYNX_REAL_MUL(P.s.z, Weight);								
						#endif						
						lptv[i].s.x = LYNX_REAL_ADD(lptv[i].s.x, P.s.x);
						lptv[i].s.y = LYNX_REAL_ADD(lptv[i].s.y, P.s.y);
						lptv[i].s.z = LYNX_REAL_ADD(lptv[i].s.z, P.s.z);						
					}
					//LYNX_VECTOR3D_SET_W((lptv[i]), 1.0f);												
				}
				else
				{
					lynxPointXMatrix(&lptv[i], (LPLYNXVECTOR3D)&lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[0], &lpsm->lpBones[lpsm->lpWeightList[lpsm->CurrentLOD][i].lpBoneIndex[0]].Obj3D.M);				
				}
			}																							
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 更新LYNXMODEL端點資料。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxUpdateModelVertices(LPLYNXMODEL lpsm)
{
	LYNXMATRIX						M1, M2;
	LYNXREAL						Inf, Influence, MaxInf, Base, TU, TV, Dot;	
	LYNXVECTOR3D					P, TP, Vec, LookDir, LitDir;
	BYTE							C, Alpha;
	int								v1, v2, v3;
	int								i, j, k;
	LPLYNXMESH						*lplpMeshs;
	LPLYNXRENDERVERTEX				lpP;
	WORD							*lpT;
	DWORD							NumV, NumT;
	int								PV1, PV2, PV3, TV1, TV2, TV3, CV1, CV2, CV3;	
	LYNXRENDERVERTEX				Tri[3];	
	LYNXREAL						Weight;
	LPLYNXRENDERVERTEX				lpTVertexList;

	if (lpsm->bUseBufferObject)
		return;

	switch (lpsm->Type)
	{
		case LYNX_HIE_MODEL:		
		case LYNX_VERTEX_ANIMATION_MODEL:		
			//(*lplpGeo)->CurrentLOD;											
			/*if (lpsm->Attribute&LYNX_MODEL_UPDATE_NORMAL)
			{
				for (i=0; i<lpsm->NumMeshs; i++)	
				{						
					lynxSequenceVectorXMatrix(&lpsm->lpTVertexNormalList[CurrentLOD][lpsm->lpMeshs[i].BaseVertex[CurrentLOD]], &lpsm->lpVertexNormalList[CurrentLOD][lpsm->lpMeshs[i].BaseVertex[CurrentLOD]], &lpsm->lpMeshs[i].Obj3D.M, lpsm->lpMeshs[i].NumVertices[CurrentLOD]);																				
				}
			}*/
			break;		

		case LYNX_HIE_BONESKIN_MODEL:	
			lynxLockVertexArray(&lpsm->VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);
			lpTVertexList = (LPLYNXRENDERVERTEX)lpsm->VertexArray.lpBuffer;

			for (i=0; i<lpsm->NumVertices[lpsm->CurrentLOD]; i++)
			{
				if (lpsm->lpWeightList[lpsm->CurrentLOD][i].NumBones > 1)
				{
					P.s.x = P.s.y = P.s.z = LYNX_REAL(0);
					lpTVertexList[i].x = lpTVertexList[i].y = lpTVertexList[i].z = LYNX_REAL(0);

					for (j=0; j<lpsm->lpWeightList[lpsm->CurrentLOD][i].NumBones; j++)
					{
						lynxPointXMatrix(&P, &lpsm->lpWeightList[lpsm->CurrentLOD][i].lpOffset[j], &lpsm->lpBones[lpsm->lpWeightList[lpsm->CurrentLOD][i].lpBoneIndex[j]].Obj3D.M);																						
						#ifdef __FIXED_POINT__							
							P.s.x = LYNX_REAL_MUL(P.s.x, lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]);
							P.s.y = LYNX_REAL_MUL(P.s.y, lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]);
							P.s.z = LYNX_REAL_MUL(P.s.z, lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]);							
						#else
							Weight = lpsm->lpWeightList[lpsm->CurrentLOD][i].lpWeight[j]*LYNX_WEIGHT_ISCALE;
							P.s.x = LYNX_REAL_MUL(P.s.x, Weight);
							P.s.y = LYNX_REAL_MUL(P.s.y, Weight);
							P.s.z = LYNX_REAL_MUL(P.s.z, Weight);							
						#endif						
						lpTVertexList[i].x = LYNX_REAL_ADD(lpTVertexList[i].x, P.s.x);
						lpTVertexList[i].y = LYNX_REAL_ADD(lpTVertexList[i].y, P.s.y);
						lpTVertexList[i].z = LYNX_REAL_ADD(lpTVertexList[i].z, P.s.z);						
					}
					if (lpsm->Attribute&LYNX_MODEL_UPDATE_NORMAL)
					{
						lynxVectorXMatrix((LPLYNXVECTOR3D)&lpTVertexList[i].nx, &lpsm->lpVertexNormalList[lpsm->CurrentLOD][i], &lpsm->lpBones[lpsm->lpWeightList[lpsm->CurrentLOD][i].lpBoneIndex[0]].Obj3D.M);
						lynxNormalise((LPLYNXVECTOR3D)&lpTVertexList[i].nx, 3);						
					}
				}
				else
				{
					lynxPointXMatrix((LPLYNXVECTOR3D)&lpTVertexList[i].x, &lpsm->lpWeightList[0][i].lpOffset[0], &lpsm->lpBones[lpsm->lpWeightList[0][i].lpBoneIndex[0]].Obj3D.M);									
					if (lpsm->Attribute&LYNX_MODEL_UPDATE_NORMAL)
					{
						lynxVectorXMatrix((LPLYNXVECTOR3D)&lpTVertexList[i].nx, &lpsm->lpVertexNormalList[0][i], &lpsm->lpBones[lpsm->lpWeightList[0][i].lpBoneIndex[0]].Obj3D.M);
						lynxNormalise((LPLYNXVECTOR3D)&lpTVertexList[i].nx, 3);
					}
				}					
			}
			lynxUnlockVertexArray(&lpsm->VertexArray);	
			break;
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief Render一個LYNXMODEL資料。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxRenderModel(LPLYNXMODEL lpsm)
{
	int					o, p;		
	LPLYNXMESH			lpMesh;	

	if (lpsm->Mesh.Obj3D.Visibility > 0.0f && lpsm->Mesh.bInFrustum)
	{		
		lynxSetVertexArray(&lpsm->VertexArray, 0, 0);
		lynxSetIndexArray(&lpsm->IndexArray, 0);
		for (o=0; o<lpsm->NumMeshs; o++)
		{
			lpMesh = &lpsm->lpMeshs[o];		
			
			if (lpMesh->Obj3D.Visibility)
			{	
				for (p=0; p<lpsm->lpMaterials[lpMesh->MaterialIndex].NumPass; p++)
				{
					lynxSetMaterialPass(&lpsm->lpMaterials[lpMesh->MaterialIndex].lpMaterialPass[p]);			
					lpMesh->Obj3D.Render(lpMesh);					
				}			
			}
		}
	}		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 設定LYNXMODEL動畫資料的撥放時間參數。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxSetModelTimes(LPLYNXMODEL lpsm, LYNXREAL st, LYNXREAL et, LYNXREAL t)
{
	lpsm->Mesh.Obj3D.StartTime = st;
	lpsm->Mesh.Obj3D.EndTime = et;
	lpsm->Mesh.Obj3D.Time = t;
	lpsm->Mesh.Obj3D.LastTime = t;
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
void		lynxSetModelTime(LPLYNXMODEL lpsm, LYNXREAL t)
{
	lpsm->Mesh.Obj3D.Time = t;
	lpsm->Mesh.Obj3D.LastTime = t;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 由名稱搜尋LYNXMODEL中的特定的Mesh 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LPLYNXOBJ3D		lynxSearchObj3DByName(LPLYNXMODEL lpm, char *name)
{
	int								i;

	switch (lpm->Type)
	{
		case LYNX_HIE_BONESKIN_MODEL:
			for (i=0; i<lpm->NumBones; i++)
			{
				if (!strcmp(name, lpm->lpBones[i].Obj3D.Name))
					return (&lpm->lpBones[i].Obj3D);
			}
			break;

		case LYNX_HIE_MODEL:
			for (i=0; i<lpm->NumMeshs; i++)
			{
				if (!strcmp(name, lpm->lpMeshs[i].Obj3D.Name))
					return (&lpm->lpMeshs[i].Obj3D);
			}
			break;
	}

	//Ouch!!!
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 由端點搜尋LYNXMODEL中的特定的Mesh。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LPLYNXOBJ3D		lynxSearchObj3DByVertexIndex(LPLYNXMODEL lpm, int vi)
{
	int								i;

	switch (lpm->Type)
	{
		case LYNX_HIE_BONESKIN_MODEL:
			return NULL;
			break;

		case LYNX_HIE_MODEL:
			for (i=0; i<lpm->NumMeshs; i++)
			{
				if (lpm->lpMeshs[i].BaseVertex[0] <= vi && (lpm->lpMeshs[i].BaseVertex[0]+lpm->lpMeshs[i].NumVertices[0]) >= vi)
					return (&lpm->lpMeshs[i].Obj3D);
			}
			break;
	}

	//Ouch!!!
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxModel 
 *  @brief 設定LYNXMODEL的參數。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxSetModelAttribute(LPLYNXMODEL lpsm, int attr)
{
	lpsm->Attribute = attr;	
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
void		lynxSetModelVisibility(LPLYNXMODEL lpsm, LYNXREAL v)
{
	int							i;

	lpsm->Mesh.Obj3D.Visibility = v;	
	for (i=0; i<lpsm->NumMeshs; i++)
	{
		lpsm->lpMeshs[i].Obj3D.Visibility = v;
	}
	if (!lpsm->bUseBufferObject)
	{
		if (lpsm->lpColorVertexList[lpsm->CurrentLOD])
		{
			for (i=0; i<lpsm->NumVertices[lpsm->CurrentLOD]; i++)
			{
				lpsm->lpColorVertexList[lpsm->CurrentLOD][i].s.Alpha = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(v, LYNX_REAL(255)));
			}
		}		
	}
}