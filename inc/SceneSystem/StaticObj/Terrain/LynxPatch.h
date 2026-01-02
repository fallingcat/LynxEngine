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

#ifndef __LYNXPATCH_H__
#define __LYNXPATCH_H__

#include <LynxRenderableObj.h>
#include <LynxSystem.h>

//
// Patch Class
// Store information needed at the Patch level
//
namespace LynxEngine 
{
	namespace SceneSystem 
	{
		// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
		#define MAX_VARIANCE_DEPTH						9

		//
		// TriTreeNode Struct
		// Store the triangle tree data, but no coordinates!
		//
		struct TriTreeNode
		{
			TriTreeNode *LeftChild;
			TriTreeNode *RightChild;
			TriTreeNode *BaseNeighbor;
			TriTreeNode *LeftNeighbor;
			TriTreeNode *RightNeighbor;
		};
		class LYNXENGCLASS CTerrainPatch : public CRenderableObj
		{
			LYNX_DECLARE_CLASS(CTerrainPatch);
		protected:
			CRenderableObj*									m_lpTerrain;
			unsigned char*									m_HeightMap;							// Pointer to height map to use
			int												m_WorldX, m_WorldZ;						// World coordinate offset of this patch.
			int												m_ToleranceX, m_ToleranceZ;

			unsigned char									m_VarianceLeft[ 1<<(MAX_VARIANCE_DEPTH)];	// Left variance tree
			unsigned char									m_VarianceRight[1<<(MAX_VARIANCE_DEPTH)];	// Right variance tree

			unsigned char*									m_CurrentVariance;						// Which varience we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
			unsigned char									m_VarianceDirty;						// Does the Varience Tree need to be recalculated for this Patch?
			unsigned char									m_isVisible;							// Is this patch visible in the current frame?

			TriTreeNode										m_BaseLeft;								// Left base triangle tree node
			TriTreeNode										m_BaseRight;							// Right base triangle tree node

		public:				
			CTerrainPatch();
			CTerrainPatch(CRenderableObj* lpt);
			virtual ~CTerrainPatch();

			LYNXINLINE void									SetlpTerrain(CRenderableObj* lpt) {m_lpTerrain = lpt; };
			TriTreeNode*									GetBaseLeft()  { return &m_BaseLeft; }
			TriTreeNode*									GetBaseRight() { return &m_BaseRight; }
			char											isDirty()     { return m_VarianceDirty; }
			int												isVisibile( ) { return m_isVisible; }
			void											SetVisibility( int eyeX, int eyeY, int leftX, int leftY, int rightX, int rightY );
			void											GetTolerance(int *x, int *z) { *x = m_ToleranceX; *z = m_ToleranceZ;};
			void											SetTolerance(int x, int z) {m_ToleranceX = x; m_ToleranceZ = z;};

			
			virtual void									Init( int heightX, int heightY, int worldX, int worldY, unsigned char *hMap );
			virtual void									Reset();
			virtual void									Tessellate();
			virtual void									Render();
			virtual void									ComputeVariance();

			virtual void									Split( TriTreeNode *tri);
			virtual void									RecursTessellate( TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY, int node );
			virtual void									RecursRender( TriTreeNode *tri, int rightX, int rightZ, int leftX, int leftY, int apexX, int apexY );
			virtual unsigned char							RecursComputeVariance(	int leftX,  int leftY,  unsigned char leftZ,
																					int rightX, int rightY, unsigned char rightZ,
																					int apexX,  int apexY,  unsigned char apexZ,
																					int node);
		};
	}
}

#endif
