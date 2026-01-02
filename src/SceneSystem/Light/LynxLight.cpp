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
//  ����:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxEngine.h>
#include <LynxObjProxy.h>
#include <SceneSystem/LynxScn.h>
#include <SceneSystem/LynxSceneSystem.h>
#include <SceneSystem/Light/LynxLight.h>
#include <ScriptSystem/LynxScript.h>
#include <ScriptSystem/LynxScriptSystem.h>
#include <ScriptSystem/LynxVMachine.h>
#include <GraphicsSystem/LynxGraphicsSystem.h>
#include <GraphicsSystem/LynxRenderer.h>
#include <GraphicsSystem/LynxShadowMap.h>
#include <GraphicsSystem/LynxDepthMap.h>
#include <GraphicsSystem/LynxOffscreen.h>
#include <GraphicsSystem/LynxDepthStencilState.h>
#include <MaterialSystem/LynxMaterial.h>
#include <MaterialSystem/LynxMaterialTechnique.h>
#include <MaterialSystem/LynxMaterialPass.h>
#ifdef __LUA__
	#include <LynxLuaScript.h>
#endif

namespace LynxEngine 
{
	//-------------------------------------------------------------------------------------------------------
	//
	//  ����:   
	//-------------------------------------------------------------------------------------------------------
	CString	GetEnumStr(SceneSystem::CLight::TYPE e)
	{
		CString				Type;
		
		switch (e)
		{
			case SceneSystem::CLight::AMBIENT:
				Type = _T("AMBIENT");
				break;
			case SceneSystem::CLight::DIRECTIONAL:
				Type = _T("DIRECTIONAL");
				break;
			case SceneSystem::CLight::POINT:
				Type = _T("POINT");
				break;
			case SceneSystem::CLight::SPOT:
				Type = _T("SPOT");
				break;
		}
		return Type;
	}
	namespace SceneSystem 
	{	
		IMPLEMENT_CLASSSCRIPT(CLight, CDynamicObj)

