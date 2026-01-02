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

#ifndef __LYNXPARAM_H__
#define __LYNXPARAM_H__

#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CParam
	{	
	public:
		enum TYPE{
			STATIC = 0,
			STRING,
			ANSI_STRING,
			BOOL,
			INT,
			LONG,
			DWORD,
			REAL,
			FLOAT,
			DOUBLE,
			VECTOR2D,
			VECTOR3D,
			VECTOR4D,
			MATRIX3,
			MATRIX4,
			UB_RGB,
			UB_RGBA,
			FLOAT_RGB,
			FLOAT_RGBA,
			ENUM,
			LIST_ENUM,
			RANGE_INT,
			RANGE_FLOAT,
			BROWSER,
			ARRAY,
			NATIVE_ARRAY,
			LIST,
			STRUCTURE,
			OBJ,
			LPOBJ,
			COUNTEDPTR,
			SHADER_CLASS,
		};
		static const int PARAM_READONLY			= 0x1;		
		static const int PARAM_SIZE_READONLY	= 0x2;	
		static const int PARAM_NO_BROWSER		= 0x4;	
	protected:			
		CString						m_GroupName;
		CString						m_Name;
		CString						m_Comment;
		TYPE						m_Type;
		void*						m_lpValue;
		int							m_Flag;		
		CParam*						m_ParentParam;
		CList<CParam*>				m_SubParamList;
		static CObjPtr				m_NULLPtr;	
	public:	
		CParam();		
		CParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv, int flag = 0, const CString& comment = "");
		virtual ~CParam();			
		CParam&						operator =(const CParam& rhs);		
		void						Setup(const CString& groupname, CParam* parent, const CString& name, void* const lpv, int flag = 0, const CString& comment = "");
		LYNXINLINE const CString&	GetGroupName() {return m_GroupName; };
		LYNXINLINE const CString&	GetName() {return m_Name; };
		LYNXINLINE const CString&	GetComment() {return m_Comment; };
		LYNXINLINE TYPE				GetType() {return m_Type; };
		LYNXINLINE void*			GetlpValue() {return m_lpValue; };
		LYNXINLINE void				SetFlag(int f) {m_Flag = f; };
		LYNXINLINE int				GetFlag() const {return m_Flag; };
		//LYNXINLINE void				SetParentParam(CParam* p) {m_ParentParam = p; m_ParentParam->A};
		LYNXINLINE CParam*			GetParentParam() const {return m_ParentParam; };
		LYNXINLINE void				AddSubParam(CParam* p) {m_SubParamList.push_back(p); p->m_ParentParam = this; };
		LYNXINLINE CList<CParam*>	GetSubParamList() {return m_SubParamList; };
		virtual CObj*				vGetlpObj() {return NULL; };
		virtual void				vSetlpObj(CObj* lpo) {};
		virtual void				vSetlpObj(void* lpo) {};
		virtual LYNXINLINE LYNXBOOL	vIsTheSameTypeObjPointer(CObj* lpo) {return LYNX_FALSE; };	
		virtual LYNXINLINE LYNXBOOL	vIsTheSameSubTypeObjPointer(CObj* lpo) {return LYNX_FALSE; };	
	};	
}

#endif