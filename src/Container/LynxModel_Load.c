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
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInitModelSystem(void)
{
	char				Path[256];
	
	#ifdef __DIRECT3D__
		#ifdef _XBOX
			lynxLoadCompiledVertexShader(&gW_VS, lynxGetMeshVertexDecl(), "../shaders/w.s.xvu");	
			lynxLoadCompiledVertexShader(&gW_SB_VS, lynxGetMeshVertexDecl(), "./shaders/w_sb.s.xvu");		
			lynxLoadCompiledVertexShader(&gW_SB_SPHEREENV_VS, lynxGetMeshVertexDecl(), "./shaders/w_sb_SphereEnv.s.xvu");		
			lynxLoadCompiledVertexShader(&gW_SILHOUETTE_VS, lynxGetMeshVertexDecl(), "./shaders/w_Silhouette.s.xvu");			
			lynxLoadCompiledVertexShader(&gW_CARTOON_SB_VS, lynxGetMeshVertexDecl(), "./shaders/w_cartoon_sb.s.xvu");		

			lynxLoadCompiledVertexShader(&gWCS_VS, lynxGetBlendVertexDecl(), "../shaders/wcs.s.xvu");	
			lynxLoadCompiledVertexShader(&gWCS_SB_VS, lynxGetBlendVertexDecl(), "./shaders/wcs_sb.s.xvu");		
			lynxLoadCompiledVertexShader(&gWCS_SB_SPHEREENV_VS, lynxGetBlendVertexDecl(), "./shaders/wcs_sb_SphereEnv.s.xvu");		
			lynxLoadCompiledVertexShader(&gWCS_SILHOUETTE_VS, lynxGetBlendVertexDecl(), "./shaders/wcs_Silhouette.s.xvu");			
			lynxLoadCompiledVertexShader(&gWCS_CARTOON_SB_VS, lynxGetBlendVertexDecl(), "./shaders/wcs_cartoon_sb.s.xvu");		

			lynxLoadCompiledVertexShader(&gSB_VS, lynxGetMeshVertexDecl(), "./shaders/shadowbuffer.s.xvu");	
			lynxLoadCompiledPixelShader(&gCARTOON_SB_PS, "./shaders/cartoon_shadowbuffer.s.xpu");	
		#else						
		#endif
	#endif		
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
void	lynxRecursionTraverseModelBone(LPLYNXMODEL lpm,  LPLYNXBONE lpb)
{	
	int							i;		
	
	lpm->lplpTraverseBones[lpm->TraverseIndex++] = lpb;
	for (i=0; i<lpb->Obj3D.NumChildren; i++)
	{
		lynxRecursionTraverseModelBone(lpm, (LPLYNXBONE)lpb->Obj3D.lpChildrenData[i].lpObject);
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
void	lynxTraverseModelBone(LPLYNXMODEL lpm,  LPLYNXBONE lpb)
{	
	lpm->TraverseIndex = 0;
	if (lpm->lplpTraverseBones)
		LYNX_FREE(lpm->lplpTraverseBones);
	lpm->lplpTraverseBones = (LPLYNXBONE *)LYNX_MALLOC((lpm->NumBones) * sizeof(LPLYNXBONE));
	lynxRecursionTraverseModelBone(lpm, lpb);
	lpb->Obj3D.ParentData.lpObject = NULL;
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
void	lynxRecursionTraverseModelMesh(LPLYNXMODEL lpm,  LPLYNXMESH lpb)
{	
	int							i;		
	
	lpm->lplpTraverseMeshs[lpm->TraverseIndex++] = lpb;
	for (i=0; i<lpb->Obj3D.NumChildren; i++)
	{
		lynxRecursionTraverseModelMesh(lpm, (LPLYNXMESH)lpb->Obj3D.lpChildrenData[i].lpObject);
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
void	lynxTraverseModelMesh(LPLYNXMODEL lpm,  LPLYNXMESH lpb)
{	
	int							i;

	lpm->TraverseIndex = 0;
	if (lpm->lplpTraverseMeshs)
		LYNX_FREE(lpm->lplpTraverseMeshs);
	lpm->lplpTraverseMeshs = (LPLYNXMESH *)LYNX_MALLOC((lpm->NumMeshs) * sizeof(LPLYNXMESH));
	if (lpm->Type == LYNX_VERTEX_ANIMATION_MODEL)
	{
		for (i=0; i<lpm->NumMeshs; i++)
			lpm->lplpTraverseMeshs[i] = &lpm->lpMeshs[i];
	}
	else
	{
		lynxRecursionTraverseModelMesh(lpm, lpb);
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
LYNXBOOL	lynxLoadModelM(LPLYNXMODEL lpsm, const char *filename, LPLYNXFILE lpfs, long offset, LYNXBOOL bbo)
{		
	int					i, j, k, v, Keys;//, TexIndex;
	char				Mark[6] = "LYNXM";	
	char				Check[6], MName[128];
	DWORD				Version;
	LYNXFILE			MS, *lpFile;
	LYNXVECTOR3D		Temp, Normal, v1, v2;
	LYNXVECTOR3DFORIO	Pivot;
	DWORD				NumMaterials, MatIndex[256], CurrentLOD;	
	LPLYNXMESH			lpM;	
	WORD				ObjMatIndex, wNumChildren, ChildIndex, wIndex;	
	LYNXREAL			D, Deg, Len, fR, fG, fB, fA;
	DWORD				VOffset, TOffset;
	LYNXUVPOINT			UVCoor;	
	char				*Token;	
	int					bMatLoop;
	BYTE				PrimitiveType;
	WORD				T1, T2, T3, C1, C2, C3;
	int					bLoop, MipMapLevel;
	LYNXREAL			Value[4];
	WORD				*lpI;
	LYNXMATRIX			M;
	LYNXBOOL			bStripify;

	lynxDebugOutputPrintf("Start to load model\n");	
	lynxInitModel(lpsm);
	lpsm->bUseBufferObject = bbo;	
		
	lpsm->Type = LYNX_HIE_MODEL;
	lpsm->SourceType = LYNX_ST_ORIGIN;

	if (lpfs)
	{
		lpFile = lpfs;
	}
	else
	{
		if (lynxGetUseZipFile())
		{
			lpFile = &MS;
			if (!LYNX_OPEN_FILE(lpFile, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_ZIPFILE_STR))
				goto ErrorExit;			
		}
		else
		{
			lpFile = &MS;
			if (!LYNX_OPEN_FILE(lpFile, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_MEMORY_STR))//LYNX_FILE_STR))
				goto ErrorExit;			
		}	
	}
	
	lynxDebugOutputPrintf("Open model file OK\n");	
	LYNX_READ_FILE(Check, 5, 1, lpFile);
	Check[5] = 0x00;
	if (strcmp(Mark, Check))
		goto ErrorExit;		
		
	LYNX_READ_FILE(&Version, sizeof(DWORD), 1, lpFile);					

	LYNX_READ_FILE(&lpsm->NumVertices[0], sizeof(DWORD), 1, lpFile);	
	LYNX_SEEK_FILE(lpFile, sizeof(DWORD)*2, LYNX_SEEK_CUR);	
	LYNX_READ_FILE(&lpsm->NumTriangles[0], sizeof(DWORD), 1, lpFile);		
	
	if (lpsm->NumVertices[0])
	{
		lpsm->lpVertexList[0] = (LPLYNXVECTOR3D)LYNX_ALIGN_MALLOC(128, lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));
		if (lpsm->lpVertexList[0] == NULL)
			goto ErrorExit;			
		lpsm->lpVertexNormalList[0] = (LPLYNXVECTOR3D)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));
		if (lpsm->lpVertexNormalList[0] == NULL)
			goto ErrorExit;					
		lpsm->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXUVPOINT));		
		if (lpsm->lpUVVertexList[0] == NULL)
			goto ErrorExit;			
		lpsm->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC((lpsm->NumVertices[0])*sizeof(LYNXCOLORRGBA));
		if (lpsm->lpColorVertexList[0] == NULL)
			goto ErrorExit;						
	}
	
	if (lpsm->NumTriangles[0])
	{
		lpsm->lpTriangleList[0] = (LPLYNXSIMPLETRIANGLE)LYNX_MALLOC(lpsm->NumTriangles[0]*sizeof(LYNXSIMPLETRIANGLE));		
	}

	LYNX_READ_FILE(&lpsm->NumMaterials, sizeof(DWORD), 1, lpFile);	
	lpsm->lpMaterials = (LPLYNXMATERIAL)LYNX_MALLOC(lpsm->NumMaterials*sizeof(LYNXMATERIAL));
	if (lpsm->lpMaterials == NULL)
		goto ErrorExit;		
	
	lynxDebugOutputPrintf("Start to load model material\n");	
	for (i=0; i<lpsm->NumMaterials; i++)
	{
		lynxInitMaterial(&lpsm->lpMaterials[i]);
		lynxCreateMaterial(&lpsm->lpMaterials[i], 1);
		LYNX_READ_FILE(lpsm->lpMaterials[i].Name, sizeof(char), 64, lpFile);				
		LYNX_READ_FILE(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs, sizeof(int), 1, lpFile);		
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs);
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs);
		for (j=0; j<lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs; j++)
		{
			LYNX_READ_FILE(lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList[j], sizeof(char), 64, lpFile);		
		}		
		LYNX_READ_FILE(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].StartTime, sizeof(LYNXREAL), 1, lpFile);		
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].Time = lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].StartTime;
		LYNX_READ_FILE(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].EndTime, sizeof(LYNXREAL), 1, lpFile);		
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].EndTime = LYNX_REAL_SUB(lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].EndTime, LYNX_REAL(1));
		LYNX_READ_FILE(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].Interval, sizeof(LYNXREAL), 1, lpFile);		
		LYNX_READ_FILE(&bLoop, sizeof(int), 1, lpFile);				
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].PlayMode = bLoop;		
		LYNX_READ_FILE(&MipMapLevel, sizeof(int), 1, lpFile);		
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].MipMapLevel = MipMapLevel;

		if (lpsm->bUseBufferObject)
		{
			lynxDuplicateVertexLayout(&lpsm->lpMaterials[i].lpMaterialPass[0].VertexLayout, lynxGetBuiltinVertexLayout(LYNX_MESH_VERTEX));
			lynxLoadCompiledVertexShader(&lpsm->lpMaterials[i].lpMaterialPass[0].VS, "builtin/model/lynxM.vso", NULL, 0);	
			lynxLoadCompiledPixelShader(&lpsm->lpMaterials[i].lpMaterialPass[0].PS, "builtin/common/lynxDiffuseMap.pso", NULL, 0);					
		}
		else
		{
		}
	}					
	
	lynxDebugOutputPrintf("Start to load model mesh\n");		
	LYNX_READ_FILE(&lpsm->NumMeshs, sizeof(DWORD), 1, lpFile);	
	lpsm->lpMeshs = (LPLYNXMESH)LYNX_MALLOC((lpsm->NumMeshs) * sizeof(LYNXMESH)); 

	lpsm->Mesh.Obj3D.NumChildren = 1;
	lpsm->Mesh.Obj3D.lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(sizeof(LYNXLINKDATA) * (lpsm->Mesh.Obj3D.NumChildren));
	lpsm->Mesh.Obj3D.lpChildrenData[0].lpObject = (LPVOID)(&lpsm->lpMeshs[0]);	
		
	/* Dummy Geometry */	
	strcpy(MName, filename);
	Token = (char *)strtok(MName, "\\/");
	while(Token)
	{
		Token = (char *)strtok(NULL, "\\/");
		if (Token)
			strcpy(MName, Token);
	}
	strtok(MName, ".");		
	strcpy(lpsm->Mesh.Obj3D.Name, MName);

	for (i=0; i<lpsm->NumMeshs; i++)
	{
		lynxInitMesh(&lpsm->lpMeshs[i]);
		lpsm->lpMeshs[i].lpModel = lpsm;
	}
	lpsm->lpMeshs[0].Obj3D.ParentData.lpObject = (LPVOID)&lpsm->Mesh;

	CurrentLOD = 0;//lpsm->CurrentLOD;
	VOffset = 0;	
	TOffset = 0;

	/* Read Geometry */
	for (i=0; i<lpsm->NumMeshs; i++)
	{			
		lpM = &lpsm->lpMeshs[i];	
		LYNX_READ_FILE(&lpM->Obj3D.Name, 64, 1, lpFile);		
		LYNX_READ_FILE(&lpM->Obj3D.Visibility, sizeof(LYNXREAL), 1, lpFile);			
		LYNX_READ_FILE(&lpM->Obj3D.PivotM, sizeof(LYNXMATRIX), 1, lpFile);	
		LYNX_READ_FILE(&Pivot, sizeof(LYNXVECTOR3DFORIO), 1, lpFile);	
		LYNX_VECTOR3D_SET_W(lpM->Obj3D.Pivot, 1.0f);
		LYNX_READ_FILE(&NumMaterials, sizeof(DWORD), 1, lpFile);	
		for (j=0; j<NumMaterials; j++)
		{
			LYNX_READ_FILE(&MatIndex[j], sizeof(DWORD), 1, lpFile);	
			if (MatIndex[j] >= lpsm->NumMaterials)
			{
				MatIndex[j] = 0;
			}
		}		
		lpM->BaseVertex[CurrentLOD] = VOffset;		
		LYNX_READ_FILE(&lpM->NumVertices[CurrentLOD], sizeof(DWORD), 1, lpFile);			
		
		/* Read Vertex Data */
		for (j=0; j<lpM->NumVertices[CurrentLOD]; j++)
		{
			LYNX_READ_FILE(&lpsm->lpVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.x, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_READ_FILE(&lpsm->lpVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.y, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_READ_FILE(&lpsm->lpVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.z, sizeof(LYNXREAL), 1, lpFile);							
			LYNX_VECTOR3D_SET_W(lpsm->lpVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j], 1.0f);

			LYNX_READ_FILE(&lpsm->lpVertexNormalList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.x, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_READ_FILE(&lpsm->lpVertexNormalList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.y, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_READ_FILE(&lpsm->lpVertexNormalList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.z, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_VECTOR3D_SET_W(lpsm->lpVertexNormalList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j], 1.0f);
			lynxNormalise(&lpsm->lpVertexNormalList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j], 3);						

			LYNX_READ_FILE(&UVCoor.u, sizeof(LYNXREAL), 1, lpFile);
			LYNX_READ_FILE(&UVCoor.v, sizeof(LYNXREAL), 1, lpFile);					
			//LYNX_RV_SET_UV(lpsm->lpTVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j], 0, (UVCoor.u), (UVCoor.v));		
			
			LYNX_READ_FILE(&fR, sizeof(LYNXREAL), 1, lpFile);					
			LYNX_READ_FILE(&fG, sizeof(LYNXREAL), 1, lpFile);					
			LYNX_READ_FILE(&fB, sizeof(LYNXREAL), 1, lpFile);					
			LYNX_READ_FILE(&fA, sizeof(LYNXREAL), 1, lpFile);	
			if (fR == -1 || fG == -1 || fB == -1 || fA == -1)
			{
				//LYNX_RV_SET_RGBA(lpsm->lpTVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j], 255, 255, 255, 255);
			}
			else
			{
				//LYNX_RV_SET_RGBA(lpsm->lpTVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j], LYNX_ROUND(fR*255), LYNX_ROUND(fG*255), LYNX_ROUND(fB*255), LYNX_ROUND(fA*255));
			}
		}		
		
		LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);				
		for (j=0; j<lpM->NumVertices[CurrentLOD]; j++)

		{
			LYNX_READ_FILE(&lpsm->lpUVVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].u, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_READ_FILE(&lpsm->lpUVVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].v, sizeof(LYNXREAL), 1, lpFile);				
		}			
		
		LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);									
		for (j=0; j<lpM->NumVertices[CurrentLOD]; j++)
		{
			LYNX_READ_FILE(&fR, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_READ_FILE(&fG, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_READ_FILE(&fB, sizeof(LYNXREAL), 1, lpFile);				
			LYNX_READ_FILE(&fA, sizeof(LYNXREAL), 1, lpFile);				
			lpsm->lpColorVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.Red = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(fR, LYNX_REAL(255)));
			lpsm->lpColorVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.Green = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(fG, LYNX_REAL(255)));
			lpsm->lpColorVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.Blue = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(fB, LYNX_REAL(255)));
			lpsm->lpColorVertexList[CurrentLOD][lpM->BaseVertex[CurrentLOD]+j].s.Alpha = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(fA, LYNX_REAL(255)));
		}			

		/* Read face Data */
		lpM->BaseTriangle[CurrentLOD] = TOffset;
		LYNX_READ_FILE(&lpM->NumTriangles[CurrentLOD], sizeof(int), 1, lpFile);				
		for (j=0; j<(DWORD)lpM->NumTriangles[CurrentLOD]; j++)
		{
			LYNX_READ_FILE(&wIndex, sizeof(WORD), 1, lpFile);
			lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].v[0] = (DWORD)(lpM->BaseVertex[CurrentLOD] + wIndex);
			LYNX_READ_FILE(&wIndex, sizeof(WORD), 1, lpFile);
			lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].v[1] = (DWORD)(lpM->BaseVertex[CurrentLOD] + wIndex);
			LYNX_READ_FILE(&wIndex, sizeof(WORD), 1, lpFile);
			lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].v[2] = (DWORD)(lpM->BaseVertex[CurrentLOD] + wIndex);


			LYNX_READ_FILE(&ObjMatIndex, sizeof(WORD), 1, lpFile);	
			if (ObjMatIndex < NumMaterials)
				lpM->MaterialIndex = MatIndex[ObjMatIndex];			
			else
				lpM->MaterialIndex = 0;		

			LYNX_READ_FILE(&Normal, sizeof(LYNXVECTOR3DFORIO), 1, lpFile);			
			LYNX_VECTOR3D_SET_W(Normal, 1.0f);

			//LYNX_READ_FILE(&lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].Normal, sizeof(LYNXVECTOR3DFORIO), 1, lpFile);	
			
			//v1.s.x = lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v2].s.x - lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v1].s.x;
			//v1.s.y = lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v2].s.y - lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v1].s.y;
			//v1.s.z = lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v2].s.z - lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v1].s.z;
			//v2.s.x = lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v3].s.x - lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v1].s.x;
			//v2.s.y = lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v3].s.y - lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v1].s.y;
			//v2.s.z = lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v3].s.z - lpsm->lpVertexList[CurrentLOD][lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].s.v1].s.z;
			//lynxCrossProduct(&lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].Normal, &v1, &v2);
			//lynxNormalise(&lpsm->lpTriangleList[CurrentLOD][lpM->BaseTriangle[CurrentLOD]+j].Normal, 3);	
		}
		/* Read Texture Face Data */
		LYNX_SEEK_FILE(lpFile, sizeof(WORD)*lpM->NumTriangles[CurrentLOD]*3, LYNX_SEEK_CUR);
		/* Read Color Face Data */
		LYNX_SEEK_FILE(lpFile, sizeof(WORD)*lpM->NumTriangles[CurrentLOD]*3, LYNX_SEEK_CUR);			

		VOffset += lpM->NumVertices[CurrentLOD];		
		TOffset += lpM->NumTriangles[CurrentLOD];		

		/* Read Children Data */
		LYNX_READ_FILE(&wNumChildren, sizeof(WORD), 1, lpFile);
		lpM->Obj3D.NumChildren = (DWORD)wNumChildren;
		lpM->Obj3D.lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpM->Obj3D.NumChildren * sizeof(LYNXLINKDATA));
		for (j=0; j<(DWORD)lpM->Obj3D.NumChildren; j++)
		{			
			LYNX_READ_FILE(&ChildIndex, sizeof(WORD), 1, lpFile);		
			lpM->Obj3D.lpChildrenData[j].ChildIndex = ChildIndex;
			lpM->Obj3D.lpChildrenData[j].lpObject = (LPVOID)(&lpsm->lpMeshs[ChildIndex]);
			lpsm->lpMeshs[ChildIndex].Obj3D.ParentData.lpObject = (LPVOID)lpM;
			lpsm->lpMeshs[ChildIndex].Obj3D.ParentData.ChildIndex = i;
		}
	
		/* Read Rotation Animation Key */
		LYNX_READ_FILE(&Keys, sizeof(int), 1, lpFile);	
		lpM->Obj3D.RotateKey.lpKeys = NULL;
		//lpM->Obj3D.RotateKey.lpKeyValues = NULL;
		if (Keys != -1)
		{
			LYNX_READ_FILE(&lpM->Obj3D.RotateKey.NumKeys, sizeof(int), 1, lpFile);
			#ifdef __FIXED_POINT__
				lpM->Obj3D.RotateKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpM->Obj3D.RotateKey.NumKeys * sizeof(LYNXREALKEY));
			#else
				lpM->Obj3D.RotateKey.lpKeys = (LPLYNXSHORTKEY)LYNX_MALLOC(lpM->Obj3D.RotateKey.NumKeys * sizeof(LYNXSHORTKEY));
			#endif			
			for (k=0; k<lpM->Obj3D.RotateKey.NumKeys; k++)
			{
				LYNX_READ_FILE(&lpM->Obj3D.RotateKey.lpKeys[k].STime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpM->Obj3D.RotateKey.lpKeys[k].ETime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpM->Obj3D.RotateKey.lpKeys[k].NumValues, sizeof(int), 1, lpFile);

				LYNX_READ_FILE(Value, sizeof(LYNXREAL), 4, lpFile);
				#ifdef __FIXED_POINT__
					lpM->Obj3D.RotateKey.lpKeys[k].lpValues[0] = Value[0];
					lpM->Obj3D.RotateKey.lpKeys[k].lpValues[1] = Value[1];
					lpM->Obj3D.RotateKey.lpKeys[k].lpValues[2] = Value[2];
					lpM->Obj3D.RotateKey.lpKeys[k].lpValues[3] = Value[3];
				#else
					lpM->Obj3D.RotateKey.lpKeys[k].lpValues[0] = LYNX_ROUND(Value[0]*LYNX_ROTATE_KEY_SCALE);
					lpM->Obj3D.RotateKey.lpKeys[k].lpValues[1] = LYNX_ROUND(Value[1]*LYNX_ROTATE_KEY_SCALE);
					lpM->Obj3D.RotateKey.lpKeys[k].lpValues[2] = LYNX_ROUND(Value[2]*LYNX_ROTATE_KEY_SCALE);
					lpM->Obj3D.RotateKey.lpKeys[k].lpValues[3] = LYNX_ROUND(Value[3]*LYNX_ROTATE_KEY_SCALE);
				#endif								
			}
		}	
		/* Read Translate Animation Key */
		LYNX_READ_FILE(&Keys, sizeof(int), 1, lpFile);	
		lpM->Obj3D.TranslateKey.lpKeys = NULL;		
		if (Keys != -1)
		{
			LYNX_READ_FILE(&lpM->Obj3D.TranslateKey.NumKeys, sizeof(int), 1, lpFile);
			lpM->Obj3D.TranslateKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpM->Obj3D.TranslateKey.NumKeys * sizeof(LYNXREALKEY));			
			for (k=0; k<lpM->Obj3D.TranslateKey.NumKeys; k++)
			{
				LYNX_READ_FILE(&lpM->Obj3D.TranslateKey.lpKeys[k].STime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpM->Obj3D.TranslateKey.lpKeys[k].ETime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpM->Obj3D.TranslateKey.lpKeys[k].NumValues, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(lpM->Obj3D.TranslateKey.lpKeys[k].lpValues, sizeof(LYNXREAL), lpM->Obj3D.TranslateKey.lpKeys[k].NumValues, lpFile);				
			}
		}
		/* Read Scale Animation Key */
		LYNX_READ_FILE(&Keys, sizeof(int), 1, lpFile);	
		lpM->Obj3D.ScaleKey.lpKeys = NULL;
		//lpM->Obj3D.ScaleKey.lpKeyValues = NULL;
		if (Keys != -1)
		{
			LYNX_READ_FILE(&lpM->Obj3D.ScaleKey.NumKeys, sizeof(int), 1, lpFile);			
			lpM->Obj3D.ScaleKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpM->Obj3D.ScaleKey.NumKeys * sizeof(LYNXREALKEY));
			//lpM->Obj3D.ScaleKey.lpKeyValues = (float *)LYNX_MALLOC(lpM->Obj3D.ScaleKey.NumKeys * 3 * sizeof(float));
			for (k=0; k<lpM->Obj3D.ScaleKey.NumKeys; k++)
			{
				LYNX_READ_FILE(&lpM->Obj3D.ScaleKey.lpKeys[k].STime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpM->Obj3D.ScaleKey.lpKeys[k].ETime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpM->Obj3D.ScaleKey.lpKeys[k].NumValues, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(lpM->Obj3D.ScaleKey.lpKeys[k].lpValues, sizeof(LYNXREAL), lpM->Obj3D.ScaleKey.lpKeys[k].NumValues, lpFile);				
			}
		}	

		/* Read Obj3D.Visibility Animation Key */
		LYNX_READ_FILE(&Keys, sizeof(int), 1, lpFile);	
		lpM->Obj3D.VisKey.lpKeys = NULL;
		//lpM->Obj3D.VisKey.lpKeyValues = NULL;
		if (Keys != -1)
		{
			LYNX_READ_FILE(&lpM->Obj3D.VisKey.NumKeys, sizeof(int), 1, lpFile);
			#ifdef __FIXED_POINT__
				lpM->Obj3D.VisKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpM->Obj3D.VisKey.NumKeys * sizeof(LYNXREALKEY));
			#else
				lpM->Obj3D.VisKey.lpKeys = (LPLYNXSHORTKEY)LYNX_MALLOC(lpM->Obj3D.VisKey.NumKeys * sizeof(LYNXSHORTKEY));
			#endif			
			for (k=0; k<lpM->Obj3D.VisKey.NumKeys; k++)
			{
				LYNX_READ_FILE(&lpM->Obj3D.VisKey.lpKeys[k].STime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpM->Obj3D.VisKey.lpKeys[k].ETime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpM->Obj3D.VisKey.lpKeys[k].NumValues, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(Value, sizeof(LYNXREAL), 1, lpFile);
				#ifdef __FIXED_POINT__				
					lpM->Obj3D.VisKey.lpKeys[k].lpValues[0] = Value[0];
				#else
					lpM->Obj3D.VisKey.lpKeys[k].lpValues[0] = LYNX_ROUND(Value[0]*LYNX_ROTATE_KEY_SCALE);
				#endif				
			}
		}
		
		LYNX_READ_FILE(&bStripify, sizeof(LYNXBOOL), 1, lpFile);		
		if (bStripify)
		{
			LYNX_READ_FILE(&lpM->NumPrimitive, sizeof(unsigned short), 1, lpFile);
			lpM->lpPrimitive = (LPLYNXPRIMITIVE)LYNX_MALLOC(sizeof(LYNXPRIMITIVE)*lpM->NumPrimitive);
			for (j=0; j<lpM->NumPrimitive; j++)
			{				
				LYNX_READ_FILE(&PrimitiveType, sizeof(BYTE), 1, lpFile);								
				lpM->lpPrimitive[j].BaseIndex = 0;
				LYNX_READ_FILE(&lpM->lpPrimitive[j].NumIndex, sizeof(unsigned int), 1, lpFile);
				switch (PrimitiveType)
				{
					case 0:
						lpM->lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_LIST;
						lpM->lpPrimitive[j].NumPrimitives = lpM->lpPrimitive[j].NumIndex/3;
						break;
					case 1:
						lpM->lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_STRIP;
						lpM->lpPrimitive[j].NumPrimitives = lpM->lpPrimitive[j].NumIndex-2;
						break;
					case 2:
						lpM->lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_FAN;
						lpM->lpPrimitive[j].NumPrimitives = lpM->lpPrimitive[j].NumIndex-2;
						break;
				}
				lpM->lpPrimitive[j].lpIndex = (unsigned short *)LYNX_MALLOC(sizeof(unsigned short)*lpM->lpPrimitive[j].NumIndex);
				LYNX_READ_FILE(lpM->lpPrimitive[j].lpIndex, sizeof(unsigned short), lpM->lpPrimitive[j].NumIndex, lpFile);
				for (k=0; k<lpM->lpPrimitive[j].NumIndex; k++)
				{
					lpM->lpPrimitive[j].lpIndex[k] += lpM->BaseVertex[0];
				}			
			}
		}		
		else
		{
			lpsm->lpMeshs[i].NumPrimitive = 1;				
			lpsm->lpMeshs[i].lpPrimitive = (LPLYNXPRIMITIVE)LYNX_MALLOC(sizeof(LYNXPRIMITIVE)*lpsm->lpMeshs[i].NumPrimitive);
			for (j=0; j<lpsm->lpMeshs[i].NumPrimitive; j++)
			{
				lpsm->lpMeshs[i].lpPrimitive[j].lpIndex = NULL;					
				lpsm->lpMeshs[i].lpPrimitive[j].Type = LYNX_PRIMITIVE_TRIANGLE_LIST;
				lpsm->lpMeshs[i].lpPrimitive[j].BaseIndex = 0;
				lpsm->lpMeshs[i].lpPrimitive[j].NumIndex = lpsm->lpMeshs[i].NumTriangles[0]*3;
				lpsm->lpMeshs[i].lpPrimitive[j].NumPrimitives = lpsm->lpMeshs[i].lpPrimitive[j].NumIndex/3;
				lpsm->lpMeshs[i].lpPrimitive[j].lpIndex = (unsigned short *)LYNX_MALLOC(sizeof(unsigned short)*lpsm->lpMeshs[i].lpPrimitive[j].NumIndex);					
				for (k=0; k<lpsm->lpMeshs[i].NumTriangles[0]; k++)
				{
					lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k*3] = lpsm->lpTriangleList[0][lpsm->lpMeshs[i].BaseTriangle[0]+k].v[0];					
					lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k*3+1] = lpsm->lpTriangleList[0][lpsm->lpMeshs[i].BaseTriangle[0]+k].v[1];					
					lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k*3+2] = lpsm->lpTriangleList[0][lpsm->lpMeshs[i].BaseTriangle[0]+k].v[2];					
				}
			}							
		}
	}	
			
	if (!lpfs)
		LYNX_CLOSE_FILE(lpFile);			
	
	lynxTraverseModelMesh(lpsm, &lpsm->lpMeshs[0]);	

	lynxDebugOutputPrintf("%d material\n", lpsm->NumMaterials);		
	//put material loading here avoid seek file when loading model			
	for (i=0; i<lpsm->NumMaterials; i++)
	{
		for (j=0; j<lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs; j++)
		{
			lynxDebugOutputPrintf("load texture %s \n", lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList[j]);		
			lynxLoadTexture(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lpTexIndexList[j], NULL, 0, lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList[j], NULL, 0, LYNX_NORMAL_TEXTURE);										
		}
	}		

	lynxDebugOutputPrintf("Start to setup model\n");	
	lynxSetupModel(lpsm);	
	lynxDebugOutputPrintf("Setup model OK\n");	
	return TRUE;	

