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

#include <LynxMesh.h>
#include <LynxModel.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMesh 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInitMesh(LPLYNXMESH lpmd)
{
	int							i;

	lynxInitObject3D(&lpmd->Obj3D);	
	lpmd->lpModel = NULL;	
	lpmd->bInFrustum = TRUE;
	lpmd->DynamicAttr = 0;	
	for (i=0; i<LYNX_MAX_LOD; i++)
	{
		lpmd->NumVertices[i] = 0;	
		lpmd->NumTriangles[i] = 0;		
	}	
	lpmd->TextureMotionMode = LYNX_TM_STATIC;	
	lpmd->TextureMotionArg.NumArgs = 0;
	lpmd->TextureMotionArg.lpArg = NULL;
	lpmd->NumBones = 0;
	lpmd->lpBoneShaderData = NULL;
	lpmd->NumPrimitive = 0;
	lpmd->lpPrimitive = NULL;
	for (i=0; i<6; i++)
	{
		lpmd->MaterialEffectData.EnvMapIndex[i] = LYNX_NULL_TEXTURE;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMesh 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxReleaseMesh(LPLYNXMESH lpmd)
{	
	lynxReleaseObject3D(&lpmd->Obj3D);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMesh 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxInstanceMesh(LPLYNXMESH lpd, LPCLYNXMESH lps)
{
	int							i;
	
	lynxInitObject3D(&lpd->Obj3D);	
	lynxInstanceObject3D(&lpd->Obj3D, &lps->Obj3D);	
	for (i=0; i<LYNX_MAX_LOD; i++)
	{
		lpd->BaseVertex[i] = lps->BaseVertex[i];
		lpd->NumVertices[i] = lps->NumVertices[i];	
		lpd->BaseTriangle[i] = lps->BaseTriangle[i];
		lpd->NumTriangles[i] = lps->NumTriangles[i];
	}	
	lpd->MaterialIndex = lps->MaterialIndex;	

	lpd->TextureMotionMode = lps->TextureMotionMode;
	lpd->TextureMotionArg.NumArgs = lps->TextureMotionArg.NumArgs;
	lpd->TextureMotionArg.lpArg = lps->TextureMotionArg.lpArg;
	
	lynxInstanceVertexArray(&lpd->VertexArray, &lps->VertexArray);
	lynxInstanceIndexArray(&lpd->IndexArray, &lps->IndexArray);

	lpd->NumBones = lps->NumBones;
	lpd->lpBoneShaderData = lps->lpBoneShaderData;	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMesh 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void		lynxGetMeshTransM(LPLYNXMESH lpm, LYNXREAL time)
{
	lynxGetObject3DTransM(&lpm->Obj3D, time);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMesh 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxSetMeshAnimationKeys(LPLYNXMESH lpmesh, LPLYNXANIMATIONKEY lptran, LPLYNXANIMATIONKEY lpscl, LPLYNXSHORTANIMATIONKEY lprot, LPLYNXSHORTANIMATIONKEY lpvis)
{	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMesh 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxRenderMesh(LPCLYNXMESH lpm)
{
	int					i, j, k;
	LPLYNXMODEL			lpM;
	LPLYNXMESH			lpMesh;
	BYTE				Alpha;	

	lpMesh = (LPLYNXMESH)lpm;	
	lpM = (LPLYNXMODEL)lpMesh->lpModel;

	/*
	if (lpMesh->Obj3D.VisKey.lpKeys)
	{
		Alpha = LYNX_ROUND((lpMesh)->Obj3D.Visibility*255.0f);
		for (k=lpMesh->BaseVertex[lpM->LOD]; k<lpMesh->BaseVertex[lpM->LOD]+lpMesh->NumVertices[lpM->LOD]; k++)
			LYNX_RV_SET_A(lpM->lpTVertexList[lpM->LOD][k], Alpha);								
	}					
	else
	{
		if (lpM->lpColorVertexList[lpM->LOD])
		{
			for (k=lpMesh->BaseVertex[lpM->LOD]; k<lpMesh->BaseVertex[lpM->LOD]+lpMesh->NumVertices[lpM->LOD]; k++)
				LYNX_RV_SET_A(lpM->lpTVertexList[lpM->LOD][k], lpM->lpColorVertexList[lpM->LOD][k].s.Alpha); 								
		}													
	}
	*/
	//if (lpMesh->lpPrimitive[i].lpIndex)
	{
		//D3DDEVICE_SetTransform(glpRenderer->SystemData3D.lpD3DDevice, D3DTS_WORLD, (D3DMATRIX *)(&(lpMesh->Obj3D.M)));
		lynxSetWorldMatrix(&lpMesh->Obj3D.M);			
		for (i=0; i<lpMesh->NumPrimitive; i++)			
			lynxDrawElements(lpMesh->lpPrimitive[i].Type, lpMesh->BaseVertex[lpM->CurrentLOD], lpMesh->NumVertices[lpM->CurrentLOD], lpMesh->lpPrimitive[i].BaseIndex,lpMesh->lpPrimitive[i].NumPrimitives); 
			//lynxDrawPrimitive(&lpMesh->lpPrimitive[i], lpMesh->BaseVertex[lpM->LOD], lpMesh->NumVertices[lpM->LOD]);
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxMesh 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void	lynxRenderMeshBO(LPCLYNXMESH lpm)
{
	int					i, j, k, Base, Num;
	LPLYNXMODEL			lpM;
	LPLYNXMESH			lpMesh;
	LYNXREAL			Alpha[4], VC[4];	
	LYNXMATRIX			TM1;	

	lpMesh = (LPLYNXMESH)lpm;	
		
	//if (lpMesh->lpPrimitive[i].lpIndex)
	{
		lpM = (LPLYNXMODEL)lpMesh->lpModel;
		switch (lpM->Type)
		{
			case LYNX_HIE_MODEL:
				lynxTransposeMatrix2(&TM1, &lpMesh->Obj3D.M);										
				lynxSetVertexShaderConstant(lpMesh->VertexConstantIndex, (LYNXREAL *)&TM1, 3);	//we only need 3 rows
				break;

			case LYNX_HIE_BONESKIN_MODEL:									
				for (i=0; i<lpMesh->NumBones; i++)
				{
					lynxTransposeMatrix2(&TM1, &lpM->lpBones[lpMesh->lpBoneShaderData[i].BoneIndex].Obj3D.M);										
					lynxSetVertexShaderConstant(lpMesh->lpBoneShaderData[i].ConstantIndex, (LYNXREAL *)&TM1, 3);	//we only need 3 rows
				}								
				break;
		}
		if (lpMesh->Obj3D.VisKey.lpKeys)
		{
			Alpha[0] = 1.0f;
			Alpha[1] = 1.0f;
			Alpha[2] = 1.0f;
			Alpha[3] = (lpMesh)->Obj3D.Visibility;						
		}					
		else
		{
			Alpha[0] = 1.0f;
			Alpha[1] = 1.0f;
			Alpha[2] = 1.0f;
			Alpha[3] = (lpMesh)->Obj3D.Visibility;						
		}					
		lynxSetVertexShaderConstant(lynxGetlpRenderer()->ShaderManager.FogValueConstant-1, Alpha, 1);
		for (i=0; i<lpMesh->NumPrimitive; i++)
			lynxDrawElementsBO(lpMesh->lpPrimitive[i].Type, lpMesh->BaseVertex[lpM->CurrentLOD], lpMesh->NumVertices[lpM->CurrentLOD], lpMesh->lpPrimitive[i].BaseIndex,lpMesh->lpPrimitive[i].NumPrimitives); 
			//lynxDrawPrimitiveBO(&lpMesh->lpPrimitive[i], lpMesh->BaseVertex[lpM->LOD], lpMesh->NumVertices[lpM->LOD]);			
	}
}
////-------------------------------------------------------------------------------------------------------
///**
// *  @ingroup LynxMesh 
// *  @brief 。
// *
// *	@param 。
// *	@param 。
// *	@param 。
// *	@return。 
// */
////-------------------------------------------------------------------------------------------------------
// void	lynxGeometryMotion(LPLYNXRENDERVERTEX lprest, LYNXGEOMETRYMOTION type, LPLYNXVERTEX3D lporg, int sv, int ev, float *lparg, BYTE *lpfix, float skip)
//{
//	int								i, j, o, FI, Offset;
//	float							Phase, AmpY, PhaseU, PhaseV;	
//	float							AmpU, AmpV, SpeedU, SpeedV, StepU, StepV, IncU, IncV;	
//	BYTE							FixMask;
//
//	switch (type)
//	{
//		//Notes : Texture must be tiled, use unwarp may cause error
//		case LYNX_GM_SINWAVE:							
//			AmpU =  lparg[0];
//			StepU = lparg[1]*LYNXPI;
//			SpeedU = lparg[2];
//
//			AmpV =  lparg[9];
//			StepV = lparg[10]*LYNXPI;
//			SpeedV = lparg[11];
//
//			Phase = lparg[3];
//			PhaseU = lparg[4];
//			PhaseV = lparg[5];
//
//			lparg[12] += SpeedU*skip;
//			if (lparg[12] > (LYNXPI*2))
//				lparg[12] -= (LYNXPI*2);
//			IncU = lparg[12];
//
//			lparg[13] += SpeedV*skip;
//			if (lparg[13] > (LYNXPI*2))
//				lparg[13] -= (LYNXPI*2);
//			IncV = lparg[13];
//
//			for (j=sv; j<ev; j++)
//			{
//				o = j - sv;
//				FI = o>>3;
//				Offset = o&0x7;
//				FixMask = lpfix[FI] >> Offset;
//				if (!(FixMask&0x1))
//				{
//					//AmpY = (sin(lporguv[o+stv].v*StepV+IncV+PhaseU*lporguv[o+stv].u))*AmpV;					
//					//AmpY += (sin(lporguv[o+stv].u*StepU+IncU+Phase+PhaseV*lporguv[o+stv].v))*AmpU;
//
//					AmpY = (((sin(lporg[o].TexCoord[0].v*StepV+PhaseV+IncV+Phase*lporg[o].TexCoord[0].u))*AmpV));// + (PhaseU*lporguv[o+stv].u)*AmpU);					
//					AmpY += (((sin(lporg[o].TexCoord[0].u*StepU+PhaseU+IncU+Phase*lporg[o].TexCoord[0].v))*AmpU));// + (PhaseV*lporguv[o+stv].v)*AmpV);
//
//					LYNX_RV_X(lprest[j]) = lporg[o].x+AmpY*lparg[6];										
//					LYNX_RV_Y(lprest[j]) = lporg[o].y+AmpY*lparg[7];										
//					LYNX_RV_Z(lprest[j]) = lporg[o].z+AmpY*lparg[8];																			
//				}
//			}
//			break;
//	}			
//}