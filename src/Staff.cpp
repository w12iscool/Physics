
#include "Staff.h"

Rectangle& Staff::getRect()
{
    return m_staffRect;
}

float& Staff::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Staff::getAngle()
{
    return m_angle;
}

float& Staff::getDrawAngle()
{
    return m_drawAngle;
}

float& Staff::getWidth()
{
    return m_width;
}

float& Staff::getHeight()
{
    return m_height;
}

float& Staff::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Staff::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Staff::getDirection()
{
    return m_direction;
}

void Staff::render()
{
    Rectangle source = Rectangle(0, 0, (float)(*m_staffTexture).width, (float)(*m_staffTexture).height);
    Rectangle dest = Rectangle(m_staffRect.x, m_staffRect.y, m_width, m_height);

    DrawTexturePro((*m_staffTexture), source, dest, Vector2(m_staffRect.width / 2, m_staffRect.height / 2), m_drawAngle, WHITE);
    for (const auto& arrow : m_fireBallVector)
    {
        if (!arrow.active) continue;

        Rectangle arrowSrc = { 0, 0, (float)(*m_fireBallTexture).width, (float)(*m_fireBallTexture).height };
        Rectangle arrowDest = { arrow.pos.x, arrow.pos.y, m_fireBallWidth, m_fireBallHeight };

        float arrowAngle = atan2f(arrow.velocity.y, arrow.velocity.x) * RAD2DEG;

        DrawTexturePro(*m_fireBallTexture, arrowSrc, arrowDest,
            Vector2{ m_fireBallWidth / 2, m_fireBallHeight / 2 }, arrow.angleDeg, WHITE);
    }

    for (const auto& e : m_explosions)
    {
        if (!e.active) continue;

        DrawCircleV(e.pos, e.radius, Fade(ORANGE, 0.6f));
        int lineWeight{ 5 };
        for (int i{ -1 }; i <= lineWeight; ++i)
        {
            DrawCircleLines(e.pos.x, e.pos.y, e.radius + i, Fade(RED, 0.6f));
        }
    }
}

void Staff::initTextures()
{
    (*m_staffTexture) = LoadTexture("./resources/staff.png");
    (*m_fireBallTexture) = LoadTexture("./resources/fireball.png");
}

void Staff::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
    UpdateTimer(&freezeTimer);
    UpdateTimer(&timer);
    UpdateTimer(&m_shootTimer);

    float physicalAngleRad = m_angle + (PI / 2.0f);
    Vector2 itemCenter = { m_staffRect.x, m_staffRect.y };
    Vector2 arrowCenter = { m_fireBallRect.x, m_fireBallRect.y };
    Vector2 secondItemCenter = { otherRect.x, otherRect.y };

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_staffRect.width, m_staffRect.height, physicalAngleRad);
    std::vector<Vector2> arrowPoly = getRotatedRect(arrowCenter, m_fireBallRect.width, m_fireBallRect.height, physicalAngleRad);
    std::vector<Vector2> otherItemPoly = getRotatedRect(secondItemCenter, otherRect.width, otherRect.height, physicalAngleRad);
    Vector2 circleCenter = { b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30 };
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

    if (TimerDone(&m_shootTimer))
    {
        float angleRad = m_angle;
        Vector2 dir = {cosf(angleRad), sinf(angleRad)};

        Vector2 start = {
            m_staffRect.x + dir.x * m_width * 0.5f,
            m_staffRect.y + dir.y * m_width * 0.5f,
        };

        m_fireBall fireball;
        fireball.pos = start;
        fireball.velocity = { dir.x * m_fireBallSpeed, dir.y * m_fireBallSpeed };
        fireball.angleDeg = m_drawAngle;
        m_fireBallVector.push_back(fireball);

        StartTimer(&m_shootTimer, m_staffTimerLifeTime);
    }

    float delta = GetFrameTime();
    for (auto& fi : m_fireBallVector)
    {
        if (!fi.active) continue;

        fi.pos.x += fi.velocity.x * delta;
        fi.pos.y +=  fi.velocity.y * delta;

        Rectangle boxRect = m_borderBox.getRect();

        Vector2 topCenter = {
            boxRect.x + boxRect.width / 2.0f,
            boxRect.y
        };

        // Bottom center
        Vector2 bottomCenter = {
            boxRect.x + boxRect.width / 2.0f,
            boxRect.y + boxRect.height
        };

        // Left center
        Vector2 leftCenter = {
            boxRect.x,
            boxRect.y + boxRect.height / 2.0f
        };

        // Right center
        Vector2 rightCenter = {
            boxRect.x + boxRect.width,
            boxRect.y + boxRect.height / 2.0f
        };

        if (fi.pos.x < boxRect.x ||
    fi.pos.x > boxRect.x + boxRect.width ||
    fi.pos.y < boxRect.y ||
    fi.pos.y > boxRect.y + boxRect.height)
        {
            m_Explosion e;
            e.pos = fi.pos;
            e.radius = 0.0f;
            e.active = true;
            m_explosions.push_back(e);

            fi.active = false;
        }
    }

    // for (auto& e : m_explosions)
    // {
    //     if (!e.active) continue;
    //
    //     e.radius += e.growSpeed * delta;
    //     if (e.radius >= m_maxRadius)
    //     {
    //         e.active = false;
    //     }
    // }

    m_fireBallVector.erase(std::remove_if(m_fireBallVector.begin(), m_fireBallVector.end(),
        [](const m_fireBall& a) { return !a.active; }), m_fireBallVector.end());

    for (auto& fi : m_fireBallVector)
    {
        if (!fi.active) continue;

        float dist = Vector2Distance(fi.pos, circleCenter);
        if (dist < radius + m_fireBallWidth * 0.5f)
        {
            fi.active = false;
            ball.setColor(RAYWHITE);
            ball.takeDamage(m_damage);

            m_Explosion e;
            e.pos = fi.pos;
            e.radius = 0.0f;
            e.active = true;

            m_damage += 1;
            m_maxRadius += 30.0f;
            m_growthSpeed += 150.0f;
            m_explosions.push_back(e);

            fi.active = false;
        }
    }

    for (auto& e : m_explosions)
    {
        if (!e.active) continue;

        e.radius += m_growthSpeed * delta;
        if (e.radius >= m_maxRadius)
        {
            e.active = false;
            continue;
        }

        float dist = Vector2Distance(e.pos, circleCenter);
        if (dist < radius + e.radius)
        {
            ball.setColor(RAYWHITE);
            ball.takeDamage(m_damage);

            m_damage += 1;
            m_maxRadius += 30.0f;
            m_growthSpeed += 150.0f;
            e.active = false;
        }
    }
}

void Staff::freezeSword()
{
    m_orbitSpeed = 0.0f;
}

bool& Staff::getFrozen()
{
    return m_isFrozen;
}

bool& Staff::getCollDb()
{
    return m_collisionDb;
}

void Staff::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_staffRect, orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);
}

void Staff::setDirection(int direction)
{
    m_direction = direction;
}

void Staff::resetState()
{
    m_maxRadius = m_normalStateMaxRadius;
    m_growthSpeed = m_normalStateExplosionSpeed;
    m_damage = m_normalStateDamage;
}

std::string Staff::getName()
{
    return m_name;
}
