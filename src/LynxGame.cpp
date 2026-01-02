#include <LynxEngine_PCH.h>
#include <LynxGame.h>

namespace LynxEngine
{
	IMPLEMENT_ENGINE_CONTROL(PEmitter)

	LynxEngine::CAnsiString			CGame::m_GameName;
	LynxEngine::CAnsiString			CGame::m_DeviceProfilePassword;
	LynxEngine::CAnsiString			CGame::m_GameProfilePassword;

	CMap<CString, CString>			CGame::m_LanguageCodeTable;		
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------
	CGame::CGame(LynxEngine::GameFramework::CKernel* lpk)
	: CObj(lpk->GetlpEngine())
	{
		m_lpKernel = lpk;
		m_Version = _T("Ver 0.1");
		m_CurrentLanguage = _T("eng");
		m_bFirstTimeInstallation = LYNX_TRUE;
		m_LocalizationResourceReader = NULL;

		m_GameName = "LynxGame";
		m_GameProfile = NULL;
		m_DeviceProfile = NULL;
		m_PlayerProfile = NULL;
		
		m_LoadingIcon = NULL;
		m_bShowLoadingIcon = LYNX_FALSE;		

		// ISO 639-1 language code to ISO 639-2/T
		m_LanguageCodeTable[_T("en")]		  = CString(_T("eng"));
		m_LanguageCodeTable[_T("ja")]		  = CString(_T("jpn"));
		m_LanguageCodeTable[_T("zh")]		  = CString(_T("zho_Hant"));
		m_LanguageCodeTable[_T("ru")]		  = CString(_T("rus"));
		m_LanguageCodeTable[_T("fr")]		  = CString(_T("fre"));
		m_LanguageCodeTable[_T("de")]		  = CString(_T("ger"));
		m_LanguageCodeTable[_T("es")]		  = CString(_T("spa"));
		m_LanguageCodeTable[_T("it")]		  = CString(_T("ita"));

		m_LanguageCodeTable[_T("en-gb")]      = CString(_T("eng"));
		m_LanguageCodeTable[_T("en-us")]      = CString(_T("eng"));
		m_LanguageCodeTable[_T("en-au")]      = CString(_T("eng"));				
		m_LanguageCodeTable[_T("zh-Hans")]    = CString(_T("zho_Hans"));
		m_LanguageCodeTable[_T("zh-CN")]	  = CString(_T("zho_Hans"));
		m_LanguageCodeTable[_T("zh-Hant")]    = CString(_T("zho_Hant"));
		m_LanguageCodeTable[_T("zh-TW")]	  = CString(_T("zho_Hant"));
		m_LanguageCodeTable[_T("zh-HK")]	  = CString(_T("zho_Hant"));				
		// Android
		m_LanguageCodeTable[_T("en_GB")]      = CString(_T("eng"));
		m_LanguageCodeTable[_T("en_US")]      = CString(_T("eng"));
		m_LanguageCodeTable[_T("en_CA")]      = CString(_T("eng"));
		m_LanguageCodeTable[_T("fr_FR")]      = CString(_T("fre"));
		m_LanguageCodeTable[_T("fr_CA")]      = CString(_T("fre"));
		m_LanguageCodeTable[_T("de_DE")]      = CString(_T("ger"));
		m_LanguageCodeTable[_T("it_IT")]      = CString(_T("ita"));
		m_LanguageCodeTable[_T("ja_JP")]      = CString(_T("jpn"));
		m_LanguageCodeTable[_T("zh_CN")]	  = CString(_T("zho_Hans"));
		m_LanguageCodeTable[_T("zh_HK")]      = CString(_T("zho_Hant"));
		m_LanguageCodeTable[_T("zh_TW")]      = CString(_T("zho_Hant"));

		lynxDebugOutputPrintf("CGame Contructor Called");
		
		REGISTER_ENGINE_CONTROL(PEmitter)
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------
	CGame::~CGame()
	{	
		m_LanguageCodeTable.clear();		

		if (m_LocalizationResourceReader)
		{
			LYNXDEL(m_LocalizationResourceReader);
		}

		if (m_PlayerProfile)
		{
			LYNXDEL(m_PlayerProfile);
			m_PlayerProfile = NULL;
		}

		if (m_GameProfile)
		{
			LYNXDEL(m_GameProfile);
			m_GameProfile = NULL;
		}

		if (m_DeviceProfile)
		{
			LYNXDEL(m_DeviceProfile);
			m_DeviceProfile = NULL;
		}

		if (m_LoadingIcon)
		{
			m_lpEngine->DestroyContainer(m_LoadingIcon);
			m_LoadingIcon = NULL;
		}

		m_LanguageTextTable.clear();

		m_Output.vClose();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :
	//-------------------------------------------------------------------------------------------------------
	const CString CGame::GetLanguageCodeFromISO_639_1(const CString& code) 
	{ 
		if (m_LanguageCodeTable.is_exist(code))
			return m_LanguageCodeTable[code]; 
		else
		{
			for (CMap<CString, CString>::CIterator Pos = m_LanguageCodeTable.begin(); Pos != m_LanguageCodeTable.end(); Pos++)
			{
				if (code.find((*Pos).first) != CString::npos)
					return (*Pos).second;
			}
		}	
		return CString(_T("eng"));
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :
	//-------------------------------------------------------------------------------------------------------
	void CGame::vSetCurrentLanguage(const CString& lang)	
	{ 
		if (LynxEngine::find(m_SupportedLanguage.begin(), m_SupportedLanguage.end(), lang) != m_SupportedLanguage.end())
		{
			m_CurrentLanguage = lang;
		}
		else
		{
			m_CurrentLanguage = m_SupportedLanguage[0];
		}

		if (m_CurrentLanguage == CString(_T("zho_Hant")) || m_CurrentLanguage == CString(_T("zho_Hans")) || m_CurrentLanguage == CString(_T("jpn")))
		{
			m_lpEngine->GetSystemFontString()->SpaceChecking(LYNX_FALSE);
		}
		else
		{
			m_lpEngine->GetSystemFontString()->SpaceChecking(LYNX_TRUE);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :
	//-------------------------------------------------------------------------------------------------------
	void CGame::SetCurrentLanguageFromISO_639_1(const CString& lang)
	{
		/*
		CPlatformFileStream File;
		CString ResourceFileName = CString(_T("../localization/")) + m_LanguageCodeTable[lang] + CString(_T("/resource.ini"));
		if (File.vOpen(ResourceFileName, CStream::Text))
		{
			File.vClose();
			m_CurrentLanguage = m_LanguageCodeTable[lang];
		}
		else
		{
			if (m_LanguageCodeTable[lang] == CString("zho_Hans"))
				m_CurrentLanguage = _T("zho_Hant");
			else
				m_CurrentLanguage = _T("eng");
		} 
		*/
				
		if (LynxEngine::find(m_SupportedLanguage.begin(), m_SupportedLanguage.end(), m_LanguageCodeTable[lang]) != m_SupportedLanguage.end())
		{
			m_CurrentLanguage = m_LanguageCodeTable[lang];
		}
		else
		{
			m_CurrentLanguage = m_SupportedLanguage[0];
		}

		if (m_CurrentLanguage == CString(_T("zho_Hant")) && m_CurrentLanguage == CString(_T("zho_Hans")) && m_CurrentLanguage == CString(_T("jpn")))
		{
			m_lpEngine->GetSystemFontString()->SpaceChecking(LYNX_FALSE);		
		}    
		else
		{
			m_lpEngine->GetSystemFontString()->SpaceChecking(LYNX_TRUE);		
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CGame::vCreate()
	{
		#if !SHIPPING		
			m_Output.vOpen(_T("../bin/missing_text.log"));
		#endif

		#ifdef __DESKTOP__
			m_Title = lynxOSGetTitle(m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetWindow());
		#endif	

		vResume();
		/*
		LYNXRECT Rect;
		LynxEngine::CSpriteCreationDesc SpriteDesc;
	
		Math::CVector2 ScaleFactor = m_lpEngine->GetlpGUISystem()->GetScaleFactor();
		Rect.x1 = 0;
		Rect.x2 = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetWidth();
		Rect.y1 = 0;
		Rect.y2 = m_lpEngine->GetlpGraphicsSystem()->GetSystemBackBuffer()->vGetHeight(); 		
    
		m_LoadingIcon = Container_Cast<CSpriteContainer*>(m_lpEngine->CreateContainer(CContainer::SPRITE));
		SpriteDesc.Default();
		SpriteDesc.m_TextureDesc.MipMapLevel = 1;
		SpriteDesc.m_TextureDesc.bSRGB = LYNX_FALSE;
		SpriteDesc.m_TextureDesc.Type = GraphicsSystem::CRenderer::TT_2D_TEXTURE;
		SpriteDesc.m_lpStream = m_lpEngine->GetlpFileSystem()->GetPackage()->GetStream();
		SpriteDesc.m_bUseBufferObject = LYNX_TRUE;		
		SpriteDesc.m_bInvertTexCoordV = LYNX_FALSE;
		SpriteDesc.m_Width = 64 * ScaleFactor.x * 2;
		SpriteDesc.m_Height = 64 * ScaleFactor.y * 2;
		//SpriteDesc.m_Number = 4;
		//SpriteDesc.m_Length = 64.0f;
		SpriteDesc.m_FileName = _T("../texture/ui/loading.png");
		//SpriteDesc.m_FileExt = _T("png");
		m_LoadingIcon->vCreate(&SpriteDesc);
		m_LoadingIcon->SetAlignmentType(CSpriteContainer::H_ALIGN_CENTER|CSpriteContainer::V_ALIGN_CENTER);
		m_LoadingIcon->SetOriginType(CSpriteContainer::OT_CENTER);
		m_LoadingIcon->SetAlignmentRect(&Rect);		
		*/
		
		return LYNX_TRUE;	
	}
    //-------------------------------------------------------------------------------------------------------
	//
	//  :
	//-------------------------------------------------------------------------------------------------------
	void CGame::vPause()
    {
        m_lpKernel->vPause();
		m_lpKernel->vPauseRendering();
    }
    //-------------------------------------------------------------------------------------------------------
	//
	//  :
	//-------------------------------------------------------------------------------------------------------
	void CGame::vResume()
    {
        m_lpKernel->vResume();
		m_lpKernel->vResumeRendering();
    }
	//-------------------------------------------------------------------------------------------------------
	//
	//  :
	//-------------------------------------------------------------------------------------------------------
	void CGame::vTerminate()
    {
        m_lpKernel->vTerminate();
    }
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	void CGame::CreateDeviceProfile(const CString& filename, const CAnsiString& password)
	{
		m_DeviceProfile = LYNXNEW CUserProfile(filename);
		m_DeviceProfile->SetPassword(password);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	void CGame::vLoadDeviceProfile()
	{
		CPlatformFileStream File;
		CString UDID = _T("");
        CAnsiString Token = "";

		if (File.vOpen(m_DeviceProfile->GetFilename()))
		{
			if (m_DeviceProfile->Load())
			{				
				UDID = m_DeviceProfile->Read("UDID",  _T(""));
                Token = CAnsiString(m_DeviceProfile->Read("Token", _T("")));
			}
			if (UDID.empty())
			{
				m_lpKernel->vGenerateUDID(LYNX_TRUE);				
				vSaveDeviceProfile();
			}
			else
			{
				m_lpKernel->SetUDIDString(UDID);
			}
            
            if (!Token.empty())
			{
				m_lpKernel->SetDeviceToken(Token);
			}
		}
		else
		{
			m_lpKernel->vGenerateUDID(LYNX_TRUE);				
			vSaveDeviceProfile();
		}					
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	void CGame::vSaveDeviceProfile()
	{		
		m_DeviceProfile->Write("UDID", m_lpKernel->GetUDIDString());
        m_DeviceProfile->Write("Token", m_lpKernel->GetDeviceToken());
		m_DeviceProfile->Write("Type", LynxEngine::GetEnumStr(m_lpKernel->GetDeviceType()));
		m_DeviceProfile->Write("CPUClass", LynxEngine::GetEnumStr(m_lpKernel->GetCPUClass()));
		m_DeviceProfile->Write("GPUClass", LynxEngine::GetEnumStr(m_lpKernel->GetGPUClass()));
		m_DeviceProfile->Write("MemoryClass", LynxEngine::GetEnumStr(m_lpKernel->GetMemoryClass()));
		m_DeviceProfile->Save();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	void CGame::CreateGameProfile(const CString& filename, const CAnsiString& password)
	{
		m_GameProfile = LYNXNEW CGameProfile(filename);
		m_GameProfile->SetPassword(password);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	void CGame::CreatePlayerProfile(const CString& filename, const CAnsiString& password)
	{
		m_PlayerProfile = LYNXNEW CPlayerProfile(filename);
		m_PlayerProfile->SetPassword(password);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	CString CGame::GetText(const CString& lang, const CString& target)
	{
		CString ResourceFileName;
		unsigned int Decoder;
	    
		if (!m_LocalizationResourceReader)
		{
			CPlatformFileStream File;

			ResourceFileName = CString(_T("../localization/")) + lang + CString(_T("/resource.ini"));
			if (lang == CString(_T("eng")))
			{
				Decoder = LynxEngine::CStream::ANSI;
			}
			else if (lang == CString(_T("zho_Hant")))
			{
				Decoder = LynxEngine::CStream::UTF8;
			}
			else if (lang == CString(_T("jpn")))
			{			
				Decoder = LynxEngine::CStream::UTF8;
			}
			else if (lang == CString(_T("fre")))
			{
				Decoder = LynxEngine::CStream::UTF8;
			}
			else
			{
				Decoder = LynxEngine::CStream::UTF8;
			}
	        
			if (!File.vOpen(ResourceFileName, CStream::Text|CStream::Read))
			{
				ResourceFileName = _T("../localization/eng/resource.ini");        
				Decoder = LynxEngine::CStream::ANSI;
			}
			File.vClose();
			m_LocalizationResourceReader = LYNXNEW CIniReader(ResourceFileName, Decoder);
		}
	    
		if (m_LocalizationResourceReader)
		{
			return  m_LocalizationResourceReader->ReadString(_T("Text"), target.c_str(), _T(""));
		}
		else
		{
			return  CString(_T("[Error] No resource file found! "));
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
    CString CGame::GetText(const CString& target)
	{
		return GetText(m_CurrentLanguage, target);
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CGame::FindText(const CString& section, const CString& id, const CString& key)
	{
		if (m_LanguageTextTable.isObject())
		{
			if (m_LanguageTextTable[section].isObject())
			{
				if (m_LanguageTextTable[section][id].isObject())
				{
					if (m_LanguageTextTable[section][id][key].isString())
					{
						return LYNX_TRUE;
					}
				}
			}
		}

		return LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	CString	CGame::GetText(const CString& section, const CString& id, const CString& key)
	{
		if (m_LanguageTextTable.isObject())
		{
			if (m_LanguageTextTable[section].isObject())
			{
				if (m_LanguageTextTable[section][id].isObject())
				{
					if (m_LanguageTextTable[section][id][key].isString())
					{
						return m_LanguageTextTable[section][id][key].JSON_VALUE_AS_CSTRING();					
					}
				}
			}
		}

	#if !SHIPPING		
		LYNXCHAR Buffer[256];
		lynxSprintf(Buffer, _T("Missing Text : Section:%s, ID:%s, Key:%s\r\n"), section.c_str(), id.c_str(), key.c_str());
		m_Output.vAppend(CString(Buffer));
	#endif

#if !SHIPPING
		return CString(NO_TEXT_FOUND);
#else
		return m_LanguageTextTable["LT_SYSTEM_MSG"]["DEFAULT_TEXT_NOT_FOUND"]["desc"].JSON_VALUE_AS_CSTRING();
#endif
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	int CGame::GetNumText(const CString& section)
	{
		if (m_LanguageTextTable.isObject())
		{
			if (m_LanguageTextTable[section].isObject())
			{
				return m_LanguageTextTable[section].size();
			}
		}
		return 0;		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CGame::LoadSound(SoundSystem::CSoundPtr& s, const CString& id)
	{
		if (m_SoundTable.isObject())
		{
			if (m_SoundTable[id].isObject())
			{
				if (m_SoundTable[id]["filename"].isString())
				{
					CString Filename = m_SoundTable[id]["filename"].JSON_VALUE_AS_CSTRING();
					int Loop = m_SoundTable[id]["loop_times"].asInt();
					float Volume = m_SoundTable[id]["volume"].asDouble();
					LYNXBOOL bShared = m_SoundTable[id]["share"].asBool();

					m_lpEngine->GetlpSoundSystem()->LoadSound(s, id, Filename, bShared);
					if (s)
					{
						s->SetVolume(Volume);
						s->SetLoops(Loop);

						return LYNX_TRUE;
					}										
				}				
			}
		}
		return LYNX_FALSE;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//  :   
	//-------------------------------------------------------------------------------------------------------	
	LYNXBOOL CGame::LoadMusic(SoundSystem::CMusicPtr& s, const CString& id)
	{
		if (m_SoundTable.isObject())
		{
			if (m_SoundTable[id].isObject())
			{
				if (m_SoundTable[id]["filename"].isString())
				{
					CString Filename = m_SoundTable[id]["filename"].JSON_VALUE_AS_CSTRING();
					int Loop = m_SoundTable[id]["loop_times"].asInt();
					float Volume = m_SoundTable[id]["volume"].asDouble();
					LYNXBOOL bStreaming = m_SoundTable[id]["share"].asBool();

					m_lpEngine->GetlpSoundSystem()->LoadMusic(s, id, Filename, bStreaming);
					if (s)
					{
						s->SetVolume(Volume);
						s->SetLoops(Loop);

						return LYNX_TRUE;
					}										
				}				
			}
		}
		return LYNX_FALSE;
	}
}