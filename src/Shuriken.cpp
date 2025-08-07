//
// Created by Harshad on 8/4/2025.
//

#include "Shuriken.h"

Rectangle& Shuriken::getRect()
{
    return m_shurikenRect;
}

float& Shuriken::getOrbitSpeed()
{
    return m_orbitSpeed;
}

float& Shuriken::getAngle()
{
    return m_angle;
}

float& Shuriken::getDrawAngle()
{
    return m_drawAngle;
}

float& Shuriken::getWidth()
{
    return m_width;
}

float& Shuriken::getHeight()
{
    return m_height;
}

float& Shuriken::getRadiusOffset()
{
    return m_radiusOffset;
}

float& Shuriken::getNormalOrbitSpeed()
{
    return m_normalOrbitSpeed;
}

float& Shuriken::getDirection()
{
    return m_direction;
}

void Shuriken::render()
{
    Item::render();
}

void Shuriken::initTextures()
{

}

void Shuriken::handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer,
    float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed,
    bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool)
{
}

void Shuriken::freezeSword()
{
}

bool& Shuriken::getFrozen()
{
}

bool& Shuriken::getCollDb()
{
}

void Shuriken::rotate(Ball& target, float& orbitSpeed)
{
}

void Shuriken::setDirection(int direction)
{
}

void Shuriken::resetState()
{
}

std::string Shuriken::getName()
{
}
