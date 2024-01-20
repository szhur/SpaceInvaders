// ◦ Playrix ◦
#include "stdafx.h"
#include "MainDelegate.h"
#include "MainWidget.h"

namespace Core::Debug {
	template<typename T>
	auto GetResourceMemoryUsage(MemoryInUseInfo& memoryInUseInfo) {
		return memoryInUseInfo.GetMemoryUsage([](MemoryInUseInfo& memoryInUseInfoLocal) {
			Core::resourceManager.GetMemoryInUse<T>(memoryInUseInfoLocal);
			});
	}

	auto GetModelAnimationsMemoryUsage(MemoryInUseInfo& memoryInUseInfo) {
		return memoryInUseInfo.GetMemoryUsage([](MemoryInUseInfo& memoryInUseInfoLocal) {
			Render::ModelAnimation::SpawnedTotalMemoryInUse(memoryInUseInfoLocal);
		});
	}
} // namespace Core::Debug

namespace SpaceInvaders {

using namespace Core;

enum {
	windowWidth = 1024,
	windowHeight = 768
};

MainDelegate::MainDelegate()
{
}

void MainDelegate::GameContentSize(int /*deviceWidth*/, int /*deviceHeight*/, int &width, int &height)
{
	width = windowWidth;
	height = windowHeight;
}

void MainDelegate::ScreenMode(DeviceMode &mode)
{
	mode = DeviceMode::Windowed;
}

void MainDelegate::RegisterTypes()
{
	REGISTER_WIDGET_XML(MainWidget, "MainWidget");
}

void MainDelegate::LoadResources()
{
	using namespace std::chrono_literals;

	Core::resourceFactory->LoadResourcesXml("Resources.xml", LoadOptions::Default);

	ParticleEffectLoader::LoadEffects("Example2_Flames.xml", "TestGroup", true);
	ParticleEffectLoader::LoadEffects("Example3_FindObject.xml", "TestGroup", true);


	Core::guiManager.LoadLayers("Layers.xml");

	Core::mainScreen.pushLayer("TestLayer");
}

void MainDelegate::OnResourceLoaded() {
	if (Core::appInstance->AllResourcesLoaded()) {
		Core::appInstance->SetLoaded();
	}
}

} // namespace SpaceInvaders
