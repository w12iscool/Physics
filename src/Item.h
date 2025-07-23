//
// Created by Harshad on 7/17/2025.
//
#pragma once
#include <raylib.h>
#include "Ball.h"
#include "box2d/box2d.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <cmath>

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
    struct Projection
    {
        float min;
        float max;
    };
public:
    [[nodiscard]] float getWidth() const;
    float getHeight();
    Vector2 getPos();

    virtual void rotate(Ball& ball, Rectangle& itemRect, float orbitSpeed, float angle, float drawAngle, float width, float height);
    virtual void render();

    Vector2 Normalize(Vector2 v);
    float Dot(Vector2 a, Vector2 b);
    Vector2 Perpendicular(Vector2 v);
    Vector2 RotatePoint(Vector2 point, Vector2 origin, float angleRad);

    Projection projectPolygon(const std::vector<Vector2>& vertices, Vector2 axis);
    bool Overlaps(Projection a, Projection b);

    std::vector<Vector2> getRotatedRect(Vector2 center, float width, float height, float angleRad);
    bool satCollisions(const std::vector<Vector2>& polyA, const std::vector<Vector2>& polyB);
    bool satCircleVsPolygon(Vector2 circleCenter, float radius, const std::vector<Vector2>& poly);

    bool CheckCollisionCircleRotatedRec(Ball& ball, Rectangle& rec, float recRocation);
};




