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
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9Renderer.h>
#include <GraphicsSystem/D3D9Renderer/LynxD3D9PixelShader.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D9__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9PixelShader::CD3D9PixelShader(CGraphicsSystem *pgs)
		: CPixelShader(pgs)
		{	
			m_lpPS = NULL;
			m_lpConstantTable = NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		///
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D9PixelShader::~CD3D9PixelShader(void)
		{
			Release();
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::Release()
		{		
			if (m_lpPS)
			{
				m_lpPS->Release();
				m_lpPS = NULL;
			}
			if (m_lpConstantTable)
			{
				m_lpConstantTable->Release();
				m_lpConstantTable = NULL;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9PixelShader::LoadCompiledShader(CStream& fs, const LYNXCHAR *filename)
		{
			int								i;
			LYNXFILE						fp, *lpFile;		
			BYTE							*lpData;	
			DWORD							Len;		
			LYNXCHAR						ConstantTableStr[256], Name[64], Path[256], FileName[256], Ext[8];
			D3DXCONSTANTTABLE_DESC			ConstantTableDesc;

			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			lynxSprintf(Path, _T("%sps/%s"), lpR->GetCachedCompiledShaderPath().c_str(), filename);
			if (!(fs.vOpen(Path, (CStream::Read|CStream::Binary))))
			{
				return LYNX_FALSE;
			}
			
			lpData = NULL;
			Len = fs.vSize();
			lpData = (BYTE *)LYNXALLOC(Len);
			fs.vRead(lpData, Len, sizeof(BYTE));    

			HRESULT hRes = lpR->GetlpD3DDevice()->CreatePixelShader((DWORD *)lpData, &m_lpPS);				
			if (FAILED(hRes))
			{
				fs.vClose();
				if (lpData)
					LYNXFREE(lpData);
				return LYNX_FALSE;
			}	
			D3DXGetShaderConstantTable((const DWORD*)lpData, &m_lpConstantTable);
			m_lpConstantTable->GetDesc(&ConstantTableDesc);

			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::LoadCompiledPixelShader <"))+Path+CString(_T(">\n")));
			lynxFromAnsiStr(ConstantTableStr, (char *)ConstantTableDesc.Creator);
			LYNX_LOG(glpLogger, CString(_T("\tCreator : ")) + ConstantTableStr + CString(_T(" \n")));
			LYNXCHAR Buffer[32];
			lynxItoA(ConstantTableDesc.Version, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tVersion : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(ConstantTableDesc.Constants, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tConstants : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(m_lpConstantTable->GetBufferSize(), Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tConstant Size : ")) + Buffer + CString(_T(" \n")));

			fs.vClose();
			if (lpData)
				LYNXFREE(lpData);
			
			//Yahoo!!
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9PixelShader::CompileShader(CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, LYNXBOOL brecompile, LYNXBOOL bdebug) 
		{ 
			LYNXERRORNO				Ret;
			LYNXFILE				fp, *lpFile, fpo;		
			BYTE					*lpData;	
			DWORD					Len;		
			CString					FilenameWithMacro;
			LYNXCHAR				Name[64], Path[256], FileName[256], Ext[8], OutputFileName[2048], OutputFolder[2048], OutputPath[2048];
			char					Version[16];
			LYNXCHAR				ConstantTableStr[512], ErrorMsg[1024];	
			LYNXBOOL				bHLSL, bDebug;
			int						NumDefs;
			LYNXLONG				SrcTime, CachedTime;
			LPD3DXBUFFER			lpCode;	
			LPD3DXBUFFER			lpErrorMessage;
			LPCSTR					Profile;
			DWORD					Flag;			
			D3DXCONSTANTTABLE_DESC	ConstantTableDesc;
			LYNXBOOL				bFoundShader;

			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			lynxSeparateFileExtByPath(FileName, Ext, filename);	
			lynxStrCpy(Ext, _T("psh"));
			ComputeFilenameWithMacros(FilenameWithMacro, FileName, def);
			lynxStrCpy(OutputFileName, FilenameWithMacro.c_str());
			//lynxStrCpy(OutputFileName, FileName);			
			
			bFoundShader = LYNX_FALSE;
			lynxStrCat(OutputFileName, _T(".pso"));			
			lynxSprintf(OutputPath, _T("%sps/%s"), m_lpGraphicsSystem->GetlpRenderer()->GetCachedCompiledShaderPath().c_str(), OutputFileName);	
			for (CList<CString>::CIterator SrcPath = m_lpGraphicsSystem->GetlpRenderer()->GetShaderSrcPathList().begin(); SrcPath != m_lpGraphicsSystem->GetlpRenderer()->GetShaderSrcPathList().end(); SrcPath++)
			{
				lynxSprintf(Path, _T("%sps/%s.%s"), (*SrcPath).c_str(), FileName, Ext);	
				lpR->m_ShaderIncludeHandler.ComputeCurrentShaderPath(Path);			
				if (m_lpEngine->GetlpFileSystem()->FindFile(Path))
				//if ((fs.vOpen(Path, (CStream::Read|CStream::Binary))))
				{			
					bFoundShader = LYNX_TRUE;
					//fs.vClose();
					break;
				}	
			}
			if (!bFoundShader)
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Can't find Pixel Shader <"))+Path+CString(_T(">\n")));
				return LYNX_FALSE;
			}
			
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

			Version[0] = 0x00;	
			if (ver)
			{
				if (!lynxStriCmp(ver, _T("1_1")))
				{
					strcpy(Version, "ps_1_1");		
				}
				else if (!lynxStriCmp(ver, _T("2_0")))
				{
					strcpy(Version, "ps_2_0");		
				}
				else if (!lynxStriCmp(ver, _T("2_a")))
				{
					strcpy(Version, "ps_2_a");		
				}
				else if (!lynxStriCmp(ver, _T("2_b")))
				{
					strcpy(Version, "ps_2_b");		
				}
				else if (!lynxStriCmp(ver, _T("3_0")))
				{
					strcpy(Version, "ps_3_0");		
				}
			}	

			if (!fs.vOpen(Path, (CStream::Read|CStream::Binary)))
				return LYNX_FALSE;
			
			if (!lynxStriCmp(Ext, _T("psh")))
			{
				bHLSL = LYNX_TRUE;
			}
			else
			{
				bHLSL = LYNX_FALSE;
			}

			if (!fs.vOpen(Path, (CStream::Read|CStream::Binary)))
				return LYNX_FALSE;

			lpData = NULL;
			Len = fs.vSize();
			lpData = (BYTE *)LYNXALLOC(Len);
			fs.vRead(lpData, Len, sizeof(BYTE));    			
			fs.vClose();			

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
			Flag |= D3DXSHADER_PACKMATRIX_ROWMAJOR;
			if (bDebug)
			{
				Flag |= D3DXSHADER_DEBUG;//|D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;				
				Flag &= !D3DXSHADER_OPTIMIZATION_LEVEL3;
				Flag |= D3DXSHADER_SKIPOPTIMIZATION;
			}

			NumDefs = 1;
			NumDefs += lpR->m_ShaderMacros.size();
			if (def)
				NumDefs += def->size();

			D3DXMACRO* lpD3DXMacro = LYNXGLOBAL_NEW D3DXMACRO[NumDefs];
			NumDefs = 0;
			//Global shader macros
			for (int i=0; i<lpR->m_ShaderMacros.size(); i++)
			{
				lpD3DXMacro[NumDefs].Name = lpR->m_ShaderMacros[i].Name.c_str();
				lpD3DXMacro[NumDefs].Definition = lpR->m_ShaderMacros[i].Definition.c_str();
				NumDefs++;
			}
			//Local shader macros
			if (def)
			{
				for (int i=0; i<def->size(); i++)
				{
					lpD3DXMacro[NumDefs].Name = (*def)[i].Name.c_str();
					lpD3DXMacro[NumDefs].Definition = (*def)[i].Definition.c_str();
					NumDefs++;
				}				
			}
			lpD3DXMacro[NumDefs].Name = NULL;
			lpD3DXMacro[NumDefs].Definition = NULL;

			HRESULT hRes;
			lpErrorMessage = NULL;
			if (bHLSL) // HLSL shader			
			{				
				hRes = D3DXCompileShaderFromFile(Path, lpD3DXMacro, NULL, "main", Profile, Flag, &lpCode, &lpErrorMessage, &m_lpConstantTable);
			}
			else // Assembly shader
			{
				hRes = D3DXAssembleShader((const char*)lpData, (UINT)(strlen((const char*)lpData)), lpD3DXMacro, NULL, Flag, &lpCode, &lpErrorMessage);				
			}
			LYNXGLOBAL_DEL_ARRAY(lpD3DXMacro);
			lpD3DXMacro = NULL;

			if (FAILED(hRes))
			{
				if (lpErrorMessage)
				{
					lynxFromAnsiStr(ErrorMsg, (char *)lpErrorMessage->GetBufferPointer());
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::[Error] "))+ErrorMsg+CString(_T("!!\n")));
				}
				return LYNX_FALSE;
			}			
			if (lpData)
				LYNXFREE(lpData);

			m_lpConstantTable->GetDesc(&ConstantTableDesc);
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Compile Pixel Shader <"))+Path+CString(_T(">\n")));
			lynxFromAnsiStr(ConstantTableStr, (char *)ConstantTableDesc.Creator);
			LYNX_LOG(glpLogger, CString(_T("\tCreator : ")) + ConstantTableStr + CString(_T(" \n")));
			LYNXCHAR Buffer[32];
			lynxItoA(ConstantTableDesc.Version, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tVersion : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(ConstantTableDesc.Constants, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tConstants : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(m_lpConstantTable->GetBufferSize(), Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tConstant Size : ")) + Buffer + CString(_T(" \n")));

			hRes = lpR->GetlpD3DDevice()->CreatePixelShader((DWORD*)(lpCode->GetBufferPointer()), &m_lpPS);	
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

			CPlatformFileStream OutStream;
			if (OutStream.vOpen(OutputPath, (CStream::Binary|CStream::Write)))
			{
				OutStream.vWrite(lpCode->GetBufferPointer(), lpCode->GetBufferSize(), sizeof(BYTE));
				OutStream.vClose();
			}
			lpCode->Release();
			lpCode = NULL;
			
			//Yahoo!!
			return LYNX_OK;	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9PixelShader::vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def, CStream* stream) 
		{ 
			CStream* Stream = stream;
			if (!Stream)
				Stream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();

			if (m_lpGraphicsSystem->GetConfig().bCompileShaderOnTheFly)
			{
				CString Target = LynxEngine::GetEnumStr(m_lpGraphicsSystem->GetlpRenderer()->GetShaderTarget());

				LYNXBOOL bShaderDebug = LYNX_FALSE;
				LYNXBOOL bNeedToRecompile = m_lpGraphicsSystem->m_bNeedToRecompileShader;
				#if (defined __SHADER_DEBUG__) && (defined _DEBUG)
					bShaderDebug = LYNX_TRUE;
					bNeedToRecompile = LYNX_TRUE;
				#endif								

				if (CompileShader(*Stream, filename, Target.c_str(), def, bNeedToRecompile, bShaderDebug))
				{
					return LYNX_TRUE;
				}
				else 
				{
					return LYNX_FALSE; 
				}
			}
			else
			{
				if (LoadCompiledShader(*Stream, filename)) 
				{
					return LYNX_TRUE;
				} 
				else 
				{
					return LYNX_FALSE; 
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D9PixelShader::vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, CStream* stream) 
		{ 
			return LYNX_FALSE; 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSet() 
		{ 
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			lpR->GetlpD3DDevice()->SetPixelShader(m_lpPS);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetTexture(const int channel, CTexture* t)  
		{
			if (channel >= 0)
			{			
				m_lpGraphicsSystem->GetlpRenderer()->SetTexture(channel, t); 
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetTexture(const char *name, CTexture* t)  
		{
			if (m_ResourceMap[name])
				m_ResourceMap[name]->Set(t);
			/*
			else
			{
				int Channel = vGetTextureIndex(name);

				if (Channel >= 0)
				{			
					m_lpGraphicsSystem->GetlpRenderer()->SetTexture(Channel, t); 								
				}
			}
			*/
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetSampler(const char *name, CSamplerState* lps)
		{ 
			m_lpGraphicsSystem->GetlpRenderer()->SetSamplerState(vGetTextureIndex(name), lps);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CD3D9PixelShader::vGetTextureIndex(const char *name)
		{
			return m_lpConstantTable->GetSamplerIndex(name);	
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantF(const char *name, const float f) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloat(lpR->GetlpD3DDevice(), name, f);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantF(const char *name, const float *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, lpdata, num);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantI(const char *name, const int i) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetInt(lpR->GetlpD3DDevice(), name, i);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantI(const char *name, const int *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), name, lpdata, num);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantB(const char *name, const BOOL b) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetBool(lpR->GetlpD3DDevice(), name, b);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantB(const char *name, const BOOL *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetBoolArray(lpR->GetlpD3DDevice(), name, lpdata, num);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantV2(const char *name, const float* f) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, f, 2);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantV2(const char *name, const float *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, lpdata, num*2);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantV3(const char *name, const float* f) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, f, 3);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantV3(const char *name, const float *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, lpdata, num*3);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantV4(const char *name, const float* f) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, f, 4);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantV4(const char *name, const float *lpdata, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, lpdata, num*4);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantM(const char *name, LPCLYNXMATRIX lpm) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, (float*)lpm, 16);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D9PixelShader::vSetConstantM(const char *name, LPCLYNXMATRIX lpm, int num) 
		{
			CD3D9Renderer* lpR = (CD3D9Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, (float*)lpm, 16*num);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetShaderConstantCounter, 1)
		}
	}
}

#endif
