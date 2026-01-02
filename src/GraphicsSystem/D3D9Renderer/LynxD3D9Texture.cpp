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
#include <LynxEngine.h>
#include <LynxLogger.h>
#ifdef __FREEIMAGE__
	#include <FreeImage.h>
#endif
#include <pnginfo.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Buffer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Texture.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Texture::CD3D9Texture(CGraphicsSystem *pgs)
		: CTexture(pgs)
		{	
			m_lpTexture = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9Texture::~CD3D9Texture(void)
		{
			if (m_lpTexture)
			{
				m_lpTexture->Release();
				m_lpTexture = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		IDirect3DSurface9* CD3D9Texture::GetMipMapSurface(int face, int level)
		{
			LYNX_ASSERT(m_lpTexture);

			IDirect3DSurface9* lpSurface = NULL;
			switch (m_TextureType)
			{
				default:
					return NULL;
					break;

				case CRenderer::TT_2D_TEXTURE:
					if(FAILED(((IDirect3DTexture9*)m_lpTexture)->GetSurfaceLevel(level, &lpSurface)))
					{       
						return NULL;
					}	
					break;

				case CRenderer::TT_CUBE_TEXTURE:
					if(FAILED(((IDirect3DCubeTexture9*)m_lpTexture)->GetCubeMapSurface((D3DCUBEMAP_FACES)face, level, &lpSurface)))
					{       
						return NULL;
					}
					break;
			}
			return lpSurface;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Texture::vGetLevel(int face, int level)
		{
			LYNX_ASSERT(m_lpTexture);

			if (((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetlpSurface())
				ReleaseMipMapSurface(((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetlpSurface());
			IDirect3DSurface9* lpSurface = GetMipMapSurface(face, level);
			((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->SetlpSurface(lpSurface);
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Texture::LoadTextureOthers(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
		{
			#ifdef __FREEIMAGE__
				FreeImageIO IOData;

				IOData.read_proc	= FreeImage_ReadFileProc;
				IOData.write_proc	= FreeImage_WriteFileProc;
				IOData.tell_proc	= FreeImage_TellFileProc;
				IOData.seek_proc	= FreeImage_SeekFileProc;

				FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromHandle(&IOData, &fs);
				if (fif != FIF_UNKNOWN) 
				{
					CRenderer::FORMAT Format;
					FIBITMAP *dib = FreeImage_LoadFromHandle(fif, &IOData, &fs);
					if (dib)
					{
						FREE_IMAGE_TYPE Type = FreeImage_GetImageType(dib);
						int bpp = FreeImage_GetBPP(dib);
						int Width = FreeImage_GetWidth(dib);
						int Height = FreeImage_GetHeight(dib);
						BYTE* lpDIBData = FreeImage_GetBits(dib);	
						BYTE* lpData = LYNXALLOC(Width*Height*4);
						BYTE* lpDIBC = lpDIBData;
						BYTE* lpC = lpData;
						switch (bpp)
						{
							case 32:
								Format = CRenderer::FORMAT_A8R8G8B8;
								for (int y=Height-1; y>=0; y--)
								{
									lpDIBC = lpDIBData + (y*Width*4);
									for (int x=0; x<Width; x++)
									{
										*(lpC+0) = *(lpDIBC+0);
										*(lpC+1) = *(lpDIBC+1);
										*(lpC+2) = *(lpDIBC+2);
										*(lpC+3) = *(lpDIBC+3);

										lpDIBC += 4;
										lpC += 4;
									}
								}
								break;

							case 24:
								if (m_lpGraphicsSystem->GetlpRenderer()->CheckTextureFormat(CRenderer::FORMAT_R8G8B8))
								{
									Format = CRenderer::FORMAT_R8G8B8;
									for (int y=Height-1; y>=0; y--)
									{
										lpDIBC = lpDIBData + (y*Width*3);
										for (int x=0; x<Width; x++)
										{
											*(lpC+0) = *(lpDIBC+0);
											*(lpC+1) = *(lpDIBC+1);
											*(lpC+2) = *(lpDIBC+2);
										
											lpDIBC += 3;
											lpC += 3;
										}
									}
								}
								else
								{							
									Format = CRenderer::FORMAT_A8R8G8B8;
									bpp = 32;
									for (int y=Height-1; y>=0; y--)
									{
										lpDIBC = lpDIBData + (y*Width*3);
										for (int x=0; x<Width; x++)
										{
											*(lpC+0) = *(lpDIBC+0);
											*(lpC+1) = *(lpDIBC+1);
											*(lpC+2) = *(lpDIBC+2);
											*(lpC+3) = 255;

											lpDIBC += 3;
											lpC += 4;
										}
									}
								}														
								break;

							case 8:
								Format = CRenderer::FORMAT_L8;
								for (int y=Height-1; y>=0; y--)
								{
									lpDIBC = lpDIBData + (y*Width*3);
									for (int x=0; x<Width; x++)
									{
										*(lpC) = *(lpDIBC);
									
										lpDIBC ++;
										lpC ++;
									}
								}
								break;
						}

						if (!vCreate(name, Width, Height, mipmap, type, Format))
							return LYNX_FALSE;

						BYTE* lpContext = (BYTE*)m_lpSrcBuffer->vMap(CRenderer::LOCK_TO_WRITE);
							lynxMemCpy(lpContext, lpData, Width*Height*(bpp>>3));
						m_lpSrcBuffer->vUnmap();

						m_VideoMemoryUsage = Width*Height*(bpp>>3);

						LYNXFREE(lpData);
						// free the loaded FIBITMAP
						FreeImage_Unload(dib);
					}
				}
			
				//Yahoo!!
				return LYNX_TRUE;
			#else
				return LYNX_FALSE;
			#endif
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Texture::LoadTGA(CStream& fs, TGAHEADER* lpheader)
		{
			BYTE                            *lpB;
			DWORD                           *lpDW;
			WORD                            *lpW;
			BYTE							P;
			long                            y, x;
			long                            i;
			PALETTEENTRY                    PE[2], Pal[256];				
			BYTE							*lpMem, *lpOMem, *lpSrc;
			int								BPP = 0;

			BYTE* lpContext = (BYTE*)m_lpSrcBuffer->vMap(CRenderer::LOCK_TO_WRITE);
			switch (m_lpSrcBuffer->GetFormat())
			{
				case CRenderer::FORMAT_A8R8G8B8:	
					BPP = 4;
					lpSrc = (BYTE *)LYNXALLOC(lpheader->imwidth*lpheader->imheight * 4);
					if (lpheader->imdepth == 24)
					{		
						lpOMem = lpMem = (BYTE *)LYNXALLOC(lpheader->imwidth*lpheader->imheight * 3);
						fs.vRead(lpMem, sizeof(BYTE), lpheader->imwidth*lpheader->imheight*3);										
						if (lpheader->imdesc & 0x20)
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
								for (x=0; x<lpheader->imwidth; x++)
								{
									lynxMemCpy(lpB, lpMem, 3);
									lpB += 3;														
									lpMem += 3;							
									*lpB++ = 255;
								}
							}
						}
						else
						{						
							//FILE* fp = fopen("c:/mario.txt", "wt"); 
							//char Line[2048];
							//Math::CVector3 Pos(0, 0, 0);
							//int Index = 0;
							for (y=0; y<lpheader->imheight; y++)												
							{	
								lpB = ( ((BYTE *)(lpSrc)) + (lpheader->imheight - y - 1) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());														
								for (x=0; x<lpheader->imwidth; x++)												
								{
									/*
									Pos.x = x;
									Pos.y = y;
									if (!((*lpMem) == 255 && *(lpMem+1) == 255 && *(lpMem+2) == 255))
									{
										sprintf(Line, "Pos[%d].x = %f;  Pos[%d].y = %f;  Pos[%d].z = %f;\r\n", Index, Pos.x, Index, Pos.y, Index, Pos.z);
										fputs(Line, fp);
										sprintf(Line, "Color[%d].Red = %d;  Color[%d].Green = %d;  Color[%d].Blue = %d;	Color[%d].Alpha = 255;\r\n", Index, *(lpMem+2), Index, *(lpMem+1), Index, *(lpMem+0), Index);
										fputs(Line, fp);
										Index++;
									}
									*/

									lynxMemCpy(lpB, lpMem, 3);
									lpB += 3;														
									lpMem += 3;							
									*lpB++ = 255;								
								}								
							}		
							//fclose(fp);
						}      																		
						LYNXFREE(lpOMem);
					}
					else if (lpheader->imdepth == 32)
					{				
						lpOMem = lpMem = (BYTE *)LYNXALLOC(lpheader->imwidth*lpheader->imheight * 4);
						fs.vRead(lpMem, sizeof(BYTE), lpheader->imwidth*lpheader->imheight*4);										
						if (lpheader->imdesc & 0x20)
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
								for (x=0; x<lpheader->imwidth; x++)
								{
									lynxMemCpy(lpB, lpMem, 4);
									lpB += 4;														
									lpMem += 4;														
								}
							}
						}
						else
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (lpheader->imheight - y - 1) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
								for (x=0; x<lpheader->imwidth; x++)
								{
									lynxMemCpy(lpB, lpMem, 4);
									lpB += 4;														
									lpMem += 4;														
								}
							}
						}			
						LYNXFREE(lpOMem);
					}
					break;
				
				case CRenderer::FORMAT_R8G8B8:	
					BPP = 3;
					lpSrc = (BYTE *)LYNX_MALLOC(lpheader->imwidth*lpheader->imheight*3);				
					if (lpheader->imdepth == 24)
					{
						lpOMem = lpMem = (BYTE *)LYNXALLOC(lpheader->imwidth*lpheader->imheight * 3);
						fs.vRead(lpMem, sizeof(BYTE), lpheader->imwidth*lpheader->imheight*3);										
						if (lpheader->imdesc & 0x20)
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
								for (x=0; x<lpheader->imwidth; x++)
								{
									lynxMemCpy(lpB, lpMem, 3);
									lpB += 3;														
									lpMem += 3;														
								}
							}
						}
						else
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (lpheader->imheight - y - 1) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
								for (x=0; x<lpheader->imwidth; x++)
								{
									lynxMemCpy(lpB, lpMem, 3);
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
								lpB = ( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
								for (x=0; x<lpheader->imwidth; x++)
								{
									lynxMemCpy(lpB, lpMem, 3);
									lpB += 3;
									lpMem += 4;								
								}
							}
						}
						else
						{
							for (y=0; y<lpheader->imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (lpheader->imheight - y - 1) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
								for (x=0; x<lpheader->imwidth; x++)
								{
									lynxMemCpy(lpB, lpMem, 3);
									lpB += 3;
									lpMem += 4;								
								}
							}
						}					
						LYNXFREE(lpOMem);
					}
					break;
					
				case CRenderer::FORMAT_A4R4G4B4:						
				case CRenderer::FORMAT_A1R5G5B5:	
					BPP = 2;					
					break;

				case CRenderer::FORMAT_P8:						
					break;
			}				
			lynxMemCpy(lpContext, lpSrc, lpheader->imwidth*lpheader->imheight*BPP);
			m_lpSrcBuffer->vUnmap();    	    								
			LYNXFREE(lpSrc);

			m_VideoMemoryUsage = lpheader->imwidth*lpheader->imheight*BPP;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Texture::LoadTextureTGA(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
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
					if (m_lpGraphicsSystem->GetlpRenderer()->CheckTextureFormat(CRenderer::FORMAT_R8G8B8))
						Format = CRenderer::FORMAT_R8G8B8;	
					else
						Format = CRenderer::FORMAT_A8R8G8B8;
					break;
			}
			if (!vCreate(name, TGAHeader.imwidth, TGAHeader.imheight, mipmap, type, Format))
				return LYNX_FALSE;

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
					LoadTGA(fs, &TGAHeader);								
					break;
			}			
			if (m_MipMapLevel > 1 || m_MipMapLevel == 0)
				D3DXFilterTexture((IDirect3DTexture9*)m_lpTexture, NULL, mipmap, D3DX_DEFAULT);			
			
			m_bLoaded = LYNX_TRUE;

			//Yahoo!!
			return LYNX_TRUE;     
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Texture::LoadPNG(BYTE* data, png_infop info)
		{
			BYTE                            *lpB, *lpSrc, *lpMem;
			long                            y, x;
			int								BPP = 0;

			BYTE* lpContext = (BYTE*)m_lpSrcBuffer->vMap(CRenderer::LOCK_TO_WRITE);
			lpSrc = lpContext;
			lpMem = data;
			switch (m_lpSrcBuffer->GetFormat())
			{
				case CRenderer::FORMAT_A8R8G8B8:	
					BPP = 4;
					if (info->color_type == PNG_COLOR_TYPE_RGB)
					{		
						for (y=0; y<info->height; y++)
						{
							lpB = ( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
							for (x=0; x<info->width; x++)
							{
								*(lpB+0) = *(lpMem+2);
								*(lpB+1) = *(lpMem+1);
								*(lpB+2) = *(lpMem+0);
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
							lpB = ( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
							for (x=0; x<info->width; x++)
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
					break;
				
				case CRenderer::FORMAT_R8G8B8:	
					BPP = 3;
					if (info->color_type == PNG_COLOR_TYPE_RGB)
					{		
						for (y=0; y<info->height; y++)
						{
							lpB = ( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
							for (x=0; x<info->width; x++)
							{
								*(lpB+0) = *(lpMem+2);
								*(lpB+1) = *(lpMem+1);
								*(lpB+2) = *(lpMem+0);								

								lpB += 3;														
								lpMem += 3;															
							}
						}						
					}
					else if (info->color_type == PNG_COLOR_TYPE_RGBA)
					{		
						for (y=0; y<info->height; y++)
						{
							lpB = ( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->GetPitch());
							for (x=0; x<info->width; x++)
							{
								*(lpB+0) = *(lpMem+2);
								*(lpB+1) = *(lpMem+1);
								*(lpB+2) = *(lpMem+0);								

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
			m_lpSrcBuffer->vUnmap();    	    								

			m_VideoMemoryUsage = info->width*info->height*BPP;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Texture::LoadTexturePNG(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
		{
			CRenderer::FORMAT	Format;

			png_structp png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, 
				/*error_ptr*/NULL,
				PNGError,
				PNGWarning,
				/*mem_ptr*/NULL, 
				PNGMalloc,
				PNGFree);        
			if (!png_ptr)
				return LYNX_FALSE;

			png_infop info_ptr = png_create_info_struct(png_ptr);			
			if (!info_ptr)
				return LYNX_FALSE;

			png_set_read_fn(png_ptr, &fs, PNGRead);							
			
			DWORD sig_read = 0;
			png_set_sig_bytes(png_ptr, sig_read);
  
			png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

			int Width = info_ptr->width;
			int Height = info_ptr->height;

			if (Width <= 0 || Width > 4096)
				return LYNX_FALSE;

			if (Height <= 0 || Height > 4096)
				return LYNX_FALSE;

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

				case PNG_COLOR_TYPE_PALETTE:
					Format = CRenderer::FORMAT_P8;
					break;

				default: 
					Format = CRenderer::FORMAT_R8G8B8;	
					break;
			}

			if (!vCreate(name, Width, Height, mipmap, type, Format))
				return LYNX_FALSE;

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

				if (m_MipMapLevel >1 || m_MipMapLevel == 0)
					D3DXFilterTexture((IDirect3DTexture9*)m_lpTexture, NULL, mipmap, D3DX_DEFAULT);			

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
		LYNXBOOL CD3D9Texture::DelayLoadTexturePNG(CStream& fs)
		{
			CRenderer::FORMAT	Format;

			png_structp png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, 
				/*error_ptr*/NULL,
				PNGError,
				PNGWarning,
				/*mem_ptr*/NULL, 
				PNGMalloc,
				PNGFree);        
			if (!png_ptr)
				return LYNX_FALSE;

			png_infop info_ptr = png_create_info_struct(png_ptr);			
			if (!info_ptr)
				return LYNX_FALSE;					
			
			png_set_read_fn(png_ptr, &fs, PNGRead);

			DWORD sig_read = 0;
			png_set_sig_bytes(png_ptr, sig_read);  
			png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);			

			int Width = info_ptr->width;
			int Height = info_ptr->height;

			if (Width <= 0 || Width > 4096)
				return LYNX_FALSE;

			if (Height <= 0 || Height > 4096)
				return LYNX_FALSE;

			DWORD row_bytes = png_get_rowbytes(png_ptr, info_ptr);        
			png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
			BYTE* data = (BYTE*)LYNXALLOC(row_bytes * Height);

			for (int i=0; i<Height; ++i)
			{
				lynxMemCpy(data+(row_bytes*i), row_pointers[i], row_bytes);
			}			

			LoadPNG(data, info_ptr);
			
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			LYNXFREE(data);

			if (m_MipMapLevel >1 || m_MipMapLevel == 0)
				D3DXFilterTexture((IDirect3DTexture9*)m_lpTexture, NULL, m_MipMapLevel, D3DX_DEFAULT);						
			
			m_bLoaded = LYNX_TRUE;

			//Yahoo!!
			return LYNX_TRUE;     
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Texture::LoadDDS(CStream& fs, DWORD len)
		{
			void* lpC = m_lpSrcBuffer->vMap(CRenderer::LOCK_TO_WRITE_DISCARD_OLD);
			fs.vRead(lpC, 1, len);										
			m_lpSrcBuffer->vUnmap();  
			m_VideoMemoryUsage += len;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Texture::LoadTextureDDS(CStream& fs, const LYNXCHAR* name, int mipmaplevel, CRenderer::TEXTURETYPE type)
		{
			DDSHEADER						DDSHeader;
			int								RealOffset;	
			char							Magic[4];
			LYNXBOOL						Error;
			int								i;
			DWORD							Len, MinLen;
			CRenderer::FORMAT				Format;
			LYNXBOOL						bCompressed;

			RealOffset = 0;	

			fs.vRead(Magic, sizeof(DWORD), 1);
			fs.vRead(&DDSHeader, sizeof(DDSHEADER), 1);
			RealOffset += (sizeof(DWORD)+sizeof(DDSHEADER));

			LYNX_BYTE_ORDER(&DDSHeader.dwWidth, sizeof(DDSHeader.dwWidth));
			LYNX_BYTE_ORDER(&DDSHeader.dwHeight, sizeof(DDSHeader.dwHeight));	
			LYNX_BYTE_ORDER(&DDSHeader.dwDepth, sizeof(DDSHeader.dwDepth));	
			
			int Width = DDSHeader.dwWidth;
			int Height = DDSHeader.dwHeight;	
			int Depth = DDSHeader.dwDepth;	

			bCompressed = LYNX_FALSE;
			switch (DDSHeader.ddspf.dwFourCC)
			{
				case MAKEFOURCC('D','X','T','1'):
					MinLen = 8;
					Format = CRenderer::FORMAT_DXT1;					
					bCompressed = LYNX_TRUE;
					break;
				case MAKEFOURCC('D','X','T','2'):
					MinLen = 8;
					Format = CRenderer::FORMAT_DXT2;					
					bCompressed = LYNX_TRUE;
					break;
				case MAKEFOURCC('D','X','T','3'): 
					MinLen = 16;
					Format = CRenderer::FORMAT_DXT3; 
					bCompressed = LYNX_TRUE;
					break;
				case MAKEFOURCC('D','X','T','4'): 
					MinLen = 16;
					Format = CRenderer::FORMAT_DXT4; 
					bCompressed = LYNX_TRUE;
					break;
				case MAKEFOURCC('D','X','T','5'): 
					MinLen = 16;
					Format = CRenderer::FORMAT_DXT5;
					bCompressed = LYNX_TRUE;
					break;
				case MAKEFOURCC('A','T','I','1'): 
					MinLen = 8;
					Format = CRenderer::FORMAT_ATI1N;					 
					bCompressed = LYNX_TRUE;
					break;
				case MAKEFOURCC('A','T','I','2'): 
					MinLen = 16;
					Format = CRenderer::FORMAT_ATI2N;					
					bCompressed = LYNX_TRUE;
					break;
				case 34:  
					MinLen = 4;
					Format = CRenderer::FORMAT_G16R16; 
					break;
				case 36:  
					MinLen = 8;
					Format = CRenderer::FORMAT_A16B16G16R16; 
					break;
				case 111: 
					MinLen = 2;
					Format = CRenderer::FORMAT_R16F; 
					break;
				case 112: 
					MinLen = 4;
					Format = CRenderer::FORMAT_G16R16F; 
					break;
				case 113: 
					MinLen = 8;
					Format = CRenderer::FORMAT_A16B16G16R16F; 
					break;
				case 114: 
					MinLen = 4;
					Format = CRenderer::FORMAT_R32F; 
					break;
				case 115: 
					MinLen = 8;
					Format = CRenderer::FORMAT_G32R32F; 
					break;
				case 116: 
					MinLen = 16;
					Format = CRenderer::FORMAT_A32B32G32R32F; 
					break;
				default:
					switch (DDSHeader.ddspf.dwRGBBitCount)
					{
						case 8: 
							MinLen = 1;
							Format = CRenderer::FORMAT_L8; 
							break;
						case 16:
							MinLen = 2;
							Format = (DDSHeader.ddspf.dwABitMask == 0xF000)? CRenderer::FORMAT_A4R4G4B4 : 
									 (DDSHeader.ddspf.dwABitMask == 0xFF00)? CRenderer::FORMAT_A8L8 : 
									 (DDSHeader.ddspf.dwBBitMask == 0x1F)? CRenderer::FORMAT_R5G6B5 : CRenderer::FORMAT_L16;
							break;
						case 24: 
							MinLen = 3;
							Format = CRenderer::FORMAT_X8R8G8B8; 
							break;
						case 32:
							MinLen = 4;
							Format = (DDSHeader.ddspf.dwRBitMask == 0x3FF00000)? CRenderer::FORMAT_A2B10G10R10 : CRenderer::FORMAT_A8R8G8B8;
							break;				
					}
					break;
			}

			m_VideoMemoryUsage = 0;
			if (DDSHeader.dwHeaderFlags & DDS_SURFACE_FLAGS_CUBEMAP)
			{
			}
			else if (DDSHeader.dwHeaderFlags & DDS_SURFACE_FLAGS_TEXTURE)
			{
				if (mipmaplevel <= DDSHeader.dwMipMapCount)
				{
					if (DDSHeader.dwMipMapCount == 0 || DDSHeader.dwMipMapCount == 1)
						Error = vCreate(name, DDSHeader.dwWidth, DDSHeader.dwHeight, 1, type, Format);
					else
						Error = vCreate(name, DDSHeader.dwWidth, DDSHeader.dwHeight, mipmaplevel, type, Format);
				}
				else
					Error = vCreate(name, DDSHeader.dwWidth, DDSHeader.dwHeight, DDSHeader.dwMipMapCount, type, Format);		

				if (!Error)		
					return LYNX_FALSE;	      	
			}	
			if (bCompressed)
				Len = ((Width + 3) >> 2) * ((Height + 3) >> 2);
			else
				Len = Width * Height;
			Len = Len * MinLen;	
			if (Depth)
				Len *= Depth;
			if (DDSHeader.dwHeaderFlags & DDS_SURFACE_FLAGS_CUBEMAP)
				Len *= 6;	
			LoadDDS(fs, Len);
			RealOffset += Len;	

			if (mipmaplevel == 0)
			{	
				for (i=1; i<DDSHeader.dwMipMapCount; i++)
				{
					Len = Len/4;
					if (Len < MinLen)
						Len = MinLen;				
					vGetLevel(0, i);
					LoadDDS(fs, Len);		
					RealOffset += Len;
				}				
				vGetLevel(0, 0);
			}	
			else
			{
				if (mipmaplevel <= DDSHeader.dwMipMapCount)
				{
					for (i=1; i<DDSHeader.dwMipMapCount; i++)
					{
						Len = Len/4;
						if (Len < MinLen)
							Len = MinLen;
						if (i < mipmaplevel)
						{
							vGetLevel(0, i);
							LoadDDS(fs, Len);		
						}
						else
						{
							fs.vSeek(Len, CStream::SeekCur);										
						}
						RealOffset += Len;
					}
					vGetLevel(0, 0);
				}
				else
				{
					for (i=1; i<DDSHeader.dwMipMapCount; i++)
					{
						Len = Len/4;
						if (Len < MinLen)
							Len = MinLen;					
						vGetLevel(0, i);
						LoadDDS(fs, Len);		
						RealOffset += Len;;
					}
					vGetLevel(0, 0);
				}
			}

			m_bLoaded = LYNX_TRUE;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Texture::vLoad(CStream& fs, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc)
		{
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

			LYNXCHAR				Path[256], Name[256], Ext[16];				
			
			if ((lynxStrLen(filename) == 0))
			{
				return LYNX_FALSE;
			}
			lynxSeparateFileExtByPath(Name, Ext, filename);									    
			
			// Read TGA texture instead of PSD format.
			if (!lynxStriCmp(Ext, _T("psd")))
				lynxStrCpy(Ext, _T("tga"));

			// Use dds texture wwhenever it's possible
			lynxStrCpy(Path, Name);
			lynxStrCat(Path, _T(".dds"));
			//if (fs.vOpen(Path, CStream::Read|CStream::Binary))
			if (m_lpEngine->GetlpFileSystem()->FindFile(Path))
			{
				//fs.vClose();
				lynxStrCpy(Ext, _T("dds"));
			}
			else
			{
				lynxStrCpy(Path, Name);
				lynxStrCat(Path, _T("."));
				lynxStrCat(Path, Ext);			

				if (m_lpEngine->GetlpFileSystem()->FindFile(Path))
				{
					//fs.vClose();
				}
				else
				{
					lynxStrCpy(Path, Name);
					lynxStrCat(Path, _T(".tga"));				
					if (m_lpEngine->GetlpFileSystem()->FindFile(Path))
					{
						//fs.vClose();
						lynxStrCpy(Ext, _T("tga"));
					}
				}
			}

			lynxStrCpy(Path, Name);
			lynxStrCat(Path, _T("."));
			lynxStrCat(Path, Ext);			

			m_FullPath = Path;				
			lynxStrLower(Ext);
			m_Ext = Ext;

			/*
			if (m_bDelayLoading)
			{
				m_FullPath = _T("../texture/checker.png");
				m_Ext = "png";
			}
			*/

			CStream* stream = &fs;
			
			/*
			OPEN_Z_FILE(stream, m_FullPath.c_str())
			else
			{
				if (!stream->vOpen(m_FullPath.c_str(), CStream::Binary|CStream::Read))
				{
					m_bLoaded = LYNX_TRUE;
					LYNX_LOG(glpLogger, CString(_T("Can't find texture <"))+Path+CString(_T(">\n")));
					return LYNX_FALSE;			
				}
			}
			*/

			PACKAGE_OPEN_FILE(stream, m_FullPath, CStream::Read | CStream::Binary)
			if (!bResult)
			{
				m_bLoaded = LYNX_TRUE;
				LYNX_LOG(glpLogger, CString(_T("Can't find texture <")) + Path + CString(_T(">\n")));
				return LYNX_FALSE;
			}

			LYNXBOOL bLoadTexture = LYNX_FALSE;
			m_bLoaded = LYNX_FALSE;
			if (!lynxStriCmp(m_Ext, _T("png")))
				bLoadTexture = LoadTexturePNG((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);
			else if (!lynxStriCmp(m_Ext, _T("tga")))
				bLoadTexture = LoadTextureTGA((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);
			else if (!lynxStriCmp(m_Ext, _T("dds")))
				bLoadTexture = LoadTextureDDS((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);
			else
				bLoadTexture = LoadTextureOthers((*stream), m_FullPath.c_str(), m_MipMapLevel, m_TextureType);				
						
			(*stream).vClose();

			if (bLoadTexture)
			{
				LYNX_LOG(glpLogger, CString(_T("Loaded texture <"))+Path+CString(_T(">\n")));
				return LYNX_TRUE;
			}
			else
			{
				LYNX_LOG(glpLogger, CString(_T("Can't Loaded texture <"))+Path+CString(_T(">\n")));
				return LYNX_TRUE;
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Texture::vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format)
		{
			D3DLOCKED_RECT					LockedRect;			
			DWORD							NumMip;
			DWORD							Usage;
			LPDIRECT3DSURFACE9				lpSurface;
			D3DSURFACE_DESC					Desc;
			D3DFORMAT						PixelFormat;
			int								i, j;
			BYTE							*lpB;
			LYNXBOOL						bDepthTexture;

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
			m_lpSrcBuffer->SetName(CString(name)+_T(".SrcBuffer"));
			m_lpSrcBuffer->SetlpTexture(this);
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			if (m_lpTexture)
			{
				m_lpTexture->Release();
				m_lpTexture = NULL;
			}

			switch (type)
			{
				case CRenderer::TT_2D_TEXTURE:
					PixelFormat = CD3D9Renderer::TranslateFormat(format);
					Usage = 0;
					if (bDepthTexture)
						Usage |= D3DUSAGE_DEPTHSTENCIL;
					if (FAILED(lpR->GetlpD3DDevice()->CreateTexture(w, h, mipmaplevel, Usage, PixelFormat, D3DPOOL_MANAGED, (IDirect3DTexture9**)&m_lpTexture, NULL)))		
					{       
						return LYNX_FALSE;
					}		
					vGetLevel(0, 0);
					//glpRenderer->TextureManager.UsedTextureMemory += ((width*height*lpBuffer->bpp)>>3);		
					break;

				case CRenderer::TT_CUBE_TEXTURE:
					PixelFormat = CD3D9Renderer::TranslateFormat(format);
					Usage = 0;
					if(FAILED(lpR->GetlpD3DDevice()->CreateCubeTexture(w, mipmaplevel, Usage, PixelFormat, D3DPOOL_DEFAULT, (IDirect3DCubeTexture9**)&m_lpTexture, NULL)))
					{       
						return LYNX_FALSE;
					}
					vGetLevel(0, 0);
					break;							
			}			
			SetName(name);			

			/*
			if (!lynxCheckPowerOf2(w, 18) || !lynxCheckPowerOf2(h, 18))
			{
				m_MipMapLevel = 1;
			}
			*/

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9Texture::vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable)
		{
			D3DLOCKED_RECT					LockedRect;			
			DWORD							NumMip;
			DWORD							Usage;
			LPDIRECT3DSURFACE9				lpSurface;
			D3DSURFACE_DESC					Desc;
			D3DFORMAT						PixelFormat;
			int								i, j;
			BYTE							*lpB;
			LYNXBOOL						bDepthTexture;

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
			m_lpSrcBuffer->SetName(CString(name)+_T(".SrcBuffer"));
			m_lpSrcBuffer->SetlpTexture(this);
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			switch (type)
			{
				case CRenderer::TT_2D_TEXTURE:
					PixelFormat = CD3D9Renderer::TranslateFormat(format);					
					if (bDepthTexture)
						Usage = D3DUSAGE_DEPTHSTENCIL;
					else
						Usage = D3DUSAGE_RENDERTARGET;					
					if (FAILED(lpR->GetlpD3DDevice()->CreateTexture(w, h, 1, Usage, PixelFormat, D3DPOOL_DEFAULT, (IDirect3DTexture9**)&m_lpTexture, NULL)))		
					{       
						return LYNX_FALSE;
					}		
					vGetLevel(0, 0);
					//glpRenderer->TextureManager.UsedTextureMemory += ((width*height*lpBuffer->bpp)>>3);		
					break;

				case CRenderer::TT_CUBE_TEXTURE:
					PixelFormat = CD3D9Renderer::TranslateFormat(format);
					Usage = D3DUSAGE_RENDERTARGET;
					if(FAILED(lpR->GetlpD3DDevice()->CreateCubeTexture(w, 1, Usage, PixelFormat, D3DPOOL_DEFAULT, (IDirect3DCubeTexture9**)&m_lpTexture, NULL)))
					{       
						return LYNX_FALSE;
					}
					vGetLevel(0, 0);
					break;							
			}			
			if (ms > 1)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpMSSrcBuffer, LynxEngine::BUFFER);
				m_lpMSSrcBuffer->vCreate(NULL, w, h, format, ms, blockable);
				m_lpMSSrcBuffer->SetName(CString(name)+_T(".MSSrcBuffer"));
			}
			if (bDepthTexture && lpzb)
			{
				(*lpzb) = m_lpSrcBuffer;
			}
			else
			{
				if (lpzb)
					(*lpzb)->vCreate(NULL, w, h, zfmt, ms, blockable);
			}			

			SetName(name);
			if (ms > 1)
				m_VideoMemoryUsage = w * h * CRenderer::GetFormatBytes(format) * ms;
			else
				m_VideoMemoryUsage = w * h * CRenderer::GetFormatBytes(format);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBuffer* CD3D9Texture::vSetAsRT(int channel) 
		{
			return m_lpGraphicsSystem->GetlpRenderer()->SetRenderBuffer(channel, GetSrcBuffer());
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9Texture::vSet(int channel) 
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
			}
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			LYNXBOOL bSRGB = (m_lpGraphicsSystem->m_Config.bGammaCorrection && m_bSRGB);
			lpR->SetSRGBRead(channel, bSRGB); 			
			lpR->GetlpD3DDevice()->SetTexture(channel, (IDirect3DBaseTexture9*)m_lpTexture); 							
		}
	}
}

#endif