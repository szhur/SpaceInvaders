// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

namespace SpaceInvaders {

using namespace Core;

class MovableObject
{
public:
    MovableObject(
            std::function<FPoint(float)> func
        ,   Render::SpritePtr sprite
        ,   float time
    );

    void Update(float dt);

    FPoint GetCurrentPosition() const;

    Render::SpritePtr GetSprite() const;

    virtual bool IsValid() const;

    void Invalidate();

private:
    Render::SpritePtr _sprite = nullptr;

    std::function<FPoint(float)> _func;

protected:
    float _timer = 0.0f;
    float _lifeTime;
};

} // namespace SpaceInvaders