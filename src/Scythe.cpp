#include "Scythe.h"

Rectangle& Scythe::getRect()
{
    return m_scytheRect;
}

float& Scythe::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Scythe::getAngle()
{
    return m_angle;
}

float& Scythe::getDrawAngle()
{
    return m_drawAngle;
}

float& Scythe::getWidth()
{
    return m_width;
}

float& Scythe::getHeight()
{
    return m_height;
}

float& Scythe::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Scythe::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Scythe::getDirection()
{
    return m_direction;
}

void Scythe::render()
{

    Rectangle source = Rectangle(0, 0, (float)(*m_scytheTexture).width, (float)(*m_scytheTexture).height);
    Rectangle dest = Rectangle(m_scytheRect.x, m_scytheRect.y, m_width, m_height);

    DrawTexturePro((*m_scytheTexture), source, dest, Vector2(m_scytheRect.width / 2, m_scytheRect.height / 2), m_drawAngle, WHITE);}

void Scythe::initTextures()
{
    (*m_scytheTexture) = LoadTexture("./resources/scythe.png");
}

void Scythe::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);
    UpdateTimer(&m_poisonTimer);

    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_scytheRect.x, m_scytheRect.y};
    Vector2 secondItemCenter = {otherRect.x, otherRect.y};

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_scytheRect.width, m_scytheRect.height, physicalAngleRad);
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
                StartTimer(&freezeTimer, freezeLifeTime);
                m_direction *= -1;
                otherDirection *= -1;
                const float deflectionStrength = DEG2RAD * 8.0f;
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
        StartTimer(&m_poisonTimer, m_poisonLifeTime);
        ball.takeDamage(m_damage);
        m_poisonAmt += 1;
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


    if (TimerDone(&m_poisonTimer))
    {
        ball.setColor(PURPLE);
        ball.takeDamage(m_poisonAmt);
        StartTimer(&m_poisonTimer, m_poisonLifeTime);
    }

}

void Scythe::freezeSword()
{
    m_orbitSpeed = 0;
}

bool& Scythe::getFrozen()
{
    return m_isFrozen;
}

bool& Scythe::getCollDb()
{
    return m_collisionDb;
}

void Scythe::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_scytheRect, orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);
}

void Scythe::setDirection(int direction)
{
    m_direction = direction;
}

void Scythe::resetState()
{
    m_poisonAmt = m_normalStatePoison;
}
