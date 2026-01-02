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

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__OGLES3__)) || !defined(__USED_RENDERER_ONLY__) )

#ifdef __WIN32__
	typedef char					GLchar;
#endif

#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Renderer.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3PixelShader.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3PixelShader::COGLES3PixelShader(CGraphicsSystem *pgs)
		:CPixelShader(pgs)
		{	
			m_GLHandle = 0;	
			m_Program = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3PixelShader::~COGLES3PixelShader(void)
		{
			if (m_GLHandle)
			{
				GL_TRACE(glDeleteShader(m_GLHandle);)
				m_GLHandle = 0;
			}			
			m_Program = 0;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3PixelShader::vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def, CStream* stream)
		{
            CStream* Stream = stream;
			CString FileName;	
            CAnsiString AnsiShaderSource;			
            CAnsiString Defines;
			LYNXCHAR Name[256], Ext[32];
			
			lynxSeparateFileExtByPath(Name, Ext, filename);	
			lynxStrCat(Name, _T(".psh"));		
			FileName = m_lpGraphicsSystem->GetlpRenderer()->GetShaderSrcPath() + CString(_T("ps/")) + Name;	
			
            if (!Stream)
                Stream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
            
			if (!Stream->vOpen(FileName, (CStream::Read|CStream::Binary)))
            {
				if (Stream)
					m_lpEngine->GetlpFileSystem()->GetPackage()->DiscardStream(Stream);

                LYNX_LOG(glpLogger, CString("Can't find shader <")+FileName+CString(">\n")); 
				return LYNX_FALSE;
            }
			
			DWORD ShaderSize = Stream->vSize();
            char* ShaderSource = LYNXCORE_NEW char[ShaderSize+1];
			Stream->vRead(ShaderSource, sizeof(char), ShaderSize);
			Stream->vClose();
            ShaderSource[ShaderSize] = 0x00;
            
            if (def)
            {
                for (int i=0; i<def->size(); i++)
                {
                    Defines += CAnsiString("#define ") + (*def)[i].Name + CAnsiString(" ") + (*def)[i].Definition + CAnsiString("\r\n");
                }
            }            
            AnsiShaderSource = Defines + ShaderSource;  
            LYNXCORE_DEL_ARRAY(ShaderSource);
			LYNX_LOG(glpLogger, AnsiShaderSource + CString("\n"));
           
			const char* Source = (const char*)AnsiShaderSource.c_str();
			
			GL_TRACE(m_GLHandle = glCreateShader(GL_FRAGMENT_SHADER);)			
			GL_TRACE(glShaderSource(m_GLHandle, 1, &Source, NULL);)
			GL_TRACE(glCompileShader(m_GLHandle);)			
			
			#if defined(_DEBUG) || defined(DEBUG)
				GLint logLength;
				GL_TRACE(glGetShaderiv(m_GLHandle, GL_INFO_LOG_LENGTH, &logLength);)
				if (logLength > 0)
				{
					GLchar *log = (GLchar *)malloc(logLength);
					GL_TRACE(glGetShaderInfoLog(m_GLHandle, logLength, &logLength, log);)
					LYNX_LOG(glpLogger, CString("[Error] Shader Error(")+CString(Name)+CString("): ")+ CString(log));
					free(log);
				}
			#endif
			
			GLint Status;
			GL_TRACE(glGetShaderiv(m_GLHandle, GL_COMPILE_STATUS, &Status);)
			if (Status == 0)
			{
				if (Stream)
					m_lpEngine->GetlpFileSystem()->GetPackage()->DiscardStream(Stream);

				GL_TRACE(glDeleteShader(m_GLHandle);)
				return LYNX_FALSE;
			}		
			
			if (Stream)
				m_lpEngine->GetlpFileSystem()->GetPackage()->DiscardStream(Stream);

			return LYNX_TRUE;			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSet() 
		{ 				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantF(const char *name, const float f) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1f(Location, f);		)		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantF(const char *name, const float *lpdata, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1fv(Location, num, (const GLfloat*)lpdata);	)		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantI(const char *name, const int i) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1i(Location, i);		)	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantI(const char *name, const int *lpdata, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1iv(Location, num, lpdata);)				
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantB(const char *name, const BOOL b) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1i(Location, b);	)		
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantB(const char *name, const BOOL *lpdata, int num) 
		{
			int* Bool = LYNXCORE_NEW int[num];
			
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			
			for (int i=0; i<num; i++)
				Bool[i] = lpdata[i];
			
			GL_TRACE(glUniform1iv(Location, num, Bool);			)

			LYNXCORE_DEL_ARRAY(Bool);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantV2(const char *name, const float *lpdata) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform2f(Location, lpdata[0], lpdata[1]);			)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantV2(const char *name, const float *lpdata, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform2fv(Location, num, lpdata);)			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantV3(const char *name, const float *lpdata) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform3f(Location, lpdata[0], lpdata[1], lpdata[2]);	)		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantV3(const char *name, const float *lpdata, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform3fv(Location, num, (const GLfloat*)lpdata);	)		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantV4(const char *name, const float *lpdata) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform4f(Location, lpdata[0], lpdata[1], lpdata[2], lpdata[3]);		)	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantV4(const char *name, const float *lpdata, int num) 
		{
			int* Bool = LYNXCORE_NEW int[num];
			
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform4fv(Location, num, (const GLfloat*)lpdata);		)	

			LYNXCORE_DEL_ARRAY(Bool);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantM(const char *name, LPCLYNXMATRIX lpm) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniformMatrix4fv(Location, 1, GL_FALSE, (const GLfloat*)lpm);)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetConstantM(const char *name, LPCLYNXMATRIX lpm, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniformMatrix4fv(Location, num, GL_FALSE, (const GLfloat*)lpm);)
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetTexture(const int channel, CTexture* t)  
		{
			GL_TRACE(COGLES3ShaderGroup* lpSG = (COGLES3ShaderGroup*)(m_lpGraphicsSystem->GetlpRenderer()->GetCachedShaderGroup());		)	
			#if defined(_DEBUG) || defined(DEBUG)
				if(channel < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find texture at ") + CString(name));
				}				
			#endif
			if (channel >= 0)
			{				
				int Index = lpSG->GetTextureChannel();
                m_lpGraphicsSystem->GetlpRenderer()->SetTexture(Index, t);				
				GL_TRACE(glUniform1i(channel, Index);)
				lpSG->UseTextureChannel();
			}
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetTexture(const char *name, CTexture* t)  
		{
			if (m_ResourceMap[name])
				m_ResourceMap[name]->Set(t);
			/*
            else
			{
				COGLES3ShaderGroup* lpSG = (COGLES3ShaderGroup*)(m_lpGraphicsSystem->GetlpRenderer()->GetCachedShaderGroup());			
				GLint Location = glGetUniformLocation(m_Program, name);	
				#if defined(_DEBUG) || defined(DEBUG)
					if(Location < 0)
					{
						LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find texture : ") + CString(name));
					}				
				#endif
				if (Location >= 0)
				{				
					int Index = lpSG->GetTextureChannel();
					// Hack code : to reset sampler every time 
					m_lpGraphicsSystem->GetlpRenderer()->SetTexture(Index, t);
					glUniform1i(Location, Index);
					lpSG->UseTextureChannel();
				}
			}
            */
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3PixelShader::vSetSampler(const char *name, CSamplerState* lps)
		{ 
			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int COGLES3PixelShader::vGetTextureIndex(const char *name)
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	

			return Location;
		}
	}	
}

#endif