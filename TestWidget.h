// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

namespace Example {

using namespace Core;

class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, pugi::xml_node elem);

	void Draw() override;
	void Update(float dt) override;
	
	bool MouseDown(const IPoint& mousePos) override;
	bool MouseMove(const IPoint& mousePos) override;

private:
	void Init();

private:
	float _timer = 0.0f;
	
	float _scale = 0.0f;
	float _angle = 0.0f;
	
	Render::SpritePtr _warShip = nullptr;

	EffectsContainer _effCont;
	ParticleEffectPtr _eff;

	TimedSpline<FPoint> _spline;
};

} // namespace Example
