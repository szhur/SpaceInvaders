// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

namespace SpaceInvaders {

using namespace Core;

class MainDelegate : public Core::EngineAppDelegate {
public:
	MainDelegate();

	virtual void GameContentSize(int deviceWidth, int deviceHeight, int &width, int &height) override;
	virtual void ScreenMode(DeviceMode &mode) override;

	virtual void RegisterTypes() override;

	virtual void LoadResources() override;
	virtual void OnResourceLoaded() override;
private:
	std::unique_ptr<Core::CoThread> _loaderThread;
};

} // namespace SpaceInvaders
