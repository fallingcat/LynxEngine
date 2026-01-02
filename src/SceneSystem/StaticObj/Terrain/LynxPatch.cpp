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

#include <SceneSystem/StaticObj/Terrain/LynxPatch.h>
#include <SceneSystem/StaticObj/Terrain/LynxTerrain.h>

namespace LynxEngine 
{
	namespace SceneSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   				
		//-------------------------------------------------------------------------------------------------------
		CTerrainPatch::CTerrainPatch()
		{
			m_lpTerrain = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   				
		//-------------------------------------------------------------------------------------------------------
		CTerrainPatch::CTerrainPatch(CRenderableObj* lpt)
		{
			m_lpTerrain= lpt;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   				
		//-------------------------------------------------------------------------------------------------------
		CTerrainPatch::~CTerrainPatch()
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   		
		//		Split a single Triangle and link it into the mesh.
		//		Will correctly force-split diamonds.
		//-------------------------------------------------------------------------------------------------------
		void CTerrainPatch::Split(TriTreeNode *tri)
		{
			// We are already split, no need to do it again.
			if (tri->LeftChild)
				return;

			// If this triangle is not in a proper diamond, force split our base neighbor
			if ( tri->BaseNeighbor && (tri->BaseNeighbor->BaseNeighbor != tri) )
				Split(tri->BaseNeighbor);

			// Create children and link into mesh
			tri->LeftChild  = ((CTerrain*)m_lpTerrain)->AllocateTri();
			tri->RightChild = ((CTerrain*)m_lpTerrain)->AllocateTri();

			// If creation failed, just exit.
			if ( !tri->LeftChild )
				return;

			// Fill in the information we can get from the parent (neighbor pointers)
			tri->LeftChild->BaseNeighbor  = tri->LeftNeighbor;
			tri->LeftChild->LeftNeighbor  = tri->RightChild;

			tri->RightChild->BaseNeighbor  = tri->RightNeighbor;
			tri->RightChild->RightNeighbor = tri->LeftChild;

			// Link our Left Neighbor to the LYNXNEW children
			if (tri->LeftNeighbor != NULL)
			{
				if (tri->LeftNeighbor->BaseNeighbor == tri)
					tri->LeftNeighbor->BaseNeighbor = tri->LeftChild;
				else if (tri->LeftNeighbor->LeftNeighbor == tri)
					tri->LeftNeighbor->LeftNeighbor = tri->LeftChild;
				else if (tri->LeftNeighbor->RightNeighbor == tri)
					tri->LeftNeighbor->RightNeighbor = tri->LeftChild;
				else
					;// Illegal Left Neighbor!
			}

			// Link our Right Neighbor to the LYNXNEW children
			if (tri->RightNeighbor != NULL)
			{
				if (tri->RightNeighbor->BaseNeighbor == tri)
					tri->RightNeighbor->BaseNeighbor = tri->RightChild;
				else if (tri->RightNeighbor->RightNeighbor == tri)
					tri->RightNeighbor->RightNeighbor = tri->RightChild;
				else if (tri->RightNeighbor->LeftNeighbor == tri)
					tri->RightNeighbor->LeftNeighbor = tri->RightChild;
				else
					;// Illegal Right Neighbor!
			}

			// Link our Base Neighbor to the LYNXNEW children
			if (tri->BaseNeighbor != NULL)
			{
				if ( tri->BaseNeighbor->LeftChild )
				{
					tri->BaseNeighbor->LeftChild->RightNeighbor = tri->RightChild;
					tri->BaseNeighbor->RightChild->LeftNeighbor = tri->LeftChild;
					tri->LeftChild->RightNeighbor = tri->BaseNeighbor->RightChild;
					tri->RightChild->LeftNeighbor = tri->BaseNeighbor->LeftChild;
				}
				else
					Split( tri->BaseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
			}
			else
			{
				// An edge triangle, trivial case.
				tri->LeftChild->RightNeighbor = NULL;
				tri->RightChild->LeftNeighbor = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Tessellate a Patch.
		//		Will continue to split until the variance metric is met.
		//-------------------------------------------------------------------------------------------------------
		void CTerrainPatch::RecursTessellate( TriTreeNode *tri,
									int leftX,  int leftZ,
									int rightX, int rightZ,
									int apexX,  int apexZ,
									int node )
		{
			float					TriVariance;
			int						centerX, centerZ;


			if (((CTerrain*)m_lpTerrain)->m_lpViewPosition)
			{				
				centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
				centerZ = (leftZ + rightZ)>>1; // Compute Z coord...

				if (m_lpTerrain)
				{
					if ( node < (1<<MAX_VARIANCE_DEPTH) )
					{
						// Extremely slow distance metric (sqrt is used).
						// Replace this with a faster one!
						float distance = 1.0f + sqrtf( SQR((float)centerX - ((CTerrain*)m_lpTerrain)->m_lpViewPosition->s.x) +
													SQR((float)centerZ - ((CTerrain*)m_lpTerrain)->m_lpViewPosition->s.z) );
						
						// Egads!  A division too?  What's this world coming to!
						// This should also be replaced with a faster operation.
						TriVariance = ((float)m_CurrentVariance[node] * ((CTerrain*)m_lpTerrain)->m_SizeX * 2)/distance;	// Take both distance and variance into consideration
					}
				}

				if ( (node >= (1<<MAX_VARIANCE_DEPTH)) ||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
					(TriVariance > ((CTerrain*)m_lpTerrain)->m_FrameVariance))	// OR if we are not below the variance tree, test for variance.									
				{
					Split(tri);														// Split this triangle.
					
					if (tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
						((abs(leftX - rightX) >= 3) || (abs(leftZ - rightZ) >= 3)))	// Tessellate all the way down to one vertex per height field entry
					{
						RecursTessellate( tri->LeftChild,   apexX,  apexZ, leftX, leftZ, centerX, centerZ,    node<<1  );
						RecursTessellate( tri->RightChild, rightX, rightZ, apexX, apexZ, centerX, centerZ, 1+(node<<1) );
					}				
				}
			}
			else
			{
				centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
				centerZ = (leftZ + rightZ)>>1; // Compute Z coord...

				Split(tri);														// Split this triangle.
					
				if (tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
					((abs(leftX - rightX) >= m_ToleranceX) || (abs(leftZ - rightZ) >= m_ToleranceZ)))	// Tessellate all the way down to one vertex per height field entry
				{
					RecursTessellate( tri->LeftChild,   apexX,  apexZ, leftX, leftZ, centerX, centerZ,    node<<1  );
					RecursTessellate( tri->RightChild, rightX, rightZ, apexX, apexZ, centerX, centerZ, 1+(node<<1) );
				}				
			}						
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Render the tree.  Simple no-fan method.
		//-------------------------------------------------------------------------------------------------------		
		void CTerrainPatch::RecursRender( TriTreeNode *tri, int rightX, int rightZ, int leftX, int leftZ, int apexX, int apexZ )
		{
			#ifdef __OPENGL__

			float v[3][3];
			float out[3];
			LYNXUVPOINT		uv;
			static float			Tile = 3.0f;

			if ( tri->LeftChild )					// All non-leaf nodes have both children, so just check for one
			{
				int centerX = (leftX + rightX)>>1;	// Compute X coordinate of center of Hypotenuse
				int centerZ = (leftZ + rightZ)>>1;	// Compute Z coord...

				RecursRender( tri->LeftChild,  leftX, leftZ, apexX,   apexZ, centerX, centerZ );
				RecursRender( tri->RightChild, apexX, apexZ, rightX, rightZ, centerX, centerZ );
			}
			else									// A leaf node!  Output a triangle to be rendered.
			{
				// Actual number of rendered triangles...
				((CTerrain *)m_lpTerrain)->m_NumTrisRendered++;

				GLfloat leftY  = m_HeightMap[(leftZ *((CTerrain*)m_lpTerrain)->m_SizeX)+leftX ];
				GLfloat rightY = m_HeightMap[(rightZ*((CTerrain*)m_lpTerrain)->m_SizeX)+rightX];
				GLfloat apexY  = m_HeightMap[(apexZ *((CTerrain*)m_lpTerrain)->m_SizeX)+apexX ];						
					
				uv.u = (float)(m_WorldX+rightX)*Tile/(float)1024.0f;
				uv.v = (float)(m_WorldZ+rightZ)*Tile/(float)1024.0f;
				glTexCoord2f(uv.u, uv.v);					

				float fColor = (60.0f + rightY) / 256.0f;
				if ( fColor > 1.0f )  
					fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );					
				
				/*
				float fColor = -(out[0]*0.57f-out[1]*0.57f+out[2]*0.57f);
				if (fColor < 0.2f)
					fColor = 0.2f;
				else if (fColor > 1.0f)
					fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );
				*/					
				// Output the RIGHT VERTEX for the triangle
				glVertex3f(		(GLfloat) rightX,
								(GLfloat) rightY,				
								(GLfloat) rightZ );
				

				uv.u = (float)(m_WorldX+apexX)*Tile/(float)1024.0f;
				uv.v = (float)(m_WorldZ+apexZ)*Tile/(float)1024.0f;
				glTexCoord2f(uv.u, uv.v);

				fColor = (60.0f + apexY) / 256.0f;
				if ( fColor > 1.0f )  
					fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );					

				/*
				float fColor = -(out[0]*0.57f-out[1]*0.57f+out[2]*0.57f);
				if (fColor < 0.2f)
					fColor = 0.2f;
				else if (fColor > 1.0f)
					fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );
				*/									
				// Output the APEX VERTEX for the triangle
				glVertex3f(		(GLfloat) apexX,
								(GLfloat) apexY,
								(GLfloat) apexZ );				


				uv.u = (float)(m_WorldX+leftX)*Tile/(float)1024.0f;
				uv.v = (float)(m_WorldZ+leftZ)*Tile/(float)1024.0f;
				glTexCoord2f(uv.u, uv.v);

				fColor = (60.0f + leftY) / 256.0f;
				if ( fColor > 1.0f )  
					fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );					

				/*
				float fColor = -(out[0]*0.57f-out[1]*0.57f+out[2]*0.57f);
				if (fColor < 0.2f)
					fColor = 0.2f;
				else if (fColor > 1.0f)
					fColor = 1.0f;
				glColor3f( fColor, fColor, fColor );
				*/									
				// Output the LEFT VERTEX for the triangle
				glVertex3f(		(GLfloat) leftX,
								(GLfloat) leftY,
								(GLfloat) leftZ );				
				
			}
			#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Computes Variance over the entire tree.  Does not examine node relationships.
		//-------------------------------------------------------------------------------------------------------		
		unsigned char CTerrainPatch::RecursComputeVariance( int leftX,  int leftY,  unsigned char leftZ,
															int rightX, int rightY, unsigned char rightZ,
															int apexX,  int apexY,  unsigned char apexZ,
															int node)
		{
			//        /|\
			//      /  |  \
			//    /    |    \
			//  /      |      \
			//  ~~~~~~~*~~~~~~~  <-- Compute the X and Y coordinates of '*'
			//
			int centerX = (leftX + rightX) >>1;		// Compute X coordinate of center of Hypotenuse
			int centerZ = (leftZ + rightZ) >>1;		// Compute Y coord...
			unsigned char myVariance;

			// Get the height value at the middle of the Hypotenuse
			unsigned char centerY  = m_HeightMap[(centerZ * ((CTerrain*)m_lpTerrain)->m_SizeX) + centerX];

			// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
			// Use values passed on the stack instead of re-accessing the Height Field.
			myVariance = abs((int)centerY - (((int)leftY + (int)rightY)>>1));

			// Since we're after speed and not perfect representations,
			//    only calculate variance down to an 8x8 block
			if ( (abs(leftX - rightX) >= 8) ||
				(abs(leftZ - rightZ) >= 8) )
			{
				// Final Variance for this node is the max of it's own variance and that of it's children.
				myVariance = MAX( myVariance, RecursComputeVariance( apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    node<<1 ) );
				myVariance = MAX( myVariance, RecursComputeVariance( rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(node<<1)) );
			}

			// Store the final variance for this node.  Note Variance is never zero.
			if (node < (1<<MAX_VARIANCE_DEPTH))
				m_CurrentVariance[node] = 1 + myVariance;

			return myVariance;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Initialize a patch.
		//-------------------------------------------------------------------------------------------------------		
		void CTerrainPatch::Init( int heightX, int heightZ, int worldX, int worldZ, unsigned char *hMap )
		{
			// Clear all the relationships
			m_BaseLeft.RightNeighbor = m_BaseLeft.LeftNeighbor = m_BaseRight.RightNeighbor = m_BaseRight.LeftNeighbor =
			m_BaseLeft.LeftChild = m_BaseLeft.RightChild = m_BaseRight.LeftChild = m_BaseLeft.LeftChild = NULL;

			// Attach the two m_Base triangles together
			m_BaseLeft.BaseNeighbor = &m_BaseRight;
			m_BaseRight.BaseNeighbor = &m_BaseLeft;

			// Store Patch offsets for the world and heightmap.
			m_WorldX = worldX;
			m_WorldZ = worldZ;

			m_ToleranceX = 32;
			m_ToleranceZ = 32;

			// Store pointer to first byte of the height data for this patch.
			m_HeightMap = &hMap[heightZ * ((CTerrain*)m_lpTerrain)->m_SizeX + heightX];

			// Initialize flags
			m_VarianceDirty = 1;
			m_isVisible = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Reset the patch.
		//-------------------------------------------------------------------------------------------------------		
		void CTerrainPatch::Reset()
		{
			// Assume patch is not visible.
			m_isVisible = 0;

			// Reset the important relationships
			m_BaseLeft.LeftChild = m_BaseLeft.RightChild = m_BaseRight.LeftChild = m_BaseLeft.LeftChild = NULL;

			// Attach the two m_Base triangles together
			m_BaseLeft.BaseNeighbor = &m_BaseRight;
			m_BaseRight.BaseNeighbor = &m_BaseLeft;

			// Clear the other relationships.
			m_BaseLeft.RightNeighbor = m_BaseLeft.LeftNeighbor = m_BaseRight.RightNeighbor = m_BaseRight.LeftNeighbor = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Compute the variance tree for each of the Binary Triangles in this patch.
		//-------------------------------------------------------------------------------------------------------		
		void CTerrainPatch::ComputeVariance()
		{
			// Compute variance on each of the base triangles...
			m_CurrentVariance = m_VarianceLeft;
			RecursComputeVariance(	0,										m_HeightMap[((CTerrain*)m_lpTerrain)->m_PatchSizeZ * ((CTerrain*)m_lpTerrain)->m_SizeX], ((CTerrain*)m_lpTerrain)->m_PatchSizeZ,
									((CTerrain*)m_lpTerrain)->m_PatchSizeX, m_HeightMap[((CTerrain*)m_lpTerrain)->m_PatchSizeZ],									0, 
									0,										m_HeightMap[0],																			0,
									1);

			m_CurrentVariance = m_VarianceRight;
			RecursComputeVariance(	((CTerrain*)m_lpTerrain)->m_PatchSizeX,	m_HeightMap[((CTerrain*)m_lpTerrain)->m_PatchSizeZ],																				0,          
									0,										m_HeightMap[((CTerrain*)m_lpTerrain)->m_PatchSizeZ * ((CTerrain*)m_lpTerrain)->m_SizeX],											((CTerrain*)m_lpTerrain)->m_PatchSizeZ, 
									((CTerrain*)m_lpTerrain)->m_PatchSizeX, m_HeightMap[(((CTerrain*)m_lpTerrain)->m_PatchSizeZ * ((CTerrain*)m_lpTerrain)->m_SizeX) + ((CTerrain*)m_lpTerrain)->m_PatchSizeX],	((CTerrain*)m_lpTerrain)->m_PatchSizeZ, 
									1);

			// Clear the dirty flag for this patch
			m_VarianceDirty = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Discover the orientation of a triangle's points:		
		//		Taken from "Programming Principles in Computer Graphics", L. Ammeraal (Wiley)
		//-------------------------------------------------------------------------------------------------------		
		inline int orientation( int pX, int pZ, int qX, int qZ, int rX, int rZ )
		{
			int aX, aZ, bX, bZ;
			float d;

			aX = qX - pX;
			aZ = qZ - pZ;

			bX = rX - pX;
			bZ = rZ - pZ;

			d = (float)aX * (float)bZ - (float)aZ * (float)bX;
			return (d < 0) ? (-1) : (d > 0);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Set patch's visibility flag.
		//-------------------------------------------------------------------------------------------------------		
		void CTerrainPatch::SetVisibility( int eyeX, int eyeZ, int leftX, int leftZ, int rightX, int rightZ )
		{
			// Get patch's center point
			int patchCenterX = m_WorldX + ((CTerrain*)m_lpTerrain)->m_PatchSizeX / 2;
			int patchCenterZ = m_WorldZ + ((CTerrain*)m_lpTerrain)->m_PatchSizeZ / 2;
			
			// Set visibility flag (orientation of both triangles must be counter clockwise)
			m_isVisible = (orientation( eyeX,  eyeZ,  rightX, rightZ, patchCenterX, patchCenterZ ) < 0) &&
						  (orientation( leftX, leftZ, eyeX,   eyeZ,   patchCenterX, patchCenterZ ) < 0);

			m_isVisible = 1;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Create an approximate mesh.
		//-------------------------------------------------------------------------------------------------------		
		void CTerrainPatch::Tessellate()
		{
			// Split each of the base triangles
			m_CurrentVariance = m_VarianceLeft;
			RecursTessellate (	&m_BaseLeft,
								m_WorldX,											m_WorldZ+((CTerrain*)m_lpTerrain)->m_PatchSizeZ,
								m_WorldX+((CTerrain*)m_lpTerrain)->m_PatchSizeX,	m_WorldZ,
								m_WorldX,											m_WorldZ,
								1 );
							
			m_CurrentVariance = m_VarianceRight;
			RecursTessellate(	&m_BaseRight,
								m_WorldX+((CTerrain*)m_lpTerrain)->m_PatchSizeX,	m_WorldZ,
								m_WorldX,											m_WorldZ+((CTerrain*)m_lpTerrain)->m_PatchSizeZ,
								m_WorldX+((CTerrain*)m_lpTerrain)->m_PatchSizeX,	m_WorldZ+((CTerrain*)m_lpTerrain)->m_PatchSizeZ,
								1 );
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//		Render the mesh.
		//-------------------------------------------------------------------------------------------------------		
		void CTerrainPatch::Render()
		{			
			#ifdef __OPENGL__

			LYNXMATRIX			M;

			LYNX_IDENTITY_MATRIX(M);
			//M.s.m11 = 2.5f;
			//M.s.m33 = 2.5f;
			M.s.m22 = 0.7f;
			M.s.m41 = (GLfloat)m_WorldX*M.s.m11;
			M.s.m43 = (GLfloat)m_WorldZ*M.s.m33;
			lynxSetWorldMatrix(&M);

			glBegin(GL_TRIANGLES);				
				
				RecursRender(&m_BaseLeft,					
					((CTerrain*)m_lpTerrain)->m_PatchSizeX,		0,
					0,											((CTerrain*)m_lpTerrain)->m_PatchSizeZ,
					0,											0);
				
				RecursRender(&m_BaseRight,					
					0,										((CTerrain*)m_lpTerrain)->m_PatchSizeZ,
					((CTerrain*)m_lpTerrain)->m_PatchSizeX,	0,
					((CTerrain*)m_lpTerrain)->m_PatchSizeX,	((CTerrain*)m_lpTerrain)->m_PatchSizeZ);
			
			glEnd();			

			#endif
		}
	}
}
