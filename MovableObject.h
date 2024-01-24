// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

namespace SpaceInvaders {

using namespace Core;

class MovableObject
{
public:
    MovableObject(
            std::function<void(TimedSpline<FPoint>&)> func
        ,   Render::SpritePtr sprite
        ,   float time
    );

    void Update(float dt);

    FPoint GetCurrentPosition() const;

    Render::SpritePtr GetSprite() const;

    virtual bool IsValid() const;

    void Invalidate();

protected:
    float _timer = 0.0f;
    float _lifeTime;

private:
    Render::SpritePtr _sprite = nullptr;

    TimedSpline<FPoint> _spline;
};

} // namespace SpaceInvaders