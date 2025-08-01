//
// Created by Harshad on 7/28/2025.
//
#pragma once

#include <csetjmp>

#include "Weapon.h"
#include "Item.h"
#include "Ball.h"
#include "RaylibTimer.h"

class Mace : public Item, public Weapon {
private:

    float m_width{ 45 };
    float m_height{ 90 };
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 3.0f };
    float m_normalOrbitSpeed{ 3.0f };
    int m_damage{ 1 };
    float m_radiusOffset{ 50 };

    Rectangle m_maceRect;
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

    b2Vec2 m_knockBack{ 1, 1 };

    Timer m_speedTimer = { 0.0f };
    float m_speedLifeTime{ 3.0f };

    std::unique_ptr<Texture2D> m_maceTexture = std::make_unique<Texture2D>();

    Timer m_parryTimer{ 0 };
    float m_parryLifeTime{ 0.3f };

    bool m_parryBool = false;

    const float m_normalStateOrbitSpeed{ m_normalOrbitSpeed };
    const int m_normalStateDamage{ m_damage };
    std::string m_name{ "Mace" };
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
