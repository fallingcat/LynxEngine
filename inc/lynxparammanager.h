//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXPARAMMANAGER_H__
#define __LYNXPARAMMANAGER_H__

#include <LynxEngineClasses.h>
#include <LynxString.h>
#include <LynxList.h>
#include <LynxBoolParam.h>
#include <LynxRealParam.h>
#include <LynxFloatParam.h>
#include <LynxIntParam.h>
#include <LynxURGBParam.h>
#include <LynxURGBAParam.h>
#include <LynxVector2dParam.h>
#include <LynxVector3dParam.h>
#include <LynxStringParam.h>
#include <LynxEnumParam.h>
#include <LynxArrayParam.h>
#include <LynxListParam.h>
#include <LynxNativeArrayParam.h>
#include <LynxObjPointerParam.h>
#include <LynxShaderClassPtrParam.h>

namespace LynxEngine 
{
	class LYNXENGCLASS CParamManager
	{	
	public:		
	protected:				
		CList<CParamGroup*>			m_ParamGroupList;	
	private:
		CParamManager&	operator =(const CParamManager& rhs) {return *this; };	
	public:	
		CParamManager();
		virtual ~CParamManager();			

		LYNXINLINE CList<CParamGroup*>&	GetParamGroupList() {return m_ParamGroupList; };
		void						Add(CParam* p);		
	};

