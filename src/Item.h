//
// Created by Harshad on 7/17/2025.
//
#pragma once
#include <raylib.h>
#include "Ball.h"
#include "box2d/box2d.h"


class Ball;
class Item {
private:
    float m_width{ 15 };
    float m_height{ 70 };
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 5.0f };

    Rectangle m_itemRect;
    float m_drawAngle;

public:
    float getWidth() const;
    float getHeight();
    Vector2 getPos();

    void rotate(Ball& ball);
    void render();
};




