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
#include <D3DCompiler.h>
#include <LynxEngine.h>
#include <LynxLogger.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11Renderer.h>
#include <GraphicsSystem/D3D11Renderer/LynxD3D11VertexShader.h>

#if ( (defined(__USED_RENDERER_ONLY__) && defined(__D3D11__)) || !defined(__USED_RENDERER_ONLY__) )

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11VertexShader::CD3D11VertexShader(CGraphicsSystem *pgs)
		: CVertexShader(pgs)
		{	
			m_lpVS = NULL;
			m_lpReflection = NULL;
			m_NumBoundCBs = 0;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11VertexShader::~CD3D11VertexShader(void)
		{
			Release();
		}				
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::Release()
		{
			if (m_lpVS)
			{
				m_lpVS->Release();
				m_lpVS = NULL;
			}
			if (m_lpReflection)
			{
				m_lpReflection->Release();
				m_lpReflection = NULL;
			}

			for (int i=0; i<m_ConstantBufferArray.size(); i++)
			{
				LYNXDEL m_ConstantBufferArray[i];
			}
			m_ConstantBufferArray.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::CreateConstantBuffers()
		{
			D3D11_SHADER_DESC ShaderDesc;
			m_lpReflection->GetDesc(&ShaderDesc);
			m_ConstantBufferArray.resize(ShaderDesc.ConstantBuffers);
			D3D11_SHADER_BUFFER_DESC BufferDesc;
			D3D11_SHADER_INPUT_BIND_DESC BindDesc;
			m_NumBoundCBs = 0;
			for (int i=0; i<MAX_BOUND_CB; i++)
				m_D3DBufferArray[i] = NULL;

			m_ConstantGroupArray.resize(ShaderDesc.ConstantBuffers);
			for (int i=0; i<ShaderDesc.ConstantBuffers; i++)
			{
				m_lpReflection->GetConstantBufferByIndex(i)->GetDesc(&BufferDesc);
				m_lpReflection->GetResourceBindingDescByName(BufferDesc.Name, &BindDesc);
				
				m_ConstantBufferArray[i] = (CD3D11Buffer*)m_lpGraphicsSystem->GetlpRenderer()->CreateBuffer();
				m_ConstantBufferArray[i]->vCreate(CRenderer::CONSTANT_BUFFER, BufferDesc.Size, 1, NULL, CRenderer::DYNAMIC|CRenderer::CPU_WRITE);
				m_ConstantBufferArray[i]->SetName(m_Name+_T(".CB.")+CString(BufferDesc.Name));
				m_ConstantGroupArray[i] = m_lpGraphicsSystem->GetlpRenderer()->CreateShaderConstantGroup(this, BufferDesc.Name);

				m_D3DBufferArray[BindDesc.BindPoint] = (ID3D11Buffer*)m_ConstantBufferArray[i]->GetlpResource();
				if (BindDesc.BindPoint+1 > m_NumBoundCBs)
					m_NumBoundCBs = BindDesc.BindPoint+1;
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CD3D11Buffer* CD3D11VertexShader::GetConstantBuffer(const CAnsiString& name)
		{
			CString Name = m_Name + _T(".CB.") + CString(name);
			CArray<CD3D11Buffer*>::CIterator CB = LynxEngine::find(m_ConstantBufferArray.begin(), m_ConstantBufferArray.end(), Name);
			if (CB != m_ConstantBufferArray.end())
				return (*CB);

			return NULL;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11VertexShader::LoadCompiledShader(CStream& fs, const LYNXCHAR *filename)
		{
			int								i;
			BYTE							*lpData;	
			DWORD							Len;		
			LYNXCHAR						ConstantTableStr[256], Name[64], Path[256], FileName[256], Ext[8];
			D3D11_SHADER_DESC 				ShaderDesc;

			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			lynxSprintf(Path, _T("%svs/%s"), lpR->GetCachedCompiledShaderPath().c_str(), filename);
			if (!(fs.vOpen(Path, (CStream::Read|CStream::Binary))))
			{
				return LYNX_FALSE;
			}
			
			lpData = NULL;
			Len = fs.vSize();
			lpData = (BYTE *)LYNXNEW BYTE[Len];
			fs.vRead(lpData, Len, sizeof(BYTE));    

			HRESULT hRes = lpR->GetlpD3DDevice()->CreateVertexShader((void *)lpData, Len, NULL, &m_lpVS);				
			if (FAILED(hRes))
			{
				fs.vClose();
				if (lpData)
					LYNXDEL [] lpData;
				return LYNX_FALSE;
			}	

			hRes = D3DReflect(lpData, Len, IID_ID3D11ShaderReflection, (void **)&m_lpReflection);
			if (FAILED(hRes))
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::[Error] Can't get shader reflection!!\n")));
				return LYNX_FALSE;
			}		

			m_lpReflection->GetDesc(&ShaderDesc);
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load Compiled Vertex Shader <"))+Path+CString(_T(">\n")));
			lynxFromAnsiStr(ConstantTableStr, (char *)ShaderDesc.Creator);
			LYNX_LOG(glpLogger, CString(_T("\tCreator : ")) + ConstantTableStr + CString(_T(" \n")));
			LYNXCHAR Buffer[32];
			lynxItoA(ShaderDesc.Version, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tVersion : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(ShaderDesc.ConstantBuffers , Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tConstant Buffers : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(ShaderDesc.BoundResources, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tBound Resources : ")) + Buffer + CString(_T(" \n")));	
			lynxItoA(ShaderDesc.InstructionCount, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tInstructions : ")) + Buffer + CString(_T(" \n")));	
			
			fs.vClose();
			if (lpData)
				LYNXDEL [] lpData;
			
			//Yahoo!!
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11VertexShader::LoadCachedShader(CStream& fs, const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, LYNXBOOL brecompile, LYNXBOOL bdebug) 
		{ 
			LYNXERRORNO				Ret;
			LYNXFILE				fp, *lpFile, fpo;	
			CString					FilenameWithMacro;
			LYNXCHAR				CurrentShaderPath[512];
			LYNXCHAR				Name[64], Path[256], FileName[256], Ext[8], OutputFileName[256], OutputFolder[256], OutputPath[256], Cat[128], OldPath[256];
			char					Version[16];
			CString					ConstantTableStr, ErrorMsg;	
			LYNXBOOL				bHLSL, bDebug;
			int						NumDefs;
			LYNXLONG				SrcTime, CachedTime;
			ID3D10Blob*				lpCode = NULL;	
			ID3D10Blob*				lpErrorMessage = NULL;
			LPCSTR					Profile;
			DWORD					Flag;			
			D3D11_SHADER_DESC 		ShaderDesc;

			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());

			lynxSeparateFileExtByPath(FileName, Ext, filename);	
			lynxStrCpy(Ext, _T("vsh"));
			ComputeFilenameWithMacros(FilenameWithMacro, FileName, def);
			lynxStrCpy(OutputFileName, FilenameWithMacro.c_str());
			//lynxStrCpy(OutputFileName, FileName);	

			lynxStrCat(OutputFileName, _T(".vso"));			
			lynxSprintf(OutputPath, _T("%svs/%s"), m_lpGraphicsSystem->GetlpRenderer()->GetCachedCompiledShaderPath().c_str(), OutputFileName);	
			lynxSprintf(Path, _T("%svs/%s.%s"), m_lpGraphicsSystem->GetlpRenderer()->GetShaderSrcPath().c_str(), FileName, Ext);
			lpR->m_ShaderIncludeHandler.ComputeCurrentShaderPath(Path);			
			if (!(fs.vOpen(Path, (CStream::Read|CStream::Binary))))
			{				
				lynxSprintf(Path, _T("%svs/%s.%s"), m_lpGraphicsSystem->GetlpRenderer()->GetUserShaderSrcPath().c_str(), FileName, Ext);								
				lpR->m_ShaderIncludeHandler.ComputeCurrentShaderPath(Path);
				if (!(fs.vOpen(Path, (CStream::Read|CStream::Binary))))
				{
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Can't find Vertex Shader <"))+Path+CString(_T(">\n")));
					return LYNX_FALSE;
				}
			}	
			fs.vClose();
			
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
				else if (!lynxStriCmp(ver, _T("4_0")))
				{
					strcpy(Version, "vs_4_0");		
				}
				else if (!lynxStriCmp(ver, _T("5_0")))
				{
					strcpy(Version, "vs_5_0");		
				}
				else
				{
					strcpy(Version, "vs_5_0");		
				}
			}	

			if (bdebug)
				bDebug = LYNX_TRUE;
			else
				bDebug = LYNX_FALSE;

			Profile = Version;
			
			Flag = 0;
			Flag |= D3DCOMPILE_IEEE_STRICTNESS;
			Flag |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
			Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
			//Temp  code
			Flag |= D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
			if (bDebug)
			{
				Flag |= D3DCOMPILE_DEBUG;//|D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT;				
				Flag &= ~D3DCOMPILE_OPTIMIZATION_LEVEL3;
				Flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
			}

			NumDefs = 1;
			NumDefs += lpR->m_ShaderMacros.size();
			if (def)
				NumDefs += def->size();

			D3D_SHADER_MACRO* lpD3DXMacro = LYNXNEW D3D_SHADER_MACRO[NumDefs];
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
			
		#ifdef __WINAPP__			
			HRESULT hRes = D3DCompileFromFile(Path, lpD3DXMacro, &(lpR->m_ShaderIncludeHandler), "main", Profile, Flag, 0, &lpCode, &lpErrorMessage);			
		#else
			HRESULT hRes = D3DX11CompileFromFile(Path, lpD3DXMacro, NULL, "main", Profile, Flag, 0, NULL, &lpCode, &lpErrorMessage, NULL);			
		#endif
			if (FAILED(hRes))
			{
				if (lpErrorMessage)
				{
					ErrorMsg = (char *)lpErrorMessage->GetBufferPointer();
					LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::[Error] "))+ErrorMsg+CString(_T("!!\n")));
				}
				return LYNX_FALSE;
			}
			LYNXDEL [] lpD3DXMacro;
			lpD3DXMacro = NULL;

			hRes = lpR->GetlpD3DDevice()->CreateVertexShader((void*)(lpCode->GetBufferPointer()), lpCode->GetBufferSize(), NULL, &m_lpVS);	
			if (FAILED(hRes))
			{
				lpCode->Release();
				lpCode = NULL;
				return LYNX_FALSE;
			}

			hRes = D3DReflect(lpCode->GetBufferPointer(), lpCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void **)&m_lpReflection);
			if (FAILED(hRes))
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::[Error] Can't get shader reflection!!\n")));
				return LYNX_FALSE;
			}		

			m_lpReflection->GetDesc(&ShaderDesc);
			LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Compile Vertex Shader <"))+Path+CString(_T(">\n")));
			ConstantTableStr = (char *)ShaderDesc.Creator;
			LYNX_LOG(glpLogger, CString(_T("\tCreator : ")) + ConstantTableStr + CString(_T(" \n")));
			LYNXCHAR Buffer[32];
			lynxItoA(ShaderDesc.Version, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tVersion : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(ShaderDesc.ConstantBuffers , Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tConstant Buffers : ")) + Buffer + CString(_T(" \n")));
			lynxItoA(ShaderDesc.BoundResources, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tBound Resources : ")) + Buffer + CString(_T(" \n")));	
			lynxItoA(ShaderDesc.InstructionCount, Buffer, 10);
			LYNX_LOG(glpLogger, CString(_T("\tInstructions : ")) + Buffer + CString(_T(" \n")));	
						
			// Cache compiled shader
			lynxStrCpy(OutputFolder, OutputPath);
			int Len = lynxStrLen(OutputFolder);
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
		LYNXBOOL CD3D11VertexShader::vLoad(const LYNXCHAR *filename, const CArray<CRenderer::SHADERMACRO>* def, CStream* stream) 
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

				if (LoadCachedShader(*Stream, filename, Target.c_str(), def, bNeedToRecompile, bShaderDebug))
				{
					CreateConstantBuffers();
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
					CreateConstantBuffers();
					return LYNX_TRUE;
				} 
				else 
				{
					return LYNX_FALSE; 
				}
			}			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CD3D11VertexShader::vCompile(const LYNXCHAR *filename, const LYNXCHAR *ver, const CArray<CRenderer::SHADERMACRO>* def, CStream* stream) 
		{ 
			return LYNX_FALSE; 
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSet() 
		{ 
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			lpR->GetlpDeviceContext()->VSSetShader(m_lpVS, NULL, 0);	
			lpR->GetlpDeviceContext()->VSSetConstantBuffers(0, m_NumBoundCBs, m_D3DBufferArray);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantF(const char *name, const float f) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloat(lpR->GetlpD3DDevice(), name, f);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantF(const char *name, const float *lpdata, int num) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, lpdata, num);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantI(const char *name, const int i) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetInt(lpR->GetlpD3DDevice(), name, i);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantI(const char *name, const int *lpdata, int num) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetIntArray(lpR->GetlpD3DDevice(), name, lpdata, num);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantB(const char *name, const BOOL b) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetBool(lpR->GetlpD3DDevice(), name, b);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantB(const char *name, const BOOL *lpdata, int num) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetBoolArray(lpR->GetlpD3DDevice(), name, lpdata, num);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantV2(const char *name, const float* lpdata) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloat(lpR->GetlpD3DDevice(), name, f);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantV2(const char *name, const float *lpdata, int num) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, lpdata, num);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantV3(const char *name, const float* lpdata) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloat(lpR->GetlpD3DDevice(), name, f);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantV3(const char *name, const float *lpdata, int num) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, lpdata, num);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantV4(const char *name, const float* lpdata) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloat(lpR->GetlpD3DDevice(), name, f);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantV4(const char *name, const float *lpdata, int num) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, lpdata, num);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantM(const char *name, LPCLYNXMATRIX lpm) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, (float*)lpm, 16);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetConstantM(const char *name, LPCLYNXMATRIX lpm, int num) 
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			//m_lpConstantTable->SetFloatArray(lpR->GetlpD3DDevice(), name, (float*)lpm, 16);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetTexture(const int channel, CTexture* t)  
		{
			CD3D11Renderer* lpR = (CD3D11Renderer*)(m_lpGraphicsSystem->GetlpRenderer());
			if (m_lpGraphicsSystem->m_Config.bGammaCorrection) 
				lpR->SetSRGBRead(channel, t->m_bSRGB); 			
			if (t && t->vIsValid())
			{
				ID3D11ShaderResourceView* ShaderRes[1] = {(((CD3D11Buffer*)((t->GetSrcBuffer())))->GetlpSRView()) };
				lpR->GetlpDeviceContext()->VSSetShaderResources(channel, 1, ShaderRes); 
				((CD3D11SamplerState*)(&(*(t->GetSamplerState()))))->VSSet(channel);
				//vSetConstantF("gAlphaReference", t->m_AlphaReference);
			}
			else
			{
				ID3D11ShaderResourceView* ShaderRes[1] = {NULL };
				lpR->GetlpDeviceContext()->VSSetShaderResources(channel, 1, ShaderRes); 				
			}			
			lpR->SetCachedTexture(channel, t);

			INC_COUNTER(m_lpGraphicsSystem->GetlpRenderer()->m_SetTextureCounter, 1)
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetTexture(const char *name, CTexture* t)  
		{
			int Channel = vGetTextureIndex(name);
			if (Channel >= 0)
			{
				vSetTexture(Channel, t); 				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CD3D11VertexShader::vSetSampler(const char *name, CSamplerState* lps)
		{ 
			D3D11_SHADER_INPUT_BIND_DESC Desc;
			if (m_lpReflection)
			{
				m_lpReflection->GetResourceBindingDescByName(name, &Desc);
				((CD3D11SamplerState*)lps)->VSSet(Desc.BindPoint);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		int CD3D11VertexShader::vGetTextureIndex(const char *name)
		{
			D3D11_SHADER_INPUT_BIND_DESC Desc;
			if (m_lpReflection)
			{
				m_lpReflection->GetResourceBindingDescByName(name, &Desc);
				return Desc.BindPoint;
			}
			else
				return -1;
		}
	}	
}

#endif