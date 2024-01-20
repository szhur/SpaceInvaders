// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

#include "MovableObject.h"

namespace SpaceInvaders {

using namespace Core;

class MainWidget : public GUI::Widget
{
public:
	MainWidget(const std::string& name, pugi::xml_node elem);

	void Draw() override;
	void Update(float dt) override;
	
	bool MouseDown(const IPoint& mousePos) override;

private:
	void Init();

private:
	float _scale = 0.0f;
	float _angle = 0.0f;
	
	Render::SpritePtr _warShip = nullptr;
	Render::SpritePtr _fire = nullptr;

	std::vector<MovableObject> _shotVec;
};

} // namespace SpaceInvaders
