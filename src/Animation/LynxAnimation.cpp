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
//  隤芣?:
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <Animation/LynxAnimation.h>
#include <Animation/LynxAnimationKeys.h>

namespace LynxEngine 
{	
	//-------------------------------------------------------------------------------------------------------
	//
	//  
	//-------------------------------------------------------------------------------------------------------
	CString GetEnumStr(Animation::CAnimation::KEYSTYPE e)
	{
		CString				KeysType;

		switch (e)
		{
			case Animation::CAnimation::KT_POSITION:
				KeysType = _T("Position");
				break;
			case Animation::CAnimation::KT_ROTATION:
				KeysType = _T("Rotation");
				break;
			case Animation::CAnimation::KT_SCALE:
				KeysType = _T("Scale");
				break;
			case Animation::CAnimation::KT_COLOR:
				KeysType = _T("Color");
				break;
			case Animation::CAnimation::KT_TEXTURE:
				KeysType = _T("Texture");
				break;
		}
		return KeysType;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	PLAYMODE TranslatePlayMode(const CString& playmode)
	{
		for (int i = 0; i < NUM_PLAYMODE; ++i)
		{
			if (GetEnumStr((PLAYMODE)i) == playmode)
				return (PLAYMODE)i;
		}

		return PM_ONCE;
	}

	namespace Animation
	{
		IMPLEMENT_CLASSSCRIPT(CAnimation, CObj)
		//-------------------------------------------------------------------------------------------------------
		//
		//  
		//-------------------------------------------------------------------------------------------------------
		CAnimation::CAnimation(CEngine* lpe)
		: CObj(lpe)
		{
			m_Time = m_LastTime = 0.0f;		
			m_StartTime = m_EndTime = 0.0f;
			m_Step = 1.0f;
			m_PlayMode = PM_LOOP;
			m_PlayDir = 1.0f;			
			m_bPause = LYNX_FALSE;
			m_bIntTime = LYNX_TRUE;
			m_LoopTimes = 0;
			m_bIsThisFrameStop = LYNX_FALSE;
			IMPLEMENT_PARAM_FLOAT(m_StartTime, 0)
			IMPLEMENT_PARAM_FLOAT(m_EndTime, 0)
			IMPLEMENT_PARAM_FLOAT(m_Step, 0)
			//IMPLEMENT_PARAM_ENUM(m_PlayMode, PLAYMODE, 0);
			IMPLEMENT_PARAM_ENUM(m_PlayMode, 0)
			m_PlaybackEventList.clear();

			INIT_CLASSSCRIPT
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  
		//-------------------------------------------------------------------------------------------------------
		CAnimation::~CAnimation(void)
		{
			m_Time = m_LastTime = 0.0f;				
			m_PlaybackEventList.clear();
		}	
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ??  
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::vInstance(CAnimation& rhs)
		{
			CObj::vInstance(rhs);

			m_bPlayStop = rhs.m_bPlayStop;
			m_StartTime = rhs.m_StartTime;
			m_EndTime = rhs.m_EndTime;
			m_Time = rhs.m_Time;
			m_LastTime = rhs.m_LastTime;
			m_Step = rhs.m_Step;
			m_PlayMode = rhs.m_PlayMode;
			m_LoopTimes = rhs.m_LoopTimes;
			m_PivotM = rhs.m_PivotM;	
			m_InversePivotM = rhs.m_InversePivotM;	
			m_M = rhs.m_M;

			for (int i=0; i<(int)NUM_KEYTYPE; i++) 
				m_AnimationKeys[i] = rhs.m_AnimationKeys[i];
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::CreateKeys(KEYSTYPE kt)
		{
			m_AnimationKeys[kt] = CAnimationKeysPtr(LYNXNEW CAnimationKeys(this));			
			m_AnimationKeys[kt]->SetKeysType(kt);
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::CreateKeys(KEYSTYPE kt, int num, int numv)
		{
			m_AnimationKeys[kt] = CAnimationKeysPtr(LYNXNEW CAnimationKeys(this, num, numv));			
			m_AnimationKeys[kt]->SetKeysType(kt);
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::DeleteKeys(KEYSTYPE kt)
		{
			LYNXDEL m_AnimationKeys[kt];						
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::SetKeyStartTime(KEYSTYPE kt, int key, float s)
		{
			m_AnimationKeys[kt]->m_Keys[key].SetStartTime(s);			
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::SetKeyEndTime(KEYSTYPE kt, int key, float e)
		{
			m_AnimationKeys[kt]->m_Keys[key].SetEndTime(e);			
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::SetKeyValue(KEYSTYPE kt, int key, int v, float d)
		{
			m_AnimationKeys[kt]->m_Keys[key].SetValue(v, d);			
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::SetKeyValues(KEYSTYPE kt, int key, int num, float* d)
		{
			m_AnimationKeys[kt]->m_Keys[key].SetValues(d, num);			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::SetKeyValues(KEYSTYPE kt, const CArray<Animation::ANIMATIONKEYDATA>& keys, float lifeTime)
		{
			if (!keys.empty())
			{
				int num = keys.size();
				int numv(keys[0].Values.size());
				CreateKeys(kt, num, numv);
				for (int i = 0; i < num - 1; i++)
				{
					m_AnimationKeys[kt]->m_Keys[i].SetStartTime(keys[i].StartTime);
					m_AnimationKeys[kt]->m_Keys[i].SetEndTime(keys[i + 1].StartTime);
					for (int v = 0; v < numv; v++)
					{
						m_AnimationKeys[kt]->m_Keys[i].SetValue(v, keys[i].Values[v]);
					}
				}

				m_AnimationKeys[kt]->m_Keys[num - 1].SetStartTime(keys[num - 1].StartTime);
				m_AnimationKeys[kt]->m_Keys[num - 1].SetEndTime(lifeTime);
				for (int v = 0; v < numv; v++)
				{
					m_AnimationKeys[kt]->m_Keys[num - 1].SetValue(v, keys[num - 1].Values[v]);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		int CAnimation::Load(CStream& stream)
		{
			int				NumKeys;
			int				ST, ET, MaxTime;
			LYNXREAL		KeyValues[4];

			LYNX_ASSERT(stream.IsValid());

			MaxTime = 0;			
			/* Read Rotation Animation Key */
			stream.vRead(&NumKeys, sizeof(int), 1);			
			CreateKeys(KT_ROTATION, NumKeys, 4);
			for (int k=0; k<m_AnimationKeys[KT_ROTATION]->m_Keys.size(); k++)
			{					
				stream.vRead(&ST, sizeof(int), 1);
				stream.vRead(&ET, sizeof(int), 1);
				if (ET > MaxTime)
					MaxTime = ET;
				m_AnimationKeys[KT_ROTATION]->m_Keys[k].SetStartTime(ST);
				m_AnimationKeys[KT_ROTATION]->m_Keys[k].SetEndTime(ET);
				stream.vRead(KeyValues, sizeof(LYNXREAL), 4);	
				m_AnimationKeys[KT_ROTATION]->m_Keys[k].SetValues(KeyValues, 4);
			}
			/* Read Translate Animation Key */
			stream.vRead(&NumKeys, sizeof(int), 1);
			CreateKeys(KT_POSITION, NumKeys, 3);
			for (int k=0; k<m_AnimationKeys[KT_POSITION]->m_Keys.size(); k++)
			{					
				stream.vRead(&ST, sizeof(int), 1);
				stream.vRead(&ET, sizeof(int), 1);
				if (ET > MaxTime)
					MaxTime = ET;
				m_AnimationKeys[KT_POSITION]->m_Keys[k].SetStartTime(ST);
				m_AnimationKeys[KT_POSITION]->m_Keys[k].SetEndTime(ET);
				stream.vRead(KeyValues, sizeof(LYNXREAL), 3);	
				m_AnimationKeys[KT_POSITION]->m_Keys[k].SetValues(KeyValues, 3);			
			}
			/* Read Scale lnimation Key */
			stream.vRead(&NumKeys, sizeof(int), 1);
			CreateKeys(KT_SCALE, NumKeys, 3);
			for (int k=0; k<m_AnimationKeys[KT_SCALE]->m_Keys.size(); k++)
			{					
				stream.vRead(&ST, sizeof(int), 1);
				stream.vRead(&ET, sizeof(int), 1);
				if (ET > MaxTime)
					MaxTime = ET;
				m_AnimationKeys[KT_SCALE]->m_Keys[k].SetStartTime(ST);
				m_AnimationKeys[KT_SCALE]->m_Keys[k].SetEndTime(ET);
				stream.vRead(KeyValues, sizeof(LYNXREAL), 3);	
				m_AnimationKeys[KT_SCALE]->m_Keys[k].SetValues(KeyValues, 3);						
			}
			/*
			SetRange(0.0f, MaxTime-1);				
			SetStep(1.0f);				
			SetPlayMode(PM_LOOP);
			*/

			return MaxTime;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::SkipLoading(CStream& stream)
		{
			LYNX_ASSERT(stream.IsValid());

			int NumKeys;
			stream.vSeek(sizeof(LYNXMATRIX)*2, CStream::SeekCur);							
			stream.vRead(&NumKeys, sizeof(int), 1);				
			stream.vSeek(((sizeof(int)*2)+(sizeof(LYNXREAL)*4))*NumKeys, CStream::SeekCur);				
			/* Read Translate Animation Key */
			stream.vRead(&NumKeys, sizeof(int), 1);				
			stream.vSeek(((sizeof(int)*2)+(sizeof(LYNXREAL)*3))*NumKeys, CStream::SeekCur);				
			/* Read Scale Animation Key */
			stream.vRead(&NumKeys, sizeof(int), 1);				
			stream.vSeek(((sizeof(int)*2)+(sizeof(LYNXREAL)*3))*NumKeys, CStream::SeekCur);				
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::UpdateTransform()
		{
			static Math::CReal  Values[4];
			static Math::CMatrix4 TM;
			float TrimmedTime;

			LYNX_IDENTITY_MATRIX(m_M);		 		

			if (m_bIntTime)
				TrimmedTime = LYNX_FLOOR(m_Time);
			else
				TrimmedTime = m_Time;
			
			if (m_AnimationKeys[KT_ROTATION] && m_AnimationKeys[KT_ROTATION]->m_Keys.size())
			{
				m_AnimationKeys[KT_ROTATION]->Lerp(Values, TrimmedTime);
				lynxQuaternion2Matrix(&m_M, (LPLYNXQUATERNION)Values);
			}			
			
			if (m_AnimationKeys[KT_SCALE] && m_AnimationKeys[KT_SCALE]->m_Keys.size())
			{
				m_AnimationKeys[KT_SCALE]->Lerp(Values, TrimmedTime);

				m_M.m11 = m_M.m11 * Values[0];
				m_M.m21 = m_M.m21 * Values[1];
				m_M.m31 = m_M.m31 * Values[2];

				m_M.m12 = m_M.m12 * Values[0];
				m_M.m22 = m_M.m22 * Values[1];
				m_M.m32 = m_M.m32 * Values[2];

				m_M.m13 = m_M.m13 * Values[0];
				m_M.m23 = m_M.m23 * Values[1];
				m_M.m33 = m_M.m33 * Values[2];
			}
			
			if (m_AnimationKeys[KT_POSITION] && m_AnimationKeys[KT_POSITION]->m_Keys.size())
			{
				m_AnimationKeys[KT_POSITION]->Lerp(Values, TrimmedTime);

				m_M.m41 += Values[0];
				m_M.m42 += Values[1];
				m_M.m43 += Values[2];				
			}						
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		void CAnimation::UpdateTextureUVTransform()
		{
			static Math::CReal  Values[4];
			static Math::CMatrix4 TM;
			float TrimmedTime;

			LYNX_IDENTITY_MATRIX(m_TextureUVM);		 		

			if (m_bIntTime)
				TrimmedTime = LYNX_FLOOR(m_Time);
			else
				TrimmedTime = m_Time;
			
			if (m_AnimationKeys[KT_UV] && m_AnimationKeys[KT_UV]->m_Keys.size())
			{
				m_AnimationKeys[KT_UV]->Lerp(Values, TrimmedTime);

				m_TextureUVM.m41 += Values[0];
				m_TextureUVM.m42 += Values[1];
			}						
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		float CAnimation::UpdateVisibility()
		{
			static Math::CReal  Values[4];
			static Math::CMatrix4 TM;
			float TrimmedTime;
			float Visibility = 1.0f;

			if (m_bIntTime)
				TrimmedTime = LYNX_FLOOR(m_Time);
			else
				TrimmedTime = m_Time;
			
			if (m_AnimationKeys[KT_VISIBILITY] && m_AnimationKeys[KT_VISIBILITY]->m_Keys.size())
			{
				m_AnimationKeys[KT_VISIBILITY]->Lerp(Values, TrimmedTime);
				Visibility = Values[0];				
			}		
			return Visibility;
		}
		//-------------------------------------------------------------------------------------------------------
		/**
		*	@brief ??
		*
		*	@param ??
		*	@param ?? 
		*	@return ??
		*/
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CAnimation::Play(LYNXREAL step)
		{
			if (m_bPause)
				return LYNX_FALSE;

			m_bPlayStop = FALSE;	
			if (m_EndTime == m_StartTime)
			{
				m_Time = m_EndTime;
				m_bPlayStop = TRUE;
			}
			else
			{
				m_Time += m_Step*step*m_PlayDir;
				switch (m_PlayMode)
				{
					case PM_LOOP:			
						while (m_Time >= m_EndTime)
						{
							m_bPlayStop = TRUE;
							m_Time -= (m_EndTime);							
							m_Time += (m_StartTime + 1);
							m_LoopTimes++;
						}
						while (m_Time < m_StartTime)
						{
							m_bPlayStop = TRUE;
							m_Time -= (m_StartTime);
							m_Time += (m_EndTime-1);
							m_LoopTimes++;
						}
						break;

					case PM_ONCE:
						if (m_Time >= m_EndTime)
						{
							m_bPlayStop = TRUE;
							m_Time = m_EndTime;
						}
						else if (m_Time < m_StartTime)
						{
							m_bPlayStop = TRUE;
							m_Time = m_StartTime;
						}
						break;

					case PM_BOUNCE:
						if (m_Time >= m_EndTime)
						{
							m_bPlayStop = TRUE;
							m_Time = m_EndTime-m_Step*step;
							m_PlayDir = -1.0f;
							m_LoopTimes++;
						}
						else if (m_Time < m_StartTime)
						{
							m_bPlayStop = TRUE;
							m_Time = m_StartTime;
							m_PlayDir = 1.0f;
							m_LoopTimes++;
						}
						break;
				}
			}

			for (CList<PLAYBACKEVENTDATA>::iterator Event = m_PlaybackEventList.begin(); Event != m_PlaybackEventList.end(); Event++)
			{
				if (m_PlayDir > 0.0f)
				{
					if ((*Event).EndTime < 0)
					{
						if (m_LastTime < ((*Event).TriggerTime) && m_Time >= ((*Event).TriggerTime))
						{
							if ((*Event).Func)
							{
								(*Event).Func();
							}
						}
					}
					else
					{
						if (m_Time > ((*Event).TriggerTime) && m_Time <= ((*Event).EndTime))
						{
							if ((*Event).Func)
							{
								(*Event).Func();
							}
						}
					}
				}
				else if (m_PlayDir < 0.0f)
				{
					if ((*Event).EndTime < 0)
					{
						if (m_LastTime > ((*Event).TriggerTime) && m_Time <= ((*Event).TriggerTime))
						{
							if ((*Event).Func)
							{
								(*Event).Func();
							}
						}
					}
					else
					{
						if (m_Time < ((*Event).TriggerTime) && m_Time >= ((*Event).EndTime))
						{
							if ((*Event).Func)
							{
								(*Event).Func();
							}
						}
					}
				}
			}
			m_LastTime = m_Time;
			m_bIsThisFrameStop = m_bPlayStop;
			return m_bPlayStop;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CAnimation::vRegisterScriptClassVar(ScriptSystem::CScript* const lps)
		{
			REGISTER_SCRIPT_VAR(lps, "m_StartTime",		LynxEngine::Animation::CAnimation, m_StartTime)
			REGISTER_SCRIPT_VAR(lps, "m_EndTime",		LynxEngine::Animation::CAnimation, m_EndTime)
			//REGISTER_SCRIPT_VAR(lps, "m_PlayMode",		LynxEngine::Animation::CAnimation, m_PlayMode)
			
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//  
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CAnimation::vRegisterScriptClassMethod(ScriptSystem::CScript* const lps)
		{	
			RegisterSuperClassMethod(lps);

			REGISTER_SCRIPT_METHOD(lps, "CreateKeys",			LynxEngine::Animation::CAnimation,		void,		_CreateKeys, 			(int, int, int))
			REGISTER_SCRIPT_METHOD(lps, "SetKeyStartTime",		LynxEngine::Animation::CAnimation,		void,		SetKeyStartTime, 		(int, int, float))
			REGISTER_SCRIPT_METHOD(lps, "SetKeyEndTime",		LynxEngine::Animation::CAnimation,		void,		SetKeyEndTime,			(int, int, float))
			REGISTER_SCRIPT_METHOD(lps, "SetKeyValue",			LynxEngine::Animation::CAnimation,		void,		SetKeyValue,			(int, int, int, float))
			REGISTER_SCRIPT_METHOD(lps, "SetPlayMode",			LynxEngine::Animation::CAnimation,		void,		SetPlayMode,			(int))
						
			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CAnimation::vLoad(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			CString PlayMode(_T("Loop")), Name(_T("Default"));
			float Start(0.0f), End(0.0f), Step(1.0f);

			PlayMode = file->Read(_T("Mode"), PlayMode, Root);
			Step = file->Read(_T("Step"), Step, Root);
			Start = file->Read(_T("Start"), Start, Root);
			End = file->Read(_T("End"), End, Root);
			Name = file->Read(_T("Name"), Name, Root);

			SetName(Name);
			SetStep(Step);
			SetRange(Start, End);
			SetPlayMode(TranslatePlayMode(PlayMode));

			ReadAnimationKeyFrame(file, Root, KT_SCALE, _T("Scale"), End);
			ReadAnimationKeyFrame(file, Root, KT_POSITION, _T("Position"), End);
			ReadAnimationKeyFrame(file, Root, KT_ROTATION, _T("Rotation"), End);
			ReadAnimationKeyFrame(file, Root, KT_COLOR, _T("Color"), End);
			ReadAnimationKeyFrame(file, Root, KT_UV, _T("UV"), End);

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		LYNXBOOL CAnimation::vSave(CUserProfile* file, CXMLElement* node)
		{
			CXMLElement* Root = node;
			if (!Root)
				Root = file->GetDoc()->RootElement();

			float Start(0.0f), End(0.0f);
			GetRange(&Start, &End);

			file->Write(_T("Mode"), GetEnumStr(m_PlayMode), Root);
			file->Write(_T("Step"), m_Step, Root);
			file->Write(_T("Start"), (int)Start, Root);
			file->Write(_T("End"), (int)End, Root);
			file->Write(_T("Name"), m_Name, Root);

			WriteAnimationKeyFrame(file, Root, KT_SCALE, _T("Scale"));
			WriteAnimationKeyFrame(file, Root, KT_POSITION, _T("Position"));
			WriteAnimationKeyFrame(file, Root, KT_ROTATION, _T("Rotation"));
			WriteAnimationKeyFrame(file, Root, KT_COLOR, _T("Color"));
			WriteAnimationKeyFrame(file, Root, KT_UV, _T("UV"));

			return LYNX_TRUE;
		}			
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		void CAnimation::ReadAnimationKeyFrame(CUserProfile* file, CXMLElement* node, KEYSTYPE kt, const CString& key, float LifeTime)
		{
			CXMLElement* KeyTypes = node->FirstChildElement(CString(key + _T("Keys")));
			if (KeyTypes)
			{
				static CArray<Animation::ANIMATIONKEYDATA> Keys;
				Keys.clear();

				Animation::ANIMATIONKEYDATA KeyData;
				int Time(0);
				CVector2 UV(0, 0);
				CVector3 Vector3(0, 0, 0), RotationBase(0, 0, 0), RotationFromData;
				LYNXCOLORRGBA Color;
				LYNXQUATERNION Quaternion, QuaternionBase = { 1, 0, 0, 0 }, QuaternionOfMatrix4;
				CMatrix4 Matrix4x4;

				CXMLElement* Key = KeyTypes->FirstChildElement(_T("Key"));
				while (Key)
				{					
					Time = file->Read(_T("Start"), Time, Key);
					KeyData.StartTime = (float)Time;

					switch (kt)
					{
					case KT_SCALE:
					case KT_POSITION:					
						Vector3 = file->Read(_T(key), Vector3, Key);

						KeyData.Values.resize(3, 1.0f);
						KeyData.Values[0] = Vector3.x;
						KeyData.Values[1] = Vector3.y;
						KeyData.Values[2] = Vector3.z;
						break;

					case KT_ROTATION:						
						RotationFromData = file->Read(_T(key), RotationFromData, Key);
						Vector3 = RotationFromData - RotationBase;
						RotationBase = RotationFromData;

						Matrix4x4.Identity();
						lynxEulerAnglesXYZ2Matrix(&Matrix4x4, Vector3);
						Matrix4x4.GetRotation(QuaternionOfMatrix4);
						lynxQuaternionXQuaternion(&Quaternion, &QuaternionBase, &QuaternionOfMatrix4);
						QuaternionBase = Quaternion;						

						KeyData.Values.resize(4, 0.0f);
						KeyData.Values[0] = Quaternion.w;
						KeyData.Values[1] = Quaternion.x;
						KeyData.Values[2] = Quaternion.y;
						KeyData.Values[3] = Quaternion.z;
						break;

					case KT_COLOR:						
						Color = file->Read(_T(key), Color, Key);

						KeyData.Values.resize(4, 1.0f);
						KeyData.Values[0] = Color.Red / 255.0f;
						KeyData.Values[1] = Color.Green / 255.0f;
						KeyData.Values[2] = Color.Blue / 255.0f;
						KeyData.Values[3] = Color.Alpha / 255.0f;
						break;

					case KT_UV:
						UV = file->Read(_T(key), UV, Key);

						KeyData.Values.resize(2, 1.0f);
						KeyData.Values[0] = UV.x;
						KeyData.Values[1] = UV.y;
						break;
					}
					
					Keys.push_back(KeyData);
					Key = Key->NextSiblingElement();
				}
				
				SetKeyValues(kt, Keys, LifeTime + 1);				
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		void CAnimation::WriteAnimationKeyFrame(CUserProfile* file, CXMLElement* node, KEYSTYPE kt, const CString& key)
		{		
			if (m_AnimationKeys[kt] && m_AnimationKeys[kt]->m_Keys.size()>0)
			{
				CXMLElement* KeyTypes = file->GetDoc()->NewElement(CString(key + _T("Keys")));
				node->InsertEndChild(KeyTypes);

				CArray<LynxEngine::Animation::CAnimationKey>& AnimationKeys = m_AnimationKeys[kt]->m_Keys;
				for (int i = 0; i < AnimationKeys.size(); ++i)
				{
					CXMLElement* Key = file->GetDoc()->NewElement(_T("Key"));
					KeyTypes->InsertEndChild(Key);

					int StartTime = AnimationKeys[i].GetStartTime();
					file->Write(_T("Start"), StartTime, Key);

					CVector3 Rotation;
					CMatrix4 Matrix4x4;
					LYNXQUATERNION Quaternion;
					const CArray<CReal>& Values = AnimationKeys[i].GetValues();									
					switch (kt)
					{
					case KT_SCALE:
					case KT_POSITION:
						file->Write(key, CVector3(Values[0], Values[1], Values[2]), Key);
						break;
					
					case KT_ROTATION:
						Quaternion.w = Values[0];
						Quaternion.x = Values[1];
						Quaternion.y = Values[2];
						Quaternion.z = Values[3];
						lynxQuaternion2Matrix(Matrix4x4, &Quaternion);
						lynxMatrix2EulerAnglesXYZ(Rotation, Matrix4x4);
						
						file->Write(key, Rotation, Key);
						break;

					case KT_COLOR:
						LYNXCOLORRGBA Color;
						Color.Red = Values[0] * 255.0f;
						Color.Green = Values[1] * 255.0f;
						Color.Blue = Values[2] * 255.0f;
						Color.Alpha = Values[3] * 255.0f;
						
						file->Write(key, Color, Key);
						break;

					case KT_UV:
						CVector2 UV;
						UV.x = Values[0];
						UV.y = Values[1];
						
						file->Write(key, UV, Key);
						break;
					}
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------	
		void CAnimation::AddPlaybackEvent(Math::CReal& t_time, Math::CReal& e_time, std::function<void(void)> callback)
		{
			PLAYBACKEVENTDATA Data;

			Data.TriggerTime = t_time;
			Data.EndTime = e_time;
			Data.Func = callback;

			m_PlaybackEventList.push_back(Data);
		}
	}
}	