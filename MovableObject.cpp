// ◦ Playrix ◦
#include "stdafx.h"
#include "MovableObject.h"

namespace SpaceInvaders {

using namespace Core;

MovableObject::MovableObject(float x) 
{
    _timer = 0.0f;

	_spline.addKey(0.0f, FPoint(x, 400.0f));
	_spline.addKey(0.5f, FPoint(x, 2200.0f));
	_spline.addKey(1.0f, FPoint(x, 4000.0f));
	_spline.CalculateGradient();
}

FPoint MovableObject::GetCurrentPosition() const
{
    return _spline.getGlobalFrame(std::clamp(_timer, 0.0f, 1.0f));
}

void MovableObject::Update(float dt)
{
    _timer += dt;
}

bool MovableObject::IsValid() const
{
    return _timer < 1.0f;
}

} // namespace SpaceInvaders