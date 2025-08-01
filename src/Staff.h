//
// Created by Harshad on 7/29/2025.
//



#include "Weapon.h"
#include "RaylibTimer.h"
#include "Item.h"
#include "Ball.h"
#include "Constants.h"
#include "Box.h"

class Weapon;
class Item;
class Ball;
class Box;
class Staff : public Item, public Weapon {
private:
    float m_width{ 70 };
    float m_height{ 70 };
    float m_fireBallWidth{ 30 };
    float m_fireBallHeight{ 30 };
    float m_maxRadius{ 80 };
    Vector2 m_pos{ 0, 0 };
    Vector2 m_fireBallPos{ 0, 0 };
    bool m_fireBallShot{ false };
    Timer m_shootTimer{ 0 };
    Vector2 m_fireBallVelocity{};
    float m_fireBallSpeed{ 600.0f };
    float m_angle{ 0.0f };
    float m_orbitSpeed{ 7.0f };
    float m_normalOrbitSpeed{ 7.0f };
    int m_damage{ 1 };
    float m_radiusOffset{ 40 };
    float m_staffTimerLifeTime{ 2.0f };
    Timer m_fireBallInterval{ 0 };
    float m_fireBallIntervalLifeTime{ 0.1f };
    int m_currentNumOfArrows{ 0 };
    float m_growthSpeed{ 200.0f };

    int m_numOfArrows{ 1 };
    int m_addition{ 0 };

    struct m_fireBall
    {
        Vector2 pos;
        Vector2 velocity;
        bool active = true;
        float angleDeg;
    };

    std::vector<m_fireBall> m_fireBallVector = {};

    Rectangle m_staffRect;
    Rectangle m_fireBallRect;
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
    std::unique_ptr<Texture2D> m_staffTexture = std::make_unique<Texture2D>();
    std::unique_ptr<Texture2D> m_fireBallTexture = std::make_unique<Texture2D>();

    Box m_borderBox;

    struct m_Explosion {
        Vector2 pos;
        float radius = 0.0f;
        bool active = true;
    };

    std::vector<m_Explosion> m_explosions;

    const float m_normalStateExplosionSpeed = {m_growthSpeed};
    const float m_normalStateMaxRadius = {m_maxRadius};
    const int m_normalStateDamage = {m_damage};
    std::string m_name{ "Staff" };
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




