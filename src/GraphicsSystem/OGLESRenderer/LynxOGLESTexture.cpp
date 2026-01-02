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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES__)) || !defined(__USED_RENDERER_ONLY__) )

#include <GraphicsSystem/OGLESRenderer/LynxOGLESRenderer.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESTexture.h>
#include <GraphicsSystem/OGLESRenderer/LynxOGLESBuffer.h>

#ifdef __WIN32__
	typedef DWORD					uint32_t;
#endif

#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff

static char gPVRTexIdentifier[8] = "PVR!";

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
		COGLESTexture::COGLESTexture(CGraphicsSystem *pgs)
		: CTexture(pgs)
		{	
			m_GLHandle = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLESTexture::~COGLESTexture(void)
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
		void COGLESTexture::vGetLevel(int face, int level)
		{
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESTexture::LoadTextureBMP(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
		{
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESTexture::LoadPVR(CStream& fs, PVRHEADER* lpheader)
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
				
				lpData = LYNXNEW char[DataSize];
				fs.vRead(lpData, DataSize, sizeof(char));
				glCompressedTexImage2D(GL_TEXTURE_2D, Level, m_PixelFormat, Width, Height, 0, DataSize, lpData);
				LYNXDEL_ARRAY(lpData);
				
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
		LYNXBOOL COGLESTexture::LoadTexturePVR(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
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
			
			//Yahoo!!
			return LYNX_TRUE;     
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESTexture::LoadTGA(CStream& fs, TGAHEADER* lpheader)
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
					lpSrc = (BYTE *)LYNXNEW BYTE[lpheader->imwidth*lpheader->imheight*4];
					if (lpheader->imdepth == 24)
					{		
						lpOMem = lpMem = (BYTE *)LYNXNEW BYTE[lpheader->imwidth*lpheader->imheight*4];
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
						LYNXDEL_ARRAY(lpOMem);
					}
					else if (lpheader->imdepth == 32)
					{				
						lpOMem = lpMem = (BYTE *)LYNXNEW BYTE[lpheader->imwidth*lpheader->imheight*4];
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
						LYNXDEL_ARRAY(lpOMem);
					}
					break;
					
				case CRenderer::FORMAT_R8G8B8:	
					lpSrc = (BYTE *)LYNX_MALLOC(lpheader->imwidth*lpheader->imheight*3);				
					if (lpheader->imdepth == 24)
					{
						lpOMem = lpMem = (BYTE *)LYNXNEW BYTE[lpheader->imwidth*lpheader->imheight*3];
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
						LYNXDEL_ARRAY(lpOMem);
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
									*(lpB+0) = *(lpMem+3);
									*(lpB+1) = *(lpMem+2);
									*(lpB+2) = *(lpMem+1);									
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
						LYNXDEL_ARRAY(lpOMem);
					}
					break;
					
				case CRenderer::FORMAT_P8:						
					break;
			}		
			glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, lpheader->imwidth, lpheader->imheight, 0, ImageFormat, GL_UNSIGNED_BYTE, lpSrc);
			
			LYNXDEL_ARRAY(lpSrc);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESTexture::LoadTextureTGA(CStream& fs, const LYNXCHAR* name, int mipmap, CRenderer::TEXTURETYPE type)
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
			
			//if (mipmap == 0)
			//	glGenerateMipmap(m_GLHandle);
			
			//Yahoo!!
			return LYNX_TRUE;     
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESTexture::vLoad(CStream& fs, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc)
		{
			CRenderer::TEXTURETYPE TextureType;
			
			if (desc)
			{
				m_MipMapLevel = desc->MipMapLevel;
				TextureType = desc->Type;
				m_bSRGB = desc->bSRGB;
				m_LODBias = desc->bSRGB;					
			}
			else
			{
				m_MipMapLevel = 0;
				TextureType = CRenderer::TT_2D_TEXTURE;
			}
			
			LYNXCHAR				Name[256], Ext[256];	
			LYNXCHAR				Path[256];
			CString					FinalPath;
			
			if ((lynxStrLen(filename) == 0))
			{
				return LYNX_FALSE;
			}
			lynxSeparateFileExtByPath(Name, Ext, filename);									    
			
			// Read TGA texture instead of PSD format.
			if (!lynxStriCmp(Ext, _T("psd")))
				lynxStrCpy(Ext, _T("tga"));

			// Use pvr texture wwhenever it's possible
			lynxStrCpy(Path, Name);
			lynxStrCat(Path, _T(".pvr"));
			if (fs.vOpen(Path, CStream::Read|CStream::Binary))
			{
				fs.vClose();
				lynxStrCpy(Ext, _T("pvr"));
			}
			else
			{
				lynxStrCpy(Path, Name);
				lynxStrCat(Path, _T("."));
				lynxStrCat(Path, Ext);			

				if (fs.vOpen(Path, CStream::Read|CStream::Binary))
				{
					fs.vClose();
				}
				else
				{
					lynxStrCpy(Path, Name);
					lynxStrCat(Path, _T(".png"));				
					if (fs.vOpen(Path, CStream::Read|CStream::Binary))
					{
						fs.vClose();
						lynxStrCpy(Ext, _T("tga"));
					}
				}
			}

			lynxStrCpy(Path, Name);
			lynxStrCat(Path, _T("."));
			lynxStrCat(Path, Ext);			

			if (!fs.vOpen(Path, CStream::Read|CStream::Binary))
			{
				LYNX_LOG(glpLogger, CString(_T("Can't find texture <"))+Path+CString(_T(">\n")));
				return LYNX_FALSE;
			}			
			
			LYNXBOOL bLoadTexture = LYNX_FALSE;
			if (!lynxStriCmp(Ext, _T("bmp")))	
				bLoadTexture = LoadTextureBMP(fs, filename, m_MipMapLevel, TextureType);						
			else if (!lynxStriCmp(Ext, _T("tga")))
				bLoadTexture = LoadTextureTGA(fs, filename, m_MipMapLevel, TextureType);
			else if (!lynxStriCmp(Ext, _T("pvr")))
				bLoadTexture = LoadTexturePVR(fs, filename, m_MipMapLevel, TextureType);
			
			fs.vClose();
			
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
		LYNXBOOL COGLESTexture::vCreate(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format)
		{
			DWORD		NumMip;
			DWORD		Usage;			
			BYTE*		lpB;
			
			m_TextureType = type;
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpSrcBuffer, LynxEngine::BUFFER);
			m_lpSrcBuffer->vCreate(0, w, h, format, 0, LYNX_FALSE);
			m_lpSrcBuffer->SetName(CString(name)+_T(".SrcBuffer"));
			((COGLESBuffer*)&(*m_lpSrcBuffer))->SetBufferType(COGLESBuffer::BT_TEXTURE2D);
			
			m_PixelFormat = COGLESRenderer::TranslateFormat(format);
			
			switch (type)
			{
				case CRenderer::TT_2D_TEXTURE:					
					glGenTextures(1, &m_GLHandle);
					glBindTexture(GL_TEXTURE_2D, m_GLHandle);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					vGetLevel(0, 0);					
					//glpRenderer->TextureManager.UsedTextureMemory += ((width*height*lpBuffer->bpp)>>3);		
					break;
					
				case CRenderer::TT_CUBE_TEXTURE:
					//PixelFormat = CD3D9Renderer::TranslateFormat(format);
					//Usage = 0;
					
					vGetLevel(0, 0);
					break;							
			}			
			m_Width = w;
			m_Height = h;
			SetName(name);
			((COGLESBuffer*)&(*m_lpSrcBuffer))->SetGLHandle(m_GLHandle);
			((COGLESBuffer*)&(*m_lpSrcBuffer))->SetlpTexture(this);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLESTexture::vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable)
		{
			DWORD							NumMip;
			DWORD							Usage;
			int								i, j;
			BYTE							*lpB;
			
			m_TextureType = type;
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpSrcBuffer, LynxEngine::BUFFER);
			m_lpSrcBuffer->vCreate(0, w, h, format, 0, LYNX_FALSE);
			m_lpSrcBuffer->SetName(CString(name)+_T(".SrcBuffer"));
			((COGLESBuffer*)&(*m_lpSrcBuffer))->SetBufferType(COGLESBuffer::BT_TEXTURE2D);
			
			switch (format)
			{
				case CRenderer::FORMAT_R8G8B8:
					m_PixelFormat = GL_RGB;
					break;
				case CRenderer::FORMAT_A8R8G8B8:
					m_PixelFormat = GL_RGBA;
					break;
			}
			
			switch (type)
			{
				case CRenderer::TT_2D_TEXTURE:
					glGenTextures(1, &m_GLHandle);
					glBindTexture(GL_TEXTURE_2D, m_GLHandle);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					//vGetLevel(0, 0);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
					//glpRenderer->TextureManager.UsedTextureMemory += ((width*height*lpBuffer->bpp)>>3);		
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
			((COGLESBuffer*)&(*m_lpSrcBuffer))->SetGLHandle(m_GLHandle);
			((COGLESBuffer*)&(*m_lpSrcBuffer))->SetlpTexture(this);
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESTexture::vUpdate(const int level, const void* data)
        {
            glBindTexture(GL_TEXTURE_2D, m_GLHandle);
			glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBuffer* COGLESTexture::vSetAsRT(int channel) 
		{
			return m_lpGraphicsSystem->GetlpRenderer()->SetRenderBuffer(channel, GetSrcBuffer());
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLESTexture::vSet(int channel) 
		{ 			
			glActiveTexture(GL_TEXTURE0+channel);       				
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_GLHandle);	
		}
	}
}

#endif