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

#ifndef __LYNXGUISYSTEM_H__
#define __LYNXGUISYSTEM_H__

#include <LynxEngineClasses.h>
#include <LynxSystem.h>

namespace LynxEngine 
{		
	namespace GUISystem 
	{		
		class LYNXENGCLASS CGUISystem : public CSystem
		{	
			LYNX_DECLARE_CLASS(CGUISystem);
		public:												
		protected:		
			CFactory<CControl, CAnsiString, CGUISystem*> m_ControlFactory;

			LYNXRECT						m_ClipRect;
			int								m_ReferenceWidth;
			int								m_ReferenceHeight;
			Math::CVector2					m_OriginPos;
			Math::CVector2					m_PositionFactor;
			Math::CVector2					m_ScaleFactor;
		public:						
			CGUISystem(CEngine* const lpengine);				
			virtual ~CGUISystem();							

			DECLARE_CLASSSCRIPT
			LYNXBOOL						vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);

			virtual LYNXBOOL				vCreate();
			virtual LYNXBOOL				vInit();
			LYNXFORCEINLINE void			SetClipRect(const LYNXRECT& rect) { m_ClipRect = rect; }
			LYNXFORCEINLINE const LYNXRECT&	GetClipRect() const { return m_ClipRect; }
			void							RegisterControl(const CAnsiString& t, CControl* (*func)(CGUISystem*));
			void							ComputeLayoutFactor(LYNXBOOL bkeepratio = LYNX_TRUE, LYNXAXISTYPE major_axis = LYNX_Z_AXIS);
			void							ResetLayoutFactor() {m_PositionFactor = Math::CVector2(1.0f, 1.0f); m_ScaleFactor = Math::CVector2(1.0f, 1.0f); };
			LYNXFORCEINLINE void			SetReference(int w, int h) {m_ReferenceWidth = w; m_ReferenceHeight = h; };

			LYNXFORCEINLINE Math::CVector2&	GetPositionFactor() {return m_PositionFactor; }
			LYNXFORCEINLINE Math::CVector2&	GetScaleFactor() {return m_ScaleFactor; }
			LYNXFORCEINLINE int&			GetReferenceWidth() {return m_ReferenceWidth; }
			LYNXFORCEINLINE int&			GetReferenceHeight() {return m_ReferenceHeight; }

			Math::CVector2&					GetOriginPos() {return m_OriginPos; };
			CControl*						CreateControl(const CAnsiString& classname);			
		private:
			static CControl*				CreateCButton(CGUISystem* s);
			static CControl*				CreateCStateButton(CGUISystem* s);
			static CControl*				CreateCImage(CGUISystem* s);
			static CControl*				CreateCSlider(CGUISystem* s);			
			static CControl*				CreateCText(CGUISystem* s);
		};		
	}
}
#endif