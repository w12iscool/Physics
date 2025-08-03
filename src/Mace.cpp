//
// Created by Harshad on 7/28/2025.
//

#include "Mace.h"

#include <numbers>
Rectangle& Mace::getRect()
{
    return m_maceRect;
}

float& Mace::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Mace::getAngle()
{
    return m_angle;
}

float& Mace::getDrawAngle()
{
    return m_drawAngle;
}

float& Mace::getWidth()
{
    return m_width;
}

float& Mace::getHeight()
{
    return m_height;
}

float& Mace::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Mace::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Mace::getDirection()
{
    return m_direction;
}

void Mace::render()
{
    Rectangle source = Rectangle(0, 0, (float)(*m_maceTexture).width, (float)(*m_maceTexture).height);
    Rectangle dest = Rectangle(m_maceRect.x, m_maceRect.y, m_width, m_height);

    DrawTexturePro((*m_maceTexture), source, dest, Vector2(m_maceRect.width / 2, m_maceRect.height / 2), m_drawAngle, WHITE);
}

void Mace::initTextures()
{
    (*m_maceTexture) = LoadTexture("./resources/mace.png");
}

void Mace::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);
    UpdateTimer(&m_speedTimer);


    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_maceRect.x, m_maceRect.y};
    Vector2 secondItemCenter = {otherRect.x, otherRect.y};

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_maceRect.width, m_maceRect.height, physicalAngleRad);
    std::vector<Vector2> otherItemPoly = getRotatedRect(secondItemCenter, otherRect.width, otherRect.height, physicalAngleRad);
    Vector2 circleCenter = Vector2(b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30);
    float radius = ball.getRadius();

    bool hit = satCircleVsPolygon(circleCenter, radius, itemPoly);
    bool itemsHit = satCollisions(itemPoly, otherItemPoly);

    if (itemsHit)
    {
        if (!ball.getFrozen() && !ball2.getFrozen())
        {
            if (!m_collisionDb && !m_debounce)
            {
                m_debounce = true;
                m_direction *= -1;
                otherDirection *= -1;
                const float deflectionStrength = DEG2RAD * 12.0f;
                m_angle += m_direction * deflectionStrength;
                otherAngle += otherDirection * deflectionStrength;
                m_collisionDb = true;
                otherDb = true;

                StartTimer(&freezeTimer, freezeLifeTime);
                StartTimer(&timer, lifeTime);
                gameFrozen = true;
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
        ball.setColor(RAYWHITE);
    }

    if (TimerDone(&timer))
    {
        m_debounce = false;
    }

    if (TimerDone(&freezeTimer))
    {
        ball.setColor(ball.getDefaultColor());
        gameFrozen = false;
    }
    else
    {
        gameFrozen = true;
    }

    if (TimerDone(&m_speedTimer))
    {
        m_normalOrbitSpeed += 2;
        m_damage += 1;
        StartTimer(&m_speedTimer, m_speedLifeTime);
    }
}

void Mace::freezeSword()
{
    m_orbitSpeed = 0;
}

bool& Mace::getFrozen()
{
    return m_isFrozen;
}

bool& Mace::getCollDb()
{
    return m_collisionDb;
}

void Mace::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_maceRect, m_orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);
}

void Mace::setDirection(int direction)
{
    m_direction = direction;
}

void Mace::resetState()
{
    m_normalOrbitSpeed = m_normalStateOrbitSpeed;
    m_damage = m_normalStateDamage;
}

std::string Mace::getName()
{
    return m_name;
}
