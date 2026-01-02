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
#include <GameFramework/LynxGameFrameworkKernel.h>
#include <Container/LynxCameraContainer.h>
#include <GraphicsSystem/LynxVertexLayout.h>
#include <CollisionSystem/LynxAABB.h>

namespace LynxEngine 
{		
	IMPLEMENT_CLASSSCRIPT(CCameraContainer, CContainer)
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------
	CCameraContainer::CCameraContainer(CEngine *lpengine)	
	:CContainer(lpengine)
	{
		m_ContainerType = CAMERA;		
		m_Exposure = 1.0f;
		m_ExposureSpeed = 0.05f;		
			
		m_bMotionBlur = LYNX_FALSE;
		m_MotionBlurKernel = 5;
		m_MotionBlurScale = 1.0f;
		m_bCameraMovingBlur = LYNX_FALSE;
		m_CameraMovingBlurKernel = 5;
		m_CameraMovingBlurScale = 1.0f;
		m_BrightPassThreshold = 0.9f;

		m_bDOF = LYNX_FALSE;		
		m_DOFBlurKernel = 5;
		m_DOFBlurScale = 1.0f;
		m_DOFSamplingKernel = 5.0f;

		m_bBlooming = LYNX_TRUE;		
		m_BloomingBlurKernel = 5;
		m_BloomingBlurScale = 2.0f;	

		m_FocalRange[0] = 10.0f;
		m_FocalRange[1] = 20.0f;
		m_FocalRange[2] = 800.0f;
		m_FocalRange[3] = 1300.0f;	

		m_ViewportRect.x1 = 0;
		m_ViewportRect.y1 = 0;
		m_ViewportRect.x2 = 0;
		m_ViewportRect.y2 = 0;

		m_SplitSchemeWeight = 0.5f;

		SetFOV(LYNX_REAL(60),  LYNX_REAL(60));	
		m_Near = LYNX_REAL(1.0f);
		m_Far = LYNX_REAL(500.0f);
		m_FrustumFlag = LYNX_TEST_NEAR | LYNX_TEST_FAR | LYNX_TEST_LEFT | LYNX_TEST_RIGHT | LYNX_TEST_TOP | LYNX_TEST_BOTTOM;
		CalculateFrustum();
		LYNX_VECTOR3D(m_UpDir, LYNX_REAL(0), LYNX_REAL(1), LYNX_REAL(0));				

		m_bMirror = LYNX_FALSE;		

		IMPLEMENT_PARAM_FLOAT(m_Exposure, 0)
		IMPLEMENT_PARAM_FLOAT(m_ExposureSpeed, 0)
		IMPLEMENT_PARAM_BOOL(m_bBlooming, 0)	
		IMPLEMENT_PARAM_BOOL(m_bDOF, 0)			
		IMPLEMENT_PARAM_BOOL(m_bMotionBlur, 0)
		IMPLEMENT_PARAM_INT(m_MotionBlurKernel, 0)
		IMPLEMENT_PARAM_FLOAT(m_MotionBlurScale, 0)
		IMPLEMENT_PARAM_BOOL(m_bCameraMovingBlur, 0)
		IMPLEMENT_PARAM_INT(m_CameraMovingBlurKernel, 0)
		IMPLEMENT_PARAM_FLOAT(m_CameraMovingBlurScale, 0)
		IMPLEMENT_PARAM_INT(m_DOFBlurKernel, 0)
		IMPLEMENT_PARAM_FLOAT(m_DOFBlurScale, 0)
		IMPLEMENT_PARAM_FLOAT(m_DOFSamplingKernel, 0)
		IMPLEMENT_PARAM_INT(m_BloomingBlurKernel, 0)
		IMPLEMENT_PARAM_FLOAT(m_BloomingBlurScale, 0)

		IMPLEMENT_PARAM_FLOAT(m_Near, 0)
		IMPLEMENT_PARAM_FLOAT(m_Far, 0)

		IMPLEMENT_PARAM_FLOAT(m_SplitSchemeWeight, 0)

		IMPLEMENT_PARAM_RANGE_FLOAT(m_BrightPassThreshold, 0.0f, 1.0f, 200.0f, 0)
		IMPLEMENT_PARAM_FLOAT(m_XFOV, 0)
		IMPLEMENT_PARAM_FLOAT(m_YFOV, 0)		

		IMPLEMENT_PARAM_NATIVE_ARRAY(m_FocalRange, CParam::FLOAT, 4, CParam::PARAM_SIZE_READONLY)		

		INIT_CLASSSCRIPT
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
	CCameraContainer::~CCameraContainer(void)
	{		
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
	void CCameraContainer::SetViewportRect() 
	{
		if (m_ViewportRect.x2 <= 0 || m_ViewportRect.y2 <= 0)        
        {
            m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(0, 0, m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth(), m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight());
			UpdateProjectionMatrix(((float)m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth())/((float)m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight()));
        }
        else
        {
            m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetViewportRect(m_ViewportRect.x1, m_ViewportRect.y1, m_ViewportRect.x2, m_ViewportRect.y2);
			UpdateProjectionMatrix(-1.0f);
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
	void CCameraContainer::CalculateFarCornerDir(Math::CReal ratio)
	{	
		Math::CReal		a, d, w, h, f;

		a = m_XFOV * 0.5f;
		w = lynxSin(a);
		d = lynxCos(a);
		h = w / ratio;

		m_FarCornerDir[0].x = -w;
		m_FarCornerDir[0].y = h;
		m_FarCornerDir[0].z = d;
		lynxNormalise(&m_FarCornerDir[0], 3);
		f = 1.0f/m_FarCornerDir[0].z;
		m_FarCornerDir[0] *= f;

		m_FarCornerDir[1].x = w;
		m_FarCornerDir[1].y = h;
		m_FarCornerDir[1].z = d;
		lynxNormalise(&m_FarCornerDir[1], 3);
		f = 1.0f/m_FarCornerDir[1].z;
		m_FarCornerDir[1] *= f;

		m_FarCornerDir[2].x = -w;
		m_FarCornerDir[2].y = -h;
		m_FarCornerDir[2].z = d;
		lynxNormalise(&m_FarCornerDir[2], 3);
		f = 1.0f/m_FarCornerDir[2].z;
		m_FarCornerDir[2] *= f;

		m_FarCornerDir[3].x = w;
		m_FarCornerDir[3].y = -h;
		m_FarCornerDir[3].z = d;
		lynxNormalise(&m_FarCornerDir[3], 3);
		f = 1.0f/m_FarCornerDir[3].z;
		m_FarCornerDir[3] *= f;
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
	void CCameraContainer::CalculateFrustum()
	{	
		LYNXREAL					d;

		d = LYNX_REAL_SUB(LYNX_REAL(90), LYNX_REAL_MUL(m_XFOV, LYNX_REAL_HALF));
		m_Sin_XFOV[0] = lynxQuickSin(d);
		m_Cos_XFOV[0] = lynxQuickCos(d);
		d = -LYNX_REAL_SUB(LYNX_REAL(90), LYNX_REAL_MUL(m_XFOV, LYNX_REAL_HALF));	
		m_Sin_XFOV[1] = lynxQuickSin(d);
		m_Cos_XFOV[1] = lynxQuickCos(d);
		
		d = LYNX_REAL_SUB(LYNX_REAL(90), LYNX_REAL_MUL(m_YFOV, LYNX_REAL_HALF));
		m_Sin_YFOV[0] = lynxQuickSin(d);
		m_Cos_YFOV[0] = lynxQuickCos(d);
		d = -LYNX_REAL_SUB(LYNX_REAL(90), LYNX_REAL_MUL(m_YFOV, LYNX_REAL_HALF));
		m_Sin_YFOV[1] = lynxQuickSin(d);
		m_Cos_YFOV[1] = lynxQuickCos(d);

		//CalculateFarPlane
		if (m_FrustumFlag & LYNX_TEST_FAR)
		{
			m_Frustum.Far.Normal.x = -m_lpDirection->x;
			m_Frustum.Far.Normal.y = -m_lpDirection->y;
			m_Frustum.Far.Normal.z = -m_lpDirection->z;

			m_Frustum.FarP.x = m_lpCenter->x + LYNX_REAL_MUL(m_lpDirection->x, m_Far);
			m_Frustum.FarP.y = m_lpCenter->y + LYNX_REAL_MUL(m_lpDirection->y, m_Far);
			m_Frustum.FarP.z = m_lpCenter->z + LYNX_REAL_MUL(m_lpDirection->z, m_Far);

			//Frustum.Far.Distance = lynxLength(&D, 3);
			m_Frustum.Far.Distance = LYNX_DOT_PRODUCT(m_Frustum.Far.Normal, m_Frustum.FarP);
		}

		//CalculateNearPlane
		if (m_FrustumFlag & LYNX_TEST_NEAR)
		{
			m_Frustum.Near.Normal.x = m_lpDirection->x;
			m_Frustum.Near.Normal.y = m_lpDirection->y;
			m_Frustum.Near.Normal.z = m_lpDirection->z;

			m_Frustum.NearP.x = m_lpCenter->x + LYNX_REAL_MUL(m_lpDirection->x, m_Near);
			m_Frustum.NearP.y = m_lpCenter->y + LYNX_REAL_MUL(m_lpDirection->y, m_Near);
			m_Frustum.NearP.z = m_lpCenter->z + LYNX_REAL_MUL(m_lpDirection->z, m_Near);

			//Frustum.Near.Distance = lynxLength(&D, 3);
			m_Frustum.Near.Distance = LYNX_DOT_PRODUCT(m_Frustum.Near.Normal, m_Frustum.NearP);
		}	
		
		//CalculateLeftPlane
		if (m_FrustumFlag & LYNX_TEST_LEFT)
		{		
			m_Frustum.Left.Normal.x = LYNX_REAL_MUL(m_lpDirection->x, m_Cos_XFOV[0]) + LYNX_REAL_MUL(m_lpDirection->z, m_Sin_XFOV[0]); 
			m_Frustum.Left.Normal.z = LYNX_REAL_MUL(-m_lpDirection->x, m_Sin_XFOV[0]) + LYNX_REAL_MUL(m_lpDirection->z, m_Cos_XFOV[0]);  
			m_Frustum.Left.Normal.y = m_lpDirection->y;
			m_Frustum.Left.Distance = LYNX_DOT_PRODUCT(m_Frustum.Left.Normal, (*m_lpCenter));
		}

		//CalculateRightPlane
		if (m_FrustumFlag & LYNX_TEST_RIGHT)
		{	
			m_Frustum.Right.Normal.x = LYNX_REAL_MUL(m_lpDirection->x, m_Cos_XFOV[1]) + LYNX_REAL_MUL(m_lpDirection->z, m_Sin_XFOV[1]); 
			m_Frustum.Right.Normal.z = LYNX_REAL_MUL(-m_lpDirection->x, m_Sin_XFOV[1]) + LYNX_REAL_MUL(m_lpDirection->z, m_Cos_XFOV[1]);  
			m_Frustum.Right.Normal.y = m_lpDirection->y;
			m_Frustum.Right.Distance = LYNX_DOT_PRODUCT(m_Frustum.Right.Normal, (*m_lpCenter));
		}

		//CalculateTopPlane
		if (m_FrustumFlag & LYNX_TEST_TOP)
		{	
			m_Frustum.Top.Normal.y = LYNX_REAL_MUL(m_lpDirection->y, m_Cos_YFOV[0]) - LYNX_REAL_MUL(m_lpDirection->z, m_Sin_YFOV[0]); 
			m_Frustum.Top.Normal.z = LYNX_REAL_MUL(m_lpDirection->y, m_Sin_YFOV[0]) + LYNX_REAL_MUL(m_lpDirection->z, m_Cos_YFOV[0]);  
			m_Frustum.Top.Normal.x = m_lpDirection->x;
			m_Frustum.Top.Distance = LYNX_DOT_PRODUCT(m_Frustum.Top.Normal, (*m_lpCenter));
		}

		//CalculateBottomPlane
		if (m_FrustumFlag & LYNX_TEST_BOTTOM)
		{		
			m_Frustum.Bottom.Normal.y = LYNX_REAL_MUL(m_lpDirection->y, m_Cos_YFOV[1]) - LYNX_REAL_MUL(m_lpDirection->z, m_Sin_YFOV[1]); 
			m_Frustum.Bottom.Normal.z = LYNX_REAL_MUL(m_lpDirection->y, m_Sin_YFOV[1]) + LYNX_REAL_MUL(m_lpDirection->z, m_Cos_YFOV[1]);  
			m_Frustum.Bottom.Normal.x = m_lpDirection->x;
			m_Frustum.Bottom.Distance = LYNX_DOT_PRODUCT(m_Frustum.Bottom.Normal, (*m_lpCenter));
		}	
	}	
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*		computes a frustum with given far and near planes
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------		
	void CCameraContainer::CalculateFrustumAABB(CollisionSystem::CAABB& aabb, const Math::CReal& nearp, const Math::CReal& farp)
	{
		Math::CVector3 FrustumP[8], Pos;
		for (int i=0; i<4; i++)
		{
			PointXMatrix(FrustumP[i], GetFarCornerDir(i)*nearp, m_M);
			PointXMatrix(FrustumP[i+4], GetFarCornerDir(i)*farp, m_M);			
		}
		aabb.Set(FrustumP, 8);
		
		return;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief 。
	*		calculates split plane distances in view space
	*
	*	@param 。
	*	@param 。  
	*	@return 。 
	*/
	//-------------------------------------------------------------------------------------------------------	
	void CCameraContainer::CalculateSplitPositions(CArray<Math::CReal>& pdists, int num)
	{
		// Practical split scheme:
		//
		// CLi = n*(f/n)^(i/numsplits)
		// CUi = n + (f-n)*(i/numsplits)
		// Ci = CLi*(lambda) + CUi*(1-lambda)
		//
		// lambda scales between logarithmic and uniform
		//	 
		for (int i = 0; i < num; i++)
		{
			float fIDM = i / (float)num;
			float fLog = m_Near * powf(m_Far/m_Near, fIDM);
			float fUniform = m_Near + (m_Far - m_Near) * fIDM;
			pdists[i] = fLog * m_SplitSchemeWeight + fUniform * (1 - m_SplitSchemeWeight);
		}
		// make sure border values are accurate
		pdists[0] = m_Near;
		pdists[num] = m_Far;
	}
	//-------------------------------------------------------------------------------------------------------
	/**
	*	@brief ?C
	*
	*	@param ?C
	*	@param ?C  
	*	@return ?C 
	*/
	//-------------------------------------------------------------------------------------------------------
	LYNXBOOL CCameraContainer::vPlay(CReal step)
	{
		LYNXBOOL bStop = LYNX_FALSE;
		static Math::CReal Values[4];
		CMatrix4 TM;

		CContainer::vPlay(step);			
			
		SaveTransformationMatrix();
				
		if (!m_lpParentContainer && m_AnimationSet->GetCurrentAnimation())
		{
			bStop = m_AnimationSet->GetCurrentAnimation()->Play(step);		
			m_AnimationSet->GetCurrentAnimation()->UpdateTransform();
			PushTransform();
			TM = m_M;
			lynxMatrixXMatrix(&m_M, &m_AnimationSet->GetCurrentAnimation()->m_M, &TM);
			//m_M = m_AnimationSet->GetCurrentAnimation()->m_M;	
			
			static float Temp[3];

			Temp[0] = m_M.m21;
			Temp[1] = m_M.m22;
			Temp[2] = m_M.m23;

			m_M.m21 = m_M.m31;
			m_M.m22 = m_M.m32;
			m_M.m23 = m_M.m33;

			m_M.m31 = -Temp[0];
			m_M.m32 = -Temp[1];
			m_M.m33 = -Temp[2];
			
			LYNXREAL			m41, m42, m43;

			m41 = -LYNX_DOT_PRODUCT((*m_lpRightDir),	(*m_lpCenter));
			m42 = -LYNX_DOT_PRODUCT((*m_lpUpDir),		(*m_lpCenter));
			m43 = -LYNX_DOT_PRODUCT((*m_lpDirection),	(*m_lpCenter));		

			LYNX_MATRIX(m_ViewMatrix,
		  				m_lpRightDir->x, m_lpUpDir->x, m_lpDirection->x, LYNX_REAL(0),
						m_lpRightDir->y, m_lpUpDir->y, m_lpDirection->y, LYNX_REAL(0),
						m_lpRightDir->z, m_lpUpDir->z, m_lpDirection->z, LYNX_REAL(0),
						m41,			 m42,		   m43,			     LYNX_REAL(1));							
			PopTransform();
		}

		return bStop;
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
	void CCameraContainer::UpdateViewMatrix()
	{
		LYNXREAL			m41, m42, m43;

		UpdateTransform();

		m41 = -LYNX_DOT_PRODUCT((*m_lpRightDir), (*m_lpCenter));
		m42 = -LYNX_DOT_PRODUCT((*m_lpUpDir), (*m_lpCenter));
		m43 = -LYNX_DOT_PRODUCT((*m_lpDirection), (*m_lpCenter));		

		LYNX_MATRIX(m_ViewMatrix,
		  			m_lpRightDir->x, m_lpUpDir->x, m_lpDirection->x, LYNX_REAL(0),
					m_lpRightDir->y, m_lpUpDir->y, m_lpDirection->y, LYNX_REAL(0),
					m_lpRightDir->z, m_lpUpDir->z, m_lpDirection->z, LYNX_REAL(0),
					m41,			 m42,		   m43,			     LYNX_REAL(1));			
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
	void CCameraContainer::LookAt(LPLYNXVECTOR3D const lpv)
	{
		LYNXVECTOR3D			dv,dn;	
		LYNXREAL				f;
		LYNXREAL				m41, m42, m43;

		LYNX_VECTOR3D_COPY((m_LookPoint), (*lpv));	
		LYNX_VECTOR3D_SUB((*m_lpDirection), (*lpv), (*m_lpCenter));    
		lynxNormalise(m_lpDirection, 3);

		LYNX_VECTOR3D_ADD((dv), (*m_lpDirection), m_UpDir);        				
		lynxNormalise(&dv, 3);

		f = LYNX_DOT_PRODUCT(dv, (*m_lpDirection));
		if (f == 0.0f)
		{
			f = 0.1f;
		}
		LYNX_VECTOR3D_MUL(dn, f, (*m_lpDirection));	
		LYNX_VECTOR3D_SUB((*m_lpUpDir), dv, dn);	
		lynxNormalise(m_lpUpDir, 3);	
		lynxCrossProduct(m_lpRightDir, m_lpUpDir, m_lpDirection);		
		
		m41 = -LYNX_DOT_PRODUCT((*m_lpRightDir), (*m_lpCenter));
		m42 = -LYNX_DOT_PRODUCT((*m_lpUpDir), (*m_lpCenter));
		m43 = -LYNX_DOT_PRODUCT((*m_lpDirection), (*m_lpCenter));	

		LYNX_MATRIX(m_ViewMatrix,
					m_lpRightDir->x , m_lpUpDir->x , m_lpDirection->x , LYNX_REAL(0),
					m_lpRightDir->y , m_lpUpDir->y , m_lpDirection->y , LYNX_REAL(0),
					m_lpRightDir->z , m_lpUpDir->z , m_lpDirection->z , LYNX_REAL(0),
					m41				, m42			   , m43		  , LYNX_REAL(1));					

		m_LocalM = m_M;
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
    void CCameraContainer::UpdateProjectionMatrix(float ratio)
    {
        float Ratio;
        
		if (ratio < 0.0f)
		{
			if (m_ViewportRect.x2 <= 0 || m_ViewportRect.y2 <= 0)
			{
				ratio = (float)(m_lpEngine->GetlpGraphicsSystem()->GetBackbufferWidth()) / (float)(m_lpEngine->GetlpGraphicsSystem()->GetBackbufferHeight());
			}
			else
			{
				ratio = (float)(m_ViewportRect.x2 - m_ViewportRect.x1) / (float)(m_ViewportRect.y2 - m_ViewportRect.y1);
			}
		}

        if (m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->IsPortraitMode())
            Ratio = 1.0f / ratio;
        else
            Ratio = ratio;
        lynxPerspectiveProjectionMatrix(&m_ProjectionMatrix, m_Near, m_Far, m_XFOV, Ratio);
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
    void CCameraContainer::UpdateOrthoProjectionMatrix()
    {
        float W, H;
        
		if (m_ViewportRect.x2 <= 0 || m_ViewportRect.y2 <= 0)
		{
			W = (float)(m_lpEngine->GetlpGraphicsSystem()->GetBackbufferWidth());
			H = (float)(m_lpEngine->GetlpGraphicsSystem()->GetBackbufferHeight());
		}
		else
		{
			W = (float)(m_ViewportRect.x2 - m_ViewportRect.x1);
			H = (float)(m_ViewportRect.y2 - m_ViewportRect.y1);
		}		
        lynxOrthoProjectionMatrix(&m_ProjectionMatrix, W, H, m_Near, m_Far);
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
	LYNXBOOL CCameraContainer::vCreate(const CCreationDesc* desc)
	{
		return LYNX_TRUE;
	}					
	//-------------------------------------------------------------------------------------------------------
	//
	//  說明:   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CCameraContainer::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
	{		
		RegisterSuperClassMethod(lps);

		REGISTER_SCRIPT_METHOD(lps, "LookAt",					LynxEngine::CCameraContainer,	void,	_LookAt,				(float, float, float))		
		REGISTER_SCRIPT_METHOD(lps, "UpdateViewMatrix",			LynxEngine::CCameraContainer,	void,	UpdateViewMatrix,		(void))					
		REGISTER_SCRIPT_METHOD(lps, "UpdateProjectionMatrix",	LynxEngine::CCameraContainer,	void,	UpdateProjectionMatrix,	(float))				
		
		return LYNX_TRUE;
	}
}