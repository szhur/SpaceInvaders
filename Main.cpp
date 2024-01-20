// ◦ Playrix ◦
#include "stdafx.h"
#include "MainDelegate.h"
#include <Core/Main/CommandLineArguments.h>
#include <Debug/CrashReport.h>
#include <Core/Main/Main.h>

namespace AppDetails {

	using namespace Core;

	static std::string GetWriteDirectory() {
#if ENGINE_OS_WINDOWS
		return IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "write_directory");
#else
		return IO::Path::GetSpecialFolderPath(SpecialFolder::LocalDocuments);
#endif
	}

	static std::string GetBaseDirectory([[maybe_unused]] const Core::CommandLineArguments& commandLineArguments) {
		const std::string baseDirectoryName = "base_p";
#if ENGINE_OS_WINDOWS
		return IO::Path::Combine(IO::Directory::GetCurrentDirectory(), baseDirectoryName);
#elif ENGINE_OS_MACOS
		const std::string appInstalledDirectory = IO::Path::GetSpecialFolderPath(SpecialFolder::Bundle);
		return IO::Path::Combine(appInstalledDirectory, "Contents/Resources", baseDirectoryName);
#else
		return {};
#endif
	}

} // namespace AppDetails


int Core::Main(const CommandLineArguments& commandLineArguments) {
	ParticleSystem::SetTexturesPath("textures/particles");

	const auto writeDirectory = AppDetails::GetWriteDirectory();
	const std::string baseDirectory = AppDetails::GetBaseDirectory(commandLineArguments);
	LOG_INFO("Path := " << baseDirectory);

	IO::Directory::CreateDirectories(writeDirectory); // Необходимо, чтобы директория существовала
	Core::fileSystem.SetWriteDirectory(writeDirectory);
	Core::GlobalVars::Init(IO::EncodedFileSystem::EncoderType::Raw, {}, {}, {});

	Debug::CrashReport::Initialize(IO::Path::Combine(writeDirectory, "CrashReports"), "CrashReport", 5,
		Debug::CrashReport::Options::DumpLuaStack |
		Debug::CrashReport::Options::DumpLuaLocals |
		Debug::CrashReport::Options::Minidump);

	IO::Directory::SetCurrentDirectory(baseDirectory);
	Core::fileSystem.MountDirectory("base", baseDirectory);

	Core::GlobalConstants::Init();

	Core::AddStandardPlatformLogSink();
	Log::Instance().AddSink(MakeIntrusiveAtomic<TextFileLogSink>("log.txt", true));
	Log::Instance().AddSink(MakeIntrusiveAtomic<HtmlFileLogSink>("log.htm", true));
	Log::Instance().AddSink(MakeIntrusiveAtomic<RotatingTextFileLogSink>(10 * 1024, "log.rotating.txt", false));

	Core::RunApplicationMainLoop(commandLineArguments, new SpaceInvaders::MainDelegate());

	return 0;
}
