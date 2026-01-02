#include <LynxEngine_PCH.h>
#include <pnginfo.h>
#include <GUISystem/LynxHitMask.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem
	{
		png_voidp PNGMalloc(png_structp png_ptr, png_size_t size);
		void PNGFree(png_structp png_ptr, png_voidp ptr);
		void PNGError(png_structp png_ptr, png_const_charp msg);
		void PNGWarning(png_structp png_ptr, png_const_charp msg);
		void PNGRead(png_structp png_ptr, png_bytep data, png_size_t length);
	}
}

namespace LynxEngine
{
	namespace GUISystem
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHitMask::CHitMask(CEngine* lpe)
		: CObj(lpe)
		{            
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CHitMask::~CHitMask()
		{
			for (int i=0; i<m_Array.size(); i++)
				m_Array[i].clear();
			m_Array.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CHitMask::CreateFromFile(const CString& filename)
		{
			png_structp png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, 
				/*error_ptr*/NULL,
				GraphicsSystem::PNGError,
				GraphicsSystem::PNGWarning,
				/*mem_ptr*/NULL, 
				GraphicsSystem::PNGMalloc,
				GraphicsSystem::PNGFree);        

			CString Path;
			LYNXBOOL bFound = LYNX_FALSE;


			CMemoryStream FileStream;
			CStream* ZStream = &FileStream;						

			for (CList<CString>::CIterator SearchPath = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTextureSearchPathList().begin(); SearchPath != m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTextureSearchPathList().end(); SearchPath++)
            {
				Path = (*SearchPath) + filename;

				OPEN_Z_FILE(ZStream, Path.c_str())
				else
				{
					ZStream->vOpen(Path, CStream::Binary|CStream::Read);					
				}

				if (ZStream->IsValid())
				{
					ZStream->vClose();
					bFound = LYNX_TRUE;
					break;
				}
			}
			if (!bFound)
			{
				Path = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath() + filename;
			}

			OPEN_Z_FILE(ZStream, Path.c_str())
			else
			{
				ZStream->vOpen(Path, CStream::Binary|CStream::Read);				
			}

			if (ZStream->IsValid())
			{
				png_infop info_ptr = png_create_info_struct(png_ptr);			
				png_set_read_fn(png_ptr, ZStream, GraphicsSystem::PNGRead);

				DWORD sig_read = 0;
				png_set_sig_bytes(png_ptr, sig_read);
  
				png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

				m_Width = info_ptr->width;
				m_Height = info_ptr->height;

				int channels = 0;
				DWORD row_bytes = png_get_rowbytes(png_ptr, info_ptr);        
				png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
				BYTE* data = (BYTE*)LYNXALLOC(row_bytes * m_Height);           

				for (int i=0; i<m_Height; ++i)
				{
					lynxMemCpy(data+(row_bytes*i), row_pointers[i], row_bytes);
				}			

				m_Array.resize(m_Width);
				for (int x=0; x<m_Width; x++)
				{
					m_Array[x].resize(m_Height);
				}

				BYTE* lpMem = data;
				if (info_ptr->color_type == PNG_COLOR_TYPE_RGBA)
				{		
					for (int y=0; y<info_ptr->height; y++)
					{
						for (int x=0; x<info_ptr->width; x++)
						{
							if(*(lpMem+3) == 255)
							{
								m_Array[x][y] = LYNX_TRUE;
							}
							else
							{
								m_Array[x][y] = LYNX_FALSE;
							}
							lpMem += 4;															
						}
					}						
				}				

				png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
				LYNXFREE(data);

				ZStream->vClose();
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		void CHitMask::CreateFromArray(const int w, const int h, const LYNXBOOL* data)
		{
		}
        //-------------------------------------------------------------------------------------------------------
		//
		//  說明:
		//-------------------------------------------------------------------------------------------------------
        LYNXBOOL CHitMask::HitTest(int x, int y, int w, int h)
        {
			CVector2 Scale((float)m_Width/(float)w, (float)m_Height/(float)h);

			int FX = LYNX_ROUND((x * Scale.x));
			int FY = LYNX_ROUND((y * Scale.y));
			if (FX < 0)
				FX = 0;
			if (FX >= m_Width)
				FX = m_Width -1;

			if (FY < 0)
				FY = 0;
			if (FY >= m_Height)
				FY = m_Height -1;

			return m_Array[FX][FY];
        }
	}
}