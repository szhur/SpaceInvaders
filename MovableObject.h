// ◦ Playrix ◦
#pragma once

#include <PlayrixEngine.h>

namespace SpaceInvaders {

using namespace Core;

class MovableObject
{
public:
    MovableObject(float x);

    void Update(float dt);

    FPoint GetCurrentPosition() const;

    bool IsValid() const;
private:
    float _timer = 1.0f;

    TimedSpline<FPoint> _spline;
};

} // namespace SpaceInvaders