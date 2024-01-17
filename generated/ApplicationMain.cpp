// ◦ Playrix ◦

#include <EngineConfig.h>
#include <Core/Main/Main.h>

#if ENGINE_OS_ANDROID
#include <jni.h>
#elif ENGINE_OS_WINDOWS_RUNTIME
#include <uwp/UwpThreading.h>
#elif ENGINE_OS_EMSCRIPTEN
#include <emscripten.h>
#endif

#if ENGINE_CONFIG_ENABLE_ADDRESS_SANITIZER
#include <sanitizer/asan_interface.h>
#include <filesystem>
static void ErrorReportCallbackFromAsan(const char* report) {
	FILE* f = fopen("asan.log", "a+t");
	if (f) {
		fprintf(f, "%s", report);
		fclose(f);
	}
}
#endif

namespace Core::Details { int MainImpl(const CommandLineArguments&); }

#if ENGINE_OS_WINDOWS_DESKTOP

#include <Platform/Windows/WindowsDeclarations.h>

int __stdcall WinMain(HINSTANCE, HINSTANCE, char* /* lpCmdLine */, int /* nCmdShow */) {
#if ENGINE_CONFIG_ENABLE_ADDRESS_SANITIZER
	std::error_code ec;
	std::filesystem::remove(std::filesystem::path("asan.log"), ec);
	__asan_set_error_report_callback(ErrorReportCallbackFromAsan);
#endif
	return Core::Details::MainImpl(Core::CreateCommandLineArguments(__argc, __argv));
}

#elif ENGINE_OS_WINDOWS_RUNTIME

namespace Core { extern ::Windows::UI::Xaml::Application^ MakeUwpApplication(); }

int __cdecl main(::Platform::Array<::Platform::String^>^ /* args */) {
	using namespace Core;
	::Windows::UI::Xaml::Application::Start(ref new ::Windows::UI::Xaml::ApplicationInitializationCallback(
		[](::Windows::UI::Xaml::ApplicationInitializationCallbackParams^ p) {
			(void)p; // Unused parameter
			UwpThreading::RememberUiThread(ThisThread::GetId());
			auto app = Core::MakeUwpApplication();
			UwpThreading::StartPlayrixThread([]() {
				Core::Details::MainImpl(Core::CreateCommandLineArguments(__argc, __argv));
			});
		})
	);
}

#elif ENGINE_OS_IOS

int main(const int argc, char* const argv[]) {
	return Core::Details::MainImpl(Core::CreateCommandLineArguments(argc, argv));
}

#elif ENGINE_OS_MACOS

namespace Core { extern void MakeApplication(); }

int main(const int argc, char* const argv[]) {
	Core::MakeApplication();
	return Core::Details::MainImpl(Core::CreateCommandLineArguments(argc, argv));
}

#elif ENGINE_OS_ANDROID

namespace Core::Android {
jint JNI_OnLoadInternal(JavaVM* vm, void* reserved);
} // namespace Android

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	const jint result = Core::Android::JNI_OnLoadInternal(vm, reserved);
	if (result != JNI_ERR) {
		const Core::CommandLineArguments commandLineArguments = { "" };
		Core::Details::MainImpl(commandLineArguments);
	}
	return result;
}

#elif ENGINE_OS_LINUX

int main(const int argc, char* const argv[], const char* const /*envp*/ []) {
	return Core::Details::MainImpl(Core::CreateCommandLineArguments(argc, argv));
}

#elif ENGINE_OS_EMSCRIPTEN

EMSCRIPTEN_KEEPALIVE int main(const int argc, char* const argv[], const char* const /*envp*/ []) {
	return Core::Details::MainImpl(Core::CreateCommandLineArguments(argc, argv));
}

#else

#error Unknown platform!

#endif
