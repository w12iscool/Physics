//
// Created by Harshad on 7/17/2025.
//
#pragma once

#include "raylib.h"
#include "box2d/box2d.h"
#include "Constants.h"
#include "Item.h"
#include "Random.h"
#include "RaylibTimer.h"
#include <string>

class Item;
class Ball {
private:
    float m_radius{ 30 };
    Vector2 m_pos = Vector2((SCREEN_WIDTH - m_radius) / 2, (SCREEN_HEIGHT - m_radius) / 2);
    Color m_color{ RED };
    bool m_isFrozen{ false };
    bool m_freezeDebounce{ false };
    int m_health{ 100 };

private:
    b2BodyId m_ballId = { 0 };
    b2Vec2 previousVelocity;
public:
    int getRadius() const;
    Vector2 getPos();

    void initBallBox2d(b2WorldId& worldId);
    void renderCircle();
    b2BodyId getBallId();
    void testStopTime();
    void setPos(Vector2 newPos);
    void keepMoving();
    void setFrozen(bool frozen);
    void setColor(Color newColor);
    void takeDamage(int damage);
    bool getFrozen();

    void handleFreezing(float& orbitSpeed, float normalOrbitSpeed, Ball& ball, bool& gameFrozen);
};




