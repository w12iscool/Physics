//
// Created by Harshad on 7/26/2025.
//

#pragma once

#include "Ball.h"
#include "RaylibTimer.h"
#include <raylib.h>
#include <string>

class Ball;
class Weapon {
public:
    virtual ~Weapon() = default;

    virtual void initTextures() = 0;
    virtual void render() = 0;
    virtual void rotate(Ball& target, float& orbitSpeed) = 0;
    virtual void handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed, bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& getOtherAngle, bool& parryFreeze) = 0;


    virtual Rectangle& getRect() = 0;
    virtual float& getOrbitSpeed() = 0;
    virtual float& getAngle() = 0;
    virtual float& getDrawAngle() = 0;
    virtual float& getWidth() = 0;
    virtual float& getHeight() = 0;
    virtual float& getRadiusOffset() = 0;
    virtual float& getNormalOrbitSpeed() = 0;
    virtual float& getDirection() = 0;
    virtual bool& getFrozen() = 0;
    virtual bool& getCollDb() = 0;

    virtual void setDirection(int direction) = 0;
    virtual void resetState() = 0;
    virtual std::string getName() = 0;

};





