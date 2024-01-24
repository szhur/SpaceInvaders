// ◦ Playrix ◦
#include "stdafx.h"
#include "Enemy.h"

namespace SpaceInvaders {

Enemy::Enemy(
        std::function<void(TimedSpline<FPoint>&)> func
    ,   Render::SpritePtr sprite
    ,   float time
    ,   int health
)
    : MovableObject(func, sprite, time)
    , _health(health)  
{}

void Enemy::Damage()
{
    _health--;
}

bool Enemy::IsValid() const
{
    return _timer < _lifeTime && _health > 0;
}

} // namespace SpaceInvaders