		Math::CMatrix4 CLight::m_FaceViewM[6];
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------
		CLight::CLight(CScene* const lpscene)
		: CDynamicObj(lpscene)
		{			
			m_bDynamic = LYNX_TRUE;
			m_bCastShadow = LYNX_TRUE;
			m_DynamicObjType = LIGHT;
			m_Intensity = 1.0f;
			m_Radius = 1.0f;
			m_Color.Red = 0xff;
			m_Color.Green = 0xff;
			m_Color.Blue = 0xff;
			m_lpShadowMap = NULL;
			m_bShadowmapDirty = LYNX_TRUE;
			m_DepthBias = 0;//0.05f;//0.00001f;
			m_ShadowFilterType = GraphicsSystem::CShadowMap::PCF_4X4;
			//m_ShadowFilterType = GraphicsSystem::CShadowMap::VSM_1X1;
			m_ShadowmapWidth = 0;
			m_ShadowmapHeight = 0;
			m_Camera = LYNXNEW CCameraContainer(lpscene->GetlpEngine());

			m_Deg = 30.0f;
			m_FadeOutDeg = 5.0f;

			m_bProjectedLightMap  = LYNX_FALSE;

			m_FaceViewM[0].Set(0.0f, 0.0f, 1.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f,
							  -1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 1.0f);

			m_FaceViewM[1].Set(0.0f, 0.0f, -1.0f, 0.0f,
							   0.0f, 1.0f,  0.0f, 0.0f,
							   1.0f, 0.0f,  0.0f, 0.0f,
							   0.0f, 0.0f,  0.0f, 1.0f);

			m_FaceViewM[2].Set(1.0f,  0.0f, 0.0f, 0.0f,
							   0.0f,  0.0f, 1.0f, 0.0f,
							   0.0f, -1.0f, 0.0f, 0.0f,
							   0.0f,  0.0f, 0.0f, 1.0f);

			m_FaceViewM[3].Set(1.0f, 0.0f,  0.0f, 0.0f,
							   0.0f, 0.0f, -1.0f, 0.0f,
							   0.0f, 1.0f,  0.0f, 0.0f,
							   0.0f, 0.0f,  0.0f, 1.0f);

			m_FaceViewM[4].Set(1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 1.0f);

			m_FaceViewM[5].Set(-1.0f, 0.0f,  0.0f, 0.0f,
							    0.0f, 1.0f,  0.0f, 0.0f,
							    0.0f, 0.0f, -1.0f, 0.0f,
							    0.0f, 0.0f,  0.0f, 1.0f);			

			INIT_CLASSSCRIPT

			IMPLEMENT_PARAM_BOOL_COMMENT(m_bCastShadow, _T("Does this object cast shadow or not"), 0)
			IMPLEMENT_PARAM_FLOAT(m_Intensity, 0)
			IMPLEMENT_PARAM_FLOAT(m_Radius, 0)
			IMPLEMENT_PARAM_URGB(m_Color, 0)
			IMPLEMENT_PARAM_INT(m_ShadowmapWidth, 0)
			IMPLEMENT_PARAM_INT(m_ShadowmapHeight, 0)
			IMPLEMENT_PARAM_FLOAT(m_DepthBias, 0)
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------
		CLight::~CLight(void)
		{
			vRelease();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------
		void CLight::vRelease(void)
		{			
			LYNXDEL m_Camera;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CLight::vCreate(int w, int h)
		{		
			LYNX_ASSERT(m_lpScene);

			if (!CRenderableObj::Create(CContainer::REFERENCE))
				return LYNX_FALSE;
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------
		void CLight::CalculateViewMatrix()
		{
			Math::CMatrix4 M;

			m_Camera->SetLocalMatrix(GetTransformMatrix());
			m_Camera->UpdateViewMatrix();
			//lynxGetViewMatrixFromTransformMatrix(&M, &GetTransformMatrix());
			//m_Camera.SetViewMatrix(M);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------
		void CLight::MaskLightedArea(CCameraContainer* cam)
		{
			if (m_LightType == SPOT || m_LightType == POINT)
			{				
				GraphicsSystem::CGraphicsSystem* lpG = m_lpEngine->GetlpGraphicsSystem();			

				//lpG->SetColorWriteMask(0);				
				
				lpG->GetPostProcessMaterial(GraphicsSystem::LIGHT_PROXY_RENDERING)->GetTechnique((MaterialSystem::CMaterial::TECHTYPE)0).GetPass(0).Set(m_lpContainer, cam);

				LYNXCOLORRGBA DummyColor;
				lpG->GetlpRenderer()->Clear(GraphicsSystem::CRenderer::STENCIL_BUFFER, DummyColor, 1.0f, 0x0);

				CList<CObjProxy*>::CIterator Pos;
				for (Pos = m_lpContainer->GetProxyList().begin(); Pos != m_lpContainer->GetProxyList().end(); ++Pos)
				{
					(*Pos)->vRender(cam);
				}				
				//lpG->SetColorWriteMask(GraphicsSystem::CRenderer::RED_CHANNEL|GraphicsSystem::CRenderer::GREEN_CHANNEL|GraphicsSystem::CRenderer::BLUE_CHANNEL|GraphicsSystem::CRenderer::ALPHA_CHANNEL);				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------
		void CLight::CalculateViewMatrix(int face)
		{
			Math::CMatrix4 M;
			
			lynxMemCpy(&M, &m_FaceViewM[face], sizeof(LYNXMATRIX));		
			M.m[3][0] = -(GetTransformMatrix().m[3][0]*m_FaceViewM[face].m[0][0]+GetTransformMatrix().m[3][1]*m_FaceViewM[face].m[1][0]+GetTransformMatrix().m[3][2]*m_FaceViewM[face].m[2][0]);
			M.m[3][1] = -(GetTransformMatrix().m[3][0]*m_FaceViewM[face].m[0][1]+GetTransformMatrix().m[3][1]*m_FaceViewM[face].m[1][1]+GetTransformMatrix().m[3][2]*m_FaceViewM[face].m[2][1]);
			M.m[3][2] = -(GetTransformMatrix().m[3][0]*m_FaceViewM[face].m[0][2]+GetTransformMatrix().m[3][1]*m_FaceViewM[face].m[1][2]+GetTransformMatrix().m[3][2]*m_FaceViewM[face].m[2][2]);			
			m_Camera->SetViewMatrix(M);
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����: Build a matrix for cropping light's projection given vectors are in light's clip space 
		//-------------------------------------------------------------------------------------------------------		
		CMatrix4 CLight::BuildCropMatrix(const CVector3 &min, const CVector3 &max)
		{
			CReal fScaleX, fScaleY, fScaleZ;
			CReal fOffsetX, fOffsetY, fOffsetZ;

			fScaleX = 2.0f / (max.x - min.x);
			fScaleY = 2.0f / (max.y - min.y);

			fOffsetX = -0.5f * (max.x + min.x) * fScaleX;
			fOffsetY = -0.5f * (max.y + min.y) * fScaleY;

			fScaleZ = 1.0f / (max.z - min.z);
			fOffsetZ = -min.z * fScaleZ;

			// crop volume matrix
			return CMatrix4(fScaleX,	0.0f,		0.0f,		0.0f,
							0.0f,		fScaleY,    0.0f,		0.0f,
							0.0f,		0.0f,		fScaleZ,	0.0f,
							fOffsetX,	fOffsetY,	fOffsetZ,   1.0f );
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����: Helper function for computing AABB in clip space
		//-------------------------------------------------------------------------------------------------------		
		CollisionSystem::CAABB CLight::CreateScreenSpaceAABB(CollisionSystem::CAABB &bb, const CMatrix4& vpm)
		{
			CVector4 Transformed[8];
			CVector3 Transformed3[8];

			// for each point
			for(int i=0;i<8;i++)
			{
				// transform to projection space
				Math::VectorXMatrix(Transformed[i], CVector4(bb.GetV()[i], 1.0f), vpm); 				

				// compute clip-space coordinates
				Transformed3[i].x = Transformed[i].x / Transformed[i].w;
				Transformed3[i].y = Transformed[i].y / Transformed[i].w;
				Transformed3[i].z = Transformed[i].z / Transformed[i].w;
			}
			return CollisionSystem::CAABB(Transformed3, 8);
		}
		////-------------------------------------------------------------------------------------------------------
		////
		////  ����: Crops the light volume on given frustum (scene-independent projection)  
		////-------------------------------------------------------------------------------------------------------		
		CMatrix4 CLight::CalculateCropMatrix(CollisionSystem::CAABB& aabb)
		{
			CollisionSystem::CAABB CropBB;

			// find boundaries in light�s clip space
			CMatrix4 VPM;
			lynxMatrixXMatrix(&VPM, &m_Camera->GetViewMatrix(), &m_Camera->GetProjectionMatrix());

			/*
			CVector3 FrustumP[8];
			for (int i=0; i<4; i++)
			{
				FrustumP[i] = m_Camera.GetFarCornerDir(i)*m_Camera.m_Near;
				FrustumP[i+4] = m_Camera.GetFarCornerDir(i)*m_Camera.m_Far;				
			}
			CollisionSystem::CAABB FrustumBB(FrustumP, 8);
			*/
			CropBB = CreateScreenSpaceAABB(aabb, VPM);

			// use default near plane
			// CropBB.m_Min.z = 0.0f;

			// finally, create matrix
			return BuildCropMatrix(CropBB.Max(), CropBB.Min());
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------
		void CLight::ClearVisList()
		{
			m_VisStaticOpaqueObjList.clear();						
			m_VisDynamicOpaqueObjList.clear();
		}		
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CLight::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);
											
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  ����:   
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CLight::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			RegisterSuperClassVar(lps);

			REGISTER_SCRIPT_VAR(lps, "m_Radius",		LynxEngine::SceneSystem::CLight, m_Radius)
			REGISTER_SCRIPT_VAR(lps, "m_Deg",			LynxEngine::SceneSystem::CLight, m_Deg)

			return LYNX_TRUE;
		}		
	}
}