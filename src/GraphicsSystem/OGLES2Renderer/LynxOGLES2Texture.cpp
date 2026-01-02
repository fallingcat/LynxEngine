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

#include <LynxEngine_PCH.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES2__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Renderer.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Texture.h>
#include <GraphicsSystem/OGLES2Renderer/LynxOGLES2Buffer.h>
#include <pnginfo.h>
#ifdef __ANDROID__
    #include <ktx.h>
#endif

#ifdef __WIN32__
	//#if !defined uint32_t
		//typedef DWORD					uint32_t;
	//#endif
#endif

#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff

static char gPVRTexIdentifier[8] = "PVR!";

#define KTX_OPENGL_ES2 1

enum
{
	kPVRTextureFlagTypePVRTC_2 = 24,
	kPVRTextureFlagTypePVRTC_4
};

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2Texture::COGLES2Texture(CGraphicsSystem *pgs)
		: CTexture(pgs)
		{	
			m_GLHandle = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES2Texture::~COGLES2Texture(void)
		{
			if (m_GLHandle)
			{
				glDeleteTextures(1, &m_GLHandle);
				m_GLHandle = 0;
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Texture::vGetLevel(int face, int level)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::LoadTextureBMP(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
		{
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Texture::LoadPVR(CStream& fs, PVRHEADER* lpheader)
		{
			uint32_t DataLength = 0, DataOffset = 0, DataSize = 0;
			uint32_t BlockSize = 0, WidthBlocks = 0, HeightBlocks = 0;
			uint32_t Width = 0, Height = 0, bpp = 4, Level;
			char* lpData = NULL;
		
			uint32_t FormatFlags = lpheader->flags & PVR_TEXTURE_FLAG_TYPE_MASK;
			DataLength = lpheader->dataLength;
			
			Width = lpheader->width;
			Height = lpheader->height;
			Level = 0;
			
			// Calculate the data size for each texture level and respect the minimum number of blocks
			while (DataOffset < DataLength)
			{
				if (FormatFlags == kPVRTextureFlagTypePVRTC_4)
				{
					BlockSize = 4 * 4; // Pixel by pixel block size for 4bpp
					WidthBlocks = Width / 4;
					HeightBlocks = Height / 4;
					bpp = 4;
				}
				else
				{
					BlockSize = 8 * 4; // Pixel by pixel block size for 2bpp
					WidthBlocks = Width / 8;
					HeightBlocks = Height / 4;
					bpp = 2;
				}
				
				// Clamp to minimum number of blocks
				if (WidthBlocks < 2)
					WidthBlocks = 2;
				if (HeightBlocks < 2)
					HeightBlocks = 2;
				
				DataSize = WidthBlocks * HeightBlocks * ((BlockSize  * bpp) / 8);
				
				lpData = (char*)LYNXALLOC(DataSize);
					fs.vRead(lpData, DataSize, sizeof(char));
					GL_TRACE(glCompressedTexImage2D(GL_TEXTURE_2D, Level, m_PixelFormat, Width, Height, 0, DataSize, lpData));
				LYNXFREE(lpData);
				
				DataOffset += DataSize;
				Width = LYNX_MAX((Width >> 1), 1);
				Height = LYNX_MAX((Height >> 1), 1);
				Level++;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::LoadTexturePVR(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
		{
			PVRHEADER			PVRHeader;
			CRenderer::FORMAT	Format;		
			
			if ( (fs.vRead(&PVRHeader, 1, sizeof(PVRHEADER)) != sizeof (PVRHEADER)) ) 
			{				
				return LYNX_FALSE;
			}		
			LYNX_BYTE_ORDER(&PVRHeader.width, sizeof(PVRHeader.width));
			LYNX_BYTE_ORDER(&PVRHeader.height, sizeof(PVRHeader.height));	
			LYNX_BYTE_ORDER(&PVRHeader.pvrTag, sizeof(PVRHeader.pvrTag));	
			LYNX_BYTE_ORDER(&PVRHeader.numMipmaps, sizeof(PVRHeader.numMipmaps));	
			LYNX_BYTE_ORDER(&PVRHeader.flags, sizeof(PVRHeader.flags));	
			LYNX_BYTE_ORDER(&PVRHeader.dataLength, sizeof(PVRHeader.dataLength));
			LYNX_BYTE_ORDER(&PVRHeader.bpp, sizeof(PVRHeader.bpp));
			LYNX_BYTE_ORDER(&PVRHeader.bitmaskAlpha, sizeof(PVRHeader.bitmaskAlpha));
			
			int Width = PVRHeader.width;
			int Height = PVRHeader.height;	
			
			if (gPVRTexIdentifier[0] != ((PVRHeader.pvrTag >>  0) & 0xff) ||
				gPVRTexIdentifier[1] != ((PVRHeader.pvrTag >>  8) & 0xff) ||
				gPVRTexIdentifier[2] != ((PVRHeader.pvrTag >> 16) & 0xff) ||
				gPVRTexIdentifier[3] != ((PVRHeader.pvrTag >> 24) & 0xff))
			{
				return LYNX_FALSE;
			}
			
			uint32_t FormatFlags = PVRHeader.flags & PVR_TEXTURE_FLAG_TYPE_MASK;
			
			if (!(FormatFlags == kPVRTextureFlagTypePVRTC_4 || FormatFlags == kPVRTextureFlagTypePVRTC_2))
				return LYNX_FALSE;
			
			LYNXBOOL bHasAlpha; 
			#ifdef __iOS__
				if (CFSwapInt32LittleToHost(PVRHeader.bitmaskAlpha))
					bHasAlpha = TRUE;
				else
					bHasAlpha = FALSE;
			#else
				bHasAlpha = PVRHeader.bitmaskAlpha;
			#endif
			
			if (FormatFlags == kPVRTextureFlagTypePVRTC_4)
			{
				if (bHasAlpha)
					Format = CRenderer::FORMAT_RGBA_PVRTC_4;
				else 
					Format = CRenderer::FORMAT_RGB_PVRTC_4;
			}
			else if (FormatFlags == kPVRTextureFlagTypePVRTC_2)
			{
				if (bHasAlpha)
					Format = CRenderer::FORMAT_RGBA_PVRTC_2;
				else
					Format = CRenderer::FORMAT_RGB_PVRTC_2;
			}
	
			if (!vCreate(name, Width, Height, mipmap, type, Format))
			{
				return LYNX_FALSE;
			}
			
			switch (type & 0xff)
			{
				case CRenderer::TT_CUBE_TEXTURE:								
					break;
					
				case CRenderer::TT_2D_TEXTURE:				
					LoadPVR(fs, &PVRHeader);											
					break;
			}	

			m_bLoaded = LYNX_TRUE;
		
			//Yahoo!!
			return LYNX_TRUE;     
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::LoadTextureKTX(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
		{
            #ifdef __ANDROID__
                int size = fs.vSize();
				void* bytes = LYNXALLOC(size);
                fs.vRead(bytes, sizeof(BYTE), size);
                //LoadKTX(bytes, size, name, mipmap, type);

                GLenum target;
                GLenum glerror;
                GLboolean isMipmapped;
                KTX_dimensions dim;
                KTX_error_code ktxerror;

                m_GLHandle = 0;
                ktxerror = ktxLoadTextureM(bytes, size, &m_GLHandle, &target, &dim, &isMipmapped, &glerror, 0, NULL);

                if (!Create(name, dim.width, dim.height, mipmap, type, CRenderer::FORMAT_RGB8_ETC1, m_GLHandle))
                {
                    return LYNX_FALSE;
                }
                
				LYNXFREE(bytes);

                m_bLoaded = LYNX_TRUE;
            #endif
									
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Texture::LoadTGA(CStream& fs, TGAHEADER* lpheader)
		{
			BYTE                *lpB;
			DWORD               *lpDW;
			WORD                *lpW;
			BYTE				P;
			long                y, x;
			long                i;
			PALETTEENTRY        PE[2], Pal[256];	
			GLenum				ImageFormat;		
			BYTE				*lpMem, *lpOMem, *lpSrc;

			switch (lpheader->imdepth)
			{
				case 8:
					break;
				case 32:
					ImageFormat = GL_RGBA;
					break;
				case 24:
					ImageFormat = GL_RGB;
					break;
			}
			
			switch (m_lpSrcBuffer->GetFormat())
			{
				case CRenderer::FORMAT_A8R8G8B8:	
					lpSrc = (BYTE *)LYNXALLOC(lpheader->imwidth*lpheader->imheight*4);
					if (lpheader->imdepth == 24)
					{		
						lpOMem = lpMem = (BYTE *)LYNXALLOC(lpheader->imwidth*lpheader->imheight*4);
						fs.vRead(lpMem, sizeof(BYTE), lpheader->imwidth*lpheader->imheight*3);										
						if (lpheader->imdesc & 0x20)
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y * lpheader->imwidth * 4));
								for (x=0; x<lpheader->imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);									
									lpB += 3;														
									lpMem += 3;							
									*lpB++ = 255;
								}
							}								
						}
						else
						{
							for (y=0; y<lpheader->imheight; y++)												
							{	
								lpB = ( ((BYTE *)(lpSrc)) + (lpheader->imheight - y - 1) * lpheader->imwidth * 4);						
								for (x=0; x<lpheader->imwidth; x++)												
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);
									lpB += 3;														
									lpMem += 3;							
									*lpB++ = 255;
								}
							}																						
						}      																		
						LYNXFREE(lpOMem);
					}
					else if (lpheader->imdepth == 32)
					{				
						lpOMem = lpMem = (BYTE *)LYNXALLOC(lpheader->imwidth*lpheader->imheight*4);
						fs.vRead(lpMem, sizeof(BYTE), lpheader->imwidth*lpheader->imheight*4);										
						if (lpheader->imdesc & 0x20)
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y * lpheader->imwidth * 4));
								for (x=0; x<lpheader->imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);
									*(lpB+3) = *(lpMem+3);
									lpB += 4;														
									lpMem += 4;														
								}
							}													
						}
						else
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (lpheader->imheight - y - 1) * lpheader->imwidth * 4);
								for (x=0; x<lpheader->imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);
									*(lpB+3) = *(lpMem+3);
									
									lpB += 4;														
									lpMem += 4;														
								}
							}															
						}			
						LYNXFREE(lpOMem);
					}
					break;
				
				case CRenderer::FORMAT_R8G8B8:	
					lpSrc = (BYTE *)LYNX_MALLOC(lpheader->imwidth*lpheader->imheight*3);				
					if (lpheader->imdepth == 24)
					{
						lpOMem = lpMem = (BYTE *)LYNXALLOC(lpheader->imwidth*lpheader->imheight*3);
						fs.vRead(lpMem, sizeof(BYTE), lpheader->imwidth*lpheader->imheight*3);										
						if (lpheader->imdesc & 0x20)
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y * lpheader->imwidth * 3));
								for (x=0; x<lpheader->imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);
									lpB += 3;														
									lpMem += 3;														
								}
							}													
						}
						else
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (lpheader->imheight - y - 1) * lpheader->imwidth * 3);
								for (x=0; x<lpheader->imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);
									lpB += 3;
									lpMem += 3;	
								}
							}																	
						}
						LYNXFREE(lpOMem);
					}
					else if (lpheader->imdepth == 32)
					{
						lpOMem = lpMem = (BYTE *)LYNX_MALLOC(lpheader->imwidth*lpheader->imheight*4);
						fs.vRead(lpMem, sizeof(BYTE), lpheader->imwidth*lpheader->imheight*4);										
						if (lpheader->imdesc & 0x20)
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y * lpheader->imwidth * 3));
								for (x=0; x<lpheader->imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);
									lpB += 3;
									lpMem += 4;								
								}
							}															
						}
						else
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (lpheader->imheight - y - 1) * lpheader->imwidth * 3);
								for (x=0; x<lpheader->imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);
									lpB += 3;
									lpMem += 4;								
								}
							}															 
						}					
						LYNXFREE(lpOMem);
					}
					break;

				case CRenderer::FORMAT_P8:						
					break;
			}		
			LOCK_MUTEX
			GL_TRACE(glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, lpheader->imwidth, lpheader->imheight, 0, ImageFormat, GL_UNSIGNED_BYTE, lpSrc);)
			UNLOCK_MUTEX
			
			LYNXFREE(lpSrc);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::LoadTextureTGA(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
		{
			TGAHEADER			TGAHeader;
			CRenderer::FORMAT	Format;		
			
			if ( (fs.vRead(&TGAHeader, 1, sizeof(TGAHEADER)) != sizeof (TGAHEADER)) ) 
			{				
				return LYNX_FALSE;
			}		
			LYNX_BYTE_ORDER(&TGAHeader.imwidth, sizeof(TGAHeader.imwidth));
			LYNX_BYTE_ORDER(&TGAHeader.imheight, sizeof(TGAHeader.imheight));	
			
			int Width = TGAHeader.imwidth;
			int Height = TGAHeader.imheight;	

			/***********************************************************************/
			/* Skip descriptive bytes at end of header, idlen specifies the number */
			/***********************************************************************/
			fs.vSeek(TGAHeader.idlen, CStream::SeekCur);

			switch (TGAHeader.imdepth)
			{
				case 8:
					Format = CRenderer::FORMAT_P8;
					break;
				case 32:
					Format = CRenderer::FORMAT_A8R8G8B8;					
					break;
				case 24:
					Format = CRenderer::FORMAT_R8G8B8;					
					break;
			}
			if (!vCreate(name, TGAHeader.imwidth, TGAHeader.imheight, mipmap, type, Format))
			{
				return LYNX_FALSE;
			}

			switch (type & 0xff)
			{
				case CRenderer::TT_CUBE_TEXTURE:								
					/*for (i=0; i<6; i++)
					{
						lynxGetTextureSrcBuffer(texindex, i);														
						ilynxD3DLoadCubeTextureTGAToBuffer(texindex, &lptexinfo->SrcBuffer, lpheader->imwidth, lpheader, fp);								
					}
					lynxGetTextureSrcBuffer(texindex, 0);*/
					break;

				case CRenderer::TT_2D_TEXTURE:				
					LoadTGA(fs, &TGAHeader);											
					break;
			}	
			
			if (m_MipMapLevel == 0)
				GL_TRACE(glGenerateMipmap(GL_TEXTURE_2D);)

			m_bLoaded = LYNX_TRUE;
									
			//Yahoo!!
			return LYNX_TRUE;     
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Texture::LoadPNG(BYTE* data, png_infop info)
		{
			BYTE                *lpB, *lpSrc, *lpMem;
			long                y, x;
			int					BPP = 0;
            GLenum				ImageFormat;

            switch (info->color_type)
			{
				case PNG_COLOR_TYPE_RGBA:
					ImageFormat = GL_RGBA;
					break;
				case PNG_COLOR_TYPE_RGB:
					ImageFormat = GL_RGB;
					break;
			}
            
			lpMem = data;
			switch (m_lpSrcBuffer->GetFormat())
			{
				case CRenderer::FORMAT_A8R8G8B8:	
					BPP = 4;
					lpSrc = (BYTE *)LYNXALLOC(info->width*info->height*4);
					if (info->color_type == PNG_COLOR_TYPE_RGB)
					{		
						for (y=0; y<info->height; y++)
						{
							lpB = ( (BYTE *)(lpSrc) + (y * info->width * 4));
							for (x=0; x<info->width; x++)
							{
								*(lpB+0) = *(lpMem+0);
								*(lpB+1) = *(lpMem+1);
								*(lpB+2) = *(lpMem+2);
								*(lpB+3) = 255;

								lpB += 4;														
								lpMem += 3;															
							}
						}						
					}
					else if (info->color_type == PNG_COLOR_TYPE_RGBA)
					{		
						for (y=0; y<info->height; y++)
						{
							lpB = ( (BYTE *)(lpSrc) + (y * info->width * 4));
							for (x=0; x<info->width; x++)
							{
								*(lpB+0) = *(lpMem+0);
								*(lpB+1) = *(lpMem+1);
								*(lpB+2) = *(lpMem+2);
								*(lpB+3) = *(lpMem+3);

								lpB += 4;														
								lpMem += 4;															
							}
						}						
					}					
					break;
				
				case CRenderer::FORMAT_R8G8B8:	
					BPP = 3;
					lpSrc = (BYTE *)LYNXALLOC(info->width*info->height*3);
					if (info->color_type == PNG_COLOR_TYPE_RGB)
					{		
						for (y=0; y<info->height; y++)
						{
							lpB = ( (BYTE *)(lpSrc) + (y * info->width * 3));
							for (x=0; x<info->width; x++)
							{
								*(lpB+0) = *(lpMem+0);
								*(lpB+1) = *(lpMem+1);
								*(lpB+2) = *(lpMem+2);

								lpB += 3;														
								lpMem += 3;															
							}
						}						
					}
					else if (info->color_type == PNG_COLOR_TYPE_RGBA)
					{		
						for (y=0; y<info->height; y++)
						{
							lpB = ( (BYTE *)(lpSrc) + (y * info->width * 3));
							for (x=0; x<info->width; x++)
							{
								*(lpB+0) = *(lpMem+0);
								*(lpB+1) = *(lpMem+1);
								*(lpB+2) = *(lpMem+2);

								lpB += 3;														
								lpMem += 4;															
							}
						}						
					}
					break;
					
				case CRenderer::FORMAT_A4R4G4B4:						
				case CRenderer::FORMAT_A1R5G5B5:						
					break;

				case CRenderer::FORMAT_P8:						
					break;
			}					

			LOCK_MUTEX			
			GL_TRACE(glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, info->width, info->height, 0, ImageFormat, GL_UNSIGNED_BYTE, lpSrc);)
			LYNXDEL(lpSrc);
			UNLOCK_MUTEX

			m_VideoMemoryUsage = info->width*info->height*BPP;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::LoadTexturePNG(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
		{
			CRenderer::FORMAT	Format;

			png_structp png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, 
				/*error_ptr*/NULL,
				PNGError,
				PNGWarning,
				/*mem_ptr*/NULL, 
				PNGMalloc,
				PNGFree);        

			png_infop info_ptr = png_create_info_struct(png_ptr);			
			png_set_read_fn(png_ptr, &fs, PNGRead);

			DWORD sig_read = 0;
			png_set_sig_bytes(png_ptr, sig_read);
  
			png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

			int Width = info_ptr->width;
			int Height = info_ptr->height;

			int channels = 0;
			switch (info_ptr->color_type)
			{
				case PNG_COLOR_TYPE_RGBA:
					Format = CRenderer::FORMAT_A8R8G8B8;
					break;

				case PNG_COLOR_TYPE_RGB:
					if (m_lpGraphicsSystem->GetlpRenderer()->CheckTextureFormat(CRenderer::FORMAT_R8G8B8))
						Format = CRenderer::FORMAT_R8G8B8;	
					else
						Format = CRenderer::FORMAT_A8R8G8B8;
					break;

				default: 					
					break;
			}

			if (!vCreate(name, Width, Height, mipmap, type, Format))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
				return LYNX_FALSE;
            }

			if (!m_bDelayLoading)
			{
				DWORD row_bytes = png_get_rowbytes(png_ptr, info_ptr);        
				png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
				BYTE* data = (BYTE*)LYNXALLOC(row_bytes * Height);           

				for (int i=0; i<Height; ++i)
				{
					lynxMemCpy(data+(row_bytes*i), row_pointers[i], row_bytes);
				}			

				switch (type & 0xff)
				{
					case CRenderer::TT_CUBE_TEXTURE:								
						/*for (i=0; i<6; i++)
						{
							lynxGetTextureSrcBuffer(texindex, i);														
							ilynxD3DLoadCubeTextureTGAToBuffer(texindex, &lptexinfo->SrcBuffer, lpheader->imwidth, lpheader, fp);								
						}
						lynxGetTextureSrcBuffer(texindex, 0);*/
						break;

					default:		
						LoadPNG(data, info_ptr);
						break;
				}
				png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
				LYNXFREE(data);

				if (m_MipMapLevel == 0)
					GL_TRACE(glGenerateMipmap(GL_TEXTURE_2D);)

				m_bLoaded = LYNX_TRUE;			
			}
			else
			{
				png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			}
			
			//Yahoo!!
			return LYNX_TRUE;     
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::DelayLoadTexturePNG(CStream& fs)
		{
			CRenderer::FORMAT	Format;

			png_structp png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, 
				/*error_ptr*/NULL,
				PNGError,
				PNGWarning,
				/*mem_ptr*/NULL, 
				PNGMalloc,
				PNGFree);        

			png_infop info_ptr = png_create_info_struct(png_ptr);			
			png_set_read_fn(png_ptr, &fs, PNGRead);

			DWORD sig_read = 0;
			png_set_sig_bytes(png_ptr, sig_read);
  
			png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

			int Width = info_ptr->width;
			int Height = info_ptr->height;

			int channels = 0;
			switch (info_ptr->color_type)
			{
				case PNG_COLOR_TYPE_RGBA:
					Format = CRenderer::FORMAT_A8R8G8B8;
					break;

				case PNG_COLOR_TYPE_RGB:
					if (m_lpGraphicsSystem->GetlpRenderer()->CheckTextureFormat(CRenderer::FORMAT_R8G8B8))
						Format = CRenderer::FORMAT_R8G8B8;	
					else
						Format = CRenderer::FORMAT_A8R8G8B8;
					break;

				default: 					
					break;
			}

			DWORD row_bytes = png_get_rowbytes(png_ptr, info_ptr);        
			png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
			BYTE* data = (BYTE*)LYNXALLOC(row_bytes * Height);           

			for (int i=0; i<Height; ++i)
			{
				lynxMemCpy(data+(row_bytes*i), row_pointers[i], row_bytes);
			}			

			LOCK_MUTEX
			GL_TRACE(glBindTexture(GL_TEXTURE_2D, m_GLHandle);)
			UNLOCK_MUTEX

			switch (m_TextureType & 0xff)
			{
				case CRenderer::TT_CUBE_TEXTURE:								
					/*for (i=0; i<6; i++)
					{
						lynxGetTextureSrcBuffer(texindex, i);														
						ilynxD3DLoadCubeTextureTGAToBuffer(texindex, &lptexinfo->SrcBuffer, lpheader->imwidth, lpheader, fp);								
					}
					lynxGetTextureSrcBuffer(texindex, 0);*/
					break;

				default:		
					LoadPNG(data, info_ptr);
					break;
			}
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			LYNXFREE(data);

			if (m_MipMapLevel == 0)
				GL_TRACE(glGenerateMipmap(GL_TEXTURE_2D);)
			
			m_bLoaded = LYNX_TRUE;

			//Yahoo!!
			return LYNX_TRUE;     
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::vLoad(CStream& fs, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc)
		{
            LYNXBOOL bLoadTexture;
            LYNXCHAR Name[256], Ext[256];	
            LYNXCHAR Path[256];            

			if (desc)
			{
				m_MipMapLevel		= desc->MipMapLevel;
				m_TextureType		= desc->Type;
				m_bSRGB				= desc->bSRGB;
				m_LODBias			= desc->LODBias;					
				m_AlphaReference	= desc->AlphaReference;
				m_bDelayLoading		= desc->bDelayLoading;	
			}
			else
			{
				m_MipMapLevel		= 0;
				m_TextureType		= CRenderer::TT_2D_TEXTURE;
				m_bSRGB				= LYNX_FALSE;
				m_LODBias			= 0;					
				m_AlphaReference	= 1.0f;
				m_bDelayLoading		= LYNX_FALSE;	
			}
            
            if (desc && desc->bLoadFromMemory && desc->Width > 0 && desc->Height > 0)
            {
                lynxStrCpy(Path, filename);
                bLoadTexture = vCreate(filename, desc->Width, desc->Height, m_MipMapLevel, m_TextureType, desc->TargetFormat); 
                if (bLoadTexture && desc->lpData)
                {                  
					LOCK_MUTEX
					GL_TRACE(glTexImage2D(GL_TEXTURE_2D, 0, COGLES2Renderer::TranslateFormat(desc->TargetFormat), desc->Width, desc->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, desc->lpData);)
					UNLOCK_MUTEX
                }
            }
            else
            {
                CString					FinalPath;
                
                if ((lynxStrLen(filename) == 0))
                {
                    return LYNX_FALSE;
                }
                lynxSeparateFileExtByPath(Name, Ext, filename);
                //if (!lynxStrStr(Name, _T("fonts")))
                //    lynxStrCpy(Name, _T("../texture/black"));

                //lynxStrCpy(Name, _T("../texture/checker_4"));
                
				// Read TGA texture instead of PSD format.
				//if (!lynxStriCmp(Ext, _T("psd")))
					//lynxStrCpy(Ext, _T("tga"));

			#if defined __iOS__
				// Use pvr texture wwhenever it's possible
				lynxStrCpy(Path, Name);
				lynxStrCat(Path, _T(".pvr"));
				if (fs.vOpen(Path, CStream::Read|CStream::Binary))
				{
					fs.vClose();
					lynxStrCpy(Ext, _T("pvr"));
				}
				else
			#elif defined __ANDROID__
				lynxStrCpy(Path, Name);
				lynxStrCat(Path, _T(".ktx"));
				if (fs.vOpen(Path, CStream::Read|CStream::Binary))
				{
					fs.vClose();
					lynxStrCpy(Ext, _T("ktx"));
				}
				else
			#endif
				{
					lynxStrCpy(Path, Name);
					lynxStrCat(Path, _T("."));
					lynxStrCat(Path, Ext);			

					if (fs.vOpen(Path, CStream::Read|CStream::Binary))
					{
						fs.vClose();
					}					
				}

				lynxStrCpy(Path, Name);
				lynxStrCat(Path, _T("."));
				lynxStrCat(Path, Ext);			
                
				m_FullPath = Path;				
				lynxStrLower(Ext);
				m_Ext = Ext;

				if (m_bDelayLoading)
				{
					//m_FullPath = _T("../texture/checker.png");
				}

				CStream* stream = &fs;
				OPEN_Z_FILE(stream, m_FullPath.c_str())
				else
				{
					if (!stream->vOpen(m_FullPath.c_str(), CStream::Binary | CStream::Read))
					{
						m_bLoaded = LYNX_TRUE;
						LYNX_LOG(glpLogger, CString(_T("Can't find texture <")) + m_FullPath + CString(_T(">\n")));
						return LYNX_FALSE;			
					}
				}

				/*
				if (!fs.vOpen(Path, CStream::Read|CStream::Binary))
                {
					m_bLoaded = LYNX_TRUE;
                    LYNX_LOG(glpLogger, CString(_T("Can't find texture <"))+Path+CString(_T(">\n")));
                    return LYNX_FALSE;
                }
				*/

                bLoadTexture = LYNX_FALSE;
				m_bLoaded = LYNX_FALSE;
                if (!lynxStriCmp(Ext, _T("bmp")))	
                    bLoadTexture = LoadTextureBMP((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);
			    else if (!lynxStriCmp(Ext, _T("png")))
                    bLoadTexture = LoadTexturePNG((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);
                else if (!lynxStriCmp(Ext, _T("tga")))
                    bLoadTexture = LoadTextureTGA((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);
                else if (!lynxStriCmp(Ext, _T("pvr")))
                    bLoadTexture = LoadTexturePVR((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);
				else if (!lynxStriCmp(Ext, _T("ktx")))
                    bLoadTexture = LoadTextureKTX((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);
                        
                (*stream).vClose();           
            }
			
			if (bLoadTexture)
			{
				LYNX_LOG(glpLogger, CString(_T("Loaded texture <"))+Path+CString(_T(">\n")));
				return LYNX_TRUE;
			}
			else
			{
				LYNX_LOG(glpLogger, CString(_T("Can't load texture <"))+Path+CString(_T(">\n")));
				return LYNX_TRUE;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::Create(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, GLuint handle)
		{		
			DWORD		NumMip;
			DWORD		Usage;			
			BYTE*		lpB;
            LYNXBOOL    bDepthTexture;		

			switch (format)
			{
				case CRenderer::FORMAT_D16_LOCKABLE:
				case CRenderer::FORMAT_D32:
				case CRenderer::FORMAT_D15S1:
				case CRenderer::FORMAT_D24S8:
				case CRenderer::FORMAT_D24X8:
				case CRenderer::FORMAT_D24X4S4:
				case CRenderer::FORMAT_D16:
				case CRenderer::FORMAT_D32F_LOCKABLE:
				case CRenderer::FORMAT_D24FS8:
					bDepthTexture = LYNX_TRUE;
					break;
                    
				default:
					bDepthTexture = LYNX_FALSE;
					break;
			}
            
            m_TextureType = type;
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpSrcBuffer, LynxEngine::BUFFER);
			m_lpSrcBuffer->vCreate(0, w, h, format, 0, LYNX_FALSE);
			m_lpSrcBuffer->SetName(CString(name)+_T(".SrcBuffer"));
			((COGLES2Buffer*)&(*m_lpSrcBuffer))->SetBufferType(COGLES2Buffer::BT_TEXTURE2D);
			
			m_PixelFormat = COGLES2Renderer::TranslateFormat(format);
			
			LOCK_MUTEX

			if (m_GLHandle)
			{
				glDeleteTextures(1, &m_GLHandle);
				m_GLHandle = 0;
			}

			switch (type)
			{
				case CRenderer::TT_2D_TEXTURE:		
					if (handle)
						m_GLHandle = handle;
					else
						GL_TRACE(glGenTextures(1, &m_GLHandle);)
					GL_TRACE(glBindTexture(GL_TEXTURE_2D, m_GLHandle);)
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    if (bDepthTexture)
                    {
                        GL_TRACE(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);)
                    }
					vGetLevel(0, 0);
					//glpRenderer->TextureManager.UsedTextureMemory += ((width*height*lpBuffer->bpp)>>3);		
					break;

				case CRenderer::TT_CUBE_TEXTURE:
					//PixelFormat = CD3D9Renderer::TranslateFormat(format);
					//Usage = 0;
					
					vGetLevel(0, 0);
					break;							
			}			
			UNLOCK_MUTEX

			m_Width = w;
			m_Height = h;

			if (!lynxCheckPowerOf2(m_Width, 18) || !lynxCheckPowerOf2(m_Height, 18))
			{
				m_MipMapLevel = 1;
			}
			SetName(name);
			((COGLES2Buffer*)&(*m_lpSrcBuffer))->SetGLHandle(m_GLHandle);
            ((COGLES2Buffer*)&(*m_lpSrcBuffer))->SetlpTexture(this);			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format)
		{
			return Create(name, w, h, mipmaplevel, type, format, 0);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES2Texture::vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable)
		{			
			DWORD							NumMip;
			DWORD							Usage;
			int								i, j;
			BYTE							*lpB;

			m_TextureType = type;
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpSrcBuffer, LynxEngine::BUFFER);
			m_lpSrcBuffer->vCreate(0, w, h, format, 0, LYNX_FALSE);
			m_lpSrcBuffer->SetName(CString(name)+_T(".SrcBuffer"));
			((COGLES2Buffer*)&(*m_lpSrcBuffer))->SetBufferType(COGLES2Buffer::BT_TEXTURE2D);
		
			m_PixelFormat = COGLES2Renderer::TranslateFormat(format);
			GLenum PixelType = COGLES2Renderer::TranslateFormatToPixelType(format);
		
			LOCK_MUTEX

            switch (type)
			{
				case CRenderer::TT_2D_TEXTURE:
					GL_TRACE(glGenTextures(1, &m_GLHandle);)
					GL_TRACE(glBindTexture(GL_TEXTURE_2D, m_GLHandle);)
					GL_TRACE(glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, w, h, 0, m_PixelFormat, PixelType, NULL);)
                    break;

				case CRenderer::TT_CUBE_TEXTURE:
					//PixelFormat = CD3D9Renderer::TranslateFormat(format);
					//Usage = D3DUSAGE_RENDERTARGET;
					//if(FAILED(lpR->GetlpD3DDevice()->CreateCubeTexture(w, 1, Usage, PixelFormat, D3DPOOL_DEFAULT, (IDirect3DCubeTexture9**)&m_lpTexture, NULL)))
					//{       
					//	return LYNX_FALSE;
					//}
					vGetLevel(0, 0);
					break;							
			}			

			UNLOCK_MUTEX
			/*
			 if (ms > 1)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpMSSrcBuffer, LynxEngine::BUFFER);
				m_lpMSSrcBuffer->vCreate(NULL, w, h, format, ms, blockable);
				m_lpMSSrcBuffer->SetName(CString(name)+_T(".MSSrcBuffer"));
			}
			 */
			if (lpzb)
				(*lpzb)->vCreate(NULL, w, h, zfmt, ms, blockable);

			m_Width = w;
			m_Height = h;
			SetName(name);
			((COGLES2Buffer*)&(*m_lpSrcBuffer))->SetGLHandle(m_GLHandle);
            ((COGLES2Buffer*)&(*m_lpSrcBuffer))->SetlpTexture(this);
            //((COGLES2Buffer*)&(*m_lpSrcBuffer))->GenerateFrameBuffer();			
       
			return LYNX_TRUE;
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Texture::vUpdate(const int level, const void* data)
        {
			LOCK_MUTEX

            switch (m_TextureType & 0xff)
			{
				case CRenderer::TT_CUBE_TEXTURE:
					break;
                    
				default:
                case CRenderer::TT_2D_TEXTURE:
					GL_TRACE(glBindTexture(GL_TEXTURE_2D, m_GLHandle);)
                    GL_TRACE(glTexImage2D(GL_TEXTURE_2D, level, m_PixelFormat, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);)
                    
                    if (m_MipMapLevel == 0)
                        GL_TRACE(glGenerateMipmap(GL_TEXTURE_2D);)
					break;
			}

			UNLOCK_MUTEX
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBuffer* COGLES2Texture::vSetAsRT(int channel) 
		{
			return m_lpGraphicsSystem->GetlpRenderer()->SetRenderBuffer(channel, GetSrcBuffer());
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES2Texture::vSet(int channel) 
		{
			if (m_bDelayLoading && !m_bLoaded)
			{
				CMemoryStream FileStream;//
				CStream* stream = &FileStream;
				OPEN_Z_FILE(stream, m_FullPath.c_str())
				else
				{
					if (!stream->vOpen(m_FullPath.c_str(), CStream::Binary|CStream::Read))
					{
						LYNX_LOG(glpLogger, CString(_T("Can't find texture <"))+m_FullPath+CString(_T(">\n")));								
					}
				}

				if (stream->IsValid())
				{
					if (m_Ext == CString(_T("png")))
					{
						DelayLoadTexturePNG(*stream);
					}
					else if (m_Ext == CString("tga"))
					{
						//DelayLoadTextureTGA(File);
					}
					else if (m_Ext == CString("dds"))
					{
						//DelayLoadTextureDDS(File);
					}
					else
					{
						//DelayLoadTextureOthers(File);
					}					
					stream->vClose();		
					m_bLoaded = LYNX_TRUE;
				}

				/*
				CMemoryStream FileStream;//
				CStream* File = &FileStream;//m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
				if (File->vOpen(m_FullPath, CStream::Binary|CStream::Read))
				{
					if (m_Ext == CString(_T("png")))
					{
						DelayLoadTexturePNG(*File);
					}
					else if (m_Ext == CString("tga"))
					{
						//DelayLoadTextureTGA(File);
					}
					else if (m_Ext == CString("dds"))
					{
						//DelayLoadTextureDDS(File);
					}
					else
					{
						//DelayLoadTextureOthers(File);
					}					
					File->vClose();				
				}
				*/
			}
            LOCK_MUTEX

			GL_TRACE(glActiveTexture(GL_TEXTURE0+channel);)       				
			GL_TRACE(glBindTexture(GL_TEXTURE_2D, m_GLHandle);)			

			UNLOCK_MUTEX
		}
	}
}

#endif