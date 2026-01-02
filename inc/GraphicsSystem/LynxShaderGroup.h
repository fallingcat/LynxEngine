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

#ifndef __LYNXSHADERGROUP_H__
#define __LYNXSHADERGROUP_H__

#include <LynxResource.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{	
	namespace GraphicsSystem 
	{		
		typedef struct {
			CVertexLayout*					lpVertexLayout;
			CShaderClass*					lpVertexShader;
			CShaderClass*					lpGeometryShader;
			CShaderClass*					lpHullShader;
			CShaderClass*					lpDomainShader;
			CShaderClass*					lpPixelShader;			
		}SHADERGROUPDESC;

		class LYNXENGCLASS CShaderGroup : public CResource
		{	
			LYNX_DECLARE_CLASS(CShaderGroup);
		public:						
		protected:		
			CGraphicsSystem*					m_lpGraphicsSystem;

			CVertexLayoutPtr					m_lpVertexLayout;
			CShaderClassPtr						m_lpVertexShader;
			CShaderClassPtr						m_lpGeometryShader;
			CShaderClassPtr						m_lpHullShader;
			CShaderClassPtr						m_lpDomainShader;
			CShaderClassPtr						m_lpPixelShader;			
		public:			
			CShaderGroup(CGraphicsSystem* lpg);
			virtual ~CShaderGroup();

			LYNXFORCEINLINE void				AddVertexLayout(CVertexLayoutPtr vl) {m_lpVertexLayout = vl; };
			LYNXFORCEINLINE void				AddVertexShader(CShaderClassPtr s) {m_lpVertexShader = s; };
			LYNXFORCEINLINE void				AddGeometryShader(CShaderClassPtr s) {m_lpGeometryShader = s; };
			LYNXFORCEINLINE void				AddHullShader(CShaderClassPtr s) {m_lpHullShader = s; };
			LYNXFORCEINLINE void				AddDomainShader(CShaderClassPtr s) {m_lpDomainShader = s; };
			LYNXFORCEINLINE void				AddPixelShader(CShaderClassPtr s) {m_lpPixelShader = s; };	

			LYNXFORCEINLINE CVertexLayoutPtr	GetVertexLayout()  {return  m_lpVertexLayout; };
			LYNXFORCEINLINE CShaderClassPtr		GetVertexShader() {return  m_lpVertexShader; };
			LYNXFORCEINLINE CShaderClassPtr		GetGeometryShader() {return  m_lpGeometryShader; };
			LYNXFORCEINLINE CShaderClassPtr		GetHullShader() {return  m_lpHullShader; };
			LYNXFORCEINLINE CShaderClassPtr		GetDomainShader() {return  m_lpDomainShader; };
			LYNXFORCEINLINE CShaderClassPtr		GetPixelShader() {return  m_lpPixelShader; };
			
			virtual LYNXBOOL					vCreate();
			virtual void						vSet(CContainer* lpo, CCameraContainer* cam);

			friend class MaterialSystem::CPass;
		};		

		LYNXINLINE bool operator == (CShaderGroup& lhs, CShaderGroup& rhs) 
		{
			return (lhs.GetVertexLayout() == rhs.GetVertexLayout() &&
					lhs.GetVertexShader() == rhs.GetVertexShader() &&
					lhs.GetGeometryShader() == rhs.GetGeometryShader() &&
					lhs.GetHullShader() == rhs.GetHullShader() &&
					lhs.GetDomainShader() == rhs.GetDomainShader() &&
					lhs.GetPixelShader() == rhs.GetPixelShader()); 
		}

		LYNXINLINE bool operator == (CShaderGroupPtr& lhs, CShaderGroupPtr& rhs) 
		{
			return (lhs->GetVertexLayout() == rhs->GetVertexLayout() &&
					lhs->GetVertexShader() == rhs->GetVertexShader() &&
					lhs->GetGeometryShader() == rhs->GetGeometryShader() &&
					lhs->GetHullShader() == rhs->GetHullShader() &&
					lhs->GetDomainShader() == rhs->GetDomainShader() &&
					lhs->GetPixelShader() == rhs->GetPixelShader()); 
		}

		LYNXINLINE bool operator == (CShaderGroupPtr& lhs, SHADERGROUPDESC& rhs) 
		{
			return (lhs->GetVertexLayout() == rhs.lpVertexLayout &&
					lhs->GetVertexShader() == rhs.lpVertexShader &&
					lhs->GetGeometryShader() == rhs.lpGeometryShader &&
					lhs->GetHullShader() == rhs.lpHullShader &&
					lhs->GetDomainShader() == rhs.lpDomainShader &&
					lhs->GetPixelShader() == rhs.lpPixelShader); 
		}	
	}	
}

#endif