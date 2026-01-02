#ifndef __LYNXSCROLLGROUP_H__
#define __LYNXSCROLLGROUP_H__

#include <LynxEngineClasses.h>
#include <LynxCreationDesc.h>
#include <Container/LynxSpriteContainer.h>
#include <GUISystem/LynxControl.h>

namespace LynxEngine
{
	namespace GUISystem
	{
		class CScrollGroup : public CObj
		{
			LYNX_DECLARE_CLASS(CScrollGroup);
            
            static const unsigned int BOUNCE_BACK_TO_LEFT      = 0x1;
            static const unsigned int BOUNCE_BACK_TO_RIGHT     = 0x1<<1;
            static const unsigned int BOUNCE_BACK_TO_TOP       = 0x1<<2;
            static const unsigned int BOUNCE_BACK_TO_BOTTOM    = 0x1<<3;
			
			static const unsigned int X_SCROLL				   = 0x1;
			static const unsigned int Y_SCROLL				   = 0x2;
		public:
			CVector2					m_SnapSize;
			LYNXBOOL					m_bSnappingStop;
            LYNX_BITFIELD_BOOL(m_bClip)
			int							m_RenderOrder;
			int							m_BottomExtentionHight;
		protected:
			LYNX_BITFIELD_BOOL(m_bInitShow)
			LYNX_BITFIELD_BOOL(m_bInitActive)
			LYNX_BITFIELD_BOOL(m_bShow)
			LYNX_BITFIELD_BOOL(m_bActive)

			GUISystem::CContext*		m_Context;	
            CControl*                   m_ParentControl;
			LYNXRECT					m_Rect;
			LYNXRECT					m_ViewableRect;	//顯示範圍
			Math::CVector2				m_Size;			//內容大小	
            LYNXBOOL                    m_bBouncingX;
            LYNXBOOL                    m_bBouncingY;
			LYNXBOOL                    m_bAutoMoving;
			float						m_AutoMovingDir;
			int							m_AutoMovingTargetPage;
            float                       m_BouncingSpeed;
            unsigned int                m_BouncingType;
            Math::CVector2				m_PanVelocity;
			Math::CVector2				m_Offset;
			float						m_BoundRatioLeft;
			float						m_BoundRatioRight;
			float						m_BoundRatioTop;
			float						m_BoundRatioDown;			
			CList<CControl*>			m_ControlList;
			float						m_PanVelocityScale;
			int							m_ScrollFlag;
		public:	
			CScrollGroup(CEngine* lpe);
			virtual ~CScrollGroup();	

			LYNXFORCEINLINE LYNXBOOL    IsActive() {return m_bActive; }
			LYNXFORCEINLINE void        SetParentControl(CControl* c) {m_ParentControl = c; }            
			LYNXFORCEINLINE void		SetContext(GUISystem::CContext* c) {m_Context = c; }
			LYNXFORCEINLINE CContext*	GetContext() {return m_Context; }

			LYNXBOOL					vInit();
			LYNXBOOL					vCreate();
			LYNXBOOL					Create(const LYNXRECT& rect);
			void						vLoop(float step);
			void						vRender();

			LYNXFORCEINLINE void		SetScrollFlag(const int flag) {m_ScrollFlag = flag;}
			LYNXFORCEINLINE const int	GetScrollFlag() {return m_ScrollFlag;}
			
			LYNXFORCEINLINE void		SetRenderOrder(int o) {m_RenderOrder = o; };
			void						SortControls();

			void						AddControl(CControl* con, LYNXBOOL bcompute_size = LYNX_TRUE);
			void						RemoveControl(CControl* con);
			void						RemoveAllControls();
			void						ReleaseAllControls();

			int							GetCurrentPage();
			void						SetCurrentPage(int p);
			void						GoNextPage(const float dir);
			void						GoOffset(const CVector2& offset);

			LYNXFORCEINLINE void		SetPanVelocityScale(const float s) {m_PanVelocityScale = s; }
			LYNXFORCEINLINE void		SetPanVelocity(const Math::CVector2& v) {m_PanVelocity = v * m_PanVelocityScale; }			
			
			LYNXFORCEINLINE CList<CControl*>& GetControlList() {return m_ControlList; }
			LYNXFORCEINLINE LYNXRECT&	GetRect() {return m_Rect; }
			LYNXFORCEINLINE void		SetRect(LYNXRECT& r) {m_Rect = r; }

			LYNXFORCEINLINE const Math::CVector2& GetOffset() const {return m_Offset; }
			LYNXFORCEINLINE void		GetOffset(Math::CVector2& o) const {o = m_Offset; }
			LYNXFORCEINLINE void		SetOffset(const Math::CVector2& o) {m_Offset = o; }
			void						ResetOffset();
			void						SetTransitionBoundRatio(float xleft, float xright, float ytop, float ydown);
            LYNXFORCEINLINE void        SetSize(int w, int h) {m_Size.x = w; m_Size.y = h; }
			LYNXFORCEINLINE float       GetWidth() {return m_Size.x; }
			LYNXFORCEINLINE float       GetHeight() {return m_Size.y; }
            
			void						SetInitShow(LYNXBOOL b);
			void						SetInitActiveState(LYNXBOOL b);
			void						Show(LYNXBOOL b);
			void						Activate(LYNXBOOL b);
			void						ResetShow();
			void						ResetActiveState();
			LYNXFORCEINLINE LYNXBOOL	IsShown() const {return m_bShow; };
			void						ResetControlTransitionOffset();

			void						Transit();
			void						TransitBack(CArray<WORD>* list = NULL);
			LYNXBOOL					IsStop();
			LYNXBOOL					IsTransitionDone();
			LYNXBOOL					IsStepInDone();
			LYNXBOOL					IsStepOutDone();
			void						SetupContextStepOutList(CArray<WORD>* list);
			void						SetupContextStepIntList(CArray<WORD>* list);

            LYNXBOOL                    IsHit(Math::CVector2& pos);
			void						ComputeSizeByRect(LYNXRECT& rect);
			void						ComputeSizeWithControlList();
            
            LYNXFORCEINLINE void        SetBouncing(const LYNXBOOL bx, const LYNXBOOL by) {m_bBouncingX = bx; m_bBouncingY = by; }
            LYNXFORCEINLINE void        SetBouncingSpeed(const float s) {m_BouncingSpeed = s; }
		};
	}
}
#endif

