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
//  Created by Owen Wu : 2006/11/16
//  Last Update : 
//
//###########################################################################

#ifndef __LYNXANIMATION_H__
#define __LYNXANIMATION_H__

#include <LynxObj.h>
#include <LynxMathWrap.h>
#include <LynxCoreEnumString.h>
#include <LynxEngineClasses.h>

namespace LynxEngine 
{
	namespace Animation
	{
		typedef struct {
			int								StartTime, EndTime;
			CArray<Math::CReal>				Values;			
		}ANIMATIONKEYDATA;

		class LYNXENGCLASS CAnimation : public CObj
		{	
			LYNX_DECLARE_CLASS(CAnimation);
		public:						
			typedef enum {								
				KT_POSITION = 0,
				KT_ROTATION,
				KT_SCALE,
				KT_COLOR,
				KT_TEXTURE,				
				KT_VERTEX,
				KT_UV,
				KT_VISIBILITY,
				NUM_KEYTYPE,
			}KEYSTYPE;	

			typedef struct {
				float							TriggerTime;					
				float							EndTime;
				std::function<void(void)>		Func;
			}PLAYBACKEVENTDATA;

			LYNXBOOL							m_bIntTime;
			LYNXBOOL							m_bPlayStop;
			LYNXBOOL							m_bIsThisFrameStop;
			Math::CReal							m_Time, m_LastTime;
			Math::CMatrix4						m_M;					
			Math::CMatrix4						m_TextureUVM;	
			Math::CMatrix4						m_PivotM;	
			Math::CMatrix4						m_InversePivotM;
			LYNXBOOL							m_bPause;
			int									m_LoopTimes;
			
			DECLAR_PARAM_FLOAT(m_StartTime)
			DECLAR_PARAM_FLOAT(m_EndTime)			
			DECLAR_PARAM_FLOAT(m_Step)
			DECLAR_PARAM_ENUM(LynxEngine, PLAYMODE, m_PlayMode)

			CAnimationKeysPtr					m_AnimationKeys[NUM_KEYTYPE];			
		protected:					
			Math::CReal							m_PlayDir;
			CList<PLAYBACKEVENTDATA>			m_PlaybackEventList;

		public:			
			CAnimation(CEngine* lpe);			
			virtual ~CAnimation();	

			DECLARE_CLASSSCRIPT
			LYNXBOOL							vRegisterScriptClassVar(ScriptSystem::CScript* const lps);
			LYNXBOOL							vRegisterScriptClassMethod(ScriptSystem::CScript* const lps);
			void								_CreateKeys(KEYSTYPE kt, int num, int numv) {CreateKeys(kt, num, numv); };

			void								CreateKeys(KEYSTYPE kt);
			void								CreateKeys(KEYSTYPE kt, int num, int numv);
			void								DeleteKeys(KEYSTYPE kt);
			void								SetKeyStartTime(KEYSTYPE kt, int key, float s);
			void								SetKeyEndTime(KEYSTYPE kt, int key, float e);
			void								SetKeyValue(KEYSTYPE kt, int key, int v, float d);
			void								SetKeyValues(KEYSTYPE kt, int key, int num, float* d);			
			void								SetKeyValues(KEYSTYPE kt, const CArray<Animation::ANIMATIONKEYDATA>& keys, float lifeTime);

			LYNXINLINE void						ResetTime() {m_Time = m_StartTime; m_bPlayStop = LYNX_FALSE; m_LoopTimes = 0;};
			LYNXINLINE void						SetRange(LYNXREAL s, LYNXREAL e) {m_StartTime  = s; m_EndTime = e; m_Time = m_StartTime;};
			LYNXINLINE void						GetRange(LYNXREAL* s, LYNXREAL* e) {*s = m_StartTime; *e = m_EndTime; };
			LYNXINLINE void						SetPlayMode(PLAYMODE pm) {m_PlayMode = pm; };
			LYNXINLINE void						SetStep(LYNXREAL s) {m_Step = s; };
			virtual void						vInstance(CAnimation& rhs);
			int									Load(CStream& stream);
			static void							SkipLoading(CStream& stream);
			LYNXBOOL							Play(LYNXREAL step);
			void								UpdateTransform();
			void								UpdateTextureUVTransform();
			float								UpdateVisibility();
			LYNXINLINE void						Pause(LYNXBOOL b) {m_bPause = b; }

			void								AddPlaybackEvent(Math::CReal& t_time, Math::CReal& e_time, std::function<void(void)> callback);

			LYNXBOOL							vLoad(CUserProfile* file, CXMLElement* c);
			LYNXBOOL							vSave(CUserProfile* file, CXMLElement* c);
						
			void								ReadAnimationKeyFrame(CUserProfile* file, CXMLElement* node, KEYSTYPE kt, const CString& key, float LifeTime);
			void								WriteAnimationKeyFrame(CUserProfile* file, CXMLElement* node, KEYSTYPE kt, const CString& key);
		};
	}
	LYNXENGCLASS CString GetEnumStr(Animation::CAnimation::KEYSTYPE e);	
	LYNXENGCLASS PLAYMODE TranslatePlayMode(const CString& playmode);
}
#endif

