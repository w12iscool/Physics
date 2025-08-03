//
// Created by Harshad on 7/26/2025.
//

#include "Bow.h"

#include <numbers>

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

    DrawTexturePro((*m_bowTexture), source, dest, Vector2(m_bowRect.width / 2, m_bowRect.height / 2), m_drawAngle, WHITE);

    for (const auto& arrow : arrowsVector)
    {
        if (!arrow.active) continue;

        Rectangle arrowSrc = { 0, 0, (float)(*m_arrowTexture).width, (float)(*m_arrowTexture).height };
        Rectangle arrowDest = { arrow.pos.x, arrow.pos.y, m_arrowWidth, m_arrowHeight };

        float arrowAngle = atan2f(arrow.velocity.y, arrow.velocity.x) * RAD2DEG;

        DrawTexturePro(*m_arrowTexture, arrowSrc, arrowDest,
            Vector2{ m_arrowWidth / 2, m_arrowHeight / 2 }, arrow.angleDeg, WHITE);
    }
}

void Bow::initTextures()
{
    (*m_bowTexture) = LoadTexture("./resources/bow.png");
    (*m_arrowTexture) = LoadTexture("./resources/arrow.png");
}

void Bow::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);
    UpdateTimer(&m_shootTimer);
    UpdateTimer(&m_arrowInterval);

    lifeTime = m_bowTimerLifeTime;

    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = { m_bowRect.x, m_bowRect.y };
    Vector2 arrowCenter = { m_arrowRect.x, m_arrowRect.y };
    Vector2 secondItemCenter = { otherRect.x, otherRect.y };

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_bowRect.width, m_bowRect.height, physicalAngleRad);
    std::vector<Vector2> arrowPoly = getRotatedRect(arrowCenter, m_arrowRect.width, m_arrowRect.height, physicalAngleRad);
    std::vector<Vector2> otherItemPoly = getRotatedRect(secondItemCenter, otherRect.width, otherRect.height, physicalAngleRad);
    Vector2 circleCenter = { b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30 };
    float radius = ball.getRadius();

    bool hit = satCircleVsPolygon(circleCenter, radius, itemPoly);
    bool itemsHit = satCollisions(itemPoly, otherItemPoly) || satCollisions(otherItemPoly, arrowPoly);

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

    // Freeze logic
    if (TimerDone(&freezeTimer))
    {
        ball.setColor(ball.getDefaultColor());
        gameFrozen = false;
    }
    else
    {
        gameFrozen = true;
    }

    if (TimerDone(&m_shootTimer) && !m_burstActive)
    {
        StartTimer(&m_shootTimer, m_bowTimerLifeTime);
        StartTimer(&m_arrowInterval, m_arrowIntervalLifeTime);

        m_burstActive = true;
    }

    if (m_burstActive && TimerDone(&m_arrowInterval))
    {
        float angleRad = m_angle;
        Vector2 dir = { cosf(angleRad), sinf(angleRad) };

        Vector2 start = {
            m_bowRect.x + dir.x * m_width * 0.5f,
            m_bowRect.y + dir.y * m_height * 0.5f
        };

        m_Arrow arrow;
        arrow.pos = start;
        arrow.velocity = { dir.x * m_arrowSpeed, dir.y * m_arrowSpeed };
        arrow.angleDeg = m_drawAngle;
        arrowsVector.push_back(arrow);

        m_currentNumOfArrows++;

        if (m_currentNumOfArrows == m_numOfArrows)
        {
            m_burstActive = false;
            m_currentNumOfArrows = 0;
        }
        else
        {
            StartTimer(&m_arrowInterval, m_arrowIntervalLifeTime);
        }
    }

    float delta = GetFrameTime();

    for (auto& arrow : arrowsVector)
    {
        if (!arrow.active) continue;

        arrow.pos.x += arrow.velocity.x * delta;
        arrow.pos.y += arrow.velocity.y * delta;

        if (arrow.pos.x < 0 || arrow.pos.x > GetScreenWidth() || arrow.pos.y < 0 || arrow.pos.y > GetScreenHeight())
        {
            arrow.active = false;
        }
    }

    arrowsVector.erase(std::remove_if(arrowsVector.begin(), arrowsVector.end(),
            [](const m_Arrow& a) { return !a.active; }), arrowsVector.end());

    for (auto& arrow : arrowsVector)
    {
        if (!arrow.active) continue;

        float dist = Vector2Distance(arrow.pos, circleCenter);
        if (dist < radius + m_arrowWidth * 0.5f)
        {
            arrow.active = false;
            ball.setColor(RAYWHITE);
            ball.takeDamage(m_damage);
            m_numOfArrows++;
        }
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

void Bow::setDirection(int direction)
{
    m_direction = direction;
}

void Bow::resetState()
{
    m_numOfArrows = m_originalMaxNumOfArrows;
}

std::string Bow::getName()
{
    return m_name;
}
