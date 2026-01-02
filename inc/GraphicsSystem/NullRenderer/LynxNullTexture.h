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

#ifndef __LYNXNULLTEXTURE_H__
#define __LYNXNULLTEXTURE_H__

#include <GraphicsSystem/LynxTexture.h>
#include <GraphicsSystem/NullRenderer/LynxNullBuffer.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CNullTexture : public CTexture
		{
			LYNX_DECLARE_CLASS(CNullTexture);
		protected:			
			CNullBuffer*					m_lpSrcBuffer;
		public:						
			CNullTexture(CGraphicsSystem* lpsys):CTexture(lpsys) {m_lpSrcBuffer = LYNXNEW CNullBuffer(lpsys); };
			virtual ~CNullTexture() {LYNXDEL m_lpSrcBuffer; };	
						
			LYNXINLINE LYNXBOOL				vIsValid() {return LYNX_TRUE; };
			LYNXBOOL						vLoad(CStream& stream, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL) {SetName(filename); return LYNX_TRUE; };

			LYNXBOOL						vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format) {SetName(name); return LYNX_TRUE; };
			LYNXBOOL						vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable = LYNX_FALSE) {SetName(name); return LYNX_TRUE; };
			LYNXINLINE void					vSet(int channel) {};
			CBuffer*						vSetAsRT(int channel) {return NULL; };			
			LYNXINLINE int					vGetWidth() {return 0; };
			LYNXINLINE int					vGetHeight() {return 0; };
			LYNXINLINE int					vGetDepth() {return 0; };
			LYNXINLINE WORD					vGetMipMapLevels() { return 0; };
			//LYNXINLINE LYNXTEXTURETYPE		GetType() { return m_lpInfo->Type; };
			LYNXINLINE void					vGetLevel(int face, int level) { };
			LYNXINLINE void                 vUpdate(const int level, const void* data) {};
		};
	}		
}

#endif