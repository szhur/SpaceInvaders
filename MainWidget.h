// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

#include "Enemy.h"

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

	bool CheckContact(
			const FPoint& shot
		, 	const FPoint& enemy
		, 	float epsilon = 100.0f
	);

private:
	float _scale = 0.2f;
	
	Render::SpritePtr _warShip = nullptr;
	Render::SpritePtr _fire = nullptr;
	Render::SpritePtr _star = nullptr;
	Render::SpritePtr _meteor = nullptr;

	std::vector<MovableObject> _shotVec;

	std::vector<Enemy> _enemyVec;
};

} // namespace SpaceInvaders
