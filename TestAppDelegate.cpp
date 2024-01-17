// ◦ Playrix ◦
#include "stdafx.h"
#include "TestAppDelegate.h"
#include "TestWidget.h"

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

namespace Example {

using namespace Core;

enum {
	windowWidth = 1024,
	windowHeight = 768
};

TestAppDelegate::TestAppDelegate()
{
}

void TestAppDelegate::GameContentSize(int /*deviceWidth*/, int /*deviceHeight*/, int &width, int &height)
{
	//
	// Здесь задаётся размер игрового экрана в логических единицах.
	//
	// Это значит, что и в оконном, и в полноэкранном режиме размер экрана
	// всегда будет равен windowWidth x windowHeight "пикселей".
	//
	width = windowWidth;
	height = windowHeight;
}

void TestAppDelegate::ScreenMode(DeviceMode &mode)
{
	mode = DeviceMode::Windowed;
}

void TestAppDelegate::RegisterTypes()
{
	//
	// Чтобы можно было декларативно использовать виджет в xml по имени,
	// его необходимо зарегистрировать таким образом.
	//
	REGISTER_WIDGET_XML(TestWidget, "TestWidget");
}

void TestAppDelegate::LoadResources()
{
	using namespace std::chrono_literals;
	//
	// Обычно в этом методе выполняется скрипт, в котором определяется,
	// какие ресурсы нужно загрузить и какой игровой слой положить на экран.
	//

	//
	// Так загружаются описания ресурсов: какие текстуры, анимации и т.п.
	// будут видны в медеджере ресурсов по имени.
	//
	Core::resourceFactory->LoadResourcesXml("Resources.xml", LoadOptions::Default);
	//
	// Загрузка эффектов, созданных в редакторе частиц.
	//
	ParticleEffectLoader::LoadEffects("Example2_Flames.xml", "TestGroup", true);
	ParticleEffectLoader::LoadEffects("Example3_FindObject.xml", "TestGroup", true);

	//
	// Загрузка слоёв.
	//
	// Слои создаются все сразу (вызываются конструкторы всех виджетов и т.п.),
	// но методы Draw() и Update() начинают вызываться только тогда, когда слой
	// добавлен на экран. В момент добавления на экран у виджетов слоя вызывается метод
	// AcceptMessage с параметром "Init".
	//
	Core::guiManager.LoadLayers("Layers.xml");
	//
	// Слой кладётся на экран.
	//
	// В этот момент у виджетов слоя один раз вызывается метод AcceptMessage с параметром "Init"
	// и начинают вызываться методы Draw() и Update() в каждом кадре.
	//
	Core::mainScreen.pushLayer("TestLayer");
}

void TestAppDelegate::OnResourceLoaded() {
	if (Core::appInstance->AllResourcesLoaded()) {
		Core::appInstance->SetLoaded();
	}
}

void TestAppDelegate::OnPostDraw() {
	if (!Render::isFontLoaded("arial"))
		return;

	//
	// Перед вызовом метода Render::PrintString() нужно привязать нужный шрифт
	//
	Render::BindFont("arial");
	Debug::MemoryInUseInfo memoryInUseInfo;

	auto stat = *Render::SpriteBatch2::GetStat();
	auto stat2 = *Render::device.GetStat();
	int dy = static_cast<int>(Render::getFontHeight());
	int x = Render::device.Width() - 5;
	int y = Render::device.Height() - 20;
	int line = 0;
	Render::PrintString(x, y - dy * line++, std::string("fps ") + utils::lexical_cast(Core::appInstance->GetCurrentFps()), 1.0f, RightAlign, BottomAlign);

	Render::PrintString(x, y - dy * line++, std::string("Vm ") + utils::lexical_cast(Render::device.GetVideoMemoryUsage().GetTotal() / 1024 / 1024.f, 2) + std::string("M"), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Am ") + utils::lexical_cast(Debug::GetResourceMemoryUsage<MM::AudioResource>(memoryInUseInfo).heap / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Ram ") + utils::lexical_cast(utils::GetProcSize() / 0x100000) + std::string("M"), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Textures: ") + utils::lexical_cast(Debug::GetResourceMemoryUsage<Render::Texture>(memoryInUseInfo).heap / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
	const auto effectsMemoryInUse = memoryInUseInfo.GetMemoryUsage([](Debug::MemoryInUseInfo& memoryInUseInfoLocal) {
		ParticleEffect::TotalMemoryInUse(memoryInUseInfoLocal);
		Core::resourceManager.GetMemoryInUse<ParticleEffect>(memoryInUseInfoLocal);
		});
	Render::PrintString(x, y - dy * line++, std::string("Effects: ") + utils::lexical_cast(effectsMemoryInUse.heap / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("EmitterMasks: ") + utils::lexical_cast(Debug::GetResourceMemoryUsage<EmitterMask>(memoryInUseInfo).heap / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("3D: ") + utils::lexical_cast((Debug::GetResourceMemoryUsage<Render::ModelAnimationData>(memoryInUseInfo).heap + Debug::GetModelAnimationsMemoryUsage(memoryInUseInfo).heap) / 1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Batch Instances: ") + utils::lexical_cast(stat.num_instances), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Batch Begin/End Pairs: ") + utils::lexical_cast(stat.num_begin_end_pairs), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Batch Draw Calls: ") + utils::lexical_cast(stat.num_draw_calls), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Batch DIP Calls: ") + utils::lexical_cast(stat.num_dip_calls), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Device Draw Calls: ") + utils::lexical_cast(stat2.num_draw_calls), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Device DIP Calls: ") + utils::lexical_cast(stat2.num_dip_calls), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Device Drawn Primitives: ") + utils::lexical_cast(stat2.num_drawn_primitives), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Materials Created: ") + utils::lexical_cast(stat2.num_materials_created), 1.0f, RightAlign, BottomAlign);
	Render::PrintString(x, y - dy * line++, std::string("Default Materials Freed: ") + utils::lexical_cast(stat2.num_default_materials_freed), 1.0f, RightAlign, BottomAlign);
}

} // namespace Example
