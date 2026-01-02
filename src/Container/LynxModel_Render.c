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
/********************************************************************************************************
//
//  說明:   
//*******************************************************************************************************/
void	ilynxOpenGLESRenderModelNormal(void *lpsm)
{
	int					i, j, o;
	int					PV1, PV2, PV3;
	int					TV1, TV2, TV3;
	int					CV1, CV2, CV3;
	LPLYNXMODEL			lpM;
	LPLYNXMESH			lpMesh;
	BYTE				Alpha;

	lpM = (LPLYNXMODEL)lpsm;	
	#ifdef __USE_VBO__	
		#ifdef __FIXED_POINT__		
			if (!strstr(lpM->Mesh.Obj3D.Name, "camera") && !strstr(lpM->Mesh.Obj3D.Name, "bullet"))
			{
				glBindBuffer(GL_ARRAY_BUFFER, lpM->VertexBuffer);
				glVertexPointer (3, GL_FIXED, 0, NULL);			
				glBindBuffer(GL_ARRAY_BUFFER, lpM->UVBuffer);
				glTexCoordPointer(2, GL_FIXED, 0, NULL);		
				glBindBuffer(GL_ARRAY_BUFFER, lpM->ColorBuffer);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, NULL);										
			}
			else
			{
				#ifdef __FIXED_POINT__
					glVertexPointer (3, GL_FIXED, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0]);	
					glTexCoordPointer(2, GL_FIXED, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0].TexCoord[0]);
				#else
					glVertexPointer (3, GL_FLOAT, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0]);	
					glTexCoordPointer(2, GL_FLOAT, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0].TexCoord[0]);
				#endif
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0].color);	    			
			}
		#endif
	#else
		#ifdef __FIXED_POINT__
			glVertexPointer (3, GL_FIXED, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0]);	
			glTexCoordPointer(2, GL_FIXED, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0].TexCoord[0]);
		#else
			glVertexPointer (3, GL_FLOAT, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0]);	
			glTexCoordPointer(2, GL_FLOAT, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0].TexCoord[0]);
		#endif
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(LYNXRENDERVERTEX), (void *)&lpM->lpTVertexList[0][0].color);	    			
	#endif

	for (o=0; o<lpM->NumMeshs; o++)
	{
		lpMesh = &lpM->lpMeshs[o];

		lynxSetMaterialPass(&lpM->lpMaterials[lpMesh->MaterialIndex].lpMaterialPass[0]);
		lynxSetWorldMatrix(&lpMesh->Obj3D.M);		
		if (lpMesh->Obj3D.Visibility)
		{
			glDrawElements(GL_TRIANGLES, lpMesh->NumTriangles[0]*3, GL_UNSIGNED_SHORT, &(((WORD *)(lpM->IndexArray.lpBuffer))[lpMesh->BaseTriangle[0]*3]));    			
		}
	}

	#ifdef __FIXED_POINT__
		#ifdef __USE_VBO__	
			glBindBuffer(GL_ARRAY_BUFFER, 0);	
		#endif
	#endif	
}
#endif	

