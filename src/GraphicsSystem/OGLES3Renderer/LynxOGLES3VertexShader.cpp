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

#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3Renderer.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3VertexShader.h>
#include <GraphicsSystem/OGLES3Renderer/LynxOGLES3ShaderGroup.h>

#ifdef __WIN32__
	typedef char					GLchar;
#endif

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3VertexShader::COGLES3VertexShader(CGraphicsSystem *pgs)
		: CVertexShader(pgs)
		{			
			m_GLHandle = 0;
			m_Program = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		COGLES3VertexShader::~COGLES3VertexShader(void)
		{			
			if (m_GLHandle)
			{
				glDeleteShader(m_GLHandle);
				m_GLHandle = 0;
			}
			m_Program = 0;
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::Release()
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3VertexShader::LoadCompiledShader(CStream& fs, const LYNXCHAR *filename)
		{
			int								i;
			LYNXFILE						fp, *lpFile;		
			BYTE							*lpData;	
			DWORD							Len;		
			LYNXCHAR						ConstantTableStr[256], Name[64], Path[256], FileName[256], Ext[8];
			//D3DXCONSTANTTABLE_DESC			ConstantTableDesc;

			lynxSprintf(Path, _T("%svs/%s"), m_lpGraphicsSystem->GetlpRenderer()->GetCachedCompiledShaderPath().c_str(), filename);
			
			/*
			if (!(fs.vOpen(Path, (CStream::Read|CStream::Binary))))
			{
				return LYNX_FALSE;
			}
			
			lpData = NULL;
			Len = fs.vSize();
			lpData = (BYTE *)LYNXCORE_NEW BYTE[Len];
			fs.vRead(lpData, Len, sizeof(BYTE));    

			HRESULT hRes = lpR->GetlpD3DDevice()->CreateVertexShader((DWORD *)lpData, &m_lpVS);				
			if (FAILED(hRes))
			{
				fs.vClose();
				if (lpData)
					LYNXCORE_DEL [] lpData;
				return LYNX_FALSE;
			}	
			D3DXGetShaderConstantTable((const DWORD*)lpData, &m_lpConstantTable);
			m_lpConstantTable->GetDesc(&ConstantTableDesc);

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::LoadCompiledVertexShader <"))+Path+CString(_T(">\n")));
			lynxFromAnsiStr(ConstantTableStr, (char *)ConstantTableDesc.Creator);
			LYNX_LOG(glpLogger, CString(_T("Creator : ")) + ConstantTableStr + CString(_T(" \n")));
			LYNXCHAR Buffer[32];
			lynxItoA(ConstantTableDesc.Version, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("Version : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(ConstantTableDesc.Constants, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("Constants : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(m_lpConstantTable->GetBufferSize(), Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("Constant Size : ")) + Buffer + CString(_T(" \n")));

			fs.vClose();
			if (lpData)
				LYNXCORE_DEL [] lpData;
			*/

			//Yahoo!!
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3VertexShader::LoadCachedShader(CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, LYNXBOOL brecompile, LYNXBOOL bdebug) 
		{ 
			
			/*
			m_GLHandle = glCreateShader(type);
			glShaderSource(m_GLHandle, 1, &source, NULL);
			glCompileShader(m_GLHandle);
			*/
			

			LYNXERRORNO				Ret;
			LYNXFILE				fp, *lpFile, fpo;		
			BYTE					*lpData;	
			DWORD					Len;		
			LYNXCHAR				Name[64], Path[256], FileName[256], Ext[8], OutputFileName[256], OutputFolder[256], OutputPath[256], Cat[128], OldPath[256];
			char					Version[16];
			LYNXCHAR				ConstantTableStr[512], ErrorMsg[1024];	
			LYNXBOOL				bHLSL, bDebug;
			int						NumDefs;
			LYNXLONG				SrcTime, CachedTime;
			//LPD3DXBUFFER			lpCode;	
			//LPD3DXBUFFER			lpErrorMessage;
			//LPCSTR					Profile;
			DWORD					Flag;			
			//D3DXCONSTANTTABLE_DESC	ConstantTableDesc;

			lynxSeparateFileExtByPath(FileName, Ext, filename);	
			lynxStrCpy(Ext, _T("vsh"));
			lynxStrCpy(OutputFileName, FileName);
			
			lynxStrCat(OutputFileName, _T(".vso"));			
			lynxSprintf(OutputPath, _T("%svs/%s"), m_lpGraphicsSystem->GetlpRenderer()->GetCachedCompiledShaderPath().c_str(), OutputFileName);	
			lynxSprintf(Path, _T("%svs/%s.%s"), m_lpGraphicsSystem->GetlpRenderer()->GetShaderSrcPath().c_str(), FileName, Ext);	
			
			if (!brecompile)
			{
				SrcTime = lynxFileModifyTime(Path);	
				if (fs.vOpen(OutputPath, (CStream::Read|CStream::Binary)))
				{
					fs.vClose();

					CachedTime = lynxFileModifyTime(OutputPath);
					if (CachedTime >= SrcTime)
					{
						Ret = LoadCompiledShader(fs, OutputFileName);
						return Ret;
					}
				}
			}		
			
			/*
			Version[0] = 0x00;	
			if (ver)
			{
				if (!lynxStriCmp(ver, _T("1_1")))
				{
					strcpy(Version, "vs_1_1");		
				}
				else if (!lynxStriCmp(ver, _T("2_0")))
				{
					strcpy(Version, "vs_2_0");		
				}
				else if (!lynxStriCmp(ver, _T("2_X")))
				{
					strcpy(Version, "vs_2_a");		
				}		
				else if (!lynxStriCmp(ver, _T("3_0")))
				{
					strcpy(Version, "vs_3_0");		
				}
			}	

			if (!lynxStriCmp(Ext, _T("vsh")))
				bHLSL = LYNX_TRUE;
			else
			{
				bHLSL = LYNX_FALSE;

				if (!fs.vOpen(Path, (CStream::Read|CStream::Binary)))
					return LYNX_FALSE;

				lpData = NULL;
				Len = fs.vSize();
				lpData = (BYTE *)LYNXCORE_NEW BYTE[Len];
				fs.vRead(lpData, Len, sizeof(BYTE));    			
				fs.vClose();
			}

			if (bdebug)
				bDebug = LYNX_TRUE;
			else
				bDebug = LYNX_FALSE;

			if (Version && Version[0] != 0x00)
			{
				Profile = Version;
			}
			else
			{
				Profile = D3DXGetVertexShaderProfile(lpR->GetlpD3DDevice());
			}
			Flag = 0;
			Flag |= D3DXSHADER_IEEE_STRICTNESS;
			Flag |= D3DXSHADER_OPTIMIZATION_LEVEL3;
			if (bDebug)
				Flag |= D3DXSHADER_DEBUG|D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;				

			HRESULT hRes;
			if (bHLSL) // HLSL shader			
			{				
				hRes = D3DXCompileShaderFromFile(Path, (const D3DXMACRO *)def, NULL, "main", Profile, Flag|D3DXSHADER_PACKMATRIX_COLUMNMAJOR, &lpCode, &lpErrorMessage, &m_lpConstantTable);
			}
			else // Assembly shader
			{
				hRes = D3DXAssembleShader((const char*)lpData, (UINT)(strlen((const char*)lpData)), (const D3DXMACRO *)def, NULL, Flag|D3DXSHADER_PACKMATRIX_ROWMAJOR, &lpCode, &lpErrorMessage);				
				if (lpData)
					LYNXCORE_DEL [] lpData;
			}			

			if (FAILED(hRes))
			{
				if (lpErrorMessage)
				{
					lynxFromAnsiStr(ErrorMsg, (char *)lpErrorMessage->GetBufferPointer());
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::[Error] "))+ErrorMsg+CString(_T("!!\n")));
				}
				return LYNX_FALSE;
			}			
			m_lpConstantTable->GetDesc(&ConstantTableDesc);

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Compile Vertex Shader <"))+Path+CString(_T(">\n")));
			lynxFromAnsiStr(ConstantTableStr, (char *)ConstantTableDesc.Creator);
			LYNX_LOG(glpLogger, CString(_T("Creator : ")) + ConstantTableStr + CString(_T(" \n")));
			LYNXCHAR Buffer[32];
			lynxItoA(ConstantTableDesc.Version, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("Version : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(ConstantTableDesc.Constants, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("Constants : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(m_lpConstantTable->GetBufferSize(), Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("Constant Size : ")) + Buffer + CString(_T(" \n")));

			hRes = lpR->GetlpD3DDevice()->CreateVertexShader((DWORD*)(lpCode->GetBufferPointer()), &m_lpVS);	
			if (FAILED(hRes))
			{
				lpCode->Release();
				lpCode = NULL;				
				return LYNX_FALSE;
			}
			// Cache compiled shader
			lynxStrCpy(OutputFolder, OutputPath);
			Len = lynxStrLen(OutputFolder);
			for (int i=Len; i>=0; --i)
			{
				if (OutputFolder[i] == '/')
				{
					OutputFolder[i] = (LYNXCHAR)NULL;
					break;
				}
			}
			lynxCreateFolder(OutputFolder);
			
			CFileStream OutStream;
			if (OutStream.vOpen(OutputPath, (CStream::Binary|CStream::Write)))
			{
				OutStream.vWrite(lpCode->GetBufferPointer(), lpCode->GetBufferSize(), sizeof(BYTE));
				OutStream.vClose();
			}
			lpCode->Release();
			lpCode = NULL;
			*/
			
			//Yahoo!!
			return LYNX_OK;	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3VertexShader::vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def, CStream* stream) 
		{
            CStream* Stream = stream;
			CString FileName;
            CAnsiString AnsiShaderSource;
            CAnsiString Defines;
			LYNXCHAR Name[256], Ext[32];
						
			lynxSeparateFileExtByPath(Name, Ext, filename);	
			lynxStrCat(Name, _T(".vsh"));
					   
			FileName = m_lpGraphicsSystem->GetlpRenderer()->GetShaderSrcPath() + CString(_T("vs/")) + Name;	
			
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
            
			const char* Source = (const char*)AnsiShaderSource.c_str();
			
			GL_TRACE(m_GLHandle = glCreateShader(GL_VERTEX_SHADER);	)	
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
			
			
			/*
			CFileStream FileStream;
			if (m_lpGraphicsSystem->GetConfig().bCompileShaderOnTheFly)
			{
				CString Target = LynxEngine::GetEnumStr(m_lpGraphicsSystem->GetlpRenderer()->GetShaderTarget());

				if (LoadCachedShader(FileStream, filename, Target.c_str(), def, m_lpGraphicsSystem->m_bNeedToRecompileShader, LYNX_FALSE))
				{
					SetName(filename);				
					return LYNX_TRUE;
				}
				else 
				{
					SetName(filename);		
					//SetName(CString(_T("Error : can't load "))+CString(filename));		
					return LYNX_FALSE; 
				}
			}
			else
			{
				if (LoadCompiledShader(FileStream, filename)) 
				{
					SetName(filename);				
					return LYNX_TRUE;
				} 
				else 
				{
					SetName(filename);		
					//SetName(CString(_T("Error : can't load "))+CString(filename));		
					return LYNX_FALSE; 
				}
			}
			*/
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL COGLES3VertexShader::vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, CStream* stream)
		{ 
			return LYNX_FALSE; 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSet() 
		{ 		
		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantF(const char *name, const float f) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1f(Location, f);	)			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantF(const char *name, const float *lpdata, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1fv(Location, num, (const GLfloat*)lpdata);)			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantI(const char *name, const int i) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1i(Location, i);	)		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantI(const char *name, const int *lpdata, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1iv(Location, num, lpdata);				)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantB(const char *name, const BOOL b) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			GL_TRACE(glUniform1i(Location, b);			)
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantB(const char *name, const BOOL *lpdata, int num) 
		{
			int* Bool = LYNXCORE_NEW int[num];
			
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);	)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif
			
			for (int i=0; i<num; i++)
				Bool[i] = lpdata[i];
			
			GL_TRACE(glUniform1iv(Location, num, Bool);	)		

			LYNXCORE_DEL_ARRAY(Bool);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantV2(const char *name, const float *lpdata) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform2f(Location, lpdata[0], lpdata[1]);)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantV2(const char *name, const float *lpdata, int num) 
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
		void COGLES3VertexShader::vSetConstantV3(const char *name, const float *lpdata) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform3f(Location, lpdata[0], lpdata[1], lpdata[2]);)			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantV3(const char *name, const float *lpdata, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform3fv(Location, num, (const GLfloat*)lpdata);)			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantV4(const char *name, const float *lpdata) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform4f(Location, lpdata[0], lpdata[1], lpdata[2], lpdata[3]);)			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantV4(const char *name, const float *lpdata, int num) 
		{
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find constant : ") + CString(name));
				}				
			#endif			
			GL_TRACE(glUniform4fv(Location, num, (const GLfloat*)lpdata);)
        }
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetConstantM(const char *name, LPCLYNXMATRIX lpm) 
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
		void COGLES3VertexShader::vSetConstantM(const char *name, LPCLYNXMATRIX lpm, int num) 
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
		void COGLES3VertexShader::vSetTexture(const int channel, CTexture* t)  
		{
			COGLES3ShaderGroup* lpSG = (COGLES3ShaderGroup*)(m_lpGraphicsSystem->GetlpRenderer()->GetCachedShaderGroup());			
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetTexture(const char *name, CTexture* t)  
		{
			COGLES3ShaderGroup* lpSG = (COGLES3ShaderGroup*)(m_lpGraphicsSystem->GetlpRenderer()->GetCachedShaderGroup());			
			GL_TRACE(GLint Location = glGetUniformLocation(m_Program, name);)	
			#if defined(_DEBUG) || defined(DEBUG)
				if (Location < 0)
				{
					//LYNX_LOG(glpLogger, CString("Shader Warning(")+m_Name+CString("): ")+ CString("Can't find texture : ") + CString(name));
				}				
			#endif
			if (Location >= 0)
			{				
				int Index = lpSG->GetTextureChannel();						
				m_lpGraphicsSystem->GetlpRenderer()->SetTexture(Index, t);
				lpSG->UseTextureChannel();
			}
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void COGLES3VertexShader::vSetSampler(const char *name, CSamplerState* lps)
		{ 
			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int COGLES3VertexShader::vGetTextureIndex(const char *name)
		{
			return 0;
		}
	}	
}

#endif