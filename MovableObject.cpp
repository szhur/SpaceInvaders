// ◦ Playrix ◦
#include "stdafx.h"
#include "MovableObject.h"

namespace SpaceInvaders {

using namespace Core;

MovableObject::MovableObject(
        std::function<void(TimedSpline<FPoint>&)> func
    ,   Render::SpritePtr sprite
    ,   float time
)
    : _sprite(sprite)
    , _lifeTime(time)
{
    func(_spline);
}

FPoint MovableObject::GetCurrentPosition() const
{
    return _spline.getGlobalFrame(std::clamp(_timer, 0.0f, _lifeTime));
}

void MovableObject::Update(float dt)
{
    _timer += dt;
}

Render::SpritePtr MovableObject::GetSprite() const
{
    return _sprite;
}

bool MovableObject::IsValid() const
{
    return _timer < _lifeTime;
}

void MovableObject::Invalidate()
{
    _timer = _lifeTime;
}

} // namespace SpaceInvaders