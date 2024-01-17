// ◦ Playrix ◦
#include "stdafx.h"
#include "TestWidget.h"

namespace Example {

using namespace Core;

TestWidget::TestWidget(const std::string& name, pugi::xml_node elem)
	: GUI::Widget(name, elem)
{
	Init();
}

void TestWidget::Init()
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

void TestWidget::Draw()
{
	//
	// Получаем текущее положение курсора мыши.
	//
	IPoint mousePos = Core::mainInput.GetMousePos();

	//
	// Проталкиваем в стек текущее преобразование координат, чтобы в дальнейшем
	// можно было восстановить это преобразование вызовом PopMatrix.
	//
	Render::device.PushMatrix();
	
	//
	// Изменяем текущее преобразование координат, перемещая центр координат в позицию мыши
	// и поворачивая координаты относительно этого центра вокруг оси z на угол _angle.
	//
	Render::device.MatrixTranslate((float)mousePos.x, (float)mousePos.y, 0);
	Render::device.MatrixRotate(Vector3(0, 0, 1), Angle::FromRadians(_angle));

	Render::device.MatrixScale(_scale);
	Render::device.MatrixTranslate(-_warShip->Width() * 0.5f, -_warShip->Height() * 0.5f, 0.0f);
		
	//
	// При отрисовке спрайта можно задать прямоугольник, в который
	// он будет отрисован (соответственно отмасштабирован).
	//
	FRect rect = _warShip->GetRect();

	//
	// При отрисовке спрайта можно задать часть изображения, которая
	// будет отрисована, в UV координатах. UV координаты нормализованы,
	// то есть вне зависимости от размера изображения в текселях,
	// UV координаты задаются в диапазоне 0..1.
	//
	FRect uv(0, 1, 0, 1); // (xStart, xEnd, yStart, yEnd)

	_warShip->Draw(rect, uv);

	//
	// Воостанавливаем прежнее преобразование координат, сохранённое ранее в стек.
	//
	Render::device.PopMatrix();
	
	//
	// Получаем текущие координаты объекта, двигающегося по сплайну
	//
	FPoint currentPosition = _spline.getGlobalFrame(std::clamp(_timer / 6.0f, 0.0f, 1.0f));

	//
	// И рисуем объект в этих координатах
	//
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
	//_sprite2->Draw();
	Render::device.PopMatrix();

	//
	// Этот вызов отключает текстурирование при отрисовке.
	//
	Render::device.SetTexturing(false);
	
	{
		//
		// Обьект PushColor() сохраняет в стек текущий цвет вершин и устанавливает новый.
		//
		Render::PushColor _(Color(255, 128, 0, 255));

		//
		// Метод DrawRect() выводит в графическое устройство прямоугольник, состоящий их двух
		// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
		// если разрешено текстурирование.
		//
		// Перед вызовом DrawRect() должен быть вызов Texture::Bind(), либо SetTexturing(false),
		// иначе визуальный результат будет непредсказуемым.
		//
		Render::DrawRect(924, 0, 100, 100);

	}
	
	//
	// Опять включаем текстурирование.
	//
	Render::device.SetTexturing(true);

	//
	// Рисуем все эффекты, которые добавили в контейнер эффектов _effCont.
	//
	_effCont.Draw();

	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(mousePos.x) + ", " + utils::lexical_cast(mousePos.y), 1.f, CenterAlign);

}

void TestWidget::Update(float dt)
{
	_effCont.Update(dt);

	_timer += dt * 2;
}

bool TestWidget::MouseDown(const IPoint &mousePos)
{
	if (Core::mainInput.GetMouseRightButton())
	{
		//
		// При нажатии на правую кнопку мыши, создаём эффект шлейфа за мышкой.
		//
		_eff = _effCont.AddEffect("Iskra");
		_eff->posX = mousePos.x + 0.f;
		_eff->posY = mousePos.y + 0.f;
		_eff->Reset();
		
		//
		// И изменяем угол наклона текстуры.
		//
		_angle += 25;
		while (_angle > 360)
		{
			_angle -= 360;
		}
	}
	else
	{
		//
		// При нажатии на левую кнопку мыши, создаём временный эффект, который завершится сам.
		//
		ParticleEffectPtr eff = _effCont.AddEffect("FindItem2");
		eff->posX = mousePos.x + 0.f;
		eff->posY = mousePos.y + 0.f;
		eff->Reset();
	}
	return false;
}

bool TestWidget::MouseMove(const IPoint &mousePos)
{
	if (_eff)
	{
		//
		// Если эффект создан, то перемещаем его в позицию мыши.
		//
		_eff->posX = mousePos.x + 0.f;
		_eff->posY = mousePos.y + 0.f;
	}
	return true;
}

} // namespace Example
