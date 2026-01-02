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
#include <Font/LynxFont.h>
#include <Font/LynxFontString.h>
#include <Shader/LynxSpriteVS.h>
#include <Shader/LynxFontStringVS.h>

namespace LynxEngine
{	
	#ifdef __MOBILE__
		int CFontString::m_BatchSize = 64; 
	#elif defined __DESKTOP__
		int CFontString::m_BatchSize = 96; 
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
	CFontString::CFontString(CEngine *lpengine)	
	: CSpriteContainer(lpengine)
	{
		m_lpFont = NULL;
		m_MaxLen = 0;
		m_AlignmentType = H_ALIGN_CENTER | V_ALIGN_CENTER;
		m_OriginType = OT_CENTER;
        m_bAutoReturn = LYNX_FALSE;

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
	CFontString::~CFontString(void)
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
	void CFontString::vRelease(void)
	{	
		LYNXDEL [] m_PosArray;
		LYNXDEL [] m_TexCoordArray;
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
	LYNXBOOL CFontString::Create(DWORD maxlen, LYNXBOOL bbo)
	{			
		m_PosArray		= LYNXCORE_NEW LYNXVECTOR4D[m_BatchSize];
		m_TexCoordArray = LYNXCORE_NEW LYNXVECTOR4D[m_BatchSize];

		m_MaxLen = m_BatchSize;

		m_lpEngine->CreateResource(m_VertexArray, VERTEX_ARRAY);
		m_lpEngine->ClearInvalidResource();
		m_VertexArray->SetName(m_Name+CString(_T(".VertexArray")));
		GraphicsSystem::FONTVERTEX* lpFV = LYNXCORE_NEW GraphicsSystem::FONTVERTEX[m_MaxLen*4];
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
		LYNXCORE_DEL [] lpFV;
		
		m_lpEngine->CreateResource(m_IndexArray, INDEX_ARRAY);
		m_lpEngine->ClearInvalidResource();
		m_IndexArray->SetName(m_Name+CString(_T(".IndexArray")));
		WORD *lpI = LYNXCORE_NEW WORD[m_MaxLen*6];	
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
		LYNXCORE_DEL [] lpI;

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
	void CFontString::SetFont(CFont* lpfont)
	{
		if (m_lpFont == lpfont)
			return;
		
		m_lpFont = lpfont;
		SetlpCurrentMaterial(m_lpFont->m_lpMaterial);
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
	void CFontString::Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{		
		LYNXCHAR	Buffer[512];
		va_list		argptr;		
	    LYNXRECT Rect;
        
        va_start(argptr,fmt);
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);
        
        int Len = PrintfLength(Buffer);
	
        Rect.x1 = x;
        Rect.x2 = Rect.x1 + Len;
        Rect.y1 = y;
        Rect.y2 = Rect.y1 + m_lpFont->m_CharHeight;
        
        SetAlignmentRect(&Rect);
        Printf(lpc, Buffer);
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
	void CFontString::Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{		
		LYNXCHAR	Buffer[512];
		va_list		argptr;		
		CReal		W, H;
		int			C;
		DWORD		Len, NumCharacters;
		
		va_start(argptr,fmt);		
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);			

		NumCharacters = 0;
		Len = lynxStrLen(Buffer);
		CReal Offset = 0.0f;
		H = m_lpFont->m_CharHeight;
		for (int i=0; i<Len; i++)
		{			
			C = Buffer[i]-0x21;
			if (C >= 256) 
				C = -1;			
			if (C < 0)
				Offset += (m_lpFont->m_CharWidth[1] + m_lpFont->m_CharacterOffset.x);
			else
			{
				Offset += (m_lpFont->m_CharWidth[C] + m_lpFont->m_CharacterOffset.x);
				NumCharacters++;
			}
		}		
		
		// Align string
		Align(Offset, H);
		int OffsetX = Offset*0.5f;
        int OffsetY = 0;

		int NumBatchCharacters = 0;
		int i = 0;
		H = m_lpFont->m_CharHeight;
		Offset = 0;//m_AlignmentOffset.x;		
		m_AlignmentOffset.x = m_AlignmentOffset.y = 0;

		GraphicsSystem::CRenderer* lpR = ((CEngine*)m_lpEngine)->GetlpGraphicsSystem()->GetlpRenderer();
		if (lpc)
			m_lpCurrentMaterial->m_Diffuse.PackedColor = lpc->PackedColor;			
		lpR->SetVertexArray(m_VertexArray, 0, 0);
		lpR->SetIndexArray(m_IndexArray, 0);					
		MaterialSystem::CTechnique& Technique = m_lpCurrentMaterial->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0);
		Technique.GetPass(0).Set(this, NULL);	
		Shader::CFontStringVS* lpShader = static_cast<Shader::CFontStringVS*>((GraphicsSystem::CShaderClass*)(Technique.GetPass(0).GetVS()));

		while (Buffer[i])
		{			
			C = Buffer[i]-0x21;
			if (C >= 256) 
				C = -1;
			if (C < 0)
			{
				Offset += (m_lpFont->m_CharWidth[1] + m_lpFont->m_CharacterOffset.x);
                if (m_bAutoReturn)
                {
                    int WordIndex = i+1;
                    int WordLength = Offset;
                    while (Buffer[WordIndex] && ((int)(Buffer[WordIndex])-0x21) >= 0)
                    {
                        WordLength += (m_lpFont->m_CharWidth[Buffer[WordIndex]-0x21] + m_lpFont->m_CharacterOffset.x);
                        WordIndex++;
                    }
                    Offset -= m_lpFont->m_CharacterOffset.x;
                    if (WordLength >= float(m_AlignmentRect.x2 - m_AlignmentRect.x1))
                    {
                        OffsetY -= H;
                        Offset = 0;                    
                    }
                }
			}
			else
			{
				W = m_lpFont->m_CharWidth[C];
                
                if (m_bAutoReturn)
                {
                    if ((Offset + m_lpFont->m_CharWidth[C]) >= float(m_AlignmentRect.x2 - m_AlignmentRect.x1))
                    {
                        OffsetY -= H;
                        Offset = 0;
                    }
                }
                
				float OriginX = Offset - OffsetX;

				m_PosArray[NumBatchCharacters].x = 0 + OriginX;
				m_PosArray[NumBatchCharacters].y = OffsetY + H * 0.5f;
				m_PosArray[NumBatchCharacters].z = W;	
				m_PosArray[NumBatchCharacters].w = -H;	

				m_TexCoordArray[NumBatchCharacters].x = m_lpFont->m_CharUV[C][0].u; 
				m_TexCoordArray[NumBatchCharacters].y = m_lpFont->m_CharUV[C][0].v;
				m_TexCoordArray[NumBatchCharacters].z = m_lpFont->m_CharUV[C][3].u - m_lpFont->m_CharUV[C][0].u; 
				m_TexCoordArray[NumBatchCharacters].w = m_lpFont->m_CharUV[C][3].v - m_lpFont->m_CharUV[C][0].v;
				
				Offset += (m_lpFont->m_CharWidth[C] + m_lpFont->m_CharacterOffset.x);					
				NumBatchCharacters++;
			}

			if (NumBatchCharacters >= m_MaxLen)
			{
				lpShader->SetParam((float*)m_PosArray, (float*)m_TexCoordArray, NumBatchCharacters);				
				lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumBatchCharacters*4, 0, NumBatchCharacters*2);		
				NumBatchCharacters = 0;
			}
			i++;
		}		
        if (NumBatchCharacters > 0)
        {
            lpShader->SetParam((float*)m_PosArray, (float*)m_TexCoordArray, NumBatchCharacters);		
            lpR->DrawElementsBO(GraphicsSystem::CRenderer::TRIANGLE_LIST, 0, NumBatchCharacters*4, 0, NumBatchCharacters*2);		
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
	int CFontString::PrintfLength(const LYNXCHAR *fmt, ...)
	{		
		LYNXCHAR	Buffer[512];
		va_list		argptr;		
		CReal		W, H;
		int			C;
		DWORD		Len, NumCharacters;
		
		va_start(argptr,fmt);		
		lynxVsnPrintf(Buffer, 510, fmt, argptr);
		va_end(argptr);			

		NumCharacters = 0;
		Len = lynxStrLen(Buffer);
		CReal Offset = 0.0f;
		H = m_lpFont->m_CharHeight;
		for (int i=0; i<Len; i++)
		{			
			C = Buffer[i]-0x21;
			if (C >= 256) 
				C = -1;			
			if (C < 0)
				Offset += (m_lpFont->m_CharWidth[1] + m_lpFont->m_CharacterOffset.x);
			else
			{
				Offset += (m_lpFont->m_CharWidth[C] + m_lpFont->m_CharacterOffset.x);
				NumCharacters++;
			}
		}	
		return Offset;
	}	
}