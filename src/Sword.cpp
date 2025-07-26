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
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed, bool& otherIsFrozen, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle)
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
            if (!m_collisionDb)
            {
                m_direction *= -1;
                otherDirection *= -1;
                const float deflectionStrength = DEG2RAD * 12.0f;
                m_angle += m_direction * deflectionStrength;
                otherAngle += otherDirection * deflectionStrength;
                m_collisionDb = true;
                otherDb = true;
            }

        }
    }
    else
    {
        m_collisionDb = false;
    }

    if (hit && !m_debounce && !itemsHit)
    {
        m_debounce = true;
        StartTimer(&timer, lifeTime);
        StartTimer(&freezeTimer, freezeLifeTime);
        ball.takeDamage(m_damage);
        m_damage += 1;
        ball.setColor(RAYWHITE);
    }

    if (TimerDone(&timer))
    {
        m_debounce = false;
    }

    if (TimerDone(&freezeTimer))
    {
        ball.setWhoHitMe(hitItem::itemHit::None);
        ball.setColor(ball.getDefaultColor());
        gameFrozen = false;
    }
    else
    {
        ball.setWhoHitMe(hitItem::itemHit::Sword);
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

bool& Sword::getCollDb()
{
    return m_collisionDb;
}

void Sword::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_swordRect, orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);
}
