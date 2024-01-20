// ◦ Playrix ◦
#include "stdafx.h"
#include "MainWidget.h"

namespace SpaceInvaders {

using namespace Core;

MainWidget::MainWidget(const std::string& name, pugi::xml_node elem)
	: GUI::Widget(name, elem)
{
	Init();
}

bool compareFloat(float lhs, float rhs, float epsilon = 0.001f)
{
	if (lhs + epsilon > rhs && lhs - epsilon < rhs)
		return true;

	return false;
}

void MainWidget::Init()
{
	_warShip = Core::resourceManager.Get<Render::Sprite>("Warship");
	_fire = Core::resourceManager.Get<Render::Sprite>("Fire");

	_scale = 0.2f;
	_angle = 0;
}

void MainWidget::Draw()
{
	IPoint mousePos = Core::mainInput.GetMousePos();

	Render::device.PushMatrix();
	
	Render::device.MatrixTranslate((float)mousePos.x, 50.0f, 0);
	Render::device.MatrixRotate(Vector3(0, 0, 1), Angle::FromRadians(_angle));

	Render::device.MatrixScale(_scale);
	Render::device.MatrixTranslate(-_warShip->Width() * 0.5f, -_warShip->Height() * 0.5f, 0.0f);
		
	FRect rect = _warShip->GetRect();

	FRect uv(0, 1, 0, 1);

	_warShip->Draw(rect, uv);

	Render::device.PopMatrix();

	for (auto const & shot : _shotVec)
	{
		FPoint currentPosition = shot.GetCurrentPosition();

		Render::device.PushMatrix();
		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
		_fire->Draw();
		Render::device.PopMatrix();
	}
}

void MainWidget::Update(float dt)
{
	for (auto & shot : _shotVec)
	{
		if (!shot.IsValid())
		{
			_shotVec.erase(_shotVec.begin());

			continue;
		}

		shot.Update(dt);
	}
}

bool MainWidget::MouseDown(const IPoint &mousePos)
{
	auto mouseX = Core::mainInput.GetMousePos().x / _scale;

	_shotVec.push_back(MovableObject(mouseX)); // TODO: Probably reconsider this logic

	return false;
}

} // namespace SpaceInvaders
