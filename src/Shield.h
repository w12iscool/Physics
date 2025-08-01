//
// Created by Harshad on 7/26/2025.
//
#pragma once

#include "Weapon.h"
#include "Item.h"
#include "Ball.h"


class Ball;
class Item;
class Weapon;


class Shield : public Item, public Weapon {
private:

    float m_width{ 70 };
    float m_height{ 30 };
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 6.0f };
    float m_normalOrbitSpeed{ 6.0f };
    int m_damage{ 1 };
    float m_radiusOffset{ 10 };

    Rectangle m_shieldRect;
    float m_drawAngle;

    struct Projection
    {
        float min;
        float max;
    };

    float m_drawRadius;
    bool m_debounce;

    bool m_isFrozen = false;

    bool m_collisionDb{ false };

    float m_direction = -1;
    std::unique_ptr<Texture2D> m_shieldTexture = std::make_unique<Texture2D>();

    const float m_normalStateWidth{ m_width };
    std::string m_name{ "Shield" };
public:

    Rectangle& getRect() override;
    float& getOrbitSpeed() override;
    float& getAngle() override;
    float& getDrawAngle() override;
    float& getWidth() override;
    float& getHeight() override;
    float& getRadiusOffset() override;
    float& getNormalOrbitSpeed() override;
    float& getDirection() override;

    void render() override;
    void initTextures() override;
    void handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed, bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle, bool& parrybool) override;
    void freezeSword();

    bool& getFrozen() override;
    bool& getCollDb() override;
    void rotate(Ball& target, float& orbitSpeed) override;
    void setDirection(int direction) override;
    void resetState() override;
    std::string getName() override;
};




