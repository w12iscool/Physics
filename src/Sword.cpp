//
// Created by Harshad on 7/23/2025.
//

#include "Sword.h"

Rectangle& Sword::getRect()
{
    return m_swordRect;
}

float& Sword::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Sword::getAngle()
{
    return m_angle;
}

float& Sword::getDrawAngle()
{
    return m_drawAngle;
}

float& Sword::getWidth()
{
    return m_width;
}

float& Sword::getHeight()
{
    return m_height;
}

float& Sword::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Sword::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Sword::getDirection()
{
    return m_direction;
}

void Sword::render()
{
    Rectangle source = Rectangle(0, 0, (float)(*m_swordTexture).width, (float)(*m_swordTexture).height);
    Rectangle dest = Rectangle(m_swordRect.x, m_swordRect.y, m_width, m_height);

    DrawTexturePro((*m_swordTexture), source, dest, Vector2(m_swordRect.width / 2, m_swordRect.height / 2), m_drawAngle, WHITE);
}

void Sword::initTextures()
{
    (*m_swordTexture) = LoadTexture("./resources/sword.png");
}

void Sword::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed, bool& otherIsFrozen, bool& gameFrozen)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);


    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_swordRect.x, m_swordRect.y};
    Vector2 secondItemCenter = {otherRect.x, otherRect.y};

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_swordRect.width, m_swordRect.height, physicalAngleRad);
    std::vector<Vector2> otherItemPoly = getRotatedRect(secondItemCenter, otherRect.width, otherRect.height, physicalAngleRad);
    Vector2 circleCenter = Vector2(b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30);
    float radius = ball.getRadius();

    bool hit = satCircleVsPolygon(circleCenter, radius, itemPoly);
    bool itemsHit = satCollisions(itemPoly, otherItemPoly);

    if (itemsHit)
    {
        if (!ball.getFrozen() && !ball2.getFrozen())
        {
            m_direction *= -1;
        }
    }

    if (hit && !m_debounce)
    {
        m_debounce = true;
        StartTimer(&timer, lifeTime);
        StartTimer(&freezeTimer, freezeLifeTime);
        ball.takeDamage(m_damage);
        m_damage += 1;
    }

    if (TimerDone(&timer))
    {
        m_debounce = false;
    }

    if (TimerDone(&freezeTimer))
    {
        gameFrozen = false;
    }
    else
    {
        gameFrozen = true;
    }
}

void Sword::freezeSword()
{
    m_orbitSpeed = 0.0f;
}

bool& Sword::getFrozen()
{
    return m_isFrozen;
}
