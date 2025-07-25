//
// Created by Harshad on 7/17/2025.
//

#include "Ball.h"

#include <iostream>

int Ball::getRadius() const
{
    return m_radius;
}

Vector2 Ball::getPos()
{
    return m_pos;
}

void Ball::initBallBox2d(b2WorldId& worldId)
{
    float pixelsToMeters{30};
    b2BodyDef ballDef = b2DefaultBodyDef();
    ballDef.position = b2Vec2(m_pos.x / 30, m_pos.y / 30);
    ballDef.type = b2_dynamicBody;
    m_ballId = b2CreateBody(worldId, &ballDef);

    b2Circle circle;
    // circle.center = b2Vec2(m_pos.x / pixelsToMeters, m_pos.y / pixelsToMeters);
    circle.center = b2Vec2(0, 0);
    circle.radius = m_radius / pixelsToMeters;
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 0.2;
    shapeDef.material.restitution = 1.0f;
    shapeDef.material.friction = 0.1f;
    b2ShapeId circleShapeId = b2CreateCircleShape(m_ballId, &shapeDef, &circle);
}

void Ball::renderCircle()
{
    float pixelsToMeters{30};
    DrawCircle(b2Body_GetPosition(m_ballId).x * pixelsToMeters, b2Body_GetPosition(m_ballId).y * pixelsToMeters,
               m_radius, m_color);

    // Black outline (maybe not very efficient)
    DrawCircleLines(b2Body_GetPosition(m_ballId).x * pixelsToMeters, b2Body_GetPosition(m_ballId).y * pixelsToMeters, m_radius - 1, BLACK);
    DrawCircleLines(b2Body_GetPosition(m_ballId).x * pixelsToMeters, b2Body_GetPosition(m_ballId).y * pixelsToMeters, m_radius, BLACK);
    DrawCircleLines(b2Body_GetPosition(m_ballId).x * pixelsToMeters, b2Body_GetPosition(m_ballId).y * pixelsToMeters, m_radius + 1, BLACK);

    std::string temp = std::to_string(m_health);
    const char* healthText = temp.c_str();
    int fontSize = 30;
    int textWidth = MeasureText(healthText, fontSize);

    DrawText(
        healthText,
        b2Body_GetPosition(m_ballId).x * pixelsToMeters - textWidth / 2,
        b2Body_GetPosition(m_ballId).y * pixelsToMeters - fontSize / 2,
        fontSize,
        BLACK
    );
}

b2BodyId Ball::getBallId()
{
    return m_ballId;
}

void Ball::testStopTime()
{


    float epsilon = 0.0001f;

    b2Vec2 velocity = b2Body_GetLinearVelocity(m_ballId);

    float speedSquared = b2Dot(velocity, velocity);

   if (speedSquared > (epsilon * epsilon))
   {
       previousVelocity = velocity;
   }


    if (IsKeyDown(KEY_W))
    {
        m_isFrozen = true;
        b2Body_SetLinearVelocity(m_ballId, b2Vec2(0.0f, 0.0f ));
    }
    if (IsKeyDown(KEY_B))
    {
        m_isFrozen = false;
    }

    if (!m_isFrozen)
    {
        if (IsKeyDown(KEY_S))
        {
            b2Body_SetLinearVelocity(m_ballId, b2Vec2(Random::get(-50, 50), Random::get(-50, 50) ));
        }
    }

}

void Ball::setPos(Vector2 newPos)
{
    m_pos = newPos;
}

void Ball::keepMoving()
{
    b2Vec2 velocity = b2Body_GetLinearVelocity(m_ballId);

    if (!m_isFrozen)
    {
        if (velocity.x == 0 || velocity.y == 0)
        {
            b2Body_SetLinearVelocity(m_ballId, b2Vec2(Random::get(-50, 50), Random::get(-50, 50)));
        }
    }
}


void Ball::setFrozen(bool frozen)
{
    m_isFrozen = frozen;
}

void Ball::setColor(Color newColor)
{
    m_color = newColor;
}

void Ball::takeDamage(int damage)
{
    m_health -= damage;
}

bool Ball::getFrozen()
{
    return m_isFrozen;
}

void Ball::handleFreezing(float& orbitSpeed, float normalOrbitSpeed, Ball& otherBall, bool& gameFrozen)
{
    if (m_isFrozen)
    {
        m_color = RAYWHITE;
        orbitSpeed = 0;
        b2Body_SetLinearVelocity(m_ballId, b2Vec2(0.0f, 0.0f ));
        b2Body_SetGravityScale(m_ballId, 0.0f);
    }
    else
    {
        m_color = RED;
        orbitSpeed = normalOrbitSpeed;
        b2Body_SetGravityScale(m_ballId, 1.0f);
    }
}
