//
// Created by Harshad on 8/2/2025.
//

#include "Wrench.h"

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

    DrawTexturePro((*m_wrenchTexture), source, dest, Vector2(m_wrenchRect.width / 2, m_wrenchRect.height / 2), m_drawAngle, WHITE);}

void Wrench::initTextures()
{
    (*m_wrenchTexture) = LoadTexture("./resources/wrench.png");
}

void Wrench::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
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
}

void Wrench::setDirection(int direction)
{
    m_direction = direction;
}

void Wrench::resetState()
{
    m_numOfTurrets = m_normalStateTurrets;
}

std::string Wrench::getName()
{
    return m_name;
}

void Wrench::wrenchSpecificFunction()
{
    // std::cout << "\r" << m_wrenchRect.x << std::flush;
}



