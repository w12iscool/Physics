//
// Created by Harshad on 7/23/2025.
//
#pragma once

#include <raylib.h>
#include "Item.h"
#include "Ball.h"
#include <iostream>
#include "RaylibTimer.h"
#include "box2d/box2d.h"

#include <memory>

class Item;
class Ball;

class Sword : public Item {
private:
    float m_width{ 50 };
    float m_height{ 120 };
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 6.0f };
    float m_normalOrbitSpeed{ 6.0f };
    int m_damage{ 1 };
    float m_radiusOffset{ 55 };

    Rectangle m_swordRect;
    float m_drawAngle;

    struct Projection
    {
        float min;
        float max;
    };

    float m_drawRadius;
    bool m_debounce;

    bool m_isFrozen = false;


    float m_direction = 1;
    std::unique_ptr<Texture2D> m_swordTexture = std::make_unique<Texture2D>();
public:
    Rectangle& getRect();
    float& getOrbitSpeed();
    float& getAngle();
    float& getDrawAngle();
    float& getWidth();
    float& getHeight() override;
    float& getRadiusOffset();
    float& getNormalOrbitSpeed();
    float& getDirection();

    void render() override;
    void initTextures();
    void handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed, bool& otherFrozenBool, bool& gameFrozen);
    void freezeSword();

    bool& getFrozen();
};




