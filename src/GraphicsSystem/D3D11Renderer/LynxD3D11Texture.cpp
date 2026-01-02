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
#include <LynxFileStream.h>
#ifdef __FREEIMAGE__
	#include <FreeImage.h>
#endif
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Buffer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Texture.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{		
	namespace GraphicsSystem 
	{		
		////-------------------------------------------------------------------------------------------------------
		////
		////  說明:   
		////-------------------------------------------------------------------------------------------------------
		//int CD3D9Renderer::TranslateTextureType(TEXTURETYPE type)
		//{
		//	switch (type)
		//	{
		//		case TT_2D_TEXTURE:
		//			return LYNX_NORMAL_TEXTURE;
		//			break;

		//		case TT_CUBE_TEXTURE:
		//			return LYNX_CUBE_TEXTURE;
		//			break;

		//		default:
		//			return LYNX_NORMAL_TEXTURE;
		//			break;
		//	}
		//}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Texture::CD3D11Texture(CGraphicsSystem *pgs)
		: CTexture(pgs)
		{	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Texture::~CD3D11Texture(void)
		{
			LYNXDEL m_lpSrcBuffer;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Texture::LoadTGA(CStream& stream, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc)
		{
			TGAHEADER TGAHeader;
			CRenderer::FORMAT Format;
			BYTE* lpSrc = NULL;
			BYTE *lpMem, *lpOMem;
			BYTE* lpB;
			D3D11_SUBRESOURCE_DATA Data;

			if ( (stream.vRead(&TGAHeader, 1, sizeof(TGAHEADER)) != sizeof (TGAHEADER)) ) 
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
			stream.vSeek(TGAHeader.idlen, CStream::SeekCur);

			switch (TGAHeader.imdepth)
			{
				case 8:
					Format = CRenderer::FORMAT_P8;
					break;
				case 32:
					Format = CRenderer::FORMAT_A8R8G8B8;
					break;
				case 24:
					Format = CRenderer::FORMAT_A8R8G8B8;
					break;
			}

			switch (Format)
			{
				case CRenderer::FORMAT_A8R8G8B8:	
					lpSrc = (BYTE *)LYNXNEW BYTE[TGAHeader.imwidth*TGAHeader.imheight*4];
					if (TGAHeader.imdepth == 24)
					{		
						lpOMem = lpMem = (BYTE *)LYNXNEW BYTE[TGAHeader.imwidth*TGAHeader.imheight*3];
						stream.vRead(lpMem, sizeof(BYTE), TGAHeader.imwidth*TGAHeader.imheight*3);										
						if (TGAHeader.imdesc & 0x20)
						{
							for (int y=0; y<TGAHeader.imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y) * (TGAHeader.imwidth*4) );
								for (int x=0; x<TGAHeader.imwidth; x++)
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
							for (int y=0; y<TGAHeader.imheight; y++)												
							{	
								lpB = ( ((BYTE *)(lpSrc)) + (TGAHeader.imheight - y - 1) * (TGAHeader.imwidth*4) );						
								for (int x=0; x<TGAHeader.imwidth; x++)												
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
						LYNXDEL [] lpOMem;
					}
					else if (TGAHeader.imdepth == 32)
					{				
						lpOMem = lpMem = (BYTE *)LYNXNEW BYTE[TGAHeader.imwidth*TGAHeader.imheight*4];
						stream.vRead(lpMem, sizeof(BYTE), TGAHeader.imwidth*TGAHeader.imheight*4);										
						if (TGAHeader.imdesc & 0x20)
						{
							for (int y=0; y<TGAHeader.imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y) * (TGAHeader.imwidth*4) );
								for (int x=0; x<TGAHeader.imwidth; x++)
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
							for (int y=0; y<TGAHeader.imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (TGAHeader.imheight - y - 1) * (TGAHeader.imwidth*4) );
								for (int x=0; x<TGAHeader.imwidth; x++)
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
						LYNXDEL [] lpOMem;
					}
					Data.pSysMem = lpSrc;
					Data.SysMemPitch = TGAHeader.imwidth*4;
					break;
				
				case CRenderer::FORMAT_R8G8B8:	
					lpSrc = (BYTE *)LYNX_MALLOC(TGAHeader.imwidth*TGAHeader.imheight*3);				
					if (TGAHeader.imdepth == 24)
					{
						lpOMem = lpMem = (BYTE *)LYNXNEW BYTE[TGAHeader.imwidth*TGAHeader.imheight*3];
						stream.vRead(lpMem, sizeof(BYTE), TGAHeader.imwidth*TGAHeader.imheight*3);										
						if (TGAHeader.imdesc & 0x20)
						{
							for (int y=0; y<TGAHeader.imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y) * (TGAHeader.imwidth*3) );
								for (int x=0; x<TGAHeader.imwidth; x++)
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
							for (int y=0; y<TGAHeader.imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (TGAHeader.imheight - y - 1) * (TGAHeader.imwidth*3) );
								for (int x=0; x<TGAHeader.imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);

									lpB += 3;
									lpMem += 3;	
								}
							}			
						}
						LYNXDEL [] lpOMem;
					}
					else if (TGAHeader.imdepth == 32)
					{
						lpOMem = lpMem = (BYTE *)LYNX_MALLOC(TGAHeader.imwidth*TGAHeader.imheight*4);
						stream.vRead(lpMem, sizeof(BYTE), TGAHeader.imwidth*TGAHeader.imheight*4);										
						if (TGAHeader.imdesc & 0x20)
						{
							for (int y=0; y<TGAHeader.imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (y) * (TGAHeader.imwidth*3) );
								for (int x=0; x<TGAHeader.imwidth; x++)
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
							for (int y=0; y<TGAHeader.imheight; y++)
							{
								lpB = ( (BYTE *)(lpSrc) + (TGAHeader.imheight - y - 1) * (TGAHeader.imwidth*3) );
								for (int x=0; x<TGAHeader.imwidth; x++)
								{
									*(lpB+0) = *(lpMem+2);
									*(lpB+1) = *(lpMem+1);
									*(lpB+2) = *(lpMem+0);

									lpB += 3;
									lpMem += 4;								
								}
							}
						}					
						LYNXDEL [] lpOMem;
					}
					Data.pSysMem = lpSrc;
					Data.SysMemPitch = TGAHeader.imwidth*3;
					break;
					
				case CRenderer::FORMAT_A4R4G4B4:						
				case CRenderer::FORMAT_A1R5G5B5:	
					/*lpSrc = (BYTE *)LYNX_MALLOC(TGAHeader.imwidth*TGAHeader.imheight*2);
					if (TGAHeader.imdepth == 24)
					{					
						lpOMem = lpMem = (BYTE *)LYNXNEW BYTE[TGAHeader.imwidth*TGAHeader.imheight*3];
						stream.vRead(lpMem, sizeof(BYTE), TGAHeader.imwidth*TGAHeader.imheight*3);										
						if (TGAHeader.imdesc & 0x20)
						{
							for (y=0; y<TGAHeader.imheight; y++)
							{
								lpW = (WORD *)( (BYTE *)(lpSrc) + (y) * ((CD3D9Buffer*)((CBuffer*)m_lpSrcBuffer))->m_lPitch);							
								for (x=0; x<TGAHeader.imwidth; x++)
								{
									*lpW = (WORD)lynxConvertRGBFromColorFormat(lpb->Format, *(lpMem+2), *(lpMem+1), *(lpMem), 255);						
									lpW++;
									lpMem += 3;								
								}
							}
						}
						else
						{
							for (y=0; y<TGAHeader.imheight; y++)
							{
								lpW = (WORD *)( (BYTE *)(lpSrc) + (TGAHeader.imheight - y - 1) * lpb->lPitch);							
								for (x=0; x<TGAHeader.imwidth; x++)
								{
									*lpW = (WORD)lynxConvertRGBFromColorFormat(lpb->Format, *(lpMem+2), *(lpMem+1), *(lpMem), 255);						
									lpW++;
									lpMem += 3;								
								}
							}			
						}					
						LYNX_FREE(lpOMem);
					}
					else if (TGAHeader.imdepth == 32)
					{
						lpOMem = lpMem = (BYTE *)LYNX_MALLOC(TGAHeader.imwidth*TGAHeader.imheight*4);
						LYNX_READ_FILE(lpMem, sizeof(BYTE), TGAHeader.imwidth*TGAHeader.imheight*4, fp);										
						if (TGAHeader.imdesc & 0x20)
						{
							for (y=0; y<TGAHeader.imheight; y++)
							{
								lpW = (WORD *)( (BYTE *)(lpSrc) + (y) * lpb->lPitch);							
								for (x=0; x<TGAHeader.imwidth; x++)
								{
									*lpW = (WORD)lynxConvertRGBFromColorFormat(lpb->Format, *(lpMem+2), *(lpMem+1), *(lpMem), *(lpMem+3));						
									lpW++;
									lpMem += 4;								
								}
							}
						}
						else
						{
							for (y=0; y<TGAHeader.imheight; y++)
							{
								lpW = (WORD *)( (BYTE *)(lpSrc) + (TGAHeader.imheight - y - 1) * lpb->lPitch);							
								for (x=0; x<TGAHeader.imwidth; x++)
								{
									*lpW = (WORD)lynxConvertRGBFromColorFormat(lpb->Format, *(lpMem+2), *(lpMem+1), *(lpMem), *(lpMem+3));						
									lpW++;
									lpMem += 4;								
								}
							}
						}					
						LYNX_FREE(lpOMem);
					}			*/
					break;

				case CRenderer::FORMAT_P8:						
					break;
			}

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
			Create(filename, Width, Height, m_MipMapLevel, TextureType, Format, 1, D3D11_BIND_SHADER_RESOURCE, &Data);

			if (lpSrc)
				LYNXDEL [] lpSrc;

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Texture::vLoad(CStream& stream, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc)
		{		
			#ifdef __FREEIMAGE__
				FreeImageIO IOData;

				IOData.read_proc	= FreeImage_ReadFileProc;
				IOData.write_proc	= FreeImage_WriteFileProc;
				IOData.tell_proc	= FreeImage_TellFileProc;
				IOData.seek_proc	= FreeImage_SeekFileProc;
			#endif

			LYNXCHAR				Path[256], Name[256], Ext[16];				
			
			if (desc)
			{
				m_MipMapLevel		= desc->MipMapLevel;
				//TextureType			= desc->Type;
				m_bSRGB				= desc->bSRGB;
				m_LODBias			= desc->bSRGB;					
				m_AlphaReference	= desc->AlphaReference;
			}
			else
			{
				m_MipMapLevel	= 0;
				//TextureType		= CRenderer::TT_2D_TEXTURE;
			}

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
			if (stream.vOpen(Path, CStream::Read|CStream::Binary))
			{
				stream.vClose();
				lynxStrCpy(Ext, _T("dds"));
			}
			else
			{
				lynxStrCpy(Path, Name);
				lynxStrCat(Path, _T("."));
				lynxStrCat(Path, Ext);			

				if (stream.vOpen(Path, CStream::Read|CStream::Binary))
				{
					stream.vClose();
				}
				else
				{
					lynxStrCpy(Path, Name);
					lynxStrCat(Path, _T(".png"));				
					if (stream.vOpen(Path, CStream::Read|CStream::Binary))
					{
						stream.vClose();
						lynxStrCpy(Ext, _T("tga"));
					}
				}
			}

			lynxStrCpy(Path, Name);
			lynxStrCat(Path, _T("."));
			lynxStrCat(Path, Ext);			
						
			if (!stream.vOpen(Path))
				return LYNX_FALSE;

			if (!lynxStriCmp(Ext, _T("tga")))
			{
				if (!LoadTGA(stream, filename, desc))
				{
					stream.vClose();
					return LYNX_FALSE;
				}
				stream.vClose();					
			}
			else
			{			
				#ifdef __FREEIMAGE__
					FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromHandle(&IOData, &stream);
					if (fif == FIF_UNKNOWN) 
					{		
						if (!lynxStriCmp(Ext, _T("tga")))
						{
							if (!LoadTGA(stream, filename, desc))
							{
								stream.vClose();
								return LYNX_FALSE;
							}
							stream.vClose();					
						}
					}
					else
					{
						CRenderer::FORMAT Format;
						D3D11_SUBRESOURCE_DATA Data;
						FIBITMAP *dib = FreeImage_LoadFromHandle(fif, &IOData, &stream);
						if (dib)
						{
							FREE_IMAGE_TYPE Type = FreeImage_GetImageType(dib);
							int bpp = FreeImage_GetBPP(dib);
							int w = FreeImage_GetWidth(dib);
							int h = FreeImage_GetHeight(dib);
							BYTE* lpDIBData = FreeImage_GetBits(dib);	
							BYTE* lpData = LYNXNEW BYTE[w*h*4];
							BYTE* lpDIBC = lpDIBData;
							BYTE* lpC = lpData;
							switch (bpp)
							{
								case 32:
									Format = CRenderer::FORMAT_A8R8G8B8;
									for (int y=h-1; y>=0; y--)
									{
										lpDIBC = lpDIBData + (y*w*4);
										for (int x=0; x<w; x++)
										{
											*(lpC+0) = *(lpDIBC+2);
											*(lpC+1) = *(lpDIBC+1);
											*(lpC+2) = *(lpDIBC+0);
											*(lpC+3) = *(lpDIBC+3);

											lpDIBC += 4;
											lpC += 4;
										}
									}
									Data.pSysMem = lpData;
									Data.SysMemPitch = w*bpp>>3;
									break;

								case 24:
									Format = CRenderer::FORMAT_R8G8B8;
									Format = CRenderer::FORMAT_A8R8G8B8;
									bpp = 32;
									for (int y=h-1; y>=0; y--)
									{
										lpDIBC = lpDIBData + (y*w*3);
										for (int x=0; x<w; x++)
										{
											*(lpC+0) = *(lpDIBC+2);
											*(lpC+1) = *(lpDIBC+1);
											*(lpC+2) = *(lpDIBC+0);
											*(lpC+3) = 255;

											lpDIBC += 3;
											lpC += 4;
										}
									}
									Data.pSysMem = lpData;
									Data.SysMemPitch = w*bpp>>3;
									break;
							}

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
							Create(filename, w, h, m_MipMapLevel, TextureType, Format, 1, D3D11_BIND_SHADER_RESOURCE, &Data);

							LYNXDEL [] lpData;
							// free the loaded FIBITMAP
							FreeImage_Unload(dib);
							stream.vClose();
						}
					}
				#endif
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Texture::Create(const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, UINT bindflag, D3D11_SUBRESOURCE_DATA* data)
		{
			LYNXCHAR		Str[256];

			switch (type)
			{
				case CRenderer::TT_2D_TEXTURE:
				{
					ID3D11Texture2D* pTexture2D = NULL;
					D3D11_TEXTURE2D_DESC Desc;
					Desc.Width = w;
					Desc.Height = h;
					Desc.MipLevels = mipmaplevel;
					Desc.ArraySize = 1;
					Desc.Format = CD3D11Renderer::TranslateFormat(format, (m_bSRGB & m_lpGraphicsSystem->m_Config.bGammaCorrection));
					Desc.SampleDesc.Count = ms;
					Desc.SampleDesc.Quality = 0;
					if (bindflag & D3D11_BIND_RENDER_TARGET)
						Desc.Usage = D3D11_USAGE_DEFAULT;
					else
						Desc.Usage = D3D11_USAGE_IMMUTABLE;
					Desc.BindFlags = bindflag; 
					Desc.CPUAccessFlags = 0;
					Desc.MiscFlags = 0;
					if (Desc.MipLevels == 0)
					{						
						//Desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
						Desc.MipLevels = 1;
					}
					HRESULT hResult = ((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateTexture2D(&Desc, data, &pTexture2D);
					if (hResult != S_OK)   
					{
						lynxStrCpy(Str, DXGetErrorString(hResult));
						return LYNX_FALSE;
					}
					SetName(name);
					m_lpGraphicsSystem->GetlpEngine()->CreateResource(m_lpSrcBuffer, LynxEngine::BUFFER);										
					m_lpSrcBuffer->SetName(GetName()+CString(_T(".SrcBuffer")));
					m_lpSrcBuffer->SetlpTexture(this);
					m_lpSrcBuffer->vSetSize(w, h);
					((CD3D11Buffer*)((CResource*)m_lpSrcBuffer))->SetlpResource(pTexture2D);
				
					if (bindflag & D3D11_BIND_SHADER_RESOURCE)
					{
						ID3D11ShaderResourceView* pSRView = NULL;
						D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
						ZeroMemory(&SRVDesc, sizeof(SRVDesc));
						SRVDesc.Format = Desc.Format;
						SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						SRVDesc.Texture2D.MipLevels = Desc.MipLevels;
						((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateShaderResourceView(pTexture2D, &SRVDesc, &pSRView);
						((CD3D11Buffer*)((CResource*)m_lpSrcBuffer))->SetlpSRView(pSRView);
					}

					if (bindflag & D3D11_BIND_RENDER_TARGET)
					{
						ID3D11RenderTargetView* pRTView = NULL;
						D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
						ZeroMemory(&RTVDesc, sizeof(RTVDesc));
						RTVDesc.Format = Desc.Format;
						RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
						RTVDesc.Texture2D.MipSlice = 0;
						((CD3D11Renderer*)m_lpGraphicsSystem->GetlpRenderer())->GetlpD3DDevice()->CreateRenderTargetView(pTexture2D, &RTVDesc, &pRTView);
						((CD3D11Buffer*)((CResource*)m_lpSrcBuffer))->SetlpRTView(pRTView);
					}
					m_VideoMemoryUsage = w * h * CRenderer::GetFormatBytes(format) * ms;
				}
				break;
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11Texture::vCreateRT(CBufferPtr* lpzb, CRenderer::FORMAT zfmt, const LYNXCHAR *name, int w, int h, int mipmaplevel, CRenderer::TEXTURETYPE type, CRenderer::FORMAT format, int ms, LYNXBOOL blockable)
		{
			if (ms <= 1)
				ms = 1;

			LYNXBOOL bRet = Create(name, w, h, mipmaplevel, type, format, ms, D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET, NULL);
			if (!bRet)
				return LYNX_FALSE;
			if (lpzb)
			{
				if (m_lpGraphicsSystem->GetlpRenderer()->CreateDepthBuffer((*lpzb), w, h, zfmt, ms) != LYNX_OK)
					return LYNX_TRUE;
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CBuffer* CD3D11Texture::vSetAsRT(int channel) 
		{
			return m_lpGraphicsSystem->GetlpRenderer()->SetRenderBuffer(channel, (GetSrcBuffer()));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11Texture::vSet(int channel) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			if (m_lpGraphicsSystem->m_Config.bGammaCorrection) 
				lpR->SetSRGBRead(channel, m_bSRGB); 			
			ID3D11ShaderResourceView*  SRView[1] = {((CD3D11Buffer*)&(*m_lpSrcBuffer))->GetlpSRView()};
			lpR->GetlpDeviceContext()->VSSetShaderResources(channel, 1, SRView);
			lpR->GetlpDeviceContext()->PSSetShaderResources(channel, 1, SRView); 		
			m_SamplerState->vSet(channel);
		}
	}
}

#endif