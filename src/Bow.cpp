//
// Created by Harshad on 7/26/2025.
//

#include "Bow.h"

Rectangle& Bow::getRect()
{
    return m_bowRect;
}

float& Bow::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Bow::getAngle()
{
    return m_angle;
}

float& Bow::getDrawAngle()
{
    return m_drawAngle;
}

float& Bow::getWidth()
{
    return m_width;
}

float& Bow::getHeight()
{
    return m_height;
}

float& Bow::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Bow::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Bow::getDirection()
{
    return m_direction;
}

void Bow::render()
{
    Rectangle source = Rectangle(0, 0, (float)(*m_bowTexture).width, (float)(*m_bowTexture).height);
    Rectangle dest = Rectangle(m_bowRect.x, m_bowRect.y, m_width, m_height);

    Rectangle sourceArrow = Rectangle(0, 0, (float)(*m_arrowTexture).width, (float)(*m_arrowTexture).height);
    Rectangle destArrow = Rectangle(m_arrowRect.x, m_arrowRect.y, m_arrowWidth, m_arrowHeight);

    DrawTexturePro((*m_bowTexture), source, dest, Vector2(m_bowRect.width / 2, m_bowRect.height / 2), m_drawAngle, WHITE);
    DrawTexturePro((*m_arrowTexture), sourceArrow, destArrow, Vector2(m_arrowRect.width / 2, m_arrowRect.height / 2), m_drawAngle, WHITE);
}

void Bow::initTextures()
{
    (*m_bowTexture) = LoadTexture("./resources/bow.png");
    (*m_arrowTexture) = LoadTexture("./resources/arrow.png");
}

void Bow::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle)
{

    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);


    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_bowRect.x, m_bowRect.y};
    Vector2 secondItemCenter = {otherRect.x, otherRect.y};

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_bowRect.width, m_bowRect.height, physicalAngleRad);
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

void Bow::freezeSword()
{
    m_orbitSpeed = 0;
}

bool& Bow::getFrozen()
{
    return m_isFrozen;
}

bool& Bow::getCollDb()
{
    return m_collisionDb;
}

void Bow::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_bowRect, m_orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);
}
