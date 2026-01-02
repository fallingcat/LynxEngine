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

#ifndef __LYNXSCNTYPE_H__
#define __LYNXSCNTYPE_H__

namespace LynxEngine
{
	namespace SceneSystem 
	{
		typedef struct LightmapData{
				char				TexName[64];
		}LIGHTMAPDATA , *LPLIGHTMAPDATA;

		typedef struct TriangleFanData{
				DWORD				NumVerts;
				WORD				*lpv;
				LYNXTEXTUREINDEX	TextureIndex;
		}TRIANGLEFANDATA , *LPTRIANGLEFANDATA;
	}
}
#endif
