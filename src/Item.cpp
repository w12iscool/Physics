//
// Created by Harshad on 7/17/2025.
//

#include "Item.h"

#include <numbers>

float Item::getWidth() const
{
    return m_width;
}

float Item::getHeight()
{
    return m_height;
}

Vector2 Item::getPos()
{
    return m_pos;
}

inline float tau = 2 * (std::numbers::pi);

void Item::rotate(Ball& ball)
{
    float increment = (tau / 360.0f) * m_orbitSpeed;
    Vector2 origin = Vector2(b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30);
    float radius = ball.getRadius() + 40;

    m_angle += increment;
    if (m_angle > tau)
        m_angle -= tau;

    m_itemRect = {origin.x + radius * cos(m_angle), origin.y + radius * sin(m_angle), m_width, m_height};
    m_headRect = {m_itemRect.x, m_itemRect.y, m_width, 20};
    m_drawAngle = RAD2DEG * (m_angle + std::numbers::pi / 2.0f);
}


void Item::render()
{
    DrawRectanglePro(m_itemRect, Vector2(m_width / 2, m_height / 2), m_drawAngle, BLUE);
    DrawRectanglePro(m_headRect, Vector2(m_width / 2, m_height / 2), m_drawAngle, RED);
}

bool Item::CheckCollisionCircleRotatedRec(Ball& ball, Rectangle& rec, float recRocation)
{
    Vector2 recCenter = {rec.x + rec.width / 2.0f, rec.y + rec.height / 2.0f};
    Vector2 ballCenter = {b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30};
    Vector2 vecToCircle =  Vector2Subtract(ballCenter, recCenter);
    Vector2 circlePos = Vector2Rotate(vecToCircle, -recRocation);

    float closestX = Clamp(circlePos.x, -rec.width / 2.0f, rec.width / 2.0f);
    float closestY = Clamp(circlePos.y, -rec.height / 2.0f, rec.height / 2.0f);

    Vector2 closestPoint = {closestX, closestY};
    float distance = Vector2Distance(circlePos, closestPoint);

    return distance < ball.getRadius();
}



