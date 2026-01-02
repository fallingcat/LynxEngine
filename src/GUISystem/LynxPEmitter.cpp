#include <LynxEngine_PCH.h>
#include <LynxEngine.h>

#include <GUISystem/LynxPEmitter.h>

#define SAFE_RELEASE_CONTAINER(x)	if (x) { m_lpEngine->DestroyContainer(x); (x) = NULL; }

namespace LynxEngine
{
	namespace GUISystem
	{
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CPEmitterControlCreationDesc::Default()
		{
			m_PEDesc.Default();
			m_CreateMethod = CM_FROM_LAYOUT;
			m_Delay = CVector2(0.0f, 0.0f);
			m_PEFile = NULL;
			m_lpAnimation = NULL;
			m_bShow = LYNX_TRUE;
			m_bActive = LYNX_TRUE;
			m_Size = CVector2(0.0f, 0.0f);
			m_RenderOrder = 0;
			m_NumOfEmitter = 1;
			m_bUseSpawnArea = LYNX_FALSE;
		}

		// IMPLEMENT_CLASSSCRIPT(CPEmitter, CControl)
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		CPEmitter::CPEmitter(GUISystem::CGUISystem* lpsys)
			: GUISystem::CControl(lpsys),
			m_bUseSpawnArea(LYNX_FALSE)
		{
			m_lpEngine = glpKernel->GetlpEngine();
			m_ContainerList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		CPEmitter::~CPEmitter()
		{
			for (CPEMITTERARRAY_ITER iter = m_ContainerList.begin(); iter != m_ContainerList.end(); ++iter)
			{
				SAFE_RELEASE_CONTAINER(*iter);
			}
			m_ContainerList.clear();
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CPEmitter::vCreateFromLayout(CUserProfile* file, CXMLElement* node, GUISystem::CContext* context)
		{
			LYNXRECT ControlRect = { 0, 0, 0, 0 }, SpawnArea = { 0, 0, 640, 1136 };
			LYNXBOOL bShow(LYNX_TRUE), bActive(LYNX_TRUE), bUseSpawnArea(LYNX_FALSE), bUseFile(LYNX_FALSE);
			CString Class, Name, Lookup, Value, Group, File, ID;
			CVector2 Size, Delay;
			CVector3 Pos;
			int GroupID(-1), RenderOrder(0), NumOfEmitter(1), EmitInterval(30);

			Class = file->Read(_T("Class"), Class, node);
			ID = file->Read(_T("ID"), ID, node);
			Lookup = file->Read(_T("LookupID"), Lookup, node);
			if (Lookup.empty())
			{
				Name = file->Read(_T("Name"), Name, node);
			}
			else
			{
				Name = glpKernel->GetlpGame()->GetText(Lookup);
			}
			Group = file->Read(_T("Group"), Group, node);
			GroupID = file->Read(_T("GroupID"), GroupID, node);
			Size = file->Read(_T("Size"), Size, node);
			ControlRect = file->Read(_T("Rect"), ControlRect, node);
			bActive = file->Read(_T("Active"), bActive, node);
			File = file->Read(_T("File"), File, node);
			NumOfEmitter = file->Read(_T("NumOfEmitter"), NumOfEmitter, node);
			Delay = file->Read(_T("Delay"), Delay, node);
			bUseSpawnArea = file->Read(_T("UseSpawnArea"), bUseSpawnArea, node);
			SpawnArea = file->Read(_T("SpawnArea"), SpawnArea, node);
			bShow = file->Read(_T("Show"), bShow, node);
			RenderOrder = file->Read(_T("RenderOrder"), RenderOrder, node);

			CUserProfile PEFile(File);
			bUseFile = PEFile.Load();

			CPEmitterControlCreationDesc Desc;
			Desc.Default();
			Desc.m_CreateMethod = (bUseFile) ? CPEmitterControlCreationDesc::CM_FROM_FILE : CPEmitterControlCreationDesc::CM_FROM_LAYOUT;
			Desc.m_PEFile = (bUseFile) ? &PEFile : file;
			Desc.m_bShow = bShow;
			Desc.m_bActive = bActive;
			Desc.m_Rect = ControlRect;
			Desc.m_Size = Size;
			Desc.m_RenderOrder = RenderOrder;
			Desc.m_NumOfEmitter = NumOfEmitter;
			Desc.m_Delay = Delay;
			Desc.m_bUseSpawnArea = bUseSpawnArea;
			Desc.m_SpawnArea = SpawnArea;

			vCreate(&Desc);
			
			SetName(Name);			
			SetID(context->GetDefinedControlID(ID));
			vInit();
			if (context->GetParent())
				SetParent(context->GetParent(), NULL);

			GUISystem::CScrollGroup* ScrollGroup = context->FindScrollGroup(Group);
			if (ScrollGroup)
				ScrollGroup->AddControl(this);
			if (GroupID >= 0)
			{
				SetGroupID(GroupID);
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		LYNXBOOL CPEmitter::vCreate(CCreationDesc* desc)
		{			
			const CString& OriginPath = m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->GetTexturePath();
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(_T(""));

			const Math::CVector2& ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);

			CPEmitterControlCreationDesc* Desc = (CPEmitterControlCreationDesc*)desc;
			int NumEmitter = Desc->m_NumOfEmitter;
			const CVector2& Delay = Desc->m_Delay;
			int EmitInterval = LYNX_ABS(Delay.y - Delay.x);
			CPEmitterControlCreationDesc::CREATEMETHOD Method = Desc->m_CreateMethod;
			LynxEngine::CPEmitterCreationDesc& PEDesc = Desc->m_PEDesc;
			CUserProfile* PEFile = Desc->m_PEFile;

			for (int i = 0; i < NumEmitter; ++i)
			{
				CPEmitterContainer* lpPE = static_cast<CPEmitterContainer*>(m_lpEngine->CreateContainer(CContainer::PEMITTER));
				LYNX_ASSERT(lpPE);

				switch (Method)
				{
				case CPEmitterControlCreationDesc::CM_FROM_FILE:
					lpPE->vLoad(PEFile, PEFile->GetRootElement());
					break;
				case CPEmitterControlCreationDesc::CM_FROM_LAYOUT:
					lpPE->vLoad(PEFile, PEFile->GetRootElement()->FirstChildElement(_T("ParticleEmitter")));
					break;
				case CPEmitterControlCreationDesc::CM_RUNTIME:
					lpPE->vCreate(&PEDesc);
					break;
				}

				lpPE->m_EmitInterval = (EmitInterval > 0) ? (Delay.x + lynxRand() % EmitInterval) / 1000.0f : EmitInterval / 1000.0f;
				lpPE->Emit(LYNX_FALSE);
				lpPE->Reset();

				m_ContainerList.push_back(lpPE);
			}

			LYNXBOOL bActive = Desc->m_bActive;
			LYNXBOOL bShow = Desc->m_bShow;
			LYNXRECT ControlRect = Desc->m_Rect;
			CVector2 Size = Desc->m_Size;
			int RenderOrder = Desc->m_RenderOrder;
			if (ControlRect.x2 == -1)
				ControlRect.x2 = ControlRect.x1 + Size.x;
			if (ControlRect.y2 == -1)
				ControlRect.y2 = ControlRect.y1 + Size.y;

			ControlRect.x1 *= ScaleFactor.x;
			ControlRect.y1 *= ScaleFactor.y;
			ControlRect.x2 *= ScaleFactor.x;
			ControlRect.y2 *= ScaleFactor.y;

			Activate(bActive);
			SetInitActiveState(bActive);
			Show(bShow);
			SetInitShowState(bShow);
			SetAlignmentRect(ControlRect);
			SetRenderOrder(RenderOrder);			

			m_bUseSpawnArea = Desc->m_bUseSpawnArea;
			m_SpawnArea = Desc->m_SpawnArea;

			// recover texture path
			m_lpEngine->GetlpGraphicsSystem()->GetlpRenderer()->SetTexturePath(OriginPath);			

			return LYNX_TRUE;
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CPEmitter::vLoop(float step)
		{
			if (!m_bShow)
				return;
			
			CControl::vLoop(step);

			for (CPEMITTERARRAY_ITER iter = m_ContainerList.begin(); iter != m_ContainerList.end(); ++iter)
			{
				LYNXBOOL bStop = (*iter)->vPlay(step);
				if (bStop && m_bUseSpawnArea)
				{
					int w = LYNX_ABS(m_SpawnArea.x2 - m_SpawnArea.x1);
					int h = LYNX_ABS(m_SpawnArea.y2 - m_SpawnArea.y1);
					const Math::CVector2& ScaleFactor = (m_lpEngine->GetlpGUISystem()) ? m_lpEngine->GetlpGUISystem()->GetScaleFactor() : Math::CVector2(1.0f, 1.0f);

					(*iter)->Reset();
					(*iter)->Emit(LYNX_TRUE);
					(*iter)->vSetPosition(
						CVector3((m_SpawnArea.x1 + lynxRand() % w) * ScaleFactor.x, (m_SpawnArea.y1 + lynxRand() % h) * ScaleFactor.y, 0.0f)
						);
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CPEmitter::vRender()
		{
			if (!m_bShow)
				return;
				
			for (CPEMITTERARRAY_ITER iter = m_ContainerList.begin(); iter != m_ContainerList.end(); ++iter)
			{
				(*iter)->vRender();
			}			
		}
		//-------------------------------------------------------------------------------------------------------
		//
		//
		//-------------------------------------------------------------------------------------------------------
		void CPEmitter::Reset()
		{
			for (CPEMITTERARRAY_ITER iter = m_ContainerList.begin(); iter != m_ContainerList.end(); ++iter)
			{
				(*iter)->Reset();
				(*iter)->Emit(LYNX_TRUE);
			}
		}
	}
}