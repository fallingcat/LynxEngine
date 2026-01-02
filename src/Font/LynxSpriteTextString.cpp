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
#include <Font/LynxTrueTypeFont.h>
#include <Font/LynxSpriteTextString.h>

#define MAX_CONTEXT_SIZE                1024
#define MAX_STRING_SIZE                 4096

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
	CSpriteTextString::CSpriteTextString(CEngine *lpengine)	
	: CSpriteContainer(lpengine)
	{
		m_AlignmentType = H_ALIGN_CENTER | V_ALIGN_CENTER;
		m_OriginType = OT_CENTER;
        m_bAutoReturn = LYNX_FALSE;
		m_bSpaceChecking = LYNX_TRUE;
		m_Scale = 1.0f;
        m_Context = NULL;
		m_bShadow = LYNX_TRUE;
		m_bOutline = LYNX_FALSE;
		m_OutlineColor.Red = 255;
		m_OutlineColor.Green = 0;
		m_OutlineColor.Blue = 0;
		m_OutlineColor.Alpha = 255;
		m_OutlineWidth = 3;
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
	CSpriteTextString::~CSpriteTextString(void)
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
	void CSpriteTextString::vRelease(void)
	{
        if (m_Context)
        {
            LYNXDEL m_Context;
            m_Context = NULL;
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
    void CSpriteTextString::ReleaseContext(void)
    {
        if (m_Context)
        {
            LYNXDEL m_Context;
            m_Context = NULL;
        }
    }
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param sw, sh :  Sptite size, -1 means it will be the same as the texture size.
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CSpriteTextString::Create(const int sw, const int sh, const int tw, const int th, const int alignment)
	{
        int SizeX, SizeY;
        
		CString OldPath = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath();
		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T("../texture/"));
		#if defined(__WIN32__)
			SizeX = tw;
			SizeY = th;			
		#else
			SizeX = 1;
			while (SizeX < tw)
			{
				SizeX <<= 1;
			}
        
			SizeY = 1;
			while (SizeY < th)
			{
				SizeY <<= 1;
			}
        
			if (SizeX > MAX_CONTEXT_SIZE)
				SizeX = MAX_CONTEXT_SIZE;
         
			if (SizeY > MAX_CONTEXT_SIZE)
				SizeY = MAX_CONTEXT_SIZE;
		#endif

		LYNX_ASSERT(SizeX > 0)
		LYNX_ASSERT(SizeY > 0)
		LYNX_ASSERT(SizeX <= MAX_STRING_SIZE)
		LYNX_ASSERT(SizeY <= MAX_STRING_SIZE)

		LynxEngine::CSpriteClusterCreationDesc SpriteDesc;
		SpriteDesc.Default();
		SpriteDesc.m_TextureDesc.MipMapLevel = 1;
		SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
		SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
		SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
		SpriteDesc.m_bUseBufferObject = LYNX_TRUE;
		SpriteDesc.m_bApplyUIFactor = LYNX_FALSE;
		SpriteDesc.m_Number = 0;
		SpriteDesc.m_UVOffset[0].x = 0.0f;
		SpriteDesc.m_UVOffset[0].y = 0.0f;
        if (sw < 0 || sh <0)
        {
            SpriteDesc.m_Width = SizeX;
            SpriteDesc.m_Height = SizeY;
        }
        else
        {
            SpriteDesc.m_Width = sw;
            SpriteDesc.m_Height = sh;
        }
		SpriteDesc.m_UVOffset[1].x = (float)(SpriteDesc.m_Width) / (float)SizeX;
		SpriteDesc.m_UVOffset[1].y = (float)(SpriteDesc.m_Height) / (float)SizeY;
		#if defined __iOS__ || defined __ANDROID__
			SpriteDesc.m_bInvertTexCoordV = LYNX_TRUE;
		#else
			SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
		#endif        		
		LYNXGUID Name;
		lynxOSMakeGUID(&Name);
        SpriteDesc.m_FileName = _T("white.png");
		SpriteDesc.m_Name = lynxOSGUIDToString(&Name);
		SpriteDesc.m_MaterialName = SpriteDesc.m_Name + CString(".SpriteText.Material");
		vCreate(&SpriteDesc);
		SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
		SetOriginType(CSpriteContainer::OT_CENTER);
		
		m_Context = LYNXNEW CPlatformBitmapContext(m_lpEngine->GetlpGraphicsSystem());
		m_Context->vCreate(SizeX, SizeY, alignment);
		GetlpCurrentMaterial()->GetTechnique(0).GetPass(0).GetTextureAnimation(0)->SetTexture(0, m_Context->GetTexture());

        m_OriginalSize.x = m_Width;
        m_OriginalSize.y = m_Height;
        m_PrintSize.x = tw;
        m_PrintSize.y = th;
		m_TextAlignment = alignment;

		m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(OldPath);
        
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
	void CSpriteTextString::SetFont(CFontPtr& lpfont)
	{
		if (m_Context)
			m_Context->SetFont(static_cast<CPlatformTrueTypeFont*>((CFont*)lpfont));
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
	void CSpriteTextString::SetOriginalSize(const float w, const float h)
    {
        vSetSize(w, h);
        
        m_OriginalSize.x = w;
        m_OriginalSize.y = h;
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
	const float CSpriteTextString::SetScale(const float s)
    {
        float Old = m_Scale;
        m_Scale = s;
        vSetSize(m_OriginalSize.x*s, m_OriginalSize.y*s);
        return Old;
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
	int CSpriteTextString::Printf(int x, int y, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{	
		LYNXCHAR	Buffer[MAX_STRING_SIZE];
		va_list		argptr;		
	    LYNXRECT	OldRect, Rect;
        
        va_start(argptr,fmt);
		lynxVsnPrintf(Buffer, MAX_STRING_SIZE-8, fmt, argptr);
		va_end(argptr);

		if (m_Context)
			m_Context->vPrint(Buffer, x, y, lpc, m_bShadow, m_bOutline, &m_OutlineColor, m_OutlineWidth);

		//vRender();

		return 0;
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
	Math::CVector2 CSpriteTextString::Printf(const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{		
		LYNXCHAR	Buffer[MAX_STRING_SIZE];
		va_list		argptr;		
		Math::CVector2 Size;
		
		va_start(argptr,fmt);		
		lynxVsnPrintf(Buffer, MAX_STRING_SIZE-8, fmt, argptr);
		va_end(argptr);
        
        LYNXRECT Rect;
		
		if (m_TextAlignment & CSpriteContainer::H_ALIGN_LEFT)
		{
			Rect.x1 = 0; 
			Rect.x2 = m_PrintSize.x;
			Rect.y1 = 0;//(m_OriginalSize.y - m_PrintSize.y) * 0.5f;
			Rect.y2 = Rect.y1 + m_PrintSize.y;
		}
		else if (m_TextAlignment & CSpriteContainer::H_ALIGN_CENTER)
		{
			Rect.x1 = (m_OriginalSize.x - m_PrintSize.x) * 0.5f;
			Rect.x2 = Rect.x1 + m_PrintSize.x;
			Rect.y1 = (m_OriginalSize.y - m_PrintSize.y) * 0.5f;
			Rect.y2 = Rect.y1 + m_PrintSize.y;
		}
		else
		{
			Rect.x2 = m_OriginalSize.x;
			Rect.x1 = Rect.x2 - m_PrintSize.x; 			
            Rect.y1 = 0;//(m_OriginalSize.y - m_PrintSize.y) * 0.5f;
			Rect.y2 = Rect.y1 + m_PrintSize.y;
		}        
		
		if (m_Context)
			m_Context->vPrint(Buffer, Rect, lpc, m_bShadow, m_bOutline, &m_OutlineColor, m_OutlineWidth);

		//vRender();

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
	Math::CVector2 CSpriteTextString::Printf(const LYNXRECT& rect, const LYNXCOLORRGBA* lpc, const LYNXCHAR *fmt, ...)
	{		
		LYNXCHAR	Buffer[MAX_STRING_SIZE];
		va_list		argptr;		
		Math::CVector2 Size;
		
		va_start(argptr,fmt);		
		lynxVsnPrintf(Buffer, MAX_STRING_SIZE-8, fmt, argptr);
		va_end(argptr);
        
        LYNXRECT Rect = rect;
        
        Rect.x2 -= Rect.x1;
        Rect.x1 = 0;
        Rect.y2 -= Rect.y1;
        Rect.y1 = 0;
        if (m_Context)
			m_Context->vPrint(Buffer, Rect, lpc, m_bShadow, m_bOutline, &m_OutlineColor, m_OutlineWidth);

		//vRender();

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
	int CSpriteTextString::PrintfLength(const LYNXCHAR *fmt, ...)
	{				
		return 0;
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
	Math::CVector2 CSpriteTextString::PrintfSize(const LYNXCHAR *fmt, ...)
	{		
		Math::CVector2 Size;
		
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
	void CSpriteTextString::vRender(CCameraContainer* cam)
	{
		#ifdef __ANDROID__
			m_Context->UpdateTextureIfNecessary();
		#endif

		CSpriteContainer::vRender(cam);
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
	void CSpriteTextString::SetTextAlignment(const DWORD a) 
	{
		m_TextAlignment = a; 
		if (m_Context)
		{
			m_Context->vSetTextAlignment(m_TextAlignment);
		}
	}
}