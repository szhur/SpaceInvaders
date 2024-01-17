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
	_sprite1 = Core::resourceManager.Get<Render::Sprite>("btnStart_Text");
	_sprite2 = Core::resourceManager.Get<Render::Sprite>("Star");
	_circle = Core::resourceManager.Get<Render::Sprite>("Circle");

	_curSprite = 0;
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

	if (_curSprite == 0)
	{
		//
		// Метод Sprite::Draw() без параметров отрисовывает изображение в центре координат (0, 0).
		// Центр координат и положение осей были заданы выше установкой текущего преобразования.
		//
		_sprite1->Draw();
	}
	else
	{
		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(-_circle->Width() * 0.5f, -_circle->Height() * 0.5f, 0.0f);
		
		//
		// При отрисовке спрайта можно задать прямоугольник, в который
		// он будет отрисован (соответственно отмасштабирован).
		//
		FRect rect = _circle->GetRect();

		//
		// При отрисовке спрайта можно задать часть изображения, которая
		// будет отрисована, в UV координатах. UV координаты нормализованы,
		// то есть вне зависимости от размера изображения в текселях,
		// UV координаты задаются в диапазоне 0..1.
		//
		FRect uv(0, 1, 0, 1); // (xStart, xEnd, yStart, yEnd)

		_circle->Draw(rect, uv);
	}

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
	_sprite2->Draw();
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
	//
	// Обновим контейнер с эффектами
	//
	_effCont.Update(dt);

	//
	// dt - значение времени в секундах, прошедшее от предыдущего кадра.
	// Оно может принимать разные значения, в зависимости от производительности системы
	// и сложности сцены.
	//
	// Для того, чтобы наша анимация зависела только от времени, и не зависела от
	// производительности системы, мы должны рассчитывать её от этого значения.
	//
	// Увеличиваем наш таймер с удвоенной скоростью.
	//
	_timer += dt * 2;
	
	//
	// Зацикливаем таймер в диапазоне (0, 2п).
	// Это нужно делать для предотвращения получения некорректных значений,
	// если вдруг переполнится разрядная сетка (float переполнился) или задержка
	// от предыдущего кадра была слишкой большой (система тормози-и-ит).
	//
	// Диапазон значений выбран равным (0, 2п), потому что мы используем это значение
	// для расчёта синуса, и большие значения будут просто периодически повторять результат.
	//
	while (_timer > 2 * Math::PI)
	{
		_timer -= 2 * Math::PI;
	}
	
	//
	// Анимирование параметра масштабирования в зависимости от таймера.
	//
	_scale = 0.8f + 0.25f * sinf(_timer);
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

		//
		// Изменяем значение с 0 на 1 и наоборот.
		//
		_curSprite = 1 - _curSprite;
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

bool TestWidget::MouseUp(const IPoint &/*mousePos*/)
{
	if (_eff)
	{
		//
		// Если эффект создан, то при отпускании мыши завершаем его.
		//
		_eff->Finish();
		_eff = nullptr;
	}
	return true;
}

void TestWidget::AcceptMessage(const Message& /*message*/)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//

	/*const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();*/
}

bool TestWidget::KeyPressed(int keyCode)
{
	//
	// keyCode - виртуальный код клавиши.
	// В качестве значений для проверки нужно использовать константы VK_.
	//

	if (keyCode == VK_A) {
		// Реакция на нажатие кнопки A
	}
	return true;
}

bool TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode код введённого символа
	//

	if (unicodeChar == L'а') {
		// Реакция на ввод символа 'а'
	}
	return true;
}

} // namespace Example