	#ifdef __TOOL__
		// ------------------------------------------------------------------
		#define IMPLEMENT_PARAM(n, b) \
			n##_Param.Setup(this->GetClassName(), NULL, (#n), (void*)&n, b);\
			m_ParamManager.Add(&(n##_Param));

		#define IMPLEMENT_PARAM2(p, n, b) \
		    p->Setup(this->GetClassName(), NULL, (#n), (void*)&n, b);\
			m_ParamManager.Add(p);

		#define IMPLEMENT_PARAM_COMMENT(n, b, c) \
			n##_Param.Setup(this->GetClassName(), NULL, (#n), (void*)&n, b, c);\
			m_ParamManager.Add(&(n##_Param));

		#define IMPLEMENT_PARAM_COMMENT2(p, n, b, c) \
			p->Setup(this->GetClassName(), NULL, (#n), (void*)&n, b, c);\
			m_ParamManager.Add(p);

		#define SET_PARAM_FLAG(n, f)\
			n##_Param.SetFlag(f);

		#define SET_PARAM_FLAG2(n, f)\
			n##_pParam->SetFlag(f);
		// ------------------------------------------------------------------
		#define DECLAR_PARAM_MANAGER \
			LynxEngine::CParamManager m_ParamManager;

		#define DECLAR_PARAM_BOOL(n) \
			LYNXBOOL				n;\
			LynxEngine::CBoolParam	n##_Param;
		#define IMPLEMENT_PARAM_BOOL(n, b) \
			IMPLEMENT_PARAM(n, b)
			//LynxEngine::CParam* n##_pParam = new LynxEngine::CBoolParam;\
			//IMPLEMENT_PARAM2(n##_pParam, n, b)
		#define IMPLEMENT_PARAM_BOOL_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)		
			//LynxEngine::CParam* n##_pParam = new LynxEngine::CBoolParam;\
			//IMPLEMENT_PARAM_COMMENT2(n##_pParam, n, b, c) 
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_REAL(n) \
			LynxEngine::Math::CReal	n;\
			LynxEngine::CRealParam	n##_Param;
		#define IMPLEMENT_PARAM_REAL(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_REAL_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)		
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_FLOAT(n) \
			float					n;\
			LynxEngine::CFloatParam	n##_Param;
		#define IMPLEMENT_PARAM_FLOAT(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_FLOAT_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)		
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_RANGE_FLOAT(n) \
			float							n;\
			LynxEngine::CRangeFloatParam	n##_Param;
		//-------------------------------------------------------------------------------------------------------
		// a0 : min, a1 : max, a2 : steps
		//-------------------------------------------------------------------------------------------------------
		#define IMPLEMENT_PARAM_RANGE_FLOAT(n, a0, a1, a2, b) \
			IMPLEMENT_PARAM(n, b);\
			n##_Param.SetupEx(a0, a1, a2);
		#define IMPLEMENT_PARAM_RANGE_FLOAT_COMMENT(n, a0, a1, a2, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c);\
			n##_Param.SetupEx(a0, a1, a2);
		// ------------------------------------------------------------------
	
		#define DECLAR_PARAM_INT(n) \
			int						n;\
			LynxEngine::CIntParam	n##_Param;
		#define IMPLEMENT_PARAM_INT(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_INT_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------
		
		#define DECLAR_PARAM_RANGE_INT(n) \
			int							n;\
			LynxEngine::CRangeIntParam	n##_Param;
		#define IMPLEMENT_PARAM_RANGE_INT(n, a0, a1, b) \
			IMPLEMENT_PARAM(n, b);\
			n##_Param.SetupEx(a0, a1);
		#define IMPLEMENT_PARAM_RANGE_INT_COMMENT(n, a0, a1, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c);\
			n##_Param.SetupEx(a0, a1);
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_DWORD(n) \
			LYNXDWORD				n;\
			LynxEngine::CDwordParam	n##_Param;
		#define IMPLEMENT_PARAM_DWORD(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_DWORD_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_URGB(n) \
			LYNXCOLORRGB			n;\
			LynxEngine::CURGBParam	n##_Param;
		#define IMPLEMENT_PARAM_URGB(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_URGB_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_URGBA(n) \
			LYNXCOLORRGBA			n;\
			LynxEngine::CURGBAParam	n##_Param;
		#define IMPLEMENT_PARAM_URGBA(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_URGBA_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_VECTOR2D(n) \
			LynxEngine::Math::CVector2	n;\
			LynxEngine::CVector2DParam	n##_Param;
		#define IMPLEMENT_PARAM_VECTOR2D(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_VECTOR2D_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_VECTOR3D(n) \
			LynxEngine::Math::CVector3	n;\
			LynxEngine::CVector3DParam	n##_Param;
		#define IMPLEMENT_PARAM_VECTOR3D(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_VECTOR3D_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_MATRIX4(n) \
			LynxEngine::Math::CMatrix4	n;\
			LynxEngine::CMatrix4Param	n##_Param;
		#define IMPLEMENT_PARAM_MATRIX4(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_MATRIX4_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------
		
		//#define DECLAR_PARAM_ENUM(t, n) \
		//	t							n;\
		//	LynxEngine::CEnumParam<t>	n##_Param;\
		//	void Implement_##n(LYNXBOOL b) {IMPLEMENT_PARAM(n, b); n##_Param.SetupEx(LynxEngine::NUM_##t, NULL); };\
		//	void Implement_##n(LYNXBOOL b, const LynxEngine::CString& c) {IMPLEMENT_PARAM_COMMENT(n, b, c); n##_Param.SetupEx(LynxEngine::NUM_##t, NULL); }

		#define DECLAR_PARAM_ENUM(p, t, n) \
			p::t						n;\
			LynxEngine::CEnumParam<p::t>	n##_Param;\
			void Implement_##n(LYNXBOOL b) {IMPLEMENT_PARAM(n, b); n##_Param.SetupEx(p::NUM_##t, NULL); };\
			void Implement_##n(LYNXBOOL b, const LynxEngine::CString& c) {IMPLEMENT_PARAM_COMMENT(n, b, c); n##_Param.SetupEx(p::NUM_##t, NULL); }

		#define IMPLEMENT_PARAM_ENUM(n, b)						Implement_##n(b);
		#define IMPLEMENT_PARAM_ENUM_COMMENT(n, c, b)			Implement_##n(b, c);		
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_LIST_ENUM(t, n) \
			t								n;\
			LynxEngine::CListEnumParam<t>	n##_Param;
		#define IMPLEMENT_PARAM_LIST_ENUM(n, b) \
			IMPLEMENT_PARAM(n, b)
			//n##_Param.SetupEx(LYNX_ARRAY_SIZE(LynxEngine::g##t##_EnumData), LynxEngine::g##t##_EnumData)
		#define IMPLEMENT_PARAM_LIST_ENUM_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		#define SETUP_PARAM_LIST_ENUM(n, l)						n##_Param.SetupEx(&(l));
			//n##_Param.SetupEx(LYNX_ARRAY_SIZE(LynxEngine::g##t##_EnumData), LynxEngine::g##t##_EnumData)*/
		// ------------------------------------------------------------------

		// ------------------------------------------------------------------
		//
		// ------------------------------------------------------------------
		#define DECLAR_PARAM_STRING(n) \
			LynxEngine::CString			n;\
			LynxEngine::CStringParam	n##_Param;
		// ------------------------------------------------------------------
		//
		// ------------------------------------------------------------------
		#define IMPLEMENT_PARAM_STRING(n, b) \
			IMPLEMENT_PARAM(n, b)
			//LynxEngine::CParam* n##_pParam = new LynxEngine::CStringParam;\
			//IMPLEMENT_PARAM2(n##_pParam, n, b)			
		// ------------------------------------------------------------------
		//
		// ------------------------------------------------------------------
		#define IMPLEMENT_PARAM_STRING_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
			//LynxEngine::CParam* n##_pParam = new LynxEngine::CStringParam;\
			//IMPLEMENT_PARAM_COMMENT2(n##_pParam, n, b, c)			
		
		// ------------------------------------------------------------------
		//
		// ------------------------------------------------------------------
		#define DECLAR_PARAM_ANSISTRING(n) \
			LynxEngine::CAnsiString			n;\
			LynxEngine::CAnsiStringParam	n##_Param;
		// ------------------------------------------------------------------
		//
		// ------------------------------------------------------------------
		#define IMPLEMENT_PARAM_ANSISTRING(n, b) \
			IMPLEMENT_PARAM(n, b)
		// ------------------------------------------------------------------
		//
		// ------------------------------------------------------------------
		#define IMPLEMENT_PARAM_ANSISTRING_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)

		// ------------------------------------------------------------------
		
		// Array Param ------------------------------------------------------
		#define DECLAR_PARAM_ARRAY(t, n) \
			LynxEngine::CArray<t>				n;\
			LynxEngine::CArrayParam<t>			n##_Param;
		#define IMPLEMENT_PARAM_ARRAY(n, t, b) \
			IMPLEMENT_PARAM(n, b);\
			n##_Param.SetElementType(t);			
		#define IMPLEMENT_PARAM_ARRAY_COMMENT(n, t, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c);\
			n##_Param.SetElementType(t);
		// ------------------------------------------------------------------

		// Native Array Param -----------------------------------------------
		#define DECLAR_PARAM_NATIVE_ARRAY(t, n, s) \
			t									n[s];\
			LynxEngine::CNativeArrayParam<t>	n##_Param;
		#define IMPLEMENT_PARAM_NATIVE_ARRAY(n, t, s, b) \
			IMPLEMENT_PARAM(n, b);\
			n##_Param.SetElementType(t);\
			n##_Param.vSetNumElements(s)			;
		#define IMPLEMENT_PARAM_NATIVE_ARRAY_COMMENT(n, t, s, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c);\
			n##_Param.SetElementType(t);\
			n##_Param.vSetNumElements(s);
		// ------------------------------------------------------------------

		// List Param ------------------------------------------------------
		#define DECLAR_PARAM_LIST(t, n) \
			LynxEngine::CList<t>				n;\
			LynxEngine::CListParam<t>			n##_Param;
		#define IMPLEMENT_PARAM_LIST(n, t, b) \
			IMPLEMENT_PARAM(n, b);\
			n##_Param.SetElementType(t);			
		#define IMPLEMENT_PARAM_LIST_COMMENT(n, t, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c);\
			n##_Param.SetElementType(t);
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_LPOBJ(t, n) \
			t*						n;\
			LynxEngine::CObjPointerParam<t>	n##_Param;		
		#define IMPLEMENT_PARAM_LPOBJ(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_LPOBJ_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_COUNTEDPTR(t, n) \
			CCountedPtr<t>					n;\
			LynxEngine::CCountedPtrParam<t>	n##_Param;
		#define IMPLEMENT_PARAM_COUNTEDPTR(n, b) \
			IMPLEMENT_PARAM(n, b)
		#define IMPLEMENT_PARAM_COUNTEDPTR_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_SHADERCLASS(n) \
			LynxEngine::GraphicsSystem::CShaderClassPtr		n;\
			LynxEngine::CShaderClassPtrParam				n##_Param;
		#define IMPLEMENT_PARAM_SHADERCLASS(n, b) \
			IMPLEMENT_PARAM(n, b)			
		#define IMPLEMENT_PARAM_SHADERCLASS_COMMENT(n, c, b) \
			IMPLEMENT_PARAM_COMMENT(n, b, c)
		// ------------------------------------------------------------------	
		#define READYONLY_PARAM(n) \
			n##_Param.SetFlag(CParam::PARAM_READONLY);
	#else
		#define DECLAR_PARAM_MANAGER
		#define SET_PARAM_FLAG(n, f)

		#define DECLAR_PARAM_BOOL(n)				LYNXBOOL	n;					
		#define IMPLEMENT_PARAM_BOOL(n, b) 
		#define IMPLEMENT_PARAM_BOOL_COMMENT(n, c, b) 

		#define DECLAR_PARAM_FLOAT(n)				float		n;			
		#define IMPLEMENT_PARAM_FLOAT(n, b) 
		#define IMPLEMENT_PARAM_FLOAT_COMMENT(n, c, b)
		
		#define DECLAR_PARAM_RANGE_FLOAT(n)			float		n;			
		#define IMPLEMENT_PARAM_RANGE_FLOAT(n, a0, a1, a2, b) 
		#define IMPLEMENT_PARAM_RANGE_FLOAT_COMMENT(n, a0, a1, a2, c, b)

		#define DECLAR_PARAM_INT(n)					int			n;			
		#define IMPLEMENT_PARAM_INT(n, b) 
		#define IMPLEMENT_PARAM_INT_COMMENT(n, c, b)

		#define DECLAR_PARAM_RANGE_INT(n)			int			n;
		#define IMPLEMENT_PARAM_RANGE_INT(n, a0, a1, b) 
		#define IMPLEMENT_PARAM_RANGE_INT_COMMENT(n, a0, a1, c, b)

		#define DECLAR_PARAM_DWORD(n)				LYNXDWORD				n;			
		#define IMPLEMENT_PARAM_DWORD(n, b)
		#define IMPLEMENT_PARAM_DWORD_COMMENT(n, c, b)

		#define DECLAR_PARAM_URGB(n)				LYNXCOLORRGB	n;
		#define IMPLEMENT_PARAM_URGB(n, b) 
		#define IMPLEMENT_PARAM_URGB_COMMENT(n, c, b) 

		#define DECLAR_PARAM_URGBA(n)				LYNXCOLORRGBA	n;
		#define IMPLEMENT_PARAM_URGBA(n, b) 
		#define IMPLEMENT_PARAM_URGBA_COMMENT(n, c, b) 

		#define DECLAR_PARAM_VECTOR2D(n) 			LynxEngine::Math::CVector2	n;
		#define IMPLEMENT_PARAM_VECTOR2D(n, b)
		#define IMPLEMENT_PARAM_VECTOR2D_COMMENT(n, c, b)

		#define DECLAR_PARAM_VECTOR3D(n) 			LynxEngine::Math::CVector3	n;			
		#define IMPLEMENT_PARAM_VECTOR3D(n, b)
		#define IMPLEMENT_PARAM_VECTOR3D_COMMENT(n, c, b)

		#define DECLAR_PARAM_MATRIX4(n)				LynxEngine::Math::CMatrix4	n;			
		#define IMPLEMENT_PARAM_MATRIX4(n, b)
		#define IMPLEMENT_PARAM_MATRIX4_COMMENT(n, c, b)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_ENUM(p, t, n)			p::t		n;
		#define IMPLEMENT_PARAM_ENUM(n, b) 
		#define IMPLEMENT_PARAM_ENUM_COMMENT(n, c, b) 
		//#define IMPLEMENT_PARAM_ENUM(n, t, b) 
		//#define IMPLEMENT_PARAM_ENUM_COMMENT(n, t, b, c) 

		#define DECLAR_PARAM_LIST_ENUM(t, n) 		t			n;			
		#define IMPLEMENT_PARAM_LIST_ENUM(n, t, b)
		#define IMPLEMENT_PARAM_LIST_ENUM_COMMENT(n, t, c, b)
		#define SETUP_PARAM_LIST_ENUM(n, l)
		// ------------------------------------------------------------------

		#define DECLAR_PARAM_STRING(n)				LynxEngine::CString		n;			
		#define IMPLEMENT_PARAM_STRING(n, b) 
		#define IMPLEMENT_PARAM_STRING_COMMENT(n, c, b)

		#define DECLAR_PARAM_ANSISTRING(n)			LynxEngine::CAnsiString	n;			
		#define IMPLEMENT_PARAM_ANSISTRING(n, b) 
		#define IMPLEMENT_PARAM_ANSISTRING_COMMENT(n, c, b)

		#define DECLAR_PARAM_ARRAY(t, n)			LynxEngine::CArray<t>	n;
		#define IMPLEMENT_PARAM_ARRAY(n, t, b) 
		#define IMPLEMENT_PARAM_ARRAY_COMMENT(n, t, c, b)

		#define DECLAR_PARAM_NATIVE_ARRAY(t, n, s) 	t			n[s];
		#define IMPLEMENT_PARAM_NATIVE_ARRAY(n, t, s, b) 
		#define IMPLEMENT_PARAM_NATIVE_ARRAY_COMMENT(n, t, s, c, b) 

		#define DECLAR_PARAM_LIST(t, n)				LynxEngine::CList<t>	n;
		#define IMPLEMENT_PARAM_LIST(n, t, b) 
		#define IMPLEMENT_PARAM_LIST_COMMENT(n, t, c, b)

		#define DECLAR_PARAM_LPOBJ(t, n) 			t*						n;		
		#define IMPLEMENT_PARAM_LPOBJ(n, b)
		#define IMPLEMENT_PARAM_LPOBJ_COMMENT(n, c, b)

		#define DECLAR_PARAM_COUNTEDPTR(t, n)		CCountedPtr<t>			n;
		#define IMPLEMENT_PARAM_COUNTEDPTR(n, b)
		#define IMPLEMENT_PARAM_COUNTEDPTR_COMMENT(n, c, b)

		#define DECLAR_PARAM_SHADERCLASS(n)			LynxEngine::GraphicsSystem::CShaderClassPtr		n;
		#define IMPLEMENT_PARAM_SHADERCLASS(n, b)
		#define IMPLEMENT_PARAM_SHADERCLASS_COMMENT(n, c, b)

		#define READYONLY_PARAM(n)
	#endif
}

#endif