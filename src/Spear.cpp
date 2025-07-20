//
// Created by Harshad on 7/18/2025.
//

#include "Spear.h"

#include <vector>

inline float tau = 2 * (std::numbers::pi);
void Spear::rotate(Ball& ball)
{
    float increment = (tau / 360.0f) * m_orbitSpeed;
    Vector2 origin = Vector2(b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30);
    m_drawRadius = ball.getRadius() + 40;

    m_angle += increment;
    if (m_angle > tau)
        m_angle -= tau;

    m_itemRect = {origin.x + m_drawRadius * cos(m_angle), origin.y + m_drawRadius * sin(m_angle), m_width, m_height};
    m_headRect = {m_itemRect.x, m_itemRect.y, m_width, 20};
    m_drawAngle = RAD2DEG * (m_angle + std::numbers::pi / 2.0f);
}

void Spear::render()
{
    DrawRectanglePro(m_itemRect, Vector2(m_width / 2, m_height / 2), m_drawAngle, BLUE);
    DrawRectanglePro(m_headRect, Vector2(m_width / 2, m_height / 2), m_drawAngle, RED);
}

void Spear::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);
    Vector2 circleCenter = Vector2((b2Body_GetPosition(ball.getBallId()).x * 30), (b2Body_GetPosition(ball.getBallId()).y * 30));
    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    if (!m_debounce)
    {
        if (CheckCollisionCircleRotatedRec(ball, m_itemRect, physicalAngleRad) || CheckCollisionCircleRotatedRec(ball, m_headRect, physicalAngleRad))
        {
            m_debounce = true;
            StartTimer(&timer, lifeTime);
            StartTimer(&freezeTimer, freezeLifeTime);
            m_height += 1;
            std::cout << "hit!";
        }
    }
    if (TimerDone(&timer))
    {
        m_debounce = false;
    }

    if (TimerDone(&freezeTimer))
    {
        m_orbitSpeed = m_normalOrbitSpeed;
        ball.setColor(RED);
        ball.setFrozen(false);
        ball2.setFrozen(false);
        // 50 is the normal gravity
        b2Body_SetGravityScale(ball.getBallId(), 1.0f);
        b2Body_SetGravityScale(ball2.getBallId(), 1.0f);
    }
    else
    {
        m_orbitSpeed = 0.0f;
        ball.setColor(RAYWHITE);
        ball.setFrozen(true);
        ball2.setFrozen(true);
        b2Body_EnableSleep(ball.getBallId(), true);
        b2Body_EnableSleep(ball2.getBallId(), true);
        b2Body_SetLinearVelocity(ball.getBallId(), b2Vec2(0.0f, 0.0f));
        b2Body_SetLinearVelocity(ball2.getBallId(), b2Vec2(0.0f, 0.0f));
        b2Body_SetGravityScale(ball.getBallId(), 0.0f);
        b2Body_SetGravityScale(ball2.getBallId(), 0.0f);

    }
}
