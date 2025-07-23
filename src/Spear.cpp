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

    Rectangle source = Rectangle(0, 0, (float)(*m_stickTexture).width, (float)(*m_stickTexture).height);
    Rectangle dest = Rectangle(m_itemRect.x, m_itemRect.y, m_width, m_height);

    Rectangle sourceHead = Rectangle(0, 0, (float)(*m_spearHeadTexture).width, (float)(*m_spearHeadTexture).height);
    Rectangle destHead = Rectangle(m_headRect.x, m_headRect.y, 45, 50);

    DrawTexturePro((*m_stickTexture), source, dest, Vector2(m_itemRect.width / 2, m_itemRect.height / 2), m_drawAngle, WHITE);

    Vector2 offsetDir = Vector2{ cos(m_angle), sin(m_angle) };

    float offset = m_height / 2.0f;

    Rectangle spearHeadRect = m_headRect;
    spearHeadRect.x += offsetDir.x * offset;
    spearHeadRect.y += offsetDir.y * offset;
    spearHeadRect.width = 50;
    spearHeadRect.height = 50;

    DrawTexturePro(
        *m_spearHeadTexture,
        sourceHead,
        spearHeadRect,
        Vector2(spearHeadRect.width / 2, spearHeadRect.height / 2),
        m_drawAngle,
        WHITE
    );

}

void Spear::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);
    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_itemRect.x, m_itemRect.y};
    Vector2 headCenter = {m_headRect.x, m_headRect.y};

    std::vector<Vector2> itemRectPoly = getRotatedRect(itemCenter, m_itemRect.width, m_itemRect.height, physicalAngleRad);
    std::vector<Vector2> headRectPoly = getRotatedRect(headCenter, m_headRect.width, m_headRect.height, physicalAngleRad);

    Vector2 circleCenter = Vector2(b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30);
    float radius = ball.getRadius();

    bool hit = satCircleVsPolygon(circleCenter, radius, itemRectPoly) || satCircleVsPolygon(circleCenter, radius, headRectPoly);

    if (hit && !m_debounce)
    {
        m_debounce = true;
        StartTimer(&timer, lifeTime);
        StartTimer(&freezeTimer, freezeLifeTime);
        m_height += 1;
        ball.takeDamage(m_damage);
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

void Spear::initTextures()
{
    (*m_stickTexture) = LoadTexture("./resources/stick.png");
    (*m_spearHeadTexture) = LoadTexture("./resources/spearHead.png");
}
