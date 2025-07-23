//
// Created by Harshad on 7/18/2025.
//
#pragma once

#include "Item.h"
#include <numbers>
#include "Ball.h"
#include <iostream>
#include <raylib.h>
#include "RaylibTimer.h"
#include "box2d/box2d.h"

#include <memory>

class Item;
class Ball;
class Spear : public Item {
private:
    float m_width{ 25 };
    float m_height{ 75 };
    float m_headWidth{ 20 };
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_headAngle{ 0.0 };
    float m_orbitSpeed{ 0.0f };
    float m_normalOrbitSpeed{ 5.0f };
    int m_damage{ 1 };

    Rectangle m_itemRect;
    Rectangle m_headRect;
    float m_drawAngle;

    struct Projection
    {
        float min;
        float max;
    };

    float m_drawRadius;
    float m_headDrawAngle;
    bool m_debounce{ false };

    float m_direction { 1.0f };
    bool m_isFrozen = false;
    float m_radiusOffset{ 40 };

    std::unique_ptr<Texture2D> m_stickTexture = std::make_unique<Texture2D>();
    std::unique_ptr<Texture2D> m_spearHeadTexture = std::make_unique<Texture2D>();

public:
    Rectangle& returnItemRect();
    Rectangle& returnHeadRect();
    float& getOrbitSpeed();
    float& getNormalOrbitSpeed();
    float& getWidth();
    float& getHeight();
    float& getDrawAngle();
    float& getAngle();
    float& getDirection();
    float& getRadiusOffset();
    float& getHeadWidth();
    float& getHeadAngle();
    float& getHeadDrawAngle();

    void render() override;
    void handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float& otherNormalOrbitSpeed);
    void initTextures();
    void rotateHead();
};

