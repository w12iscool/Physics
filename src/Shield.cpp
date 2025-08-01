//
// Created by Harshad on 7/26/2025.
//

#include "Shield.h"

Rectangle& Shield::getRect()
{
    return m_shieldRect;
}

float& Shield::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Shield::getAngle()
{
    return m_angle;
}

float& Shield::getDrawAngle()
{
    return m_drawAngle;
}

float& Shield::getWidth()
{
    return m_width;
}

float& Shield::getHeight()
{
    return m_height;
}

float& Shield::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Shield::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Shield::getDirection()
{
    return m_direction;
}

void Shield::render()
{
    Rectangle source = Rectangle(0, 0, (float)(*m_shieldTexture).width, (float)(*m_shieldTexture).height);
    Rectangle dest = Rectangle(m_shieldRect.x, m_shieldRect.y, m_width, m_height);

    DrawTexturePro((*m_shieldTexture), source, dest, Vector2(m_shieldRect.width / 2, m_shieldRect.height / 2), m_drawAngle, WHITE);}

void Shield::initTextures()
{
    (*m_shieldTexture) = LoadTexture("./resources/shield.png");
}

void Shield::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);


    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_shieldRect.x, m_shieldRect.y};
    Vector2 secondItemCenter = {otherRect.x, otherRect.y};

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_shieldRect.width, m_shieldRect.height, physicalAngleRad);
    std::vector<Vector2> otherItemPoly = getRotatedRect(secondItemCenter, otherRect.width, otherRect.height, physicalAngleRad);
    Vector2 circleCenter = Vector2(b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30);
    float radius = ball.getRadius();

    bool hit = satCircleVsPolygon(circleCenter, radius, itemPoly);
    bool itemsHit = satCollisions(itemPoly, otherItemPoly);



    if (itemsHit && !m_debounce)
    {
        m_debounce = true;
        StartTimer(&timer, lifeTime);
        StartTimer(&freezeTimer, freezeLifeTime);
        ball.takeDamage(m_damage);
        m_width += 1;
        ball.setColor(RAYWHITE);

        m_direction *= -1;
        otherDirection *= -1;
        const float deflectionStrength = DEG2RAD * 12.0f;
        m_angle += m_direction * deflectionStrength;
        otherAngle += otherDirection * deflectionStrength;
        m_collisionDb = true;
        otherDb = true;
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

void Shield::freezeSword()
{
    m_orbitSpeed = 0.0f;
}

bool& Shield::getFrozen()
{
    return m_collisionDb;
}

bool& Shield::getCollDb()
{
    return m_collisionDb;
}

void Shield::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_shieldRect, orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);
}

void Shield::setDirection(int direction)
{
    m_direction = direction;
}

void Shield::resetState()
{
    m_width = m_normalStateWidth;
}

std::string Shield::getName()
{
    return m_name;
}
