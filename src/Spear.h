//
// Created by Harshad on 7/18/2025.
//
#pragma once

#include "Item.h"
#include <numbers>
#include "Ball.h"
#include <iostream>
#include <raylib.h>
#include "RaylibTimer.h"
#include "box2d/box2d.h"

class Item;
class Ball;
class Spear : public Item {
private:
    float m_width{ 25 };
    float m_height{ 75 };
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 5.0f };
    float m_normalOrbitSpeed{ 5.0f };

    Rectangle m_itemRect;
    Rectangle m_headRect;
    float m_drawAngle;
    float m_drawRadius;
    bool m_debounce{ false };

public:
    void rotate(Ball& ball) override;
    void render() override;
    void handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime);
};

