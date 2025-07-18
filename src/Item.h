//
// Created by Harshad on 7/17/2025.
//
#pragma once
#include <raylib.h>
#include "Ball.h"
#include "box2d/box2d.h"
#include "raymath.h"

class Ball;
class Item {
private:
    float m_width{ 25 };
    float m_height{ 50 };
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 5.0f };

    Rectangle m_itemRect;
    Rectangle m_headRect;
    float m_drawAngle;

public:
    [[nodiscard]] float getWidth() const;
    float getHeight();
    Vector2 getPos();

    virtual void rotate(Ball& ball);
    virtual void render();
    bool CheckCollisionCircleRotatedRec(Ball& ball, Rectangle& rec, float recRocation);
};




