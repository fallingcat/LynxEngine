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
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterialSystem.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Font/LynxTextureFont.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CTextureFont::CTextureFont(CEngine* const lpengine)
	: CFont(lpengine)
	{
		m_FontType = FT_TEXTURE_FONT;
        m_CharacterOffset.x = 0.0f;
        m_CharacterOffset.y = 0.0f;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CTextureFont::~CTextureFont(void)
	{			
		m_CharData.clear();
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CTextureFont::Create()
	{			
		return LYNX_TRUE;
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CTextureFont::SetCharacterOffset(const Math::CVector2& offset) 
	{
		for (CMap<DWORD, FONTDATA>::CIterator c=m_CharData.begin(); c!=m_CharData.end(); c++)
		{		
			(*c).second.Offset = offset;
		}		
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CTextureFont::SetCharacterOffset(const DWORD i, const Math::CVector2& offset) 
	{
		m_CharData[i].Offset = offset;				
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CTextureFont::LoadFixed(const LYNXCHAR *filename, CReal cw, CReal ch, int si, int num, int x, int y, int w, int h)
	{
		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;
		
		DDesc.Default();		
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();
				
		if (!m_lpMaterial)
		{
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_lpMaterial, MaterialSystem::CMaterial::STANDARD);	
			m_lpMaterial->SetResourceFlag(SYSTEM_DEFINED);
			m_lpMaterial->SetName(m_Name + CString(_T(".Material")));
			m_lpMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).vCreatePass(1);
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vSetNumTextureAnimation(1);
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->Create(1);
			GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;
			TexDesc.Default();
			TexDesc.bSRGB = LYNX_FALSE;
            TexDesc.MipMapLevel = 1;				
            TexDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
            TexDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
            TexDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
            TexDesc.SamplerDesc.MinLOD = 0;
            TexDesc.SamplerDesc.MaxLOD = 0;
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->LoadTexture(0, *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), filename, &TexDesc);	
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());		
			
			RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateRasterizerState(RDesc);		
			
			DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
			DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateDepthStencilState(DDesc);	

			BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
			BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
			BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateBlendState(BDesc);				
				
			m_lpMaterial->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_FONT));		
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("FontStringVS", "DiffuseMapXColorPS");
			//m_lpMaterial->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE));		
			//m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("SpriteVS", "DiffuseMapXColorPS");
		}

		float Width = m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0)->vGetWidth();
		float Height = m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0)->vGetHeight();

		int TU = x;
		int TV = y;
		m_CharHeight = ch;
		for (int i=si; i<si+num; ++i)
		{
			m_CharData[i].Width = cw;
            m_CharData[i].Height = m_CharHeight;
			m_CharData[i].UV[0].u = (float)TU/Width;		m_CharData[i].UV[0].v = (float)TV/Height;
			m_CharData[i].UV[1].u = (float)(TU+w)/Width;	m_CharData[i].UV[1].v = (float)TV/Height;
			m_CharData[i].UV[2].u = (float)TU/Width;		m_CharData[i].UV[2].v = (float)(TV+h)/Height;
			m_CharData[i].UV[3].u = (float)(TU+w)/Width;	m_CharData[i].UV[3].v = (float)(TV+h)/Height;
			m_CharData[i].Offset = Math::CVector2(0.0f, 0.0f);

			TU += w;
		}

		m_CharData[0x20].Width = m_CharData[0x22].Width;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CTextureFont::LoadVar(const LYNXCHAR *filename, int row, CReal scale, int* si, int* num, int* x, int* y, int w[][128], int h, int tw, int th)
	{
        GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

		DDesc.Default();		
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_lpMaterial, MaterialSystem::CMaterial::STANDARD);	
		m_lpMaterial->SetResourceFlag(SYSTEM_DEFINED);
		m_lpMaterial->SetName(m_Name + CString(_T(".Material")));
		m_lpMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).vCreatePass(1);
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vSetNumTextureAnimation(1);
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->Create(1);
		GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;
		TexDesc.Default();
		TexDesc.bSRGB = LYNX_FALSE;
        TexDesc.MipMapLevel = 1;				
        TexDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
        TexDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
        TexDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
        TexDesc.SamplerDesc.MinLOD = 0;
        TexDesc.SamplerDesc.MaxLOD = 0;
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->LoadTexture(0, *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), filename, &TexDesc);	
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
				
		RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateRasterizerState(RDesc);		
		
		DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
		DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateDepthStencilState(DDesc);
		
		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
		BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
		BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateBlendState(BDesc);				
				
		m_lpMaterial->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_FONT));		
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("FontStringVS", "DiffuseMapXColorPS");
		//m_lpMaterial->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE));		
		//m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("SpriteVS", "DiffuseMapXColorPS");
		
		float Width;
		float Height;

		if (tw == -1 || th == -1)
		{
			Width = m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0)->vGetWidth();
			Height = m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0)->vGetHeight();
		}
		else
		{
			Width = tw;
			Height = th;
		}
		
		for (int Row=0; Row<row; Row++)
		{
			int TU = x[Row];
			int TV = y[Row];
			m_CharHeight = h*scale;
			for (int i=si[Row]; i<si[Row]+num[Row]; ++i)
			{
				int CharcterWidth = (w[Row][i-si[Row]]);
				m_CharData[i].Width = CharcterWidth*scale;
                m_CharData[i].Height = m_CharHeight;
				m_CharData[i].UV[0].u = (float)TU/Width;					m_CharData[i].UV[0].v = (float)TV/Height;
				m_CharData[i].UV[1].u = (float)(TU+CharcterWidth)/Width;	m_CharData[i].UV[1].v = (float)TV/Height;
				m_CharData[i].UV[2].u = (float)TU/Width;					m_CharData[i].UV[2].v = (float)(TV+h)/Height;
				m_CharData[i].UV[3].u = (float)(TU+CharcterWidth)/Width;	m_CharData[i].UV[3].v = (float)(TV+h)/Height;
				m_CharData[i].Offset = Math::CVector2(0.0f, 0.0f);

				TU += CharcterWidth;
			}
		}
		m_CharData[0x20].Width = m_CharData[0x22].Width;
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CTextureFont::LoadVarByCharacterOffset(const LYNXCHAR *filename, int row, CReal scale, int* si, int* num, int* x, int* y, int off[][128], int h, int tw, int th)
	{
        GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

		DDesc.Default();		
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_lpMaterial, MaterialSystem::CMaterial::STANDARD);	
		m_lpMaterial->SetResourceFlag(SYSTEM_DEFINED);
		m_lpMaterial->SetName(m_Name + CString(_T(".Material")));
		m_lpMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).vCreatePass(1);
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vSetNumTextureAnimation(1);
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->Create(1);
		GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;
		TexDesc.Default();
		TexDesc.bSRGB = LYNX_FALSE;
        TexDesc.MipMapLevel = 1;				
        TexDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
        TexDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
        TexDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
        TexDesc.SamplerDesc.MinLOD = 0;
        TexDesc.SamplerDesc.MaxLOD = 0;
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->LoadTexture(0, *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), filename, &TexDesc);	
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
				
		RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateRasterizerState(RDesc);		
		
		DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
		DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateDepthStencilState(DDesc);
		
		BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
		BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
		BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
		BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateBlendState(BDesc);				
				
		m_lpMaterial->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_FONT));		
		m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("FontStringVS", "DiffuseMapXColorPS");
		//m_lpMaterial->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_SPRITE));		
		//m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("SpriteVS", "DiffuseMapXColorPS");
		
		float Width;
		float Height;

		if (tw == -1 || th == -1)
		{
			Width = m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0)->vGetWidth();
			Height = m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->GetTexture(0)->vGetHeight();
		}
		else
		{
			Width = tw;
			Height = th;
		}
		
		for (int Row=0; Row<row; Row++)
		{
			int TU = x[Row];
			int TV = y[Row];
			m_CharHeight = h*scale;
			for (int i=si[Row]; i<si[Row]+num[Row]; ++i)
			{
				int CharcterWidth = (off[Row][(i+1)-si[Row]] - off[Row][i-si[Row]]);
				m_CharData[i].Width = CharcterWidth*scale;
                m_CharData[i].Height = m_CharHeight;
				m_CharData[i].UV[0].u = (float)TU/Width;					m_CharData[i].UV[0].v = (float)TV/Height;
				m_CharData[i].UV[1].u = (float)(TU+CharcterWidth)/Width;	m_CharData[i].UV[1].v = (float)TV/Height;
				m_CharData[i].UV[2].u = (float)TU/Width;					m_CharData[i].UV[2].v = (float)(TV+h)/Height;
				m_CharData[i].UV[3].u = (float)(TU+CharcterWidth)/Width;	m_CharData[i].UV[3].v = (float)(TV+h)/Height;
				m_CharData[i].Offset = Math::CVector2(0.0f, 0.0f);

				TU += CharcterWidth;
			}
		}
		m_CharData[0x20].Width = m_CharData[0x22].Width;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	void CTextureFont::LoadFnt(const LYNXCHAR *filename, float scale)
	{
		CPlatformFileStream File;
		CString Line;
		CArray<CString> TextureFilenameArray;
		LYNXCHAR* Value;
		CList<CString> ParamList;
		CString CurrentCommand;
		DWORD CharaterID;
        LYNXBOOL bASCII;
		int X, Y, Width, Height, XAdvance;
		Math::CVector2 Offset;
		int TextureWidth, TextureHeight;

        bASCII = LYNX_TRUE;
		TextureFilenameArray.clear();

		File.vOpen(filename, CStream::Read|CStream::Text|CStream::ANSI);				
		while (File.vRead(Line))
		{
            Offset = Math::CVector2(0.0f, 0.0f);
			ParamList.clear();			
			LYNXCHAR* Tok = lynxStrTok((LYNXCHAR*)Line.c_str(), _T(" "));
			if (Tok)
				CurrentCommand = Tok;
			else
				CurrentCommand = _T("");

			while (Tok)
			{
				ParamList.push_back(Tok);				
				Tok = lynxStrTok(NULL, _T(" "));
			}

			for (CList<CString>::CIterator Param = ParamList.begin(); Param != ParamList.end(); Param++)
			{
				LYNXCHAR* Tok2 = lynxStrTok((LYNXCHAR*)(*Param).c_str(), _T("="));
				if (Tok2)
				{
					if (CurrentCommand == CString(_T("page")))
					{
						if (!lynxStriCmp(Tok2, _T("file")))
						{
							Value = lynxStrTok(NULL, _T("="));
							Value = lynxStrTok(Value, _T("\""));							
							if (Value)
								TextureFilenameArray.push_back(CString(Value));
						}
					}
					else if (CurrentCommand == CString(_T("common")))
					{
						if (!lynxStriCmp(Tok2, _T("scaleW")))
						{
							Value = lynxStrTok(NULL, _T("="));
							if (Value)
								TextureWidth = lynxAtoi(Value);
						}
						else if (!lynxStriCmp(Tok2, _T("scaleH")))
						{
							Value = lynxStrTok(NULL, _T("="));
							if (Value)
								TextureHeight = lynxAtoi(Value);
						}
					}
					else if (CurrentCommand == CString(_T("char")))
					{
						if (!lynxStriCmp(Tok2, _T("id")))
						{
							Value = lynxStrTok(NULL, _T("="));			
							CharaterID = lynxAtoi(Value);
						}
						else if (!lynxStriCmp(Tok2, _T("x")))
						{
							Value = lynxStrTok(NULL, _T("="));							
							X = lynxAtoi(Value);
						}
						else if (!lynxStriCmp(Tok2, _T("y")))
						{
							Value = lynxStrTok(NULL, _T("="));
							Y = lynxAtoi(Value);
						}
						else if (!lynxStriCmp(Tok2, _T("width")))
						{
							Value = lynxStrTok(NULL, _T("="));
							Width = lynxAtoi(Value);
						}
						else if (!lynxStriCmp(Tok2, _T("height")))
						{
							Value = lynxStrTok(NULL, _T("="));
							Height = lynxAtoi(Value);
						}
						else if (!lynxStriCmp(Tok2, _T("xoffset")))
						{
							Value = lynxStrTok(NULL, _T("="));
                            int Number = lynxAtoi(Value);
                            if (Number >= 0)
                                Offset.x += -Number * scale;
						}
						else if (!lynxStriCmp(Tok2, _T("yoffset")))
						{
							Value = lynxStrTok(NULL, _T("="));
                            int Number = lynxAtoi(Value);
                            if (Number >= 0)
                                Offset.y += Number * scale;
						}
						else if (!lynxStriCmp(Tok2, _T("page")))
						{
							Value = lynxStrTok(NULL, _T("="));
						}
                        else if (!lynxStriCmp(Tok2, _T("xadvance")))
						{
							Value = lynxStrTok(NULL, _T("="));
                            XAdvance = lynxAtoi(Value);
						}
					}
				}				
			}

			if (CurrentCommand == CString(_T("char")))
			{
				int TU = X;
				int TV = Y;				
				int TW = Width;
				int TH = Height;// * 0.5f;

                if (TH * scale > m_CharHeight)
                    m_CharHeight = TH * scale;
                
				m_CharData[CharaterID].Height = TH * scale;
                m_CharData[CharaterID].Width = TW * scale;
				m_CharData[CharaterID].UV[0].u = (float)TU/TextureWidth;		m_CharData[CharaterID].UV[0].v = (float)TV/TextureHeight;
				m_CharData[CharaterID].UV[1].u = (float)(TU+TW)/TextureWidth;	m_CharData[CharaterID].UV[1].v = (float)TV/TextureHeight;
				m_CharData[CharaterID].UV[2].u = (float)TU/TextureWidth;		m_CharData[CharaterID].UV[2].v = (float)(TV+TH)/TextureHeight;
				m_CharData[CharaterID].UV[3].u = (float)(TU+TW)/TextureWidth;	m_CharData[CharaterID].UV[3].v = (float)(TV+TH)/TextureHeight;
				m_CharData[CharaterID].Offset = Offset + Math::CVector2((XAdvance - TW) * scale, 0.0f);//Offset;
                
                if (CharaterID > 255)
                {
                    bASCII = LYNX_FALSE;
                }
			}
		}
        
        if (!bASCII)
        {
            for (CMap<DWORD, FONTDATA>::CIterator Data = m_CharData.begin(); Data != m_CharData.end(); Data++)
            {
                (*Data).second.Offset.y -= m_CharHeight * 0.26f;
            }
        }
        //m_CharHeight *= 1.3f;
		//m_CharData[0x20].Width = m_CharData[0x22].Width;
		
		File.vClose();


        LYNXCHAR Path[512], Name[256], Ext[32];
        lynxSeparatePathFilenameExtByFullPath(Path, Name, Ext, filename);
        CString TexturePath = CString(Path) + CString(_T("/"));
		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(TexturePath.c_str());

		GraphicsSystem::CRenderer::DEPTHSTENCILSTATEDESC DDesc;
		GraphicsSystem::CRenderer::BLENDSTATEDESC		 BDesc;
		GraphicsSystem::CRenderer::RASTERIZERSTATEDESC	 RDesc;
		GraphicsSystem::CRenderer::SAMPLERSTATEDESC		 SamDesc;

		DDesc.Default();		
		BDesc.Default();
		RDesc.Default();
		SamDesc.Default();

		CString MatName = CString(filename) + CString(_T(".Material"));
		m_lpMaterial = m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->FindMaterial(MatName); 							
		if (!m_lpMaterial)
		{
			m_lpEngine->GetlpGraphicsSystem()->GetlpMaterialSystem()->CreateMaterial(m_lpMaterial, MaterialSystem::CMaterial::STANDARD);	
			m_lpMaterial->SetResourceFlag(SYSTEM_DEFINED);
			m_lpMaterial->SetName(MatName);
			m_lpMaterial->vAddTechnique(MaterialSystem::CMaterial::SIMPLE);
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).vCreatePass(1);
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vSetNumTextureAnimation(1);
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->Create(1);
			GraphicsSystem::CRenderer::TEXTUREDESC TexDesc;
			TexDesc.Default();
			TexDesc.bSRGB = LYNX_FALSE;
			TexDesc.MipMapLevel = 1;				
			TexDesc.SamplerDesc.AddressU = GraphicsSystem::CRenderer::AM_CLAMP;
			TexDesc.SamplerDesc.AddressV = GraphicsSystem::CRenderer::AM_CLAMP;
			TexDesc.SamplerDesc.AddressW = GraphicsSystem::CRenderer::AM_CLAMP;
			TexDesc.SamplerDesc.MinLOD = 0;
			TexDesc.SamplerDesc.MaxLOD = 0;
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->LoadTexture(0, *(m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream()), TextureFilenameArray[0].c_str(), &TexDesc);	
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).m_TextureAnimationArray[0]->SetShaderConstantName(MaterialSystem::CMaterialSystem::GetMapString(MaterialSystem::DIFFUSEMAP).c_str());
				
			RDesc.CullMode = GraphicsSystem::CRenderer::CULL_NONE;
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateRasterizerState(RDesc);		
		
			DDesc.DepthMode = GraphicsSystem::CRenderer::DB_NONE;
			DDesc.DepthFunc = GraphicsSystem::CRenderer::TF_ALWAYS;			
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateDepthStencilState(DDesc);
		
			BDesc.RTBlendParam[0].bBlend	= LYNX_TRUE;
			BDesc.RTBlendParam[0].SrcBlend	= GraphicsSystem::CRenderer::BF_SRC_ALPHA;
			BDesc.RTBlendParam[0].DestBlend	= GraphicsSystem::CRenderer::BF_ONE_MINUS_SRC_ALPHA;
			BDesc.RTBlendParam[0].BlendOp	= GraphicsSystem::CRenderer::BO_ADD;
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).CreateBlendState(BDesc);				
				
			m_lpMaterial->CreateVertexLayout(GetEnumStr(GraphicsSystem::VT_FONT));		
			m_lpMaterial->GetTechnique(MaterialSystem::CMaterial::SIMPLE).GetPass(0).vCreateShaderClasses("FontStringVS", "DiffuseMapXColorPS");
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	int CTextureFont::GetCharacterWidth(int c)
	{
		return (m_CharData[c].Width + m_CharData[c].Offset.x);
	}
}