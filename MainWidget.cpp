// ◦ Playrix ◦
#include "stdafx.h"
#include "MainWidget.h"
#include "Common.h"

namespace SpaceInvaders {

using namespace Core;

MainWidget::MainWidget(const std::string& name, pugi::xml_node elem)
	: GUI::Widget(name, elem)
{
	Init();
}

bool MainWidget::CheckContact(
		const FPoint& shotPos
	, 	const FPoint& enemyPos
	,	float epsilon
)
{
	bool contactX = shotPos.x + epsilon > enemyPos.x && shotPos.x - epsilon < enemyPos.x;

	bool contactY = shotPos.y + epsilon > enemyPos.y && shotPos.y - epsilon < enemyPos.y;

	return contactX && contactY;
}

void MainWidget::Init()
{
	_warShip = Core::resourceManager.Get<Render::Sprite>("Warship");
	_fire = Core::resourceManager.Get<Render::Sprite>("Fire");
	_star = Core::resourceManager.Get<Render::Sprite>("Star");
	_meteor = Core::resourceManager.Get<Render::Sprite>("Meteor");
}

void MainWidget::Draw()
{
	IPoint mousePos = Core::mainInput.GetMousePos();

	Render::device.PushMatrix();
	
	Render::device.MatrixTranslate((float)mousePos.x, 50.0f, 0);
	Render::device.MatrixScale(_scale);
	Render::device.MatrixTranslate(-_warShip->Width() * 0.5f, -_warShip->Height() * 0.5f, 0.0f);
		
	FRect rect = _warShip->GetRect();

	FRect uv(0, 1, 0, 1);

	_warShip->Draw(rect, uv);

	Render::device.PopMatrix();

	// TODO: Move loop content to separate function, remove duplications
	for (auto const & shot : _shotVec)
	{
		FPoint currentPosition = shot.GetCurrentPosition();

		Render::device.PushMatrix();
		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
		shot.GetSprite()->Draw();
		Render::device.PopMatrix();
	}

	for (auto const & enemy : _enemyVec)
	{
		FPoint currentPosition = enemy.GetCurrentPosition();

		Render::device.PushMatrix();
		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
		enemy.GetSprite()->Draw();
		Render::device.PopMatrix();
	}
}

void MainWidget::Update(float dt)
{
	for (auto & shot : _shotVec)
	{
		for (auto & enemy : _enemyVec)
		{
			if (CheckContact(shot.GetCurrentPosition(), enemy.GetCurrentPosition()))
			{
				enemy.Damage();

				shot.Invalidate();
			}
		}
	}

	// TODO: Maybe move deletion to upper loop
	for (size_t i = 0; i < _shotVec.size(); ++i)
	{
		auto & shot = _shotVec[i];

		if (!shot.IsValid())
		{
			_shotVec.erase(_shotVec.begin() + i);

			--i;

			continue;
		}

		shot.Update(dt);
	}

	for (size_t i = 0; i < _enemyVec.size(); ++i)
	{
		auto & enemy = _enemyVec[i];

		if (!enemy.IsValid())
		{
			_enemyVec.erase(_enemyVec.begin() + i);

			--i;

			continue;
		}

		enemy.Update(dt);
	}

	int choice = rand() % 200;
	if (choice <= 1)
	{
		float startX = static_cast<float>(rand() % static_cast<int>(1024 / _scale));
		float maxY = windowHeight / _scale;

		_enemyVec.push_back(
			Enemy(
					[&startX, &maxY](TimedSpline<FPoint>& spline){
						spline.addKey(0.0f, FPoint(startX, 4000.0f));
						spline.addKey(5.0f, FPoint(startX, 3000.0f));
						spline.addKey(10.0f, FPoint(startX, 2000.0f));
						spline.addKey(15.0f, FPoint(startX, 1000.0f));
						spline.addKey(20.0f, FPoint(startX, 0.0f));
						spline.CalculateGradient();
					}
				,	_star
				,	20.0f
				,	3
			)
		);
	}
}

bool MainWidget::MouseDown(const IPoint &mousePos)
{
	auto mouseX = Core::mainInput.GetMousePos().x / _scale;

	_shotVec.push_back(
		MovableObject(
				[&mouseX](TimedSpline<FPoint>& spline){
					spline.addKey(0.0f, FPoint(mouseX, 400.0f));
					spline.addKey(0.5f, FPoint(mouseX, 2200.0f));
					spline.addKey(1.0f, FPoint(mouseX, 4000.0f));
					spline.CalculateGradient();
				}
			,	_fire
			,	1.0f
		)
	); 

	return false;
}

} // namespace SpaceInvaders
