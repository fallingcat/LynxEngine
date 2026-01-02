#ifndef __LYNXSPRITE_H__
#define __LYNXSPRITE_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class LYNXENGCLASS CSpriteCreationDesc : public CCreationDesc
		{	
			LYNX_DECLARE_CLASS(CSpriteCreationDesc);	
		public:
			LynxEngine::CSpriteCreationDesc m_SpriteDesc;		
		public:				
			CSpriteCreationDesc() { Default(); };
			void Default()	{m_SpriteDesc.Default(); };
		};

		class CSprite : public CControl
		{
			LYNX_DECLARE_CLASS(CSprite);	
		protected:				
			CContainer*					m_lpContainer;
            LYNXBOOL                    m_bUseCustomHitRect;
			LYNXRECT					m_Rect;			
			Math::CVector2				m_Size;
			SoundSystem::CSoundPtr      m_HitSound;

			CVector2					m_TransitionOffset;
			CVector2					m_TotalOffset;
			LYNXRECT					m_TargetAlignmentRect;
		public:			
			CSprite(CGUISystem* lpsys);
			virtual ~CSprite();	

			DECLARE_CLASSSCRIPT
			LYNXBOOL					vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			void						_SetTargetScale(const float target, const float steps) {SetTargetScale(target, steps); };			

			LYNXBOOL					vInit();
			LYNXBOOL					vCreate(CCreationDesc* desc);
			void						vSetAlignmentType(const DWORD type);
			void						vRender();
			void						vSetSize(float w, float h);
			LYNXBOOL					vHitTest(const LYNXPOINT2D* p);
			void						vOnHit();
			LYNXFORCEINLINE void		vSetPosition(const Math::CVector3& pos) {m_lpContainer->vSetPosition(pos); };			
			void						SetTransitionOffset(const Math::CVector2& offset);
			void						TransitionLoop();
            
            void                        SetHitSound(const CString& name, const CString& filename);
			
            LYNXFORCEINLINE void        GetHitRect(LYNXRECT& r) {r = m_Rect; };
            LYNXFORCEINLINE void        UseCustomHitRect(const LYNXRECT* r)
            {
                if (r)
                {
                    m_bUseCustomHitRect = LYNX_TRUE;
                    m_Rect = *r;
                }
                else
                {
                    m_bUseCustomHitRect = LYNX_FALSE;
                }
            }

			LYNXFORCEINLINE CContainer* GetlpContainer() {return m_lpContainer; };
		};
	}
}
#endif

