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

#ifndef __LYNXTEXTURE_H__
#define __LYNXTEXTURE_H__

#include <LynxResource.h>
#include <png.h>

#ifdef __WIN32__
	#ifdef __FREEIMAGE__
		#include <FreeImage.h>
	#endif
#endif

namespace LynxEngine 
{
	#define DDS_FOURCC 0x00000004  // DDPF_FOURCC
	#define DDS_RGB    0x00000040  // DDPF_RGB
	#define DDS_RGBA   0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS

	#define DDS_HEADER_FLAGS_TEXTURE    0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
	#define DDS_HEADER_FLAGS_MIPMAP     0x00020000  // DDSD_MIPMAPCOUNT
	#define DDS_HEADER_FLAGS_VOLUME     0x00800000  // DDSD_DEPTH
	#define DDS_HEADER_FLAGS_PITCH      0x00000008  // DDSD_PITCH
	#define DDS_HEADER_FLAGS_LINEARSIZE 0x00080000  // DDSD_LINEARSIZE

	#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
	#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
	#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

	#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
	#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
	#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
	#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
	#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
	#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

	#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
								   DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
								   DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

	#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME

	typedef struct {
		DWORD					dwSize;
		DWORD					dwFlags;
		DWORD					dwFourCC;
		DWORD					dwRGBBitCount;
		DWORD					dwRBitMask;
		DWORD					dwGBitMask;
		DWORD					dwBBitMask;
		DWORD					dwABitMask;
	}DDSPIXELFORMAT;

	typedef struct {
		DWORD					dwSize;
		DWORD					dwHeaderFlags;
		DWORD					dwHeight;
		DWORD					dwWidth;
		DWORD					dwPitchOrLinearSize;
		DWORD					dwDepth; // only if DDS_HEADER_FLAGS_VOLUME is set in dwHeaderFlags
		DWORD					dwMipMapCount;
		DWORD					dwReserved1[11];
		DDSPIXELFORMAT			ddspf;
		DWORD					dwSurfaceFlags;
		DWORD					dwCubemapFlags;
		DWORD					dwReserved2[3];
	}DDSHEADER;

	typedef struct {
	   unsigned char			idlen;    //length of optional identification sequence
	   unsigned char			cmtype;   //indicates whether a palette is present
	   unsigned char			imtype;   //image data type (e.g., uncompressed RGB)
	   //unsigned short			cmorg;    //first palette index, if present
	   //unsigned short			cmorg2;    //first palette index, if present
	   unsigned short			cmcnt;    //number of palette entries, if present
	   unsigned char			cmsize;   //number of bits per palette entry
	   unsigned short			imxorg;   //horiz pixel coordinate of lower left of image
	   unsigned short			imyorg;   //vert pixel coordinate of lower left of image
	   unsigned short			imwidth;  //image width in pixels
	   unsigned short			imheight; //image height in pixels
	   unsigned char			imdepth;  //image color depth (bits per pixel)
	   unsigned char			imdesc;   //image attribute flags
	}TGAHEADER;

	typedef struct _PVRTexHeader
	{
		DWORD headerLength;
		DWORD height;
		DWORD width;
		DWORD numMipmaps;
		DWORD flags;
		DWORD dataLength;
		DWORD bpp;
		DWORD bitmaskRed;
		DWORD bitmaskGreen;
		DWORD bitmaskBlue;
		DWORD bitmaskAlpha;
		DWORD pvrTag;
		DWORD numSurfs;
	}PVRHEADER;
	
	namespace GraphicsSystem 
	{		
		class LYNXENGCLASS CTexture : public CResource
		{
			LYNX_DECLARE_CLASS(CTexture);
		public:			
			DECLAR_PARAM_INT(m_MipMapLevel)
			DECLAR_PARAM_INT(m_LODBias)
			DECLAR_PARAM_BOOL(m_bSRGB)		//Do the sRGB reading or not.
			DECLAR_PARAM_FLOAT(m_AlphaReference)
			LYNXBOOL						m_bDelayLoading;	
			LYNXBOOL						m_bLoaded;
		protected:
			CGraphicsSystem*				m_lpGraphicsSystem;
			CRenderer::TEXTURETYPE			m_TextureType;
			CBufferPtr						m_lpSrcBuffer;
			CSamplerStatePtr				m_SamplerState;	
			CString							m_FullPath;
			CString							m_Ext;
		public:						
			CTexture(CGraphicsSystem* lpsys);
			virtual ~CTexture();	
			
			//LYNXINLINE CTexture&			operator =(const CTexture& t) {m_lpGraphicsSystem = t.m_lpGraphicsSystem; m_lpInfo = t.m_lpInfo; lynxDuplicateTexture(&m_Index, t.m_Index); return (*this); };		
			//LYNXINLINE CTexture&			operator =(const LYNXTEXTUREINDEX t) {SetIndex(t); return (*this); };		

			virtual LYNXBOOL				vIsValid() = 0;
			
			void							SetlpGraphicsSystem(CGraphicsSystem* pgs) {m_lpGraphicsSystem = pgs; };
			void							CreateSamplerState(const CRenderer::SAMPLERSTATEDESC& desc);
			CSamplerStatePtr&				GetSamplerState() {return m_SamplerState; };
			CRenderer::FORMAT				GetFormat();
			LYNXFORCEINLINE CRenderer::TEXTURETYPE	GetTextureType() const {return m_TextureType; };
			LYNXFORCEINLINE CBuffer*		GetSrcBuffer() {return m_lpSrcBuffer; };

			virtual LYNXBOOL				vLoad(CStream& fs, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc = NULL) = 0;
			virtual LYNXBOOL				vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format) = 0;
			virtual LYNXBOOL				vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable = LYNX_FALSE) = 0;
            virtual void                    vUpdate(const int level, const void* data) = 0;
			virtual void					vSet(int channel) = 0;
			virtual CBuffer*				vSetAsRT(int channel) = 0;			
			virtual int						vGetWidth() = 0;
			virtual int						vGetHeight() = 0;
			virtual int						vGetDepth() = 0;
			virtual WORD					vGetMipMapLevels() = 0;					
			virtual void					vGetLevel(int face, int level) = 0;				
		};

	#ifdef __WIN32__
		#ifdef __FREEIMAGE__
			unsigned DLL_CALLCONV FreeImage_ReadFileProc(void *buffer, unsigned size, unsigned count, fi_handle handle);
			unsigned DLL_CALLCONV FreeImage_WriteFileProc(void *buffer, unsigned size, unsigned count, fi_handle handle);
			int DLL_CALLCONV FreeImage_SeekFileProc(fi_handle handle, long offset, int origin);
			long DLL_CALLCONV FreeImage_TellFileProc(fi_handle handle);
		#endif
	#endif

		png_voidp PNGMalloc(png_structp png_ptr, png_size_t size);
		void PNGFree(png_structp png_ptr, png_voidp ptr);
		void PNGError(png_structp png_ptr, png_const_charp msg);
		void PNGWarning(png_structp png_ptr, png_const_charp msg);
		void PNGRead(png_structp png_ptr, png_bytep data, png_size_t length);
	}		
}

#endif
