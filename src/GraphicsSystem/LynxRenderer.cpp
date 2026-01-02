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
//  Created by Owen Wu : 2006/11/07
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <Container/LynxSprite3DClusterContainer.h>
#include <Container/LynxSpriteGrassContainer.h>
#include <Container/LynxCrossSpriteGrassContainer.h>

namespace LynxEngine
{			
	namespace GraphicsSystem
	{
		CRenderer::FIXEDVERTEXLAYOUT CRenderer::m_SystemFixedVertexLayout = 0;
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		CRenderer::CRenderer()
		{
			m_RendererType = VOID_RENDERER;

			m_lpGraphicsSystem = NULL; 
			m_bUseCachedState = LYNX_TRUE;

			m_bHardwareBusy = LYNX_FALSE;

			m_WindowOrigin.x = 0;
			m_WindowOrigin.y = 0;
            
            m_bPortraitMode = LYNX_FALSE;
            
			m_CachedVS = NULL; 
			m_CachedPS = NULL;
			for (int i=0; i<MAX_NUM_TEXTURE_CHANNEL; i++)
			{
				m_CachedTexture[i] = NULL;
				m_CachedSamplerState[i] = NULL;
			}
			m_CachedDepthStencilState = NULL;
			m_CachedBlendState = NULL;
			m_CachedRasterizerState = NULL;
			for (int i=0; i<MAX_NUM_MRT; i++)
				m_CachedRT[i] = NULL;
			m_CachedDB = NULL;

			m_CachedVertexLayout = NULL;
			m_CachedVertexArray = NULL;
			m_CachedIndexArray = NULL;

            m_bCachedScissorEnabled = LYNX_FALSE;
            
			m_Pixel2TexelOffset.x = 0.0f;
			m_Pixel2TexelOffset.y = 0.0f;

			m_UserShaderSrcPath = _T("../shader/d3d/");
			m_CachedCompiledShaderPath = _T("../cached/driverdesc/shader/");
			m_ShaderMacros.clear();
            
            m_bSharedShaderGroup = LYNX_TRUE;
			m_bSharedShaderClass = LYNX_TRUE;
			m_bCacheShaderGroup = LYNX_TRUE;
            
            m_TextureSearchPathList.clear();
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		CRenderer::CRenderer(CGraphicsSystem* lpgs)
		: CObj(lpgs->GetlpEngine())
		{
			m_RendererType = VOID_RENDERER;

			m_lpGraphicsSystem = lpgs; 
			m_bUseCachedState = LYNX_TRUE;

			m_bHardwareBusy = LYNX_FALSE;

			m_WindowOrigin.x = 0;
			m_WindowOrigin.y = 0;

			m_bPortraitMode = LYNX_FALSE;

			m_CachedVS = NULL; 
			m_CachedPS = NULL;
			for (int i=0; i<MAX_NUM_TEXTURE_CHANNEL; i++)
			{
				m_CachedTexture[i] = NULL;
				m_CachedSamplerState[i] = NULL;
			}
			m_CachedDepthStencilState = NULL;
			m_CachedBlendState = NULL;
			m_CachedRasterizerState = NULL;
			for (int i=0; i<MAX_NUM_MRT; i++)
				m_CachedRT[i] = NULL;
			m_CachedDB = NULL;

			m_CachedVertexLayout = NULL;
			m_CachedVertexArray = NULL;
			m_CachedIndexArray = NULL;
            
            m_bCachedScissorEnabled = LYNX_FALSE;

			m_Pixel2TexelOffset.x = 0.0f;
			m_Pixel2TexelOffset.y = 0.0f;

			m_UserShaderSrcPath = _T("../shader/d3d/");
			m_CachedCompiledShaderPath = _T("../cached/driverdesc/shader/");
			m_ShaderMacros.clear();
            
            m_bSharedShaderGroup = LYNX_TRUE;
			m_bSharedShaderClass = LYNX_TRUE;
			m_bCacheShaderGroup = LYNX_TRUE;
            
            m_TextureSearchPathList.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		CRenderer::~CRenderer()
		{			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::Close()
		{
			m_lpBackBuffer.Reset();
			m_lpDepthBuffer.Reset();
			m_lpOffscreenDepthBuffer.Reset();

			m_ShaderMacros.clear();
			ClearCachedResource();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::ClearCachedResource() 
		{ 
			m_CachedResourceList.clear(); 

			m_lpGraphicsSystem->GetlpEngine()->ClearInvalidResource();
			m_lpGraphicsSystem->GetlpEngine()->ClearInvalidVertexLayout();
			m_lpGraphicsSystem->GetlpEngine()->ClearInvalidShaderClass();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRenderer::CreateDepthBuffer(CBufferPtr& buffer, DWORD w, DWORD h, FORMAT format, int num_ms)
		{
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(buffer, BUFFER);
			return (buffer->vCreate(NULL, w, h, format, num_ms, LYNX_FALSE));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRenderer::CreateRenderBuffer(CBufferPtr& buffer, DWORD w, DWORD h, FORMAT format, int num_ms, LYNXBOOL blockable)
		{
			m_lpGraphicsSystem->GetlpEngine()->CreateResource(buffer, BUFFER);
			return (buffer->vCreate(NULL, w, h, format, num_ms, blockable));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::AddDefinedValue(LynxEngine::CAnsiString& def, LynxEngine::CAnsiString& value)
		{
			SHADERMACRO ShaderMacro;

			ShaderMacro.Name = def;
			ShaderMacro.Definition = value;
			m_ShaderMacros.push_back(ShaderMacro);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::AddDefinedValue(const char* def, const char* value)
		{
			CAnsiString Def(def);
			CAnsiString Value(value);
			AddDefinedValue(Def, Value);
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CString& CRenderer::ComputeCachedCompiledShaderPath() 
		{
			CString				Target;
			LYNXCHAR			Buffer[16];

			Target = LynxEngine::GetEnumStr(m_ShaderTarget);
			m_CachedCompiledShaderPath = CString(_T("../cached/graphics/")) + CString(GetAPIString()) + CString(_T("/")) + CString(GetDriverDesc()) + CString(_T("/shader/")) + Target + CString(_T("/"));			
			m_ShaderSrcPath.push_back(CString(_T("../../engine/shader/d3d/")));			
			m_ShaderSrcPath.push_back(CString(_T("engine/shader/d3d/")));			
			m_ShaderSrcPath.push_back(CString(_T("shader/d3d/")));			

			m_UserShaderSrcPath = _T("../shader/ogles/");

			m_ShaderMacros.clear();
			switch (m_ShaderTarget)
			{
				case SHADER_TARGET_11:
					AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("10"));
					break;
				case SHADER_TARGET_20:
					AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("20"));
					break;
				case SHADER_TARGET_2X:
					AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("21"));
					break;
				case SHADER_TARGET_30:
					AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("30"));
					break;
				case SHADER_TARGET_40:
					AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("40"));
					break;
				case SHADER_TARGET_41:
					AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("41"));
					break;
				case SHADER_TARGET_50:
					AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("50"));
					break;
				default:
					AddDefinedValue(CAnsiString("SHADER_MODEL"), CAnsiString("30"));
					break;
			}			
			//AddDefinedValue(CAnsiString("PCF_1X1"), CAnsiString("1"));
			AddDefinedValue(CAnsiString("PCF_4X4"), CAnsiString("1"));
			//AddDefinedValue(CAnsiString("PCF_2X2"), CAnsiString("1"));

			// TODO : Adjust the batch size according to the HW
			//CSprite3DClusterContainer::m_BatchSize = 64;
			//CSpriteGrassContainer::m_BatchSize = 64;
			//CCrossSpriteGrassContainer::m_BatchSize = 64;
			//CFontString::m_BatchSize = 16*4;
			
			//lynxSprintf(Buffer, _T("%d"), CFontString::m_BatchSize);
			//AddDefinedValue(CAnsiString("FONTSTRING_BATCH_SIZE"), CAnsiString(Buffer));

			//lynxSprintf(Buffer, _T("%d"), CSprite3DClusterContainer::m_BatchSize);
			//AddDefinedValue(CAnsiString("SPRITE3DCLUSTER_BATCH_SIZE"), CAnsiString(Buffer));
			
			lynxSprintf(Buffer, _T("%d"), CSpriteGrassContainer::m_BatchSize);
			AddDefinedValue(CAnsiString("GRASS_BATCH_SIZE"), CAnsiString(Buffer));

			lynxSprintf(Buffer, _T("%d"), CCrossSpriteGrassContainer::m_BatchSize);
			AddDefinedValue(CAnsiString("CROSSGRASS_BATCH_SIZE"), CAnsiString(Buffer));			

			lynxSprintf(Buffer, _T("%d"), MAX_NUM_TEXTURE_CHANNEL);
			AddDefinedValue(CAnsiString("MAX_TEXTURE_CHANNELS"), CAnsiString(Buffer));	
						
			//AddDefinedValue(CString("PARALLAX"), CString("1"));
			
			return m_CachedCompiledShaderPath; 
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRenderer::NeedToRecompileShader()
		{
			LYNXFINDDATA FindData;
			LYNXCHAR FindPath[256];

			lynxInitFindData(&FindData);
			lynxStrCpy(FindPath, m_CachedCompiledShaderPath.c_str());
			int Len = (int)m_CachedCompiledShaderPath.length();
			FindPath[Len-1] = (LYNXCHAR)NULL;
			lynxFindFolders(FindPath, &FindData);
			if (!FindData.NumFounds)
			{
				lynxCreateFolder(m_CachedCompiledShaderPath.c_str());
				lynxFreeFounds(&FindData);
				return LYNX_TRUE;
			}
			lynxFreeFounds(&FindData);
			return LYNX_FALSE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::GetSubPixelOffset(LYNXUVPOINT* lpuv)
		{
			lpuv->u = -m_Pixel2TexelOffset.x/((float)(m_CachedRT[0]->vGetWidth()));
			lpuv->v = -m_Pixel2TexelOffset.y/((float)(m_CachedRT[0]->vGetHeight()));
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CRenderer::LoadVertexShader(CShaderPtr& s, const LYNXCHAR *name, const CArray<SHADERMACRO>* def, CStream* stream) 
		{
			LYNXCHAR FileName[2048], Ext[8], Name[2048];
			CString ResourceName;

			lynxStrCpy(Name, name);
			lynxStrLower(Name);
			lynxSeparateFileExtByPath(FileName, Ext, Name);				
			CShader::ComputeFilenameWithMacros(ResourceName, this, FileName, def);
			s = m_lpGraphicsSystem->GetlpEngine()->FindResource(CString(ResourceName));
			if (!s || !m_bSharedShaderClass)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, VERTEX_SHADER);			
				s->SetName(ResourceName);
				if (!s->vLoad(Name, def, stream))
					return LYNX_FALSE;												
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load VertexShader("))+CString(ResourceName)+CString(_T(")\r\n")));
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::VertexShader("))+CString(ResourceName)+CString(_T(") is already loaded\r\n")));
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------		
		LYNXBOOL CRenderer::LoadVertexShader(CVertexShaderPtr& s, const LYNXCHAR *name, const CArray<SHADERMACRO>* def, CStream* stream) 
		{
			LYNXCHAR FileName[2048], Ext[8], Name[2048];
			CString ResourceName;

			lynxStrCpy(Name, name);
			lynxStrLower(Name);
			lynxSeparateFileExtByPath(FileName, Ext, Name);	
			
			CShader::ComputeFilenameWithMacros(ResourceName, this, FileName, def);
			s = m_lpGraphicsSystem->GetlpEngine()->FindResource(CString(ResourceName));
			if (!s || !m_bSharedShaderClass)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, VERTEX_SHADER);			
				s->SetName(ResourceName);
				if (!s->vLoad(Name, def, stream))
					return LYNX_FALSE;													
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load VertexShader("))+CString(ResourceName)+CString(_T(")\r\n")));
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::VertexShader("))+CString(ResourceName)+CString(_T(") is already loaded\r\n")));
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVertexLayout* CRenderer::SetVertexLayout(CVertexLayout* vl)
		{
			CVertexLayout* V = m_CachedVertexLayout;

			if (m_bUseCachedState)
			{
				if (vl == m_CachedVertexLayout)
					return V;
			}
						
			vl->vSet();
			
			//INC_COUNTER(m_SetShaderCounter, 1)

			m_CachedVertexLayout = vl;

			return V;
		}	
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVertexArray* CRenderer::SetVertexArray(CVertexArray* va, int stream, int first)
		{
			//INC_COUNTER(m_SetShaderCounter, 1)

			CVertexArray* V = m_CachedVertexArray;

			if (m_bUseCachedState)
			{
				if (va == m_CachedVertexArray)
					return V;
			}
									
			va->vSet(stream, first);

			m_CachedVertexArray = va;

			return V;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CIndexArray* CRenderer::SetIndexArray(CIndexArray* ia, int first)
		{
			//INC_COUNTER(m_SetShaderCounter, 1)

			CIndexArray* V = m_CachedIndexArray;

			if (m_bUseCachedState)
			{
				if (ia == m_CachedIndexArray)
					return V;
			}
						
			ia->vSet(first);

			m_CachedIndexArray = ia;

			return V;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CVertexShader* CRenderer::SetVertexShader(CVertexShader* vs)
		{
			CVertexShader* V = m_CachedVS;

			if (m_bUseCachedState)
			{
				if (vs == m_CachedVS)
					return V;
			}
			
			if (vs)
				vs->vSet();
			else
				NullVertexShader();

			INC_COUNTER(m_SetShaderCounter, 1)

			m_CachedVS = vs;

			return V;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CRenderer::LoadPixelShader(CShaderPtr& s, const LYNXCHAR *name, const CArray<SHADERMACRO>* def, CStream* stream) 
		{
			LYNXCHAR FileName[2048], Ext[8], Name[2048];
			CString ResourceName;

			lynxStrCpy(Name, name);
			lynxStrLower(Name);
			lynxSeparateFileExtByPath(FileName, Ext, Name);				
			CShader::ComputeFilenameWithMacros(ResourceName, this, FileName, def);
			s = m_lpGraphicsSystem->GetlpEngine()->FindResource(CString(ResourceName));
			if (!s || !m_bSharedShaderClass)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, PIXEL_SHADER);		
				//If the system prefers compile shader on the fly when first time running, then go to cached compiled shader path to check the shaders and decide if the shaders need to be recompiled or not.
				//Otherwise, just load pre-compiled shaders.
				s->SetName(ResourceName);
				if (!s->vLoad(Name, def, stream))
					return LYNX_FALSE;				
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load PixelShader("))+CString(ResourceName)+CString(_T(")\r\n")));
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::PixelShader("))+CString(ResourceName)+CString(_T(") is already loaded\r\n")));
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CRenderer::LoadPixelShader(CPixelShaderPtr& s, const LYNXCHAR *name, const CArray<SHADERMACRO>* def, CStream* stream) 
		{
			LYNXCHAR FileName[2048], Ext[8], Name[2048];
			CString ResourceName;

			lynxStrCpy(Name, name);
			lynxStrLower(Name);
			lynxSeparateFileExtByPath(FileName, Ext, Name);				
			CShader::ComputeFilenameWithMacros(ResourceName, this, FileName, def);
			s = m_lpGraphicsSystem->GetlpEngine()->FindResource(CString(ResourceName));
			if (!s || !m_bSharedShaderClass)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, PIXEL_SHADER);		
				//If the system prefers compile shader on the fly when first time running, then go to cached compiled shader path to check the shaders and decide if the shaders need to be recompiled or not.
				//Otherwise, just load pre-compiled shaders.
				s->SetName(ResourceName);
				if (!s->vLoad(Name, def, stream))
					return LYNX_FALSE;				
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Load PixelShader("))+CString(ResourceName)+CString(_T(")\r\n")));
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::PixelShader("))+CString(ResourceName)+CString(_T(") is already loaded\r\n")));
			}
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CPixelShader* CRenderer::SetPixelShader(CPixelShader* ps)
		{
			CPixelShader* P = m_CachedPS;

			if (m_bUseCachedState)
			{
				if (ps == m_CachedPS)
					return P;
			}
			
			if (ps)
				ps->vSet();
			else
				NullPixelShader();

			INC_COUNTER(m_SetShaderCounter, 1)
			
			m_CachedPS = ps;

			return P;
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CRenderer::LoadTexture(CTexturePtr& t, CStream& fs, const LYNXCHAR *filename, CRenderer::TEXTUREDESC* desc)
		{
			LYNXCHAR Path[256], Name[256], Ext[16];
            LYNXBOOL bTexture = LYNX_FALSE;
            
            if (!filename || filename[0] == NULL)
                return LYNX_FALSE;
                
			lynxStrCpy(Path, m_lpGraphicsSystem->GetlpRenderer()->GetTexturePath());
			lynxStrCat(Path, filename);
			t = m_lpGraphicsSystem->GetlpEngine()->FindResource(CString(Path));
			if (t == NULL)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(t, TEXTURE);
				LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::LoadTexture(")) + CString(Path) + CString(_T(")\r\n")));
				bTexture = t->vLoad(fs, Path, desc);
				if (!bTexture)
				{
					//m_lpGraphicsSystem->GetlpEngine()->DestroyResource(t->GetName());
					t.Reset();
					m_lpEngine->ClearInvalidResource();
				}
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Texture(")) + CString(Path) + CString(_T(") is already loaded\r\n")));
				bTexture = LYNX_TRUE;
			}

			if (!bTexture)
			{
				for (CList<CString>::CIterator SearchPath = m_TextureSearchPathList.begin(); SearchPath != m_TextureSearchPathList.end(); SearchPath++)
				{
					lynxStrCpy(Path, (*SearchPath).c_str());
					lynxStrCat(Path, filename);
					t = m_lpGraphicsSystem->GetlpEngine()->FindResource(CString(Path));
					if (t == NULL)
					{
						m_lpGraphicsSystem->GetlpEngine()->CreateResource(t, TEXTURE);
						LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::LoadTexture(")) + CString(Path) + CString(_T(")\r\n")));
						bTexture = t->vLoad(fs, Path, desc);
						if (bTexture)
						{
							break;
						}
						else
						{
							//m_lpGraphicsSystem->GetlpEngine()->DestroyResource(t->GetName());
							t.Reset();
							m_lpEngine->ClearInvalidResource();
						}
					}
					else
					{
						LYNX_LOG(glpLogger, CString(GetClassName()) + CString(_T("::Texture(")) + CString(Path) + CString(_T(") is already loaded\r\n")));
						bTexture = LYNX_TRUE;
						break;
					}
				}
			}

            if (!bTexture)
            {
				return LYNX_FALSE;
            }
            
            if (desc)
            {
                t->m_AlphaReference = desc->AlphaReference;
				if (t->m_MipMapLevel == 1)
                {
                    desc->SamplerDesc.MinLOD = 0;
                    desc->SamplerDesc.MaxLOD = 0;
                }
                
                if (GetAPIString() == CString(_T("OGL ES 2.0")))
                {
                    if (!lynxCheckPowerOf2(t->vGetWidth(), 18) || !lynxCheckPowerOf2(t->vGetHeight(), 18))
                    {
                        desc->SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
                        desc->SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
                        desc->SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
                    }
                }
                t->CreateSamplerState(desc->SamplerDesc);
            }
            else
            {
                t->m_AlphaReference = 1.0f;
            }

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CTexture* CRenderer::SetTexture(int channel, CTexture* t)
		{
			CTexture* T = m_CachedTexture[channel];

			#if !SHIPPING
				if (t)
				{
					LYNX_ASSERT(t->GetSrcBuffer() != m_CachedRT[0])
				}
			#endif
            
            #ifndef __iOS__
                // TODO : Should be moved to shader class to improve the performance
                if (t && GetCachedPixelShader())
                    GetCachedPixelShader()->vSetConstantF("gAlphaReference", t->m_AlphaReference);  
            #endif
            
            if (m_bUseCachedState)
            {
                if (t == m_CachedTexture[channel])
                    return T;
            }
           
			if (t)
			{
				t->vSet(channel);
				SetSamplerState(channel, t->GetSamplerState());
			}
			else
				m_lpGraphicsSystem->GetSystemTexture(WHITE_TEXTURE)->vSet(channel);

			INC_COUNTER(m_SetTextureCounter, 1)

			m_CachedTexture[channel] = t;

			return T;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::CreateDepthStencilState(CDepthStencilStatePtr& s, const DEPTHSTENCILSTATEDESC& desc)
		{
			// Don't shared render state object in tool because we don't want the change on one state object affact to other state objects
			#if defined(__TOOL__)
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, DEPTHSTENCIL_STATE);
				s->vCreate(desc);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateDepthStencilState("))+s->GetName()+CString(_T(")\r\n")));
			#else

			LYNXCRC NewCRC = CObj::ComputeCRCCode(CDepthStencilState::MakeName(desc));
			s = m_lpGraphicsSystem->GetlpEngine()->FindResource(NewCRC);
			//s = m_lpGraphicsSystem->GetlpEngine()->FindResource(CDepthStencilState::MakeName(desc));
			if (s == NULL)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, DEPTHSTENCIL_STATE);
				s->vCreate(desc);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateDepthStencilState("))+s->GetName()+CString(_T(")\r\n")));
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::DepthStencil state("))+s->GetName()+CString(_T(") is already loaded\r\n")));
			}
		#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  說明:   
		//-------------------------------------------------------------------------------------------------------
		CDepthStencilState* CRenderer::SetDepthStencilState(CDepthStencilState* s)
		{
			CDepthStencilState* S = m_CachedDepthStencilState;

			if (m_bUseCachedState)
			{
				if (s == m_CachedDepthStencilState)
					return S;
			}

			s->vSet();
			INC_COUNTER(m_SetRenderStateCounter, 1)
			PERF_COUNTER_INC(_T("Renderer"), _T("SetRS"), 1)

			m_CachedDepthStencilState = s;

			return S;
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::CreateBlendState(CBlendStatePtr& s, const BLENDSTATEDESC& desc)
		{
			// Don't shared render state object in tool because we don't want the change on one state object affact to other state objects
			#if defined(__TOOL__) 
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, BLEND_STATE);
				s->vCreate(desc);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateBlendState("))+s->GetName()+CString(_T(")\r\n")));
			#else			
			LYNXCRC NewCRC = CObj::ComputeCRCCode(CBlendState::MakeName(desc));
			s = m_lpGraphicsSystem->GetlpEngine()->FindResource(NewCRC);
			//s = m_lpGraphicsSystem->GetlpEngine()->FindResource(CBlendState::MakeName(desc));
			if (s == NULL)
			{				
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, BLEND_STATE);
				s->vCreate(desc);
				LYNXCHAR CRCCode[64];
				lynxSprintf(CRCCode, _T("0x%x"), s->GetCRCCode());
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateBlendState("))+s->GetName()+CString(_T("):<")+CString(CRCCode)+CString(_T(">\r\n"))));
			}
			else
			{			
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Blend state("))+s->GetName()+CString(_T(") is already loaded\r\n")));
			}
		#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		CBlendState* CRenderer::SetBlendState(CBlendState* s)
		{
			CBlendState* S = m_CachedBlendState;

			if (m_bUseCachedState)
			{
				if (s == m_CachedBlendState)
					return S;
			}

			s->vSet();
			INC_COUNTER(m_SetRenderStateCounter, 1)
			PERF_COUNTER_INC(_T("Renderer"), _T("SetRS"), 1)

			m_CachedBlendState = s;

			return S;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::CreateRasterizerState(CRasterizerStatePtr& s, const RASTERIZERSTATEDESC& desc)
		{
			// Don't shared render state object in tool because we don't want the change on one state object affact to other state objects
			#if defined(__TOOL__)
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, RASTERIZER_STATE);
				s->vCreate(desc);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateRasterizerState("))+s->GetName()+CString(_T(")\r\n")));
			#else
            LYNXCRC NewCRC = CObj::ComputeCRCCode(CRasterizerState::MakeName(desc));
			s = m_lpGraphicsSystem->GetlpEngine()->FindResource(NewCRC);
			//s = m_lpGraphicsSystem->GetlpEngine()->FindResource(CRasterizerState::MakeName(desc));
			if (s == NULL)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, RASTERIZER_STATE);
				s->vCreate(desc);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateRasterizerState("))+s->GetName()+CString(_T(")\r\n")));
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Rasterizer state("))+s->GetName()+CString(_T(") is already loaded\r\n")));
			}
		#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		CRasterizerState* CRenderer::SetRasterizerState(CRasterizerState* s)
		{
			CRasterizerState* S = m_CachedRasterizerState;

			if (m_bUseCachedState)
			{
				if (s == m_CachedRasterizerState)
					return S;
			}

			s->vSet();
			INC_COUNTER(m_SetRenderStateCounter, 1)
			PERF_COUNTER_INC(_T("Renderer"), _T("SetRS"), 1)

			m_CachedRasterizerState = s;

			return S;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::CreateSamplerState(CSamplerStatePtr& s, const SAMPLERSTATEDESC& desc)
		{
			// Don't shared render state object in tool because we don't want the change on one state object affact to other state objects
			#if defined(__TOOL__)
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, SAMPLER_STATE);
				s->vCreate(desc);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateSamplerState("))+s->GetName()+CString(_T(")\r\n")));
			#else
            LYNXCRC NewCRC = CObj::ComputeCRCCode(CSamplerState::MakeName(desc));
			s = m_lpGraphicsSystem->GetlpEngine()->FindResource(NewCRC);
			//s = m_lpGraphicsSystem->GetlpEngine()->FindResource(CSamplerState::MakeName(desc));
			if (s == NULL)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(s, SAMPLER_STATE);
				s->vCreate(desc);
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateSamplerState("))+s->GetName()+CString(_T(")\r\n")));
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::Sampler state("))+s->GetName()+CString(_T(") is already loaded\r\n")));
			}
		#endif
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		CSamplerState* CRenderer::SetSamplerState(int channel, CSamplerState* s)
		{
			CSamplerState* S = m_CachedSamplerState[channel];

			/*
            // Currently, sampler state is attached to texture so it wouldn't need to check the cached state since cached texture
            // has been checked. In the future, think about moving samper state out of texture so same texture can work with differnt
            // sampler state object.
            if (m_bUseCachedState)
            {
                if (s == m_CachedSamplerState[channel])
                    return S;
            }
		    */

			s->vSet(channel);
			INC_COUNTER(m_SetRenderStateCounter, 1)
			PERF_COUNTER_INC(_T("Renderer"), _T("SetRS"), 1)

			m_CachedSamplerState[channel] = s;

			return S;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		void CRenderer::CreateVertexLayout(CVertexLayoutPtr& v, CString& name, CRenderer::VERTEXSLOT* vs, int num)
		{
			v = m_lpGraphicsSystem->GetlpEngine()->FindResource(name);
			if (v == NULL)
			{
				m_lpGraphicsSystem->GetlpEngine()->CreateResource(v, VERTEX_LAYOUT);
				v->SetName(name);
				v->vCreate(vs, num);				
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::CreateVertexLayout("))+v->GetName()+CString(_T(")\r\n")));
			}
			else
			{
				LYNX_LOG(glpLogger, CString(GetClassName())+CString(_T("::VertexLayout("))+v->GetName()+CString(_T(") is already loaded\r\n")));
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		CShaderGroup* CRenderer::vSetShaderGroup(CShaderGroup* lpsg, CContainer* lpo, CCameraContainer* cam)
		{
			CShaderGroup* S = m_CachedShaderGroup;
					
			/*
			if (m_bUseCachedState)
			{
				if (lpsg == m_CachedShaderGroup && lpo == m_CachedShaderGroupContainer && cam == m_CachedShaderGroupCamera)
					return S;
			}					
			m_CachedShaderGroupContainer = lpo;
            m_CachedShaderGroupCamera = cam;
            */
            m_CachedShaderGroup = lpsg;
			lpsg->vSet(lpo, cam);			
			
			return S;		
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  隤芣?:   
		//-------------------------------------------------------------------------------------------------------
		DWORD CRenderer::GetFormatBytes(FORMAT f)
		{
			switch (f)
			{
				default:
				case FORMAT_NULL:
					return 0;
					break;
				case FORMAT_R8G8B8:
					return 3;
					break;
				case FORMAT_A8R8G8B8:
					return 4;          
					break;
				case FORMAT_X8R8G8B8:
					return 4;           
					break;
				case FORMAT_R5G6B5:
					return 2;           
					break;
				case FORMAT_X1R5G5B5:
					return 2;           
					break;
				case FORMAT_A1R5G5B5:
					return 2;           
					break;
				case FORMAT_A4R4G4B4:
					return 2;           
					break;
				case FORMAT_R3G3B2:
					return 1;           
					break;
				case FORMAT_A8:
					return 1;           
					break;
				case FORMAT_A8R3G3B2:
					return 2;           
					break;
				case FORMAT_X4R4G4B4:
					return 2;           
					break;
				case FORMAT_A2B10G10R10:
					return 4;           
					break;
				case FORMAT_A8B8G8R8:
					return 4;           
					break;
				case FORMAT_X8B8G8R8:
					return 4;           
					break;
				case FORMAT_G16R16:
					return 4;           
					break;
				case FORMAT_A2R10G10B10:
					return 4;           
					break;
				case FORMAT_A16B16G16R16:
					return 8;           
					break;
				case FORMAT_A8P8:
					return 2;           
					break;
				case FORMAT_P8:
					return 1;           
					break;
				case FORMAT_L8:
					return 1;           
					break;                 
				case FORMAT_A8L8:
					return 2;           
					break;
				case FORMAT_A4L4:
					return 1;           
					break;
				case FORMAT_V8U8:
					return 2;           
					break;
				case FORMAT_L6V5U5:
					return 2;           
					break;
				case FORMAT_X8L8V8U8:
					return 3;           
					break;
				case FORMAT_Q8W8V8U8:
					return 4;           
					break;
				case FORMAT_V16U16:
					return 4;           
					break;
				case FORMAT_A2W10V10U10:
					return 4;           
					break;
				case FORMAT_UYVY:
					return 1;           
					break;
				case FORMAT_R8G8_B8G8:
					return 4;           
					break;
				case FORMAT_YUY2:
					return 1;           
					break;
				case FORMAT_G8R8_G8B8:
					return 4;           
					break;
				case FORMAT_DXT1:
					return 4;           
					break;
				case FORMAT_DXT2:
					return 4;           
					break;
				case FORMAT_DXT3:
					return 4;           
					break;
				case FORMAT_DXT4:
					return 4;           
					break;
				case FORMAT_DXT5:
					return 4;           
					break;
				case FORMAT_ATI1N:
					return 4;
					break;
				case FORMAT_ATI2N:
					return 4;
					break;
				case FORMAT_D16_LOCKABLE:
					return 2;           
					break;
				case FORMAT_D32:
					return 4;           
					break;
				case FORMAT_D15S1:
					return 2;           
					break;
				case FORMAT_D24S8:
					return 4;           
					break;
				case FORMAT_D24X8:
					return 4;           
					break;
				case FORMAT_D24X4S4:
					return 4;           
					break;
				case FORMAT_D16:
					return 2;           
					break;
				case FORMAT_D32F_LOCKABLE:
					return 4;           
					break;      
				case FORMAT_D24FS8:
					return 4;           
					break;
				case FORMAT_L16:
					return 2;           
					break;
				case FORMAT_VERTEXDATA:
					return 1;           
					break;
				case FORMAT_INDEX16:
					return 2;           
					break;
				case FORMAT_INDEX32:
					return 4;           
					break;
				case FORMAT_Q16W16V16U16:
					return 8;           
					break;
				case FORMAT_MULTI2_ARGB8:
					return 4;           
					break;
				case FORMAT_R16F:
					return 2;           
					break;
				case FORMAT_G16R16F:
					return 4;           
					break;
				case FORMAT_A16B16G16R16F:
					return 8;           
					break;
				case FORMAT_R32F:
					return 4;           
					break;
				case FORMAT_G32R32F:
					return 8;           
					break;
				case FORMAT_A32B32G32R32F:
					return 16;           
					break;
				case FORMAT_CxV8U8:
					return 2;           
					break;
			}
		}
	}
}
