// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

namespace Example {

using namespace Core;

///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
///
class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, pugi::xml_node elem);

	void Draw() override;
	void Update(float dt) override;
	
	void AcceptMessage(const Message& message) override;
	
	bool MouseDown(const IPoint& mousePos) override;
	bool MouseMove(const IPoint& mousePos) override;
	bool MouseUp(const IPoint& mousePos) override;

	bool KeyPressed(int keyCode) override;
	bool CharPressed(int unicodeChar) override;

private:
	void Init();

private:
	float _timer = 0.0f;
	
	float _scale = 0.0f;
	float _angle = 0.0f;
	
	Render::SpritePtr _sprite1 = nullptr;
	Render::SpritePtr _sprite2 = nullptr;
	Render::SpritePtr _circle = nullptr;
	
	int _curSprite = 0;

	EffectsContainer _effCont;
	ParticleEffectPtr _eff;

	TimedSpline<FPoint> _spline;
};

} // namespace Example
