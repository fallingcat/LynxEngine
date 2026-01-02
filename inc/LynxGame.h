#ifndef __LYNXGAME_H__
#define __LYNXGAME_H__

#include <LynxEngineClasses.h>
#include <LynxObj.h>

#define DECLARE_ENGINE_CONTROL(c)				static GUISystem::CControl*	Create##c(GUISystem::CGUISystem* s);
#define REGISTER_ENGINE_CONTROL(c)				glpKernel->GetlpEngine()->GetlpGUISystem()->RegisterControl(LynxEngine::CString("C"#c), LynxEngine::CGame::Create##c);
#define IMPLEMENT_ENGINE_CONTROL(c) \
	GUISystem::CControl* LynxEngine::CGame::Create##c(GUISystem::CGUISystem* s)\
																{\
		return LYNXNEW LynxEngine::GUISystem::C##c(s);\
																}

namespace LynxEngine
{
	class CGame : public CObj
	{
	public:				
		#ifdef __DESKTOP__
			CString								m_Title;
		#endif		

			typedef struct {
				LYNXBOOL						bDone;
				LYNXTHREADHANDLE				Thread;
				LYNXMUTEX						Mutex;
				CSpriteContainer*				LoadingIcon;
			}LOADINGTHREAD;			
	protected:		
		GameFramework::CKernel*					m_lpKernel;		

		CString                                 m_Version;
		LYNXBOOL                                m_bFirstTimeInstallation;

		CFileOutputDevice						m_Output;

		static CMap<CString, CString>			m_LanguageCodeTable;
		CList<CString>							m_SupportedLanguage;
		CString									m_CurrentLanguage;
		CIniReader*								m_LocalizationResourceReader;		

		static LynxEngine::CAnsiString			m_GameName;
		static LynxEngine::CAnsiString			m_DeviceProfilePassword;
		static LynxEngine::CAnsiString			m_GameProfilePassword;
		CUserProfile*							m_DeviceProfile;		
		CPlayerProfile*							m_PlayerProfile;
		CGameProfile*							m_GameProfile;
		CSpriteContainer*						m_LoadingIcon;
		LYNXBOOL								m_bShowLoadingIcon;		
		CJsonValue								m_LanguageTextTable;
		CJsonValue								m_SoundTable;
		CArray<CString>							m_ProfanityTextTable;
		CArray<CString>							m_RandNamePart1Table;
		CArray<CString>							m_RandNamePart2Table;
	public:
		CGame(GameFramework::CKernel* lpk);	
		~CGame();			
		
		virtual LYNXBOOL						vCreate();
        virtual void                            vPause();
        virtual void                            vResume();
		virtual void                            vTerminate();

		LYNXFORCEINLINE void					SetVersion(const CString& ver)  {m_Version = ver; }
		LYNXFORCEINLINE const CString&			GetVersion() const {return m_Version; }
		
		LYNXFORCEINLINE void					AddSupportLanguage(const CString lang) { if (LynxEngine::find(m_SupportedLanguage.begin(), m_SupportedLanguage.end(), lang) == m_SupportedLanguage.end()) m_SupportedLanguage.push_back(lang); }
		LYNXFORCEINLINE CList<CString>			GetSupportLanguage() { return  m_SupportedLanguage; }

		LYNXFORCEINLINE void					SetFirstTimeInstallation(const LYNXBOOL b)  {m_bFirstTimeInstallation = b; }
		LYNXFORCEINLINE const LYNXBOOL			IsFirstTimeInstallation() const {return m_bFirstTimeInstallation; }		

		virtual void							vSetCurrentLanguage(const CString& lang);
		LYNXFORCEINLINE const CString&			GetCurrentLanguage() const {return m_CurrentLanguage; }
		void                                    SetCurrentLanguageFromISO_639_1(const CString& lang);
		static const CString					GetLanguageCodeFromISO_639_1(const CString& code);
		CString									GetText(const CString& target);
		CString									GetText(const CString& lang, const CString& target);
		CString									GetText(const CString& section, const CString& id, const CString& key);
		LYNXBOOL								FindText(const CString& section, const CString& id, const CString& key);
		int										GetNumText(const CString& section);		
		LYNXFORCEINLINE const CArray<CString>&	GetProfanityTable() { return m_ProfanityTextTable; }

		LYNXBOOL								LoadMusic(SoundSystem::CMusicPtr& s, const CString& id);
		LYNXBOOL								LoadSound(SoundSystem::CSoundPtr& s, const CString& id);

		void									CreateDeviceProfile(const CString& filename, const CAnsiString& password);
		LYNXFORCEINLINE CUserProfile*			GetDeviceProfile() const {return m_DeviceProfile; }		
		virtual void							vLoadDeviceProfile();
		virtual void							vSaveDeviceProfile();

		void									CreateGameProfile(const CString& filename, const CAnsiString& password);
		LYNXFORCEINLINE CGameProfile*			GetGameProfile() const {return m_GameProfile; }		

		void									CreatePlayerProfile(const CString& filename, const CAnsiString& password);
		LYNXFORCEINLINE CPlayerProfile*			GetPlayerProfile() const {return m_PlayerProfile; }							

		DECLARE_ENGINE_CONTROL(PEmitter)
	};	
}

#endif