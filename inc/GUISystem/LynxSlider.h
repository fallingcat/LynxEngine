#ifndef __LYNXSLIDER_H__
#define __LYNXSLIDER_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class LYNXENGCLASS CSliderCreationDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CSliderCreationDesc);	
		public:
            LynxEngine::CSpriteCreationDesc m_SpriteDesc;
			LynxEngine::CSpriteCreationDesc m_BarSpriteDesc;
			DWORD							m_AlignmentType;
            LYNXRECT                        m_Rect;  
            int                             m_RangeMin;
            int                             m_RangeMax;
            int                             m_Pos;
		public:				
			CSliderCreationDesc() { Default(); };
			void Default()	{m_SpriteDesc.Default(); m_BarSpriteDesc.Default(); m_AlignmentType = CSpriteContainer::H_ALIGN_CENTER; m_RangeMin = 0; m_RangeMax = 100; m_Pos = 0; };
		};
        
		class CSlider : public CControl
		{
			LYNX_DECLARE_CLASS(CSlider);	
		public:
			LYNXBOOL					m_bShowCustomText;
			CString						m_CustomText;
		protected:
			//CContainer*					m_lpContainer;
            CContainer*					m_lpBarContainer;
			WORD						m_AlignmentType;			
            int                         m_CurrentPos;
            int                         m_Min, m_Max;
            float                       m_Step;
			LYNXRECT					m_RenderRect;	
        public:			
			CSlider(CGUISystem* lpsys);
			virtual ~CSlider();	
        
            // Script ------------------------------------------------------------------------------
			DECLARE_CLASSSCRIPT
			LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
        
            
			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);
			CSpriteContainer::ORIGINTYPE vSetOriginType(const CSpriteContainer::ORIGINTYPE ot);
			DWORD						vSetAlignmentType(const DWORD type);
			void						vSetAlignmentRect(const LYNXRECT& r);
			void						vGetAlignmentRect(LYNXRECT& r);
			void						vRender();
			void						vSetSize(float w, float h);
			LYNXBOOL					vHitTest(const LYNXPOINT2D* p);
			void						vOnHit() {};
            void                        vSetPosition(const Math::CVector3& pos);
			void						vGetPosition(Math::CVector3& pos);
			
            void                        SetPos(int pos);
            LYNXFORCEINLINE int         GetPos() const {return m_CurrentPos; };
            void                        SetRange(int min, int max);
            LYNXFORCEINLINE int         GetRangeMin() const {return m_Min; };
            LYNXFORCEINLINE int         GetRangeMax() const {return m_Max; };
                    
			LYNXFORCEINLINE CContainer* GetlpContainer() {return m_lpContainer; };
            LYNXFORCEINLINE CContainer* GetlpBarContainer() {return m_lpBarContainer; };
		};
	}
}
#endif


