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

#include <LynxParticleEmitter.h>

//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitParticle(LPLYNXPARTICLE lpp)
{
	lpp->r = LYNX_REAL(0);
	lpp->Color.PackedColor = 0xffffffff;
	lpp->DegX = LYNX_REAL(0);
	lpp->DegY = LYNX_REAL(0);
	lpp->DegZ = LYNX_REAL(0);
	lpp->LifeTime = LYNX_REAL(0);
	lpp->Sprite3DTime = LYNX_REAL(0);
	lpp->lpNext = NULL;
	lpp->lpLast = NULL;
	lpp->lpParticle = NULL;
	//LYNX_VECTOR3D(lpp->Vel, LYNX_REAL(0), LYNX_REAL(0), LYNX_REAL(0));
	//LYNX_VECTOR3D(lpp->Vel0, LYNX_REAL(0), LYNX_REAL(0), LYNX_REAL(0));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxReleaseParticle(LPLYNXPEMITTER lpm, LPLYNXPARTICLE lpp)
{
	if (lpp->lpLast)
		lpp->lpLast->lpNext = lpp->lpNext;
	if (lpp->lpNext)
		lpp->lpNext->lpLast = lpp->lpLast;	
	if (lpp == lpm->lpParticleList)
		lpm->lpParticleList = lpp->lpNext;
	else if (lpp == lpm->lpCurrentParticle)
		lpm->lpCurrentParticle = lpp->lpLast;
	LYNX_FREE(lpp);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxInitParticleEmitter(LPLYNXPEMITTER lpm)
{
	lynxInitObject3D(&lpm->Obj3D);
	lpm->lpMaterial = NULL;
	lpm->Type = LYNX_PARTICLE_EMITTER_POINT;	
	lpm->Noise.s.x = LYNX_REAL(0); lpm->Noise.s.y = LYNX_REAL(0); lpm->Noise.s.z = LYNX_REAL(0); 
	lpm->Gravity.s.x = LYNX_REAL(0); lpm->Gravity.s.y = LYNX_REAL(-0.49f); lpm->Gravity.s.z = LYNX_REAL(0);
	lpm->NumParticle = 0;
	lpm->NumEmitedParticle = 0;
	//lpm->Pos.s.x = 0.0f; lpm->Pos.s.y = 0.0f; lpm->Pos.s.z = 0.0f;	
	lpm->StartDegX = LYNX_REAL(0);
	lpm->StartDegY = LYNX_REAL(0);
	lpm->StartDegZ = LYNX_REAL(0);
	lpm->RotateSpeedX = LYNX_REAL(0);
	lpm->RotateSpeedY = LYNX_REAL(0);
	lpm->RotateSpeedZ = LYNX_REAL(0);
	lpm->Vel0.s.x = LYNX_REAL(0); lpm->Vel0.s.y = LYNX_REAL(0); lpm->Vel0.s.z = LYNX_REAL(0);
	lpm->Vel.s.x = LYNX_REAL(0); lpm->Vel.s.y = LYNX_REAL(0); lpm->Vel.s.z = LYNX_REAL(0);
	lpm->lpParticleList = NULL;
	lpm->lpParticle = NULL;
	lpm->lpCurrentParticle = NULL;
	lpm->bTrail = FALSE;
	lpm->MaxTrailStep = 8;	
	lpm->ParticleRatio = LYNX_REAL(1);
	lpm->LifeTimeLimit = LYNX_PARTICLE_MAX_LIFE_TIME;
	lpm->MaxParticle = LYNX_MAX_NUM_PARTICLE;
	lpm->EmitVel.s.x = LYNX_REAL(0); lpm->EmitVel.s.y = LYNX_REAL(0); lpm->EmitVel.s.z = LYNX_REAL(0);
	lpm->Density = 2;
	lpm->bLoop = FALSE;	
	lpm->bEmit = FALSE;
	lpm->Range = LYNX_REAL(20);	
	lpm->lpDeadParticleList = NULL;
	lpm->lpCurrentDeadParticle = NULL;
	lpm->ParticleType = LYNX_PARTICLE_SPRITE3D;
	lpm->NoiseType = LYNX_PARTICLE_NOISE_NONE;
	lpm->EmitTime = lpm->Time = LYNX_REAL(0);
	lpm->EmitIntervalTime = LYNX_REAL(0);
	lpm->Flag = LYNX_CROSSTEXSPRITE_X | LYNX_CROSSTEXSPRITE_Z;
	lpm->bSingleTexture = FALSE;
	lpm->Obj3D.Play = (LPLYNXPLAYPROC)lynxPlayParticleEmitter;
	lpm->Obj3D.Render = (LPLYNXRENDERPROC)lynxRenderParticleEmitter;

	lynxInitAnimationKey(&lpm->ParticleSizeKey);
	lynxInitShortAnimationKey(&lpm->ParticleColorKey);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxCreateParticleEmitterSizeKeys(LPLYNXPEMITTER lpm, int num)
{
	lpm->ParticleSizeKey.NumKeys = num;
	lpm->ParticleSizeKey.lpKeys = (LPLYNXREALKEY)LYNX_MALLOC(lpm->ParticleSizeKey.NumKeys*sizeof(LYNXREALKEY));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxCreateParticleEmitterColorKeys(LPLYNXPEMITTER lpm, int num)
{
	lpm->ParticleColorKey.NumKeys = num;
	lpm->ParticleColorKey.lpKeys = (LPLYNXSHORTKEY)LYNX_MALLOC(lpm->ParticleColorKey.NumKeys*sizeof(LYNXSHORTKEY));
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxReleaseParticleEmitterKeys(LPLYNXPEMITTER lpm)
{
	lynxReleaseParticleEmitterColorKeys(lpm);
	lynxReleaseParticleEmitterSizeKeys(lpm);
	lynxReleaseObject3D(&lpm->Obj3D);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxReleaseParticleEmitterSizeKeys(LPLYNXPEMITTER lpm)
{
	lpm->ParticleSizeKey.NumKeys = 0;
	if (lpm->ParticleSizeKey.lpKeys)
		LYNX_FREE(lpm->ParticleSizeKey.lpKeys);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxReleaseParticleEmitterColorKeys(LPLYNXPEMITTER lpm)
{
	lpm->ParticleColorKey.NumKeys = 0;
	if (lpm->ParticleColorKey.lpKeys)
		LYNX_FREE(lpm->ParticleColorKey.lpKeys);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxDuplicateParticleEmitter(LPLYNXPEMITTER lpmd, LPLYNXPEMITTER lpms)
{
	LPLYNXRENDERVERTEX						lpVertexArray;			
	int										i;

	lynxInitParticleEmitter(lpmd);	
	lpmd->Type = lpms->Type;
	lpmd->bLoop = lpms->bLoop;	
	lpmd->bTrail = lpms->bTrail;	
	lpmd->Density = lpms->Density;
	lpmd->EmitIntervalTime = lpms->EmitIntervalTime;	
	lpmd->EmitVel.s.x = lpms->EmitVel.s.x;
	lpmd->EmitVel.s.y = lpms->EmitVel.s.y;
	lpmd->EmitVel.s.z = lpms->EmitVel.s.z;
	lpmd->RotateSpeedX = lpms->RotateSpeedX;
	lpmd->RotateSpeedY = lpms->RotateSpeedY;
	lpmd->RotateSpeedZ = lpms->RotateSpeedZ;
	lpmd->Flag = lpms->Flag;
	lpmd->Gravity.s.x = lpms->Gravity.s.x;
	lpmd->Gravity.s.y = lpms->Gravity.s.y;
	lpmd->Gravity.s.z = lpms->Gravity.s.z;
	lpmd->LifeTimeLimit = lpms->LifeTimeLimit;
	lpmd->MaxParticle = lpms->MaxParticle;
	lpmd->MaxTrailStep = lpms->MaxTrailStep;
	lpmd->Noise.s.x = lpms->Noise.s.x;
	lpmd->Noise.s.y = lpms->Noise.s.y;
	lpmd->Noise.s.z = lpms->Noise.s.z;
	lpmd->NoiseType = lpms->NoiseType;	
	lpmd->ParticleRatio = lpms->ParticleRatio;
	lpmd->ParticleType = lpms->ParticleType;
	lpmd->Range = lpms->Range;
	lpmd->Vel0.s.x = lpms->Vel0.s.x;
	lpmd->Vel0.s.y = lpms->Vel0.s.y;
	lpmd->Vel0.s.z = lpms->Vel0.s.z;
	lpmd->bEmit = lpms->bEmit;
	lpmd->bSingleTexture = lpms->bSingleTexture;

	lynxDuplicateAnimationKey(&lpmd->ParticleSizeKey, &lpms->ParticleSizeKey);
	lynxDuplicateShortAnimationKey(&lpmd->ParticleColorKey, &lpms->ParticleColorKey);

	switch (lpmd->ParticleType)
	{
		case LYNX_PARTICLE_SPRITE3D:
			lpmd->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXTEXSPRITE3D));
			lynxDuplicateTexSprite3D((LPLYNXTEXSPRITE3D)lpmd->lpParticle, (LPLYNXTEXSPRITE3D)lpms->lpParticle);			
			#ifdef __TOOL__
				lynxCreateVertexArray(&lpmd->VertexArray, lynxGetBuiltinVertexLayout(LYNX_RENDER_VERTEX), 4, LYNX_LOCK_TO_WRITE, LYNX_FALSE);
			#else
				lynxCreateVertexArray(&lpmd->VertexArray, lynxGetBuiltinVertexLayout(LYNX_RENDER_VERTEX), 4, LYNX_LOCK_TO_WRITE, LYNX_TRUE);
			#endif			
			lynxLockVertexArray(&lpmd->VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);
			lpVertexArray = (LPLYNXRENDERVERTEX)lpmd->VertexArray.lpBuffer;
	
			for (i=0; i<4; i++)
			{	
				LYNX_RV_SET_XYZ(lpVertexArray[i], ((LPLYNXTEXSPRITE3D)(lpmd->lpParticle))->TexSprite.V[i].x,
												((LPLYNXTEXSPRITE3D)(lpmd->lpParticle))->TexSprite.V[i].y,
												((LPLYNXTEXSPRITE3D)(lpmd->lpParticle))->TexSprite.V[i].z);					

				LYNX_RV_SET_UV(lpVertexArray[i], 0, ((LPLYNXTEXSPRITE3D)(lpmd->lpParticle))->TexSprite.V[i].TexCoord[0].u,
													((LPLYNXTEXSPRITE3D)(lpmd->lpParticle))->TexSprite.V[i].TexCoord[0].v);									  		
				
				LYNX_RV_SET_RGBA(lpVertexArray[i], 255, 255, 255, 255);				
			}	
			lynxUnlockVertexArray(&lpmd->VertexArray);	
			break;

		case LYNX_PARTICLE_CROSS_SPRITE:
			lpmd->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXCROSSTEXSPRITE));
			lynxDuplicateCrossTexSprite((LPLYNXCROSSTEXSPRITE)lpmd->lpParticle, (LPLYNXCROSSTEXSPRITE)lpms->lpParticle);
			break;

		case LYNX_PARTICLE_MODEL:
			lpmd->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXMODEL));
			lynxInstanceModel((LPLYNXMODEL)lpmd->lpParticle, (LPLYNXMODEL)lpms->lpParticle);
			break;

		case LYNX_PARTICLE_PEMITTER:
			lpmd->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXPEMITTER));			
			lynxDuplicateParticleEmitter((LPLYNXPEMITTER)lpmd->lpParticle, (LPLYNXPEMITTER)lpms->lpParticle);
			break;
	}
	lynxResetParticleEmitter(lpmd);
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxReleaseParticleEmitter(LPLYNXPEMITTER lpm)
{	
	LPLYNXPARTICLE						lpP, lpCP;

	if (lpm->lpParticle)
	{
		switch (lpm->ParticleType)
		{
			case LYNX_PARTICLE_SPRITE3D:
				lynxReleaseTexSprite3D((LPLYNXTEXSPRITE3D)lpm->lpParticle);
				break;

			case LYNX_PARTICLE_CROSS_SPRITE:
				lynxReleaseCrossTexSprite((LPLYNXCROSSTEXSPRITE)lpm->lpParticle);
				break;

			case LYNX_PARTICLE_MODEL:
				lynxReleaseModel((LPLYNXMODEL)lpm->lpParticle);
				break;

			case LYNX_PARTICLE_PEMITTER:
				lynxReleaseParticleEmitter((LPLYNXPEMITTER)lpm->lpParticle);
				break;
		}
		LYNX_FREE(lpm->lpParticle);
	}
	lynxReleaseAllParticles(lpm);	
	lynxReleaseParticleEmitterKeys(lpm);	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxReleaseAllParticles(LPLYNXPEMITTER lpm)
{	
	LPLYNXPARTICLE						lpP, lpCP;
	
	lpP = lpm->lpParticleList;
	while (lpP)
	{
		lpCP = lpP;
		lpP = lpP->lpNext;
		lynxReleaseParticle(lpm, lpCP);			
	}
	lpP = lpm->lpDeadParticleList;
	while (lpP)
	{
		lpCP = lpP;
		lpP = lpP->lpNext;
		lynxReleaseParticle(lpm, lpCP);			
	}	
	lpm->lpParticleList = NULL;
	lpm->lpDeadParticleList = NULL;
	lpm->lpMaterial = NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxCreateParticleEmitter(LPLYNXPEMITTER lpm, LPLYNXVECTOR3D lppos, LPLYNXVECTOR3D lpvel, const char *filename, LYNXPARTICLETYPE type, LPLYNXOBJ3D lpsub)
{
	LYNXCOLORRGBA							C;
	LPLYNXPEMITTER							lpSubPE;

	LPLYNXRENDERVERTEX						lpVertexArray;			
	int										i;

	lynxInitParticleEmitter(lpm);		
	if (lppos)
	{
		lynxSetObject3DPosition(&lpm->Obj3D, lppos);		
	}
	if (lpvel)
	{
		lpm->Vel0.s.x = lpvel->s.x; lpm->Vel0.s.y = lpvel->s.y; lpm->Vel0.s.z = lpvel->s.z;
		lpm->Vel.s.x = lpvel->s.x; lpm->Vel.s.y = lpvel->s.y; lpm->Vel.s.z = lpvel->s.z;
	}

	//ParticleR = lpm->ParticleSizeKey.lpKeys[0].lpValues[0];	
	lpm->ParticleType = type;
	switch (type)
	{
		case LYNX_PARTICLE_SPRITE3D:
			lpm->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXTEXSPRITE3D));
			lynxLoadTexSprite3D((LPLYNXTEXSPRITE3D)lpm->lpParticle, filename, LYNX_REAL(20), LYNX_REAL_MUL(LYNX_REAL(20), lpm->ParticleRatio), NULL);									
			lpm->lpMaterial = &((LPLYNXTEXSPRITE3D)lpm->lpParticle)->TexSprite.Material;
			#ifdef __TOOL__
				lynxCreateVertexArray(&lpm->VertexArray, lynxGetBuiltinVertexLayout(LYNX_RENDER_VERTEX), 4, LYNX_BOF_STATIC, LYNX_FALSE);			
			#else
				lynxCreateVertexArray(&lpm->VertexArray, lynxGetBuiltinVertexLayout(LYNX_RENDER_VERTEX), 4, LYNX_BOF_STATIC, LYNX_TRUE);		
			#endif
			lynxLockVertexArray(&lpm->VertexArray, 0, 0, LYNX_LOCK_TO_WRITE);
			lpVertexArray = (LPLYNXRENDERVERTEX)lpm->VertexArray.lpBuffer;
	
			for (i=0; i<4; i++)
			{	
				LYNX_RV_SET_XYZ(lpVertexArray[i], ((LPLYNXTEXSPRITE3D)(lpm->lpParticle))->TexSprite.V[i].x,
												((LPLYNXTEXSPRITE3D)(lpm->lpParticle))->TexSprite.V[i].y,
												((LPLYNXTEXSPRITE3D)(lpm->lpParticle))->TexSprite.V[i].z);					

				LYNX_RV_SET_UV(lpVertexArray[i], 0, ((LPLYNXTEXSPRITE3D)(lpm->lpParticle))->TexSprite.V[i].TexCoord[0].u,
													((LPLYNXTEXSPRITE3D)(lpm->lpParticle))->TexSprite.V[i].TexCoord[0].v);									  		
				
				LYNX_RV_SET_RGBA(lpVertexArray[i], 255, 255, 255, 255);				
			}	
			lynxUnlockVertexArray(&lpm->VertexArray);	
			break;

		case LYNX_PARTICLE_CROSS_SPRITE:
			lpm->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXCROSSTEXSPRITE));
			lynxLoadCrossTexSprite((LPLYNXCROSSTEXSPRITE)lpm->lpParticle, filename, LYNX_REAL(20), LYNX_REAL_MUL(LYNX_REAL(20), lpm->ParticleRatio), NULL, lpm->Flag);
			lpm->lpMaterial = &((LPLYNXCROSSTEXSPRITE)lpm->lpParticle)->TexSprite.Material;
			break;

		case LYNX_PARTICLE_MODEL:
			lpm->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXMODEL));
			lynxLoadModel((LPLYNXMODEL)lpm->lpParticle, filename, NULL, 0, TRUE);
			lpm->lpMaterial = &((LPLYNXMODEL)lpm->lpParticle)->lpMaterials[0];
			break;

		case LYNX_PARTICLE_PEMITTER:
			lpm->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXPEMITTER));						
			if (lpsub)
			{				
				lynxDuplicateParticleEmitter((LPLYNXPEMITTER)lpm->lpParticle, (LPLYNXPEMITTER)lpsub);
			}
			else
			{
				lpSubPE = (LPLYNXPEMITTER)lpm->lpParticle;
				lynxCreateParticleEmitter(lpSubPE, lppos, lpvel, filename, LYNX_PARTICLE_SPRITE3D, NULL);
				lynxCreateParticleEmitterSizeKeys(lpSubPE, 2);				
				lpSubPE->ParticleSizeKey.lpKeys[0].STime = 0;
				lpSubPE->ParticleSizeKey.lpKeys[0].ETime = LYNX_PARTICLE_MAX_LIFE_TIME;
				lpSubPE->ParticleSizeKey.lpKeys[0].NumValues = 1;
				lpSubPE->ParticleSizeKey.lpKeys[0].lpValues[0] = LYNX_REAL(2);
				lpSubPE->ParticleSizeKey.lpKeys[1].STime = LYNX_PARTICLE_MAX_LIFE_TIME;
				lpSubPE->ParticleSizeKey.lpKeys[1].ETime = LYNX_PARTICLE_MAX_LIFE_TIME+1;
				lpSubPE->ParticleSizeKey.lpKeys[1].NumValues = 1;
				lpSubPE->ParticleSizeKey.lpKeys[1].lpValues[0] = LYNX_REAL(2);

				lynxCreateParticleEmitterColorKeys(lpSubPE, 2);				
				lpSubPE->ParticleColorKey.lpKeys[0].STime = 0;
				lpSubPE->ParticleColorKey.lpKeys[0].ETime = LYNX_PARTICLE_MAX_LIFE_TIME;
				lpSubPE->ParticleColorKey.lpKeys[0].NumValues = 4;
				lpSubPE->ParticleColorKey.lpKeys[0].lpValues[0] = 255;
				lpSubPE->ParticleColorKey.lpKeys[0].lpValues[1] = 255;
				lpSubPE->ParticleColorKey.lpKeys[0].lpValues[2] = 255;
				lpSubPE->ParticleColorKey.lpKeys[0].lpValues[3] = 255;
				lpSubPE->ParticleColorKey.lpKeys[1].STime = LYNX_PARTICLE_MAX_LIFE_TIME;
				lpSubPE->ParticleColorKey.lpKeys[1].ETime = LYNX_PARTICLE_MAX_LIFE_TIME+1;
				lpSubPE->ParticleColorKey.lpKeys[1].NumValues = 4;
				lpSubPE->ParticleColorKey.lpKeys[1].lpValues[0] = 255;
				lpSubPE->ParticleColorKey.lpKeys[1].lpValues[1] = 255;
				lpSubPE->ParticleColorKey.lpKeys[1].lpValues[2] = 255;
				lpSubPE->ParticleColorKey.lpKeys[1].lpValues[3] = 0;
			}			
			lpm->lpMaterial = ((LPLYNXPEMITTER)lpm->lpParticle)->lpMaterial;
			break;
	}	

	//Yahoo!!!
	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxChangeParticleType(LPLYNXPEMITTER lpm, LYNXPARTICLETYPE type)
{
	LYNXCOLORRGBA							C;
	LYNXREAL								ParticleR;
	
	switch (lpm->ParticleType)
	{
		case LYNX_PARTICLE_SPRITE3D:
			lynxReleaseTexSprite3D((LPLYNXTEXSPRITE3D)lpm->lpParticle);
			break;

		case LYNX_PARTICLE_CROSS_SPRITE:
			lynxReleaseCrossTexSprite((LPLYNXCROSSTEXSPRITE)lpm->lpParticle);
			break;

		case LYNX_PARTICLE_MODEL:
			lynxReleaseModel((LPLYNXMODEL)lpm->lpParticle);
			break;
	}
	lpm->ParticleType = type;
	if (lpm->lpParticle)
		LYNX_FREE(lpm->lpParticle);

	ParticleR = lpm->ParticleSizeKey.lpKeys[0].lpValues[0];
	switch (type)
	{
		case LYNX_PARTICLE_SPRITE3D:
			lpm->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXTEXSPRITE3D));
			lynxLoadTexSprite3D((LPLYNXTEXSPRITE3D)lpm->lpParticle, "", LYNX_REAL_MUL(ParticleR, LYNX_REAL(2)), LYNX_REAL_MUL(LYNX_REAL_MUL(ParticleR, LYNX_REAL(2)), lpm->ParticleRatio), NULL);
			break;

		case LYNX_PARTICLE_CROSS_SPRITE:
			lpm->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXCROSSTEXSPRITE));
			lynxLoadCrossTexSprite((LPLYNXCROSSTEXSPRITE)lpm->lpParticle, "", LYNX_REAL_MUL(ParticleR, LYNX_REAL(2)), LYNX_REAL_MUL(LYNX_REAL_MUL(ParticleR, LYNX_REAL(2)), lpm->ParticleRatio), NULL, lpm->Flag);
			break;

		case LYNX_PARTICLE_MODEL:
			lpm->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXMODEL));
			lynxLoadModel((LPLYNXMODEL)lpm->lpParticle, "", NULL, 0, TRUE);
			break;

		case LYNX_PARTICLE_PEMITTER:
			lpm->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXPEMITTER));			
			lynxInitParticleEmitter((LPLYNXPEMITTER)lpm->lpParticle);
			break;
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxSetParticleEmitterMaxParticle(LPLYNXPEMITTER lpm, int num)
{
	LPLYNXPARTICLE						lpP;

	lpm->MaxParticle = num;
	lpm->NumParticle = 0;
	lpP = lpm->lpParticleList;
	while (lpP)
	{
		lpP = lpP->lpNext;
		if (lpP)
			lynxReleaseParticle(lpm, lpP->lpLast);	
	}
	lpm->lpParticleList = NULL;

	lpP = lpm->lpDeadParticleList;
	while (lpP)
	{
		lpP = lpP->lpNext;
		if (lpP)
			lynxReleaseParticle(lpm, lpP->lpLast);	
	}
	lpm->lpDeadParticleList = NULL;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxCreateParticle(LPLYNXPEMITTER lpm, LPLYNXVECTOR3D lpvel)
{
	LPLYNXPARTICLE					lpP;
	LYNXVECTOR3D					Pos, TPos;
	LYNXMATRIX						M;	
	LPLYNXTEXSPRITE3D				lpSprite3D;
	LPLYNXCROSSTEXSPRITE			lpCrossSprt;

	if (lpm->NumParticle >= lpm->MaxParticle)
	{	
		if (!lpm->bLoop)
		{
			if (lpm->NumEmitedParticle >= lpm->MaxParticle) 
			{
				lpm->bEmit = FALSE;
				return FALSE;
			}
		}	
		switch (lpm->Type)
		{
			case LYNX_PARTICLE_EMITTER_POINT:
				LYNX_VECTOR3D_COPY((TPos), (*lpm->Obj3D.lpCenter));
				break;

			case LYNX_PARTICLE_EMITTER_PLANE:				
				Pos.s.x = LYNX_REAL_MUL(LYNX_REAL_SUB(LYNX_REAL(5000), LYNX_REAL((rand()%10000))), LYNX_REAL(0.0001f));
				Pos.s.y = LYNX_REAL_MUL(LYNX_REAL_SUB(LYNX_REAL(5000), LYNX_REAL((rand()%10000))), LYNX_REAL(0.0001f));
				Pos.s.z = LYNX_REAL(0);
				lynxPointXMatrix(&TPos, &Pos, &lpm->Obj3D.M);												
				break;			
		}

		lpP = lpm->lpDeadParticleList;			
		/*
		if (lpP)
		{
			lpm->lpDeadParticleList = lpP->lpNext;
			if (lpm->lpDeadParticleList)
				lpm->lpDeadParticleList->lpLast = NULL;
			if (lpm->lpParticleList == NULL)
			{
				lynxInitParticle(lpP);
				lpm->lpCurrentParticle->lpNext = lpP;
				lpP->lpLast = lpm->lpCurrentParticle;
				lpP->r = r;
				lpP->Color.PackedColor = lpc->PackedColor;
				//lpP->Pos.s.x = lpm->Obj3D.TCenter.s.x; lpP->Pos.s.y = lpm->Obj3D.TCenter.s.y; lpP->Pos.s.z = lpm->Obj3D.TCenter.s.z;
				lpP->Pos.s.x = TPos.s.x; lpP->Pos.s.y = TPos.s.y; lpP->Pos.s.z = TPos.s.z;
   				lpP->Vel0.s.x = lpm->Vel0.s.x+lpvel->s.x; lpP->Vel0.s.y = lpm->Vel0.s.y+lpvel->s.y; lpP->Vel0.s.z = lpm->Vel0.s.z+lpvel->s.z;
				lpP->Vel.s.x = lpP->Vel0.s.x; lpP->Vel.s.y = lpP->Vel0.s.y; lpP->Vel.s.z = lpP->Vel0.s.z;
				lpP->LifeTimeLimit = lpm->LifeTimeLimit;
				lpm->lpCurrentParticle = lpP;			
			}
			else
			{
				lynxInitParticle(lpP);
				lpm->lpCurrentParticle->lpNext = lpP;
				lpP->lpLast = lpm->lpCurrentParticle;
				lpP->r = r;
				lpP->Color.PackedColor = lpc->PackedColor;
				//lpP->Pos.s.x = lpm->Obj3D.TCenter.s.x; lpP->Pos.s.y = lpm->Obj3D.TCenter.s.y; lpP->Pos.s.z = lpm->Obj3D.TCenter.s.z;
				lpP->Pos.s.x = TPos.s.x; lpP->Pos.s.y = TPos.s.y; lpP->Pos.s.z = TPos.s.z;
   				lpP->Vel0.s.x = lpm->Vel0.s.x+lpvel->s.x; lpP->Vel0.s.y = lpm->Vel0.s.y+lpvel->s.y; lpP->Vel0.s.z = lpm->Vel0.s.z+lpvel->s.z;
				lpP->Vel.s.x = lpP->Vel0.s.x; lpP->Vel.s.y = lpP->Vel0.s.y; lpP->Vel.s.z = lpP->Vel0.s.z;
				lpP->LifeTimeLimit = lpm->LifeTimeLimit;
				lpm->lpCurrentParticle = lpP;
			}				
			lpP->DegX = lpm->StartDegX;
			lpP->DegY = lpm->StartDegY;
			lpP->DegZ = lpm->StartDegZ;
			
			if (lpm->bSingleTexture)				
			{
				switch (lpm->ParticleType)
				{
					case LYNX_PARTICLE_SPRITE3D:
						lpSprite3D = (LPLYNXTEXSPRITE3D)lpm->lpParticle;
						lpm->lpCurrentParticle->Sprite3DTime = rand()%((int)(lpSprite3D->TexSprite.EndTime+1));
						if (lpm->lpCurrentParticle->Sprite3DTime > lpSprite3D->TexSprite.EndTime)
							lpm->lpCurrentParticle->Sprite3DTime = lpSprite3D->TexSprite.EndTime;
						break;
				}
			}
		}
		*/
		if (lpP)
		{
			lpm->lpDeadParticleList = lpP->lpNext;
			if (lpm->lpDeadParticleList)
				lpm->lpDeadParticleList->lpLast = NULL;
			if (lpm->lpParticleList == NULL)
			{
				lynxInitParticle(lpP);
				lpm->lpParticleList = lpP;
				lpm->lpParticleList->DegX = LYNX_REAL((rand()%(360)));//lpm->StartDegX;
				lpm->lpParticleList->DegY = LYNX_REAL((rand()%(360)));//lpm->StartDegY;
				lpm->lpParticleList->DegZ = LYNX_REAL((rand()%(360)));//lpm->StartDegZ;
				lpm->lpParticleList->lpLast = NULL;
				lpm->lpParticleList->lpNext = NULL;			
				LYNX_VECTOR3D_COPY(lpm->lpParticleList->Pos, TPos);											
				LYNX_VECTOR3D_ADD(lpm->lpParticleList->Vel0, lpm->Vel0, (*lpvel));   		
				LYNX_VECTOR3D_COPY(lpm->lpParticleList->Vel, lpm->lpParticleList->Vel0);														
				lpm->lpCurrentParticle = lpm->lpParticleList;				
			}
			else
			{
				lynxInitParticle(lpP);
				lpP->DegX = LYNX_REAL((rand()%(360)));//lpm->StartDegX;
				lpP->DegY = LYNX_REAL((rand()%(360)));//lpm->StartDegY;
				lpP->DegZ = LYNX_REAL((rand()%(360)));//lpm->StartDegZ;
				lpm->lpCurrentParticle->lpNext = lpP;
				lpP->lpLast = lpm->lpCurrentParticle;				
				LYNX_VECTOR3D_COPY(lpP->Pos, TPos);											
				LYNX_VECTOR3D_ADD(lpP->Vel0, lpm->Vel0, (*lpvel));   		
				LYNX_VECTOR3D_COPY(lpP->Vel, lpP->Vel0);																	
				lpm->lpCurrentParticle = lpP;
			}				
			lpm->lpCurrentParticle->LifeTimeLimit = lpm->LifeTimeLimit;		
			lpm->lpCurrentParticle->lpParticle = lpm->lpParticle;

			if (lpm->bSingleTexture)				
			{
				switch (lpm->ParticleType)
				{
					case LYNX_PARTICLE_SPRITE3D:
						lpSprite3D = (LPLYNXTEXSPRITE3D)lpm->lpParticle;						
						lpm->lpCurrentParticle->Sprite3DTime = (rand()%(int)(LYNX_REAL_ADD(lpSprite3D->TexSprite.Material.lpMaterialPass[0].TextureFrame[0].EndTime, LYNX_REAL(1))));
						if (lpm->lpCurrentParticle->Sprite3DTime > lpSprite3D->TexSprite.Material.lpMaterialPass[0].TextureFrame[0].EndTime)
							lpm->lpCurrentParticle->Sprite3DTime = lpSprite3D->TexSprite.Material.lpMaterialPass[0].TextureFrame[0].EndTime;
						break;
				}
			}
		}
		
		if (lpm->ParticleType == LYNX_PARTICLE_PEMITTER)
		{
			lpm->lpCurrentParticle->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXPEMITTER));			
			lynxDuplicateParticleEmitter((LPLYNXPEMITTER)lpm->lpCurrentParticle->lpParticle, (LPLYNXPEMITTER)lpm->lpParticle);
		}
		lpm->NumEmitedParticle = LYNX_REAL_ADD(lpm->NumEmitedParticle, LYNX_REAL(1));
		return TRUE;
	}
	else
	{
		switch (lpm->Type)
		{
			case LYNX_PARTICLE_EMITTER_POINT:
				LYNX_VECTOR3D_COPY(TPos, (*lpm->Obj3D.lpCenter));				
				break;

			case LYNX_PARTICLE_EMITTER_PLANE:				
				Pos.s.x = LYNX_REAL_MUL(LYNX_REAL_SUB(LYNX_REAL(5000), LYNX_REAL((rand()%10000))), LYNX_REAL(0.0001f));
				Pos.s.y = LYNX_REAL_MUL(LYNX_REAL_SUB(LYNX_REAL(5000), LYNX_REAL((rand()%10000))), LYNX_REAL(0.0001f));
				Pos.s.z = LYNX_REAL(0);
				lynxPointXMatrix(&TPos, &Pos, &lpm->Obj3D.M);								
				break;			
		}

		if (lpm->lpParticleList == NULL)
		{
			lpm->lpParticleList = (LPLYNXPARTICLE)LYNX_MALLOC(sizeof(LYNXPARTICLE));
			lynxInitParticle(lpm->lpParticleList);
			lpm->lpParticleList->DegX = LYNX_REAL((rand()%(360)));//lpm->StartDegX;
			lpm->lpParticleList->DegY = LYNX_REAL((rand()%(360)));//lpm->StartDegY;
			lpm->lpParticleList->DegZ = LYNX_REAL((rand()%(360)));//lpm->StartDegZ;
			lpm->lpParticleList->lpLast = NULL;
			lpm->lpParticleList->lpNext = NULL;			
			LYNX_VECTOR3D_COPY(lpm->lpParticleList->Pos, TPos);										
			LYNX_VECTOR3D_ADD(lpm->lpParticleList->Vel0, lpm->Vel0, (*lpvel));   		
			LYNX_VECTOR3D_COPY(lpm->lpParticleList->Vel, lpm->lpParticleList->Vel0);										
			lpm->lpCurrentParticle = lpm->lpParticleList;			
		}
		else
		{
			lpP = (LPLYNXPARTICLE)LYNX_MALLOC(sizeof(LYNXPARTICLE));
			lynxInitParticle(lpP);
			lpP->DegX = LYNX_REAL((rand()%(360)));//lpm->StartDegX;
			lpP->DegY = LYNX_REAL((rand()%(360)));//lpm->StartDegY;
			lpP->DegZ = LYNX_REAL((rand()%(360)));//lpm->StartDegZ;
			lpm->lpCurrentParticle->lpNext = lpP;
			lpP->lpLast = lpm->lpCurrentParticle;			
			LYNX_VECTOR3D_COPY(lpP->Pos, TPos);										
   			LYNX_VECTOR3D_ADD(lpP->Vel0, lpm->Vel0, (*lpvel));   		
			LYNX_VECTOR3D_COPY(lpP->Vel, lpP->Vel0);																
			lpm->lpCurrentParticle = lpP;
		}
		lpm->lpCurrentParticle->LifeTimeLimit = lpm->LifeTimeLimit;		
		lpm->lpCurrentParticle->lpParticle = lpm->lpParticle;

		if (lpm->bSingleTexture)				
		{
			switch (lpm->ParticleType)
			{
				case LYNX_PARTICLE_SPRITE3D:
					lpSprite3D = (LPLYNXTEXSPRITE3D)lpm->lpParticle;
					lpm->lpCurrentParticle->Sprite3DTime = (rand()%(int)(LYNX_REAL_ADD(lpSprite3D->TexSprite.Material.lpMaterialPass[0].TextureFrame[0].EndTime, LYNX_REAL(1))));					
					if (lpm->lpCurrentParticle->Sprite3DTime > lpSprite3D->TexSprite.Material.lpMaterialPass[0].TextureFrame[0].EndTime)
						lpm->lpCurrentParticle->Sprite3DTime = lpSprite3D->TexSprite.Material.lpMaterialPass[0].TextureFrame[0].EndTime;
					break;
			}
		}
		if (lpm->ParticleType == LYNX_PARTICLE_PEMITTER)
		{
			lpm->lpCurrentParticle->lpParticle = (LPLYNXOBJ3D)LYNX_MALLOC(sizeof(LYNXPEMITTER));			
			lynxDuplicateParticleEmitter((LPLYNXPEMITTER)lpm->lpCurrentParticle->lpParticle, (LPLYNXPEMITTER)lpm->lpParticle);
		}
		lpm->NumParticle++;
		lpm->NumEmitedParticle++;		
	}	

	//Yahoo!!!
	return TRUE;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
