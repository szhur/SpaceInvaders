// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

#include "MovableObject.h"

namespace SpaceInvaders {

using namespace Core;

class Enemy : public MovableObject
{
public:
    Enemy(
            std::function<void(TimedSpline<FPoint>&)> func
        ,   Render::SpritePtr sprite
        ,   float time
        ,   int health
    );

    void Damage();

    bool IsValid() const override;
private:
    int _health;
};

} // namespace SpaceInvaders