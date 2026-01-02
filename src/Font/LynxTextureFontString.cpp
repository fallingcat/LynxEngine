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
#include <GraphicsSystem/LynxVertexLayout.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Font/LynxTextureFont.h>
#include <Font/LynxTextureFontString.h>
#include <Shader/LynxSpriteVS.h>
#include <Shader/LynxFontStringVS.h>

namespace LynxEngine
{	
	#ifdef __MOBILE__
		int CTextureFontString::m_BatchSize = 64; 
	#elif defined __DESKTOP__
		int CTextureFontString::m_BatchSize = 96; 
	#endif	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CTextureFontString::CTextureFontString(CEngine *lpengine)	
	: CSpriteContainer(lpengine)
	{
		//m_lpFont = NULL;
		m_MaxLen = 0;
		m_AlignmentType = H_ALIGN_CENTER | V_ALIGN_CENTER;
		m_OriginType = OT_CENTER;
        m_bAutoReturn = LYNX_FALSE;
		m_bSpaceChecking = LYNX_TRUE;
		m_Scale.x = 1.0f;
		m_Scale.y = 1.0f;

		m_PosArray = NULL;
		m_TexCoordArray = NULL;
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
	CTextureFontString::~CTextureFontString(void)
	{		
		vRelease();
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
	void CTextureFontString::vRelease(void)
	{	
		if (m_PosArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_PosArray);
			m_PosArray = NULL;
		}

		if (m_TexCoordArray)
		{
			LYNXGLOBAL_DEL_ARRAY(m_TexCoordArray);
			m_TexCoordArray = NULL;
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
	LYNXBOOL CTextureFontString::Create(DWORD maxlen, LYNXBOOL bbo)
	{			
		vRelease();

		m_PosArray		= LYNXGLOBAL_NEW LYNXVECTOR4D[m_BatchSize];
		m_TexCoordArray = LYNXGLOBAL_NEW LYNXVECTOR4D[m_BatchSize];

		m_MaxLen = m_BatchSize;

		m_lpEngine->CreateResource(m_VertexArray, VERTEX_ARRAY);
		m_lpEngine->ClearInvalidResource();
		m_VertexArray->SetName(m_Name+CString(_T(".VertexArray")));
		GraphicsSystem::FONTVERTEX* lpFV = LYNXGLOBAL_NEW GraphicsSystem::FONTVERTEX[m_MaxLen*4];
		CReal W = 0.5f;
		CReal H = 0.5f;
		for (int i=0; i<m_MaxLen; ++i)
		{
			lpFV[i*4+0].TexCoord.u = 0.0f;	lpFV[i*4+0].TexCoord.v = 0.0f;
			lpFV[i*4+1].TexCoord.u = 1.0f;	lpFV[i*4+1].TexCoord.v = 0.0f;
			lpFV[i*4+2].TexCoord.u = 0.0f;	lpFV[i*4+2].TexCoord.v = 1.0f;
			lpFV[i*4+3].TexCoord.u = 1.0f;	lpFV[i*4+3].TexCoord.v = 1.0f;				

			lpFV[i*4+0].Instance = i;
			lpFV[i*4+1].Instance = i;
			lpFV[i*4+2].Instance = i;
			lpFV[i*4+3].Instance = i;				
		}		
		m_VertexArray->vCreate(m_lpEngine->FindVertexLayout(GetEnumStr(GraphicsSystem::VT_FONT)), 4*m_MaxLen, GraphicsSystem::CRenderer::STATIC, bbo, lpFV);				
		LYNXGLOBAL_DEL_ARRAY(lpFV);
		
		m_lpEngine->CreateResource(m_IndexArray, INDEX_ARRAY);
		m_lpEngine->ClearInvalidResource();
		m_IndexArray->SetName(m_Name+CString(_T(".IndexArray")));
		WORD *lpI = LYNXGLOBAL_NEW WORD[m_MaxLen*6];	
		for (int i=0; i<m_MaxLen; i++)
		{
			lpI[i*6] = i*4;
			lpI[i*6+1] = i*4+1;
			lpI[i*6+2] = i*4+2;
			
			lpI[i*6+3] = i*4+2;
			lpI[i*6+4] = i*4+1;
			lpI[i*6+5] = i*4+3;		
		}			
		m_IndexArray->vCreate(sizeof(WORD), 6*m_MaxLen, GraphicsSystem::CRenderer::STATIC|GraphicsSystem::CRenderer::WRITEONLY, bbo, lpI);	
		LYNXGLOBAL_DEL_ARRAY(lpI);

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
	void CTextureFontString::SetFont(CFontPtr& lpfont)
	{
		if (m_lpFont == lpfont)
			return;
		
		m_lpFont = lpfont;
		SetlpCurrentMaterial(static_cast<CTextureFont*>((CFont*)m_lpFont)->m_lpMaterial);
		SetCurrentMaterialTechnique(MaterialSystem::CMaterial::SIMPLE);	
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
	int CTextureFontString::Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{		
		LYNXCHAR	Buffer[512];
		va_list		argptr;		
	    LYNXRECT	OldRect, Rect;		
        
		if (!m_lpFont)
			return 0;

		CTextureFont* Font = static_cast<CTextureFont*>((CFont*)m_lpFont);

        va_start(argptr,fmt);
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);
        
        int Len = PrintfLength(Buffer);
	
        Rect.x1 = x;
        Rect.x2 = Rect.x1 + Len;
        Rect.y1 = y;
		Rect.y2 = Rect.y1 + (Font->m_CharHeight * m_Scale.y);
        
		ORIGINTYPE OldOT = SetOriginType(CSpriteContainer::OT_UPPER_LEFT);
		DWORD OldAT = SetAlignmentType(CSpriteContainer::H_ALIGN_LEFT | CSpriteContainer::V_ALIGN_CENTER);
		GetAlignmentRect(&OldRect);
        SetAlignmentRect(&Rect);
        
		Printf(lpc, Buffer);

		SetOriginType(OldOT);
		SetAlignmentType(OldAT);
		SetAlignmentRect(&OldRect);

		return Len;
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
	Math::CVector2 CTextureFontString::Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{		
		LYNXCHAR	Buffer[512];
		va_list		argptr;		
		CReal		W, H;
		DWORD		C;
		DWORD		Len;
		Math::CVector2 Size;
		
		if (!m_lpFont)
			return CVector2(0.0f, 0.0f);

		CTextureFont* Font = static_cast<CTextureFont*>((CFont*)m_lpFont);

		va_start(argptr,fmt);		
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);		

		LYNXBOOL bUTF8 = LYNX_FALSE;

		#ifndef _UNICODE
			if (lynxIsUnicodeString(Buffer))
			{
                bUTF8 = LYNX_TRUE;
			}		
        #endif

		Size = PrintfSize(Buffer);		
		H = Font->m_CharHeight * m_Scale.y;
		// Align string
		Align(Size.x, H);

		CReal Offset = 0.0f;
		int OffsetX = Size.x*0.5f;
		int OffsetY = 0;		
		int NumBatchCharacters = 0;
		
		H = Font->m_CharHeight * m_Scale.y;
		Offset = 0;
		m_AlignmentOffset.x = m_AlignmentOffset.y = 0;

		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		if (lpc)
			m_lpCurrentMaterial->m_Diffuse.PackedColor = lpc->PackedColor;			
		lpR->SetVertexArray(m_VertexArray, 0, 0);
		lpR->SetIndexArray(m_IndexArray, 0);					
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
		Technique.GetPass(0).Set(this, NULL);	
		Shader::CFontStringVS* lpShader = static_cast<Shader::CFontStringVS*>((GraphicsSystem::CShaderClass*)(Technique.GetPass(0).GetVS()));

        LYNXCHAR* CurrentStr = Buffer;
		while ((*CurrentStr))
		{
            if (bUTF8)
			{
				C = lynxDecodeUnicode(&CurrentStr, LYNX_UTF8);
			}
			else
			{
				C = (*CurrentStr);
				CurrentStr++;
			}
            
			if (C == 10)
			{
				OffsetY -= Font->m_CharHeight * m_Scale.y;
				Offset = 0;                    
			}
			else if (C == 0x20)
			{
				Offset += (Font->m_CharData[0x20].Width + Font->m_CharData[0x20].Offset.x) * m_Scale.x;
				if (m_bAutoReturn)
				{
					if (m_bSpaceChecking)
					{
						LYNXCHAR* Word = CurrentStr;
						int WordLength = Offset;
						DWORD Code;
                        
						while ((*Word))
						{
							Code = lynxDecodeUnicode(&Word, LYNX_UTF8);
							if (Code == 0x20)
								break;
                            
							WordLength += (Font->m_CharData[Code].Width + Font->m_CharData[Code].Offset.x) * m_Scale.x;
						}
						//WordLength -= m_lpFont->m_CharData[Code].Offset.x * m_Scale;
						if (WordLength >= float(m_AlignmentRect.x2 - m_AlignmentRect.x1))
						{
							OffsetY -= Font->m_CharHeight * m_Scale.y;
							Offset = 0;                    
						}
					}
					else
					{
						if (Offset >= float(m_AlignmentRect.x2 - m_AlignmentRect.x1))
						{
							OffsetY -= Font->m_CharHeight * m_Scale.y;
							Offset = 0;
						}						
					}
				}
			}
			else
			{
				W = Font->m_CharData[C].Width * m_Scale.x;
				H = Font->m_CharData[C].Height * m_Scale.y;
			
                if (m_bAutoReturn)
				{
					if ((Offset + (Font->m_CharData[C].Width + Font->m_CharData[C].Offset.x)* m_Scale.x) >= float(m_AlignmentRect.x2 - m_AlignmentRect.x1))
					{
						OffsetY -= Font->m_CharHeight * m_Scale.y;
						Offset = 0;
					}						
				}
                
				float OriginX = Offset - OffsetX;

				m_PosArray[NumBatchCharacters].x = 0 + OriginX;
				m_PosArray[NumBatchCharacters].y = OffsetY + ((Font->m_CharHeight * 0.5f) - (Font->m_CharData[C].Offset.y) + (Font->m_CharacterOffset.y)) * m_Scale.y;
				m_PosArray[NumBatchCharacters].z = W;	
				m_PosArray[NumBatchCharacters].w = -H;	

				m_TexCoordArray[NumBatchCharacters].x = Font->m_CharData[C].UV[0].u;
				m_TexCoordArray[NumBatchCharacters].y = Font->m_CharData[C].UV[0].v;
				m_TexCoordArray[NumBatchCharacters].z = Font->m_CharData[C].UV[3].u - Font->m_CharData[C].UV[0].u;
				m_TexCoordArray[NumBatchCharacters].w = Font->m_CharData[C].UV[3].v - Font->m_CharData[C].UV[0].v;
				
				Offset += (Font->m_CharData[C].Width + Font->m_CharData[C].Offset.x) * m_Scale.x;
				NumBatchCharacters++;
			}

			if (NumBatchCharacters >= m_MaxLen)
			{
				lpShader->SetParam((float*)m_PosArray, (float*)m_TexCoordArray, NumBatchCharacters);				
				lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumBatchCharacters*4, 0, NumBatchCharacters*2);		
				NumBatchCharacters = 0;
			}
		}		
		if (NumBatchCharacters > 0)
		{
			lpShader->SetParam((float*)m_PosArray, (float*)m_TexCoordArray, NumBatchCharacters);		
			lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumBatchCharacters*4, 0, NumBatchCharacters*2);		
		}

		return Size;
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
	int CTextureFontString::PrintfLength(const LYNXCHAR *fmt, ...)
	{				
		LYNXCHAR	Buffer[512];
		va_list		argptr;		
		CReal		W, H;
		DWORD		C;
		DWORD		Len;
		
		va_start(argptr,fmt);		
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);			

		return PrintfSize(Buffer).x;
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
	Math::CVector2 CTextureFontString::PrintfSize(const LYNXCHAR *fmt, ...)
	{		
		LYNXCHAR	Buffer[512];
		va_list		argptr;		
		CReal		W, H;
		DWORD		C;
		DWORD		Len;
		Math::CVector2 Size;
		
		if (!m_lpFont)
			return CVector2(0.0f, 0.0f);

		CTextureFont* Font = static_cast<CTextureFont*>((CFont*)m_lpFont);

		va_start(argptr,fmt);		
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);			

		LYNXBOOL bUTF8 = LYNX_FALSE;

		#ifndef _UNICODE
			if (lynxIsUnicodeString(Buffer))
			{
                bUTF8 = LYNX_TRUE;
			}		
        #endif

		CReal Offset = 0.0f;
		int OffsetX = Offset*0.5f;
		int OffsetY = 0;
		H = Font->m_CharHeight * m_Scale.y;
        LYNXCHAR* CurrentStr = Buffer;
		Size.x = -1;
        
		while ((*CurrentStr))
		{
            if (bUTF8)
			{
				C = lynxDecodeUnicode(&CurrentStr, LYNX_UTF8);
			}
			else
			{
				C = (*CurrentStr);
				CurrentStr++;
			}
            
			if (C == 10)
			{
				OffsetY -= Font->m_CharHeight * m_Scale.y;
				Offset = 0;                    				
			}
			else if (m_bAutoReturn && C == 0x20)
			{
				Offset += (Font->m_CharData[0x20].Width + Font->m_CharData[0x20].Offset.x) * m_Scale.x;
				if (int(Offset) > Size.x)
					Size.x = Offset;
				if (m_bSpaceChecking)
				{
					LYNXCHAR* Word = CurrentStr;
					int WordLength = Offset;
					DWORD Code;
                    
					while ((*Word))
					{
						Code = lynxDecodeUnicode(&Word, LYNX_UTF8);
						if (Code == 0x20)
							break;
                        
						WordLength += (Font->m_CharData[Code].Width + Font->m_CharData[Code].Offset.x) * m_Scale.x;
					}						
					if (WordLength >= float(m_AlignmentRect.x2 - m_AlignmentRect.x1))
					{
						OffsetY -= Font->m_CharHeight * m_Scale.y;
						Offset = 0;                    
					}
				}
				else
				{
					if (Offset >= float(m_AlignmentRect.x2 - m_AlignmentRect.x1))
					{
						OffsetY -= Font->m_CharHeight * m_Scale.y;
						Offset = 0;
					}						
				}								
			}
			else
			{
				if (Font->m_CharData.is_exist(C))
				{
					W = Font->m_CharData[C].Width * m_Scale.x;
					H = Font->m_CharData[C].Height * m_Scale.y;
			
					if (m_bAutoReturn)
					{
						if ((Offset + (Font->m_CharData[C].Width + Font->m_CharData[C].Offset.x)* m_Scale.x) >= float(m_AlignmentRect.x2 - m_AlignmentRect.x1))
						{
							OffsetY -= Font->m_CharHeight * m_Scale.y;
							Offset = 0;
						}						
					}                
					Offset += (Font->m_CharData[C].Width + Font->m_CharData[C].Offset.x) * m_Scale.x;
					if (int(Offset) > Size.x)
						Size.x = Offset;
				}
			}	
		}
		Size.y = -(OffsetY - Font->m_CharHeight * m_Scale.y);

		return Size;
	}
}