//		Move dead particle to dead particle list
//-------------------------------------------------------------------------------------------------------
void lynxDeleteParticle(LPLYNXPEMITTER lpm, LPLYNXPARTICLE lpp)
{
	if (lpp->lpLast)
		lpp->lpLast->lpNext = lpp->lpNext;
	if (lpp->lpNext)
		lpp->lpNext->lpLast = lpp->lpLast;	
	if (lpp == lpm->lpParticleList)
		lpm->lpParticleList = lpp->lpNext;
	else if (lpp == lpm->lpCurrentParticle)
		lpm->lpCurrentParticle = lpp->lpLast;

	if (lpm->lpDeadParticleList == NULL)
	{
		lpm->lpDeadParticleList = lpp;
		lpm->lpDeadParticleList->lpLast = NULL;
		lpm->lpDeadParticleList->lpNext = NULL;
		lpm->lpCurrentDeadParticle = lpp;			
	}
	else
	{
		lpm->lpCurrentDeadParticle->lpNext = lpp;
		lpp->lpLast = lpm->lpCurrentDeadParticle;
		lpp->lpNext = NULL;
		lpm->lpCurrentDeadParticle = lpp;		
	}		
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxPlayParticle(LPLYNXPEMITTER lpm, LPLYNXPARTICLE lpp, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LPLYNXTEXSPRITE3D					lpSprite3D;
	LPLYNXCROSSTEXSPRITE				lpCrossSprt;

	switch (lpm->ParticleType)
	{
		case LYNX_PARTICLE_SPRITE3D:
			lpSprite3D = (LPLYNXTEXSPRITE3D)lpp->lpParticle;
			if (!lpm->bSingleTexture)				
			{
				lpp->lpParticle->Play(lpp->lpParticle, skip, mode);			
				lpp->Sprite3DTime = lynxGetTexSprite3DTime(lpSprite3D);
			}				
			lpp->DegX = LYNX_REAL_ADD(lpp->DegX, lpm->RotateSpeedX);
			break;

		case LYNX_PARTICLE_CROSS_SPRITE:
			lpp->DegX = LYNX_REAL_ADD(lpp->DegX, lpm->RotateSpeedX);
			lpp->DegY = LYNX_REAL_ADD(lpp->DegY, lpm->RotateSpeedY);
			lpp->DegZ = LYNX_REAL_ADD(lpp->DegZ, lpm->RotateSpeedZ);
			lpCrossSprt = (LPLYNXCROSSTEXSPRITE)lpp->lpParticle;
			if (!lpm->bSingleTexture)							
			{
				lpp->lpParticle->Play(lpp->lpParticle, skip, mode);							
				lpp->Sprite3DTime = lynxGetCrossTexSpriteTime(lpCrossSprt);
			}
			break;			

		case LYNX_PARTICLE_PEMITTER:
		case LYNX_PARTICLE_MODEL:
			lpp->DegX = LYNX_REAL_ADD(lpp->DegX, lpm->RotateSpeedX);
			lpp->DegY = LYNX_REAL_ADD(lpp->DegY, lpm->RotateSpeedY);
			lpp->DegZ = LYNX_REAL_ADD(lpp->DegZ, lpm->RotateSpeedZ);
			lpp->lpParticle->Play(lpp->lpParticle, skip, mode);			
			break;
	}
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
LYNXBOOL lynxPlayParticleEmitter(LPLYNXPEMITTER lpm, LYNXREAL skip, LYNXPLAYMODE mode)
{
	LPLYNXPARTICLE						lpP, lpCP;
	LYNXREAL							R, Base, Times;
	LYNXCOLORRGBA						C;
	int									i, NumP, MOD;
	LYNXVECTOR3D						V;
	LYNXREAL							dR, dG, dB, dA, dRadius, fA, ParticleR;	
	LYNXBOOL							bStop, bEmit;
	short								PColor[4];
	int									cPColor[4];

	bStop = FALSE;	
	
	if (skip > LYNX_REAL(3))
		skip = LYNX_REAL(3);
	/*
	lpm->Vel.s.x = lpm->Vel0.s.x + lpm->Time*lpm->Accel.s.x; lpm->Vel.s.y = lpm->Vel0.s.y + lpm->Time*lpm->Accel.s.y; lpm->Vel.s.z = lpm->Vel0.s.z + lpm->Time*lpm->Accel.s.z;
	if (lynxLength(&lpm->Vel, 3) > 3)
	{
		lynxNormalise(&lpm->Vel, 3);
		LYNX_VECTOR3D_MUL(lpm->Vel, 3, lpm->Vel);
	}
	lpm->Pos.s.x += lpm->Vel.s.x; lpm->Pos.s.y += lpm->Vel.s.y; lpm->Pos.s.z += lpm->Vel.s.z;			
	*/

	V.s.x = LYNX_REAL(0);
	V.s.y = LYNX_REAL(0);
	V.s.z = LYNX_REAL(0.1f);
	lynxUpdateObject3D(&lpm->Obj3D);	
	//LYNX_MATRIX_GET_POS(lpm->Obj3D.TCenter, lpm->Obj3D.M);
	lynxVectorXMatrix(&lpm->EmitVel, &V, &lpm->Obj3D.M);	
	//lynxNormalise(&lpm->EmitVel, 3);
	//lpm->EmitVel.s.x *= lpm->Speed;
	//lpm->EmitVel.s.y *= lpm->Speed;
	//lpm->EmitVel.s.z *= lpm->Speed;	

	//MOD = LYNX_ROUND(lpm->Range*10);
	MOD = LYNX_REAL_ROUND_INT(LYNX_REAL_MUL(lpm->Range, LYNX_REAL(10)));
	if (MOD == 0)
		MOD = 1;
	Base = lpm->Range;
	Times = LYNX_REAL_DIV(lynxLength(&lpm->EmitVel, 3), LYNX_REAL(10));	
	bEmit = FALSE;
	if (LYNX_REAL_SUB(lpm->Time, lpm->EmitTime) >= lpm->EmitIntervalTime)
	{
		bEmit = TRUE;	
		lpm->EmitTime = lpm->Time;
	}
	if (lpm->bEmit)
	{
		NumP = LYNX_REAL_ROUND_INT( ((lpm->Density + ((rand() % lpm->Density)/2))*skip) );	
		for (i=0; i<NumP; i++)
		{
			V.s.x = LYNX_REAL_ADD(lpm->EmitVel.s.x, LYNX_REAL_MUL(LYNX_REAL_SUB( LYNX_REAL( ((rand()%MOD)/5) ), Base ), Times));	//V = lpm->EmitVel + ((rand()%MOD)/5.0f - Base)*Times;  
			V.s.y = LYNX_REAL_ADD(lpm->EmitVel.s.y, LYNX_REAL_MUL(LYNX_REAL_SUB( LYNX_REAL( ((rand()%MOD)/5) ), Base ), Times)); 
			V.s.z = LYNX_REAL_ADD(lpm->EmitVel.s.z, LYNX_REAL_MUL(LYNX_REAL_SUB( LYNX_REAL( ((rand()%MOD)/5) ), Base ), Times));			
			
			ParticleR = lpm->ParticleSizeKey.lpKeys[0].lpValues[0];
			R = LYNX_REAL_ADD(ParticleR, LYNX_REAL_DIV(LYNX_REAL_MUL( LYNX_REAL(rand()%10), ParticleR), LYNX_REAL(10)));	//R = (rand() % 10)*ParticleR/10.0f+ParticleR;						
			if (bEmit)
			{					
				lynxCreateParticle(lpm, &V);				
			}						
		}	
	}

	lpP = lpm->lpParticleList;	
	while (lpP)
	{
		lpP->LifeTime = LYNX_REAL_ADD(lpP->LifeTime, skip);	
		lynxPlayParticle(lpm, lpP, skip, mode);

		if (lpP->LifeTime > lpP->LifeTimeLimit)
		{
			lpCP = lpP;
			lpP = lpP->lpNext;
			if (lpCP)
			{
				lynxDeleteParticle(lpm, lpCP);				
			}
		}
		else
		{
			lynxGetShortAnimKeyShortValues(&lpm->ParticleColorKey, lpP->LifeTime, PColor, 4);				
			lpP->Color.s.Red = PColor[0];
			lpP->Color.s.Green = PColor[1];
			lpP->Color.s.Blue = PColor[2];
			lpP->Color.s.Alpha = PColor[3];
			lynxGetAnimKeyValues(&lpm->ParticleSizeKey, lpP->LifeTime, &lpP->r, 1);

			switch (lpm->NoiseType)
			{
				case LYNX_PARTICLE_NOISE_NONE:
					LYNX_VECTOR3D_ADD(lpP->Vel, lpP->Vel, lpm->Gravity);					
					break;

				case LYNX_PARTICLE_NOISE_RANDOM:
					if (lpm->Noise.s.x >= LYNX_REAL(1))
						lpP->Vel.s.x = lpP->Vel0.s.x + lpP->LifeTime*lpm->Gravity.s.x + (lpm->Noise.s.x - (rand()%100)/(50.0f/lpm->Noise.s.x));
					else
						lpP->Vel.s.x = lpP->Vel0.s.x + lpP->LifeTime*lpm->Gravity.s.x;
					if (lpm->Noise.s.y >= LYNX_REAL(1))
						lpP->Vel.s.y = lpP->Vel0.s.y + lpP->LifeTime*lpm->Gravity.s.y + (lpm->Noise.s.y - (rand()%100)/(50.0f/lpm->Noise.s.y));
					else
						lpP->Vel.s.y = lpP->Vel0.s.y + lpP->LifeTime*lpm->Gravity.s.y;
					if (lpm->Noise.s.z >= LYNX_REAL(1))
						lpP->Vel.s.z = lpP->Vel0.s.z + lpP->LifeTime*lpm->Gravity.s.z + (lpm->Noise.s.z - (rand()%100)/(50.0f/lpm->Noise.s.z));			
					else
						lpP->Vel.s.z = lpP->Vel0.s.z + lpP->LifeTime*lpm->Gravity.s.z;			
					break;

				case LYNX_PARTICLE_NOISE_ZEGMA:
					LYNX_VECTOR3D_ADD(lpP->Vel, lpP->Vel, lpm->Gravity);										
					
					if (lpm->Noise.s.x >= 0.001f)
						lpP->Vel.s.x += (lpm->Noise.s.x - (rand()%100)/(50.0f/lpm->Noise.s.x));
					if (lpm->Noise.s.y >= 0.001f)
						lpP->Vel.s.y += (lpm->Noise.s.y - (rand()%100)/(50.0f/lpm->Noise.s.y));
					if (lpm->Noise.s.z >= 0.001f)
						lpP->Vel.s.z += (lpm->Noise.s.z - (rand()%100)/(50.0f/lpm->Noise.s.z));			
					break;
			}			
			LYNX_VECTOR3D_MUL(V, skip, lpP->Vel);
			LYNX_VECTOR3D_ADD(lpP->Pos, lpP->Pos, V);			
			lpP = lpP->lpNext;
		}
	}
	if (!lpm->lpParticleList)	
		bStop = TRUE;

	lpm->Time = LYNX_REAL_ADD(lpm->Time, skip);

	return bStop;
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxRenderParticleEmitter(LPLYNXPEMITTER lpm)
{
	LPLYNXPARTICLE						lpP;
	int									i, Step;
	LYNXREAL							Len, R;
	LPLYNXTEXSPRITE3D					lpSprite3D;
	LPLYNXCROSSTEXSPRITE				lpCrossSprt;
	LPLYNXMODEL							lpM;
	LYNXVECTOR3D						Vel, Pos, V;
	LPLYNXPEMITTER						lpPE;
	LYNXMATRIX							VM;

	lpP = lpm->lpParticleList;	

	switch (lpm->ParticleType)
	{
		case LYNX_PARTICLE_SPRITE3D:
			lynxGetViewMatrix(&VM);
			lynxSetSprite3DViewMatrix(&VM);
			lynxSetViewMatrix(NULL);
			while (lpP)
			{
				lpSprite3D = (LPLYNXTEXSPRITE3D)lpP->lpParticle;
				lynxSetTexSprite3DTimes(lpSprite3D, lpP->Sprite3DTime, lpP->Sprite3DTime, lpP->Sprite3DTime);
				//LYNX_IDENTITY_MATRIX(lpSprite3D->Obj3D.LocalM);
				//LYNX_IDENTITY_MATRIX(lpSprite3D->Obj3D.GlobalM);				
				LYNX_VECTOR3D_COPY((*lpSprite3D->Obj3D.lpCenter), lpP->Pos);				
				R = LYNX_REAL_MUL(lpP->r, LYNX_REAL(2));
				lynxSetTexSprite3DSize(lpSprite3D, R, LYNX_REAL_MUL(R, lpm->ParticleRatio));
				lynxSetTexSpriteColor(&lpSprite3D->TexSprite, &lpP->Color);
				
				if (lpP->DegX)
					lynxRenderTexSprite3DCenterRotate(lpSprite3D, lpP->DegX);		
				else					
					lpSprite3D->Obj3D.Render(lpSprite3D);
				
				
				//lynxRenderTexSprite3DCenter(lpSprite3D);
				//lynxSetVertexArray(&lpm->VertexArray, 0, sizeof(LYNXRENDERVERTEX));
				//D3DDEVICE_DrawPrimitive(lynxGetlpRender()->SystemData3D.lpD3DDevice, D3DPT_TRIANGLESTRIP, 0, 2);			

				if (lpm->bTrail)
				{
					LYNX_VECTOR3D_COPY(Vel, lpP->Vel);									
					Step = LYNX_REAL_ROUND_INT(lynxLength(&Vel, 3));
					//Step = LYNX_ROUND((Len*4));
					if (Step > lpm->MaxTrailStep)
						Step = lpm->MaxTrailStep;
					lynxNormalise(&Vel, 3);
					Len = LYNX_REAL_DIV(lpP->r, LYNX_REAL(5));
					LYNX_VECTOR3D_MUL(Vel, Len, Vel);		//Vel *= Len			

					for (i=1; i<=Step; i++)
					{
						LYNX_VECTOR3D_MUL(V, LYNX_REAL(i), Vel);
						LYNX_VECTOR3D_ADD((*lpSprite3D->Obj3D.lpCenter), (lpP->Pos), V); //lpSprite3D->Obj3D.lpCenter = lpP->Pos + Vel*i
						if (lpP->DegX)
							lynxRenderTexSprite3DCenterRotate(lpSprite3D, lpP->DegX);		
						else							
							lpSprite3D->Obj3D.Render(lpSprite3D);

						//lynxRenderTexSprite3DCenter(lpSprite3D);
						//lynxSetVertexArray(&lpm->VertexArray, 0, sizeof(LYNXRENDERVERTEX));
						//D3DDEVICE_DrawPrimitive(lynxGetlpRender()->SystemData3D.lpD3DDevice, D3DPT_TRIANGLESTRIP, 0, 2);	
					}
				}
				lpP = lpP->lpNext;
			}
			lynxSetViewMatrix(&VM);			
			break;

		case LYNX_PARTICLE_CROSS_SPRITE:
			while (lpP)
			{
				lpCrossSprt = (LPLYNXCROSSTEXSPRITE)lpP->lpParticle;
				lynxSetCrossTexSpriteTimes(lpCrossSprt, lpP->Sprite3DTime, lpP->Sprite3DTime, lpP->Sprite3DTime);
				lynxSetObject3DPosition(&lpCrossSprt->Obj3D, &lpP->Pos);				
				R = LYNX_REAL_MUL(lpP->r, LYNX_REAL(2));
				lynxSetCrossTexSpriteSize(lpCrossSprt, R, LYNX_REAL_MUL(R, lpm->ParticleRatio));
				lynxSetCrossTexSpriteColor(lpCrossSprt, &lpP->Color);
				lynxRenderCrossTexSpriteCenter(lpCrossSprt);		
				if (lpm->bTrail)
				{
					Vel.s.x = lpP->Vel.s.x; Vel.s.y = lpP->Vel.s.y; Vel.s.z = lpP->Vel.s.z;
					Step = LYNX_ROUND(lynxLength(&Vel, 3));
					//Step = LYNX_ROUND((Len*4));
					if (Step > lpm->MaxTrailStep)
						Step = lpm->MaxTrailStep;
					lynxNormalise(&Vel, 3);
					Len = LYNX_REAL_DIV(lpP->r, 5);
					Vel.s.x *= Len; Vel.s.y *= Len; Vel.s.z *= Len;

					for (i=1; i<=Step; i++)
					{
						lpCrossSprt->Obj3D.lpCenter->s.x = lpP->Pos.s.x+Vel.s.x*i; lpCrossSprt->Obj3D.lpCenter->s.y = lpP->Pos.s.y+Vel.s.y*i; lpCrossSprt->Obj3D.lpCenter->s.z = lpP->Pos.s.z+Vel.s.z*i;
						lynxRenderCrossTexSpriteCenter(lpCrossSprt);		
					}
				}
				lpP = lpP->lpNext;
			}
			break;

		case LYNX_PARTICLE_MODEL:
			while (lpP)
			{
				lpM = (LPLYNXMODEL)lpP->lpParticle;
				lynxSetModelTimes(lpM, lpP->Sprite3DTime, lpP->Sprite3DTime, lpP->Sprite3DTime);
				lynxSetModelPosition(lpM, &lpP->Pos);	
				lynxPlayModel(lpM, 0, TRUE);
				lynxUpdateModel(lpM);
				lpP = lpP->lpNext;
			}
			break;

		case LYNX_PARTICLE_PEMITTER:
			while (lpP)
			{
				lpPE = (LPLYNXPEMITTER)lpP->lpParticle;
				lynxSetObject3DPosition(&lpPE->Obj3D, &lpP->Pos);				
				lynxRenderParticleEmitter(lpPE);				
				lpP = lpP->lpNext;
			}
			break;
	}	
}
//-------------------------------------------------------------------------------------------------------
/**
 *  @ingroup LynxPEmitter 
 *  @brief 。
 *
 *	@param 。
 *	@param 。
 *	@param 。
 *	@return。 
 */
//-------------------------------------------------------------------------------------------------------
void lynxResetParticleEmitter(LPLYNXPEMITTER lpm)
{
	LPLYNXPARTICLE						lpP, lpCP;

	lpm->bEmit = TRUE;
	
	lpP = lpm->lpParticleList;	
	while (lpP)
	{
		lpCP = lpP;		
		lpP = lpP->lpNext;
		lynxDeleteParticle(lpm, lpCP);					
	}	
	/*
	lpP = lpm->lpDeadParticleList;	
	while (lpP)
	{
		lpCP = lpP;		
		lynxDeleteParticle(lpm, lpCP);			
		lpP = lpP->lpNext;
	}
	*/

	//lpm->lpCurrentDeadParticle = NULL;	
	lpm->lpCurrentParticle = NULL;		
	lpm->lpParticleList = NULL;
	lpm->NumEmitedParticle = 0;
	lpm->Time = lpm->EmitTime = LYNX_REAL(0);
}