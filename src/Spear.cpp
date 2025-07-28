//
// Created by Harshad on 7/18/2025.
//

#include "Spear.h"

#include <vector>

void Spear::initTextures()
{
    (*m_stickTexture) = LoadTexture("./resources/stick.png");
    (*m_spearHeadTexture) = LoadTexture("./resources/spearHead.png");
}

Rectangle& Spear::getRect()
{
    return m_itemRect;
}

Rectangle& Spear::returnHeadRect()
{
    return m_headRect;
}

float& Spear::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Spear::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Spear::getWidth()
{
    return m_width;
}

float& Spear::getHeight()
{
    return m_height;
}

float& Spear::getDrawAngle()
{
    return m_drawAngle;
}

float& Spear::getAngle()
{
    return m_angle;
}

float& Spear::getDirection()
{
    return m_direction;
}

float& Spear::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Spear::getHeadWidth()
{
    return m_headWidth;
}

float& Spear::getHeadAngle()
{
    return m_headAngle;
}

float& Spear::getHeadDrawAngle()
{
    return m_headDrawAngle;
}

void Spear::rotateHead()
{
    m_headRect = {m_itemRect.x, m_itemRect.y, m_width, 20};
}

bool& Spear::getFrozen()
{
    return m_isFrozen;
}

bool& Spear::getCollDb()
{
    return m_collisionDb;
}

void Spear::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_itemRect, orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);
}

void Spear::render()
{
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

void Spear::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed, bool& otherIsFrozen, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);


    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_itemRect.x, m_itemRect.y};
    Vector2 secondItemCenter = {otherRect.x, otherRect.y};

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_itemRect.width, m_itemRect.height, physicalAngleRad);
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
                const float deflectionStrength = DEG2RAD * 8.0f;
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
        m_height += 30;
        m_radiusOffset += 15;
        ball.setColor(RAYWHITE);
    }

    if (TimerDone(&timer))
    {
        m_debounce = false;
    }

    if (TimerDone(&freezeTimer))
    {
        ball.setColor(ball.getDefaultColor());
        ball.setWhoHitMe(hitItem::itemHit::None);
        gameFrozen = false;
    }
    else
    {
        ball.setWhoHitMe(hitItem::itemHit::Spear);
        gameFrozen = true;
    }
}


