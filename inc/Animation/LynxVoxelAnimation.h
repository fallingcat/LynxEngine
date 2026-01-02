#ifndef __VOXELANIMATION_H__
#define __VOXELANIMATION_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

namespace LynxEngine 
{
	namespace Animation
	{
		class CVoxelAnimation : public CObj
		{
			LYNX_DECLARE_CLASS(CVoxelAnimation);
		public:											
			LYNXBOOL						m_bPlayStop;
			Math::CReal						m_Time, m_LastTime;
			Math::CMatrix4					m_M;			

			DECLAR_PARAM_FLOAT(m_StartTime)
			DECLAR_PARAM_FLOAT(m_EndTime)			
			DECLAR_PARAM_FLOAT(m_Step)
			DECLAR_PARAM_ENUM(LynxEngine, PLAYMODE, m_PlayMode)
		protected:	
			Math::CReal						m_PlayDir;
			int								m_Voxel;
			CAnimationKeys*					m_PositionAnimationKeys;			
			CAnimationKeys*					m_ColorAnimationKeys;			
		public:	
			CVoxelAnimation(CEngine* lpe);			
			virtual ~CVoxelAnimation();	

			void							Create(int voxel, int pos_key, int color_key);
			void							vInstance(CVoxelAnimation& rhs);

			CAnimationKeys*					GetPositionAnimationKeys() {return m_PositionAnimationKeys; };
			CAnimationKeys*					GetColorAnimationKeys() {return m_ColorAnimationKeys; };

			LYNXFORCEINLINE void			SetVoxel(int voxel) {m_Voxel = voxel; };
			LYNXFORCEINLINE int				GetVoxel() const {return m_Voxel; };
			void							SetPositionKey(int key, int st, int et, Math::CVector3& pos);
			void							AddPositionKey(int st, Math::CVector3& pos);
			void							SetColorKey(int key, int st, int et, LYNXCOLORRGBA& color);
			void							AddColorKey(int st, LYNXCOLORRGBA& color);

			LYNXBOOL						Play(LYNXREAL step);
		};
	}	

	LYNXFORCEINLINE bool operator == (const Animation::CVoxelAnimation* lhs, const Animation::CVoxelAnimation& rhs) {return (lhs->GetVoxel() == rhs.GetVoxel()); };
}

#endif

