//
// Created by Harshad on 7/28/2025.
//

#pragma once

#include "Weapon.h"
#include "Item.h"
#include "Ball.h"
#include "RaylibTimer.h"

class Ball;
class Item;
class Weapon;

class Scythe : public Item, public Weapon {
private:
    float m_width{ 60 };
    float m_height{ 80 };
    Vector2 m_pos{ 0, 0 };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 10.0f };
    float m_normalOrbitSpeed{ 10.0f };
    int m_damage{ 1 };
    float m_radiusOffset{ 40 };
    int m_poisonAmt{ 1 };

    Rectangle m_scytheRect;
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

    std::unique_ptr<Texture2D> m_scytheTexture = std::make_unique<Texture2D>();

    Timer m_poisonTimer = { 0 };
    float m_poisonLifeTime{ 1.25f };

    const int m_normalStatePoison = {m_normalStatePoison};
    std::string m_name{ "Scythe" };

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