ErrorExit:
	if (!lpfs)
		LYNX_CLOSE_FILE(lpFile);		
	return FALSE;
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
LYNXBOOL				lynxLoadModelMS(LPLYNXMODEL lpsm, const char *filename, LPLYNXFILE lpfs, long offset, LYNXBOOL bbo)
{
	int					i, j, k, v, Keys, Type;
	char				Mark[] = "MS";	
	char				Check[4], AniName[128];	
	DWORD				MainVer, SubVer, BoneIndex, AniLen;
	LYNXFILE			MS, *lpFile;
	LYNXVECTOR3D		Temp, Normal, v1, v2;
	DWORD				NumMaterials, MatIndex[256], CurrentLOD;
	MYMATERIALLIST		Mat[256];	
	WORD				ObjMatIndex, wValue;
	int					HieIndex;	
	LYNXREAL			D, Deg, Len;
	LYNXVECTOR3D		Point, TempV;
	LYNXMATRIX			M;
	LYNXTEXTUREINDEX	Tex;
	char				*Token;
	LYNXREAL			fR, fG, fB, fA;	
	int					bMatLoop, bLoop, MipMapLevel;		
	BYTE				PrimitiveType;
	LYNXTRIANGLE		TempTri;
	LYNXREAL			Value[4];
	WORD				*lpI;
	LYNXBOOL			bStripify;
	
	lynxInitModel(lpsm);

	lpsm->bUseBufferObject = bbo;	
	lpsm->Type = LYNX_VERTEX_ANIMATION_MODEL;
	lpsm->SourceType = LYNX_ST_ORIGIN;

	if (lpfs)
	{
		lpFile = lpfs;
	}
	else
	{
		if (lynxGetUseZipFile())
		{
			lpFile = &MS;
			if (!LYNX_OPEN_FILE(lpFile, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_ZIPFILE_STR))
				goto ErrorExit;			
		}
		else
		{
			lpFile = &MS;
			if (!LYNX_OPEN_FILE(lpFile, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_FILE_STR))
				goto ErrorExit;			
		}	
	}

	LYNX_READ_FILE(&Check, 2, 1, lpFile);
	if (Mark[0] != Check[0] || Mark[1] != Check[1])
	{
		goto ErrorExit;		
	}
		
	LYNX_READ_FILE(&MainVer, sizeof(DWORD), 1, lpFile);		
	
	lpsm->Type = LYNX_VERTEX_ANIMATION_MODEL;		
	
	LYNX_READ_FILE(&lpsm->NumMaterials, sizeof(DWORD), 1, lpFile);	
	lpsm->lpMaterials = (LPLYNXMATERIAL)LYNX_MALLOC(lpsm->NumMaterials*sizeof(LYNXMATERIAL));
	if (lpsm->lpMaterials == NULL)
		goto ErrorExit;		

	for (i=0; i<lpsm->NumMaterials; i++)
	{
		lynxInitMaterial(&lpsm->lpMaterials[i]);
		lynxCreateMaterial(&lpsm->lpMaterials[i], 1);
		LYNX_READ_FILE(lpsm->lpMaterials[i].Name, sizeof(char), 64, lpFile);		
		LYNX_READ_FILE(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs, sizeof(int), 1, lpFile);		
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs);
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs);
		for (j=0; j<lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs; j++)
		{
			LYNX_READ_FILE(lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList[j], sizeof(char), 64, lpFile);		
		}		
		LYNX_READ_FILE(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].StartTime, sizeof(LYNXREAL), 1, lpFile);		
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].Time = lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].StartTime;
		LYNX_READ_FILE(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].EndTime, sizeof(LYNXREAL), 1, lpFile);		
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].EndTime = LYNX_REAL_SUB(lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].EndTime, LYNX_REAL(1));		
		LYNX_READ_FILE(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].Interval, sizeof(LYNXREAL), 1, lpFile);		
		LYNX_READ_FILE(&bLoop, sizeof(int), 1, lpFile);				
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].PlayMode = bLoop;		
		LYNX_READ_FILE(&MipMapLevel, sizeof(int), 1, lpFile);		
		lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].MipMapLevel = MipMapLevel;

		if (lpsm->bUseBufferObject)
		{
			lynxDuplicateVertexLayout(&lpsm->lpMaterials[i].lpMaterialPass[0].VertexLayout, lynxGetBuiltinVertexLayout(LYNX_RENDER_VERTEX));			
			lynxLoadCompiledVertexShader(&lpsm->lpMaterials[i].lpMaterialPass[0].VS, "builtin/model/lynxMS.vso", NULL, 0);	
			lynxLoadCompiledPixelShader(&lpsm->lpMaterials[i].lpMaterialPass[0].PS, "builtin/common/lynxDiffuseMap.pso", NULL, 0);					
		}
		else
		{
		}
	}			
	LYNX_READ_FILE(&lpsm->Mesh.Obj3D.Name, sizeof(char), 64, lpFile);	
	LYNX_READ_FILE(&lpsm->Mesh.Obj3D.TransM, sizeof(LYNXMATRIX), 1, lpFile);		
	
	LYNX_READ_FILE(&lpsm->NumVertices[0], sizeof(DWORD), 1, lpFile);
	lpsm->lpVertexList[0] = (LPLYNXVECTOR3D)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));
	if (lpsm->lpVertexList[0] == NULL)
		goto ErrorExit;		
	lpsm->lpVertexNormalList[0] = (LPLYNXVECTOR3D)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));
	if (lpsm->lpVertexNormalList[0] == NULL)
		goto ErrorExit;			
	
	for (i=0; i<lpsm->NumVertices[0]; i++)
	{
		LYNX_READ_FILE(&lpsm->lpVertexList[0][i].s.x, sizeof(LYNXVECTOR3DFORIO), 1, lpFile);
		LYNX_VECTOR3D_SET_W(lpsm->lpVertexList[0][i], 1.0f);
		LYNX_READ_FILE(&TempV.s.x, sizeof(LYNXVECTOR3DFORIO), 1, lpFile);
		LYNX_READ_FILE(&TempV.s.x, sizeof(LYNXUVPOINT), 1, lpFile);
		LYNX_SEEK_FILE(lpFile, sizeof(LYNXREAL)*4, LYNX_SEEK_CUR);		
	}
	LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);
	lpsm->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXUVPOINT));
	if (lpsm->lpUVVertexList[0] == NULL)
		goto ErrorExit;		
	for (i=0; i<lpsm->NumVertices[0]; i++)
	{
		LYNX_READ_FILE(&lpsm->lpUVVertexList[0][i], sizeof(LYNXUVPOINT), 1, lpFile);
	}		
	
	LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);
	lpsm->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXCOLORRGBA));
	if (lpsm->lpColorVertexList[0] == NULL)
		goto ErrorExit;		
	for (i=0; i<lpsm->NumVertices[0]; i++)
	{
		LYNX_READ_FILE(&fR, sizeof(LYNXREAL), 1, lpFile);				
		LYNX_READ_FILE(&fG, sizeof(LYNXREAL), 1, lpFile);				
		LYNX_READ_FILE(&fB, sizeof(LYNXREAL), 1, lpFile);				
		LYNX_READ_FILE(&fA, sizeof(LYNXREAL), 1, lpFile);				

		lpsm->lpColorVertexList[0][i].s.Red = LYNX_ROUND(fR*255.0f);
		lpsm->lpColorVertexList[0][i].s.Green = LYNX_ROUND(fG*255.0f);
		lpsm->lpColorVertexList[0][i].s.Blue = LYNX_ROUND(fB*255.0f);
		lpsm->lpColorVertexList[0][i].s.Alpha = LYNX_ROUND(fA*255.0f);
	}		

	LYNX_READ_FILE(&lpsm->NumTriangles[0], sizeof(DWORD), 1, lpFile);
	lpsm->lpTriangleList[0] = (LPLYNXSIMPLETRIANGLE)LYNX_MALLOC(lpsm->NumTriangles[0]*sizeof(LYNXSIMPLETRIANGLE));	
	if (lpsm->lpTriangleList[0] == NULL)
		goto ErrorExit;		
	for (i=0; i<lpsm->NumTriangles[0]; i++)
	{
		TempTri.v1 = 0;
		LYNX_READ_FILE(&TempTri.v1, sizeof(DWORD), 1, lpFile);	
		LYNX_READ_FILE(&TempTri.v2, sizeof(DWORD), 1, lpFile);	
		LYNX_READ_FILE(&TempTri.v3, sizeof(DWORD), 1, lpFile);	
		LYNX_READ_FILE(&ObjMatIndex, sizeof(WORD), 1, lpFile);	
		LYNX_READ_FILE(&TempTri.Plane.Normal, sizeof(LYNXVECTOR3DFORIO), 1, lpFile);	
		LYNX_VECTOR3D_SET_W(TempTri.Plane.Normal, 1.0f);
		
		lpsm->lpTriangleList[0][i].v[0] = TempTri.v1; 
		lpsm->lpTriangleList[0][i].v[1] = TempTri.v2; 
		lpsm->lpTriangleList[0][i].v[2] = TempTri.v3; 	 		
	}
	LYNX_SEEK_FILE(lpFile, sizeof(DWORD)*3*lpsm->NumTriangles[0], LYNX_SEEK_CUR);
	LYNX_SEEK_FILE(lpFile, sizeof(DWORD)*3*lpsm->NumTriangles[0], LYNX_SEEK_CUR);		

	/* Dummy Geometry */	
	strcpy(AniName, filename);
	Token = (char *)strtok(AniName, "\\/");
	while(Token)
	{
		Token = (char *)strtok(NULL, "\\/");
		if (Token)
			strcpy(AniName, Token);
	}
	strtok(AniName, ".");		
	strcpy(lpsm->Mesh.Obj3D.Name, AniName);

	//Read Mesh data
	LYNX_READ_FILE(&lpsm->NumMeshs, sizeof(DWORD), 1, lpFile);
	lpsm->lpMeshs = (LPLYNXMESH)LYNX_MALLOC(lpsm->NumMeshs*sizeof(LYNXMESH));
	if (lpsm->lpMeshs == NULL)
		goto ErrorExit;		
	lpsm->lpMeshs[0].Obj3D.ParentData.lpObject = (LPVOID)&lpsm->Mesh;
	lpsm->Mesh.Obj3D.NumChildren = 1;
	lpsm->Mesh.Obj3D.lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpsm->Mesh.Obj3D.NumChildren*sizeof(LYNXLINKDATA));
	lpsm->Mesh.Obj3D.lpChildrenData[0].lpObject = (LPVOID)&lpsm->lpMeshs[0];

	for (i=0; i<lpsm->NumMeshs; i++)
	{
		lpsm->lpMeshs[i].Obj3D.ParentData.lpObject = (LPVOID)&lpsm->Mesh;
		lynxInitMesh(&lpsm->lpMeshs[i]);	
		lpsm->lpMeshs[i].lpModel = lpsm;
		LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.Name, sizeof(char), 64, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.Visibility, sizeof(float), 1, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.TransM, sizeof(LYNXMATRIX), 1, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].BaseVertex, sizeof(int), 1, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].NumVertices, sizeof(DWORD), 1, lpFile);
		LYNX_SEEK_FILE(lpFile, sizeof(int), LYNX_SEEK_CUR);
		LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);
		LYNX_SEEK_FILE(lpFile, sizeof(int), LYNX_SEEK_CUR);
		LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].BaseTriangle, sizeof(int), 1, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].NumTriangles, sizeof(DWORD), 1, lpFile);
		
		j = lpsm->lpMeshs[i].BaseTriangle[0];
		lpsm->lpMeshs[i].MaterialIndex = 0;		

		LYNX_READ_FILE(&HieIndex, sizeof(short), 1, lpFile);
		if (i != 0 && HieIndex >=0)
		{
			lpsm->lpMeshs[i].Obj3D.ParentData.lpObject = (LPVOID)&lpsm->lpMeshs[HieIndex];
			lpsm->lpMeshs[i].Obj3D.ParentData.ChildIndex = HieIndex;
		}

		LYNX_READ_FILE(&wValue, sizeof(WORD), 1, lpFile);		
		lpsm->lpMeshs[i].Obj3D.NumChildren = wValue;
		if (lpsm->lpMeshs[i].Obj3D.NumChildren)
		{
			lpsm->lpMeshs[i].Obj3D.lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpsm->lpMeshs[i].Obj3D.NumChildren*sizeof(LYNXLINKDATA));
			for (j=0; j<lpsm->lpMeshs[i].Obj3D.NumChildren; j++)
			{
				LYNX_READ_FILE(&wValue, sizeof(WORD), 1, lpFile);
				lpsm->lpMeshs[i].Obj3D.lpChildrenData[j].lpObject = (LPVOID)&lpsm->lpMeshs[HieIndex];
				lpsm->lpMeshs[i].Obj3D.lpChildrenData[j].ChildIndex = wValue;
			}
		}	

		/* Read Obj3D.Visibility Animation Key */
		LYNX_READ_FILE(&Keys, sizeof(int), 1, lpFile);	
		lpsm->lpMeshs[i].Obj3D.VisKey.lpKeys = NULL;
		if (Keys != -1)
		{
			LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.VisKey.NumKeys, sizeof(int), 1, lpFile);
			#ifdef __FIXED_POINT__					
				lpsm->lpMeshs[i].Obj3D.VisKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpsm->lpMeshs[i].Obj3D.VisKey.NumKeys * sizeof(LYNXREALKEY));
			#else
				lpsm->lpMeshs[i].Obj3D.VisKey.lpKeys = (LPLYNXSHORTKEY)LYNX_MALLOC(lpsm->lpMeshs[i].Obj3D.VisKey.NumKeys * sizeof(LYNXSHORTKEY));
			#endif
			for (k=0; k<lpsm->lpMeshs[i].Obj3D.VisKey.NumKeys; k++)
			{
				LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.VisKey.lpKeys[k].STime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.VisKey.lpKeys[k].ETime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.VisKey.lpKeys[k].NumValues, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(Value, sizeof(float), 1, lpFile);
				#ifdef __FIXED_POINT__
					lpsm->lpMeshs[i].Obj3D.VisKey.lpKeys[k].lpValues[0] = Value[0];
				#else
					lpsm->lpMeshs[i].Obj3D.VisKey.lpKeys[k].lpValues[0] = LYNX_ROUND(Value[0]*LYNX_ROTATE_KEY_SCALE);
				#endif
			}
		}								
		
		LYNX_READ_FILE(&bStripify, sizeof(LYNXBOOL), 1, lpFile);		
		if (bStripify)
		{
			LYNX_READ_FILE(&lpsm->lpMeshs[i].NumPrimitive, sizeof(unsigned short), 1, lpFile);
			lpsm->lpMeshs[i].lpPrimitive = (LPLYNXPRIMITIVE)LYNX_MALLOC(sizeof(LYNXPRIMITIVE)*lpsm->lpMeshs[i].NumPrimitive);
			for (j=0; j<lpsm->lpMeshs[i].NumPrimitive; j++)
			{
				lpsm->lpMeshs[i].lpPrimitive[j].lpIndex = NULL;
				LYNX_READ_FILE(&PrimitiveType, sizeof(BYTE), 1, lpFile);								
				lpsm->lpMeshs[i].lpPrimitive[j].NumIndex = 0;
				LYNX_READ_FILE(&lpsm->lpMeshs[i].lpPrimitive[j].NumIndex, sizeof(unsigned int), 1, lpFile);
				switch (PrimitiveType)
				{
					case 0:
						lpsm->lpMeshs[i].lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_LIST;
						lpsm->lpMeshs[i].lpPrimitive[j].NumPrimitives = lpsm->lpMeshs[i].lpPrimitive[j].NumIndex/3;
						break;
					case 1:
						lpsm->lpMeshs[i].lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_STRIP;
						lpsm->lpMeshs[i].lpPrimitive[j].NumPrimitives = lpsm->lpMeshs[i].lpPrimitive[j].NumIndex-2;
						break;
					case 2:
						lpsm->lpMeshs[i].lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_FAN;
						lpsm->lpMeshs[i].lpPrimitive[j].NumPrimitives = lpsm->lpMeshs[i].lpPrimitive[j].NumIndex-2;
						break;
				}	
				lpsm->lpMeshs[i].lpPrimitive[j].lpIndex = (unsigned short *)LYNX_MALLOC(sizeof(unsigned short)*lpsm->lpMeshs[i].lpPrimitive[j].NumIndex);
				LYNX_READ_FILE(lpsm->lpMeshs[i].lpPrimitive[j].lpIndex, sizeof(unsigned short), lpsm->lpMeshs[i].lpPrimitive[j].NumIndex, lpFile);
			}
		}		
	}	

	LYNX_READ_FILE(&lpsm->NumLOD, sizeof(DWORD), 1, lpFile);	
	LYNX_READ_FILE(&lpsm->NumFrames, sizeof(DWORD), 1, lpFile);		
	LYNX_READ_FILE(&lpsm->SkipFrames, sizeof(int), 1, lpFile);				
	lpsm->Mesh.Obj3D.Time = 0;
	lpsm->Mesh.Obj3D.EndTime = (float)(lpsm->NumFrames-1);
	
	if (!lpfs)
		LYNX_CLOSE_FILE(lpFile);			

	lynxTraverseModelMesh(lpsm, &lpsm->lpMeshs[0]);

	//put material loading here avoid seek file when loading model
	for (i=0; i<lpsm->NumMaterials; i++)
	{
		for (j=0; j<lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs; j++)
			lynxLoadTexture(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lpTexIndexList[j], NULL, 0, lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList[j], NULL, 0, LYNX_NORMAL_TEXTURE);				
	}	
			
	if (lpfs)
	{
		LYNX_READ_FILE(AniName, sizeof(char), 64, lpFile);
		LYNX_READ_FILE(&AniLen, sizeof(DWORD), 1, lpFile);

		lpsm->AniStr.Type = LYNX_MEMORY_STR;
		if (!lynxCreateMemStr(&lpsm->AniStr.MS, AniLen))			
			goto ErrorExit;			
		LYNX_READ_FILE(lpsm->AniStr.MS.Buffer, 1, AniLen, lpFile);
	}
	else
	{
		strcpy(AniName, filename);
		for (i=(int)strlen(AniName)-1; i>=0; i--)
		{
			if (AniName[i] == '.')
				break;
		}
		AniName[i] = 0x00;
		strcat(AniName, ".mva");
		
		if (lynxGetUseZipFile())
		{
			lpFile = &lpsm->AniStr;
			if (!LYNX_OPEN_FILE(lpFile, AniName,  (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_ZIPFILE_STR))
				goto ErrorExit;						
		}
		else
		{
			lpFile = &lpsm->AniStr;
			if (!LYNX_OPEN_FILE(lpFile, AniName, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_MEMORY_STR))
				goto ErrorExit;			
		}		
	}
		
	//lynxReadFileToMemStr(&lpsm->AniStr, AniName);		
	
	/*
	glpShapeAniV[0] = (LPLYNXVECTOR3D)LYNX_MALLOC(sizeof(LYNXVECTOR3D)*lpsm->NumVertices[0]);
	if (!LYNX_OPEN_FILE(lpFile, filename))
		goto ErrorExit;		

	for (i=0; i<lpsm->NumFrames; i++)
	{
		glpShapeAniV[i] = (LPLYNXVECTOR3D)LYNX_MALLOC(sizeof(LYNXVECTOR3D)*lpsm->NumVertices[0]);
		for (j=0; j<lpsm->NumVertices[0]; j++)
		{
			LYNX_READ_FILE(&glpShapeAniV[i][j].s.x , sizeof(float), 1, lpFile);	
			LYNX_READ_FILE(&glpShapeAniV[i][j].s.y , sizeof(float), 1, lpFile);	
			LYNX_READ_FILE(&glpShapeAniV[i][j].s.z , sizeof(float), 1, lpFile);					
		}
	}
	*/	
	lynxSetupModel(lpsm);	
		
	return TRUE;

ErrorExit:
	if (!lpfs)
		LYNX_CLOSE_FILE(lpFile);		
	return FALSE;
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
void ilynxWeightValueFilter(LPLYNXWEIGHTDATA lpw)
{
	WORD					Total, Offset;
	int						i;

	Total = 0;

	for (i=0; i<lpw->NumBones; i++)
	{
		Total += lpw->lpWeight[i];
	}
	if (Total > LYNX_WEIGHT_SCALE)
	{
		Offset = Total - LYNX_WEIGHT_SCALE;
		Offset /= lpw->NumBones;
		for (i=0; i<lpw->NumBones; i++)
		{
			lpw->lpWeight[i] -= Offset;
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
LYNXBOOL	lynxLoadModelMBS(LPLYNXMODEL lpsm, const char *filename, LPLYNXFILE lpfs, long offset, LYNXBOOL bbo)
{
	int					i, j, k, v, Type;
	char				Mark[] = "LYNXMBS";	
	char				Check[8], Name[64];	
	DWORD				MainVer, NumBone;
	int					BoneIndex, MeshRoot;
	LYNXFILE			MS, *lpFile;	
	DWORD				CurrentLOD;
	char				*Token;	
	WORD				ObjMatIndex, wNumChildren, ChildIndex, wIndex;					
	LYNXREAL			fR, fG, fB, Value[4];
	int					RootIndex;
	char				MName[128];	
	BYTE				PrimitiveType;
	LYNXTRIANGLE		TempTri;	
	LYNXBOOL			bStripify;

	lynxInitModel(lpsm);
	CurrentLOD = 0;
	lpsm->bUseBufferObject = bbo;	
	lpsm->Type = LYNX_HIE_BONESKIN_MODEL;
	lpsm->SourceType = LYNX_ST_ORIGIN;

	if (lpfs)
	{
		lpFile = lpfs;
	}
	else
	{
		if (lynxGetUseZipFile())
		{
			lpFile = &MS;
			if (!LYNX_OPEN_FILE(lpFile, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_ZIPFILE_STR))
				goto ErrorExit;			
		}
		else
		{
			lpFile = &MS;
			if (!LYNX_OPEN_FILE(lpFile, filename, (LYNX_FOT_READ|LYNX_FOT_BINARY), LYNX_MEMORY_STR))//LYNX_FILE_STR))
				goto ErrorExit;			
		}	
	}

	LYNX_READ_FILE(&Check, 7, 1, lpFile);
	Check[7] = 0x00;
	if (strcmp(Mark, Check))	
		goto ErrorExit;					
	LYNX_BYTE_ORDER_READ_FILE(&MainVer, sizeof(DWORD), 1, lpFile);    
	
	MeshRoot = 0;	
	LYNX_BYTE_ORDER_READ_FILE(&lpsm->NumMaterials, sizeof(DWORD), 1, lpFile);	    
	if (lpsm->NumMaterials)
	{
		lpsm->lpMaterials = (LPLYNXMATERIAL)LYNX_MALLOC(lpsm->NumMaterials*sizeof(LYNXMATERIAL));
		if (lpsm->lpMaterials == NULL)
			goto ErrorExit;		
		for (i=0; i<lpsm->NumMaterials; i++)
		{
			lynxInitMaterial(&lpsm->lpMaterials[i]);
			lynxCreateMaterial(&lpsm->lpMaterials[i], 1);
			LYNX_READ_FILE(lpsm->lpMaterials[i].Name, sizeof(char), 64, lpFile);
			lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs = 1;
			lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lpTexIndexList = (LPLYNXTEXTUREINDEX)LYNX_MALLOC(sizeof(LYNXTEXTUREINDEX)*lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs);
			lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList = (char (*)[64])LYNX_MALLOC(sizeof(char)*64*lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs);
			for (j=0; j<lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].NumTexs; j++)
			{
				LYNX_READ_FILE(lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList[j], sizeof(char), 64, lpFile);		
			}					
			LYNX_BYTE_ORDER_READ_FILE(&fR, sizeof(LYNXREAL), 1, lpFile);
			LYNX_BYTE_ORDER_READ_FILE(&fG, sizeof(LYNXREAL), 1, lpFile);
			LYNX_BYTE_ORDER_READ_FILE(&fB, sizeof(LYNXREAL), 1, lpFile);
            
            lpsm->lpMaterials[i].lpMaterialPass[0].Diffuse.s.Red = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(fR, LYNX_REAL(255)));
			lpsm->lpMaterials[i].lpMaterialPass[0].Diffuse.s.Green = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(fG, LYNX_REAL(255)));
			lpsm->lpMaterials[i].lpMaterialPass[0].Diffuse.s.Blue = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(fB, LYNX_REAL(255)));
			lpsm->lpMaterials[i].lpMaterialPass[0].Diffuse.s.Alpha = 255;		

			if (lpsm->bUseBufferObject)
			{
				lynxDuplicateVertexLayout(&lpsm->lpMaterials[i].lpMaterialPass[0].VertexLayout, lynxGetBuiltinVertexLayout(LYNX_BLEND_VERTEX));
				lynxLoadCompiledVertexShader(&lpsm->lpMaterials[i].lpMaterialPass[0].VS, "builtin/model/lynxMBS.vso", NULL, 0);					
				lynxLoadCompiledPixelShader(&lpsm->lpMaterials[i].lpMaterialPass[0].PS, "builtin/common/lynxDiffuseMap.pso", NULL, 0);					
			}
			else
			{
				//lynxDuplicateVertexLayout(&lpsm->lpMaterials[i].lpMaterialPass[0].VertexLayout, &gBlendVertexLayout);
			}
		}
	}
	LYNX_READ_FILE(&lpsm->Mesh.Obj3D.TransM, sizeof(LYNXMATRIX), 1, lpFile);		

	LYNX_READ_FILE(&lpsm->NumMeshs, sizeof(DWORD), 1, lpFile);		
	lpsm->lpMeshs = (LPLYNXMESH)LYNX_MALLOC(lpsm->NumMeshs*sizeof(LYNXMESH));	
	for (i=0; i<lpsm->NumMeshs; i++)
	{
		lynxInitMesh(&lpsm->lpMeshs[i]);
		lpsm->lpMeshs[i].lpModel = lpsm;
		LYNX_READ_FILE(lpsm->lpMeshs[i].Obj3D.Name, sizeof(char), 64, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.Visibility, sizeof(LYNXREAL), 1, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].MaterialIndex, sizeof(int), 1, lpFile);			
		LYNX_READ_FILE(&lpsm->lpMeshs[i].BaseVertex[0], sizeof(int), 1, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].NumVertices[0], sizeof(DWORD), 1, lpFile);
		LYNX_SEEK_FILE(lpFile, sizeof(int), LYNX_SEEK_CUR);
		LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);
		LYNX_SEEK_FILE(lpFile, sizeof(int), LYNX_SEEK_CUR);
		LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].BaseTriangle[0], sizeof(int), 1, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].NumTriangles[0], sizeof(DWORD), 1, lpFile);
		LYNX_READ_FILE(&lpsm->lpMeshs[i].NumBones, sizeof(int), 1, lpFile);
		LYNX_ASSERT(lpsm->lpMeshs[i].NumBones <= 44);
		lpsm->lpMeshs[i].lpBoneShaderData = (LPLYNXBONESHADERDATA)LYNX_MALLOC(lpsm->lpMeshs[i].NumBones*sizeof(LYNXBONESHADERDATA));
		for (j=0; j<lpsm->lpMeshs[i].NumBones; j++)
		{
			LYNX_READ_FILE(&lpsm->lpMeshs[i].lpBoneShaderData[j].BoneIndex, sizeof(int), 1, lpFile);
		}
		LYNX_READ_FILE(&BoneIndex, sizeof(int), 1, lpFile);	
		lpsm->lpMeshs[i].Obj3D.ParentData.ChildIndex = BoneIndex;
		if (BoneIndex == -1)
		{
			MeshRoot = i;
			lpsm->lpMeshs[i].Obj3D.ParentData.lpObject = NULL;
		}
		else
		{
			lpsm->lpMeshs[i].Obj3D.ParentData.lpObject = (LPVOID)&lpsm->lpMeshs[BoneIndex];
		}
		
		LYNX_READ_FILE(&lpsm->lpMeshs[i].Obj3D.NumChildren, sizeof(DWORD), 1, lpFile);	
		if (lpsm->lpMeshs[i].Obj3D.NumChildren > 0)
		{				
			lpsm->lpMeshs[i].Obj3D.lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpsm->lpMeshs[i].Obj3D.NumChildren*sizeof(LYNXLINKDATA));
			if (lpsm->lpMeshs[i].Obj3D.lpChildrenData == NULL)
				goto ErrorExit;		
			for (j=0; j<lpsm->lpMeshs[i].Obj3D.NumChildren; j++)
			{
				LYNX_READ_FILE(&BoneIndex, sizeof(int), 1, lpFile);	
				lpsm->lpMeshs[i].Obj3D.lpChildrenData[j].ChildIndex = BoneIndex;
				lpsm->lpMeshs[i].Obj3D.lpChildrenData[j].lpObject = (LPVOID)&lpsm->lpMeshs[BoneIndex];
			}
		}

		LYNX_READ_FILE(&bStripify, sizeof(LYNXBOOL), 1, lpFile);		
		if (bStripify)
		{
			LYNX_READ_FILE(&lpsm->lpMeshs[i].NumPrimitive, sizeof(unsigned short), 1, lpFile);
			lpsm->lpMeshs[i].lpPrimitive = (LPLYNXPRIMITIVE)LYNX_MALLOC(sizeof(LYNXPRIMITIVE)*lpsm->lpMeshs[i].NumPrimitive);
			for (j=0; j<lpsm->lpMeshs[i].NumPrimitive; j++)
			{
				lpsm->lpMeshs[i].lpPrimitive[j].lpIndex = NULL;
				LYNX_READ_FILE(&PrimitiveType, sizeof(BYTE), 1, lpFile);
				lpsm->lpMeshs[i].lpPrimitive[j].BaseIndex = 0;
				LYNX_READ_FILE(&lpsm->lpMeshs[i].lpPrimitive[j].NumIndex, sizeof(unsigned int), 1, lpFile);
				switch (PrimitiveType)
				{
					case 0:
						lpsm->lpMeshs[i].lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_LIST;
						lpsm->lpMeshs[i].lpPrimitive[j].NumPrimitives = lpsm->lpMeshs[i].lpPrimitive[j].NumIndex/3;
						break;
					case 1:
						lpsm->lpMeshs[i].lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_STRIP;
						lpsm->lpMeshs[i].lpPrimitive[j].NumPrimitives = lpsm->lpMeshs[i].lpPrimitive[j].NumIndex-2;
						break;
					case 2:
						lpsm->lpMeshs[i].lpPrimitive[j].Type = (LYNXPRIMITIVETYPE)LYNX_PRIMITIVE_TRIANGLE_FAN;
						lpsm->lpMeshs[i].lpPrimitive[j].NumPrimitives = lpsm->lpMeshs[i].lpPrimitive[j].NumIndex-2;
						break;
				}	
				lpsm->lpMeshs[i].lpPrimitive[j].lpIndex = (unsigned short *)LYNX_MALLOC(sizeof(unsigned short)*lpsm->lpMeshs[i].lpPrimitive[j].NumIndex);
				LYNX_READ_FILE(lpsm->lpMeshs[i].lpPrimitive[j].lpIndex, sizeof(unsigned short), lpsm->lpMeshs[i].lpPrimitive[j].NumIndex, lpFile);					
				for (k=0; k<lpsm->lpMeshs[i].lpPrimitive[j].NumIndex; k++)
					lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k] += lpsm->lpMeshs[i].BaseVertex[0];					
			}
		}					
	}			

	LYNX_READ_FILE(&lpsm->NumVertices[0], sizeof(DWORD), 1, lpFile);
	lpsm->lpVertexList[0] = (LPLYNXVECTOR3D)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));
	if (lpsm->lpVertexList[0] == NULL)
		goto ErrorExit;				
	lpsm->lpVertexNormalList[0]= (LPLYNXVECTOR3D)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXVECTOR3D));
	if (lpsm->lpVertexNormalList[0] == NULL)
		goto ErrorExit;	
	lpsm->lpWeightList[0] = (LPLYNXWEIGHTDATA)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXWEIGHTDATA));
	if (lpsm->lpWeightList[0] == NULL)
		goto ErrorExit;				

	for (i=0; i<lpsm->NumVertices[0]; i++)
	{
		lpsm->lpWeightList[0][i].lpBoneIndex = NULL;
		lpsm->lpWeightList[0][i].lpWeight = NULL;
		lpsm->lpWeightList[0][i].lpOffset = NULL;			

		LYNX_READ_FILE(&lpsm->lpVertexNormalList[0][i].s.x, sizeof(LYNXVECTOR3DFORIO), 1, lpFile);				
		LYNX_VECTOR3D_SET_W(lpsm->lpVertexNormalList[0][i], 1.0f);
		lynxNormalise(&lpsm->lpVertexNormalList[0][i], 3);		
		
		LYNX_READ_FILE(&NumBone, sizeof(DWORD), 1, lpFile);
		LYNX_ASSERT(NumBone <= 4);
		lpsm->lpWeightList[0][i].NumBones = NumBone;
		lpsm->lpWeightList[0][i].lpBoneIndex = (BYTE *)LYNX_MALLOC(lpsm->lpWeightList[0][i].NumBones*sizeof(BYTE));
		#ifdef __FIXED_POINT__
			lpsm->lpWeightList[0][i].lpWeight = (LYNXREAL *)LYNX_MALLOC(lpsm->lpWeightList[0][i].NumBones*sizeof(LYNXREAL));
		#else
			lpsm->lpWeightList[0][i].lpWeight = (WORD *)LYNX_MALLOC(lpsm->lpWeightList[0][i].NumBones*sizeof(WORD));
			//lpsm->lpWeightList[0][i].lpWeight = (LYNXREAL *)LYNX_MALLOC(lpsm->lpWeightList[0][i].NumBones*sizeof(LYNXREAL));
		#endif
		lpsm->lpWeightList[0][i].lpOffset = (LPLYNXVECTOR3D)LYNX_ALIGN_MALLOC(128, lpsm->lpWeightList[0][i].NumBones*sizeof(LYNXVECTOR3D));
		lpsm->lpWeightList[0][i].lpOffsetNormal = (LPLYNXVECTOR3D)LYNX_ALIGN_MALLOC(128, lpsm->lpWeightList[0][i].NumBones*sizeof(LYNXVECTOR3D));
		if (!lpsm->lpWeightList[0][i].lpBoneIndex ||
			!lpsm->lpWeightList[0][i].lpWeight ||
			!lpsm->lpWeightList[0][i].lpOffset)
			goto ErrorExit;		

		for (j=0; j<lpsm->lpWeightList[0][i].NumBones; j++)
		{
			LYNX_READ_FILE(&BoneIndex, sizeof(int), 1, lpFile);
			lpsm->lpWeightList[0][i].lpBoneIndex[j] = BoneIndex;
			LYNX_READ_FILE(&Value[0], sizeof(LYNXREAL), 1, lpFile);
			#ifdef __FIXED_POINT__
				lpsm->lpWeightList[0][i].lpWeight[j] = Value[0];
			#else
				lpsm->lpWeightList[0][i].lpWeight[j] = LYNX_ROUND(Value[0]*LYNX_WEIGHT_SCALE);
				//lpsm->lpWeightList[0][i].lpWeight[j] = Value[0]*LYNX_WEIGHT_SCALE;
			#endif
			LYNX_READ_FILE(&lpsm->lpWeightList[0][i].lpOffset[j], sizeof(LYNXVECTOR3DFORIO), 1, lpFile);			
			LYNX_VECTOR3D_SET_W(lpsm->lpWeightList[0][i].lpOffset[j], 1.0f);
			if (MainVer >= 2)
			{
				LYNX_READ_FILE(&lpsm->lpWeightList[0][i].lpOffsetNormal[j], sizeof(LYNXVECTOR3DFORIO), 1, lpFile);			
				LYNX_VECTOR3D_SET_W(lpsm->lpWeightList[0][i].lpOffsetNormal[j], 1.0f);
			}
		}		
		ilynxWeightValueFilter(&lpsm->lpWeightList[0][i]);
	}		
	lpsm->lpUVVertexList[0] = (LPLYNXUVPOINT)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXUVPOINT));
	if (lpsm->lpUVVertexList[0] == NULL)
		goto ErrorExit;		

	for (i=0; i<lpsm->NumVertices[0]; i++)
	{
		LYNX_READ_FILE(&lpsm->lpUVVertexList[0][i], sizeof(LYNXUVPOINT), 1, lpFile);
	}	

	lpsm->lpColorVertexList[0] = (LPLYNXCOLORRGBA)LYNX_MALLOC(lpsm->NumVertices[0]*sizeof(LYNXCOLORRGBA));
	if (lpsm->lpColorVertexList[0] == NULL)
		goto ErrorExit;		
	for (i=0; i<lpsm->NumVertices[0]; i++)
	{
		LYNX_READ_FILE(&lpsm->lpColorVertexList[0][i], sizeof(LYNXCOLORRGBA), 1, lpFile);						
	}		

	LYNX_READ_FILE(&lpsm->NumTriangles[0], sizeof(DWORD), 1, lpFile);	
	lpsm->lpTriangleList[0] = (LPLYNXSIMPLETRIANGLE)LYNX_MALLOC((lpsm->NumTriangles[0])*sizeof(LYNXSIMPLETRIANGLE));		
	if (lpsm->lpTriangleList[0] == NULL)
		goto ErrorExit;		
	for (i=0; i<lpsm->NumTriangles[0]; i++)
	{
		LYNX_READ_FILE(&TempTri.v1, sizeof(DWORD), 1, lpFile);	
		LYNX_READ_FILE(&TempTri.v2, sizeof(DWORD), 1, lpFile);	
		LYNX_READ_FILE(&TempTri.v3, sizeof(DWORD), 1, lpFile);	
		LYNX_READ_FILE(&TempTri.Plane.Normal, sizeof(LYNXVECTOR3DFORIO), 1, lpFile);	
		LYNX_VECTOR3D_SET_W(TempTri.Plane.Normal, 1.0f);
		LYNX_READ_FILE(&TempTri.Plane.Distance, sizeof(LYNXREAL), 1, lpFile);	
		LYNX_SEEK_FILE(lpFile, sizeof(DWORD), LYNX_SEEK_CUR);
		LYNX_SEEK_FILE(lpFile, sizeof(LYNXTEXTUREINDEX)*LYNX_MAX_TEXTURE_CHANNEL, LYNX_SEEK_CUR);
		LYNX_SEEK_FILE(lpFile, 4, LYNX_SEEK_CUR);

		lpsm->lpTriangleList[0][i].v[0] = TempTri.v1; 
		lpsm->lpTriangleList[0][i].v[1] = TempTri.v2; 
		lpsm->lpTriangleList[0][i].v[2] = TempTri.v3; 
	}				
	
	/* Dummy Geometry */	
	strcpy(MName, filename);
	Token = (char *)strtok(MName, "\\/");
	while(Token)
	{
		Token = (char *)strtok(NULL, "\\/");
		if (Token)
			strcpy(MName, Token);
	}
	strtok(MName, ".");		
	strcpy(lpsm->Mesh.Obj3D.Name, MName);	

	LYNX_READ_FILE(&lpsm->NumBones, sizeof(DWORD), 1, lpFile);	
	if (lpsm->NumBones)
	{		
		lpsm->lpBones = (LPLYNXBONE)LYNX_MALLOC((lpsm->NumBones)*sizeof(LYNXBONE));
		if (lpsm->lpBones == NULL)
			goto ErrorExit;		

		for (i=0; i<lpsm->NumBones; i++)
		{
			lynxInitBone(&lpsm->lpBones[i]);
			lpsm->lpBones[i].ID = i;
			LYNX_READ_FILE(lpsm->lpBones[i].Obj3D.Name, 1, 64, lpFile);	
			LYNX_READ_FILE(&BoneIndex, sizeof(int), 1, lpFile);	
			lpsm->lpBones[i].Obj3D.ParentData.ChildIndex = BoneIndex;
			if (BoneIndex == -1)
			{
				lpsm->lpBones[i].Obj3D.ParentData.lpObject = NULL;
				RootIndex = i;
			}
			else
			{
				lpsm->lpBones[i].Obj3D.ParentData.lpObject = (LPVOID)&lpsm->lpBones[BoneIndex];
			}
			
			LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.NumChildren, sizeof(DWORD), 1, lpFile);	
			if (lpsm->lpBones[i].Obj3D.NumChildren > 0)
			{				
				lpsm->lpBones[i].Obj3D.lpChildrenData = (LPLYNXLINKDATA)LYNX_MALLOC(lpsm->lpBones[i].Obj3D.NumChildren*sizeof(LYNXLINKDATA));
				if (lpsm->lpBones[i].Obj3D.lpChildrenData == NULL)
					goto ErrorExit;		
				for (j=0; j<lpsm->lpBones[i].Obj3D.NumChildren; j++)
				{
					LYNX_READ_FILE(&BoneIndex, sizeof(int), 1, lpFile);	
					lpsm->lpBones[i].Obj3D.lpChildrenData[j].ChildIndex = BoneIndex;
					lpsm->lpBones[i].Obj3D.lpChildrenData[j].lpObject = (LPVOID)&lpsm->lpBones[BoneIndex];
				}
			}					

			/* Read Rotation Animation Key */			
			lpsm->lpBones[i].Obj3D.RotateKey.lpKeys = NULL;			
			LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.RotateKey.NumKeys, sizeof(int), 1, lpFile);
			#ifdef __FIXED_POINT__
				lpsm->lpBones[i].Obj3D.RotateKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpsm->lpBones[i].Obj3D.RotateKey.NumKeys * sizeof(LYNXREALKEY));
			#else
				lpsm->lpBones[i].Obj3D.RotateKey.lpKeys = (LPLYNXSHORTKEY)LYNX_MALLOC(lpsm->lpBones[i].Obj3D.RotateKey.NumKeys * sizeof(LYNXSHORTKEY));			
			#endif
			for (j=0; j<lpsm->lpBones[i].Obj3D.RotateKey.NumKeys; j++)
			{
				LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].STime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].ETime, sizeof(int), 1, lpFile);
				lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].NumValues = 4;
				LYNX_READ_FILE(Value, sizeof(LYNXREAL), 4, lpFile);
				#ifdef __FIXED_POINT__
					lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].lpValues[0] = Value[0];
					lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].lpValues[1] = Value[1];
					lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].lpValues[2] = Value[2];
					lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].lpValues[3] = Value[3];
				#else
					lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].lpValues[0] = LYNX_ROUND(Value[0]*LYNX_ROTATE_KEY_SCALE);
					lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].lpValues[1] = LYNX_ROUND(Value[1]*LYNX_ROTATE_KEY_SCALE);
					lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].lpValues[2] = LYNX_ROUND(Value[2]*LYNX_ROTATE_KEY_SCALE);
					lpsm->lpBones[i].Obj3D.RotateKey.lpKeys[j].lpValues[3] = LYNX_ROUND(Value[3]*LYNX_ROTATE_KEY_SCALE);
				#endif				
			}				

			/* Read Translate Animation Key */
			lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys = NULL;			
			LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.TranslateKey.NumKeys, sizeof(int), 1, lpFile);
			lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpsm->lpBones[i].Obj3D.TranslateKey.NumKeys * sizeof(LYNXREALKEY));
			//lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys = (LPLYNXSHORTKEY)LYNX_MALLOC(lpsm->lpBones[i].Obj3D.TranslateKey.NumKeys * sizeof(LYNXSHORTKEY));
			for (j=0; j<lpsm->lpBones[i].Obj3D.TranslateKey.NumKeys; j++)
			{
				LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys[j].STime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys[j].ETime, sizeof(int), 1, lpFile);
				lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys[j].NumValues = 3;
				/*
				LYNX_READ_FILE(Value, sizeof(float), 3, lpFile);
				lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys[j].lpValues[0] = LYNX_ROUND(Value[0]*LYNX_TRANSLATE_KEY_SCALE);
				lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys[j].lpValues[1] = LYNX_ROUND(Value[1]*LYNX_TRANSLATE_KEY_SCALE);
				lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys[j].lpValues[2] = LYNX_ROUND(Value[2]*LYNX_TRANSLATE_KEY_SCALE);
				*/
				LYNX_READ_FILE(lpsm->lpBones[i].Obj3D.TranslateKey.lpKeys[j].lpValues, sizeof(LYNXREAL), 3, lpFile);					
			}

			/* Read Scale Animation Key */
			lpsm->lpBones[i].Obj3D.ScaleKey.lpKeys = NULL;			
			LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.ScaleKey.NumKeys, sizeof(int), 1, lpFile);
			lpsm->lpBones[i].Obj3D.ScaleKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpsm->lpBones[i].Obj3D.ScaleKey.NumKeys * sizeof(LYNXREALKEY));			
			for (j=0; j<lpsm->lpBones[i].Obj3D.ScaleKey.NumKeys; j++)
			{
				LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.ScaleKey.lpKeys[j].STime, sizeof(int), 1, lpFile);
				LYNX_READ_FILE(&lpsm->lpBones[i].Obj3D.ScaleKey.lpKeys[j].ETime, sizeof(int), 1, lpFile);
				lpsm->lpBones[i].Obj3D.ScaleKey.lpKeys[j].NumValues = 3;
				LYNX_READ_FILE(lpsm->lpBones[i].Obj3D.ScaleKey.lpKeys[j].lpValues, sizeof(LYNXREAL), 3, lpFile);	
			}			
		}
	}			
	LYNX_IDENTITY_MATRIX(lpsm->Mesh.Obj3D.TransM);	

	if (!lpfs)
		LYNX_CLOSE_FILE(lpFile);			

	if (!bStripify)
	{
		for (i=0; i<lpsm->NumMeshs; i++)
		{
			lpsm->lpMeshs[i].NumPrimitive = 1;				
			lpsm->lpMeshs[i].lpPrimitive = (LPLYNXPRIMITIVE)LYNX_MALLOC(sizeof(LYNXPRIMITIVE)*lpsm->lpMeshs[i].NumPrimitive);
			for (j=0; j<lpsm->lpMeshs[i].NumPrimitive; j++)
			{
				lpsm->lpMeshs[i].lpPrimitive[j].lpIndex = NULL;					
				lpsm->lpMeshs[i].lpPrimitive[j].Type = LYNX_PRIMITIVE_TRIANGLE_LIST;
				lpsm->lpMeshs[i].lpPrimitive[j].BaseIndex = 0;
				lpsm->lpMeshs[i].lpPrimitive[j].NumIndex = lpsm->lpMeshs[i].NumTriangles[0]*3;
				lpsm->lpMeshs[i].lpPrimitive[j].NumPrimitives = lpsm->lpMeshs[i].lpPrimitive[j].NumIndex/3;
				lpsm->lpMeshs[i].lpPrimitive[j].lpIndex = (unsigned short *)LYNX_MALLOC(sizeof(unsigned short)*lpsm->lpMeshs[i].lpPrimitive[j].NumIndex);					
				for (k=0; k<lpsm->lpMeshs[i].NumTriangles[0]; k++)
				{
					lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k*3] = lpsm->lpTriangleList[0][lpsm->lpMeshs[i].BaseTriangle[0]+k].v[0];					
					lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k*3+1] = lpsm->lpTriangleList[0][lpsm->lpMeshs[i].BaseTriangle[0]+k].v[1];					
					lpsm->lpMeshs[i].lpPrimitive[j].lpIndex[k*3+2] = lpsm->lpTriangleList[0][lpsm->lpMeshs[i].BaseTriangle[0]+k].v[2];										
				}
			}			
		}
	}
	
	lynxTraverseModelMesh(lpsm, &lpsm->lpMeshs[MeshRoot]);
	lynxTraverseModelBone(lpsm, &lpsm->lpBones[RootIndex]);
	
	lpsm->Mesh.MaterialIndex = lpsm->lpMeshs[0].MaterialIndex;

	//put material loading here avoid seek file when loading model		
	for (i=0; i<lpsm->NumMaterials; i++)
	{
		lynxLoadTexture(&lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lpTexIndexList[0], NULL, 0, lpsm->lpMaterials[i].lpMaterialPass[0].TextureFrame[0].lplpTexList[0], NULL, 0, LYNX_NORMAL_TEXTURE);
	}			
	
	lynxSetupModel(lpsm);	

	//Yahoo!!!	
	return TRUE;	

