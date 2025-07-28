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
class Bow : public Item, public Weapon {
private:


    float m_width{ 150 };
    float m_height{ 100 };
    float m_arrowWidth{ 50 };
    float m_arrowHeight{ 70 };
    Vector2 m_pos{ 0, 0 };
    Vector2 m_arrowPos{ 0, 0 };
    bool m_arrowShot{ false };
    Timer m_shootTimer{ 0 };
    Vector2 m_arrowVelocity{};
    float m_arrowSpeed{ 600.0f };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 6.0f };
    float m_normalOrbitSpeed{ 6.0f };
    int m_damage{ 1 };
    float m_radiusOffset{ 10 };
    float m_bowTimerLifeTime{ 1.0f };
    Timer m_arrowInterval{ 0 };
    float m_arrowIntervalLifeTime{ 0.1f };
    bool m_burstActive = false;
    int m_currentNumOfArrows{ 0 };

    int m_numOfArrows{ 1 };
    int m_addition{ 0 };

    struct m_Arrow
    {
        Vector2 pos;
        Vector2 velocity;
        bool active = true;
        float angleDeg;
    };

    std::vector<m_Arrow> arrowsVector{};
    Rectangle m_bowRect;
    Rectangle m_arrowRect;
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
    std::unique_ptr<Texture2D> m_bowTexture = std::make_unique<Texture2D>();
    std::unique_ptr<Texture2D> m_arrowTexture = std::make_unique<Texture2D>();
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
    void handleCollision(Ball& ball, Timer& timer, float& lifeTime, Ball& ball2, Timer& freezeTimer, float& freezeLifeTime, float& otherOrbitSpeed, Rectangle& otherRect, float otherNormalOrbitSpeed, bool& otherFrozenBool, bool& gameFrozen, float& otherDirection, bool& otherDb, float& otherAngle) override;
    void freezeSword();

    bool& getFrozen() override;
    bool& getCollDb() override;
    void rotate(Ball& target, float& orbitSpeed) override;
};




