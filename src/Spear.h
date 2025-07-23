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
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 5.0f };
    float m_normalOrbitSpeed{ 5.0f };
    int m_damage{ 5 };

    Rectangle m_itemRect;
    Rectangle m_headRect;
    float m_drawAngle;

    struct Projection
    {
        float min;
        float max;
    };

    float m_drawRadius;
    bool m_debounce{ false };

    std::unique_ptr<Texture2D> m_stickTexture = std::make_unique<Texture2D>();
    std::unique_ptr<Texture2D> m_spearHeadTexture = std::make_unique<Texture2D>();

public:
    void rotate(Ball& ball);
    void render() override;
    void handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime);
    void initTextures();
};