ErrorExit:
	if (!lpfs)
		LYNX_CLOSE_FILE(lpFile);		
	return FALSE;
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
LYNXBOOL lynxLoadModel(LPLYNXMODEL lpsm, const char *filename, LPLYNXFILE lpfs, long offset, LYNXBOOL bbo)
{		
	int	 i;
	const char* lpExt;
	LYNXBOOL bRet = LYNX_FALSE;

	lpExt = NULL;
	//lynxReleaseModel(lpsm);
	for (i=(int)strlen(filename)-1; i>=0; i--)
	{
		if (filename[i] == '.')
		{
			lpExt = &filename[i+1];
			break;
		}
	}
	lynxDebugOutputPrintf("Laod model %s\n", filename);
	if (lpExt)
	{
		if ((!lynxStriCmp(lpExt, "ms") && strlen(lpExt) == 2) ||
		    (!lynxStriCmp(lpExt, "msf") && strlen(lpExt) == 3) )
		{
			bbo = FALSE;
			bRet = lynxLoadModelMS(lpsm, filename, lpfs, offset, bbo);
		}
		else if ((!lynxStriCmp(lpExt, "mbs") && strlen(lpExt) == 3) ||
			     (!lynxStriCmp(lpExt, "mbsf") && strlen(lpExt) == 4) )
		{
			bRet = lynxLoadModelMBS(lpsm, filename, lpfs, offset, bbo);		
		}
		else if ((!lynxStriCmp(lpExt, "m") && strlen(lpExt) == 1) ||
			     (!lynxStriCmp(lpExt, "mf") && strlen(lpExt) == 2) )
		{
			bRet = lynxLoadModelM(lpsm, filename, lpfs, offset, bbo);
		}
	}
	if (bRet)
	{
		lynxSetLogf("\tLynxGraphics", "Load model <%s>\n", filename);	
		lynxDebugOutputPrintf("Laod model %s OK\n", filename);	
	}
	return bRet;
}