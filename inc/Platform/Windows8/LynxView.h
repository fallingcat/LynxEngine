#ifndef __LYNXVIEW_H__
#define __LYNXVIEW_H__

#ifdef __WINAPP__
	
	ref class LynxView : public Windows::ApplicationModel::Core::IFrameworkView
	{	
	public:    
		// IFrameworkView Methods
		void								Initialize(_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		void								SetWindow(_In_ Windows::UI::Core::CoreWindow^ window);
		void								Load(_In_ Platform::String^ entryPoint);
		void								Run();
		void								Uninitialize();

		// Event Handlers
		void								OnWindowSizeChanged(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void								OnCharacterReceived(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::CharacterReceivedEventArgs^ args;
		void								OnKeyDown(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::KeyEventArgs^ args);
		void								OnKeyUp(_In_ Windows::UI::Core::CoreWindow^ sender,	_In_ Windows::UI::Core::KeyEventArgs^ args);
		void								OnPointerPressed(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::PointerEventArgs^ args);
		void								OnPointerReleased(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::PointerEventArgs^ args);
		void								OnPointerMoved(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::PointerEventArgs^ args);
		void								OnWindowActivated(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::WindowActivatedEventArgs^ args);
		void								OnLogicalDpiChanged(_In_ Platform::Object^ sender);
		void								OnActivated(_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, _In_ Windows::ApplicationModel::Activation::IActivatedEventArgs^ args;
	};

#endif

#endif
