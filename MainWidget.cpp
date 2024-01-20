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

void MainWidget::Init()
{
	_warShip = Core::resourceManager.Get<Render::Sprite>("Warship");

	_scale = 0.2f;
	_angle = 0;

	_spline.addKey(0.0f, FPoint(100.0f, 100.0f));
	_spline.addKey(0.25f, FPoint(150.0f, 300.0f));
	_spline.addKey(0.5f, FPoint(500.0f, 300.0f));
	_spline.addKey(0.75f, FPoint(630.0f, 450.0f));
	_spline.addKey(1.0f, FPoint(600.0f, 550.0f));
	_spline.CalculateGradient();
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

	FPoint currentPosition = _spline.getGlobalFrame(std::clamp(_timer / 6.0f, 0.0f, 1.0f));

	Render::device.PushMatrix();
	Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
	//_sprite2->Draw();
	Render::device.PopMatrix();
}

void MainWidget::Update(float dt)
{
	_effCont.Update(dt);

	_timer += dt * 2;
}

bool MainWidget::MouseDown(const IPoint &mousePos)
{
	if (Core::mainInput.GetMouseRightButton())
	{
		_eff = _effCont.AddEffect("Iskra");
		_eff->posX = mousePos.x + 0.f;
		_eff->posY = mousePos.y + 0.f;
		_eff->Reset();
		
		_angle += 25;
		while (_angle > 360)
		{
			_angle -= 360;
		}
	}
	else
	{
		ParticleEffectPtr eff = _effCont.AddEffect("FindItem2");
		eff->posX = mousePos.x + 0.f;
		eff->posY = mousePos.y + 0.f;
		eff->Reset();
	}
	return false;
}

bool MainWidget::MouseMove(const IPoint &mousePos)
{
	if (_eff)
	{
		_eff->posX = mousePos.x + 0.f;
		_eff->posY = mousePos.y + 0.f;
	}
	return true;
}

} // namespace SpaceInvaders
