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

#ifndef __LYNXFLOATPARAM_H__
#define __LYNXFLOATPARAM_H__

#include <LynxParam.h>

namespace LynxEngine 
{
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CFloatParam : public CParam
	{	
	public:
	protected:					
	public:	
		CFloatParam() {m_Type = CParam::FLOAT; };
		CFloatParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CParam(groupname, parent, name, lpv) {m_Type = CParam::FLOAT; };
		virtual ~CFloatParam() {};			

		LYNXINLINE					operator float*() { return ((float*)m_lpValue); };			
		LYNXINLINE float			GetFloat() { return (*((float*)m_lpValue)); };		
	};		
	/**	
	*  @brief Base object class of the entire engine。
	* 
	*  The basic object class of the entire engine, every class of the engine is drived from this class.
	*  It records a unique ID and a reference counter. 
	*	
	*/
	class LYNXENGCLASS CRangeFloatParam : public CFloatParam
	{	
	public:
	protected:					
		float						m_Min, m_Max;
		float						m_Scale;
	public:	
		CRangeFloatParam() {m_Type = CParam::RANGE_FLOAT; m_Min = 0.0f; m_Max = 1.0f; m_Scale = 100.0f; };
		CRangeFloatParam(const CString& groupname, CParam* parent, const CString& name, void* const lpv): CFloatParam(groupname, parent, name, lpv) {m_Type = CParam::RANGE_FLOAT; m_Min = 0.0f; m_Max = 1.0f; m_Scale = 100.0f; };
		virtual ~CRangeFloatParam() {};			

		LYNXINLINE void				SetupEx(float min, float max, float scale) {m_Min = min; m_Max = max; m_Scale = scale; };			
		LYNXINLINE float			GetMin() const { return m_Min; };			
		LYNXINLINE float			GetMax() const { return m_Max; };			
		LYNXINLINE float			GetScale() const { return m_Scale; };			
	};		
}

#endif