//
// Created by Harshad on 8/2/2025.
//

#include "Wrench.h"

#include "../third_party/box2d/src/body.h"

#include <numbers>

Rectangle& Wrench::getRect()
{
    return m_wrenchRect;
}

float& Wrench::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Wrench::getAngle()
{
    return m_angle;
}

float& Wrench::getDrawAngle()
{
    return m_drawAngle;
}

float& Wrench::getWidth()
{
    return m_width;
}

float& Wrench::getHeight()
{
    return m_height;
}

float& Wrench::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Wrench::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Wrench::getDirection()
{
    return m_direction;
}

void Wrench::render()
{
    Rectangle source = Rectangle(0, 0, (float)(*m_wrenchTexture).width, (float)(*m_wrenchTexture).height);
    Rectangle dest = Rectangle(m_wrenchRect.x, m_wrenchRect.y, m_width, m_height);

    DrawTexturePro((*m_wrenchTexture), source, dest, Vector2(m_wrenchRect.width / 2, m_wrenchRect.height / 2), m_drawAngle, WHITE);

    for (auto& t : m_turretsVector)
    {
        DrawCircle(t.m_turretPos.x, t.m_turretPos.y, 20, SKYBLUE);
        DrawRectanglePro(t.m_turrectGun, Vector2(t.m_turrectGun.width / 2, m_wrenchRect.height / 2), t.m_drawAngle, SKYBLUE);
    }

    for (auto& pellet : m_pelletsVector)
    {
        if (!pellet.active) continue;

        DrawCircle(pellet.pos.x, pellet.pos.y, m_pelletRadius, DARKGRAY);
    }
}


void Wrench::initTextures()
{
    (*m_wrenchTexture) = LoadTexture("./resources/wrench.png");
}

void Wrench::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
    UpdateTimer(&timer);
    UpdateTimer(&freezeTimer);
    UpdateTimer(&m_parryTimer);
    for (auto& t : m_turretsVector)
    {
        UpdateTimer(&t.m_shootTimer);
    }


    float physicalAngleRad = m_angle + (std::numbers::pi / 2.0f);
    Vector2 itemCenter = {m_wrenchRect.x, m_wrenchRect.y};
    Vector2 secondItemCenter = {otherRect.x, otherRect.y};

    std::vector<Vector2> itemPoly = getRotatedRect(itemCenter, m_wrenchRect.width, m_wrenchRect.height, physicalAngleRad);
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


        // Vector2 newTurretPos = Vector2((b2Body_GetPosition(ball.getBallId()).x * 30), (b2Body_GetPosition(ball.getBallId()).y * 30) + 30);
        Vector2 newTurretPos = Vector2(m_wrenchRect.x, m_wrenchRect.y);
        Color newTurretColor;
        b2BodyId newTurretId;
        float newTurretDir{ 1 };
        float newTurretRadius{ 20 };
        float newTurretAngle{ 0.0f };
        float newTurretWidth { 15 };
        float newTurretHeight{ 25 };
        Rectangle newTurretGun = {1000, 1000, newTurretWidth, newTurretHeight};
        Timer newTurretTimer;

        b2BodyDef ballDef = b2DefaultBodyDef();
        ballDef.position = b2Vec2(newTurretPos.x / 30, newTurretPos.y / 30);
        ballDef.type = b2_staticBody;
        newTurretId = b2CreateBody(b2Body_GetWorld(ball.getBallId()), &ballDef);

        b2Circle circle;
        circle.center = b2Vec2(0, 0);
        circle.radius = newTurretRadius / 30;
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 0.2;
        b2ShapeId circleShapeId = b2CreateCircleShape(newTurretId, &shapeDef, &circle);

        turret newTurret
        {
            newTurretGun,
            newTurretPos,
            newTurretColor,
            newTurretId,
            newTurretDir,
            newTurretRadius,
            newTurretAngle,
            0.0f,
            newTurretWidth,
            newTurretHeight,
            newTurretTimer
        };

        m_turretsVector.push_back(newTurret);
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

    for (auto& t : m_turretsVector)
    {
        if (TimerDone(&t.m_shootTimer))
        {
            float angleRad = t.m_angle;
            Vector2 dir {cosf(angleRad), sinf(angleRad)};

            Vector2 start {
                t.m_turrectGun.x + dir.x * t.m_width * 0.5f,
                t.m_turrectGun.y + dir.y * t.m_height * 0.5f
            };

            m_pellet pellet;
            pellet.pos = start;
            pellet.velocity = { dir.x * m_pelletSpeed, dir.y * m_pelletSpeed };
            pellet.angleDeg = t.m_drawAngle;
            m_pelletsVector.push_back(pellet);
            StartTimer(&t.m_shootTimer, m_turretShootLifeTime);
        }
    }

    for (auto& p : m_pelletsVector)
    {
        if (!p.active) continue;

        float dist = Vector2Distance(p.pos, circleCenter);
        if (dist < radius + m_pelletRadius)
        {
            ball.setColor(RAYWHITE);
            ball.takeDamage(m_damage);

            p.active = false;
        }
    }

    float deltaTime = GetFrameTime();
    for (auto& p : m_pelletsVector)
    {
        if (!p.active) continue;
        p.pos.x += p.velocity.x * deltaTime;
        p.pos.y += p.velocity.y * deltaTime;
    }

}

void Wrench::freezeSword()
{
    m_orbitSpeed = 0.0f;
}

bool& Wrench::getFrozen()
{
    return m_isFrozen;
}

bool& Wrench::getCollDb()
{
    return m_collisionDb;
}

void Wrench::rotate(Ball& target, float& orbitSpeed)
{
    Item::rotate(target, m_wrenchRect, orbitSpeed, m_angle, m_drawAngle, m_width, m_height, m_radiusOffset, m_direction, m_isFrozen);

    for (auto& t : m_turretsVector)
    {
        float tau = 2 * (std::numbers::pi);
        float increment = (tau / 360.0f) * m_orbitSpeed;
        Vector2 origin = Vector2(b2Body_GetPosition(t.m_bodyId).x * 30, b2Body_GetPosition(t.m_bodyId).y * 30);
        float radius = t.m_radius + m_turretRadiusOffset;

        t.m_angle += increment;
        if (t.m_angle > tau)
            t.m_angle -= tau;

        t.m_turrectGun = {origin.x + radius * cos(t.m_angle), origin.y + radius * sin(t.m_angle), t.m_width, t.m_height};
        t.m_drawAngle = RAD2DEG * (t.m_angle + std::numbers::pi / 2.0f);
    }
}

void Wrench::setDirection(int direction)
{
    m_direction = direction;
}

void Wrench::resetState()
{
    for (auto& t : m_turretsVector)
    {
        b2DestroyBody(t.m_bodyId);
    }

    m_turretsVector.clear();
}

std::string Wrench::getName()
{
    return m_name;
}

void Wrench::wrenchSpecificFunction()
{

}



