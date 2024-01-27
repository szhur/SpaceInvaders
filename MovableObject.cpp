// ◦ Playrix ◦
#include "stdafx.h"
#include "MovableObject.h"

namespace SpaceInvaders {

using namespace Core;

MovableObject::MovableObject(
        std::function<FPoint(float)> func
    ,   Render::SpritePtr sprite
    ,   float time
)
    : _sprite(sprite)
    , _func(func)
    , _lifeTime(time)
{}

FPoint MovableObject::GetCurrentPosition() const
{
    return _func(_timer);
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