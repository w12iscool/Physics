//
// Created by Harshad on 7/25/2025.
//

#include "Dagger.h"

Rectangle& Dagger::getRect()
{
    return m_daggerRect;
}

float& Dagger::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Dagger::getAngle()
{
    return m_angle;
}

float& Dagger::getDrawAngle()
{
    return m_drawAngle;
}

float& Dagger::getWidth()
{
    return m_width;
}

float& Dagger::getHeight()
{
    return m_height;
}

float& Dagger::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Dagger::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Dagger::getDirection()
{
    return m_direction;
}

void Dagger::render()
{
    Rectangle source = Rectangle(0, 0, (float)(*m_daggerTexture).width, (float)(*m_daggerTexture).height);
    Rectangle dest = Rectangle(m_daggerRect.x, m_daggerRect.y, m_width, m_height);

    DrawTexturePro((*m_daggerTexture), source, dest, Vector2(m_daggerRect.width / 2, m_daggerRect.height / 2), m_drawAngle, WHITE);
}

void Dagger::initTextures()
{
    (*m_daggerTexture) = LoadTexture("./resources/dagger.png");
}

void Dagger::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);


    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_daggerRect.x, m_daggerRect.y};
    Vector2 secondItemCenter = {otherRect.x, otherRect.y};

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_daggerRect.width, m_daggerRect.height, physicalAngleRad);
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
        m_normalOrbitSpeed += 10;

        ball.setColor(RAYWHITE);

        std::cout << m_direction << "\n";
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
}

void Dagger::freezeSword()
{
    m_orbitSpeed = 0.0f;
}

bool& Dagger::getFrozen()
{
    return m_isFrozen;
}

bool& Dagger::getCollDb()
{
    return m_collisionDb;
}

void Dagger::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_daggerRect, orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);
}

void Dagger::setDirection(int direction)
{
    m_direction = direction;
}

void Dagger::resetState()
{
    m_normalOrbitSpeed = m_normalOrbitSpeed;
}

std::string Dagger::getName()
{
    return m_name;
}
